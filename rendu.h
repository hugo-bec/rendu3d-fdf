#ifndef RENDU_H
#define RENDU_H

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "struct.h"


SDL_Window* init_sdl();
void init_renderer();

void afficherPoint(Point2D p, int epaisseur, int r, int g, int b);

void bresenham(Point2D p1, Point2D p2);



#endif
