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
#define CELLALIVE2 'a'
#define CELLDEAD ' '
#define SQUARESIZE 3
#define TAUXMUT 5
#define TAUXAPO 0
#define GENMULT 1
#define RATIOYESNO 1
#define RECENTDEAD '*'
#define RECENTDEADYESNO 0

#define NBITERATION 10
#define POPTOT 5
#define XGEN 60
#define YGEN 700


void menu(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, SDL_Texture *texture);
void iniTab(char *tab, SDL_Rect *rect);

#endif
