#include <iostream>
#include <vector>
#include <omp.h>
#include "Functions.h"
#include <fstream>

//В кристалле ищется свободная точка с наименьшим потенциалом
std::vector<double>& Field::min()
{
  
  std::vector<double> min;
  min.resize(4);
  //min[0] = x; min[1] = y; min[2] = z; min[3] = pot
 #pragma omp parallel for
  for (double x = 0; x < size[0]; ++x)
		for (double y = 0; y < size[1]; ++y)
			for (double z = 1; z < size[2]; ++z)
				if (zone[x][y][z][0] == 0 && zone[x][y][z][1] < min[4] )
        {
          min[0] = x; 
          min[1] = y;
          min[2] = z;
          min[3] = zone[x][y][z][1];
        }

  return min;
}

void advanced_movement(Field& field, size_t mol_quantity)
{
  
  std::ofstream file("mol_paths_adv.txt", std::ios::in)
  std::vector<double> min;
  min.resize(4)
  for (size_t mol = 0; mol< mol_quantity; ++mol)
  {
    field.lj_potencial();
    min = field.min();
    
    field[min[0]][min[1]][min[2]][0] = 1;
    
    file << i << min[0] << min[1] << min[2] << min[3];
  }
}
               
       
