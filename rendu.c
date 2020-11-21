#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "constantes.h"
#include "struct.h"



SDL_Window* pWindow;
SDL_Renderer* renderer;


SDL_Window* init_sdl(){
    pWindow = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) != 0 ) {
    	fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
		exit(EXIT_FAILURE);
    }

	pWindow = SDL_CreateWindow("3dmatrices",
	SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    LARGEUR_FENETRE,
	HAUTEUR_FENETRE,
	SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    return pWindow;
}

void init_renderer(){
    renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // Création du renderer
    if(renderer == NULL) {
       printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
       exit(EXIT_FAILURE);
    }
}



void afficherPoint(Point2D p, int epaisseur, int r, int g, int b){
	SDL_SetRenderDrawColor(renderer, r, g, b,   255);
	if (epaisseur <= 1) {
		epaisseur=1;
		SDL_RenderDrawPoint(renderer, p.x, p.y);
	} else {
		SDL_Rect rect;
		rect.x = p.x-(epaisseur/2); rect.y = p.y-(epaisseur/2);
		rect.w = epaisseur; rect.h = epaisseur;
		SDL_RenderFillRect(renderer, &rect);
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0,   255);
}


/*
 *	https://fr.wikipedia.org/wiki/Algorithme_de_trac%C3%A9_de_segment_de_Bresenham
 *  Merci également à Jacques-Olivier Lapeyre.
 */

void bresenham(Point2D p1, Point2D p2){

	int e, dx, dy;
	afficherPoint(p2, 2, 128,128,128);

    dx = p2.x - p1.x;
    if (dx != 0) {
        if (dx > 0) {
            dy = p2.y - p1.y;
            if (dy > 0) {
                if (dx >= dy) {
                    e = dx; dx = e*2; dy *= 2;
                    while (1) {
                        printf("brnhm: 1\n");
                        afficherPoint(p1, 2, 128,128,128);
                        if ((p1.x += 1) == p2.x) {break;}
                        if ((e -= dy) < 0) {
                            p1.y += 1; e += dx;
                        }
                    }
                } else {
                    e = dy; dy = e*2; dx *= 2;
                    while (1) {
                        printf("brnhm: 2\n");
                        afficherPoint(p1, 2, 128,128,128);
                        if ((p1.y += 1) == p2.y) {break;}
                        if ((e -= dx) < 0) {
                            p1.x += 1; e += dy;
                        }
                    }
                }
            } else if (dy < 0) {
                if (dx >= -dy) {
                    e = dx; dx = e*2; dy *= 2;
                    while (1) {
                        printf("brnhm: 3\n");
                        afficherPoint(p1, 2, 128,128,128);
                        if ((p1.x += 1) == p2.x) {break;}
                        if ((e += dy) < 0) {
                            p1.y -= 1; e += dx;
                        }
                    }
                } else {
                    e = dy; dy = e*2; dx *= 2;
                    while (1) {
                        printf("brnhm: 4\n");
                        afficherPoint(p1, 2, 128,128,128);
                        if ((p1.y -= 1) == p2.y) {break;}
                        if ((e += dx) > 0) {
                            p1.x += 1; e += dy;
                        }
                    }
                }
            } else {    //dy == 0
                while (p1.x != p2.x) {
                    afficherPoint(p1, 2, 128,128,128);
                    (p1.x += 1);
                }
            }
        } else if (dx < 0){    //dx < 0
            dy = p2.y - p1.y;
            if (dy > 0) {
                if (-dx >= dy) {
                    e = dx; dx *= 2; dy *= 2;
                    while (1) {
                        printf("brnhm: 5+\n");
                        afficherPoint(p1, 2, 128,128,128);
                        if ((p1.x -= 1) == p2.x) {break;}
                        if ((e += dy) >= 0) {
                            p1.y += 1; e += dx;
                        }
                    }
                } else {
                    e = dy; dy = e*2; dx *= 2;
                    while (1) {
                        printf("brnhm: 6\n");
                        afficherPoint(p1, 2, 128,128,128);
                        if ((p1.y += 1) == p2.y) {break;}
                        if ((e += dx) < 0) {
                            p1.x -= 1; e += dy;
                        }
                    }

                }
            } else if (dy < 0) {    //dy < 0
                if (dx <= dy) {
                    e = dx; dx = e*2; dy *= 2;
                    while (1) {
                        printf("brnhm: 7\n");
                        afficherPoint(p1, 2, 128,128,128);
                        if ((p1.x -= 1) == p2.x) {break;}
                        if ((e -= dy) >= 0) {
                            p1.y -= 1; e += dx;
                        }
                    }
                } else {
                    e = dy; dy = e*2; dx *= 2;
                    while (1) {
                        printf("brnhm: 8\n");
                        afficherPoint(p1, 2, 128,128,128);
                        if ((p1.y -= 1) == p2.y) {break;}
                        if ((e -= dx) >= 0) {
                            p1.x -= 1; e += dy;
                        }
                    }
                }
            } else {    //dy == 0
                while (p1.x != p2.x) {
                    afficherPoint(p1, 2, 128,128,128);
                    (p1.x -= 1);
                }
            }
        } else {    //dx == 0
            dy = p2.y - p1.y;
            if (dy != 0) {
                if (dy > 0) {
                    while (p1.y != p2.y) {
                        afficherPoint(p1, 2, 128,128,128);
                        (p1.y += 1);
                    }
                } else {
                    while (p1.y != p2.y) {
                        afficherPoint(p1, 2, 128,128,128);
                        (p1.y -= 1);
                    }
                }
            }
        }
    }
}
