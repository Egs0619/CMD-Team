#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "jjuggumi.h"
#include "canvas.h"

#define DIALOG_DURATION_SEC		4

void draw(void);
void print_status(void);

int msgROW, msgCOL;

// (zero-base) row행, col열로 커서 이동
void gotoxy(int row, int col) {
	COORD pos = { col,row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// row행, col열에 ch 출력
void printxy(char ch, int row, int col) {
	gotoxy(row, col);
	printf("%c", ch);
}

void map_init(int n_row, int n_col) {
	// 두 버퍼를를 완전히 비우기
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		// 대입문 이렇게 쓸 수 있는데 일부러 안 가르쳐줬음
		back_buf[i][0] = back_buf[i][N_COL - 1] = '*';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '*' : ' ';
		}
	}
}

// back_buf[row][col]이 이동할 수 있는 자리인지 확인하는 함수
bool placable(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
}

// 상단에 맵을, 하단에는 현재 상태를 출력
void display(void) {
	draw();
	gotoxy(N_ROW + 4, 0);  // 추가로 표시할 정보가 있으면 맵과 상태창 사이의 빈 공간에 출력
	print_status();
}

void draw(void) {
	for (int row = 0; row < N_ROW; row++) {
		for (int col = 0; col < N_COL; col++) {
			if (front_buf[row][col] != back_buf[row][col]) {
				front_buf[row][col] = back_buf[row][col];
				printxy(front_buf[row][col], row, col);
			}
		}
	}
}

void print_status(void) {
	printf("no. of players left: %d\n", n_alive);
	for (int p = 0; p < n_player; p++) {
		printf("player %2d: %5s\n", p, player[p] ? "alive" : "DEAD");		
	}
}

void dialog(char message[]) 
{
	int msglength = 0;
	int ctdown = 5;

	for (int i = 0; message[i] != '\0'; i++) // 메세지의 길이 확인
	{
		msglength++;
	}
	int msgwidth = msglength + 6; // *

	if (msgwidth < 10) // 메시지창의 최소 너비 규정
	{
		msgwidth = 10;
	}

	msgROW = (N_ROW / 2) - 1; // 세로 시작점
	msgCOL = (N_COL - msgwidth) / 2; // 가로 시작점

	for (int i = 0; i < msgwidth; i++) // 첫 번째 줄
	{
		printxy('*', msgROW, msgCOL + i);
	}

	gotoxy(msgROW + 1, msgCOL); // 두 번째 줄
	printf("* %d ", DIALOG_DURATION_SEC);
	for (int i = 0; i < msglength; i++)
	{
		printf("%c", message[i]);
	}
	if (msglength < 4) // 가시성을 위해 임의대로 추가한 공간비율 계산 과정
	{
		int empty = 4 - msglength;
		for (int i = 0; i < empty; i++)
		{
			printf(" ");
		}
	}
	printf(" *");

	for (int i = 0; i < msgwidth; i++) // 세 번째 줄 
	{
		printxy('*', msgROW + 2, msgCOL + i);
	}

	for (int i = DIALOG_DURATION_SEC; i > -1; i--)
	{
		gotoxy(msgROW + 1, msgCOL + 2);
		printf("%d", i);
		Sleep(1000);
	}



	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < msgwidth; j++)
		{
			printxy(front_buf[msgROW+i][msgCOL+j], msgROW + i, msgCOL + j);
		}
	}

	gotoxy(16, 0);
}

void intro(void)
{
	for (int i = 0; i < COL_MAX; i++)
	{
		printxy('#', 0, i);
	}
	for (int j = 0; j < ROW_MAX / 2; j++)
	{
		printxy('#', j, 0);
		printxy('#', j, COL_MAX);
	}
	for (int i = 0; i < COL_MAX+1; i++)
	{
		printxy('#', ROW_MAX / 2, i);
	}

	gotoxy(8, 0);
	printf("#            ***      ***** *             *            *    ***   *             #\n");
	Sleep(333);
	printf("#           *   *       *   *     ***     *   ****   * *   *   *  *             #\n");
	Sleep(333);
	printf("#           *   *     ** ** *    *   * ****      * *** *    ***   *             #\n");
	Sleep(333);
	printf("#            ***         ** *    *   *    *     *    * *      ****              #\n");
	Sleep(333);
	printf("#             *         *  *      ***     *    *     * *      *  *              #\n");
	Sleep(333);
	printf("#         *********      **               *            *      ****              #\n");
	Sleep(2000);
}



