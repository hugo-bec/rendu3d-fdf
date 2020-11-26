#ifndef RENDU_H
#define RENDU_H

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "struct.h"


SDL_Window* init_sdl();
void init_renderer();

void calculer_vecteurs_plancam();
Point2D* proj_point(Point3D* p, Point2D* proj);
void update_cam();

void afficher_point(Point2D* p, int epaisseur, int r, int g, int b);
void afficher_aretes(Graphe3D* g, int epaisseur, int r, int v, int b);

void bresenham(Point2D* p1, Point2D* p2, int epaisseur, int r, int g, int b);



#endif
