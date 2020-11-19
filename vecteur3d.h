#ifndef VECTEUR3D_H
#define VECTEUR3D_H

#include "struct.h"


void printCoordPoint3D(Point3D p);
void printCoordVecteur3D(Vecteur3D v);

double scalaire3D(Vecteur3D va, Vecteur3D vb);
double longueur(Vecteur3D v);

void addition(Vecteur3D vt[], int nbv, Vecteur3D* vbuf);
void produitk(Vecteur3D v, double k, Vecteur3D* vbuf);

Vecteur3D getVecteur(Point3D pa, Point3D pb);
double getAngle(Vecteur3D va, Vecteur3D vb);


#endif
