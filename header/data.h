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
Data data_Init(int nb_donne,int ligne);

// Notify data struct
Data data_Notify(Data d,char* buffer,int i);

// Load data struct
Data data_Load(int c,int l,char* csv_path,int sizebuffer);

// Noramlized data struct
Data data_Normalized(Data d);

// Free data struct
void data_Free(Data d);