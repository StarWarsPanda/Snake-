#include <tice.h>
#include <keypadc.h>
#include <graphx.h>
#include <stdio.h>
#include <math.h>
#include <fileioc.h>
#include <Keys.h>
#include <string.h>

#include <debug.h>

#include "Color.h"

#define frameSpeed 7

struct vec2
{
	uint8_t x, y, direction;
};

struct AppVarData
{
	char type;

	uint64_t data;
};

struct AppVarDataString
{
	char type;

	char data[15];
};

void wrap(uint8_t* out, uint8_t min, uint8_t max);

char* dataToString(uint32_t data1, uint8_t data2)
{
	char* string = new char[5];

	string[0] = (char)(0xff & (data1 >> 24));
	string[1] = (char)(0xff & (data1 >> 16));
	string[2] = (char)(0xff & (data1 >> 8));
	string[3] = (char)(0xff & data1);
	string[4] = (char)(data2);

	return string;
}

int staticDraw(const char* appVarName)
{
	uint8_t returnValue = 1;

	ti_var_t data = ti_Open(appVarName, "r");
	uint16_t length = ti_GetSize(data);
	AppVarData varData[length];

	if (ti_Read(varData, sizeof(AppVarData), length, data) == length)
	{
		for (int i = 0; i < length; i++)
		{
			if (varData[i].data >> 63)
			{

				switch (varData[i].type)
				{
					case 'b':gfx_SetTextBGColor(0xff & varData[i].data);break;
					case 'c':gfx_SetColor(0xff & varData[i].data);break;
					case 'f':gfx_FillRectangle_NoClip(0xfff & (varData[i].data >> 28), 0xff & (varData[i].data >> 20), 0xfff & (varData[i].data >> 8), 0xff & varData[i].data);break;
					case 'g':gfx_SetTextFGColor(0xff & varData[i].data);break;
					case 'i':gfx_Circle_NoClip(0xfff & (varData[i].data >> 16), 0xff & (varData[i].data >> 8), 0xff & varData[i].data);break;
					case 'j':gfx_FillCircle_NoClip(0xfff & (varData[i].data >> 16), 0xff & (varData[i].data >> 8), 0xff & varData[i].data);break;
					case 'l':gfx_Line_NoClip(0xfff & (varData[i].data >> 28), 0xff & (varData[i].data >> 20), 0xfff & (varData[i].data >> 8), 0xff & varData[i].data);break;
					case 'p':gfx_PrintStringXY(dataToString(0xffffffff & (varData[i].data >> 32),0xff & (varData[i].data >> 24)), 0xfff & (varData[i].data >> 8), 0xff & varData[i].data); break;
					case 'r':gfx_Rectangle_NoClip(0xfff & (varData[i].data >> 28), 0xff & (varData[i].data >> 20), 0xfff & (varData[i].data >> 8), 0xff & varData[i].data);break;
					case 's':gfx_FillScreen(0xff & varData[i].data);break;
					case 't':gfx_FillTriangle_NoClip(0xfff & (varData[i].data >> 48), 0xff & (varData[i].data >> 40), 0xfff & (varData[i].data >> 28), 0xff & (varData[i].data >> 20), 0xfff & (varData[i].data >> 8), 0xff & varData[i].data);break;
					case 'x':gfx_SetPixel(0xfff & (varData[i].data >> 8), 0xff & varData[i].data);break;
					case 'z':gfx_SetTextScale(0xf & (varData[i].data >> 4), 0xf & varData[i].data);break;
				}
			}
			else
			{
				switch (varData[i].type)
				{
					case 'b':gfx_SetTextBGColor(0xff & varData[i].data); break;
					case 'c':gfx_SetColor(0xff & varData[i].data); break;
					case 'f':gfx_FillRectangle(0xfff & (varData[i].data >> 28), 0xff & (varData[i].data >> 20), 0xfff & (varData[i].data >> 8), 0xff & varData[i].data); break;
					case 'g':gfx_SetTextFGColor(0xff & varData[i].data); break;
					case 'i':gfx_Circle(0xfff & (varData[i].data >> 16), 0xff & (varData[i].data >> 8), 0xff & varData[i].data); break;
					case 'j':gfx_FillCircle(0xfff & (varData[i].data >> 16), 0xff & (varData[i].data >> 8), 0xff & varData[i].data); break;
					case 'l':gfx_Line(0xfff & (varData[i].data >> 28), 0xff & (varData[i].data >> 20), 0xfff & (varData[i].data >> 8), 0xff & varData[i].data); break;
					case 'p':gfx_PrintStringXY(dataToString(0xffffffff & (varData[i].data >> 32), 0xff & (varData[i].data >> 24)), 0xfff & (varData[i].data >> 8), 0xff & varData[i].data); break;
					case 'r':gfx_Rectangle(0xfff & (varData[i].data >> 28), 0xff & (varData[i].data >> 20), 0xfff & (varData[i].data >> 8), 0xff & varData[i].data); break;
					case 's':gfx_FillScreen(0xff & varData[i].data); break;
					case 't':gfx_FillTriangle(0xfff & (varData[i].data >> 48), 0xff & (varData[i].data >> 40), 0xfff & (varData[i].data >> 28), 0xff & (varData[i].data >> 20), 0xfff & (varData[i].data >> 8), 0xff & varData[i].data); break;
					case 'x':gfx_SetPixel(0xfff & (varData[i].data >> 8), 0xff & varData[i].data); break;
					case 'z':gfx_SetTextScale(0xf & (varData[i].data >> 4), 0xf & varData[i].data); break;
				}
			}
		}
	}
	else
	{
		returnValue = 0;
	}

	ti_Close(data);
	return returnValue;
}

