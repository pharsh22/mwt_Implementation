void print_ccb(Arrangement_2::Ccb_halfedge_const_circulator circ)
{
	Arrangement_2::Ccb_halfedge_const_circulator curr = circ;
	do
	{
		Arrangement_2::Halfedge_const_handle he = curr;
		std::cout << "(" << he->source()->point() << ") ";	
		std::cout << "[" << he->curve() << "] -- ";
	}while(--curr != circ);
	// These changes are made to follow the definition given in the paper.
	// While walking on the boundary of the hole, we want the face to appear on the right side as we walk from one vertex to another. Hence we want to travel in clockwise order.
	// While walking on the boundary of a bounded face, we want the face to appear on the right as well. So we walk in counterclockwise direction.
	std::cout << std::endl;
}

void print_face(Arrangement_2::Face_const_handle f)
{
	// Print the outer boundary
	if(f->is_unbounded())
		std::cout << "Unbounded face.\n";
	else
	{
		std::cout << "Outer Boundary: ";
		print_ccb(f->outer_ccb());
	}

	// Print the boundary of each of the holes
	Arrangement_2::Hole_const_iterator hi;
	int index = 1;
	for(hi = f->holes_begin(); hi != f->holes_end(); ++hi, ++index)
	{
		std::cout << " Hole #" << index << ": ";
		print_ccb(*hi);
	}

	// Print the isolated vertices.
	Arrangement_2::Isolated_vertex_const_iterator iv;
	for(iv = f->isolated_vertices_begin(), index = 1;
		iv != f->isolated_vertices_end();
		++iv, ++index)
	{
		std::cout << "Isolated vertex #" << index << ":"
				  << "(" << iv->point() << ")\n";
	}
}


void print_arrangement(const Arrangement_2 &arr)
{
	// Print the arrangement vertices
	Arrangement_2::Vertex_const_iterator vit;
	std::cout << arr.number_of_vertices() << " vertices:\n";
	int index;
	for(vit = arr.vertices_begin(), index = 0; vit != arr.vertices_end(); ++vit, ++index)
	{
		std::cout << index << ". (" << vit->point() << ")";
		if(vit->is_isolated())
			std::cout << " - Isolated.\n";
		else
			std::cout << " - degree " << vit->degree() << "\n";
	}
	std::cout << "\n";

	// Print the arrangement edges.
	Arrangement_2::Edge_const_iterator eit;
	std::cout << arr.number_of_edges() << " edges:\n";
	std::ofstream my_output;
	double cost=0, square_distance;
	my_output.open("mwtOutput.txt");
	for(eit = arr.edges_begin(); eit != arr.edges_end(); ++eit)
	{
		std::cout << "[" << eit->curve() << "]\n";
		my_output << eit->curve() << "\n";
		square_distance = CGAL::squared_distance(eit->curve().source(), eit->curve().target());
		cost = cost+sqrt(square_distance);
		// std::cout << "Cost = " << sqrt(square_distance) << "\n";
	}
	std::cout << "Total cost of the implementation = " << cost << "\n";
	my_output.close();
	std::cout << "\n";

	// Print the arrangement faces.
	Arrangement_2::Face_const_iterator fit;
	std::cout << arr.number_of_faces() << " faces:\n";
	for(fit = arr.faces_begin(); fit != arr.faces_end(); ++fit)
		print_face (fit);
	std::cout << "\n";
}