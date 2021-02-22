#include <stdlib.h>
#include <math.h>
#include "struct.h"



double longueur2Dv(Vecteur2D* v){
	return sqrt(v->x*v->x + v->y*v->y);
}
double longueur2Dp(Point2D* pa, Point2D* pb){
	return sqrt((pb->x-pa->x)*(pb->x-pa->x) + (pb->y-pa->y)*(pb->y-pa->y));
}
double scalaire2D(Vecteur2D* va, Vecteur2D* vb){
	return (va->x*vb->x)+(va->y*vb->y);
}

//retourne l'angle de ACB
double getAngle2Dp(Point2D* pa, Point2D* pb, Point2D* pc){
    double a = longueur2Dp(pb, pc);
    double b = longueur2Dp(pa, pc);
    double c = longueur2Dp(pa, pb);
    return acos((a*a+b*b-c*c)/(2*a*b));
}

double getAngle2Dv(Vecteur2D* va, Vecteur2D* vb){
    return acos( scalaire2D(va, vb) / (longueur2Dv(va)*longueur2Dv(vb)) );
}

void getVecteur2D(Point2D* pa, Point2D* pb, Vecteur2D* vbuf){
	vbuf->x = pb->x - pa->x;
	vbuf->y = pb->y - pa->y;
}
