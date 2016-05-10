#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <picodrv.h>
#include <pico_errors.h>
#include <sys/time.h>
#include <pthread.h>
#include <cmath>
#include <locale.h>

using namespace std;
// #define SORT_RADIX
// #define SORT_RADIX2
#define SORT_RADIX3

#define SIZE 4096

#define VERBOSE
#define VERBOSE_THREAD
#define SWSIZE  SIZE
#include "jit_isa.h"

typedef struct {
  int key;
  int len;
  int *A;
  int *B;
  int *C;
}pe_t;

typedef struct {
  int          task_id;
  vam_vm_t     *VM;
  vector<int>  *nPR;

  int         len;

  int        *PR0_In1;
  int     SizePR0_In1;
  int        *PR0_In2;
  int     SizePR0_In2;
  int        *PR0_Out;
  int     SizePR0_Out;

  int        *PR1_In1;
  int     SizePR1_In1;
  int        *PR1_In2;
  int     SizePR1_In2;
  int        *PR1_Out;
  int     SizePR1_Out;

  int        *PR2_In1;
  int     SizePR2_In1;
  int        *PR2_In2;
  int     SizePR2_In2;
  int        *PR2_Out;
  int     SizePR2_Out;

  int        *PR3_In1;
  int     SizePR3_In1;
  int        *PR3_In2;
  int     SizePR3_In2;
  int        *PR3_Out;
  int     SizePR3_Out;

  int        *PR4_In1;
  int     SizePR4_In1;
  int        *PR4_In2;
  int     SizePR4_In2;
  int        *PR4_Out;
  int     SizePR4_Out;

  int        *PR5_In1;
  int     SizePR5_In1;
  int        *PR5_In2;
  int     SizePR5_In2;
  int        *PR5_Out;
  int     SizePR5_Out;

  int        *PR6_In1;
  int     SizePR6_In1;
  int        *PR6_In2;
  int     SizePR6_In2;
  int        *PR6_Out;
  int     SizePR6_Out;

  int        *PR7_In1;
  int     SizePR7_In1;
  int        *PR7_In2;
  int     SizePR7_In2;
  int        *PR7_Out;
  int     SizePR7_Out;

  int        *PR8_In1;
  int     SizePR8_In1;
  int        *PR8_In2;
  int     SizePR8_In2;
  int        *PR8_Out;
  int     SizePR8_Out;

  int        *PR9_In1;
  int     SizePR9_In1;
  int        *PR9_In2;
  int     SizePR9_In2;
  int        *PR9_Out;
  int     SizePR9_Out;

  int        *PR10_In1;
  int     SizePR10_In1;
  int        *PR10_In2;
  int     SizePR10_In2;
  int        *PR10_Out;
  int     SizePR10_Out;

  int        *PR11_In1;
  int     SizePR11_In1;
  int        *PR11_In2;
  int     SizePR11_In2;
  int        *PR11_Out;
  int     SizePR11_Out;

  int        *PR12_In1;
  int     SizePR12_In1;
  int        *PR12_In2;
  int     SizePR12_In2;
  int        *PR12_Out;
  int     SizePR12_Out;

  int        *PR13_In1;
  int     SizePR13_In1;
  int        *PR13_In2;
  int     SizePR13_In2;
  int        *PR13_Out;
  int     SizePR13_Out;

  int        *PR14_In1;
  int     SizePR14_In1;
  int        *PR14_In2;
  int     SizePR14_In2;
  int        *PR14_Out;
  int     SizePR14_Out;

}task_pk_t;

void * Threads_Call(void *pk)
{
#ifdef VERBOSE_THREAD
  printf("\r\n");
#endif

  struct timeval start, end;
  int timeuse;

  task_pk_t *p      = (task_pk_t*) pk;
#ifdef VERBOSE_THREAD
  printf("[DEBUG->task_thread:%2d] task thread start\r\n", p->task_id);
#endif
  vam_vm_t    *VM      = p->VM;
  vector<int> *nPR     = p->nPR;
  int         err;

  #ifdef SORT_RADIX
    err =   vnew(VM, nPR);                                                                                                errCheck(err, FUN_VNEW);
    err =   vlpr(VM, nPR->at(0), RADIX);                                                                                   errCheck(err, FUN_VLPR);
    err = vtieio(VM, nPR->at(0), p->PR0_In1, p->SizePR0_In1, p->PR0_In2, p->SizePR0_In2, p->PR0_Out, p->SizePR0_Out);     errCheck(err, FUN_VTIEIO);
    err = vstart(VM, nPR);                                                                                                errCheck(err, FUN_VSTART);
    err =   vdel(VM, nPR);
  #endif

  #ifdef SORT_RADIX2
    err =   vnew(VM, nPR);                                                                                                errCheck(err, FUN_VNEW);

    err =   vlpr(VM, nPR->at(0),  RADIX);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(1),  RADIX);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(2),  RADIX);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(3),  RADIX);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(4),  MERGE);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(5),  MERGE);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(6),  MERGE);                                                                                 errCheck(err, FUN_VLPR);

    err = vtieio(VM, nPR->at(0),  p->PR0_In1, p->SizePR0_In1, p->PR0_In2, p->SizePR0_In2, nPR->at(4), p->SizePR0_Out);    errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(1),  p->PR1_In1, p->SizePR1_In1, p->PR1_In2, p->SizePR1_In2, nPR->at(4), p->SizePR1_Out);    errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(2),  p->PR2_In1, p->SizePR2_In1, p->PR2_In2, p->SizePR2_In2, nPR->at(5), p->SizePR2_Out);    errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(3),  p->PR3_In1, p->SizePR3_In1, p->PR3_In2, p->SizePR3_In2, nPR->at(5), p->SizePR3_Out);    errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(4),  nPR->at(0), p->SizePR4_In1, nPR->at(1), p->SizePR4_In2, nPR->at(6), p->SizePR4_Out);    errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(5),  nPR->at(2), p->SizePR5_In1, nPR->at(3), p->SizePR5_In2, nPR->at(6), p->SizePR5_Out);    errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(6),  nPR->at(4), p->SizePR6_In1, nPR->at(5), p->SizePR6_In2, p->PR6_Out, p->SizePR6_Out);    errCheck(err, FUN_VTIEIO);

    err = vstart(VM, nPR);                                                                                                errCheck(err, FUN_VSTART);
    err =   vdel(VM, nPR);                                                                                                errCheck(err, FUN_VDEL);
  #endif

  #ifdef SORT_RADIX3
    err =   vnew(VM, nPR);                                                                                                errCheck(err, FUN_VNEW);

    err =   vlpr(VM, nPR->at(0),  RADIX);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(1),  RADIX);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(2),  RADIX);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(3),  RADIX);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(4),  MERGE);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(5),  MERGE);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(6),  MERGE);                                                                                 errCheck(err, FUN_VLPR);

    err =   vlpr(VM, nPR->at(7),  RADIX);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(8),  RADIX);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(9),  RADIX);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(10), RADIX);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(11), MERGE);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(12), MERGE);                                                                                 errCheck(err, FUN_VLPR);
    err =   vlpr(VM, nPR->at(13), MERGE);                                                                                 errCheck(err, FUN_VLPR);

    err =   vlpr(VM, nPR->at(14), MERGE);                                                                                 errCheck(err, FUN_VLPR);

    err = vtieio(VM, nPR->at(0),  p->PR0_In1, p->SizePR0_In1, p->PR0_In2, p->SizePR0_In2, nPR->at(4), p->SizePR0_Out);    errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(1),  p->PR1_In1, p->SizePR1_In1, p->PR1_In2, p->SizePR1_In2, nPR->at(4), p->SizePR1_Out);    errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(2),  p->PR2_In1, p->SizePR2_In1, p->PR2_In2, p->SizePR2_In2, nPR->at(5), p->SizePR2_Out);    errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(3),  p->PR3_In1, p->SizePR3_In1, p->PR3_In2, p->SizePR3_In2, nPR->at(5), p->SizePR3_Out);    errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(4),  nPR->at(0), p->SizePR4_In1, nPR->at(1), p->SizePR4_In2, nPR->at(6), p->SizePR4_Out);    errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(5),  nPR->at(2), p->SizePR5_In1, nPR->at(3), p->SizePR5_In2, nPR->at(6), p->SizePR5_Out);    errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(6),  nPR->at(4), p->SizePR6_In1, nPR->at(5), p->SizePR6_In2, nPR->at(14),p->SizePR6_Out);    errCheck(err, FUN_VTIEIO);

    err = vtieio(VM, nPR->at(7),  p->PR7_In1, p->SizePR7_In1, p->PR7_In2, p->SizePR7_In2, nPR->at(11), p->SizePR7_Out);   errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(8),  p->PR8_In1, p->SizePR8_In1, p->PR8_In2, p->SizePR8_In2, nPR->at(11), p->SizePR8_Out);   errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(9),  p->PR9_In1, p->SizePR9_In1, p->PR9_In2, p->SizePR9_In2, nPR->at(12), p->SizePR9_Out);   errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(10), p->PR10_In1,p->SizePR10_In1,p->PR10_In2,p->SizePR10_In2,nPR->at(12), p->SizePR10_Out);  errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(11), nPR->at(7), p->SizePR11_In1,nPR->at(8), p->SizePR11_In2,nPR->at(13), p->SizePR11_Out);  errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(12), nPR->at(9), p->SizePR12_In1,nPR->at(10),p->SizePR12_In2,nPR->at(13), p->SizePR12_Out);  errCheck(err, FUN_VTIEIO);
    err = vtieio(VM, nPR->at(13), nPR->at(11),p->SizePR13_In1,nPR->at(12),p->SizePR13_In2,nPR->at(14), p->SizePR13_Out);  errCheck(err, FUN_VTIEIO);

    err = vtieio(VM, nPR->at(14), nPR->at(6), p->SizePR14_In1,nPR->at(13),p->SizePR14_In2,p->PR14_Out, p->SizePR14_Out);  errCheck(err, FUN_VTIEIO);

    err = vstart(VM, nPR);                                                                                                errCheck(err, FUN_VSTART);
    err =   vdel(VM, nPR);                                                                                                errCheck(err, FUN_VDEL);
  #endif

