#include <iostream>
#include "SnakeGame.h"

/*
TODO LIST
- Make A MENU
- Top List
- colors
*/

using namespace std;

Snakepart :: Snakepart(int col, int row){
  x = col;
  y = row;
}
Snakepart :: Snakepart(){
  x = 0;
  y = 0;
}
SnakeGame::SnakeGame(){
  initscr();  //ncurses TUI모드 사용 시작, 가장 먼저 호출해야함
  nodelay(stdscr, true);
  keypad(stdscr, true);   //입력 시 키보드 특수 키 입력을 가능하게 설정하는 함수
  noecho(); //문자 입력 시 입력한 값을 화면에 보이지 않게함
  curs_set(0);
  getmaxyx(stdscr, maxHeight, maxWidth);

  partchar = 'x';
  oldalchar = (char)219;
  FD = '*';
  food->x = 0;
  food->y = 0;
  for(int i = 0; i<5; i++)
    snake.push_back(Snakepart(40+i, 10));

  point = 0;
  delay = 110000;
  getFood = false;
  direction = 'l';
  srand(time(0));
  putFood();

  for(int i = 0 ; i<maxWidth-1; i++){
    move(maxHeight-2, i);
    addch(oldalchar);
  }
  for(int i = 0; i<maxHeight-1; i++){
    move(i, maxWidth-2);
    addch(oldalchar);
  }
  for(int i =0; i<snake.size(); i++){
    move(snake[i].y, snake[i].x);
    addch(partchar);
  }
  move(maxHeight-1, 0);
  printw("%d", point);
  move(food->y, food->x);
  addch(FD);
  refresh();
}
SnakeGame::~SnakeGame(){
  nodelay(stdscr, false);
  getch();  //실행파일이 바로 종료되지 않게 하기 위해 사용한 것
  endwin(); //ncurses TUI 모드 사용 종료
}
void SnakeGame::putFood(){
  while (true) {
    int tempX = rand()%maxWidth+1;
    int tempY = rand()%maxHeight+1;

    for(int i =0; i<snake.size(); i++)
      if(snake[i].x ==tempX && snake[i].y == tempY) continue;
    if(tempX>=maxWidth-2 || tempY>=maxHeight-3) continue;
    food->x = tempX;
    food->y = tempY;
    break;
  }
  move(food->y, food->x);
  addch(FD);
  refresh();  //호출되기 전까지 수행했던 작업들을 스크린에 업데이트 하는 함수
}

bool SnakeGame ::collision(){
  if(snake[0].x ==0|| snake[0].x ==maxWidth-1 ||snake[0].y==0||snake[0].y==maxHeight-2)
    return true;
  for(int i = 2; i<snake.size(); i++){
    if(snake[0].x == snake[i].x && snake[i].y ==snake[0].y) return true;
  }
  if(snake[0].x == food->x && snake[0].y == food->y){
    getFood =true;
    putFood();
    point+=10;
    move(maxHeight-1, 0);
    printw("%d", point);
    if(point%100==0) delay-=10000;
  }
  else
    getFood = false;

  return false;
}
//입력으로 움직임 조절하기
void SnakeGame ::moveSnake(){
  //getch() : 한 문자를 입력받는 함수
  int tmp = getch();  //getch로 입력받은 값은 tmp에 저장된다.

  switch (tmp) {
    case KEY_LEFT:
      if(direction!='r') direction = 'l';
      break;
    case KEY_RIGHT:
      if(direction !='l') direction = 'r';
      break;
    case KEY_UP:
      if(direction != 'd') direction = 'u';
      break;
    case KEY_DOWN:
      if(direction!='u') direction = 'd';
      break;
    case KEY_BACKSPACE:
      direction = 'q';
      break;
  }
  if(!getFood){
    move(snake[snake.size()-1].y, snake[snake.size()-1].x);
    addch(' ');
    refresh();
    snake.pop_back();
  }
  //진행방향과 반대방향입력시 실패 추가하기
  //snake가 자신의 body와 닿으면 실패 추가하기
  if(direction =='l')
    snake.insert(snake.begin(), Snakepart(snake[0].x-1, snake[0].y));
  else if (direction =='r')
    snake.insert(snake.begin(), Snakepart(snake[0].x+1, snake[0].y));
  else if (direction =='u')
    snake.insert(snake.begin(), Snakepart(snake[0].x, snake[0].y-1));
  else if (direction =='d')
    snake.insert(snake.begin(), Snakepart(snake[0].x, snake[0].y+1));

  move(snake[0].y, snake[0].x);
  addch(partchar);
  refresh();
}
void SnakeGame::Start(){
  //Make A Menu Here

  while(true){
    if(collision()){
      move(maxWidth/2-4, maxHeight/2);
      printw("Game Over..");
      break;
    }
    moveSnake();
    if(direction =='q') break;
    usleep(delay);
  }

}
