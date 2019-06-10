#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Machine.h"

// Whole mess of messy global variables

sf::Font font;

int selectedTile = -1;
bool currentPlayer = 0;
bool sleep = false;
int line = -1;
int gameResults[2];
int playingGame = -1;
int humanPlayer;
std::string prevMachine1;
std::string prevMachine2;
unsigned int numberOfComputerGames = 0;
unsigned int ComputerVComputerWins[2] = {0, 0};
unsigned int numberOfHumanGames = 0;
unsigned int HumanVComputerWins[2] = {0, 0};

Machine first(0, 3, 1, -1);
Machine second(1, 3, 1, -1);

int grid[9];

const int HEIGHT = 25*16;
const int WIDTH  = 25*32;

const float SCALEX = WIDTH/25;
const float SCALEY = HEIGHT/25;

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
  first.clearMoves();
  second.clearMoves();
  prevMachine1 = "";
  prevMachine2 = "";
}

void clearScore() {
  numberOfComputerGames = 0;
  ComputerVComputerWins[0] = 0;
  ComputerVComputerWins[1] = 0;
  numberOfHumanGames = 0;
  HumanVComputerWins[0] = 0;
  HumanVComputerWins[1] = 0;
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
    if (humanPlayer == 0) {
      humanPlayer = 2;
    } else {
      humanPlayer = 0;
    }
    clear();
    break;

    case sf::Keyboard::C:
    clearScore();
    clear();
    break;

    case sf::Keyboard::F1:
    first.save();
    second.save();
    break;

    case sf::Keyboard::F2:
    first.load();
    second.load();
    clear();
    break;

    case sf::Keyboard::F3:
    first.forget();
    second.forget();
    clear();
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
    if (b != -343) line = b;
  } else {
    gameResults[0] = 1;
    gameResults[1] = 1;
  }
  if (humanPlayer != 0) {
    humanPlayer = humanPlayer%2+1;
    if (gameResults[humanPlayer-1] == 0) {
      HumanVComputerWins[0]++;
    } else if (gameResults[humanPlayer-1] == 2){
      HumanVComputerWins[1]++;
    }
    numberOfHumanGames++;
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
  if (selectedTile == -343) {
      printf("Player %i fortified!!\n", currentPlayer+1);
    endgame(-343);
  }
  selectedTile = -1;
}

// Function to draw a cross at a certain position on the board

sf::VertexArray drawCross(int x, int y, float scale) {
  sf::VertexArray cross(sf::Quads, 8);

  x *= SCALEY;
  y *= SCALEY;

  scale *= SCALEY;

  cross[0].position = sf::Vector2f(1.5f*scale+x, 2.5f*scale+y);
  cross[1].position = sf::Vector2f(6.5f*scale+x, 7.5f*scale+y);
  cross[2].position = sf::Vector2f(7.5f*scale+x, 6.5f*scale+y);
  cross[3].position = sf::Vector2f(2.5f*scale+x, 1.5f*scale+y);

  cross[4].position = sf::Vector2f(1.5f*scale+x, 6.5f*scale+y);
  cross[5].position = sf::Vector2f(6.5f*scale+x, 1.5f*scale+y);
  cross[6].position = sf::Vector2f(7.5f*scale+x, 2.5f*scale+y);
  cross[7].position = sf::Vector2f(2.5f*scale+x, 7.5f*scale+y);

  for (int i = 0; i < 8; i++) {
    cross[i].color = sf::Color::Red;
  }
  return cross;
}

// Function to draw a circle at a certain position on the board

sf::CircleShape drawCircle(int x, int y, float scale) {
  scale *= SCALEY;
  sf::CircleShape circle(2.75f*scale,64);

  x *= SCALEY;
  y *= SCALEY;

  circle.setFillColor(sf::Color::Transparent);
  circle.setOutlineThickness(-1.f*scale);
  circle.setOutlineColor(sf::Color::Blue);
  circle.setPosition(sf::Vector2f(1.75f*scale+x, 1.75f*scale+y));

  return circle;
}

// Sloppy function to draw the win line on the screen

