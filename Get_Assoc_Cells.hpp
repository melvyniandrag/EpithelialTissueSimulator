#include <iostream>
#include <cassert>
#include <vector>
#include "cell.hpp"
#include "coordinate.hpp"

using std::cout;
using std::endl;
using std::vector;

int pos;

vector<int> GetCells(int coord, vector<cell> sim_cells)
{		  
		vector<int> cells; 
		for(auto c : sim_cells){			
			if(c.ContainsVertex(coord))
				cells.push_back(c.GetIndex());			                           // Collect all of the cells which include this vertex. 
		}
        assert(cells.size() == 3);
		// There ought to be three cells with this vertex. 
        int c1 = cells.at(0);                                                      // The first cell containing this vertex.
		vector<int> verts = sim_cells[c1].GetVertices();                           // All of the vertices in this first cell.	
		vector <int>::iterator i = find (verts.begin(), verts.end(), coord);       // Locate the vertex in the first cell.
    
		if (i != verts.end())
            pos = distance(verts.begin(), i);
            
		int next = verts.at((pos + 1) % verts.size());
        
        int c2 = cells.at(1);
        if(sim_cells[c2].ContainsVertex(next) == 0){
            int tmp = cells.at(2);
            cells[2] = cells[1];
            cells[1] = tmp;
        }	
		std::reverse(cells.begin(), cells.end());	
		return cells;
}
