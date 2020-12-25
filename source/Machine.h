#ifndef Machine_h
#define Machine_h

#include <vector>
#include "Tdata.h"

class Machine
{
public:
  Machine(int playerType, int winGrowth, int tieGrowth, int loseGrowth);
  ~Machine();
  int intRotation(int input);
  std::string stringRotation(std::string input);
  int tile(bool currentPlayer, std::string gameState);
  float getWeightPercentages(std::string gameState, int weight);
  void learn(int result);
  void save();
  void load();
  void forget();
  void clearMoves();

private:
  bool player;
  int win;
  int tie;
  int lose;
  std::vector<Tdata> data;
  std::vector<int> listOfMoves;
  std::vector<int> listOfMovePositions;
};

#endif
