COMP = g++ -std=c++11

all: NagaiHondaModel

NagaiHondaModel: NagaiHondaModel.o NagaiHondaForce.o cell.o Topology.o
	$(COMP)  NagaiHondaModel.o NagaiHondaForce.o cell.o Topology.o -o NagaiHondaModel

NagaiHondaModel.o: NagaiHondaModel.cpp cell.hpp NagaiHondaForce.hpp coordinate.hpp printCellProperties.hpp EnergyCalculator.hpp
	$(COMP) -c NagaiHondaModel.cpp

Topology.o: Topology.cpp Topology.hpp
	$(COMP) -c Topology.cpp

NagaiHondaForce.o: NagaiHondaForce.cpp NagaiHondaForce.hpp cell.hpp coordinate.hpp Get_Assoc_Cells.hpp AreaForce.hpp PerAdhForce.hpp
	$(COMP) -c NagaiHondaForce.cpp

cell.o: cell.cpp cell.hpp coordinate.hpp
	$(COMP) -c cell.cpp

clean:
	rm *.o Images/*.off Images/*.rgb

