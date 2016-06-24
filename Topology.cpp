/* 
* This file performs the topological changes for the mesh.
* T1 swap - when an edge gets two small and is reoriented.
* T2 swap - when a triangular cell has a small area and is deleted.
* 
* I identify two types of interior edges. There are fully interior 
* edges and half interior edges. A full interior edge will affect four 
* cells when it undergoes the transformation. A half interior edge will 
* affect only three cells whereas an full interior edge will affect 
* four cells.
* 
*/

#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath> /* The cmath routines use radians, not degrees !*/
#include <algorithm>
#include <cassert>
#include <random>
#include "cell.hpp"
#include "coordinate.hpp"

const double swap_low_bd = 0.1 ;

using namespace std;

bool full_int, half_int, both;

/***********************************************************************
*                                                                      *
*************** Functions used by the topology functions ***************
*                                                                      *
***********************************************************************/
double len(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

vector<double> MidPoint (double x1, double y1, double x2, double y2)
{
    vector<double> point;
    point.push_back((x1 + x2)/2);
    point.push_back((y1 + y2)/2);
    return point;	
}


double M_Perp(double x1, double y1, double x2, double y2)
{
  return -1.0*(x2 - x1) / (y2 - y1);
}

/***********************************************************************
*                                                                      *
********************* Topology functions *******************************
*                                                                      *
***********************************************************************/

double epsilon = 0.00001;


/* 
* Remember to always try to break the code up into the smallest
* meaningful pieces possible. I am making the T1 swap its own routine, 
* the search for cells ci, etc.
*/


void T1_Swap(cell& c1, cell& c2, cell& c3, cell& c4, coordinate& I, 
			 coordinate& Ip1, double delta, double * X, double * Y)
{
    /* The T1 routine performs I T1 swap on 4 cells if the vertices I 
    * and Ip1 get too close together. The technical details of this 
    * routine are layed out with great detail in my thesis paper. */
    int i = I.index;
    int ip1 = Ip1.index;
    
    /* delete I from c1, delete Ip1 from c2. */
    c1.EraseVert(i);
    c2.EraseVert(ip1);
    /* calculate the midpoint of AB, and the perpendicular slope to AB.*/
    vector<double> mp = MidPoint(X[i], Y[i], X[ip1], Y[ip1]);
    double m = M_Perp(X[i] + epsilon, Y[i] + epsilon, X[ip1] - epsilon, Y[ip1]-epsilon);

	double theta = atan(m);
	//std::cout << "m is " << m << std::endl;
	//std::cout << "theta is " << theta <<std::endl;
    double dx = abs(delta*cos(theta));
    double dy = abs(delta*sin(theta));

	double avg_c2_x = c2.avgx(X);
	double avg_c2_y = c2.avgy(Y);

	double sign_c2_x = 1;
	double sign_c2_y = 1;

	if((avg_c2_x - mp.at(0)) < 0)
	  sign_c2_x = -1;
	if((avg_c2_y - mp.at(1)) < 0)
	  sign_c2_y = -1;
	
	double sign_c1_x = -1*sign_c2_x;
	double sign_c1_y = -1*sign_c2_y;

    /* Change I and Ip1 in the coordinate lists */
	double dxi = sign_c2_x*dx;
	double dyi = sign_c2_y*dy;
	double dxip1 = sign_c1_x*dx;
    double dyip1 = sign_c1_y*dy;

	
    X[i] = mp.at(0) + dxi; Y[i] = mp.at(1) + dyi;
    X[ip1] = mp.at(0) + dxip1; Y[ip1] = mp.at(1) + dyip1;
	//std::cout << "X[i] is " << X[i] << std::endl;
	//std::cout << "Y[i] is " << Y[i] << std::endl;
  
    /* Insert Ip1 after I in c4, insert I after Ip1 in c3. */
    c3.InsertVert(ip1, i);
    c4.InsertVert(i, ip1);
}


void Perform_T1s(vector<cell>& sim_cells, vector<coordinate>& coords, double delta, double * X, double * Y)
{
    /* This function sniffs out the locations of the T1 swaps and
    * performs them on the whole mesh. As of now this is a very
    * 'brute force' algorithm. See Knuth, D. "Premature optimization
    * is the root of all evil. That is my alibi. */
    

  for(auto c : sim_cells)         
    {
        vector <int> verts = c.GetVertices(); 
        for(int i = 0; i < verts.size(); i++)                
        {
            int Im1 = verts.at((i-1)%(verts.size()));        
            int I = verts.at(i);
            int Ip1 = verts.at((i+1)%(verts.size()));
            int Ip2 = verts.at((i+2)%(verts.size()));
            
            coordinate prev = coords.at(Im1);
            coordinate curr = coords.at(I);
            coordinate next = coords.at(Ip1);
            coordinate next_next = coords.at(Ip2); 

            double prev_x      = X[Im1];
            double curr_x      = X[I];
            double next_x      = X[Ip1];
            double next_next_x = X[Ip2];

            double prev_y      = Y[Im1];
            double curr_y      = Y[I];
            double next_y      = Y[Ip1];
            double next_next_y = Y[Ip2];

            
            if((curr.IsInner)&&(next.IsInner)&&\
			   (len(curr_x, curr_y, next_x, next_y) < delta))
            {
			  //cout << "***************** STARTING T1 ***************************" <<endl;
                /* Find which cells are affected by this edge. */
                /* The four involved cells will be:
                * 1) The current cell with the small edge.
                * 2) The other cell involving both of these two vertices.
                * 3) The cell involving (i+1) and (i+2)
                * 4) The cell involving (i-1) and (i)
                */
                int id1 = c.GetIndex();
                int id2, id3, id4;
                for(auto c_search : sim_cells)
                {
                    /* We could make a function called contains_both() */
                    if((c_search.GetIndex() != id1)&&\
					   (c_search.ContainsVertex(curr.index))&&\
					   (c_search.ContainsVertex(next.index)))
                    {
					  id2 = c_search.GetIndex();
					  //cout << "found c2" <<endl;
                    }

                    if((c_search.GetIndex() != id1)&&\
					   (c_search.ContainsVertex(next.index))&&\
					   (c_search.ContainsVertex(next_next.index)))
                    {
					    id3 = c_search.GetIndex();
						//cout << "found c3" <<endl;
                    
					}
                    if((c_search.GetIndex() != id1)&&\
					   (c_search.ContainsVertex(prev.index))&&\
					   (c_search.ContainsVertex(curr.index)))
                    {
                        id4 = c_search.GetIndex();
						//cout << "found c4" <<endl;

                    }
                }
				T1_Swap(sim_cells[id1], sim_cells[id2], \
						sim_cells[id3], sim_cells[id4], \
						coords[I], coords[Ip1] , delta, X, Y); 
                }
        }
    }
}

void Random_T1s(vector<cell>& sim_cells, vector<coordinate>& coords, int max_swaps, double swap_len, int swap_odds, double * X, double * Y)
{
    int num_swaps = 0;
    for(vector<cell>::iterator c = sim_cells.begin(); c != sim_cells.end(); ++c)              //Go through every cell in the mesh
    {
		    std::random_device rd; // obtain a random number from hardware
			std::mt19937 eng(rd()); // seed the generator
			std::uniform_int_distribution<> distr(0, swap_odds); // define the range
			int should_swap = distr(eng);
			if(should_swap == 1 && c->GetNumSides() > 3 && num_swaps < max_swaps)
			{
				vector <int> verts = c->GetVertices(); 
                int Im1 = verts.at(0);                                     // These are the indices 
                int I = verts.at(1);
                int Ip1 = verts.at(2);
                int Ip2 = verts.at(3);
                
                coordinate prev = coords.at(Im1);
                coordinate curr = coords.at(I);
                coordinate next = coords.at(Ip1);
                coordinate next_next = coords.at(Ip2);

                double prev_x      = X[Im1];
                double curr_x      = X[I];
                double next_x      = X[Ip1];
                double next_next_x = X[Ip2];

                double prev_y      = Y[Im1];
                double curr_y      = Y[I];
                double next_y      = Y[Ip1];
                double next_next_y = Y[Ip2];             
                    
                
                if((curr.IsInner)&&(next.IsInner)\
				   &&(len(curr_x, curr_y, next_x, next_y) < swap_len))
				  //&&(len(curr_x, curr_y, next_x, next_y) > swap_low_bd))
				  // Then the edge is fully inside and it is critically small.
                {
                    /* Find which cells are affected by this edge. */
                    /* The four involved cells will be:
                    * 1) The current cell with the small edge.
                    * 2) The other cell involving both of these two vertices.
                    * 3) The cell involving (i-1) and (i)
                    * 4) The cell involving (i+1) and (i+2)
                    */
                    int id1 = c - sim_cells.begin();
                    int id2, id3, id4;
                    for(auto c_search : sim_cells)
                    {
                        /* We could make a function called contains_both() */
                        if((c_search.GetIndex() != id1)&&
						   (c_search.ContainsVertex(curr.index))&&
						   (c_search.ContainsVertex(next.index)))
                        {
                            id2 = c_search.GetIndex();
                        }
    
                        if((c_search.GetIndex() != id1)&&
						   (c_search.ContainsVertex(next.index))&&
						   (c_search.ContainsVertex(next_next.index)))
                        {
                            id3 = c_search.GetIndex();
                        }
                        if((c_search.GetIndex() != id1)&&
						   (c_search.ContainsVertex(prev.index))&&
						   (c_search.ContainsVertex(curr.index)))
                        {
                            id4 = c_search.GetIndex();
                        }
                    }
                    T1_Swap(sim_cells[id1], sim_cells[id2], sim_cells[id3], sim_cells[id4], coords[I], coords[Ip1] , swap_low_bd, X, Y); // This is upside down
                    num_swaps++;
                    max_swaps--;
                }//if(curr)
        } // if(max_swaps)
    }//for(sim_cell iterator)
    for(vector<cell>::iterator c = sim_cells.begin(); c != sim_cells.end(); ++c)              //Go through every cell in the mesh
    {
		    std::random_device rd; // obtain a random number from hardware
			std::mt19937 eng(rd()); // seed the generator
			std::uniform_int_distribution<> distr(0, swap_odds); // define the range
			int should_swap = distr(eng);
			if((should_swap == 1)&&(c->GetNumSides() > 4))
			{
				vector <int> verts = c->GetVertices(); 
                int Im1 = verts.at(1);                                     // These are the indices 
                int I = verts.at(2);
                int Ip1 = verts.at(3);
                int Ip2 = verts.at(4);
                coordinate prev = coords.at(Im1);
                coordinate curr = coords.at(I);
                coordinate next = coords.at(Ip1);
                coordinate next_next = coords.at(Ip2);
                double prev_x      = X[Im1];
                double curr_x      = X[I];
                double next_x      = X[Ip1];
                double next_next_x = X[Ip2];

                double prev_y      = Y[Im1];
                double curr_y      = Y[I];
                double next_y      = Y[Ip1];
                double next_next_y = Y[Ip2]; 
                
                if((curr.IsInner)&&(next.IsInner)&&(len(curr_x, curr_y, next_x, next_y) < swap_len)&&(len(curr_x, curr_y, next_x, next_y) > swap_low_bd*1.1)) // Then the edge is fully inside and it is critically small.
                {
                    /* Find which cells are affected by this edge. */
                    /* The four involved cells will be:
                    * 1) The current cell with the small edge.
                    * 2) The other cell involving both of these two vertices.
                    * 3) The cell involving (i-1) and (i)
                    * 4) The cell involving (i+1) and (i+2)
                    */
                    int id1 = c - sim_cells.begin();
                    int id2, id3, id4;
               for(auto c_search : sim_cells)
                    {
                        /* We could make a function called contains_both() */
                        if((c_search.GetIndex() != id1)&&(c_search.ContainsVertex(curr.index))&&(c_search.ContainsVertex(next.index)))
                        {
                            id2 = c_search.GetIndex();
                        }
    
                        if((c_search.GetIndex() != id1)&&(c_search.ContainsVertex(next.index))&&(c_search.ContainsVertex(next_next.index)))
                        {
                            id3 = c_search.GetIndex();
                        }
                        if((c_search.GetIndex() != id1)&&(c_search.ContainsVertex(prev.index))&&(c_search.ContainsVertex(curr.index)))
                        {
                            id4 = c_search.GetIndex();
                        }
                    }
                    T1_Swap(sim_cells[id1], sim_cells[id2], sim_cells[id3], sim_cells[id4], coords[I], coords[Ip1] , swap_low_bd, X, Y); // This is upside down
                    num_swaps++;
                    max_swaps--;
                 }//if(curr)
        } // if(max_swaps)
    }//for(sim_cell iterator)
    cout << num_swaps << " inital random T1 swaps occured!" << endl;
}

void Perform_T2s(vector<cell>& sim_cells, vector<coordinate>& coords, double delta, double * X, double * Y)
{
	/* Loop over all of the cells in the mesh. 
	 * For each cell, check the number of vertices in the cell.
	 * If there are three vertices, check the length of the edges in the cell.
	 * If all of the edges are below delta, then the cell becomes a point.
	 * We will compute the centroid of the cell, delete two of the coordinates
	 * and reset the remaining vertex to the centroid.
	 * Then we look over all of the cells in the mesh and find the cells containing the deleted vertices. 
	 * The vertex is then replaced by the index of the centroid vert.
	 */
	for(vector<cell>::iterator c = sim_cells.begin(); c != sim_cells.end(); c++){
		vector<int> v = c->GetVertices();
		if(v.size() == 3){ // Then c is a triangle
			coordinate A = coords.at(v.at(0));
			coordinate B = coords.at(v.at(1));
			coordinate C = coords.at(v.at(2));
            int aidx = A.index;
            int bidx = B.index;
            int cidx = C.index;
            
			if(len(X[aidx], Y[aidx], X[bidx], Y[bidx]) < delta || 
			   len(X[aidx], Y[aidx], X[cidx], Y[cidx]) < delta || 
			   len(X[bidx], Y[bidx], X[cidx], Y[cidx]) < delta)
			  {
				cout << "T2 Swap Occurred!" << endl;
				X[aidx] = (X[aidx]+X[bidx] + X[cidx])/3;
                Y[aidx] = (Y[aidx]+Y[bidx] + Y[cidx])/3;// Update the a coordinates.
				vector <coordinate>::iterator i = find (coords.begin(), coords.end(), B); // Delete the b and c coordinates from the list.
				coords.erase(i);
				i = find (coords.begin(), coords.end(), C); 
				coords.erase(i);
				sim_cells.erase(c); // This only works because the size of the container is going to be larger than one.
				int found = 0;
				for(vector<cell>::iterator other = sim_cells.begin(); other != sim_cells.end(); other++)
				{
					if(other->ContainsVertex(B.index) && other->ContainsVertex(A.index))
					{
						other->EraseVert(B.index);
						found++;
					}
					if(other->ContainsVertex(C.index) && other->ContainsVertex(A.index))
					{
						other->EraseVert(C.index);
						found++;
					}
					if(other->ContainsVertex(B.index) && other->ContainsVertex(C.index))
					{
						other->ReplaceVert(B.index, A.index); // Replace b by a
						other->EraseVert(C.index); // delete c. Problem solved.
						found++;
					}
					if(found == 3) break;
				}
			  }
		}
	}
    
}

