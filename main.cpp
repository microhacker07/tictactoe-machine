#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include "Machine.h"
#include "version.h"

// Whole mess of messy global variables

int selectedTile = -1;
bool currentPlayer = 0;
bool sleep = false;
int line = -1;
int gameResults[2];
int playingGame = -1;
bool training = 0;

Machine first(0, 3, 1, -1);
Machine second(1, 3, 1, -1);

int grid[9];

const int height = 25*16;
const int width  = 25*16;

const float scaleX = width/25;
const float scaleY = height/25;

// Clears and resets the tictactoe board

void clear() {
  for (int i = 0; i < 9; i++) {
    grid[i] = 0;
  }
  currentPlayer = 0;
  line = -1;
  selectedTile = -1;
  playingGame = 1;
  for (int i = 0; i > 2; i++) {
    gameResults[i] = -1;
  }
}

// This function take in a SFML keycode input and handles it

void input(sf::RenderWindow* window, int keycode) {
  switch (keycode) {
    case sf::Keyboard::Escape:
    printf("\nClosing Window from escape button.\n");
    window->close();
    break;

    case sf::Keyboard::R:
    clear();
    break;

    case sf::Keyboard::T:
    training = !training;
    break;

    case sf::Keyboard::F1:
    first.save();
    second.save();
    break;

    case sf::Keyboard::F2:
    first.load();
    second.load();
    break;

    case sf::Keyboard::F3:
    first.forget();
    second.forget();
    break;

  }
  int numpad[9] = {
    sf::Keyboard::Numpad1, sf::Keyboard::Numpad2, sf::Keyboard::Numpad3,
    sf::Keyboard::Numpad4, sf::Keyboard::Numpad5, sf::Keyboard::Numpad6,
    sf::Keyboard::Numpad7, sf::Keyboard::Numpad8, sf::Keyboard::Numpad9
  };

  int numberKeys[9] = {
    sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3,
    sf::Keyboard::Num4, sf::Keyboard::Num5, sf::Keyboard::Num6,
    sf::Keyboard::Num7, sf::Keyboard::Num8, sf::Keyboard::Num9
  };

  int correctNumPair[9] {6, 7, 8, 3, 4, 5, 0, 1, 2};

  for (int i = 0; i < 9; i++) {
    if ((keycode == numpad[i] && selectedTile < 0)) {
      selectedTile = correctNumPair[i];
    } else if (keycode == numberKeys[i] && selectedTile < 0) {
      selectedTile = i;
    }
  }
}

// Handles window events like keypresses, user closes window, etc

void eventHandler(sf::RenderWindow* window) {
  sf::Event event;

  while (window->pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
      printf("\nClosing Window.\n");
      window->close();
      break;

      case sf::Event::KeyPressed:
      input(window, event.key.code);
      break;
    }
  }
}

// Determines winner of game for learning, probably needs a rewrite

void endgame(int b) {
  if (currentPlayer == 0) {
    gameResults[0] = 0;
    gameResults[1] = 2;
  } else {
    gameResults[0] = 2;
    gameResults[1] = 0;
  }
  if (b!=-1) {
    line = b;
  } else {
    gameResults[0] = 1;
    gameResults[1] = 1;
  }
  playingGame = -1;
}

// Updates the game

void update() {
// if a tile on the board is picked and the tile is empty
  if (playingGame == 1 && (selectedTile > -1 && grid[selectedTile] == 0)) {
    grid[selectedTile] = currentPlayer+1;
    currentPlayer = !currentPlayer;

    // Win Detection
    for (int i = 0; i < 3; i++) {
      if (grid[i*3]!=0 && (grid[i*3]==grid[i*3+1] && grid[i*3+1]==grid[i*3+2])) endgame(i);
      if (grid[i]!=0 && (grid[i]==grid[i+3] && grid[i+3]==grid[i+6])) endgame(i+3);
    }
    if (grid[4]!=0 && (grid[0]==grid[4]&&grid[4]==grid[8])) endgame(6);
    if (grid[4]!=0 && (grid[2]==grid[4]&&grid[4]==grid[6])) endgame(7);

    int x = 1;
    for (int i = 0; i < 9; i++) x *= grid[i];
    if(x!=0&&playingGame==1) endgame(-1);
    // End of Win Detection

  }
  selectedTile = -1;
}

// Function to draw a cross at a certain position on the board

