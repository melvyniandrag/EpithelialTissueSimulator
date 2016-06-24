#include <iostream>
#include <vector>
#include <algorithm>
#include "cell.hpp"
#include "coordinate.hpp"


std::vector<double> AreaForce(int current_vertex, std::vector<cell> simulation_cells,  std::vector<int> Associated_Cells, std::vector<coordinate> coordinates, double* X, double* Y, double beta, double lambda)
{
  // We can make this faster by storing the cell data in Associated_Cells, 
  // And passing around a couple of cells in the vector instead of passing
  // around the whole cell vector.
  
  std::vector<double> force_vec {0,0}; // initialize the force vector to <0,0>
  for(std::vector<int>::iterator it = Associated_Cells.begin(); it != Associated_Cells.end(); ++it)
  {
    int idx = *it; // Double check this
    cell my_cell = simulation_cells.at(idx); // This is the cell we are focusing on now.
    double target_area = my_cell.GetTargetArea(); // Target area
    double area = my_cell.ComputeArea(X, Y); // Actual area
    std::vector<int> my_verts = my_cell.GetVertices(); // vertices in the cell.
    // Find the local index of the vertex in this vector and get the surrounding vertex indices.
    int l_idx = std::find(my_verts.begin(), my_verts.end(), current_vertex) - my_verts.begin();
    int next_vert = my_verts.at((l_idx + 1) % my_verts.size());  // No issue with modulo division here because the l_idx will be positive.
    int prev_vert;// Modulo division gives negative 1 if the l_idx is 0.
    if(l_idx == 0)
    {
      prev_vert = my_verts.back();
    }
    else
    {
      prev_vert = my_verts.at(l_idx - 1);
    }
    // get the coordinates of the previous and following coordinates.
    coordinate prev_coord = coordinates.at(prev_vert);
    coordinate next_coord = coordinates.at(next_vert);
    
    double prev_coord_x = X[coordinates.at(prev_vert).index];
    double prev_coord_y = Y[coordinates.at(prev_vert).index];
    
    double next_coord_x = X[coordinates.at(next_vert).index];
    double next_coord_y = Y[coordinates.at(next_vert).index];
    
    force_vec.at(0) -= lambda*(area - target_area)*(next_coord_y - prev_coord_y); // Notice the negative here!!!!!
    force_vec.at(1) -= lambda*(area - target_area)*(prev_coord_x - next_coord_x);
  }
  
  return force_vec;
  
}
