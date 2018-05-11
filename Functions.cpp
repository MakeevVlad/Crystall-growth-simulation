#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <random>

#include "Functions.h"


//Для сортировки по первым элементам
bool cmp(double* x, double* y)
{
	return *x > *y;
}



//*****Фунции движения********************************************************************


//Эта функция задает точку присоединения, если энергия частицы достаточно мала
void direction(Molecule& mol, Field& field)
{

	std::vector<std::vector<double>> pot;

	//Инициализация вектора pot
	pot.resize(10);
	for (auto& c : pot)
		c.resize(4);

	//Случай, когда частица перемещается строго по  Oy (тут происходит заполнение массива потенциалов потенциалом :))
	if (mol.dir[0] == 0)
	{
		//Направления движения частицы(выбор следующей клетки)
		size_t y = mol.y + mol.dir[0];

		//Pot[0] - описание точки, на которой находится частица 
		pot[0][0] = field[mol.x][mol.y][mol.z][1];
		pot[0][1] = mol.x;
		pot[0][2] = mol.y;
		pot[0][3] = mol.z;

		size_t i = 1;
		for (size_t x = mol.x - 1; x <= mol.x + 1; ++x)
			for (size_t z = mol.z - 1; z <= mol.z + 1; ++z, ++i )
			{
				if (field[x][y][z][0] == 1) //В простейшем случае свободная частица не может выбить уже закреплённую
				{
					pot[i][0] = 10000;
					pot[i][1] = x;
					pot[i][2] = y;
					pot[i][3] = z;

					continue;
				}
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


	size_t pot_size = pot.size();
	//Массив с вероятностями
	std::vector<double> p(pot_size + 1);

	//Для поиска максимумального и минимального потенциалов
	double pot_max[2] { 0, 0 };//0 - значение, 1 - номер
	double pot_min[2] { 1, 0 };//0 - значение, 1 - номер


	//Эти два цикла отвечают за создание вектора вероятностей НЕприсоединения; если pot = 1000, то ничего не присоединиться
	for (size_t i = 0; i < pot.size();++i)
	{
		if (pot[i][0] == 10000) continue;
		p[pot.size()] += pot[i][0];
		
	}
	for (size_t i = 0; i < pot.size(); ++i)
	{	
		if (pot[i][0] == 10000) { p[i] = 1; continue; }
		//Вероятность присоединения в соответствующую точку 
		p[i] = 1 - ( pot[i][0] / p[pot.size()] );

		//Поиск максимальной вероятности
		if (p[i] > pot_max[0] && p[i] != 1) { pot_max[0] = p[i]; pot_max[1] = i; };
		//Поиск минимальной вероятности
		if (p[i] < pot_min[0]) { pot_min[0] = p[i]; pot_min[1] = i; };
	}
	

	//Выбор направления
	double p_r;
	std::srand(std::time(0));
	p_r = std::rand() % size_t((pot_max[0] - pot_min[0] + 0.01) * 1000) + pot_min[0] * 1000 - 1;
	p_r /= 1000;

	if (p_r >= pot_max[0])
	{
		field[size_t(pot[pot_max[1]][1])][size_t(pot[pot_max[1]][2])][size_t(pot[pot_max[1]][3])][0] = 1;
	}

	if (p_r <= pot_min[0])
	{
		field[size_t(pot[pot_min[1]][1])][size_t(pot[pot_min[1]][2])][size_t(pot[pot_min[1]][3])][0] = 1;
	}

	else
		for (size_t j = 1; j <= p.size(); ++j)
			if (p_r < p[j] && p_r >= p[j + 1])
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
	if (mol.energy - mol.DELTA_EN <= crit_energy)
	{
		direction(mol, field);
	}
	//Случай, когда частица двигается вдоль осей:
	if ((abs(mol.dir[0]) == 1 && mol.dir[1] == 0) || (abs(mol.dir[1]) == 1 && mol.dir[0] == 0))
	{

		if (field[mol.x + mol.dir[0]][mol.y + mol.dir[1]][mol.z][0] == 0) //Если последующая ячейка свободна
		{
			//Случай, когда частица переходит строго вперёд
			if (field[mol.x + mol.dir[0]][mol.y + mol.dir[1]][mol.z - 1][0] == 1)
			{
				mol.En_loss();
				field[mol.x][mol.y][mol.z][0] = 0; //Убираем молекулу из нвчальной координаты
				mol.x += mol.dir[0];
				mol.y += mol.dir[1];
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
	//Генератор случайных чисел(uniform distribution)
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> uniform(0, 1000);

	std::srand(std::time(nullptr));
	for (auto& y : zone)
		for (auto& z : y)
			for (auto& val : z)
				val[1] += uniform(gen);

}

/*
const std::vector<double>& Field::operator[](size_t i)
const {
	return zone[i];
}*/


//*****Функции класса Molecule***********************************************************
/*
Molecule::Molecule(Field field)
{	
	std::srand(std::time(nullptr));
	
	x = rand() % field.size[0];
	y = rand() % field.size[1];
	z = 1;

	int variants[3] = { 0, 1, -1 };

	//Выбор направления движения******Условный оператор нужен, чтобы нельзя было выбрать dir = {0, 0}
	dir[0] = variants[rand() % 3];
	dir[0] == 0 ? dir[1] = variants[rand() % 2 + 1] : dir[1] = variants[rand() % 3];

	//Генератор случайных чисел(uniform distribution)
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> uniform(0, MAX_ENERGY);

	energy = uniform(gen);
	

}
*/
Molecule::Molecule(Field field)
{
	x = 5; //Для отладки
	y = 5;
	z = 1;


	dir[0] = 0;
	dir[1] = 1;

	energy = 150;



}

void Molecule::En_loss()
{
	energy -= DELTA_EN;
}
