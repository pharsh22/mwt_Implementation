===== To run the algorithm which is implemented in c++ ======
Required Libraries
	- CGAL
	- dependencies for CGAL (it will be installed with CGAL if you install using brew or install them separately otherwise)
	- gcc/clang compiler
	- The program was built on MacOS, the auther is not aware if it will be supported in Visual Studio

To complie the program, run the following commands in terminal in the project directory
NOTE: The '$' sign at the beginning is just a terminal prompt

	$ cgal_create_CMakeLists -s runProgram
	$ cmake -DCGAL_DIR=$HOME/CGAL4.9 . (note the dot at the end)
	$ make
	$ ./runProgram

Input: Set of points in 2D. It reads the points from "input.txt" file which stores individual points on each line as 'x y'

Output: The terminal output is the arrangement that is formed by adding edges between those points.
		First group: List of vertices in the arrangement
		Second group: List of edges in the form of [x1 y1 x2 y2] in the arrangement
		Third group: List of faces in the arrangement in the format "current point current edge -- next point next edge" until all the points and edges of the face boundary are covered
		NOTE: The edge doesn't have to be directed from current point to next point, it might be directed from next point to current point and hence the edge_start and edge_end vertex do not matter.
		The C++ main program generates a "mwtOutput.txt" file, which will be used by Matlab at the visualization step.

===== To run .r file for delaunay triangulation =====
Required tools
	- R binaries installed on the system
	- R Studio

To run the program
	- Click on the "Source" button in R Studio


===== To run the .m file to visualize the triangulation in Matlab =====
Open the matlab script in Matlab2015 or any other version and run