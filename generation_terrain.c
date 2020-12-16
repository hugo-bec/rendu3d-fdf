#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#include "struct.h"
#include "graphe.h"
#include "vecteur3d.h"



Point3D* affecter_point3d(Point3D* p, double x, double y, double z){
    p->x = x;
    p->y = y;
    p->z = z;
    return p;
}


GrapheStatique3D* quadrillage_terrain(GrapheStatique3D* g, size_t nbp_x, size_t nbp_y){
    size_t indice = 0;
    for (size_t i = 0; i < nbp_x; i++) {
        for (size_t j = 0; j < nbp_y-1; j++) {
            g->tab_aretes[indice].p1 = g->tab_points + (i*nbp_y+j);
            g->tab_aretes[indice].p2 = g->tab_points + (i*nbp_y+j+1);
            indice++;
        }
    }
    for (size_t i = 0; i < nbp_x-1; i++) {
        for (size_t j = 0; j < nbp_y; j++) {
            g->tab_aretes[indice].p1 = g->tab_points + i*nbp_y + j;
            g->tab_aretes[indice].p2 = g->tab_points + i*nbp_y + nbp_y + j;
            indice++;
        }
    }
    return g;
}


GrapheStatique3D* generation_random(GrapheStatique3D* g,
    size_t nbp_x, size_t nbp_y,
    double niveau, double echelle, int relief){

    size_t indice = 0;  //i*nbp_y + j

    for (int i = 0; i < nbp_x; i++) {
        for (int j = 0; j < nbp_y; j++) {
            affecter_point3d(g->tab_points + indice,
                i*echelle - (nbp_x*echelle/2),
                j*echelle - (nbp_y*echelle/2),
                (rand()%relief*2) - relief + 1 + niveau);
            indice++;
        }
    }
    return quadrillage_terrain(g, nbp_x, nbp_y);
}


GrapheStatique3D* generation_double_boucle(GrapheStatique3D* g,
    size_t nbp_x, size_t nbp_y,
    double niveau, double echelle, int relief){

    size_t indice = 0;  //i*nbp_y + j

    for (int i = 0; i < nbp_x; i++) {
        for (int j = 0; j < nbp_y; j++) {
            niveau += (rand()%relief*2) - relief + 1;
            //g->tab_points[indice].x = i*echelle - (nbp_x*echelle/2);
            //g->tab_points[indice].y = j*echelle - (nbp_y*echelle/2);
            //g->tab_points[indice].z = niveau;
            affecter_point3d(g->tab_points + indice,
                i*echelle - (nbp_x*echelle/2),
                j*echelle - (nbp_y*echelle/2),
                niveau);
            indice++;
        }
    }
    return quadrillage_terrain(g, nbp_x, nbp_y);
}


GrapheStatique3D* generation_spirale(GrapheStatique3D* g,
    size_t nbp_x, size_t nbp_y,
    double niveau, double echelle, int relief){

    size_t i=0, j=0;
    for (size_t k=0; k<(nbp_x/2)+1; k++) {
        i=k;
        j=k;
        for (; j<nbp_x-k-1; j++) {
            //printf("indice : %ld (i=%ld, j=%ld)\n", i*nbp_y + j, i,j);
            niveau += (rand()%relief*2) - relief + 1;
            affecter_point3d(g->tab_points + (i*nbp_y + j),
                i*echelle - (nbp_x*echelle/2),
                j*echelle - (nbp_y*echelle/2),
                niveau);
        }
        for (; i<nbp_y-k-1; i++) {
            //printf("indice : %ld (i=%ld, j=%ld)\n", i*nbp_y + j, i,j);
            niveau += (rand()%relief*2) - relief + 1;
            affecter_point3d(g->tab_points + (i*nbp_y + j),
                i*echelle - (nbp_x*echelle/2),
                j*echelle - (nbp_y*echelle/2),
                niveau);
        }
        for (; j>k; j--) {
            //printf("indice : %ld (i=%ld, j=%ld)\n", i*nbp_y + j, i,j);
            niveau += (rand()%relief*2) - relief + 1;
            affecter_point3d(g->tab_points + (i*nbp_y + j),
                i*echelle - (nbp_x*echelle/2),
                j*echelle - (nbp_y*echelle/2),
                niveau);
        }
        for (; i>k; i--) {
            //printf("indice : %ld (i=%ld, j=%ld)\n", i*nbp_y + j, i,j);
            niveau += (rand()%relief*2) - relief + 1;
            affecter_point3d(g->tab_points + (i*nbp_y + j),
                i*echelle - (nbp_x*echelle/2),
                j*echelle - (nbp_y*echelle/2),
                niveau);
        }
        //printf("------------------------------------------fin loop k=%ld\n", k);
    }

    if (nbp_x%2 != 0) {
        niveau += (rand()%relief*2) - relief + 1;
        i = nbp_x/2;
        j = nbp_y%2==0 ? nbp_y/2-1 : nbp_y/2;
        printf("MILLIEU indexe : %ld\n", i*nbp_y + j);
        affecter_point3d(g->tab_points + (i*nbp_y + j),
            i*echelle - (nbp_x*echelle/2),
            j*echelle - (nbp_y*echelle/2),
            niveau);
    }

    return quadrillage_terrain(g, nbp_x, nbp_y);
}



