#ifndef RENDU_H
#define RENDU_H

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "struct.h"

//SDL
SDL_Window* init_sdl();
void init_renderer();
void init_texture();

//RENDU ECRAN
void calculer_vecteurs_plancam();
Point2D* proj_point(Point3D* p, Point2D* proj);
void update_cam();

void afficher_point2D(Point2D* p, int epaisseur, int r, int g, int b);


//GRAPHE
void afficher_point(Point3D* p, int epaisseur, int r, int v, int b);
/*inline void afficher_point(Point3D* p, int epaisseur, int r, int v, int b){
	Point2D proj;
	proj_point( p, &proj);
	afficher_point2D(&proj, epaisseur, r,v,b);
}*/

void afficher_points(Graphe3D* g, int epaisseur, int r, int v, int b);

void afficher_arete(Arete3D* a, int epaisseur, int r, int v, int b);
void afficher_aretes(Graphe3D* g, int epaisseur, int r, int v, int b);

void afficher_points_gstat(GrapheStatique3D* g, int epaisseur, int r, int v, int b);
void afficher_aretes_gstat(GrapheStatique3D* g, int epaisseur, int r, int v, int b);

void afficher_couleur_relief_points(GrapheStatique3D* g, int epaisseur, int zmin, int zmax);

//Tracé de segments
void bresenham(Point2D* p1, Point2D* p2, int epaisseur, int r, int g, int b);



#endif
