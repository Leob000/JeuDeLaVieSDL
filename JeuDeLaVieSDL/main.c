#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "functions.h"

void exitError(const char *message){
    SDL_Log("Erreur %s: %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

int main(int argc, char * argv[]) {
    srand((int)time(NULL));
    char *tab = (char*)malloc(sizeof(char)*COLONNES*LIGNES);
    if(NULL == tab) printf("Erreur allocation tab\n");
    
    SDL_Rect *rect = (SDL_Rect*)malloc(sizeof(SDL_Rect)*COLONNES*LIGNES);
    if(NULL == rect) printf("Erreur allocation rect\n");
    /*-------------------------------------------------------*/
    if(SDL_Init(SDL_INIT_VIDEO) != 0) exitError("Impossible d'initialiser SDL");
    
    SDL_Window *window = SDL_CreateWindow("Jeu de la vie", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(NULL == window) exitError("SDL_CreateWindow");
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer) exitError("SDL_CreateRenderer");

    SDL_Surface *surface = SDL_LoadBMP("/Users/leoburgund/Documents/Coding/Xcode/JeuDeLaVieSDL/JeuDeLaVieSDL/GOL.bmp");
    if(surface == NULL) exitError("SDL_LoadBMP");
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == NULL) exitError("SDL_CreateTextureFromSurface");
    /*-------------------------------------------------------*/

    iniTab(tab, rect);
    menu(tab, rect, renderer, texture);
    
    /*-------------------------------------------------------*/
    if(renderer != NULL) SDL_DestroyRenderer(renderer);
    if(window != NULL) SDL_DestroyWindow(window);
    free(tab);
    free(rect);
    SDL_Quit();
    return EXIT_SUCCESS;
}
