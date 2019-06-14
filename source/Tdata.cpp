#include "Tdata.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

Tdata::Tdata(std::string str) {
  tStrData = str;
  for (int i = 0; i < 9; i++) {
    if (tStrData[i]=='0') {
      weight[i] = INITIAL;
      newTiles[i] = true;
    } else {
      weight[i] = -1;
      newTiles[i] = false;
    }
  }
}

Tdata::Tdata(std::string str, int savedWeight[], bool savedNewTiles[]) {
  tStrData = str;
  for (int i = 0; i < 9; i++) {
    if (savedWeight[i] < MAXWEIGHT) {
      weight[i] = savedWeight[i];
    } else {
      weight[i] = MAXWEIGHT;
    }
    newTiles[i] = savedNewTiles[i];
  }
}

Tdata::~Tdata() {}

int Tdata::chooseTile() {
  int position = -1;

  std::vector<unsigned long int> range;
  range.push_back(0);
  unsigned long int sum = 0;
  std::vector<int> validTiles;

  for (int i = 0; i < 9; i++) {
    if (weight[i]!=-1) {
      sum += weight[i]+(newTiles[i]*50);
      range.push_back(sum);
      validTiles.push_back(i);
    }
  }

  if (sum == 0) {
    return -343;
  }

  unsigned long int random = rand()%range[range.size()-1];
  for (unsigned int i = 0; i < range.size()-1; i++) {
    if (range[i] <= random && random < range[i+1]) {
      position = validTiles[i];
    }
  }

  return position;
}

void Tdata::changeWeight(int result, int tile) {
  if (-1 < weight[tile] + result && weight[tile] + result < MAXWEIGHT) weight[tile] += result;
  newTiles[tile] = false;
}

float Tdata::getPercent(int index) {

  unsigned long int sum = 0;

  if (weight[index] == -1) return -1.f;

  for (int i = 0; i < 9; i++) {
    if (weight[i]!=-1) {
      sum += weight[i]+(newTiles[i]*50);
    }
  }
  float percent = 100*(float)(weight[index]+(newTiles[index]*50))/sum;
  return percent;
}

void Tdata::reset() {
  for (int i = 0; i < 9; i++) {
    if (tStrData[i]=='0') {
      weight[i] = INITIAL;
      newTiles[i] = true;
    } else {
      weight[i] = -1;
      newTiles[i] = false;
    }
  }
}
