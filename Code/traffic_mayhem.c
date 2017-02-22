/**
* Title: traffic_mayhem.c
* Abstract: This program implements a simplified version of the classic
*			game Frogger. The player can move up, down, right, and left.
*			The goal is to obtain all required items per level. The 
*			player will to navigate through traffic while going for the goal.
*
*			Disclaimer: None of the sounds, music or sprites belong to me.
* Author: Eduardo Trejo
* Date: 12/09/2014
*/

#include <allegro.h>
#include "mappyal.h"

#define MODE GFX_SAFE
#define WIDTH 760
#define HEIGHT 750
#define RED makecol(255, 0, 0)
#define WHITE makecol(255,255,255)
#define BOTTOM 12

BITMAP *buffer;
BITMAP *frogger;
BITMAP *enemy;
BITMAP *enemy2;
BITMAP *enemy3;
BITMAP *enemy4;
BITMAP *enemy5;
BITMAP *lives;
BITMAP *items;
BITMAP *items2;
BITMAP *title;
BITMAP *temp;
SAMPLE *crash;
MIDI *music;
MIDI *music2;

// Timer variables.
clock_t start;
int counter;

int level, yoffset = BOTTOM, gear, life, limit;
int speed, levelChanged, startGame;
int gameover;
int oldpy, oldpx;
int pitch, volume, pan;
float angle, eAngle;
int obtained, obtained1, obtained2, obtained3, obtained4;
int once1, once2, once3, once4, max, won, lost;
int facing, level;

facing = 1;
startGame = 0;
levelChanged = 0;
once1 = 0, once2 = 0, once3 = 0, once4 = 0;
limit = 0;
won = 0;
max = 4;
lost = 0;
gameover = 0;
volume = 158;
pan = 128;
pitch = 1100;
life = 3;
counter = 60;
obtained = 0, obtained1 = 0, obtained2 = 0, obtained3 = 0, obtained4 = 0;

struct tagFrog
{
	int x, y;
	int width, height;
	int speed;
} myFrog;

struct tagCar
{
	int x, y;
	int width, height;
	int speed;
	int ang;
} Enemy[20];

struct tagHealth
{
	int x, y;

} Health[5];

struct tagItems
{
	int x, y;

} Items[4];

//tile grabber
BITMAP *grabframe(BITMAP *source,
	int width, int height,
	int startx, int starty,
	int columns, int frame)
{
	BITMAP *temp = create_bitmap(width, height);
	int x = startx + (frame % columns) * width;
	int y = starty + (frame / columns) * height;
	blit(source, temp, x, y, 0, 0, width, height);
	return temp;
}

int collided(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	return (blockdata->tl);
}

void drawMyFrog(float angle)
{
	// Rotates sprite depending on the angle passed in.
	rotate_sprite(buffer, frogger, myFrog.x, myFrog.y, ftofix(angle));
}

void drawEnemy()
{
	int i;
	for (i = 0; i < 20; i++)
	{
		if (i < 3)
		{
			draw_sprite(buffer, enemy, Enemy[i].x, Enemy[i].y);
		}
		else if (i < 6)
		{
			draw_sprite(buffer, enemy2, Enemy[i].x, Enemy[i].y);
		}
		else if (i < 10)
		{
			draw_sprite(buffer, enemy3, Enemy[i].x, Enemy[i].y);
		}
		else if (i < 13)
		{
			draw_sprite(buffer, enemy4, Enemy[i].x, Enemy[i].y);
		}
		else if (i < 15)
		{
			draw_sprite(buffer, enemy5, Enemy[i].x, Enemy[i].y);
		}
		else if (i < 18)
		{
			draw_sprite(buffer, enemy4, Enemy[i].x, Enemy[i].y);
		}
		else
		{
			draw_sprite(buffer, enemy5, Enemy[i].x, Enemy[i].y);
		}
	
	}

}
void drawEnemy2(float eAngle)
{

	int i;
	for (i = 0; i < 3; i++)
	{
		rotate_sprite(buffer, enemy, Enemy[i].x, Enemy[i].y, ftofix(eAngle));
	}
	for (i = 3; i < 6; i++)
	{
		rotate_sprite(buffer, enemy2, Enemy[i].x, Enemy[i].y, ftofix(eAngle));
	}
	for (i = 6; i < 10; i++)
	{
		rotate_sprite(buffer, enemy3, Enemy[i].x, Enemy[i].y, ftofix(eAngle));
	}
	for (i = 10; i < 13; i++)
	{
		rotate_sprite(buffer, enemy4, Enemy[i].x, Enemy[i].y, ftofix(eAngle));
	}
	for (i = 13; i < 15; i++)
	{
		rotate_sprite(buffer, enemy5, Enemy[i].x, Enemy[i].y, ftofix(eAngle));
	}
	for (i = 15; i < 18; i++)
	{
		rotate_sprite(buffer, enemy4, Enemy[i].x, Enemy[i].y, ftofix(eAngle));
	}
	for (i = 18; i < 20; i++)
	{
		rotate_sprite(buffer, enemy5, Enemy[i].x, Enemy[i].y, ftofix(eAngle));
	}
}

