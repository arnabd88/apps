
/*
 * Source: http://rosettacode.org/mw/index.php?title=Sorting_algorithms/Quicksort&oldid=223058
 * 
 * License: content is available under GNU Free Documentation License 1.2 unless otherwise noted.
 * 
 * Please refer to GNU Free Documentation License 1.2 @ http://www.gnu.org/licenses/fdl-1.2.html
 * 
 * Please also refer to following link for additional disclaimer: 
 * http://rosettacode.org/wiki/Rosetta_Code:General_disclaimer
 * 
 * */


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

enum format_t { T=0, F=1, X=2 };

#ifdef INST
extern int printFaultSitesData(void);
extern int printFaultInjectionData(void);
#endif

static
void init_array(int *a, int n) {
	int lim = 200 ;
	for(int k=0; k<n; k++)
		a[k] = random()%lim ;
}


enum format_t p0(int n, int m, int enable) { enum format_t val = X ;
	if(enable==0) val = X ; else if(n < m) val = T ; else val = F ;
	return val ; }
enum format_t p1(int n, int m, int enable) { enum format_t val = X ;
	if(enable==0) val = X ; else if(n >= m) val = T ; else val = F ;
	return val ; }
enum format_t p2(int n, int m, int enable) { enum format_t val = X ;
	if(enable==0) val = X ; else if(n > m) val = T ; else val = F ;
	return val ; }
enum format_t p3(int n, int m, int enable) { enum format_t val = X ;
	if(enable==0) val = X ; else if(n < m) val = T ; else val = F ;
	return val ; }
// enum format_t p4(int n, int m, int enable) { enum format_t val = X ;
// 	if(enable==0) val = X ; else if(n <= m) val = T ; else val = F ;
// 	return val ; }

struct predicate {
 char *loc ;
 enum format_t pred[4] ;
 struct predicate* link ;
} ;


struct predicate* head ;
struct predicate* curr ;

void getNextLink()
{
  if(head==NULL) {
	head = (struct predicate*)malloc(sizeof(struct predicate));
	curr = head ;
  } else {
	struct predicate* local = (struct predicate*)malloc(sizeof(struct predicate));
	curr->link = local ;
	curr = local ;
  }
}

void setLink(char* c, p01,p02, p0e, p11, p12, p1e, p21, p22, p2e, p31, p32, p3e) {
	getNextLink() ;
	curr->loc = c ;
  	curr->pred[0] = p0(p01,p02,p0e) ; 
  	curr->pred[1] = p1(p11,p12,p1e) ; 
  	curr->pred[2] = p2(p21,p22,p2e) ; 
  	curr->pred[3] = p3(p31,p22,p3e) ; 
	
}

