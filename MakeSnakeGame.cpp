#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>

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

class Gate{
private:
  int x;
  int y;
public:
  Gate(int a, int b){
    x = a;
    y = b;
  }
  int getX(){return x;}
  int getY(){return y;}
};

int main() {
  WINDOW* GameBoard;  //게임이 진행되는 창
  WINDOW* ScoreBoard; //점수가 보여지는 창
  WINDOW* MissionBoard; //

  srand(time(NULL));
  initscr();
  //resize_term(100, 100);
  noecho();
  curs_set(0);    //화면에 보이는 커서 설정, 0 : 커서 안보이게
  keypad(stdscr, true);
  timeout(200);
  start_color();
  init_pair(1, COLOR_BLUE, COLOR_YELLOW);

  list<Snake> snakes;
  list<Snake>::iterator it;

  vector<Gate> gate;

  clock_t start, end;
  bool quit = false;
  int point = 0; //게임이 끝났을 때 최종 점수.
  int GrowItem = 0;
  int PoisonItem = 0;
  int dir = 2; // 처음에는 무조건 오른쪽으로 간다.
  int end_dir = 0; // tail방향의 방향키 입력 감지
  int foodX = rand() % 38 + 1;
  int foodY = rand() % 38 + 1; //최종도착지 좌표. 랜덤으로 정해진다.
  int poisonX = rand() % 38 + 1;
  int poisonY = rand() % 38 + 1;
  int ch; //방향키 입력 받기
  int Ssize = 3;
  int batchsec = 100000;
  int sec = 10;
  int gatecnt = 0;

  for(int i = 0; i < Ssize; i++)
    snakes.push_front(Snake(Ssize+i, Ssize)); // 처음 snake의 좌표. 3개

  for(int i = 1; i < 39; i++){
    gate.push_back(Gate(0, i));
    gate.push_back(Gate(39, i));
    gate.push_back(Gate(i, 0));
    gate.push_back(Gate(i, 39));
  }

  end = clock();

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

    if(dir == 1) x--;       //move to left
    else if(dir == 2) x++;  //move to right
    else if(dir == 3) y--;  //move to down
    else if(dir == 4) y++;  //move to up

    GameBoard = newwin(40, 40, 1, 1);
    wbkgd(GameBoard, COLOR_PAIR(1));
    wattron(GameBoard, COLOR_PAIR(1));
    wborder(GameBoard, '#', '#', '#', '#', '#', '#', '#', '#');

    int gaterand1 = 3;//rand() % 152;
    int gaterand2 = 10;//rand() % 152;

    mvwaddch(GameBoard, gate[gaterand1].getY(),gate[gaterand1].getX(), '!');
    mvwaddch(GameBoard, gate[gaterand2].getY(),gate[gaterand2].getX(), '!');

    if(x == foodX && y == foodY){
      foodX = rand() % 38 + 1;
      foodY = rand() % 38 + 1;
      point++; // 목표 맞췄다면 다시 목표 설정하고 point 1점씩 증가.
      GrowItem++;
    }
    else if(x==poisonX && y==poisonY){
      poisonX = rand() % 38 + 1;
      poisonY = rand() % 38 + 1;
      snakes.pop_back();
      snakes.pop_back(); // poison 만나면 tail 잘린다.
      PoisonItem++;
      point--;
    }
    else if(x == gate[gaterand1].getX() && y == gate[gaterand1].getY()){
      snakes.pop_back();
      x = gate[gaterand2].getX();
      y = gate[gaterand2].getY();

      if (x == 0){
        dir = 2;
        x++;
      }
      else if (x == 39){
        dir = 1;
        x--;
      }
      else if (y == 0){
        dir = 4;
        y++;
      }
      else if (y == 39){
        dir = 3;
        y--;
      }
    }
    else if(x == gate[gaterand2].getX() && y == gate[gaterand2].getY()){
      snakes.pop_back();
      x = gate[gaterand1].getX();
      y = gate[gaterand1].getY();

      if (x == 0){
        dir = 2;
        x++;
      }
      else if (x == 39){
        dir = 1;
        x--;
      }
      else if (y == 0){
        dir = 4;
        y++;
      }
      else if (y == 39){
        dir = 3;
        y--;
      }
      }
    else
      snakes.pop_back(); // 좌표다 못맞추었을 때 tail자름.

    snakes.push_front(Snake(x, y)); // head 바꿔주기

    if(y > 38 || x > 38 || y < 1 || x < 1) quit = true; // GameBoard 창 크기 넘어가면 게임 끝
    //erase(); // snake의 개수 유지하고 출력해주도록 지워주기
    mvwaddch(GameBoard, foodY, foodX, 'O'); // food 지점 출력
    mvwaddch(GameBoard, poisonY, poisonX, 'X'); //poison 지점 출력
    wrefresh(GameBoard);
    start = clock();

    if((start - end) >= batchsec){ // 5초쯤 정도마다 자리 바꿈
      if((start - end) % batchsec >= 0 && (start - end) % batchsec <= 999){
        if(sec != ((start - end) / batchsec)){
          foodX = rand() % 38 + 1;
          foodY = rand() % 38 + 1;
          poisonX = rand() % 38 + 1;
          poisonY = rand() % 38 + 1;
          sec = (start - end) / batchsec;
      }
      }
    }

    for(it = snakes.begin(); it != snakes.end();it++){
      mvwaddch(GameBoard, (*it).getY(), (*it).getX(), '*'); // snake 출력
      //if((*it).getY()==y && (*it).getX()==x && it!=snakes.begin()) quit= true;
    }
    wrefresh(GameBoard);
    //ScoreBoard 설정
    ScoreBoard = newwin(18, 25, 1, 45);
    wbkgd(ScoreBoard, COLOR_PAIR(1));
    wattron(ScoreBoard, COLOR_PAIR(1));
    wborder(ScoreBoard, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(ScoreBoard, 1, 1, "<< Score Board >>");
    mvwprintw(ScoreBoard, 15, 1, "Total_Point : %i", point);
    mvwprintw(ScoreBoard, 3, 1, "GrowItem : %i", GrowItem);
    mvwprintw(ScoreBoard, 4, 1, "PoisonItem : %i", PoisonItem);
    wrefresh(ScoreBoard);
    //MissionBoard 설정
    MissionBoard = newwin(18, 25, 23, 45);
    wbkgd(MissionBoard, COLOR_PAIR(1));
    wattron(MissionBoard, COLOR_PAIR(1));
    wborder(MissionBoard, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(MissionBoard, 1, 1, "<< Mission >>");
    wrefresh(MissionBoard);

    if(snakes.size() < 3) quit = true;  //뱀 길이 3보다 작으면 종료

  }
  timeout(-1);
  erase();
  mvprintw(1, 1, "You gained a total of %i points.\n", point);
  refresh();
  getch();
  delwin(GameBoard);
  endwin();
  return 0;
}