#pragma endregion


void setup();
void menuUI();
void gmUI();
void settingsUI();
void storeUI();
void pauseUI();
void scoresUI();
void endl();

void UIChange(uint8_t& menuSelected, void* out);

void ditherHalf(uint8_t color1, uint8_t color2)
{
	int i = 0;

	if (color1 != 0) gfx_FillScreen(color1);

	gfx_SetColor(color2);

	gfx_SetPixel(319, 0);
	gfx_SetPixel(0, 239);

	for (i = 2; i < 240; i += 2)
	{
		gfx_Line_NoClip(i, 239, 0, 239 - i);
	}

	for (i = 1; i < 80; i += 2)
	{
		gfx_Line_NoClip(i, 0, i + 239, 239);
	}

	for (i = 0; i < 238; i += 2)
	{
		gfx_Line_NoClip(i + 81, 0, 319, 238 - i);
	}
}

enum UI
{
	u_menu = 0,
	u_store,
	u_settings,
	u_snake,
	u_pause,
	u_scores
};

enum Dir
{
	d_none = 0,
	d_up,
	d_right,
	d_down,
	d_left
};

int main(void)
{

	setup();

	vec2 snakePos[300] = { {0,0, d_up} };
	//amount of snake between head and tail, including head
	uint16_t length = 4;

	uint16_t snakeUpdater = 1;

	//uint16_t x = 50;
	//uint8_t y = 30, w = 3, h = 3;

	//bool sec = true;

	uint8_t type = u_menu;

	ti_var_t file;
	uint16_t scores[10] = { 0 };

	ti_CloseAll();

	file = ti_Open("SnakePP", "r");

	if (file)
		ti_Read(scores, sizeof(uint16_t) * 10, 1, file);

	bool paused = false;
	bool classicEnabled = false;
	bool isCollidedWithWall = false;

	vec2 apple;
	apple.x = randInt(5, 14);
	apple.y = randInt(5, 9);
	apple.direction = randInt(d_up, d_left);

	snakePos[0] = { 10,7,d_up };
	snakePos[1] = { 10,8,d_up };
	snakePos[2] = { 10,9,d_up };
	snakePos[3] = { 10,10,d_up };

	uint32_t frame = 0;
	uint16_t deltaFrame;

	uint8_t selected = 0;

	gfx_Begin();

	gfx_ZeroScreen();

	/* Set up a buffer to not have flashing */
	gfx_SetDrawBuffer();

	switch (type)
	{
		case u_menu:
			menuUI();
			selected = 0;
			break;
		case u_store:
			storeUI();
			break;
		case u_settings:
			settingsUI();
			break;
		case u_snake:
			gfx_SetColor(7);
			for (int i = 0; i < length; i++)
			{
				gfx_FillRectangle_NoClip(snakePos[i].x, snakePos[i].y, 16, 16);
			}
			break;
		case u_pause:
			pauseUI();
			break;
		case u_scores:
			scoresUI();
			break;
	}

	do
	{
		frame++;
		/* Keyboard events */
		kb_Scan();

		if (Key_Clear) type = 255;

		/*Clear screen*/
		gfx_FillScreen(255);

		switch (type)
		{
		case u_menu:
			if (Key_Down) selected = (++selected) % 4;
			if (Key_Up) selected--;
			if (selected > 3) selected = 3;

			if (Key_Enter) 
			{
				if (selected == 0)
				{
					apple.x = randInt(5, 14);
					apple.y = randInt(5, 9);

					snakePos[0] = { 10,7,d_up };
					snakePos[1] = { 10,8,d_up };
					snakePos[2] = { 10,9,d_up };
					snakePos[3] = { 10,10,d_up };

					length = 4;

					classicEnabled = false;
				}

				UIChange(selected, &type);
			}

			dbg_sprintf(dbgout, "Selected Menu Item:%d\n", selected);

			menuUI();
			gfx_SetColor(231);
			switch (selected)
			{
			case 0:
				gfx_Rectangle_NoClip(106, 78, 109, 35);
				break;
			case 1:
				gfx_SetColor(224);
				gfx_Rectangle_NoClip(106, 116, 109, 35);
				break;
			case 2:
				gfx_SetColor(224);
				gfx_Rectangle_NoClip(106, 154, 109, 35);
				break;
			case 3:
				gfx_Rectangle_NoClip(106, 192, 109, 35);
				break;
			}
			break;
		case u_store:
			type = u_menu;
			menuUI();
			//storeUI();
			break;
		case u_settings:
			type = u_menu;
			menuUI();
			//settingsUI();
			break;
		case u_snake:
		{
			if (Key_Enter && !classicEnabled) 
			{ 
				apple.x = randInt(5, 14);
				apple.y = randInt(5, 9);

				snakePos[0] = { 10,7,d_up };
				snakePos[1] = { 10,8,d_up };
				snakePos[2] = { 10,9,d_up };
				snakePos[3] = { 10,10,d_up };

				length = 4;

				frame = 0;

				classicEnabled = true;
			}
				
			if (!classicEnabled)
			{
				if (Key_Down) selected = (++selected) % 3;
				if (Key_Up) selected--;

				if (selected > 2)selected = 2;

				gmUI();

				gfx_SetColor(231);
				switch (selected)
				{
				case 0:
					gfx_Rectangle_NoClip(106, 72, 111, 41);
					break;
				case 1:
					gfx_Rectangle_NoClip(106, 117, 111, 41);
					break;
				case 2:
					gfx_Rectangle_NoClip(106, 163, 111, 41);
					break;
				}

			}

				if (classicEnabled)
				{
					deltaFrame = frame % frameSpeed;

					if (Key_Mode) paused = false;
					if (Key_Delete) paused = true;

					if (Key_Down && snakePos[0].direction != d_up) { snakePos[0].direction = d_down; snakeUpdater = 1; }
					if (Key_Left && snakePos[0].direction != d_right) { snakePos[0].direction = d_left; snakeUpdater = 1; }
					if (Key_Right && snakePos[0].direction != d_left) { snakePos[0].direction = d_right; snakeUpdater = 1; }
					if (Key_Up && snakePos[0].direction != d_down) { snakePos[0].direction = d_up; snakeUpdater = 1; }

					gfx_ZeroScreen();

					if (frame >= 0 && frame < 60) gfx_PrintStringXY("3", 150, 110);
					if (frame > 60 && frame < 120) gfx_PrintStringXY("2", 150, 110);
					if (frame > 120 && frame < 150) gfx_PrintStringXY("1", 150, 110);
					if (frame > 150 && frame < 160) gfx_PrintStringXY("GO!", 145, 110);

					gfx_SetColor(7);
					for (int i = 0; i < length; i++)
					{

						gfx_FillRectangle_NoClip(snakePos[i].x << 4, snakePos[i].y << 4, 16, 16);

						if (deltaFrame == 0 && !paused && frame > 179)
						{
							switch (snakePos[i].direction)
							{
							case d_down:
								if (++snakePos[i].y > 14) { snakePos[i].y = 0; isCollidedWithWall = true; }
								break;
							case d_left:
								if (--snakePos[i].x > 19) { snakePos[i].x = 19; isCollidedWithWall = true; }
								break;
							case d_right:
								if (++snakePos[i].x > 19) { snakePos[i].x = 0; isCollidedWithWall = true; }
								break;
							case d_up:
								if (--snakePos[i].y > 14) { snakePos[i].y = 14; isCollidedWithWall = true; }
								break;
							}

						}

						if (i > 0 && !paused)
						{
							if (snakePos[0].x == snakePos[i].x && snakePos[0].y == snakePos[i].y) classicEnabled = false;

							if (selected < 2 && snakePos[i].x == apple.x && snakePos[i].y == apple.y)
							{
								apple.x = randInt(0, 19);
								apple.y = randInt(0, 14);
							}
						}
					}

					gfx_SetColor(224);
					gfx_FillRectangle_NoClip(apple.x << 4, apple.y << 4, 16, 16);

					if (paused) pauseUI();

					if ((selected == 0 || selected == 2) && isCollidedWithWall) { type = u_menu; isCollidedWithWall = false; selected = 0; }

					if ((frame > 180) && ((frame % (5 * frameSpeed)) == 0) && (selected == 2))
					{
						/*   ____
							|4  1|
							| \/ |
							| /\ |
							|3  2|
							 ¯¯¯¯
						*/

						switch (apple.direction)
						{
						case 1:
							if (++apple.x > 19) { apple.x = 19; apple.direction = 4; }
							if (--apple.y > 14) { apple.y = 0; apple.direction = 2; }
							break;
						case 2:
							if (++apple.x > 19) { apple.x = 19; apple.direction = 3; }
							if (++apple.y > 14) { apple.y = 14; apple.direction = 1; }
							break;
						case 3:
							if (--apple.x > 19) { apple.x = 0; apple.direction = 2; }
							if (++apple.y > 14) { apple.y = 14; apple.direction = 4; }
							break;
						case 4:
							if (--apple.x > 19) { apple.x = 0; apple.direction = 1; }
							if (--apple.y > 14) { apple.y = 0; apple.direction = 3; }
							break;
						}
					}

					if ((frame > 180) && (deltaFrame == 0) && (!paused))
					{
						
						

						for (int i = length - 1; i > 0; i--)
						{
							snakePos[i].direction = snakePos[i - 1].direction;
						}

						if (snakePos[0].x == apple.x && snakePos[0].y == apple.y)
						{
							vec2 lastSnakePart = snakePos[length - 1];
							switch (lastSnakePart.direction)
							{
							case d_down:
								lastSnakePart.y--;
								break;
							case d_left:
								lastSnakePart.x++;
								break;
							case d_right:
								lastSnakePart.x--;
								break;
							case d_up:
								lastSnakePart.y++;
								break;
							}
							length++;
							snakePos[length - 1] = lastSnakePart;
							apple.x = randInt(0, 19);
							apple.y = randInt(0, 14);
							if (selected == 2) apple.direction = randInt(d_up, d_left);
						}

					}

				}
			}
			break;
		case u_scores:
			scoresUI();
			break;
		}

        /* Swap the buffer and the screen to update the frame */
		gfx_SwapDraw();

		dbg_sprintf(dbgout, "Current UI Index:%d\n", type);

	} while (!(type == 255));

	/*
	file = ti_Open("SnakePP", "w");
	if (file)
		ti_Write(&data, sizeof(data), 1, file);
	*/

	gfx_End();
	endl();

	return 0;
}

