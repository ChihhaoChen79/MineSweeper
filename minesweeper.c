#include "minesweeper.h"

typedef struct minecell* cell;

struct minecell {
	int m_Mines; // -1: this cell is a mine. 0~8: number of neighboring mines
	bool m_Clicked;
	bool m_Flag;
};

struct minegame {
	cell** m_AllCells;
	int m_TotalRow;
	int m_TotalCol;
	int m_NumberOfMines;
};

cell NewCell(); // create empty cell, no mine, not clicked, no flag
void SetMine(game m, int row, int col); // set a mine at [row,col] and adjust adjacent cells' mine number
void InitialMines(game m, int number); // set [number] mines in [m]
void DrawMineGame(game m); // draw the current status
void Click(game m, int row, int col); // click on [row,col]
bool Win(game m); // check if all non-mine cells are clicked
void RevealAll(game m); // set all cells to clicked
void RevealMines(game m); // set mines to clicked


cell NewCell() {
	cell TempCell = (cell)malloc(sizeof(struct minecell));
	if (!TempCell)
		exit(0);
	TempCell->m_Mines = 0;
	TempCell->m_Clicked = false;
	TempCell->m_Flag = false;
	return TempCell;
}

game NewGame(int row, int col, int number) {
	game TempGame = (game)malloc(sizeof(struct minegame));
	if (!TempGame)
		exit(0);
	TempGame->m_AllCells = (cell**)malloc(sizeof(cell*) * row);
	if (!TempGame->m_AllCells)
		exit(0);
	int i, j;
	for (i = 0; i < row; i++) {
		cell* NewRows = (cell*)malloc(sizeof(cell) * col);
		if (!NewRows)
			exit(0);
		for (j = 0; j < col; j++) {
			NewRows[j] = NewCell();
		}
		TempGame->m_AllCells[i] = NewRows;
	}
	TempGame->m_TotalRow = row;
	TempGame->m_TotalCol = col;
	TempGame->m_NumberOfMines = number;
	InitialMines(TempGame, number);
	return TempGame;
}

void SetMine(game m, int row, int col) {
	if (row < 0 || row >= m->m_TotalRow || col < 0 || col >= m->m_TotalCol) {
		printf("Mine location out of range.\n");
		return;
	}
	m->m_AllCells[row][col]->m_Mines = -1;
	if (row != 0 && col != 0 && m->m_AllCells[row - 1][col - 1]->m_Mines != -1) { // can access top left corner && it is not a mine
		m->m_AllCells[row - 1][col - 1]->m_Mines++;
	}
	if (row != 0 && col != m->m_TotalCol - 1 && m->m_AllCells[row - 1][col + 1]->m_Mines != -1) { // can access top right corner && it is not a mine
		m->m_AllCells[row - 1][col + 1]->m_Mines++;
	}
	if (row != m->m_TotalRow - 1 && col != 0 && m->m_AllCells[row + 1][col - 1]->m_Mines != -1) { // can access bottom left corner && it is not a mine
		m->m_AllCells[row + 1][col - 1]->m_Mines++;
	}
	if (row != m->m_TotalRow - 1 && col != m->m_TotalCol - 1 && m->m_AllCells[row + 1][col + 1]->m_Mines != -1) { // can access bottom right corner && it is not a mine
		m->m_AllCells[row + 1][col + 1]->m_Mines++;
	}
	if (row != 0 && m->m_AllCells[row - 1][col]->m_Mines != -1) { // can access top cell and it is not a mine
		m->m_AllCells[row - 1][col]->m_Mines++;
	}
	if (row != m->m_TotalRow - 1 && m->m_AllCells[row + 1][col]->m_Mines != -1) { // can access bottom cell and it is not a mine
		m->m_AllCells[row + 1][col]->m_Mines++;
	}
	if (col != 0 && m->m_AllCells[row][col - 1]->m_Mines != -1) { // can access left cell and it is not a mine
		m->m_AllCells[row][col - 1]->m_Mines++;
	}
	if (col != m->m_TotalCol - 1 && m->m_AllCells[row][col + 1]->m_Mines != -1) { // can access right cell and it is not a mine
		m->m_AllCells[row][col + 1]->m_Mines++;
	}
}

