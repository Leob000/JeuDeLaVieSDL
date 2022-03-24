#include "functions.h"

void showTab(char *tab){
    int i, j;
    printf("--------------------------\n");
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            printf("%c", tab[i + j * COLONNES]);
        }
        printf("\n");
    }
    printf("--------------------------\n");
}

float ratio(char *tab){
    int i, j, cpt = 0;
    float res, size = LIGNES*COLONNES;
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            if(tab[i + j*COLONNES] == CELLALIVE) cpt++;
        }
    }
    res = cpt/size;
    return res;
}

void showGen(char *tab, SDL_Rect *rect, SDL_Renderer *renderer){
    int i, j;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            if(tab[i + j * COLONNES] == CELLALIVE){
                SDL_RenderDrawRect(renderer, &rect[i + j * COLONNES]);
                SDL_RenderFillRect(renderer, &rect[i + j * COLONNES]);
            }
            else if(RECENTDEADYESNO == 1 && tab[i + j * COLONNES] == RECENTDEAD){
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderDrawRect(renderer, &rect[i + j * COLONNES]);
                SDL_RenderFillRect(renderer, &rect[i + j * COLONNES]);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

int nombreVoisins(char *tab, int x, int y){
    int cpt=0;
    if(tab[y * COLONNES + (x + 1 + COLONNES)%COLONNES] == CELLALIVE) cpt++; //voisin de droite
    if(tab[y * COLONNES + (x - 1 + COLONNES)%COLONNES] == CELLALIVE) cpt++; //voisin de gauche
    if(tab[((y + 1 + LIGNES)%LIGNES) * COLONNES + x] == CELLALIVE) cpt++; //voisin du bas
    if(tab[((y - 1 + LIGNES)%LIGNES) * COLONNES + x] == CELLALIVE) cpt++; //voisin du haut
    if(tab[((y + 1 + LIGNES)%LIGNES) * COLONNES + (x + 1 + COLONNES)%COLONNES] == CELLALIVE) cpt++; //voisin bas droite
    if(tab[((y - 1 + LIGNES)%LIGNES) * COLONNES + (x + 1 + COLONNES)%COLONNES] == CELLALIVE) cpt++; //voisin haut droite
    if(tab[((y + 1 + LIGNES)%LIGNES) * COLONNES + (x - 1 + COLONNES)%COLONNES] == CELLALIVE) cpt++; //voisin bas gauche
    if(tab[((y - 1 + LIGNES)%LIGNES) * COLONNES + (x - 1 + COLONNES)%COLONNES] == CELLALIVE) cpt++; //voisin haut gauche
    return cpt;
}

void tabGenPlusOne(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, int gameMode){
    char *tabtemp;
    int i, j, n, a = 0;
    tabtemp = (char*)malloc(sizeof(char)*COLONNES*LIGNES);
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            n = nombreVoisins(tab, i, j);
            if(n==3) tabtemp[i + j * COLONNES] = CELLALIVE;
            else if(n==2) tabtemp[i + j * COLONNES] = tab[i + j * COLONNES];
            else {
                if(RECENTDEADYESNO == 1 && tab[i + j * COLONNES] == CELLALIVE) tabtemp[i + j * COLONNES] = RECENTDEAD;
                else tabtemp[i + j * COLONNES] = CELLDEAD;
            }
        }
    }
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            tab[i + j * COLONNES] = tabtemp[i + j * COLONNES];
        }
    }
    if(gameMode==2){
        for(j=0;j<LIGNES;++j){
            for(i=0;i<COLONNES;++i){
                if(rand()%1000+1 <= TAUXMUT){
                    tab[i + j * COLONNES] = CELLALIVE;
                    a = 1;
                }
                if(rand()%1000+1 <= TAUXAPO){
                    if(a == 1){
                        if(rand()%2+1 == 1) tab[i + j * COLONNES] = CELLDEAD;
                        else tab[i + j * COLONNES] = CELLALIVE;
                    }
                    else tab[i + j * COLONNES] = CELLDEAD;
                }
                a = 0;
            }
        }
    }
    if(gameMode==3){
        for(j=LIGNES/3;j<LIGNES*2/3;++j){
            for(i=COLONNES/3;i<COLONNES*2/3;++i){
                if(rand()%1000+1 <= TAUXMUT){
                    tab[i + j * COLONNES] = CELLALIVE;
                    a = 1;
                }
                if(rand()%1000+1 <= TAUXAPO){
                    if(a == 1){
                        if(rand()%2+1 == 1) tab[i + j * COLONNES] = CELLDEAD;
                        else tab[i + j * COLONNES] = CELLALIVE;
                    }
                    else tab[i + j * COLONNES] = CELLDEAD;
                }
                a = 0;
            }
        }
    }
    free(tabtemp);
    showGen(tab, rect, renderer);
}