void setup()
{
	ti_CloseAll();

	dbg_Debugger();

	dbg_ClearConsole();

	dbg_sprintf(dbgout, "Debugger ready!\n");

	srand(os_ApdTimer);
	srandom(os_ApdTimer);

	gfx_SetTextTransparentColor(0);
	gfx_SetTextBGColor(0);
	gfx_SetTextFGColor(255);
	gfx_SetTextScale(3, 3);

}

void endl()
{
	ti_CloseAll();
}

/* All the static UIs */
void menuUI()
{
	ditherHalf(33, 41);

	gfx_SetTextScale(3, 3);
	gfx_PrintStringXY("Snake++", 80, 23);

	gfx_SetColor(255);

	gfx_Rectangle_NoClip(106, 78, 109, 35);
	gfx_Rectangle_NoClip(106, 116, 109, 35);
	gfx_Rectangle_NoClip(106, 154, 109, 35);
	gfx_Rectangle_NoClip(106, 192, 109, 35);

	gfx_SetColor(38);

	gfx_FillRectangle_NoClip(107, 79, 107, 33);
	gfx_FillRectangle_NoClip(107, 117, 107, 33);
	gfx_FillRectangle_NoClip(107, 155, 107, 33);
	gfx_FillRectangle_NoClip(107, 193, 107, 33);

	gfx_SetTextScale(1, 1);

	gfx_PrintStringXY("Game Modes", 125, 92);
	gfx_PrintStringXY("Store", 140, 130);
	gfx_PrintStringXY("Settings", 132, 168);
	gfx_PrintStringXY("Quit", 145, 206);

	gfx_SetTextScale(3, 3);
}

