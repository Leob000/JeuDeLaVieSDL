#ifndef functions_h
#define functions_h

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define COLONNES (WINDOW_WIDTH/10*2)
#define LIGNES (WINDOW_HEIGHT/10*2)
#define CELLALIVE '@'
#define CELLDEAD ' '
#define SQUARESIZE 4
#define TAUXMUT 6
#define TAUXAPO 5
#define GENMULT 1
#define RATIOYESNO 1
#define RECENTDEAD '*'
#define RECENTDEADYESNO 1

void menu(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, SDL_Texture *texture);
void gameLoop(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, int gameMode);
void iniTab(char *tab, SDL_Rect *rect);
void randomInput(char *tab, SDL_Rect *rect, SDL_Renderer *renderer);
void showEvolution(char *tab, SDL_Rect *rect, SDL_Renderer *renderer);
void showGen(char *tab, SDL_Rect *rect, SDL_Renderer *renderer);
void tabGenPlusOne(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, int gameMode);
int nombreVoisins(char *tab, int x, int y);
void showTab(char *tab);

#endif
