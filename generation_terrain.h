#ifndef GENERATION_TERRAIN_H
#define GENERATION_TERRAIN_H



GrapheStatique3D* quadrillage_terrain(GrapheStatique3D* g, size_t nbp_x, size_t nbp_y);

GrapheStatique3D* generation_random(GrapheStatique3D* g,
    size_t nbp_x, size_t nbp_y,
    double niveau, double echelle, int relief);

GrapheStatique3D* generation_double_boucle(GrapheStatique3D* g,
    size_t nbp_x, size_t nbp_y,
    double niveau, double echelle, int relief);

GrapheStatique3D* generation_spirale(GrapheStatique3D* g,
    size_t nbp_x, size_t nbp_y,
    double niveau, double echelle, int relief);

GrapheStatique3D* generation_bruit_perlin(GrapheStatique3D* g,
    size_t nbp_x, size_t nbp_y,
    double niveau, double echelle, int relief,
    double frequence);

#endif
