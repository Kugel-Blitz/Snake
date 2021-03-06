﻿#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <easyx.h>
#include <string>
#include <thread>
#include <windows.h>
#include <conio.h>
#include "resource.h"
#include "utilities.h"
#include "graphic.h"
#include "maps.h"

extern color theme[];
extern int themeNumber;
extern maps mapResource[];
extern int mapNumber;
extern int spawnNumber;
extern int map[LENGTH + 2][LENGTH + 2];

bool windowCreated = false;

int numberOfRow = 12, numberOfColumn = 12;
int fruitRow, fruitColumn;
double timePerFrame;
double accelerate(int length);
int keyToQuaternary(char input, int quaternaryVector, int length); //interpret the user input to "orientation value"
void quaternaryToVector(int quaternaryVector, int* currentRow, int* currentColumn); //manipulate the position of the snake's head directly
char coreToScreen(int number, int length, int quaternaryVector);

int main() {

	/*create the seed for randoming*/
	srand((unsigned)time(NULL));

	/*data initialization*/
	themeNumber = rand() % 9;
	mapNumber = rand() % 7;
	spawnNumber = rand() % 4;
	int quaternaryVector = mapResource[mapNumber].spawn[spawnNumber].quaternaryVector;
	int currentRow = mapResource[mapNumber].spawn[spawnNumber].spawnX,
		currentColumn = mapResource[mapNumber].spawn[spawnNumber].spawnY;
	int length = 1;
	bool fruitExists = false;
	bool hitBody = false;
	bool firstLoop = true;

	snake* oldBody = (snake*)malloc(sizeof(snake));
	oldBody->x = mapResource[mapNumber].spawn[spawnNumber].spawnX;
	oldBody->y = mapResource[mapNumber].spawn[spawnNumber].spawnY;
	oldBody->next = nullptr;
	snake* head = oldBody;
	snake* tail = oldBody;

	/*window initialization*/
	if (windowCreated == false) {
		initgraph(HORIZENTAL, VERTICAL);
		windowCreated = true;
	}
	setlinestyle(PS_NULL);

	mapInput(mapResource[mapNumber].mapMacro);

	welcome(head);

	while (true) {
		bool fruitEaten = false;

		/*detect user keyboard input, pause or go, and judge the next location of the snake's head based on it*/
		char key = '\0';
		if (key = _kbhit()) {
			key = _getch();
			if (key == ' ') {
				paused();
				_getch();
				statistics(length);
			}
			else
				quaternaryVector = keyToQuaternary(key, quaternaryVector, length);
		}

		quaternaryToVector(quaternaryVector, &currentRow, &currentColumn);

		/*place food randomly until the food is not located on the snake's body*/
		//if (firstLoop)
			//printMap();
		placeFruit(fruitExists, head);

		/*detect what exists at the next position where the snake's head locates*/
		if (currentRow == fruitRow && currentColumn == fruitColumn) {
			++length;
			fruitExists = false;
			fruitEaten = true;
			std::thread sound(eatSound);
			sound.detach();
		}
		else if (map[currentColumn][currentRow] == 1) {
			switch (quaternaryVector) {
			case 0:
				rightRectangle(head->x, head->y);
				break;
			case 1:
				upRectangle(head->x, head->y);
				break;
			case 2:
				leftRectangle(head->x, head->y);
				break;
			case 3:
				downRectangle(head->x, head->y);
				break;
			}
			break;
		}
		else {
			for (snake* tempBody = head; tempBody != tail; tempBody = tempBody->next)
				if (tempBody->x == currentRow && tempBody->y == currentColumn)
					hitBody = true;
		}

		/*the movement of snake*/
		snake* newBody = (snake*)malloc(sizeof(snake));
		newBody->x = currentRow;
		newBody->y = currentColumn;
		newBody->previous = nullptr;
		newBody->next = oldBody;
		head = newBody;
		oldBody->previous = newBody;
		if (fruitEaten == false) {
			snake* temp = tail->previous;
			clearrectangle(tail->x * CUBE, tail->y * CUBE, tail->x * CUBE + CUBE, tail->y * CUBE + CUBE);
			free(tail);
			tail = temp;
			tail->next = nullptr;
		}
		oldBody = newBody;

		/*place food randomly until the food is not located on the snake's body*/
		placeFruit(fruitExists, head);

		/*output*/
		for (snake* tempBody = head; tempBody != nullptr; tempBody = tempBody->next) {
			visualSnake(tempBody);
		}
		if (fruitEaten || firstLoop)
			statistics(length);

		if (hitBody == true)
			break;

		timePerFrame = accelerate(length);
		Sleep(timePerFrame);

		firstLoop = false;
	}

	/*judge win or loss after ending*/ //TODO need to fix
	if (length != numberOfRow * numberOfColumn) {
		char key = '\0';
		gameOver(length);
		while (key = _getch())
			if (key == ' ')
				main();
	}
	else
		youWin();
	return 0;
}