void randomInput(char *tab, SDL_Rect *rect, SDL_Renderer *renderer){
    int i, j;
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            if(rand()%2+1 == 1) tab[i + j * COLONNES] = CELLDEAD;
            else tab[i + j * COLONNES] = CELLALIVE;
        }
    }
    showGen(tab, rect, renderer);
}

void iniTab(char *tab, SDL_Rect *rect){
    int i, j;
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            tab[i + j * COLONNES] = CELLDEAD;
            rect[i + j * COLONNES].x = i*(SQUARESIZE+1);
            rect[i + j * COLONNES].y = j*(SQUARESIZE+1);
            rect[i + j * COLONNES].w = SQUARESIZE;
            rect[i + j * COLONNES].h = SQUARESIZE;
        }
    }
}

//Etat de la cellule à une position relative (xx, yy) d'une celule point de départ (x,y)
int relCellState(char *tab, int x, int y, int xx, int yy){
    if(tab[((y + yy + LIGNES)%LIGNES) * COLONNES + (x + xx + COLONNES)%COLONNES] == CELLALIVE) return 1;
    return 0;
}


//Mettre condidtion CELLALIVE en 1er pour la cellule étudiée
int findSquare(char *tab, int x, int y){
    if(tab[x + y * COLONNES] == CELLALIVE && relCellState(tab, x, y, -1, -1) == 0 && relCellState(tab, x, y, 0, -1) == 0 && relCellState(tab, x, y, 1, -1) == 0 && relCellState(tab, x, y, 2, -1) == 0 && relCellState(tab, x, y, -1, 0) == 0 && relCellState(tab, x, y, 1, 0) == 1 && relCellState(tab, x, y, 2, 0) == 0 && relCellState(tab, x, y, -1, 1) == 0 && relCellState(tab, x, y, 0, 1) == 1 && relCellState(tab, x, y, 1, 1) == 1 && relCellState(tab, x, y, 2, 1) == 0 && relCellState(tab, x, y, -1, 2) == 0 && relCellState(tab, x, y, 0, 2) == 0 && relCellState(tab, x, y, 1, 2) == 0 && relCellState(tab, x, y, 2, 2) == 0) return 1;
    return 0;
}

int find3LineVert(char *tab, int x, int y){
    if(tab[x + y * COLONNES] == CELLALIVE && relCellState(tab, x, y, -1, -1) == 0 && relCellState(tab, x, y, 0, -1) == 0 && relCellState(tab, x, y, 1, -1) == 0 && relCellState(tab, x, y, 2, -1) == 0 && relCellState(tab, x, y, 3, -1) == 0 && relCellState(tab, x, y, -1, 0) == 0 && relCellState(tab, x, y, 1, 0) == 1 && relCellState(tab, x, y, 2, 0) == 1 && relCellState(tab, x, y, 3, 0) == 0 && relCellState(tab, x, y, -1, 1) == 0 && relCellState(tab, x, y, 0, 1) == 0 && relCellState(tab, x, y, 1, 1) == 0 && relCellState(tab, x, y, 2, 1) == 0 && relCellState(tab, x, y, 3, 1) == 0) return 1;
    return 0;
}

int find3LineHori(char *tab, int x, int y){
    if(tab[x + y * COLONNES] == CELLALIVE && relCellState(tab, x, y, -1, -1) == 0 && relCellState(tab, x, y, 0, -1) == 0 && relCellState(tab, x, y, 1, -1) == 0 && relCellState(tab, x, y, -1, 0) == 0 && relCellState(tab, x, y, 1, 0) == 0 && relCellState(tab, x, y, -1, 1) == 0 && relCellState(tab, x, y, 0, 1) == 1 && relCellState(tab, x, y, 1, 1) == 0 && relCellState(tab, x, y, -1, 2) == 0 && relCellState(tab, x, y, 0, 2) == 1 && relCellState(tab, x, y, 1, 2) == 0 && relCellState(tab, x, y, -1, 3) == 0 && relCellState(tab, x, y, 0, 3) == 0 && relCellState(tab, x, y, -1, 3) == 0) return 1;
    return 0;
}

