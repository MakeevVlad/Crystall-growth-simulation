#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <random>
#include <fstream>

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
	field.lj_potencial();
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

//Эта функция отвечает за перемещение частицы
bool movement(Molecule& mol, Field& field)
{
	std::cout << "c0 : " << mol.x << mol.y << mol.z << std::endl;

	if (mol.z == field.get_size_z())
		//"Падение" молекулы
		while (field[mol.x][mol.y][mol.z - 1][0] == 0)
		{
			--mol.z;
			std::cout << " ->" << mol.z;
		}

	//Для случая, если частица совершает первое перемещение
	field[mol.x][mol.y][mol.z][0] = 1;

	//Энергия, обладая которой частица не может свободно двигаться
	
	//Проверка на возможность движения
	if (mol.energy - mol.ALONG_EN <= mol.CRIT_EN)
	{
		direction(mol, field);
		return 1;
	}

	size_t x_max = field.get_size_x(), y_max = field.get_size_y();


	//Координаты точки по направлению(вынесено, чтобы каждый раз не писать)
	size_t _X = mol.x != 0 ? (mol.x + mol.dir[0]) % x_max : (x_max + mol.dir[0]) % x_max;
	size_t _Y = mol.y != 0 ? (mol.y + mol.dir[1]) % y_max : (y_max + mol.dir[1]) % y_max;

	//Случай, когда частица двигается вдоль осей:
	//!!! % cord_max означает, что частица при переходе за поле автоматически перемещается нв другую его сторону
	if ((abs(mol.dir[0]) == 1 && mol.dir[1] == 0) || (abs(mol.dir[1]) == 1 && mol.dir[0] == 0))
	{
		if (field[_X][_Y][mol.z][0] == 0) //Если последующая ячейка свободна
		{
			//Случай, когда частица переходит строго вперёд
			if (field[_X][_Y][mol.z - 1][0] == 1)
			{
				//Проверка на возможность движения
				if (mol.energy - mol.ALONG_EN <= mol.CRIT_EN)
				{
					direction(mol, field);
					return 1;
				}

				mol.along();

				field[mol.x][mol.y][mol.z][0] = 0; //Убираем молекулу из начальной координаты

				//Следующие координаты
				mol.x = _X;
				mol.y = _Y;

				field[mol.x][mol.y][mol.z][0] = 1;
				return 0;
			}
			//Случай, когда частица переходит на уровень вниз
			else
			{
				//Проверка на возможность движения
				if (mol.energy - mol.FALLING_EN <= mol.CRIT_EN)
				{
					direction(mol, field);
					return 1;
				}

				mol.falling();

				field[mol.x][mol.y][mol.z][0] = 0; //Убираем молекулу из начальной координаты

				//Следующие координаты
				mol.x = _X;
				mol.y = _Y;

				//"Падение" частицы
				while (field[(mol.x + mol.dir[0]) % x_max][(mol.y + mol.dir[1]) % y_max][mol.z - 1][0] == 0) --mol.z;
				
				field[mol.x][mol.y][mol.z][0] = 1;
				return 0;
			}
		}
		//Если последующая ячейка занята(происходит выбор между отражением и переходом наверх)
		else
		{
			//Отражение

			//Проверка на возможность движения
			if (mol.energy - mol.ALONG_EN <= mol.CRIT_EN)
				{
					direction(mol, field);
					return 1;
				}

			mol.along();

			mol.dir[0] *= -1;
			mol.dir[1] *= -1;
			movement(mol, field);


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
	this->lj_potencial();
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

	 for (double x = 0; x < size[0]; ++x)
		 for (double y = 0; y < size[1]; ++y)
			 for (double z = 0; z < size[2]; ++z)
			 {
				 for (double x1 = 0; x1 < size[0]; ++x1)
					 for (double y1 = 0; y1 < size[1]; ++y1)
						 for (double z1 = 0; z1 < size[2]; ++z1)
						 {
							 if (zone[size_t(x1)][size_t(y1)][size_t(z1)][0] == 1 && (x != x1 || y != y1 || z != z1))
							 {
								 double r = sqrt((x - x1)*(x - x1) + (y - y1)*(y - y1) + (z - z1)*(z - z1));
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
	
	dir[0] = -1;
	dir[1] = 0;

	energy = 350;

}

Molecule::~Molecule()
{
	delete &x, &y, &z, dir, &energy;
}


//выйгрыш энергии частицой при переходе на уровень вниз
void Molecule::falling()
{
	energy += FALLING_EN;
}
//Потеря энергии частицой при переходе на уровень вверх
void Molecule::ascent()
{
	energy -= ASCENT_EN;
}
//Потеря энергии при движении по оси
void Molecule::along()
{
	energy -= ALONG_EN;
}


