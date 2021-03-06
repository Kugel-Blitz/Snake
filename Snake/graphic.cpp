﻿#pragma once

#include <easyx.h>
#include <conio.h>
#include <math.h>
#include "graphic.h"

extern int themeNumber;

void upRightRectangle(int y, int x) {
	solidrectangle(y * CUBE + 6, x * CUBE + 0, y * CUBE + 18, x * CUBE + 18);
	solidrectangle(y * CUBE + 6, x * CUBE + 6, y * CUBE + 24, x * CUBE + 18);
}

void upLeftRectangle(int y, int x) {
	solidrectangle(y * CUBE + 6, x * CUBE + 0, y * CUBE + 18, x * CUBE + 18);
	solidrectangle(y * CUBE + 0, x * CUBE + 6, y * CUBE + 18, x * CUBE + 18);
}

void downRightRectangle(int y, int x) {
	solidrectangle(y * CUBE + 6, x * CUBE + 6, y * CUBE + 18, x * CUBE + 24);
	solidrectangle(y * CUBE + 6, x * CUBE + 6, y * CUBE + 24, x * CUBE + 18);
}

void downLeftRectangle(int y, int x) {
	solidrectangle(y * CUBE + 6, x * CUBE + 6, y * CUBE + 18, x * CUBE + 24);
	solidrectangle(y * CUBE + 0, x * CUBE + 6, y * CUBE + 18, x * CUBE + 18);
}

void downRectangle(int y, int x) {
	solidrectangle(y * CUBE + 6, x * CUBE + 6, y * CUBE + 18, x * CUBE + 24);
}

void upRectangle(int y, int x) {
	solidrectangle(y * CUBE + 6, x * CUBE + 0, y * CUBE + 18, x * CUBE + 18);
}

void rightRectangle(int y, int x) {
	solidrectangle(y * CUBE + 6, x * CUBE + 6, y * CUBE + 24, x * CUBE + 18);
}

void leftRectangle(int y, int x) {
	solidrectangle(y * CUBE + 0, x * CUBE + 6, y * CUBE + 18, x * CUBE + 18);
}

void verticalRectangle(int y, int x) {
	solidrectangle(y * CUBE + 6, x * CUBE + 0, y * CUBE + 18, x * CUBE + 24);
}

void horizontalRectangle(int y, int x) {
	solidrectangle(y * CUBE + 0, x * CUBE + 6, y * CUBE + 24, x * CUBE + 18);
}

void dotRectangle(int y, int x) {
	solidrectangle(y * CUBE + 6, x * CUBE + 6, y * CUBE + 18, x * CUBE + 18);
}

void fruitRectangle(int y, int x) {
	solidrectangle(y * CUBE + 9, x * CUBE + 9, y * CUBE + 15, x * CUBE + 15);
}

void smallFontsOutput(int height, int width) {
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = height;
	f.lfWidth = width;
	f.lfPitchAndFamily = FIXED_PITCH;
	_tcscpy_s(f.lfFaceName, _T("Small Fonts"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
}

extern int length;
void paused() {
	clearrectangle(0, 336, HORIZENTAL, VERTICAL);
	smallFontsOutput(48, 16);
	settextcolor(theme[themeNumber].foreground);
	outtextxy(54, 336, _T("GAME  PAUSED"));
	settextcolor(theme[themeNumber].accent);

	smallFontsOutput(20, 8);
	settextcolor(theme[themeNumber].foreground);
	outtextxy(46, 384, _T("PRESS ANY KEY TO CONTINUE"));
	settextcolor(theme[themeNumber].accent);
}

void gameOver(int length) {
	clearrectangle(0, 384, HORIZENTAL, VERTICAL);
	wchar_t s[5];
	wsprintf(s, L"%d", length);
	smallFontsOutput(48, 18);
	settextcolor(theme[themeNumber].accent);
	outtextxy(164 - ((int)log10(length) + 1) * 9, 336, s);

	smallFontsOutput(20, 8);
	outtextxy(14, 384, _T("GAME OVER"));
	settextcolor(theme[themeNumber].foreground);
	outtextxy(120, 384, _T("PRESS SPACE TO REPLAY"));
	settextcolor(theme[themeNumber].accent);
}

void youWin() {
	smallFontsOutput(48, 16);
	settextcolor(theme[themeNumber].foreground);
	setbkmode(TRANSPARENT);
	outtextxy(100, 144, _T("YOU  WIN"));
	settextcolor(theme[themeNumber].accent);
	setbkmode(OPAQUE);
}

void statistics(int length) {
	clearrectangle(0, 336, HORIZENTAL, VERTICAL);
	wchar_t s[5];
	wsprintf(s, L"%d", length);
	smallFontsOutput(48, 18);
	settextcolor(theme[themeNumber].foreground);
	outtextxy(164 - ((int)log10(length) + 1) * 9, 336, s);
	settextcolor(theme[themeNumber].accent);

	smallFontsOutput(20, 8);
	settextcolor(theme[themeNumber].foreground);
	outtextxy(72, 384, _T("PRESS SPACE TO PAUSE"));
	settextcolor(theme[themeNumber].accent);
}

extern maps  mapResource[];
extern int mapNumber;
void welcome(snake*& head) {
	setbkcolor(theme[themeNumber].background);
	clearrectangle(0, 0, HORIZENTAL, VERTICAL);

	printMap();
	dotRectangle(head->x, head->y);

	smallFontsOutput(20, 8);
	settextcolor(theme[themeNumber].foreground);
	outtextxy(2, 342, _T("PRESS ANY                     TO CHANGE THEME"));
	outtextxy(38, 363, _T("PRESS      OR      TO CHANGE MAP"));
	outtextxy(72, 384, _T("PRESS SPACE TO PLAY"));
	settextcolor(theme[themeNumber].accent);
	outtextxy(98, 363, _T("<"));
	outtextxy(146, 363, _T(">"));
	outtextxy(98, 342, _T("NUMBER"));

	while (true) {
		char temp = _getch();
		if (temp == ' ')
			break;
		else if (temp >= '0' && temp <= '9') {
			themeNumber = temp - '0';
			welcome(head);
			break;
		}
		else if (temp == ',' || temp == '<') {
			if (mapNumber == 0)
				mapNumber = 6;
			else
				--mapNumber;
			mapInput(mapResource[mapNumber].mapMacro);
			welcome(head);
			break;
		}
		else if (temp == '.' || temp == '>') {
			(++mapNumber) %= 7;
			mapInput(mapResource[mapNumber].mapMacro);
			welcome(head);
			break;
		}
	}
}

extern int numberOfRow, numberOfColumn;
extern int map[LENGTH + 2][LENGTH + 2];
void printMap() {
	setfillcolor(theme[themeNumber].foreground);
	for (int i = 0; i < numberOfRow + 2; i++) {
		for (int j = 0; j < numberOfColumn + 2; j++) {
			if (map[i][j] == 1) {
				//setfillstyle(BS_HATCHED, HS_DIAGCROSS);
				solidrectangle(j * CUBE, i * CUBE, j * CUBE + CUBE, i * CUBE + CUBE);
				//setfillstyle(BS_SOLID);
			}
		}
	}
	setfillcolor(theme[themeNumber].background);
	for (int i = 0; i < numberOfRow + 2; i++)
		for (int j = 0; j < numberOfColumn + 2; j++)
			if (map[i][j] == 0)
				solidrectangle(j * CUBE, i * CUBE, j * CUBE + CUBE, i * CUBE + CUBE);
	setfillcolor(theme[themeNumber].accent);
}
