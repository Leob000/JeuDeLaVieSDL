#ifndef functions_h
#define functions_h

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define COLONNES (WINDOW_WIDTH/(SQUARESIZE + 1)) //WINDOW_WIDTH doit être divisible par SQUARESIZE + 1
#define LIGNES (WINDOW_HEIGHT/(SQUARESIZE + 1)) //Idem
#define CELLALIVE '@'
#define CELLDEAD ' '
#define SQUARESIZE 3
#define TAUXMUT 6
#define TAUXAPO 0
#define GENMULT 1
#define RATIOYESNO 1
#define RECENTDEAD '*'
#define RECENTDEADYESNO 0

#define NBALGO 10
#define POPTOT 5
#define XGEN 60
#define YGEN 700


void menu(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, SDL_Texture *texture);
void gameLoop(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, int gameMode);
void iniTab(char *tab, SDL_Rect *rect);
void randomInput(char *tab, SDL_Rect *rect, SDL_Renderer *renderer);
void showEvolution(char *tab, SDL_Rect *rect, SDL_Renderer *renderer);
void showGen(char *tab, SDL_Rect *rect, SDL_Renderer *renderer);
void tabGenPlusOne(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, int gameMode);
int nombreVoisins(char *tab, int x, int y);
void showTab(char *tab);
void gameLoopGenetic(char *tab, SDL_Rect *rect, SDL_Renderer *renderer);
int fitness(char *tab, int nbalg);
int relCellState(char *tab, int x, int y, int xx, int yy);
int findSquare(char *tab, int x, int y);
int find3LineVert(char *tab, int x, int y);
int find3LineHori(char *tab, int x, int y);


#endif
