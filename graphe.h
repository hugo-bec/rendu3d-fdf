#ifndef GRAPHE_H
#define GRAPHE_H


void print_point(Point3D* p);
void print_graphe(Graphe3D* g);

void* get_noeud_element(Noeud* n, size_t i);
void* ajout_element(Noeud* n, void* vp_elem);

void* ajout_arete_i(Graphe3D* g, int a, int b);
void ajout_aretes_i(Graphe3D* g, int tab[][2], int n);

void* ajout_point_xyz(Graphe3D* g, double x, double y, double z);
void ajout_points_xyz(Graphe3D* g, double tab[][3], int n);





#endif
