#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "constantes.h"
#include "struct.h"
#include "vecteur3d.h"


//SDL
SDL_Window* pWindow;
SDL_Renderer* renderer;

//SDL texture
Uint32 *pixels;
void *tmp;
int pitch;
SDL_Texture *texture;

//repère cartesien
Point3D origine = {0, 0, 0};
Vecteur3D v_cartesien_x = {1, 0, 0};
Vecteur3D v_cartesien_y = {0, 1, 0};
Vecteur3D v_cartesien_z = {0, 0, 1};

//camera
Vecteur3D vcam, vplan_cam_i, vplan_cam_j;
Point3D pcam;
double rayon = 1000;
double alpha = 0, beta = 0;



void bresenham(Point2D* po1, Point2D* po2, int epaisseur, int r, int g, int b);     //je le laisse en bas


SDL_Window* init_sdl(){
    pWindow = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) != 0 ) {
    	fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
		exit(EXIT_FAILURE);
    }

	pWindow = SDL_CreateWindow("3dpoints",
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

void init_texture(){
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
					LARGEUR_FENETRE, HAUTEUR_FENETRE); /* On devrait vérifier que la fonction a réussi */
	if(texture == NULL) {
       printf("Erreur lors de la creation de la texture : %s",SDL_GetError());
       exit(EXIT_FAILURE);
    }
	SDL_LockTexture(texture, NULL, &tmp, &pitch);
	pixels = tmp;
	SDL_UnlockTexture(texture);
}


void calculer_vecteurs_plancam(){
	Vecteur3D vpi[3];
	Vecteur3D v_cart_rayon[3];
	produitk(v_cartesien_x, rayon, &v_cart_rayon[0]);
	produitk(v_cartesien_y, rayon, &v_cart_rayon[1]);
	produitk(v_cartesien_z, rayon, &v_cart_rayon[2]);

	produitk(v_cart_rayon[0], -cos(alpha)*cos(beta), &vpi[0]);
	produitk(v_cart_rayon[1], -cos(alpha)*sin(beta), &vpi[1]);
	produitk(v_cart_rayon[2], sin(alpha), &vpi[2]);
	addition(vpi, 3, &vplan_cam_j);

	produitk(v_cart_rayon[0], sin(beta), &vpi[0]);
	produitk(v_cart_rayon[1], -cos(beta), &vpi[1]);
	addition(vpi, 2, &vplan_cam_i);
}

Point2D* proj_point(Point3D* p, Point2D* proj){

	Vecteur3D vcam_p;
	getVecteur(&pcam, p, &vcam_p);

	proj->x = ((scalaire3D(vcam_p, vplan_cam_i) / scalaire3D(vplan_cam_i, vplan_cam_i))
		*GRANDEUR_FENETRE*5 + (LARGEUR_FENETRE/2)) + 0.5;
	proj->y = ((scalaire3D(vcam_p, vplan_cam_j) / scalaire3D(vplan_cam_j, vplan_cam_j))
		*GRANDEUR_FENETRE*5 + (HAUTEUR_FENETRE/2)) + 0.5;
}

void update_cam(){
	pcam.x = rayon*sin(alpha)*cos(beta);
	pcam.y = rayon*sin(alpha)*sin(beta);
	pcam.z = rayon*cos(alpha);
	getVecteur(&pcam, &origine, &vcam);

	calculer_vecteurs_plancam();
}



void afficher_point2Dv1(Point2D* p, int epaisseur, int r, int g, int b){
	SDL_SetRenderDrawColor(renderer, r, g, b,   255);
	if (epaisseur <= 1) {
		epaisseur=1;
		SDL_RenderDrawPoint(renderer, p->x, p->y);
	} else {
		SDL_Rect rect;
		rect.x = p->x-(epaisseur/2); rect.y = p->y-(epaisseur/2);
		rect.w = epaisseur; rect.h = epaisseur;
		SDL_RenderFillRect(renderer, &rect);
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0,   255);
}

