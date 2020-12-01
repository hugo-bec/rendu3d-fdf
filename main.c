#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>


#include "constantes.h"
#include "struct.h"
#include "graphe.h"
#include "rendu.h"
#include "vecteur3d.h"
#include "generation_terrain.h"




extern SDL_Window* pWindow;
extern SDL_Renderer* renderer;

extern double rayon;
extern double alpha, beta;




int main(int argc, char const *argv[])
{
	int quit = 0;
	SDL_Event event;

	srand(time(NULL));

	if(init_sdl()) {
		init_renderer();
		SDL_RenderClear(renderer);

		update_cam();

		int mx, my;
		Point2D proj1, proj2;
		Graphe3D pyramide = {NULL, NULL};
		Graphe3D prisme = {NULL, NULL};

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


		//initialisation du terrain
		size_t nbp_x = 30, nbp_y = 30;
		GrapheStatique3D* terrain_stat = creer_graphe_statique(
			nbp_x * nbp_y,
			((nbp_x-1) * nbp_y) + ((nbp_y-1) * nbp_x) );
		init0_graphe_statique(terrain_stat);

		// formation du terrain
		generation_double_boucle(terrain_stat, nbp_x, nbp_y, 0, 50, 20);

		

		update_cam();
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
								update_cam();
								break;
							case SDLK_DOWN:
								alpha -= 0.05;
								update_cam();
								break;
							case SDLK_LEFT:
								beta += 0.05;
								update_cam();
								break;
							case SDLK_RIGHT:
								beta -= 0.05;
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
						alpha = (HAUTEUR_FENETRE-my)/500.0;
						beta = (LARGEUR_FENETRE-mx)/500.0;
						update_cam();
						break;
				}
			}

			SDL_RenderClear(renderer);



			//afficher_points(&terrain,  4, 128, 255, 0);
			//afficher_points_gstat(terrain_stat, 4, 128,255,0);
			afficher_aretes_gstat(terrain_stat, 1, 128,255,0);

			//afficher_aretes(&pyramide,  4, 255,255,0);
			//afficher_aretes(&prisme,  4, 0,128,255);

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
