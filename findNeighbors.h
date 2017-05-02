void get_boundaries(Point_2 node, int level, double *upperBoundaryX, double *upperBoundaryY, double *lowerBoundaryX, double *lowerBoundaryY)
{
	int levelPower;
	double targetx = node.x();
	double targety = node.y();

	if(level == 0)
	{
		return;
		// technically, you don't need to find the limit for level 0, coz there are no neighboring points at level 0, coz the minimum distance between the points is 1.
		*upperBoundaryX = targetx + 0.5;
		*lowerBoundaryX = *upperBoundaryX - 1;

		*upperBoundaryY = targety + 0.5;
		*lowerBoundaryY = *upperBoundaryY - 1;
	}

	// if level is anything other than level 0, the boundary is defined by multiples of 3.
	// levelPower defines the length of the grid_cell at that level. This length is used to increase the boundary limit accordingly.
	levelPower = pow(3, level-1);
	// Lets assume, a point is (0.5, 0.5) then finding boundaries is essentially finding the box within which the current point could have neighbours. For this point, level 1 neighbors are in the range x(-1, 2) and y(-1,2), coz the length of the cell is 1. So one cell beyond the cell containing current point.

	*upperBoundaryX = 0;
	while(*upperBoundaryX < targetx)
		*upperBoundaryX += levelPower;

	*lowerBoundaryX = *upperBoundaryX - 2*levelPower;
	*upperBoundaryX = *upperBoundaryX + levelPower;

	*upperBoundaryY = 0;
	while(*upperBoundaryY < targety)
		*upperBoundaryY += levelPower;

	*lowerBoundaryY = *upperBoundaryY - 2*levelPower;
	*upperBoundaryY = *upperBoundaryY + levelPower;
}


// This function is to just check, given two points if they are neighbors for a given level
bool check_neighbors(Point_2 node1, Point_2 node2, int level)
{
	double upperBoundaryX=0, lowerBoundaryX=0, upperBoundaryY=0, lowerBoundaryY=0;

	get_boundaries(node1, level, &upperBoundaryX, &upperBoundaryY, &lowerBoundaryX, &lowerBoundaryY);

	if(node2.x() < upperBoundaryX && node2.x() > lowerBoundaryX &&
		node2.y() < upperBoundaryY && node2.y() > lowerBoundaryY &&
		!(node2.x() == node1.x() && node2.y() == node1.y()) )
	{
		return true;
	}

	else
		return false;
}

void findNeighbors(std::vector<Point_2> nodes, int level, std::vector<std::vector<int> > &adjacentVerticesList)
{
	std::vector<int> neighbors;
	double upperBoundaryX=0, lowerBoundaryX=0, upperBoundaryY=0, lowerBoundaryY=0;
	double levelPower;
	double targetx, targety;
	int i,n;

	// Need to clear the previous entries for the neighbors before adding new ones to prevent duplication
	adjacentVerticesList.clear();

	for(n=0; n<nodes.size(); n++)
	{
		neighbors.clear();

		// If level is 0 then, the boundary of the cell containing the point is defined by the next biggest integer and the next lowest integer.
		// for ex., if a point is (12.5, 10.5), its x-boundary is (12,13) and y-boundary is (10,11).

		get_boundaries(nodes[n], level, &upperBoundaryX, &upperBoundaryY, &lowerBoundaryX, &lowerBoundaryY);

		// Because this loop starts at n+1, the vertext that is already accounted for an edge, will not be accounted again and hence the neighbor won't appear in a vertex that comes at a later stage
		for(i=n+1; i<nodes.size(); i++)
		{
			// This if conidition checks if the point is in the neighborhood of the traget point and also makes sure it doesn't put itself in the neighbors list. Also, the points that are in the cell of the current point, are included in its neighborhood.

			if(nodes[i].x() < upperBoundaryX && nodes[i].x() > lowerBoundaryX &&
				nodes[i].y() < upperBoundaryY && nodes[i].y() > lowerBoundaryY &&
				!(nodes[i].x() == targetx && nodes[i].y() == targety) )
			{
				neighbors.push_back(i);
				// std::cout << "Pushing " <<nodes[i] << ")\n";
			}
		}

		adjacentVerticesList.push_back(neighbors);
	}

	return;
}