sf::VertexArray drawWinLine() {
  sf::VertexArray winLine(sf::Quads, 4);

  int x = line%3;
  int y = line;
  x *= 8*SCALEY;
  y *= 8*SCALEY;

  if (-1 < line && line < 3) {
    winLine[0].position = sf::Vector2f(4.f*SCALEY, 4.85f*SCALEY+y);
    winLine[1].position = sf::Vector2f(21.f*SCALEY, 4.85f*SCALEY+y);
    winLine[2].position = sf::Vector2f(21.f*SCALEY, 4.15f*SCALEY+y);
    winLine[3].position = sf::Vector2f(4.f*SCALEY, 4.15f*SCALEY+y);
  } else if (2 < line && line < 6) {
    winLine[0].position = sf::Vector2f(4.85f*SCALEY+x, 4.f*SCALEY);
    winLine[1].position = sf::Vector2f(4.85f*SCALEY+x, 21.f*SCALEY);
    winLine[2].position = sf::Vector2f(4.15f*SCALEY+x, 21.f*SCALEY);
    winLine[3].position = sf::Vector2f(4.15f*SCALEY+x, 4.f*SCALEY);
  } else if (line == 6) {
    winLine[0].position = sf::Vector2f(4.75f*SCALEY, 4.25f*SCALEY);
    winLine[1].position = sf::Vector2f(4.25f*SCALEY, 4.75f*SCALEY);
    winLine[2].position = sf::Vector2f(20.25f*SCALEY, 20.75f*SCALEY);
    winLine[3].position = sf::Vector2f(20.75f*SCALEY, 20.25f*SCALEY);
  } else {
    winLine[0].position = sf::Vector2f(20.25f*SCALEY, 4.25f*SCALEY);
    winLine[1].position = sf::Vector2f(20.75f*SCALEY, 4.75f*SCALEY);
    winLine[2].position = sf::Vector2f(4.75f*SCALEY, 20.75f*SCALEY);
    winLine[3].position = sf::Vector2f(4.25f*SCALEY, 20.25f*SCALEY);
  }

  for (int i = 0; i < 4; i++) {
    winLine[i].color = sf::Color::Green;
  }

  return winLine;
}

sf::VertexArray drawBoard(float x1, float y1, float x2, float y2, float thickness) {
  sf::VertexArray board(sf::Quads, 16);

  thickness *= -1;

  float xthird = (x2 - x1)/ 3 + x1 - (thickness/2);
  float ythird = (y2 - y1)/ 3 + y1 - (thickness/2);

  float x2third = 2*((x2-x1)/3)+x1 - (thickness/2);
  float y2third = 2*((y2-y1)/3)+y1 - (thickness/2);

  x1++;
  y1++;
  x2--;
  y2--;

  board[0 ].position = sf::Vector2f(x1*SCALEY,  ythird*SCALEY);
  board[1 ].position = sf::Vector2f(x2*SCALEY, ythird*SCALEY);
  board[2 ].position = sf::Vector2f(x2*SCALEY, (ythird+thickness)*SCALEY);
  board[3 ].position = sf::Vector2f(x1*SCALEY,  (ythird+thickness)*SCALEY);

  board[4 ].position = sf::Vector2f(x1*SCALEY,  y2third*SCALEY);
  board[5 ].position = sf::Vector2f(x2*SCALEY, y2third*SCALEY);
  board[6 ].position = sf::Vector2f(x2*SCALEY, (y2third+thickness)*SCALEY);
  board[7 ].position = sf::Vector2f(x1*SCALEY,  (y2third+thickness)*SCALEY);

  board[8 ].position = sf::Vector2f(xthird*SCALEY,  y1*SCALEY);
  board[9 ].position = sf::Vector2f((xthird+thickness)*SCALEY,  y1*SCALEY);
  board[10].position = sf::Vector2f((xthird+thickness)*SCALEY,  y2*SCALEY);
  board[11].position = sf::Vector2f(xthird*SCALEY,  y2*SCALEY);

  board[12].position = sf::Vector2f(x2third*SCALEY, y1*SCALEY);
  board[13].position = sf::Vector2f((x2third+thickness)*SCALEY, y1*SCALEY);
  board[14].position = sf::Vector2f((x2third+thickness)*SCALEY, y2*SCALEY);
  board[15].position = sf::Vector2f(x2third*SCALEY, y2*SCALEY);

  return board;
}

sf::Text basicText(std::string str, float size, float x, float y) {
  sf::Text text(str, font);
  text.setCharacterSize(size*SCALEY);
  text.setColor(sf::Color::White);
  text.setPosition(x*SCALEY, y*SCALEY);

  return text;
}

// This does the rendering to the screen