static
void func(int *a, int n) {
  int i, j, p, t;
//-- L0: 
  setLink("L0", n,2,0,0,0,0,0,0,0,0,0,0);
  // getNextLink() ;
  // curr->loc = "L0" ;
  // curr->pred[0] = p0(n,2,0) ; curr->pred[1] = p1(0,0,0) ; curr->pred[2] = p2(0,0,0) ; curr->pred[3] = p3(0,0,0) ; 
  if (n < 2) {
//-- L1
  getNextLink() ;
  curr->loc = "L1" ;
  curr->pred[0] = p0(n,2,1) ; curr->pred[1] = p1(0,0,0) ; curr->pred[2] = p2(0,0,0) ; curr->pred[3] = p3(0,0,0) ;
    return;
  }
//-- L2
  getNextLink() ;
  curr->loc = "L2" ;
  curr->pred[0] = p0(n,2,1) ; curr->pred[1] = p1(0,0,0) ; curr->pred[2] = p2(0,0,0) ; curr->pred[3] = p3(0,0,0) ;
  p = a[n / 2];
  for (i = 0, j = n - 1;; i++, j--) {
//-- L3
  getNextLink() ;
  curr->loc = "L3" ;
  curr->pred[0] = p0(n,2,1) ; curr->pred[1] = p1(i,j,1) ; curr->pred[2] = p2(p,a[i],1) ; curr->pred[3] = p3(p,a[j],1) ;
    while (a[i] < p) {
//-- L4
  getNextLink() ;
  curr->loc = "L4" ;
  curr->pred[0] = p0(n,2,1) ; curr->pred[1] = p1(i,j,1) ; curr->pred[2] = p2(p,a[i],1) ; curr->pred[3] = p3(p,a[j],1) ; 
      i++;
    }
//-- L5
  getNextLink() ;
  curr->loc = "L5" ;
  curr->pred[0] = p0(n,2,1) ; curr->pred[1] = p1(i,j,1) ; curr->pred[2] = p2(p,a[i],1) ; curr->pred[3] = p3(p,a[j],1) ;
    while (p < a[j]) {
//-- L6
  getNextLink() ;
  curr->loc = "L6" ;
  curr->pred[0] = p0(n,2,1) ; curr->pred[1] = p1(i,j,1) ; curr->pred[2] = p2(p,a[i],1) ; curr->pred[3] = p3(p,a[j],1) ;
      j--;
    }
//-- L7
  getNextLink() ;
  curr->loc = "L7" ;
  curr->pred[0] = p0(n,2,1) ; curr->pred[1] = p1(i,j,1) ; curr->pred[2] = p2(p,a[i],1) ; curr->pred[3] = p3(p,a[j],1) ;
    if (i >= j) {
//-- L8
  getNextLink() ;
  curr->loc = "L8" ;
  curr->pred[0] = p0(n,2,1) ; curr->pred[1] = p1(i,j,1) ; curr->pred[2] = p2(p,a[i],1) ; curr->pred[3] = p3(p,a[j],1) ;
      break;
     }
//-- L9
  getNextLink() ;
  curr->loc = "L9" ;
  curr->pred[0] = p0(n,2,1) ; curr->pred[1] = p1(i,j,1) ; curr->pred[2] = p2(p,a[i],1) ; curr->pred[3] = p3(p,a[j],1) ;
    t = a[i];
    a[i] = a[j];
    a[j] = t;
//-- L10
  getNextLink() ;
  curr->loc = "L10" ;
  curr->pred[0] = p0(n,2,1) ; curr->pred[1] = p1(i,j,1) ; curr->pred[2] = p2(p,a[i],1) ; curr->pred[3] = p3(p,a[j],1) ;
  }
  func(a, i);
  func(a + i, n - i);
}


int main( int argc, char**  argv)
{

 double runtime=0.0 ;
 struct timeval start, end ;
 if(argc < 3)
	printf("\nInfo: Insufficient arguments!\n\n");

  int n = atoi(argv[1]);
  int tsteps = atoi(argv[2]);

  dimcount=1;
  psgdim[0] = 0;
  dim0 = n;

  int *a = (int*)malloc(n*sizeof(int));
  psgProtect(a, (long long) &a[0], (long long) &a[n-1]);

  /* Initialize array */
  init_array(a, n);
  gettimeofday(&start, NULL);
  func(a, n);
  gettimeofday(&end, NULL);

  printf("Total time taken to execute the kernel: %lf seconds\n", (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000);
  runtime=(double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/(double)1000000;


	if(argc>=4)
	{
		print_data_1d(n,a,argv[3],1);
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
	
  return 0;



}
 
// int main (int argc, char *argv[]) {
//   if(argc!=2){    
//     printf("\nERROR: Insufficient arguments!\n");
//     exit(-1);      
//   }
//   FILE *fp;
//   fp=fopen(argv[1],"w");
//   int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};
//   int n = sizeof a / sizeof a[0];
//   int i;
//   for (i = 0; i < n; i++){
// #ifdef DEBUG
//     printf("%d%s", a[i], i == n - 1 ? "\n" : " ");
// #endif
//   }
//   quick_sort(a, n);
//   for (i = 0; i < n; i++){
// #ifdef DEBUG
//     printf("%d%s", a[i], i == n - 1 ? "\n" : " ");
// #endif    
//     fprintf(fp,"%d\n",a[i]);    
//   }
// #ifdef INST
//   printFaultSitesData();
//   printFaultInjectionData();
// #endif
//   fclose(fp);
//   return 0;
// }
