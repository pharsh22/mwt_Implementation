int get_start_position(std::vector<int> vertex_type, Arrangement_2::Ccb_halfedge_const_circulator &start_node)
{
	std::vector<int>::iterator i1 = vertex_type.begin();
	Arrangement_2::Ccb_halfedge_const_circulator circ = start_node;	// keep the original circulator secure
	int start_position;

	// Normally, each sequence has at least one refles and at least one convex vertex and hence, we don't need to worry about edge cases where every vertex is either a convex or a reflex vertex.

	if(*i1 == 1)
	{
		start_position = vertex_type.size()-1;
		++start_node;	// going back
	} // we're moving in the counter clockwise direction so when we go from 1->2, we're decrementing the circulator. True for eveything else

	else
	{
		start_position = 0;
		while(*i1 != 1 && i1 != vertex_type.end())
		{
			++start_position;
			++i1;
			--start_node;
		}
		// once a convex vertex is found, pick the vertex before that one as backward suppoer vertex
		--start_position;
		++start_node;
	
	// We're bound to find at least one vertex which is convex. If all the vertices were reflex, we'd have return from the previous function itself, coz we can't triangulate the sequence without a convex vertex

	}

	return start_position;	
}