#ifdef VERBOSE_THREAD
  printf("[DEBUG->task_thread:%2d] Thread Done\r\n", p->task_id);
#endif
  return NULL;
}

void InsertionSort(int array[], int len)
{
  // L1:
  int i, j, index;
  for (i = 1; i < len; i++)
  {
    // L2:
    index = array[i];
    j = i;
    while ( (j > 0) && (array[j-1] > index) ) {
      // L3:
      array[j] = array[j - 1];
      j--;
    }
    array[j] = index;
  }
}

// Function to Merge Arrays L and R into A.
// lefCount = number of elements in L
// rightCount = number of elements in R.
void Merge(int *A,int *L,int leftCount,int *R,int rightCount) {
  int i,j,k;

  // i - to mark the index of left aubarray (L)
  // j - to mark the index of right sub-raay (R)
  // k - to mark the index of merged subarray (A)
  i = 0; j = 0; k =0;

  while(i<leftCount && j< rightCount) {
    if(L[i]  < R[j]) A[k++] = L[i++];
    else A[k++] = R[j++];
  }
  while(i < leftCount) A[k++] = L[i++];
  while(j < rightCount) A[k++] = R[j++];
}

// Recursive function to sort an array of integers.
void MergeSort(int *A, int n) {
  int mid,i, *L, *R;
  if(n < 2) return; // base condition. If the array has less than two element, do nothing.

  mid = n/2;  // find the mid index.

  // create left and right subarrays
  // mid elements (from index 0 till mid-1) should be part of left sub-array
  // and (n-mid) elements (from mid to n-1) will be part of right sub-array
  L = (int*)malloc(mid*sizeof(int));
  R = (int*)malloc((n- mid)*sizeof(int));

  for(i = 0;i<mid;i++) L[i] = A[i]; // creating left subarray
  for(i = mid;i<n;i++) R[i-mid] = A[i]; // creating right subarray

  MergeSort(L,mid);  // sorting the left subarray
  MergeSort(R,n-mid);  // sorting the right subarray
  Merge(A,L,mid,R,n-mid);  // Merging L and R into A as sorted list.
  free(L);
  free(R);
}