void gmUI()
{
	ditherHalf(41, 33);

	gfx_SetColor(255);

	gfx_Rectangle_NoClip(106, 72, 111, 41);
	gfx_Rectangle_NoClip(106, 117, 111, 41);
	gfx_Rectangle_NoClip(106, 163, 111, 41);

	gfx_SetColor(38);

	gfx_FillRectangle_NoClip(107, 73, 109, 39);
	gfx_FillRectangle_NoClip(107, 118, 109, 39);
	gfx_FillRectangle_NoClip(107, 164, 109, 39);

	gfx_SetTextScale(3, 3);

	gfx_PrintStringXY("Game Modes", 55, 17);

	gfx_SetTextScale(1, 1);

	gfx_PrintStringXY("Classic", 136, 88);
	gfx_PrintStringXY("Wrap", 145, 133);
	gfx_PrintStringXY("Winged", 140, 181);
}

void storeUI()
{

	ditherHalf(41, 33);

	gfx_SetColor(231);
	gfx_FillRectangle_NoClip(217, 4, 16, 16);

	gfx_SetColor(255);

	gfx_Rectangle_NoClip(9, 30, 70, 26);
	gfx_Rectangle_NoClip(83, 30, 112, 26);
	gfx_Rectangle_NoClip(200, 30, 112, 26);
	gfx_Rectangle_NoClip(9, 59, 70, 172);
	gfx_Rectangle_NoClip(83, 59, 112, 172);
	gfx_Rectangle_NoClip(200, 59, 112, 172);

	gfx_SetColor(10);

	gfx_FillRectangle_NoClip(10, 31, 68, 24);
	gfx_FillRectangle_NoClip(84, 31, 110, 24);
	gfx_FillRectangle_NoClip(201, 31, 110, 24);
	gfx_FillRectangle_NoClip(10, 60, 68, 170);
	gfx_FillRectangle_NoClip(84, 60, 110, 170);
	gfx_FillRectangle_NoClip(201, 60, 110, 170);

	gfx_SetTextScale(1, 1);

	gfx_PrintStringXY("POWER", 25, 34);
	gfx_PrintStringXY("UPs", 34, 45);

	gfx_PrintStringXY("SNAKE", 118, 34);
	gfx_PrintStringXY("CUSTOMIZATION", 88, 45);

	gfx_PrintStringXY("ENVIRONMENT", 214, 34);
	gfx_PrintStringXY("CUSTOMIZATION", 205, 45);
}