void afficher_point2D(Point2D* p, int epaisseur, int r, int g, int b){
	//printf("a\n");
	//printf("p %x\n", r<<24 | g<<16 | b<<8 | 255<<0);
	//pixels[5] = 3999999999;
	if (p->y >= 0 && p->y < HAUTEUR_FENETRE && p->x >= 0 && p->x < LARGEUR_FENETRE) {
		pixels[p->y * LARGEUR_FENETRE + p->x] = r<<24 | g<<16 | b<<8 | 255<<0;
	}
}




/*
 * RENDU GRAPHES
 */
 //optimisation à faire si point3d n'est pas dans l'angle de la camera
void afficher_point(Point3D* p, int epaisseur, int r, int v, int b){
	Point2D proj;
	proj_point( p, &proj);
	afficher_point2D(&proj, epaisseur, r,v,b);
}

 //optimisation à faire si a->p1 && a->p2 tout deux à doite ou à gauche ou en
 //haut ou en bas hors du champ de la cam
void afficher_arete(Arete3D* a, int epaisseur, int r, int v, int b){
	Point2D proj1, proj2;
	proj_point( a->p1 , &proj1 );
	proj_point( a->p2 , &proj2 );
	bresenham(&proj1, &proj2, epaisseur, r,v,b);
}



void afficher_points(Graphe3D* g, int epaisseur, int r, int v, int b){
	Noeud* ni = g->point_tete;
	while (ni != NULL) {
		afficher_point((Point3D*)ni->elem, epaisseur, r,v,b);
		ni = ni->suiv;
	}
}

void afficher_aretes(Graphe3D* g, int epaisseur, int r, int v, int b){
	Noeud* ni = g->arete_tete;
	while (ni != NULL) {
		afficher_arete((Arete3D*)ni->elem, epaisseur, r,v,b);
		ni = ni->suiv;
	}
}



void afficher_points_gstat(GrapheStatique3D* g, int epaisseur, int r, int v, int b){
	for (Point3D* p = g->tab_points; p < g->tab_points + g->nbPoints-1; p++) {
		afficher_point(p, epaisseur, r,v,b);
	}
}

void afficher_aretes_gstat(GrapheStatique3D* g, int epaisseur, int r, int v, int b){
	/*for (Arete3D* a = g->tab_aretes; a < g->tab_aretes + g->nbAretes-1; a++) {
		afficher_arete(a, epaisseur, r,v,b);
	}*/
	for (size_t i = 0; i < g->nbAretes; i++) {
		afficher_arete(g->tab_aretes+i, epaisseur, r,v,b);
	}
}


// rendre cette fonction plus parametrable
void afficher_couleur_relief_points(GrapheStatique3D* g, int epaisseur, int zmin, int zmax){
	int deltaz = zmax - zmin;
	for (Point3D* p = g->tab_points; p < g->tab_points + g->nbPoints-1; p++) {
		if (p->z <= zmin + (deltaz/3)) {
			afficher_point(p, epaisseur, 0, 255, 255);
		}
		else if (p->z > zmin + (deltaz/3) && p->z <= zmin + (deltaz/2)) {
			afficher_point(p, epaisseur, 255, 255, 0);
		}
		else if (p->z > zmin + (deltaz/2) && p->z <= zmin + (deltaz/2)+(deltaz/4)+(deltaz/8)) {
			afficher_point(p, epaisseur, 0, 255, 0);
		}
		else {
			afficher_point(p, epaisseur, 255, 255, 255);
		}
		//afficher_point(p, epaisseur, r,v,b);
	}
}



/*
 * RENDU COMPTEUR FPS
 */




/*
 *	BRENSENHAM
 *
 *	https://fr.wikipedia.org/wiki/Algorithme_de_trac%C3%A9_de_segment_de_Bresenham
 *  Merci également à Jacques-Olivier Lapeyre.
 */
