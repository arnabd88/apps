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

#define coeff1 0.5
#define coeff2 0.7

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
void init_array(double *h, double *e, int N)
{
    int i, j;

        for (j=0; j<N; j++) {
            h[j] = ((double)j)/N;
            e[j] = ((double)j)/N;
        }

}

static 
void func(double *h, double *e, int N, int T )
{

	int t, i ;
#pragma scop
    for (t=1; t<=T; t++){
	    for (i=1; i<=N-1; i++)
		    e[i] = e[i] - coeff1*(h[i]-h[i-1]);
	    for (i=0; i<=N-1; i++)
		    h[i] = h[i] - coeff2*(e[i+1]-e[i]);
    }
#pragma endscop

}


int main ( int argc, char** argv )
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

  double *h = (double*)malloc(n*sizeof(double));
  double *e = (double*)malloc((n+1)*sizeof(double));

  psgProtect(h, (long long) &h[0], (long long) &h[(n) - 1]);
  psgProtect(e, (long long) &e[0], (long long) &e[(n+1) - 1]);

  /* Initialize array */
  init_array(h,e,n);

  gettimeofday(&start, NULL);
  func(h, e, n, tsteps);
  gettimeofday(&end, NULL);

  printf("Total time taken to execute the kernel: %lf seconds\n", (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000);
  runtime=(double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000;


	if(argc>=4)
	{
		print_data_1d(n,h,argv[3],1);
		print_data_1d(n+1,h,argv[3],0);
	}

   if(argc>=5) writeOVRData(argv[4],runtime,0);
   if(argc>=6) writeFPRData(argv[5],detectCounter,0);
	

	if(detectCounter)
		printf("\nInfo: Soft error detected\n");
	else
		printf("\nInfo: No Soft Errors detected\n");

	free(h);
	free(e);

	#ifdef INST
		printFaultSitesData();
		printFaultInjectionData();
	#endif
	


}

