#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>


#include "constantes.h"
#include "struct.h"
#include "rendu.h"
#include "vecteur3d.h"



extern SDL_Window* pWindow;
extern SDL_Renderer* renderer;
Point3D origine = {0, 0, 0};
double rayon = 1000;
double alpha = 0, beta = 0;

Vecteur3D v_cartesien_x = {1, 0, 0};
Vecteur3D v_cartesien_y = {0, 1, 0};
Vecteur3D v_cartesien_z = {0, 0, 1};

Vecteur3D vcam,
	vplan_cam_i, vplan_cam_j;
Point3D pcam;
Point3D pnord;




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


Point2D* projPoint1(Point3D* p, Point2D* proj){

	Vecteur3D vcam_p = getVecteur(pcam, *p);

	proj->x = (int) ((scalaire3D(vcam_p, vplan_cam_i) / scalaire3D(vplan_cam_i, vplan_cam_i))
		*GRANDEUR_FENETRE*5 + (LARGEUR_FENETRE/2)) + 0.5;
	proj->y = (int) ((scalaire3D(vcam_p, vplan_cam_j) / scalaire3D(vplan_cam_j, vplan_cam_j))
		*GRANDEUR_FENETRE*5 + (HAUTEUR_FENETRE/2)) + 0.5;
}


void updateCam(){
	pcam.x = rayon*sin(alpha)*cos(beta);
	pcam.y = rayon*sin(alpha)*sin(beta);
	pcam.z = rayon*cos(alpha);
	vcam = getVecteur(pcam, origine);

	calculer_vecteurs_plancam();
}


NoeudArete3D* ajoutArete(NoeudArete3D* n, Point3D* p1, Point3D* p2){
	Arete3D* a = malloc(sizeof(Arete3D));
	a->p1 = p1; a->p2 = p2;
	NoeudArete3D* new_n = malloc(sizeof(NoeudArete3D));
	new_n->arete = a;
	new_n->suiv = NULL;

	if (n == NULL) {
		return new_n;
	} else {
		while (n->suiv != NULL) {
			n = n->suiv;
		}
		n->suiv = new_n;
		return new_n;
	}
}

/*void freeNoeud(NoeudArete3D* n){		//A FAIRE
	NoeudArete3D* tete_prisme = n;
	while (n != NULL) {
		while (n->suiv != NULL) {
			n = n->suiv;
		}
		n = tete_prisme;
	}
}*/


void afficherAretes(NoeudArete3D* tete, int epaisseur, int r, int g, int b){
	NoeudArete3D* ni = tete;
	Point2D proj1, proj2;
	while (ni != NULL) {
		projPoint1(ni->arete->p1, &proj1);
		projPoint1(ni->arete->p2, &proj2);
		bresenham(&proj1, &proj2, epaisseur, r,g,b);
		ni = ni->suiv;
	}
}


int main(int argc, char const *argv[])
{
	int quit = 0;
	SDL_Event event;


	if(init_sdl()) {
		init_renderer();
		SDL_RenderClear(renderer);

		int mx, my;
		Point2D proj1, proj2;

		Point3D pcard[] = {{0,0,rayon}		//nord
							,{0,0,-rayon}	//sud
							,{rayon,0,0}	//est
							,{-rayon,0,0}	//ouest
							,{0,rayon,0}	//arriere
							,{0,-rayon,0}};	//avant

		Point3D ppyramide[] = {{100, -100, 100}		//base
							, {100, 100, 100}
							, {-100, -100, 100}
							, {-100, 100, 100}
							, {0, 0, -100}};		//sommet


		NoeudArete3D* tete_prisme = NULL;
		tete_prisme = ajoutArete(tete_prisme, pcard+1,pcard+2);
		ajoutArete(tete_prisme, pcard+1,pcard+3);
		ajoutArete(tete_prisme, pcard+1,pcard+4);
		ajoutArete(tete_prisme, pcard+1,pcard+5);
		ajoutArete(tete_prisme, pcard+0,pcard+2);
		ajoutArete(tete_prisme, pcard+0,pcard+3);
		ajoutArete(tete_prisme, pcard+0,pcard+4);
		ajoutArete(tete_prisme, pcard+0,pcard+5);
		ajoutArete(tete_prisme, pcard+2,pcard+4);
		ajoutArete(tete_prisme, pcard+4,pcard+3);
		ajoutArete(tete_prisme, pcard+3,pcard+5);
		ajoutArete(tete_prisme, pcard+5,pcard+2);

		NoeudArete3D* tete_pyramide = NULL;
		tete_pyramide = ajoutArete(tete_pyramide, ppyramide+0,ppyramide+1);
		ajoutArete(tete_pyramide, ppyramide+1,ppyramide+3);
		ajoutArete(tete_pyramide, ppyramide+3,ppyramide+2);
		ajoutArete(tete_pyramide, ppyramide+2,ppyramide+0);
		ajoutArete(tete_pyramide, ppyramide+4,ppyramide+0);
		ajoutArete(tete_pyramide, ppyramide+4,ppyramide+1);
		ajoutArete(tete_pyramide, ppyramide+4,ppyramide+2);
		ajoutArete(tete_pyramide, ppyramide+4,ppyramide+3);

		afficherAretes(tete_prisme,  4, 0,128,255);
		afficherAretes(tete_pyramide, 4, 255,255,0);


		updateCam();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0,   255);
		SDL_RenderPresent(renderer);


		while (!quit) {
			while (SDL_PollEvent(&event)){
				switch(event.type) {
			        case SDL_QUIT: // Clic sur la croix
			            quit=1;
			            break;

					case SDL_KEYDOWN:
						switch( event.key.keysym.sym )
                        {
							case SDLK_UP:
								alpha += 0.05;
								updateCam();
								break;
							case SDLK_DOWN:
								alpha -= 0.05;
								updateCam();
								break;
							case SDLK_LEFT:
								beta += 0.05;
								updateCam();
								break;
							case SDLK_RIGHT:
								beta -= 0.05;
								updateCam();
								break;
						}
						break;

					case SDL_MOUSEWHEEL:
						//printf("Roll %d\n", event.wheel.y*10);
						rayon += event.wheel.y*100;
						updateCam();
						break;

					case SDL_MOUSEMOTION:
						SDL_GetMouseState(&mx, &my);
						//printf("mx: %d, my: %d\n", mx, my);
						alpha = (HAUTEUR_FENETRE-my)/500.0;
						beta = (LARGEUR_FENETRE-mx)/500.0;
						updateCam();
						break;
				}
			}

			SDL_RenderClear(renderer);

			afficherAretes(tete_prisme,  4, 0,128,255);
			afficherAretes(tete_pyramide, 4, 255,255,0);

			SDL_RenderPresent(renderer);
		}

		//freeNoeud

		SDL_DestroyWindow(pWindow);
	}
	else {
		fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Quit();

	return 0;

}