void bresenham(Point2D* po1, Point2D* po2, int epaisseur, int r, int g, int b){

    Point2D p1 = {po1->x, po1->y};
    Point2D p2 = {po2->x, po2->y};

	int e, dx, dy;
	afficher_point2D(&p2, 2, r,g,b);

    dx = p2.x - p1.x;
    if (dx != 0) {
        if (dx > 0) {
            dy = p2.y - p1.y;
            if (dy > 0) {
                if (dx >= dy) {
                    e = dx; dx = e*2; dy *= 2;
                    while (1) {
                        //printf("brnhm: 1\n");
                        afficher_point2D(&p1, epaisseur, r,g,b);
                        if ((p1.x += 1) == p2.x) {break;}
                        if ((e -= dy) < 0) {
                            p1.y += 1; e += dx;
                        }
                    }
                } else {
                    e = dy; dy = e*2; dx *= 2;
                    while (1) {
                        //printf("brnhm: 2\n");
                        afficher_point2D(&p1, epaisseur, r,g,b);
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
                        //printf("brnhm: 3\n");
                        afficher_point2D(&p1, epaisseur, r,g,b);
                        if ((p1.x += 1) == p2.x) {break;}
                        if ((e += dy) < 0) {
                            p1.y -= 1; e += dx;
                        }
                    }
                } else {
                    e = dy; dy = e*2; dx *= 2;
                    while (1) {
                        //printf("brnhm: 4\n");
                        afficher_point2D(&p1, epaisseur, r,g,b);
                        if ((p1.y -= 1) == p2.y) {break;}
                        if ((e += dx) > 0) {
                            p1.x += 1; e += dy;
                        }
                    }
                }
            } else {    //dy == 0
                //printf("brnhm: VERTICAL +++\n");
                while (p1.x != p2.x) {
                    afficher_point2D(&p1, epaisseur, r,g,b);
                    (p1.x += 1);
                }
            }
        } else if (dx < 0){    //dx < 0
            dy = p2.y - p1.y;
            if (dy > 0) {
                if (-dx >= dy) {
                    e = dx; dx *= 2; dy *= 2;
                    while (1) {
                        //printf("brnhm: 5+\n");
                        afficher_point2D(&p1, epaisseur, r,g,b);
                        if ((p1.x -= 1) == p2.x) {break;}
                        if ((e += dy) >= 0) {
                            p1.y += 1; e += dx;
                        }
                    }
                } else {
                    e = dy; dy = e*2; dx *= 2;
                    while (1) {
                        //printf("brnhm: 6\n");
                        afficher_point2D(&p1, epaisseur, r,g,b);
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
                        //printf("brnhm: 7\n");
                        afficher_point2D(&p1, epaisseur, r,g,b);
                        if ((p1.x -= 1) == p2.x) {break;}
                        if ((e -= dy) >= 0) {
                            p1.y -= 1; e += dx;
                        }
                    }
                } else {
                    e = dy; dy = e*2; dx *= 2;
                    while (1) {
                        //printf("brnhm: 8\n");
                        afficher_point2D(&p1, epaisseur, r,g,b);
                        if ((p1.y -= 1) == p2.y) {break;}
                        if ((e -= dx) >= 0) {
                            p1.x -= 1; e += dy;
                        }
                    }
                }
            } else {    //dy == 0
                //printf("brnhm: VERTICAL -\n");
                while (p1.x != p2.x) {
                    afficher_point2D(&p1, epaisseur, r,g,b);
                    (p1.x -= 1);
                }
            }
        }
    } else {	//dx == 0
		dy = p2.y - p1.y;
		if (dy != 0) {
			if (dy > 0) {
				while (p1.y != p2.y) {
					afficher_point2D(&p1, epaisseur, r,g,b);
					(p1.y += 1);
				}
			} else {
				while (p1.y != p2.y) {
					afficher_point2D(&p1, epaisseur, r,g,b);
					(p1.y -= 1);
				}
			}
		} else {
			afficher_point2D(&p1, epaisseur, r,g,b);
		}
	}
}
