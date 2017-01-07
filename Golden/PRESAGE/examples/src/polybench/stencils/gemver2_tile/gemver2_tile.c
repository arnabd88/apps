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
/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 8.0.0.  Version 8.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2014, plus Amendment 1 (published
   2015-05-15).  */
/* We do not support C11 <threads.h>.  */
  int t1, t2, t3, t4, t5, t6;
 register int lbv, ubv;
/* Start of CLooG code */
if (N >= 1) {
  for (t2=0;t2<=floord(N-1,32);t2++) {
    for (t3=0;t3<=floord(N-1,32);t3++) {
      for (t4=32*t3;t4<=min(N-1,32*t3+31);t4++) {
        lbv=32*t2;
        ubv=min(N-1,32*t2+31);
#pragma ivdep
#pragma vector always
        for (t5=lbv;t5<=ubv;t5++) {
          B[(t4)*N + t5] = A[(t4)*N + t5] + u1[t4]*v1[t5] + u2[t4]*v2[t5];;
          x[t5] = x[t5] + beta* B[(t4)*N + t5]*y[t4];;
        }
      }
    }
  }
  for (t2=0;t2<=floord(N-1,32);t2++) {
    lbv=32*t2;
    ubv=min(N-1,32*t2+31);
#pragma ivdep
#pragma vector always
    for (t3=lbv;t3<=ubv;t3++) {
      x[t3] = x[t3] + z[t3];;
    }
  }
  for (t2=0;t2<=floord(N-1,32);t2++) {
    for (t3=0;t3<=floord(N-1,32);t3++) {
      for (t4=32*t2;t4<=min(N-1,32*t2+31);t4++) {
        for (t5=32*t3;t5<=min(N-1,32*t3+31);t5++) {
          w[t4] = w[t4] + alpha* B[(t4)*N + t5]*x[t5];;
        }
      }
    }
  }
}
/* End of CLooG code */

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

