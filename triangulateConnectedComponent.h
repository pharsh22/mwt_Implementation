void triangulate_connected_component(Arrangement_2 &arr, int level, std::vector<Point_2> nodes, Arrangement_2::Ccb_halfedge_const_circulator circ)
{
	// Level passed to this function is the current level Ai. We're making ^Ai right now and next we'll add remaining edges in Si+1 that do not intersect with anything from the arrangement.

	// In here make a vector of "vertex_type" and store values 0-reflex/1-convex
	std::vector<int> vertex_type;		// 0 - reflex, 		1 - convex
	std::vector<int> vertex_visited;	// 0 - not visited, 1 - visited

	int i, k=0, x;
	int start_position, count, number_of_vertices;
	int vl_minus_1_index=0, vl_index=0;
	int vq_plus_1_index=0, vq_index=0;
	int vl_plus_1_index, rcs_index, rce_index;
	int number_of_edges_added;
	int chain_start_index, chain_end_index;

	Arrangement_2::Ccb_halfedge_const_circulator start_node, before_start_node, after_start_node;	
	Arrangement_2::Ccb_halfedge_const_circulator curr_edge, next_edge;
	Arrangement_2::Ccb_halfedge_const_circulator chain_start, chain_end, next_to_chain_end;
	Arrangement_2::Ccb_halfedge_const_circulator vl_minus_1_node, vl_node, vl_plus_1_node;
	Arrangement_2::Ccb_halfedge_const_circulator vq_node, vq_plus_1_node;
	Arrangement_2::Ccb_halfedge_const_circulator reflex_chain_start, reflex_chain_end;
	Arrangement_2::Ccb_halfedge_const_circulator add_end_node, add_start_node;
	Arrangement_2::Ccb_halfedge_const_circulator c1, c2;
	Arrangement_2::Ccb_halfedge_const_circulator checkWithinFaceNode;

	bool intersection_test, chain_end_is_vl, visibility_test;

	curr_edge = circ;
	next_edge = --curr_edge; // going in counterclockwise direction
	++curr_edge;

	// std::cout << "Counter clockwise ordering of the hole:\n";
	// If there is no convex vertex in the sequence, the face can't be triangulated.	
	bool does_have_convex_vertex = false;	

	// Keep track of how many vertices are there in the hole, if there are only 2 vertices or less, do nothing
	number_of_vertices = 0;
	do
	{
		vertex_visited.push_back(0);

		// std::cout << "(" << curr_edge->target()->point() << ")-->";
		// std::cout << "(" << curr_edge->source()->point() << ")-->";
		// std::cout << "(" << next_edge->source()->point() << ") : ";

		x = orientation(curr_edge->target()->point(), curr_edge->source()->point(), next_edge->source()->point());
		if(x == 1)
		{
			vertex_type.push_back(1);
			does_have_convex_vertex = true;
			// std::cout << "Convex\n";
		}
		else
		{
			vertex_type.push_back(0);
			// std::cout << "Reflex\n";
		}
		--next_edge;	// counterclockwise direction
		number_of_vertices++;
	}while(--curr_edge != circ);

	if(does_have_convex_vertex == false)
	{
		// std::cout << "Only reflex vertices. Can't do anything.\n";
		return;	// there's no convex vertex, the face can be triangulated form the inside only.
	}

	if(number_of_vertices <= 2)
	{
		// std::cout << "Only " << number_of_vertices << " vertices. Nothing to be done.\n";
		return; // there's nothing to triangulate
	}

	// if(number_of_vertices == 3)
	// {
	// 	std::cout << "Already triangulated.\n";
	// 	return;
	// }


	// Find the starting vertex: backward support vertex (the vertex before a convex vertex)
	start_node = circ;
	start_position = get_start_position(vertex_type, start_node);

	do
	{
		chain_end_is_vl = false;		
		count = 0;
		vertex_visited[start_position] = 1;	// mark the starting vertex as visited
		++k;
		++count;

		// Starting from start_position, keep moving till you find a convex vertex
		vl_minus_1_index = (start_position+1)%vertex_type.size();
		vl_minus_1_node = --start_node;
		++start_node;

		while(vertex_type[vl_minus_1_index] != 1)
		{
			vl_minus_1_index = (vl_minus_1_index+1)%vertex_type.size();
			--vl_minus_1_node;
		}

		vl_index = (vl_minus_1_index+1)%vertex_type.size();
		vl_node = --vl_minus_1_node;
		++vl_minus_1_node;

		vl_plus_1_node = --vl_node;
		++vl_node;
		vl_plus_1_index = (vl_index+1)%vertex_type.size();


		// start_node is visible to vl_node
		bool vjk_vl_intersects, neighbor_condition;

		checkWithinFaceNode = start_node;
		--checkWithinFaceNode;

		visibility_test = is_visible(start_node->source()->point(), checkWithinFaceNode->source()->point(), vl_node->source()->point(), arr);

		neighbor_condition = check_neighbors(start_node->source()->point(), vl_minus_1_node->source()->point(), level+1);

		// Check if vl_minus_1_node and start_node are neighbors
		if(visibility_test)
		{
			if(neighbor_condition)
			{
				chain_start = start_node;
				chain_end = vl_node;
				chain_end_is_vl = true;				
				chain_start_index = start_position;
				chain_end_index = vl_index;
			}
			else
			{
				chain_start = start_node;
				chain_end = start_node;
				chain_start_index = start_position;
				chain_end_index = start_position;
			}

			next_to_chain_end = --chain_end;
			++chain_end;

			// std::cout << "Chain start = " << chain_start->source()->point() << "\n";
			// std::cout << "Chain end = " << chain_end->source()->point() << "\n";

			number_of_edges_added = 0;

			if(chain_start != chain_end)	// #of vertices in the chain > 1
			{
				if(vertex_type[vl_plus_1_index] == 0) // next to fsv is reflex
				{
					// Find the backward convex vertex of that vertex
					vq_plus_1_node = vl_node;
					vq_plus_1_index = vl_index;

					while(vertex_type[vq_plus_1_index] != 1)
					{
						vq_plus_1_index = (vq_plus_1_index-1+vertex_type.size())%vertex_type.size();
						++vq_plus_1_node;	// going back
					}

					vq_node = ++vq_plus_1_node;	// going back
					--vq_plus_1_node;


					// Check if vq_plus_1_index is neighbor of vl_plus_1_node and Vq is visible to vl_plus_1_node
					// This function is just to add more reflex vertices to the maximal reflex chain
					checkWithinFaceNode = vq_node;
					--checkWithinFaceNode;

					if(check_neighbors(vq_plus_1_node->source()->point(), vl_plus_1_node->source()->point(), level+1) &&
						is_visible(vq_node->source()->point(), checkWithinFaceNode->source()->point(), vl_plus_1_node->source()->point(), arr) )
					{
						reflex_chain_start = vl_plus_1_node;
						reflex_chain_end = vl_plus_1_node;
						rcs_index = vl_plus_1_index;
						rce_index = vl_plus_1_index;

						while(vertex_type[rce_index] != 1 &&
							  check_neighbors(reflex_chain_end->source()->point(), vq_plus_1_node->source()->point(), level+1 ))
						{
							--reflex_chain_end;		// counter clockwise traversal
							rce_index = (rce_index+1)%vertex_type.size();
						}

						// If the vertex at which we stopped is a convex vertex, go back by one.
						if(vertex_type[rce_index] == 1)
						{
							rce_index = (rce_index-1+number_of_vertices)%number_of_vertices;
							++reflex_chain_end;
						}

						for(c1=reflex_chain_start; c1 != reflex_chain_end; --c1)
						{
							checkWithinFaceNode = c1;
							--checkWithinFaceNode;

							if(is_visible(vq_node->source()->point(), checkWithinFaceNode->source()->point(), c1->source()->point(), arr))
							{
								reflex_chain_end = ++c1;
								rce_index = (rce_index-1+number_of_vertices)%number_of_vertices;
								break;	// if they are not visible, end the sequence there
							}
							// rce_index = (rce_index+1)%number_of_vertices;
						}

						chain_start = start_node;
						chain_end = reflex_chain_end;
						chain_start_index = start_position;
						chain_end_index = rce_index;
						next_to_chain_end = --chain_end;
						++chain_end;
						chain_end_is_vl = false;						
					}
				}

				// If Vj+k != Vl-2, then add edges from Vj+k to all vertices till Vl-2.
				++vl_node;
				++vl_node;
				add_end_node = vl_node;	// Vl-2
				--vl_node;
				--vl_node;

				Arrangement_2::Halfedge_handle h1, h2;
				h2 = arr.non_const_handle(start_node);
				h1 = arr.non_const_handle(vl_node);

				before_start_node = ++start_node;
				--start_node;

				after_start_node = --start_node;
				++start_node;

				checkWithinFaceNode = start_node;
				--checkWithinFaceNode;

				visibility_test = is_visible(start_node->source()->point(), checkWithinFaceNode->source()->point(), vl_node->source()->point(), arr);

				if(visibility_test)
				{
					arr.insert_at_vertices(Segment_2(vl_node->source()->point() , start_node->source()->point()) , h1->source(), h2->source());
					++number_of_edges_added;
					++k;
					++count;
				}

				vl_node = ++vl_plus_1_node;
				--vl_plus_1_node;


				// chain_end = ++next_to_chain_end;

				if(chain_end_is_vl == true)
					chain_end = vl_node;

				start_node = --before_start_node;
				++before_start_node;

				if(start_node != add_end_node)
				{
					c1 = ++after_start_node;

					// Add an edge from vl(vl_node) to every vertex in Vj+k to Vl-2
					do
					{
						--c1;
						h2 = arr.non_const_handle(c1);

						checkWithinFaceNode = c1;
						--checkWithinFaceNode;

						visibility_test = is_visible(c1->source()->point(), checkWithinFaceNode->source()->point(), vl_node->source()->point(), arr);

						if(visibility_test)
						{
							arr.insert_at_vertices(Segment_2(vl_node->source()->point() , c1->source()->point()) , h1->source(), h2->source());
							++number_of_edges_added;
							++k;
							++count;
						}
					}while(c1 != add_end_node);
					start_node = --before_start_node;
					++before_start_node;
				}

				// std::cout << "Start node = " << start_node->source()->point() << "\n";
				// std::cout << "vl_plus_1_node = " << vl_plus_1_node->source()->point() << "\n";
				// std::cout << "chain_end = " << chain_end->source()->point() << "\n";
				// std::cout << "vl_node = " << vl_node->source()->point() << "\n";

				// Add an edge from Vj+k to every vertex in Vl+1 to Vs
				if( chain_end != start_node && chain_end != vl_node)
				{
					h1 = arr.non_const_handle(start_node);
					c1 = vl_plus_1_node;	//Vl, and we'll start adding edges from the next vertex
					c2 = --vl_plus_1_node;
					++vl_plus_1_node;
					++c1;	// Vl
					++c2;

					// std::cout << "Vl_node = " << vl_node->source()->point() << "\n";
					// std::cout << "Vl+1 = " << vl_plus_1_node->source()->point() << "\n";
					// std::cout << "Chain end = " << chain_end->source()->point() << "\n";


					/*
					%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
					Now that we have used Vl to add edges in the arrangement, if you refer to Vl, using one of the circulator, it refers to the last added bounded region and hence we're no longer referring to the boundary of the hole associated with the unbounded face.

					Instead, use Vl+1 for the reference. Then move back by one to refer to Vl and you're still on the boundary of the hole that corresponds to the unbounded face. And this way you're referring to Vl now but with the correct circulator.
					%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
					*/

					do
					{
						c1 = c2;
						--c2;
						h2 = arr.non_const_handle(c1);

						checkWithinFaceNode = start_node;
						--checkWithinFaceNode;

						visibility_test = is_visible(start_node->source()->point(), checkWithinFaceNode->source()->point(), c1->source()->point(), arr);

						if(visibility_test)
						{
							arr.insert_at_vertices(Segment_2(start_node->source()->point() , c1->source()->point()) , h1->source(), h2->source());
							++number_of_edges_added;
							++k;
							++count;
						}
					}while(c1 != chain_end && c1 != start_node);
				}

				// std::cout << "Number of edges added = " << number_of_edges_added << "\n";

			}
			// else if(vertex_visited[vl_minus_1_index] == 1)	// Vl-1 is visited, then triangulate the face that is just created by this visited vertex
			// 	// Kind of complicated and I don't know how to deal with it
			// {
			// 	// As per the implementation of the algorithm, this condition shouldn't be checked, because, as long as there are possible triangulations in the bounded face, it'll go on to triangulating the region further.
			// 	std::cout << "Uh Oh, it's going to Stop!\n";

			// }
		}

		else
		{
			// if Vl is not visible to Vjk, mark Vjk as visited, it's hopeless, you can't connect it to any other vertices, move on
			chain_end = --start_node;

		}

		start_position = (start_position+count)%number_of_vertices;
		start_node = ++next_to_chain_end;

	}while(k < number_of_vertices);

}