void cleanUp(char *tab){
    int i, j;
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            if(rand()%4+1 <= 3){
                if(findSquare(tab, i, j) == 1){
                    tab[i + j * COLONNES] = CELLDEAD;
                    tab[(i+1) + j * COLONNES] = CELLDEAD;
                    tab[i + (j+1) * COLONNES] = CELLDEAD;
                    tab[(i+1) + (j+1) * COLONNES] = CELLDEAD;
                }
                if(find3LineVert(tab, i, j) == 1){
                    tab[i + j * COLONNES] = CELLDEAD;
                    tab[(i+1) + j * COLONNES] = CELLDEAD;
                    tab[(i+2) + j * COLONNES] = CELLDEAD;
                }
                if(find3LineHori(tab, i, j) == 1){
                    tab[i + j * COLONNES] = CELLDEAD;
                    tab[i + (j+1) * COLONNES] = CELLDEAD;
                    tab[i + (j+2) * COLONNES] = CELLDEAD;
                }
            }
        }
    }
}

int fitness(char *tab, int nbalg){
    int i, j, cpt = 0;
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            if(tab[i + j * COLONNES] == CELLALIVE) cpt++;
            if(nbalg > 0){
                if(findSquare(tab, i, j) == 1) cpt -= 8;
                if(find3LineVert(tab, i, j) == 1) cpt -= 4;
                if(find3LineHori(tab, i, j) == 1) cpt -= 4;
            }
        }
    }
    return cpt;
}

void gameLoopGenetic(char *tab, SDL_Rect *rect, SDL_Renderer *renderer){
    char *tabStockStart = (char*)malloc(sizeof(char)*COLONNES*LIGNES*POPTOT);
    if(NULL == tabStockStart) printf("Erreur allocation tabStockStart\n");
    char *tabStockFinish = (char*)malloc(sizeof(char)*COLONNES*LIGNES*POPTOT);
    if(NULL == tabStockFinish) printf("Erreur allocation tabStockFinish\n");
    int *tabFitness = (int*)malloc(sizeof(int)*POPTOT);
    if(NULL == tabFitness) printf("Erreur allocation tabFitness\n");
    //NE PAS OUBLIER D AJOUTER LES FREETAB A LA FIN DE LA FONCTION OU QUAND BREAK
    
    int i, j, k, l, maxk=0, indiceMaxk=0;
    SDL_bool programLaunched = SDL_TRUE;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    //Initialisation des 2 tableaux de stock
    for(i=0;i<COLONNES*LIGNES*POPTOT;++i){
        tabStockStart[i] = CELLDEAD;
        tabStockFinish[i] = CELLDEAD;
    }
    
    while(programLaunched){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            programLaunched = SDL_FALSE;
                            break;
                        case SDLK_e:
                            //On répète l'algo d'évolution un nombre NBALGO de fois
                            for(l=0;l<NBALGO;++l){
                                
                                //Nombre total d'organismes étudiés à la fois = POPTOT
                                for(k=0;k<POPTOT;++k){
                                    
                                    //On prend dans tabStockStart le tab que l'on veut étudier
                                    for(j=0;j<LIGNES;++j){
                                        for(i=0;i<COLONNES;++i){
                                            tab[i + j * COLONNES] = tabStockStart[i + COLONNES * (j + LIGNES * k)];
                                        }
                                    }
                                    
                                    printf("Algo numéro %d, Testing organisme %d\n", l+1, k+1);
                                    //Phase courte XGEN de mutation puis phase longue YGEN de survie/évolution sans mutation
                                    for(i=0;i<(XGEN/(l+1));++i){
                                        tabGenPlusOne(tab, rect, renderer, 3);
                                    }
                                    //Plus tard accélérer le processus, stop si tab fulldead en ygen
                                    for(i=0;i<YGEN;++i){
                                        tabGenPlusOne(tab, rect, renderer, 1);
                                    }
                                    
                                    //Cleanup des structures non intéressantes
                                    //if(l > 0) cleanUp(tab);
                                    
                                    //Stockage du score de fitness dans le tableau fitness
                                    tabFitness[k] = fitness(tab, l);
                                    
                                    //Stock le tableau 2D dans tabStockFinish
                                    for(j=0;j<LIGNES;++j){
                                        for(i=0;i<COLONNES;++i){
                                            tabStockFinish[i + COLONNES * (j + LIGNES * k)] = tab[i+ j * COLONNES];
                                        }
                                    }
                                }
                                
                                //On repère l'indice du tableau avec le meilleur fitness
                                //Version basique où on récupère juste le top1 meilleur fitness, et on rempli le tab start de top1
                                //Plus tard faire version où l'on rempli un certain %aléatoire du start avec un autre certain top% des meilleurs fitness
                                for(k=0;k<POPTOT;++k){
                                    if(tabFitness[k] > maxk){
                                        maxk = tabFitness[k];
                                        indiceMaxk = k;
                                    }
                                }
                                //Stockage du tableau top1fitness dans tabstart
                                for(k=0;k<POPTOT;++k){
                                    for(j=0;j<LIGNES;++j){
                                        for(i=0;i<COLONNES;++i){
                                            tabStockStart[i + COLONNES * (j + LIGNES * k)] = tabStockFinish[i + COLONNES * (j + LIGNES * indiceMaxk)];
                                        }
                                    }
                                }
                                
                                //Affichage du tableau topfitness pdt un peu de temps
                                for(j=0;j<LIGNES;++j){
                                    for(i=0;i<COLONNES;++i){
                                        tab[i + j*COLONNES] = tabStockFinish[i + COLONNES * (j + LIGNES * indiceMaxk)];
                                    }
                                }
                                printf("Affichage du meilleur tableau de l'algo numéro %d, sélectionné pour être le point de départ du prochain algo\n", l+1);
                                showGen(tab, rect, renderer);
                                SDL_Delay(2000);
                                
                                maxk = 0;
                                indiceMaxk = 0;
                            }
                            for(j=0;j<LIGNES;++j){
                                for(i=0;i<COLONNES;++i){
                                    tab[i + j * COLONNES] = tabStockStart[i + j * COLONNES];
                                }
                            }
                            printf("Algorithmes terminés, possibilité d'appyer sur la touche espace pour voir l'évolution du tableau en cours\n");
                            showGen(tab, rect, renderer);
                            continue;
                        case SDLK_SPACE:
                            tabGenPlusOne(tab, rect, renderer, 1);
                            continue;
                        default:
                            continue;
                    }
                case SDL_QUIT:
                    programLaunched = SDL_FALSE;
                    break;
                default:
                    break;
            }
        }
    }
}


