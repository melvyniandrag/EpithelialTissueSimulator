#include <vector>

int NagaiHondaForce(std::vector<coordinate>& coordinate_list,
		     std::vector<cell>& simulation_cells, double step, double delta
					, double * X, double * Y, double * TX, double * TY, double beta, double lambda );