void draw(sf::RenderWindow* window) {
  window->clear(sf::Color(20, 20, 20));

  int x, y;

  window->draw(drawBoard(0.f, 0.f, 25.f, 25.f, 1.f));
  window->draw(drawBoard(25.f, 12.5f, 37.5f, 25.f, .5f));
  window->draw(drawBoard(37.5f, 12.5f, 50.f, 25.f, .5f));

  std::string percentStr;

  if (prevMachine1 != "") {

    for (int i = 0; i < 9; i++) {
      x = (i%3)*4.16666;
      y = (i/3)*4.16666;
      float temp = first.getWeightPercentages(prevMachine1, i);
      if (temp < -.5f) {
        percentStr = " Filled";
      } else {
        percentStr = std::to_string(temp);
        percentStr = percentStr.substr(0,5) + '%';
      }
      window->draw(basicText(percentStr, .9f, 25.75f+x, 14.f+y));
    }
  }

  if (prevMachine2 != "") {

    for (int i = 0; i < 9; i++) {
      x = (i%3)*4.16666;
      y = (i/3)*4.16666;
      float temp = second.getWeightPercentages(prevMachine2, i);
      if (temp < -.5f) {
        percentStr = " Filled";
      } else {
        percentStr = std::to_string(temp);
        percentStr = percentStr.substr(0,5) + '%';
      }
      window->draw(basicText(percentStr, .9f, 38.25f+x, 14.f+y));
    }
  }

  std::string player1Str, player2Str;
  if (humanPlayer == 1) {
    player1Str = "Human";
    player2Str = "Computer";
  } else if (humanPlayer == 2) {
    player1Str = "Computer";
    player2Str = "Human";
  } else {
    player1Str = "Computer";
    player2Str = "Computer";
  }

  window->draw(basicText("Cpu Player 1", 1.f, 28.f, 12.25f));
  window->draw(basicText("Cpu Player 2", 1.f, 40.5f, 12.25f));
  window->draw(basicText(player1Str, 1.5f, 29.5f, 1.5f));
  window->draw(basicText(player2Str, 1.5f, 41.5f, 1.5f));

  window->draw(basicText("Human Wins: " + std::to_string(HumanVComputerWins[0]), 1.f, 25.f, 5.f));
  window->draw(basicText("Cpu Wins: " + std::to_string(HumanVComputerWins[1]), 1.f, 25.f, 6.f));
  window->draw(basicText("HvC Draws: " + std::to_string(numberOfHumanGames-(HumanVComputerWins[0]+HumanVComputerWins[1])), 1.f, 25.f, 7.f));

  window->draw(basicText("Cpu 1 Wins: " + std::to_string(ComputerVComputerWins[0]), 1.f, 25.f, 8.5f));
  window->draw(basicText("Cpu 2 Wins: " + std::to_string(ComputerVComputerWins[1]), 1.f, 25.f, 9.5f));
  window->draw(basicText("CvC Draws: " + std::to_string(numberOfComputerGames-(ComputerVComputerWins[0]+ComputerVComputerWins[1])), 1.f, 25.f, 10.5f));

  window->draw(drawCross (25.f, 0.f, .5f));
  window->draw(drawCircle(37.5f, 0.f, .5f));

  for (int i = 0; i < 9; i++) {
    x = (i%3)*8.33333;
    y = (i/3)*8.33333;
    if (grid[i] == 1) window->draw(drawCross (x, y, 1.f));
    if (grid[i] == 2) window->draw(drawCircle(x, y, 1.f));
  }

  if (line!=-1) window->draw(drawWinLine());

  window->display();
}

int main(int argc, char const *argv[]) {
  srand(time(0));

  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  sf::RenderWindow window;
  window.create(sf::VideoMode(WIDTH, HEIGHT), "Tic-Tac-Toe", sf::Style::Close, settings);
  window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2-WIDTH/2, sf::VideoMode::getDesktopMode().height/2-HEIGHT/2));
  window.setVerticalSyncEnabled(true);

  if (!font.loadFromFile("assets/arial.ttf"))
  {
    printf("Failed to load fonts\n");
  }

  clear();
  humanPlayer = 2;

  std::string currentGameState = "000000000";

  int wait = 0;
  int draws;
  bool afterGame = 0;

  while(window.isOpen()) {

    eventHandler(&window);
    update();

    // Start of First Learner
    if (playingGame == 1 && humanPlayer != 1) {
      for (int i = 0; i < 9; i++) currentGameState[i] = grid[i]+48;
      if (currentPlayer == 0) prevMachine1 = currentGameState;
      selectedTile = first.tile(currentPlayer, currentGameState);
      update();
    }
    // End of First Learner

    // Start of Second Learner
    if (playingGame == 1 && humanPlayer != 2) {
      for (int i = 0; i < 9; i++) currentGameState[i] = grid[i]+48;
      if (currentPlayer == 1) prevMachine2 = currentGameState;
      selectedTile = second.tile(currentPlayer, currentGameState);
      update();
    }
    // End of Second Learner

    if (playingGame == -1 && !afterGame) {
      first.learn(gameResults[0]);
    }

    if (playingGame == -1 && !afterGame) {
      second.learn(gameResults[1]);
      afterGame = true;
    }

    if (humanPlayer != 0||wait==1) draw(&window);
    if (playingGame == -1) wait++;
    if (wait > 30 || (humanPlayer==0&&wait>2)) {
      if (humanPlayer == 0) {
        if (gameResults[0]==2) ComputerVComputerWins[0]++;
        if (gameResults[1]==2) ComputerVComputerWins[1]++;
      }
      clear();
      afterGame = false;
      wait = 0;
      numberOfComputerGames++;
      draws = numberOfComputerGames - (ComputerVComputerWins[0] + ComputerVComputerWins[1]);
      printf("Number of Games: %i, Player 1: %i - %f%%, Player 2: %i - %f%%, Draw: %i - %f%%\r",
              numberOfComputerGames,
              ComputerVComputerWins[0], 100*((float)ComputerVComputerWins[0]/numberOfComputerGames),
              ComputerVComputerWins[1], 100*((float)ComputerVComputerWins[1]/numberOfComputerGames),
              draws, 100*((float)draws/numberOfComputerGames)
             );
    }

  }

  return 0;
}
