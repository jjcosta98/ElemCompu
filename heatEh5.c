#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include "tools.h"
#include "hdf5.h"

#define FILE            "hex2.h5"
#define DATASET         "DS1"
#define DIM0            11
#define DIM1            11


 

/* Método Euler Explícito */

int main(void)
{
int pasox=20, pasot=20; /*Se cambia el pasot para ver la convergencia o divergencia*/
int b = (pasot+1)/10;	
hid_t           file, space, dset, dcpl;    /* Handles */
herr_t          status;
H5D_layout_t    layout;
hsize_t         dims[2] = {b, pasox+1};
double          wdata[b][pasox+1],          /* Write buffer */
                rdata[b][pasox+1];	/* Read buffer */	

int i,j;
double alfa=1.0 ,C ,tf=0.01 , dx , dt;
double x0=0.0;
double xf=1.0;
double **A, **T;
double *t;
double *x;
double *T0, Ta=0.0, Tb=0.0;



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
int contador=0;
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
  if(i%10==0)
		{
		wdata[contador][j]=T[i][j];
		contador=contador+1;
		}
	
  
  free(ATk);
}


for (i=0;i<pasot+1;i++){
    for (j=0;j<pasox+1;j++){
      
      printf("%f \t",T[i][j]);
    }
    printf("\n");
  }



/*
FILE *HeatE2 = fopen("HeatE2.dat", "w");
	for (i = pasot/5; i < pasot+1; i++){
		for (j=pasox/5; j< pasox+1; j++){
    		fprintf(HeatE2, "%f %f %f", t[i], x[j],T[i][j]);
    		fprintf(HeatE2, "\n");
		}
	}
	fclose(HeatE2);
*/

	
   
                

    
     /* Initialize data.*/
   
			
		 
    
    /*
     * Create a new file using the default properties.
     */
    file = H5Fcreate (FILE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    /*
     * Create dataspace.  Setting maximum size to NULL sets the maximum
     * size to be the current size.
     */
    space = H5Screate_simple (2, dims, NULL);

    /*
     * Create the dataset creation property list, set the layout to
     * compact.
     */
    dcpl = H5Pcreate (H5P_DATASET_CREATE);
    status = H5Pset_layout (dcpl, H5D_COMPACT);

    /*
     * Create the dataset.  We will use all default properties for this
     * example.
     */
    dset = H5Dcreate (file, DATASET, H5T_IEEE_F32BE, space, H5P_DEFAULT, dcpl,
                H5P_DEFAULT);

    /*
     * Write the data to the dataset.
     */
    status = H5Dwrite (dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                wdata[0]);

    /*
     * Close and release resources.
     */
    status = H5Pclose (dcpl);
    status = H5Dclose (dset);
    status = H5Sclose (space);
    status = H5Fclose (file);


    /*
     * Now we begin the read section of this example.
     */

    /*
     * Open file and dataset using the default properties.
     */
    file = H5Fopen (FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
    dset = H5Dopen (file, DATASET, H5P_DEFAULT);

    /*
     * Retrieve the dataset creation property list, and print the
     * storage layout.
     */
    dcpl = H5Dget_create_plist (dset);
    layout = H5Pget_layout (dcpl);
    printf ("Storage layout for %s is: ", DATASET);
    switch (layout) {
        case H5D_COMPACT:
            printf ("H5D_COMPACT\n");
            break;
        case H5D_CONTIGUOUS:
            printf ("H5D_CONTIGUOUS\n");
            break;
        case H5D_CHUNKED:
            printf ("H5D_CHUNKED\n");
    }

    /*
     * Read the data using the default properties.
     */
    status = H5Dread (dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                rdata[0]);

    /*
     * Output the data to the screen.
     */
    printf ("%s:\n", DATASET);
    for (i=0; i<contador; i++) {
        printf (" [");
        for (j=0; j<pasox+1; j++)
            printf (" %f", rdata[i][j]);
        printf ("]\n");
    }

    /*
     * Close and release resources.
     */
    status = H5Pclose (dcpl);
    status = H5Dclose (dset);
    status = H5Fclose (file);



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

