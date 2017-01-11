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
  int t1, t2, t3, t4, t5, t6, t7, t8, t9;
 register int lbv, ubv;
/* Start of CLooG code */
if (M >= 1) {
  for (t2=0;t2<=floord(M-1,32);t2++) {
    for (t3=0;t3<=floord(M-1,32);t3++) {
      for (t4=32*t2;t4<=min(M-1,32*t2+31);t4++) {
        lbv=32*t3;
        ubv=min(M-1,32*t3+31);
#pragma ivdep
#pragma vector always
        for (t5=lbv;t5<=ubv;t5++) {
          sum2[(t4)*M + t5] = 0.0;;
          temp2d[(t4)*M + t5] = 0.0;;
        }
      }
    }
  }
  for (t2=0;t2<=floord(M-1,32);t2++) {
    for (t3=0;t3<=floord(M-1,32);t3++) {
      for (t5=0;t5<=floord(M-1,32);t5++) {
        for (t6=32*t2;t6<=min(M-1,32*t2+31);t6++) {
          for (t7=32*t3;t7<=min(M-1,32*t3+31);t7++) {
            for (t9=32*t5;t9<=min(M-1,32*t5+31);t9++) {
              temp2d[(t7)*M + t6] = temp2d[(t7)*M + t6] + block[(t7)*M + t9] * cos1[(t6)*M + t9];;
            }
          }
        }
      }
      for (t5=0;t5<=floord(M-1,32);t5++) {
        for (t6=32*t2;t6<=min(M-1,32*t2+31);t6++) {
          for (t7=32*t3;t7<=min(M-1,32*t3+31);t7++) {
            lbv=32*t5;
            ubv=min(M-1,32*t5+31);
#pragma ivdep
#pragma vector always
            for (t9=lbv;t9<=ubv;t9++) {
              sum2[(t9)*M + t6] = sum2[(t9)*M + t6] + cos1[(t9)*M + t7] * temp2d[(t7)*M + t6];;
            }
          }
        }
      }
    }
  }
  for (t2=0;t2<=floord(M-1,32);t2++) {
    for (t3=0;t3<=floord(M-1,32);t3++) {
      for (t4=32*t2;t4<=min(M-1,32*t2+31);t4++) {
        lbv=32*t3;
        ubv=min(M-1,32*t3+31);
#pragma ivdep
#pragma vector always
        for (t5=lbv;t5<=ubv;t5++) {
          block[(t4)*M + t5] = (sum2[(t4)*M + t5]);;
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