int main(int argc, char* argv[])
{
  printf("Begin...\r\n");
  setlocale(LC_NUMERIC, ""); // for thounds seperator
  printf("%'d\r\n", SIZE);

  struct timeval start, end;
  int timeuse;

  int i;
  int j;
  int sum;


  #ifdef SORT_RADIX

    int *A = new int[SWSIZE * 2];
    int *B = new int[SWSIZE * 2];
    int *C = new int[SWSIZE * 2];
    int *D = new int[SWSIZE * 2];
    int *E = new int[SWSIZE * 2];
    int *F = new int[SWSIZE * 2];
    int *G = new int[SWSIZE * 2];
    int *H = new int[SWSIZE * 2];
    int *I = new int[SWSIZE * 2];
    int *J = new int[SWSIZE * 2];
    int *K = new int[SWSIZE * 2];

    for (i = 0; i < SWSIZE * 2; i++) {
      A[i] = -2;
      B[i] = -2;
      C[i] = -2;
      D[i] = -2;
      E[i] = -2;
      F[i] = -2;
      G[i] = -2;
      H[i] = -2;
      I[i] = -2;
      J[i] = -2;
      K[i] = -2;
    }

    for (i = 0; i < SWSIZE; i++) {
      A[i] = SWSIZE * 2 - 0 - i;
    }

    for (i = SWSIZE; i < SWSIZE * 2; i++) {
      A[i] = 999999;
    }

    for (i = 0; i < SWSIZE; i++) {
      K[i] = SWSIZE * 2 - 0 - i;
    }


  #endif


  #ifdef SORT_RADIX2
    int *A = new int[SWSIZE * 2 * 4];
    int *B = new int[SWSIZE * 2 * 4];
    int *C = new int[SWSIZE * 2 * 4];
    int *D = new int[SWSIZE * 2 * 4];
    int *E = new int[SWSIZE * 2 * 4];
    int *F = new int[SWSIZE * 2 * 4];
    int *G = new int[SWSIZE * 2 * 4];
    int *H = new int[SWSIZE * 2 * 4];
    int *I = new int[SWSIZE * 2 * 4];
    int *J = new int[SWSIZE * 2 * 4];
    int *K = new int[SWSIZE * 2 * 4];

    for (i = 0; i < SWSIZE * 2 * 4; i++) {
      A[i] = -2;
      B[i] = -2;
      C[i] = -2;
      D[i] = -2;
      E[i] = -2;
      F[i] = -2;
      G[i] = -2;
      H[i] = -2;
      I[i] = -2;
      J[i] = -2;
      K[i] = -2;
    }

    for (i = 0; i < SWSIZE; i++) {
      A[i] = SWSIZE * 4 * 2 - 0 - i * 4;
      B[i] = SWSIZE * 4 * 2 - 1 - i * 4;
      C[i] = SWSIZE * 4 * 2 - 2 - i * 4;
      D[i] = SWSIZE * 4 * 2 - 3 - i * 4;
    }

    for (i = SWSIZE; i < SWSIZE * 2; i++) {
      A[i] = 999999;
      B[i] = 999999;
      C[i] = 999999;
      D[i] = 999999;
    }

    for (i = 0; i < SWSIZE * 4; i++) {
      K[i] = SWSIZE * 4 * 2 - 0 - i;
    }
  #endif

  #ifdef SORT_RADIX3
    int *A = new int[SWSIZE * 8];
    int *B = new int[SWSIZE * 8];
    int *C = new int[SWSIZE * 8];
    int *D = new int[SWSIZE * 8];
    int *E = new int[SWSIZE * 8];
    int *F = new int[SWSIZE * 8];
    int *G = new int[SWSIZE * 8];
    int *H = new int[SWSIZE * 8];
    int *I = new int[SWSIZE * 8];
    int *J = new int[SWSIZE * 8];
    int *K = new int[SWSIZE * 8];

    for (i = 0; i < SWSIZE * 8; i++) {
      A[i] = -2;
      B[i] = -2;
      C[i] = -2;
      D[i] = -2;
      E[i] = -2;
      F[i] = -2;
      G[i] = -2;
      H[i] = -2;
      I[i] = -2;
      J[i] = -2;
      K[i] = -2;
    }

    for (i = 0; i < SWSIZE; i++) {
      A[i] = SWSIZE * 8 * 2 - 0 - i * 8;
      B[i] = SWSIZE * 8 * 2 - 1 - i * 8;
      C[i] = SWSIZE * 8 * 2 - 2 - i * 8;
      D[i] = SWSIZE * 8 * 2 - 3 - i * 8;
      E[i] = SWSIZE * 8 * 2 - 4 - i * 8;
      F[i] = SWSIZE * 8 * 2 - 5 - i * 8;
      G[i] = SWSIZE * 8 * 2 - 6 - i * 8;
      H[i] = SWSIZE * 8 * 2 - 7 - i * 8;
    }

    for (i = SWSIZE; i < SWSIZE * 2; i++) {
      A[i] = 999999;
      B[i] = 999999;
      C[i] = 999999;
      D[i] = 999999;
      E[i] = 999999;
      F[i] = 999999;
      G[i] = 999999;
      H[i] = 999999;
    }

    for (i = 0; i < SWSIZE * 8; i++) {
      K[i] = SWSIZE * 8 * 2 - 0 - i;
    }
  #endif
  //////////////////////////////////////////////////////////////////////////////
  gettimeofday(&start, NULL);

  #ifdef SORT_RADIX
    InsertionSort(K, SWSIZE);
  #endif

  #ifdef SORT_RADIX2
    // InsertionSort(K, SWSIZE * 4);
    MergeSort(K, SWSIZE * 4);
  #endif

  #ifdef SORT_RADIX3
    // InsertionSort(K, SWSIZE * 8);
    MergeSort(K, SWSIZE * 8);
  #endif

  gettimeofday(&end, NULL);
  timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  printf("CPU %'4d threads :\t%'9d us\r\n", 1, timeuse);
  //////////////////////////////////////////////////////////////////////////////
  int err;
  vam_vm_t VM;
  VM.VAM_TABLE = NULL;
  VM.BITSTREAM_TABLE = NULL;
  VAM_VM_INIT(&VM, argc, argv);
  // VAM_TABLE_SHOW(VM);
  printf("[DEBUG->VAM_VM_INIT] VM_INIT Done\r\n");
  //////////////////////////////////////////////////////////////////////////////
  // VM.VAM_TABLE->at(0).status  = 1;
  // VM.VAM_TABLE->at(1).status  = 1;
  // VM.VAM_TABLE->at(2).status  = 1;
  // VM.VAM_TABLE->at(3).status  = 1;
  // VM.VAM_TABLE->at(4).status  = 1;
  // VM.VAM_TABLE->at(5).status  = 1;
  // VM.VAM_TABLE->at(6).status  = 1;
  VM.VAM_TABLE->at(7).status  = 1;

  // VM.VAM_TABLE->at(8).status  = 0;
  // VM.VAM_TABLE->at(9).status  = 0;
  // VM.VAM_TABLE->at(10).status = 0;
  // VM.VAM_TABLE->at(11).status = 0;
  // VM.VAM_TABLE->at(12).status = 0;
  // VM.VAM_TABLE->at(13).status = 0;
  // VM.VAM_TABLE->at(14).status = 0;
  // VM.VAM_TABLE->at(15).status = 0;
  //////////////////////////////////////////////////////////////////////////////

  {
    int THREADS = 1;

    #ifdef SORT_RADIX
      int STEPS = 1;
    #endif

    #ifdef SORT_RADIX2
      int STEPS   = 7;
    #endif

    #ifdef SORT_RADIX3
      int STEPS   = 15;
    #endif

    pthread_t thread[THREADS];
    task_pk_t task_pkg[THREADS];

    vector<vector<int> > nPR(THREADS);
    for (i = 0; i < THREADS; i++) {
      nPR[i].resize(STEPS);
    }

    for (i = 0; i < THREADS; i++) {
      task_pkg[i].task_id = i;
      task_pkg[i].VM      = &VM;
      task_pkg[i].nPR     = &nPR[i];
      task_pkg[i].len     = SIZE / THREADS;

      #ifdef SORT_RADIX
        task_pkg[i].PR0_In1      = A;
        task_pkg[i].SizePR0_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR0_In2      = NULL;
        task_pkg[i].SizePR0_In2  = 0;
        task_pkg[i].PR0_Out      = J;
        task_pkg[i].SizePR0_Out  = task_pkg[i].len;
      #endif

      #ifdef SORT_RADIX2
        task_pkg[i].PR0_In1      = A;
        task_pkg[i].SizePR0_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR0_In2      = NULL;
        task_pkg[i].SizePR0_In2  = 0;
        task_pkg[i].PR0_Out      = NULL;
        task_pkg[i].SizePR0_Out  = task_pkg[i].len;

        task_pkg[i].PR1_In1      = B;
        task_pkg[i].SizePR1_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR1_In2      = NULL;
        task_pkg[i].SizePR1_In2  = 0;
        task_pkg[i].PR1_Out      = NULL;
        task_pkg[i].SizePR1_Out  = task_pkg[i].len;

        task_pkg[i].PR2_In1      = C;
        task_pkg[i].SizePR2_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR2_In2      = NULL;
        task_pkg[i].SizePR2_In2  = 0;
        task_pkg[i].PR2_Out      = NULL;
        task_pkg[i].SizePR2_Out  = task_pkg[i].len;

        task_pkg[i].PR3_In1      = D;
        task_pkg[i].SizePR3_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR3_In2      = NULL;
        task_pkg[i].SizePR3_In2  = 0;
        task_pkg[i].PR3_Out      = NULL;
        task_pkg[i].SizePR3_Out  = task_pkg[i].len;

        task_pkg[i].PR4_In1      = NULL;
        task_pkg[i].SizePR4_In1  = task_pkg[i].len;
        task_pkg[i].PR4_In2      = NULL;
        task_pkg[i].SizePR4_In2  = task_pkg[i].len;
        task_pkg[i].PR4_Out      = NULL;
        task_pkg[i].SizePR4_Out  = task_pkg[i].len * 2;

        task_pkg[i].PR5_In1      = NULL;
        task_pkg[i].SizePR5_In1  = task_pkg[i].len;
        task_pkg[i].PR5_In2      = NULL;
        task_pkg[i].SizePR5_In2  = task_pkg[i].len;
        task_pkg[i].PR5_Out      = NULL;
        task_pkg[i].SizePR5_Out  = task_pkg[i].len * 2;

        task_pkg[i].PR6_In1      = NULL;
        task_pkg[i].SizePR6_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR6_In2      = NULL;
        task_pkg[i].SizePR6_In2  = task_pkg[i].len * 2;
        task_pkg[i].PR6_Out      = J;
        task_pkg[i].SizePR6_Out  = task_pkg[i].len * 2 * 2;
      #endif

      #ifdef SORT_RADIX3
        task_pkg[i].PR0_In1      = A;
        task_pkg[i].SizePR0_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR0_In2      = NULL;
        task_pkg[i].SizePR0_In2  = 0;
        task_pkg[i].PR0_Out      = NULL;
        task_pkg[i].SizePR0_Out  = task_pkg[i].len;

        task_pkg[i].PR1_In1      = B;
        task_pkg[i].SizePR1_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR1_In2      = NULL;
        task_pkg[i].SizePR1_In2  = 0;
        task_pkg[i].PR1_Out      = NULL;
        task_pkg[i].SizePR1_Out  = task_pkg[i].len;

        task_pkg[i].PR2_In1      = C;
        task_pkg[i].SizePR2_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR2_In2      = NULL;
        task_pkg[i].SizePR2_In2  = 0;
        task_pkg[i].PR2_Out      = NULL;
        task_pkg[i].SizePR2_Out  = task_pkg[i].len;

        task_pkg[i].PR3_In1      = D;
        task_pkg[i].SizePR3_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR3_In2      = NULL;
        task_pkg[i].SizePR3_In2  = 0;
        task_pkg[i].PR3_Out      = NULL;
        task_pkg[i].SizePR3_Out  = task_pkg[i].len;

        task_pkg[i].PR4_In1      = NULL;
        task_pkg[i].SizePR4_In1  = task_pkg[i].len;
        task_pkg[i].PR4_In2      = NULL;
        task_pkg[i].SizePR4_In2  = task_pkg[i].len;
        task_pkg[i].PR4_Out      = NULL;
        task_pkg[i].SizePR4_Out  = task_pkg[i].len * 2;

        task_pkg[i].PR5_In1      = NULL;
        task_pkg[i].SizePR5_In1  = task_pkg[i].len;
        task_pkg[i].PR5_In2      = NULL;
        task_pkg[i].SizePR5_In2  = task_pkg[i].len;
        task_pkg[i].PR5_Out      = NULL;
        task_pkg[i].SizePR5_Out  = task_pkg[i].len * 2;

        task_pkg[i].PR6_In1      = NULL;
        task_pkg[i].SizePR6_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR6_In2      = NULL;
        task_pkg[i].SizePR6_In2  = task_pkg[i].len * 2;
        task_pkg[i].PR6_Out      = NULL;
        task_pkg[i].SizePR6_Out  = task_pkg[i].len * 2 * 2;
        ///////////////////////////////////////////////////////
        task_pkg[i].PR7_In1      = E;
        task_pkg[i].SizePR7_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR7_In2      = NULL;
        task_pkg[i].SizePR7_In2  = 0;
        task_pkg[i].PR7_Out      = NULL;
        task_pkg[i].SizePR7_Out  = task_pkg[i].len;

        task_pkg[i].PR8_In1      = F;
        task_pkg[i].SizePR8_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR8_In2      = NULL;
        task_pkg[i].SizePR8_In2  = 0;
        task_pkg[i].PR8_Out      = NULL;
        task_pkg[i].SizePR8_Out  = task_pkg[i].len;

        task_pkg[i].PR9_In1      = G;
        task_pkg[i].SizePR9_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR9_In2      = NULL;
        task_pkg[i].SizePR9_In2  = 0;
        task_pkg[i].PR9_Out      = NULL;
        task_pkg[i].SizePR9_Out  = task_pkg[i].len;

        task_pkg[i].PR10_In1      = H;
        task_pkg[i].SizePR10_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR10_In2      = NULL;
        task_pkg[i].SizePR10_In2  = 0;
        task_pkg[i].PR10_Out      = NULL;
        task_pkg[i].SizePR10_Out  = task_pkg[i].len;

        task_pkg[i].PR11_In1      = NULL;
        task_pkg[i].SizePR11_In1  = task_pkg[i].len;
        task_pkg[i].PR11_In2      = NULL;
        task_pkg[i].SizePR11_In2  = task_pkg[i].len;
        task_pkg[i].PR11_Out      = NULL;
        task_pkg[i].SizePR11_Out  = task_pkg[i].len * 2;

        task_pkg[i].PR12_In1      = NULL;
        task_pkg[i].SizePR12_In1  = task_pkg[i].len;
        task_pkg[i].PR12_In2      = NULL;
        task_pkg[i].SizePR12_In2  = task_pkg[i].len;
        task_pkg[i].PR12_Out      = NULL;
        task_pkg[i].SizePR12_Out  = task_pkg[i].len * 2;

        task_pkg[i].PR13_In1      = NULL;
        task_pkg[i].SizePR13_In1  = task_pkg[i].len * 2;
        task_pkg[i].PR13_In2      = NULL;
        task_pkg[i].SizePR13_In2  = task_pkg[i].len * 2;
        task_pkg[i].PR13_Out      = NULL;
        task_pkg[i].SizePR13_Out  = task_pkg[i].len * 2 * 2;
        //////////////////////////////////////////////////////////////////////////
        task_pkg[i].PR14_In1      = NULL;
        task_pkg[i].SizePR14_In1  = task_pkg[i].len * 2 * 2;
        task_pkg[i].PR14_In2      = NULL;
        task_pkg[i].SizePR14_In2  = task_pkg[i].len * 2 * 2;
        task_pkg[i].PR14_Out      = J;
        task_pkg[i].SizePR14_Out  = task_pkg[i].len * 2 * 2 * 2;
      #endif
    }

    gettimeofday(&start, NULL);
    for (i = 0; i < THREADS; i++) {
      pthread_create(&thread[i], NULL, Threads_Call, (void *)&task_pkg[i]);
    }
    for (i = 0; i < THREADS; i++) {
      pthread_join(thread[i], NULL);
    }
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("JIT %'4d threads :\t%'9d us\t", THREADS, timeuse);

    printf("\r\n");

    #ifdef SORT_RADIX
      // for (i = 0; i < SWSIZE * 2; i++) {
      //   printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i, A[i], B[i], C[i], D[i], E[i], F[i], G[i], H[i], I[i], J[i], K[i]);
      // }

      for (i = 1; i < SWSIZE; i++) {
        //   printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i, A[i], B[i], C[i], D[i], E[i], F[i], G[i], H[i], I[i], J[i], K[i]);
        if (J[i] != K[i] || J[i] < J[i - 1]) {
          printf("Error at:%d\r\n", i);
          printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i - 1, A[i - 1], B[i - 1], C[i - 1], D[i - 1], E[i - 1], F[i - 1], G[i - 1], H[i - 1], I[i - 1], J[i - 1], K[i - 1]);
          printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i - 0, A[i - 0], B[i - 0], C[i - 0], D[i - 0], E[i - 0], F[i - 0], G[i - 0], H[i - 0], I[i - 0], J[i - 0], K[i - 0]);
          printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i + 1, A[i + 1], B[i + 1], C[i + 1], D[i + 1], E[i + 1], F[i + 1], G[i + 1], H[i + 1], I[i + 1], J[i + 1], K[i + 1]);
          VAM_VM_CLEAN(&VM);
          delete[] A;
          delete[] B;
          delete[] C;
          delete[] D;
          delete[] E;
          delete[] F;
          delete[] G;
          delete[] H;
          delete[] I;
          delete[] J;
          delete[] K;
          exit(1);
        }
      }
    #endif


    #ifdef SORT_RADIX2
      // for (i = 0; i < SWSIZE * 2 * 4; i++) {
      //   printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i, A[i], B[i], C[i], D[i], E[i], F[i], G[i], H[i], I[i], J[i], K[i]);
      // }

      for (i = 1; i < SWSIZE * 4; i++) {
        //   printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i, A[i], B[i], C[i], D[i], E[i], F[i], G[i], H[i], I[i], J[i], K[i]);
        if (J[0] != K[0] || J[i] < J[i - 1]) {
          printf("Error at:%d\r\n", i);
          printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i - 1, A[i - 1], B[i - 1], C[i - 1], D[i - 1], E[i - 1], F[i - 1], G[i - 1], H[i - 1], I[i - 1], J[i - 1], K[i - 1]);
          printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i - 0, A[i - 0], B[i - 0], C[i - 0], D[i - 0], E[i - 0], F[i - 0], G[i - 0], H[i - 0], I[i - 0], J[i - 0], K[i - 0]);
          printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i + 1, A[i + 1], B[i + 1], C[i + 1], D[i + 1], E[i + 1], F[i + 1], G[i + 1], H[i + 1], I[i + 1], J[i + 1], K[i + 1]);
          VAM_VM_CLEAN(&VM);
          delete[] A;
          delete[] B;
          delete[] C;
          delete[] D;
          delete[] E;
          delete[] F;
          delete[] G;
          delete[] H;
          delete[] I;
          delete[] J;
          delete[] K;
          exit(1);
        }
      }
    #endif

    #ifdef SORT_RADIX3
      // for (i = 0; i < SWSIZE * 8; i++) {
      //   printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i, A[i], B[i], C[i], D[i], E[i], F[i], G[i], H[i], I[i], J[i], K[i]);
      // }

      for (i = 1; i < SWSIZE * 8; i++) {
        //   printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i, A[i], B[i], C[i], D[i], E[i], F[i], G[i], H[i], I[i], J[i], K[i]);
        if (J[0] != K[0] || J[i] < J[i - 1]) {
          printf("Error at:%d\r\n", i);
          printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i - 1, A[i - 1], B[i - 1], C[i - 1], D[i - 1], E[i - 1], F[i - 1], G[i - 1], H[i - 1], I[i - 1], J[i - 1], K[i - 1]);
          printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i - 0, A[i - 0], B[i - 0], C[i - 0], D[i - 0], E[i - 0], F[i - 0], G[i - 0], H[i - 0], I[i - 0], J[i - 0], K[i - 0]);
          printf("%6d:\tA:%6d B:%6d C:%6d D:%6d E:%6d F:%6d G:%6d H:%6d I:%6d J:%6d K:%6d\n", i + 1, A[i + 1], B[i + 1], C[i + 1], D[i + 1], E[i + 1], F[i + 1], G[i + 1], H[i + 1], I[i + 1], J[i + 1], K[i + 1]);
          VAM_VM_CLEAN(&VM);
          delete[] A;
          delete[] B;
          delete[] C;
          delete[] D;
          delete[] E;
          delete[] F;
          delete[] G;
          delete[] H;
          delete[] I;
          delete[] J;
          delete[] K;
          exit(1);
        }
      }
    #endif

    printf("Passed!\r\n");
  }

  VAM_VM_CLEAN(&VM);
  delete[] A;
  delete[] B;
  delete[] C;
  delete[] D;
  delete[] E;
  delete[] F;
  delete[] G;
  delete[] H;
  delete[] I;
  delete[] J;
  delete[] K;

  return 0;
}


