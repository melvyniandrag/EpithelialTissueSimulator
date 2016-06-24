#include <stdio.h>
#include <vector>

void printCellProperties(std::vector<cell> cells, std::vector<coordinate> coordinate_list, double * X, double * Y)
{
	for(std::vector<cell>::iterator it = cells.begin();
			it != cells.end(); ++it)
	{
		int index = it->GetIndex();
		printf("Area of Cell %i : %f \n", index, it->ComputeArea(X, Y));
		printf("Perimeter of Cell %i : %f \n\n", index, it->ComputePerimeter(X, Y));
	
	}
}
