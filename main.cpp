#include <stdio.h>
#include <stdlib.h>
#include "model_tableau.h"

int main(){
	const char* fic = "att48.tsp";
	int *dimension = new int;
	double *tab = lire_fichier(fic,dimension);
	double *aretes = calcul_aretes(tab,*dimension);
	int *v0 = new int;
	int *v1 = new int;
	double *L = new double;
	double alpha = 0.5;
	double gamma = 2.5;

	calcul_sommet_L(aretes,v0,v1,L,*dimension);

	xfig_sommets("test.fig",*dimension,tab,*v0,*v1);

	std::vector< std::vector<int> > T;
	glouton(*dimension,aretes,*v0,*v1,T);
	xfig_trajets_sommets("test.fig",*dimension,tab,*v0,*v1,T);

	
	delete[](tab);
	delete[](aretes);
	delete[](dimension);
	delete[](v0);
	delete[](v1);
	delete[](L);

	return 0;
}
