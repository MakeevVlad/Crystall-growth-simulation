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
void set_configuration(Field& field, Molecule& mol);
size_t set_mode();

int main()
{

	std::cout << "============================================================\n" <<
				 "You are welcomed by Crystal growth simulator\n" <<
				 "made by MCL 1303 student (C)Makeev Vladislav for summer exam\n" <<
				 "============================================================\n";

	size_t x, y, z;
	size_t num;
	char var;

	std::cout << "--------------------------\n" <<
				 "Enter molecules quantity: ";
	std::cin >> num;
	std::cout << "\n-----------------------------" <<
				 "\nEnter field size (x, y, z): ";
	std::cin >> x >> y >> z;

	Field field(x, y, z);
	Molecule mol(field);

	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "Default simulation configuration: \n\nsigma = " << field.sigma << " e = " << field.e << std::endl;
	std::cout << "ALONG_EN = " << mol.ALONG_EN << " ASCENT_EN = " << mol.ASCENT_EN
		<< " FALLING_EN = " << mol.FALLING_EN << "\nMAX_ENERGY = "
		<< mol.MAX_ENERGY << " CRIT_EN = " << mol.CRIT_EN << std::endl;
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "Do you want to change simulation configuration? y, n\n";
	std::cin >> var;
	switch (var)
	{
	case 'y':
		set_configuration(field, mol);
		std::cout << "\n-------------------------------------------------------------\n";
		std::cout << "New configuration: \n\nsigma = " << field.sigma << " e = " << field.e << std::endl;
		std::cout << "ALONG_EN = " << mol.ALONG_EN << " ASCENT_EN = " << mol.ASCENT_EN
			<< " FALLING_EN = " << mol.FALLING_EN << "\nMAX_ENERGY = "
			<< mol.MAX_ENERGY << " CRIT_EN = " << mol.CRIT_EN << std::endl;
		std::cout << "\n-------------------------------------------------------------\n";
		break;
	}


	size_t val = set_mode();
	size_t i = 0;
	switch (val)
	{
	case 1:
		for (int j = 0; j < num; ++j)
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

	case 2:
		advanced_movement(field, num);
		break;
	}


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