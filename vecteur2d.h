#ifndef VECTEUR2D_H
#define VECTEUR2D_H

double longueur2Dv(Vecteur2D* v);
double longueur2Dp(Point2D* pa, Point2D* pb);

double scalaire2D(Vecteur2D* va, Vecteur2D* vb);

double getAngle2Dp(Point2D* pa, Point2D* pb, Point2D* pc);
double getAngle2Dv(Vecteur2D* pa, Vecteur2D* pb);

void getVecteur2D(Point2D* pa, Point2D* pb, Vecteur2D* vbuf);

#endif