sf::VertexArray drawCross(int x, int y) {
  sf::VertexArray cross(sf::Quads, 8);

  x *= 8*scaleY;
  y *= 8*scaleY;

  cross[0].position = sf::Vector2f(1.5f*scaleY+x, 2.5f*scaleY+y);
  cross[1].position = sf::Vector2f(6.5f*scaleY+x, 7.5f*scaleY+y);
  cross[2].position = sf::Vector2f(7.5f*scaleY+x, 6.5f*scaleY+y);
  cross[3].position = sf::Vector2f(2.5f*scaleY+x, 1.5f*scaleY+y);

  cross[4].position = sf::Vector2f(1.5f*scaleY+x, 6.5f*scaleY+y);
  cross[5].position = sf::Vector2f(6.5f*scaleY+x, 1.5f*scaleY+y);
  cross[6].position = sf::Vector2f(7.5f*scaleY+x, 2.5f*scaleY+y);
  cross[7].position = sf::Vector2f(2.5f*scaleY+x, 7.5f*scaleY+y);

  for (int i = 0; i < 8; i++) {
    cross[i].color = sf::Color::Red;
  }
  return cross;
}

// Function to draw a circle at a certain position on the board

sf::CircleShape drawCircle(int x, int y) {
  sf::CircleShape circle(2.75f*scaleY,64);

  x *= 8*scaleY;
  y *= 8*scaleY;

  circle.setFillColor(sf::Color::Transparent);
  circle.setOutlineThickness(-1.f*scaleY);
  circle.setOutlineColor(sf::Color::Blue);
  circle.setPosition(sf::Vector2f(1.75f*scaleY+x, 1.75f*scaleY+y));

  return circle;
}

// Sloppy function to draw the win line on the screen

sf::VertexArray drawWinLine() {
  sf::VertexArray winLine(sf::Quads, 4);

  int x = line%3;
  int y = line;
  x *= 8*scaleY;
  y *= 8*scaleY;

  if (-1 < line && line < 3) {
    winLine[0].position = sf::Vector2f(4.f*scaleY, 4.85f*scaleY+y);
    winLine[1].position = sf::Vector2f(21.f*scaleY, 4.85f*scaleY+y);
    winLine[2].position = sf::Vector2f(21.f*scaleY, 4.15f*scaleY+y);
    winLine[3].position = sf::Vector2f(4.f*scaleY, 4.15f*scaleY+y);
  } else if (2 < line && line < 6) {
    winLine[0].position = sf::Vector2f(4.85f*scaleY+x, 4.f*scaleY);
    winLine[1].position = sf::Vector2f(4.85f*scaleY+x, 21.f*scaleY);
    winLine[2].position = sf::Vector2f(4.15f*scaleY+x, 21.f*scaleY);
    winLine[3].position = sf::Vector2f(4.15f*scaleY+x, 4.f*scaleY);
  } else if (line == 6) {
    winLine[0].position = sf::Vector2f(4.75f*scaleY, 4.25f*scaleY);
    winLine[1].position = sf::Vector2f(4.25f*scaleY, 4.75f*scaleY);
    winLine[2].position = sf::Vector2f(20.25f*scaleY, 20.75f*scaleY);
    winLine[3].position = sf::Vector2f(20.75f*scaleY, 20.25f*scaleY);
  } else {
    winLine[0].position = sf::Vector2f(20.25f*scaleY, 4.25f*scaleY);
    winLine[1].position = sf::Vector2f(20.75f*scaleY, 4.75f*scaleY);
    winLine[2].position = sf::Vector2f(4.75f*scaleY, 20.75f*scaleY);
    winLine[3].position = sf::Vector2f(4.25f*scaleY, 20.25f*scaleY);
  }

  for (int i = 0; i < 4; i++) {
    winLine[i].color = sf::Color::Green;
  }

  return winLine;
}

// This does the rendering to the screen

void draw(sf::RenderWindow* window) {
  window->clear(sf::Color(20, 20, 20));

  // Base cross (#) of the game

  sf::VertexArray base(sf::Quads, 16);

  base[0 ].position = sf::Vector2f(1.f*scaleY,  8.f*scaleY);
  base[1 ].position = sf::Vector2f(24.f*scaleY, 8.f*scaleY);
  base[2 ].position = sf::Vector2f(24.f*scaleY, 9.f*scaleY);
  base[3 ].position = sf::Vector2f(1.f*scaleY,  9.f*scaleY);

  base[4 ].position = sf::Vector2f(1.f*scaleY,  16.f*scaleY);
  base[5 ].position = sf::Vector2f(24.f*scaleY, 16.f*scaleY);
  base[6 ].position = sf::Vector2f(24.f*scaleY, 17.f*scaleY);
  base[7 ].position = sf::Vector2f(1.f*scaleY,  17.f*scaleY);

  base[8 ].position = sf::Vector2f(8.f*scaleY,  1.f*scaleY);
  base[9 ].position = sf::Vector2f(9.f*scaleY,  1.f*scaleY);
  base[10].position = sf::Vector2f(9.f*scaleY,  24.f*scaleY);
  base[11].position = sf::Vector2f(8.f*scaleY,  24.f*scaleY);

  base[12].position = sf::Vector2f(16.f*scaleY, 1.f*scaleY);
  base[13].position = sf::Vector2f(17.f*scaleY, 1.f*scaleY);
  base[14].position = sf::Vector2f(17.f*scaleY, 24.f*scaleY);
  base[15].position = sf::Vector2f(16.f*scaleY, 24.f*scaleY);

  window->draw(base);

  int x, y;

  for (int i = 0; i < 9; i++) {
    x = i%3;
    y = i/3;
    if (grid[i] == 1) window->draw(drawCross (x, y));
    if (grid[i] == 2) window->draw(drawCircle(x, y));
  }

  if (line!=-1) window->draw(drawWinLine());

  window->display();
}

