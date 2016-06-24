/*
 * This file holds all of the class information for the cell objects. 
 * cells need to know the vertices which make them up,
 * be able to calculate their area and perimeter, and be able to perform
 * the necessary topological changes described by our model. 
 */
#ifndef CELL
#define CELL

#include <vector>
#include <cassert>
#include <cmath>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include "coordinate.hpp"

const double pi = 3.1415;

class cell
{
public:
	cell(int index, std::vector<int> AssociatedVertices, double target_area, double gamma)
	{	
		assert(index >= 0);
		m_AssociatedVertices = AssociatedVertices;
		m_index = index;
		m_target_area = target_area;
		m_target_perimeter = std::sqrt(pi * m_target_area);
		m_gamma = gamma; 
	}
	
	cell(){}
	
	std::vector<int> GetVertices(){return m_AssociatedVertices;};
	int GetIndex(){return m_index;};
   	void SetIndex(int index){m_index = index;};
    void SetTargetArea(double area){m_target_area = area;};
	double GetTargetArea(){return m_target_area;};
	double GetTargetPerimeter(){return m_target_perimeter;};
	double ComputeArea(double * X, double * Y);
	double ComputePerimeter(double * X, double * Y);
	void PrintCell(std::ofstream &OffFile);
	int ContainsVertex(int index);
	void SetGamma(double gamma){m_gamma = gamma;};
	double GetGamma(){return m_gamma;};
	void InsertVert(int v1, int v2);
	void EraseVert(int index)
	{
		std::vector<int>::iterator it = std::find(m_AssociatedVertices.begin(), m_AssociatedVertices.end(), index); 
		m_AssociatedVertices.erase(it);
	};
	void ReplaceVert(int before, int after)
	{
		std::vector<int>::iterator it = std::find(m_AssociatedVertices.begin(), m_AssociatedVertices.end(), before); 
		*it = after;
	};
	void SetVertices(std::vector<int> vertices){m_AssociatedVertices = vertices;};
	int GetNumSides(){return m_AssociatedVertices.size();};
  double avgx(double * X);
  double avgy(double * Y);
private:
	std::vector<int> m_AssociatedVertices; //Counterclockwise
	int m_index;	
	double m_target_area;
	double m_target_perimeter;
	double m_gamma; // We can change this later.
};
#endif
