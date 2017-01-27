
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

void setLink(char* c, int p01, int p02, int p0e, int p11, int p12, int p1e, int p21, int p22, int p2e, int p31, int p32, int p3e) {
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
  if (n < 2) {
//-- L1
  setLink("L1", n,2,1,0,0,0,0,0,0,0,0,0);
    return;
  }
//-- L2
  setLink("L2", n,2,1,0,0,0,0,0,0,0,0,0);
  p = a[n / 2];
  for (i = 0, j = n - 1;; i++, j--) {
//-- L3
  setLink("L3", n,2,1,i,j,1,p,a[i],1,p,a[j],1);
    while (a[i] < p) {
//-- L4
  setLink("L4", n,2,1,i,j,1,p,a[i],1,p,a[j],1);
      i++;
    }
//-- L5
  setLink("L5", n,2,1,i,j,1,p,a[i],1,p,a[j],1);
    while (p < a[j]) {
//-- L6
  setLink("L6", n,2,1,i,j,1,p,a[i],1,p,a[j],1);
      j--;
    }
//-- L7
  setLink("L7", n,2,1,i,j,1,p,a[i],1,p,a[j],1);
    if (i >= j) {
//-- L8
  setLink("L8", n,2,1,i,j,1,p,a[i],1,p,a[j],1);
      break;
     }
//-- L9
  setLink("L9", n,2,1,i,j,1,p,a[i],1,p,a[j],1);
    t = a[i];
    a[i] = a[j];
    a[j] = t;
//-- L10
  setLink("L10", n,2,1,i,j,1,p,a[i],1,p,a[j],1);
  }
  func(a, i);
  func(a + i, n - i);
}


int main( int argc, char**  argv)
{

 double runtime=0.0 ;
 struct timeval start, end ;
 FILE *fp ;
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

   if(argc>=6) writeOVRData(argv[5],runtime,0);
   if(argc>=7) writeFPRData(argv[6],detectCounter,0);

   //--- Write The Collected PTG ---
   printf("Argc = %d\n",argc);
   if(argc>=5) {
   	fp = fopen(argv[4], "a");
   	fprintf(fp,"Starting Here\n");
   	while(head!=NULL) {
   	   fprintf(fp, "%s :", head->loc);
   	   for(int i=0; i<4; i++)
   	     fputc(48+head->pred[i], fp);
   	   fprintf(fp, "\n");
   	   head = head->link ;
   	}
   	fclose(fp);
   }
   //-------------------------------
	

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
