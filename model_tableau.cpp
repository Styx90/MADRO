#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "model_tableau.h"

int indice(int i, int j, int n){
	return i*n+j;
}

//Lecture sur un fichier standard
double *lire_fichier(const char *nom_fichier, int *dimension){
	std::ifstream fichier;

	fichier.open(nom_fichier, std::ifstream::in);

	if (!fichier){
		std::cerr << "Erreur à l'ouverture du fichier\n";
		exit(1);
	}

	int n,i;
	double x,y,min_x,min_y,max_x,max_y;
	std::string ligne;
//
	*dimension = 0;
	while (std::getline(fichier, ligne)){
		(*dimension)++;
	}
	*dimension = *dimension - 7;
	fichier.close();
//
	fichier.open(nom_fichier, std::ifstream::in);
	while (ligne.compare("NODE_COORD_SECTION") != 0){
		std::getline(fichier, ligne);
	}

	double *tab = new double[(*dimension)*2];
	n = 0; x = 0; y = 0; min_x = 10000 ; min_y = 10000; max_x = 0 ; max_y = 0;
	for(i= 1; i <= *dimension ; i++){
		fichier >> n >> x >> y;
		tab[indice(n-1,0,2)] = x;
		tab[indice(n-1,1,2)] = y;
		if (x < min_x){min_x = x;}else{if (x > max_x){max_x = x;}}
		if (y < min_y){min_y = y;}else{if (x > max_y){max_y = y;}}

	}

	fichier.close();

	return tab;
}


int verif_derniers(	std::vector<int>& derniers_arrets,int v1){
	int i;
	i=0;
	while(i < derniers_arrets.size() && derniers_arrets[i] == v1){i++;}
	return (i==(derniers_arrets.size())?1:0);
}

void calculer_param(double *aretes, std::vector< std::vector<int> >& T, int *r, double *t, int dimension){
	//calcul de r
	int i,j;
	*r = 0;
	for(i=0 ; i<T.size() ; i++){
		*r = std::max(*r,(int)T[i].size());
	}

	//calcul de t
	*t = 0;
	for(i=0 ; i<T.size() ; i++){
		for(j=1 ; j<T[i].size() ; j++){
			*t = *t + aretes[indice(T[i][j-1],T[i][j],dimension)];
		}
	}
		
	return;
}

double calculer_objectif(double alpha, int r, double gamma, double L, int k, double t){
	return alpha*r+(1-alpha)*(gamma*L*k+t);
}

double calculer_trajet(double *aretes, std::vector< std::vector<int> >& T, int num_trajet, int dimension){
	int j;double trajet;
	trajet = 0;
	for(j=1 ; j<T[num_trajet].size() ; j++){
		trajet = trajet + aretes[indice(T[num_trajet][j-1],T[num_trajet][j],dimension)];
	}
	return trajet;	
}

void voisinage(int dimension, double *aretes, int v0, int v1, std::vector< std::vector<int> >& T, int nb_iter, double L){
	int i,j,k,l,cpt,min_i,som_cour,*r;
	double min,obj_cour,*t;

	//
	cpt=0;r = new int;t = new double;
	while (cpt < nb_iter){
		calculer_param(aretes,T,r,t,dimension);
		for(i=0 ; i<T.size() ; i++){
			obj_cour = calculer_objectif(1,*r,1,L,(int)T.size(),*t);
			for(j=0 ; j<T.size() ; j++){
				if(j!=i){//Si on est pas dans le trajet courant, on tente d'ajouter les sommets de ce trajet un par un au trajet courant
					for(k=1 ; k<T[j].size()-1 ; k++){
						som_cour = T[j][k];
						std::cout << "som_cour = " << som_cour << "\n";
						
					}
				}
			}
		}
		cpt++;
	}
				
	return;
}

