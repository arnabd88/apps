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
void init_array(double *a, int N)
{
    int j;

    for (j=0; j<N; j++) {
        a[j] = ((double)j)/N;
    }
}

static 
void func (double *a, double *b, int N, int T)
{

int t, j, i ;
#pragma scop
    for (t = 0; t < T; t++) {
        for (i = 2; i < N - 1; i++) {
            b[i] = 0.33333 * (a[i-1] + a[i] + a[i + 1]);
        }
        for (j = 2; j < N - 1; j++) {
            a[j] = b[j];
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

  double *a = (double*)malloc(n*sizeof(double));
  double *b = (double*)malloc(n*sizeof(double));
  psgProtect(a, (long long) &a[0], (long long) &a[n - 1]);
  psgProtect(b, (long long) &b[0], (long long) &b[n - 1]);

  /* Initialize array */

  init_array(a,n);
  gettimeofday(&start, NULL);
  func(a,b,n,tsteps);
  gettimeofday(&end,NULL);

  printf("Total time taken to execute the kernel: %lf seconds\n", (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000);
  runtime=(double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000;


	if(argc>=4)
	{
		print_data_1d(n,a,argv[3],1);
		print_data_1d(n,b,argv[3],0);
	}

   if(argc>=5) writeOVRData(argv[4],runtime,0);
   if(argc>=6) writeFPRData(argv[5],detectCounter,0);
	

	if(detectCounter)
		printf("\nInfo: Soft error detected\n");
	else
		printf("\nInfo: No Soft Errors detected\n");

	free(a);
	free(b);

	#ifdef INST
		printFaultSitesData();
		printFaultInjectionData();
	#endif
	
}

