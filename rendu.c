#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "constantes.h"
#include "rendu.h"
#include "struct.h"
#include "vecteur3d.h"
#include "vecteur2d.h"


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



void afficher_arete(Arete3D* a, int epaisseur, int r, int v, int b);
void afficher_aretev2(Arete3D* a, int epaisseur, int r, int v, int b);
void bresenham(Point2D* po1, Point2D* po2, int epaisseur, int r, int g, int b);     //je les laisse en bas


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
    for (size_t i=0; i<epaisseur; i++) {
        for (size_t j=0; j<epaisseur; j++) {
            if ((p->y+(j-(epaisseur/2))) >= 0
                && (p->y+(j-(epaisseur/2))) < HAUTEUR_FENETRE
                && (p->x+(i-(epaisseur/2))) >= 0
                && (p->x+(i-(epaisseur/2))) < LARGEUR_FENETRE)
            {
    		    pixels[(p->y+(j-(epaisseur/2))) * LARGEUR_FENETRE + (p->x+(i-(epaisseur/2)))] = r<<24 | g<<16 | b<<8 | 255<<0;
            }
        }
    }
	/*if (p->y >= 0 && p->y < HAUTEUR_FENETRE && p->x >= 0 && p->x < LARGEUR_FENETRE) {
		pixels[p->y * LARGEUR_FENETRE + p->x] = r<<24 | g<<16 | b<<8 | 255<<0;
	}*/
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
void afficher_arete0(Arete3D* a, int epaisseur, int r, int v, int b){
	Point2D proj1, proj2 ;//= {500, 6000};//= {500, -400}
	proj_point( a->p1 , &proj1 );
	proj_point( a->p2 , &proj2 );

    bresenham(&proj1, &proj2, epaisseur, r,v,b);
}



void afficher_arete02(Arete3D* a, int epaisseur, int r, int v, int b){
	Point2D proj1, proj2 ;//= {500, 6000};//= {500, -400}
	proj_point( a->p1 , &proj1 );
	proj_point( a->p2 , &proj2 );

    if (proj1.x < 0 && proj2.x < 0) {
        if (proj1.x > LARGEUR_FENETRE && proj2.x > LARGEUR_FENETRE) {
            if (proj1.y < 0 && proj2.y < 0) {
                if (proj1.y > HAUTEUR_FENETRE && proj2.y > HAUTEUR_FENETRE) {
                    return;
                }
                else {
                    bresenham(&proj1, &proj2, epaisseur, r,v,b);
                }
            }
            else {
                bresenham(&proj1, &proj2, epaisseur, r,v,b);
            }
        }
        else {
            bresenham(&proj1, &proj2, epaisseur, r,v,b);
        }
    } else {
        bresenham(&proj1, &proj2, epaisseur, r,v,b);
    }
}



