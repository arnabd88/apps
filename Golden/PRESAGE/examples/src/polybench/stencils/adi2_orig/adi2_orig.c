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
void init_array(double *X, double *A, double *B, int N)
{
    int i, j;

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            A[(i)*N + j] = (1+(i*j)%1024)/2.0;
            B[(i)*N + j] = (1+(i*j)%1024)/3.0;
            X[(i)*N + j] = (1+(i*j)%1024)/3.0;
        }
    }
}

static 
void func(double *X, double *A, double *B, int N, int T)
{

  int t, i1, i2;
#pragma scop
    for (t = 0; t < T; t++) {

        for (i1=0; i1<N; i1++) {
            for (i2 = 1; i2 < N; i2++) {
                X[(i1)*N + i2] = X[(i1)*N + i2] - X[(i1)*N + i2-1] * A[(i1)*N + i2] / B[(i1)*N + i2-1];
                B[(i1)*N + i2] = B[(i1)*N + i2] - A[(i1)*N + i2] * A[(i1)*N + i2] / B[(i1)*N + i2-1];
            }
        }

        for (i1=1; i1<N; i1++) {
            for (i2 = 0; i2 < N; i2++) {
                X[(i1)*N + i2] = X[(i1)*N + i2] - X[(i1-1)*N + i2] * A[(i1)*N + i2] / B[(i1-1)*N + i2];
                B[(i1)*N + i2] = B[(i1)*N + i2] - A[(i1)*N + i2] * A[(i1)*N + i2] / B[(i1-1)*N + i2];
            }
        }
    }
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

  double *X = (double*)malloc(n*(n+13)*sizeof(double));
  double *A = (double*)malloc(n*(n+23)*sizeof(double));
  double *B = (double*)malloc(n*(n+37)*sizeof(double));
  psgProtect(X, (long long) &X[0], (long long) &X[(n*(n+13)) - 1]);
  psgProtect(A, (long long) &A[0], (long long) &A[(n*(n+23)) - 1]);
  psgProtect(B, (long long) &B[0], (long long) &B[(n*(n+37)) - 1]);

  /* Initialize array */

  init_array(X, A, B,n);
  gettimeofday(&start, NULL);
  func(X, A, B,n,tsteps);
  gettimeofday(&end,NULL);

  printf("Total time taken to execute the kernel: %lf seconds\n", (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000);
  runtime=(double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000;


	if(argc>=4)
	{
		print_data_2d(n,n,X,argv[3],1);
		print_data_2d(n,n,A,argv[3],0);
		print_data_2d(n,n,B,argv[3],0);
	}

   if(argc>=5) writeOVRData(argv[4],runtime,0);
   if(argc>=6) writeFPRData(argv[5],detectCounter,0);
	

	if(detectCounter)
		printf("\nInfo: Soft error detected\n");
	else
		printf("\nInfo: No Soft Errors detected\n");

	free(X);
	free(A);
	free(B);

	#ifdef INST
		printFaultSitesData();
		printFaultInjectionData();
	#endif
	
  
}