/*
11/13/15, VADD, SIZE: 524880000 = 1.9 GBytes
================================================================================
Begin...
CPU    1 threads :      417,612 us
JIT    1 threads :    2,928,620 us (with PR)
JIT    1 threads :    2,927,495 us
JIT    2 threads :    1,448,748 us
JIT    4 threads :    1,398,468 us <-
JIT    6 threads :    1,413,245 us
JIT    8 threads :    1,413,486 us
JIT   10 threads :    1,407,700 us
JIT   12 threads :    1,405,887 us
Passed!
*/

/*
11/13/15, VMUL, SIZE: 524880000 = 1.9 GBytes
================================================================================
Begin...
CPU    1 threads :      431,807 us
JIT    1 threads :    6,582,495 us (with PR)
JIT    1 threads :    6,579,973 us
JIT    2 threads :    3,216,971 us
JIT    4 threads :    1,646,160 us
JIT    6 threads :    1,471,663 us <-
JIT    8 threads :    1,555,613 us
JIT   10 threads :    1,471,380 us
JIT   12 threads :    1,506,018 us
Passed!
*/

/*
11/13/15, A^2+b^2, SIZE: 524880000 = 1.9 GBytes
================================================================================
Begin...
CPU    1 threads :      417,908 us
JIT    1 threads :    8,658,581 us (with PR)
JIT    1 threads :    8,646,399 us
JIT    2 threads :    4,272,384 us
JIT    4 threads :    2,157,717 us
JIT    6 threads :    1,466,381 us
JIT    8 threads :    1,517,133 us
JIT   10 threads :    1,464,127 us
JIT   12 threads :    1,441,734 us
Passed!
*/

/*
03/31/16, VMUL, SIZE: 524880000 = 1.9 GBytes
================================================================================
Begin...
CPU    1 threads :      419,407 us
JIT    8 threads :   49,452,013 us
0:  A:1 B:1 C:1 D:1
1:  A:2 B:2 C:4 D:4
2:  A:3 B:3 C:9 D:9
3:  A:4 B:4 C:16  D:16
4:  A:5 B:5 C:25  D:25
5:  A:6 B:6 C:36  D:36
6:  A:7 B:7 C:49  D:49
7:  A:8 B:8 C:64  D:64
8:  A:9 B:9 C:81  D:81
9:  A:10  B:10  C:100 D:100
Passed!
*/

/*
04/01/16, VADD, SIZE: 26214400  = 100 MBytes
================================================================================
CPU    1 threads :      24208 us
JIT    1 threads :     296062 us
JIT    2 threads :     167116 us
JIT    4 threads :     127317 us
JIT    8 threads :     153507 us
*/

/*
04/01/16, VMUL, SIZE: 524880000 = 1.9 GBytes
================================================================================
Begin...
524,880,000
CPU    1 threads :    419,488 us
JIT   16 threads :  1,483,885 us  Passed!
JIT    8 threads :  1,551,444 us  Passed!
JIT    4 threads :  1,493,106 us  Passed!
JIT    2 threads :  2,736,046 us  Passed!
JIT    1 threads :  5,603,247 us  Passed!
*/