// Displays lifes left.
void drawLives()
{
	if (level == 1)
	{
		if (life == 3)
		{
			Health[0].x = 60;
			Health[0].y = SCREEN_H - 30;

			Health[1].x = 80;
			Health[1].y = SCREEN_H - 30;

			Health[2].x = 100;
			Health[2].y = SCREEN_H - 30;

			textprintf_ex(buffer, font, 10, SCREEN_H - 29, RED, -1,
				"Lives: ");

			draw_sprite(buffer, lives, Health[0].x, Health[0].y);
			draw_sprite(buffer, lives, Health[1].x, Health[1].y);
			draw_sprite(buffer, lives, Health[2].x, Health[2].y);
		}
		else if (life == 2)
		{
			Health[0].x = 60;
			Health[0].y = SCREEN_H - 30;

			Health[1].x = 80;
			Health[1].y = SCREEN_H - 30;

			textprintf_ex(buffer, font, 10, SCREEN_H - 29, RED, -1,
				"Lives: ");

			draw_sprite(buffer, lives, Health[0].x, Health[0].y);
			draw_sprite(buffer, lives, Health[1].x, Health[1].y);
		}
		else if (life == 1)
		{
			textprintf_ex(buffer, font, 10, SCREEN_H - 29, RED, -1,
				"Lives: ");

			Health[0].x = 60;
			Health[0].y = SCREEN_H - 30;

			draw_sprite(buffer, lives, Health[0].x, Health[0].y);
		}
		else
		{
			textprintf_ex(buffer, font, 10, SCREEN_H - 29, RED, -1,
				"Lives: %d", life);
		}
	}
	else
	{
		if (life == 3)
		{
			Health[0].x = 60;
			Health[0].y = 2;

			Health[1].x = 80;
			Health[1].y = 2;

			Health[2].x = 100;
			Health[2].y = 2;

			textprintf_ex(buffer, font, 10, 8, RED, -1,
				"Lives: ");

			draw_sprite(buffer, lives, Health[0].x, Health[0].y);
			draw_sprite(buffer, lives, Health[1].x, Health[1].y);
			draw_sprite(buffer, lives, Health[2].x, Health[2].y);
		}
		else if (life == 2)
		{
			Health[0].x = 60;
			Health[0].y = 2;

			Health[1].x = 80;
			Health[1].y = 2;

			textprintf_ex(buffer, font, 10, 8, RED, -1,
				"Lives: ");

			draw_sprite(buffer, lives, Health[0].x, Health[0].y);
			draw_sprite(buffer, lives, Health[1].x, Health[1].y);
		}
		else if (life == 1)
		{
			textprintf_ex(buffer, font, 10, 8, RED, -1,
				"Lives: ");

			Health[0].x = 60;
			Health[0].y = 2;

			draw_sprite(buffer, lives, Health[0].x, Health[0].y);
		}
		else
		{
			textprintf_ex(buffer, font, 10, 8, RED, -1,
				"Lives: %d", life);
		}
	}
}

// Timer. 
void timeCount()
{
	if (clock() > start + CLOCKS_PER_SEC)
	{
		counter--;
		if (counter == 0)
		{
			limit = 1;
			gameover = 1;
		}
		start = clock();
	}
}

// Prints out the status.
void status(counter, obtained, level)
{
	
	if (level == 1)
	{
		// Depending on the counter print out will differ.
		if (counter == 60)
		{
			textprintf_ex(buffer, font, 140, SCREEN_H - 29, WHITE, 0, "Time - 1:00");
			textprintf_ex(buffer, font, 250, SCREEN_H - 29, WHITE, 0, "Level: %d", level);
			textprintf_ex(buffer, font, 550, SCREEN_H - 29, RED, -1,
				"Items Recovered: %d/%d", obtained, max);
		}
		else if (counter >= 10 && counter <= 59)
		{
			textprintf_ex(buffer, font, 140, SCREEN_H - 29, WHITE, 0, "Time - 0:%d", counter);
			textprintf_ex(buffer, font, 250, SCREEN_H - 29, WHITE, 0, "Level: %d", level);
			textprintf_ex(buffer, font, 550, SCREEN_H - 29, RED, -1,
				"Items Recovered: %d/%d", obtained, max);
		}
		else
		{
			textprintf_ex(buffer, font, 140, SCREEN_H - 29, WHITE, 0, "Time - 0:0%d", counter);
			textprintf_ex(buffer, font, 250, SCREEN_H - 29, WHITE, 0, "Level: %d", level);
			textprintf_ex(buffer, font, 550, SCREEN_H - 29, RED, -1,
				"Items Recovered: %d/%d", obtained, max);
		}
	}
	else
	{
		// Depending on the counter print out will differ.
		if (counter == 60)
		{
			textprintf_ex(buffer, font, 140, 8, WHITE, 0, "Time - 1:00");
			textprintf_ex(buffer, font, 250, 8, WHITE, 0, "Level: %d", level);
			textprintf_ex(buffer, font, 550, 8, RED, -1,
				"Items Recovered: %d/%d", obtained, max);
		}
		else if (counter >= 10 && counter <= 59)
		{
			textprintf_ex(buffer, font, 140, 8, WHITE, 0, "Time - 0:%d", counter);
			textprintf_ex(buffer, font, 250, 8, WHITE, 0, "Level: %d", level);
			textprintf_ex(buffer, font, 550, 8, RED, -1,
				"Items Recovered: %d/%d", obtained, max);
		}
		else
		{
			textprintf_ex(buffer, font, 140, 8, WHITE, 0, "Time - 0:0%d", counter);
			textprintf_ex(buffer, font, 250, 8, WHITE, 0, "Level: %d", level);
			textprintf_ex(buffer, font, 550, 8, RED, -1,
				"Items Recovered: %d/%d", obtained, max);
		}
	}
	
}