void afficher_aretev1(Arete3D* a, int epaisseur, int r, int v, int b){
	Point2D proj1, proj2 ;//= {500, 6000};//= {500, -400}
    unsigned int minx=0, maxx=LARGEUR_FENETRE,
                miny=0, maxy=HAUTEUR_FENETRE;
	proj_point( a->p1 , &proj1 );
	proj_point( a->p2 , &proj2 );

    //bresenham(&proj1, &proj2, epaisseur, r,v,b);

    //if (proj1.x == proj2.x) { bresenham(&proj1, &proj2, epaisseur, r,v,b); return; }

	if (proj1.x >= 0) {
		if (proj1.x < LARGEUR_FENETRE) {    //+1 et -1 pour éviter des divisions par zero par la suite
			//p1 PARTIE CENTRE
			if (proj1.y >= 0) {
				if (proj1.y < HAUTEUR_FENETRE) {
					bresenham(&proj1, &proj2, epaisseur, r,v,b);
				} else {
					//p1 CENTRE BAS

                    if (proj2.y < HAUTEUR_FENETRE) {
                        if (proj2.x >= 0) {
    						if (proj2.x < LARGEUR_FENETRE) {
    							bresenham(&proj1, &proj2, epaisseur, r,v,b);
    						} else {
    							//p2 PARTIE DROITE
    							/*double coefdir_coin = (proj1.y-HAUTEUR_FENETRE)/(LARGEUR_FENETRE-proj1.x);
    							double coefdir_segment = (proj1.y - proj2.y)/(proj2.x - proj1.x);
    							if (coefdir_segment > coefdir_coin) {
    								bresenham(&proj1, &proj2, epaisseur, r,v,b);
    							}
                                else { return; }*/

                                int coef_coin = (proj1.y-HAUTEUR_FENETRE)*(proj2.x - proj1.x);
                                //(A*B)-> si A=0: p2.y<HF donc s'affiche, si B=0: impossible car proj2.x>LF
                                int coef_segment = (proj1.y - proj2.y)*(LARGEUR_FENETRE-proj1.x);
                                //(C*D)-> si C=0: impossible car p2.y<HF, si D=0: impossible car p1.x<LF
                                if (coef_segment >= coef_coin) {
                                    bresenham(&proj1, &proj2, epaisseur, r,v,b);
                                }
                                else { return; }

    						}
    					} else {
    						//p2 PARTIE GAUCHE
    						/*double coefdir_coin = (proj1.y - HAUTEUR_FENETRE)/proj1.x;
    						double coefdir_segment = (proj1.y - proj2.y)/(proj1.x - proj2.x);
    						if (coefdir_segment > coefdir_coin) {
    							bresenham(&proj1, &proj2, epaisseur, r,v,b);
    						}
                            else { return; }*/

                            int coef_coin = (proj1.y-HAUTEUR_FENETRE)*(proj1.x - proj2.x);
                            //(A*B)-> si A=0: p2.y<HF donc afficher, si B=0: p2.y<HF donc afficher
                            int coef_segment = (proj1.y - proj2.y)*proj1.x;
                            //(C*D)-> si C=0: impossible car p2.y<HF, si D=0: ne s'affiche pas car p2.x<0
                            if (coef_segment >= coef_coin) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }

    					}
                    }
                    else { return; }

				}
			} else {
				//p1 CENTRE HAUT

                if (proj2.y >= 0) {
                    if (proj2.x >= 0) {
                        if (proj2.x < LARGEUR_FENETRE) {
                            bresenham(&proj1, &proj2, epaisseur, r,v,b);
                        } else {
                            //p2 PARTIE DROITE
                            /*double coefdir_coin = -proj1.y/(LARGEUR_FENETRE - proj1.x);
                            double coefdir_segment = (proj2.y - proj1.y)/(proj2.x - proj1.x);
                            if (coefdir_segment > coefdir_coin) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }*/

                            int coef_coin = -proj1.y*(proj2.x - proj1.x);
                            //(A*B)-> si A=0: impossible car p1.y<0, si B=0: impossible car p2.x>LF et p1.x au centre
                            int coef_segment = (proj2.y - proj1.y)*(LARGEUR_FENETRE - proj1.x);
                            //(C*D)-> si C=0: impossible car p2.y>=0 et p1.y<0, D=0: ne s'affiche pas
                            if (coef_segment >= coef_coin) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }

                        }
                    } else {
                        //p2 PARTIE GAUCHE
                        /*double coefdir_coin = -proj1.y/proj1.x;
                        double coefdir_segment = (proj2.y-proj1.y)/(proj1.x-proj2.x);
                        if (coefdir_segment > coefdir_coin) {
                            bresenham(&proj1, &proj2, epaisseur, r,v,b);
                        }
                        else { return; }*/

                        int coef_coin = -proj1.y*(proj1.x-proj2.x);
                        //(A*B)-> si A=0: impossible car p1.y<0, si B=0: impossible car p2.x<0 et p1.x>=0
                        int coef_segment = (proj2.y-proj1.y)*proj1.x;
                        //(C*D)-> si C=0: impossible car p2.y>=0 et p1.y<0, D=0: ne pas afficher car p2.x<0
                        if (coef_segment >= coef_coin) {
                            bresenham(&proj1, &proj2, epaisseur, r,v,b);
                        }
                        else { return; }

                    }
                }
                else { return; }

			}
		} else {
			//p1 PARTIE DROITE
            //if (proj1.x == proj2.x) { return; }

            if (proj2.x < LARGEUR_FENETRE) {
                if (proj1.y >= 0) {
                    if (proj1.y < HAUTEUR_FENETRE) {
                        //p1 AU MILIEU A DROITE

                        /*if (proj2.y >= 0 && proj2.y < HAUTEUR_FENETRE) {
                            bresenham(&proj1, &proj2, epaisseur, r,v,b);
                        } else {*/
                            if (proj2.y < 0) {
                                /*double coefdir_coin_haut = proj1.y/(proj1.x-LARGEUR_FENETRE);
                                double coefdir_segment = -proj2.y/(proj1.x-proj2.x);
                                if (coefdir_coin_haut > coefdir_segment) {
                                    bresenham(&proj1, &proj2, epaisseur, r,v,b);
                                }
                                else { return; }*/

                                int coef_coin_haut = proj1.y*(proj1.x-proj2.x);
                                //(A*B)-> si A=0: ne s'affiche pas (inutile car p2.y<0),
                                //B=0: impossible car p2.x<LF et p1.x>=LF
                                int coef_segment = -proj2.y*(proj1.x-LARGEUR_FENETRE);
                                //(C*D)-> si C=0: impossible car p2.y<0, D=0: s'affiche
                                if (coef_segment <= coef_coin_haut) {
                                    bresenham(&proj1, &proj2, epaisseur, r,v,b);
                                }
                                else { return; }

                            } else if (proj2.y < HAUTEUR_FENETRE) {
                                /*double coefdir_coin_bas = (HAUTEUR_FENETRE-proj1.y)/(proj1.x-LARGEUR_FENETRE);
                                double coefdir_segment = (HAUTEUR_FENETRE-proj2.y)/(proj1.x-proj2.x);
                                if (coefdir_coin_bas > coefdir_segment) {
                                    bresenham(&proj1, &proj2, epaisseur, r,v,b);
                                }
                                else { return; }*/

                                int coef_coin_bas = (HAUTEUR_FENETRE-proj1.y)*(proj1.x-proj2.x);
                                //(A*B)-> si A=0: ne s'affiche pas (inutile car p2.y>HF)
                                //B=0: impossible car p2.x<LF et p1.x>=LF
                                int coef_segment = (HAUTEUR_FENETRE-proj2.y)*(proj1.x-LARGEUR_FENETRE);
                                //(C*D)-> si C=0: s'affiche, D=0: s'affche
                                if (coef_segment <= coef_coin_bas) {
                                    bresenham(&proj1, &proj2, epaisseur, r,v,b);
                                }
                                else { return; }

                            } else {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                        //}

                    } else {
                        //p1 EN BAS A DROITE

                        if (proj2.y < HAUTEUR_FENETRE+1) {
                            /*double coefdir_coin_haut = proj1.y/(proj1.x-LARGEUR_FENETRE);
                            double coefdir_coin_bas = (proj1.y-HAUTEUR_FENETRE)/proj1.x;
                            double coefdir_segment = (proj1.y-proj2.y)/(proj1.x-proj2.x);
                            if (coefdir_coin_bas < coefdir_segment
                                && coefdir_coin_haut > coefdir_segment) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }*/

                            int coef_coin_haut = proj1.y*(proj1.x-proj2.x);
                            //(A*B) -> si A=0: impossible car p1.y<HF,
                            //B=0: impossible car p2.x<LF et p1.x >= LF
                            int coef_segment = (proj1.y-proj2.y)*(proj1.x-LARGEUR_FENETRE);
                            //(C*D) -> si C=0: impossible car p1.y>=HF et p2.y<HF
                            //D=0: doit s'afficher dans n'importe quel cas donc OK
                            if (coef_coin_haut >= coef_segment) {

                                int coef_coin_bas = (proj1.y-HAUTEUR_FENETRE)*(proj1.x-proj2.x);
                                //(A*B) -> si A=0: doit s'afficher car coef_coin_haut >= coef_segment donc OK
                                //B=0: impossible car p2.x<LF et p1.x >= LF
                                coef_segment = (proj1.y-proj2.y)*proj1.x;
                                //(C*D) -> si C=0: impossible car p1.y>=HF et p2.y<HF, D=0: impossible car p1.x>LF
                                if (coef_coin_bas <= coef_segment) {
                                    bresenham(&proj1, &proj2, epaisseur, r,v,b);
                                }
                                else { return; }
                            }
                            else { return; }
                        }
                        else { return; }

                    }
                } else {
                    //p1 EN HAUT A DROITE

                    if (proj2.y >= 0) {
                        /*double coefdir_coin_haut = -proj1.y/proj1.x;
                        double coefdir_coin_bas = (HAUTEUR_FENETRE-proj1.y)/(proj1.x-LARGEUR_FENETRE);
                        double coefdir_segment = (proj2.y-proj1.y)/(proj1.x-proj2.x);
                        if (coefdir_coin_haut < coefdir_segment
                            && coefdir_coin_bas > coefdir_segment) {
                            bresenham(&proj1, &proj2, epaisseur, r,v,b);
                        }
                        else { return; }*/

                        int coef_coin_haut = -proj1.y*(proj1.x-LARGEUR_FENETRE);
                        //(A*B) -> si A=0: impossible car p1.y<0, B=0: s'affiche même si peut ne pas l'être
                        int coef_segment = (HAUTEUR_FENETRE-proj1.y)*proj1.x;
                        //(C*D) -> si C=0: impossible car p1.y<0, D=0: impossible car p1.x>LF
                        if (coef_coin_haut <= coef_segment) {
                            int coef_coin_bas = (HAUTEUR_FENETRE-proj1.y)*(proj1.x-proj2.x);
                            //(A*B) -> si A=0: impossibla car p1.y<0, B=0: impossible car p1.x>=LF et p2.x<LF
                            coef_segment = (proj2.y-proj1.y)*(proj1.x-LARGEUR_FENETRE);
                            //(C*D) -> si C=0: impossible car p1.y<0 et p2.y>=0,
                            //D=0: doit s'afficher car coef_coin_haut <= coef_segment
                            if (coef_coin_bas >= coef_segment) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }
                        }
                        else { return; }
                    }
                    else { return; }

                }
            }
            else { return; }
		}
	} else {
		//p1 PARTIE GAUCHE
        //if (proj1.x == proj2.x) { return; }

        if (proj2.x >= 0) {
            if (proj1.y >= 0) {
                if (proj1.y < HAUTEUR_FENETRE) {
                    //p1 AU MILIEU A GAUCHE

                    /*if (proj2.y >= 0 && proj2.y < HAUTEUR_FENETRE) {
                        bresenham(&proj1, &proj2, epaisseur, r,v,b);
                    } else {*/
                        if (proj2.y < 0) {
                            /*double coefdir_coin_haut = proj1.y/-proj1.x;
                            double coefdir_segment = (proj1.y-proj2.y)/(proj2.x-proj1.x);
                            if (coefdir_coin_haut > coefdir_segment) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }*/

                            int coef_coin_haut = proj1.y*(proj2.x-proj1.x);
                            //(A*B) -> si A=0: ne s'affiche pas donc OK (car p2.y<0)
                            //B=0: impossible car p1.x<0 et p2.x>=0
                            int coef_segment = (proj1.y-proj2.y)*-proj1.x;
                            //(C*D) -> si C=0: impossible car p1.y>=0 et p2.y<0,
                            //D=0: s'affiche dans tout les cas donc OK
                            if (coef_coin_haut >= coef_segment) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }

                        } else if (proj2.y < HAUTEUR_FENETRE) {
                            /*double coefdir_coin_bas = (HAUTEUR_FENETRE-proj1.y)/-proj1.x;
                            double coefdir_segment = (proj2.y-proj1.y)/(proj2.x-proj1.x);
                            if (coefdir_coin_bas > coefdir_segment) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }*/

                            int coef_coin_bas = (HAUTEUR_FENETRE-proj1.y)*(proj2.x-proj1.x);
                            //(A*B) -> si A=0: impossible car p1.y<HF, B=0: impossible car p2.x>=0 et p1.x<0
                            int coef_segment = (proj2.y-proj1.y)*-proj1.x;
                            //(C*D) -> si C=0: impossibel car p2.y>=HF et p1.y<HF
                            //D=0: s'affiche même si peut ne pas l'être
                            if (coef_coin_bas >= coef_segment) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }
                        } else {
                            bresenham(&proj1, &proj2, epaisseur, r,v,b);
                        }
                    //}

                }
                else {
                    //p1 EN BAS A GAUCHE

                    if (proj2.y < HAUTEUR_FENETRE) {
                        /*double coefdir_coin_haut = proj1.y/-proj1.x;
                        double coefdir_coin_bas = (proj1.y-HAUTEUR_FENETRE)/(LARGEUR_FENETRE-proj1.x);
                        double coefdir_segment = (proj1.y-proj2.y)/(proj2.x-proj1.x);
                        if (coefdir_coin_haut > coefdir_segment
                            && coefdir_coin_bas < coefdir_segment) {
                            bresenham(&proj1, &proj2, epaisseur, r,v,b);
                        }
                        else { return; }*/

                        int coef_coin_haut = proj1.y*(proj2.x-proj1.x);
                        //(A*B) -> si A=0: impossible car p1.y>HF, B=0: impossible car p2.x>=0 et p1.x<0
                        int coef_segment = (proj1.y-proj2.y)*(-proj1.x);
                        //(C*D) -> si C=0: impossibla car p2.y<HF et p1.y>=HF,
                        //D=0: s'affiche même si peut ne pas l'être
                        if (coef_coin_haut >= coef_segment) {

                            int coef_coin_bas = (proj1.y-HAUTEUR_FENETRE)*(proj2.x-proj1.x);
                            //(A*B) -> si A=0: s'affiche donc OK (car p2.y<HF et coef_coin_haut >= coef_segment)
                            //B=0: impossible car p2.x>=0 et p1.x<0
                            coef_segment = (proj1.y-proj2.y)*(LARGEUR_FENETRE-proj1.x);
                            //(C*D) -> si C=0: impossibla car p2.y<HF et p1.y>=HF, D=0: impossible car p1.x<0
                            if (coef_coin_bas <= coef_segment) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }
                        }
                        else { return; }

                    }
                    else { return; }

                }
            }
            else {
                //p1 EN HAUT A GAUCHE

                if (proj2.y >= 0) {
                    /*double coefdir_coin_haut = -proj1.y/(LARGEUR_FENETRE-proj1.x);
                    double coefdir_coin_bas = (HAUTEUR_FENETRE-proj1.y)/-proj1.x;
                    double coefdir_segment = (proj2.y-proj1.y)/(proj2.x-proj1.x);
                    if (coefdir_coin_haut < coefdir_segment
                        && coefdir_coin_bas > coefdir_segment) {
                        bresenham(&proj1, &proj2, epaisseur, r,v,b);
                    }
                    else { return; }*/

                    int coef_coin_haut = -proj1.y*(proj2.x-proj1.x);
                    //(A*B) -> A=0: impossible car p1.y<0, B=0: impossible car p2.x>=0 et p1.x<0
                    int coef_segment = (proj2.y-proj1.y)*(LARGEUR_FENETRE-proj1.x);
                    //(C*D) -> C=0: impossible car p2.y>=0 et p1.y<0, D=0: impossible car p1.x<0
                    if (coef_coin_haut <= coef_segment) {

                        int coef_coin_bas = (HAUTEUR_FENETRE-proj1.y)*(proj2.x-proj1.x);
                        //(A*B) -> si A=0: impossible car p1.y<0, B=0: impossible car p2.x>=0 et p1.x<0
                        coef_segment = (proj2.y-proj1.y)*-proj1.x;
                        //(C*D) -> si C=0: impossible car p2.y>=0 et p1.y<0, D=0: impossible car p1.x<0
                        if (coef_coin_bas >= coef_segment) {
                            bresenham(&proj1, &proj2, epaisseur, r,v,b);
                        }
                    }
                    else { return; }

                }
                else { return; }

            }
        }
        else { return; }

	}
}

