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
void init_array(double *ex, double *ey, double *hz, int nx, int ny)
{
    int i, j;

    for (i=0; i<nx+1; i++)  {
        for (j=0; j<ny; j++)  {
            ey[(i)*(nx+1) + j] = 0;
        }
    }

    for (i=0; i<nx; i++)  {
        for (j=0; j<ny+1; j++)  {
            ex[(i)*nx + j] = 0;
        }
    }

    for (j=0; j<ny; j++)  {
        ey[(0)*(nx+1) + j] = ((double)j)/ny;
    }

    for (i=0; i<nx; i++)    {
        for (j=0; j<ny; j++)  {
            hz[(i)*nx + j] = 0;
        }
    }
}

static 
void func(double *ex, double *ey, double *hz, int nx, int ny, int tmax)
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
  int t1, t2, t3;
 register int lbv, ubv;
/* Start of CLooG code */
if ((ny >= 1) && (tmax >= 1)) {
  if ((nx >= 2) && (ny >= 2)) {
    for (t1=0;t1<=tmax-1;t1++) {
      ey[(0)*(nx+1) + 0] = t1;;
      for (t3=t1+1;t3<=t1+ny-1;t3++) {
        ey[(0)*(nx+1) + (-t1+t3)] = t1;;
        ex[(0)*nx + (-t1+t3)] = ex[(0)*nx + (-t1+t3)] - 0.5*(hz[(0)*nx + (-t1+t3)]-hz[(0)*nx + (-t1+t3)-1]);;
      }
      for (t2=t1+1;t2<=t1+nx-1;t2++) {
        ey[((-t1+t2))*(nx+1) + 0] = ey[((-t1+t2))*(nx+1) + 0] - 0.5*(hz[((-t1+t2))*nx + 0]-hz[((-t1+t2)-1)*nx + 0]);;
        for (t3=t1+1;t3<=t1+ny-1;t3++) {
          ey[((-t1+t2))*(nx+1) + (-t1+t3)] = ey[((-t1+t2))*(nx+1) + (-t1+t3)] - 0.5*(hz[((-t1+t2))*nx + (-t1+t3)]-hz[((-t1+t2)-1)*nx + (-t1+t3)]);;
          ex[((-t1+t2))*nx + (-t1+t3)] = ex[((-t1+t2))*nx + (-t1+t3)] - 0.5*(hz[((-t1+t2))*nx + (-t1+t3)]-hz[((-t1+t2))*nx + (-t1+t3)-1]);;
          hz[((-t1+t2-1))*nx + (-t1+t3-1)]=hz[((-t1+t2-1))*nx + (-t1+t3-1)]-0.7*(ex[((-t1+t2-1))*nx + (-t1+t3-1)+1]-ex[((-t1+t2-1))*nx + (-t1+t3-1)]+ey[((-t1+t2-1)+1)*(nx+1) + (-t1+t3-1)]-ey[((-t1+t2-1))*(nx+1) + (-t1+t3-1)]);;
        }
        hz[((-t1+t2-1))*nx + (ny-1)]=hz[((-t1+t2-1))*nx + (ny-1)]-0.7*(ex[((-t1+t2-1))*nx + (ny-1)+1]-ex[((-t1+t2-1))*nx + (ny-1)]+ey[((-t1+t2-1)+1)*(nx+1) + (ny-1)]-ey[((-t1+t2-1))*(nx+1) + (ny-1)]);;
      }
      for (t3=t1+1;t3<=t1+ny;t3++) {
        hz[((nx-1))*nx + (-t1+t3-1)]=hz[((nx-1))*nx + (-t1+t3-1)]-0.7*(ex[((nx-1))*nx + (-t1+t3-1)+1]-ex[((nx-1))*nx + (-t1+t3-1)]+ey[((nx-1)+1)*(nx+1) + (-t1+t3-1)]-ey[((nx-1))*(nx+1) + (-t1+t3-1)]);;
      }
    }
  }
  if ((nx >= 2) && (ny == 1)) {
    for (t1=0;t1<=tmax-1;t1++) {
      ey[(0)*(nx+1) + 0] = t1;;
      for (t2=t1+1;t2<=t1+nx-1;t2++) {
        ey[((-t1+t2))*(nx+1) + 0] = ey[((-t1+t2))*(nx+1) + 0] - 0.5*(hz[((-t1+t2))*nx + 0]-hz[((-t1+t2)-1)*nx + 0]);;
        hz[((-t1+t2-1))*nx + 0]=hz[((-t1+t2-1))*nx + 0]-0.7*(ex[((-t1+t2-1))*nx + 0 +1]-ex[((-t1+t2-1))*nx + 0]+ey[((-t1+t2-1)+1)*(nx+1) + 0]-ey[((-t1+t2-1))*(nx+1) + 0]);;
      }
      hz[((nx-1))*nx + 0]=hz[((nx-1))*nx + 0]-0.7*(ex[((nx-1))*nx + 0 +1]-ex[((nx-1))*nx + 0]+ey[((nx-1)+1)*(nx+1) + 0]-ey[((nx-1))*(nx+1) + 0]);;
    }
  }
  if ((nx == 1) && (ny >= 2)) {
    for (t1=0;t1<=tmax-1;t1++) {
      ey[(0)*(nx+1) + 0] = t1;;
      for (t3=t1+1;t3<=t1+ny-1;t3++) {
        ey[(0)*(nx+1) + (-t1+t3)] = t1;;
        ex[(0)*nx + (-t1+t3)] = ex[(0)*nx + (-t1+t3)] - 0.5*(hz[(0)*nx + (-t1+t3)]-hz[(0)*nx + (-t1+t3)-1]);;
      }
      for (t3=t1+1;t3<=t1+ny;t3++) {
        hz[(0)*nx + (-t1+t3-1)]=hz[(0)*nx + (-t1+t3-1)]-0.7*(ex[(0)*nx + (-t1+t3-1)+1]-ex[(0)*nx + (-t1+t3-1)]+ey[(0 +1)*(nx+1) + (-t1+t3-1)]-ey[(0)*(nx+1) + (-t1+t3-1)]);;
      }
    }
  }
  if ((nx <= 0) && (ny >= 2)) {
    for (t1=0;t1<=tmax-1;t1++) {
      for (t3=t1;t3<=t1+ny-1;t3++) {
        ey[(0)*(nx+1) + (-t1+t3)] = t1;;
      }
    }
  }
  if ((nx == 1) && (ny == 1)) {
    for (t1=0;t1<=tmax-1;t1++) {
      ey[(0)*(nx+1) + 0] = t1;;
      hz[(0)*nx + 0]=hz[(0)*nx + 0]-0.7*(ex[(0)*nx + 0 +1]-ex[(0)*nx + 0]+ey[(0 +1)*(nx+1) + 0]-ey[(0)*(nx+1) + 0]);;
    }
  }
  if ((nx <= 0) && (ny == 1)) {
    for (t1=0;t1<=tmax-1;t1++) {
      ey[(0)*(nx+1) + 0] = t1;;
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

  double *ex = (double*)malloc(n*(n+1)*sizeof(double));
  double *ey = (double*)malloc((n+1)*n*sizeof(double));
  double *hz = (double*)malloc(n*n*sizeof(double));

  psgProtect(ex, (long long) &ex[0], (long long) &ex[(n*(n+1)) - 1]);
  psgProtect(ey, (long long) &ex[0], (long long) &ey[((n+1)*n) - 1]);
  psgProtect(hz, (long long) &ex[0], (long long) &hz[(n*n) - 1]);

/* Initialize array */

  init_array(ex, ey, hz, n, n);
  gettimeofday(&start, NULL);
  func(ex, ey, hz, n, n, tsteps);
  gettimeofday(&end,NULL);

  printf("Total time taken to execute the kernel: %lf seconds\n", (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000);
  runtime=(double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000;


	if(argc>=4)
	{
		print_data_2d(n,n+1,ex,argv[3],1);
		print_data_2d(n+1,n,ey,argv[3],0);
		print_data_2d(n,n,hz,argv[3],0);
	}

   if(argc>=5) writeOVRData(argv[4],runtime,0);
   if(argc>=6) writeFPRData(argv[5],detectCounter,0);
	

	if(detectCounter)
		printf("\nInfo: Soft error detected\n");
	else
		printf("\nInfo: No Soft Errors detected\n");

	free(ex);
	free(ey);
	free(hz);

	#ifdef INST
		printFaultSitesData();
		printFaultInjectionData();
	#endif
	


}
