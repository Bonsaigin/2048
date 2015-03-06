// Author: Ian Gibson

//A clone of the popular 2048 game.
//The game is played on a 4x4 (variable) grid. When an arrow key
//is pressed, the contents of that board shift in that direction
//as far as they can. If two matching numbers are pushed into each
//other, they are combined.
//Uses ncurses for I/O in a new window

#include <ncurses.h>
#include <random>

#define ROW 1
#define COLUMN 8
#define WIN_VAL 2048

enum direction {UP, LEFT, DOWN, RIGHT};

using namespace std;

//int grid[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
int grid[ROW][COLUMN] = {0};

bool winningBoard(void)
{
  //Check the board for the winning value
  //Return true if the board is winning
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COLUMN; j++) {
      if (grid[i][j] == WIN_VAL) return true;
    }
  }
  return false;
}

void printGrid(void)
{
  //Print the current board and instructions
  clear(); 	//Allows the board to be reprinted in the same location
  printw("Use the arrow keys to shift the tiles!\n");
  printw("\n");
  for (int w = 0; w < COLUMN; w++) {
    printw("+----");
  }
  printw("+\n");

  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COLUMN; j++) {
      if (grid[i][j]) printw("|%4d",grid[i][j]);
      else printw("|    ");
    }
    printw("|\n");

    for (int w = 0; w < COLUMN; w++) {
      printw("+----");
    }
    printw("+\n");
  }
  printw("\n");

  if (winningBoard()) printw("You won! Keep going\n");
}

void shift(direction dir)
{
  //Shift the board in the specified direction
  //If the board has just been shifted, the function will check again
  //to see if it should be shifted another time

  bool justShifted;
  bool checkAgain = true;

  switch(dir) {
    case UP:
      //Shift up
      while(checkAgain) {
        justShifted = false;
        for (int i = 1; i < ROW; i++) {
          for (int j = 0; j < COLUMN; j++) {
            //Make a shift if the current tile has a number, and the tile can be
            //pushed (an empty tile or matching value is the next tile in its path)
            if (grid[i][j] && (grid[i-1][j] == 0 || grid[i-1][j] == grid[i][j])) {
              grid[i-1][j] += grid[i][j];
              grid[i][j] = 0;
              justShifted = true;
            }
          }
        }
        checkAgain = justShifted; 
      }
      break;
    case DOWN:
      //Shift down
      while(checkAgain) {
        justShifted = false;
        for (int i = ROW - 2; i >= 0; i--) {
          for (int j = 0; j < COLUMN; j++) {
            if (grid[i][j] && (grid[i+1][j] == 0 || grid[i+1][j] == grid[i][j])) {
              grid[i+1][j] += grid[i][j];
              grid[i][j] = 0;
              justShifted = true;
            }
          }
        }
        checkAgain = justShifted;
      }
      break;
    case LEFT:
      //Shift left
      while(checkAgain) {
        justShifted = false;
        for (int i = 0; i < ROW; i++) {
          for (int j = 1; j < COLUMN; j++) {
            if (grid[i][j] && (grid[i][j-1] == 0 || grid[i][j-1] == grid[i][j])) {
              grid[i][j-1] += grid[i][j];
              grid[i][j] = 0;
              justShifted = true;
            }
          }
        }
        checkAgain = justShifted;
      }
      break;
    case RIGHT:
       //Shift right
       while(checkAgain) {
         justShifted = false;
         for (int i = 0; i < ROW; i++) {
          for (int j = COLUMN - 2; j >= 0; j--) {
            if (grid[i][j] && (grid[i][j+1] == 0 || grid[i][j+1] == grid[i][j])) {
              grid[i][j+1] += grid[i][j];
              grid[i][j] = 0;
              justShifted = true;
            }
          }
        }
        checkAgain = justShifted;
      }
      break;
  } 
}

void randomTwo(void)
{
  //Adds a 2 to a random empty tile on the board
  int x,y;
  do {
    x = rand() % ROW;
    y = rand() % COLUMN;
  } while (grid[x][y] != 0);
  grid[x][y] = 2; 
}

bool boardFull(void)
{
  //Return true if the board is full
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COLUMN; j++) {
      if (grid[i][j] == 0) return false;
    }
  }
  return true;
}

int main(void)
{
  bool gameOver = 0;
  int direction;
  
  //Initialize ncurses
  initscr();
  cbreak();
  keypad(stdscr, TRUE);

  while(!boardFull()) {
    randomTwo();
    printGrid();
    int c = getch();
    switch(c) {
      case KEY_UP:
        shift(UP);
        break;
      case KEY_DOWN:
        shift(DOWN);
        break;
      case KEY_LEFT:
        shift(LEFT);
        break;
      case KEY_RIGHT:
        shift(RIGHT);
        break;
      default:
        break;
    }
  }

  printw("You lost the game.\nPress any key to exit.\n");
  getch();	//wait for user input
  endwin();	//close the window
  
  return 0;
}
