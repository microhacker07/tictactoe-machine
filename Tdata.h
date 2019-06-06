#ifndef Tdata_h
#define Tdata_h

#include <iostream>

class Tdata
{
public:
  Tdata(std::string str);
  Tdata(std::string str, int arr[]);
  ~Tdata();
  std::string getTStr();
  int chooseTile();
  int getWeight(int index) { return weight[index]; }
  void changeWeight(int result, int tile);

private:
  std::string tStrData;
  int weight[9];
};

#endif
