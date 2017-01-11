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
void init_array( double *cos1, double *block, int M)
{
    int i, j;

    for (i=0; i<M; i++) {
        for (j=0; j<M; j++) {
            cos1[(i)*M + j] = (1+(i*j)%1024)/2.0;
            block[(i)*M + j] = (1+(i*j)%1024)/2.0;
        }
    }
}


static 
void func( double *cos1, double *block, double *temp2d, double *sum2, int M)
{

  int i, j, k;
#pragma scop
    for (i = 0; i < M; i++)
        for (j = 0; j < M; j++) {
            temp2d[(i)*M + j] = 0.0;
            for (k = 0; k < M; k++)
                temp2d[(i)*M + j] = temp2d[(i)*M + j] + block[(i)*M + k] * cos1[(j)*M + k];
        }

    for (i = 0; i < M; i++)
        for (j = 0; j < M; j++) {
            sum2[(i)*M + j] = 0.0;
            for (k = 0; k < M; k++)
                sum2[(i)*M + j] = sum2[(i)*M + j] + cos1[(i)*M + k] * temp2d[(k)*M + j];
            block[(i)*M + j] = (sum2[(i)*M + j]);
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

  double *cos1 = (double*)malloc(n*(n+13)*sizeof(double));
  double *block = (double*)malloc(n*(n+23)*sizeof(double));
  double *temp2d = (double*)malloc(n*(n+43)*sizeof(double));
  double *sum2 = (double*)malloc(n*n*sizeof(double));

  psgProtect( cos1, (long long) &cos1[0], (long long) &cos1[(n*(n+13)) - 1]);
  psgProtect( block, (long long) &block[0], (long long) &block[(n*(n+23)) - 1]);
  psgProtect( temp2d, (long long) &temp2d[0], (long long) &temp2d[(n*(n+43)) - 1]);
  psgProtect( sum2, (long long) &sum2[0], (long long) &sum2[(n*n) - 1]);

  /* Initialize array */
  init_array(cos1, block, n);

  gettimeofday(&start, NULL);
  func(cos1, block, temp2d, sum2, n);
  gettimeofday(&end, NULL);

  printf("Total time taken to execute the kernel: %lf seconds\n", (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000);
  runtime=(double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000;


	if(argc>=4)
	{
		print_data_2d(n,n+13,cos1,argv[3],1);
		print_data_2d(n,n+23,block,argv[3],0);
		print_data_2d(n,n+43,temp2d,argv[3],0);
		print_data_2d(n,n,sum2,argv[3],0);
	}

   if(argc>=5) writeOVRData(argv[4],runtime,0);
   if(argc>=6) writeFPRData(argv[5],detectCounter,0);
	

	if(detectCounter)
		printf("\nInfo: Soft error detected\n");
	else
		printf("\nInfo: No Soft Errors detected\n");

	free(cos1);
	free(block);
	free(temp2d);
	free(sum2);


	#ifdef INST
		printFaultSitesData();
		printFaultInjectionData();
	#endif
	
	return 0;
}
