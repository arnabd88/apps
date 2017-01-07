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
  int t1, t2, t3, t4, t5;
 register int lbv, ubv;
/* Start of CLooG code */
if ((N >= 4) && (T >= 1)) {
  for (t1=0;t1<=floord(T-1,32);t1++) {
    for (t2=2*t1;t2<=min(floord(2*T+N-3,32),floord(64*t1+N+61,32));t2++) {
      if (t1 <= floord(32*t2-N+1,64)) {
        if ((N+1)%2 == 0) {
          a[(N-2)] = b[(N-2)];;
        }
      }
      if (N == 4) {
        for (t3=max(32*t1,16*t2-1);t3<=min(min(T-1,32*t1+31),16*t2+14);t3++) {
          b[2] = 0.33333 * (a[2 -1] + a[2] + a[2 + 1]);;
          a[2] = b[2];;
        }
      }
      for (t3=max(ceild(32*t2-N+2,2),32*t1);t3<=min(min(min(floord(32*t2-N+32,2),T-1),32*t1+31),16*t2-2);t3++) {
        for (t4=32*t2;t4<=2*t3+N-2;t4++) {
          b[(-2*t3+t4)] = 0.33333 * (a[(-2*t3+t4)-1] + a[(-2*t3+t4)] + a[(-2*t3+t4) + 1]);;
          a[(-2*t3+t4-1)] = b[(-2*t3+t4-1)];;
        }
        a[(N-2)] = b[(N-2)];;
      }
      for (t3=max(ceild(32*t2-N+33,2),32*t1);t3<=min(min(T-1,32*t1+31),16*t2-2);t3++) {
        for (t4=32*t2;t4<=32*t2+31;t4++) {
          b[(-2*t3+t4)] = 0.33333 * (a[(-2*t3+t4)-1] + a[(-2*t3+t4)] + a[(-2*t3+t4) + 1]);;
          a[(-2*t3+t4-1)] = b[(-2*t3+t4-1)];;
        }
      }
      if (N >= 5) {
        for (t3=max(32*t1,16*t2-1);t3<=min(min(floord(32*t2-N+32,2),T-1),32*t1+31);t3++) {
          b[2] = 0.33333 * (a[2 -1] + a[2] + a[2 + 1]);;
          for (t4=2*t3+3;t4<=2*t3+N-2;t4++) {
            b[(-2*t3+t4)] = 0.33333 * (a[(-2*t3+t4)-1] + a[(-2*t3+t4)] + a[(-2*t3+t4) + 1]);;
            a[(-2*t3+t4-1)] = b[(-2*t3+t4-1)];;
          }
          a[(N-2)] = b[(N-2)];;
        }
      }
      for (t3=max(max(ceild(32*t2-N+33,2),32*t1),16*t2-1);t3<=min(min(T-1,32*t1+31),16*t2+14);t3++) {
        b[2] = 0.33333 * (a[2 -1] + a[2] + a[2 + 1]);;
        for (t4=2*t3+3;t4<=32*t2+31;t4++) {
          b[(-2*t3+t4)] = 0.33333 * (a[(-2*t3+t4)-1] + a[(-2*t3+t4)] + a[(-2*t3+t4) + 1]);;
          a[(-2*t3+t4-1)] = b[(-2*t3+t4-1)];;
        }
      }
    }
  }
}
/* End of CLooG code */
	
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

