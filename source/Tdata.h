#ifndef Tdata_h
#define Tdata_h

#include <iostream>

class Tdata
{
public:
  Tdata(std::string str);
  Tdata(std::string str, int savedWeight[], bool savedNewTiles[]);
  ~Tdata();
  std::string getTStr() { return tStrData; }
  int chooseTile();
  int getWeight(int index) { return weight[index]; }
  bool getNewTiles(int index) { return newTiles[index]; }
  void changeWeight(int result, int tile);
  float getPercent(int index);
  void reset();

private:
  std::string tStrData;
  int weight[9];
  bool newTiles[9];
  int MAXWEIGHT = 1000;
  int INITIAL = 3;
};

#endif
