#include "DxLib.h"
#include "Player.h"
#include"Monster1.h"
#include"Monster2.h"
#include"Monster3.h"
#include<math.h>

#define Block1Amount 15
#define Block2Amount 5
#define Block2XRightEdge 320
#define GroungBlockAmount 100
#define Shot2Damage 150
#define PlayerXBack 50
#define PlayerMaxFlag 5
#define PlayerShotSpeed 3
#define Monster3Back 20
#define Block3Amount 4
#define Block3Speed 3

int PlayerX, PlayerY;
int PlayerW, PlayerH;
int PlayerHP, PlayerAttack, PlayerDefence;
int PlayerShotX, PlayerShotY;
int PlayerShotWH, PlayerShotStrLen;
int PlayerShotFlag[PlayerMaxFlag];
int PlayerDefenceX, PlayerDefenceY;

int Block1X, Block1Y;
int Block1WH, Block1StrLen;

int Block2X[Block2Amount], Block2Y[Block2Amount];
int Block2WH, Block2StrLen;
int Block2Flag[Block2Amount];
int Block2HitCount[Block2Amount];

int Monster1aX, Monster1aY;
int Monster1aW, Monster1aH;

int Monster1bX, Monster1bY;
int Monster1bW, Monster1bH;

int Monster2X, Monster2Y;
int Monster2W, Monster2H;

double Shot2X, Shot2Y;
double Shot2Sx, Shot2Sy;
int Shot2WH, Shot2StrLen;
int Shot2Flag;

int Monster3X, Monster3Y;
int Monster3W, Monster3H;
int Monster3HP, Monster3Attack, Monster3Defence;

int Block3MonveMent, Block3X, Block3Y, Block3WH, Block3StrLen;
int Block3XRightEdge;