void settingsUI()
{
	ditherHalf(41, 33);

	gfx_SetColor(255);

	gfx_Rectangle_NoClip(254, 210, 62, 26);
	gfx_Rectangle_NoClip(12, 10, 87, 26);
	gfx_FillRectangle_NoClip(15, 45, 81, 5);
	gfx_Rectangle_NoClip(104, 10, 87, 26);
	gfx_Rectangle_NoClip(126, 40, 16, 16);
	gfx_Rectangle_NoClip(152, 40, 16, 16);
	gfx_Rectangle_NoClip(12, 63, 91, 13);

	gfx_SetColor(10);

	gfx_FillRectangle_NoClip(255, 211, 60, 24);
	gfx_FillRectangle_NoClip(13, 11, 85, 24);
	gfx_FillRectangle_NoClip(105, 11, 85, 24);
	gfx_FillRectangle_NoClip(13, 64, 89, 11);

	gfx_SetTextScale(1, 1);

	gfx_PrintStringXY("RETURN", 261, 219);

	gfx_PrintStringXY("SCREEN", 31, 14);
	gfx_PrintStringXY("BRIGHTNESS", 16, 25);

	gfx_PrintStringXY("BACKGROUND", 108, 14);
	gfx_PrintStringXY("COLORS", 123, 25);
}

void pauseUI()
{
	ditherHalf(0, 41);

	gfx_SetColor(10);
	gfx_FillRectangle_NoClip(70, 47, 180, 146);

	gfx_SetColor(255);
	gfx_Rectangle_NoClip(69, 46, 182, 148);
	gfx_Rectangle_NoClip(73, 50, 174, 41);
	gfx_Rectangle_NoClip(73, 98, 174, 41);
	gfx_Rectangle_NoClip(73, 148, 174, 41);

	gfx_SetTextScale(3, 3);

	gfx_PrintStringXY("RESUME", 90, 59);
	gfx_PrintStringXY("SAVE", 112, 107);
	gfx_PrintStringXY("EXIT", 111, 158);
}

void scoresUI()
{
	ditherHalf(41, 33);
}

void UIChange(uint8_t& menuSelected, void* out)
{
	switch (menuSelected)
	{
		//"Game Modes"
		case 0:
			*((uint8_t*)out) = u_snake;
			return;
		//"Store"
		case 1:
			*((uint8_t*)out) = u_store;
			return;
		//"Settings"
		case 2:
			*((uint8_t*)out) = u_settings;
			return;
		//"Quit"
		case 3:
			*((uint8_t*)out) = 255;
			return;
	}
}