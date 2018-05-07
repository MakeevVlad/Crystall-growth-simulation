#include <vector>
#include <iostream>
#include "Functions.h"


int main()
{


	size_t x, y, z;
	x = 10;
	y = 10;
	z = 0;

	size_t ch;

	Field field(100, 100, 100);
	
	ch = field.get_size_x();
	Molecule mol(field);
	//field.potencial();

	return 0;
}
