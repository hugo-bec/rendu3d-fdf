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

typedef struct Vecteur2D Vecteur2D;
struct Vecteur2D{
	double x;
	double y;
};

/*struct Camera{
	Vecteur3D ;
}*/





/**************************************
 *	GRAPHE
 **************************************/

typedef struct Arete3D Arete3D;
struct Arete3D{
	Point3D* p1;
	Point3D* p2;
};

typedef struct Noeud Noeud;
struct Noeud{
	void* elem;
	Noeud* suiv;
};

typedef struct Graphe3D Graphe3D;
struct Graphe3D{
	Noeud* point_tete;
	Noeud* arete_tete;
};

typedef struct GrapheStatique3D GrapheStatique3D;
struct GrapheStatique3D{
	size_t nbPoints;
	size_t nbAretes;
	Point3D* tab_points;
	Arete3D* tab_aretes;
};









#endif
