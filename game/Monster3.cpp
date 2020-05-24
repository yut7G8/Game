#include"DxLib.h"

#define Monster3HPMAX 5000
#define Monster3AttackMAX 500
#define Monster3DefenceMAX 1000
#define Monster3Speed 3

extern int Monster3X, Monster3Y;
extern int Monster3W, Monster3H;
extern int Monster3HP, Monster3Attack, Monster3Defence;

extern int Block1X, Block1Y;
extern int PlayerH;

void Monster3_Initialize()
{
	int Monster3Graph;
	Monster3Graph = LoadGraph("Monster3FromLeft.jpg");
	GetGraphSize(Monster3Graph, &Monster3W, &Monster3H);
	Monster3X = 640 - Monster3W, Monster3Y = 300 - Monster3H + PlayerH;
	Monster3HP = Monster3HPMAX;
	Monster3Attack = Monster3AttackMAX;
	Monster3Defence = Monster3DefenceMAX;
}

void Monster3_Update_LeftMovement()
{
	Monster3X -= Monster3Speed;
}

void Monster3_Update_RightMovement()
{
	Monster3X += Monster3Speed;
}

void Monster3_Draw_Left()
{
	LoadGraphScreen(Monster3X, Monster3Y, "Monster3FromLeft.jpg", TRUE);
}

void Monster3_Draw_Right()
{
	LoadGraphScreen(Monster3X, Monster3Y, "Monster3FromRight.jpg", TRUE);
}

void Monster3_Status_Draw()
{
	printfDx("Monster3HP=%d\n", Monster3HP);
	printfDx("Monster3Attack=%d\n", Monster3Attack);
	printfDx("Monster3Defence=%d\n", Monster3Defence);
	printfDx("Monster3X=%d\n", Monster3X);
	printfDx("Monster3Y=%d\n", Monster3Y);
}