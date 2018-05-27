#include <vector>
#include <iostream>
#include <ctime>
#include <random>
#include <omp.h>
#include <fstream>

#include "Functions.h"

bool movement(Molecule& mol, Field& field);
void direction(Molecule& mol, Field& field);
void collect_data(Field& field);
void smart_data_collect(Field& field);

int main()
{
	Field field(10, 10, 20);

	Molecule mol(field);
	
	size_t i = 0;

	for (int j = 0; j < 100; ++j)
	{
		std::cout << std::endl << j << ") ";
		do
		{
			i = movement(mol, field);

		} while (i == 0);

		i = 0;
		mol.mol_generator(field);

	}
	
	
	//collect_data(field1);
	smart_data_collect(field);
	system("show.py");
	system("pause");
}



//Перевод из data в smart_data
void transcription(Field& field)
{
	std::ifstream file("crystal.txt", std::ios::in);

	size_t x, y, z;
	double p;

	for (size_t i = 0; i < 4000; ++i)
	{
		std::cout << i;
		file >> x >> y >> z >> p;

		field[x][y][z][0] = 1;
		field[x][y][z][1] = p;

	}
	file.close();

	smart_data_collect(field);
}


/*
int main()
{
	Field field(10, 10, 20);

	field.lj_potencial();

	system("pause");

}*/