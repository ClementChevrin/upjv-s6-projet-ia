#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../header/data.h"

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

// Fonction erreur
void neurone_Erreur(Data d,Neurone** n,double lambda)
{
	int k = 1;
	int nb_neurone = d->col-1;
	while(nb_neurone/2)
	{
		k++;
		nb_neurone = nb_neurone/2;
	}
	int* ligne_by_col = (int*)malloc(sizeof(int)*k);
	nb_neurone = d->col-1;
	int i = 0;
	while(nb_neurone/2)
	{
		ligne_by_col[i]=nb_neurone;
		nb_neurone = nb_neurone/2;
		i++;
	}
	ligne_by_col[k-1]=d->max[d->col-1]-d->min[d->col-1];
	k--;
	while(k)
	{
		for (int i = 0; i < ligne_by_col[k-1]; ++i)
		{
			for (int j = 0; j < ligne_by_col[k]; ++j) n[i][k-1]->err += n[j][k]->err;
			n[i][k-1]->err=lambda*n[i][k-1]->in*(1-n[i][k-1]->in)*n[i][k-1]->err;
		}
		k--;
	}
	/*for (int i = 0; i < 11; ++i)
	{
		printf("%lf\n",n[i][0]->err);
	}
	printf("\n");*/
	free(ligne_by_col);
}

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
			n[i][j]->poids = malloc(sizeof(double)*nb_neurone);
			for(int cpt=0;cpt<nb_neurone;cpt++) 
			{
				n[i][j]->poids[cpt]=0.1;
			}
			n[i][j]->in = 0;
			n[i][j]->err = 0;
		}
	}
	return n;
}

// Free neurone struct
void neurone_Free(Neurone** n,int nb,int c)
{
	for(int i = 0; i < nb-1 ; i++)
	{
		for (int j = 0; j < c; ++j) 
		{
			free(n[i][j]->poids);
			free(n[i][j]);
		}
		free(n[i]);
	}
	free(n);
}

// Fonction d'apprentisage
void neurone_Apprentisage(Data d,int ligne,Neurone** n,double lambda)
{
	for (int i = 0; i < d->col-1; ++i)
	{
		for (int j = 0; j < d->col-1; ++j) n[i][0]->in += d->critere[j][ligne]*n[i][0]->poids[j];
		n[i][0]->out = neurone_Sigmoide(n[i][0]->in,lambda);
	}
	int nb_neurone = d->col-1;
	int k = 1;
	while(nb_neurone/2 > 1)
	{
		for (int i = 0; i < nb_neurone/2; ++i)
		{
			for (int j = 0; j < nb_neurone; ++j) n[i][k]->in += n[j][k-1]->out*n[i][k]->poids[j];
			n[i][k]->out = neurone_Sigmoide(n[i][k]->in,lambda);
		}
		k++;
		nb_neurone = nb_neurone/2;
	}
	for (int i = 0; i < d->max[d->col-1]-d->min[d->col-1]; ++i)
	{
		for (int j = 0; j < nb_neurone; ++j) n[i][k]->in += n[j][k-1]->out*n[i][k]->poids[j];
		n[i][k]->out = neurone_Sigmoide(n[i][k]->in,lambda);
	}
	int max = 0;
	for (int i = 0; i < d->max[d->col-1]-d->min[d->col-1]; ++i) if(n[max][k]->out < n[i][k]->out) max = i;
	max += d->min[d->col-1];

	// Ajout de l'erreur initiale
	for (int i = 0; i < d->col-1; ++i) for (int j = 0; j < k; ++j) n[i][j]->err = 0;
	for (int i = 0; i < d->max[d->col-1]-d->min[d->col-1]; ++i) n[i][k]->err = lambda*n[i][k]->in*(1-n[i][k]->in)*(max-d->critere[ligne][d->col-1]);
	printf("%lf   ->    %d\n",n[0][k]->err,max);
}

// Fonction de correction des poids
void neurone_Correction(Data d,Neurone** n,double alpha)
{
	//printf("%lf\n",n[0][0]->poids);
	int k = 1;
	int nb_neurone = d->col-1;
	while(nb_neurone/2)
	{
		k++;
		nb_neurone = nb_neurone/2;
	}
	int* ligne_by_col = (int*)malloc(sizeof(int)*k);
	nb_neurone = d->col-1;
	int i = 0;
	while(nb_neurone/2)
	{
		ligne_by_col[i]=nb_neurone;
		nb_neurone = nb_neurone/2;
		i++;
	}
	ligne_by_col[k-1]=d->max[d->col-1]-d->min[d->col-1];
	k--;

	for (int i = 0; i < k; ++i)
	{
		for (int j = 0; j < ligne_by_col[k]; ++j)
		{
			n[j][i]->poids[j] = n[j][i]->poids[j] - alpha * n[j][i]->err * n[j][i]->in;  
		}
	}
	//printf("%lf\n",n[0][0]->poids);
}