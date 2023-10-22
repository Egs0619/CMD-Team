#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "jjuggumi.h"
#include "canvas.h"

#define DIALOG_DURATION_SEC		4

void draw(void);
void print_status(void);

int msgROW, msgCOL;

// (zero-base) row��, col���� Ŀ�� �̵�
void gotoxy(int row, int col) {
	COORD pos = { col,row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// row��, col���� ch ���
void printxy(char ch, int row, int col) {
	gotoxy(row, col);
	printf("%c", ch);
}

void map_init(int n_row, int n_col) {
	// �� ���۸��� ������ ����
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		// ���Թ� �̷��� �� �� �ִµ� �Ϻη� �� ����������
		back_buf[i][0] = back_buf[i][N_COL - 1] = '*';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '*' : ' ';
		}
	}
}

// back_buf[row][col]�� �̵��� �� �ִ� �ڸ����� Ȯ���ϴ� �Լ�
bool placable(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
}

// ��ܿ� ����, �ϴܿ��� ���� ���¸� ���
void display(void) {
	draw();
	gotoxy(N_ROW + 4, 0);  // �߰��� ǥ���� ������ ������ �ʰ� ����â ������ �� ������ ���
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

	for (int i = 0; message[i] != '\0'; i++) // �޼����� ���� Ȯ��
	{
		msglength++;
	}
	int msgwidth = msglength + 6; // *

	if (msgwidth < 10) // �޽���â�� �ּ� �ʺ� ����
	{
		msgwidth = 10;
	}

	msgROW = (N_ROW / 2) - 1; // ���� ������
	msgCOL = (N_COL - msgwidth) / 2; // ���� ������

	for (int i = 0; i < msgwidth; i++) // ù ��° ��
	{
		printxy('*', msgROW, msgCOL + i);
	}

	gotoxy(msgROW + 1, msgCOL); // �� ��° ��
	printf("* %d ", DIALOG_DURATION_SEC);
	for (int i = 0; i < msglength; i++)
	{
		printf("%c", message[i]);
	}
	if (msglength < 4) // ���ü��� ���� ���Ǵ�� �߰��� �������� ��� ����
	{
		int empty = 4 - msglength;
		for (int i = 0; i < empty; i++)
		{
			printf(" ");
		}
	}
	printf(" *");

	for (int i = 0; i < msgwidth; i++) // �� ��° �� 
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



