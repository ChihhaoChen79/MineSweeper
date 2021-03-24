#ifndef MINESWEEPER_H
#define MINESWEEPER_H
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>

typedef struct minegame* game;

game NewGame(int row, int col, int number);
void PlayGame(game m);
void ReleaseMineGame(game m);

#endif // !MINESWEEPER_H
