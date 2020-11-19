#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "struct.h"



void printCoordPoint3D(Point3D p){
	printf("x:%lf y:%lf z:%lf\n", p.x, p.y, p.z);
}
void printCoordVecteur3D(Vecteur3D v){
	printf("x:%lf y:%lf z:%lf\n", v.x, v.y, v.z);
}


double scalaire3D(Vecteur3D va, Vecteur3D vb){
	return (va.x*vb.x)+(va.y*vb.y)+(va.z*vb.z);
}
double longueur(Vecteur3D v){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
void addition(Vecteur3D vt[], int nbv, Vecteur3D* vbuf){
	vbuf->x=0; vbuf->y=0; vbuf->z=0;
	for (size_t i=0; i<nbv; i++) {
		vbuf->x += vt[i].x;
		vbuf->y += vt[i].y;
		vbuf->z += vt[i].z;
	}
}
void produitk(Vecteur3D v, double k, Vecteur3D* vbuf){
	vbuf->x = v.x*k;
	vbuf->y = v.y*k;
	vbuf->z = v.z*k;
}

Vecteur3D getVecteur(Point3D pa, Point3D pb){
	Vecteur3D vecteur;
	vecteur.x = pb.x - pa.x;
	vecteur.y = pb.y - pa.y;
	vecteur.z = pb.z - pa.z;
	return vecteur;
}

double getAngle(Vecteur3D va, Vecteur3D vb){
	return acos( scalaire3D(va, vb) / (longueur(va)*longueur(vb)) );
}
