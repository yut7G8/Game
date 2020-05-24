#include"DxLib.h"
#include"Monster2.h"
#include<math.h>

extern int Monster2X, Monster2Y;
extern int Monster2W, Monster2H;

extern double Shot2X, Shot2Y;
extern double Shot2Sx, Shot2Sy;

extern int Shot2WH, Shot2StrLen;

extern int PlayerX, PlayerY;
extern int PlayerW, PlayerH;

extern int Shot2Flag;

extern int Block1Y;

//Monster2初期データセット//
void Monster2_Initialize() 
{
	int Monster2Graph;
	Monster2Graph = LoadGraph("Monster2.jpg");
	GetGraphSize(Monster2Graph, &Monster2W, &Monster2H);
	Monster2X = 640 - Monster2W, Monster2Y = 0;
	Shot2Flag = 0;
}

//Monster2動作//
void Monster2_Update_Up()
{
	Monster2Y--;
	Monster2_Update_Shot();
	
}

void Monster2_Update_Down()
{
	Monster2Y++;
	Monster2_Update_Shot();
}

void Monster2_Update_Shot()
{
	if (Shot2Flag == 0)
	{
		Shot2X = Monster2X + Monster2W / 2 - Shot2WH / 2;
		Shot2Y = Monster2Y + Monster2H / 2 - Shot2WH / 2;
		Shot2Flag = 1;

		double PS, PSx, PSy, Px, Py, Sx, Sy;
		
		Sx = Shot2X + Shot2WH / 2;
		Sy = Shot2Y + Shot2WH / 2;

		Px = PlayerX + PlayerW / 2;
		Py = PlayerY + PlayerH / 2;

		PSx = Sx - Px;
		PSy = Sy - Py;

		PS = sqrt(PSx * PSx + PSy * PSy);

		Shot2Sx = PSx / PS * 8;
		Shot2Sy = PSy / PS * 8;
	}
	else if (Shot2Flag == 1)
	{
		Shot2X -= Shot2Sx;
		Shot2Y -= Shot2Sy;

		if (Shot2X < 0 || Shot2X>640 || Shot2Y > 300 + PlayerH || Shot2Y < 0)Shot2Flag = 0;
	}
}

//Monster2描画//
void Monster2_Draw()
{
	LoadGraphScreen(Monster2X, Monster2Y, "Monster2.jpg", TRUE);
	int RED = GetColor(255, 255, 0);
	DrawFormatString(Shot2X, Shot2Y, RED, "●");
}
