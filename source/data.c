#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct data
typedef struct data
{
	int ligne;          // Nombre de ligne de donnee
	int colonne;        // Nombre de colonne de donnee
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
	double* min;        // Valeur minimale de chaque critere
	double* max;        // Valeur maximale de chaque critere
	double* coef;       // Coefficient de normalisation de chaque critere
}*Data;


// Init data struct
Data data_Init(int nb_donne,int ligne)
{
	Data donnee = (Data)malloc(sizeof(struct data));
	donnee->colonne=nb_donne;
	donnee->ligne=ligne;
	donnee->critere = (double**)malloc(sizeof(double*)*nb_donne);
	for (int i = 0; i < nb_donne; ++i) donnee->critere[i]=(double*)malloc(sizeof(double)*ligne);
	donnee-> min = (double*)malloc(sizeof(double)*nb_donne); 
	for (int i = 0; i < nb_donne; ++i) donnee-> min[i] = 999999999;
	donnee-> max = (double*)malloc(sizeof(double)*nb_donne); 
	for (int i = 0; i < nb_donne; ++i) donnee-> max[i] = -999999999;
	donnee-> coef = (double*)malloc(sizeof(double)*nb_donne); 
	for (int i = 0; i < nb_donne; ++i) donnee-> coef[i] = 0;
	return donnee;
}

// Notify data struct
Data data_Notify(Data donnee,char* buffer,int ligne)
{
    // La définitions de séparateurs connus.
    const char * separators = ",;";
    int j = 0;
    char * strToken = strtok ( buffer, separators );
    while ( strToken != NULL ) 
    {    	
    	donnee->critere[j][ligne] = atof(strToken);
    	if (atof(strToken) < donnee->min[j]) donnee->min[j]=atof(strToken);
    	if (atof(strToken) > donnee->max[j]) donnee->max[j]=atof(strToken);
    	j++; 
        strToken = strtok ( NULL, separators );
    }
	return donnee;
}

// Load data struct
Data data_Load(int colonne,int ligne,char* csv_path,int sizebuffer)
{
	FILE* csv = fopen(csv_path,"r");
	if (csv != NULL)
	{
		Data donnee = data_Init(colonne,ligne);
		int current_ligne = 0;
		char* buffer=(char*)malloc(sizeof(char)*sizebuffer);
		while(fgets(buffer,sizebuffer,csv)!=NULL) 
		{	
			data_Notify(donnee,buffer,current_ligne);
			current_ligne++;
		}
		free(buffer);
		fclose(csv);
		return donnee;
	}
	else fprintf(stdout, "Erreur : Lecture du fichier \n");
	return NULL;
}

// Noramlized data struct
Data data_Normalized(Data donnee)
{
	for (int ligne = 0; ligne < donnee->ligne; ++ligne) 
	{
		for (int colonne = 0; colonne < donnee->colonne; ++colonne) 
		{
			donnee->critere[colonne][ligne]=((donnee->critere[colonne][ligne]-donnee->min[colonne])/(donnee->max[colonne]-donnee->min[colonne]));
		}
	}
}

// Free data struct
void data_Free(Data donnee)
{
	for (int colonne = 0; colonne < donnee->colonne; ++colonne) free(donnee->critere[colonne]);
	free(donnee-> critere);
	free(donnee-> min);
	free(donnee-> max);
	free(donnee-> coef);
	free(donnee);
}