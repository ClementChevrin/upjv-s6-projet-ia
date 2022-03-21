#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../header/data.h"

typedef struct neurone
{
	double poids;
	double entree, erreur;
}*Neurone;

// Fonction sigmoide
double neurone_Sigmoide(double x,double lambda)
{
	return 1/(1+exp(-lambda*x));
}

// Fonction erreur
//double neurone_Erreur()

// Init neurone struct
Neurone** neurone_Init(int nb, int c)
{
	int nb_neurone = nb;
	int couche = c;
	Neurone** n=(Neurone**)malloc(sizeof(Neurone*)*nb_neurone);
	for (int i = 0; i < nb_neurone; ++i)
	{
		n[i] = (Neurone*)malloc(sizeof(Neurone)*couche);
		for (int j = 0; j < couche; ++j) 
		{	
			n[i][j]=(Neurone)malloc(sizeof(struct neurone));
			n[i][j]->poids = 1;
		}
	}
	return n;
}

// Free neurone struct
void neurone_Free(Neurone** n,int nb,int c)
{
	for(int i = 0; i < nb ; i++)
	{
		for (int j = 0; j < c; ++j) free(n[i][j]);
		free(n[i]);
	}
	free(n);
}