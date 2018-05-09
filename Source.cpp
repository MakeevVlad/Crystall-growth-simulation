#include <vector>
#include <iostream>
#include "Functions.h"

void movement(Molecule& mol, Field& field);
void direction(Molecule& mol, Field& field);

int main()
{


	size_t x, y, z;
	x = 10;
	y = 10;
	z = 0;

	double ch;

	Field field(10, 10, 10);
	
	ch = field.get_size_x();
	Molecule mol(field);
	//field.potencial() ;

	movement(mol, field);
	
	return 0;
}

