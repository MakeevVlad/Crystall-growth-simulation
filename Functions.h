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

	//Задаёт макс. размер кристалла
	void set_size(size_t len, size_t wid, size_t hei);

	//Узнать макс. размеры кристалла
	size_t& get_size_x();
	size_t& get_size_y();
	size_t& get_size_z();

	//Задаёт карту потенциала
	void potencial();

	//Взятие индекса
	std::vector<std::vector<std::vector<double>>>& operator[](size_t i);
	const std::vector<double>& operator[](size_t i) const;

};

class Molecule
{

public:
	size_t x, y, z;
	double mass = 0;
	double charge = 0;
	int dir[2]; //dir[0] <-> x, dir[1] <-> y
	double energy;

	double DELTA_EN = 100;
	double MAX_ENERGY = 400;

	Molecule(Field field);
	Molecule();
	~Molecule();

	void En_loss();
	
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