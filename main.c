#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>


#include "constantes.h"
#include "struct.h"
#include "rendu.h"
#include "graphe.h"
#include "vecteur3d.h"
#include "vecteur2d.h"
#include "generation_terrain.h"



/*
 * variables globales init dans rendu.c :
 */
//SDL
extern SDL_Window* pWindow;
extern SDL_Renderer* renderer;

//SDL Texture
extern Uint32 *pixels;
extern void *tmp;
extern int pitch;
extern SDL_Texture *texture;

//Caméra
extern double rayon;
extern double alpha, beta;




int main(int argc, char const *argv[])
{
	int quit = 0;
	SDL_Event event;
	clock_t t_deb_frame, t_fin_frame;
	float sensi_souris = 200;

	srand(time(NULL));

	if(init_sdl()) {
		init_renderer();
		init_texture();
		SDL_RenderClear(renderer);


		//test
		Point2D pa = {500, 600},
				pb = {500, 400},
				pc = {0, HAUTEUR_FENETRE};
		//Arete3D* atest = {&pa, &pb};
		/*Vecteur2D va, vb;
		getVecteur2D(&pa, &pb, &va);
		getVecteur2D(&pa, &pc, &vb);
		printf("angle: %lf\n", getAngle2Dv(&vb, &va));*/



		int mx, my;
		Point2D proj1, proj2;
		Graphe3D pyramide = {NULL, NULL};
		Graphe3D prisme = {NULL, NULL};
		Point2D rect_ecran[] = {{300, 300},
				{LARGEUR_FENETRE-300, 300},
				{LARGEUR_FENETRE-300, HAUTEUR_FENETRE-300},
				{300, HAUTEUR_FENETRE-300}};

		// nord, sud, est, ouest, arriere, avant
		double points_cardinaux[][3] = {{0,0,rayon},{0,0,-rayon},{rayon,0,0},{-rayon,0,0},{0,rayon,0},{0,-rayon,0}};
		ajout_points_xyz(&prisme, points_cardinaux, 6);
		int aretes_prisme[][2] = {{1,2},{1,3},{1,4},{1,5},{0,2},{0,3},{0,4},{0,5},{2,4},{4,3},{3,5},{5,2}};
		ajout_aretes_i(&prisme, aretes_prisme, 12);


		//0-3base, 4sommet
		double points_pyramide[][3] = {{100,-100,100},{100,100,100},{-100,-100,100},{-100,100,100},{0,0,-100}};
		ajout_points_xyz(&pyramide, points_pyramide, 5);
		int aretes_pyramide[][2] = {{0,1},{1,3},{3,2},{2,0},{4,0},{4,1},{4,2},{4,3}};
		ajout_aretes_i(&pyramide, aretes_pyramide, 8);


		/*Graphe3D gaelle = {NULL, NULL};
		double points_gaelle[][3] = {{0,-100,100},{0,0,0},{0,100,100},{0,0,200}};
		ajout_points_xyz(&gaelle, points_gaelle, 4);
		int aretes_gaelle[][2] = {{0,1},{1,2},{2,3},{3,0}};
		ajout_aretes_i(&gaelle, aretes_gaelle, 4);*/


		//initialisation du terrain
		size_t nbp_x = 500, nbp_y = 500;
		GrapheStatique3D* terrain = creer_terrain_plat(nbp_x, nbp_y, 50);
		//printf("nbaretes: %lu\n", terrain->nbAretes);

		// formation du terrain
		//generation_double_boucle(terrain, nbp_x, nbp_y, 0, 50, 50);
		//generation_spirale(terrain, nbp_x, nbp_y, 0, 50, 20);
		//generation_random(terrain, nbp_x, nbp_y, 0, 100);

		generation_bruit_perlin(terrain, nbp_x, nbp_y, 0 /*niveau*/, 1500 /*relief*/, 0.02 /*frequence*/);
		generation_bruit_perlin(terrain, nbp_x, nbp_y, 0 /*niveau*/, 400 /*relief*/, 0.10 /*frequence*/);
		generation_bruit_perlin(terrain, nbp_x, nbp_y, 0 /*niveau*/, 100 /*relief*/, 0.30 /*frequence*/);

		//printf("%lu\n", CLOCKS_PER_SEC);

		while (!quit) {

			t_deb_frame = t_fin_frame;

			while (SDL_PollEvent(&event)){
				switch(event.type) {
			        case SDL_QUIT: // Clic sur la croix
			            quit=1;
			            break;

					case SDL_KEYDOWN:
						switch( event.key.keysym.sym )
                        {
							case SDLK_UP:
								alpha += sensi_souris;
								update_cam();
								break;
							case SDLK_DOWN:
								alpha -= sensi_souris;
								update_cam();
								break;
							case SDLK_LEFT:
								beta += sensi_souris;
								update_cam();
								break;
							case SDLK_RIGHT:
								beta -= sensi_souris;
								update_cam();
								break;
						}
						break;

					case SDL_MOUSEWHEEL:
						//printf("Roll %d\n", event.wheel.y*10);
						rayon += event.wheel.y*100;
						update_cam();
						break;

					case SDL_MOUSEMOTION:
						SDL_GetMouseState(&mx, &my);
						//printf("mx: %d, my: %d\n", mx, my);
						alpha = (HAUTEUR_FENETRE-my)/sensi_souris;
						beta = (LARGEUR_FENETRE-mx)/sensi_souris;
						update_cam();
						break;
				}
			}

			//Nétotage de l'ancienne frame
			SDL_RenderClear(renderer);	//renderer
			memset(pixels, 0, LARGEUR_FENETRE * HAUTEUR_FENETRE * sizeof(Uint32));	//texture

			SDL_LockTexture(texture, NULL, &tmp, &pitch);


				//afficher_points_gstat(terrain, 1, 128,255,0);
				afficher_aretes_gstat(terrain, 1, 128,255,0);
				//afficher_couleur_relief_points(terrain, 2, -750, 750);

				afficher_aretes(&pyramide,  4, 255,255,0);
				afficher_aretes(&prisme,  4, 0,128,255);
				//afficher_aretes(&gaelle,  4, 255,0,255);

				/*for (size_t i=0; i<4; i++) {
					bresenham(rect_ecran+(i%4), rect_ecran+((i+1)%4), 5, 255, 0, 255);
				}*/


			SDL_UnlockTexture(texture);
			// afficher la nouvelle frame
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);


			t_fin_frame = clock();
			printf("fps: %lf\r", CLOCKS_PER_SEC/((double)t_fin_frame-t_deb_frame));
			fflush(stdout);
		}

		//freeNoeud

		//SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);

		SDL_DestroyWindow(pWindow);
	}
	else {
		fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Quit();

	return 0;

}
