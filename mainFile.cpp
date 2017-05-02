#include <CGAL/Cartesian.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <fstream>
#include <vector>
#include <CGAL/intersections.h>
#include <cmath>

typedef double								Number_type;
typedef CGAL::Cartesian<Number_type>		Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel>	Traits_2;
typedef Traits_2::Point_2					Point_2;
typedef Traits_2::X_monotone_curve_2		Segment_2;
typedef CGAL::Arrangement_2<Traits_2>		Arrangement_2;
typedef Arrangement_2::Vertex_handle		Vertex_handle;
typedef Arrangement_2::Halfedge_handle		Halfedge_handle;
typedef Arrangement_2::Face_handle			Face_handle;

#include "arr_print.h"
#include "findDelta.h"
#include "findNeighbors.h"
#include "checkIntersection.h"
#include "addSi_plus_1_sorted.h"
#include "isFaceTriangulated.h"
#include "getStartPosition.h"
#include "triangulateConnectedComponent.h"
#include "triangulateCurrentLevel.h"

int main()
{
	// Open the input file and read the nodes and store into a vector
	std::ifstream my_input;
	my_input.open("input.txt");
	double x,y;
	std::vector<Point_2> nodes;

	while(my_input >> x >> y)
	{
		nodes.push_back(Point_2(x,y));
	}

	my_input.close();

	// Refine the input and make all the coordinates 0.5. This can be done while finding delta itself and hence done
	double maxDist;
	int delta, i, j, level=0, number_of_levels, temp_int;
	bool intersect_result, triangulated_result;

	maxDist = findDelta(nodes);
	delta = power3delta(maxDist);

	temp_int = 3*delta;
	number_of_levels = 1;
	while(temp_int > 1)
	{
		number_of_levels++;
		temp_int /= 3;
	} // if number_of_Levels=4, there are 5 levels in there essentially, it includes level 0 as well.

	// Make an arrangement with isolated vertices and probabily keep all the vertex_handles handy in a vector
	Arrangement_2 arr;
	std::vector<Vertex_handle> all_vertex_handles;

/*
############################################################################
The whole algorithm is based on the assumption that the input has unique vertices. If there are any vertices which are repeated, the result might be undesired.
############################################################################
*/

	// at level 0, all the vertices are added to the unbounded face and hence, it is treated separately
	add_non_intersecting_edges(arr, 0, nodes, all_vertex_handles);

	for(i=1; i<number_of_levels; i++)
	{
		add_non_intersecting_edges(arr, i, nodes, all_vertex_handles);
		traingulate_current_level(arr, i, nodes, all_vertex_handles);
	}

	print_arrangement(arr);

	return 0;
}













