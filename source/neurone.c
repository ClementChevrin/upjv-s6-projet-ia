#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
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
			for (int j = 0; j < ligne_by_col[k]; ++j) n[i][k-1]->err += n[j][k]->err*n[j][k]->poids[i];
			n[i][k-1]->err=lambda*n[i][k-1]->out*(1-n[i][k-1]->out)*n[i][k-1]->err;
		}
		k--;
	}
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
double neurone_Apprentisage(Data d,int ligne,Neurone** n,double lambda)
{
	double errtot=0;
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
	for (int i = 0; i < (int)(d->max[d->col-1]-d->min[d->col-1]); ++i) if(n[max][k]->out < n[i][k]->out) max = i;
	//max += d->min[d->col-1];
	printf("max = %d\n",max);
	// Ajout de l'erreur initiale
	for (int i = 0; i < d->col-1; ++i) for (int j = 0; j < k; ++j) n[i][j]->err = 0;
	int outDesired=(int)(((d->critere[d->col-1][ligne])*(d->max[d->col-1]-d->min[d->col-1])));
	//printf("outDesired=%d\n",outDesired);
	for (int i = 0; i < d->max[d->col-1]-d->min[d->col-1]; ++i) 
		{
			if(i==outDesired)
			{
				n[i][k]->err = 1-n[i][k]->out;
			}
			else
				n[i][k]->err=0-n[i][k]->out;
			printf("neurone %d %d = %f\n",i,k,n[i][k]->out);
			errtot+=(n[i][k]->err)*(n[i][k]->err);
		}
	errtot=errtot/(d->max[d->col-1]-d->min[d->col-1]);
	return errtot;
}

// Fonction de correction des poids
void neurone_Correction(Data d,Neurone** n,double alpha)
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

	for (int i = 0; i <= k; ++i)
		for (int j = 0; j < ligne_by_col[k]; ++j)
		{
			for(int y=0;y<ligne_by_col[k-1];++y)
			{
				n[j][i]->poids[y] = n[j][i]->poids[y] + alpha * n[j][i]->err * n[j][i]->in;
			}
		}
}

//Fonction de prédiction
int neurone_Prediction(Data d,int ligne,Neurone** n,double lambda)
{
	int k = 1;
	int nb_neurone = d->col-1;
	while(nb_neurone/2)
	{
		k++;
		nb_neurone = nb_neurone/2;
	}
	k--;
	neurone_Apprentisage(d,ligne,n,lambda);
	int pred=0;
	int outDesired=(int)(((d->critere[d->col-1][ligne])*(d->max[d->col-1]-d->min[d->col-1])));
	printf("désirée = %d\n",outDesired);
	for (int i = 0; i < (d->max[d->col-1]-d->min[d->col-1]); ++i) 
	{
		printf("out=%f\n",n[i][k]->out);
		if(n[pred][k]->out < n[i][k]->out) pred = i;
	}
	printf("pred=%d\n",pred);
	pred += d->min[d->col-1];
	if(pred==outDesired)
		return 1;
	else
		return 0;
}