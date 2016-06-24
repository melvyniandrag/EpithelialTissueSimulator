/*
 * This file will create a hexagonal mesh, then perturb a number of 
 * vertices as specified by some parameter.
 * This takes a pointer to the coordinate list, and the cell vector and generates the contents.
 */
 
#include <vector>
#include "cell.hpp"
#include "coordinate.hpp"
#include <iostream>
#include <time.h>

using namespace std;

void hex_mesh(std::vector<coordinate> & coordinate_list, \
	std::vector<cell> & simulation_cells, int n, double * X, double * Y) // n  must be odd. n is the dimension of the mesh.
{
  int idx = 0;
  int is_interior = 1;
  // Make the coordinates
  for(int y = 1; y <= 2*n - 1; y++)
  {
		if(y == 1) // Make the initial set of coordinates
		{
			X[idx] = 1; Y[idx] = 1;
            coordinate_list.push_back(coordinate(idx, 1, 1, 0)); // This one is padding
			idx++;
	        for(int x = 0; x < (n-1)/2; x++)
			{
				is_interior = 0;
                X[idx] = 4*(x+1); Y[idx] = y;
				coordinate_list.push_back(coordinate(idx, 4*(x+1), y, is_interior)); // which ones are the interior coordinates? This is easy to see.
				idx++;
                X[idx] = 4*(x+1) + 1; Y[idx] = y;               
				coordinate_list.push_back(coordinate(idx, 4*(x+1) + 1, y, is_interior));
				idx++;
			}
            X[idx] = 1; Y[idx] = 1;
			coordinate_list.push_back(coordinate(idx, 1, 1, 0)); // This one is padding
			idx++;
		}
		else if(y == 2*n-1) // Make the final set of coordinates
		{
            X[idx] = 1; Y[idx] = 1;
			coordinate_list.push_back(coordinate(idx, 1, 1, 0)); // This one is padding
			idx++;
			for(int x = 0; x < (n-1)/2; x++)
			{
				is_interior = 0;
                X[idx] = 4*(x+1); Y[idx] = y;
				coordinate_list.push_back(coordinate(idx, 4*(x+1), y, is_interior)); // which ones are the interior coordinates? This is easy to see.
				idx++;
                X[idx] = 4*(x+1) + 1; Y[idx] = y;
				coordinate_list.push_back(coordinate(idx, 4*(x+1) + 1, y, is_interior));
				idx++;
			}
            X[idx] = 1; Y[idx] = 1;
			coordinate_list.push_back(coordinate(idx, 1, 1, 0)); // This one is padding
			idx++;
		}
		// Make the intermediate coordinates
		else if(y == 2)
		{
			for(int x = 0; x < (n+1)/2; x++) 
			{
				is_interior = 0;
                X[idx] = 2 + 4*x; Y[idx] = y;
				coordinate_list.push_back(coordinate(idx, 2 + 4*x, y, is_interior)); // which ones are the interior coordinates? This is easy to see.
				idx++;
                X[idx] = 2 + 4*x + 1; Y[idx] = y;
				coordinate_list.push_back(coordinate(idx, 2 + 4*x + 1, y, is_interior));
				idx++;				
			} 
		}
		else if(y == 2*n - 2)
		{
			for(int x = 0; x < (n+1)/2; x++) 
			{
				is_interior = 0;
                X[idx] = 2 + 4*x; Y[idx] = y;
				coordinate_list.push_back(coordinate(idx, 2 + 4*x, y, is_interior)); // which ones are the interior coordinates? This is easy to see.
				idx++;
                X[idx] = 2 + 4*x + 1; Y[idx] = y;
				coordinate_list.push_back(coordinate(idx, 2 + 4*x + 1, y, is_interior));
				idx++;				
			} 
		}
		else if(y % 2 == 0)
		{
			for(int x = 0; x < (n+1)/2; x++) 
			{
				if (x == 0){is_interior = 0;}
				else {is_interior = 1;};
                X[idx] = 2 + 4*x; Y[idx] = y;
				coordinate_list.push_back(coordinate(idx, 2 + 4*x, y, is_interior)); // which ones are the interior coordinates? This is easy to see.
				idx++;
				if (x == (n+1)/2 - 1){is_interior = 0;}
				else {is_interior = 1;};
                X[idx] = 2 + 4*x + 1; Y[idx] = y;
				coordinate_list.push_back(coordinate(idx, 2 + 4*x + 1, y, is_interior));
				idx++;				
			} 
		}
		else
		{
			for(int x = 0; x < (n+1)/2; x++) 
			{
				if (x == 0){is_interior = 0;}
				else {is_interior = 1;};
                X[idx] = 1 + 4*x; Y[idx] = y;
				coordinate_list.push_back(coordinate(idx, 1 + 4*x, y, is_interior)); // which ones are the interior coordinates? This is easy to see.
				idx++;
				if (x == (n+1)/2 - 1){is_interior = 0;}
				else {is_interior = 1;}
                X[idx] = 1 + 4*x + 3; Y[idx] = y;                
				coordinate_list.push_back(coordinate(idx, 1 + 4*x + 3, y, is_interior));
				idx++;				
			} 
		}
	}
	
	int cc_idx = 0; // current cell index
	
	
	//Generate the cells from the vertices
	std::vector<int> vl; //vertex list
	int vpr = n + 1; // verts per row.
	
	// Generate the cells in the first row
	for(int i = 0; i < n/2; i++)
	{
		int N = 2*i + 1;
		vl.clear();	
		vl.push_back(N);
		vl.push_back(N + 1);
		vl.push_back(N + vpr +1);
		vl.push_back(N + 2*vpr + 1);
		vl.push_back(N + 2*vpr);
		vl.push_back(N + vpr);
		simulation_cells.push_back(cell(cc_idx, vl));
		cc_idx++;
	}
	// Generate the cells int the subsequent rows
	for (int row = 1; row < 2*n - 3; row++)
	{
		// make an even row
		if(row % 2 == 0)
		{
			for(int vert = vpr*row + 1; vert <= vpr*(row+1) - 3; vert+=2)
			{
				vl.clear();
				vl.push_back(vert);
				vl.push_back(vert + 1);
				vl.push_back(vert + vpr +1);
				vl.push_back(vert + 2*vpr + 1);
				vl.push_back(vert + 2*vpr);
				vl.push_back(vert + vpr);
				simulation_cells.push_back(cell(cc_idx, vl));
				cc_idx++;
			}
		}
		// make the odd row
		else if (row % 2 != 0)
		{
			for(int vert = vpr*row; vert <= vpr*(row + 1) - 2; vert+=2)
			{
				vl.clear();
				vl.push_back(vert);
				vl.push_back(vert + 1);
				vl.push_back(vert + vpr +1);
				vl.push_back(vert + 2*vpr + 1);
				vl.push_back(vert + 2*vpr);
				vl.push_back(vert + vpr);
				simulation_cells.push_back(cell(cc_idx, vl));
				cc_idx++;
			}
		}
	}
	
}

void perturb_mesh(std::vector<coordinate> & coordinate_list, \
	std::vector<cell> & simulation_cells, int n, int num_perturb, double * X, double * Y)
{
    
   	srand(time(NULL));
	for(std::vector<coordinate>::iterator pos = coordinate_list.begin();
		pos != coordinate_list.end(); ++pos)
	{
		if (pos->IsInner)
		{
			X[pos->index] += 0.5*(rand()/(double)(RAND_MAX) - 0.5);
			X[pos->index] += 0.5*(rand()/(double)(RAND_MAX) - 0.5);
		}
	}  
    
}


