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
  noecho();
  curs_set(0);    //화면에 보이는 커서 설정, 0 : 커서 안보이게
  keypad(stdscr, true);
  timeout(200);

  list<Snake> snakes;
  list<Snake>::iterator it;

  bool quit = false;
  int point = 0; //게임이 끝났을 때 최종 점수.
  int dir = 2; // 처음에는 무조건 오른쪽으로 간다.
  int end_dir = 0; // tail방향의 방향키 입력 감지
  int foodX = rand()%29 +1;
  int foodY = rand()%10 +1; //최종도착지 좌표. 랜덤으로 정해진다.
  int poisonX = rand()%29 +1;
  int poisonY = rand()%10 +1;
  int ch; //방향키 입력 받기
  int Ssize = 3;

  for(int i = 0; i < Ssize; i++)
    snakes.push_front(Snake(Ssize+i, Ssize)); // 처음 snake의 좌표. 3개

  while(!quit){
    ch = getch();
    switch (ch) {
      case KEY_LEFT:
        if(dir!= 2 ) dir = 1;
        else end_dir = 1;
        break;
      case KEY_RIGHT:
        if(dir != 1) dir = 2;
        else end_dir = 1;
        break;
      case KEY_UP:
        if(dir != 4) dir = 3;
        else end_dir = 1;
        break;
      case KEY_DOWN:
        if(dir!= 3) dir = 4;
        else end_dir = 1;
        break;
      case 'q':
        quit = true;
        break;
    } // dir값 지정하기 위한 switch-case문. 방향키를 입력받아 적용시키기 위함이다.

    if(end_dir == 1)
      quit = true;

    int x = 0, y = 0;
    Snake logic = snakes.front();
    x += logic.getX();
    y += logic.getY(); //head 부분 좌표

    if(dir == 1) --x;       //move to left
    else if(dir == 2) ++x;  //move to right
    else if(dir == 3) --y;  //move to down
    else if(dir == 4) ++y;  //move to up

    snakes.push_front(Snake(x, y)); // head 바꿔주기

    if(x == foodX && y == foodY){
      foodX = rand() % 29;
      foodY = rand() % 10;
      point++; // 목표 맞췄다면 다시 목표 설정하고 point 1점씩 증가.
    }
    else if(x==poisonX && y==poisonY){
      poisonX = rand() % 29;
      poisonY = rand() % 10;
      //뱀사이즈 1 줄이는 코드 작성하기
    }
    else
      snakes.pop_back(); // 좌표다 못맞추었을 때 tail자름.

    //if(y > 11 || x > 29 || y < 0 || x < -1) quit = true;
    erase(); // snake의 개수 유지하고 출력해주도록 지워주기
    mvaddch(foodY, foodX, 'O'); // food 지점 출력
    mvaddch(poisonY, poisonX, 'X'); //poison 지점 출력
    for(it = snakes.begin(); it != snakes.end();it++){
      mvaddch((*it).getY(), (*it).getX(), '*'); // snake 출력
      //if((*it).getY()==y && (*it).getX()==x && it!=snakes.begin()) quit= true;
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