void glouton(int dimension, double *aretes, int v0, int v1, std::vector< std::vector<int> >& T){
	std::vector<int> sommets_ouverts;
	std::vector<int> derniers_arrets;

	srand (time(NULL));

	int i,j;
	for(i=0 ; i<dimension ; i++){
		if(i != v0){sommets_ouverts.push_back(i);}
	}

	double min_v0, min_ouvert,tmp_min;
	int v0_min_i, ouvert_min_i, tmp_i, choix_min,prec_min,tmp_som_ouvert,init;
	init = 1;
	while(sommets_ouverts.size() > 1 || verif_derniers(derniers_arrets,v1) != 1){
		//Chercher l'arret le plus proche de v0 noté v0_min
		min_v0 = 10000;v0_min_i=0;
		for(i=0 ; i < sommets_ouverts.size() ; i++){
			tmp_som_ouvert = sommets_ouverts[i];
			if(tmp_som_ouvert != v0){
				tmp_min = aretes[indice(v0,tmp_som_ouvert,dimension)];
				if(tmp_min < min_v0){min_v0 = tmp_min; v0_min_i = tmp_som_ouvert;}
			}
		}
		//Chercher l'arret ouvert qui minimise la distance parmi tous les derniers arrets noté ouvert_min_i et son prédécesseur prec_min
		min_ouvert = 10000;ouvert_min_i=0;prec_min = 0;
		for(i=0 ; i<derniers_arrets.size() ; i++){
			tmp_i = derniers_arrets[i];
			if (tmp_i != v1){
				for(j=0 ; j<sommets_ouverts.size() ; j++){			
					tmp_som_ouvert = sommets_ouverts[j];
					if(tmp_som_ouvert != tmp_i ){
						tmp_min = aretes[indice(tmp_i,tmp_som_ouvert,dimension)];
						if(tmp_min < min_ouvert){min_ouvert = tmp_min; ouvert_min_i = tmp_som_ouvert;prec_min = tmp_i;}
					}
				}
			}
		}
		//Choisir entre les deux celui qu'on veut de manière aléatoire
		double r = rand()%10;
		choix_min = (r > 7?v0_min_i:ouvert_min_i);
		if(init == 1){choix_min = v0_min_i;init=0;}//On ne peut pas prendre un chemin ouvert en premier il n'existe pas
		//On met a jour sommets_ouverts et derniers_arrets
		if(choix_min != v1){
			i=0;
			while(sommets_ouverts[i] != choix_min){i++;}
			sommets_ouverts.erase(sommets_ouverts.begin()+i);
		}
		if(choix_min == ouvert_min_i){
			j=0;
				while(derniers_arrets[j] != prec_min){j++;}
			derniers_arrets[j] = choix_min;
		}else {
			derniers_arrets.push_back(choix_min);
		}
		//On ajoute le nouveau sommet dans T
		if(choix_min == ouvert_min_i){
			T[j].push_back(choix_min);
		}else {
			T.resize(T.size()+1);
			T[T.size()-1].push_back(v0);
			T[T.size()-1].push_back(choix_min);
		}
	}
	
	return;
}

//Afficher les trajets et les sommets sur xfig
void xfig_trajets_sommets (const char* nom_fichier, int dimension, double *tab, int v0, int v1, std::vector< std::vector<int> >& T){
	std::ofstream fichier;
	int i,j,couleur,epaisseur;
	
	fichier.open(nom_fichier,std::ios::out | std::ios::trunc);

	if (!fichier){
		std::cerr << "Erreur à l'ouverture du fichier\n";
		exit(1);
	}

	fichier << "#FIG 3.2"<<std::endl;
	fichier << "Landscape"<<std::endl;
	fichier << "Center"<<std::endl;
	fichier << "Metric"<<std::endl;
	fichier << "A4"<<std::endl;      
	fichier << "100.00"<<std::endl;
	fichier << "Single"<<std::endl;
	fichier << "-2"<<std::endl;
	fichier << "1200 2"<<std::endl;  
	
	//Sommets	
	for(i=0 ; i<dimension ; i++){
		if(i == v1 || i == v0 ){ couleur = 4;epaisseur = 6;}else{ couleur = 1;epaisseur = 4;}
		fichier << "1 4 0 "<< epaisseur<<" "<< couleur <<" 7 50 0 -1 0.000 1 0.0000 " << tab[indice(i,0,2)] << " " << tab[indice(i,1,2)] << " 10 10 0 0 0 0" << std::endl;
	}

//	fichier << "4 1 12 50 0 14 12 0.7584 4 90 90 "<< tab[indice(v0,0,2)] <<" " << tab[indice(v0,1,2)] << "v0\001\n";

	//Trajets
	for(i=0 ; i<T.size() ; i++){
		couleur = i%6;
		for(j=1 ; j<T[i].size() ; j++){
			fichier << "2 1 0 2 "<< couleur <<" 7 100 0 -1 0.000 0 0 -1 0 0 2" << std::endl;
			fichier << "	 "<< tab[indice(T[i][j-1],0,2)] << " "<< tab[indice(T[i][j-1],1,2)] << " " << tab[indice(T[i][j],0,2)] << " "<< tab[indice(T[i][j],1,2)] << std::endl;
		}
	}

	fichier.close();
	return;
}

