#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct data
typedef struct data
{
	int ligne;
	int col;
	double** critere;	// fixe_acidity           0
						// volatile_acidity       1
						// citric_acid            2
						// residual_sugar         3
						// chlorides              4
						// free_sulfur_dioxide    5
						// total_sulfur_dioxide   6
						// density                7
						// pH                     8
						// sulphates              9
						// alcohol                10
	                    // quality                11 
	double* min;
	double* max;
	double* coef;
}*Data;


// Init data struct
Data data_Init(int nb_donne,int ligne)
{
	Data d = (Data)malloc(sizeof(struct data));
	d->col=nb_donne;
	d->ligne=ligne;
	d->critere = (double**)malloc(sizeof(double*)*nb_donne);
	for (int i = 0; i < nb_donne; ++i) d->critere[i]=(double*)malloc(sizeof(double)*ligne);
	d-> min = (double*)malloc(sizeof(double)*nb_donne); 
	for (int i = 0; i < nb_donne; ++i) d-> min[i] = 999999999;
	d-> max = (double*)malloc(sizeof(double)*nb_donne); 
	for (int i = 0; i < nb_donne; ++i) d-> max[i] = -999999999;
	d-> coef = (double*)malloc(sizeof(double)*nb_donne); 
	for (int i = 0; i < nb_donne; ++i) d-> coef[i] = 0;
	return d;
}

// Notify data struct
Data data_Notify(Data d,char* buffer,int i)
{
    // La définitions de séparateurs connus.
    const char * separators = ",;";
    int j = 0;
    char * strToken = strtok ( buffer, separators );
    while ( strToken != NULL ) 
    {    	
    	d->critere[j][i] = atof(strToken);
    	if (atof(strToken) < d->min[j]) d->min[j]=atof(strToken);
    	if (atof(strToken) > d->max[j]) d->max[j]=atof(strToken);
    	j++; 
        strToken = strtok ( NULL, separators );
    }
	return d;
}

// Load data struct
Data data_Load(int c,int l,char* csv_path,int sizebuffer)
{
	FILE* csv = fopen(csv_path,"r");
	if (csv != NULL)
	{
		Data donnee = data_Init(c,l);
		int i = 0;
		char* buffer=(char*)malloc(sizeof(char)*sizebuffer);
		while(fgets(buffer,sizebuffer,csv)!=NULL) 
		{	
			data_Notify(donnee,buffer,i);
			i++;
		}
		free(buffer);
		fclose(csv);
		return donnee;
	}
	else fprintf(stdout, "Erreur : Lecture du fichier \n");
	return NULL;
}

// Noramlized data struct
Data data_Normalized(Data d)
{
	for (int i = 0; i < d->ligne; ++i) for (int j = 0; j < d->col; ++j) d->critere[j][i]=(d->critere[j][i]-d->min[j])/(d->max[j]-d->min[j]);
}

// Free data struct
void data_Free(Data d)
{
	for (int i = 0; i < d->col; ++i) free(d->critere[i]);
	free(d-> critere);
	free(d-> min);
	free(d-> max);
	free(d-> coef);
	free(d);
}