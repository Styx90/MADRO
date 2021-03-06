#include<stdio.h>
#include <stdlib.h>
#include <string>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>
#include <time.h> 
#include <algorithm>  
#ifndef MODEL_TAB
#define MODEL_TAB

typedef struct list_sommet {
     int sommet;
     struct list_sommet * suiv;
} L_som ;

double *lire_fichier(const char *nom_fichier, int *dimension);
int indice(int i, int j, int n);
double *calcul_aretes(double *tab, int dimension);
void calcul_sommet_L(double *aretes, int *v0, int *v1, double *L,int dimension);
void xfig_sommets (const char* nom_fichier, int dimension, double *tab, int v0, int v1);
void glouton(int dimension, double *aretes, int v0, int v1, std::vector< std::vector<int> >& T);
void xfig_trajets_sommets (const char* nom_fichier, int dimension, double *tab, int v0, int v1, std::vector< std::vector<int> >& T);
void calculer_param(double *aretes, std::vector< std::vector<int> >& T, int *r, double *t, int dimension);
double calculer_objectif(double alpha, int r, double gamma, double L, int k, double t);
double calculer_trajet(double *aretes, std::vector< std::vector<int> >& T, int num_trajet, int dimension);
void voisinage(int dimension, double *aretes, int v0, int v1, std::vector< std::vector<int> >& T, int nb_iter, double L);

#endif
