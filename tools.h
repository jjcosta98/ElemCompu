//Se crea el archivo tools.h para poder hacer el include en los archivos heatE.c y heatI.c

double **MatDin(int fil, int col);
double *VectDin(int n);
double *MatMult(double **A, double *xx, int dimension, int fil);
double *JacobiM(int l, double **a, double *b, int nit);
