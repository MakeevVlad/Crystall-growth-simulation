#include <vector>
#include <iostream>
#include "Functions.h"

void coord(size_t x, size_t y)
{
	x = rand() % 100 + 1;
	y = rand() % 100 + 1;
}

size_t direction(Molecule mol, Field field)
{
	for (size_t x = mol.x - 1; x <= mol.x + 1; ++x)
		for (size_t y = mol.y -1; y <= mol.y + 1; ++y)
			for (size_t z = mol.z; z <= mol.z + 1; ++z )
		{
			if (x == mol.x && y == mol.y && z == mol.z) continue;
			if (field[x][y][z][0] == 1) continue; //В простейшем случае свободная частица не может выбить уже закреплённую

			//Поиск минимальной энергии взаимодействия 
			double pot = 0;
			
		}
}


//*****Функции класса Field***************************************************************

Field::Field() { set_size(100, 100, 100); }
Field::Field(size_t len, size_t wid, size_t hei)
{
	set_size(len, wid, hei);

}

void Field::set_size(size_t len, size_t wid, size_t hei)
{
	size[0] = len; //x_size
	size[1] = wid; //y_size
	size[2] = hei; //z_size
}

size_t& Field::get_size_x() 
{ return size[0]; }
size_t& Field::get_size_y() 
{ return size[1]; }
size_t& Field::get_size_z() 
{ return size[2]; }

std::vector<std::vector<std::vector<double>>>& Field::operator[](size_t i)
{
	return zone[i];
}

//*****Функции класса Molecule***********************************************************