void InitialMines(game m, int number) {
	int i, row, col;
	for (i = 0; i < number; i++) {
		do {
			srand(time(0) + rand());
			row = rand() % m->m_TotalRow;
			col = rand() % m->m_TotalCol;
		} while (m->m_AllCells[row][col]->m_Mines == -1);
		SetMine(m, row, col);
	}
}

void DrawMineGame(game m) {
	int i, j;
	printf("%d by %d MineSweeper Game. Total Mines: %d\n", m->m_TotalRow, m->m_TotalCol, m->m_NumberOfMines);
	for (i = -1; i < m->m_TotalRow; i++) {
		for (j = -1; j < m->m_TotalCol; j++) {
			if (i == -1 && j == -1)
				printf("r\\c|");
			else if (i == -1)
				printf("%3d|", j);
			else if (j == -1)
				printf("%3d|", i);
			else if (m->m_AllCells[i][j]->m_Flag)
				printf("FLG|");
			else if (!m->m_AllCells[i][j]->m_Clicked)
				printf("   |");
			else if (m->m_AllCells[i][j]->m_Mines == -1)
				printf(":( |");
			else
				printf("%3d|", m->m_AllCells[i][j]->m_Mines);
		}
		printf("\n");
		for (j = -1; j < m->m_TotalCol - 1; j++) {
			printf("---+");
		}
		printf("---\n");
	}
}

void PlayGame(game m) {
	int row, col, step;
	while (!Win(m)) {
		system("cls");
		DrawMineGame(m);
		do {
			printf("Choose your step: 1 = click, 2 = flag\n");
			scanf_s("%d", &step);
			if (step != 1 && step != 2)
				printf("wrong input.\n");
		} while (step != 1 && step != 2);
		if (step == 1) {
			do {
				printf("Enter row: ");
				scanf_s("%d", &row);
				printf("Enter col: ");
				scanf_s("%d", &col);
				if (row < 0 || row >= m->m_TotalRow || col < 0 || col >= m->m_TotalCol)
					printf("Input row/column out of range\n");
			} while (row < 0 || row >= m->m_TotalRow || col < 0 || col >= m->m_TotalCol);
			if (!m->m_AllCells[row][col]->m_Flag) {
				if (m->m_AllCells[row][col]->m_Mines == -1) { // step on mine
					RevealMines(m);
					system("cls");
					DrawMineGame(m);
					printf("You stepped on a mine!\n");
					return;
				}
				else
					Click(m, row, col);
			}
		}
		else if (step == 2) {
			do {
				printf("Enter row: ");
				scanf_s("%d", &row);
				printf("Enter col: ");
				scanf_s("%d", &col);
				if (row < 0 || row >= m->m_TotalRow || col < 0 || col >= m->m_TotalCol)
					printf("Input row/column out of range\n");
				else if (m->m_AllCells[row][col]->m_Clicked)
					printf("This cell is already clicked\n");
			} while (row < 0 || row >= m->m_TotalRow || col < 0 || col >= m->m_TotalCol || m->m_AllCells[row][col]->m_Clicked);
			m->m_AllCells[row][col]->m_Flag = !m->m_AllCells[row][col]->m_Flag;
		}
	}
	RevealAll(m);
	system("cls");
	DrawMineGame(m);
	printf("You win!\n");
}

