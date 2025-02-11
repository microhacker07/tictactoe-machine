# tictactoe-machine
A tic-tac-toe machine learning game. (Thermonuclear war is depreciated!)
This project is written in C++ and uses the SFML library for displaying and drawing to a window. 

This machine learning project works with having "weights" on each open space. 
The machine then picks a random space, with spaces that have more "weight" to them having a higher chance of being picked. 
It learns by reinforcement with by recording all the moves it makes throughout one game. 
Then based on the outcome of the game (whether it wins, loses, or ties), it will remove or add weights on each move that led to that outcome. 
So if it loses a match, it is less likely to make the same moves as before. 
In this program, I have it so it will add 3 weights for a win, 1 weight of it ties, and it will remove 1 weight of it loses. 
This setup favors winning and still makes getting a tie beneficial, so you might see sometimes that the computer picked a move that led to a tie, even though it could of won. 
Also the program has it so it will weight moves it has never done before greatly to make it try new moves.

TL:DR: Records all moves and uses weight to play and learn.

Lost: -1 lbs

Draw: +1 lbs

Win: +3 lbs 

## Keybinding:

* Numpad 1-9 => number position is coordinated to the same spot on the board.
* 1-9 => (For non-numpads devices) 1-3 is the top row, 4-6 is the middle, and 7-9 is bottom row all going from left to right.
* t => toggle computer vs computer mode for training.
* r => clear/reset the tic-tac-toe board.
* c => clear the statistics of number of games, wins, etc
* s => toggles 'slowmo' mode. Useful for viewing training.
* F1 => saves the 'brain' of both machine learning players under the filenames of "MLplayer_1.txt" and "MLplayer_2.txt"
* F2 => loads the 'brain' of both machine learning players from the files "MLplayer_1.txt" and "MLplayer_2.txt"
* F3 => makes both machine learning players 'forget' everything they know
* esc => closes the program

## Development

Instructions build from source.

### Prerequisites

* C++ Compiler
* [SFML 2.5](https://www.sfml-dev.org/download/sfml/2.5.1/)
* [SCons](https://scons.org/pages/download.html)

SFML for cross-platform graphical interface.

SCons is the build tool.

## Setup

1. Clone the repo
```sh
git clone https://github.com/microhacker07/tictactoe-machine.git
cd tictactoe-machine
```
2. Compile the project
```sh
scons
```
3. Run the executable
```sh
./tictactoe
```
