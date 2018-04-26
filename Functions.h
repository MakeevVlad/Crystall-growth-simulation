#pragma once

#include <vector>
#include <iostream>
#include "Functions.h"

typedef std::vector<std::vector<std::vector<double>>> tdim_vec;

class Field
{

private:
	//Само 3d поле
	tdim_vec zone[2]; //Zone[0] -- занята ли ячейка; zone[1] -- потенциал
	//Ограничения поля по размеру
	size_t size[3];

public:


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

};

class Molecule
{
private:



public:
	double x, y, z;
	const double mass;
	const double charge;
	double dir[2];
	double energy;

	double delta_en = 100;

	Molecule();

	void En_loss();
};

//Movements

//Direction choice
