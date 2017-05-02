bool compare_segments(const std::pair< std::pair<Segment_2, int>, std::pair<int, int> > &a, const std::pair< std::pair<Segment_2, int>, std::pair<int, int> > &b)
{
	return a.first.second < b.first.second;
}

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
		std::pair <int, int> ij;
		std::pair <Segment_2, int> segment_pair;
		std::vector< std::pair< std::pair<Segment_2, int>, std::pair<int, int> > > current_level_segments;
		Arrangement_2::Vertex_const_iterator vit;

		findNeighbors(nodes, level, adjacentVerticesList);

		// Add edges between neighbors such that it doesn't intersect
		for(i=0; i<nodes.size(); ++i)
		{
			for(j=0; j<adjacentVerticesList[i].size(); j++)
			{
				segment_pair = std::make_pair(Segment_2(nodes[i], nodes[adjacentVerticesList[i][j]]), CGAL::squared_distance(nodes[i], nodes[adjacentVerticesList[i][j]]) );
				ij = std::make_pair(i,j);
				current_level_segments.push_back(std::make_pair(segment_pair, ij));
			}
		}

		// Sort this vector according to its length
		sort(current_level_segments.begin(), current_level_segments.end(), compare_segments);

		for(i=0; i<current_level_segments.size(); i++)
		{
			intersect_result = does_intersect(current_level_segments[i].first.first, arr);
			if(!intersect_result)
			{
				// std::cout << "Doesn't intersect.\n";
				arr.insert_at_vertices(current_level_segments[i].first.first, all_vertex_handles[current_level_segments[i].second.first], all_vertex_handles[adjacentVerticesList[current_level_segments[i].second.first][current_level_segments[i].second.second]]);
			}			
		}


	}

	return;	

}