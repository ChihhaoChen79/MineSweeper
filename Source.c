#include <stdio.h>
#include "minesweeper.h"


int main() {
	int row, col, n;
	do {
		printf("Enter row: ");
		scanf_s("%d", &row);
		printf("Enter col: ");
		scanf_s("%d", &col);
		printf("Enter number of mines(%d to %d): ", 1, row * col / 2);
		scanf_s("%d", &n);
		if (row < 2 || col < 2)
			printf("Size too small\n");
		if (n<1 || n>row * col / 2)
			printf("Suggest mine numbers: %d to %d\n", 1, row * col / 2);
	} while (row < 2 || col < 2 || n < 1 || n>row * col / 2);

	game g = NewGame(row, col, n);
	PlayGame(g);
	ReleaseMineGame(g);

	return 0;
}