void gameLoop(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, int gameMode){
    int i, cpt = 0;
    SDL_bool programLaunched = SDL_TRUE;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    if(gameMode == 1) randomInput(tab, rect, renderer);
    while(programLaunched){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            programLaunched = SDL_FALSE;
                            break;
                        case SDLK_SPACE:
                            for(i=0;i<GENMULT;++i){
                                tabGenPlusOne(tab, rect, renderer, gameMode);
                            }
                            cpt++;
                            printf("Gen = %d, Ratio Vivant/Mort = %f\n", cpt*GENMULT, ratio(tab));
                            continue;
                        case SDLK_c:
                            for(i=0;i<GENMULT;++i){
                                cleanUp(tab);
                                tabGenPlusOne(tab, rect, renderer, gameMode);
                            }
                            cpt++;
                            printf("Gen = %d, Ratio Vivant/Mort = %f\n", cpt*GENMULT, ratio(tab));
                            continue;
                        default:
                            continue;
                    }
                case SDL_QUIT:
                    programLaunched = SDL_FALSE;
                    break;
                default:
                    break;
            }
        }
    }
}

void menu(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, SDL_Texture *texture){
    SDL_bool programLaunched = SDL_TRUE;
    //Où mettre le sprite de menu
    //SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    //SDL_RenderClear(renderer);
    //SDL_RenderPresent(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    while(programLaunched){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            programLaunched = SDL_FALSE;
                            break;
                        case SDLK_a:
                            gameLoop(tab, rect, renderer, 1);
                            break;
                        case SDLK_z:
                            gameLoop(tab, rect, renderer, 2);
                        case SDLK_e:
                            gameLoopGenetic(tab, rect, renderer);
                        default:
                            continue;
                    }
                case SDL_QUIT:
                    programLaunched = SDL_FALSE;
                    break;
                default:
                    break;
            }
        }
    }
}
