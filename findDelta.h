double findDelta(std::vector<Point_2> &nodes)
{
	/*
	=================================================
	Input: Points in 2D in the form of a vector of type Point_2 (double x, double y)
	Output: Delta = spread of the points = furthest pair of points/closest pair of points
	====
	As of now, I'm using an O(n^2) algorithm, but it can be modified at later stages if need be.
	Also, after finding the min distance, move all the points to (0.5,0.5) origin in this function itself.
	Find the ditance from current point to all other points. Find the max of these ditances.

	NOTE: Here, it is assumed that the minimum distance between the points is 1. In case where minimum distance between points is different, we can keep track of the same and then delta = max_dist/min_dist
	=================================================
	*/

	int sqrDiff, i, j, minx=INT_MAX, miny=INT_MAX; // minx and miny signifies the smallest coordinates in the dataset which can be used to move the data points at a later stage.
	double distance, maxDist;

	for(i=0; i<nodes.size(); i++)
	{
		if(nodes[i].x() < minx )
			minx = nodes[i].x();

		if(nodes[i].y() < miny )
			miny = nodes[i].y();

		for(j=i+1; j<nodes.size(); j++)
		{
			sqrDiff = CGAL::squared_distance(nodes[i], nodes[j]);
			distance = sqrt(sqrDiff);
			if(maxDist < distance)
			{
				// here, we can keep track of the closest pair as well. But in our case, we know the closest pair is 1
				maxDist = distance;
			}
		}
	}

	int signx = 1, signy = 1;

	// Once the min point is found, shift the axes by moving every point to left/right by min distance
	// if points have negative coordinates, they need to move to right else left.
	
	if(minx < 0)
		signx = -1;
	if(miny < 0)
		signy = -1;

	for(i=0; i<nodes.size(); i++)
	{
		nodes[i] = Point_2(nodes[i].x() - signx*minx + 0.5, nodes[i].y() - signy*miny + 0.5);
	}

	return maxDist;
}

int power3delta(double delta)
{
	/*
	=======================================
	Input: A real number
	Output: Next largest power of 3.

	Given an integer/real number, this function returns the next largest power of 3.
	=======================================
	*/
	
	int x = 3;
	// While comparing int and double, int is type promoted to double, so this is safe.
	while(x < delta)
		x *= 3;
	return x;
}