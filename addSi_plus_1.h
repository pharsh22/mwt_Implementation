void add_non_intersecting_edges(Arrangement_2 &arr, int level, std::vector<Point_2> &nodes, std::vector<Vertex_handle> &all_vertex_handles)
{

	/*
	===============================================
	Input: Current Arrangement, level, Point_2 nodes and the handles to those nodes in the arrangement
	Output: Modified arrangement
	Function: Find the edges that belong to the level and add those which do not intersect with any other edges in the current arrangement
	===============================================
	*/

	int i,j;
	// if level = 0, you're adding edges in an unbounded face
	if(level == 0)
	{
		Face_handle uf = arr.unbounded_face();
		for(i=0; i<nodes.size(); i++)
		{
			all_vertex_handles.push_back(arr.insert_in_face_interior(nodes[i], uf));
		}
	}

	// If level is anything else, just add edges that do not intersect
	// Find neighboring nodes in the arrangment
	else
	{
		bool intersect_result;
		std::vector<std::vector<int> > adjacentVerticesList;
		Arrangement_2::Vertex_const_iterator vit;

		findNeighbors(nodes, level, adjacentVerticesList);

		// Add edges between neighbors such that it doesn't intersect
		for(i=0, vit = arr.vertices_begin(); i<nodes.size(); ++i, ++vit)
		{
			for(j=0; j<adjacentVerticesList[i].size(); j++)
			{
				// std::cout << "[" << nodes[i] << ", " << nodes[adjacentVerticesList[i][j]] << "]: ";
				intersect_result = does_intersect(Segment_2(nodes[i], nodes[adjacentVerticesList[i][j]]), arr);
				// If it doesn't intersect any existing edges, add it to the arrangement, otherwise do nothing
				if(!intersect_result)
				{
					// std::cout << "Doesn't intersect.\n";
					arr.insert_at_vertices(Segment_2(nodes[i], nodes[adjacentVerticesList[i][j]]), all_vertex_handles[i], all_vertex_handles[adjacentVerticesList[i][j]]);
				}
			}
		}
	}

	return;	

}