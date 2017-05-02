/*
=========================================
The next three functions are used to check whether given two edges intersect or not
=========================================
*/

bool onSegment(Point_2 p, Point_2 q, Point_2 r)
{
	/*
	======================================
	Input: 3 vertex points
	Output: if 3 points are colinear

	Given three colinear points p,q,r, the function checks if point q lies on line segment 'pr'
	======================================
	*/

	// Vertical and horizontal segments do not follow the last condition and hence we have to add two more conditions for vertical and horizontal segments

	if(p.x() == r.x())	// vertical segments, and because they are colinear (this condition is checked only when points are colinear), q.x() has to be the same
	{
		if(q.y() < std::max(p.y(), r.y()) && q.y() > std::min(p.y(), r.y()))
			return true;
		else
			return false;
	}

	if(p.y() == r.y())	// horizontal segments
	{
		if(q.x() < std::max(p.x(), r.x()) && q.x() > std::min(p.x(), r.x()))
			return true;
		else
			return false;
	}

	if(q.x() < std::max(p.x(), r.x() ) && q.x() > std::min(p.x(), r.x() ) &&
	   q.y() < std::max(p.y(), r.y() ) && q.y() > std::min(p.y(), r.y() ) )
		return true;

	return false;
}

int orientation(Point_2 p, Point_2 q, Point_2 r)
{
	/*
	======================================
	To find the orientation of ordered triplet(p,q,r).
	The funciton returns following values
	0 --> p,q,r are colinear
	1 --> Clockwise
	2 --> counter-clockwise
	======================================
	*/

	int val;
	val = (q.y() - p.y())*(r.x() - q.x()) -
		  (q.x() - p.x())*(r.y() - q.y());

	if(val == 0)
		return 0;

	return (val > 0)? 1:2;
}

bool checkIntersection(Segment_2 test1, Segment_2 test2)
{
	/*
	======================================
	Given two line segments, check if they intersect or not
	false = doesn't intersect
	true = intersects
	======================================
	*/

	Point_2 p1, q1, p2, q2;

	p1 = test1.source();
	q1 = test1.target();

	p2 = test2.source();
	q2 = test2.target();

	// std::cout << "p1 = " << p1 << ", q1 = " << q1 << "\n";
	// std::cout << "p2 = " << p2 << ", q2 = " << q2 << "\n";

	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// std::cout << "o1 = " << o1 << "\n";
	// std::cout << "o2 = " << o2 << "\n";
	// std::cout << "o3 = " << o3 << "\n";
	// std::cout << "o4 = " << o4 << "\n";

	if(p1 == p2 && q1 == q2)
		return true;

	if(p1 == q2 && q1 == p2)
		return true;

	if(o1 != 0 && o2 != 0 && o3 != 0 && o4 != 0 && o1 != o2 && o3 != o4)
		return true;

	if(o1 == 0 && onSegment(p1, p2, q1))
		return true;

	if(o2 == 0 && onSegment(p1, q2, q1))
		return true;

	if(o3 == 0 && onSegment(p2, p1, q2))
		return true;

	if(o4 == 0 && onSegment(p2, q1, q2))
		return true;

	return false;
}

bool does_intersect(Segment_2 test_segment, const Arrangement_2 &arr)
{
	// Print all the edges in the arrangement
	// Print the arrangement edges.
	Arrangement_2::Edge_const_iterator eit;

	bool checkResult=false;
	for(eit = arr.edges_begin(); eit != arr.edges_end(); ++eit)
	{
		checkResult = checkIntersection(test_segment, eit->curve());
		if(checkResult)
			return true;
	}

	return false;
}