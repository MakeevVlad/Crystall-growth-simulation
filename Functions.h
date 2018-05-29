#pragma once

#include <vector>
#include <iostream>

typedef std::vector<std::vector<std::vector<double>>> tdim_vec;



class Field
{

public:
	//Само 3d поле
	std::vector<std::vector<std::vector<std::vector<double>>>> zone; //Zone[0] -- занята ли ячейка; zone[1] -- потенциал
	 //Ограничения поля по размеру
	size_t size[3];

	//Конструкторы
	Field();
	Field(size_t len, size_t wid, size_t hei);

	//Узнать макс. размеры кристалла
	size_t& get_size_x();
	size_t& get_size_y();
	size_t& get_size_z();

	//Задаёт макс. размер кристалла
	void set_size(size_t len, size_t wid, size_t hei);

	//Задаёт карту потенциала
	void potencial_uniform();
	void lj_potencial();
	void lj_potencial(size_t _x, size_t _y, size_t _z, int n = 1);

	//Взятие индекса
	std::vector<std::vector<std::vector<double>>>& operator[](size_t i);

};

class Molecule
{

public:
	size_t x, y, z;
	const double mass = 0;
	const double charge = 1;

	double energy;
	int dir[2]; //dir[0] <-> x, dir[1] <-> y
	
	//Энергия, которую частица теряет или получает при смене позиции
	double ALONG_EN = 25;
	double ASCENT_EN = 40;
	double FALLING_EN = 10;

	double MAX_ENERGY = 400;
	double CRIT_EN = 15;
	//Конструкторы
	 Molecule();
	~Molecule();

	Molecule(Field field);
	Molecule(Field field, size_t _x, size_t _y, size_t _z, int dir0, int dir1);
	
	//Генератор молекул
	void mol_generator(Field& field);
	
	//Поиск минимального потенциала по всему полю
	std::vector<double>& min();

	//выйгрыш энергии частицой при переходе на уровень вниз
	void falling(size_t n = 1);
	void falling(Field& field, size_t _x, size_t _y, size_t _z, size_t n);
	double falling_check(Field& field, size_t _x, size_t _y, size_t _z, size_t n);

	//Потеря энергии частицой при переходе на уровень вверх
	void ascent(size_t n = 1);
	void ascent(Field& field, size_t _x, size_t _y, size_t _z, size_t n);
	double ascent_check(Field& field, size_t _x, size_t _y, size_t _z, size_t n);

	//Потеря энергии при движении по оси
	void along();
	void along(Field& field, size_t _x, size_t _y, size_t _z);
	double along_check(Field& field, size_t _x, size_t _y, size_t _z);
};

//Movements

//Direction choice
/*
namespace shift
{
	void movement(Molecule& mol, Field& field);
	void direction(Molecule& mol, Field& field);
}
*/
