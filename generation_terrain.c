#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#include "struct.h"
#include "graphe.h"



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










//a
