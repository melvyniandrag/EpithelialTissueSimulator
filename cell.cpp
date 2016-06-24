#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include "cell.hpp"
#include <iostream>
#include "coordinate.hpp"

double cell::avgx(double * X)
{
  double avg = 0;
  for(auto v : m_AssociatedVertices)
	avg += X[v]; 
  return avg/m_AssociatedVertices.size();
}


double cell::avgy(double * Y)
{
  double avg = 0;
  for(auto v : m_AssociatedVertices)
	avg += Y[v]; 
  return avg/m_AssociatedVertices.size();
}

void cell::InsertVert(int v1, int v2)
{
  //std::cout << "Before InsertVert" << std::endl;
  //for (auto c : m_AssociatedVertices)
	//std::cout << c << std::endl;
  //  std::cout << "v1, v2 " << v1 << " " << v2 << std::endl;
		std::vector<int>::iterator after_this = find(m_AssociatedVertices.begin(), m_AssociatedVertices.end(), v1);
		if(after_this != m_AssociatedVertices.end())
			m_AssociatedVertices.insert(next(after_this), v2);
		else if(after_this == m_AssociatedVertices.end())
		  m_AssociatedVertices.insert(next(after_this), v2);
		else
			std::cout << "Nowhere to insert vertex. See InsertVert()" << std::endl;
		//for (auto c : m_AssociatedVertices)
	//std::cout << c << std::endl;


		//		std::cout << "After InsertVert" << std::endl;

}

double cell::ComputePerimeter(double * X, double * Y)
{	
	/*
	 * This function will take in the list of coordinates and 
	 * calculate the perimeter of the cell using them.
	 */
	double perimeter = 0.0;
	unsigned int n = m_AssociatedVertices.size();
	for(unsigned int edge = 0; edge < n - 1; edge++)
	{
		perimeter += std::abs(std::sqrt(std::pow((X[m_AssociatedVertices[edge]] - X[m_AssociatedVertices[edge + 1]]),2)
						    + std::pow((Y[m_AssociatedVertices[edge]] - Y[m_AssociatedVertices[edge + 1]]),2)));
	}
	perimeter += std::abs(std::sqrt(std::pow((X[m_AssociatedVertices[n-1]] - X[m_AssociatedVertices[0]]),2)
						+ std::pow((Y[m_AssociatedVertices[n-1]]- Y[m_AssociatedVertices[0]]),2)));
	return perimeter;
}

double cell::ComputeArea(double * X, double * Y)
{
	/* 
	 * This function will use the cells list of associated vertices and 
	 * the list of coordinates to calculate the area inside the polygon.
	*/
	double area = 0.0;
	unsigned int n = m_AssociatedVertices.size();
	for(unsigned int i = 0; i < n - 1; i++)
	{
		area += X[m_AssociatedVertices[i]]*Y[m_AssociatedVertices[i+1]] 
			  - X[m_AssociatedVertices[i+1]]*Y[m_AssociatedVertices[i]];
	}
	area += X[m_AssociatedVertices[n-1]]*Y[m_AssociatedVertices[0]]
	      - X[m_AssociatedVertices[0]]*Y[m_AssociatedVertices[n-1]]; 
	return std::abs(0.5*area);
}	

void cell::PrintCell(std::ofstream &OffFile)
{
	/*
	 * This function will print out the coordinates in whatever format I choose.
	 * For now we will use geomview.
	 */
	 OffFile << m_AssociatedVertices.size() << " ";
	 for(unsigned int vert = 0; vert < m_AssociatedVertices.size(); vert++)
	 {
		 OffFile << m_AssociatedVertices[vert] << " ";
	 }
     if ((this->GetGamma() != 0.5) && (this->GetTargetArea() != 3.0))
        OffFile << "\t1 0 0 0" << std::endl;
     else if (this->GetTargetArea() != 3.0)
        OffFile << "\t0 1 0 0" << std::endl;   
     else if (this->GetGamma() != 0.5)
        OffFile << "\t0 0 1 0" << std::endl;
     else
        OffFile << std::endl;
}

int cell::ContainsVertex(int index)
{
	for (unsigned int vert = 0; vert < m_AssociatedVertices.size(); vert++)
	{
		if(index == m_AssociatedVertices[vert]) return 1;
	}
	return 0;
}
