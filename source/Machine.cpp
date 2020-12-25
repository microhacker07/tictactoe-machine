#include "Machine.h"

#include <fstream>
#include <string>

Machine::Machine(int playerType, int winGrowth, int tieGrowth, int loseGrowth) {
  player = playerType;
  win = winGrowth;
  tie = tieGrowth;
  lose = loseGrowth;
}

Machine::~Machine() {}

int Machine::intRotation(int input) {
  input = 6 - ((input%3) *3) + (input/3);
  return input;
}

std::string Machine::stringRotation(std::string gameState) {
  std::string temp = "000000000";
  for (int i = 0; i < 9; i++) {
    temp[i] = gameState[intRotation(i)];
  }
  return temp;
}

int Machine::tile(bool currentPlayer, std::string gameState) {
  if (currentPlayer==player) {
    int rot;
    int selectedTile;
    int point = -1;
    bool created = false;

    for (int i = 0; i < 5; i++) {
      gameState = stringRotation(gameState);
      rot = i+1;
      for (unsigned int i = 0; i < data.size(); i++) {
        if (data[i].getTStr() == gameState) {
          point = i;
          break;
        }
      }
      if (point != -1) break;
    }

    if (point==-1) {
      point = data.size();
      data.push_back(Tdata(gameState));
      created = true;
    }
    selectedTile = data[point].chooseTile();
    if (selectedTile == -343) {
      data[point].reset();
      selectedTile = data[point].chooseTile();
    }
    listOfMoves.push_back(point);
    listOfMovePositions.push_back(selectedTile);
    for (int i = 0; i < rot; i++) if (!created) selectedTile = intRotation(selectedTile);
    return selectedTile;
  }
  return -1;
}

float Machine::getWeightPercentages(std::string gameState, int weight) {
  int index = -1;
  for (int i = 0; i < 5; i++) {
    gameState = stringRotation(gameState);
		weight = intRotation(weight);
    for (unsigned int i = 0; i < data.size(); i++) {
      if (data[i].getTStr() == gameState) {
        index = i;
        break;
      }
    }
    if (index != -1) break;
  }

  return data[index].getPercent(weight);
}

void Machine::learn(int result) {
  int growth;
  if (result == 2) {
    growth = win;
  } else if (result == 1) {
    growth = tie;
  } else {
    growth = lose;
  }

  for (unsigned int i = 0; i < listOfMoves.size(); i++) {
    data[listOfMoves[i]].changeWeight(growth, listOfMovePositions[i]);
  }

  listOfMoves.clear();
  listOfMovePositions.clear();
}

void Machine::save() {
  printf("Saving ML Player %i data\n", player+1);
  std::string filename = "MLplayer_";
  filename += player+'1';
  filename += ".txt";
  std::ofstream outputFile;
  std::string outputStr;
  outputFile.open(filename.c_str());

  for (unsigned int i = 0; i < data.size(); i++) {
    outputStr = data[i].getTStr();
    for (int j = 0; j < 9; j++) {
      outputStr += " " + std::to_string(data[i].getWeight(j));
    }
    for (int j = 0; j < 9; j++) {
      if (data[i].getNewTiles(j)) {
        outputStr += " true";
      } else if (data[i].getWeight(j) == -1) {
        outputStr += " null";
      } else {
        outputStr += " false";
      }
    }
    outputStr += "\n";
    outputFile << outputStr;
  }
  outputFile.close();
  printf("  Successfully Saved\n");
}

void Machine::load() {
  std::string filename = "MLplayer_";
  filename += player+'1';
  filename += ".txt";

  printf("Loading data from '%s' for ML player %i\n", filename.c_str(), player+1);

  std::ifstream inputFile;
  inputFile.open(filename.c_str());

  if (inputFile.is_open()) {
    data.clear();

    std::string board;
    int arrWeight[9];
    bool arrNewTiles[9];
    std::string temp;

    while (inputFile >> board) {
      for (int i = 0; i < 9; i++) {
        inputFile >> arrWeight[i];
      }
      for (int i = 0; i < 9; i++) {
        inputFile >> temp;
        if (temp == "true") {
          arrNewTiles[i] = true;
        } else if (temp == "false" || temp == "null") {
          arrNewTiles[i] = false;
        } else { // For compatibility with older versions
          inputFile.seekg(-9, std::ios::cur);
          break;
        }
      }
      data.push_back(Tdata(board, arrWeight, arrNewTiles));
    }
    printf("  Successfully loaded data from '%s'\n", filename.c_str());
  } else {
    printf("  Failed to open the file '%s'\n", filename.c_str());
  }

  inputFile.close();
}

void Machine::forget() {
  printf("Erasing all memories from ML player %i, so long old friend\n", player+1);
  data.clear();
}

void Machine::clearMoves() {
  listOfMoves.clear();
  listOfMovePositions.clear();
}