/*
05/07/16, Radix, 4096 * 4 Words 1 FPGA, SW:Insertion
================================================================================
CPU    1 threads :     76,306 us
[DEBUG->Download] Loading Static bit on 0 FPGA: './acc_static.bit' ...
[DEBUG->VAM_VM_INIT] VM_INIT Done
[DEBUG->task_thread: 0] SORT_RADIX2
JIT    1 threads :     73,819 us
Error at:8192
  8191: A:999999 B:999999 C:999999 D:999999 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J: 32765 K: 24576
  8192: A:    -2 B:    -2 C:    -2 D:    -2 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J: 26984 K: 24577
  8193: A:    -2 B:    -2 C:    -2 D:    -2 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J: 26988 K: 24578
--------------------------------------------------------------------------------
CPU    1 threads :     86,973 us
[DEBUG->VAM_VM_INIT] Create TieStreamTable
[DEBUG->VAM_VM_INIT] VM_INIT Done
[DEBUG->task_thread: 0] SORT_RADIX2
JIT    1 threads :     43,522 us
Error at:1
     0: A: 32768 B: 32767 C: 32766 D: 32765 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J:999999 K: 16385
     1: A: 32764 B: 32763 C: 32762 D: 32761 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J:999999 K: 16386
     2: A: 32760 B: 32759 C: 32758 D: 32757 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J:999999 K: 16387
--------------------------------------------------------------------------------
CPU    1 threads :     80,971 us
[DEBUG->VAM_VM_INIT] Create TieStreamTable
[DEBUG->VAM_VM_INIT] VM_INIT Done
[DEBUG->task_thread: 0] SORT_RADIX2
JIT    1 threads :     44,228 us
Error at:1
     0: A: 32768 B: 32767 C: 32766 D: 32765 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J: 32766 K: 16385
     1: A: 32764 B: 32763 C: 32762 D: 32761 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J: 32766 K: 16386
     2: A: 32760 B: 32759 C: 32758 D: 32757 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J: 32766 K: 16387
--------------------------------------------------------------------------------
CPU    1 threads :     78,919 us
[DEBUG->VAM_VM_INIT] Create TieStreamTable
[DEBUG->VAM_VM_INIT] VM_INIT Done
[DEBUG->task_thread: 0] SORT_RADIX2
JIT    1 threads :     43,589 us
Error at:1
     0: A: 32768 B: 32767 C: 32766 D: 32765 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J:999999 K: 16385
     1: A: 32764 B: 32763 C: 32762 D: 32761 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J:999999 K: 16386
     2: A: 32760 B: 32759 C: 32758 D: 32757 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J:999999 K: 16387
*/

/*
05/07/16, Radix, 4096 * 4 Words 1 FPGA, SW:Merge
================================================================================
CPU    1 threads :      3,050 us
[DEBUG->Download] Loading Static bit on 0 FPGA: './acc_static.bit' ...
[DEBUG->VAM_VM_INIT] VM_INIT Done
[DEBUG->task_thread: 0] SORT_RADIX2
JIT    1 threads :     73,815 us
Error at:8192
  8191: A:999999 B:999999 C:999999 D:999999 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J: 32766 K: 24576
  8192: A:    -2 B:    -2 C:    -2 D:    -2 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J: 29379 K: 24577
  8193: A:    -2 B:    -2 C:    -2 D:    -2 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J: 29387 K: 24578
--------------------------------------------------------------------------------
CPU    1 threads :      2,021 us
[DEBUG->VAM_VM_INIT] Create TieStreamTable
[DEBUG->VAM_VM_INIT] VM_INIT Done
[DEBUG->task_thread: 0] SORT_RADIX2
JIT    1 threads :     43,531 us
Error at:1
     0: A: 32768 B: 32767 C: 32766 D: 32765 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J:999999 K: 16385
     1: A: 32764 B: 32763 C: 32762 D: 32761 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J:999999 K: 16386
     2: A: 32760 B: 32759 C: 32758 D: 32757 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J:999999 K: 16387
--------------------------------------------------------------------------------
CPU    1 threads :      3,041 us
[DEBUG->VAM_VM_INIT] Create TieStreamTable
[DEBUG->VAM_VM_INIT] VM_INIT Done
[DEBUG->task_thread: 0] SORT_RADIX2
JIT    1 threads :     44,206 us
Error at:1
     0: A: 32768 B: 32767 C: 32766 D: 32765 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J: 32767 K: 16385
     1: A: 32764 B: 32763 C: 32762 D: 32761 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J: 32767 K: 16386
     2: A: 32760 B: 32759 C: 32758 D: 32757 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J: 32767 K: 16387
--------------------------------------------------------------------------------
CPU    1 threads :      3,026 us
[DEBUG->VAM_VM_INIT] Create TieStreamTable
[DEBUG->VAM_VM_INIT] VM_INIT Done
[DEBUG->task_thread: 0] SORT_RADIX2
JIT    1 threads :     43,550 us
Error at:1
     0: A: 32768 B: 32767 C: 32766 D: 32765 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J:999999 K: 16385
     1: A: 32764 B: 32763 C: 32762 D: 32761 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J:999999 K: 16386
     2: A: 32760 B: 32759 C: 32758 D: 32757 E:    -2 F:    -2 G:    -2 H:    -2 I:    -2 J:999999 K: 16387
*/








































// ================================================================================
// ================================================================================
//       task_pkg[i].PR0_In1      = A;
//       task_pkg[i].SizePR0_In1  = task_pkg[i].len * 2; // 32 items, 64 numbers
//       task_pkg[i].PR0_In2      = NULL;
//       task_pkg[i].SizePR0_In2  = 0;
//       task_pkg[i].PR0_Out      = NULL;
//       task_pkg[i].SizePR0_Out  = task_pkg[i].len * 2;

//       task_pkg[i].PR1_In1      = B;
//       task_pkg[i].SizePR1_In1  = task_pkg[i].len * 2; // 32 items, 64 numbers
//       task_pkg[i].PR1_In2      = NULL;
//       task_pkg[i].SizePR1_In2  = 0;
//       task_pkg[i].PR1_Out      = NULL;
//       task_pkg[i].SizePR1_Out  = task_pkg[i].len * 2;

//       task_pkg[i].PR2_In1      = NULL;
//       task_pkg[i].SizePR2_In1  = task_pkg[i].len * 2;
//       task_pkg[i].PR2_In2      = NULL;
//       task_pkg[i].SizePR2_In2  = task_pkg[i].len * 2;
//       task_pkg[i].PR2_Out      = E;
//       task_pkg[i].SizePR2_Out  = task_pkg[i].len * 4;
// //////////////////////
//       task_pkg[i].PR0_In1      = A;
//       task_pkg[i].SizePR0_In1  = task_pkg[i].len;
//       task_pkg[i].PR0_In2      = A;
//       task_pkg[i].SizePR0_In2  = task_pkg[i].len;
//       task_pkg[i].PR0_Out      = NULL;
//       task_pkg[i].SizePR0_Out  = task_pkg[i].len;

//       task_pkg[i].PR1_In1      = B;
//       task_pkg[i].SizePR1_In1  = task_pkg[i].len;
//       task_pkg[i].PR1_In2      = B;
//       task_pkg[i].SizePR1_In2  = task_pkg[i].len;
//       task_pkg[i].PR1_Out      = NULL;
//       task_pkg[i].SizePR1_Out  = task_pkg[i].len;

