#include"DxLib.h"

#define Monster1bXInitialValue 700

extern int Monster1aX, Monster1aY;
extern int Monster1aW, Monster1aH;

extern int Monster1bX, Monster1bY;
extern int Monster1bW, Monster1bH;

extern int Block1X, Block1Y;

extern int PlayerH;

int Monster1bGravity;

//Monster1a初期データセット//
void Monster1a_Initialize()
{
	int Monster1aGraph;
	Monster1aGraph = LoadGraph("Monster1a.jpg");
	GetGraphSize(Monster1aGraph, &Monster1aW, &Monster1aH);
	Monster1aX = 640, Monster1aY = 300 + (PlayerH - Monster1aH);
}

//Monster1b初期データセット//
void Monster1b_Initialize()
{
	int Monster1bGraph;
	Monster1bGraph = LoadGraph("Monster1b.jpg");
	GetGraphSize(Monster1bGraph, &Monster1bW, &Monster1bH);
	Monster1bX = Monster1bXInitialValue, Monster1bY = Monster1aY - PlayerH - Monster1bH;
	Monster1bGravity = 0;
}

//Monster1a動作//
void Monster1a_Update()
{
	Monster1aX--;
	if (Monster1aX == 0) Monster1aX = 640;
}

//Monster1b動作//
void Monster1b_Update()
{
	Monster1bX--;
	if (Monster1bX == 0) 
	{
		Monster1bX = Monster1bXInitialValue;
		Monster1bY = Block1Y - Monster1bH;
	}

	if (Monster1bX + Monster1bW / 2 < Block1X)
	{
		Monster1bY += Monster1bGravity;
		Monster1bGravity += 1;
		if (Monster1bY > 300)
		{
			Monster1bY = Monster1aY;
			Monster1bGravity = 0;
		}
	}
}

//Monster1a描画//
void Monster1a_Draw()
{
	LoadGraphScreen(Monster1aX, Monster1aY, "Monster1a.jpg", TRUE);
}

//Monster1b描画//
void Monster1b_Draw()
{
	LoadGraphScreen(Monster1bX, Monster1bY, "Monster1b.jpg", TRUE);
}