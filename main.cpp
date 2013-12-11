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

	std::cout << "v0 = " << *v0 << " v1 = " << *v1 << "\n";

	xfig_trajets_sommets("test.fig",*dimension,tab,*v0,*v1,T);

	int *r = new int;
	double *t = new double;

	calculer_param(aretes,T,r,t,*dimension);

	std::cout << "r = " << *r << " t = " << *t << "\n";

	double obj = calculer_objectif(alpha,*r,gamma,*L,(int)T.size(),*t);

	std::cout << "obj = " << obj << "\n";

	std::cout << "trajet = " << calculer_trajet(aretes,T,1,*dimension) << "\n";

	
	delete[](tab);
	delete[](aretes);
	delete[](dimension);
	delete[](v0);
	delete[](v1);
	delete[](L);
	delete[](r);
	delete[](t);

	return 0;
}
