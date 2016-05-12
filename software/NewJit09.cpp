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

#define THREADS 16
#define SIZE    1024 * 1024 / 4 * 128
// #define SWSIZE  SIZE
#define STEPS   1

typedef struct {
  int             task_id;
  vam_vm_t        *VM;
  vector<int>     *nPR;
  struct timeval  start;
  struct timeval  end;
  int             timeuse;
  int             len;
  int             *PR0_In1;
  int          SizePR0_In1;
  int             *PR0_In2;
  int          SizePR0_In2;
  int             *PR0_Out;
  int          SizePR0_Out;
}task_pk_t;

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

void printArray(int * array, int size){

  int i;
  printf("[ ");
  for (i = 0; i < size; i++)
    printf("%d ", array[i]);
  printf("]\n");
}

int findLargestNum(int * array, int size){

  int i;
  int largestNum = -1;

  for(i = 0; i < size; i++){
    if(array[i] > largestNum)
      largestNum = array[i];
  }

  return largestNum;
}

// Radix Sort
void radixSort(int * array, int size){

  // printf("\n\nRunning Radix Sort on Unsorted List!\n\n");

  // Base 10 is used
  int i;
  int *semiSorted = new int[sizeof(int) * size];
  int significantDigit = 1;
  int largestNum = findLargestNum(array, size);
  // printf("%d, %f\r\n", largestNum, largestNum * 1.0 / significantDigit);
  // Loop until we reach the largest significant digit
  while (largestNum / significantDigit > 0){

    // printf("\tSorting: %d's place ", significantDigit);
    // printArray(array, size);

    int bucket[10] = { 0 };

    // Counts the number of "keys" or digits that will go into each bucket
    for (i = 0; i < size; i++)
      bucket[(array[i] / significantDigit) % 10]++;

    /**
     * Add the count of the previous buckets,
     * Acquires the indexes after the end of each bucket location in the array
     * Works similar to the count sort algorithm
     **/
    for (i = 1; i < 10; i++)
      bucket[i] += bucket[i - 1];

    // Use the bucket to fill a "semiSorted" array
    for (i = size - 1; i >= 0; i--)
      semiSorted[--bucket[(array[i] / significantDigit) % 10]] = array[i];


    for (i = 0; i < size; i++)
      array[i] = semiSorted[i];

    // Move to next significant digit
    significantDigit *= 10;

    // printf("\n\tBucket: ");
    // printArray(bucket, 10);
  }
  delete[] semiSorted;
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

  err =   vnew(VM, nPR);                                                                                                errCheck(err, FUN_VNEW);
  err =   vlpr(VM, nPR->at(0), VADD);                                                                                   errCheck(err, FUN_VLPR);
  err = vtieio(VM, nPR->at(0), p->PR0_In1, p->SizePR0_In1, p->PR0_In2, p->SizePR0_In2, p->PR0_Out, p->SizePR0_Out);     errCheck(err, FUN_VTIEIO);
  gettimeofday(&p->start, NULL);
  err = vstart(VM, nPR);                                                                                                errCheck(err, FUN_VSTART);
  gettimeofday(&p->end, NULL);
  p->timeuse = 1000000 * (p->end.tv_sec - p->start.tv_sec) + p->end.tv_usec - p->start.tv_usec;
  err =   vdel(VM, nPR);


#ifdef VERBOSE_THREAD
  printf("[DEBUG->task_thread:%2d] Thread Done\r\n", p->task_id);
#endif
  return NULL;
}

int FindMaxTime(int array[], int size)
{
  int i;
  int MAX = 0;
  for (i = 1; i < size; i++) {
    if (array[MAX] < array[i]) MAX = i;
  }
  return array[MAX];
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
  gettimeofday(&start, NULL);
  for (i = 0; i < SIZE; i++) {
    C[i] = A[i] + B[i];
    // C[i] = A[i] * B[i];
  }
  // MergeSort(A, SIZE);
  // InsertionSort(A, SIZE);
  // radixSort(A, SIZE);
  gettimeofday(&end, NULL);
  int timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  printf("CPU[%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", j, MB, timeuse, ((SIZE * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));
  // printf("CPU[%4d] Size: %'10d Words\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", j, SIZE, timeuse, ((SIZE * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));
  // printf("CPU %'4d threads               :\t%'9d us\r\n", 1, timeuse);
  // printf("CPU TP                         :\t %'7.2f MB/s\r\n", ((SIZE * 1 * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));
  // for (i = 0; i < 10; i++) {
    // printf("%'5d:\tA:%'5d\tB:%'5d\tC:%'5d\tD:%'5d\r\n", i, A[SIZE - (10 - i)], B[SIZE - (10 - i)], C[SIZE - (10 - i)], D[SIZE - (10 - i)]);
  // }
////////////////////////////////////////////////////////////////////////////////////////////////////
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
    task_pkg[i].timeuse = 0;
    task_pkg[i].len     = SIZE / THREADS;

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
    // printf("[%3d]|\tA:%6d\t|B:%6d\t|C:%6d\t|D:%6d\t|E:%6d|\r\n", i, A[i], B[i], C[i], D[i]);
    if (C[i] != D[i]) {
      printf("Error!\r\n[%3d]|\tA:%6d\t|B:%6d\t|C:%6d\t|D:%6d\t|\r\n", i, A[i], B[i], C[i], D[i]);
      flag = 1;
      break;
    }
  }

  long int start_t = 1000000 * task_pkg[0].start.tv_sec + task_pkg[0].start.tv_usec;
  long int end_t   = 1000000 * task_pkg[0].end.tv_sec   + task_pkg[0].end.tv_usec;
  long int stmp;
  long int etmp;
  if (flag == 0) printf("Passed\r\n");
  for (i = 0; i < THREADS; i++) {
    stmp = 1000000 * task_pkg[i].start.tv_sec + task_pkg[i].start.tv_usec;
    start_t = start_t > stmp ? stmp : start_t;

    etmp = 1000000 * task_pkg[i].end.tv_sec   + task_pkg[i].end.tv_usec;
    end_t = end_t < etmp ? etmp : end_t;

    // printf("   [%4d] start %'10ld | end %'10ld |\r\n", i, stmp, etmp);
    // printf("   [%4d] start %'10ld, %'10ld | end %'10ld, %'10ld\r\n", i, task_pkg[i].start.tv_sec, task_pkg[i].start.tv_usec, task_pkg[i].end.tv_sec, task_pkg[i].end.tv_usec);
  }
  // printf("   [%4d] start %'10ld | end %'10ld |\r\n", i, start_t, end_t);
  timeuse = 0;
  for (i = 0; i < THREADS; i++) {
    timeuse = timeuse < task_pkg[i].timeuse ? task_pkg[i].timeuse : timeuse;
    printf("   [%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", i, MB, task_pkg[i].timeuse, ((task_pkg[i].len * 4.0 / 1024 / 1024) / (task_pkg[i].timeuse * 1.0 / 1000000)));
  }
  printf("JIT[%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", THREADS, MB, timeuse, ((SIZE * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));
  timeuse = end_t - start_t;
  printf("JIT[%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", THREADS, MB, timeuse, ((SIZE * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));
  printf("\r\n");

////////////////////////////////////////////////////////////////////////////////////////////////////
  VAM_VM_CLEAN(&VM);
  delete[] A;
  delete[] B;
  delete[] C;
  delete[] D;
  return 0;
}

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
