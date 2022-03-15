#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/data.h"

typedef struct neurone
{
	double* poids;
	double entree, erreur;
}*Neurone;