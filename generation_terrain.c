#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "struct.h"
#include "graphe.h"


GrapheStatique3D* generation_double_boucle(GrapheStatique3D* g,
    size_t nbp_x, size_t nbp_y,
    double niveau, double echelle, int relief){

    size_t indice = 0;  //i*nbp_y + j

    for (int i = 0; i < nbp_x; i++) {
        for (int j = 0; j < nbp_y; j++) {
            niveau += (rand()%relief*2) - relief + 1;
            g->tab_points[indice].x = i*echelle - (nbp_x*echelle/2);
            g->tab_points[indice].y = j*echelle - (nbp_y*echelle/2);
            g->tab_points[indice].z = niveau;
            indice++;
        }
    }

    indice = 0;
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
}


/*for (size_t i = 0; i < nbp_x-1; i++) {
    terrain_stat->tab_aretes[i].p1 = terrain_stat->tab_points + i;
    terrain_stat->tab_aretes[i].p2 = terrain_stat->tab_points + i+1;
}*/

/*for (double i=1; i<1000; i+=50) {
    for (double j=0; j<i; j+=50) {
        niveau += (rand()%20) - 9.5;
        ajout_point_xyz(&terrain, (i-j)-500, j-500, niveau);
    }
}*/
