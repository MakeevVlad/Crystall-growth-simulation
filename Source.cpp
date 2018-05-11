#include <vector>
#include <iostream>
#include <ctime>
#include <random>
#include "Functions.h"

void movement(Molecule& mol, Field& field);
void direction(Molecule& mol, Field& field);

int main()
{


	size_t x, y;
	double z = 0;
	x = 10;
	y = 10;
	std::srand(std::time(nullptr));

	
	double ch;

	
	Field field(10, 10, 10);
	
	ch = field.get_size_x();
	Molecule mol(field);
	//field.potencial() ;
	field.potencial();

	movement(mol, field);
	
	return 0;
}

