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
void init_array ( double *a, double *y_1, double *y_2, double *x1, double *x2, int N)
{
	int i, j;
    for (i=0; i<N; i++) {
        y_1[i] = i;
        y_2[i] = i+1;
        x1[i] = 0.0;
        x2[i] = 0.0;

        for (j=0; j<N; j++)
            a[(i)*N + j] = i+j+1.0;
    }

}

static void func(double *a, double *y_1, double *y_2, double *x1, double *x2, int N)
{

  int i, j ;
#pragma scop
    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            x1[i] = x1[i] + a[(i)*N + j] * y_1[j];
        }
    }

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            x2[i] = x2[i] + a[(j)*N + i] * y_2[j];
        }
    }
#pragma endscop

}

int main( int argc, char** argv)
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

  double *a = (double*)malloc(n*n*sizeof(double));
  double *y_1 = (double*)malloc((n+17)*sizeof(double));
  double *y_2 = (double*)malloc((n+19)*sizeof(double));
  double *x_1 = (double*)malloc((n+23)*sizeof(double));
  double *x_2 = (double*)malloc((n+29)*sizeof(double));

  psgProtect(a, (long long) &a[0], (long long) &a[(n*n) - 1]);
  psgProtect(y_1, (long long) &y_1[0], (long long) &y_1[(n+17) - 1]);
  psgProtect(y_2, (long long) &y_2[0], (long long) &y_2[(n+19) - 1]);
  psgProtect(x_1, (long long) &x_1[0], (long long) &x_1[(n+23) - 1]);
  psgProtect(x_2, (long long) &x_2[0], (long long) &x_2[(n+29) - 1]);

  /* Initialize array */
  init_array(a, y_1, y_2, x_1, x_2, n);
  gettimeofday(&start, NULL);
  func(a, y_1, y_2, x_1, x_2, n);
  gettimeofday(&end, NULL);

  printf("Total time taken to execute the kernel: %lf seconds\n", (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000);
  runtime=(double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000;


	if(argc>=4)
	{
		print_data_2d(n,n,a,argv[3],1);
		print_data_1d(n+17,y_1,argv[3],0);
		print_data_1d(n+19,y_2,argv[3],0);
		print_data_1d(n+23,x_1,argv[3],0);
		print_data_1d(n+29,x_2,argv[3],0);
	}

   if(argc>=5) writeOVRData(argv[4],runtime,0);
   if(argc>=6) writeFPRData(argv[5],detectCounter,0);
	

	if(detectCounter)
		printf("\nInfo: Soft error detected\n");
	else
		printf("\nInfo: No Soft Errors detected\n");

	free(a);
	free(y_1);
	free(y_2);
	free(x_1);
	free(x_2);

	#ifdef INST
		printFaultSitesData();
		printFaultInjectionData();
	#endif
	


}