//Afficher les sommets sur xfig
void xfig_sommets (const char* nom_fichier, int dimension, double *tab, int v0, int v1){
	std::ofstream fichier;
	int i,couleur;
	
	fichier.open(nom_fichier,std::ios::out | std::ios::trunc);

	if (!fichier){
		std::cerr << "Erreur à l'ouverture du fichier\n";
		exit(1);
	}

	fichier << "#FIG 3.2"<<std::endl;
	fichier << "Landscape"<<std::endl;
	fichier << "Center"<<std::endl;
	fichier << "Metric"<<std::endl;
	fichier << "A4"<<std::endl;      
	fichier << "100.00"<<std::endl;
	fichier << "Single"<<std::endl;
	fichier << "-2"<<std::endl;
	fichier << "1200 2"<<std::endl;  
	
	couleur = 1;
	for(i=0 ; i<dimension ; i++){
		if(i == v1 || i == v0 ){ couleur = 4;}else{ couleur = 1;}
		fichier << "1 4 0 2 "<< couleur <<" 7 50 0 -1 0.000 1 0.0000 " << tab[indice(i,0,2)] << " " << tab[indice(i,1,2)] << " 10 10 0 0 0 0" << std::endl;
	}

	fichier.close();
	return;
}

//Calcul de la matrice d'incidence
double *calcul_aretes(double *tab, int dimension){
	double *aretes = new double [dimension*dimension];
	int i,j;
	double tmp;

	for(i=0 ; i < dimension ; i++){
		for(j=0 ; j < i ; j++){
			tmp = sqrt((double)(tab[indice(j,0,2)]-tab[indice(i,0,2)])*(tab[indice(j,0,2)]-tab[indice(i,0,2)])+(double)(tab[indice(j,1,2)]-tab[indice(i,1,2)])*(tab[indice(j,1,2)]-tab[indice(i,1,2)]));
			aretes[indice(i,j,dimension)] = tmp;
			aretes[indice(j,i,dimension)] = tmp;
		}
	}

	return aretes;
}

//Calcul de v1 et v0 et de L
void calcul_sommet_L(double *aretes, int *v0, int *v1, double *L,int dimension){
	int i,j,vl;
	double max,tmp;

	//v0 et v1
	max = 0;*v0 = 0;*v1 = 0;
	for(i=1 ; i < dimension ; i++){
		for(j=1 ; j < i ; j++){	
			tmp = aretes[indice(i,j,dimension)];
			if ( tmp > max){
				max = tmp;
				*v0 = i;
				*v1 = j;
			}
		}
	}

	//L
	max = 0;vl = 1;
	for(i=2 ; i < dimension ; i++){
		if(i != *v1){
			tmp = aretes[indice(*v0,i,dimension)];
			if ( tmp > max){
				max = tmp;
				vl = i;
			}
		}
	}
	*L = aretes[indice(*v0,vl,dimension)] + aretes[indice(vl,*v1,dimension)];
	return;
}
