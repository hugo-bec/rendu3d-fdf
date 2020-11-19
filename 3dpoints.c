#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "vecteur3d.h"

//gcc -o 3dpoints.out 3dpoints.c `sdl2-config --cflags --libs` -lm

#define GRANDEUR_FENETRE 100
#define HAUTEUR_FENETRE 10*GRANDEUR_FENETRE
#define LARGEUR_FENETRE 15*GRANDEUR_FENETRE
#define PI 3.14159265




SDL_Window* pWindow = NULL;
SDL_Renderer* renderer;
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
	//produitk(vplan_cam_i, -1, &vplan_cam_i);

	produitk(v_cart_rayon[0], sin(beta), &vpi[0]);
	produitk(v_cart_rayon[1], -cos(beta), &vpi[1]);
	addition(vpi, 2, &vplan_cam_i);
	//produitk(vplan_cam_j, -1, &vplan_cam_j);
}


void renduPoint1(Point3D p, int r, int g, int b){

	Point2D paff;
	Vecteur3D vcam_p = getVecteur(pcam, p);

	paff.x = (scalaire3D(vcam_p, vplan_cam_i) / scalaire3D(vplan_cam_i, vplan_cam_i))*400 + (LARGEUR_FENETRE/2);
	paff.y = (scalaire3D(vcam_p, vplan_cam_j) / scalaire3D(vplan_cam_j, vplan_cam_j))*400 + (HAUTEUR_FENETRE/2);

	afficherPoint(paff, 16, r,g,b);
}


void updateCam(){
	//printf("alpha: %lf, beta: %lf\n", alpha, beta);
	pcam.x = rayon*sin(alpha)*cos(beta);
	pcam.y = rayon*sin(alpha)*sin(beta);
	pcam.z = rayon*cos(alpha);
	vcam = getVecteur(pcam, origine);

	calculer_vecteurs_plancam();

	/*calculer_vecteurs_plancam();
	printCoordVecteur3D(vplan_cam_i);
	printCoordVecteur3D(vplan_cam_j);
	printf("scalaire vplan_cam_i/j: %lf\n" ,scalaire(vplan_cam_i, vplan_cam_j));*/
}


int main(int argc, char const *argv[])
{
	int quit = 0;
	SDL_Event event;

	if (SDL_Init(SDL_INIT_VIDEO) != 0 ) {
    	fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
		return EXIT_FAILURE;
    }

	pWindow = SDL_CreateWindow("3dmatrices",
	SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    LARGEUR_FENETRE,
	HAUTEUR_FENETRE,
	SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if(pWindow) {
		renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // Création du renderer
		if(renderer == NULL) {
		   printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
		   return EXIT_FAILURE;
		}

		SDL_RenderClear(renderer);

		//SDL_SetRenderDrawColor(renderer, 0, 255, 0,   255);
		int mx, my;

		Point3D pnord = {0,0,rayon};
		Point3D psud = {0,0,-rayon};
		Point3D pest = {rayon,0,0};
		Point3D pouest = {-rayon,0,0};
		Point3D parriere = {0,rayon,0};
		Point3D pavant = {0,-rayon,0};

		Point3D p1 = {100, -100, 100};
		Point3D p2 = {100, 100, 100};
		Point3D p3 = {-100, -100, 100};
		Point3D p4 = {-100, 100, 100};
		Point3D p5 = {0, 0, -100};

		Point3D a = {1,2,3};
		Point3D b = {1,2,5};

		Vecteur3D va = {5,1,0};
		Vecteur3D vb = {2,3,0};

		updateCam();
		vcam = getVecteur(pcam, origine);

		printCoordVecteur3D(getVecteur(a, b));
		printf("scalaire va vb: %lf\n", scalaire3D(va, vb));
		printf("longueur va: %lf\n", longueur(va));
		printf("longueur vb: %lf\n", longueur(vb));

		printf("angle va vb: %lf\n", getAngle(va, vb));

		calculer_vecteurs_plancam();
		printCoordVecteur3D(vplan_cam_i);
		printCoordVecteur3D(vplan_cam_j);
		printf("scalaire vplan_cam_i/j: %lf\n" ,scalaire3D(vplan_cam_i, vplan_cam_j));

		Vecteur3D vpk;
		produitk(va, 4.26, &vpk);
		printCoordVecteur3D(vpk);

		Vecteur3D vt[3] = {{1,2,3}, {4,5,6}, {7,8,9}};
		//printCoordVecteur3D(vt[0]);
		//printCoordVecteur3D(vt[1]);
		//printCoordVecteur3D(vt[2]);
		addition(vt, 3, &vpk);
		printCoordVecteur3D(vpk);




		renduPoint1(origine, 255,0,0);
		renduPoint1(pnord, 0,255,255);
		renduPoint1(psud, 0,255,255);
		renduPoint1(pest, 0,255,255);
		renduPoint1(pouest, 0,255,255);
		renduPoint1(pavant, 0,255,255);
		renduPoint1(parriere, 0,255,255);

		renduPoint1(p1,0,255,0);
		renduPoint1(p2,0,255,0);
		renduPoint1(p3,0,255,0);
		renduPoint1(p4,0,255,0);
		renduPoint1(p5,0,255,0);






		SDL_SetRenderDrawColor(renderer, 0, 0, 0,   255);

		SDL_RenderPresent(renderer);



		while (!quit) {
			//actualiser = 0;
			while (SDL_PollEvent(&event)){
				switch(event.type) {
			        case SDL_QUIT: // Clic sur la croix
			            quit=1;
			            break;

					case SDL_KEYDOWN:
						//printf("KEY\n");
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
						//printCoordPoint3D(pcam);
						//printCoordVecteur3D(vcam);
						break;

					case SDL_MOUSEWHEEL:
						printf("Roll %d\n", event.wheel.y*10);
						rayon += event.wheel.y*100;
						updateCam();
						break;

					case SDL_MOUSEMOTION:
						SDL_GetMouseState(&mx, &my);
						printf("mx: %d, my: %d\n", mx, my);
						alpha = (HAUTEUR_FENETRE-my)/500.0;
						beta = (LARGEUR_FENETRE-mx)/500.0;
						updateCam();
						break;
				}
			}

			SDL_RenderClear(renderer);


			renduPoint1(origine, 255,0,0);
			renduPoint1(pnord, 0,255,255);
			renduPoint1(psud, 0,255,255);
			renduPoint1(pest, 0,255,255);
			renduPoint1(pouest, 0,255,255);
			renduPoint1(pavant, 0,255,255);
			renduPoint1(parriere, 0,255,255);

			renduPoint1(p1,0,255,0);
			renduPoint1(p2,0,255,0);
			renduPoint1(p3,0,255,0);
			renduPoint1(p4,0,255,0);
			renduPoint1(p5,0,255,0);


			SDL_RenderPresent(renderer);
		}

		SDL_DestroyWindow(pWindow);
	}
	else {
		fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Quit();

	return 0;

}
