/*
 *	Imagination Developer Day 2014 Buggy Challenge Simulator
 * 	© 2014 Ben Goldsworthy
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#define BLACK 0
#define WHITE 1

#define END 1
#define TJUNCTION 2
#define CROSSROAD 3
#define LEFTTURN 4
#define RIGHTTURN 5
#define STRAIGHTROAD 6
#define DEADEND 7
#define CHECKAGAINBLOCK1 8
#define CHECKAGAINBLOCK2 9
#define CHECKAGAINBLOCK3 10

#define LEFTPATH 0
#define RIGHTPATH 1
#define STRAIGHTPATH 2
#define PREVPATH 3

#define NOTTAKEN 0
#define TAKEN 1
#define NONE 2

#define LEFT 0
#define RIGHT 1
#define STRAIGHT 2
#define PREVNONE 3

#define MIDDLE2BLACKLEFTANDRIGHTWHITE (Pin1 == WHITE) && (Pin4 == WHITE) && (Pin2 == BLACK || Pin3 == BLACK)
#define ALL4BLACK (Pin1 == BLACK) && (Pin2 == BLACK) && (Pin2 == BLACK) && (Pin4 == BLACK)
#define ALL4WHITE (Pin1 == WHITE) && (Pin2 == WHITE) && (Pin3 == WHITE) && (Pin4 == WHITE)
#define LEFT3BLACKRIGHT1WHITE (((Pin1 == BLACK) && (Pin2 == BLACK)) && (Pin4 == WHITE)) || (((Pin1 == BLACK) && (Pin3 == BLACK)) && (Pin4 == WHITE))
#define LEFT1WHITERIGHT3BLACK (((Pin4 == BLACK) && (Pin2 == BLACK)) && (Pin1 == WHITE)) || ((Pin4 == BLACK) && (Pin3 == BLACK)) && (Pin1 == WHITE)
	
void MoveForward(int seconds) {
	for (int i; i < seconds; i++) {	
		printf("Action: Driving Forward for %d seconds\n", i);
	}
}

void RotateLeft() {
	printf("Action: Rotating 90° Left\n");
}

void RotateRight() {
	printf("Action: Rotating 90° Right\n");
}

int DetectJunction(int Pin1, int Pin2, int Pin3, int Pin4, bool secondCheck, int secondCheckBlock) {
	if (!secondCheck) {
		if (MIDDLE2BLACKLEFTANDRIGHTWHITE) {
			return STRAIGHTROAD;
		} else if (ALL4BLACK) {
			printf("Thought: \"I've hit a perpendicular path, but cannot tell if it is a T-junction, crossroads or the end of the maze...\"\n");
			printf("Action: Inching Forward\n");
			return CHECKAGAINBLOCK1;
		} else if (LEFT3BLACKRIGHT1WHITE) {
			printf("Thought: \"I've hit a left path, but cannot tell if it is a left turn or a T-junction...\"\n");
			printf("Action: Inching Forward\n");
			return CHECKAGAINBLOCK2;
		} else if (LEFT1WHITERIGHT3BLACK) {	
			printf("Thought: \"I've hit a right path, but cannot tell if it is a right turn or a T-junction...\"\n");
			printf("Action: Inching Forward\n");
			return CHECKAGAINBLOCK3;
		} else if (ALL4WHITE) {
			return DEADEND;
		}
	} else if (secondCheck) {
		if (secondCheckBlock == 0) {
			if (ALL4BLACK) {
				return END;
			} else if (ALL4WHITE) {
				return TJUNCTION;
			} else if (MIDDLE2BLACKLEFTANDRIGHTWHITE) {
				return CROSSROAD;
			}
		} else if (secondCheckBlock == 1) {
			if (ALL4WHITE) {
				return LEFTTURN;
			} else if (MIDDLE2BLACKLEFTANDRIGHTWHITE) {
				return TJUNCTION;
			}
		} else if (secondCheckBlock == 2) {
			if (LEFT1WHITERIGHT3BLACK) {
				return RIGHTTURN;
			} else if (MIDDLE2BLACKLEFTANDRIGHTWHITE) {
				return TJUNCTION;
			}
		}
	}
}

bool BeenHereBefore(int juncPrev) {
	if (juncPrev == PREVNONE)
		return false;
	else
		return true;
}

int main() {
	int Pin1, Pin2, Pin3, Pin4;
	int jNum = 0;
	int jUnwind[100][4];
	int direction;
	bool secondCheck = false;
	int secondCheckBlock = 0;
	int entry;

	for (int y = 0; y <= 100; y++) {
		jUnwind[y][PREVPATH] = PREVNONE;
	}

	srand(time(NULL));
	for (int x = 0; x < 50; x++)
		printf("*");
	printf("\n* Imagination Developer Day 2014 Buggy Simulator *\n");
	for (int x = 0; x < 50; x++)
		printf("*");

	for (;;) {
		printf("\n0 = Straight\n1 = Perpendicular\n2 = All White\n3 = Left Turn\n4 = Right Turn\n\n> ");
		scanf("%d", &entry);
		printf("\n");
		switch(entry)
		{
		case 0:
			Pin1 = 1;
			Pin2 = 0;
			Pin3 = 0;
			Pin4 = 1;
			break;
		case 1:
			Pin1 = 0;
			Pin2 = 0;
			Pin3 = 0;
			Pin4 = 0;
			break;
		case 2:
			Pin1 = 1;
			Pin2 = 1;
			Pin3 = 1;
			Pin4 = 1;
			break;
		case 3:
			Pin1 = 0;
			Pin2 = 0;
			Pin3 = 0;
			Pin4 = 1;
			break;
		case 4:
			Pin1 = 1;
			Pin2 = 0;
			Pin3 = 0;
			Pin4 = 0;
			break;
		default:
			printf("ERROR: Defalt Case Accessed\n");
			break;
		}

		switch(DetectJunction(Pin1, Pin2, Pin3, Pin4, secondCheck, secondCheckBlock))
		{
		case END:
			printf("Thought: \"I have reached the end, and can finally die\"\n");
			return 0;
		case TJUNCTION:
			printf("Thought: \"I'm at a T-Junction...\"\n");
			if (BeenHereBefore(jUnwind[jNum][PREVPATH])) {
				printf("Thought: \"I have been here previously...\"\n");
				if (jUnwind[jNum][PREVPATH] == LEFT) {
					printf("Thought: \"I went left last time, so I must turn 90° left to face my original direction...\"\n");
					RotateLeft();
				} else if (jUnwind[jNum][PREVPATH] == RIGHT) {
					printf("Thought: \"I went right last time, so I must turn 90° right to face my original direction...\"\n");
					RotateRight();
				}
				direction = rand() % 2;
				while (jUnwind[jNum][direction] == TAKEN) {
					direction = rand() % 2;
				}	
			} else {
				jUnwind[jNum][LEFTPATH] = jUnwind[jNum][RIGHTPATH] = NOTTAKEN;
				direction = rand() % 2;
			}
			jUnwind[jNum][direction] = TAKEN;
			if (direction == LEFT) {
				printf("Thought: \"I shall take a left...\"\n");
				jUnwind[jNum++][PREVPATH] = LEFT;
				RotateLeft();
			} else if (direction == RIGHT) {
				printf("Thought: \"I shall take a right...\"\n");
				jUnwind[jNum++][PREVPATH] = RIGHT;
				RotateRight();
			}
			secondCheck = false;
			break;
		case CROSSROAD:
			printf("Thought: \"I am at a crossroads...\"\n");
			printf("%d\n", jUnwind[jNum][PREVPATH]);
			if (BeenHereBefore(jUnwind[jNum][PREVPATH])) {
				printf("Thought: \"I have been here previously...\"\n");
				if (jUnwind[jNum][PREVPATH] == LEFT) {
					printf("Thought: \"I went left last time, so I must turn 90° left to face my original direction...\"\n");
					RotateLeft();
				} else if (jUnwind[jNum][PREVPATH] == RIGHT) {
					printf("Thought: \"I went right last time, so I must turn 90° right to face my original direction...\"\n");
					RotateRight();
				} else if (jUnwind[jNum][PREVPATH] == STRAIGHT) {
					printf("Thought: \"I went straight last time, so I must turn 180° to face my original direction...\"\n");
					RotateLeft();
					RotateLeft();
				}
				direction = rand() % 3;
				while (jUnwind[jNum][direction] == TAKEN) {
					direction = rand() % 3;
				}	
			} else {
				jUnwind[jNum][LEFTPATH] = jUnwind[jNum][RIGHTPATH] = jUnwind[jNum][STRAIGHTPATH] = NOTTAKEN;
				direction = rand() % 3;
			}

			jUnwind[jNum][direction] = TAKEN;
			if (direction == LEFT) {
				printf("Result: Going Left\n");
				jUnwind[jNum++][PREVPATH] = LEFT;
				RotateLeft();
			} else if (direction == RIGHT) {
				printf("Result: Going Right\n");
				jUnwind[jNum++][PREVPATH] = RIGHT;
				RotateRight();
			} else if (direction == STRAIGHT) {
				printf("Result: Going Straight\n");
				jUnwind[jNum++][PREVPATH] = STRAIGHT;
				MoveForward(1);
			}
			secondCheck = false;
			break;
		case LEFTTURN:
			printf("Thought: \"I am at a left turn...\"\n");
			RotateLeft();
			secondCheck = false;
			break;
		case RIGHTTURN:
			printf("Thought: \"I am at a right turn...\"\n");
			RotateRight();
			secondCheck = false;
			break;
		case STRAIGHTROAD:
			printf("Thought: \"I am on a straight...\"\n");
			MoveForward(3);
			secondCheck = false;
			break;
		case DEADEND:
			printf("Thought: \"I am at a dead end...\"\n");
			jNum--;
			RotateLeft();
			RotateLeft();
			secondCheck = false;
			break;
		case CHECKAGAINBLOCK1:
			printf("Thought: \"I should check again... (Block 1)\"\n");
			secondCheck = true;
			secondCheckBlock = 0;
			break;
		case CHECKAGAINBLOCK2:
			printf("Thought: \"I should check again... (Block 2)\"\n");
			secondCheck = true;
			secondCheckBlock = 1;
			break;
		case CHECKAGAINBLOCK3:
			printf("Thought: \"I should check again... (Block 3)\"\n");
			secondCheck = true;
			secondCheckBlock = 2;
			break;
		default:
			printf("ERROR: Default case accessed\n");
			return 1;
		}
	}	
}

