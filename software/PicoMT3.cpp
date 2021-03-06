#include <iostream>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <picodrv.h>
#include <pico_errors.h>
#include <pico_pr.h>
#include <sys/time.h>
#include <pthread.h>
#include <cmath>

using namespace std;

 #define VERBOSE
 #define VERBOSE_THREAD
// #define P_TEST
#include "jit_isa.h"
// 1024 * 10 * 1024
// 524880000 = 1.9 GBytes
// 402653184 = 1.5 GBytes
// 314572800 = 1.2 GBytes
// 78643200  = 300 MBytes
// 52428800  = 200 MBytes
// 26214400  = 100 MBytes
// FCCM Test:

#define SIZE    6291456 
//#define SIZE    1024
#define THREADS 96 
#define SWSIZE  SIZE 
#define STEPS   1
// #define ITEMS   SIZE

// #define DEBUG
#define NODEBUG

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
  int len;
  int     *In1;
  int     SizeIn1;
  int     *In2;
  int     SizeIn2;
  int     *Out;
  int     SizeOut;
}task_pk_t;

void * threads_call(void* ptr)
{
  pe_t *p = (pe_t *)ptr;
  // printf("Key:%4d\tLen:%4d\tA:%p\tB:%p\tC:%p\r\n", p->key, p->len, p->A, p->B, p->C);
  int i;
  for (i = 0; i < p->len; i++) {
    p->C[i] = p->A[i] - p->B[i];
  }
  return (void*) 0;
}

void * VMUL_Threads_Call(void *pk)
{
#ifdef VERBOSE_THREAD
  printf("\r\n");
#endif
  task_pk_t *p      = (task_pk_t*) pk;
#ifdef VERBOSE_THREAD
  printf("[DEBUG->task_thread:%2d] task thread start\r\n", p->task_id);
#endif
  vam_vm_t    *VM      = p->VM;
  vector<int> *nPR     = p->nPR;
  int         items    = p->len;
  int    *In1     = p->In1;
  int    sizeIn1  = p->SizeIn1;
  int    *In2     = p->In2;
  int    sizeIn2  = p->SizeIn2;
  int    *Out     = p->Out;
  int    sizeOut  = p->SizeOut;
  int err;
  err =   vnew(VM, nPR);                                                        errCheck(err, FUN_VNEW);
  err =   vlpr(VM, nPR->at(0),  FCCMMATCH);                                          errCheck(err, FUN_VLPR);
  err = vtieio(VM, nPR->at(0),  In1,  In2,   Out);                              errCheck(err, FUN_VTIEIO);
  err = vstart(VM, nPR, sizeIn1, 4, sizeOut);                             errCheck(err, FUN_VSTART);
  err =   vdel(VM, nPR);                                                        errCheck(err, FUN_VDEL);
#ifdef VERBOSE_THREAD
  printf("[DEBUG->task_thread] task thread done\r\n");
#endif
  return NULL;
}

int main(int argc, char* argv[])
{
  printf("Begin...\r\n");

  int *A = new int[SWSIZE];
  int *B = new int[SWSIZE];
  int *C = new int[SWSIZE];
  int *D = new int[SWSIZE];

  int i;
  int j;
  int sum;

  for (i = 0; i < SWSIZE; i++) {
    A[i] = i + 1;
    B[i] = i + 1;
    C[i] = 0;
    D[i] = 0;
  }
  //////////////////////////////////////////////////////////////////////////////
  struct timeval start, end;
  gettimeofday(&start, NULL);
  for (i = 0; i < SWSIZE; i++) {
    sum = (A[i] + B[i]) * (A[i] - B[i]); 
    C[i] = sum;
  }
  gettimeofday(&end, NULL);

  int timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  printf("CPU %4d threads :\t%9d us\r\n", 1, timeuse);

  pe_t package[THREADS];
  for (i = 0; i < THREADS; i++) {
    package[i].key = i;
    package[i].len = SIZE / THREADS;
    // int alignment_buf
    package[i].A = &A[i * package[i].len];
    package[i].B = &B[i * package[i].len];
    package[i].C = &D[i * package[i].len];
  }
  //////////////////////////////////////////////////////////////////////////////
  int err;
  vam_vm_t VM;
  VM.VAM_TABLE = NULL;
  VM.BITSTREAM_TABLE = NULL;
  VAM_VM_INIT(&VM, argc, argv);

  // for (i = 1; i <= CARD; i++) {
  //   VM.VAM_TABLE->at(i * COL - 1).status = 1;
  // }
  // VAM_TABLE_SHOW(VM);
  // VM.VAM_TABLE->at(0).status  = 0;
  // VM.VAM_TABLE->at(1).status  = 0;
  // VM.VAM_TABLE->at(2).status  = 0;
  // VM.VAM_TABLE->at(3).status  = 0;
  // VM.VAM_TABLE->at(4).status  = 1;

  // VM.VAM_TABLE->at(5).status  = 0;
  // VM.VAM_TABLE->at(6).status  = 0;
  // VM.VAM_TABLE->at(7).status  = 0;
  // VM.VAM_TABLE->at(8).status  = 0;
  // VM.VAM_TABLE->at(9).status  = 1;

  // VM.VAM_TABLE->at(10).status = 0;
  // VM.VAM_TABLE->at(11).status = 0;
  // VM.VAM_TABLE->at(12).status = 0;
  // VM.VAM_TABLE->at(13).status = 0;
  // VM.VAM_TABLE->at(14).status = 1;
  //////////////////////////////////////////////////////////////////////////////


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
    task_pkg[i].len     = SIZE;
    task_pkg[i].In1     = A;
    task_pkg[i].SizeIn1 = task_pkg[i].len;
    task_pkg[i].In2     = B;
    task_pkg[i].SizeIn2 = task_pkg[i].len;
    task_pkg[i].Out     = D;
    task_pkg[i].SizeOut = task_pkg[i].len;
  }

  gettimeofday(&start, NULL);
  for (i = 0; i < THREADS; i++) {
    pthread_create(&thread[i], NULL, VMUL_Threads_Call, (void *)&task_pkg[i]);
  }
  for (i = 0; i < THREADS; i++) {
    pthread_join(thread[i], NULL);
  }
  gettimeofday(&end, NULL);
  timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  printf("JIT %4d threads :\t%9d us\r\n", THREADS, timeuse);

  for (i = 0; i < 10; i++) {
    printf("%d:\tA:%d\tB:%d\tC:%d\tD:%d\r\n", i, A[i], B[i], C[i], D[i]);
    //if (C[i] != D[i]) {
      //printf("Error at %d:\tA:%d\tB:%d\tC:%d\tD:%d\r\nFailed!\r\n", i, A[i], B[i], C[i], D[i]);
      //VAM_VM_CLEAN(&VM);
      //delete[] A;
      //delete[] B;
      //delete[] C;
      //delete[] D;
      //exit(1);
    //}
  }
  printf("No Chack!\r\n");
  VAM_VM_CLEAN(&VM);
  delete[] A;
  delete[] B;
  delete[] C;
  delete[] D;
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
