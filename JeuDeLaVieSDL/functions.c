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
    float size = LIGNES*COLONNES;
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            if(tab[i + j*COLONNES] == CELLALIVE) cpt++;
        }
    }
    return cpt/size;
}

float ratioColor(char *tab){
    int i, j;
    float cptRed = 0, cptBlue = 0;
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            if(tab[i + j*COLONNES] == CELLALIVE) cptRed++;
            if(tab[i + j*COLONNES] == CELLALIVE2) cptBlue++;
        }
    }
    return cptRed/cptBlue;
}

void showGen(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, int gameMode){
    int i, j;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if(gameMode != 4){
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
    }
    if(gameMode == 4){
        for(j=0;j<LIGNES;++j){
            for(i=0;i<COLONNES;++i){
                if(tab[i + j * COLONNES] == CELLALIVE){
                    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &rect[i + j * COLONNES]);
                    SDL_RenderFillRect(renderer, &rect[i + j * COLONNES]);
                }
                if(tab[i + j * COLONNES] == CELLALIVE2){
                    SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
                    SDL_RenderDrawRect(renderer, &rect[i + j * COLONNES]);
                    SDL_RenderFillRect(renderer, &rect[i + j * COLONNES]);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}

int nombreVoisins(char *tab, int x, int y, int voisin){
    int cpt=0;
    if(tab[y * COLONNES + (x + 1 + COLONNES)%COLONNES] == voisin) cpt++; //voisin de droite
    if(tab[y * COLONNES + (x - 1 + COLONNES)%COLONNES] == voisin) cpt++; //voisin de gauche
    if(tab[((y + 1 + LIGNES)%LIGNES) * COLONNES + x] == voisin) cpt++; //voisin du bas
    if(tab[((y - 1 + LIGNES)%LIGNES) * COLONNES + x] == voisin) cpt++; //voisin du haut
    if(tab[((y + 1 + LIGNES)%LIGNES) * COLONNES + (x + 1 + COLONNES)%COLONNES] == voisin) cpt++; //voisin bas droite
    if(tab[((y - 1 + LIGNES)%LIGNES) * COLONNES + (x + 1 + COLONNES)%COLONNES] == voisin) cpt++; //voisin haut droite
    if(tab[((y + 1 + LIGNES)%LIGNES) * COLONNES + (x - 1 + COLONNES)%COLONNES] == voisin) cpt++; //voisin bas gauche
    if(tab[((y - 1 + LIGNES)%LIGNES) * COLONNES + (x - 1 + COLONNES)%COLONNES] == voisin) cpt++; //voisin haut gauche
    return cpt;
}

void tabGenPlusOne(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, int gameMode){
    char *tabtemp;
    int i, j, n, a = 0, nRed, nBlue, nTot;
    tabtemp = (char*)malloc(sizeof(char)*COLONNES*LIGNES);
    if(gameMode != 4){ //Application des règles simples si tout sauf mode couleur
        for(j=0;j<LIGNES;++j){
            for(i=0;i<COLONNES;++i){
                n = nombreVoisins(tab, i, j, CELLALIVE);
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
    }
    if(gameMode==2){ //Application règle Mutation/Apoptose si mode Mutation/Apoptose
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
    if(gameMode==3){ //Application règle Mutation/Apoptose si mode Sélection naturelle
        for(j=LIGNES/3;j<LIGNES*2/3;++j){
            for(i=COLONNES/3;i<COLONNES*2/3;++i){
                if(rand()%1000+1 <= TAUXMUTGEN){
                    tab[i + j * COLONNES] = CELLALIVE;
                    a = 1;
                }
                if(rand()%1000+1 <= TAUXAPOGEN){
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
    if(gameMode == 4){ //Application des règles simples si mode couleur
        for(j=0;j<LIGNES;++j){
            for(i=0;i<COLONNES;++i){
                nRed = nombreVoisins(tab, i, j, CELLALIVE);
                nBlue = nombreVoisins(tab, i, j, CELLALIVE2);
                nTot = nRed + nBlue;
                
                if(nTot == 3){
                    if(nRed > nBlue) tabtemp[i + j * COLONNES] = CELLALIVE;
                    else tabtemp[i + j * COLONNES] = CELLALIVE2;
                }
                else if(nTot == 2) tabtemp[i + j * COLONNES] = tab[i + j * COLONNES];
                else tabtemp[i + j * COLONNES] = CELLDEAD;
            }
        }
        for(j=0;j<LIGNES;++j){
            for(i=0;i<COLONNES;++i){
                tab[i + j * COLONNES] = tabtemp[i + j * COLONNES];
            }
        }
    }
    free(tabtemp);
    showGen(tab, rect, renderer, gameMode);
}

void randomInput(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, int gameMode){
    int i, j;
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            if(gameMode != 4){
                if(rand()%2+1 == 1) tab[i + j * COLONNES] = CELLDEAD;
                else tab[i + j * COLONNES] = CELLALIVE;
            }
            if(gameMode == 4){
                if(rand()%2+1 == 1) tab[i + j * COLONNES] = CELLALIVE;
                else tab[i + j * COLONNES] = CELLALIVE2;
                if(rand()%2+1 == 1) tab[i + j * COLONNES] = CELLDEAD;
            }
        }
    }
    showGen(tab, rect, renderer, gameMode);
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
            if(rand()%4+1 <= 3){ //3 chances sur 4
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
            if(nbalg > 1){
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
    if(NULL == tabStockStart){
        printf("Erreur allocation tabStockStart\n");
        exit(EXIT_FAILURE);
    }
    char *tabStockFinish = (char*)malloc(sizeof(char)*COLONNES*LIGNES*POPTOT);
    if(NULL == tabStockFinish){
        printf("Erreur allocation tabStockFinish\n");
        exit(EXIT_FAILURE);
    }
    int *tabFitness = (int*)malloc(sizeof(int)*POPTOT);
    if(NULL == tabFitness){
        printf("Erreur allocation tabFitness\n");
        exit(EXIT_FAILURE);
    }
    
    int i, j, k, l, maxk=0, indiceMaxk=0, checkVide = 1;
    SDL_bool programLaunched = SDL_TRUE;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    //Initialisation des 2 tableaux de stock
    for(i=0;i<COLONNES*LIGNES*POPTOT;++i){
        tabStockStart[i] = CELLDEAD;
        tabStockFinish[i] = CELLDEAD;
    }
    
    //On répète l'algo d'évolution un nombre NBITERATION de fois
    for(l=0;l<NBITERATION;++l){
        
        //Nombre total d'organismes étudiés à la fois = POPTOT
        for(k=0;k<POPTOT;++k){
            
            //On prend dans tabStockStart le tab que l'on veut étudier
            for(j=0;j<LIGNES;++j){
                for(i=0;i<COLONNES;++i){
                    tab[i + j * COLONNES] = tabStockStart[i + COLONNES * (j + LIGNES * k)];
                }
            }
            
            printf("Itération numéro %d, Testing organisme %d\n", l+1, k+1);
            //Phase courte XGEN de mutation puis phase longue YGEN de survie/évolution sans mutation
            for(i=0;i<(XGEN/(l+1));++i){
                tabGenPlusOne(tab, rect, renderer, 3);
            }
            //Plus tard accélérer le processus, stop si tab fulldead en ygen
            for(i=0;i<YGEN;++i){
                tabGenPlusOne(tab, rect, renderer, 1);
            }
            
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
        //Tester plus tard faire version où l'on rempli un certain %aléatoire du start avec un autre certain top% des meilleurs fitness??
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
        printf("Affichage du meilleur tableau de l'itération numéro %d, sélectionné pour être le point de départ de la prochaine itération\n", l+1);
        showGen(tab, rect, renderer, 0);
        SDL_Delay(2000);
        
        //Répète la première itéreation si elle n'a rien donné
        if(l == 0){
            for(j=0;j<LIGNES;++j){
                for(i=0;i<COLONNES;++i){
                    if(tab[i + j*COLONNES] == CELLALIVE) checkVide = 0;
                }
            }
            if(checkVide == 1){
                l -= 1;
                printf("L'itération n'a rien donné, on recommence cette itération\n");
            }
            checkVide = 1;
        }
        
        maxk = 0;
        indiceMaxk = 0;
    }
    for(j=0;j<LIGNES;++j){
        for(i=0;i<COLONNES;++i){
            tab[i + j * COLONNES] = tabStockStart[i + j * COLONNES];
        }
    }
    printf("Itération terminées, possibilité d'appyer sur la touche espace pour voir l'évolution du tableau en cours\n");
    showGen(tab, rect, renderer, 0);
    
    while(programLaunched){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            programLaunched = SDL_FALSE;
                            free(tabStockStart);
                            free(tabStockFinish);
                            free(tabFitness);
                            break;
                        case SDLK_SPACE:
                            tabGenPlusOne(tab, rect, renderer, 1);
                            continue;
                        default:
                            continue;
                    }
                case SDL_QUIT:
                    programLaunched = SDL_FALSE;
                    free(tabStockStart);
                    free(tabStockFinish);
                    free(tabFitness);
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
    if(gameMode == 1 || gameMode == 4) randomInput(tab, rect, renderer, gameMode);
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
                            if(gameMode != 4) printf("Gen = %d, Ratio Vivant/Mort = %f\n", cpt*GENMULT, ratio(tab));
                            if(gameMode == 4) printf("Gen = %d, Ratio Vivant/Mort = %f, Ratio Rouge/Bleu = %f\n", cpt*GENMULT, ratio(tab), ratioColor(tab));
                            continue;
                        case SDLK_c:
                            if(gameMode != 4){
                                for(i=0;i<GENMULT;++i){
                                    cleanUp(tab);
                                    tabGenPlusOne(tab, rect, renderer, gameMode);
                                }
                                cpt++;
                                printf("Gen = %d, Ratio Vivant/Mort = %f\n", cpt*GENMULT, ratio(tab));
                            }
                            continue;
                        default:
                            break;
                    }
                case SDL_QUIT:
                    programLaunched = SDL_FALSE;
                    break;
                default:
                    continue;
            }
        }
    }
}

void menu(char *tab, SDL_Rect *rect, SDL_Renderer *renderer, SDL_Texture *texture){
    SDL_bool programLaunched = SDL_TRUE;
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
                            gameLoop(tab, rect, renderer, 1); //gameMode1 est le mode d'input de base random
                            break;
                        case SDLK_z:
                            gameLoop(tab, rect, renderer, 2); //gameMode2 est le mode Mutations/Apoptoses
                            break;
                        case SDLK_e:
                            gameLoopGenetic(tab, rect, renderer);
                            break;
                        case SDLK_r:
                            gameLoop(tab, rect, renderer, 4); //gameMode 4 est le mode couleur
                            break;
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
