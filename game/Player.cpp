#include "DxLib.h"
#include"Player.h"
#include"Monster1.h"

#define Movement 3
#define Jump 19
#define PlayerHPMAX 1000
#define PlayerAttackMAX 100
#define PlayerDefenceMAX 100
#define DefenceDistance 10

extern int PlayerX, PlayerY;
extern int PlayerW, PlayerH;

extern int PlayerHP, PlayerAttack, PlayerDefence;

extern int Block1X, Block1Y, Block1WH;
extern int Gravity;
int Key;

extern int PlayerDefenceX, PlayerDefenceY;
extern int DefenceFlag;
extern int Monster3X;
int YellowGreen = GetColor(0, 255, 0);

void Player_Initialize() //Player初期データセット//
{
	PlayerX = 0, PlayerY = 300, Gravity = 0;
	
	int PlayerGraph;
	PlayerGraph = LoadGraph("PlayerFromLeft.bmp");
	GetGraphSize(PlayerGraph, &PlayerW, &PlayerH);
	
	PlayerHP = PlayerHPMAX;
	PlayerAttack = PlayerAttackMAX;
	PlayerDefence = PlayerDefenceMAX;
}

void Player_Update()//Player動作//
{
	Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (Key & PAD_INPUT_UP)PlayerY -= Jump;
	if (Key & PAD_INPUT_DOWN)PlayerY += Movement;
	if (Key & PAD_INPUT_RIGHT)PlayerX += Movement;
	if (Key & PAD_INPUT_LEFT)PlayerX -= Movement;
	//Jump//
	PlayerY += Gravity;
	Gravity += 1;
	if (PlayerY > 300)
	{
		PlayerY = 300;
		Gravity = 0;
	}
	if ((Key & PAD_INPUT_A) && PlayerY == 300) Gravity = 20;
	//Jump＊終＊//
	if (PlayerX < 0)PlayerX = 0;
}

void Player_Defence()
{
	int DefenceCount = 0;
	PlayerDefenceX = PlayerX + DefenceDistance;
	PlayerDefenceY = PlayerY;
	if (CheckHitKey(KEY_INPUT_RETURN) == 1 && PlayerDefenceX > Monster3X)
	{
		DrawFormatString(PlayerDefenceX, PlayerDefenceY, YellowGreen, "[]");
	}
}

void Player_Draw_Left()
{
	LoadGraphScreen(PlayerX, PlayerY, "PlayerFromLeft.bmp", TRUE);
}

void Player_Draw_Right()
{
	LoadGraphScreen(PlayerX, PlayerY, "PlayerFromRight.bmp", TRUE);
}

void Player_Status_Draw()
{
	printfDx("PlayerHP:%d\n", PlayerHP);
	printfDx("PlayerAttack:%d\n", PlayerAttack);
	printfDx("PlaerDefence:%d\n", PlayerDefence);
}