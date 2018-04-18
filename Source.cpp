#include <vector>
#include <iostream>
#include "Functions.h"

typedef std::vector<std::vector<std::vector<double>>> tdim_vec;

void coord(size_t w, size_t l)
{
  w = rand() % 100 + 1;
  l = rand() % 100 +1;
}

class field
{

private:
	tdim_vec area[2];
	size_t size[3];

public:
	field(){ this->set_size(100, 100, 100); }

	field(size_t len, size_t wid, size_t hei)
	{
		this->set_size(len, wid, hei);

	}

	void set_size(size_t len, size_t wid, size_t hei)
	{
		size[0] = len;
		size[1] = wid;
		size[2] = hei;
	}

	void get_size()
	{
		std::cout << size[0] << size[1] << size[2];
	}

};



int main()
{
  //Substrate, field[0] -- availability of crystal germ, fiel[1] -- potencial

  size_t w, l, h;

  /*
  while(1)
  {
    coord(w, l);

  }
  */

}

class molecule
{
private:
	double w, l, h;
	const double mass;
	const double charge;
	double velocity[2];

public:
	void En_loss();
};