//       task_pkg[i].PR2_In1      = NULL;
//       task_pkg[i].SizePR2_In1  = task_pkg[i].len;
//       task_pkg[i].PR2_In2      = NULL;
//       task_pkg[i].SizePR2_In2  = task_pkg[i].len;
//       task_pkg[i].PR2_Out      = E;
//       task_pkg[i].SizePR2_Out  = task_pkg[i].len;

      // task_pkg[i].PR0_In1      = A;
      // task_pkg[i].SizePR0_In1  = task_pkg[i].len * 2; // 32 items, 64 numbers
      // task_pkg[i].PR0_In2      = NULL;
      // task_pkg[i].SizePR0_In2  = 0;
      // task_pkg[i].PR0_Out      = C;
      // task_pkg[i].SizePR0_Out  = task_pkg[i].len * 2;

      // task_pkg[i].PR1_In1      = B;
      // task_pkg[i].SizePR1_In1  = task_pkg[i].len * 2; // 32 items, 64 numbers
      // task_pkg[i].PR1_In2      = NULL;
      // task_pkg[i].SizePR1_In2  = 0;
      // task_pkg[i].PR1_Out      = D;
      // task_pkg[i].SizePR1_Out  = task_pkg[i].len * 2;

      // task_pkg[i].PR2_In1      = C;
      // task_pkg[i].SizePR2_In1  = task_pkg[i].len * 2; // 32 items, 64 numbers
      // task_pkg[i].PR2_In2      = D;
      // task_pkg[i].SizePR2_In2  = task_pkg[i].len * 2; // 32 items, 64 numbers
      // task_pkg[i].PR2_Out      = E;
      // task_pkg[i].SizePR2_Out  = task_pkg[i].len * 4;



      // task_pkg[i].In1     =
      // task_pkg[i].SizeIn1 =
      // task_pkg[i].In2     = &B[i * task_pkg[i].len];
      // task_pkg[i].SizeIn2 = task_pkg[i].len;
      // // task_pkg[i].In2     = NULL; //&B[i * task_pkg[i].len];
      // // task_pkg[i].SizeIn2 = 0;    //task_pkg[i].len;
      // task_pkg[i].Out     = &D[i * task_pkg[i].len];
      // task_pkg[i].SizeOut = task_pkg[i].len * 2;

  // if (items >= MAX_SIZE) {
  //   sizeIn1  = MAX_SIZE;
  //   sizeIn2  = MAX_SIZE;
  //   sizeOut  = MAX_SIZE;
  // }
  // else {
  //   sizeIn1  = p->SizeIn1;
  //   sizeIn2  = p->SizeIn2;
  //   sizeOut  = p->SizeOut;
  // }

  // err =   vnew(VM, nPR);                                                        errCheck(err, FUN_VNEW);
  // err =   vlpr(VM, nPR->at(0),  VMUL);                                          errCheck(err, FUN_VLPR);
  // err = vtieio(VM, nPR->at(0),  In1,  In2,  Out, sizeIn1);                      errCheck(err, FUN_VTIEIO);

  // for (int i = 0; i < iter; i++) {
  //   err = vstart(VM, nPR, MAX_SIZE, MAX_SIZE, MAX_SIZE);                        errCheck(err, FUN_VSTART);
  // }

  // if (reminder != 0) {
  //   err = vtieio(VM, nPR->at(0),  In1,  In2,  Out, reminder);                   errCheck(err, FUN_VTIEIO);
  //   err = vstart(VM, nPR, reminder, reminder, reminder);                        errCheck(err, FUN_VSTART);
  // }
  // err =   vdel(VM, nPR);                                                        errCheck(err, FUN_VDEL);



  // pe_t package[THREADS];
  // for (i = 0; i < THREADS; i++) {
  //   package[i].key = i;
  //   package[i].len = SIZE / THREADS;
  //   // int alignment_buf
  //   package[i].A = &A[i * package[i].len];
  //   package[i].B = &B[i * package[i].len];
  //   package[i].C = &D[i * package[i].len];
  // }

  // {
  //   int THREADS = 16;
  //   pthread_t thread[THREADS];
  //   task_pk_t task_pkg[THREADS];

  //   vector<vector<int> > nPR(THREADS);
  //   for (i = 0; i < THREADS; i++) {
  //     nPR[i].resize(STEPS);
  //   }

  //   for (i = 0; i < THREADS; i++) {
  //     task_pkg[i].task_id = i;
  //     task_pkg[i].VM      = &VM;
  //     task_pkg[i].nPR     = &nPR[i];
  //     task_pkg[i].len     = SIZE / THREADS;
  //     task_pkg[i].In1     = &A[i * task_pkg[i].len];
  //     task_pkg[i].SizeIn1 = task_pkg[i].len;
  //     task_pkg[i].In2     = &B[i * task_pkg[i].len];
  //     task_pkg[i].SizeIn2 = task_pkg[i].len;
  //     task_pkg[i].Out     = &D[i * task_pkg[i].len];
  //     task_pkg[i].SizeOut = task_pkg[i].len;
  //   }

  //   gettimeofday(&start, NULL);
  //   for (i = 0; i < THREADS; i++) {
  //     pthread_create(&thread[i], NULL, VMUL_Threads_Call, (void *)&task_pkg[i]);
  //   }
  //   for (i = 0; i < THREADS; i++) {
  //     pthread_join(thread[i], NULL);
  //   }
  //   gettimeofday(&end, NULL);
  //   timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  //   printf("JIT %'4d threads :\t%'9d us\t", THREADS, timeuse);

  //   for (i = 0; i < SWSIZE; i++) {
  //     // printf("%d:\tA:%d\tB:%d\tC:%d\tD:%d\r\n", i, A[i], B[i], C[i], D[i]);
  //     if (C[i] != D[i]) {
  //       printf("Error at %d:\tA:%d\tB:%d\tC:%d\tD:%d\r\nFailed!\r\n", i, A[i], B[i], C[i], D[i]);
  //       VAM_VM_CLEAN(&VM);
  //       delete[] A;
  //       delete[] B;
  //       delete[] C;
  //       delete[] D;
  //       exit(1);
  //     }
  //   }
  //   printf("Passed!\r\n");
  // }

  // {
  //   int THREADS = 8;
  //   pthread_t thread[THREADS];
  //   task_pk_t task_pkg[THREADS];

  //   vector<vector<int> > nPR(THREADS);
  //   for (i = 0; i < THREADS; i++) {
  //     nPR[i].resize(STEPS);
  //   }

  //   for (i = 0; i < THREADS; i++) {
  //     task_pkg[i].task_id = i;
  //     task_pkg[i].VM      = &VM;
  //     task_pkg[i].nPR     = &nPR[i];
  //     task_pkg[i].len     = SIZE / THREADS;
  //     task_pkg[i].In1     = &A[i * task_pkg[i].len];
  //     task_pkg[i].SizeIn1 = task_pkg[i].len;
  //     task_pkg[i].In2     = &B[i * task_pkg[i].len];
  //     task_pkg[i].SizeIn2 = task_pkg[i].len;
  //     task_pkg[i].Out     = &D[i * task_pkg[i].len];
  //     task_pkg[i].SizeOut = task_pkg[i].len;
  //   }

  //   gettimeofday(&start, NULL);
  //   for (i = 0; i < THREADS; i++) {
  //     pthread_create(&thread[i], NULL, VMUL_Threads_Call, (void *)&task_pkg[i]);
  //   }
  //   for (i = 0; i < THREADS; i++) {
  //     pthread_join(thread[i], NULL);
  //   }
  //   gettimeofday(&end, NULL);
  //   timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  //   printf("JIT %'4d threads :\t%'9d us\t", THREADS, timeuse);

  //   for (i = 0; i < SWSIZE; i++) {
  //     // printf("%d:\tA:%d\tB:%d\tC:%d\tD:%d\r\n", i, A[i], B[i], C[i], D[i]);
  //     if (C[i] != D[i]) {
  //       printf("Error at %d:\tA:%d\tB:%d\tC:%d\tD:%d\r\nFailed!\r\n", i, A[i], B[i], C[i], D[i]);
  //       VAM_VM_CLEAN(&VM);
  //       delete[] A;
  //       delete[] B;
  //       delete[] C;
  //       delete[] D;
  //       exit(1);
  //     }
  //   }
  //   printf("Passed!\r\n");
  // }

  // {
  //   int THREADS = 4;
  //   pthread_t thread[THREADS];
  //   task_pk_t task_pkg[THREADS];

  //   vector<vector<int> > nPR(THREADS);
  //   for (i = 0; i < THREADS; i++) {
  //     nPR[i].resize(STEPS);
  //   }

  //   for (i = 0; i < THREADS; i++) {
  //     task_pkg[i].task_id = i;
  //     task_pkg[i].VM      = &VM;
  //     task_pkg[i].nPR     = &nPR[i];
  //     task_pkg[i].len     = SIZE / THREADS;
  //     task_pkg[i].In1     = &A[i * task_pkg[i].len];
  //     task_pkg[i].SizeIn1 = task_pkg[i].len;
  //     task_pkg[i].In2     = &B[i * task_pkg[i].len];
  //     task_pkg[i].SizeIn2 = task_pkg[i].len;
  //     task_pkg[i].Out     = &D[i * task_pkg[i].len];
  //     task_pkg[i].SizeOut = task_pkg[i].len;
  //   }

  //   gettimeofday(&start, NULL);
  //   for (i = 0; i < THREADS; i++) {
  //     pthread_create(&thread[i], NULL, VMUL_Threads_Call, (void *)&task_pkg[i]);
  //   }
  //   for (i = 0; i < THREADS; i++) {
  //     pthread_join(thread[i], NULL);
  //   }
  //   gettimeofday(&end, NULL);
  //   timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  //   printf("JIT %'4d threads :\t%'9d us\t", THREADS, timeuse);

  //   for (i = 0; i < SWSIZE; i++) {
  //     // printf("%d:\tA:%d\tB:%d\tC:%d\tD:%d\r\n", i, A[i], B[i], C[i], D[i]);
  //     if (C[i] != D[i]) {
  //       printf("Error at %d:\tA:%d\tB:%d\tC:%d\tD:%d\r\nFailed!\r\n", i, A[i], B[i], C[i], D[i]);
  //       VAM_VM_CLEAN(&VM);
  //       delete[] A;
  //       delete[] B;
  //       delete[] C;
  //       delete[] D;
  //       exit(1);
  //     }
  //   }
  //   printf("Passed!\r\n");
  // }

  // {
  //   int THREADS = 2;
  //   pthread_t thread[THREADS];
  //   task_pk_t task_pkg[THREADS];

  //   vector<vector<int> > nPR(THREADS);
  //   for (i = 0; i < THREADS; i++) {
  //     nPR[i].resize(STEPS);
  //   }

  //   for (i = 0; i < THREADS; i++) {
  //     task_pkg[i].task_id = i;
  //     task_pkg[i].VM      = &VM;
  //     task_pkg[i].nPR     = &nPR[i];
  //     task_pkg[i].len     = SIZE / THREADS;
  //     task_pkg[i].In1     = &A[i * task_pkg[i].len];
  //     task_pkg[i].SizeIn1 = task_pkg[i].len;
  //     task_pkg[i].In2     = &B[i * task_pkg[i].len];
  //     task_pkg[i].SizeIn2 = task_pkg[i].len;
  //     task_pkg[i].Out     = &D[i * task_pkg[i].len];
  //     task_pkg[i].SizeOut = task_pkg[i].len;
  //   }

  //   gettimeofday(&start, NULL);
  //   for (i = 0; i < THREADS; i++) {
  //     pthread_create(&thread[i], NULL, VMUL_Threads_Call, (void *)&task_pkg[i]);
  //   }
  //   for (i = 0; i < THREADS; i++) {
  //     pthread_join(thread[i], NULL);
  //   }
  //   gettimeofday(&end, NULL);
  //   timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  //   printf("JIT %'4d threads :\t%'9d us\t", THREADS, timeuse);

  //   for (i = 0; i < SWSIZE; i++) {
  //     // printf("%d:\tA:%d\tB:%d\tC:%d\tD:%d\r\n", i, A[i], B[i], C[i], D[i]);
  //     if (C[i] != D[i]) {
  //       printf("Error at %d:\tA:%d\tB:%d\tC:%d\tD:%d\r\nFailed!\r\n", i, A[i], B[i], C[i], D[i]);
  //       VAM_VM_CLEAN(&VM);
  //       delete[] A;
  //       delete[] B;
  //       delete[] C;
  //       delete[] D;
  //       exit(1);
  //     }
  //   }
  //   printf("Passed!\r\n");
  // }

  // #ifdef SORT256
  // gettimeofday(&start, NULL);
  // err =   vnew(VM, nPR);                                                                                                errCheck(err, FUN_VNEW);
  // gettimeofday(&end, NULL);
  // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("[DEBUG->task_thread:%2d] VNEW: %'9d us\r\n", 1, timeuse);

  // gettimeofday(&start, NULL);
  // err =   vlpr(VM, nPR->at(0),  INSERTION);                                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(1),  INSERTION);                                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(2),  INSERTION);                                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(3),  INSERTION);                                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(4),  MERGE);                                                                                 errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(5),  MERGE);                                                                                 errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(6),  MERGE);                                                                                 errCheck(err, FUN_VLPR);

  // err =   vlpr(VM, nPR->at(7),  INSERTION);                                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(8),  INSERTION);                                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(9),  INSERTION);                                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(10), INSERTION);                                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(11), MERGE);                                                                                 errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(12), MERGE);                                                                                 errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(13), MERGE);                                                                                 errCheck(err, FUN_VLPR);

  // err =   vlpr(VM, nPR->at(14), MERGE);                                                                                 errCheck(err, FUN_VLPR);
  // gettimeofday(&end, NULL);
  // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("[DEBUG->task_thread:%2d] VLPR: %'9d us\r\n", 1, timeuse);

  // gettimeofday(&start, NULL);
  // err = vtieio(VM, nPR->at(0),  p->PR0_In1, p->SizePR0_In1, p->PR0_In2, p->SizePR0_In2, nPR->at(4), p->SizePR0_Out);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(1),  p->PR1_In1, p->SizePR1_In1, p->PR1_In2, p->SizePR1_In2, nPR->at(4), p->SizePR1_Out);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(2),  p->PR2_In1, p->SizePR2_In1, p->PR2_In2, p->SizePR2_In2, nPR->at(5), p->SizePR2_Out);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(3),  p->PR3_In1, p->SizePR3_In1, p->PR3_In2, p->SizePR3_In2, nPR->at(5), p->SizePR3_Out);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(4),  nPR->at(0), p->SizePR4_In1, nPR->at(1), p->SizePR4_In2, nPR->at(6), p->SizePR4_Out);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(5),  nPR->at(2), p->SizePR5_In1, nPR->at(3), p->SizePR5_In2, nPR->at(6), p->SizePR5_Out);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(6),  nPR->at(4), p->SizePR6_In1, nPR->at(5), p->SizePR6_In2, nPR->at(14),p->SizePR6_Out);    errCheck(err, FUN_VTIEIO);

  // err = vtieio(VM, nPR->at(7),  p->PR7_In1, p->SizePR7_In1, p->PR7_In2, p->SizePR7_In2, nPR->at(11), p->SizePR7_Out);   errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(8),  p->PR8_In1, p->SizePR8_In1, p->PR8_In2, p->SizePR8_In2, nPR->at(11), p->SizePR8_Out);   errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(9),  p->PR9_In1, p->SizePR9_In1, p->PR9_In2, p->SizePR9_In2, nPR->at(12), p->SizePR9_Out);   errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(10), p->PR10_In1,p->SizePR10_In1,p->PR10_In2,p->SizePR10_In2,nPR->at(12), p->SizePR10_Out);  errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(11), nPR->at(7), p->SizePR11_In1,nPR->at(8), p->SizePR11_In2,nPR->at(13), p->SizePR11_Out);  errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(12), nPR->at(9), p->SizePR12_In1,nPR->at(10),p->SizePR12_In2,nPR->at(13), p->SizePR12_Out);  errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(13), nPR->at(11),p->SizePR13_In1,nPR->at(12),p->SizePR13_In2,nPR->at(14), p->SizePR13_Out);  errCheck(err, FUN_VTIEIO);

  // err = vtieio(VM, nPR->at(14), nPR->at(6), p->SizePR14_In1,nPR->at(13),p->SizePR14_In2,p->PR14_Out, p->SizePR14_Out);  errCheck(err, FUN_VTIEIO);
  // gettimeofday(&end, NULL);
  // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("[DEBUG->task_thread:%2d] VTIEIO: %'9d us\r\n", 1, timeuse);

  // gettimeofday(&start, NULL);
  // err = vstart(VM, nPR);                                                                                                errCheck(err, FUN_VSTART);
  // gettimeofday(&end, NULL);
  // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("[DEBUG->task_thread:%2d] VSTRAT: %'9d us\r\n", 1, timeuse);

  // gettimeofday(&start, NULL);
  // err =   vdel(VM, nPR);                                                                                                errCheck(err, FUN_VDEL);
  // gettimeofday(&end, NULL);
  // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("[DEBUG->task_thread:%2d] VDEL: %'9d us\r\n", 1, timeuse);

  // #endif

  // // Sort
  // err =   vnew(VM, nPR);                                                                            errCheck(err, FUN_VNEW);
  // err =   vlpr(VM, nPR->at(0),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(1),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(2),  MERGE);                                                             errCheck(err, FUN_VLPR);
  // err = vtieio(VM, nPR->at(0),  p->PR0_In1,     p->PR0_In2,     nPR->at(2),     p->SizePR0_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(1),  p->PR1_In1,     p->PR1_In2,     nPR->at(2),     p->SizePR1_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(2),  nPR->at(0),     nPR->at(1),     p->PR2_Out,     p->SizePR2_In1);    errCheck(err, FUN_VTIEIO);
  // err = vstart(VM, nPR,         p->SizePR0_In1, p->SizePR1_In1, p->SizePR2_Out);                    errCheck(err, FUN_VSTART);
  // err =   vdel(VM, nPR);                                                                            errCheck(err, FUN_VDEL);

  // #ifdef SORT128

  // gettimeofday(&start, NULL);
  // err =   vnew(VM, nPR);                                                                                                errCheck(err, FUN_VNEW);
  // gettimeofday(&end, NULL);
  // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("[DEBUG->task_thread:%2d] VNEW: %'9d us\r\n", 1, timeuse);

  // gettimeofday(&start, NULL);
  // err =   vlpr(VM, nPR->at(0),  INSERTION);                                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(1),  INSERTION);                                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(2),  INSERTION);                                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(3),  INSERTION);                                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(4),  MERGE);                                                                                 errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(5),  MERGE);                                                                                 errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(6),  MERGE);                                                                                 errCheck(err, FUN_VLPR);
  // gettimeofday(&end, NULL);
  // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("[DEBUG->task_thread:%2d] VLPR: %'9d us\r\n", 1, timeuse);

  // gettimeofday(&start, NULL);
  // err = vtieio(VM, nPR->at(0),  p->PR0_In1, p->SizePR0_In1, p->PR0_In2, p->SizePR0_In2, nPR->at(4), p->SizePR0_Out);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(1),  p->PR1_In1, p->SizePR1_In1, p->PR1_In2, p->SizePR1_In2, nPR->at(4), p->SizePR1_Out);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(2),  p->PR2_In1, p->SizePR2_In1, p->PR2_In2, p->SizePR2_In2, nPR->at(5), p->SizePR2_Out);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(3),  p->PR3_In1, p->SizePR3_In1, p->PR3_In2, p->SizePR3_In2, nPR->at(5), p->SizePR3_Out);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(4),  nPR->at(0), p->SizePR4_In1, nPR->at(1), p->SizePR4_In2, nPR->at(6), p->SizePR4_Out);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(5),  nPR->at(2), p->SizePR5_In1, nPR->at(3), p->SizePR5_In2, nPR->at(6), p->SizePR5_Out);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(6),  nPR->at(4), p->SizePR6_In1, nPR->at(5), p->SizePR6_In2, p->PR6_Out, p->SizePR6_Out);    errCheck(err, FUN_VTIEIO);
  // gettimeofday(&end, NULL);
  // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("[DEBUG->task_thread:%2d] VTIEIO: %'9d us\r\n", 1, timeuse);

  // gettimeofday(&start, NULL);
  // err = vstart(VM, nPR);                                                                                                errCheck(err, FUN_VSTART);
  // gettimeofday(&end, NULL);
  // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("[DEBUG->task_thread:%2d] VSTRAT: %'9d us\r\n", 1, timeuse);

  // gettimeofday(&start, NULL);
  // err =   vdel(VM, nPR);                                                                                                errCheck(err, FUN_VDEL);
  // gettimeofday(&end, NULL);
  // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("[DEBUG->task_thread:%2d] VDEL: %'9d us\r\n", 1, timeuse);
  // #endif



  // // Sort
  // err =   vnew(VM, nPR);                                                                            errCheck(err, FUN_VNEW);
  // err =   vlpr(VM, nPR->at(0),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(1),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(2),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(3),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(4),  MERGE);                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(5),  MERGE);                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(6),  MERGE);                                                             errCheck(err, FUN_VLPR);

  // err = vtieio(VM, nPR->at(0),  p->PR0_In1,     p->PR0_In2,     nPR->at(4),     p->SizePR0_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(1),  p->PR1_In1,     p->PR1_In2,     nPR->at(4),     p->SizePR1_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(2),  p->PR2_In1,     p->PR2_In2,     nPR->at(5),     p->SizePR2_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(3),  p->PR3_In1,     p->PR3_In2,     nPR->at(5),     p->SizePR3_In1);    errCheck(err, FUN_VTIEIO);

  // err = vtieio(VM, nPR->at(4),  nPR->at(0),     nPR->at(1),     nPR->at(6),     p->SizePR4_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(5),  nPR->at(2),     nPR->at(3),     nPR->at(6),     p->SizePR5_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(6),  nPR->at(4),     nPR->at(5),     p->PR6_Out,     p->SizePR6_In1);    errCheck(err, FUN_VTIEIO);

  // err = vstart(VM, nPR,         p->SizePR0_In1, p->SizePR1_In1, p->SizePR6_Out);                    errCheck(err, FUN_VSTART);
  // err =   vdel(VM, nPR);                                                                            errCheck(err, FUN_VDEL);

  // // Add
  // err =   vnew(VM, nPR);                                                                            errCheck(err, FUN_VNEW);
  // err =   vlpr(VM, nPR->at(0),  VMUL);                                                              errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(1),  VMUL);                                                              errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(2),  VMUL);                                                              errCheck(err, FUN_VLPR);
  // err = vtieio(VM, nPR->at(0),  p->PR0_In1,     p->PR0_In2,     nPR->at(2),     p->SizePR0_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(1),  p->PR1_In1,     p->PR1_In2,     nPR->at(2),     p->SizePR1_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(2),  nPR->at(0),     nPR->at(1),     p->PR2_Out,     p->SizePR2_In1);    errCheck(err, FUN_VTIEIO);
  // err = vstart(VM, nPR,         p->SizePR0_In1, p->SizePR1_In1, p->SizePR2_Out);                    errCheck(err, FUN_VSTART);
  // err =   vdel(VM, nPR);                                                                            errCheck(err, FUN_VDEL);

  // // Add
  // err =   vnew(VM, nPR);                                                                            errCheck(err, FUN_VNEW);
  // err =   vlpr(VM, nPR->at(0),  VADD);                                                              errCheck(err, FUN_VLPR);
  // err = vtieio(VM, nPR->at(0),  p->PR0_In1,     p->PR0_In2,     p->PR0_Out,     p->SizePR0_In1);    errCheck(err, FUN_VTIEIO);
  // err = vstart(VM, nPR,         p->SizePR0_In1, p->SizePR0_In2, p->SizePR0_Out);                    errCheck(err, FUN_VSTART);
  // err =   vdel(VM, nPR);                                                                            errCheck(err, FUN_VDEL);


  // err =   vnew(VM, nPR0);                                                                            errCheck(err, FUN_VNEW);
  // err =   vnew(VM, nPR1);                                                                            errCheck(err, FUN_VNEW);
  // err =   vnew(VM, nPR2);                                                                            errCheck(err, FUN_VNEW);

  // err =   vlpr(VM, nPR0->at(0),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR1->at(0),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR2->at(0),  MERGE);                                                             errCheck(err, FUN_VLPR);

  // err = vtieio(VM, nPR0->at(0),  p->PR0_In1,     p->PR0_In2,     nPR2->at(0),     p->SizePR0_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR1->at(0),  p->PR1_In1,     p->PR1_In2,     nPR2->at(0),     p->SizePR1_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR2->at(0),  nPR0->at(0),    nPR1->at(0),    p->PR2_Out,      p->SizePR2_In1);    errCheck(err, FUN_VTIEIO);

  // err = vstart(VM, nPR0,         p->SizePR0_In1, p->SizePR0_In2, p->SizePR0_Out);                    errCheck(err, FUN_VSTART);
  // err = vstart(VM, nPR1,         p->SizePR1_In1, p->SizePR1_In2, p->SizePR1_Out);                    errCheck(err, FUN_VSTART);
  // err = vstart(VM, nPR2,         p->SizePR0_In1, p->SizePR1_In1, p->SizePR2_Out);                    errCheck(err, FUN_VSTART);

  // err =   vdel(VM, nPR0);                                                                            errCheck(err, FUN_VDEL);
  // err =   vdel(VM, nPR1);                                                                            errCheck(err, FUN_VDEL);
  // err =   vdel(VM, nPR2);                                                                            errCheck(err, FUN_VDEL);
