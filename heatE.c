#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include "tools.h"



/* Método Euler Explícito */

int main()
{

int i,j;
double alfa=1.0 ,C ,tf=0.01 , dx , dt;
double x0=0.0;
double xf=1.0;
double **A, **T;
double *t;
double *x;
double *T0, Ta=0.0, Tb=0.0;

int pasox=100, pasot=1000; /*Se cambia el pasot para ver la convergencia o divergencia*/

dx=(xf-x0)/pasox;
dt=(tf)/pasot;
C=(alfa*dt)/(dx*dx);
x= VectDin(pasox+1);
t= VectDin(pasot+1);
T0= VectDin(pasox+1);
A= MatDin(pasox-1, pasox-1);
T= MatDin(pasot+1, pasox+1);

/*Vectores tiempo y espacio discretizados*/
for(j=0; j<pasox+1;j++ )
 {
		x[j]=x0+j*dx;
    T0[j]=exp(x[j]);
	}
for(i=0; i<pasot+1;i++ )
 {
		t[i]=i*dt;
	}


/*Matriz A=(I-CK)*/
for(i=0; i<pasox-1;i++){
		for(j=0;j<pasox-1;j++){
			if(j==i){
				A[i][j]=1-2*C;
			}else if(j==i-1){
				A[i][j]=C;
			}else if (j==i+1){
				A[i][j]=C;
			}else{
				A[i][j]=0.0;
			}
		}
	}

/* Matriz final T^{k+1} que es la A*T^{k}+dt*q^{k}, solo las condiciones de frontera */
for(j=0; j<pasox+1;j++)
  {
		T[0][j]=T0[j];
	}
	for(i=0;i<pasot+1;i++)
  {
		T[i][0]=Ta;
		T[i][pasox]=Tb;
	}

/* Elemento q*dt*/
double *qdt = VectDin(pasox-1);
double *v = VectDin(pasox-1);
for(i=1; i<pasot+1;i++)
{
  for(j=0; j<pasox-1;j++)
  {
    qdt[j]=cos(M_PI*t[i])*sin(2*M_PI*x[j+1])*dt;
/* Se van creando vectores T^{k} para luego multiplicarlos con A, con x variando y t fijo*/
    v[j]=T[i-1][j+1];
  }


/* Solución final Euler Explícito*/
double *ATk= MatMult(A, v, pasox-1, pasox-1);
  
  for(j=0; j<pasox-1;j++)
  {
    T[i][j+1]=ATk[j]+qdt[j];
    
  }
  free(ATk);
}


/*for (i=0;i<pasot+1;i++){
    for (j=0;j<pasox+1;j++){
      
      printf("%f \t",T[i][j]);
    }
    printf("\n");
  }
*/


FILE *HeatE = fopen("HeatE.dat", "w");
	for (i = pasot/5; i < pasot+1; i++){
		for (j=pasox/5; j< pasox+1; j++){
    		fprintf(HeatE, "%f %f %f", t[i], x[j],T[i][j]);
    		fprintf(HeatE, "\n");
		}
	}
	fclose(HeatE);


free(x);
free(t);
free(T0);
free(qdt);
free(v);
for(i=0;i<pasox-1;i++)
   {
	   free(A[i]);
   }
	 free(A);
for(i=0;i<pasot+1;i++)
   {
	   free(T[i]);
   }
	 free(T);
//free(ATk);


return 0;


}

