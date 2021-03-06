CFLAGS=-lm
SDLFLAGS=`sdl2-config --cflags --libs`


3dpoints: graphe.o vecteur3d.o vecteur2d.o rendu.o generation_terrain.o main.o
	#rm -r *.o #*.gch
	gcc -std=c99 -o $@ $^ $(SDLFLAGS) $(CFLAGS)
	rm -r *.o #*.gch
	find /home/alstm/Documents/projets_c/3dpoints/ -type f -exec touch {} +

vecteur3d.o: vecteur3d.c
	gcc -c $^ $(CFLAGS)

vecteur2d.o: vecteur2d.c
	gcc -c $^ $(CFLAGS)

rendu.o: rendu.c
	gcc -c $^ $(CFLAGS)

graphe.o: graphe.c
	gcc -c $^ $(CFLAGS)

generation_terrain.o: generation_terrain.c
	gcc -c $^ $(CFLAGS)

main.o: main.c
	gcc -c $^ $(CFLAGS)

##*.o: *.c
##	gcc -c $^ $(CFLAGS)




##
## gcc -o 3dpoints.out 3dpoints.c `sdl2-config --cflags --libs` -lm
##
## $@ 	Le nom de la cible
## $< 	Le nom de la première dépendance
## $^ 	La liste des dépendances
## $? 	La liste des dépendances plus récentes que la cible
## $* 	Le nom du fichier sans suffixe
##	https://gl.developpez.com/tutoriel/outil/makefile/
