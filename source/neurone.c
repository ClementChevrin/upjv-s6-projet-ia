#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "../header/data.h"

// struct neurone
typedef struct neurone
{
	double *poids;
	double in,out,err;
}*Neurone;

// Fonction sigmoide
double neurone_Sigmoide(double x,double lambda)
{
	return 1/(1+exp(-lambda*x));
}

// Init neurone struct
Neurone** neurone_Init(int nb_neurone, int couche)
{
	time_t t;
	srand((unsigned) time(&t));
	Neurone** n=(Neurone**)malloc(sizeof(Neurone*)*nb_neurone);
	for (int current_neurone = 0 ; current_neurone < nb_neurone ; ++current_neurone)
	{
		n[current_neurone] = (Neurone*)malloc(sizeof(Neurone)*couche);
		for (int current_couche = 0 ; current_couche < couche ; ++current_couche) 
		{	
			n[current_neurone][current_couche]=(Neurone)malloc(sizeof(struct neurone));
			n[current_neurone][current_couche]->poids = malloc(sizeof(double)*nb_neurone);

			// Initialisation des variables
			for(int current_poids = 0 ; current_poids < nb_neurone ; current_poids++) n[current_neurone][current_couche]->poids[current_poids ]=(rand()/RAND_MAX)/4;
			n[current_neurone][current_couche]->in = 0;
			n[current_neurone][current_couche]->err = 0;
		}
	}
	return n;
}

// Free neurone struct
void neurone_Free(Neurone** n,int nb_neurone,int couche)
{
	for(int current_neurone = 0; current_neurone < nb_neurone-1 ; current_neurone++)
	{
		for (int current_couche = 0; current_couche < couche; ++current_couche) 
		{
			free(n[current_neurone][current_couche]->poids);
			free(n[current_neurone][current_couche]);
		}
		free(n[current_neurone]);
	}
	free(n);
}

// Fonction d'apprentisage
double neurone_Apprentisage(Data donnee,int ligne,Neurone** n,double lambda)
{
	// Calcul de sortie de la premiere couche du reseau de neurones
	for (int current_neurone = 0; current_neurone < donnee->colonne-1; ++current_neurone)
	{
		for (int current_critere = 0; current_critere < donnee->colonne-1; ++current_critere) n[current_neurone][0]->in += donnee->critere[current_critere][ligne]*n[current_neurone][0]->poids[current_critere];
		n[current_neurone][0]->out = neurone_Sigmoide(n[current_neurone][0]->in,lambda);
	}


	// Calcul de sortie des couches intermediaires du reseau de neurones
	int nb_neurone = donnee->colonne-1;
	int k = 1;
	while(nb_neurone/2 > 1)
	{
		for (int current_neurone = 0; current_neurone < nb_neurone/2; ++current_neurone)
		{
			for (int current_critere = 0; current_critere < nb_neurone; ++current_critere) n[current_neurone][k]->in += n[current_critere][k-1]->out*n[current_neurone][k]->poids[current_critere];
			n[current_neurone][k]->out = neurone_Sigmoide(n[current_neurone][k]->in,lambda);
		}
		k++;
		nb_neurone = nb_neurone/2;
	}

	// Calcul de sortie de la dernier couche du reseau de neurones
	for (int current_neurone = 0; current_neurone < donnee->max[donnee->colonne-1]-donnee->min[donnee->colonne-1]; ++current_neurone)
	{
		for (int current_critere = 0; current_critere < nb_neurone; ++current_critere) n[current_neurone][k]->in += n[current_critere][k-1]->out*n[current_neurone][k]->poids[current_critere];
		n[current_neurone][k]->out = neurone_Sigmoide(n[current_neurone][k]->in,lambda);
	}

	// Recherche de la sortie la plus grande 
	int max = 0;
	for (int current_neurone = 0; current_neurone < (int)(donnee->max[donnee->colonne-1]-donnee->min[donnee->colonne-1]); ++current_neurone) if(n[max][k]->out < n[current_neurone][k]->out) max = current_neurone;


	// Ajout de l'erreur initiale
	for (int current_neurone = 0; current_neurone < donnee->colonne-1; ++current_neurone) for (int current_critere = 0; current_critere < k; ++current_critere) n[current_neurone][current_critere]->err = 0;

	// Sortie desire denormalisee
	int outDesired=(int)(((donnee->critere[donnee->colonne-1][ligne])*(donnee->max[donnee->colonne-1]-donnee->min[donnee->colonne-1])));
	//printf("outDesired=%d\n",outDesired);
	

	// Calcul des erreurs de la derniere couche
	double errtot=0;
	for (int current_neurone = 0; current_neurone < donnee->max[donnee->colonne-1]-donnee->min[donnee->colonne-1]; ++current_neurone) 
	{
		if(current_neurone==outDesired) n[current_neurone][k]->err = 1-n[current_neurone][k]->out;
		else n[current_neurone][k]->err=0-n[current_neurone][k]->out;
		errtot+=(n[current_neurone][k]->err)*(n[current_neurone][k]->err);
	}
	errtot=errtot/(donnee->max[donnee->colonne-1]-donnee->min[donnee->colonne-1]);
	return errtot;
}