// Sets up enemy location.
void setUpEnemy()
{
	if (levelChanged == 0)
	{
		// First line.
		Enemy[0].x = 280;
		Enemy[0].y = SCREEN_H - 113;
		Enemy[0].speed = 5;

		Enemy[1].x = 490;
		Enemy[1].y = SCREEN_H - 113;
		Enemy[1].speed = 5;

		Enemy[2].x = 680;
		Enemy[2].y = SCREEN_H - 113;
		Enemy[2].speed = 5;

		// Second line.
		Enemy[3].x = 280;
		Enemy[3].y = SCREEN_H - 200;
		Enemy[3].speed = 5;

		Enemy[4].x = 490;
		Enemy[4].y = SCREEN_H - 200;
		Enemy[4].speed = 5;

		Enemy[5].x = 680;
		Enemy[5].y = SCREEN_H - 200;
		Enemy[5].speed = 5;

		// Third line.
		Enemy[6].x = 280;
		Enemy[6].y = SCREEN_H - 295;
		Enemy[6].speed = 5;

		Enemy[7].x = 490;
		Enemy[7].y = SCREEN_H - 295;
		Enemy[7].speed = 5;

		Enemy[8].x = 680;
		Enemy[8].y = SCREEN_H - 295;
		Enemy[8].speed = 5;

		Enemy[9].x = 890;
		Enemy[9].y = SCREEN_H - 295;
		Enemy[9].speed = 5;

		// Fourth line.
		Enemy[10].x = 300;
		Enemy[10].y = SCREEN_H - 395;
		Enemy[10].speed = 10;

		Enemy[11].x = 540;
		Enemy[11].y = SCREEN_H - 395;
		Enemy[11].speed = 10;

		Enemy[12].x = 780;
		Enemy[12].y = SCREEN_H - 395;
		Enemy[12].speed = 10;

		// Fifth line.
		Enemy[13].x = 890;
		Enemy[13].y = SCREEN_H - 520;
		Enemy[13].speed = 10;

		Enemy[14].x = 490;
		Enemy[14].y = SCREEN_H - 520;
		Enemy[14].speed = 10;

		// Sixth line.
		Enemy[15].x = 300;
		Enemy[15].y = SCREEN_H - 620;
		Enemy[15].speed = 5;

		Enemy[16].x = 540;
		Enemy[16].y = SCREEN_H - 620;
		Enemy[16].speed = 5;

		Enemy[17].x = 780;
		Enemy[17].y = SCREEN_H - 620;
		Enemy[17].speed = 5;

		// Seventh line.
		Enemy[18].x = 800;
		Enemy[18].y = SCREEN_H - 690;
		Enemy[18].speed = 10;

		Enemy[19].x = 400;
		Enemy[19].y = SCREEN_H - 690;
		Enemy[19].speed = 10;
	}		
}
void secondWave()
{
	// First line.
	Enemy[0].x = 70;
	Enemy[0].y = SCREEN_H - 670;
	Enemy[0].speed = 5;
	eAngle = 192;
	
	Enemy[1].x = 70;
	Enemy[1].y = SCREEN_H - 530;
	Enemy[1].speed = 5;
	eAngle = 192;

	Enemy[2].x = 70;
	Enemy[2].y = SCREEN_H - 390;
	Enemy[2].speed = 5;
	eAngle = 192;

	Enemy[3].x = 170;
	Enemy[3].y = SCREEN_H - 240;
	Enemy[3].speed = 5;
	eAngle = 64;

	Enemy[4].x = 170;
	Enemy[4].y = SCREEN_H - 390;
	Enemy[4].speed = 5;
	eAngle = 64; 

	Enemy[5].x = 170;
	Enemy[5].y = SCREEN_H - 530;
	Enemy[5].speed = 5;
	eAngle = 64;

	Enemy[6].x = 270;
	Enemy[6].y = SCREEN_H - 570;
	Enemy[6].speed = 5;
	eAngle = 192;

	Enemy[7].x = 270;
	Enemy[7].y = SCREEN_H - 470;
	Enemy[7].speed = 5;
	eAngle = 192;

	Enemy[8].x = 270;
	Enemy[8].y = SCREEN_H - 270;
	Enemy[8].speed = 5;
	eAngle = 192;

	Enemy[9].x = 270;
	Enemy[9].y = SCREEN_H - 370;
	Enemy[9].speed = 5;
	eAngle = 192;

	Enemy[10].x = 370;
	Enemy[10].y = SCREEN_H - 670;
	Enemy[10].speed = 10;
	eAngle = 192;

	Enemy[11].x = 370;
	Enemy[11].y = SCREEN_H - 530;
	Enemy[11].speed = 10;
	eAngle = 192;

	Enemy[12].x = 370;
	Enemy[12].y = SCREEN_H - 390;
	Enemy[12].speed = 10;
	eAngle = 192;

	Enemy[13].x = 450;
	Enemy[13].y = SCREEN_H - 240;
	Enemy[13].speed = 5;
	eAngle = 64;

	Enemy[14].x = 450;
	Enemy[14].y = SCREEN_H - 390;
	Enemy[14].speed = 5;
	eAngle = 64;

	Enemy[15].x = 555;
	Enemy[15].y = SCREEN_H - 670;
	Enemy[15].speed = 5;
	eAngle = 64;

	Enemy[16].x = 555;
	Enemy[16].y = SCREEN_H - 570;
	Enemy[16].speed = 5;
	eAngle = 192;

	Enemy[17].x = 555;
	Enemy[17].y = SCREEN_H - 470;
	Enemy[17].speed = 5;
	eAngle = 192;

	Enemy[18].x = 650;
	Enemy[18].y = SCREEN_H - 270;
	Enemy[18].speed = 10;
	eAngle = 64;

	Enemy[19].x = 650;
	Enemy[19].y = SCREEN_H - 370;
	Enemy[19].speed = 10;
	eAngle = 64;
}
// Display items to obtain.
void drawItems()
{
	if (!obtained1)
	{	
		if (level == 1)
		{
			Items[0].x = 110;
			Items[0].y = 15;
			draw_sprite(buffer, items, Items[0].x, Items[0].y);
		}
		else
		{
			Items[0].x = 75;
			Items[0].y = SCREEN_H - 40;
			draw_sprite(buffer, items2, Items[0].x, Items[0].y);
		}
	
	}
	if (!obtained2)
	{
		if (level == 1)
		{
			Items[1].x = 270;
			Items[1].y = 15;
			draw_sprite(buffer, items, Items[1].x, Items[1].y);
		}
		else
		{
			Items[1].x = 275;
			Items[1].y = 25;
			draw_sprite(buffer, items2, Items[1].x, Items[1].y);
		}
		
	}
	if (!obtained3)
	{
		if (level == 1)
		{
			Items[2].x = 462;
			Items[2].y = 15;
			draw_sprite(buffer, items, Items[2].x, Items[2].y);
		}
		else
		{
			Items[2].x = 470;
			Items[2].y = SCREEN_H / 2 - 4;
			draw_sprite(buffer, items2, Items[2].x, Items[2].y);
		}
		
	}
	if (!obtained4)
	{
		if (level == 1)
		{
			Items[3].x = 622;
			Items[3].y = 15;
			draw_sprite(buffer, items, Items[3].x, Items[3].y);
		}
		else
		{
			Items[3].x = 665;
			Items[3].y = 190;
			draw_sprite(buffer, items2, Items[3].x, Items[3].y);
		}
	
	}
}

