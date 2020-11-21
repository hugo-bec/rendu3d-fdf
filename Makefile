CFLAGS=-lm
SDLFLAGS=`sdl2-config --cflags --libs`

3dpoints: vecteur3d.o rendu.o main.o
	gcc -o $@ $^ $(SDLFLAGS) $(CFLAGS)
	rm -r *.o

vecteur3d.o: vecteur3d.c
	gcc -c $^ $(CFLAGS)

rendu.o: rendu.c
	gcc -c $^ $(CFLAGS)

main.o: main.c
	gcc -c $^ $(CFLAGS)



##
## gcc -o 3dpoints.out 3dpoints.c `sdl2-config --cflags --libs` -lm
##
## $@ 	Le nom de la cible
## $< 	Le nom de la première dépendance
## $^ 	La liste des dépendances
## $? 	La liste des dépendances plus récentes que la cible
## $* 	Le nom du fichier sans suffixe
##	https://gl.developpez.com/tutoriel/outil/makefile/