/*
 *  PERLIN NOISE
 *
 *  Merci à Raouf Touti pour ses explications claire sur son blog :
 *  https://rtouti.github.io/graphics/perlin-noise-algorithm
 */

// Fonction de fondu
double fondu(double t){
	return ((6*t - 15)*t + 10)*t*t*t;
}
// Interpolation Linéaire
double interpolation_lineaire(double t, double a, double b){
	return a+t*(b-a);
}

void creer_table_permutation(int* PT, int n){
    int temp, a, b;
    for (size_t i=0; i<n; i++) {
        PT[i] = i;
    }
    for (size_t i=0; i<n*4; i++) {
        a = rand()%n;
        b = rand()%n;
        temp = PT[a];
        PT[a] = PT[b];
        PT[b] = temp;
    }
    for (size_t i=0; i<n; i++) {
        PT[n+i] = PT[i];
    }
}

double get_scalaire_perlin_points(Vecteur2D v, int valeur){
    Vecteur2D vect_const;
    switch (valeur%4) {
        case 0:
            vect_const.x = 1;
            vect_const.y = 1;
            return scalaire2D(v, vect_const);
        case 1:
            vect_const.x = -1;
            vect_const.y = 1;
            return scalaire2D(v, vect_const);
        case 2:
            vect_const.x = -1;
            vect_const.y = -1;
            return scalaire2D(v, vect_const);
        default:
            vect_const.x = 1;
            vect_const.y = -1;
            return scalaire2D(v, vect_const);
    }
}

double bruit_perlin(double x, double y, int taillept, int* PT){

    int X = (int)x & taillept;
    int Y = (int)y & taillept;
    double xf = x-X;
    double yf = y-Y;

    Vecteur2D vectHD = {xf-1.0, yf-1.0},        //Haut Droite
            vectHG = {xf, yf-1.0},              //Haut Gauche
            vectBD = {xf-1.0, yf},              //Bas Droite
            vectBG = {xf, yf};                  //Bas Gauche

    double valHD = PT[PT[X+1]+Y+1];
	double valHG = PT[PT[X]+Y+1];
	double valBD = PT[PT[X+1]+Y];
	double valBG = PT[PT[X]+Y];

    double scalaireHD = get_scalaire_perlin_points(vectHD, valHD);
	double scalaireHG = get_scalaire_perlin_points(vectHG, valHG);
	double scalaireBD = get_scalaire_perlin_points(vectBD, valBD);
	double scalaireBG = get_scalaire_perlin_points(vectBG, valBG);

    double u = fondu(xf);
	double v = fondu(yf);

    double result =  interpolation_lineaire(u,
        interpolation_lineaire(v, scalaireBG, scalaireHG),
        interpolation_lineaire(v, scalaireBD, scalaireHD)
	);
    return result;
}


GrapheStatique3D* generation_bruit_perlin(GrapheStatique3D* g,
    size_t nbp_x, size_t nbp_y,
    double niveau, double echelle, int relief,
    double frequence)
{
    size_t indice = 0;  //i*nbp_y + j

    size_t taillept = 255;
    int PT[taillept*2];
    creer_table_permutation(PT, taillept);

    for (int i = 0; i < nbp_x; i++) {
        for (int j = 0; j < nbp_y; j++) {
            affecter_point3d(g->tab_points + indice,
                i*echelle - (nbp_x*echelle/2),
                j*echelle - (nbp_y*echelle/2),

                bruit_perlin(i*frequence, j*frequence, taillept, PT)  *relief+niveau);

            indice++;
        }
    }
    return quadrillage_terrain(g, nbp_x, nbp_y);
}








//a