///////////////////////////////////////////////////////

  // err =   vnew(VM, nPR0);                                                                            errCheck(err, FUN_VNEW);
  // err =   vlpr(VM, nPR0->at(0),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err = vtieio(VM, nPR0->at(0),  p->PR0_In1,     p->PR0_In2,     p->PR0_Out,     p->SizePR0_In1);    errCheck(err, FUN_VTIEIO);
  // err = vstart(VM, nPR0,         p->SizePR0_In1, p->SizePR0_In2, p->SizePR0_Out);                    errCheck(err, FUN_VSTART);

  // err =   vnew(VM, nPR1);                                                                            errCheck(err, FUN_VNEW);
  // err =   vlpr(VM, nPR1->at(0),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err = vtieio(VM, nPR1->at(0),  p->PR1_In1,     p->PR1_In2,     p->PR1_Out,     p->SizePR1_In1);    errCheck(err, FUN_VTIEIO);
  // err = vstart(VM, nPR1,         p->SizePR1_In1, p->SizePR1_In2, p->SizePR1_Out);                    errCheck(err, FUN_VSTART);

  // err =   vnew(VM, nPR2);                                                                            errCheck(err, FUN_VNEW);
  // err =   vlpr(VM, nPR2->at(0),  MERGE);                                                             errCheck(err, FUN_VLPR);
  // err = vtieio(VM, nPR2->at(0),  p->PR0_Out,     p->PR1_Out,     p->PR2_Out,     p->SizePR2_In1);    errCheck(err, FUN_VTIEIO);
  // err = vstart(VM, nPR2,         p->SizePR0_In1, p->SizePR1_In1, p->SizePR2_Out);                    errCheck(err, FUN_VSTART);

  // err =   vdel(VM, nPR0);                                                                            errCheck(err, FUN_VDEL);
  // err =   vdel(VM, nPR1);                                                                            errCheck(err, FUN_VDEL);
  // err =   vdel(VM, nPR2);                                                                            errCheck(err, FUN_VDEL);


  // err =   vlpr(VM, nPR->at(0),  MERGE);                                                             errCheck(err, FUN_VLPR);

  // err =   vlpr(VM, nPR->at(1),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(2),  MERGE);                                                             errCheck(err, FUN_VLPR);
  // // err = vtieio(VM, nPR->at(0),  p->PR2_In1,     p->PR2_In2,     p->PR2_Out,     p->SizePR2_In1);    errCheck(err, FUN_VTIEIO);

  // err = vtieio(VM, nPR->at(1),  p->PR1_In1,     p->PR1_In2,     p->PR1_Out,     p->SizePR1_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(2),  p->PR2_In1,     p->PR2_In2,     p->PR2_Out,     p->SizePR2_In1);    errCheck(err, FUN_VTIEIO);
  // // err = vstart(VM, nPR,         p->SizePR2_In1, p->SizePR2_In2, p->SizePR2_Out);                    errCheck(err, FUN_VSTART);
  // err = vstart(VM, nPR,         p->SizePR0_In1, p->SizePR1_In1, p->SizePR2_Out);                    errCheck(err, FUN_VSTART);
  // err =   vdel(VM, nPR);                                                                            errCheck(err, FUN_VDEL);

  // err =   vnew(VM, nPR);                                                                            errCheck(err, FUN_VNEW);
  // // err =   vlpr(VM, nPR->at(0),  MERGE);                                                             errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(0),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(1),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(2),  MERGE);                                                             errCheck(err, FUN_VLPR);
  // // err = vtieio(VM, nPR->at(0),  p->PR2_In1,     p->PR2_In2,     p->PR2_Out,     p->SizePR2_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(0),  p->PR0_In1,     p->PR0_In2,     p->PR0_Out,     p->SizePR0_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(1),  p->PR1_In1,     p->PR1_In2,     p->PR1_Out,     p->SizePR1_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(2),  p->PR2_In1,     p->PR2_In2,     p->PR2_Out,     p->SizePR2_In1);    errCheck(err, FUN_VTIEIO);
  // // err = vstart(VM, nPR,         p->SizePR2_In1, p->SizePR2_In2, p->SizePR2_Out);                    errCheck(err, FUN_VSTART);
  // err = vstart(VM, nPR,         p->SizePR0_In1, p->SizePR1_In1, p->SizePR2_Out);                    errCheck(err, FUN_VSTART);
  // err =   vdel(VM, nPR);                                                                            errCheck(err, FUN_VDEL);


  // err =   vnew(VM, nPR);                                                                            errCheck(err, FUN_VNEW);
  // err =   vlpr(VM, nPR->at(0),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(1),  INSERTION);                                                         errCheck(err, FUN_VLPR);
  // err =   vlpr(VM, nPR->at(2),  MERGE);                                                             errCheck(err, FUN_VLPR);
  // err = vtieio(VM, nPR->at(0),  p->PR0_In1,     p->PR0_In2,     nPR->at(2),     p->SizePR0_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(1),  p->PR1_In1,     p->PR1_In2,     nPR->at(2),     p->SizePR1_In1);    errCheck(err, FUN_VTIEIO);
  // err = vtieio(VM, nPR->at(2),  nPR->at(0),     nPR->at(1),     p->PR2_Out,     p->SizePR2_In1);    errCheck(err, FUN_VTIEIO);
  // err = vstart(VM, nPR,         p->SizePR0_In1, p->SizePR1_In1, p->SizePR2_Out);                    errCheck(err, FUN_VSTART);
  // err =   vdel(VM, nPR);                                                                            errCheck(err, FUN_VDEL);

  // err = vtieio(VM, nPR->at(0),  In1,  In2,  Out, items);                        errCheck(err, FUN_VTIEIO);
  // err = vstart(VM, nPR, sizeIn1, sizeIn2, sizeOut);                             errCheck(err, FUN_VSTART);
