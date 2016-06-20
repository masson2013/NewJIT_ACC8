#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <picodrv.h>
#include <pthread.h>
#include <sys/time.h>
#include <locale.h>
#include <pico_errors.h>
#include "jit_bit.h"

// #define SIZE 1024 * 1024 / 4 * 128 * 4
// #define SIZE 0x100000 / 4 * 32
#define SIZE 16
// #define TSTREAM
#define SORT_BB_32
#define VERBOSE
#define VERBOSE_THREAD
// #define AVAILABLE
// #define ACC1
#define PR

#define WS              0
#define RS              1

typedef struct
{
    PicoDrv     *Pico     ;
    int          Type     ;
    uint32_t     Stream   ;
    int         *In_Data  ;
    int         *Out_Data ;
    int          Size     ;
}task_pk_t;


void * Stream_Threads_Call(void *pk)
{
#ifdef VERBOSE_THREAD
  printf("\r\n");
#endif
  task_pk_t *p = (task_pk_t *)pk;
  int err, i;
  char      ibuf[1024];
  PicoDrv   *pico     = p->Pico;
  int       type      = p->Type;
  uint32_t  stream    = p->Stream;
  int       *In_Data  = p->In_Data;
  int       *Out_Data = p->Out_Data;
  int       items     = p->Size * 4;

  if (type == WS) {
    #ifdef VERBOSE_THREAD
      printf("[DEBUG->WS_TCALL] Writing %i Bytes to 0x%08x\n", items, stream);
    #endif

    err = pico->WriteStream(stream, In_Data, items);
    if (err < 0) {
      fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
      return (void *) -1;
    }
  } else if (type == RS) {
    #ifdef VERBOSE_THREAD
      #ifndef AVAILABLE
        printf("[DEBUG->RS_TCALL] Reading %i Bytes to 0x%08x\n", items, stream);
      #else
        printf("[DEBUG->RS_TCALL] Reading %i Bytes to 0x%08x\n", i=pico->GetBytesAvailable(stream, true), stream);
        if (i < 0){
            fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
            exit(1);
        }
      #endif
    #endif

    #ifndef AVAILABLE
      err = pico->ReadStream(stream, Out_Data, items);
    #else
      err = pico->ReadStream(stream, Out_Data, i);
    #endif

    if (err < 0) {
      fprintf(stderr, "ReadingStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
      return (void *) -1;
    }
  }
}

int main(int argc, char* argv[])
{
  printf("Begin...\r\n");
  setlocale(LC_NUMERIC, ""); // for thounds seperator
  float KB = SIZE * 4.0 / 1024;
  float MB = KB         / 1024;
  float GB = MB         / 1024;
  printf("SIZE: %'5d Words, %'5d Bytes, %'5.1f KB, %'5.1f MB, %'5.1f GB\r\n", SIZE, SIZE * 4, KB, MB, GB);

  int         err, i, j, stream, stream100;
  int         room;
  int         *tmp = NULL;
  int         stream11, stream12, stream13;
  int         stream21, stream22, stream23;
  int         stream31, stream32, stream33;
  int         stream41, stream42, stream43;
  int         stream51, stream52, stream53;
  int         stream61, stream62, stream63;
  int         stream71, stream72, stream73;
  int         stream81, stream82, stream83;

  PicoDrv     *pico;

  uint32_t    buf[1024], u32, addr;
  char        ibuf[1024];
  const char* bitFileName;
  printf("Start\r\n");
  // specify the .bit file name on the command line
    switch(argc) {

      case 1: {
        if ((err = FindPico(0x505, &pico)) < 0) {
            printf("FindPico Error\r\n");
            return err;
        }
      }break;

      case 2: {
        bitFileName = argv[1];
        printf("Loading FPGA with '%s' ...\n", bitFileName);
        err = RunBitFile(bitFileName, &pico);
        if (err < 0) {
            fprintf(stderr, "RunBitFile error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
            exit(1);
        }
      }break;
    }

  printf("Opening stream50 (CMD)\r\n");
  stream = pico->CreateStream(50);
  if (stream < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream100 (CMD)\r\n");
  stream100 = pico->CreateStream(100);
  if (stream < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream, ibuf, sizeof(ibuf)));
      exit(1);
  }
//==============================================================================
  int *A = new int[SIZE * 4];
  int *B = new int[SIZE * 4];
  int *C = new int[SIZE * 4];
  int *D = new int[SIZE * 4];
  int *E = new int[SIZE * 4];


  for (i = 0; i < SIZE * 4; i++) {
    A[i] = -2;
    B[i] = -2;
    C[i] = -2;
    D[i] = -2;
    E[i] = -2;
  }

  for (i = 0; i < SIZE; i++) {
    // A[i] = i * 2 + 0;
    // B[i] = i * 2 + 1;
    A[i] = i + 0;
    B[i] = i + 10;
  }

  // for (i = 0; i < SIZE; i++) {
  //   A[i] = 70     -  i * 2;
  //   B[i] = 70 - 1 -  i * 2;
  // }
  // for (i = 0; i < SIZE; i++) {
  //   A[SIZE + i] = 0xDEADBEEF;
  // }
  // for (i = 0; i < SIZE; i++) {
  //   B[SIZE + i] = 0xDEADBEEF;
  // }

  struct timeval start, end;
  gettimeofday(&start, NULL);
  for (i = 0; i < SIZE; i++) {
    C[i] = A[i] + B[i];
  }
  gettimeofday(&end, NULL);

  int timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  printf("CPU[%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\r\n", j, MB, timeuse, ((SIZE * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));

  j = 0;
  int k;
  int node;
  uint32_t  cmd[4]; // {0xC1100000, 0xC1200000, 0xC1300001, 0xB1000000};
//==================================================================================================
  #ifdef TSTREAM
    printf("Opening stream11\r\n");
    stream11 = pico->CreateStream(11);
    if (stream11 < 0) {
        fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream11, ibuf, sizeof(ibuf)));
        exit(1);
    }

    printf("Opening stream12\r\n");
    stream12 = pico->CreateStream(12);
    if (stream12 < 0) {
        fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream12, ibuf, sizeof(ibuf)));
        exit(1);
    }

    printf("Opening stream13\r\n");
    stream13 = pico->CreateStream(13);
    if (stream13 < 0) {
        fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream13, ibuf, sizeof(ibuf)));
        exit(1);
    }
    //----------------------------------------------------------------------------------------------
    node = 1;
    cmd[3] = 0xBABEFACE;
    cmd[2] = 0xDEADBEEF;
    cmd[1] = 0xDEADBEEF;
    cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
    printf("0x%08x\r\n", cmd[0]);
    pico->WriteStream(stream, cmd, 16);

    #ifdef PR
      printf("Doing PR: %d words, %dbytes\r\n", jit_blackbox_PR1_bit_len, jit_blackbox_PR1_bit_len * 4);
      pico->WriteStream(stream100, jit_blackbox_PR1_bit, jit_blackbox_PR1_bit_len * 4);
      usleep(500);
      printf("PR Done\r\n");
    #endif

    cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
    printf("0x%08x\r\n", cmd[0]);
    pico->WriteStream(stream, cmd, 16);
    //----------------------------------------------------------------------------------------------
    cmd[0] = 0xC1100000 | (SIZE >> 16       ) ;
    cmd[1] = 0xC1200000 | (SIZE & 0x0000FFFF) ;
    cmd[2] = 0xC1300012                       ;
    cmd[3] = 0xB1000111                       ;
    pico->WriteStream(stream, cmd, 16);
    //==========================================================================
    pthread_t thread[3];
    task_pk_t task_pkg[3];

    // Package for write
    task_pkg[0].Pico     = pico;
    task_pkg[0].Type     = WS;
    task_pkg[0].Stream   = stream11;
    task_pkg[0].In_Data  = A;
    task_pkg[0].Out_Data = NULL;
    task_pkg[0].Size     = SIZE;

    // Package for write
    task_pkg[1].Pico     = pico;
    task_pkg[1].Type     = WS;
    task_pkg[1].Stream   = stream12;
    task_pkg[1].In_Data  = B;
    task_pkg[1].Out_Data = NULL;
    task_pkg[1].Size     = SIZE;

    // Package for read
    task_pkg[2].Pico     = pico;
    task_pkg[2].Type     = RS;
    task_pkg[2].Stream   = stream13;
    task_pkg[2].In_Data  = NULL;
    task_pkg[2].Out_Data = D;
    task_pkg[2].Size     = SIZE * 2;

    gettimeofday(&start, NULL);

    pthread_create(&thread[2], NULL, Stream_Threads_Call, (void *)&task_pkg[2]);
    pthread_create(&thread[1], NULL, Stream_Threads_Call, (void *)&task_pkg[1]);
    pthread_create(&thread[0], NULL, Stream_Threads_Call, (void *)&task_pkg[0]);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);

    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("JIT[%4d] Size: %'10.2f MB\tExE Time: %'10d us\tThroughput: %'10.2f MB/s\t", j, MB, timeuse, ((SIZE * 4.0 / 1024 / 1024) / (timeuse * 1.0 / 1000000)));


    // pico->WriteStream(stream11, A, SIZE * 4);
    // pico->WriteStream(stream12, B, SIZE * 4);

    // printf("%i B available to read from 13.\n", i=pico->GetBytesAvailable(stream13, true));
    // if (i < 0){
    //     fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
    //     exit(1);
    // }
    // pico->ReadStream(stream13, D, i);
    //==========================================================================
    int flag = 0;
    printf("\r\n");
    for (i = 0; i < SIZE * 2; i++) {
      printf("[%3d]|\tA:%6d\t|B:%6d\t|C:%6d\t|D:%6d\t|E:%6d|\r\n", i, A[i], B[i], C[i], D[i], E[i]);
      // if (C[i] != D[i]) {
      //   printf("Error!\r\n[%3d]|\tA:%6d\t|B:%6d\t|C:%6d\t|D:%6d\t|E:%6d|\r\n", i, A[i], B[i], C[i], D[i], E[i]);
      //   flag = 1;
      //   break;
      // }
    }

    // if (flag == 0) printf("Passed\r\n");
    printf("No Check\r\n");
    printf("Closing stream11\r\n");
    pico->CloseStream(stream11);
    printf("Closing stream12\r\n");
    pico->CloseStream(stream12);
    printf("Closing stream13\r\n");
    pico->CloseStream(stream13);
  #endif
