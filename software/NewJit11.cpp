#include <iostream>
#include <vector>
// #include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <picodrv.h>
#include <pico_errors.h>
#include <sys/time.h>
#include <pthread.h>
#include <cmath>

using namespace std;

// #define VERBOSE
// #define VERBOSE_THREAD
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

// #define THREADS 16
#define ITER 16
#define SIZE 1024 * 1024 / 4 * ITER * 8
// #define SIZE 16
// #define SIZE    1024 * 1024 / 4 * 128
// #define SWSIZE  SIZE
#define STEPS   1

typedef struct {
  int             task_id;
  vam_vm_t        *VM;
  vector<int>     *nPR;
  struct timeval  start;
  struct timeval  end;
  int             time_vnew;
  int             time_vlpr;
  int             time_vtieio;
  int             time_vstart;
  int             time_vdel;
  int             len;
  int             *PR0_In1;
  int          SizePR0_In1;
  int             *PR0_In2;
  int          SizePR0_In2;
  int             *PR0_Out;
  int          SizePR0_Out;
}task_pk_t;

typedef struct {
  int             timeuse;
  struct timeval  start;
  struct timeval  end;
  int             len;
  int             *A;
  int             *B;
  int             *C;
}sw_pk_t;

void * SW_Threads_Call(void *pk)
{
  sw_pk_t *p = (sw_pk_t*) pk;
  gettimeofday(&p->start, NULL);
  for (int i = 0; i < p->len; i++) {
    p->C[i] = p->A[i] * p->B[i];
  }
  gettimeofday(&p->end, NULL);
  p->timeuse = 1000000 * (p->end.tv_sec - p->start.tv_sec) + p->end.tv_usec - p->start.tv_usec;
  return NULL;
}

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

  gettimeofday(&p->start, NULL);
  err =   vnew(VM, nPR);                                                                                                errCheck(err, FUN_VNEW);
  gettimeofday(&p->end, NULL);
  p->time_vnew = 1000000 * (p->end.tv_sec - p->start.tv_sec) + p->end.tv_usec - p->start.tv_usec;

  gettimeofday(&p->start, NULL);
  // err =   vlpr(VM, nPR->at(0), VADD);                                                                                   errCheck(err, FUN_VLPR);
  err =   vlprBB(VM, nPR->at(0), VMUL, SIZE);                                                                                   errCheck(err, FUN_VLPR);
  gettimeofday(&p->end, NULL);
  p->time_vlpr = 1000000 * (p->end.tv_sec - p->start.tv_sec) + p->end.tv_usec - p->start.tv_usec;

  gettimeofday(&p->start, NULL);
  err = vtieio(VM, nPR->at(0), p->PR0_In1, p->SizePR0_In1, p->PR0_In2, p->SizePR0_In2, p->PR0_Out, p->SizePR0_Out);     errCheck(err, FUN_VTIEIO);
  gettimeofday(&p->end, NULL);
  p->time_vtieio = 1000000 * (p->end.tv_sec - p->start.tv_sec) + p->end.tv_usec - p->start.tv_usec;

  gettimeofday(&p->start, NULL);
  err = vstart(VM, nPR);                                                                                                errCheck(err, FUN_VSTART);
  gettimeofday(&p->end, NULL);
  p->time_vstart = 1000000 * (p->end.tv_sec - p->start.tv_sec) + p->end.tv_usec - p->start.tv_usec;

  gettimeofday(&p->start, NULL);
  err =   vdel(VM, nPR);
  gettimeofday(&p->end, NULL);
  p->time_vdel = 1000000 * (p->end.tv_sec - p->start.tv_sec) + p->end.tv_usec - p->start.tv_usec;

#ifdef VERBOSE_THREAD
  printf("[DEBUG->task_thread:%2d] Thread Done\r\n", p->task_id);
#endif
  return NULL;
}

