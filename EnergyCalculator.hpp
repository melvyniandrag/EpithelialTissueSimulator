#include <vector>
#include <cmath>
#include "omp.h"
#include "cell.hpp"
#include "coordinate.hpp"
/*
Calculate the energy in the mesh at any timestep.
*/

using namespace std;

double Distance(double x1, double y1, double x2, double y2)
{
	// This function calculates the reciprocal distance needed in the calculation of the gradients.
	return sqrt(pow((x1 - x2),2)+pow((y1 - y2),2));
}

double Energy(vector<cell> simulation_cells, vector<coordinate> coordinate_list, double * X, double * Y, double beta, double lambda)
{
	int curr_vert, next_vert;
	double my_gamma, other_gamma, avg_gamma;
	double energy = 0.0;
	for(std::vector<cell>::iterator it = simulation_cells.begin();
		it != simulation_cells.end(); ++it)
	{
		energy += lambda*pow((it->GetTargetArea() - it->ComputeArea(X, Y)),2);
		energy += beta*pow((it->GetTargetPerimeter() - it->ComputePerimeter(X, Y)),2);
		// Go through all of the vertices in the cell and make edges.
		// Find which cell is is connected to.
		std::vector<int> Vertices = it->GetVertices();
		int n_verts = Vertices.size();
		for(int i = 0; i < n_verts; i++)
		{
			if( i != n_verts - 1)
			{
				curr_vert = Vertices.at(i);
				next_vert = Vertices.at(i+1);
			}
			else // i is the last element in the vector. Wrap around. 
			{
				curr_vert = Vertices.at(n_verts - 1);
				next_vert = Vertices.at(0);
			}
			// Should make a ContainsEdge() function for cells.
			coordinate v1 = coordinate_list.at(curr_vert);
			coordinate v2 = coordinate_list.at(next_vert);
            double v1x = X[curr_vert]; double v1y = Y[curr_vert];
            double v2x = X[next_vert]; double v2y = Y[next_vert]; 
			if((v1.IsInner)&&(v2.IsInner)) // If the edge is interior
			{
				my_gamma = it->GetGamma();
				for (auto c : simulation_cells)
				{
					if ((c.ContainsVertex(curr_vert))&&(c.ContainsVertex(next_vert))&&(c.GetIndex() != it->GetIndex()))
					{
						// Then c is the other cell
						other_gamma = c.GetGamma();
					}
				}
				energy += 0.5*(my_gamma + other_gamma)*Distance(v1x, v1y, v2x, v2y);
			}
		}
	}
	return energy;
}
