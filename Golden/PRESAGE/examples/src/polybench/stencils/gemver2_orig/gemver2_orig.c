#include <math.h>
#define ceild(n,d)  ceil(((double)(n))/((double)(d)))
#define floord(n,d) floor(((double)(n))/((double)(d)))
#define max(x,y)    ((x) > (y)? (x) : (y))
#define min(x,y)    ((x) < (y)? (x) : (y))

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

/* presage specific headers */
#include "Utility.h"
#include "PresageRT.h"



#define alpha 1
#define beta 1

double t_start, t_end ;

extern long dim0;
extern long dim1;
extern long dim2;
extern int dimcount;
extern int psgdim[];
extern long long detectCounter;

#ifdef INST
extern int printFaultSitesData(void);
extern int printFaultInjectionData(void);
#endif

/* Array Initialization */
static 
void init_array(double *u1, double *u2, double *v1, double *v2, double *x, double *y, double *w, double *z, double *A, int N )
{
    int i, j;

    for (i=0; i<N; i++) {
        u1[i] = i;
        u2[i] = (i+1)/N/2.0;
        v1[i] = (i+1)/N/4.0;
        v2[i] = (i+1)/N/6.0;
        y[i] = (i+1)/N/8.0;
        z[i] = (i+1)/N/9.0;
        x[i] = 0.0;
        w[i] = 0.0;
        for (j=0; j<N; j++) {
            A[(i)*N + j] = ((double) i*j)/N;
        }
    }
}


static 
void func(double *u1, double *u2, double *v1, double *v2, double *x, double *y, double *w, double *z, double *A, double *B, int N )
{

  int i, j ;
#pragma scop
    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            B[(i)*N + j] = A[(i)*N + j] + u1[i]*v1[j] + u2[i]*v2[j];

    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            x[i] = x[i] + beta* B[(j)*N + i]*y[j];


    for (i=0; i<N; i++)
        x[i] = x[i] + z[i];

    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            w[i] = w[i] + alpha* B[(i)*N + j]*x[j];
#pragma endscop

}

int main( int argc, char** argv )
{

  double runtime=0.0 ;
  struct timeval start, end;

  if(argc < 3)
  	printf("\nINFO: Insufficient arguments!\n\n");

  int n = atoi(argv[1]);
  int tsteps = atoi(argv[2]);

  dimcount=1;
  psgdim[0] = n;
  dim0 = n;

  double *u1 = (double*)malloc(n*sizeof(double));
  double *u2 = (double*)malloc(n*sizeof(double));
  double *v1 = (double*)malloc(n*sizeof(double));
  double *v2 = (double*)malloc(n*sizeof(double));
  double *x = (double*)malloc(n*sizeof(double));
  double *w = (double*)malloc(n*sizeof(double));
  double *y = (double*)malloc(n*sizeof(double));
  double *z = (double*)malloc(n*sizeof(double));
  double *A = (double*)malloc(n*n*sizeof(double));
  double *B = (double*)malloc(n*n*sizeof(double));

  psgProtect(u1, (long long) &u1[0], (long long) &u1[n - 1]);
  psgProtect(u2, (long long) &u2[0], (long long) &u2[n - 1]);
  psgProtect(v1, (long long) &v1[0], (long long) &v1[n - 1]);
  psgProtect(v2, (long long) &v2[0], (long long) &v2[n - 1]);
  psgProtect(x, (long long) &x[0], (long long) &x[n - 1]);
  psgProtect(y, (long long) &y[0], (long long) &y[n - 1]);
  psgProtect(w, (long long) &w[0], (long long) &w[n - 1]);
  psgProtect(z, (long long) &z[0], (long long) &z[n - 1]);


    /* Initialize array */

  init_array(u1,u2,v1,v2,x,y,w,z,A,n);
  gettimeofday(&start, NULL);
  func(u1,u2,v1,v2,x,y,w,z,A,B,n);
  gettimeofday(&end,NULL);

  printf("Total time taken to execute the kernel: %lf seconds\n", (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000);
  runtime=(double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000;


	if(argc>=4)
	{
		print_data_1d(n,u1,argv[3],1);
		print_data_1d(n,u2,argv[3],0);
		print_data_1d(n,v1,argv[3],0);
		print_data_1d(n,v2,argv[3],0);
		print_data_1d(n,x,argv[3],0);
		print_data_1d(n,y,argv[3],0);
		print_data_1d(n,w,argv[3],0);
		print_data_1d(n,z,argv[3],0);
		print_data_2d(n,n,A,argv[3],0);
		print_data_2d(n,n,B,argv[3],0);
	}

   if(argc>=5) writeOVRData(argv[4],runtime,0);
   if(argc>=6) writeFPRData(argv[5],detectCounter,0);
	

	if(detectCounter)
		printf("\nInfo: Soft error detected\n");
	else
		printf("\nInfo: No Soft Errors detected\n");

	free(u1);
	free(u2);
	free(v1);
	free(v2);
	free(x);
	free(y);
	free(w);
	free(z);
	free(A);
	free(B);

	#ifdef INST
		printFaultSitesData();
		printFaultInjectionData();
	#endif
	


}

