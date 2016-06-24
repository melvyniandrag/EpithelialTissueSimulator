/* This is the meat and potatoes of the code. This algorithm will 
* move each vertex and store its new location in a temporary location
* before copying the temporary array to the old array.
*/

#include "cell.hpp"
#include "coordinate.hpp"
#include <vector>
#include <cmath>
#include <iostream>
#include <omp.h>
#include <algorithm>
#include "Get_Assoc_Cells.hpp"
#include "AreaForce.hpp"
#include "PerAdhForce.hpp"

using std::cout;
using std::endl;
using std::abs;
using std::sqrt;

int NagaiHondaForce(std::vector<coordinate>& coords,
	 std::vector<cell>& sim_cells, double step, double delta, double * X,
					double * Y, double *TX, double * TY, double beta, double lambda)
{	
	double dist = 0;
	int RETURN_VALUE = 1;
	for(std::vector<coordinate>::iterator it = coords.begin();it != coords.end();it++)
	{
		if(it->IsInner)
		{
			std::vector<int> assoc_cells = GetCells(it->index, sim_cells);
			std::vector<double> area_force = AreaForce(it->index, sim_cells, assoc_cells, coords, X, Y, beta, lambda);
			std::vector<double> per_adh_force =PerAdhForce(it->index, sim_cells, assoc_cells, coords, X, Y, beta, lambda);
            int id = it->index;
			TX[id] = step * (area_force.at(0) + per_adh_force.at(0));
			TY[id] = step * (area_force.at(1) + per_adh_force.at(1));

			/* Verify that no vertex has moved too much */
			dist = sqrt(pow(TX[id], 2) + pow(TY[id], 2));
			if(dist >  0.5*delta)
				RETURN_VALUE = 0;
		}
	}
	return RETURN_VALUE;
}

