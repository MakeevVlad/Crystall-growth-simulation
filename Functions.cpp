#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "Functions.h"



//*****Фунции движения********************************************************************


//Эта функция задает точку присоединения, если энергия частицы достаточно мала
void direction(Molecule& mol, Field& field)
{

	std::vector<std::vector<double>> pot;

	//Случай, когда частица перемещается строго по  Oy (тут происходит заполнение массива потенциалов потенциалом :))
	if (mol.dir[0] == 0)
	{
		//Направления движения частицы
		if (mol.dir[1] ==  1) size_t y = mol.y + 1;
		if (mol.dir[1] == -1) size_t y = mol.y - 1;

		pot[0][0] = field[mol.x][mol.y][mol.z][1];
		pot[0][1] = mol.x;
		pot[0][2] = mol.y;
		pot[0][3] = mol.z;

		size_t y = mol.y;
		size_t i = 1;
		for (size_t x = mol.x - 1; x <= mol.x + 1; ++x)
			for (size_t z = mol.z - 1; z <= mol.z + 1; ++z, ++i )
			{
				if (field[x][y][z][0] == 1) continue; //В простейшем случае свободная частица не может выбить уже закреплённую
				pot[i][0] = field[x][y][z][1];
				pot[i][1] = x;
				pot[i][2] = y;
				pot[i][3] = z;
			}
	}

	//Случай, когда частица перемещается строго по Ox (тут происходит заполнение массива потенциалов потенциалом :) )
	if (mol.dir[0] != 0 && mol.dir[1] != 1)
	{
		//Направления движения частицы
		if (mol.dir[1] ==  1) size_t x = mol.x + 1;
		if (mol.dir[1] == -1) size_t x = mol.x - 1;

		pot[0][0] = field[mol.x][mol.y][mol.z][1];
		pot[0][1] = mol.x;
		pot[0][2] = mol.y;
		pot[0][3] = mol.z;

		size_t i = 1;
		size_t x = mol.x;
		for (size_t y = mol.y - 1; y <= mol.y + 1; ++y)
			for (size_t z = mol.z - 1; z <= mol.z + 1; ++z, ++i )
			{
				if (field[x][y][z][0] == 1) continue; //В простейшем случае свободная частица не может выбить уже закреплённую
				pot[i][0] = field[x][y][z][1];
				pot[i][1] = x;
				pot[i][2] = y;
				pot[i][3] = z;
			}
	}

	//Случай, когда частица двигается по диагонали
	else
	{
		size_t no;
	}


	//Выбор направления
	double p_r;
	p_r = (rand() % 100) / 100;
	size_t pot_size = sizeof(pot);
	//Массив с вероятностями
	std::vector<double> p(pot_size + 1);
	

	for (size_t i = 0; i <= pot.size(); p[pot.size()] += pot[i][0], ++i);
	for (size_t i = 0; i<=pot.size();p[i] = pot[i][0] / p[pot.size()], ++i);

	if( p_r >= p[0])
	{
		field[ size_t(pot[0][1]) ][size_t(pot[0][2])][size_t(pot[0][3])][0] = 1;
	}

	if( p_r <= p[p.size() - 1])
	{
		field[size_t(pot[0][1])][size_t(pot[0][2])][size_t(pot[0][3])][0] = 1;
	}

	else
		for (size_t j = 1; j <= sizeof(p); ++j)
			if ( p_r< p[j] && p[j] >= p[j+1])
			{
				field[size_t(pot[j][1]) ][size_t(pot[j][2]) ][ size_t(pot[j][3]) ][0] = 1;

			}

}

//Эта функция отвечает за перемещение частицы
void movement(Molecule& mol, Field& field)
{	
	//Для случая, если частица совершает первое перемещение
	field[mol.x][mol.y][mol.z][0] = 1;

	//Энергия, обладая которой частица не может свободно двигаться
	double crit_energy = 100;
	//Проверка на возможность движения
	if (mol.energy - mol.delta_en <= crit_energy)
	{
		direction(mol, field);
	}
	//Случай, когда частица двигается вдоль осей:
	if ((abs(mol.dir[0]) == 1 && mol.dir[1] == 0) || (abs(mol.dir[1]) == 1 && mol.dir[0] == 0))
	{
		if (field[mol.dir[0] == 1?(mol.x + 1):(mol.x -1)][mol.dir[1] == 1 ? (mol.y + 1) : (mol.y - 1)][mol.z][0] == 0) //Если последующая ячейка свободна
		{
			//Случай, когда частица переходит строго вперёд
			if (field[mol.dir[0] == 1 ? (mol.x + 1) : (mol.x - 1)][mol.dir[1] == 1 ? (mol.y + 1) : (mol.y - 1)][mol.z - 1][0] == 1)
			{
				mol.En_loss();
				mol.dir[0] == 1 ? ++mol.x : --mol.x;
				field[mol.x][mol.y][mol.z][0] = 1;
			}
		//Здесь нужно добавить случай, когда есть возможность перехода на уровень вниз
		}
	}
}

void cooxial_movement()
{

}

//*****Функции класса Field***************************************************************

Field::Field() { set_size(100, 100, 100); }
Field::Field(size_t len, size_t wid, size_t hei)
{
	size[0] = len; //x_size
	size[1] = wid; //y_size
	size[2] = hei; //z_size

	 //Инициализация поля НУЖНО ВЫНЕСТИ КАК ОТДЕЛЬНУЮ ФУНКЦИЮ!!!!!!!!
	zone.resize(len);
	size_t i = 0;
	for (auto& y : zone) {
		y.resize(wid);
		for (auto& z : y) {
			z.resize(hei);
			i = 0;
			for (auto& val : z) {
				val.resize(2);
				if (i == 0) val[0] = 1;
				++i;
			}
		}
	}
}

void Field::set_size(size_t len, size_t wid, size_t hei)
{
	size[0] = len; //x_size
	size[1] = wid; //y_size
	size[2] = hei; //z_size

	//Инициализация поля НУЖНО ВЫНЕСТИ КАК ОТДЕЛЬНУЮ ФУНКЦИЮ!!!!!!!!
	zone.resize(len);
	for(auto& y : zone) {
		y.resize(wid);
		for (auto& z : y) {
			z.resize(hei);
			for (auto& val : z) {
				val.resize(2);
			}
		}
	}
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

//template <typename T>

void Field::potencial()
{
	for (auto& y : zone)
		for (auto& z : y)
			for (auto& val : z)
				val[1] += 1000;

}


/*
const std::vector<double>& Field::operator[](size_t i)
const {
	return zone[i];
}
*/

//*****Функции класса Molecule***********************************************************

Molecule::Molecule(Field field)
{

	x = rand() % field.size[0];
	y = rand() % field.size[1];
	z = 1;

	dir[0] = 1;
	dir[1] = 0;

	//energy = rand() % 1000;
	energy = 1000;

}

Molecule::Molecule(size_t _x, size_t _y, size_t _z)
{
	x = _x;
	y = _y;
	z = _z;
}
void Molecule::En_loss()
{
	energy -= delta_en;
}