void Click(game m, int row, int col) {
	if (m->m_AllCells[col][row]->m_Flag)
		return;
	m->m_AllCells[row][col]->m_Clicked = true;
	if (row != 0 && col != 0 && m->m_AllCells[row - 1][col - 1]->m_Mines != -1 && !m->m_AllCells[row - 1][col - 1]->m_Clicked) { // can access top left corner && it is not a mine && not clicked
		if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row - 1][col - 1]->m_Mines == 0) // both are 0: recursively click
			Click(m, row - 1, col - 1);
		else if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row - 1][col - 1]->m_Mines != 0) // next is not 0: click once
			m->m_AllCells[row - 1][col - 1]->m_Clicked = true;
		else
			return;
	}
	if (row != 0 && col != m->m_TotalCol - 1 && m->m_AllCells[row - 1][col + 1]->m_Mines != -1 && !m->m_AllCells[row - 1][col + 1]->m_Clicked) { // can access top right corner && it is not a mine && not clicked
		if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row - 1][col + 1]->m_Mines == 0)
			Click(m, row - 1, col + 1);
		else if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row - 1][col + 1]->m_Mines != 0)
			m->m_AllCells[row - 1][col + 1]->m_Clicked = true;
		else
			return;
	}
	if (row != m->m_TotalRow - 1 && col != 0 && m->m_AllCells[row + 1][col - 1]->m_Mines != -1 && !m->m_AllCells[row + 1][col - 1]->m_Clicked) { // can access bottom left corner && it is not a mine && not clicked
		if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row + 1][col - 1]->m_Mines == 0)
			Click(m, row + 1, col - 1);
		else if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row + 1][col - 1]->m_Mines != 0)
			m->m_AllCells[row + 1][col - 1]->m_Clicked = true;
		else
			return;
	}
	if (row != m->m_TotalRow - 1 && col != m->m_TotalCol - 1 && m->m_AllCells[row + 1][col + 1]->m_Mines != -1 && !m->m_AllCells[row + 1][col + 1]->m_Clicked) { // can access bottom right corner && it is not a mine && not clicked
		if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row + 1][col + 1]->m_Mines == 0)
			Click(m, row + 1, col + 1);
		else if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row + 1][col + 1]->m_Mines != 0)
			m->m_AllCells[row + 1][col + 1]->m_Clicked = true;
		else
			return;
	}
	if (row != 0 && m->m_AllCells[row - 1][col]->m_Mines != -1 && !m->m_AllCells[row - 1][col]->m_Clicked) { // can access top cell and it is not a mine && not clicked
		if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row - 1][col]->m_Mines == 0)
			Click(m, row - 1, col);
		else if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row - 1][col]->m_Mines != 0)
			m->m_AllCells[row - 1][col]->m_Clicked = true;
		else
			return;
	}
	if (row != m->m_TotalRow - 1 && m->m_AllCells[row + 1][col]->m_Mines != -1 && !m->m_AllCells[row + 1][col]->m_Clicked) { // can access bottom cell and it is not a mine && not clicked
		if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row + 1][col]->m_Mines == 0)
			Click(m, row + 1, col);
		else if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row + 1][col]->m_Mines != 0)
			m->m_AllCells[row + 1][col]->m_Clicked = true;
		else
			return;
	}
	if (col != 0 && m->m_AllCells[row][col - 1]->m_Mines != -1 && !m->m_AllCells[row][col - 1]->m_Clicked) { // can access left cell and it is not a mine && not clicked
		if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row][col - 1]->m_Mines == 0)
			Click(m, row, col - 1);
		else if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row][col - 1]->m_Mines != 0)
			m->m_AllCells[row][col - 1]->m_Clicked = true;
		else
			return;
	}
	if (col != m->m_TotalCol - 1 && m->m_AllCells[row][col + 1]->m_Mines != -1 && !m->m_AllCells[row][col + 1]->m_Clicked) { // can access right cell and it is not a mine && not clicked
		if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row][col + 1]->m_Mines == 0)
			Click(m, row, col + 1);
		else if (m->m_AllCells[row][col]->m_Mines == 0 && m->m_AllCells[row][col + 1]->m_Mines != 0)
			m->m_AllCells[row][col + 1]->m_Clicked = true;
		else
			return;
	}
}

bool Win(game m) {
	int count = 0;
	int i, j;
	for (i = 0; i < m->m_TotalRow; i++) {
		for (j = 0; j < m->m_TotalCol; j++) {
			if (!m->m_AllCells[i][j]->m_Clicked && m->m_AllCells[i][j]->m_Mines != -1)
				count++;
		}
	}
	return count == 0;
}

void RevealAll(game m) {
	for (int i = 0; i < m->m_TotalRow; i++) {
		for (int j = 0; j < m->m_TotalCol; j++) {
			m->m_AllCells[i][j]->m_Clicked = true;
		}
	}
}

void RevealMines(game m) {
	for (int i = 0; i < m->m_TotalRow; i++) {
		for (int j = 0; j < m->m_TotalCol; j++) {
			if (m->m_AllCells[i][j]->m_Mines == -1)
				m->m_AllCells[i][j]->m_Clicked = true;
		}
	}
}

void ReleaseMineGame(game m) {
	int i, j;
	for (i = 0; i < m->m_TotalRow; i++) {
		for (j = 0; j < m->m_TotalCol; j++) {
			free(m->m_AllCells[i][j]);
		}
		free(m->m_AllCells[i]);
	}
	free(m);
}