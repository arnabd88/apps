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
  int t1, t2, t3;
 register int lbv, ubv;
/* Start of CLooG code */
if ((N >= 1) && (T >= 1)) {
  if (N >= 2) {
    for (t1=1;t1<=T;t1++) {
      for (t2=t1+1;t2<=t1+N-1;t2++) {
        e[(-t1+t2)] = e[(-t1+t2)] - coeff1*(h[(-t1+t2)]-h[(-t1+t2)-1]);;
        h[(-t1+t2-1)] = h[(-t1+t2-1)] - coeff2*(e[(-t1+t2-1)+1]-e[(-t1+t2-1)]);;
      }
      h[(N-1)] = h[(N-1)] - coeff2*(e[(N-1)+1]-e[(N-1)]);;
    }
  }
  if (N == 1) {
    for (t1=1;t1<=T;t1++) {
      h[0] = h[0] - coeff2*(e[0 +1]-e[0]);;
    }
  }
}
/* End of CLooG code */

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

