#include <vector>
#include <iostream>
#include <algorithm>

#include "Functions.h"



size_t direction(Molecule mol, Field field)
{
	size_t i =0;
	std::vector<std::vector<double&>> pot;
	for (size_t x = mol.x - 1; x <= mol.x + 1; ++x)
		for (size_t y = mol.y -1; y <= mol.y + 1; ++y)
			for (size_t z = mol.z; z <= mol.z + 1; ++z )
		{
		 	if (field[x][y][z][0] == 1) continue; //В простейшем случае свободная частица не может выбить уже закреплённую

			pot[i][0] = field[x][y][z][1];
			pot[i][1] = x;
			pot[i][2] = y;
			pot[i][3] = z;
		}
	//Сортировка (Допустим, что после неё вектор будет отсортирован по мервым элементам)
	std::sort(pot.rbegin(), pot.rend());


	//Выбор направления
	r = (rand() % 100) / 100;

	//Массив с вероятностями
	double p[pot.size()];
	double c = 0;
	for (i = 0; i <= pot.size(); ++i) c += pot[i];

	for (i = 0; i<=pot.size(); ++i)
	{
		p[i] = pot[i] / c;
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

Molecule::Molecule(Field field)
{
	x = rand() % field.size[0];
	y = rand() % field.size[1];
	z = 0;
}

void Molecule::En_loss()
{
	energy -= delta_en;
}