// --Sub Optimal Player Code----------------

bool isWinner(std::string board, bool player) {
  bool result = false;
  for (int i = 0; i < 3; i++) {
    if (board[i*3]==player+'1' && (board[i*3]==board[i*3+1] && board[i*3+1]==board[i*3+2])) result = true;
    if (board[i]==player+'1' && (board[i]==board[i+3] && board[i+3]==board[i+6])) result = true;
  }
  if (board[4]==player+'1' && (board[0]==board[4]&&board[4]==board[8])) result = true;
  if (board[4]==player+'1' && (board[2]==board[4]&&board[4]==board[6])) result = true;

  return result;
}

int chooseRandomMove(std::string board, int arr[], int size) {
  std::vector<int> moves;
  for (int i = 0; i < size; i++) {
    if (board[arr[i]] == '0') {
      moves.push_back(arr[i]);
    }
  }

  if (moves.size() != 0) {
    int randomNum = rand()%moves.size();
    return moves[randomNum];
  }

  return -1;
}

int getOptimalMove(std::string board, bool player, bool currentPlayer) {
  if (player==currentPlayer) {
    std::string copy;

    for (int i = 0; i < 9; i++) {
      copy = board;
      if (copy[i] == '0') {
        copy[i] = player+1+'0';
        if (isWinner(copy, player)) {
          return i;
        }
      }
    }

    for (int i = 0; i < 9; i++) {
      copy = board;
      if (copy[i] == '0') {
        copy[i] = !player+1+'0';
        if (isWinner(copy, !player)) {
          return i;
        }
      }
    }

    int corners[] = {0, 2, 6, 8};

    int move = chooseRandomMove(board, corners, 4);
    if (move != -1) {
      return move;
    }

    if (board[4] == '0') {
      return 4;
    }

    int sides[] = {1, 3, 5, 7};
    move = chooseRandomMove(board, sides, 4);
    if (move != -1) {
      return move;
    }
  }
  return -1;
}

// -----------------------------------------

int main(int argc, char const *argv[]) {
  srand(time(0));

  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  sf::RenderWindow window;
  window.create(sf::VideoMode(width, height), "Tic-Tac-Toe", sf::Style::Close, settings);
  window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2-width/2, sf::VideoMode::getDesktopMode().height/2-height/2));
  window.setVerticalSyncEnabled(true);

  clear();

  std::string currentGameState = "000000000";

  int wait = 0;
  int draws;
  bool afterGame = 0;
  unsigned int numberOfGames = 0;
  unsigned int wins[2] = {0, 0};

  while(window.isOpen()) {

    eventHandler(&window);
    update();

    // Start of First Learner
    if (playingGame == 1) {
      for (int i = 0; i < 9; i++) currentGameState[i] = grid[i]+48;
      selectedTile = first.tile(currentPlayer, currentGameState);
      update();
    }
    // End of First Learner

    // Start of Second Learner
    if (playingGame == 1 && training) {
      for (int i = 0; i < 9; i++) currentGameState[i] = grid[i]+48;
      selectedTile = second.tile(currentPlayer, currentGameState);
      update();
    }
    // End of Second Learner
/*
    if (playingGame == 1 && training) {
      for (int i = 0; i < 9; i++) currentGameState[i] = grid[i]+48;
      selectedTile = getOptimalMove(currentGameState, currentPlayer, 1);
    }
*/
    if (playingGame == -1 && !afterGame) {
      first.learn(gameResults[0]);
    }

    if (playingGame == -1 && !afterGame) {
      second.learn(gameResults[1]);
      afterGame = true;
    }

    if (!training||wait==1) draw(&window);
    if (playingGame == -1) wait++;
    if (wait > 30 || (training&&wait>2)) {
      if (gameResults[0]==2) wins[0]++;
      if (gameResults[1]==2) wins[1]++;
      clear();
      afterGame = false;
      wait = 0;
      numberOfGames++;
      draws = numberOfGames - (wins[0] + wins[1]);
      printf("Number of Games: %i, Player 1: %i - %f%%, Player 2: %i - %f%%, Draw: %i - %f%%\n",
              numberOfGames,
              wins[0], 100*((float)wins[0]/numberOfGames),
              wins[1], 100*((float)wins[1]/numberOfGames),
              draws, 100*((float)draws/numberOfGames)
             );
    }

    //printf("Number of Games: %i, Player 1: %i, Player 2: %i\r", numberOfGames, wins[0], wins[1]);

  }

  return 0;
}
