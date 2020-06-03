#include <iostream>
#include "ncurses.h"
#include <stdlib.h>
#include <time.h>
#include <list>

using namespace std;

class Snake{
private:
  int x;
  int y;
public:
  Snake(int a, int b){
    x = a;
    y = b;
  }
  int getX(){return x;}
  int getY(){return y;}
};

int main() {
  srand(time(NULL));
  initscr();

  start_color();
  //팔레트 설정
  init_pair(1, COLOR_BLACK, COLOR_CYAN);
  init_pair(2, COLOR_YELLOW, COLOR_GREEN);

  bkgd(COLOR_PAIR(1));
  attron(COLOR_PAIR(2));
  border('*','*','*','*','*','*','*','*');
  attroff(COLOR_PAIR(2));

  noecho();
  curs_set(0);    //화면에 보이는 커서 설정, 0 : 커서 안보이게
  keypad(stdscr, true);
  timeout(200);    //수가 클 수록 느리게 작을수록 빠르게 이동, 틱

  list<Snake> snakes;
  list<Snake>::iterator it;

  bool quit = false;
  int point = 0;
  int dir = 2;
  int foodX = rand()%29 +1;
  int foodY = rand()%10 +1;
  int ch;

  for(int i = 0; i<5;i++)
    snakes.push_front(Snake(3+i, 3));

  while(!quit){
    ch = getch();
    switch (ch) {
      case KEY_LEFT:
        dir = '1';
        break;
      case KEY_RIGHT:
        dir = '2';
        break;
      case KEY_UP:
        dir = '4';
        break;
      case KEY_DOWN:
        dir = '3';
        break;
      case KEY_BACKSPACE:
         quit = true;
        break;
    }
    Snake logic = snakes.front();
    int x = logic.getX();
    int y = logic.getY();

    if(dir == 1) x--;       //move to left
    else if(dir == 2) x++;  //move to right
    else if(dir == 3) y--;  //move to down
    else if(dir == 4) y++;  //move to up

    snakes.push_front(Snake(x, y));

    if(x == foodX && y==foodY){
      foodX= rand()%29;
      foodY =rand()%10;
      point++;
    }
    else
      snakes.pop_back();

    if(y > 50 || x > 50 || y < -50 || x < -50) //충족되지 않으면 quit = true 되어 게임 끝, 다시 말해 벽에 닿으면 게임 끝
      quit = true;
    erase();
    mvaddch(foodY, foodX, 'X');
    for(it = snakes.begin(); it != snakes.end();it++){
      mvaddch((*it).getY(), (*it).getX(), 'o');
      if((*it).getY()==y && (*it).getX()==x && it!=snakes.begin()) quit= true;
    }
    mvprintw(0, 0, "You got %i points. 'q' to quit.\n", point);
    refresh();
  }
  timeout(-1);
  erase();
  mvprintw(0, 0, "You lost and gained a total of %i points.\n", point);
  refresh();
  getch();

  endwin();
  return 0;
}
