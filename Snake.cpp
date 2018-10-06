#include "graphics.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define max_Y 525

enum Status
{
	LEFT, RIGHT, UP, DOWN
};
struct Point
{
	int x, y;
};
void StartGame(int &ready)
{
	ready = 0;
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	settextstyle(0, 0, 3);
	setcolor(LIGHTBLUE);
	outtextxy(getmaxx() / 2, max_Y / 2 + 10, "An phim SPACE de bat dau choi!");
	int c;
	do{
		c = getch();
	} while (c != 32);
	ready = 1;
}
void DrawLine()
{
	setcolor(GREEN);
	line(0, max_Y, getmaxx(), max_Y);
	line(0, max_Y - 1, getmaxx(), max_Y - 1);
}
//===========================================SNAKE========================================================================
struct Snake
{
	Point _body[1000];
	int _length, _height;
	Status _status;
};
void InitSnake(Snake &s)
{
	s._length = 3; s._height = 10;
	int dx = 0;
	s._status = RIGHT;
	for (int i = s._length - 1; i >= 0; i--)
	{
		s._body[i].x = dx;
		s._body[i].y = 0;
		dx += s._height + 2;
	}
}
void DrawSnake(Snake s)
{
	setfillstyle(SOLID_FILL, RED);
	for (int i = 1; i<s._length; i++)
		bar(s._body[i].x, s._body[i].y, s._body[i].x + s._height, s._body[i].y + s._height);
	setfillstyle(SOLID_FILL, BLUE);
	bar(s._body[0].x, s._body[0].y, s._body[0].x + s._height, s._body[0].y + s._height);
}
void SnakeRun(Snake &s)
{
	for (int i = s._length - 1; i>0; i--)
		s._body[i] = s._body[i - 1];
	int dx = s._height + 2;
	switch (s._status)
	{
	case LEFT: s._body[0].x -= dx; break;
	case RIGHT: s._body[0].x += dx; break;
	case UP: s._body[0].y -= dx; break;
	case DOWN: s._body[0].y += dx; break;
	}
}
//============================================Food===========================================
struct Food
{
	Point _location;
	int _color, _radius;
};
void DrawFood(Food food)
{
	setfillstyle(SOLID_FILL, food._color);
	setcolor(food._color);
	pieslice(food._location.x, food._location.y, 0, 360, food._radius);
}
void MakeNewFood(Food &food)
{
	srand(time(NULL));
	int x = rand() % 12 * 65 + 5;
	int y = rand() % 12 * 42 + 5;
	food._location = { x, y };
}
void InitFood(Food &food)
{
	MakeNewFood(food);
	food._color = YELLOW;
	food._radius = 5;
}
//================================================Xu ly Game======================================================
void DrawScore(int score)
{
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(3, 0, 3);
	setcolor(MAGENTA);
	char *s1 = new char[15]; strcpy(s1, "DIEM: ");
	char *s2 = new char[8];
	itoa(score, s2, 10);
	strcat(s1, s2);
	outtextxy(50, max_Y + 10, s1);
}
void SnakeCollision(Snake &s, int &ready)
{
	//Xu ly bien
	if (s._body[0].x <0)
	{
		s._body[0].x = getmaxx() - s._height - 5;
		return;
	}
	if (s._body[0].x >getmaxx())
	{
		s._body[0].x = 0;
		return;
	}
	if (s._body[0].y < 0)
	{
		s._body[0].y = max_Y - s._height - 5;
		return;
	}
	if (s._body[0].y + s._height >max_Y)
	{
		s._body[0].y = 0;
		return;
	}
	//xu ly can duoi
	for (int i = 1; i<s._length; i++)
	if (s._body[0].x == s._body[i].x && s._body[0].y == s._body[i].y)
		ready = false;
}
float khoangcach(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
void SnakeEat(Snake &s, Food &food, int &score, int &dscore, int &time)
{
	float x = s._body[0].x + s._height / 2.0;
	float y = s._body[0].y + s._height / 2.0;
	if (khoangcach(x, y, (float)food._location.x, (float)food._location.y) <= s._height / 2.0 + food._radius)
	{
		score += dscore;
		MakeNewFood(food);
		s._length++;
		dscore += 2;
		if (time>0)
			time -= 4;
	}

}
void Process(Snake &s, Food food, int &score, int &ready, int time, int dscore)
{
	while (ready)
	{
		if (!kbhit())
		{
			DrawScore(score);
			SnakeRun(s);
			DrawLine();
			SnakeCollision(s, ready);
			SnakeEat(s, food, score, dscore, time);
			DrawSnake(s);
			DrawFood(food);
			delay(time);
			cleardevice();
		}
		else
		{
			int _control = getch();
			if ((_control == 'w' || _control == 'W') && s._status != DOWN)
				s._status = UP;
			else if ((_control == 'a' || _control == 'A') && s._status != RIGHT)
				s._status = LEFT;
			else if ((_control == 's' || _control == 'S') && s._status != UP)
				s._status = DOWN;
			else if ((_control == 'd' || _control == 'D') && s._status != LEFT)
				s._status = RIGHT;
		}
	}
}
void EndGame(int score)
{
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	settextstyle(0, 0, 3);
	setcolor(WHITE);
	outtextxy(getmaxx() / 2, max_Y / 2 - 5, "BAN DA THUA!");
	char s1[8]; strcpy(s1, "DIEM CUA BAN: ");
	char s2[8];
	itoa(score, s2, 10);
	strcat(s1, s2);
	outtextxy(getmaxx() / 2, max_Y / 2 + 15, s1);
	setcolor(RED);
	outtextxy(getmaxx() / 2, max_Y / 2 + 45, "BAN MUON CHOI LAI KHONG? Y/N");
	char continu;
	do{
		continu = getch();
		if (continu == 'Y' || continu == 'y')
			return;
		if (continu == 'n' || continu == 'N')
			exit(0);
	} while (continu != 'y' && continu != 'Y' && continu != 'n' && continu != 'N');
}

int main()
{
	initwindow(800, 600, "Snake");
	while (true)
	{
		cleardevice();
		int score = 0, dscore = 5, ready, time = 100;
		Snake s;
		Food f;
		StartGame(ready);
		InitSnake(s);
		DrawSnake(s);
		InitFood(f);
		Process(s, f, score, ready, time, dscore);
		EndGame(score);
	}
	return 0;
}
