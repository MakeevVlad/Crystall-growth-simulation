#pragma once

#include <vector>
#include <iostream>
#include "Functions.h"

typedef std::vector<std::vector<std::vector<double>>> tdim_vec;

class Field
{

private:
	//���� 3d ����
	tdim_vec zone[2]; //Zone[0] -- ������ �� ������; zone[1] -- ���������
	//����������� ���� �� �������
	size_t size[3];

public:
	 

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

};

class Molecule
{
private:


public:
	double x, y, z;
	const double mass;
	const double charge;
	//double velocity[2];
	
	void En_loss();
};

//Movements

//Direction choice

