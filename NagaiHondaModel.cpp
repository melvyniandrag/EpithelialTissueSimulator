#include <sys/time.h>
#include <sstream> 
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <stdio.h>
#include "coordinate.hpp"
#include "cell.hpp"
#include "NagaiHondaForce.hpp"
#include "printCellProperties.hpp"
#include <stdlib.h>
#include "EnergyCalculator.hpp"
#include "Topology.hpp"
#include "PrintOFF.hpp"
#include "MakePlots.hpp"
#include <string.h>

int iter_notification = 10; // How often to report on the current iteration? (So we know the code hasnt hit an infinite loop.)

using namespace std;

void ReadConfig(int&, int&, int&, double&, int&, double&, int&, int&, double&, int&);
void ReadMesh(vector<cell>&, vector<coordinate>&, double*, double*, double, double);
void ReadMeshChanges(vector<cell>&);
void ReadParameters(double&, double&, double&, double&);

// This is for timing the code.
double clkbegin, clkend;
double t;
double rtclock()
{
  struct timezone Tzp;
  struct timeval Tp;
  int stat;
  stat = gettimeofday (&Tp, &Tzp);
  if (stat != 0) printf("Error return from gettimeofday: %d",stat);
  return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}


int main()
{	
	/***************** READ CONFIGURATION FILE ************************/
	int n, print_all, make_movie, num_perturb, num_iters, out_freq;
	int max_swaps, swap_odds;
	double step, energy, delta, dt, swap_len; 
	double time = 0;
	ReadConfig(n, print_all, make_movie, step, num_iters, delta, out_freq, \
			   max_swaps, swap_len, swap_odds);
	
	/********************** READ PARAMETERS ***************************/
	double beta, lambda, t_area, t_gamma;
	ReadParameters(beta, lambda, t_area, t_gamma);
	
    /**************** CONSTRUCT THE X AND Y ARRAYS ********************/
    double X[n], Y[n], TX[n], TY[n];
    memset(X, 0, sizeof(X)); 
    memset(Y, 0, sizeof(Y)); 
    memset(TX, 0, sizeof(TX));
    memset(TY, 0, sizeof(TY));
    
	/************************ MAKE THE MESH ***************************/
	vector<coordinate> coords; // Should be called "vertex", not coordinate. 
	vector<cell> sim_cells;
	ReadMesh(sim_cells, coords, X, Y, t_area, t_gamma);
    //ReadMeshChanges(sim_cells);
	cout << sim_cells.size() << endl;
	for(int i = 0 ; i  < 4; i++)
	{
		if(coords.at(i).IsInner)
			cout << X[i] << " " << Y[i] << endl;
	}
	vector<int> vs;
	for(int i = 0; i < 3; i++)
	{
		vs = sim_cells.at(i).GetVertices();
		for(auto v : vs)
			cout << v << " ";
		cout << endl;
	}
    /******************** MAKE A HISTOGRAM ****************************/
    MakeHistogram("hist_data0.txt", sim_cells);
	
    /************************* WELCOME ********************************/
	cout << "\n\n****************************************************\n";
	cout << "*         Nagai-Honda Model Simulation             *\n";
	cout << "****************************************************\n\n";
	cout << "The number of cells in this simulation is " << sim_cells.size() << endl;
	cout << "The number of coordinates in this simulation is " << coords.size() << endl;
	cout << "Starting the integration... \n";
	ofstream PE; // Keeps track of the potential energy in the simulation.
	PE.open("energy.dat");
	
	/************************** MAIN LOOP *****************************/
    int proceed;
	clkbegin = rtclock();
	for(int iter = 0; iter < num_iters; iter++)
	{
	  if(iter % iter_notification == 0)
        cout << "		Current iteration: "  << iter << endl;
        
		/******************* PRINT OUT AN OFF FILE ********************/
		if(iter % out_freq == 0)
            PrintOFF(iter, sim_cells, coords, X, Y);
		
		/******************* MOVE THE VERTICES ************************/
		dt = step;
		proceed = 0;
		while (proceed == 0)
		{
			proceed = NagaiHondaForce(coords, sim_cells, dt, delta, X, Y, TX, TY, beta, lambda);
			dt = dt/2; // If any vertex moved too much, halve the step size.
		}
		time += dt;
        X[2] += TX[2];
		Y[2] += TY[2];
		

		/**************** PERFORM TOPOLOGICAL CHANGES *****************/
		//Perform_T2s(sim_cells, coords, delta, X, Y);
		Perform_T1s(sim_cells, coords, delta, X, Y);
		
        /**************** CALCULATE ENERGY IN THE MESH ****************/
		energy = Energy(sim_cells, coords, X, Y, beta, lambda);
		PE << time << " " << energy << endl;
		
	}
    
    /*********************** STOP THE TIMER ***************************/
	clkend = rtclock();
    t = clkend-clkbegin;
	cout << "The computation finished in " << t << " seconds!" << endl;
	
	/*********************** MAKE THE MOVIE ***************************/
	if(make_movie == 1)
	{
		cout << "Making animation..." << endl;
		system("bash Images/make_movie.sh");
	}
		
	
	/******************* MAKE THE PLOTS *************************/
    MakeHistogram("hist_data1.txt", sim_cells);
    if (print_all == 1)
        MakePlots(sim_cells, X, Y);    

	/****************************  DONE  ******************************/
	cout << "\n*************Simulation Complete********************\n\n";

}

void ReadConfig(int  &n, int &print_all, int &make_movie, double &step, int &num_iters, double &delta, int &out_freq, int &max_swaps, double &swap_len, int &swap_odds)
{
	string junk; // a variable to store the junk we dont want from the conf file.
	ifstream conf("config.txt");
	conf >> n; // get the info we want
	getline(conf, junk); // throw away the rest of the line.
	conf >> print_all;
	getline(conf, junk);
	conf >> make_movie;
	getline(conf, junk);
	conf >> step;
	getline(conf, junk);
	conf >> num_iters;
	getline(conf, junk);
	conf >> out_freq;
	getline(conf, junk);
	conf >> delta;
	getline(conf, junk);
    conf >> max_swaps;
	getline(conf, junk);
    conf >> swap_len;
	getline(conf, junk);
    conf >> swap_odds;
	getline(conf, junk);
	cout << "Dimension of mesh: " << n << endl;
	cout << "Number of iterations " << num_iters << endl;
	cout << "Maximum step size for the integration " << step << endl;
	cout << "Delta for T1 swap " << delta << endl;
	cout << "An image will be printed every " << out_freq << " iterations." << endl;
    cout << "The maximum number of initial T1 swaps is: " << max_swaps << endl;
}

void ReadMeshChanges(vector<cell>& sim_cells)
{
    string comment;
    int index;
    double parameter;
    int num_gamma = 0, num_area;
    ifstream changes("change_mesh.txt");
    getline(changes, comment);
    changes >> num_gamma >> num_area;
    for(int line = 0; line < num_gamma; line++)
    {
        changes >> index >> parameter;
        sim_cells[index].SetGamma(parameter);
    }
    for(int line = 0; line < num_area; line++)
    {
        changes >> index >> parameter;
        sim_cells[index].SetTargetArea(parameter);
    }
}

void ReadMesh(vector<cell>& sim_cells, vector<coordinate>& coords, double* X, double* Y, double t_area, double t_gamma)
{
/*
 * This function will read in an OFF file, and then generate 
 * a mesh from this file.
 */
	int nV, nC; // num verts, num cells.
	int vpc, vert; // verts per cell, vertex index.
	double x, y, z;
	bool interior;
	vector<int> vertVector;
	string comment;
	ifstream mesh("ic.txt"); //initial condition file.
	mesh >> nV >> nC;
	for(int v = 0; v < nV; v++)
	{
		mesh >> x >> y >> interior;
		X[v] = x;
		Y[v] = y;
		coords.push_back(coordinate(v, interior));
	}
	for(int c = 0; c < nC; c++)
	{
		mesh >> vpc;
		for(int v = 0; v < vpc; v++)
		{
			mesh >> vert;
			vertVector.push_back(vert);
		}
		sim_cells.push_back(cell(c, vertVector, t_area, t_gamma));
		vertVector.clear();
	}
	mesh.close();
}
void ReadParameters(double& beta, double& lambda, double& t_area, double& t_gamma)
{
	ifstream parameters("parameters.txt");
	parameters >> beta;
	parameters >> lambda;
	parameters >> t_area;
	parameters >> t_gamma;
	parameters.close();
}
