#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "Functions.h"


void v_zero_sort( std::vector<std::vector<double&>>& pot)
{

	std::vector<std::vector<double&>> ad;
	for (int i =0; i<= pot.size();++1)
		ad[i] = {pot[0], pot[1], pot[2], pot[3]};
}


//*****Фунции движения********************************************************************

//Эта функция задает точку присоединения, если энергия частицы достаточно мала
size_t direction(Molecule mol, Field field)
{

	std::vector<std::vector<double&>> pot;

	//Случай, когда частица перемещается строго по  Oy
	if (mol.dir[0] == 0)
	{
		//To, kuda budet dvigatsa chasstitsa
		if (mol.dir[1] ==  1) size_t y = y.mol + 1;
		if (mol.dir[1] == -1) size_t y = y.mol - 1;

		pot[0][0] = field[mol.x][mol.y][mol.z][1];
		pot[0][1] = mol.x;
		pot[0][2] = mol.y;
		pot[0][3] = mol.z;


		for (size_t x = mol.x - 1; x <= mol.x + 1; ++x)
			for (size_t z = mol.z - 1, size_t i = 1; z <= mol.z + 1; ++z, ++i )
			{
				if (field[x][y][z][0] == 1) continue; //В простейшем случае свободная частица не может выбить уже закреплённую
				(pot[i][0], pot[i][1], pot[i][2], pot[i][3]) = (field[x][y][z][1], x, y, z);
			}
	}

	//Случай, когда частица перемещается строго по Ox
	if (mol.dir[0] != 0 && mol.dir[1] != 1)
	{
		if (mol.dir[1] ==  1) size_t x = x.mol + 1;
		if (mol.dir[1] == -1) size_t x = x.mol - 1;

		pot[0][0] = field[mol.x][mol.y][mol.z][1];
		pot[0][1] = mol.x;
		pot[0][2] = mol.y;
		pot[0][3] = mol.z;

		for (size_t y = mol.x; y <= mol.y + 1; ++y)
			for (size_t z = mol.z - 1, size_t i = 1; z <= mol.z + 1; ++z, ++i )
			{
				if (field[x][y][z][0] == 1) continue; //В простейшем случае свободная частица не может выбить уже закреплённую
				(pot[i][0], pot[i][1], pot[i][2], pot[i][3]) = (field[x][y][z][1], x, y, z);
			}
	}

	//Случай, когда частица двигается по диагонали
	else
	{
		return 0;
	}

	//Сортировка (Допустим, что после неё вектор будет отсортирован по мервым элементам)
	v_zero_sort(pot)
	//Выбор направления
	p_r = (rand() % 100) / 100;
	//Массив с вероятностями
	double p[pot.size() + 1];

	for (i = 0; i <= pot.size(); p[pot.size()] += pot[i], ++i);
	for (i = 0; i<=pot.size();p[i] = pot[i] / p[pot.size()], ++i);

	if( p_r >= p[0])
	{
		field[pot[1]][pot[2]][pot[3]][0] = 1;
		return 1;
	}

	if( p_r <= p[p.size() - 1])
	{
		field[pot[1]][pot[2]][pot[3]][0] = 1;
		return 1;
	}

	else
		for (size_t j = 1; j <= p.size(); ++j)
			if ( r< p[i] && p >= p[i+1])
			{
				field[pot[1]][pot[2]][pot[3]][0] = 1;
				return 1;
			}

}

//Эта функция отвечает за перемещение частицы
size_t movement(Molecule mol, Field field)
{
	//Energiya, obladaya kotoroy chastitsa uzhe ne mozhet svobodno dvigatsa
	double crit_energy;
	//Proverka na vozmozhnost dvizheniya
	if (mol.energy - mol.delta_en <= crit_energy)
	{
		direction(mol, field);
	}
	//Случаи, когда частица двигается вдоль осей:
	//Ox
	if (abs(mol.dir[0]) != 1 && abs(mol.dir[1]) != 1)
	{
		if (field[mol.dir[0] == 1?(mol.x + 1):(mol.x -1)][mol.dir[1] == 1 ? (mol.y + 1) : (mol.y - 1)][mol.z][0] == 0) //Если последующая ячейка свободна
		{
			//Случай, когда переход переходит строго вперёд
			if (field[mol.dir[0] == 1 ? (mol.x + 1) : (mol.x - 1)][mol.dir[1] == 1 ? (mol.y + 1) : (mol.y - 1)][mol.z - 1][0])
			{
				mol.En_loss();
				mol.dir[0] == 1 ? ++mol.x : --mol.x;
				return 1;
			}
		//Здесь нужно добавить случай, когда есть возможность перехода на уровень вниз
		}
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
