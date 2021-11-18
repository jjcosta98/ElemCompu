#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include "tools.h"



/* Método Euler Implícito */

int main()
{
//Declaran variables y constantes a utilizar
int i,j,k,l;
double alfa=1.0 ,C ,tf=0.01 , dx , dt;
double x0=0.0;
double xf=1.0;
double *ti;
double *x;
double *T0, Ta=0.0, Tb=0.0;

int pasox=100, pasot=100; /*Se cambia el pasot para ver la convergencia o divergencia (en el caso del implícito si converge)*/

dx=(xf-x0)/pasox;
dt=(tf)/pasot;
C=(alfa*dt)/(dx*dx);

x= VectDin(pasox+1);
ti= VectDin(pasot+1);
T0= VectDin(pasox+1);


/*Vectores tiempo y espacio discretizados*/
for(j=0; j<pasox+1;j++ )
 {
		x[j]=x0+j*dx;
    T0[j]=exp(x[j]);
	}
for(i=0; i<pasot+1;i++ )
 {
		ti[i]=i*dt;
	}


/* Método Euler Implícito */


double **TI;
double **AI;

TI = MatDin(pasot+1, pasox+1);
AI = MatDin(pasox-1, pasox-1);


/*Matriz AI=(I+CK)*/


for(i=0; i<pasox-1;i++){
		for(j=0;j<pasox-1;j++){
			if(j==i){
				AI[i][j]=1+2*C;
			}else if(j==i-1){
				AI[i][j]=-C;
			}else if (j==i+1){
				AI[i][j]=-C;
			}else{
				AI[i][j]=0.0;
			}
		}
	}



/* Matriz final T^{k+1} que es la A*T^{k}+dt*q^{k}, solo las condiciones de frontera */



for(j=0; j<pasox+1;j++)
  {
		TI[0][j]=T0[j]; //condiciones iniciales dadas
	}
	for(i=0;i<pasot+1;i++)
  {
		TI[i][0]=0.0;
		TI[i][pasox]=0.0;
	}





/* Elemento q*dt*/


double *qIdt = VectDin(pasox-1);
double *vI = VectDin(pasox-1);
double *v2 = VectDin(pasox-1);
for(i=1; i<pasot;i++)
{
  for(j=0; j<pasox-1;j++)
  {
    qIdt[j]=cos(M_PI*ti[i+1])*sin(2*M_PI*x[j+1])*dt;


  

    /* Se van creando vectores T^{k} para luego sumarlo con q^{k+1}dt*/



    vI[j]=TI[i-1][j+1];
  }
}




/* Suma TIk +q^{k+1}*dt*/




for(i=1; i<pasot+1;i++)
{
  for(j=0; j<pasox-1;j++)
  {
    v2[j] = vI[j] + qIdt[j];
  }
  /* Se resuelve el sistema de ecuaciones lineales Ax=b utilizando Jacobi */

double *v3=JacobiM(pasox-1, AI, v2, 200); //Recibe AI y v2 para obtener T^{k+1}
		
  for(k=0;k<pasox-1;k++)
   {
			TI[i][k+1]=v3[k];
			
		}
	
	   free(v3);
	  
   

}

/*for (i=0;i<pasot+1;i++){
    for (j=0;j<pasox+1;j++){
      
      printf("%f \t",TI[i][j]);
    }
    printf("\n");
  }
  */

//se guardan las entradas de t,x, TI en un archivo .dat para luego graficar

FILE *HeatI2 = fopen("HeatI2.dat", "w");
	for (i = pasot/5; i < pasot+1; i++){
		for (j=pasox/5; j<pasox+1; j++){
    		fprintf(HeatI2, "%f %f %f", ti[i], x[j],TI[i][j]);
    		fprintf(HeatI2, "\n");
		}
	}
	fclose(HeatI2);

// Libera toda la memoria asignada y utilizada
free(x);
free(ti);
free(T0);
free(qIdt);
free(vI);
free(v2);

for(i=0;i<pasox-1;i++)
   {
	   free(AI[i]);
   }
	 free(AI);
for(i=0;i<pasot+1;i++)
   {
	   free(TI[i]);
   }
	 free(TI);



return 0;

}

