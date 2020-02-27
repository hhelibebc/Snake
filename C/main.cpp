#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <deque>

#define MAX_RC 30
enum DIRCECTION{
	LEFT,RIGHT,UP,DOWN,_LEFT = 0x4b,_RIGHT = 0x4d,_UP = 0x48,_DOWN = 0x50
}dir = RIGHT;
using namespace std;
typedef struct Point{
	char x;
	char y;
	int operator==(Point src){
		return (x == src.x && y == src.y);
	}
}Point;

deque<Point> snake;
Point food,cur = {0,15};
HANDLE hConsole;
CONSOLE_CURSOR_INFO cursorInfo = {100,FALSE};
CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
DWORD ret;
COORD coord;
char *arr[] = {"■","  ","☆"};

void Print(const char* str,Point pt){
	GetConsoleScreenBufferInfo(hConsole,&bufferInfo);
	coord.X = pt.x;
	coord.Y = pt.y;
	WriteConsoleOutputCharacterA(hConsole,str,strlen(str),coord,&ret);
}
int Travel(Point pt){
	for(int i=0;i<snake.size();i++){
		if(pt == snake[i])
			return 1;
	}
	return 0;
}
void SetFood(){
	while(1){
		food.x = (rand()%MAX_RC) *2;
		food.y = rand() & MAX_RC;
		if(!Travel(food)) break;
	}
	Print(arr[2],food);
}

int main(){
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole,&cursorInfo);
	SetFood();
	snake.push_back(cur);
	while(1){
		Sleep(100);
		if(_kbhit()){
			int ch = _getch();
			ch = _getch();
			switch(ch){
			case _LEFT:
				if(dir == UP || dir == DOWN)
					dir = LEFT;
				break;
			case _RIGHT:
				if(dir == UP || dir == DOWN)
					dir = RIGHT;
				break;
			case _UP:
				if(dir == LEFT || dir == RIGHT)
					dir = UP;
				break;
			case _DOWN:
				if(dir == LEFT || dir == RIGHT)
					dir = DOWN;
				break;
			}
		}
		switch(dir){
			case LEFT:
				cur.x -= 2;
				if(cur.x < 0)
					cur.x = MAX_RC*2-2;
				break;
			case RIGHT:
				cur.x += 2;
				if(cur.x > MAX_RC*2-2)
					cur.x = 0;
				break;
			case UP:
				cur.y -= 1;
				if(cur.y < 0)
					cur.y = MAX_RC - 1;
				break;
			case DOWN:
				cur.y += 1;
				if(cur.y > MAX_RC - 1)
					cur.y = 0;
				break;
		}
		if(!Travel(cur)){
			snake.push_back(cur);
			Print(arr[0],cur);
			if(Travel(food)){
				SetFood();
			}
			else{
				Print(arr[1],snake[0]);
				snake.pop_front();
			}
		}
		else{
			printf("GameOver!\nGameOver!\nGameOver!\nGameOver!\n");
			break;
		}
	}
	system("pause");
	return 0;
}