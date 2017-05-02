void triangulate_face(Arrangement_2 &arr, int level, std::vector<Point_2> nodes, Arrangement_2::Face_const_handle f)
{
	/*
	================================================================
	Input: A handle to a face and the arrangement
	Output: No output
	Function: Try and modify the passed face to triangulate
	================================================================
	*/


	// If the face is unbounded, we need to get a handle for the holes and triangulate all the faces.
	if(f->is_unbounded())
	{
		Arrangement_2::Hole_const_iterator hi;
		int index = 1;
		for(hi = f->holes_begin(); hi != f->holes_end(); ++hi, ++index)
		{
			triangulate_connected_component(arr, level, nodes, *hi);
		}
	}

	else
	{
		if(is_face_triangulated(f))
		{
			return;
		}
		
		triangulate_connected_component(arr, level, nodes, f->outer_ccb());

		Arrangement_2::Hole_const_iterator hi;
		int index = 1;
		for(hi = f->holes_begin(); hi != f->holes_end(); ++hi, ++index)
		{
			triangulate_connected_component(arr, level, nodes, *hi);
		}
	}
}


void traingulate_current_level(Arrangement_2 &arr, int level, std::vector<Point_2> nodes, std::vector<Vertex_handle> all_vertex_handles)
{
	/*
	============================================================
	Input: Current Arrangement and level
	Output: Nothing
	Function: Modify all the faces in current level by triangulating them as per the algorithm
	============================================================
	*/

	// Triangulate ALL THE FACES in the arrangement (Bounded and unbounded)
	Arrangement_2::Face_const_iterator fit;
	int index=1;
	for(fit = arr.faces_begin(); fit != arr.faces_end(); ++fit)
	{
		triangulate_face(arr, level, nodes, fit);
	}
}