// Detect if item has been obtained.
void itemCollision()
{
	// Top Left.
	if (Items[0].x > myFrog.x && Items[0].x < myFrog.x + frogger->w
		&& Items[0].y > myFrog.y && Items[0].y < myFrog.y + frogger->h)
	{
		obtained1 = 1;
		if (once1 == 0)
		{
			obtained++;
			once1 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}	
	// Top Right.
	if (Items[0].x + items->w > myFrog.x && Items[0].x + items->w < myFrog.x + frogger->w
		&& Items[0].y > myFrog.y && Items[0].y < myFrog.y + frogger->h)
	{
		obtained1 = 1;
		if (once1 == 0)
		{
			obtained++;
			once1 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}
	// Botton left collision.,
	if (Items[0].x > myFrog.x  && Items[0].x < myFrog.x + frogger->w
		&& Items[0].y + items->h > myFrog.y && Items[0].y + items->h < myFrog.y + frogger->h)
	{
		obtained1 = 1;
		if (once1 == 0)
		{
			obtained++;
			once1 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}
	// Bottom right collision.
	if (Items[0].x + items->w > myFrog.x && Items[0].x + items->w < myFrog.x + frogger->w
		&& Items[0].y + items->h > myFrog.y && Items[0].y + items->h < myFrog.y + frogger->h)
	{
		obtained1 = 1;
		if (once1 == 0)
		{
			obtained++;
			once1 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}
	
	// Top Left.
	if (Items[1].x > myFrog.x && Items[1].x < myFrog.x + frogger->w
		&& Items[1].y > myFrog.y && Items[1].y < myFrog.y + frogger->h)
	{
		obtained2 = 1;

		if (once2 == 0)
		{
			obtained++;
			once2 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
		
	}
	// Top Right.
	if (Items[1].x + items->w > myFrog.x && Items[1].x + items->w < myFrog.x + frogger->w
		&& Items[1].y > myFrog.y && Items[1].y < myFrog.y + frogger->h)
	{
		obtained2 = 1;
		if (once2 == 0)
		{
			obtained++;
			once2 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}
	// Botton left collision.,
	if (Items[1].x > myFrog.x  && Items[1].x < myFrog.x + frogger->w
		&& Items[1].y + items->h > myFrog.y && Items[1].y + items->h < myFrog.y + frogger->h)
	{
		obtained2 = 1;
		if (once2 == 0)
		{
			obtained++;
			once2 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}
	// Bottom right collision.
	if (Items[1].x + items->w > myFrog.x && Items[1].x + items->w < myFrog.x + frogger->w
		&& Items[1].y + items->h > myFrog.y && Items[1].y + items->h < myFrog.y + frogger->h)
	{
		obtained2 = 1;
		if (once2 == 0)
		{
			obtained++;
			once2 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}

	// Top Left.
	if (Items[2].x > myFrog.x && Items[2].x < myFrog.x + frogger->w
		&& Items[2].y > myFrog.y && Items[2].y < myFrog.y + frogger->h)
	{
		obtained3 = 1;
		if (once3 == 0)
		{
			obtained++;
			once3 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}

	}
	// Top Right.
	if (Items[2].x + items->w > myFrog.x && Items[2].x + items->w < myFrog.x + frogger->w
		&& Items[2].y > myFrog.y && Items[2].y < myFrog.y + frogger->h)
	{
		obtained3 = 1;
		if (once3 == 0)
		{
			obtained++;
			once3 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}
	// Botton left collision.,
	if (Items[2].x > myFrog.x  && Items[2].x < myFrog.x + frogger->w
		&& Items[2].y + items->h > myFrog.y && Items[2].y + items->h < myFrog.y + frogger->h)
	{
		obtained3 = 1;
		if (once3 == 0)
		{
			obtained++;
			once3 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}
	// Bottom right collision.
	if (Items[2].x + items->w > myFrog.x && Items[2].x + items->w < myFrog.x + frogger->w
		&& Items[2].y + items->h > myFrog.y && Items[2].y + items->h < myFrog.y + frogger->h)
	{
		obtained3 = 1;
		if (once3 == 0)
		{
			obtained++;
			once3 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}

	// Top Left.
	if (Items[3].x > myFrog.x && Items[3].x < myFrog.x + frogger->w
		&& Items[3].y > myFrog.y && Items[3].y < myFrog.y + frogger->h)
	{
		obtained4 = 1;
		if (once4 == 0)
		{
			obtained++;
			once4 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}

	}
	// Top Right.
	if (Items[3].x + items->w > myFrog.x && Items[3].x + items->w < myFrog.x + frogger->w
		&& Items[3].y > myFrog.y && Items[3].y < myFrog.y + frogger->h)
	{
		obtained4 = 1;
		if (once4 == 0)
		{
			obtained++;
			once4 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}
	// Botton left collision.,
	if (Items[3].x > myFrog.x  && Items[3].x < myFrog.x + frogger->w
		&& Items[3].y + items->h > myFrog.y && Items[3].y + items->h < myFrog.y + frogger->h)
	{
		obtained4 = 1;
		if (once4 == 0)
		{
			obtained++;
			once4 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}
	// Bottom right collision.
	if (Items[3].x + items->w > myFrog.x && Items[3].x + items->w < myFrog.x + frogger->w
		&& Items[3].y + items->h > myFrog.y && Items[3].y + items->h < myFrog.y + frogger->h)
	{
		obtained4 = 1;
		if (once4 == 0)
		{
			obtained++;
			once4 = 1;
			angle = 0;
			myFrog.x = 350;
			myFrog.y = (SCREEN_H - frogger->h) - 30;
		}
	}
}

// Move enemies.
void moveEnemy()
{
	int i;

	if (level == 1)
	{
		// First line.
		for (i = 0; i < 3; i++)
		{
			Enemy[i].x -= Enemy[i].speed;

			if (Enemy[i].x == -30)
			{
				Enemy[i].x = 780;
			}
		}

		// Second line.
		for (i = 3; i < 6; i++)
		{
			Enemy[i].x += Enemy[i].speed;

			if (Enemy[i].x == 790)
			{
				Enemy[i].x = -10;
			}
		}

		// Third line.
		for (i = 6; i < 10; i++)
		{

			Enemy[i].x -= Enemy[i].speed;

			if (Enemy[i].x == -30)
			{
				Enemy[i].x = 780;
			}
		}

		// Fourth line.
		for (i = 10; i < 13; i++)
		{
			Enemy[i].x += Enemy[i].speed;

			if (Enemy[i].x == 790)
			{
				Enemy[i].x = -10;
			}
		}

		// Fifth line.
		for (i = 13; i < 15; i++)
		{

			Enemy[i].x -= Enemy[i].speed;

			if (Enemy[i].x == -60)
			{
				Enemy[i].x = 780;
			}
		}

		// Sixth line.
		for (i = 15; i < 18; i++)
		{
			Enemy[i].x += Enemy[i].speed;

			if (Enemy[i].x == 790)
			{
				Enemy[i].x = -10;
			}
		}

		// Seventh line.
		for (i = 18; i < 20; i++)
		{

			Enemy[i].x -= Enemy[i].speed;

			if (Enemy[i].x == -60)
			{
				Enemy[i].x = 780;
			}
		}
	
	}
	else
	{
		// First line.
		for (i = 0; i < 3; i++)
		{
			Enemy[i].y -= Enemy[i].speed;

			if (Enemy[i].y == -30)
			{
				Enemy[i].y = 780;
			}
		}

		// Second line.
		for (i = 3; i < 6; i++)
		{
			Enemy[i].y += Enemy[i].speed;

			if (Enemy[i].y == 790)
			{
				Enemy[i].y = -30;
			}
		}
		
		// Third line.
		for (i = 6; i < 10; i++)
		{

			Enemy[i].y -= Enemy[i].speed;

			if (Enemy[i].y == -30)
			{
				Enemy[i].y = 780;
			}
		}

		// Fourth line.
		for (i = 10; i < 13; i++)
		{
			Enemy[i].y += Enemy[i].speed;

			if (Enemy[i].y == 790)
			{
				Enemy[i].y = -30;
			}
		}

		// Fifth line.
		for (i = 13; i < 15; i++)
		{

			Enemy[i].y -= Enemy[i].speed;

			if (Enemy[i].y == -60)
			{
				Enemy[i].y = 780;
			}
		}

		// Sixth line.
		for (i = 15; i < 18; i++)
		{
			Enemy[i].y += Enemy[i].speed;

			if (Enemy[i].y == 790)
			{
				Enemy[i].y = -30;
			}
		}

		// Seventh line.
		for (i = 18; i < 20; i++)
		{

			Enemy[i].y -= Enemy[i].speed;

			if (Enemy[i].y == -60)
			{
				Enemy[i].y = 780;
			}
		}
	}
}


// Frog boundary.
void frogBound()
{
	if (level == 1)
	{
		// Check boundary collision.
		if (myFrog.x < 5)
		{
			myFrog.x = 5;
		}
		if (myFrog.x > SCREEN_W - 47)
		{
			myFrog.x = SCREEN_W - 47;
		}
		if (myFrog.y > (SCREEN_H - frogger->h) - 28)
		{
			myFrog.y = (SCREEN_H - frogger->h) - 28;
		}
		if (myFrog.y < 5)
		{
			myFrog.y = 5;
		}
	}
	else
	{
		// Check boundary collision.
		if (myFrog.x < 0)
		{
			myFrog.x = 0;
		}
		if (myFrog.x > SCREEN_W - 30)
		{
			myFrog.x = SCREEN_W - 30;
		}
		if (myFrog.y > (SCREEN_H - frogger->h))
		{
			myFrog.y = (SCREEN_H - frogger->h);
		}
		if (myFrog.y < 20)
		{
			myFrog.y = 20;
		}
	}
}

// Key input.
void keyInput()
{
	/*
	Normal degrees: 0, 90, 180, 27, 360.
	Allegro degrees: 0, 64, 128, 192, 256.
	*/
	if (keypressed())
	{
		if (key[KEY_ESC])
		{
			gameover = 1;
		}
		else if (key[KEY_LEFT])
		{
			facing = 0;
			myFrog.x -= myFrog.speed;
			angle = 192;
		}
		else if (key[KEY_RIGHT])
		{
			facing = 1;
			myFrog.x += myFrog.speed;
			angle = 64;
		}
		else if (key[KEY_UP])
		{
			myFrog.y -= myFrog.speed;
			angle = 0;
		}
		else if (key[KEY_DOWN])
		{
			myFrog.y += myFrog.speed;
			angle = 128;
		}
	}
}

// Detect car collision with frogger.
void carCollision()
{
	int i;

	// First line.
	for (i = 0; i < 3; i++)
	{
		// Top left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 39 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 40)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 39 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 40)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64; 
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 39 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 40)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 39 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 40)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 39 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 40)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 39 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 40)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
	}
	
	// Second line.
	for (i = 3; i < 6; i++)
	{
		// Top left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 34 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 36)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 34 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 36)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 34 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 36)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 34 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 36)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 34 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 36)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 34 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 36)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
	}

	// Third line.
	for (i = 6; i < 10; i++)
	{
		// Top left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 39 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 33)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 39 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 33)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 39 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 33)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 39 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 33)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 39 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 33)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 39 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 33)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--; 
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}
			
			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
			
		}
	}

	// Fourth line.
	for (i = 10; i < 13; i++)
	{
		// Top left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 35 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 35)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 35 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 35)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 35 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 35)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 35 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 35)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 35 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 35)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 35 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 35)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
	}

	// Fifth line.
	for (i = 13; i < 15; i++)
	{
		// Top left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 62 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 27)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 62 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 27)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 62 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 27)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 62 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 27)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 62 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 27)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 62 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 27)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
	}

	
	// Sixth line.
	for (i = 15; i < 18; i++)
	{
		// Top left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 35 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 35)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 35 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 35)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 35 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 35)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 35 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 35)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 35 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 35)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 35 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 35)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
	}

	// Seventh line.
	for (i = 18; i < 20; i++)
	{
		// Top left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 62 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 27)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom left detection.
		if (myFrog.x > Enemy[i].x && myFrog.x < Enemy[i].x + 62 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 27)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 62 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 27)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom middle detection.
		if (myFrog.x + 16 > Enemy[i].x && myFrog.x + 16 < Enemy[i].x + 62 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 27)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Top right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 62 &&
			myFrog.y > Enemy[i].y && myFrog.y < Enemy[i].y + 27)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
		// Bottom right detection.
		if (myFrog.x + 32 > Enemy[i].x && myFrog.x + 32 < Enemy[i].x + 62 &&
			myFrog.y + 27 > Enemy[i].y && myFrog.y + 27 < Enemy[i].y + 27)
		{
			play_sample(crash, volume, pan, pitch, FALSE);
			rest(500);
			if (level == 1)
			{
				angle = 0;
				life--;
				myFrog.x = 348;
				myFrog.y = (SCREEN_H - frogger->h) - 30;
			}

			if (level == 2)
			{
				angle = 64;
				life--;
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
			}
		}
	}
}

int main()
{
	srand(time(NULL));
	allegro_init();
	install_timer();

	install_keyboard();
	set_color_depth(16);
	set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);

	//create the double buffer and clear it
	buffer = create_bitmap(SCREEN_W, SCREEN_H);
	clear(buffer);

	title = load_bitmap("titleScreen.bmp", NULL);
	frogger = load_bitmap("frog.bmp", NULL);
	enemy = load_bitmap("car.bmp", NULL);
	enemy2 = load_bitmap("car2.bmp", NULL);
	enemy3 = load_bitmap("car3.bmp", NULL);
	enemy4 = load_bitmap("car4.bmp", NULL);
	enemy5 = load_bitmap("truck.bmp", NULL);
	lives = load_bitmap("health.bmp", NULL);
	items = load_bitmap("item.bmp", NULL);
	items2 = load_bitmap("item2.bmp", NULL);

	while (!startGame && !gameover)
	{
		blit(title, screen, 0, 0, 0, 0, 760, 750);
		if (keypressed())
		{
			if (key[KEY_ESC])
			{
				gameover = 1;
			}
			else if (key[KEY_ENTER])
			{
				startGame = 1;				
			}
		}
		rest(50);
	}
	
	if (startGame)
	{
		level = 1;
		// Load the Mappy file
		if (MapLoad("level1.fmp") != 0)
		{
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Can't find level1.fmp");
		}

		if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0)
		{
			allegro_message("Error initializing sound system\n%s\n", allegro_error);
			return 1;
		}

		// Load the midi file.

		music = load_midi("commando.mid");
		if (!music)
		{
			allegro_message("Error loading Midi file");
			return 1;
		}
		if (play_midi(music, 1) != 0)
		{
			allegro_message("Error playing Midi\n%s", allegro_error);
			return 1;
		}


		crash = load_sample("beep.wav");


		// Set palette.
		MapSetPal8();

		// Sets up my frog.
		myFrog.x = 348;
		myFrog.y = (SCREEN_H - frogger->h) - 30;
		myFrog.speed = 5;

		if (level == 1)
		{
			setUpEnemy();
		}

		start = clock();
	}


	while (!gameover)
	{
		oldpy = myFrog.y;
		oldpx = myFrog.x;

		keyInput();
	
		// Draw map with single player.
		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		MapDrawFG(buffer, 0, yoffset, 0, 0, WIDTH - 1, HEIGHT - 1, 0);

		frogBound();
		drawMyFrog(angle);
		if (!facing)
		{
			if (collided(myFrog.x, myFrog.y) ||
				collided(myFrog.x, myFrog.y + frogger->h) ||
				collided(myFrog.x + frogger->w, myFrog.y) ||
				collided(myFrog.x + frogger->w, myFrog.y + frogger->h))
			{
				myFrog.x = oldpx;
				myFrog.y = oldpy;
			}
		}
		else
		{
			if (collided(myFrog.x, myFrog.y) ||
				collided(myFrog.x, myFrog.y + frogger->h) ||
				collided(myFrog.x + frogger->w, myFrog.y) ||
				collided(myFrog.x + frogger->w, myFrog.y + frogger->h))
			{
				myFrog.x = oldpx;
				myFrog.y = oldpy;
			}
		}
		drawLives();

		if (life == 0)
		{
			gameover = 1;
			lost = 1;
		}
		if (level == 1)
		{
			drawEnemy();
		}

		if (level == 1 && obtained == max)
		{
			level = 2;	
			if (level == 2)
			{
				music2 = load_midi("ff6terra.mid");
				if (!music2)
				{
					allegro_message("Error loading Midi file");
					return 1;
				}
				if (play_midi(music2, 1) != 0)
				{
					allegro_message("Error playing Midi\n%s", allegro_error);
					return 1;
				}
				
				// Load the Mappy file
				if (MapLoad("level2.fmp") != 0)
				{
					set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
					allegro_message("Can't find level1.fmp");
				}
			}

			if (levelChanged == 0)
			{
				myFrog.x = 2;
				myFrog.y = SCREEN_H / 2 - 4;
				angle = 64;
				obtained1 = 0, obtained2 = 0, obtained3 = 0, obtained4 = 0;
				secondWave();
				counter = 60;
				life = 3;
				obtained = 0;
				levelChanged = 1;	
			}
		}
		if (level == 2)
		{
			if (gameover != 1 && won != 1)
			{
				drawEnemy2(eAngle);
			}
			else if (gameover != 1 && limit != 1)
			{
				drawEnemy2(eAngle);
			}
			
		}
		if (level == 2 && obtained == max)
		{
			gameover = 1;
			won = 1;
		}
		
		
		drawItems();

		
		itemCollision();
		moveEnemy();
		carCollision();

		timeCount();
		status(counter, obtained, level);

		// Blit the double buffer.
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
	
		rest(10);
	}

	stop_midi();

	if (limit)
	{
		if (level == 1)
		{
			rectfill(screen, 0, 0, WIDTH, SCREEN_H - 30, 0);
			textout_centre_ex(screen, font, "Game Over!!! Ran out of Time",
				SCREEN_W / 2, SCREEN_H / 2,
				RED, makecol(0, 0, 0));
			rest(2000);
		}
		else
		{
			rectfill(screen, 0, 20, SCREEN_W, SCREEN_H, 0);
			textout_centre_ex(screen, font, "Game Over!!! Ran out of Time",
				SCREEN_W / 2, SCREEN_H / 2,
				RED, makecol(0, 0, 0));
			rest(2000);
		}
	}

	if (won)
	{
		rectfill(screen, 0, 20, SCREEN_W, SCREEN_H, 0);
		textout_centre_ex(screen, font, "You win! You made through the traffic and",
			SCREEN_W / 2, SCREEN_H / 2,
			RED, makecol(0, 0, 0));
		textout_centre_ex(screen, font, "obtained all the items",
			SCREEN_W / 2, SCREEN_H / 2 + 10,
			RED, makecol(0, 0, 0));
		rest(2000);
	}

	if (lost)
	{
		if (level == 1)
		{
			rectfill(screen, 0, 0, WIDTH, SCREEN_H - 30, 0);
			textout_centre_ex(screen, font, "You lose! Ran out of lives",
				SCREEN_W / 2, SCREEN_H / 2,
				RED, makecol(0, 0, 0));
			rest(2000);
		}
		else
		{
			rectfill(screen, 0, 20, SCREEN_W, SCREEN_H, 0);
			textout_centre_ex(screen, font, "You lose! Ran out of lives",
				SCREEN_W / 2, SCREEN_H / 2,
				RED, makecol(0, 0, 0));
			rest(2000);
		}
		
	}
	destroy_bitmap(buffer);
	destroy_bitmap(enemy);
	destroy_bitmap(title);
	destroy_bitmap(enemy2);
	destroy_bitmap(enemy3);
	destroy_bitmap(enemy4);
	destroy_bitmap(enemy5);
	destroy_bitmap(frogger);
	destroy_bitmap(items);
	destroy_bitmap(items2);
	destroy_bitmap(lives);
	destroy_midi(music);
	destroy_midi(music2);
	destroy_sample(crash);
	remove_sound();

	MapFreeMem();

	allegro_exit();
	return 0;
}
END_OF_MAIN()
