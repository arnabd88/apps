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

static void init_array(double *a, int N)
{
    int i, j;

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            a[i*N + j] = i*i+j*j;
        }
    }
}

static 
void func(double *a, int N, int T)
{
    int i, j, k, t;
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
if ((N >= 3) && (T >= 1)) {
  for (t1=0;t1<=floord(T-1,256);t1++) {
    for (t2=t1;t2<=min(floord(T+N-3,256),floord(256*t1+N+253,256));t2++) {
      for (t3=max(ceild(512*t2-N-252,256),t1+t2);t3<=min(min(min(min(floord(T+N-3,128),floord(256*t1+N+253,128)),floord(512*t2+N+507,256)),floord(256*t1+256*t2+N+508,256)),floord(256*t2+T+N+252,256));t3++) {
        for (t4=max(max(max(ceild(256*t2-N-29,32),ceild(128*t3-N-29,32)),ceild(-256*t2+256*t3-N-284,32)),8*t1);t4<=min(min(floord(T-1,32),8*t1+7),4*t3+3);t4++) {
          for (t5=max(max(max(max(ceild(256*t3-T-N-28,32),ceild(256*t3-32*t4-N-60,32)),ceild(256*t3-N-59,64)),8*t2),t4);t5<=min(min(min(min(floord(T+N-3,32),floord(32*t4+N+29,32)),floord(256*t3+N+252,64)),8*t2+7),8*t3-t4+7);t5++) {
            for (t6=max(max(ceild(64*t5-N-28,32),8*t3),t4+t5);t6<=min(min(min(min(min(floord(T+N-3,16),floord(32*t4+N+29,16)),floord(64*t5+N+59,32)),floord(32*t4+32*t5+N+60,32)),floord(32*t5+T+N+28,32)),8*t3+7);t6++) {
              for (t7=max(max(max(32*t4,32*t5-N+2),16*t6-N+2),-32*t5+32*t6-N-29);t7<=min(min(min(min(T-1,32*t4+31),32*t5+30),16*t6+14),-32*t5+32*t6+30);t7++) {
                for (t8=max(max(32*t5,t7+1),32*t6-t7-N+2);t8<=min(min(32*t5+31,32*t6-t7+30),t7+N-2);t8++) {
                  for (t9=max(32*t6,t7+t8+1);t9<=min(32*t6+31,t7+t8+N-2);t9++) {
                    a[((-t7+t8))*N + (-t7-t8+t9)] = (a[((-t7+t8)-1)*N + (-t7-t8+t9)-1] + a[((-t7+t8)-1)*N + (-t7-t8+t9)] + a[((-t7+t8)-1)*N + (-t7-t8+t9)+1] + a[((-t7+t8))*N + (-t7-t8+t9)-1] + a[((-t7+t8))*N + (-t7-t8+t9)] + a[((-t7+t8))*N + (-t7-t8+t9)+1] + a[((-t7+t8)+1)*N + (-t7-t8+t9)-1] + a[((-t7+t8)+1)*N + (-t7-t8+t9)] + a[((-t7+t8)+1)*N + (-t7-t8+t9)+1])/9.0;;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}
/* End of CLooG code */

}

int main(int argc, char** argv)
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
  psgProtect(a, (long long) &a[0], (long long) &a[(n*n) - 1]);

  /* Initialize array */

  init_array(a,n);
  gettimeofday(&start, NULL);
  func(a,n,tsteps);
  gettimeofday(&end,NULL);

  printf("Total time taken to execute the kernel: %lf seconds\n", (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000);
  runtime=(double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000;


	if(argc>=4)
	{
		print_data_2d(n,n,a,argv[3],1);
	}

   if(argc>=5) writeOVRData(argv[4],runtime,0);
   if(argc>=6) writeFPRData(argv[5],detectCounter,0);
	

	if(detectCounter)
		printf("\nInfo: Soft error detected\n");
	else
		printf("\nInfo: No Soft Errors detected\n");

	free(a);

	#ifdef INST
		printFaultSitesData();
		printFaultInjectionData();
	#endif
	

}