void afficher_arete(Arete3D* a, int epaisseur, int r, int v, int b){
	Point2D proj1, proj2;//= {500, 6000};//= {500, -400}
    int minx=300, maxx=LARGEUR_FENETRE-300,     //NON UNSIGNED car problème de comparaison avec les négatifs
        miny=300, maxy=HAUTEUR_FENETRE-300;
    //unsigned int minx=0, maxx=LARGEUR_FENETRE,
    //            miny=0, maxy=HAUTEUR_FENETRE;
	proj_point( a->p1 , &proj1 );
	proj_point( a->p2 , &proj2 );

	if (proj1.x >= minx) {
		if (proj1.x < maxx) {
			//p1 PARTIE CENTRE
			if (proj1.y >= miny) {

				if (proj1.y < maxy) {
					bresenham(&proj1, &proj2, epaisseur, r,v,b);
				} else {
					//p1 CENTRE BAS
                    if (proj2.y < maxy) {
                        if (proj2.x >= minx) {
    						if (proj2.x < maxx) {
    							bresenham(&proj1, &proj2, epaisseur, r,v,b);
    						} else {
    							//p2 PARTIE DROITE
                                int coef_coin = (proj1.y-maxy)*(proj2.x - proj1.x);
                                //(A*B)-> si A=0: p2.y<HF donc s'affiche, si B=0: impossible car proj2.x>LF
                                int coef_segment = (proj1.y - proj2.y)*(maxx-proj1.x);
                                //(C*D)-> si C=0: impossible car p2.y<HF, si D=0: impossible car p1.x<LF
                                if (coef_segment >= coef_coin) {
                                    bresenham(&proj1, &proj2, epaisseur, r,v,b);
                                }
                                else { return; }

    						}
    					} else {
    						//p2 PARTIE GAUCHE
                            int coef_coin = (proj1.y-maxy)*(proj1.x - proj2.x);
                            //(A*B)-> si A=0: p2.y<HF donc afficher, si B=0: p2.y<HF donc afficher
                            int coef_segment = (proj1.y - proj2.y)*(proj1.x-minx);
                            //(C*D)-> si C=0: impossible car p2.y<HF, si D=0: ne s'affiche pas car p2.x<0
                            if (coef_segment >= coef_coin) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }

    					}
                    }
                    else { return; }

				}
			} else {
				//p1 CENTRE HAUT
                if (proj2.y >= miny) {
                    if (proj2.x >= minx) {
                        if (proj2.x < maxx) {
                            bresenham(&proj1, &proj2, epaisseur, r,v,b);
                        } else {
                            //p2 PARTIE DROITE
                            int coef_coin = (miny-proj1.y)*(proj2.x - proj1.x);
                            //(A*B)-> si A=0: impossible car p1.y<0, si B=0: impossible car p2.x>LF et p1.x au centre
                            int coef_segment = (proj2.y - proj1.y)*(maxx - proj1.x);
                            //(C*D)-> si C=0: impossible car p2.y>=0 et p1.y<0, D=0: ne s'affiche pas
                            if (coef_segment >= coef_coin) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }

                        }
                    } else {
                        //p2 PARTIE GAUCHE
                        int coef_coin = (miny-proj1.y)*(proj1.x-proj2.x);
                        //(A*B)-> si A=0: impossible car p1.y<0, si B=0: impossible car p2.x<0 et p1.x>=0
                        int coef_segment = (proj2.y-proj1.y)*(proj1.x-minx);
                        //(C*D)-> si C=0: impossible car p2.y>=0 et p1.y<0, D=0: ne pas afficher car p2.x<0
                        if (coef_segment >= coef_coin) {
                            bresenham(&proj1, &proj2, epaisseur, r,v,b);
                        }
                        else { return; }

                    }
                }
                else { return; }

			}
		} else {
			//p1 PARTIE DROITE
            if (proj2.x < maxx) {
                if (proj1.y >= miny) {
                    if (proj1.y < maxy) {
                        //p1 AU MILIEU A DROITE

                            if (proj2.y < miny) {
                                int coef_coin_haut = (proj1.y-miny)*(proj1.x-proj2.x);
                                //(A*B)-> si A=0: ne s'affiche pas (inutile car p2.y<0),
                                //B=0: impossible car p2.x<LF et p1.x>=LF
                                int coef_segment = (miny-proj2.y)*(proj1.x-maxx);
                                //(C*D)-> si C=0: impossible car p2.y<0, D=0: s'affiche
                                if (coef_segment <= coef_coin_haut) {
                                    bresenham(&proj1, &proj2, epaisseur, r,v,b);
                                }
                                else { return; }

                            } else if (proj2.y < maxy) {
                                int coef_coin_bas = (maxy-proj1.y)*(proj1.x-proj2.x);
                                //(A*B)-> si A=0: ne s'affiche pas (inutile car p2.y>HF)
                                //B=0: impossible car p2.x<LF et p1.x>=LF
                                int coef_segment = (maxy-proj2.y)*(proj1.x-maxx);
                                //(C*D)-> si C=0: s'affiche, D=0: s'affche
                                if (coef_segment <= coef_coin_bas) {
                                    bresenham(&proj1, &proj2, epaisseur, r,v,b);
                                }
                                else { return; }

                            } else {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                        //}

                    } else {
                        //p1 EN BAS A DROITE

                        if (proj2.y < maxy) {
                            int coef_coin_haut = (proj1.y-miny)*(proj1.x-proj2.x);
                            //(A*B) -> si A=0: impossible car p1.y<HF,
                            //B=0: impossible car p2.x<LF et p1.x >= LF
                            int coef_segment = (proj1.y-proj2.y)*(proj1.x-maxx);
                            //(C*D) -> si C=0: impossible car p1.y>=HF et p2.y<HF
                            //D=0: doit s'afficher dans n'importe quel cas donc OK
                            if (coef_coin_haut >= coef_segment) {

                                int coef_coin_bas = (proj1.y-maxy)*(proj1.x-proj2.x);
                                //(A*B) -> si A=0: doit s'afficher car coef_coin_haut >= coef_segment donc OK
                                //B=0: impossible car p2.x<LF et p1.x >= LF
                                coef_segment = (proj1.y-proj2.y)*(proj1.x-minx);
                                //(C*D) -> si C=0: impossible car p1.y>=HF et p2.y<HF, D=0: impossible car p1.x>LF
                                if (coef_coin_bas <= coef_segment) {
                                    bresenham(&proj1, &proj2, epaisseur, r,v,b);
                                }
                                else { return; }
                            }
                            else { return; }
                        }
                        else { return; }

                    }
                } else {
                    //p1 EN HAUT A DROITE

                    if (proj2.y >= miny) {
                        int coef_coin_haut = (miny-proj1.y)*(proj1.x-maxx);
                        //(A*B) -> si A=0: impossible car p1.y<0, B=0: s'affiche même si peut ne pas l'être
                        int coef_segment = (maxy-proj1.y)*(proj1.x-minx);
                        //(C*D) -> si C=0: impossible car p1.y<0, D=0: impossible car p1.x>LF
                        if (coef_coin_haut <= coef_segment) {
                            int coef_coin_bas = (maxy-proj1.y)*(proj1.x-proj2.x);
                            //(A*B) -> si A=0: impossibla car p1.y<0, B=0: impossible car p1.x>=LF et p2.x<LF
                            coef_segment = (proj2.y-proj1.y)*(proj1.x-maxx);
                            //(C*D) -> si C=0: impossible car p1.y<0 et p2.y>=0,
                            //D=0: doit s'afficher car coef_coin_haut <= coef_segment
                            if (coef_coin_bas >= coef_segment) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }
                        }
                        else { return; }
                    }
                    else { return; }

                }
            }
            else { return; }
		}
	} else {
		//p1 PARTIE GAUCHE
        if (proj2.x >= minx) {
            if (proj1.y >= miny) {
                if (proj1.y < maxy) {
                    //p1 AU MILIEU A GAUCHE

                        if (proj2.y < miny) {
                            int coef_coin_haut = (proj1.y-miny)*(proj2.x-proj1.x);
                            //(A*B) -> si A=0: ne s'affiche pas donc OK (car p2.y<0)
                            //B=0: impossible car p1.x<0 et p2.x>=0
                            int coef_segment = (proj1.y-proj2.y)*(minx-proj1.x);
                            //(C*D) -> si C=0: impossible car p1.y>=0 et p2.y<0,
                            //D=0: s'affiche dans tout les cas donc OK
                            if (coef_coin_haut >= coef_segment) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }

                        } else if (proj2.y < maxy) {
                            int coef_coin_bas = (maxy-proj1.y)*(proj2.x-proj1.x);
                            //(A*B) -> si A=0: impossible car p1.y<HF, B=0: impossible car p2.x>=0 et p1.x<0
                            int coef_segment = (proj2.y-proj1.y)*(minx-proj1.x);
                            //(C*D) -> si C=0: impossibel car p2.y>=HF et p1.y<HF
                            //D=0: s'affiche même si peut ne pas l'être
                            if (coef_coin_bas >= coef_segment) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }
                        } else {
                            bresenham(&proj1, &proj2, epaisseur, r,v,b);
                        }
                    //}

                }
                else {
                    //p1 EN BAS A GAUCHE

                    if (proj2.y < maxy) {
                        int coef_coin_haut = (proj1.y-miny)*(proj2.x-proj1.x);
                        //(A*B) -> si A=0: impossible car p1.y>HF, B=0: impossible car p2.x>=0 et p1.x<0
                        int coef_segment = (proj1.y-proj2.y)*(minx-proj1.x);
                        //(C*D) -> si C=0: impossibla car p2.y<HF et p1.y>=HF,
                        //D=0: s'affiche même si peut ne pas l'être
                        if (coef_coin_haut >= coef_segment) {

                            int coef_coin_bas = (proj1.y-maxy)*(proj2.x-proj1.x);
                            //(A*B) -> si A=0: s'affiche donc OK (car p2.y<HF et coef_coin_haut >= coef_segment)
                            //B=0: impossible car p2.x>=0 et p1.x<0
                            coef_segment = (proj1.y-proj2.y)*(maxx-proj1.x);
                            //(C*D) -> si C=0: impossibla car p2.y<HF et p1.y>=HF, D=0: impossible car p1.x<0
                            if (coef_coin_bas <= coef_segment) {
                                bresenham(&proj1, &proj2, epaisseur, r,v,b);
                            }
                            else { return; }
                        }
                        else { return; }

                    }
                    else { return; }

                }
            }
            else {
                //p1 EN HAUT A GAUCHE

                if (proj2.y >= miny) {
                    int coef_coin_haut = (miny-proj1.y)*(proj2.x-proj1.x);
                    //(A*B) -> A=0: impossible car p1.y<0, B=0: impossible car p2.x>=0 et p1.x<0
                    int coef_segment = (proj2.y-proj1.y)*(maxx-proj1.x);
                    //(C*D) -> C=0: impossible car p2.y>=0 et p1.y<0, D=0: impossible car p1.x<0
                    if (coef_coin_haut <= coef_segment) {

                        int coef_coin_bas = (maxy-proj1.y)*(proj2.x-proj1.x);
                        //(A*B) -> si A=0: impossible car p1.y<0, B=0: impossible car p2.x>=0 et p1.x<0
                        coef_segment = (proj2.y-proj1.y)*(minx-proj1.x);
                        //(C*D) -> si C=0: impossible car p2.y>=0 et p1.y<0, D=0: impossible car p1.x<0
                        if (coef_coin_bas >= coef_segment) {
                            bresenham(&proj1, &proj2, epaisseur, r,v,b);
                        }
                    }
                    else { return; }

                }
                else { return; }

            }
        }
        else { return; }

	}
}


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
		}
	}
}
