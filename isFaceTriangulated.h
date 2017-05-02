bool is_face_triangulated(Arrangement_2::Face_const_handle f)
{
	// Input: A handle to a face
	// Output: True/False
	// 			True: Face is triangulated
	// 			False: Face is not triangulated
	if(f->is_unbounded())
		return false;
	else
	{
		// Get a halfEdge circulator to this face
		Arrangement_2::Ccb_halfedge_const_circulator circ = f->outer_ccb();

		// Find the number of edges in this face. If it's 3, its triangulated.
		int number_of_edges=0;
		Arrangement_2::Ccb_halfedge_const_circulator curr = circ;
		do
		{
			number_of_edges++;
		}while(++curr != circ);

		// std::cout << "Number of edges in this face: " << number_of_edges;

		if(number_of_edges == 3)
			return true;
		else
			return false;
	}
}