#include "functions.h"

void showTab(char *tab){
    int i, j;
    printf("--------------------------\n");
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            printf("%c", tab[j * COLONNES + i]);
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
            if(tab[j*COLONNES + i] == CELLALIVE) cpt++;
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
            if(tab[j * COLONNES + i] == CELLALIVE){
                SDL_RenderDrawRect(renderer, &rect[j * COLONNES + i]);
                SDL_RenderFillRect(renderer, &rect[j * COLONNES + i]);
            }
            else if(RECENTDEADYESNO == 1 && tab[j*COLONNES + i] == RECENTDEAD){
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderDrawRect(renderer, &rect[j * COLONNES + i]);
                SDL_RenderFillRect(renderer, &rect[j * COLONNES + i]);
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
            if(n==3) tabtemp[j*COLONNES + i] = CELLALIVE;
            else if(n==2) tabtemp[j*COLONNES + i] = tab[j*COLONNES + i];
            else {
                if(RECENTDEADYESNO == 1 && tab[j*COLONNES + i] == CELLALIVE) tabtemp[j*COLONNES + i] = RECENTDEAD;
                else tabtemp[j*COLONNES + i] = CELLDEAD;
            }
        }
    }
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            tab[j*COLONNES + i] = tabtemp[j*COLONNES + i];
        }
    }
    if(gameMode==2){
        for(j=0;j<LIGNES;++j){
            for(i=0;i<COLONNES;++i){
                if(rand()%1000+1 <= TAUXMUT){
                    tab[j * COLONNES + i] = CELLALIVE;
                    a = 1;
                }
                if(rand()%1000+1 <= TAUXAPO){
                    if(a == 1){
                        if(rand()%2+1 == 1) tab[j * COLONNES + i] = CELLDEAD;
                        else tab[j * COLONNES + i] = CELLALIVE;
                    }
                    else tab[j * COLONNES + i] = CELLDEAD;
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
            if(rand()%2+1 == 1) tab[j * COLONNES + i] = CELLDEAD;
            else tab[j * COLONNES + i] = CELLALIVE;
        }
    }
    showGen(tab, rect, renderer);
}

void iniTab(char *tab, SDL_Rect *rect){
    int i, j;
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            tab[j * COLONNES + i] = CELLDEAD;
            rect[j * COLONNES + i].x = i*(SQUARESIZE+1);
            rect[j * COLONNES + i].y = j*(SQUARESIZE+1);
            rect[j * COLONNES + i].w = SQUARESIZE;
            rect[j * COLONNES + i].h = SQUARESIZE;
        }
    }
}

int fitness1(char *tab){
    int i, j, cpt = 0;
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            if(tab[j * COLONNES + i] == CELLALIVE) cpt++;
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
                                            tab[j * COLONNES + i] = tabStockStart[(k * (LIGNES+COLONNES)) + (j * COLONNES) + i];
                                        }
                                    }
                                    
                                    //Delay pour afficher précédent starttab
                                    if(l > 0 && k == 0){
                                        printf("Affichage tableau topfitness start\n");
                                        showGen(tab, rect, renderer);
                                        SDL_Delay(1000);
                                    }
                                    
                                    printf("Algo numéro %d, Testing organisme %d\n", l+1, k+1);
                                    //Phase courte XGEN de mutation puis phase longue YGEN de survie/évolution sans mutation
                                    for(i=0;i<XGEN;++i){
                                        tabGenPlusOne(tab, rect, renderer, 2);
                                    }
                                    //Plus tard accélérer le processus, stop si tab fulldead en ygen
                                    for(i=0;i<YGEN;++i){
                                        tabGenPlusOne(tab, rect, renderer, 1);
                                    }
                                    
                                    //Stockage du score de fitness dans le tableau fitness
                                    tabFitness[k] = fitness1(tab);
                                    
                                    //Stock le tableau 2D dans tabStock
                                    for(i=0;i<COLONNES*LIGNES;++i){
                                        tabStockFinish[k*(COLONNES*LIGNES) + i] = tab[i];
                                    }
                                    
                                    //Réinitialise tab pour répéter l'étude, inutile vu que réaffecté à chaque début de boucle?
                                    for(j=0;j<LIGNES;++j){
                                        for(i=0;i<COLONNES;++i){
                                            tab[j * COLONNES + i] = CELLDEAD;
                                        }
                                    }
                                    
                                }
                                
                                //On repère l'indice du tableau avec le meilleur fitness
                                //Version basique où on récupère juste le top1 meilleur fitness, et on rempli le tab start de top1
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
                                            tabStockStart[(k * (LIGNES+COLONNES)) + (j * COLONNES) + i] = tabStockFinish[(indiceMaxk * (LIGNES+COLONNES)) + (j * COLONNES) + i];
                                        }
                                    }
                                }
                                
                                //Affichage du tableau topfitness pdt un peu de temps
                                for(j=0;j<LIGNES;++j){
                                    for(i=0;i<COLONNES;++i){
                                        tab[j*COLONNES + i] = tabStockFinish[(indiceMaxk * (LIGNES+COLONNES)) + (j * COLONNES) + i];
                                    }
                                }
                                printf("Affichage du tableau topfitness de l'algo numéro %d\n", l+1);
                                showGen(tab, rect, renderer);
                                SDL_Delay(1000);
                                
                                maxk = 0;
                                indiceMaxk = 0;
                            }
                            for(j=0;j<LIGNES;++j){
                                for(i=0;i<COLONNES;++i){
                                    tab[j*COLONNES + i] = tabStockStart[(j * COLONNES) + i];
                                }
                            }
                            printf("Done\n");
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
                            printf("Gen = %d, Ratio = %f\n", cpt*GENMULT, ratio(tab));
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
