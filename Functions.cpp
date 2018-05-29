#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <random>
#include <fstream>
#include <omp.h>

#include "Functions.h"


void collect_data(Field& field)
{
	std::ofstream file("crystal.txt", std::ios::out);

	for (double x = 0; x < field.size[0]; ++x)
		for (double y = 0; y < field.size[1]; ++y)
			for (double z = 1; z < field.size[2]; ++z)
				if (field[x][y][z][0] == 1)
					file << x << " " << y << " " << z << " " << field[x][y][z][1] << std::endl;
	file.close();
}
//В файл записывается только координата верхней молекулы
void smart_data_collect(Field& field)
{
	std::ofstream file("crystal.txt", std::ios::out);

	for (double x = -1; x <= field.size[0]; ++x)
		for (double y = -1; y <= field.size[1]; ++y)
			for (double z = 0; z < field.size[2]; ++z)
			{
				if (x == -1 || y == -1 || x == field.size[0] || y == field.size[1])
				{
					file << x << " " << y << " " << 0 << " " << 0 << std::endl;
					continue;
				}
				if (field[x][y][z][0] == 1 && (z == field.get_size_z()? 0 :  field[x][y][z + 1][0] == 0))
					file << x << " " << y << " " << z << " " << field[x][y][z][1] << std::endl;
			}
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

	//Случай, когда частица перемещается строго по Oy (тут происходит заполнение массива потенциалов потенциалом :))
	if (mol.dir[0] == 0)
	{

		size_t x_max = field.get_size_x(), y_max = field.get_size_y();
		//Направления движения частицы(выбор следующей клетки)
		size_t y = mol.y != 0 ? (mol.y + mol.dir[1]) % y_max : (y_max + mol.dir[1]) % y_max;

		//Pot[0] - описание точки, на которой находится частица
		pot[0][0] = field[mol.x][mol.y][mol.z][1];
		pot[0][1] = mol.x;
		pot[0][2] = mol.y;
		pot[0][3] = mol.z;

		size_t i = 1;
		for (size_t x = (mol.x != 0 ? mol.x - 1 : x_max - 1); x <= (mol.x + 1) % x_max; x = (++x) % x_max)
		{
			if (i == 10) break;
			for (size_t z = mol.z - 1; z <= mol.z + 1; ++z, ++i)
			{
				if (i == 10) break; //Кастыль

				if (field[x][y][z][0] == 1 || field[x][y][z == 0 ? 0 : z - 1][0] == 0)
					//В простейшем случае свободная частица не может выбить уже закреплённую
					// Также она не может повиснуть в воздухе
				{
					pot[i][0] = 1000;
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
	}
	//Случай, когда частица перемещается строго по Ox (тут происходит заполнение массива потенциалов потенциалом :) )
	else if (mol.dir[0] != 0 && abs(mol.dir[1]) != 1)
	{

		size_t x_max = field.get_size_x(), y_max = field.get_size_y();
		//Направления движения частицы
		size_t x = mol.x != 0 ? (mol.x + mol.dir[0]) % x_max : (x_max + mol.dir[0]) % x_max;

		//Pot[0] - описание точки, на которой находится частица
		pot[0][0] = field[mol.x][mol.y][mol.z][1];
		pot[0][1] = mol.x;
		pot[0][2] = mol.y;
		pot[0][3] = mol.z;

		size_t i = 1;
		for (size_t y = (mol.y != 0 ? mol.y - 1 : y_max - 1); y <= (mol.y + 1) % y_max; y = (++y) % y_max)
		{
			if (i == 10) break;
			for (size_t z = mol.z - 1; z <= mol.z + 1; ++z, ++i)
			{
				if (i == 10) break; //Кастыль


				if (field[x][y][z][0] == 1 || field[x][y][z == 0 ? 0 : z - 1][0] == 0)
					//В простейшем случае свободная частица не может выбить уже закреплённую
					// Также она не может повиснуть в воздухе
				{
					pot[i][0] = 1000;
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
	}

	//Случай, когда частица двигается по диагонали
	else
	{
		return;
	}

	/*
	size_t pot_size = pot.size();
	//Массив с вероятностями
	std::vector<double> p(pot_size + 1);
	*/


	//Эти два цикла отвечают за создание вектора вероятностей НЕприсоединения; если pot = 1000, то ничего не присоединиться
	/*
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
	}

	

	//Внимание КАСТЫЛЬ!!! НЕОБХОДИМО БУДЕТ ИСПРАВИТЬ(тут сортируется только один вектор, а координаты впоследствии выравниваются исходя из равенства вероятностей)
	for (size_t i = 0; i < pot.size(); pot[i][0] = p[i], ++i);
	std::sort(p.begin(), p.end());

	for (size_t i = 0; i < pot.size(); ++i)
		for (size_t j = 0; j < pot.size(); ++j)
		{
			if (pot[i][0] == p[j])
				for (size_t c = 0; c <= 3; ++c)
					std::swap(pot[j][c], pot[i][c]);
		}
	//Конец костыля________УБРАТЬ!!!!! Необходимо написать функцию нормальной сортировки

	*/


	/*
	//Для поиска максимумального и минимального потенциалов
	double pot_max[2]{ 0, 0 };//0 - значение, 1 - номер
	double pot_min[2]{ 1, 0 };//0 - значение, 1 - номер

	for (size_t i = 0; i < pot.size(); ++i)
	{
		//Поиск максимальной вероятности
		if (p[i] > pot_max[0] && p[i] != 1) { pot_max[0] = p[i]; pot_max[1] = i; };
		//Поиск минимальной вероятности
		if (p[i] < pot_min[0]) { pot_min[0] = p[i]; pot_min[1] = i; };
	}
	*/

	//Для простоты модели просто выбирается минимальный потенциал
	size_t num = 10;
	double pote = 10000;
	for (size_t i = 0; i < pot.size(); ++i)
	{
		if (pote > pot[i][0])
		{
			pote = pot[i][0];
			num = i;
		}

	}

	field[mol.x][mol.y][mol.z][0] = 0;
	mol.x = size_t(pot[num][1]);
	mol.y = size_t(pot[num][2]);
	mol.z = size_t(pot[num][3]);

	if(field[mol.x][mol.y][mol.z][0] == 1) std::cout << "OOO no " << std::endl;

	field[mol.x][mol.y][mol.z][0] = 1;
	//field.lj_potencial();
	return;

	//Выбор направления
/*
	try
	{
	double p_r;
	std::random_device rd;
	std::mt19937 gen(rd());
	if (pot_max[0] == 0)
		p_r = 1000;
	else
	{
		std::uniform_real_distribution<> uniform(size_t((pot_min[0] - 0.01) * 10000), size_t((pot_max[0] + 0.01) * 10000));
		p_r = uniform(gen);
	}
	p_r /= 10000;



		if (p_r >= pot_max[0] && p_r < 1)
		{
			field[mol.x][mol.y][mol.z][0] = 0;
			//Изменение координат частицы
			mol.x = size_t(pot[pot_min[1]][1]);
			mol.y = size_t(pot[pot_min[1]][2]);
			mol.z = size_t(pot[pot_min[1]][3]);
			field[size_t(pot[pot_max[1]][1])][size_t(pot[pot_max[1]][2])][size_t(pot[pot_max[1]][3])][0] = 1;
			//Для отладки
			std::cout << pot_max[0] << " " << p_r << " 1" << std::endl;
			return;
		}

		if (p_r <= pot_min[0])
		{
			field[mol.x][mol.y][mol.z][0] = 0;
			//Изменение координат частицы
			mol.x = size_t(pot[pot_min[1]][1]);
			mol.y = size_t(pot[pot_min[1]][2]);
			mol.z = size_t(pot[pot_min[1]][3]);
			field[size_t(pot[pot_min[1]][1])][size_t(pot[pot_min[1]][2])][size_t(pot[pot_min[1]][3])][0] = 1;
			//Для отладки
			std::cout << pot_min[0] << " " << p_r << " 2" << std::endl;
			return;
		}

		else
			for (size_t j = 0; j < p.size() - 1; ++j)
				if (p_r > p[j] && p_r <= p[j + 1] && p[j] != 1)
				{
					field[mol.x][mol.y][mol.z][0] = 0;
					mol.x = size_t(pot[j + 1][1]);
					mol.y = size_t(pot[j + 1][2]);
					mol.z = size_t(pot[j + 1][3]);

					field[size_t(pot[j + 1][1])][size_t(pot[j + 1][2])][size_t(pot[j + 1][3])][0] = 1;
					//Для отладки
					std::cout << p[j + 1] << " " << p_r << " 3" << std::endl;
					return;
				}
	}
	catch (...) { return; }
	*/
}

//Функция отвечающая за отражение частицы
void movement_reflection(Molecule& mol, Field& field)
{
	mol.along();
	mol.dir[0] *= -1;
	mol.dir[1] *= -1;
}
//Функция отвечающая за восхождение :)
void movement_ascent(Molecule& mol, Field& field)
{




}
//Функция отвечающая за кооксиальное движение
void movement_along(Molecule& mol, Field& field)
{

}


//Эта функция отвечает за перемещение частицы
bool movement(Molecule& mol, Field& field)
{
	std::cout << std::endl << mol.x << " " << mol.y << " " << mol.z << " ";

	//"Падение" молекулы
	if (mol.z == field.get_size_z()) 
		while (field[mol.x][mol.y][mol.z - 1][0] == 0)
		{
			--mol.z;
			std::cout << " ->" << mol.z;
		}

	//Для случая, если частица совершает первое перемещение
	field[mol.x][mol.y][mol.z][0] = 1;


	size_t x_max = field.get_size_x(), y_max = field.get_size_y();

	//Координаты точки по направлению(вынесено, чтобы каждый раз не писать)
	size_t _X = mol.x != 0 ? (mol.x + mol.dir[0]) % x_max : (x_max + mol.dir[0]) % x_max;
	size_t _Y = mol.y != 0 ? (mol.y + mol.dir[1]) % y_max : (y_max + mol.dir[1]) % y_max;
	size_t _Z = mol.z + 1;



	//Случай, когда частица двигается вдоль осей:
	//!!! % cord_max означает, что частица при переходе за поле автоматически перемещается нв другую его сторону
	if ((abs(mol.dir[0]) == 1 && mol.dir[1] == 0) || (abs(mol.dir[1]) == 1 && mol.dir[0] == 0))
	{
		if (field[_X][_Y][mol.z][0] == 0) //Если последующая ячейка свободна
		{
			//Случай, когда частица переходит строго вперёд
			if (field[_X][_Y][mol.z - 1][0] == 1)
			{

				field.lj_potencial(mol.x, mol.y, mol.z);

				//Проверка на возможность движения
				if (mol.along_check(field, _X, _Y, mol.z) <= mol.CRIT_EN)
				{
					direction(mol, field);
					return 1;
				}

				double scnd_pot;

				//Возможность перехода наискось
				if (abs(mol.dir[0]) == 1 && (field[_X][(_Y + 1) % y_max][mol.z][0] == 0 || field[_X][_Y == 0 ? y_max - 1 : _Y - 1][mol.z][0] == 0))
				{
					if (field[_X][(_Y + 1) % y_max][mol.z][1] > field[_X][_Y == 0 ? y_max - 1 : _Y - 1][mol.z][1])
					{
						scnd_pot = field[_X][_Y == 0 ? y_max - 1 : _Y - 1][mol.z][1];
						if(field[_X][_Y][mol.z][1] > scnd_pot && field[_X][_Y == 0 ? y_max - 1 : _Y - 1][mol.z - 1][0] == 1)
							_Y = _Y == 0 ? y_max - 1 : _Y - 1;
					}
					else
					{
						scnd_pot = field[_X][(_Y + 1) % y_max][mol.z][1];
						if (field[_X][_Y][mol.z][1] > scnd_pot && field[_X][(_Y + 1) % y_max][mol.z-1][0] == 1)
							_Y = _Y == 0 ? y_max - 1 : _Y - 1;
					}
				}
				else
				{
					if (field[(_X + 1) % x_max][_Y ][mol.z][0] == 1 || field[_X == 0 ? x_max - 1 : _X - 1][_Y][mol.z][0] == 1)
					{
						if (field[(_X + 1) % x_max][_Y][mol.z][1] > field[_X == 0 ? x_max - 1 : _X - 1][_Y][mol.z][1])
						{
							scnd_pot = field[_X == 0 ? x_max - 1 : _X - 1][_Y][mol.z][1];
							if ((field[_X][_Y][mol.z][1] > scnd_pot) && field[_X == 0 ? x_max - 1 : _X - 1][_Y][mol.z - 1][0] == 1)
								_X = _X == 0 ? x_max - 1 : _X - 1;
						}
						else
						{
							scnd_pot = field[(_X + 1) % x_max][_Y][mol.z][1];
							if (field[_X][_Y][mol.z][1] > scnd_pot && field[_X == 0 ? x_max - 1 : _X - 1][_Y][mol.z - 1][0] == 1)
								_X = (_X + 1) % x_max;
						}

					}
				}



				field[mol.x][mol.y][mol.z][0] = 0; //Убираем молекулу из начальной координаты

				//mol.along();
				mol.along(field, _X, _Y, mol.z);

				//Следующие координаты
				mol.x = _X;
				mol.y = _Y;

				field[mol.x][mol.y][mol.z][0] = 1;
				return 0;
			}
			//Случай, когда частица переходит на уровень вниз
			else
			{
				//mol.falling();
				
				field[mol.x][mol.y][mol.z][0] = 0; //Убираем молекулу из начальной координаты

				int  n = 0;
				size_t z0 = mol.z;
				//"Падение" частицы(z0 для того, чтобы цикл не был бесконечным
				while (field[mol.x][mol.y][z0 - 1][0] == 0) { --z0; --n; }

				field.lj_potencial(mol.x, mol.y, mol.z, n);

				mol.falling(field, _X, _Y, mol.z + n, -1 * n);

				//Следующие координаты
				mol.x = _X;
				mol.y = _Y;
				mol.z += n;
				
				field[mol.x][mol.y][mol.z][0] = 1;
				return 0;
			}
		}
		//Если последующая ячейка занята(происходит выбор между отражением и переходом наверх)
		else
		{

			//Подсчет количества я чеек подъёма
			size_t n = 1;
			while (field[_X][_Y][_Z][0] == 1) {	++n; ++_Z;	}

			field.lj_potencial(mol.x, mol.y, mol.z, n);

			//Вероятности p_reflect - отражения and p_ascent - поднятия на уровень
			double p_reflect = 1 / (1 + mol.ALONG_EN / (n * mol.ASCENT_EN));
			double p_ascent  = 1 - p_reflect;
			
			//Генерируем вероятность
			std::srand(std::time(0));
			double p = (rand() % 1000) / 1000;

			
			if( p <= p_ascent) //Подъём
			{
				//Проверка на возможность движения
				if (mol.ascent_check(field, _X, _Y, mol.z + n, n) <= mol.CRIT_EN)
				{
					direction(mol, field);
					return 1;
				}
				
				field[mol.x][mol.y][mol.z][0] = 0; //Убираем молекулу из начальной координаты

				mol.ascent(field, _X, _Y, mol.z + n, n);

				//Следующие координаты
				mol.x = _X; 
				mol.y = _Y;

				field[mol.x][mol.y][mol.z + n][0] = 1;
				return 0;

			}
			
			else //Отражение
			{	

				//Проверка на возможность движения
				if (mol.energy - mol.ALONG_EN <= mol.CRIT_EN)
				{
					direction(mol, field);
					return 1;
				}

				movement_reflection(mol, field);
				movement(mol, field) == 1? 1 : 0;
				
			}	

		}
	}
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
	//Задание начальной карты потенциала
	//this->lj_potencial();
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




//Задание рандомных потенциалов
void Field::potencial_uniform()
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
//Задание потенциала по формуле Леннарда-Джонса
void Field::lj_potencial()
{

	const double a1 = 1;
	const double a2 = 1;

#pragma omp parallel for ordered schedule(dynamic)
	for (int x = 0; x < size[0]; ++x)
		for (double y = 0; y < size[1]; ++y)
			for (double z = 0; z < size[2]; ++z)
			{
				#pragma omp parallel for ordered schedule(dynamic)
				for (int x1 = 0; x1 < size[0]; ++x1)
					for (double y1 = 0; y1 < size[1]; ++y1)
						for (double z1 = 0; z1 < size[2]; ++z1)
						{
							if (zone[size_t(x1)][size_t(y1)][size_t(z1)][0] == 1 && (x != x1 || y != y1 || z != z1))
							{
								double r = sqrt((double(x) - double(x1))*(double(x) - double(x1)) + (y - y1)*(y - y1) + (z - z1)*(z - z1));
								zone[size_t(x)][size_t(y)][size_t(z)][1] += (a1 / pow(r, 12)) - (a2 / pow(r, 6));
							}
						}
			}
}
void Field::lj_potencial(size_t _x, size_t _y, size_t _z, int n)
{
	const double a1 = 1;
	const double a2 = 1;

	size_t x_start = _x != 0 ? _x - 1 : size[0] - 1,
		y_start = _y != 0 ? _y - 1 : size[1] - 1;
		

	size_t x_end = ((_x + 1) % size[0]) + 1,
		y_end = ((_y + 1) % size[1]) + 1;

	size_t z_start, z_end;
	if (n < 0) 
	{
		z_end = _z + 1; z_start = _z + n;
	}
	else
	{
		z_end = _z + n; z_start = _z - 1;
	}
			


#pragma omp parallel for ordered schedule(dynamic)
	for (int x = x_start; x < x_end; ++x)
		for (double y = y_start; y < y_end; ++y)
			for (double z = z_start; z < z_end; ++z)
			{
				#pragma omp parallel for ordered schedule(dynamic)
				for (int x1 = 0; x1 < size[0]; ++x1)
					for (double y1 = 0; y1 < size[1]; ++y1)
						for (double z1 = 0; z1 < size[2]; ++z1)
						{
							if (zone[size_t(x1)][size_t(y1)][size_t(z1)][0] == 1 && (x != x1 || y != y1 || z != z1))
							{
								double r = sqrt((double(x) - double(x1))*(double(x) - double(x1)) + (y - y1)*(y - y1) + (z - z1)*(z - z1));
								zone[size_t(x)][size_t(y)][size_t(z)][1] += (a1 / pow(r, 12)) - (a2 / pow(r, 6));
							}
						}
			}
}



//*****Функции класса Molecule***********************************************************

Molecule::Molecule(Field field)
{
	std::srand(std::time(nullptr));
	//Генератор случайных чисел(uniform distribution)
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> uniform_x(0, field.size[0]);
	std::uniform_real_distribution<> uniform_y(0, field.size[1]);

	x = uniform_x(gen);
	y = uniform_y(gen);
	z = field.get_size_z();



	int variants[3] = { 0, 1,-1 };

	//Выбор направления движения******Условный оператор нужен, чтобы нельзя было выбрать dir = {0, 0}
	dir[0] = variants[rand() % 3];
	dir[0] == 0 ? dir[1] = variants[rand() % 2 + 1] : dir[1] = 0;


	std::uniform_real_distribution<> uniform(0, MAX_ENERGY);
	energy = uniform(gen);

}
Molecule::Molecule(Field field, size_t _x, size_t _y, size_t _z, int dir0, int dir1)
{
	x = _x;
	y = _y;
	z = _z;

	dir[0] = dir0;
	dir[1] = dir1;

	energy = 350;
}
Molecule::~Molecule()
{
	delete &x, &y, &z, dir, &energy;
}

void Molecule::mol_generator(Field& field)
{
	std::srand(std::time(nullptr));
	//Генератор случайных чисел(uniform distribution)
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> uniform_x(0, field.size[0]);
	std::uniform_real_distribution<> uniform_y(0, field.size[1]);

	x = uniform_x(gen);
	y = uniform_y(gen);
	z = field.get_size_z();



	int variants[3] = { 0, 1,-1 };

	//Выбор направления движения******Условный оператор нужен, чтобы нельзя было выбрать dir = {0, 0}
	dir[0] = variants[rand() % 3];
	dir[0] == 0 ? dir[1] = variants[rand() % 2 + 1] : dir[1] = 0;


	std::uniform_real_distribution<> uniform(0, MAX_ENERGY);
	energy = uniform(gen);

	std::cout << std::endl << "****new molecule!**** /n energy =" << energy << std::endl;
}

//выйгрыш энергии частицой при переходе на уровень вниз
void Molecule::falling(size_t n)
{
	energy += n*FALLING_EN;
}
void Molecule::falling(Field& field, size_t _x, size_t _y, size_t _z, size_t n)
{
	double del_phi = field[x][y][z][1] - field[_x][_y][_z][1];

	energy -= (del_phi*charge -  n * FALLING_EN);
}
double Molecule::falling_check(Field& field, size_t _x, size_t _y, size_t _z, size_t n)
{
	double del_phi = field[x][y][z][1] - field[_x][_y][_z][1];

	return energy - (del_phi*charge - n * FALLING_EN);
}

//Потеря энергии частицой при переходе на уровень вверх
void Molecule::ascent(size_t n)
{
	energy -= n*ASCENT_EN;
}
void Molecule::ascent(Field& field, size_t _x, size_t _y, size_t _z, size_t n)
{
	double del_phi = field[x][y][z][1] - field[_x][_y][_z][1];

	energy -= (n * ASCENT_EN - del_phi*charge);
	
}
double Molecule::ascent_check(Field& field, size_t _x, size_t _y, size_t _z, size_t n)
{
	double del_phi = field[x][y][z][1] - field[_x][_y][_z][1];

	return energy - (n * ASCENT_EN - del_phi * charge);
}

//Потеря энергии при движении по оси
void Molecule::along()
{
	energy -= ALONG_EN;
}
void Molecule::along(Field& field, size_t _x, size_t _y, size_t _z)
{
	double del_phi = field[x][y][z][1] - field[_x][_y][_z][1];

	energy -= (ALONG_EN - del_phi * charge);
}
double Molecule::along_check(Field& field, size_t _x, size_t _y, size_t _z)
{
	double del_phi = field[x][y][z][1] - field[_x][_y][_z][1];

	return energy - (ALONG_EN - del_phi * charge);
}
