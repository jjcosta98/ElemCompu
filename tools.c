#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include "tools.h"


/* Creación Matriz dinámica para producto Matriz-vector */
double **MatDin(int fil, int col)
{
	 int i;
	 double **A = NULL;
	 A = (double **)malloc(fil*sizeof(double *));
	 for(i=0;i<fil;i++)
   {
		 A[i]=(double *)malloc(col*sizeof(double));
		 if(A[i] == NULL)
     {
		   perror("ERROR. There is not enough memory");
		   exit(EXIT_FAILURE);
		 }
	 }

	 return A;
}

/* Genero Vector Dinámico para producto */
double *VectDin(int n)
{
	double *vector=NULL;
	vector=(double *) malloc((size_t) n * sizeof(double) );
	if(vector==NULL)
  {
		perror("ERROR. There is not enough memory");
		exit(EXIT_FAILURE);
	}
	return vector;
}



  /*Multiplicación Matriz Vector*/

  double *MatMult(double **A, double *xx, int dimension, int fil)
{

	int i,j;
	double *bb=NULL;
	bb=(double *) malloc((size_t) fil* sizeof(double) );
	if(bb==NULL)
  {
		perror("ERROR. There is not enough memory");
		exit(EXIT_FAILURE);
	}
	for(i=0; i<fil; i++)
  {
		double contador=0.0;
		for(j=0; j<dimension;j++)
    {
			contador+=A[i][j]*xx[j];
		}
		bb[i]=contador;

	}
	return bb;

}

double *JacobiM(int l, double **a, double *b, int nit)
{
	double *x=NULL;
	x=(double *) malloc((size_t) l * sizeof(double) );
	if(x==NULL)
  {
		perror("ERROR. There is not enough memory");
		exit(EXIT_FAILURE);
	}
  int p;
	for(p=0; p<l;p++)
  {
		x[p]=0.0000;
  }
	double *c;
	c = VectDin(l);
	int k, i, j;
	for (k=0;k<nit;k++)
  {
	for (i=0;i<l;i++) 
  {
		c[i]=b[i];
		for (j=0;j<l;j++)
    {
			if (i!=j) 
      {
				c[i]=c[i]-a[i][j]*x[j];
			}
		}	
	}
	for (i=0;i<l;i++)
  {
		x[i]=c[i]/a[i][i];
	}
  }

	return x;
}
