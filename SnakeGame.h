#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>

using namespace std;

#ifndef SNAKEGAME_H
#define SNAKEGAME_H

struct Snakepart{
  int x;
  int y;
  Snakepart(int col, int row);
  Snakepart();
};

class SnakeGame{
  int point, delay;
  int maxWidth, maxHeight;
  char direction, partchar, oldalchar, FD;
  bool getFood;
  Snakepart* food;
  vector<Snakepart> snake;

  void putFood();
  bool collision();
  void moveSnake();
public:
  SnakeGame();
  ~SnakeGame();
  void Start();
};
#endif
