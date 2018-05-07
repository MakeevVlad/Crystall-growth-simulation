#pragma once

#include <vector>
#include <iostream>
#include "Functions.h"

typedef std::vector<std::vector<std::vector<double>>> tdim_vec;

class Field
{

private:


public:
	//���� 3d ����
	std::vector<std::vector<std::vector<std::vector<double>>>> zone; //Zone[0] -- ������ �� ������; zone[1] -- ���������
					  //����������� ���� �� �������
	size_t size[3];

	//������������
	Field();
	Field(size_t len, size_t wid, size_t hei);

	//����� ����. ������ ���������
	void set_size(size_t len, size_t wid, size_t hei);

	//������ ����. ������� ���������
	size_t& get_size_x();
	size_t& get_size_y();
	size_t& get_size_z();

	//����� ����� ����������
	void potencial();

	//������ �������
	std::vector<std::vector<std::vector<double>>>& operator[](size_t i);
	//const std::vector<double>& operator[][][][](size_t i, size_t j, size_t k, size_t p) const;

	//const std::vector<double>& operator[](size_t i);
};

class Molecule
{
private:



public:
	size_t x, y, z;
	double mass = 0;
	double charge = 0;
	int dir[2];
	double energy;

	double delta_en = 100;

	Molecule(Field field);
	Molecule(size_t x, size_t y, size_t z);
	void En_loss();
};

//Movements

//Direction choice�
