#include <stdlib.h>
#include <stdio.h>

#include "struct.h"





void print_point(Point3D* p){
    printf("(%lf, %lf, %lf)", p->x, p->y, p->z);
}

void print_graphe(Graphe3D* g){
    printf("p graphe : %p\n", g);
    printf("Point: \n");
    Noeud* p_courant = g->point_tete;
    for (int i=0; p_courant != NULL; i++) {
        printf("%d : %p : ", i, (Point3D*)p_courant->elem);
        print_point((Point3D*)p_courant->elem);
        printf("\n");
        p_courant = p_courant->suiv;
    }
    printf("Aretes: \n");
    Noeud* a_courant = g->arete_tete;
    Arete3D* ai;
    for (int i=0; a_courant != NULL; i++) {
        ai = (Arete3D*)a_courant->elem;
        printf("(%p , %p)\n", ai->p1, ai->p2);
        a_courant = a_courant->suiv;
    }
}


void* get_noeud_element(Noeud* n, size_t i0){
    int i = i0;
    for (; n!=NULL && i!=0; i--) {
        n = n->suiv;
    } if (i < 0) {
        printf("ERREUR: get_noeud_element: Index incorrect: %ld\n", i0);
        exit(1);
    }
    return n->elem;
}


void* ajout_element(Noeud* n, void* vp_elem){
    if (n == NULL) {
        //n = vp_elem;
		return n;
	} else {
		while (n->suiv != NULL) {
			n = n->suiv;
		}
		n->suiv = vp_elem;
		return n->suiv;
	}
}


void* ajout_arete_i(Graphe3D* g, int a, int b){
    Point3D* p1 = get_noeud_element(g->point_tete, a);
    Point3D* p2 = get_noeud_element(g->point_tete, b);
    if (p1!=NULL && p2!=NULL) {
        Arete3D* a = malloc(sizeof(Arete3D));
    	a->p1 = p1;
        a->p2 = p2;
    	Noeud* new_n = malloc(sizeof(Noeud));
    	new_n->elem = a;
        new_n->suiv = NULL;

        if (g->arete_tete == NULL) {
            g->arete_tete = new_n;
            return new_n;
        } else {
            return ajout_element(g->arete_tete, new_n);
        }
    }
    // exit(1);
    return NULL;
}

//ajoute toutes les aretes à partir des indexes du tableau tab
void ajout_aretes_i(Graphe3D* g, int tab[][2], int n){
    for (size_t i=0; i<n; i++) {
        ajout_arete_i(g, tab[i][0], tab[i][1]);
    }
}


void* ajout_point_xyz(Graphe3D* g, double x, double y, double z){
    Point3D* p = malloc(sizeof(Point3D));
	p->x = x;
    p->y = y;
    p->z = z;
	Noeud* new_n = malloc(sizeof(Noeud));
	new_n->elem = p;
    new_n->suiv = NULL;

    if (g->point_tete == NULL) {
        g->point_tete = new_n;
        return new_n;
    } else {
        return ajout_element(g->point_tete, new_n);
    }
}

//ajoute tous les points à partir du tableau tab
void ajout_points_xyz(Graphe3D* g, double tab[][3], size_t n){
    for (size_t i=0; i<n; i++) {
        ajout_point_xyz(g, tab[i][0], tab[i][1], tab[i][2]);
    }
}




/*
 * GRAPHE STATIQUE
 */

GrapheStatique3D* creer_graphe_statique(size_t nbp, size_t nba){
    GrapheStatique3D* g = malloc(sizeof(GrapheStatique3D));
    g->nbPoints = nbp;
    g->nbAretes = nba;
    g->tab_points = malloc(sizeof(Point3D)*nbp);
    g->tab_aretes = malloc(sizeof(Arete3D)*nba);
    return g;
}

GrapheStatique3D* init0_graphe_statique(GrapheStatique3D* g){
    for (size_t i=0; i<g->nbPoints; i++) {
        g->tab_points[i].x = 0;
        g->tab_points[i].y = 0;
        g->tab_points[i].z = 0;
    }
    for (size_t i=0; i<g->nbAretes; i++) {
        g->tab_aretes[i].p1 = g->tab_points+0;
        g->tab_aretes[i].p2 = g->tab_points+0;
    }
    return g;
}













//a