// Fonction erreur
void neurone_Erreur(Data donnee,Neurone** n,double lambda)
{
	// Recuperation du nombre de couche (k) et du nombre de ligne par couche (*ligne_by_col)
	int k = 1;
	int nb_neurone = donnee->colonne-1;
	while(nb_neurone/2)
	{
		k++;
		nb_neurone = nb_neurone/2;
	}
	int* ligne_by_col = (int*)malloc(sizeof(int)*k);
	nb_neurone = donnee->colonne-1;
	int i = 0;
	while(nb_neurone/2)
	{
		ligne_by_col[i]=nb_neurone;
		nb_neurone = nb_neurone/2;
		i++;
	}
	ligne_by_col[k-1]=donnee->max[donnee->colonne-1]-donnee->min[donnee->colonne-1];

	// Retropropagation des erreurs
	k--;
	while(k)
	{
		for (int current_neurone = 0; current_neurone < ligne_by_col[k-1]; ++current_neurone)
		{
			for (int j = 0; j < ligne_by_col[k]; ++j) n[current_neurone][k-1]->err += n[j][k]->err*n[j][k]->poids[current_neurone];
			n[current_neurone][k-1]->err=lambda*n[current_neurone][k-1]->out*(1-n[current_neurone][k-1]->out)*n[current_neurone][k-1]->err;
		}
		k--;
	}
	free(ligne_by_col);
}

// Fonction de correction des poids
void neurone_Correction(Data donnee,Neurone** n,double alpha,int ligne)
{
	// Recuperation du nombre de couche (k) et du nombre de ligne par couche (*ligne_by_col)
	int k = 1;
	int nb_neurone = donnee->colonne-1;
	while(nb_neurone/2)
	{
		k++;
		nb_neurone = nb_neurone/2;
	}
	int* ligne_by_col = (int*)malloc(sizeof(int)*k);
	nb_neurone = donnee->colonne-1;
	int i = 0;
	while(nb_neurone/2)
	{
		ligne_by_col[i]=nb_neurone;
		nb_neurone = nb_neurone/2;
		i++;
	}
	ligne_by_col[k-1]=donnee->max[donnee->colonne-1]-donnee->min[donnee->colonne-1];
	
	// Corriger l'ensemble des poids de chaque neurones
	k--;
	for (int j = 0; j < ligne_by_col[0]; ++j) for(int y=0;y<ligne_by_col[0];++y) n[j][0]->poids[y] = n[j][0]->poids[y] + alpha * n[j][0]->err * donnee->critere[y][ligne];

	for (int i = 1; i <= k; ++i) for (int j = 0; j < ligne_by_col[i]; ++j) for(int y=0;y<ligne_by_col[i-1];++y) n[j][i]->poids[y] = n[j][i]->poids[y] + alpha * n[j][i]->err * n[y][i-1]->out;
}

//Fonction de prédiction
int neurone_Prediction(Data donnee,int ligne,Neurone** n,double lambda)
{
	// Recuperation du nombre de couche
	int k = 1;
	int nb_neurone = donnee->colonne-1;
	while(nb_neurone/2)
	{
		k++;
		nb_neurone = nb_neurone/2;
	}
	k--;

	// Calcul de la sortie
	neurone_Apprentisage(donnee,ligne,n,lambda);
	int pred=0;
	int outDesired=(int)(((donnee->critere[donnee->colonne-1][ligne])*(donnee->max[donnee->colonne-1]-donnee->min[donnee->colonne-1])));
	for (int i = 0; i < (donnee->max[donnee->colonne-1]-donnee->min[donnee->colonne-1]); ++i) if(n[pred][k]->out < n[i][k]->out) pred = i;
	pred += donnee->min[donnee->colonne-1];

	// 1 si l'on trouve la prediction sinon 0
	if(pred==outDesired)
		return 1;
	else
		return 0;
}