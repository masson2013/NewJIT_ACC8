/*===================================================================================
 File: StreamLoopback.cpp.

 This module is a simple command line utility designed to illustrate the operation
 of a Pico stream interface. The streams provide a buffered interface to a module
 fabricated in the FPGA.

 This program interfaces with the firmware kernel StreamLoopback128.v in the sister directory.

=====================================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <picodrv.h>
#include <pico_errors.h>
#include <pico_pr.h>
#include <pthread.h>
#include <sys/time.h>
#include <locale.h>
#include "des.h"

// #define ITEMS 128    // 2048 Bytes
// #define ITEMS 256    // 4096 Bytes
// #define ITEMS 512    // 8192 Bytes
// #define ITEMS 1024    // 16384 Bytes
// #define ITEMS 2048    // 32768 Bytes
// #define ITEMS 4096    // 65536 Bytes
// #define ITEMS 8192    // 131072 KBytes
// #define ITEMS 16384   // 262144 KBytes

// #define VERBOSE_THREAD
// #define DEBUG

#define EndianSwap32(A) ((((uint)(A) & 0xff000000) >> 24) | (((uint)(A) & 0x00ff0000) >> 8) | \
             (((uint)(A) & 0x0000ff00) << 8) | (((uint)(A) & 0x000000ff) << 24))
#define WS              0
#define RS              1

typedef struct
{
    uint32_t dL;
    uint32_t dH;
    uint32_t kL;
    uint32_t kH;
}des_t;

typedef struct
{
    PicoDrv     *Pico     ;
    int          Type     ;
    uint32_t     Stream   ;
    des_t       *In_Data  ;
    uint32_t    *Out_Data ;
    int          Size     ;
}task_pk_t;


void * Stream_Threads_Call(void *pk)
{
#ifdef VERBOSE_THREAD
  printf("\r\n");
#endif
  task_pk_t *p = (task_pk_t *)pk;
  int err;
  char      ibuf[1024];
  PicoDrv   *pico     = p->Pico;
  int       type      = p->Type;
  uint32_t  stream    = p->Stream;
  des_t     *In_Data  = p->In_Data;
  uint32_t  *Out_Data = p->Out_Data;
  int       items     = p->Size;

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
      printf("[DEBUG->RS_TCALL] Reading %i Bytes to 0x%08x\n", items, stream);
    #endif

    err = pico->ReadStream(stream, Out_Data,  items);
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
    int         err, i, j, k;
    int         stream11;
    int         stream12;
    int         stream21;
    uint32_t    buf[1024], u32, addr;
    char        ibuf[1024];
    PicoDrv     *pico1;
    PicoDrv     *pico2;
    const char* bitFileName;
    const char* prbitFileName;
    uint32_t Lbuf[2048];
    // uint32_t ret1[ITEMS / 4];
    // uint32_t ret2[ITEMS / 4];
    int available_read_size;
    struct timeval start, end;
    int timeuse;

#ifdef DEBUG
    PICO_CONFIG cfg = PICO_CONFIG();
    cfg.model = 0x505;
    if ((err = FindPico(&cfg, &pico1)) < 0) {
        printf("FindPico Error\r\n");
        return err;
    }
#else
    bitFileName = "Pico_Toplevel.bit";
    printf("Loading FPGA %d with '%s' ...\n", 1, bitFileName);
    err = RunBitFile(bitFileName, &pico1);
    if (err < 0) {
        fprintf(stderr, "RunBitFile error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
        exit(1);
    }
#endif
    int ITEMS;
    int iter;
    for (iter = 1; iter <= 150; iter++) {
        ITEMS = iter * 64 * 16;

        // ITEMS = 1024;
        ////////////////////////////////////////////////////////////////////////////
        des_t    *In_Data  =  new des_t[ITEMS];
        uint32_t *Out_Data =  new uint32_t[ITEMS * 16];
        // printf("%lu, %lu, %lu\r\n", sizeof(des_t), sizeof(In_Data), sizeof(Out_Data));
        // fill the buffer with data we'll recognize when it comes back.
        printf("%3d#%9d#%9d#", iter, ITEMS, ITEMS * 16);
        In_Data  =  new des_t[ITEMS];
        Out_Data =  new uint32_t[ITEMS * 16];
        ////////////////////////////////////////////////////////////////////////////
        for (i = 0; i < ITEMS;  i++) {
            // buf[i] = 0xFFFFFFFF;
            srand(time(NULL));
            In_Data[i].dL = rand() % 0x0FFFFFFF;
            In_Data[i].dH = rand() % 0x0FFFFFFF;
            In_Data[i].kL = rand() % 0x0FFFFFFF;
            In_Data[i].kH = rand() % 0x0FFFFFFF;
        }
        ////////////////////////////////////////////////////////////////////////////
        uchar *PText;
        uchar *PreKey;
        des_t *tmp;
        gettimeofday(&start, NULL);
        for (i = 0; i < ITEMS; i++) {
            tmp = new des_t;
            tmp->dL = EndianSwap32(In_Data[i].dH);
            tmp->dH = EndianSwap32(In_Data[i].dL);
            tmp->kL = EndianSwap32(In_Data[i].kH);
            tmp->kH = EndianSwap32(In_Data[i].kL);
            PText  = (uchar *)&tmp->dL;
            PreKey = (uchar *)&tmp->kL;
            GenerateKey(PreKey);
            Des(PText);
            delete tmp;
        }
        gettimeofday(&end, NULL);
        timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        // printf("CPU %'4d threads :\t%'9d us\r\n", 1, timeuse);
        printf("%9d#us#", timeuse);
        ////////////////////////////////////////////////////////////////////////////
        #ifdef VERBOSE_THREAD
            printf("Opening streams\r\n");
        #endif
        stream11 = pico1->CreateStream(1);
        if (stream11 < 0) {
            fprintf(stderr, "CreateStream error: %s\n", PicoErrors_FullError(stream11, ibuf, sizeof(ibuf)));
            exit(1);
        }
        #ifdef VERBOSE_THREAD
            printf("Stream : 0x%08x\r\n", stream11);
        #endif
        ////////////////////////////////////////////////////////////////////////////
        pthread_t thread[2];
        task_pk_t task_pkg[2];


        // Package for write
        task_pkg[0].Pico     = pico1;
        task_pkg[0].Type     = WS;
        task_pkg[0].Stream   = stream11;
        task_pkg[0].In_Data  = In_Data;
        task_pkg[0].Out_Data = Out_Data;
        task_pkg[0].Size     = ITEMS * 16;

        // Package for read
        task_pkg[1].Pico     = pico1;
        task_pkg[1].Type     = RS;
        task_pkg[1].Stream   = stream11;
        task_pkg[1].In_Data  = In_Data;
        task_pkg[1].Out_Data = Out_Data;
        task_pkg[1].Size     = ITEMS * 16;

        gettimeofday(&start, NULL);

        pthread_create(&thread[1], NULL, Stream_Threads_Call, (void *)&task_pkg[1]);
        pthread_create(&thread[0], NULL, Stream_Threads_Call, (void *)&task_pkg[0]);
        pthread_join(thread[0], NULL);
        pthread_join(thread[1], NULL);

        gettimeofday(&end, NULL);
        timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("%9d#us#", timeuse);

        ////////////////////////////////////////////////////////////////////////////
        // printf("Writing %i Bytes\n", ITEMS * 16);
        // err = pico1->WriteStream(stream11, In_Data, ITEMS * 16); // Write bytes not words.
        // if (err < 0) {
        //     fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
        //     exit(1);
        // }
        ////////////////////////////////////////////////////////////////////////////
        // i = 0;
        // while (i == 0) {
        //     printf("%i Bytes available to read from firmware.\n", i=pico1->GetBytesAvailable(stream11, true /* reading */));
        //     if (i < 0){
        //         fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
        //         exit(1);
        //     }
        //     if (i == 0 ) exit(1);
        // }
        // available_read_size = i;
        // printf("Reading %i Bytes\n", available_read_size);
        // err = pico1->ReadStream(stream11, buf, available_read_size);
        // if (err < 0) {
        //     fprintf(stderr, "ReadStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
        //     exit(1);
        // }
        #ifdef VERBOSE_THREAD
            printf("Close Stream\n");
        #endif
        pico1->CloseStream(stream11);

        ///////////////////////////////////////////////////////////////////////////
        for (i = 0; i < ITEMS; i++) {
            tmp = new des_t;
            tmp->dL = EndianSwap32(In_Data[i].dH);
            tmp->dH = EndianSwap32(In_Data[i].dL);
            tmp->kL = EndianSwap32(In_Data[i].kH);
            tmp->kH = EndianSwap32(In_Data[i].kL);

            PText  = (uchar *)&tmp->dL;
            PreKey = (uchar *)&tmp->kL;

            GenerateKey(PreKey);

            Des(PText);

            if  ((Out_Data[i * 4 + 0] != EndianSwap32(tmp->dH)) || (Out_Data[i * 4 + 1] != EndianSwap32(tmp->dL))) {
                printf("Error: unexpected values of Out_Data\r\n");
                printf("[%d] 0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", i, Out_Data[i * 4 + 0], Out_Data[i * 4 + 1], Out_Data[i * 4 + 2], Out_Data[i * 4 + 3]);
                printf("[%d] 0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", i, EndianSwap32(tmp->dH), EndianSwap32(tmp->dL), tmp->kH, tmp->kL);
                delete[] In_Data;
                delete[] Out_Data;
                exit(1);
            }
            // printf("[%d] 0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", i, buf[i * 4 + 0], buf[i * 4 + 1], buf[i * 4 + 2], buf[i * 4 + 3]);
            // printf("[%d] 0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", i, EndianSwap32(tmp->dH), EndianSwap32(tmp->dL), tmp->kH, tmp->kL);
            delete tmp;
        }

        // printf("Tests successful DES!\n");
        printf("Pass\r\n");
        delete[] In_Data;
        delete[] Out_Data;
    }
    printf("Done!\r\n");
    return 0;
}
