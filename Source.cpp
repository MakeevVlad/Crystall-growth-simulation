
void coord(size_t w, size_t l)
{
  w = rand() % 100 + 1;
  l = rand() % 100 +1;
}

class field
{



}
int main()
{
  //Substrate, field[0] -- availability of crystal germ, fiel[1] -- potencial
  std::vector<std::vector<std::vector<double>>> field[2];
  size_t width = 100;
  size_t lenght = 100;
  size_t heigh = 100;
  size_t w, l, h;

  double vel[2];
  size_t t;
  while(1)
  {
    coord(w, l);

  }
}
class molecule
{
  private:
    double w, l, h;
    const double mass;
    const double charge;
    double velocity[2];

  public:
    void En_loss()
}
