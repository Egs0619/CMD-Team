#include <stdio.h>
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3
#define START_LINE  39

void mugunghwa_init(void);
void move_manu(key_t key);
void move_rand(int i);
void move_exe(int i, int nx, int ny);
void tagger(void);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];
int tgstatus = 0, tgcount = 1, tgtick = 0, dropout; // dropout은 탈락자를 정하는 변수
char tgmessage;

void mugunghwa_init(void)
{
	map_init(n_alive + 4, 40);
	int x, y = 38;
	for (int i = 0; i < n_alive; i++)
	{
		x = 2 + i;
		back_buf[x][38] = '0' + i;
		px[i] = 2 + i;
		py[i] = y;

		period[i] = randint(100, 500);
	}
}

void move_manu(key_t key)
{
	// 각 방향으로 움직일 때 x, y값 delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // 움직일 방향(0~3)
	switch (key) 
	{
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	if (!placable(nx, ny))
	{
		return;
	}

	move_exe(0, nx, ny);
}

void move_rand(int player)
{
	int p = player;
	int nx=0, ny=0;

	do 
	{
		int dir = randint(0, 100);
		if (dir < 70)
		{
			nx = px[p] + 0;
			ny = py[p] - 1;
		}
		else if (dir < 80)
		{
			nx = px[p] + 1;
			ny = py[p] + 0;
		}
		else if (dir < 90)
		{
			nx = px[p] - 1;
			ny = py[p] + 0;
		}
		else if (dir < 100)
		{
			return;
		}
	} while (!placable(nx, ny));

	move_exe(p, nx, ny);
}

void move_exe(int player, int nx, int ny)
{
	int p = player;
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

void tagger(void)
{
	int startx = n_alive + 5, starty = 1;

	if (n_alive % 2 != 0)
		{
		for (int i = 0; i < 3; i++)
		{
			back_buf[(n_alive + 4) / 2 - 1 + i][1] = '#';
		}
	}
	else if (n_alive % 2 == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			back_buf[(n_alive + 4) / 2 - 1 + i][1] = '#';
		}
	}

	if (tick != 0 && tick % 500 == 0 && tgcount < 6)
	{
		switch (tgcount)
		{
		case 1:
			gotoxy(startx, starty + tgcount * 3);
			printf("무");
			tgcount++;
			break;
		case 2:
			gotoxy(startx, starty + tgcount * 3);
			printf("궁");
			tgcount++;
			break;
		case 3:
			gotoxy(startx, starty + tgcount * 3);
			printf("화");
			tgcount++;
			break;
		case 4:
			gotoxy(startx, starty + tgcount * 3);
			printf("꽃");
			tgcount++;
			break;
		case 5:
			gotoxy(startx, starty + tgcount * 3);
			printf("이");
			tgcount++;
			break;
		}
	}

	if (tick != 0 && tick % 250 == 0 && tgcount > 5)
	{
		switch(tgcount) 
		{
		case 6:
			gotoxy(startx, starty + tgcount * 3);
			printf("피");
			tgcount++;
			break;
		case 7:
			gotoxy(startx, starty + tgcount * 3);
			printf("었");
			tgcount++;
			break;
		case 8:
			gotoxy(startx, starty + tgcount * 3);
			printf("습");
			tgcount++;
			break;
		case 9:
			gotoxy(startx, starty + tgcount * 3);
			printf("니");
			tgcount++;
			break;
		case 10:
			gotoxy(startx, starty + tgcount * 3);
			printf("다");
			tgstatus = 1; // 술래 상태
			break;
		}
	}

	if (tgstatus == 1)
	{
		dropout = randint(1, 101);
		if (tick % 6750 == 0)
		{
			gotoxy(startx, 0);
			printf("                                                ");
		}
	}
}

void mugunghwa(void)
{
	mugunghwa_init();

	system("cls");
	display();
	while (1)
	{
		// player 0만 손으로 움직임(4방향)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			move_manu(key);
		}

		if (tgstatus != 1)
		{
			for (int i = 1; i < n_player; i++)
			{
				if (tick % period[i] == 0)
				{
					move_rand(i);
				}
			}
		}
		else if (tgstatus == 1)
		{
			if (dropout < 10 && dropout != 0)
			{
				int random = (1, 5);
				dialog("Someone is dead!");
				tgstatus = 0;
				tgcount = 1;
			}
		}

		tagger();

		display();
		Sleep(10);
		tick += 10;
	}
}
