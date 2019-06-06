#include "Tdata.h"
#include <vector>

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

Tdata::Tdata(std::string str) {
  tStrData = str;
  for (int i = 0; i < 9; i++) {
    if (tStrData[i]=='0') {
      weight[i] = 3;
    } else {
      weight[i] = -1;
    }
  }
}

Tdata::Tdata(std::string str, int arr[]) {
  tStrData = str;
  for (int i = 0; i < 9; i++) {
    weight[i] = arr[i];
  }
}

Tdata::~Tdata() {}

std::string Tdata::getTStr() { return tStrData; }

int Tdata::chooseTile() {
  int position = -1;

  std::vector<int> range;
  range.push_back(0);
  int sum = 0;
  std::vector<int> validTiles;

  for (int i = 0; i < 9; i++) {
    if (weight[i]!=-1) {
      sum += weight[i]*weight[i];
      range.push_back(sum);
      validTiles.push_back(i);
    }
  }

  if (sum == 0) {
    return -10;
  }

  int random = rand()%sum;
  int interval = 0;
  for (unsigned int i = 0; i < range.size()-1; i++) {
    interval++;
    if (range[i] <= random && random < range[i+1]) {
      position = validTiles[i];
      printf("Probability of moving to %i is %f%%\n", position+1, 100*(float)(range[i+1]-range[i])/sum);
    }
  }

  return position;
}

void Tdata::changeWeight(int result, int tile) {
  int prevWeight[9];
  for (int i = 0; i < 9; i++) {
    prevWeight[i] = weight[i];
  }
  if (0 < weight[tile] + result && weight[tile] + result < 1000000) weight[tile] += result;
  for (int i = 0; i < 9; i++) {
    if (i%3 == 0) printf("\n\n");
    if (weight[i] == -1) {
      printf("[NULL]");
    } else {
      printf("[%i + %i]",prevWeight[i], weight[i]-prevWeight[i]);
    }
  }
  printf("\n\n");
}