int GameClear = 0;
int GameOver = 0;
int Gravity;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);

	Player_Initialize();
	Monster1a_Initialize();
	Monster1b_Initialize();
	Monster2_Initialize();
	Monster3_Initialize();

	Block1StrLen = strlen("□");
	Block1WH = GetDrawStringWidth("□", Block1StrLen);

	Block2StrLen = strlen("■");
	Block2WH = GetDrawStringWidth("■", Block2StrLen);

	Block3StrLen = strlen("◆");
	Block3WH = GetDrawStringWidth("◆", Block3StrLen);
	Block3Y = 640;

	Shot2StrLen = strlen("●");
	Shot2WH = GetDrawStringWidth("●", Shot2StrLen);

	PlayerShotStrLen = strlen("◎");
	PlayerShotWH = GetDrawStringWidth("◎", PlayerShotStrLen);
	for (int i = 0; i < PlayerMaxFlag; i++)
	{
		PlayerShotFlag[i] = 0;
	}
	
	//Color//
	int LIGHTBLUE = GetColor(0, 255, 235);
	int RED = GetColor(255, 0, 0);
	int BlueGreen = GetColor(0, 128, 128);

	//スタート画面//
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		DrawFormatString(320, 240, RED, "GAME START!!!");
		clsDx();
		printfDx("GAME開始にはSpaceキーを入力して下さい。");
		if (CheckHitKey(KEY_INPUT_SPACE) == 1)break;
	}
	//1面//
	while (PlayerX < 600 && ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		if (CheckHitKey(KEY_INPUT_SPACE) == 1)WaitTimer(1000);//動画撮影用。完成時には消去。
		//動作//
		Player_Update();

		//Block下判定//
		if (Block1X < PlayerX + PlayerW / 2 && PlayerY > Block1Y)
		{
			if (PlayerY < Block1Y + Block1WH)
			{
				PlayerY = Block1Y + Block1WH;
				while (PlayerY < 300)
				{
					PlayerY += Gravity;
					Gravity += 1;
				}
			}
		}
		//Block上判定//
		else if (Block1X < PlayerX + PlayerW / 2 && PlayerY < Block1Y + Block1WH)
		{
			if (PlayerY + PlayerH > Block1Y)
			{
				Gravity = 0;
				PlayerY = Block1Y - PlayerH;
			}
		}
		//以下Block上下の判定はこの原理で行っている。

		Monster1a_Update();
		Monster1b_Update();

		//描画//
		//上にあるBlock1//
		for (int i = 0; i < Block1Amount; i++)
		{
			Block1X = 640 - i * Block1WH;
			Block1Y = Monster1aY - PlayerH;
			DrawFormatString(Block1X, Block1Y, LIGHTBLUE, "□");
		}
		//下にあるBlock1//
		for (int i = 0; i < GroungBlockAmount; i++)
		{
			DrawFormatString(640 - i * Block1WH, 300 + PlayerH, LIGHTBLUE, "□");
		}
		//その他描画//
		Player_Draw_Left();
		Monster1a_Draw();
		Monster1b_Draw();
		clsDx();
		Player_Status_Draw();

		//DAMAGE CHECK!!!//
		if (((PlayerX > Monster1aX&& PlayerX < Monster1aX + Monster1aW) || (Monster1aX > PlayerX&& Monster1aX < PlayerX + PlayerW)) && ((PlayerY > Monster1aY&& PlayerY < Monster1aY + Monster1aH) || (Monster1aY > PlayerY&& Monster1aY < PlayerY + PlayerH)))
		{
			PlayerDefence -= 1;
			PlayerHP -= 1;
			if (PlayerHP <= 0)
			{
				GameOver++;
				break;
			}
		}

		if (((PlayerX > Monster1bX&& PlayerX < Monster1bX + Monster1bW) || (Monster1bX > PlayerX&& Monster1bX < PlayerX + PlayerW)) && ((PlayerY > Monster1bY&& PlayerY < Monster1bY + Monster1bH) || (Monster1bY > PlayerY&& Monster1bY < PlayerY + PlayerH)))
		{
			PlayerDefence -= 2;
			PlayerHP -= 2;
			if (PlayerHP <= 0)
			{
				GameOver++;
				break;
			}
		}
	}

	PlayerX = 0, PlayerY = 300;

	//2面。Monster2が上昇するコードと下降するコードで分けている。上昇と下降で中のコードはあまり変わらないので、下降のコード部分のコメントは省きます。//
	while (1)
	{
		//GAME OVER!//
		if (GameOver == 1)break;

		//Monster2_Update_Up
		while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
		{
			if (CheckHitKey(KEY_INPUT_SPACE) == 1)WaitTimer(1000);//動画撮影用
			if (Monster2Y < 0)break;//Monster2_Update_Downに行く。
			//動作//
			Player_Update();

			//Block2は耐久度を持たせたBlockである。個々に配列を持たせる。//
			for (int i = 0; i < Block2Amount; i++)
			{
				if (Block2Flag[i] == 0)//Block2が存在したら
				{
					if (Block2X[i] < (PlayerX + PlayerW / 2) && (PlayerX + PlayerW / 2) < (Block2XRightEdge + Block2WH) && PlayerY > Block2Y[i])
					{
						if (PlayerY < Block2Y[i] + Block2WH)
						{
							PlayerY = Block2Y[i] + Block2WH;
							while (PlayerY < 300)
							{
								PlayerY += Gravity;
								Gravity += 1;
							}
						}
					}

					else if (Block2X[i] < (PlayerX + PlayerW / 2) && (PlayerX + PlayerW / 2) < (Block2XRightEdge + Block2WH) && PlayerY < Block2Y[i] + Block2WH)
					{
						if (PlayerY + PlayerH > Block2Y[i])
						{
							Gravity = 0;
							PlayerY = Block2Y[i] - PlayerH;
						}
					}
				}
			}
			//Block2の耐久度判定。Monster2の弾のHit回数をカウントして、耐久度を作成。//
			for (int i = 0; i < Block2Amount; i++)
			{
				if (Block2Flag[i] == 0)
				{
					if (((Shot2X > Block2X[i] && Shot2X < Block2X[i] + Block2WH) || (Block2X[i] > Shot2X&& Block2X[i] < Shot2X + Shot2WH)) && ((Shot2Y > Block2Y[i] && Shot2Y < Block2Y[i] + Block2WH) || (Block2Y[i] > Shot2Y&& Block2Y[i] < Shot2Y + Shot2WH)))
					{
						Shot2Flag = 0;
						Block2HitCount[i] += 1;
					}
					if (Block2HitCount[i] > 5)
					{
						Block2Flag[i] = 1;
						break;
					}
				}
			}

			Monster2_Update_Up();

			//描画//
			//耐久度があるBlock2の描画//
			for (int i = 0; i < Block2Amount; i++)
			{
				Block2X[i] = Block2XRightEdge - i * Block2WH;
				Block2Y[i] = 300 - PlayerH / 2;
				if (Block2Flag[i] == 0)
				{
					DrawFormatString(Block2X[i], Block2Y[i], RED, "■");
				}
			}
			//耐久度無のBlock2の描画//
			for (int i = 0; i < GroungBlockAmount; i++)
			{
				DrawFormatString(640 - i * Block2WH, 300 + PlayerH, LIGHTBLUE, "□");
			}
			//その他の描画//
			Player_Draw_Left();
			Monster2_Draw();
			clsDx();
			Player_Status_Draw();

			//GAME OVER!//
			if (((PlayerX > Monster2X&& PlayerX < Monster2X + Monster2W) || (Monster2X > PlayerX&& Monster2X < PlayerX + PlayerW)) && ((PlayerY > Monster2Y&& PlayerY < Monster2Y + Monster2H) || (Monster2Y > PlayerY&& Monster2Y < PlayerY + PlayerH)))
			{
				GameOver++;
				break;
			}
			//SHOT2 CHECK!!!//
			if (((Shot2X > PlayerX&& Shot2X < PlayerX + PlayerW) || (PlayerX > Shot2X&& PlayerX < Shot2X + Shot2WH)) && ((Shot2Y > PlayerY&& Shot2Y < PlayerY + PlayerH) || (PlayerY > Shot2Y&& PlayerY < Shot2Y + Shot2WH)))
			{
				PlayerHP -= Shot2Damage - PlayerDefence;
				PlayerX -= PlayerXBack;
				Shot2Flag = 0;
				if (PlayerHP <= 0)
				{
					GameOver++;
					break;
				}
			}

			//2面 CLEAR!//
			if (PlayerX > 600)break;
		}
		//GAME OVER!//
		if (GameOver == 1)break;
		//2面 CLEAR!//
		if (PlayerX > 600)break;

		//Monster2_Update_Down
		while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
		{
			if (CheckHitKey(KEY_INPUT_SPACE) == 1)WaitTimer(1000);//動画撮影用
			if (Monster2Y + Monster2H > Block1Y)break;//Monster2_Update_Upに行く。
			//動作//
			Player_Update();

			for (int i = 0; i < Block2Amount; i++)
			{
				if (Block2Flag[i] == 0)//Block2が存在したら
				{
					if (Block2X[i] < (PlayerX + PlayerW / 2) && (PlayerX + PlayerW / 2) < (Block2XRightEdge + Block2WH) && PlayerY > Block2Y[i])
					{
						if (PlayerY < Block2Y[i] + Block2WH)
						{
							PlayerY = Block2Y[i] + Block2WH;
							while (PlayerY < 300)
							{
								PlayerY += Gravity;
								Gravity += 1;
							}
						}
					}

					else if (Block2X[i] < (PlayerX + PlayerW / 2) && (PlayerX + PlayerW / 2) < (Block2XRightEdge + Block2WH) && PlayerY < Block2Y[i] + Block2WH)
					{
						if (PlayerY + PlayerH > Block2Y[i])
						{
							Gravity = 0;
							PlayerY = Block2Y[i] - PlayerH;
						}
					}
				}
			}

			for (int i = 0; i < Block2Amount; i++)
			{
				if (Block2Flag[i] == 0)
				{
					if (((Shot2X > Block2X[i] && Shot2X < Block2X[i] + Block2WH) || (Block2X[i] > Shot2X&& Block2X[i] < Shot2X + Shot2WH)) && ((Shot2Y > Block2Y[i] && Shot2Y < Block2Y[i] + Block2WH) || (Block2Y[i] > Shot2Y&& Block2Y[i] < Shot2Y + Shot2WH)))
					{
						Shot2Flag = 0;
						Block2HitCount[i] += 1;
					}
					if (Block2HitCount[i] > 5)
					{
						Block2Flag[i] = 1;
						break;
					}
				}
			}

			Monster2_Update_Down();

			//描画//

			for (int i = 0; i < Block2Amount; i++)
			{
				Block2X[i] = Block2XRightEdge - i * Block2WH;
				Block2Y[i] = 300 - PlayerH / 2;
				if (Block2Flag[i] == 0)
				{
					DrawFormatString(Block2X[i], Block2Y[i], RED, "■");
				}
			}

			for (int i = 0; i < GroungBlockAmount; i++)
			{
				DrawFormatString(640 - i * Block2WH, 300 + PlayerH, LIGHTBLUE, "□");
			}

			Player_Draw_Left();
			Monster2_Draw();
			clsDx();
			Player_Status_Draw();

			//GAME OVER!//
			if (((PlayerX > Monster2X&& PlayerX < Monster2X + Monster2W) || (Monster2X > PlayerX&& Monster2X < PlayerX + PlayerW)) && ((PlayerY > Monster2Y&& PlayerY < Monster2Y + Monster2H) || (Monster2Y > PlayerY&& Monster2Y < PlayerY + PlayerH)))
			{
				GameOver++;
				break;
			}
			//SHOT2 CHECK!!!//
			if (((Shot2X > PlayerX&& Shot2X < PlayerX + PlayerW) || (PlayerX > Shot2X&& PlayerX < Shot2X + Shot2WH)) && ((Shot2Y > PlayerY&& Shot2Y < PlayerY + PlayerH) || (PlayerY > Shot2Y&& PlayerY < Shot2Y + Shot2WH)))
			{
				PlayerHP -= Shot2Damage - PlayerDefence;
				PlayerX -= PlayerXBack;
				Shot2Flag = 0;
				if (PlayerHP <= 0)
				{
					GameOver++;
					break;
				}
			}

			//2面 CLEAR!//
			if (PlayerX > 600)break;
		}
		//GAME OVER!//
		if (GameOver == 1)break;
		//2面 CLEAR!//
		if (PlayerX > 600)break;

	}

	PlayerX = 0, PlayerY = 300;

	//3面//
	while (PlayerX < 600)
	{
		//GAME OVER!//
		if (GameOver == 1)break;
		while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
		{
			if (GameClear == 1)break;
			if (GameOver == 1)break;
			if (Monster3X < 0)break;
			if (PlayerX > 600)PlayerX = 600;

			Player_Update();
			Monster3_Update_LeftMovement();

			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				for (int i = 0; i < PlayerMaxFlag; i++)
				{
					if (PlayerShotFlag[i] == 0)
					{
						PlayerShotX = (PlayerW - PlayerShotWH) / 2 + PlayerX;
						PlayerShotY = (PlayerH - PlayerShotWH) / 2 + PlayerY;
						PlayerShotFlag[i] = 1;
						break;
					}
				}
			}

			for (int i = 0; i < PlayerMaxFlag; i++)
			{
				if (PlayerShotFlag[i] == 1)
				{
					//弾速//
					if (PlayerX < Monster3X + Monster3W / 2)
					{
						PlayerShotX += PlayerShotSpeed;
						//場外判定//
						if (PlayerShotX > 600)
						{
							PlayerShotFlag[i] = 0;
						}
					}
					else if (PlayerX > Monster3X + Monster3W / 2)
					{
						PlayerShotX -= PlayerShotSpeed;
						//場外判定//
						if (PlayerShotX < 0)
						{
							PlayerShotFlag[i] = 0;
						}
					}

					//Hit判定//
					if (((PlayerShotX > Monster3X&& PlayerShotX < Monster3X + Monster3W) || (Monster3X > PlayerShotX&& Monster3X < PlayerShotX + PlayerShotWH)) && ((PlayerShotY > Monster3Y&& PlayerShotY < Monster3Y + Monster3H) || (Monster3Y > PlayerShotY&& Monster3Y < PlayerShotY + PlayerShotWH)))
					{
						//HitBAck//
						if (PlayerX < Monster3X + Monster3W / 2)
						{
							Monster3X += Monster3Back;
						}
						else if (PlayerX > Monster3X + Monster3W / 2)
						{
							Monster3X -= Monster3Back;
						}
						//
						Monster3HP -= PlayerAttack;
						Monster3Defence -= 10;
						if (Monster3Defence < 0)
						{
							Monster3Defence = 0;
						}
						if (Monster3HP < 0)
						{
							GameClear++;
							break;
						}
						PlayerShotFlag[i] = 0;
					}
					DrawFormatString(PlayerShotX, PlayerShotY, LIGHTBLUE, "◎");
				}
			}

			if (((PlayerX > Monster3X&& PlayerX < Monster3X + Monster3W) || (Monster3X > PlayerX&& Monster3X < PlayerX + PlayerW)) && ((PlayerY > Monster3Y&& PlayerY < Monster3Y + Monster3H) || (Monster3Y > PlayerY&& Monster3Y < PlayerY + PlayerH)))
			{
				GameOver++;
				break;
			}

			//描画//
			for (int i = 0; i < GroungBlockAmount; i++)
			{
				DrawFormatString(640 - i * Block1WH, 300 + PlayerH, LIGHTBLUE, "□");
			}
			
			//動くBlock3//
			Block3X -= Block3Speed;
			Block3Y = Monster3Y - Block3WH;
			Block3XRightEdge = Block3X - Block3Amount * Block3WH;
			
			for (int i = 0; i < Block3Amount; i++)
			{
				DrawFormatString(Block3X - i * Block3WH, Block3Y, LIGHTBLUE, "◆");
			}

			if (Block3X < 0)
			{
				Block3X = 640;
			}

			if (Block3XRightEdge < (PlayerX + PlayerW / 2) && (PlayerX + PlayerW / 2) < (Block3X + Block3WH) && PlayerY > Block3Y)
			{
				if (PlayerY < Block3Y + Block3WH)
				{
					PlayerY = Block3Y + Block3WH;
					while (PlayerY < 300)
					{
						PlayerY += Gravity;
						Gravity += 1;
					}
				}
			}

			else if (Block3XRightEdge < (PlayerX + PlayerW / 2) && (PlayerX + PlayerW / 2) < (Block3X + Block3WH) && PlayerY < Block3Y + Block3WH)
			{
				if (PlayerY + PlayerH > Block3Y)
				{
					Gravity = 0;
					PlayerY = Block3Y - PlayerH;
					PlayerX -= 2 * Block3Speed;
				}
			}
			
			//Character描画//
			if (PlayerX < Monster3X + Monster3W / 2)
			{
				Player_Draw_Left();
			}
			else if (PlayerX > Monster3X + Monster3W / 2)
			{
				Player_Draw_Right();
			}
			Monster3_Draw_Left();
			clsDx();
			Player_Status_Draw();
			Monster3_Status_Draw();
		}

		//GAME CLEAR!//
		if (GameClear == 1)break;
		//GAME OVER!//
		if (GameOver == 1)break;

		while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
		{
			if (GameClear == 1)break;
			if (GameOver == 1)break;
			if (Monster3X > 600)break;
			if (PlayerX > 600)PlayerX = 600;
		
			Player_Update();
			Monster3_Update_RightMovement();

			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				for (int i = 0; i < PlayerMaxFlag; i++)
				{
					if (PlayerShotFlag[i] == 0)
					{
						PlayerShotX = (PlayerW - PlayerShotWH) / 2 + PlayerX;
						PlayerShotY = (PlayerH - PlayerShotWH) / 2 + PlayerY;
						PlayerShotFlag[i] = 1;
						break;
					}
				}
			}
			for (int i = 0; i < PlayerMaxFlag; i++)
			{
				if (PlayerShotFlag[i] == 1)
				{
					//弾速//
					if (PlayerX < Monster3X + Monster3W / 2)
					{
						PlayerShotX += PlayerShotSpeed;
						//場外判定//
						if (PlayerShotX > 600)
						{
							PlayerShotFlag[i] = 0;
						}
					}
					else if (PlayerX > Monster3X + Monster3W / 2)
					{
						PlayerShotX -= PlayerShotSpeed;
						//場外判定//
						if (PlayerShotX < 0)
						{
							PlayerShotFlag[i] = 0;
						}
					}
					//Hit判定//
					if (PlayerShotX < 0)
					{
						PlayerShotFlag[i] = 0;
					}
					if (((PlayerShotX > Monster3X&& PlayerShotX < Monster3X + Monster3W) || (Monster3X > PlayerShotX&& Monster3X < PlayerShotX + PlayerShotWH)) && ((PlayerShotY > Monster3Y&& PlayerShotY < Monster3Y + Monster3H) || (Monster3Y > PlayerShotY&& Monster3Y < PlayerShotY + PlayerShotWH)))
					{
						//HitBAck//
						if (PlayerX < Monster3X + Monster3W / 2)
						{
							Monster3X += Monster3Back;
						}
						else if (PlayerX > Monster3X + Monster3W / 2)
						{
							Monster3X -= Monster3Back;
						}
						//
						Monster3HP -= PlayerAttack;
						Monster3Defence -= 10;
						if (Monster3Defence < 0)
						{
							Monster3Defence = 0;
						}
						if (Monster3HP < 0)
						{
							GameClear++;
							break;
						}
						PlayerShotFlag[i] = 0;
					}
					DrawFormatString(PlayerShotX, PlayerShotY, LIGHTBLUE, "◎");
				}
			}

			if (((PlayerX > Monster3X&& PlayerX < Monster3X + Monster3W) || (Monster3X > PlayerX&& Monster3X < PlayerX + PlayerW)) && ((PlayerY > Monster3Y&& PlayerY < Monster3Y + Monster3H) || (Monster3Y > PlayerY&& Monster3Y < PlayerY + PlayerH)))
			{
				GameOver++;
				break;
			}

			//描画//
			for (int i = 0; i < GroungBlockAmount; i++)
			{
				DrawFormatString(640 - i * Block1WH, 300 + PlayerH, LIGHTBLUE, "□");
			}
			//動くBlock3//
			Block3X -= Block3Speed;
			Block3Y = Monster3Y - Block3WH;
			Block3XRightEdge = Block3X - Block3Amount * Block3WH;
			
			for (int i = 0; i < Block3Amount; i++)
			{
				DrawFormatString(Block3X - i * Block3WH, Block3Y, LIGHTBLUE, "◆");
			}
			
			if (Block3X < 0)
			{
				Block3X = 640;
			}

			if (Block3XRightEdge < (PlayerX + PlayerW / 2) && (PlayerX + PlayerW / 2) < (Block3X + Block3WH) && PlayerY > Block3Y)
			{
				if (PlayerY < Block3Y + Block3WH)
				{
					PlayerY = Block3Y + Block3WH;
					while (PlayerY < 300)
					{
						PlayerY += Gravity;
						Gravity += 1;
					}
				}
			}

			else if (Block3XRightEdge < (PlayerX + PlayerW / 2) && (PlayerX + PlayerW / 2) < (Block3X + Block3WH) && PlayerY < Block3Y + Block3WH)
			{
				if (PlayerY + PlayerH > Block3Y)
				{
					Gravity = 0;
					PlayerY = Block3Y - PlayerH;
					PlayerX -= 2 * Block3Speed;
				}
			}

			//Character描画//
			if (PlayerX < Monster3X + Monster3W / 2)
			{
				Player_Draw_Left();
			}
			else if (PlayerX > Monster3X + Monster3W / 2)
			{
				Player_Draw_Right();
			}
			Monster3_Draw_Right();
			clsDx();
			Player_Status_Draw();
			Monster3_Status_Draw();
		}
	}
	while (GameOver == 1 && ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		DrawFormatString(320, 240, RED, "GAME OVER!!!");
		clsDx();
		printfDx("GAME終了にはEscapeキーを入力して下さい。");
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
	}
	while (GameClear == 1 && ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		DrawFormatString(320, 240, LIGHTBLUE, "GAME CLEAR!!!");
		clsDx();
		printfDx("GAME終了にはEscapeキーを入力して下さい。");
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
	}
	DxLib_End();
	return 0;
}