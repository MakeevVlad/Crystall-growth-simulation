#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "Functions.h"



//*****������ ��������********************************************************************


//��� ������� ������ ����� �������������, ���� ������� ������� ���������� ����
size_t direction(Molecule mol, Field field)
{

	std::vector<std::vector<double>> pot;

	//������, ����� ������� ������������ ������ ��  Oy
	if (mol.dir[0] == 0)
	{
		//To, kuda budet dvigatsa chasstitsa
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
				if (field[x][y][z][0] == 1) continue; //� ���������� ������ ��������� ������� �� ����� ������ ��� �����������
				pot[i][0] = field[x][y][z][1];
				pot[i][1] = x;
				pot[i][2] = y;
				pot[i][3] = z;
			}
	}

	//������, ����� ������� ������������ ������ �� Ox
	if (mol.dir[0] != 0 && mol.dir[1] != 1)
	{
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
				if (field[x][y][z][0] == 1) continue; //� ���������� ������ ��������� ������� �� ����� ������ ��� �����������
				pot[i][0] = field[x][y][z][1];
				pot[i][1] = x;
				pot[i][2] = y;
				pot[i][3] = z;
			}
	}

	//������, ����� ������� ��������� �� ���������
	else
	{
		return 0;
	}


	//����� �����������
	double p_r;
	p_r = (rand() % 100) / 100;
	size_t pot_size = sizeof(pot);
	//������ � �������������
	std::vector<double> p(pot_size + 1);
	

	for (size_t i = 0; i <= pot.size(); p[pot.size()] += pot[i][0], ++i);
	for (size_t i = 0; i<=pot.size();p[i] = pot[i][0] / p[pot.size()], ++i);

	if( p_r >= p[0])
	{
		field[ size_t(pot[0][1]) ][size_t(pot[0][2])][size_t(pot[0][3])][0] = 1;
		return 1;
	}

	if( p_r <= p[p.size() - 1])
	{
		field[size_t(pot[0][1])][size_t(pot[0][2])][size_t(pot[0][3])][0] = 1;
		return 1;
	}

	else
		for (size_t j = 1; j <= sizeof(p); ++j)
			if ( p_r< p[j] && p[j] >= p[j+1])
			{
				field[size_t(pot[j][1]) ][size_t(pot[j][2]) ][ size_t(pot[j][3]) ][0] = 1;
				return 1;
			}

}

//��� ������� �������� �� ����������� �������
size_t movement(Molecule mol, Field field)
{
	//Energiya, obladaya kotoroy chastitsa uzhe ne mozhet svobodno dvigatsa
	double crit_energy = 100;
	//Proverka na vozmozhnost dvizheniya
	if (mol.energy - mol.delta_en <= crit_energy)
	{
		direction(mol, field);
	}
	//������, ����� ������� ��������� ����� ����:
	//Ox
	if (abs(mol.dir[0]) != 1 && abs(mol.dir[1]) != 1)
	{
		if (field[mol.dir[0] == 1?(mol.x + 1):(mol.x -1)][mol.dir[1] == 1 ? (mol.y + 1) : (mol.y - 1)][mol.z][0] == 0) //���� ����������� ������ ��������
		{
			//������, ����� ������� ��������� ������ �����
			if (field[mol.dir[0] == 1 ? (mol.x + 1) : (mol.x - 1)][mol.dir[1] == 1 ? (mol.y + 1) : (mol.y - 1)][mol.z - 1][0])
			{
				mol.En_loss();
				mol.dir[0] == 1 ? ++mol.x : --mol.x;
				return 1;
			}
		//����� ����� �������� ������, ����� ���� ����������� �������� �� ������� ����
		}
	}
}

//*****������� ������ Field***************************************************************

Field::Field() { set_size(100, 100, 100); }
Field::Field(size_t len, size_t wid, size_t hei)
{
	size[0] = len; //x_size
	size[1] = wid; //y_size
	size[2] = hei; //z_size

	 //������������� ���� ����� ������� ��� ��������� �������!!!!!!!!
	zone.resize(len);
	for (auto& y : zone) {
		y.resize(wid);
		for (auto& z : y) {
			z.resize(hei);
			for (auto& val : z) val.resize(2);
		}
	}
}

void Field::set_size(size_t len, size_t wid, size_t hei)
{
	size[0] = len; //x_size
	size[1] = wid; //y_size
	size[2] = hei; //z_size

	//������������� ���� ����� ������� ��� ��������� �������!!!!!!!!
	zone.resize(len);
	for(auto& y : zone) {
		y.resize(wid);
		for (auto& z : y) {
			z.resize(hei);
			for (auto& val : z)	val.resize(2);
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
	for (auto& i : zone)
		for (auto& j : i)
			for (auto& k : j)
				for (auto& p :k)
					k[1] = 1000;

}


/*
const std::vector<double>& Field::operator[](size_t i)
const {
	return zone[i];
}
*/

//*****������� ������ Molecule***********************************************************

Molecule::Molecule(Field field)
{
	/*
	x = rand() % field.get_size_x();
	y = rand() % field.get_size_y();
	*/
	dir[0] = 1;
	dir[1] = 0;
	energy = 1000;
	x = 50;
	y = 50;
	z = 0;
	field.zone[x][y][z][0] = 1;
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