int main(int argc, char* argv[])
{
  printf("Begin...\r\n");
  setlocale(LC_NUMERIC, ""); // for thounds seperator

  int err;
  vam_vm_t VM;
  VM.VAM_TABLE = NULL;
  VM.BITSTREAM_TABLE = NULL;
  VAM_VM_INIT(&VM, argc, argv);

  // int MAXSIZE = 1;
  // int SIZE    = 1024 * 1024 / 4 * 64;//1024 * 1024 / 4 * 128 / 2; //1024 * 1024 / 4 / 2;
  int i;
  int j;
  int sum;
  struct timeval start, end;
  int timeuse;
////////////////////////////////////////////////////////////////////////////////////////////////////
  float KB = SIZE * 4.0 / 1024;
  float MB = KB         / 1024;
  float GB = MB         / 1024;
  // printf("SIZE: %'5d Words, %'5d Bytes, %'5.1f KB, %'5.1f MB, %'5.1f GB\r\n", SIZE, SIZE * 4, KB, MB, GB);

  int *A = new int[sizeof(int) * SIZE];
  int *B = new int[sizeof(int) * SIZE];
  int *C = new int[sizeof(int) * SIZE];
  int *D = new int[sizeof(int) * SIZE];

  srand(time(NULL));
  for (i = 0; i < SIZE; i++) {
    A[i] = rand() % 1000;
    B[i] = rand() % 1000;
    C[i] = 0;
    D[i] = 0;
  }
////////////////////////////////////////////////////////////////////////////////////////////////////
  // gettimeofday(&start, NULL);
  // for (i = 0; i < SIZE; i++) {
  //   C[i] = A[i] + B[i];
  //   // C[i] = A[i] * B[i];
  // }
  // MergeSort(A, SIZE);
  // InsertionSort(A, SIZE);
  // radixSort(A, SIZE);
  // gettimeofday(&end, NULL);
  // int timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("CPU[%4d] Size: %'10d Words\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", j, SIZE, timeuse, ((SIZE * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));
  // printf("CPU %'4d threads               :\t%'9d us\r\n", 1, timeuse);
  // printf("CPU TP                         :\t %'7.2f MB/s\r\n", ((SIZE * 1 * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));
  // for (i = 0; i < 10; i++) {
  // printf("%'5d:\tA:%'5d\tB:%'5d\tC:%'5d\tD:%'5d\r\n", i, A[SIZE - (10 - i)], B[SIZE - (10 - i)], C[SIZE - (10 - i)], D[SIZE - (10 - i)]);
  //}
////////////////////////////////////////////////////////////////////////////////////////////////////
  // for (j = 1; j <= ITER; j = j * 2) {
    int THREADS = 16;
    printf("\r\n");
    printf("   -----------------------------------------------------------------------------------\r\n");

    pthread_t sw_thread[THREADS];
    sw_pk_t   sw_pkg[THREADS];
    for (i = 0; i < THREADS; i++) {
      sw_pkg[i].timeuse = 0;
      sw_pkg[i].len     =  SIZE / THREADS;
      sw_pkg[i].A       =  &A[i * sw_pkg[i].len];
      sw_pkg[i].B       =  &B[i * sw_pkg[i].len];
      sw_pkg[i].C       =  &C[i * sw_pkg[i].len];
    }

    gettimeofday(&start, NULL);
    for (i = 0; i < THREADS; i++) {
      pthread_create(&sw_thread[i], NULL, SW_Threads_Call, (void *)&sw_pkg[i]);
    }
    for (i = 0; i < THREADS; i++) {
      pthread_join(sw_thread[i], NULL);
    }
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("CPU[%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", THREADS, MB, timeuse, ((SIZE * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));

    for (i = 0; i < THREADS; i++) {
      float tKB = sw_pkg[i].len * 4.0 / 1024;
      float tMB = tKB                   / 1024;
      float tGB = tMB                   / 1024;
      printf("   [%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", i, tMB, sw_pkg[i].timeuse, (( sw_pkg[i].len * 4.0 / 1024 / 1024) / (sw_pkg[i].timeuse * 1.0 / 1000000)));
    }
    printf("   -----------------------------------------------------------------------------------\r\n");

    pthread_t thread[THREADS];
    task_pk_t task_pkg[THREADS];

    vector<vector<int> > nPR(THREADS);
    for (i = 0; i < THREADS; i++) {
      nPR[i].resize(STEPS);
    }

    for (i = 0; i < THREADS; i++) {
      task_pkg[i].task_id      = i;
      task_pkg[i].VM           = &VM;
      task_pkg[i].nPR          = &nPR[i];
      task_pkg[i].time_vnew    = 0;
      task_pkg[i].time_vlpr    = 0;
      task_pkg[i].time_vtieio  = 0;
      task_pkg[i].time_vstart  = 0;
      task_pkg[i].time_vdel    = 0;

      task_pkg[i].len          = SIZE / THREADS;

      task_pkg[i].PR0_In1      = &A[i * task_pkg[i].len];
      task_pkg[i].SizePR0_In1  = task_pkg[i].len;
      task_pkg[i].PR0_In2      = &B[i * task_pkg[i].len];
      task_pkg[i].SizePR0_In2  = task_pkg[i].len;
      task_pkg[i].PR0_Out      = &D[i * task_pkg[i].len];
      task_pkg[i].SizePR0_Out  = task_pkg[i].len;
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
    printf("JIT[%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\t", THREADS, MB, timeuse, ((SIZE * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));

    int flag = 0;
    for (i = 0; i < SIZE; i++) {
      // printf("[%3d]|\tA:%6d\t|B:%6d\t|C:%6d\t|D:%6d\t|\r\n", i, A[i], B[i], C[i], D[i]);
      // if (C[i] != D[i]) {
      //   printf("Error!\r\n[%3d]|\tA:%6d\t|B:%6d\t|C:%6d\t|D:%6d\t|\r\n", i, A[i], B[i], C[i], D[i]);
      //   flag = 1;
      //   break;
      // }
    }
    // if (flag == 0) printf("Passed\r\n");
    printf("No Check\r\n");

    for (i = 0; i < THREADS; i++) {
      float tKB = task_pkg[i].len * 4.0 / 1024;
      float tMB = tKB                   / 1024;
      float tGB = tMB                   / 1024;
      printf("   [%4d] Size: %'10.2f MB\tVNEW: %'10d us\tVLPR: %'10d us\tVTIEIO: %'10d us\tVSTART: %'10d us\tVDEL: %'10d us\tThroughput: %'10.2f MB/s\r\n", i, tMB, task_pkg[i].time_vnew, task_pkg[i].time_vlpr, task_pkg[i].time_vtieio, task_pkg[i].time_vstart, task_pkg[i].time_vdel, (( task_pkg[i].len * 4.0 / 1024 / 1024) / (task_pkg[i].time_vstart * 1.0 / 1000000)));
      // printf("   [%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", i, tMB, task_pkg[i].timeuse, (( task_pkg[i].len * 4.0 / 1024 / 1024) / (task_pkg[i].timeuse * 1.0 / 1000000)));
    }
  // }



////////////////////////////////////////////////////////////////////////////////////////////////////
  VAM_VM_CLEAN(&VM);
  delete[] A;
  delete[] B;
  delete[] C;
  delete[] D;
  return 0;
}

/* VADD
Begin...
CPU[   0] Size:       8.00 MB ExE Time:      1,760 us Throughput:   4,545.45 MB/s
   -----------------------------------------------------------------------------------
JIT[   1] Size:       8.00 MB ExE Time:     28,941 us Throughput:     276.42 MB/s Passed
   [   0] Size:       8.00 MB VNEW:        129 us VLPR:      3,089 us VTIEIO:      3,034 us VSTART:     22,494 us VDEL:         67 us Throughput:     355.65 MB/s
   -----------------------------------------------------------------------------------
JIT[   2] Size:       8.00 MB ExE Time:     23,404 us Throughput:     341.82 MB/s Passed
   [   0] Size:       4.00 MB VNEW:         72 us VLPR:      3,107 us VTIEIO:      3,027 us VSTART:     10,870 us VDEL:         45 us Throughput:     367.99 MB/s
   [   1] Size:       4.00 MB VNEW:      6,283 us VLPR:      3,092 us VTIEIO:      3,028 us VSTART:     10,853 us VDEL:         48 us Throughput:     368.56 MB/s
   -----------------------------------------------------------------------------------
JIT[   4] Size:       8.00 MB ExE Time:     31,438 us Throughput:     254.47 MB/s Passed
   [   0] Size:       2.00 MB VNEW:        114 us VLPR:      3,092 us VTIEIO:      3,025 us VSTART:      5,541 us VDEL:        948 us Throughput:     360.95 MB/s
   [   1] Size:       2.00 MB VNEW:     12,669 us VLPR:      3,112 us VTIEIO:      3,043 us VSTART:      5,496 us VDEL:        757 us Throughput:     363.90 MB/s
   [   2] Size:       2.00 MB VNEW:     13,032 us VLPR:      8,887 us VTIEIO:      3,035 us VSTART:      6,295 us VDEL:         75 us Throughput:     317.71 MB/s
   [   3] Size:       2.00 MB VNEW:      5,715 us VLPR:      3,107 us VTIEIO:      3,036 us VSTART:      6,241 us VDEL:      6,410 us Throughput:     320.46 MB/s
   -----------------------------------------------------------------------------------
JIT[   8] Size:       8.00 MB ExE Time:     53,473 us Throughput:     149.61 MB/s Passed
   [   0] Size:       1.00 MB VNEW:      1,111 us VLPR:      3,108 us VTIEIO:      3,024 us VSTART:      2,864 us VDEL:     15,801 us Throughput:     349.16 MB/s
   [   1] Size:       1.00 MB VNEW:      1,234 us VLPR:      9,102 us VTIEIO:      3,032 us VSTART:      2,857 us VDEL:      9,650 us Throughput:     350.02 MB/s
   [   2] Size:       1.00 MB VNEW:      1,495 us VLPR:     14,962 us VTIEIO:      3,024 us VSTART:      3,648 us VDEL:      8,954 us Throughput:     274.12 MB/s
   [   3] Size:       1.00 MB VNEW:     19,620 us VLPR:      9,313 us VTIEIO:      3,039 us VSTART:      2,858 us VDEL:      9,520 us Throughput:     349.90 MB/s
   [   4] Size:       1.00 MB VNEW:     26,197 us VLPR:     21,161 us VTIEIO:      3,025 us VSTART:      2,834 us VDEL:         61 us Throughput:     352.86 MB/s
   [   5] Size:       1.00 MB VNEW:     30,832 us VLPR:      3,083 us VTIEIO:      3,018 us VSTART:      2,855 us VDEL:      3,357 us Throughput:     350.26 MB/s
   [   6] Size:       1.00 MB VNEW:     18,617 us VLPR:     21,395 us VTIEIO:      3,018 us VSTART:      2,935 us VDEL:      3,291 us Throughput:     340.72 MB/s
   [   7] Size:       1.00 MB VNEW:     17,469 us VLPR:      3,134 us VTIEIO:      3,075 us VSTART:      2,910 us VDEL:     15,702 us Throughput:     343.64 MB/s
*/

/* VADD
Begin...
CPU[   0] Size:      16.00 MB ExE Time:      3,341 us Throughput:   4,788.99 MB/s
   -----------------------------------------------------------------------------------
JIT[   1] Size:      16.00 MB ExE Time:     50,228 us Throughput:     318.55 MB/s Passed
   [   0] Size:      16.00 MB VNEW:        104 us VLPR:      3,108 us VTIEIO:      3,040 us VSTART:     43,827 us VDEL:         59 us Throughput:     365.07 MB/s
   -----------------------------------------------------------------------------------
JIT[   2] Size:      16.00 MB ExE Time:     34,554 us Throughput:     463.04 MB/s Passed
   [   0] Size:       8.00 MB VNEW:         43 us VLPR:      3,073 us VTIEIO:      3,025 us VSTART:     21,625 us VDEL:         39 us Throughput:     369.94 MB/s
   [   1] Size:       8.00 MB VNEW:        101 us VLPR:      9,148 us VTIEIO:      3,033 us VSTART:     22,115 us VDEL:         69 us Throughput:     361.75 MB/s
   -----------------------------------------------------------------------------------
JIT[   4] Size:      16.00 MB ExE Time:     35,772 us Throughput:     447.28 MB/s Passed
   [   0] Size:       4.00 MB VNEW:      6,326 us VLPR:     15,390 us VTIEIO:      3,037 us VSTART:     10,847 us VDEL:         67 us Throughput:     368.77 MB/s
   [   1] Size:       4.00 MB VNEW:         92 us VLPR:      3,094 us VTIEIO:      3,031 us VSTART:     11,059 us VDEL:      7,551 us Throughput:     361.70 MB/s
   [   2] Size:       4.00 MB VNEW:      6,452 us VLPR:      9,139 us VTIEIO:      3,046 us VSTART:     10,869 us VDEL:         42 us Throughput:     368.02 MB/s
   [   3] Size:       4.00 MB VNEW:      6,272 us VLPR:      3,086 us VTIEIO:      3,051 us VSTART:     10,790 us VDEL:      1,573 us Throughput:     370.71 MB/s
   -----------------------------------------------------------------------------------
JIT[   8] Size:      16.00 MB ExE Time:     55,514 us Throughput:     288.22 MB/s Passed
   [   0] Size:       2.00 MB VNEW:        478 us VLPR:      3,098 us VTIEIO:      3,041 us VSTART:      6,184 us VDEL:     24,748 us Throughput:     323.42 MB/s
   [   1] Size:       2.00 MB VNEW:     12,810 us VLPR:     15,425 us VTIEIO:      3,033 us VSTART:      5,539 us VDEL:     13,104 us Throughput:     361.08 MB/s
   [   2] Size:       2.00 MB VNEW:      6,879 us VLPR:      8,990 us VTIEIO:      3,043 us VSTART:      5,561 us VDEL:     13,030 us Throughput:     359.65 MB/s
   [   3] Size:       2.00 MB VNEW:      6,664 us VLPR:      3,081 us VTIEIO:      3,021 us VSTART:      5,528 us VDEL:     19,182 us Throughput:     361.79 MB/s
   [   4] Size:       2.00 MB VNEW:      6,653 us VLPR:     33,699 us VTIEIO:      3,064 us VSTART:      5,540 us VDEL:        762 us Throughput:     361.01 MB/s
   [   5] Size:       2.00 MB VNEW:      6,424 us VLPR:     14,704 us VTIEIO:      3,045 us VSTART:      5,573 us VDEL:      7,063 us Throughput:     358.87 MB/s
   [   6] Size:       2.00 MB VNEW:     12,123 us VLPR:     21,310 us VTIEIO:      3,057 us VSTART:      5,583 us VDEL:      6,915 us Throughput:     358.23 MB/s
   [   7] Size:       2.00 MB VNEW:     36,391 us VLPR:      9,025 us VTIEIO:      3,058 us VSTART:      5,544 us VDEL:         43 us Throughput:     360.75 MB/s
   -----------------------------------------------------------------------------------
JIT[  16] Size:      16.00 MB ExE Time:    114,740 us Throughput:     139.45 MB/s Passed
   [   0] Size:       1.00 MB VNEW:        126 us VLPR:      3,068 us VTIEIO:      3,067 us VSTART:      4,189 us VDEL:     53,758 us Throughput:     238.72 MB/s
   [   1] Size:       1.00 MB VNEW:      7,434 us VLPR:     20,584 us VTIEIO:      3,046 us VSTART:      3,544 us VDEL:     48,093 us Throughput:     282.17 MB/s
   [   2] Size:       1.00 MB VNEW:      7,352 us VLPR:     14,520 us VTIEIO:      3,049 us VSTART:      2,882 us VDEL:     73,393 us Throughput:     346.98 MB/s
   [   3] Size:       1.00 MB VNEW:      7,402 us VLPR:     26,664 us VTIEIO:      3,029 us VSTART:      2,874 us VDEL:     42,692 us Throughput:     347.95 MB/s
   [   4] Size:       1.00 MB VNEW:      5,452 us VLPR:     91,447 us VTIEIO:      3,025 us VSTART:      2,935 us VDEL:      7,547 us Throughput:     340.72 MB/s
   [   5] Size:       1.00 MB VNEW:      5,539 us VLPR:      9,051 us VTIEIO:      3,032 us VSTART:      2,873 us VDEL:     79,488 us Throughput:     348.07 MB/s
   [   6] Size:       1.00 MB VNEW:      5,978 us VLPR:     53,475 us VTIEIO:      3,020 us VSTART:      2,926 us VDEL:     44,830 us Throughput:     341.76 MB/s
   [   7] Size:       1.00 MB VNEW:      5,571 us VLPR:     66,024 us VTIEIO:      3,052 us VSTART:      2,897 us VDEL:     21,832 us Throughput:     345.18 MB/s
   [   8] Size:       1.00 MB VNEW:      5,271 us VLPR:     78,236 us VTIEIO:      3,041 us VSTART:      2,850 us VDEL:     19,845 us Throughput:     350.88 MB/s
   [   9] Size:       1.00 MB VNEW:      5,260 us VLPR:     84,344 us VTIEIO:      3,045 us VSTART:      2,831 us VDEL:     13,728 us Throughput:     353.23 MB/s
   [  10] Size:       1.00 MB VNEW:      3,917 us VLPR:     60,578 us VTIEIO:      3,055 us VSTART:      2,893 us VDEL:     27,926 us Throughput:     345.66 MB/s
   [  11] Size:       1.00 MB VNEW:      4,701 us VLPR:     47,340 us VTIEIO:      3,058 us VSTART:      2,848 us VDEL:     51,314 us Throughput:     351.12 MB/s
   [  12] Size:       1.00 MB VNEW:      4,181 us VLPR:    100,855 us VTIEIO:      3,056 us VSTART:      2,906 us VDEL:         44 us Throughput:     344.12 MB/s
   [  13] Size:       1.00 MB VNEW:      3,407 us VLPR:     36,992 us VTIEIO:      3,050 us VSTART:      2,900 us VDEL:     61,025 us Throughput:     344.83 MB/s
   [  14] Size:       1.00 MB VNEW:      1,920 us VLPR:      3,098 us VTIEIO:      3,048 us VSTART:      2,861 us VDEL:     48,669 us Throughput:     349.53 MB/s
   [  15] Size:       1.00 MB VNEW:      2,293 us VLPR:     72,132 us VTIEIO:      3,051 us VSTART:      2,888 us VDEL:     26,662 us Throughput:     346.26 MB/s
*/

/* VMUL Non pip
Begin...
CPU[   0] Size:      16.00 MB ExE Time:      3,356 us Throughput:   4,767.58 MB/s
   -----------------------------------------------------------------------------------
JIT[   1] Size:      16.00 MB ExE Time:    133,418 us Throughput:     119.92 MB/s Passed
   [   0] Size:      16.00 MB VNEW:         97 us VLPR:      3,100 us VTIEIO:      3,040 us VSTART:    126,972 us VDEL:         70 us Throughput:     126.01 MB/s
   -----------------------------------------------------------------------------------
JIT[   2] Size:      16.00 MB ExE Time:     76,004 us Throughput:     210.52 MB/s Passed
   [   0] Size:       8.00 MB VNEW:         48 us VLPR:      3,094 us VTIEIO:      3,047 us VSTART:     63,571 us VDEL:         50 us Throughput:     125.84 MB/s
   [   1] Size:       8.00 MB VNEW:        101 us VLPR:      9,233 us VTIEIO:      3,038 us VSTART:     63,499 us VDEL:         53 us Throughput:     125.99 MB/s
   -----------------------------------------------------------------------------------
JIT[   4] Size:      16.00 MB ExE Time:     56,940 us Throughput:     281.00 MB/s Passed
   [   0] Size:       4.00 MB VNEW:        122 us VLPR:      3,115 us VTIEIO:      3,048 us VSTART:     32,432 us VDEL:         49 us Throughput:     123.33 MB/s
   [   1] Size:       4.00 MB VNEW:      6,362 us VLPR:      3,113 us VTIEIO:      3,041 us VSTART:     31,869 us VDEL:         39 us Throughput:     125.51 MB/s
   [   2] Size:       4.00 MB VNEW:      6,577 us VLPR:      9,068 us VTIEIO:      3,052 us VSTART:     31,868 us VDEL:         66 us Throughput:     125.52 MB/s
   [   3] Size:       4.00 MB VNEW:      6,354 us VLPR:     15,473 us VTIEIO:      3,040 us VSTART:     31,841 us VDEL:         57 us Throughput:     125.62 MB/s
   -----------------------------------------------------------------------------------
JIT[   8] Size:      16.00 MB ExE Time:     65,280 us Throughput:     245.10 MB/s Passed
   [   0] Size:       2.00 MB VNEW:         75 us VLPR:      3,068 us VTIEIO:      3,020 us VSTART:     16,938 us VDEL:     13,946 us Throughput:     118.08 MB/s
   [   1] Size:       2.00 MB VNEW:      6,486 us VLPR:     27,474 us VTIEIO:      3,022 us VSTART:     16,010 us VDEL:         19 us Throughput:     124.92 MB/s
   [   2] Size:       2.00 MB VNEW:      6,433 us VLPR:      3,081 us VTIEIO:      3,047 us VSTART:     15,998 us VDEL:     20,702 us Throughput:     125.02 MB/s
   [   3] Size:       2.00 MB VNEW:      6,424 us VLPR:      9,181 us VTIEIO:      3,013 us VSTART:     15,885 us VDEL:     14,712 us Throughput:     125.90 MB/s
   [   4] Size:       2.00 MB VNEW:      5,531 us VLPR:     33,623 us VTIEIO:      3,024 us VSTART:     15,943 us VDEL:         27 us Throughput:     125.45 MB/s
   [   5] Size:       2.00 MB VNEW:      6,080 us VLPR:     39,181 us VTIEIO:      3,032 us VSTART:     15,908 us VDEL:         60 us Throughput:     125.72 MB/s
   [   6] Size:       2.00 MB VNEW:      5,724 us VLPR:     20,861 us VTIEIO:      3,034 us VSTART:     15,925 us VDEL:      2,442 us Throughput:     125.59 MB/s
   [   7] Size:       2.00 MB VNEW:      5,157 us VLPR:     14,770 us VTIEIO:      3,047 us VSTART:     15,964 us VDEL:     12,009 us Throughput:     125.28 MB/s
   -----------------------------------------------------------------------------------
JIT[  16] Size:      16.00 MB ExE Time:    107,303 us Throughput:     149.11 MB/s Passed
   [   0] Size:       1.00 MB VNEW:        684 us VLPR:      3,066 us VTIEIO:      3,031 us VSTART:      8,640 us VDEL:     52,979 us Throughput:     115.74 MB/s
   [   1] Size:       1.00 MB VNEW:     56,301 us VLPR:     33,550 us VTIEIO:      3,047 us VSTART:      8,088 us VDEL:         72 us Throughput:     123.64 MB/s
   [   2] Size:       1.00 MB VNEW:        665 us VLPR:     21,591 us VTIEIO:      3,028 us VSTART:      8,015 us VDEL:     59,659 us Throughput:     124.77 MB/s
   [   3] Size:       1.00 MB VNEW:     12,987 us VLPR:      3,078 us VTIEIO:      3,065 us VSTART:      8,076 us VDEL:     59,567 us Throughput:     123.82 MB/s
   [   4] Size:       1.00 MB VNEW:        731 us VLPR:      9,098 us VTIEIO:      3,025 us VSTART:      8,377 us VDEL:     28,642 us Throughput:     119.37 MB/s
   [   5] Size:       1.00 MB VNEW:     43,370 us VLPR:     27,247 us VTIEIO:      3,035 us VSTART:      8,033 us VDEL:     16,784 us Throughput:     124.49 MB/s
   [   6] Size:       1.00 MB VNEW:     12,444 us VLPR:     27,478 us VTIEIO:      3,044 us VSTART:      8,412 us VDEL:     47,005 us Throughput:     118.88 MB/s
   [   7] Size:       1.00 MB VNEW:     12,419 us VLPR:     52,080 us VTIEIO:      3,032 us VSTART:      8,023 us VDEL:     16,906 us Throughput:     124.64 MB/s
   [   8] Size:       1.00 MB VNEW:     12,405 us VLPR:     21,362 us VTIEIO:      3,040 us VSTART:      8,025 us VDEL:     47,646 us Throughput:     124.61 MB/s
   [   9] Size:       1.00 MB VNEW:     30,657 us VLPR:     27,307 us VTIEIO:      3,026 us VSTART:      8,057 us VDEL:     22,710 us Throughput:     124.12 MB/s
   [  10] Size:       1.00 MB VNEW:     17,357 us VLPR:     27,631 us VTIEIO:      3,023 us VSTART:      8,071 us VDEL:     41,288 us Throughput:     123.90 MB/s
   [  11] Size:       1.00 MB VNEW:     17,423 us VLPR:     58,113 us VTIEIO:      3,035 us VSTART:      8,041 us VDEL:     10,575 us Throughput:     124.36 MB/s
   [  12] Size:       1.00 MB VNEW:     47,621 us VLPR:     33,367 us VTIEIO:      3,052 us VSTART:      8,094 us VDEL:      4,423 us Throughput:     123.55 MB/s
   [  13] Size:       1.00 MB VNEW:     22,904 us VLPR:     27,484 us VTIEIO:      3,033 us VSTART:      8,013 us VDEL:     28,830 us Throughput:     124.80 MB/s
   [  14] Size:       1.00 MB VNEW:     21,356 us VLPR:      3,068 us VTIEIO:      3,021 us VSTART:      8,044 us VDEL:     53,478 us Throughput:     124.32 MB/s
   [  15] Size:       1.00 MB VNEW:     46,233 us VLPR:     45,816 us VTIEIO:      3,043 us VSTART:      8,034 us VDEL:         33 us Throughput:     124.47 MB/s
*/

/*DES
Begin...
CPU[   0] Size:      16.00 MB ExE Time:      3,345 us Throughput:   4,783.26 MB/s
   -----------------------------------------------------------------------------------
JIT[   1] Size:      16.00 MB ExE Time:     50,262 us Throughput:     318.33 MB/s No Check
   [   0] Size:      16.00 MB VNEW:        133 us VLPR:      3,109 us VTIEIO:      3,027 us VSTART:     43,828 us VDEL:         50 us Throughput:     365.06 MB/s
   -----------------------------------------------------------------------------------
JIT[   2] Size:      16.00 MB ExE Time:     34,651 us Throughput:     461.75 MB/s No Check
   [   0] Size:       8.00 MB VNEW:         80 us VLPR:      3,091 us VTIEIO:      3,036 us VSTART:     21,591 us VDEL:         50 us Throughput:     370.52 MB/s
   [   1] Size:       8.00 MB VNEW:         77 us VLPR:      9,197 us VTIEIO:      3,048 us VSTART:     22,163 us VDEL:         71 us Throughput:     360.96 MB/s
   -----------------------------------------------------------------------------------
JIT[   4] Size:      16.00 MB ExE Time:     35,773 us Throughput:     447.26 MB/s No Check
   [   0] Size:       4.00 MB VNEW:         61 us VLPR:      3,040 us VTIEIO:      3,021 us VSTART:     10,850 us VDEL:      7,769 us Throughput:     368.66 MB/s
   [   1] Size:       4.00 MB VNEW:      6,192 us VLPR:      9,268 us VTIEIO:      3,062 us VSTART:     10,810 us VDEL:         42 us Throughput:     370.03 MB/s
   [   2] Size:       4.00 MB VNEW:      6,140 us VLPR:     15,456 us VTIEIO:      3,048 us VSTART:     10,957 us VDEL:         42 us Throughput:     365.06 MB/s
   [   3] Size:       4.00 MB VNEW:      6,094 us VLPR:      3,115 us VTIEIO:      3,047 us VSTART:     10,794 us VDEL:      1,625 us Throughput:     370.58 MB/s
   -----------------------------------------------------------------------------------
JIT[   8] Size:      16.00 MB ExE Time:     55,495 us Throughput:     288.31 MB/s No Check
   [   0] Size:       2.00 MB VNEW:        101 us VLPR:      3,068 us VTIEIO:      3,045 us VSTART:      5,581 us VDEL:      6,879 us Throughput:     358.36 MB/s
   [   1] Size:       2.00 MB VNEW:      6,275 us VLPR:      3,067 us VTIEIO:      3,054 us VSTART:      5,570 us VDEL:     13,026 us Throughput:     359.07 MB/s
   [   2] Size:       2.00 MB VNEW:     12,473 us VLPR:      9,231 us VTIEIO:      3,028 us VSTART:      5,554 us VDEL:     19,113 us Throughput:     360.10 MB/s
   [   3] Size:       2.00 MB VNEW:     12,712 us VLPR:     15,012 us VTIEIO:      3,047 us VSTART:      5,590 us VDEL:     12,894 us Throughput:     357.78 MB/s
   [   4] Size:       2.00 MB VNEW:      6,441 us VLPR:      8,991 us VTIEIO:      3,051 us VSTART:      5,499 us VDEL:     25,403 us Throughput:     363.70 MB/s
   [   5] Size:       2.00 MB VNEW:     30,421 us VLPR:     15,282 us VTIEIO:      3,034 us VSTART:      5,689 us VDEL:         74 us Throughput:     351.56 MB/s
   [   6] Size:       2.00 MB VNEW:     11,940 us VLPR:     21,357 us VTIEIO:      3,018 us VSTART:      5,527 us VDEL:      6,888 us Throughput:     361.86 MB/s
   [   7] Size:       2.00 MB VNEW:     11,530 us VLPR:     27,239 us VTIEIO:      3,021 us VSTART:      5,568 us VDEL:        818 us Throughput:     359.20 MB/s
   -----------------------------------------------------------------------------------
JIT[  16] Size:      16.00 MB ExE Time:    102,687 us Throughput:     155.81 MB/s No Check
   [   0] Size:       1.00 MB VNEW:      1,004 us VLPR:      3,094 us VTIEIO:      3,048 us VSTART:      3,024 us VDEL:     52,393 us Throughput:     330.69 MB/s
   [   1] Size:       1.00 MB VNEW:      7,301 us VLPR:     15,425 us VTIEIO:      3,025 us VSTART:      2,872 us VDEL:     64,849 us Throughput:     348.19 MB/s
   [   2] Size:       1.00 MB VNEW:      1,230 us VLPR:      9,013 us VTIEIO:      3,045 us VSTART:      2,892 us VDEL:     71,083 us Throughput:     345.78 MB/s
   [   3] Size:       1.00 MB VNEW:     13,631 us VLPR:     58,211 us VTIEIO:      3,047 us VSTART:      2,861 us VDEL:     22,361 us Throughput:     349.53 MB/s
   [   4] Size:       1.00 MB VNEW:     25,694 us VLPR:     39,805 us VTIEIO:      3,057 us VSTART:      2,871 us VDEL:     28,723 us Throughput:     348.31 MB/s
   [   5] Size:       1.00 MB VNEW:     12,763 us VLPR:     33,115 us VTIEIO:      3,046 us VSTART:      2,881 us VDEL:     46,609 us Throughput:     347.10 MB/s
   [   6] Size:       1.00 MB VNEW:     12,197 us VLPR:     15,329 us VTIEIO:      3,050 us VSTART:      2,846 us VDEL:     34,049 us Throughput:     351.37 MB/s
   [   7] Size:       1.00 MB VNEW:     12,180 us VLPR:      3,086 us VTIEIO:      3,049 us VSTART:      2,815 us VDEL:     77,900 us Throughput:     355.24 MB/s
   [   8] Size:       1.00 MB VNEW:     12,665 us VLPR:     63,938 us VTIEIO:      3,044 us VSTART:      2,862 us VDEL:     15,818 us Throughput:     349.41 MB/s
   [   9] Size:       1.00 MB VNEW:     11,633 us VLPR:     27,010 us VTIEIO:      3,056 us VSTART:      2,870 us VDEL:     52,729 us Throughput:     348.43 MB/s
   [  10] Size:       1.00 MB VNEW:     11,311 us VLPR:     39,180 us VTIEIO:      3,029 us VSTART:      3,864 us VDEL:     33,237 us Throughput:     258.80 MB/s
   [  11] Size:       1.00 MB VNEW:     10,617 us VLPR:     70,066 us VTIEIO:      3,049 us VSTART:      2,853 us VDEL:     10,153 us Throughput:     350.51 MB/s
   [  12] Size:       1.00 MB VNEW:      9,246 us VLPR:     21,410 us VTIEIO:      3,023 us VSTART:      2,888 us VDEL:     58,820 us Throughput:     346.26 MB/s
   [  13] Size:       1.00 MB VNEW:     64,658 us VLPR:     27,570 us VTIEIO:      3,046 us VSTART:      2,905 us VDEL:         52 us Throughput:     344.23 MB/s
   [  14] Size:       1.00 MB VNEW:     51,747 us VLPR:     33,603 us VTIEIO:      3,048 us VSTART:      2,905 us VDEL:      4,089 us Throughput:     344.23 MB/s
   [  15] Size:       1.00 MB VNEW:     14,091 us VLPR:     39,794 us VTIEIO:      3,039 us VSTART:      2,863 us VDEL:     28,113 us Throughput:     349.28 MB/s
*/

/* CPU:VMUL, FPGA:BB_VMUL
Begin...

   -----------------------------------------------------------------------------------
CPU[   1] Size:      16.00 MB ExE Time:      8,283 us Throughput:   1,931.67 MB/s
   [   0] Size:      16.00 MB ExE Time:      8,161 us Throughput:   1,960.54 MB/s
   -----------------------------------------------------------------------------------
JIT[   1] Size:      16.00 MB ExE Time:     49,960 us Throughput:     320.26 MB/s No Check
   [   0] Size:      16.00 MB VNEW:        122 us VLPR:      3,050 us VTIEIO:      3,012 us VSTART:     43,687 us VDEL:         46 us Throughput:     366.24 MB/s

   -----------------------------------------------------------------------------------
CPU[   2] Size:      16.00 MB ExE Time:      4,225 us Throughput:   3,786.98 MB/s
   [   0] Size:       8.00 MB ExE Time:      2,070 us Throughput:   3,864.73 MB/s
   [   1] Size:       8.00 MB ExE Time:      4,153 us Throughput:   1,926.32 MB/s
   -----------------------------------------------------------------------------------
JIT[   2] Size:      16.00 MB ExE Time:     34,519 us Throughput:     463.51 MB/s No Check
   [   0] Size:       8.00 MB VNEW:         59 us VLPR:      3,032 us VTIEIO:      3,011 us VSTART:     21,537 us VDEL:         43 us Throughput:     371.45 MB/s
   [   1] Size:       8.00 MB VNEW:         66 us VLPR:      9,152 us VTIEIO:      3,046 us VSTART:     22,139 us VDEL:         53 us Throughput:     361.35 MB/s

   -----------------------------------------------------------------------------------
CPU[   4] Size:      16.00 MB ExE Time:      4,543 us Throughput:   3,521.90 MB/s
   [   0] Size:       4.00 MB ExE Time:      2,508 us Throughput:   1,594.90 MB/s
   [   1] Size:       4.00 MB ExE Time:      2,143 us Throughput:   1,866.54 MB/s
   [   2] Size:       4.00 MB ExE Time:      4,432 us Throughput:     902.53 MB/s
   [   3] Size:       4.00 MB ExE Time:      2,775 us Throughput:   1,441.44 MB/s
   -----------------------------------------------------------------------------------
JIT[   4] Size:      16.00 MB ExE Time:     35,441 us Throughput:     451.45 MB/s No Check
   [   0] Size:       4.00 MB VNEW:         85 us VLPR:      3,058 us VTIEIO:      3,037 us VSTART:     10,948 us VDEL:      7,463 us Throughput:     365.36 MB/s
   [   1] Size:       4.00 MB VNEW:      6,226 us VLPR:      3,088 us VTIEIO:      3,022 us VSTART:     10,776 us VDEL:      1,451 us Throughput:     371.20 MB/s
   [   2] Size:       4.00 MB VNEW:      6,233 us VLPR:     15,203 us VTIEIO:      3,022 us VSTART:     10,787 us VDEL:         23 us Throughput:     370.82 MB/s
   [   3] Size:       4.00 MB VNEW:      6,404 us VLPR:      8,947 us VTIEIO:      3,030 us VSTART:     10,811 us VDEL:         31 us Throughput:     369.99 MB/s

   -----------------------------------------------------------------------------------
CPU[   8] Size:      16.00 MB ExE Time:      2,741 us Throughput:   5,837.29 MB/s
   [   0] Size:       2.00 MB ExE Time:      1,453 us Throughput:   1,376.46 MB/s
   [   1] Size:       2.00 MB ExE Time:      1,369 us Throughput:   1,460.92 MB/s
   [   2] Size:       2.00 MB ExE Time:      1,530 us Throughput:   1,307.19 MB/s
   [   3] Size:       2.00 MB ExE Time:      1,539 us Throughput:   1,299.55 MB/s
   [   4] Size:       2.00 MB ExE Time:      2,296 us Throughput:     871.08 MB/s
   [   5] Size:       2.00 MB ExE Time:      1,747 us Throughput:   1,144.82 MB/s
   [   6] Size:       2.00 MB ExE Time:        846 us Throughput:   2,364.07 MB/s
   [   7] Size:       2.00 MB ExE Time:      1,374 us Throughput:   1,455.60 MB/s
   -----------------------------------------------------------------------------------
JIT[   8] Size:      16.00 MB ExE Time:     55,507 us Throughput:     288.25 MB/s No Check
   [   0] Size:       2.00 MB VNEW:         48 us VLPR:      3,072 us VTIEIO:      3,035 us VSTART:      6,132 us VDEL:     18,767 us Throughput:     326.16 MB/s
   [   1] Size:       2.00 MB VNEW:      6,850 us VLPR:     21,059 us VTIEIO:      3,051 us VSTART:      5,581 us VDEL:     12,979 us Throughput:     358.36 MB/s
   [   2] Size:       2.00 MB VNEW:      6,393 us VLPR:      3,073 us VTIEIO:      3,016 us VSTART:      5,449 us VDEL:     31,576 us Throughput:     367.04 MB/s
   [   3] Size:       2.00 MB VNEW:      6,418 us VLPR:      9,157 us VTIEIO:      3,046 us VSTART:      5,560 us VDEL:     25,392 us Throughput:     359.71 MB/s
   [   4] Size:       2.00 MB VNEW:      6,819 us VLPR:     14,921 us VTIEIO:      3,047 us VSTART:      5,555 us VDEL:     19,288 us Throughput:     360.04 MB/s
   [   5] Size:       2.00 MB VNEW:      5,804 us VLPR:     27,650 us VTIEIO:      3,039 us VSTART:      5,514 us VDEL:      7,046 us Throughput:     362.71 MB/s
   [   6] Size:       2.00 MB VNEW:      6,178 us VLPR:     33,399 us VTIEIO:      3,048 us VSTART:      5,583 us VDEL:        787 us Throughput:     358.23 MB/s
   [   7] Size:       2.00 MB VNEW:      5,426 us VLPR:     40,000 us VTIEIO:      3,020 us VSTART:      5,601 us VDEL:         41 us Throughput:     357.08 MB/s

   -----------------------------------------------------------------------------------
CPU[  16] Size:      16.00 MB ExE Time:      3,293 us Throughput:   4,858.79 MB/s
   [   0] Size:       1.00 MB ExE Time:        749 us Throughput:   1,335.11 MB/s
   [   1] Size:       1.00 MB ExE Time:        624 us Throughput:   1,602.56 MB/s
   [   2] Size:       1.00 MB ExE Time:        695 us Throughput:   1,438.85 MB/s
   [   3] Size:       1.00 MB ExE Time:        824 us Throughput:   1,213.59 MB/s
   [   4] Size:       1.00 MB ExE Time:        894 us Throughput:   1,118.57 MB/s
   [   5] Size:       1.00 MB ExE Time:      1,175 us Throughput:     851.06 MB/s
   [   6] Size:       1.00 MB ExE Time:      1,405 us Throughput:     711.74 MB/s
   [   7] Size:       1.00 MB ExE Time:      1,010 us Throughput:     990.10 MB/s
   [   8] Size:       1.00 MB ExE Time:      1,018 us Throughput:     982.32 MB/s
   [   9] Size:       1.00 MB ExE Time:      1,397 us Throughput:     715.82 MB/s
   [  10] Size:       1.00 MB ExE Time:      1,975 us Throughput:     506.33 MB/s
   [  11] Size:       1.00 MB ExE Time:      1,017 us Throughput:     983.28 MB/s
   [  12] Size:       1.00 MB ExE Time:        866 us Throughput:   1,154.73 MB/s
   [  13] Size:       1.00 MB ExE Time:      2,199 us Throughput:     454.75 MB/s
   [  14] Size:       1.00 MB ExE Time:      1,277 us Throughput:     783.09 MB/s
   [  15] Size:       1.00 MB ExE Time:        967 us Throughput:   1,034.13 MB/s
   -----------------------------------------------------------------------------------
JIT[  16] Size:      16.00 MB ExE Time:    101,507 us Throughput:     157.62 MB/s No Check
   [   0] Size:       1.00 MB VNEW:         72 us VLPR:      3,116 us VTIEIO:      3,070 us VSTART:      4,226 us VDEL:     45,146 us Throughput:     236.63 MB/s
   [   1] Size:       1.00 MB VNEW:      6,509 us VLPR:      3,066 us VTIEIO:      3,057 us VSTART:      2,819 us VDEL:     40,156 us Throughput:     354.74 MB/s
   [   2] Size:       1.00 MB VNEW:      7,503 us VLPR:     57,246 us VTIEIO:      3,029 us VSTART:      2,861 us VDEL:     22,597 us Throughput:     349.53 MB/s
   [   3] Size:       1.00 MB VNEW:      6,527 us VLPR:     52,103 us VTIEIO:      3,027 us VSTART:      2,847 us VDEL:     27,846 us Throughput:     351.25 MB/s
   [   4] Size:       1.00 MB VNEW:      6,003 us VLPR:      9,175 us VTIEIO:      3,058 us VSTART:      2,875 us VDEL:     70,713 us Throughput:     347.83 MB/s
   [   5] Size:       1.00 MB VNEW:      5,940 us VLPR:     88,974 us VTIEIO:      3,027 us VSTART:      2,804 us VDEL:         24 us Throughput:     356.63 MB/s
   [   6] Size:       1.00 MB VNEW:      6,633 us VLPR:     26,565 us VTIEIO:      3,036 us VSTART:      2,838 us VDEL:     52,989 us Throughput:     352.36 MB/s
   [   7] Size:       1.00 MB VNEW:      6,514 us VLPR:     20,482 us VTIEIO:      3,050 us VSTART:      2,786 us VDEL:     58,698 us Throughput:     358.94 MB/s
   [   8] Size:       1.00 MB VNEW:      5,615 us VLPR:     14,423 us VTIEIO:      3,043 us VSTART:      2,828 us VDEL:     64,726 us Throughput:     353.61 MB/s
   [   9] Size:       1.00 MB VNEW:      5,782 us VLPR:     44,828 us VTIEIO:      3,037 us VSTART:      2,914 us VDEL:     33,904 us Throughput:     343.17 MB/s
   [  10] Size:       1.00 MB VNEW:      5,138 us VLPR:     63,326 us VTIEIO:      3,020 us VSTART:      2,835 us VDEL:     15,862 us Throughput:     352.73 MB/s
   [  11] Size:       1.00 MB VNEW:      4,518 us VLPR:     75,480 us VTIEIO:      3,048 us VSTART:      2,865 us VDEL:      4,301 us Throughput:     349.04 MB/s
   [  12] Size:       1.00 MB VNEW:      4,199 us VLPR:     81,571 us VTIEIO:      3,029 us VSTART:      2,834 us VDEL:      3,472 us Throughput:     352.86 MB/s
   [  13] Size:       1.00 MB VNEW:      3,470 us VLPR:     69,379 us VTIEIO:      3,053 us VSTART:      2,878 us VDEL:     10,384 us Throughput:     347.46 MB/s
   [  14] Size:       1.00 MB VNEW:      2,971 us VLPR:     32,683 us VTIEIO:      3,047 us VSTART:      2,862 us VDEL:     46,443 us Throughput:     349.41 MB/s
   [  15] Size:       1.00 MB VNEW:      2,296 us VLPR:     38,761 us VTIEIO:      3,020 us VSTART:      3,544 us VDEL:     39,364 us Throughput:     282.17 MB/s

*/

/* VMUL
Begin...
[DEBUG->Download] Loading Static bit on 0 FPGA: './acc_static.bit' ...
[DEBUG->Download] Loading Static bit on 1 FPGA: './acc_static.bit' ...

   -----------------------------------------------------------------------------------
CPU[   1] Size:      32.00 MB ExE Time:     13,745 us Throughput:   2,328.12 MB/s
   [   0] Size:      32.00 MB ExE Time:     13,601 us Throughput:   2,352.77 MB/s
   -----------------------------------------------------------------------------------
JIT[   1] Size:      32.00 MB ExE Time:     97,273 us Throughput:     328.97 MB/s No Check
   [   0] Size:      32.00 MB VNEW:         58 us VLPR:      6,725 us VTIEIO:      3,021 us VSTART:     87,392 us VDEL:         41 us Throughput:     366.17 MB/s

   -----------------------------------------------------------------------------------
CPU[   2] Size:      32.00 MB ExE Time:      9,668 us Throughput:   3,309.89 MB/s
   [   0] Size:      16.00 MB ExE Time:      7,416 us Throughput:   2,157.50 MB/s
   [   1] Size:      16.00 MB ExE Time:      9,593 us Throughput:   1,667.88 MB/s
   -----------------------------------------------------------------------------------
JIT[   2] Size:      32.00 MB ExE Time:     60,012 us Throughput:     533.23 MB/s No Check
   [   0] Size:      16.00 MB VNEW:         39 us VLPR:      3,104 us VTIEIO:      3,041 us VSTART:     42,865 us VDEL:         43 us Throughput:     373.26 MB/s
   [   1] Size:      16.00 MB VNEW:         74 us VLPR:     13,251 us VTIEIO:      3,049 us VSTART:     43,478 us VDEL:         69 us Throughput:     368.00 MB/s

   -----------------------------------------------------------------------------------
CPU[   4] Size:      32.00 MB ExE Time:      8,637 us Throughput:   3,704.99 MB/s
   [   0] Size:       8.00 MB ExE Time:      4,541 us Throughput:   1,761.73 MB/s
   [   1] Size:       8.00 MB ExE Time:      3,245 us Throughput:   2,465.33 MB/s
   [   2] Size:       8.00 MB ExE Time:      4,103 us Throughput:   1,949.79 MB/s
   [   3] Size:       8.00 MB ExE Time:      5,193 us Throughput:   1,540.54 MB/s
   -----------------------------------------------------------------------------------
JIT[   4] Size:      32.00 MB ExE Time:     55,441 us Throughput:     577.19 MB/s No Check
   [   0] Size:       8.00 MB VNEW:        152 us VLPR:      3,091 us VTIEIO:      3,025 us VSTART:     21,540 us VDEL:      5,289 us Throughput:     371.40 MB/s
   [   1] Size:       8.00 MB VNEW:      6,353 us VLPR:     23,654 us VTIEIO:      3,035 us VSTART:     22,220 us VDEL:         45 us Throughput:     360.04 MB/s
   [   2] Size:       8.00 MB VNEW:      6,316 us VLPR:      3,124 us VTIEIO:      3,084 us VSTART:     21,511 us VDEL:         35 us Throughput:     371.90 MB/s
   [   3] Size:       8.00 MB VNEW:      6,255 us VLPR:     13,473 us VTIEIO:      3,042 us VSTART:     21,515 us VDEL:         28 us Throughput:     371.83 MB/s

   -----------------------------------------------------------------------------------
CPU[   8] Size:      32.00 MB ExE Time:      6,865 us Throughput:   4,661.33 MB/s
   [   0] Size:       4.00 MB ExE Time:      2,382 us Throughput:   1,679.26 MB/s
   [   1] Size:       4.00 MB ExE Time:      2,349 us Throughput:   1,702.85 MB/s
   [   2] Size:       4.00 MB ExE Time:      3,463 us Throughput:   1,155.07 MB/s
   [   3] Size:       4.00 MB ExE Time:      2,244 us Throughput:   1,782.53 MB/s
   [   4] Size:       4.00 MB ExE Time:      3,427 us Throughput:   1,167.20 MB/s
   [   5] Size:       4.00 MB ExE Time:      2,646 us Throughput:   1,511.72 MB/s
   [   6] Size:       4.00 MB ExE Time:      2,090 us Throughput:   1,913.88 MB/s
   [   7] Size:       4.00 MB ExE Time:      2,422 us Throughput:   1,651.53 MB/s
   -----------------------------------------------------------------------------------
JIT[   8] Size:      32.00 MB ExE Time:     76,824 us Throughput:     416.54 MB/s No Check
   [   0] Size:       4.00 MB VNEW:        813 us VLPR:      3,050 us VTIEIO:      3,028 us VSTART:     11,295 us VDEL:     37,484 us Throughput:     354.14 MB/s
   [   1] Size:       4.00 MB VNEW:      1,090 us VLPR:      8,904 us VTIEIO:      3,021 us VSTART:     10,787 us VDEL:     31,887 us Throughput:     370.82 MB/s
   [   2] Size:       4.00 MB VNEW:      1,099 us VLPR:     25,079 us VTIEIO:      3,022 us VSTART:     10,778 us VDEL:     15,676 us Throughput:     371.13 MB/s
   [   3] Size:       4.00 MB VNEW:      1,071 us VLPR:     41,388 us VTIEIO:      3,033 us VSTART:     10,801 us VDEL:      9,634 us Throughput:     370.34 MB/s
   [   4] Size:       4.00 MB VNEW:     13,195 us VLPR:     23,162 us VTIEIO:      3,012 us VSTART:     10,864 us VDEL:      5,479 us Throughput:     368.19 MB/s
   [   5] Size:       4.00 MB VNEW:     11,988 us VLPR:     39,528 us VTIEIO:      3,011 us VSTART:     10,799 us VDEL:         24 us Throughput:     370.40 MB/s
   [   6] Size:       4.00 MB VNEW:     28,137 us VLPR:     33,641 us VTIEIO:      3,021 us VSTART:     10,801 us VDEL:         26 us Throughput:     370.34 MB/s
   [   7] Size:       4.00 MB VNEW:     11,978 us VLPR:      6,979 us VTIEIO:      3,025 us VSTART:     10,799 us VDEL:     21,751 us Throughput:     370.40 MB/s

   -----------------------------------------------------------------------------------
CPU[  16] Size:      32.00 MB ExE Time:      5,599 us Throughput:   5,715.31 MB/s
   [   0] Size:       2.00 MB ExE Time:      1,525 us Throughput:   1,311.48 MB/s
   [   1] Size:       2.00 MB ExE Time:      1,179 us Throughput:   1,696.35 MB/s
   [   2] Size:       2.00 MB ExE Time:      1,689 us Throughput:   1,184.13 MB/s
   [   3] Size:       2.00 MB ExE Time:      2,089 us Throughput:     957.40 MB/s
   [   4] Size:       2.00 MB ExE Time:      1,801 us Throughput:   1,110.49 MB/s
   [   5] Size:       2.00 MB ExE Time:      1,580 us Throughput:   1,265.82 MB/s
   [   6] Size:       2.00 MB ExE Time:      2,348 us Throughput:     851.79 MB/s
   [   7] Size:       2.00 MB ExE Time:      2,128 us Throughput:     939.85 MB/s
   [   8] Size:       2.00 MB ExE Time:      1,659 us Throughput:   1,205.55 MB/s
   [   9] Size:       2.00 MB ExE Time:      2,701 us Throughput:     740.47 MB/s
   [  10] Size:       2.00 MB ExE Time:      2,252 us Throughput:     888.10 MB/s
   [  11] Size:       2.00 MB ExE Time:      1,713 us Throughput:   1,167.54 MB/s
   [  12] Size:       2.00 MB ExE Time:      3,777 us Throughput:     529.52 MB/s
   [  13] Size:       2.00 MB ExE Time:      1,510 us Throughput:   1,324.50 MB/s
   [  14] Size:       2.00 MB ExE Time:      2,728 us Throughput:     733.14 MB/s
   [  15] Size:       2.00 MB ExE Time:      2,750 us Throughput:     727.27 MB/s
   -----------------------------------------------------------------------------------
JIT[  16] Size:      32.00 MB ExE Time:    125,474 us Throughput:     255.03 MB/s No Check
   [   0] Size:       2.00 MB VNEW:        793 us VLPR:     21,479 us VTIEIO:      3,044 us VSTART:      5,571 us VDEL:     33,481 us Throughput:     359.00 MB/s
   [   1] Size:       2.00 MB VNEW:        895 us VLPR:      9,089 us VTIEIO:      3,020 us VSTART:      5,583 us VDEL:     19,205 us Throughput:     358.23 MB/s
   [   2] Size:       2.00 MB VNEW:        748 us VLPR:      3,076 us VTIEIO:      3,051 us VSTART:      5,544 us VDEL:     19,124 us Throughput:     360.75 MB/s
   [   3] Size:       2.00 MB VNEW:      6,911 us VLPR:     21,527 us VTIEIO:      3,044 us VSTART:      5,566 us VDEL:     33,482 us Throughput:     359.32 MB/s
   [   4] Size:       2.00 MB VNEW:     63,990 us VLPR:     45,818 us VTIEIO:      3,055 us VSTART:      5,515 us VDEL:        731 us Throughput:     362.65 MB/s
   [   5] Size:       2.00 MB VNEW:     12,040 us VLPR:     91,086 us VTIEIO:      3,044 us VSTART:      5,622 us VDEL:      6,870 us Throughput:     355.75 MB/s
   [   6] Size:       2.00 MB VNEW:     11,932 us VLPR:      3,063 us VTIEIO:      3,036 us VSTART:      5,554 us VDEL:     19,257 us Throughput:     360.10 MB/s
   [   7] Size:       2.00 MB VNEW:     12,027 us VLPR:     21,542 us VTIEIO:      3,043 us VSTART:      5,533 us VDEL:     37,455 us Throughput:     361.47 MB/s
   [   8] Size:       2.00 MB VNEW:     30,666 us VLPR:     35,618 us VTIEIO:      3,052 us VSTART:      5,562 us VDEL:     31,305 us Throughput:     359.58 MB/s
   [   9] Size:       2.00 MB VNEW:     17,133 us VLPR:     21,655 us VTIEIO:      3,052 us VSTART:      5,568 us VDEL:     31,260 us Throughput:     359.20 MB/s
   [  10] Size:       2.00 MB VNEW:     35,935 us VLPR:     39,627 us VTIEIO:      3,014 us VSTART:      5,585 us VDEL:     21,138 us Throughput:     358.10 MB/s
   [  11] Size:       2.00 MB VNEW:    105,803 us VLPR:      8,775 us VTIEIO:      3,038 us VSTART:      5,592 us VDEL:         37 us Throughput:     357.65 MB/s
   [  12] Size:       2.00 MB VNEW:     22,709 us VLPR:     25,422 us VTIEIO:      3,051 us VSTART:      5,524 us VDEL:     47,741 us Throughput:     362.06 MB/s
   [  13] Size:       2.00 MB VNEW:     22,280 us VLPR:     62,120 us VTIEIO:      3,047 us VSTART:      5,585 us VDEL:     10,955 us Throughput:     358.10 MB/s
   [  14] Size:       2.00 MB VNEW:     21,587 us VLPR:     72,331 us VTIEIO:      3,021 us VSTART:      5,568 us VDEL:      6,884 us Throughput:     359.20 MB/s
   [  15] Size:       2.00 MB VNEW:     21,319 us VLPR:     35,478 us VTIEIO:      3,048 us VSTART:      5,642 us VDEL:     49,865 us Throughput:     354.48 MB/s
*/


  // long int start_t = 1000000 * task_pkg[0].start.tv_sec + task_pkg[0].start.tv_usec;
  // long int end_t   = 1000000 * task_pkg[0].end.tv_sec   + task_pkg[0].end.tv_usec;
  // long int stmp;
  // long int etmp;

  // for (i = 0; i < THREADS; i++) {
  //   stmp = 1000000 * task_pkg[i].start.tv_sec + task_pkg[i].start.tv_usec;
  //   start_t = start_t > stmp ? stmp : start_t;

  //   etmp = 1000000 * task_pkg[i].end.tv_sec   + task_pkg[i].end.tv_usec;
  //   end_t = end_t < etmp ? etmp : end_t;

  //   // printf("   [%4d] start %'10ld | end %'10ld |\r\n", i, stmp, etmp);
  //   // printf("   [%4d] start %'10ld, %'10ld | end %'10ld, %'10ld\r\n", i, task_pkg[i].start.tv_sec, task_pkg[i].start.tv_usec, task_pkg[i].end.tv_sec, task_pkg[i].end.tv_usec);
  // }
  // // printf("   [%4d] start %'10ld | end %'10ld |\r\n", i, start_t, end_t);
  // timeuse = 0;
  // for (i = 0; i < THREADS; i++) {
  //   timeuse = timeuse < task_pkg[i].timeuse ? task_pkg[i].timeuse : timeuse;
  //   printf("   [%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", i, MB, task_pkg[i].timeuse, ((task_pkg[i].len * 4.0 / 1024 / 1024) / (task_pkg[i].timeuse * 1.0 / 1000000)));
  // }
  // printf("JIT[%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", THREADS, MB, timeuse, ((SIZE * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));
  // timeuse = end_t - start_t;
  // printf("JIT[%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", THREADS, MB, timeuse, ((SIZE * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));
  // printf("\r\n");
  // VAM_TABLE_SHOW(VM);

  // pthread_t thread[THREADS];
  // task_pk_t task_pkg[THREADS];


  // for (i = 0; i < THREADS; i++) {
  //   task_pkg[i].task_id      = i;
  //   task_pkg[i].VM           = &VM;
  //   task_pkg[i].nPR          = &nPR[i];
  //   task_pkg[i].len          = SIZE / THREADS;
  //   task_pkg[i].PR0_In1      = &A[i * task_pkg[i].len];
  //   task_pkg[i].SizePR0_In1  = task_pkg[i].len;
  //   task_pkg[i].PR0_In2      = &B[i * task_pkg[i].len];
  //   task_pkg[i].SizePR0_In2  = task_pkg[i].len;
  //   task_pkg[i].PR0_Out      = &D[i * task_pkg[i].len];
  //   task_pkg[i].SizePR0_Out  = task_pkg[i].len;
  // }

  // gettimeofday(&start, NULL);
  // for (i = 0; i < THREADS; i++) {
  //   pthread_create(&thread[i], NULL, VMUL_Threads_Call, (void *)&task_pkg[i]);
  // }
  // for (i = 0; i < THREADS; i++) {
  //   pthread_join(thread[i], NULL);
  // }
  // gettimeofday(&end, NULL);
  // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("JIT %'4d threads               :\t%'9d us\r\n", THREADS, timeuse);
  // printf("JIT TP                         :\t  %'7.2f MB/s\r\n", ((SIZE * 1 * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));




/*


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







*/












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
05/10/16, VADD, CPU Bandwidth
================================================================================
CPU[   0] Size:       1.00 MB ExE Time:        260 us Throughput:   3,846.15 MB/s
CPU[   1] Size:       2.00 MB ExE Time:        498 us Throughput:   4,016.06 MB/s
CPU[   2] Size:       4.00 MB ExE Time:      1,236 us Throughput:   3,236.25 MB/s
CPU[   3] Size:       8.00 MB ExE Time:      2,250 us Throughput:   3,555.56 MB/s
CPU[   4] Size:      16.00 MB ExE Time:      4,154 us Throughput:   3,851.71 MB/s
CPU[   5] Size:      32.00 MB ExE Time:      7,414 us Throughput:   4,316.16 MB/s
CPU[   6] Size:      64.00 MB ExE Time:     13,869 us Throughput:   4,614.61 MB/s
CPU[   7] Size:     128.00 MB ExE Time:     26,067 us Throughput:   4,910.42 MB/s
CPU[   8] Size:     256.00 MB ExE Time:     51,982 us Throughput:   4,924.78 MB/s
CPU[   9] Size:     512.00 MB ExE Time:    103,293 us Throughput:   4,956.77 MB/s
CPU[  10] Size:   1,024.00 MB ExE Time:    205,136 us Throughput:   4,991.81 MB/s
CPU[  11] Size:   2,048.00 MB ExE Time:    412,765 us Throughput:   4,961.66 MB/s
*/



/*
05/10/16, VMUL, CPU Bandwidth
================================================================================
CPU[   0] Size:       1.00 MB ExE Time:        296 us Throughput:   3,378.38 MB/s
CPU[   1] Size:       2.00 MB ExE Time:        514 us Throughput:   3,891.05 MB/s
CPU[   2] Size:       4.00 MB ExE Time:      1,192 us Throughput:   3,355.70 MB/s
CPU[   3] Size:       8.00 MB ExE Time:      2,215 us Throughput:   3,611.74 MB/s
CPU[   4] Size:      16.00 MB ExE Time:      4,218 us Throughput:   3,793.27 MB/s
CPU[   5] Size:      32.00 MB ExE Time:      7,602 us Throughput:   4,209.42 MB/s
CPU[   6] Size:      64.00 MB ExE Time:     13,559 us Throughput:   4,720.11 MB/s
CPU[   7] Size:     128.00 MB ExE Time:     26,037 us Throughput:   4,916.08 MB/s
CPU[   8] Size:     256.00 MB ExE Time:     52,367 us Throughput:   4,888.57 MB/s
CPU[   9] Size:     512.00 MB ExE Time:    102,844 us Throughput:   4,978.41 MB/s
CPU[  10] Size:   1,024.00 MB ExE Time:    207,790 us Throughput:   4,928.05 MB/s
CPU[  11] Size:   2,048.00 MB ExE Time:    412,177 us Throughput:   4,968.74 MB/s
*/


/*
05/10/16, Insertion, CPU Bandwidth
================================================================================
CPU[   0] Size:         64 Words  ExE Time:          3 us Throughput:      81.38 MB/s
CPU[   1] Size:        128 Words  ExE Time:         10 us Throughput:      48.83 MB/s
CPU[   2] Size:        256 Words  ExE Time:         33 us Throughput:      29.59 MB/s
CPU[   3] Size:        512 Words  ExE Time:        134 us Throughput:      14.58 MB/s
CPU[   4] Size:      1,024 Words  ExE Time:        462 us Throughput:       8.46 MB/s
CPU[   5] Size:      2,048 Words  ExE Time:      1,791 us Throughput:       4.36 MB/s
CPU[   6] Size:      4,096 Words  ExE Time:      2,825 us Throughput:       5.53 MB/s
CPU[   7] Size:      8,192 Words  ExE Time:      9,461 us Throughput:       3.30 MB/s
CPU[   8] Size:     16,384 Words  ExE Time:     38,916 us Throughput:       1.61 MB/s
CPU[   9] Size:     32,768 Words  ExE Time:    144,257 us Throughput:       0.87 MB/s
CPU[  10] Size:     65,536 Words  ExE Time:    571,018 us Throughput:       0.44 MB/s
CPU[  11] Size:    131,072 Words  ExE Time:  2,284,959 us Throughput:       0.22 MB/s
CPU[  12] Size:    262,144 Words  ExE Time:  9,145,255 us Throughput:       0.11 MB/s
CPU[  14] Size:    524,288 Words  ExE Time: 36,613,254 us Throughput:       0.05 MB/s
*/

/*
05/10/16, Merge, CPU Bandwidth
================================================================================
CPU[   0] Size:       0.50 MB ExE Time:     15,152 us Throughput:      33.00 MB/s
CPU[   1] Size:       1.00 MB ExE Time:     29,189 us Throughput:      34.26 MB/s
CPU[   2] Size:       2.00 MB ExE Time:     58,571 us Throughput:      34.15 MB/s
CPU[   3] Size:       4.00 MB ExE Time:    117,865 us Throughput:      33.94 MB/s
CPU[   4] Size:       8.00 MB ExE Time:    238,083 us Throughput:      33.60 MB/s
CPU[   5] Size:      16.00 MB ExE Time:    483,264 us Throughput:      33.11 MB/s
CPU[   6] Size:      32.00 MB ExE Time:    983,780 us Throughput:      32.53 MB/s
CPU[   7] Size:      64.00 MB ExE Time:  1,996,641 us Throughput:      32.05 MB/s
CPU[   8] Size:     128.00 MB ExE Time:  4,049,896 us Throughput:      31.61 MB/s
CPU[   9] Size:     256.00 MB ExE Time:  8,209,354 us Throughput:      31.18 MB/s
*/


/*
05/10/16, Radix, CPU Bandwidth
================================================================================
CPU[   0] Size:       1.00 MB ExE Time:      7,101 us Throughput:     140.83 MB/s
CPU[   1] Size:       2.00 MB ExE Time:     14,378 us Throughput:     139.10 MB/s
CPU[   2] Size:       4.00 MB ExE Time:     27,730 us Throughput:     144.25 MB/s
CPU[   3] Size:       8.00 MB ExE Time:     52,295 us Throughput:     152.98 MB/s
CPU[   4] Size:      16.00 MB ExE Time:    105,093 us Throughput:     152.25 MB/s
CPU[   5] Size:      32.00 MB ExE Time:    210,243 us Throughput:     152.20 MB/s
CPU[   6] Size:      64.00 MB ExE Time:    420,823 us Throughput:     152.08 MB/s
CPU[   7] Size:     128.00 MB ExE Time:    839,623 us Throughput:     152.45 MB/s
CPU[   8] Size:     256.00 MB ExE Time:  1,679,017 us Throughput:     152.47 MB/s
CPU[   9] Size:     512.00 MB ExE Time:  3,357,308 us Throughput:     152.50 MB/s
*/





/*

CPU[   0] Size:       1.00 MB ExE Time:        253 us Throughput:   3,952.57 MB/s
JIT[   0] Size:       1.00 MB ExE Time:      2,872 us Throughput:     348.19 MB/s Passed!

CPU[   1] Size:       2.00 MB ExE Time:        240 us Throughput:   8,333.33 MB/s
JIT[   1] Size:       2.00 MB ExE Time:      5,471 us Throughput:     365.56 MB/s Passed!

CPU[   2] Size:       4.00 MB ExE Time:        872 us Throughput:   4,587.16 MB/s
JIT[   2] Size:       4.00 MB ExE Time:     10,888 us Throughput:     367.38 MB/s Passed!

CPU[   3] Size:       8.00 MB ExE Time:      1,898 us Throughput:   4,214.96 MB/s
JIT[   3] Size:       8.00 MB ExE Time:     21,887 us Throughput:     365.51 MB/s Passed!

CPU[   4] Size:      16.00 MB ExE Time:      3,599 us Throughput:   4,445.68 MB/s
JIT[   4] Size:      16.00 MB ExE Time:     43,603 us Throughput:     366.95 MB/s Passed!

CPU[   5] Size:      32.00 MB ExE Time:      6,727 us Throughput:   4,756.95 MB/s
JIT[   5] Size:      32.00 MB ExE Time:     87,628 us Throughput:     365.18 MB/s Passed!

CPU[   6] Size:      64.00 MB ExE Time:     13,001 us Throughput:   4,922.70 MB/s
JIT[   6] Size:      64.00 MB ExE Time:    174,425 us Throughput:     366.92 MB/s Passed!

*/


// void * threads_call(void* ptr)
// {
//   pe_t *p = (pe_t *)ptr;
//   // printf("Key:%4d\tLen:%4d\tA:%p\tB:%p\tC:%p\r\n", p->key, p->len, p->A, p->B, p->C);
//   int i;
//   for (i = 0; i < p->len; i++) {
//     p->C[i] = p->A[i] - p->B[i];
//   }
//   return (void*) 0;
// }

/*
void * VMUL_Threads_Call(void *pk)
{
#ifdef VERBOSE_THREAD
  printf("\r\n");
#endif
  task_pk_t *p      = (task_pk_t*) pk;
#ifdef VERBOSE_THREAD
  printf("[DEBUG->task_thread:%2d] task thread start\r\n", p->task_id);
#endif
  struct timeval start, end;
  int timeuse;
  vam_vm_t    *VM      = p->VM;
  vector<int> *nPR     = p->nPR;
  // int         items    = p->len;
  // int         *In1     = p->In1;
  // int         sizeIn1  ;
  // int         *In2     = p->In2;
  // int         sizeIn2  ;
  // int         *Out     = p->Out;
  // int         sizeOut  ;
  int         err;

  // int         iter     = items / MAX_SIZE;
  // int         reminder = items - iter * MAX_SIZE;

    gettimeofday(&start, NULL);
    err =   vnew(VM, nPR);                                                                                                errCheck(err, FUN_VNEW);
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("[DEBUG->task_thread:%2d] VNEW   : \t%'9d us\r\n", p->task_id, timeuse);

    gettimeofday(&start, NULL);
    err =   vlpr(VM, nPR->at(0), VADD);                                                                                   errCheck(err, FUN_VLPR);
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("[DEBUG->task_thread:%2d] VLPR   : \t%'9d us\r\n", p->task_id, timeuse);

    gettimeofday(&start, NULL);
    err = vtieio(VM, nPR->at(0), p->PR0_In1, p->SizePR0_In1, p->PR0_In2, p->SizePR0_In2, p->PR0_Out, p->SizePR0_Out);     errCheck(err, FUN_VTIEIO);
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("[DEBUG->task_thread:%2d] VTIEIO : \t%'9d us\r\n", p->task_id, timeuse);

    gettimeofday(&start, NULL);
    err = vstart(VM, nPR);                                                                                                errCheck(err, FUN_VSTART);
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("[DEBUG->task_thread:%2d] VSTRAT : \t%'9d us\r\n", p->task_id, timeuse);
    printf("[DEBUG->task_thread:%2d] TP     : \t  %'7.2f MB/s\r\n", p->task_id, ((p->SizePR0_Out * 1 * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));

    gettimeofday(&start, NULL);
    err =   vdel(VM, nPR);                                                                                                errCheck(err, FUN_VDEL);
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("[DEBUG->task_thread:%2d] VDEL   : \t%'9d us\r\n", p->task_id, timeuse);


// #ifdef VERBOSE_THREAD
//   printf("[DEBUG->task_thread:%2d] iter:%d\r\n", p->task_id, iter);
//   printf("[DEBUG->task_thread:%2d] reminder:%d\r\n", p->task_id, reminder);
// #endif

//   if (items >= MAX_SIZE) {
//     sizeIn1  = MAX_SIZE;
//     sizeIn2  = MAX_SIZE;
//     sizeOut  = MAX_SIZE;
//   }
//   else {
//     sizeIn1  = p->SizeIn1;
//     sizeIn2  = p->SizeIn2;
//     sizeOut  = p->SizeOut;
//   }

//   err =   vnew(VM, nPR);                                                        errCheck(err, FUN_VNEW);
//   err =   vlpr(VM, nPR->at(0),  VMUL);                                          errCheck(err, FUN_VLPR);
//   err = vtieio(VM, nPR->at(0),  In1,  In2,  Out, sizeIn1);                      errCheck(err, FUN_VTIEIO);

//   for (int i = 0; i < iter; i++) {
//     err = vstart(VM, nPR, MAX_SIZE, MAX_SIZE, MAX_SIZE);                        errCheck(err, FUN_VSTART);
//   }

//   if (reminder != 0) {
//     err = vtieio(VM, nPR->at(0),  In1,  In2,  Out, reminder);                      errCheck(err, FUN_VTIEIO);
//     err = vstart(VM, nPR, reminder, reminder, reminder);                        errCheck(err, FUN_VSTART);
//   }
//   err =   vdel(VM, nPR);                                                        errCheck(err, FUN_VDEL);
#ifdef VERBOSE_THREAD
  printf("[DEBUG->task_thread] task thread done\r\n");
#endif
  return NULL;
}
*/



/*
  for (j = 0; j < MAXSIZE; j++) {
    SIZE = SIZE * 2;
    float KB = SIZE * 4.0 / 1024;
    float MB = KB         / 1024;
    float GB = MB         / 1024;
    // printf("SIZE: %'5d Words, %'5d Bytes, %'5.1f KB, %'5.1f MB, %'5.1f GB\r\n", SIZE, SIZE * 4, KB, MB, GB);



    vector<vector<int> > nPR(THREADS);
    for (i = 0; i < THREADS; i++) {
      nPR[i].resize(STEPS);
    }

    // task_pk_t task_pkg[THREADS];
    task_pk_t *pk = new task_pk_t;

    pk->task_id      = 0;
    pk->VM           = &VM;
    pk->nPR          = &nPR[0];
    pk->len          = SIZE / THREADS;
    pk->PR0_In1      = &A[0 * pk->len];
    pk->SizePR0_In1  = pk->len;
    pk->PR0_In2      = &B[0 * pk->len];
    pk->SizePR0_In2  = pk->len;
    pk->PR0_Out      = &D[0 * pk->len];
    pk->SizePR0_Out  = pk->len;

    vector<int>  *PPR;
    PPR = &nPR[0];

    task_pk_t *p      = (task_pk_t*) pk;
    gettimeofday(&start, NULL);
    err =   vnew(&VM, PPR);                                                                                                errCheck(err, FUN_VNEW);
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("[DEBUG->task_thread:%2d] VNEW   : \t%'9d us\r\n", p->task_id, timeuse);

    gettimeofday(&start, NULL);
    err =   vlpr(&VM, PPR->at(0), VADD);                                                                                   errCheck(err, FUN_VLPR);
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("[DEBUG->task_thread:%2d] VLPR   : \t%'9d us\r\n", p->task_id, timeuse);

    gettimeofday(&start, NULL);
    err = vtieio(&VM, PPR->at(0), p->PR0_In1, p->SizePR0_In1, p->PR0_In2, p->SizePR0_In2, p->PR0_Out, p->SizePR0_Out);     errCheck(err, FUN_VTIEIO);
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("[DEBUG->task_thread:%2d] VTIEIO : \t%'9d us\r\n", p->task_id, timeuse);

    gettimeofday(&start, NULL);
    err = vstart(&VM, PPR);                                                                                                errCheck(err, FUN_VSTART);
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("[DEBUG->task_thread:%2d] VSTRAT : \t%'9d us\r\n", p->task_id, timeuse);
    // printf("[DEBUG->task_thread:%2d] TP     : \t  %'7.2f MB/s\r\n", p->task_id, ((p->SizePR0_Out * 1 * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));
    printf("JIT[%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\t", j, MB, timeuse, ((p->SizePR0_Out * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));

    gettimeofday(&start, NULL);
    err =   vdel(&VM, PPR);                                                                                                errCheck(err, FUN_VDEL);
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("[DEBUG->task_thread:%2d] VDEL   : \t%'9d us\r\n", p->task_id, timeuse);

    for (i = 0; i < SIZE; i++) {
      // printf("%d:\tA:%d\tB:%d\tC:%d\tD:%d\r\n", i, A[i], B[i], C[i], D[i]);
      if (C[i] != D[i]) {
        printf("Error at %d:\tA:%d\tB:%d\tC:%d\tD:%d\r\nFailed!\r\n", i, A[i], B[i], C[i], D[i]);
        VAM_VM_CLEAN(&VM);
        delete[] pk;
        delete[] A;
        delete[] B;
        delete[] C;
        delete[] D;
        exit(1);
      }
    }
    printf("Passed!\r\n\r\n");
    delete[] pk;
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] D;
    //////////////////////////////////////////////////////////////////////////////
  }
  */
