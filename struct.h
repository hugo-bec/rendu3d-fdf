#ifndef STRUCT_H
#define STRUCT_H


typedef struct Point3D Point3D;
struct Point3D{
	double x;
	double y;
	double z;
};

typedef struct Point2D Point2D;
struct Point2D{
	int x;
	int y;
};

typedef struct Vecteur3D Vecteur3D;
struct Vecteur3D{
	double x;
	double y;
	double z;
};



typedef struct Arete3D Arete3D;
struct Arete3D{
	Point3D* p1;
	Point3D* p2;
};

typedef struct NoeudArete3D NoeudArete3D;
struct NoeudArete3D{
	Arete3D* arete;
	NoeudArete3D* suiv;
};












#endif