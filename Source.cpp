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
void advanced_movement(Field& field, size_t mol_quantity);

int main()
{
	size_t x, y, z;
	size_t num;

	char val;


	/*
	std::cout << "Do you wand to change field size?(Y, N) (default = {x: 10, y: 10, z: 10})" << std::endl;
	std::cin >> val;
	std::cout << "Enter values(x, y, z)" << std::endl;
	switch (val)
	{
		case 'Y': 
			std::cin >> x >> y >> z;
			val = 'NULL';
			break;

		case 'N':
			x = 10;
			y = 10;
			z = 10;
			val = 'NULL';
			break;

		default:
			std::cout << "You've entered wrong value, setting up defaults!!!" << std::endl;
			x = 10;
			y = 10;
			z = 10;
			val = 'NULL';
			break;
	}
	std::cout << "Choose molecules quantity" << std::endl;
	std::cin >> val;
	try
	{
		num = size_t(val);
	}
	catch (...)
	{
		std::cout << "You've entered wrong value, setting up defaults!!! num = 500" << std::endl;
		num = 500;
	}

	Field field(x, y, z);
	Molecule mol(field);

	size_t i = 0;
	std::cout << "Choose mode: 1) Simulation; 2)'Min-pot crystal' " << std::endl;
	std::cin >> val;
	switch (val)
	{
		case '1':
			for (int j = 0; j < 500; ++j)
			{
				std::cout << std::endl << j << ") ";
				do
				{
					i = movement(mol, field);

				} while (i == 0);

				i = 0;
				mol.mol_generator(field);

			}
			break;

		case '2':	
			advanced_movement(field, num);
			break;

		default: 
			std::cout << "You've entered wrong value, running Min-pot" << std::endl;
			advanced_movement(field, num);
			break;

	}

	*/

	size_t i = 0;
	for (int j = 0; j < 500; ++j)
	{
		std::cout << std::endl << j << ") ";
		do
		{
			i = movement(mol, field);

		} while (i == 0);

		i = 0;
		mol.mol_generator(field);

	}

	//advanced_movement(field, 500);


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