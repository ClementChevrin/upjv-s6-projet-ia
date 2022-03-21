#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../header/data.h"

typedef struct neurone
{
	double poids;
	double in,out,err;
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
	int nb_neurone = nb-1;
	int couche = c;
	Neurone** n=(Neurone**)malloc(sizeof(Neurone*)*nb_neurone);
	for (int i = 0; i < nb_neurone; ++i)
	{
		n[i] = (Neurone*)malloc(sizeof(Neurone)*couche);
		for (int j = 0; j < couche; ++j) 
		{	
			n[i][j]=(Neurone)malloc(sizeof(struct neurone));
			n[i][j]->poids = 1;
			n[i][j]->in = 0;
		}
	}
	return n;
}

// Free neurone struct
void neurone_Free(Neurone** n,int nb,int c)
{
	for(int i = 0; i < nb-1 ; i++)
	{
		for (int j = 0; j < c; ++j) free(n[i][j]);
		free(n[i]);
	}
	free(n);
}

// Fonction d'apprentisage
double neurone_Apprentisage(Data d,int ligne,Neurone** n,double lambda)
{
	for (int i = 0; i < d->col-1; ++i)
	{
		for (int j = 0; j < d->col-1; ++j) n[i][0]->in += d->critere[j][ligne]*n[i][0]->poids;
		n[i][0]->out = neurone_Sigmoide(n[i][0]->in,lambda);
	}
	int nb_neurone = d->col;
	int k = 1;
	while(nb_neurone/2)
	{
		for (int i = 0; i < nb_neurone/2; ++i)
		{
			for (int j = 0; j < nb_neurone; ++j)
			{
				n[i][k]->in += n[j][k-1]->out*n[i][k]->poids;
			}
			n[i][k]->out = neurone_Sigmoide(n[i][k]->in,lambda);
		}
		k++;
		nb_neurone = nb_neurone/2;
	}
	return n[0][k-1]->out;
}