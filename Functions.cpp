#include <vector>
#include <iostream>
#include <algorithm>

#include "Functions.h"

//*****





//*****Фунции движения********************************************************************
size_t direction(Molecule mol, Field field)
{

	std::vector<std::vector<double&>> pot;

	//Sluchay, kogda chastitsa peremeshaetsa strogo po Oy
	if (mol.dir[0] == 0)
	{
		//TO, kuda budet dvigatsa chasstitsa
		if (mol.dir[1] ==  1) size_t y = y.mol + 1;
		if (mol.dir[1] == -1) size_t y = y.mol - 1;

		(pot[0][0], pot[0][1], pot[2], pot[0][3]) = (field[mol.x][mol.y][mol.z][1], mol.x, mol.y, mol.z);

		for (size_t x = mol.x - 1; x <= mol.x + 1; ++x)
			for (size_t z = mol.z - 1, size_t i = 1; z <= mol.z + 1; ++z, ++i )
			{
				if (field[x][y][z][0] == 1) continue; //В простейшем случае свободная частица не может выбить уже закреплённую
				(pot[i][0], pot[i][1], pot[i][2], pot[i][3]) = (field[x][y][z][1], x, y, z);
			}
	}

	//Sluchay, kogda chastitsa peremeshaetsa srtogo po Ox
	if (mol.dir[0] != 0 && mol.dir[1] != 1)
	{
		if (mol.dir[1] ==  1) size_t x = x.mol + 1;
		if (mol.dir[1] == -1) size_t x = x.mol - 1;

		(pot[0][0], pot[0][1], pot[2], pot[0][3]) = (field[mol.x][mol.y][mol.z][1], mol.x, mol.y, mol.z);

		for (size_t y = mol.x; y <= mol.y + 1; ++y)
			for (size_t z = mol.z - 1, size_t i = 1; z <= mol.z + 1; ++z, ++i )
			{
				if (field[x][y][z][0] == 1) continue; //В простейшем случае свободная частица не может выбить уже закреплённую
				(pot[i][0], pot[i][1], pot[i][2], pot[i][3]) = (field[x][y][z][1], x, y, z);
			}
	}

	//Sluchay, kogda chastitsa dvigaetsa po diagonali
	else
	{
		return 0;
	}

	//Сортировка (Допустим, что после неё вектор будет отсортирован по мервым элементам)
	std::sort(pot.rbegin(), pot.rend());
	//Выбор направления
	r = (rand() % 100) / 100;
	//Массив с вероятностями
	double p[pot.size() + 1];

	for (i = 0; i <= pot.size(); p[pot.size()] += pot[i], ++i);
	for (i = 0; i<=pot.size();p[i] = pot[i] / p[pot.size()], ++i)
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