//==================================================================================================
#ifdef SORT_BB_32
  printf("Opening stream11\r\n");
  stream11 = pico->CreateStream(11);
  if (stream11 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream11, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream12\r\n");
  stream12 = pico->CreateStream(12);
  if (stream12 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream12, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream13\r\n");
  stream13 = pico->CreateStream(13);
  if (stream13 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream13, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream21\r\n");
  stream21 = pico->CreateStream(21);
  if (stream21 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream21, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream22\r\n");
  stream22 = pico->CreateStream(22);
  if (stream22 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream22, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream23\r\n");
  stream23 = pico->CreateStream(23);
  if (stream23 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream23, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream31\r\n");
  stream31 = pico->CreateStream(31);
  if (stream31 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream31, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream32\r\n");
  stream32 = pico->CreateStream(32);
  if (stream32 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream32, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream33\r\n");
  stream33 = pico->CreateStream(33);
  if (stream33 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream33, ibuf, sizeof(ibuf)));
      exit(1);
  }

  node = 1;
  cmd[3] = 0xBABEFACE;
  cmd[2] = 0xDEADBEEF;
  cmd[1] = 0xDEADBEEF;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

#ifdef PR
  printf("Doing PR: %d words, %dbytes\r\n", jit_blackbox_PR1_bit_len, jit_blackbox_PR1_bit_len * 4);
  pico->WriteStream(stream100, jit_blackbox_PR1_bit, jit_blackbox_PR1_bit_len * 4);
  usleep(500);
  printf("PR Done\r\n");
#endif

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);
///////////////////////////////////////

  node = 2;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

#ifdef PR
  printf("Doing PR: %d words, %dbytes\r\n", jit_blackbox_PR2_bit_len, jit_blackbox_PR2_bit_len * 4);
  pico->WriteStream(stream100, jit_blackbox_PR2_bit, jit_blackbox_PR2_bit_len * 4);
  usleep(500);
  printf("PR Done\r\n");
#endif

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);
///////////////////////////////////////

  node = 3;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

#ifdef PR
  printf("Doing PR: %d words, %dbytes\r\n", jit_blackbox_PR3_bit_len, jit_blackbox_PR3_bit_len * 4);
  pico->WriteStream(stream100, jit_blackbox_PR3_bit, jit_blackbox_PR3_bit_len * 4);
  usleep(500);
  printf("PR Done\r\n");
#endif

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);
///////////////////////////////////////

  cmd[0] = 0xC1100000;
  cmd[1] = 0xC1200000 | SIZE;
  cmd[2] = 0xC1301001;
  cmd[3] = 0xB1000112;
  // cmd[3] = 0xB1000F00;
  pico->WriteStream(stream, cmd, 16);

  cmd[0] = 0xC2100000;
  cmd[1] = 0xC2200000 | SIZE;
  cmd[2] = 0xC2301001;
  cmd[3] = 0xB2000112;
  // cmd[3] = 0xB2000F00;
  pico->WriteStream(stream, cmd, 16);

  cmd[0] = 0xC3100000;
  cmd[1] = 0xC3200000 | SIZE;
  cmd[2] = 0xC3300012;
  cmd[3] = 0xB3120221;
  // cmd[3] = 0xB3000021;
  pico->WriteStream(stream, cmd, 16);
//////////////////////

  pthread_t thread[3];
  task_pk_t task_pkg[3];

  // Package for write
  task_pkg[0].Pico     = pico;
  task_pkg[0].Type     = WS;
  task_pkg[0].Stream   = stream11;
  task_pkg[0].In_Data  = A;
  task_pkg[0].Out_Data = NULL;
  task_pkg[0].Size     = SIZE;

  // Package for write
  task_pkg[1].Pico     = pico;
  task_pkg[1].Type     = WS;
  task_pkg[1].Stream   = stream21;
  task_pkg[1].In_Data  = B;
  task_pkg[1].Out_Data = NULL;
  task_pkg[1].Size     = SIZE;

  // Package for read
  task_pkg[2].Pico     = pico;
  task_pkg[2].Type     = RS;
  task_pkg[2].Stream   = stream33;
  task_pkg[2].In_Data  = NULL;
  task_pkg[2].Out_Data = E;
  task_pkg[2].Size     = SIZE * 2;

  gettimeofday(&start, NULL);

  pthread_create(&thread[2], NULL, Stream_Threads_Call, (void *)&task_pkg[2]);
  pthread_create(&thread[1], NULL, Stream_Threads_Call, (void *)&task_pkg[1]);
  pthread_create(&thread[0], NULL, Stream_Threads_Call, (void *)&task_pkg[0]);

  pthread_join(thread[0], NULL);
  pthread_join(thread[1], NULL);
  pthread_join(thread[2], NULL);

  printf("\r\n");
  for (i = 0; i < SIZE * 2; i++) {
    printf("%3d:\tA:%10d\tB:%10d\tC:%10d\tD:%10d\tE:%10d\r\n", i, A[i], B[i], C[i], D[i], E[i]);
  }

  printf("Closing stream11\r\n");
  pico->CloseStream(stream11);
  printf("Closing stream12\r\n");
  pico->CloseStream(stream12);
  printf("Closing stream13\r\n");
  pico->CloseStream(stream13);
  printf("Closing stream21\r\n");
  pico->CloseStream(stream21);
  printf("Closing stream22\r\n");
  pico->CloseStream(stream22);
  printf("Closing stream23\r\n");
  pico->CloseStream(stream23);
  printf("Closing stream31\r\n");
  pico->CloseStream(stream31);
  printf("Closing stream32\r\n");
  pico->CloseStream(stream32);
  printf("Closing stream33\r\n");
  pico->CloseStream(stream33);
#endif
//==================================================================================================
#ifdef INSERTION
  printf("Opening stream11\r\n");
  stream11 = pico->CreateStream(11);
  if (stream11 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream11, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream12\r\n");
  stream12 = pico->CreateStream(12);
  if (stream12 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream12, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream13\r\n");
  stream13 = pico->CreateStream(13);
  if (stream13 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream13, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream21\r\n");
  stream21 = pico->CreateStream(21);
  if (stream21 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream21, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream22\r\n");
  stream22 = pico->CreateStream(22);
  if (stream22 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream22, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream23\r\n");
  stream23 = pico->CreateStream(23);
  if (stream23 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream23, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream31\r\n");
  stream31 = pico->CreateStream(31);
  if (stream31 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream31, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream32\r\n");
  stream32 = pico->CreateStream(32);
  if (stream32 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream32, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream33\r\n");
  stream33 = pico->CreateStream(33);
  if (stream33 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream33, ibuf, sizeof(ibuf)));
      exit(1);
  }

  node = 1;
  cmd[3] = 0xBABEFACE;
  cmd[2] = 0xDEADBEEF;
  cmd[1] = 0xDEADBEEF;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

#ifdef PR
  printf("Doing PR: %d words, %dbytes\r\n", InsertionUnit_PR1_bit_len, InsertionUnit_PR1_bit_len * 4);
  pico->WriteStream(stream100, InsertionUnit_PR1_bit, InsertionUnit_PR1_bit_len * 4);
  usleep(500);
  printf("PR Done\r\n");
#endif

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);
///////////////////////////////////////

  node = 2;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

#ifdef PR
  printf("Doing PR: %d words, %dbytes\r\n", InsertionUnit_PR2_bit_len, InsertionUnit_PR2_bit_len * 4);
  pico->WriteStream(stream100, InsertionUnit_PR2_bit, InsertionUnit_PR2_bit_len * 4);
  usleep(500);
  printf("PR Done\r\n");
#endif

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);
///////////////////////////////////////

  node = 3;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

#ifdef PR
  printf("Doing PR: %d words, %dbytes\r\n", MergeUnit_PR3_bit_len, MergeUnit_PR3_bit_len * 4);
  pico->WriteStream(stream100, MergeUnit_PR3_bit, MergeUnit_PR3_bit_len * 4);
  usleep(500);
  printf("PR Done\r\n");
#endif

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);
///////////////////////////////////////

  cmd[0] = 0xC1100000;
  cmd[1] = 0xC1200000 | SIZE * 2;
  cmd[2] = 0xC1300001;
  cmd[3] = 0xB1000F00;
  pico->WriteStream(stream, cmd, 16);

  cmd[0] = 0xC2100000;
  cmd[1] = 0xC2200000 | SIZE * 2;
  cmd[2] = 0xC2300001;
  cmd[3] = 0xB2000F00;
  pico->WriteStream(stream, cmd, 16);

  cmd[0] = 0xC3100000;
  cmd[1] = 0xC3200000 | (SIZE * 2);
  cmd[2] = 0xC3300001;
  cmd[3] = 0xB3000021;
  pico->WriteStream(stream, cmd, 16);
//////////////////////
  pico->WriteStream(stream11, A, SIZE * 2 * 4);
  pico->WriteStream(stream21, B, SIZE * 2 * 4);
//////////////////////

  printf("%i B available to read from 13.\n", i=pico->GetBytesAvailable(stream13, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
//  pico->ReadStream(stream13, C, i);
//////////////////////
  printf("%i B available to read from 23.\n", i=pico->GetBytesAvailable(stream23, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
//  pico->ReadStream(stream23, D, i);
//////////////////////
//  pico->WriteStream(stream31, C, SIZE * 2 * 4);
//  pico->WriteStream(stream32, D, SIZE * 2 * 4);
//////////////////////
  printf("%i B available to read from 33.\n", i=pico->GetBytesAvailable(stream33, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  pico->ReadStream(stream33, E, i);


  printf("\r\n");
  for (i = 0; i < SIZE * 2; i++) {
    printf("%3d:\tA:%10d\tB:%10d\tC:%10d\tD:%10d\tE:%10d\r\n", i, A[i], B[i], C[i], D[i], E[i]);
  }

  printf("Closing stream11\r\n");
  pico->CloseStream(stream11);
  printf("Closing stream12\r\n");
  pico->CloseStream(stream12);
  printf("Closing stream13\r\n");
  pico->CloseStream(stream13);
  printf("Closing stream21\r\n");
  pico->CloseStream(stream21);
  printf("Closing stream22\r\n");
  pico->CloseStream(stream22);
  printf("Closing stream23\r\n");
  pico->CloseStream(stream23);
  printf("Closing stream31\r\n");
  pico->CloseStream(stream31);
  printf("Closing stream32\r\n");
  pico->CloseStream(stream32);
  printf("Closing stream33\r\n");
  pico->CloseStream(stream33);
#endif
//==================================================================================================
//       _    ____ ____       _
//      / \  / ___/ ___|  _  / |
//     / _ \| |  | |     (_) | |
//    / ___ \ |__| |___   _  | |
//   /_/   \_\____\____| (_) |_|
//==================================================================================================
#ifdef ACC1
  printf("Opening stream11\r\n");
  stream11 = pico->CreateStream(11);
  if (stream11 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream11, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream12\r\n");
  stream12 = pico->CreateStream(12);
  if (stream12 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream12, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream13\r\n");
  stream13 = pico->CreateStream(13);
  if (stream13 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream13, ibuf, sizeof(ibuf)));
      exit(1);
  }

  node = 1;
  cmd[3] = 0xBABEFACE;
  cmd[2] = 0xDEADBEEF;
  cmd[1] = 0xDEADBEEF;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

#ifdef PR
  printf("Doing PR: %d words, %dbytes\r\n", jit_blackbox_PR1_bit_len, jit_blackbox_PR1_bit_len * 4);
  pico->WriteStream(stream100, jit_blackbox_PR1_bit, jit_blackbox_PR1_bit_len * 4);
  printf("PR Done\r\n");
#endif

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  // k = 0;
  // do {
  //   printf("%i B available to read from 50. %5d\r\n", i=pico->GetBytesAvailable(stream, true), k);
  //   if (i < 0){
  //       fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
  //       exit(1);
  //   }
  //   room = i / 4;
  //   tmp = new int[room];
  //   pico->ReadStream(stream, tmp, i);
  //   printf("%d\r\n", room);
  //   // for (i = 0; i < room; i++) {
  //   //   printf("[%d] 0x%08x\r\n", i, tmp[i]);
  //   // }
  //   delete[] tmp;
  //   k++;
  // } while (room != 0 && k < 10000);

  cmd[0] = 0xC1100000;
  cmd[1] = 0xC1200000 | SIZE;
  cmd[2] = 0xC1300001;
  cmd[3] = 0xB1000000;
  pico->WriteStream(stream, cmd, 16);

  printf("Lanching ACC\r\n");
  cmd[0] = 0xA1000000;
  cmd[1] = 0x00000000;
  cmd[2] = 0x00000000;
  cmd[3] = 0x00000000;
  pico->WriteStream(stream, cmd, 16);

  pico->WriteStream(stream11, A, SIZE * 2 * 4);
  // pico->WriteStream(stream12, B, SIZE * 4);
  printf("%i B available to read from 13.\n", i=pico->GetBytesAvailable(stream13, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  // i = SIZE * 4;
  room = i / 4;
  tmp = new int[room];
  pico->ReadStream(stream13, tmp, i);
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }
  delete[] tmp;

  // printf("%i B available to read from 50.\n", i=pico->GetBytesAvailable(stream, true));
  // if (i < 0){
  //     fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
  //     exit(1);
  // }
  // room = i / 4;
  // tmp = new int[room];
  // pico->ReadStream(stream, tmp, i);
  // printf("%d\r\n", room);
  // for (i = 0; i < room; i++) {
  //   printf("[%d] 0x%08x\r\n", i, tmp[i]);
  // }
  // delete[] tmp;
  printf("Closing stream11\r\n");
  pico->CloseStream(stream11);
  printf("Closing stream12\r\n");
  pico->CloseStream(stream12);
  printf("Closing stream13\r\n");
  pico->CloseStream(stream13);
#endif
//==================================================================================================
//       _    ____ ____       ____
//      / \  / ___/ ___|  _  |___ \
//     / _ \| |  | |     (_)   __) |
//    / ___ \ |__| |___   _   / __/
//   /_/   \_\____\____| (_) |_____|
//==================================================================================================
#ifdef ACC2
  printf("Opening stream21\r\n");
  stream21 = pico->CreateStream(21);
  if (stream21 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream21, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream22\r\n");
  stream22 = pico->CreateStream(22);
  if (stream22 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream22, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream23\r\n");
  stream23 = pico->CreateStream(23);
  if (stream23 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream23, ibuf, sizeof(ibuf)));
      exit(1);
  }

  node = 2;
  cmd[3] = 0xBABEFACE;
  cmd[2] = 0xDEADBEEF;
  cmd[1] = 0xDEADBEEF;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  printf("Doing PR: %d words, %dbytes\r\n", acc_vadd_PR2_bit_len, acc_vadd_PR2_bit_len * 4);
  pico->WriteStream(stream100, acc_vadd_PR2_bit, acc_vadd_PR2_bit_len * 4);
  printf("PR Done\r\n");

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  k = 0;
  do {
    printf("%i B available to read from 50. %5d\r\n", i=pico->GetBytesAvailable(stream, true), k);
    if (i < 0){
        fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
        exit(1);
    }
    room = i / 4;
    tmp = new int[room];
    pico->ReadStream(stream, tmp, i);
    printf("%d\r\n", room);
    // for (i = 0; i < room; i++) {
    //   printf("[%d] 0x%08x\r\n", i, tmp[i]);
    // }
    delete[] tmp;
    k++;
  } while (room != 0 && k < 10000);

  cmd[0] = 0xC2100000;
  cmd[1] = 0xC2200000 | SIZE;
  cmd[2] = 0xC2300001;
  cmd[3] = 0xB2000000;
  pico->WriteStream(stream, cmd, 16);

  printf("Lanching ACC\r\n");
  cmd[0] = 0xA2000000;
  cmd[1] = 0x00000000;
  cmd[2] = 0x00000000;
  cmd[3] = 0x00000000;
  pico->WriteStream(stream, cmd, 16);

  pico->WriteStream(stream21, A, SIZE * 4);
  pico->WriteStream(stream22, B, SIZE * 4);
  printf("%i B available to read from 23.\n", i=pico->GetBytesAvailable(stream23, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  pico->ReadStream(stream23, tmp, i);
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }
  delete[] tmp;

  printf("%i B available to read from 50.\n", i=pico->GetBytesAvailable(stream, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  pico->ReadStream(stream, tmp, i);
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }
  delete[] tmp;
  pico->CloseStream(stream21);
  pico->CloseStream(stream22);
  pico->CloseStream(stream23);
#endif
//==============================================================================
//       _    ____ ____       _____
//      / \  / ___/ ___|  _  |___ /
//     / _ \| |  | |     (_)   |_ \
//    / ___ \ |__| |___   _   ___) |
//   /_/   \_\____\____| (_) |____/
//==============================================================================
#ifdef ACC3
  printf("Opening stream31\r\n");
  stream31 = pico->CreateStream(31);
  if (stream31 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream31, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream32\r\n");
  stream32 = pico->CreateStream(32);
  if (stream32 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream32, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream33\r\n");
  stream33 = pico->CreateStream(33);
  if (stream33 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream33, ibuf, sizeof(ibuf)));
      exit(1);
  }

  node = 3;
  cmd[3] = 0xBABEFACE;
  cmd[2] = 0xDEADBEEF;
  cmd[1] = 0xDEADBEEF;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  printf("Doing PR: %d words, %dbytes\r\n", acc_vadd_PR3_bit_len, acc_vadd_PR3_bit_len * 4);
  pico->WriteStream(stream100, acc_vadd_PR3_bit, acc_vadd_PR3_bit_len * 4);
  printf("PR Done\r\n");

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  k = 0;
  do {
    printf("%i B available to read from 50. %5d\r\n", i=pico->GetBytesAvailable(stream, true), k);
    if (i < 0){
        fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
        exit(1);
    }
    room = i / 4;
    tmp = new int[room];
    pico->ReadStream(stream, tmp, i);
    printf("%d\r\n", room);
    // for (i = 0; i < room; i++) {
    //   printf("[%d] 0x%08x\r\n", i, tmp[i]);
    // }
    delete[] tmp;
    k++;
  } while (room != 0 && k < 10000);

  cmd[0] = 0xC3100000;
  cmd[1] = 0xC3200000 | SIZE;
  cmd[2] = 0xC3300001;
  cmd[3] = 0xB3000000;
  pico->WriteStream(stream, cmd, 16);

  printf("Lanching ACC\r\n");
  cmd[0] = 0xA3000000;
  cmd[1] = 0x00000000;
  cmd[2] = 0x00000000;
  cmd[3] = 0x00000000;
  pico->WriteStream(stream, cmd, 16);

  pico->WriteStream(stream31, A, SIZE * 4);
  pico->WriteStream(stream32, B, SIZE * 4);
  printf("%i B available to read from 33.\n", i=pico->GetBytesAvailable(stream33, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  pico->ReadStream(stream33, tmp, i);
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }
  delete[] tmp;

  printf("%i B available to read from 50.\n", i=pico->GetBytesAvailable(stream, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  pico->ReadStream(stream, tmp, i);
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }
  delete[] tmp;
  pico->CloseStream(stream31);
  pico->CloseStream(stream32);
  pico->CloseStream(stream33);
#endif
//==============================================================================
//       _    ____ ____       _  _
//      / \  / ___/ ___|  _  | || |
//     / _ \| |  | |     (_) | || |_
//    / ___ \ |__| |___   _  |__   _|
//   /_/   \_\____\____| (_)    |_|
//==============================================================================
#ifdef ACC4
  printf("Opening stream41\r\n");
  stream41 = pico->CreateStream(41);
  if (stream41 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream41, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream42\r\n");
  stream42 = pico->CreateStream(42);
  if (stream42 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream42, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream43\r\n");
  stream43 = pico->CreateStream(43);
  if (stream43 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream43, ibuf, sizeof(ibuf)));
      exit(1);
  }

  node = 4;
  cmd[3] = 0xBABEFACE;
  cmd[2] = 0xDEADBEEF;
  cmd[1] = 0xDEADBEEF;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  printf("Doing PR: %d words, %dbytes\r\n", acc_vadd_PR4_bit_len, acc_vadd_PR4_bit_len * 4);
  pico->WriteStream(stream100, acc_vadd_PR4_bit, acc_vadd_PR4_bit_len * 4);
  printf("PR Done\r\n");

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  k = 0;
  do {
    printf("%i B available to read from 50. %5d\r\n", i=pico->GetBytesAvailable(stream, true), k);
    if (i < 0){
        fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
        exit(1);
    }
    room = i / 4;
    tmp = new int[room];
    pico->ReadStream(stream, tmp, i);
    printf("%d\r\n", room);
    // for (i = 0; i < room; i++) {
    //   printf("[%d] 0x%08x\r\n", i, tmp[i]);
    // }
    delete[] tmp;
    k++;
  } while (room != 0 && k < 10000);

  cmd[0] = 0xC4100000;
  cmd[1] = 0xC4200000 | SIZE;
  cmd[2] = 0xC4300001;
  cmd[3] = 0xB4000000;
  pico->WriteStream(stream, cmd, 16);

  printf("Lanching ACC\r\n");
  cmd[0] = 0xA4000000;
  cmd[1] = 0x00000000;
  cmd[2] = 0x00000000;
  cmd[3] = 0x00000000;
  pico->WriteStream(stream, cmd, 16);

  err = pico->WriteStream(stream41, A, SIZE * 4);
  err = pico->WriteStream(stream42, B, SIZE * 4);
  printf("%i B available to read from stream43.\n", i=pico->GetBytesAvailable(stream43, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  err = pico->ReadStream(stream43, tmp, i);
  if (err < 0) {
      fprintf(stderr, "ReadStream stream43 error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
      exit(1);
  }
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }

  printf("%i B available to read from stream.\n", i=pico->GetBytesAvailable(stream, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  pico->ReadStream(stream, tmp, i);
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }
  delete[] tmp;
  pico->CloseStream(stream41);
  pico->CloseStream(stream42);
  pico->CloseStream(stream43);
#endif
//==============================================================================
//       _    ____ ____       ____
//      / \  / ___/ ___|  _  | ___|
//     / _ \| |  | |     (_) |___ \
//    / ___ \ |__| |___   _   ___) |
//   /_/   \_\____\____| (_) |____/
//==============================================================================
#ifdef ACC5
  printf("Opening stream51\r\n");
  stream51 = pico->CreateStream(51);
  if (stream51 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream51, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream52\r\n");
  stream52 = pico->CreateStream(52);
  if (stream52 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream52, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream53\r\n");
  stream53 = pico->CreateStream(53);
  if (stream53 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream53, ibuf, sizeof(ibuf)));
      exit(1);
  }

  node = 5;
  cmd[3] = 0xBABEFACE;
  cmd[2] = 0xDEADBEEF;
  cmd[1] = 0xDEADBEEF;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  printf("Doing PR: %d words, %dbytes\r\n", acc_vadd_PR5_bit_len, acc_vadd_PR5_bit_len * 4);
  pico->WriteStream(stream100, acc_vadd_PR5_bit, acc_vadd_PR5_bit_len * 4);
  printf("PR Done\r\n");

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  k = 0;
  do {
    printf("%i B available to read from 50. %5d\r\n", i=pico->GetBytesAvailable(stream, true), k);
    if (i < 0){
        fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
        exit(1);
    }
    room = i / 4;
    tmp = new int[room];
    pico->ReadStream(stream, tmp, i);
    printf("%d\r\n", room);
    // for (i = 0; i < room; i++) {
    //   printf("[%d] 0x%08x\r\n", i, tmp[i]);
    // }
    delete[] tmp;
    k++;
  } while (room != 0 && k < 10000);

  cmd[0] = 0xC5100000;
  cmd[1] = 0xC5200000 | SIZE;
  cmd[2] = 0xC5300001;
  cmd[3] = 0xB5000000;
  pico->WriteStream(stream, cmd, 16);

  printf("Lanching ACC\r\n");
  cmd[0] = 0xA5000000;
  cmd[1] = 0x00000000;
  cmd[2] = 0x00000000;
  cmd[3] = 0x00000000;
  pico->WriteStream(stream, cmd, 16);

  err = pico->WriteStream(stream51, A, SIZE * 4);
  err = pico->WriteStream(stream52, B, SIZE * 4);
  printf("%i B available to read from stream53.\n", i=pico->GetBytesAvailable(stream53, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  err = pico->ReadStream(stream53, tmp, i);
  if (err < 0) {
      fprintf(stderr, "ReadStream stream53 error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
      exit(1);
  }
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }

  printf("%i B available to read from stream.\n", i=pico->GetBytesAvailable(stream, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  pico->ReadStream(stream, tmp, i);
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }
  delete[] tmp;
  pico->CloseStream(stream51);
  pico->CloseStream(stream52);
  pico->CloseStream(stream53);
#endif
//==============================================================================
//       _    ____ ____        __
//      / \  / ___/ ___|  _   / /_
//     / _ \| |  | |     (_) | '_ \
//    / ___ \ |__| |___   _  | (_) |
//   /_/   \_\____\____| (_)  \___/
//==============================================================================
#ifdef ACC6
  printf("Opening stream61\r\n");
  stream61 = pico->CreateStream(61);
  if (stream61 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream61, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream62\r\n");
  stream62 = pico->CreateStream(62);
  if (stream62 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream62, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream63\r\n");
  stream63 = pico->CreateStream(63);
  if (stream63 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream63, ibuf, sizeof(ibuf)));
      exit(1);
  }

  node = 6;
  cmd[3] = 0xBABEFACE;
  cmd[2] = 0xDEADBEEF;
  cmd[1] = 0xDEADBEEF;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  printf("Doing PR: %d words, %dbytes\r\n", acc_vadd_PR6_bit_len, acc_vadd_PR6_bit_len * 4);
  pico->WriteStream(stream100, acc_vadd_PR6_bit, acc_vadd_PR6_bit_len * 4);
  printf("PR Done\r\n");

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  k = 0;
  do {
    printf("%i B available to read from 50. %5d\r\n", i=pico->GetBytesAvailable(stream, true), k);
    if (i < 0){
        fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
        exit(1);
    }
    room = i / 4;
    tmp = new int[room];
    pico->ReadStream(stream, tmp, i);
    printf("%d\r\n", room);
    // for (i = 0; i < room; i++) {
    //   printf("[%d] 0x%08x\r\n", i, tmp[i]);
    // }
    delete[] tmp;
    k++;
  } while (room != 0 && k < 10000);

  cmd[0] = 0xC6100000;
  cmd[1] = 0xC6200000 | SIZE;
  cmd[2] = 0xC6300001;
  cmd[3] = 0xB6000000;
  pico->WriteStream(stream, cmd, 16);

  printf("Lanching ACC\r\n");
  cmd[0] = 0xA6000000;
  cmd[1] = 0x00000000;
  cmd[2] = 0x00000000;
  cmd[3] = 0x00000000;
  pico->WriteStream(stream, cmd, 16);

  err = pico->WriteStream(stream61, A, SIZE * 4);
  err = pico->WriteStream(stream62, B, SIZE * 4);
  printf("%i B available to read from stream63.\n", i=pico->GetBytesAvailable(stream63, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  err = pico->ReadStream(stream63, tmp, i);
  if (err < 0) {
      fprintf(stderr, "ReadStream stream63 error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
      exit(1);
  }
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }

  printf("%i B available to read from stream.\n", i=pico->GetBytesAvailable(stream, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  pico->ReadStream(stream, tmp, i);
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }
  delete[] tmp;
  pico->CloseStream(stream61);
  pico->CloseStream(stream62);
  pico->CloseStream(stream63);
#endif
//==============================================================================
//       _    ____ ____       _____
//      / \  / ___/ ___|  _  |___  |
//     / _ \| |  | |     (_)    / /
//    / ___ \ |__| |___   _    / /
//   /_/   \_\____\____| (_)  /_/
//==============================================================================
#ifdef ACC7
  printf("Opening stream71\r\n");
  stream71 = pico->CreateStream(71);
  if (stream71 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream71, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream72\r\n");
  stream72 = pico->CreateStream(72);
  if (stream72 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream72, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream73\r\n");
  stream73 = pico->CreateStream(73);
  if (stream73 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream73, ibuf, sizeof(ibuf)));
      exit(1);
  }

  node = 7;
  cmd[3] = 0xBABEFACE;
  cmd[2] = 0xDEADBEEF;
  cmd[1] = 0xDEADBEEF;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  printf("Doing PR: %d words, %dbytes\r\n", acc_vadd_PR7_bit_len, acc_vadd_PR7_bit_len * 4);
  pico->WriteStream(stream100, acc_vadd_PR7_bit, acc_vadd_PR7_bit_len * 4);
  printf("PR Done\r\n");

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  k = 0;
  do {
    printf("%i B available to read from 50. %5d\r\n", i=pico->GetBytesAvailable(stream, true), k);
    if (i < 0){
        fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
        exit(1);
    }
    room = i / 4;
    tmp = new int[room];
    pico->ReadStream(stream, tmp, i);
    printf("%d\r\n", room);
    // for (i = 0; i < room; i++) {
    //   printf("[%d] 0x%08x\r\n", i, tmp[i]);
    // }
    delete[] tmp;
    k++;
  } while (room != 0 && k < 10000);

  cmd[0] = 0xC7100000;
  cmd[1] = 0xC7200000 | SIZE;
  cmd[2] = 0xC7300001;
  cmd[3] = 0xB7000000;
  pico->WriteStream(stream, cmd, 16);

  printf("Lanching ACC\r\n");
  cmd[0] = 0xA7000000;
  cmd[1] = 0x00000000;
  cmd[2] = 0x00000000;
  cmd[3] = 0x00000000;
  pico->WriteStream(stream, cmd, 16);

  err = pico->WriteStream(stream71, A, SIZE * 4);
  err = pico->WriteStream(stream72, B, SIZE * 4);
  printf("%i B available to read from stream73.\n", i=pico->GetBytesAvailable(stream73, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  err = pico->ReadStream(stream73, tmp, i);
  if (err < 0) {
      fprintf(stderr, "ReadStream stream73 error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
      exit(1);
  }
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }

  printf("%i B available to read from stream.\n", i=pico->GetBytesAvailable(stream, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  pico->ReadStream(stream, tmp, i);
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }
  delete[] tmp;
  pico->CloseStream(stream71);
  pico->CloseStream(stream72);
  pico->CloseStream(stream73);
#endif
//==============================================================================
//       _    ____ ____        ___
//      / \  / ___/ ___|  _   ( _ )
//     / _ \| |  | |     (_)  / _ \
//    / ___ \ |__| |___   _  | (_) |
//   /_/   \_\____\____| (_)  \___/
//==============================================================================
#ifdef ACC8
  printf("Opening stream81\r\n");
  stream81 = pico->CreateStream(81);
  if (stream81 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream81, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream82\r\n");
  stream82 = pico->CreateStream(82);
  if (stream82 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream82, ibuf, sizeof(ibuf)));
      exit(1);
  }

  printf("Opening stream83\r\n");
  stream83 = pico->CreateStream(83);
  if (stream83 < 0) {
      fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream83, ibuf, sizeof(ibuf)));
      exit(1);
  }

  node = 8;
  cmd[3] = 0xBABEFACE;
  cmd[2] = 0xDEADBEEF;
  cmd[1] = 0xDEADBEEF;
  cmd[0] = 0xD000BEEF | (node << 24); // PR Start CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  printf("Doing PR: %d words, %dbytes\r\n", acc_vadd_PR8_bit_len, acc_vadd_PR8_bit_len * 4);
  pico->WriteStream(stream100, acc_vadd_PR8_bit, acc_vadd_PR8_bit_len * 4);
  printf("PR Done\r\n");

  cmd[0] = 0xD000DEAD | (node << 24); // PR End CMD
  printf("0x%08x\r\n", cmd[0]);
  pico->WriteStream(stream, cmd, 16);

  k = 0;
  do {
    printf("%i B available to read from 50. %5d\r\n", i=pico->GetBytesAvailable(stream, true), k);
    if (i < 0){
        fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
        exit(1);
    }
    room = i / 4;
    tmp = new int[room];
    pico->ReadStream(stream, tmp, i);
    printf("%d\r\n", room);
    // for (i = 0; i < room; i++) {
    //   printf("[%d] 0x%08x\r\n", i, tmp[i]);
    // }
    delete[] tmp;
    k++;
  } while (room != 0 && k < 10000);

  cmd[0] = 0xC8100000;
  cmd[1] = 0xC8200000 | SIZE;
  cmd[2] = 0xC8300001;
  cmd[3] = 0xB8000000;
  pico->WriteStream(stream, cmd, 16);

  printf("Lanching ACC\r\n");
  cmd[0] = 0xA8000000;
  cmd[1] = 0x00000000;
  cmd[2] = 0x00000000;
  cmd[3] = 0x00000000;
  pico->WriteStream(stream, cmd, 16);

  err = pico->WriteStream(stream81, A, SIZE * 4);
  err = pico->WriteStream(stream82, B, SIZE * 4);
  printf("%i B available to read from stream83.\n", i=pico->GetBytesAvailable(stream83, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  err = pico->ReadStream(stream83, tmp, i);
  if (err < 0) {
      fprintf(stderr, "ReadStream stream83 error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
      exit(1);
  }
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }

  printf("%i B available to read from stream.\n", i=pico->GetBytesAvailable(stream, true));
  if (i < 0){
      fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
      exit(1);
  }
  room = i / 4;
  tmp = new int[room];
  pico->ReadStream(stream, tmp, i);
  printf("%d\r\n", room);
  for (i = 0; i < room; i++) {
    printf("[%d] 0x%08x\r\n", i, tmp[i]);
  }
  delete[] tmp;
  pico->CloseStream(stream81);
  pico->CloseStream(stream82);
  pico->CloseStream(stream83);
#endif
//==============================================================================
  printf("Closing stream 50 (CMD)\r\n");
  pico->CloseStream(stream);
  printf("Closing stream 100 (ICAP)\r\n");
  pico->CloseStream(stream100);

  printf("Done\r\n");
  delete[] A;
  delete[] B;
  delete[] C;
  delete[] D;
  delete[] E;
  return 0;
}
