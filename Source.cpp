#include <vector>
#include <iostream>
#include <ctime>
#include <random>
#include "Functions.h"

bool movement(Molecule& mol, Field& field);
void direction(Molecule& mol, Field& field);
void collect_data(Field& field);


int main()
{



	Field field(10, 10, 10);

	
	Molecule mol[121] = { Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
						Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
						Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
						Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
						Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
						Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
						Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
						Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
		Molecule(field), Molecule(field), Molecule(field), Molecule(field), Molecule(field),
						Molecule(field)};
 
	
	Field field1(10, 10, 10);

	size_t i = 0;

	for (int j = 0; j < 121 ; ++j)
	{
		do
		{
			i = movement(mol[j], field1);

		} while (i == 0);

		i = 0;


	}


	collect_data(field1);
	system("show.py");
	system("pause");
}

