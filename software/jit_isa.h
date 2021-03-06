#include "jit_bit.h"

#define CARD            2
#define NUM_ACCs        8
#define PR
// #define AVAILABLE

#define MAX_NUM_MODULES 50
#define ROW             NUM_ACCs
#define COL             1

#define TOTAL_NODES     CARD * ROW * COL
#define VSTART_THREADS  3

#define MAX_BUF_SIZE    1024 * 8
#define MAX_CARD        24
#define PRFREE          0
#define PRBUSY          1

#define NOP             0
#define VADD            1
#define VSUB            2
#define VREDUCE         3
#define VMUL            4
#define VADDREDUCE      5
#define VSUBREDUCE      6
#define ACCMM           7
#define ACCMMM          8
#define MERGE           9
#define INSERTION       10
#define RADIX           11
#define INSERTIONNF     12
#define SQLLARGE        13
#define A2PB2           14
#define VAPBB           15
#define VAAPB           16
#define BB              17
#define DES             18

#define SIN1            0
#define SIN2            1
#define MOUT            2
#define SIN1T           3
#define SIN2T           4
#define MOUTT           5

#define NORTH           1
#define EAST            2
#define SOUTH           3
#define WEST            4

#define FUN_VNEW        0
#define FUN_VLPR        1
#define FUN_VTIEIO      2
#define FUN_VSTART      3
#define FUN_VEND        4
#define FUN_VDEL        5

#define Buf_Buf_Buf     0
#define Buf_Buf_Reg     1
#define Buf_Reg_Buf     2
#define Buf_Reg_Reg     3
#define Reg_Buf_Buf     4
#define Reg_Buf_Reg     5
#define Reg_Reg_Buf     6
#define Reg_Reg_Reg     7

#define WRITE_IN1       0
#define WRITE_IN2       1
#define READ_OUT        2
#define WS              0
#define RS              1
//==================================================================================================
typedef struct{
  uint32_t  BitSize[ROW * COL];
  uint32_t *BitAddr[ROW * COL];
}vam_Bitstream_table_item;

typedef struct{
  vam_Bitstream_table_item item[MAX_NUM_MODULES];
}vam_Bitstream_table_t;

typedef struct{
  int        status;     // Busy 1 or Idel 0
  int        card_key;   // Which FPGA card
  int        node_key;   // Which Node in FPGA card
  int        PR_key;     // Current PR type
  int        node_type;

  int        streamInA;  // Stream port A
  int        streamInB;  // Stream port B
  int        streamOut;  // Stream port C

  int        streamInAT; // Tie Stream port A
  int        streamInBT; // Tie Stream port B
  int        streamOutT; // Tie Stream port C

  int        *in1;
  int        *in2;
  int        *out;

  int        tie_in1;
  int        tie_in2;
  int        tie_out;

  int        size_in1;
  int        size_in2;
  int        size_out;

  int        tiestream;

  int        cur_cmd;    // Current CMD
  int        A3_cmd;
}vam_node_t;

typedef struct {
  FILE                  *file;
  pthread_mutex_t       vm_mutex;
  PicoDrv               *pico[CARD];
  vector<vam_node_t>    *VAM_TABLE;
  vam_Bitstream_table_t *BITSTREAM_TABLE;
  int                    TieStream_Table[CARD * NUM_ACCs][2];
}vam_vm_t;

typedef struct {
  vector<int>  *nPR;
  vam_vm_t     *VM;
  int          PR_NAME; // only for lpr
}vm_pk_t;

typedef struct {
  int       type;
  vam_vm_t *VM;
  int       card;
  int       index;
  uint32_t  stream; //Stream port number
  int      *buf;
  int       size;
}vstart_pk_t;

//==================================================================================================
void * WriteStream_Threads_Call   (void *pk);
void * ReadStream_Threads_Call    (void *pk);
void * Stream_Threads_Call        (void *pk);
void   errCheck                   (int err, int fun);
void   VAM_TABLE_SHOW             (vam_vm_t VM);
 int   VAM_TABLE_INIT             (PicoDrv **pico, vector<vam_node_t> *vam_table);
void   VAM_TABLE_CLEAN            (vam_vm_t *VM);
void   VAM_BITSTREAM_TABLE_INIT   (vam_Bitstream_table_t *BITSTREAM_TABLE);
void   VAM_VM_INIT                (vam_vm_t *VM, int argc, char* argv[]);
void   VAM_VM_CLEAN               (vam_vm_t *VM);
 int   vnew                       (vam_vm_t *VM, vector<int> *nPR);
void * vnew_Threads_Call          (void *pk);
int    vdel                       (vam_vm_t *VM, vector<int> *nPR);
void * vdel_Threads_Call          (void *pk);
int    vlpr                       (vam_vm_t *VM, int nPR, int PR_NAME);
void * vlpr_Threads_Call          (void *pk);
int vtieio(vam_vm_t *VM, int nPR, int *in1, int *in2, int *out, int size);
//==================================================================================================
void * WriteStream_Threads_Call(void *pk)
{
  vstart_pk_t *p = (vstart_pk_t *)pk;
  int err;
  char      ibuf[1024];
  int       card  = p->card;
  int       index = p->index;
  uint32_t stream = p->stream;
  int      *buf   = p->buf;
  int       items = p->size;

  #ifdef VERBOSE
    printf("[DEBUG->WS_TCALL] Writing %i Bytes to 0x%08x\n", items * 4, stream);
  #endif

  err = p->VM->pico[card]->WriteStream(stream, buf,  items * 4);
  if (err < 0) {
    fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
    return (void *) -1;
  }
}

void * ReadStream_Threads_Call(void *pk)
{
  vstart_pk_t *p = (vstart_pk_t *)pk;
  int err;
  char      ibuf[1024];
  int       card  = p->card;
  int       index = p->index;
  uint32_t stream = p->stream;
  int      *buf   = p->buf;
  int       items = p->size;

  #ifdef VERBOSE
    printf("[DEBUG->RS_TCALL] Reading %i Bytes to 0x%08x\n", items * 4, stream);
  #endif

  err = p->VM->pico[card]->ReadStream(stream, buf,  items * 4);
  if (err < 0) {
    fprintf(stderr, "ReadingStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
    return (void *) -1;
  }
}

void * Stream_Threads_Call(void *pk)
{
#ifdef VERBOSE_THREAD
  printf("\r\n");
#endif
  vstart_pk_t *p = (vstart_pk_t *)pk;
  int err, i;
  char      ibuf[1024];
  int       type  = p->type;
  int       card  = p->card;
  int       index = p->index;
  uint32_t stream = p->stream;
  int      *buf   = p->buf;
  int       items = p->size;

  if (type == WS) {
    #ifdef VERBOSE_THREAD
      printf("[DEBUG->WS_TCALL] Writing %i Bytes to 0x%08x\n", items * 4, stream);
    #endif

    err = p->VM->pico[card]->WriteStream(stream, buf,  items * 4);
    if (err < 0) {
      fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
      return (void *) -1;
    }
  } else if (type == RS) {
    #ifdef VERBOSE_THREAD
      #ifndef AVAILABLE
        printf("[DEBUG->RS_TCALL] Reading %i Bytes from 0x%08x\n", items * 4, stream);
      #else
        printf("[DEBUG->RS_TCALL] Reading %i Bytes from 0x%08x\n", i=p->VM->pico[card]->GetBytesAvailable(stream, true), stream);
        if (i < 0){
            fprintf(stderr, "GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
            exit(1);
        }
      #endif
    #endif

    #ifndef AVAILABLE
      err = p->VM->pico[card]->ReadStream(stream, buf,  items * 4);
    #else
      err = p->VM->pico[card]->ReadStream(stream, buf,  i);
    #endif

    if (err < 0) {
      fprintf(stderr, "ReadingStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
      return (void *) -1;
    }
  }

}

void errCheck(int err, int fun)
{
  if (err < 0) {
    switch (fun) {
      case FUN_VNEW: {
        printf("[ERROR->FUN_VNEW] No available Node & not suport multithreads\r\n");
        exit(1);
      }break;

      case FUN_VLPR: {
        printf("[ERROR->FUN_VLPR] PR Error\r\n");
        exit(1);
      }break;

      case FUN_VTIEIO: {
        printf("[ERROR->FUN_VTIEIO] vtieio error\r\n");
        exit(1);
      }break;

      case FUN_VSTART: {
        printf("[ERROR->FUN_VSTART] vstart error\r\n");
        exit(1);
      }break;

      case FUN_VEND: {
        printf("[ERROR->FUN_VEND] vend error\r\n");
        exit(1);
      }break;

      case FUN_VDEL: {
        printf("[ERROR->FUN_VDEL] vdel error\r\n");
        exit(1);
      }break;

      default: {
        printf("Unkonw error!\r\n");
        exit(1);
      }break;
    }
  }
}

void VAM_TABLE_SHOW(vam_vm_t VM)
{
#ifdef VERBOSE
  printf("\r\n");
#endif

#ifdef VERBOSE
  vector<vam_node_t>::iterator v = VM.VAM_TABLE->begin();
  cout << "[DEBUG->VAMTABLE] vam_table size = " << VM.VAM_TABLE->size() << endl;
  printf("====================================================================\r\n");
  while( v != VM.VAM_TABLE->end()) {
      printf("[DEBUG->VAMTABLE] status    :%d\r\n",       v->status    );
      printf("[DEBUG->VAMTABLE] card_key  :%d\r\n",       v->card_key  );
      printf("[DEBUG->VAMTABLE] node_key  :%d\r\n",       v->node_key  );
      printf("[DEBUG->VAMTABLE] PR_key    :%d\r\n",       v->PR_key    );
      printf("[DEBUG->VAMTABLE] node_type :%d\r\n",       v->node_type );
      printf("[DEBUG->VAMTABLE] s_InA     :0x%08x\r\n",   v->streamInA );
      printf("[DEBUG->VAMTABLE] s_InB     :0x%08x\r\n",   v->streamInB );
      printf("[DEBUG->VAMTABLE] s_Out     :0x%08x\r\n",   v->streamOut );
      printf("[DEBUG->VAMTABLE] in1       :%p\r\n",       v->in1       );
      printf("[DEBUG->VAMTABLE] in2       :%p\r\n",       v->in2       );
      printf("[DEBUG->VAMTABLE] out       :%p\r\n",       v->out       );
      printf("[DEBUG->VAMTABLE] tie_in1   :%d\r\n",       v->tie_in1   );
      printf("[DEBUG->VAMTABLE] tie_in2   :%d\r\n",       v->tie_in2   );
      printf("[DEBUG->VAMTABLE] tie_out   :%d\r\n",       v->tie_out   );
      printf("[DEBUG->VAMTABLE] cur_cmd   :0x%08x\r\n",   v->cur_cmd   );
      // printf("[DEBUG->VAMTABLE] in1_tmp   :%p\r\n",       v->in1_tmp   );
      // printf("[DEBUG->VAMTABLE] in2_tmp   :%p\r\n",       v->in2_tmp   );
      // printf("[DEBUG->VAMTABLE] out_tmp   :%p\r\n",       v->out_tmp   );
      // printf("[DEBUG->VAMTABLE] in_acc1 :%d\r\n",         v->in_acc1  );
      // printf("[DEBUG->VAMTABLE] in_acc2 :%d\r\n",         v->in_acc2  );
      // printf("[DEBUG->VAMTABLE] out_n   :%d\r\n",         v->out_n    );
      // printf("[DEBUG->VAMTABLE] out_e   :%d\r\n",         v->out_e    );
      // printf("[DEBUG->VAMTABLE] out_s   :%d\r\n",         v->out_s    );
      // printf("[DEBUG->VAMTABLE] out_w   :%d\r\n",         v->out_w    );

      printf("\r\n");
      v++;
  }
  printf("====================================================================\r\n");
#endif
}

int VAM_TABLE_INIT(PicoDrv **pico, vector<vam_node_t> *vam_table)
{
  #ifdef VERBOSE
    printf("\r\n");
  #endif

  int i, j;
  int vam_table_node_size = ROW * COL;

  #ifdef VERBOSE
    printf("[DEBUG->VAM_TABLE_INIT] INIT OVERLAY_TOPOlOGY\r\n");
  #endif

  int OVERLAY_TOPOlOGY[MAX_CARD][NUM_ACCs][6];
  for (i = 0; i < CARD; i++) {
    for (j = 0; j < NUM_ACCs; j++) {
      OVERLAY_TOPOlOGY[i][j][SIN1]  = j * 10 + 11;
      OVERLAY_TOPOlOGY[i][j][SIN2]  = j * 10 + 12;
      OVERLAY_TOPOlOGY[i][j][MOUT]  = j * 10 + 13;

      OVERLAY_TOPOlOGY[i][j][SIN1T] = j * 10 + 14;
      OVERLAY_TOPOlOGY[i][j][SIN2T] = j * 10 + 15;
      OVERLAY_TOPOlOGY[i][j][MOUTT] = j * 10 + 16;

      #ifdef VERBOSE
        printf("[DEBUG->VAM_TABLE_INIT] OVERLAY_TOPOlOGY:%d, %d, %d, %d, %d, %d\r\n", OVERLAY_TOPOlOGY[i][j][SIN1],  OVERLAY_TOPOlOGY[i][j][SIN2],  OVERLAY_TOPOlOGY[i][j][MOUT], OVERLAY_TOPOlOGY[i][j][SIN1T], OVERLAY_TOPOlOGY[i][j][SIN2T], OVERLAY_TOPOlOGY[i][j][MOUTT]);
      #endif
    }
  }

  #ifdef VERBOSE
    printf("[DEBUG->VAM_TABLE_INIT] INIT\r\n");
  #endif
  for (i = 0; i < CARD; i++) {
    for (j = 0; j < vam_table_node_size; j++) {
      vam_node_t tmp;
      tmp.status     = 0;
      tmp.card_key   = i;
      tmp.node_key   = j;
      tmp.PR_key     = 0;
      tmp.node_type  = -1;

      tmp.streamInA  = pico[i]->CreateStream(OVERLAY_TOPOlOGY[i][j][SIN1]);
      tmp.streamInB  = pico[i]->CreateStream(OVERLAY_TOPOlOGY[i][j][SIN2]);
      tmp.streamOut  = pico[i]->CreateStream(OVERLAY_TOPOlOGY[i][j][MOUT]);

      tmp.streamInAT = pico[i]->CreateStream(OVERLAY_TOPOlOGY[i][j][SIN1T]);
      tmp.streamInBT = pico[i]->CreateStream(OVERLAY_TOPOlOGY[i][j][SIN2T]);
      tmp.streamOutT = pico[i]->CreateStream(OVERLAY_TOPOlOGY[i][j][MOUTT]);

      tmp.in1        = NULL;
      tmp.in2        = NULL;
      tmp.out        = NULL;

      tmp.tie_in1    = 0;
      tmp.tie_in2    = 0;
      tmp.tie_out    = 0;

      tmp.tiestream  = 0;

      tmp.cur_cmd    = 0x00000000;
      tmp.A3_cmd     = 0x00000000;
      vam_table->push_back(tmp);
    }
  }
  #ifdef VERBOSE
    printf("[DEBUG->VAM_TABLE_INIT] DONE\r\n");
  #endif
}

void VAM_TABLE_CLEAN(vam_vm_t *VM)
{
  #ifdef VERBOSE
    printf("\r\n");
  #endif

  #ifdef VERBOSE
    printf("[DEBUG->VAM_TABLE_CLEAN] CLEAN\r\n");
  #endif
  vector<vam_node_t>::iterator v = VM->VAM_TABLE->begin();
  while( v != VM->VAM_TABLE->end()) {
    #ifdef VERBOSE
      printf("[DEBUG->VAM_TABLE_CLEAN] CLOSE STREAM 0x%08x, 0x%08x, 0x%08x on Card %d\r\n", v->streamInA, v->streamInB, v->streamOut, v->card_key);
    #endif
    if (VM->VAM_TABLE->size() == 0) {
      cout << "VAM TABLE Size is 0" << endl;
      exit(1);
    }
    VM->pico[v->card_key]->CloseStream(v->streamInA);
    VM->pico[v->card_key]->CloseStream(v->streamInB);
    VM->pico[v->card_key]->CloseStream(v->streamOut);

    VM->pico[v->card_key]->CloseStream(v->streamInAT);
    VM->pico[v->card_key]->CloseStream(v->streamInBT);
    VM->pico[v->card_key]->CloseStream(v->streamOutT);
    v++;
  }
  #ifdef VERBOSE
    printf("[DEBUG->VAM_TABLE_CLEAN] DONE\r\n");
  #endif
}

void VAM_BITSTREAM_TABLE_INIT(vam_Bitstream_table_t *BITSTREAM_TABLE)
{
  #ifdef VERBOSE
    printf("\r\n");
  #endif

  #ifdef VERBOSE
    printf("[DEBUG->VAM_BITSTREAM_TABLE_INIT] INIT\r\n");
  #endif
     BITSTREAM_TABLE->item[RADIX].BitAddr[0]       = (uint32_t*)RadixSort_PR1_bit;
     BITSTREAM_TABLE->item[RADIX].BitAddr[1]       = (uint32_t*)RadixSort_PR2_bit;
     BITSTREAM_TABLE->item[RADIX].BitAddr[2]       = (uint32_t*)RadixSort_PR3_bit;
     BITSTREAM_TABLE->item[RADIX].BitAddr[3]       = (uint32_t*)RadixSort_PR4_bit;
     BITSTREAM_TABLE->item[RADIX].BitAddr[4]       = (uint32_t*)RadixSort_PR5_bit;
     BITSTREAM_TABLE->item[RADIX].BitAddr[5]       = (uint32_t*)RadixSort_PR6_bit;
     BITSTREAM_TABLE->item[RADIX].BitAddr[6]       = (uint32_t*)RadixSort_PR7_bit;
     BITSTREAM_TABLE->item[RADIX].BitAddr[7]       = (uint32_t*)RadixSort_PR8_bit;

     BITSTREAM_TABLE->item[RADIX].BitSize[0]       = (uint32_t) RadixSort_PR1_bit_len;
     BITSTREAM_TABLE->item[RADIX].BitSize[1]       = (uint32_t) RadixSort_PR2_bit_len;
     BITSTREAM_TABLE->item[RADIX].BitSize[2]       = (uint32_t) RadixSort_PR3_bit_len;
     BITSTREAM_TABLE->item[RADIX].BitSize[3]       = (uint32_t) RadixSort_PR4_bit_len;
     BITSTREAM_TABLE->item[RADIX].BitSize[4]       = (uint32_t) RadixSort_PR5_bit_len;
     BITSTREAM_TABLE->item[RADIX].BitSize[5]       = (uint32_t) RadixSort_PR6_bit_len;
     BITSTREAM_TABLE->item[RADIX].BitSize[6]       = (uint32_t) RadixSort_PR7_bit_len;
     BITSTREAM_TABLE->item[RADIX].BitSize[7]       = (uint32_t) RadixSort_PR8_bit_len;
////////////////////////////////////////////////////////////////////////////////////////////////////
     BITSTREAM_TABLE->item[MERGE].BitAddr[0]       = (uint32_t*)MergeUnit_PR1_bit;
     BITSTREAM_TABLE->item[MERGE].BitAddr[1]       = (uint32_t*)MergeUnit_PR2_bit;
     BITSTREAM_TABLE->item[MERGE].BitAddr[2]       = (uint32_t*)MergeUnit_PR3_bit;
     BITSTREAM_TABLE->item[MERGE].BitAddr[3]       = (uint32_t*)MergeUnit_PR4_bit;
     BITSTREAM_TABLE->item[MERGE].BitAddr[4]       = (uint32_t*)MergeUnit_PR5_bit;
     BITSTREAM_TABLE->item[MERGE].BitAddr[5]       = (uint32_t*)MergeUnit_PR6_bit;
     BITSTREAM_TABLE->item[MERGE].BitAddr[6]       = (uint32_t*)MergeUnit_PR7_bit;
     BITSTREAM_TABLE->item[MERGE].BitAddr[7]       = (uint32_t*)MergeUnit_PR8_bit;

     BITSTREAM_TABLE->item[MERGE].BitSize[0]       = (uint32_t) MergeUnit_PR1_bit_len;
     BITSTREAM_TABLE->item[MERGE].BitSize[1]       = (uint32_t) MergeUnit_PR2_bit_len;
     BITSTREAM_TABLE->item[MERGE].BitSize[2]       = (uint32_t) MergeUnit_PR3_bit_len;
     BITSTREAM_TABLE->item[MERGE].BitSize[3]       = (uint32_t) MergeUnit_PR4_bit_len;
     BITSTREAM_TABLE->item[MERGE].BitSize[4]       = (uint32_t) MergeUnit_PR5_bit_len;
     BITSTREAM_TABLE->item[MERGE].BitSize[5]       = (uint32_t) MergeUnit_PR6_bit_len;
     BITSTREAM_TABLE->item[MERGE].BitSize[6]       = (uint32_t) MergeUnit_PR7_bit_len;
     BITSTREAM_TABLE->item[MERGE].BitSize[7]       = (uint32_t) MergeUnit_PR8_bit_len;
////////////////////////////////////////////////////////////////////////////////////////////////////
     BITSTREAM_TABLE->item[INSERTION].BitAddr[0]   = (uint32_t*)InsertionUnit_PR1_bit;
     BITSTREAM_TABLE->item[INSERTION].BitAddr[1]   = (uint32_t*)InsertionUnit_PR2_bit;
     BITSTREAM_TABLE->item[INSERTION].BitAddr[2]   = (uint32_t*)InsertionUnit_PR3_bit;
     BITSTREAM_TABLE->item[INSERTION].BitAddr[3]   = (uint32_t*)InsertionUnit_PR4_bit;
     BITSTREAM_TABLE->item[INSERTION].BitAddr[4]   = (uint32_t*)InsertionUnit_PR5_bit;
     BITSTREAM_TABLE->item[INSERTION].BitAddr[5]   = (uint32_t*)InsertionUnit_PR6_bit;
     BITSTREAM_TABLE->item[INSERTION].BitAddr[6]   = (uint32_t*)InsertionUnit_PR7_bit;
     BITSTREAM_TABLE->item[INSERTION].BitAddr[7]   = (uint32_t*)InsertionUnit_PR8_bit;

     BITSTREAM_TABLE->item[INSERTION].BitSize[0]   = (uint32_t) InsertionUnit_PR1_bit_len;
     BITSTREAM_TABLE->item[INSERTION].BitSize[1]   = (uint32_t) InsertionUnit_PR2_bit_len;
     BITSTREAM_TABLE->item[INSERTION].BitSize[2]   = (uint32_t) InsertionUnit_PR3_bit_len;
     BITSTREAM_TABLE->item[INSERTION].BitSize[3]   = (uint32_t) InsertionUnit_PR4_bit_len;
     BITSTREAM_TABLE->item[INSERTION].BitSize[4]   = (uint32_t) InsertionUnit_PR5_bit_len;
     BITSTREAM_TABLE->item[INSERTION].BitSize[5]   = (uint32_t) InsertionUnit_PR6_bit_len;
     BITSTREAM_TABLE->item[INSERTION].BitSize[6]   = (uint32_t) InsertionUnit_PR7_bit_len;
     BITSTREAM_TABLE->item[INSERTION].BitSize[7]   = (uint32_t) InsertionUnit_PR8_bit_len;
////////////////////////////////////////////////////////////////////////////////////////////////////
     BITSTREAM_TABLE->item[INSERTIONNF].BitAddr[0] = (uint32_t*)InsertionSortNF_PR1_bit;
     BITSTREAM_TABLE->item[INSERTIONNF].BitAddr[1] = (uint32_t*)InsertionSortNF_PR2_bit;
     BITSTREAM_TABLE->item[INSERTIONNF].BitAddr[2] = (uint32_t*)InsertionSortNF_PR3_bit;
     BITSTREAM_TABLE->item[INSERTIONNF].BitAddr[3] = (uint32_t*)InsertionSortNF_PR4_bit;
     BITSTREAM_TABLE->item[INSERTIONNF].BitAddr[4] = (uint32_t*)InsertionSortNF_PR5_bit;
     BITSTREAM_TABLE->item[INSERTIONNF].BitAddr[5] = (uint32_t*)InsertionSortNF_PR6_bit;
     BITSTREAM_TABLE->item[INSERTIONNF].BitAddr[6] = (uint32_t*)InsertionSortNF_PR7_bit;
     BITSTREAM_TABLE->item[INSERTIONNF].BitAddr[7] = (uint32_t*)InsertionSortNF_PR8_bit;

     BITSTREAM_TABLE->item[INSERTIONNF].BitSize[0] = (uint32_t) InsertionSortNF_PR1_bit_len;
     BITSTREAM_TABLE->item[INSERTIONNF].BitSize[1] = (uint32_t) InsertionSortNF_PR2_bit_len;
     BITSTREAM_TABLE->item[INSERTIONNF].BitSize[2] = (uint32_t) InsertionSortNF_PR3_bit_len;
     BITSTREAM_TABLE->item[INSERTIONNF].BitSize[3] = (uint32_t) InsertionSortNF_PR4_bit_len;
     BITSTREAM_TABLE->item[INSERTIONNF].BitSize[4] = (uint32_t) InsertionSortNF_PR5_bit_len;
     BITSTREAM_TABLE->item[INSERTIONNF].BitSize[5] = (uint32_t) InsertionSortNF_PR6_bit_len;
     BITSTREAM_TABLE->item[INSERTIONNF].BitSize[6] = (uint32_t) InsertionSortNF_PR7_bit_len;
     BITSTREAM_TABLE->item[INSERTIONNF].BitSize[7] = (uint32_t) InsertionSortNF_PR8_bit_len;
////////////////////////////////////////////////////////////////////////////////////////////////////
     BITSTREAM_TABLE->item[VADD].BitAddr[0]        = (uint32_t*)acc_vadd_PR1_bit;
     BITSTREAM_TABLE->item[VADD].BitAddr[1]        = (uint32_t*)acc_vadd_PR2_bit;
     BITSTREAM_TABLE->item[VADD].BitAddr[2]        = (uint32_t*)acc_vadd_PR3_bit;
     BITSTREAM_TABLE->item[VADD].BitAddr[3]        = (uint32_t*)acc_vadd_PR4_bit;
     BITSTREAM_TABLE->item[VADD].BitAddr[4]        = (uint32_t*)acc_vadd_PR5_bit;
     BITSTREAM_TABLE->item[VADD].BitAddr[5]        = (uint32_t*)acc_vadd_PR6_bit;
     BITSTREAM_TABLE->item[VADD].BitAddr[6]        = (uint32_t*)acc_vadd_PR7_bit;
     BITSTREAM_TABLE->item[VADD].BitAddr[7]        = (uint32_t*)acc_vadd_PR8_bit;

     BITSTREAM_TABLE->item[VADD].BitSize[0]        = (uint32_t) acc_vadd_PR1_bit_len;
     BITSTREAM_TABLE->item[VADD].BitSize[1]        = (uint32_t) acc_vadd_PR2_bit_len;
     BITSTREAM_TABLE->item[VADD].BitSize[2]        = (uint32_t) acc_vadd_PR3_bit_len;
     BITSTREAM_TABLE->item[VADD].BitSize[3]        = (uint32_t) acc_vadd_PR4_bit_len;
     BITSTREAM_TABLE->item[VADD].BitSize[4]        = (uint32_t) acc_vadd_PR5_bit_len;
     BITSTREAM_TABLE->item[VADD].BitSize[5]        = (uint32_t) acc_vadd_PR6_bit_len;
     BITSTREAM_TABLE->item[VADD].BitSize[6]        = (uint32_t) acc_vadd_PR7_bit_len;
     BITSTREAM_TABLE->item[VADD].BitSize[7]        = (uint32_t) acc_vadd_PR8_bit_len;
  // //////////////////////////////////////////////////////////////////////////////
     BITSTREAM_TABLE->item[VMUL].BitAddr[0]        = (uint32_t*)acc_vmul_PR1_bit;
     BITSTREAM_TABLE->item[VMUL].BitAddr[1]        = (uint32_t*)acc_vmul_PR2_bit;
     BITSTREAM_TABLE->item[VMUL].BitAddr[2]        = (uint32_t*)acc_vmul_PR3_bit;
     BITSTREAM_TABLE->item[VMUL].BitAddr[3]        = (uint32_t*)acc_vmul_PR4_bit;
     BITSTREAM_TABLE->item[VMUL].BitAddr[4]        = (uint32_t*)acc_vmul_PR5_bit;
     BITSTREAM_TABLE->item[VMUL].BitAddr[5]        = (uint32_t*)acc_vmul_PR6_bit;
     BITSTREAM_TABLE->item[VMUL].BitAddr[6]        = (uint32_t*)acc_vmul_PR7_bit;
     BITSTREAM_TABLE->item[VMUL].BitAddr[7]        = (uint32_t*)acc_vmul_PR8_bit;

     BITSTREAM_TABLE->item[VMUL].BitSize[0]        = (uint32_t) acc_vmul_PR1_bit_len;
     BITSTREAM_TABLE->item[VMUL].BitSize[1]        = (uint32_t) acc_vmul_PR2_bit_len;
     BITSTREAM_TABLE->item[VMUL].BitSize[2]        = (uint32_t) acc_vmul_PR3_bit_len;
     BITSTREAM_TABLE->item[VMUL].BitSize[3]        = (uint32_t) acc_vmul_PR4_bit_len;
     BITSTREAM_TABLE->item[VMUL].BitSize[4]        = (uint32_t) acc_vmul_PR5_bit_len;
     BITSTREAM_TABLE->item[VMUL].BitSize[5]        = (uint32_t) acc_vmul_PR6_bit_len;
     BITSTREAM_TABLE->item[VMUL].BitSize[6]        = (uint32_t) acc_vmul_PR7_bit_len;
     BITSTREAM_TABLE->item[VMUL].BitSize[7]        = (uint32_t) acc_vmul_PR8_bit_len;
  // //////////////////////////////////////////////////////////////////////////////
     BITSTREAM_TABLE->item[BB].BitAddr[0]          = (uint32_t*)jit_blackbox_PR1_bit;
     BITSTREAM_TABLE->item[BB].BitAddr[1]          = (uint32_t*)jit_blackbox_PR2_bit;
     BITSTREAM_TABLE->item[BB].BitAddr[2]          = (uint32_t*)jit_blackbox_PR3_bit;
     BITSTREAM_TABLE->item[BB].BitAddr[3]          = (uint32_t*)jit_blackbox_PR4_bit;
     BITSTREAM_TABLE->item[BB].BitAddr[4]          = (uint32_t*)jit_blackbox_PR5_bit;
     BITSTREAM_TABLE->item[BB].BitAddr[5]          = (uint32_t*)jit_blackbox_PR6_bit;
     BITSTREAM_TABLE->item[BB].BitAddr[6]          = (uint32_t*)jit_blackbox_PR7_bit;
     BITSTREAM_TABLE->item[BB].BitAddr[7]          = (uint32_t*)jit_blackbox_PR8_bit;

     BITSTREAM_TABLE->item[BB].BitSize[0]          = (uint32_t) jit_blackbox_PR1_bit_len;
     BITSTREAM_TABLE->item[BB].BitSize[1]          = (uint32_t) jit_blackbox_PR2_bit_len;
     BITSTREAM_TABLE->item[BB].BitSize[2]          = (uint32_t) jit_blackbox_PR3_bit_len;
     BITSTREAM_TABLE->item[BB].BitSize[3]          = (uint32_t) jit_blackbox_PR4_bit_len;
     BITSTREAM_TABLE->item[BB].BitSize[4]          = (uint32_t) jit_blackbox_PR5_bit_len;
     BITSTREAM_TABLE->item[BB].BitSize[5]          = (uint32_t) jit_blackbox_PR6_bit_len;
     BITSTREAM_TABLE->item[BB].BitSize[6]          = (uint32_t) jit_blackbox_PR7_bit_len;
     BITSTREAM_TABLE->item[BB].BitSize[7]          = (uint32_t) jit_blackbox_PR8_bit_len;
  //////////////////////////////////////////////////////////////////////////////

  #ifdef VERBOSE
    printf("[DEBUG->VAM_BITSTREAM_TABLE_INIT] DONE\r\n");
  #endif
}

void VAM_VM_INIT(vam_vm_t *VM, int argc, char* argv[])
{
  #ifdef VERBOSE
    printf("\r\n");
  #endif

  int i, j;
  int index;
  int err;
  int tmp;
  const char* bitFileName;
  char        ibuf[1024];
  #ifdef VERBOSE
    printf("[DEBUG->VAM_VM_INIT] INIT\r\n");
  #endif

  pthread_mutex_init(&VM->vm_mutex, NULL);
  VM->VAM_TABLE = new vector<vam_node_t>;
  VM->BITSTREAM_TABLE = new vam_Bitstream_table_t;

  switch(argc) {
    case 1: {
      bitFileName = argv[1];;
      for (i = 0; i < CARD; i++) {
        PICO_CONFIG cfg = PICO_CONFIG();
        cfg.model = 0x505;
        if ((err = FindPico(&cfg, &VM->pico[i])) < 0) {
          printf("[DEBUG->Download] FindPico Error\r\n");
          exit(1);
        }
      }
      // Init TieStream Table
      #ifdef VERBOSE
      printf("[DEBUG->VAM_VM_INIT] Create TieStreamTable\r\n");
      #endif
      VM->file = fopen(".TieStreamTable", "r+");
      if (VM->file == NULL) {
        printf("[DEBUG->VAM_VM_INIT] TieStreamTable Error\r\n");
        exit(1);
      }
      for (index = 0; index < CARD * NUM_ACCs; index++) {
        // err = fscanf(VM->file, "%d:%d:%d:%d\r\n", &tmp, &VM->TieStream_Table[index][0], &VM->TieStream_Table[index][1], &VM->TieStream_Table[index][2]);
        err = fscanf(VM->file, "%d:%d,%d\r\n", &tmp, &VM->TieStream_Table[index][0], &VM->TieStream_Table[index][1]);
      }
      fclose(VM->file);
    }break;

    case 2: {
      bitFileName = argv[1];;
      for (i = 0; i < CARD; i++) {
        // #ifdef VERBOSE
        printf("[DEBUG->Download] Loading Static bit on %d FPGA: '%s' ...\n", i, bitFileName);
        // #endif
        err = RunBitFile(bitFileName, &VM->pico[i]);
        if (err < 0) {
            fprintf(stderr, "RunBitFile error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
            exit(1);
        }
      }
      // Init TieStream Table
      // printf("[DEBUG->VAM_VM_INIT] Create TieStreamTable\r\n");
      // VM->file = fopen(".TieStreamTable", "w+");
      // if (VM->file == NULL) {
      //   printf("[DEBUG->VAM_VM_INIT] TieStreamTable Error\r\n");
      //   exit(1);
      // }
      for (index = 0; index < CARD * NUM_ACCs; index++) {
        VM->TieStream_Table[index][0] = 0;
        VM->TieStream_Table[index][1] = 0;
        // VM->TieStream_Table[index][2] = 0;
      }
    }break;

    default: {
      fprintf(stderr, "Please specify the .bit file on the command line.\n"
                      "For example: pbc ../firmware/M501_LX240_StreamLoopback128.bit\n");
      exit(1);
    }break;
  }

  #ifdef VERBOSE
    printf("[DEBUG->VAM_VM_INIT] TieStreamTable Loaded\r\n");
    for (index = 0; index < CARD * NUM_ACCs; index++) {
      printf("%4d:%2d,%4d\r\n", index, VM->TieStream_Table[index][0], VM->TieStream_Table[index][1]);
      // printf("%4d:%2d:%2d:%2d\r\n", index, VM->TieStream_Table[index][0], VM->TieStream_Table[index][1], VM->TieStream_Table[index][2]);
    }
  #endif

  VAM_BITSTREAM_TABLE_INIT(VM->BITSTREAM_TABLE);
  VAM_TABLE_INIT(VM->pico, VM->VAM_TABLE);
  #ifdef VERBOSE
    printf("[DEBUG->VAM_VM_INIT] DONE\r\n");
  #endif
}

void VAM_VM_CLEAN(vam_vm_t *VM)
{
  #ifdef VERBOSE
    printf("\r\n");
  #endif

  #ifdef VERBOSE
    printf("[DEBUG->VAM_VM_CLEAN] Close File\r\n");
  #endif
  int index;
  VM->file = fopen(".TieStreamTable", "w+");
  if (VM->file == NULL) {
    printf("[DEBUG->VAM_VM_INIT] TieStreamTable Error\r\n");
    exit(1);
  }
  for (index = 0; index < CARD * NUM_ACCs; index++) {
    fprintf(VM->file, "%4d:%2d,%4d\r\n", index, VM->TieStream_Table[index][0], VM->TieStream_Table[index][1]);
    // fprintf(VM->file, "%4d: %d: %d: %d\r\n", index, VM->TieStream_Table[index][0], VM->TieStream_Table[index][1], VM->TieStream_Table[index][2]);
  }
  fclose(VM->file);

  #ifdef VERBOSE
    printf("[DEBUG->VAM_VM_CLEAN] Destroy VM Mutex\r\n");
  #endif
    pthread_mutex_destroy(&VM->vm_mutex);
  #ifdef VERBOSE
    printf("[DEBUG->VAM_VM_CLEAN] CLEAN\r\n");
  #endif
    VAM_TABLE_CLEAN(VM);
  #ifdef VERBOSE
    printf("[DEBUG->VAM_VM_CLEAN] Del VAM_TABLE\r\n");
  #endif
    delete VM->VAM_TABLE;
  #ifdef VERBOSE
    printf("[DEBUG->VAM_VM_CLEAN] Del BITSTREAM_TABLE\r\n");
  #endif
    delete VM->BITSTREAM_TABLE;
  #ifdef VERBOSE
    printf("[DEBUG->VAM_VM_CLEAN] DONE\r\n");
  #endif
}
//==================================================================================================
//  ____    ____ .__   __.  ___________    __    ____
//  \   \  /   / |  \ |  | |   ____\   \  /  \  /   /
//   \   \/   /  |   \|  | |  |__   \   \/    \/   /
//    \      /   |  . `  | |   __|   \            /
//     \    /    |  |\   | |  |____   \    /\    /
//      \__/     |__| \__| |_______|   \__/  \__/
//==================================================================================================
int vnew(vam_vm_t *VM, vector<int> *nPR)
{
  #ifdef VERBOSE
    printf("\r\n");
  #endif
  pthread_t thread;
  vm_pk_t vnew_package;
  vnew_package.nPR  = nPR;
  vnew_package.VM   = VM;

  pthread_create(&thread, NULL, vnew_Threads_Call, (void*) &vnew_package);
  #ifdef VERBOSE
    printf("[DEBUG->vnew] vnew thread created\r\n");
  #endif
    pthread_join(thread, NULL);
  #ifdef VERBOSE
    printf("[DEBUG->vnew] vnew thread joined\r\n");
  #endif
  return 0;
}

void * vnew_Threads_Call(void *pk)
{
  #ifdef VERBOSE_THREAD
    printf("\r\n");
  #endif
  int  i;
  int  obtained = 0;
  vm_pk_t *p = (vm_pk_t *) pk;
  int size = p->nPR->size(); // Size is the nodes requires

  while (obtained < size){

    #ifdef VERBOSE_THREAD
      printf("[DEBUG->vnew_TCALL] vnew thread request mutex, obtained:%d, needed:%d\r\n", obtained, size);
    #endif
    pthread_mutex_lock(&p->VM->vm_mutex);
    #ifdef VERBOSE_THREAD
      printf("[DEBUG->vnew_TCALL] vnew thread get mutex...\r\n");
    #endif

    vector<vam_node_t>::iterator v = p->VM->VAM_TABLE->begin();
    while( v != p->VM->VAM_TABLE->end()) {
      if(v->status == PRFREE) {
        p->nPR->at(obtained) = v->card_key << 4 | v->node_key; // 8'h {card_key, node_key} Save the info of both
        v->status = PRBUSY;
        #ifdef VERBOSE_THREAD
          int       nPR       = p->nPR->at(obtained);
          int       nPR_card  = nPR >> 4;
          int       nPR_node  = nPR & 0xF;
          int       nPR_index = nPR_card * ROW + nPR_node;
          printf("[DEBUG->vnew_TCALL] vnew thread get id: %d, Card:%d, Node:%d, index:%d\r\n", nPR, nPR_card, nPR_node, nPR_index);
        #endif
        obtained++;
        break;
      }
      v++;
    }

    #ifdef VERBOSE_THREAD
      printf("[DEBUG->vnew_TCALL] vnew thread release mutex...\r\n");
    #endif
    pthread_mutex_unlock(&p->VM->vm_mutex);
  }
  return NULL;
}
//==================================================================================================
//  ____    ____  _______   _______  __
//  \   \  /   / |       \ |   ____||  |
//   \   \/   /  |  .--.  ||  |__   |  |
//    \      /   |  |  |  ||   __|  |  |
//     \    /    |  '--'  ||  |____ |  `----.
//      \__/     |_______/ |_______||_______|
//==================================================================================================
int vdel(vam_vm_t *VM, vector<int> *nPR)
{
  #ifdef VERBOSE
    printf("\r\n");
  #endif

  pthread_t thread;
  vm_pk_t vdel_package;
  vdel_package.nPR  = nPR;
  vdel_package.VM   = VM;

  pthread_create(&thread, NULL, vdel_Threads_Call, (void*) &vdel_package);
  #ifdef VERBOSE
    printf("[DEBUG->vdel] vdel thread created\r\n");
  #endif
    pthread_join(thread, NULL);
  #ifdef VERBOSE
    printf("[DEBUG->vdel] vdel thread joined\r\n");
  #endif
  return 0;
}

void * vdel_Threads_Call(void *pk)
{
#ifdef VERBOSE_THREAD
  printf("\r\n");
#endif
  int       err;
  int       stream;
  int       card  ;
  int       node  ;
  int       index ;
  int       i     ;
  int       size  ;
  int       obtained = 0;
  vm_pk_t *p = (vm_pk_t *) pk;

  #ifdef VERBOSE_THREAD
    printf("[DEBUG->vdel_TCALL] vdel thread request mutex...\r\n");
  #endif

  pthread_mutex_lock(&p->VM->vm_mutex);
  #ifdef VERBOSE_THREAD
    printf("[DEBUG->vdel_TCALL] vdel thread get mutex...\r\n");
  #endif

  #ifdef VERBOSE_THREAD
    printf("[DEBUG->vdel_TCALL] Reset and Del\r\n");
  #endif
  size = p->nPR->size();
  for (i = 0; i < size; i++) {
    card  = p->nPR->at(i) >> 4;
    node  = p->nPR->at(i) & 0xF;
    index = card * ROW + node;
    p->VM->VAM_TABLE->at(index).status     = PRFREE;
    p->VM->VAM_TABLE->at(index).in1        = NULL;
    p->VM->VAM_TABLE->at(index).in2        = NULL;

    if (p->VM->VAM_TABLE->at(index).out != NULL) {
      if (p->VM->VAM_TABLE->at(index).tie_out == 1){
        if(p->VM->VAM_TABLE->at(index).node_type == Buf_Buf_Reg ||
           p->VM->VAM_TABLE->at(index).node_type == Buf_Reg_Reg ||
           p->VM->VAM_TABLE->at(index).node_type == Reg_Buf_Reg ||
           p->VM->VAM_TABLE->at(index).node_type == Reg_Reg_Reg ) {
           #ifdef VERBOSE_THREAD
             printf("[DEBUG->vdel_TCALL] out tie 1\r\n");
           #endif
           delete[] p->VM->VAM_TABLE->at(index).out;
        }
      } else {
        #ifdef VERBOSE_THREAD
          printf("[DEBUG->vdel_TCALL] out tie 0\r\n");
        #endif
        p->VM->VAM_TABLE->at(index).out        = NULL;
      }
    }
    p->VM->VAM_TABLE->at(index).tie_in1    =  0;
    p->VM->VAM_TABLE->at(index).tie_in2    =  0;
    p->VM->VAM_TABLE->at(index).tie_out    =  0;
    p->VM->VAM_TABLE->at(index).node_type  = -1;
    p->VM->VAM_TABLE->at(index).tiestream  =  0;
  }

  #ifdef VERBOSE_THREAD
    printf("[DEBUG->vdel_TCALL] vdel thread done and release mutex...\r\n");
  #endif
  pthread_mutex_unlock(&p->VM->vm_mutex);
  return NULL;
}
//==================================================================================================
//  ____    ____  __      .______   .______
//  \   \  /   / |  |     |   _  \  |   _  \
//   \   \/   /  |  |     |  |_)  | |  |_)  |
//    \      /   |  |     |   ___/  |      /
//     \    /    |  `----.|  |      |  |\  \----.
//      \__/     |_______|| _|      | _| `._____|
//==================================================================================================
int vlpr(vam_vm_t *VM, int nPR, int PR_NAME)
{
  #ifdef VERBOSE
    printf("\r\n");
  #endif

  struct timeval start, end;
  int timeuse;

  int card     = nPR >> 4;
  int node     = nPR & 0xF;
  int index    = card * ROW + node;

  #ifdef VERBOSE
    printf("[DEBUG->vlpr] vlpr thread request mutex...\r\n");
  #endif

  pthread_mutex_lock(&VM->vm_mutex);
  #ifdef VERBOSE
    printf("[DEBUG->vlpr] vlpr thread get mutex...\r\n");
  #endif

  #ifdef VERBOSE
    printf("[DEBUG->vlpr] Reset and Del\r\n");
  #endif
//==================================================================================================
  #ifdef VERBOSE
    printf("[DEBUG->vlpr] nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR, card, node, index);
  #endif

  uint32_t    cmd[4];
  int         icap_stream;
  int         cmd_stream;
  int         err;
  char        ibuf[1024];

  cmd[3] = 0xBABEFACE;
  cmd[2] = 0xDEADBEEF;
  cmd[1] = 0xDEADBEEF;
  cmd[0] = 0xD000BEEF | (node + 1 << 24); // PR Start CMD
  #ifdef VERBOSE
    printf("[DEBUG->vlpr] Opening cmd streams 50\r\n");
    printf("[DEBUG->vlpr] Sending Start PR command to JIT, 0x%08x\r\n", cmd[0]);
  #endif
  cmd_stream = VM->pico[card]->CreateStream(50);
  VM->pico[card]->WriteStream(cmd_stream, cmd, 16);

  if (VM->TieStream_Table[index][1] != PR_NAME) { // if the node does not have this acc before
    // VM->VAM_TABLE->at(index).PR_key = PR_NAME;
    VM->TieStream_Table[index][1] = PR_NAME;

    #ifdef VERBOSE
      printf("[DEBUG->vlpr] Opening streams 100 for ICAP\r\n");
    #endif
    icap_stream = VM->pico[card]->CreateStream(100);

    // Send PR
    #ifdef VERBOSE
      printf("[DEBUG->vlpr] Writing %i Bytes to PR%d\n", VM->BITSTREAM_TABLE->item[PR_NAME].BitSize[node] * 4, node);
    #endif

    #ifdef PR
    // gettimeofday(&start, NULL);
    err = VM->pico[card]->WriteStream(icap_stream, VM->BITSTREAM_TABLE->item[PR_NAME].BitAddr[node], VM->BITSTREAM_TABLE->item[PR_NAME].BitSize[node] * 4); // Write bytes not words.
    // gettimeofday(&end, NULL);
    // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("[DEBUG->vlpr] Size: %d Bytes, VLPR: %'9d us\r\n", VM->BITSTREAM_TABLE->item[PR_NAME].BitSize[node] * 4, timeuse);
    // printf("[DEBUG->vlpr] Thoughtput %lf MB/s\r\n", (VM->BITSTREAM_TABLE->item[PR_NAME].BitSize[node] * 4.0 / 1024 / 1024) / (timeuse / 1000000) );
    if (err < 0) {
        fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
        return -1;
    }
    #endif

    usleep(500); // Need some delay for ICAP to finish the PR process. 500ms is more stable

    #ifdef VERBOSE
      printf("[DEBUG->vlpr] Closing streams 100 for ICAP\r\n");
    #endif
    VM->pico[card]->CloseStream(icap_stream);
  } else {
    #ifdef VERBOSE
      printf("[DEBUG->vlpr] Acc has been loaded already!\r\n");
    #endif
  }

  cmd[0] = 0xD000DEAD | (node + 1 << 24); // PR End CMD
  #ifdef VERBOSE
    printf("[DEBUG->vlpr] Sending End PR command to JIT, 0x%08x\r\n", cmd[0]);
  #endif
  VM->pico[card]->WriteStream(cmd_stream, cmd, 16);

  #ifdef VERBOSE
    printf("[DEBUG->vlpr] Closing cmd streams 50\r\n");
  #endif
  VM->pico[card]->CloseStream(cmd_stream);
//==================================================================================================
    VM->VAM_TABLE->at(index).A3_cmd = 0xC0300001 | (node + 1 << 24) ;
//==================================================================================================
  #ifdef VERBOSE
    printf("[DEBUG->vlpr] vlpr thread done and release mutex...\r\n");
  #endif
  pthread_mutex_unlock(&VM->vm_mutex);
  return 0;
}
// int vlpr(vam_vm_t *VM, int nPR, int PR_NAME)
// {
//   #ifdef VERBOSE
//     printf("\r\n");
//   #endif

//   pthread_t thread;
//   vm_pk_t vlpr_package;
//   // vlpr_package.nPR  = nPR here, is a number
//   vlpr_package.VM      = VM;
//   vlpr_package.PR_NAME = nPR << 16 | PR_NAME;

//   pthread_create(&thread, NULL, vlpr_Threads_Call, (void*) &vlpr_package);
//   #ifdef VERBOSE
//     printf("[DEBUG->vlpr] vlpr thread created\r\n");
//   #endif
//     pthread_join(thread, NULL);
//   #ifdef VERBOSE
//     printf("[DEBUG->vlpr] vlpr thread joined\r\n");
//   #endif
//   return 0;
// }

// void * vlpr_Threads_Call(void *pk)
// {
//   #ifdef VERBOSE_THREAD
//     printf("\r\n");
//   #endif
//   vm_pk_t *p   = (vm_pk_t *) pk;
//   int nPR      = p->PR_NAME >> 16;     // Grep nPR, a number
//   int PR_NAME  = p->PR_NAME & 0xFFFF;  // Grep PR Type
//   int card     = nPR >> 4;
//   int node     = nPR & 0xF;
//   int index    = card * ROW + node;
//   vam_vm_t *VM = p->VM;

//   #ifdef VERBOSE_THREAD
//     printf("[DEBUG->vlpr_TCALL] vlpr thread request mutex...\r\n");
//   #endif

//   pthread_mutex_lock(&p->VM->vm_mutex);
//   #ifdef VERBOSE_THREAD
//     printf("[DEBUG->vlpr_TCALL] vlpr thread get mutex...\r\n");
//   #endif

//   #ifdef VERBOSE_THREAD
//     printf("[DEBUG->vlpr_TCALL] Reset and Del\r\n");
//   #endif
// //==================================================================================================
//   #ifdef VERBOSE_THREAD
//     printf("[DEBUG->vlpr_TCALL] nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR, card, node, index);
//   #endif

//   uint32_t    cmd[4];
//   int         icap_stream;
//   int         cmd_stream;
//   int         err;
//   char        ibuf[1024];

//   cmd[3] = 0xBABEFACE;
//   cmd[2] = 0xDEADBEEF;
//   cmd[1] = 0xDEADBEEF;
//   cmd[0] = 0xD000BEEF | (node + 1 << 24); // PR Start CMD
//   #ifdef VERBOSE_THREAD
//     printf("[DEBUG->vlpr_TCALL] Opening cmd streams 50\r\n");
//     printf("[DEBUG->vlpr_TCALL] Sending Start PR command to JIT, 0x%08x\r\n", cmd[0]);
//   #endif
//   cmd_stream = VM->pico[card]->CreateStream(50);
//   VM->pico[card]->WriteStream(cmd_stream, cmd, 16);

//   if (VM->VAM_TABLE->at(index).PR_key != PR_NAME) { // if the node does not have this acc before
//     VM->VAM_TABLE->at(index).PR_key = PR_NAME;

//     #ifdef VERBOSE_THREAD
//       printf("[DEBUG->vlpr_TCALL] Opening streams 100 for ICAP\r\n");
//     #endif
//     icap_stream = VM->pico[card]->CreateStream(100);

//     // Send PR
//     #ifdef VERBOSE_THREAD
//       printf("[DEBUG->vlpr_TCALL] Writing %i Bytes to PR%d\n", VM->BITSTREAM_TABLE->item[PR_NAME].BitSize[node] * 4, node);
//     #endif

//     #ifdef PR
//     err = VM->pico[card]->WriteStream(icap_stream, VM->BITSTREAM_TABLE->item[PR_NAME].BitAddr[node], VM->BITSTREAM_TABLE->item[PR_NAME].BitSize[node] * 4); // Write bytes not words.
//     if (err < 0) {
//         fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
//         return NULL;
//     }
//     #endif

//     usleep(500); // Need some delay for ICAP to finish the PR process. 500ms is more stable

//     #ifdef VERBOSE_THREAD
//       printf("[DEBUG->vlpr_TCALL] Closing streams 100 for ICAP\r\n");
//     #endif
//     VM->pico[card]->CloseStream(icap_stream);
//   }

//   cmd[0] = 0xD000DEAD | (node + 1 << 24); // PR End CMD
//   #ifdef VERBOSE_THREAD
//     printf("[DEBUG->vlpr_TCALL] Sending End PR command to JIT, 0x%08x\r\n", cmd[0]);
//   #endif
//   VM->pico[card]->WriteStream(cmd_stream, cmd, 16);

//   // int k, room, i;
//   // int *tmp = NULL;
//   // k = 0;
//   // do {
//   //   #ifdef VERBOSE_THREAD
//   //     printf("[DEBUG->vlpr_TCALL] %i B available to read from 50. %5d\r\n", i=VM->pico[card]->GetBytesAvailable(cmd_stream, true), k);
//   //   #endif
//   //   if (i < 0){
//   //       fprintf(stderr, "[DEBUG->vlpr_TCALL] GetBytesAvailable error: %s\n", PicoErrors_FullError(i, ibuf, sizeof(ibuf)));
//   //       exit(1);
//   //   }
//   //   room = i / 4;
//   //   tmp = new int[room];
//   //   VM->pico[card]->ReadStream(cmd_stream, tmp, i);
//   //   #ifdef VERBOSE_THREAD
//   //     for (i = 0; i < room; i++) {
//   //       // printf("[DEBUG->vlpr_TCALL] [%d] 0x%08x\r\n", i, tmp[i]);
//   //     }
//   //   #endif
//   //   delete[] tmp;
//   //   k++;
//   // } while (room != 0 && k < 10000);

//   #ifdef VERBOSE_THREAD
//     printf("[DEBUG->vlpr_TCALL] Closing cmd streams 50\r\n");
//   #endif
//   VM->pico[card]->CloseStream(cmd_stream);
// //==================================================================================================
//   #ifdef VERBOSE_THREAD
//     printf("[DEBUG->vlpr_TCALL] vlpr thread done and release mutex...\r\n");
//   #endif
//   pthread_mutex_unlock(&p->VM->vm_mutex);
//   return NULL;
// }
int vlprBB(vam_vm_t *VM, int nPR, int PR_NAME, int size)
{
  #ifdef VERBOSE
    printf("\r\n");
  #endif

  struct timeval start, end;
  int timeuse;

  int card     = nPR >> 4;
  int node     = nPR & 0xF;
  int index    = card * ROW + node;

  #ifdef VERBOSE
    printf("[DEBUG->vlpr] vlpr thread request mutex...\r\n");
  #endif

  pthread_mutex_lock(&VM->vm_mutex);
  #ifdef VERBOSE
    printf("[DEBUG->vlpr] vlpr thread get mutex...\r\n");
  #endif

  #ifdef VERBOSE
    printf("[DEBUG->vlpr] Reset and Del\r\n");
  #endif
//==================================================================================================
  #ifdef VERBOSE
    printf("[DEBUG->vlpr] nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR, card, node, index);
  #endif

  uint32_t    cmd[4];
  int         icap_stream;
  int         cmd_stream;
  int         err;
  char        ibuf[1024];

  cmd[3] = 0xBABEFACE;
  cmd[2] = 0xDEADBEEF;
  cmd[1] = 0xDEADBEEF;
  cmd[0] = 0xD000BEEF | (node + 1 << 24); // PR Start CMD
  #ifdef VERBOSE
    printf("[DEBUG->vlpr] Opening cmd streams 50\r\n");
    printf("[DEBUG->vlpr] Sending Start PR command to JIT, 0x%08x\r\n", cmd[0]);
  #endif
  cmd_stream = VM->pico[card]->CreateStream(50);
  VM->pico[card]->WriteStream(cmd_stream, cmd, 16);

  if (VM->TieStream_Table[index][1] != BB) { // if the node does not have this acc before
    // VM->VAM_TABLE->at(index).PR_key = BB;
    VM->TieStream_Table[index][1] = BB;

    #ifdef VERBOSE
      printf("[DEBUG->vlpr] Opening streams 100 for ICAP\r\n");
    #endif
    icap_stream = VM->pico[card]->CreateStream(100);

    // Send PR
    #ifdef VERBOSE
      printf("[DEBUG->vlpr] Writing %i Bytes to PR%d\n", VM->BITSTREAM_TABLE->item[BB].BitSize[node] * 4, node);
    #endif

    #ifdef PR
    // gettimeofday(&start, NULL);
    err = VM->pico[card]->WriteStream(icap_stream, VM->BITSTREAM_TABLE->item[BB].BitAddr[node], VM->BITSTREAM_TABLE->item[BB].BitSize[node] * 4); // Write bytes not words.
    // gettimeofday(&end, NULL);
    // timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("[DEBUG->vlpr] Size: %d Bytes, VLPR: %'9d us\r\n", VM->BITSTREAM_TABLE->item[BB].BitSize[node] * 4, timeuse);
    // printf("[DEBUG->vlpr] Thoughtput %lf MB/s\r\n", (VM->BITSTREAM_TABLE->item[BB].BitSize[node] * 4.0 / 1024 / 1024) / (timeuse / 1000000) );
    if (err < 0) {
        fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
        return -1;
    }
    #endif

    usleep(500); // Need some delay for ICAP to finish the PR process. 500ms is more stable

    #ifdef VERBOSE
      printf("[DEBUG->vlpr] Closing streams 100 for ICAP\r\n");
    #endif
    VM->pico[card]->CloseStream(icap_stream);
  } else {
    #ifdef VERBOSE
      printf("[DEBUG->vlpr] Acc has been loaded already!\r\n");
    #endif
  }

  cmd[0] = 0xD000DEAD | (node + 1 << 24); // PR End CMD
  #ifdef VERBOSE
    printf("[DEBUG->vlpr] Sending End PR command to JIT, 0x%08x\r\n", cmd[0]);
  #endif
  VM->pico[card]->WriteStream(cmd_stream, cmd, 16);

  #ifdef VERBOSE
    printf("[DEBUG->vlpr] Closing cmd streams 50\r\n");
  #endif
  VM->pico[card]->CloseStream(cmd_stream);
//==================================================================================================
  switch (PR_NAME) {
    case INSERTION : {
      VM->VAM_TABLE->at(index).A3_cmd = 0xC0300001 | (node + 1 << 24) | size << 8;
    }break;

    case MERGE : {
      VM->VAM_TABLE->at(index).A3_cmd = 0xC0300212 | (node + 1 << 24);
    }break;

    case VADD : {
      VM->VAM_TABLE->at(index).A3_cmd = 0xC0300002 | (node + 1 << 24);
    }break;

    case VMUL : {
      VM->VAM_TABLE->at(index).A3_cmd = 0xC0300202 | (node + 1 << 24);
    }break;

    case DES : {
      VM->VAM_TABLE->at(index).A3_cmd = 0xC0300F02 | (node + 1 << 24);
    }break;

    default: {
      VM->VAM_TABLE->at(index).A3_cmd = 0xC0300002 | (node + 1 << 24) ;
    }break;
  }
//==================================================================================================
  #ifdef VERBOSE
    printf("[DEBUG->vlpr] vlpr thread done and release mutex...\r\n");
  #endif
  pthread_mutex_unlock(&VM->vm_mutex);
  return 0;
}

//==================================================================================================
//  ____    ____ .___________. __   _______  __    ______
//  \   \  /   / |           ||  | |   ____||  |  /  __  \
//   \   \/   /  `---|  |----`|  | |  |__   |  | |  |  |  |
//    \      /       |  |     |  | |   __|  |  | |  |  |  |
//     \    /        |  |     |  | |  |____ |  | |  `--'  |
//      \__/         |__|     |__| |_______||__|  \______/
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  ,-----.  ,-----.  ,-----.
//  |  |) /_ |  |) /_ |  |) /_
//  |  .-.  \|  .-.  \|  .-.  \
//  |  '--' /|  '--' /|  '--' /
//  `------' `------' `------'
//--------------------------------------------------------------------------------------------------
int vtieio(vam_vm_t *VM, int nPR, int *in1, int size_in1, int *in2, int size_in2, int *out, int size_out)
{
#ifdef VERBOSE
  printf("\r\n");
#endif

  uint32_t  cmd[4] = {0, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
  char      ibuf[1024];
  int       err;
  int       cmd_stream;

  // int       card  = nPR >> 4;
  // int       node  = nPR & 0xF;
  // int       index = card * ROW + node;

  int       nPR_card  = nPR >> 4;
  int       nPR_node  = nPR & 0xF;
  int       nPR_index = nPR_card * ROW + nPR_node;

#ifdef VERBOSE
  printf("[DEBUG->vtieio] Buf_Buf_Buf, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR, nPR_card, nPR_node, nPR_index);
  // printf("[DEBUG->vtieio] Opening CMD Stream\r\n");
#endif

  VM->VAM_TABLE->at(nPR_index).node_type = Buf_Buf_Buf;

  // request Mutex for CMD Stream
  #ifdef VERBOSE
    printf("[DEBUG->vtieio] vstart thread request mutex for cmd_stream\r\n");
  #endif
  pthread_mutex_lock(&VM->vm_mutex);

  #ifdef VERBOSE
    printf("[DEBUG->vtieio] vstart thread get mutex...\r\n");
  #endif
  cmd_stream = VM->pico[nPR_card]->CreateStream(50);

  // cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 >> 16)        ;
  // cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 & 0x0000FFFF) ;
  cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | (size_out >> 16       ) ;
  cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | (size_out & 0x0000FFFF) ;
  // cmd[2] = 0xC0300202 | (nPR_node + 1 << 24) ;//| (0 << 8 | 0 << 4 | 2)   ; // 0x0002
  cmd[2] = VM->VAM_TABLE->at(nPR_index).A3_cmd;
  cmd[3] = 0xB0000111 | (nPR_node + 1 << 24)                           ;

  VM->VAM_TABLE->at(nPR_index).cur_cmd = cmd[3];
  VM->VAM_TABLE->at(nPR_index).in1     = in1;
  VM->VAM_TABLE->at(nPR_index).in2     = in2;
  VM->VAM_TABLE->at(nPR_index).out     = out;

  VM->VAM_TABLE->at(nPR_index).size_in1 = size_in1;
  VM->VAM_TABLE->at(nPR_index).size_in2 = size_in2;
  VM->VAM_TABLE->at(nPR_index).size_out = size_out;

  #ifdef VERBOSE
    printf("[DEBUG->vtieio] Sending command to nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR, nPR_card, nPR_node, nPR_index);
    printf("[DEBUG->vtieio] Sending command : %d, 0x%x, 0x%x\r\n", (size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1, (size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 >> 16, (size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 & 0x0000FFFF);
    printf("[DEBUG->vtieio] Sending command :0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", cmd[0], cmd[1], cmd[2], cmd[3]);
  #endif
  err = VM->pico[nPR_card]->WriteStream(cmd_stream, cmd, 16);
  if (err < 0) {
    fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
    return -1;
  }

  VM->pico[nPR_card]->CloseStream(cmd_stream);
  // Releast Mutex on CMD Stream
  #ifdef VERBOSE
    printf("[DEBUG->vtieio] vstart thread release mutex...\r\n");
  #endif
  pthread_mutex_unlock(&VM->vm_mutex);

  return 0;
}
//--------------------------------------------------------------------------------------------------
//  ,-----.  ,-----.  ,------.
//  |  |) /_ |  |) /_ |  .--. '
//  |  .-.  \|  .-.  \|  '--'.'
//  |  '--' /|  '--' /|  |\  \
//  `------' `------' `--' '--'
//--------------------------------------------------------------------------------------------------
int vtieio(vam_vm_t *VM, int nPR, int *in1, int size_in1, int *in2, int size_in2, int out, int size_out)
{
#ifdef VERBOSE
  printf("\r\n");
#endif

  uint32_t  cmd[4] = {0, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
  char      ibuf[1024];
  int       err;
  int       cmd_stream;

  int       nPR_card  = nPR >> 4;
  int       nPR_node  = nPR & 0xF;
  int       nPR_index = nPR_card * ROW + nPR_node;

  // int       in1_card  = in1 >> 4;
  // int       in1_node  = in1 & 0xF;
  // int       in1_index = in1_card * ROW + in1_node;

  // int       in2_card  = in2 >> 4;
  // int       in2_node  = in2 & 0xF;
  // int       in2_index = in2_card * ROW + in2_node;

  int       out_card  = out >> 4;
  int       out_node  = out & 0xF;
  int       out_index = out_card * ROW + out_node;
  // ===========================================================================
  VM->VAM_TABLE->at(nPR_index).node_type = Buf_Buf_Reg;

  // request Mutex for CMD Stream
  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread request mutex for cmd_stream\r\n");
  #endif
  pthread_mutex_lock(&VM->vm_mutex);

  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread get mutex...\r\n");
  #endif
  cmd_stream = VM->pico[nPR_card]->CreateStream(50);

  // cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 >> 16)        ;
  // cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 & 0x0000FFFF) ;
  cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | (size_out >> 16       ) ;
  cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | (size_out & 0x0000FFFF) ;
  // cmd[2] = 0xC0300001 | (nPR_node + 1 << 24)                           ;
  cmd[2] = VM->VAM_TABLE->at(nPR_index).A3_cmd;


  if (nPR_card == out_card) {
    #ifdef VERBOSE
      printf("[DEBUG->vtieio] BBR, nPR:%3d, Card:%3d, Node:%3d, index:%3d\r\n", nPR, nPR_card, nPR_node, nPR_index);
      printf("[DEBUG->vtieio] BBR, Out:%3d, Card:%3d, Node:%3d, index:%3d\r\n", out, out_card, out_node, out_index);
    #endif

    cmd[3] = 0xB0000112 | (nPR_node + 1 << 24)                       ;

  } else {
    #ifdef VERBOSE
      printf("[DEBUG->vtieio] BBR', nPR:%3d, Card:%3d, Node:%3d, index:%3d\r\n", nPR, nPR_card, nPR_node, nPR_index);
      printf("[DEBUG->vtieio] BBR', Out:%3d, Card:%3d, Node:%3d, index:%3d\r\n", out, out_card, out_node, out_index);
    #endif

    cmd[3] = 0xB0000113 | (nPR_node + 1 << 24)                       ;

  }

  VM->VAM_TABLE->at(nPR_index).cur_cmd = cmd[3];
  VM->VAM_TABLE->at(nPR_index).in1     = in1;
  VM->VAM_TABLE->at(nPR_index).in2     = in2;
  VM->VAM_TABLE->at(nPR_index).out     = NULL;
  VM->VAM_TABLE->at(nPR_index).tie_out = out;

  VM->VAM_TABLE->at(nPR_index).size_in1 = size_in1;
  VM->VAM_TABLE->at(nPR_index).size_in2 = size_in2;
  VM->VAM_TABLE->at(nPR_index).size_out = size_out;

  #ifdef VERBOSE
    // printf("[DEBUG->vtieio] Sending command to nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR, nPR_card, nPR_node, nPR_index);
    // printf("[DEBUG->vtieio] Sending command : %d, 0x%x, 0x%x\r\n", (size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1, (size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 >> 16, (size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 & 0x0000FFFF);
    printf("[DEBUG->vtieio] CMD: 0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", cmd[0], cmd[1], cmd[2], cmd[3]);
  #endif
  err = VM->pico[nPR_card]->WriteStream(cmd_stream, cmd, 16);
  if (err < 0) {
    fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
    return -1;
  }

  VM->pico[nPR_card]->CloseStream(cmd_stream);
  // Releast Mutex on CMD Stream
  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread release mutex...\r\n");
  #endif
  pthread_mutex_unlock(&VM->vm_mutex);
  return 0;
}
//--------------------------------------------------------------------------------------------------
//  ,------. ,------. ,-----.
//  |  .--. '|  .--. '|  |) /_
//  |  '--'.'|  '--'.'|  .-.  \
//  |  |\  \ |  |\  \ |  '--' /
//  `--' '--'`--' '--'`------'
//--------------------------------------------------------------------------------------------------
int vtieio(vam_vm_t *VM, int nPR, int in1, int size_in1, int in2, int size_in2, int *out, int size_out)
{
#ifdef VERBOSE
  printf("\r\n");
#endif

  uint32_t  cmd[4] = {0, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
  char      ibuf[1024];
  int       err;
  int       cmd_stream;

  int       nPR_card  = nPR >> 4;
  int       nPR_node  = nPR & 0xF;
  int       nPR_index = nPR_card * ROW + nPR_node;

  int       in1_card  = in1 >> 4;
  int       in1_node  = in1 & 0xF;
  int       in1_index = in1_card * ROW + in1_node;

  int       in2_card  = in2 >> 4;
  int       in2_node  = in2 & 0xF;
  int       in2_index = in2_card * ROW + in2_node;
  // ===========================================================================
  VM->VAM_TABLE->at(nPR_index).node_type = Reg_Reg_Buf;

  // request Mutex for CMD Stream
  #ifdef VERBOSE
    printf("[DEBUG->vtieio] vstart thread request mutex for cmd_stream\r\n");
  #endif
  pthread_mutex_lock(&VM->vm_mutex);

  #ifdef VERBOSE
    printf("[DEBUG->vtieio] vstart thread get mutex...\r\n");
  #endif
  cmd_stream = VM->pico[nPR_card]->CreateStream(50);

  // cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 >> 16)                 ;
  // cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 & 0x0000FFFF)          ;
  // cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | (size_out >> 16       ) ;
  // cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | (size_out & 0x0000FFFF) ;
  cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | (size_in1 >> 16       ) ;
  cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | (size_in1 & 0x0000FFFF) ;
  // cmd[2] = 0xC0300001 | (nPR_node + 1 << 24)                                ;
  cmd[2] = VM->VAM_TABLE->at(nPR_index).A3_cmd;

  if (nPR_card == in1_card && nPR_card == in2_card) {
    #ifdef VERBOSE
      printf("[DEBUG->vtieio] RRB, nPR:%3d, Card:%3d, Node:%3d, index:%3d\r\n", nPR, nPR_card, nPR_node, nPR_index);
      printf("[DEBUG->vtieio] RRB, in1:%3d, Card:%3d, Node:%3d, index:%3d\r\n", in1, in1_card, in1_node, in1_index);
      printf("[DEBUG->vtieio] RRB, in2:%3d, Card:%3d, Node:%3d, index:%3d\r\n", in2, in2_card, in2_node, in2_index);
    #endif

    cmd[3] = 0xB0000221 | (nPR_node + 1 << 24) | (in1_node + 1 << 20) | (in2_node + 1 << 16) ;
  }
  //------------------------------------------------------------------------------------------------
  //  ,------. ,--.,------. ,-----.
  //  |  .--. '|  ||  .--. '|  |) /_
  //  |  '--'.'`-' |  '--'.'|  .-.  \
  //  |  |\  \     |  |\  \ |  '--' /
  //  `--' '--'    `--' '--'`------'
  //------------------------------------------------------------------------------------------------
  if (nPR_card != in1_card && nPR_card == in2_card) {
    #ifdef VERBOSE
      printf("[DEBUG->vtieio] R'RB, nPR:%3d, Card:%3d, Node:%3d, index:%3d\r\n", nPR, nPR_card, nPR_node, nPR_index);
      printf("[DEBUG->vtieio] R'RB, in1:%3d, Card:%3d, Node:%3d, index:%3d\r\n", in1, in1_card, in1_node, in1_index);
      printf("[DEBUG->vtieio] R'RB, in2:%3d, Card:%3d, Node:%3d, index:%3d\r\n", in2, in2_card, in2_node, in2_index);
    #endif

    cmd[3] = 0xB0000321 | (nPR_node + 1 << 24) | (in1_node + 1 << 20) | (in2_node + 1 << 16) ;

    int src = (in1_node + 1) * 10 + 6;
    int des = (nPR_node + 1) * 10 + 4;

    if (VM->TieStream_Table[in1_index][0] != des) {
      #ifdef VERBOSE
        printf("[DEBUG->vtieio] New TieSTream: %d -> %d\r\n", src, des);
      #endif
      err = VM->pico[in1_card]->TieStreams(src, VM->pico[nPR_card], des);
      if (err < 0) {
          fprintf(stderr, "TieStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
          exit(1);
      }
      VM->TieStream_Table[in1_index][0] = des;
    } else {
      #ifdef VERBOSE
        printf("[DEBUG->vtieio] Original TieSTream: %d -> %d\r\n", src, des);
      #endif
    }
  }

  VM->VAM_TABLE->at(nPR_index).cur_cmd = cmd[3];
  VM->VAM_TABLE->at(nPR_index).in1     = NULL;
  VM->VAM_TABLE->at(nPR_index).in2     = NULL;
  VM->VAM_TABLE->at(nPR_index).tie_in1 = in1;
  VM->VAM_TABLE->at(nPR_index).tie_in2 = in2;
  VM->VAM_TABLE->at(nPR_index).out     = out;

  VM->VAM_TABLE->at(nPR_index).size_in1 = size_in1;
  VM->VAM_TABLE->at(nPR_index).size_in2 = size_in2;
  VM->VAM_TABLE->at(nPR_index).size_out = size_out;

  #ifdef VERBOSE
    printf("[DEBUG->vtieio]  CMD: 0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", cmd[0], cmd[1], cmd[2], cmd[3]);
  #endif
  err = VM->pico[nPR_card]->WriteStream(cmd_stream, cmd, 16);
  if (err < 0) {
    fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
    return -1;
  }

  VM->pico[nPR_card]->CloseStream(cmd_stream);
  // Releast Mutex on CMD Stream
  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread release mutex...\r\n");
  #endif
  pthread_mutex_unlock(&VM->vm_mutex);

  return 0;
}
//--------------------------------------------------------------------------------------------------
//  ,------. ,------. ,------.
//  |  .--. '|  .--. '|  .--. '
//  |  '--'.'|  '--'.'|  '--'.'
//  |  |\  \ |  |\  \ |  |\  \
//  `--' '--'`--' '--'`--' '--'
//--------------------------------------------------------------------------------------------------
int vtieio(vam_vm_t *VM, int nPR, int in1, int size_in1, int in2, int size_in2, int out, int size_out)
{
#ifdef VERBOSE
  printf("\r\n");
#endif

  uint32_t  cmd[4] = {0, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
  char      ibuf[1024];
  int       err;
  int       cmd_stream;
  // int       card  = nPR >> 4;
  // int       node  = nPR & 0xF;
  // int       index = card * ROW + node;

  int       nPR_card  = nPR >> 4;
  int       nPR_node  = nPR & 0xF;
  int       nPR_index = nPR_card * ROW + nPR_node;

  int       in1_card  = in1 >> 4;
  int       in1_node  = in1 & 0xF;
  int       in1_index = in1_card * ROW + in1_node;

  int       in2_card  = in2 >> 4;
  int       in2_node  = in2 & 0xF;
  int       in2_index = in2_card * ROW + in2_node;

  int       out_card  = out >> 4;
  int       out_node  = out & 0xF;
  int       out_index = out_card * ROW + out_node;
  // ===========================================================================

  VM->VAM_TABLE->at(nPR_index).node_type = Reg_Reg_Reg;

  // request Mutex for CMD Stream
  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread request mutex for cmd_stream\r\n");
  #endif
  pthread_mutex_lock(&VM->vm_mutex);

  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread get mutex...\r\n");
  #endif
  cmd_stream = VM->pico[nPR_card]->CreateStream(50);

  // cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 >> 16)                 ;
  // cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 & 0x0000FFFF)          ;
  // cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | (size_out >> 16       ) ;
  // cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | (size_out & 0x0000FFFF) ;
  cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | (size_in1 >> 16       ) ;
  cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | (size_in1 & 0x0000FFFF) ;
  // cmd[2] = 0xC0300001 | (nPR_node + 1 << 24)                                ;
  cmd[2] = VM->VAM_TABLE->at(nPR_index).A3_cmd;

  if (nPR_card == in1_card && nPR_card == in2_card && nPR_card == out_card) {
    #ifdef VERBOSE
      printf("[DEBUG->vtieio] RRR, nPR:%3d, Card:%3d, Node:%3d, index:%3d\r\n", nPR, nPR_card, nPR_node, nPR_index);
      printf("[DEBUG->vtieio] RRR, in1:%3d, Card:%3d, Node:%3d, index:%3d\r\n", in1, in1_card, in1_node, in1_index);
      printf("[DEBUG->vtieio] RRR, in2:%3d, Card:%3d, Node:%3d, index:%3d\r\n", in2, in2_card, in2_node, in2_index);
      printf("[DEBUG->vtieio] RRR, out:%3d, Card:%3d, Node:%3d, index:%3d\r\n", out, out_card, out_node, out_index);
    #endif

    cmd[3] = 0xB0000222 | (nPR_node + 1 << 24) | (in1_node + 1 << 20) | (in2_node + 1 << 16) ;
  }
  //------------------------------------------------------------------------------------------------
  //  ,------. ,------. ,------. ,--.
  //  |  .--. '|  .--. '|  .--. '|  |
  //  |  '--'.'|  '--'.'|  '--'.'`-'
  //  |  |\  \ |  |\  \ |  |\  \
  //  `--' '--'`--' '--'`--' '--'
  //------------------------------------------------------------------------------------------------
  if (nPR_card == in1_card && nPR_card == in2_card && nPR_card != out_card) {
    #ifdef VERBOSE
      printf("[DEBUG->vtieio] RRR', nPR:%3d, Card:%3d, Node:%3d, index:%3d\r\n", nPR, nPR_card, nPR_node, nPR_index);
      printf("[DEBUG->vtieio] RRR', in1:%3d, Card:%3d, Node:%3d, index:%3d\r\n", in1, in1_card, in1_node, in1_index);
      printf("[DEBUG->vtieio] RRR', in2:%3d, Card:%3d, Node:%3d, index:%3d\r\n", in2, in2_card, in2_node, in2_index);
      printf("[DEBUG->vtieio] RRR', out:%3d, Card:%3d, Node:%3d, index:%3d\r\n", out, out_card, out_node, out_index);
    #endif

    cmd[3] = 0xB0000223 | (nPR_node + 1 << 24) | (in1_node + 1 << 20) | (in2_node + 1 << 16) ;
  }

  VM->VAM_TABLE->at(nPR_index).cur_cmd = cmd[3];

  VM->VAM_TABLE->at(nPR_index).in1 = NULL;
  VM->VAM_TABLE->at(nPR_index).in2 = NULL;
  VM->VAM_TABLE->at(nPR_index).out = NULL;

  VM->VAM_TABLE->at(nPR_index).tie_in1 = in1;
  VM->VAM_TABLE->at(nPR_index).tie_in2 = in2;
  VM->VAM_TABLE->at(nPR_index).tie_out = out;

  VM->VAM_TABLE->at(nPR_index).size_in1 = size_in1;
  VM->VAM_TABLE->at(nPR_index).size_in2 = size_in2;
  VM->VAM_TABLE->at(nPR_index).size_out = size_out;

  #ifdef VERBOSE
    printf("[DEBUG->vtieio]  CMD: 0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", cmd[0], cmd[1], cmd[2], cmd[3]);
  #endif

  err = VM->pico[nPR_card]->WriteStream(cmd_stream, cmd, 16);
  if (err < 0) {
    fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
    return -1;
  }

  VM->pico[nPR_card]->CloseStream(cmd_stream);
  // Releast Mutex on CMD Stream
  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread release mutex...\r\n");
  #endif
  pthread_mutex_unlock(&VM->vm_mutex);
  return 0;
}
//--------------------------------------------------------------------------------------------------
//  ,-----.  ,------. ,-----.
//  |  |) /_ |  .--. '|  |) /_
//  |  .-.  \|  '--'.'|  .-.  \
//  |  '--' /|  |\  \ |  '--' /
//  `------' `--' '--'`------'
//--------------------------------------------------------------------------------------------------
int vtieio(vam_vm_t *VM, int nPR, int *in1, int size_in1, int in2, int size_in2, int *out, int size_out)
{
#ifdef VERBOSE
  printf("\r\n");
#endif

  uint32_t  cmd[4] = {0, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
  char      ibuf[1024];
  int       err;
  int       cmd_stream;

  int       nPR_card  = nPR >> 4;
  int       nPR_node  = nPR & 0xF;
  int       nPR_index = nPR_card * ROW + nPR_node;

  // int       in1_card  = in1 >> 4;
  // int       in1_node  = in1 & 0xF;
  // int       in1_index = in1_card * ROW + in1_node;

  int       in2_card  = in2 >> 4;
  int       in2_node  = in2 & 0xF;
  int       in2_index = in2_card * ROW + in2_node;

  // int       out_card  = out >> 4;
  // int       out_node  = out & 0xF;
  // int       out_index = out_card * ROW + out_node;

  // ===========================================================================

  VM->VAM_TABLE->at(nPR_index).node_type = Buf_Reg_Buf;

  // request Mutex for CMD Stream
  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread request mutex for cmd_stream\r\n");
  #endif
  pthread_mutex_lock(&VM->vm_mutex);

  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread get mutex...\r\n");
  #endif
  cmd_stream = VM->pico[nPR_card]->CreateStream(50);


  cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 >> 16)                 ;
  cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 & 0x0000FFFF)          ;
  cmd[2] = 0xC0300001 | (nPR_node + 1 << 24)                                ;

  if (nPR_card == in2_card) {
    #ifdef VERBOSE
      printf("[DEBUG->vtieio] BRB, nPR:%3d, Card:%3d, Node:%3d, index:%3d\r\n", nPR, nPR_card, nPR_node, nPR_index);
      printf("[DEBUG->vtieio] BRB, in2:%3d, Card:%3d, Node:%3d, index:%3d\r\n", in2, in2_card, in2_node, in2_index);
    #endif

    cmd[3] = 0xB0000121 | (nPR_node + 1 << 24) | (in2_node + 1 << 16)          ;
  } else {
    #ifdef VERBOSE
      printf("[DEBUG->vtieio] BR'B, nPR:%3d, Card:%3d, Node:%3d, index:%3d\r\n", nPR, nPR_card, nPR_node, nPR_index);
      printf("[DEBUG->vtieio] BR'B, in2:%3d, Card:%3d, Node:%3d, index:%3d\r\n", in2, in2_card, in2_node, in2_index);
    #endif

    cmd[3] = 0xB0000131 | (nPR_node + 1 << 24) | (in2_node + 1 << 16)          ;

    //----------------------------------------------------------------------------------------------
    //  ,-----.  ,------. ,--.,-----.
    //  |  |) /_ |  .--. '|  ||  |) /_
    //  |  .-.  \|  '--'.'`-' |  .-.  \
    //  |  '--' /|  |\  \     |  '--' /
    //  `------' `--' '--'    `------'
    //----------------------------------------------------------------------------------------------
    int src = (in2_node + 1) * 10 + 6;
    int des = (nPR_node + 1) * 10 + 5;

    if (VM->TieStream_Table[in2_index][0] != des) {
      #ifdef VERBOSE
        printf("[DEBUG->vtieio] New TieSTream: %d -> %d\r\n", src, des);
      #endif
      err = VM->pico[in2_card]->TieStreams(src, VM->pico[nPR_card], des);
      if (err < 0) {
          fprintf(stderr, "TieStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
          exit(1);
      }
      VM->TieStream_Table[in2_index][0] = des;
    } else {
      #ifdef VERBOSE
        printf("[DEBUG->vtieio] Original TieSTream: %d -> %d\r\n", src, des);
      #endif
    }

    // #ifdef VERBOSE
    //   printf("[DEBUG->vtieio] TieSTream: %d -> %d\r\n", (in2_node + 1) * 10 + 6, (nPR_node + 1) * 10 + 5);
    // #endif
    // err = VM->pico[in2_card]->TieStreams((in2_node + 1) * 10 + 6, VM->pico[nPR_card], (nPR_node + 1) * 10 + 5);
    // if (err < 0) {
    //     fprintf(stderr, "TieStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
    //     exit(1);
    // }
  }

  VM->VAM_TABLE->at(nPR_index).cur_cmd = cmd[3];
  VM->VAM_TABLE->at(nPR_index).in1     = in1;
  VM->VAM_TABLE->at(nPR_index).in2     = NULL;
  VM->VAM_TABLE->at(nPR_index).tie_in2 = in2;
  VM->VAM_TABLE->at(nPR_index).out     = out;

  VM->VAM_TABLE->at(nPR_index).size_in1 = size_in1;
  VM->VAM_TABLE->at(nPR_index).size_in2 = size_in2;
  VM->VAM_TABLE->at(nPR_index).size_out = size_out;

  #ifdef VERBOSE
    printf("[DEBUG->vtieio] CMD: 0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", cmd[0], cmd[1], cmd[2], cmd[3]);
  #endif
  err = VM->pico[nPR_card]->WriteStream(cmd_stream, cmd, 16);
  if (err < 0) {
    fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
    return -1;
  }

  VM->pico[nPR_card]->CloseStream(cmd_stream);
  // Releast Mutex on CMD Stream
  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread release mutex...\r\n");
  #endif
  pthread_mutex_unlock(&VM->vm_mutex);
  return 0;
}
//--------------------------------------------------------------------------------------------------
//  ,------. ,-----.  ,-----.
//  |  .--. '|  |) /_ |  |) /_
//  |  '--'.'|  .-.  \|  .-.  \
//  |  |\  \ |  '--' /|  '--' /
//  `--' '--'`------' `------'
//--------------------------------------------------------------------------------------------------
int vtieio(vam_vm_t *VM, int nPR, int in1, int size_in1, int *in2, int size_in2, int *out, int size_out)
{
#ifdef VERBOSE
  printf("\r\n");
#endif
  int       src, des;
  uint32_t  cmd[4] = {0, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
  char      ibuf[1024];
  int       err;
  int       cmd_stream;

  int       nPR_card  = nPR >> 4;
  int       nPR_node  = nPR & 0xF;
  int       nPR_index = nPR_card * ROW + nPR_node;

  int       in1_card  = in1 >> 4;
  int       in1_node  = in1 & 0xF;
  int       in1_index = in1_card * ROW + in1_node;

  // int       in2_card  = in2 >> 4;
  // int       in2_node  = in2 & 0xF;
  // int       in2_index = in2_card * ROW + in2_node;

  // int       out_card  = out >> 4;
  // int       out_node  = out & 0xF;
  // int       out_index = out_card * ROW + out_node;

  // ===========================================================================
  VM->VAM_TABLE->at(nPR_index).node_type = Reg_Buf_Buf;

  // request Mutex for CMD Stream
  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread request mutex for cmd_stream\r\n");
  #endif
  pthread_mutex_lock(&VM->vm_mutex);

  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread get mutex...\r\n");
  #endif
  cmd_stream = VM->pico[nPR_card]->CreateStream(50);

  cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | (size_out >> 16)               ;
  cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | (size_out & 0x0000FFFF)        ;
  cmd[2] = 0xC0300001 | (nPR_node + 1 << 24)                                  ;

  if (nPR_card == in1_card) {
    #ifdef VERBOSE
      printf("[DEBUG->vtieio] RBB, nPR:%3d, Card:%3d, Node:%3d, index:%3d\r\n", nPR, nPR_card, nPR_node, nPR_index);
      printf("[DEBUG->vtieio] RBB, in1:%3d, Card:%3d, Node:%3d, index:%3d\r\n", in1, in1_card, in1_node, in1_index);
    #endif

    cmd[3] = 0xB0000211 | (nPR_node + 1 << 24) | (in1_node + 1 << 20)           ;

  } else {
    #ifdef VERBOSE
      printf("[DEBUG->vtieio] R'BB, nPR:%3d, Card:%3d, Node:%3d, index:%3d\r\n", nPR, nPR_card, nPR_node, nPR_index);
      printf("[DEBUG->vtieio] R'BB, in1:%3d, Card:%3d, Node:%3d, index:%3d\r\n", in1, in1_card, in1_node, in1_index);
    #endif

    cmd[3] = 0xB0000311 | (nPR_node + 1 << 24) | (in1_node + 1 << 20)     ;

    //----------------------------------------------------------------------------------------------
    //  ,------. ,--.,-----.  ,-----.
    //  |  .--. '|  ||  |) /_ |  |) /_
    //  |  '--'.'`-' |  .-.  \|  .-.  \
    //  |  |\  \     |  '--' /|  '--' /
    //  `--' '--'    `------' `------'
    //----------------------------------------------------------------------------------------------
    src = (in1_node + 1) * 10 + 6;
    des = (nPR_node + 1) * 10 + 4;

    if (VM->TieStream_Table[in1_index][0] != des) {
      #ifdef VERBOSE
        printf("[DEBUG->vtieio] New TieSTream: %d -> %d\r\n", src, des);
      #endif
      err = VM->pico[in1_card]->TieStreams(src, VM->pico[nPR_card], des);
      if (err < 0) {
          fprintf(stderr, "TieStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
          exit(1);
      }
      VM->TieStream_Table[in1_index][0] = des;
    } else {
      #ifdef VERBOSE
        printf("[DEBUG->vtieio] Original TieSTream: %d -> %d\r\n", src, des);
      #endif
    }

  }

  VM->VAM_TABLE->at(nPR_index).cur_cmd = cmd[3];
  VM->VAM_TABLE->at(nPR_index).in1     = NULL;
  VM->VAM_TABLE->at(nPR_index).tie_in1 = in1;
  VM->VAM_TABLE->at(nPR_index).in2     = in2;
  VM->VAM_TABLE->at(nPR_index).out     = out;

  VM->VAM_TABLE->at(nPR_index).size_in1 = size_in1;
  VM->VAM_TABLE->at(nPR_index).size_in2 = size_in2;
  VM->VAM_TABLE->at(nPR_index).size_out = size_out;

  #ifdef VERBOSE
    printf("[DEBUG->vtieio]  CMD: 0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", cmd[0], cmd[1], cmd[2], cmd[3]);
  #endif
  err = VM->pico[nPR_card]->WriteStream(cmd_stream, cmd, 16);
  if (err < 0) {
    fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
    return -1;
  }

  VM->pico[nPR_card]->CloseStream(cmd_stream);
  // Releast Mutex on CMD Stream
  #ifdef VERBOSE_CMD
    printf("[DEBUG->vtieio] vstart thread release mutex...\r\n");
  #endif
  pthread_mutex_unlock(&VM->vm_mutex);
  return 0;
}
//--------------------------------------------------------------------------------------------------
//  ,-----.  ,------. ,------.
//  |  |) /_ |  .--. '|  .--. '
//  |  .-.  \|  '--'.'|  '--'.'
//  |  '--' /|  |\  \ |  |\  \
//  `------' `--' '--'`--' '--'
//--------------------------------------------------------------------------------------------------
int vtieio(vam_vm_t *VM, int nPR, int *in1, int size_in1, int in2, int size_in2, int out, int size_out)
{
#ifdef VERBOSE
  printf("\r\n");
#endif

  uint32_t  cmd[4] = {0, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
  char      ibuf[1024];
  int       err;
  int       cmd_stream;

  int       nPR_card  = nPR >> 4;
  int       nPR_node  = nPR & 0xF;
  int       nPR_index = nPR_card * ROW + nPR_node;

  // int       in1_card  = in1 >> 4;
  // int       in1_node  = in1 & 0xF;
  // int       in1_index = in1_card * ROW + in1_node;

  int       in2_card  = in2 >> 4;
  int       in2_node  = in2 & 0xF;
  int       in2_index = in2_card * ROW + in2_node;

  int       out_card  = out >> 4;
  int       out_node  = out & 0xF;
  int       out_index = out_card * ROW + out_node;

#ifdef VERBOSE
  printf("[DEBUG->vtieio] Buf_Reg_Reg\r\n");
  printf("[DEBUG->vtieio] nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR, nPR_card, nPR_node, nPR_index);
  // printf("[DEBUG->vtieio] in1:0x%08x, Card:%d, Node:%d, index:%d\r\n", in1, in1_card, in1_node, in1_index);
  printf("[DEBUG->vtieio] in1:0x%08x, Card:%d, Node:%d, index:%d\r\n", in2, in2_card, in2_node, in2_index);
  printf("[DEBUG->vtieio] out:0x%08x, Card:%d, Node:%d, index:%d\r\n", out, out_card, out_node, out_index);
#endif

  VM->VAM_TABLE->at(nPR_index).node_type = Buf_Reg_Reg;

  // request Mutex for CMD Stream
  #ifdef VERBOSE
    printf("[DEBUG->vtieio] vstart thread request mutex for cmd_stream\r\n");
  #endif
  pthread_mutex_lock(&VM->vm_mutex);

  #ifdef VERBOSE
    printf("[DEBUG->vtieio] vstart thread get mutex...\r\n");
  #endif
  cmd_stream = VM->pico[nPR_card]->CreateStream(50);

  cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 >> 16)                 ;
  cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 & 0x0000FFFF)          ;
  cmd[2] = 0xC0300001 | (nPR_node + 1 << 24)                                ;
  cmd[3] = 0xB0000122 | (nPR_node + 1 << 24) | (in2_node + 1 << 16)         ;

  VM->VAM_TABLE->at(nPR_index).cur_cmd = cmd[3];
  VM->VAM_TABLE->at(nPR_index).in1     = in1;
  VM->VAM_TABLE->at(nPR_index).tie_in2 = in2;
  VM->VAM_TABLE->at(nPR_index).tie_out = out;

  VM->VAM_TABLE->at(nPR_index).size_in1 = size_in1;
  VM->VAM_TABLE->at(nPR_index).size_in2 = size_in2;
  VM->VAM_TABLE->at(nPR_index).size_out = size_out;

  #ifdef VERBOSE
    printf("[DEBUG->vtieio] Sending command to nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR, nPR_card, nPR_node, nPR_index);
    printf("[DEBUG->vtieio] Sending command : %d, 0x%x, 0x%x\r\n", (size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1, (size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 >> 16, (size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 & 0x0000FFFF);
    printf("[DEBUG->vtieio] Sending command :0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", cmd[0], cmd[1], cmd[2], cmd[3]);
  #endif
  err = VM->pico[nPR_card]->WriteStream(cmd_stream, cmd, 16);
  if (err < 0) {
    fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
    return -1;
  }

  VM->pico[nPR_card]->CloseStream(cmd_stream);
  // Releast Mutex on CMD Stream
  #ifdef VERBOSE
    printf("[DEBUG->vtieio] vstart thread release mutex...\r\n");
  #endif
  pthread_mutex_unlock(&VM->vm_mutex);

  return 0;
}
//--------------------------------------------------------------------------------------------------
//  ,------. ,-----.  ,------.
//  |  .--. '|  |) /_ |  .--. '
//  |  '--'.'|  .-.  \|  '--'.'
//  |  |\  \ |  '--' /|  |\  \
//  `--' '--'`------' `--' '--'
//--------------------------------------------------------------------------------------------------
int vtieio(vam_vm_t *VM, int nPR, int in1, int size_in1, int *in2, int size_in2, int out, int size_out)
{
#ifdef VERBOSE
  printf("\r\n");
#endif

  uint32_t  cmd[4] = {0, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
  char      ibuf[1024];
  int       err;
  int       cmd_stream;

  int       nPR_card  = nPR >> 4;
  int       nPR_node  = nPR & 0xF;
  int       nPR_index = nPR_card * ROW + nPR_node;

  int       in1_card  = in1 >> 4;
  int       in1_node  = in1 & 0xF;
  int       in1_index = in1_card * ROW + in1_node;

  // int       in2_card  = in2 >> 4;
  // int       in2_node  = in2 & 0xF;
  // int       in2_index = in2_card * ROW + in2_node;

  int       out_card  = out >> 4;
  int       out_node  = out & 0xF;
  int       out_index = out_card * ROW + out_node;

#ifdef VERBOSE
  printf("[DEBUG->vtieio] Reg_Buf_Reg\r\n");
  printf("[DEBUG->vtieio] nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR, nPR_card, nPR_node, nPR_index);
  printf("[DEBUG->vtieio] in1:0x%08x, Card:%d, Node:%d, index:%d\r\n", in1, in1_card, in1_node, in1_index);
  // printf("[DEBUG->vtieio] in1:0x%08x, Card:%d, Node:%d, index:%d\r\n", in2, in2_card, in2_node, in2_index);
  printf("[DEBUG->vtieio] out:0x%08x, Card:%d, Node:%d, index:%d\r\n", out, out_card, out_node, out_index);
#endif

  VM->VAM_TABLE->at(nPR_index).node_type = Reg_Buf_Reg;

  // request Mutex for CMD Stream
  #ifdef VERBOSE
    printf("[DEBUG->vtieio] vstart thread request mutex for cmd_stream\r\n");
  #endif
  pthread_mutex_lock(&VM->vm_mutex);

  #ifdef VERBOSE
    printf("[DEBUG->vtieio] vstart thread get mutex...\r\n");
  #endif
  cmd_stream = VM->pico[nPR_card]->CreateStream(50);

  cmd[0] = 0xC0100000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 >> 16)                 ;
  cmd[1] = 0xC0200000 | (nPR_node + 1 << 24) | ((size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 & 0x0000FFFF)          ;
  cmd[2] = 0xC0300001 | (nPR_node + 1 << 24)                                ;
  cmd[3] = 0xB0000212 | (nPR_node + 1 << 24) | (in1_node + 1 << 20)         ;

  VM->VAM_TABLE->at(nPR_index).cur_cmd = cmd[3];
  VM->VAM_TABLE->at(nPR_index).tie_in1 = in1;
  VM->VAM_TABLE->at(nPR_index).in2     = in2;
  VM->VAM_TABLE->at(nPR_index).tie_out = out;

  VM->VAM_TABLE->at(nPR_index).size_in1 = size_in1;
  VM->VAM_TABLE->at(nPR_index).size_in2 = size_in2;
  VM->VAM_TABLE->at(nPR_index).size_out = size_out;

  #ifdef VERBOSE
    printf("[DEBUG->vtieio] Sending command to nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR, nPR_card, nPR_node, nPR_index);
    printf("[DEBUG->vtieio] Sending command : %d, 0x%x, 0x%x\r\n", (size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1, (size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 >> 16, (size_in1 == 0) ? ( (size_in2 == 0) ? size_out : size_in2 ) : size_in1 & 0x0000FFFF);
    printf("[DEBUG->vtieio] Sending command :0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", cmd[0], cmd[1], cmd[2], cmd[3]);
  #endif
  err = VM->pico[nPR_card]->WriteStream(cmd_stream, cmd, 16);
  if (err < 0) {
    fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
    return -1;
  }

  VM->pico[nPR_card]->CloseStream(cmd_stream);
  // Releast Mutex on CMD Stream
  #ifdef VERBOSE
    printf("[DEBUG->vtieio] vstart thread release mutex...\r\n");
  #endif
  pthread_mutex_unlock(&VM->vm_mutex);

  return 0;
}
//==================================================================================================
//  ____    ____   _______.___________.    ___      .______     .___________.
//  \   \  /   /  /       |           |   /   \     |   _  \    |           |
//   \   \/   /  |   (----`---|  |----`  /  ^  \    |  |_)  |   `---|  |----`
//    \      /    \   \       |  |      /  /_\  \   |      /        |  |
//     \    / .----)   |      |  |     /  _____  \  |  |\  \----.   |  |
//      \__/  |_______/       |__|    /__/     \__\ | _| `._____|   |__|
//==================================================================================================
int vstart(vam_vm_t *VM, vector<int> *nPR)//, int size_in1, int size_in2, int size_out)
{
#ifdef VERBOSE
  printf("\r\n");
#endif
  struct timeval start, end;
  int timeuse1;
  int timeuse2;
  uint32_t  cmd[4] = {0, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
  char      ibuf[1024];
  int       err;
  int       cmd_stream;
  int       card  ;
  int       node  ;
  int       index ;
  int       i     ;
  int       size  ;
  int      *tmp = NULL;

  // pthread_t threads[VSTART_THREADS];
  // int       threadsRet[VSTART_THREADS];

  pthread_t   **threads    = NULL;
  int         **threadsRet = NULL;
  vstart_pk_t **package    = NULL;

  size = nPR->size();
  threads = new pthread_t* [size];    // size nodes
  for (i = 0; i < size; i++) {
    threads[i] = new pthread_t[3];    // 3 threads for each node
  }

  threadsRet = new int* [size]; // size nodes
  for (i = 0; i < size; i++) {
    threadsRet[i] = new int[3]; // 3 threads for each node
  }

  package = new vstart_pk_t* [size]; // Package for two write and one read
  for (i = 0; i < size; i++) {
    package[i] = new vstart_pk_t[3]; // 3 packages for each node
  }

  // gettimeofday(&start, NULL);
  for (i = size -1; i > -1; i--) {
    card  = nPR->at(i) >> 4;
    node  = nPR->at(i) & 0xF;
    index = card * ROW + node;


    // // request Mutex for CMD Stream
    // #ifdef VERBOSE
    //   printf("[DEBUG->vstart_TCALL] vstart thread request mutex for cmd_stream\r\n");
    // #endif
    // pthread_mutex_lock(&VM->vm_mutex);

    // #ifdef VERBOSE
    //   printf("[DEBUG->vstart_TCALL] vstart thread get mutex...\r\n");
    // #endif
    // cmd_stream = VM->pico[card]->CreateStream(50);
    // cmd[0] = 0xA0000000 | (node + 1 << 24) ; // Go Command to Acc
    // err = VM->pico[card]->WriteStream(cmd_stream, cmd, 16);
    // if (err < 0) {
    //   fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
    //   return -1;
    // }
    // #ifdef VERBOSE
    //   printf("[DEBUG->vtieio] Opening CMD Stream and send cmd: 0x%08x\r\n", cmd[0]);
    // #endif
    // VM->pico[card]->CloseStream(cmd_stream);
    // // Releast Mutex on CMD Stream
    // #ifdef VERBOSE
    //   printf("[DEBUG->vstart_TCALL] vstart thread release mutex...\r\n");
    // #endif
    // pthread_mutex_unlock(&VM->vm_mutex);

    // printf("node_type:%d\r\n", VM->VAM_TABLE->at(index).node_type);
    switch (VM->VAM_TABLE->at(index).node_type) {
      //------------------------------------------------------------------------
      //  ,-----.  ,-----.  ,-----.
      //  |  |) /_ |  |) /_ |  |) /_
      //  |  .-.  \|  .-.  \|  .-.  \
      //  |  '--' /|  '--' /|  '--' /
      //  `------' `------' `------'
      //------------------------------------------------------------------------
      case Buf_Buf_Buf: {
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Buf_Buf_Buf, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
        #endif

        package[i][WRITE_IN1].type   = WS;
        package[i][WRITE_IN1].VM     = VM;
        package[i][WRITE_IN1].card   = card;
        package[i][WRITE_IN1].index  = index;
        package[i][WRITE_IN1].stream = VM->VAM_TABLE->at(index).streamInA;
        package[i][WRITE_IN1].buf    = VM->VAM_TABLE->at(index).in1;
        package[i][WRITE_IN1].size   = VM->VAM_TABLE->at(index).size_in1;

        package[i][WRITE_IN2].type   = WS;
        package[i][WRITE_IN2].VM     = VM;
        package[i][WRITE_IN2].card   = card;
        package[i][WRITE_IN2].index  = index;
        package[i][WRITE_IN2].stream = VM->VAM_TABLE->at(index).streamInB;
        package[i][WRITE_IN2].buf    = VM->VAM_TABLE->at(index).in2;
        package[i][WRITE_IN2].size   = VM->VAM_TABLE->at(index).size_in2;

        package[i][READ_OUT].type   = RS;
        package[i][READ_OUT].VM     = VM;
        package[i][READ_OUT].card   = card;
        package[i][READ_OUT].index  = index;
        package[i][READ_OUT].stream = VM->VAM_TABLE->at(index).streamOut;
        package[i][READ_OUT].buf    = VM->VAM_TABLE->at(index).out;
        package[i][READ_OUT].size   = VM->VAM_TABLE->at(index).size_out;

        #ifdef VERBOSE
          printf("[DEBUG->vstart create] Buf_Buf_Buf, Thread Read Out created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
        threadsRet[i][READ_OUT]  = pthread_create(&threads[i][READ_OUT], NULL, Stream_Threads_Call, (void*) &package[i][READ_OUT]);

        if (package[i][WRITE_IN1].buf != NULL) {
        #ifdef VERBOSE
          printf("[DEBUG->vstart create] Buf_Buf_Buf, Thread Write IN1 created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
          threadsRet[i][WRITE_IN1] = pthread_create(&threads[i][WRITE_IN1], NULL, Stream_Threads_Call, (void*) &package[i][WRITE_IN1]);
        }

        if (package[i][WRITE_IN2].buf != NULL) {
        #ifdef VERBOSE
          printf("[DEBUG->vstart create] Buf_Buf_Buf, Thread Write IN2 created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
          threadsRet[i][WRITE_IN2] = pthread_create(&threads[i][WRITE_IN2], NULL, Stream_Threads_Call, (void*) &package[i][WRITE_IN2]);
        }

        // if (package[i][WRITE_IN1].buf  != NULL)
        //   pthread_join(threads[i][WRITE_IN1], NULL);
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart join] Buf_Buf_Buf, Thread Write IN1 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif

        // if (package[i][WRITE_IN2].buf != NULL)
        //   pthread_join(threads[i][WRITE_IN2], NULL);
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart join] Buf_Buf_Buf, Thread Write IN2 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif

        // pthread_join(threads[i][READ_OUT],  NULL);
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart join] Buf_Buf_Buf, Thread Read  OUT Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif

        // if (package[WRITE_IN1].buf != NULL)
        //   pthread_join(threads[i][WRITE_IN1], NULL);
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart] Buf_Buf_Buf, Thread Write IN1 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif

        // if (package[WRITE_IN2].buf != NULL)
        //   pthread_join(threads[i][WRITE_IN2], NULL);
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart] Buf_Buf_Buf, Thread Write IN2 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif

        // pthread_join(threads[i][READ_OUT],  NULL);
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart] Buf_Buf_Buf, Thread Write OUT Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif

      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,-----.  ,-----.  ,------.
      //  |  |) /_ |  |) /_ |  .--. '
      //  |  .-.  \|  .-.  \|  '--'.'
      //  |  '--' /|  '--' /|  |\  \
      //  `------' `------' `--' '--'
      //--------------------------------------------------------------------------------------------------
      case Buf_Buf_Reg: {
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Buf_Buf_Reg, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
        #endif

        package[i][WRITE_IN1].type   = WS;
        package[i][WRITE_IN1].VM     = VM;
        package[i][WRITE_IN1].card   = card;
        package[i][WRITE_IN1].index  = index;
        package[i][WRITE_IN1].stream = VM->VAM_TABLE->at(index).streamInA;
        package[i][WRITE_IN1].buf    = VM->VAM_TABLE->at(index).in1;
        package[i][WRITE_IN1].size   = VM->VAM_TABLE->at(index).size_in1;

        package[i][WRITE_IN2].type   = WS;
        package[i][WRITE_IN2].VM     = VM;
        package[i][WRITE_IN2].card   = card;
        package[i][WRITE_IN2].index  = index;
        package[i][WRITE_IN2].stream = VM->VAM_TABLE->at(index).streamInB;
        package[i][WRITE_IN2].buf    = VM->VAM_TABLE->at(index).in2;
        package[i][WRITE_IN2].size   = VM->VAM_TABLE->at(index).size_in2;

        if (package[i][WRITE_IN1].buf != NULL) {
        #ifdef VERBOSE
          printf("[DEBUG->vstart create] Buf_Buf_Reg, Thread Write IN1 created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
          threadsRet[i][WRITE_IN1] = pthread_create(&threads[i][WRITE_IN1], NULL, Stream_Threads_Call, (void*) &package[i][WRITE_IN1]);
        }

        if (package[i][WRITE_IN2].buf != NULL) {
        #ifdef VERBOSE
          printf("[DEBUG->vstart create] Buf_Buf_Reg, Thread Write IN2 created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
          threadsRet[i][WRITE_IN2] = pthread_create(&threads[i][WRITE_IN2], NULL, Stream_Threads_Call, (void*) &package[i][WRITE_IN2]);
        }
      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,------. ,------. ,-----.
      //  |  .--. '|  .--. '|  |) /_
      //  |  '--'.'|  '--'.'|  .-.  \
      //  |  |\  \ |  |\  \ |  '--' /
      //  `--' '--'`--' '--'`------'
      //--------------------------------------------------------------------------------------------------
      case Reg_Reg_Buf: {
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Reg_Reg_Buf, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
        #endif

        package[i][READ_OUT].type   = RS;
        package[i][READ_OUT].VM     = VM;
        package[i][READ_OUT].card   = card;
        package[i][READ_OUT].index  = index;
        package[i][READ_OUT].stream = VM->VAM_TABLE->at(index).streamOut;
        package[i][READ_OUT].buf    = VM->VAM_TABLE->at(index).out;
        package[i][READ_OUT].size   = VM->VAM_TABLE->at(index).size_out;

        #ifdef VERBOSE
          printf("[DEBUG->vstart create] Reg_Reg_Buf, Thread Read Out created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
        threadsRet[i][READ_OUT]  = pthread_create(&threads[i][READ_OUT], NULL, Stream_Threads_Call, (void*) &package[i][READ_OUT]);

      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,------. ,------. ,------.
      //  |  .--. '|  .--. '|  .--. '
      //  |  '--'.'|  '--'.'|  '--'.'
      //  |  |\  \ |  |\  \ |  |\  \
      //  `--' '--'`--' '--'`--' '--'
      //--------------------------------------------------------------------------------------------------
      case Reg_Reg_Reg: {
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Reg_Reg_Reg, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
        #endif
      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,-----.  ,------. ,-----.
      //  |  |) /_ |  .--. '|  |) /_
      //  |  .-.  \|  '--'.'|  .-.  \
      //  |  '--' /|  |\  \ |  '--' /
      //  `------' `--' '--'`------'
      //--------------------------------------------------------------------------------------------------
      case Buf_Reg_Buf: {
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Buf_Reg_Buf, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
        #endif

        package[i][WRITE_IN1].type   = WS;
        package[i][WRITE_IN1].VM     = VM;
        package[i][WRITE_IN1].card   = card;
        package[i][WRITE_IN1].index  = index;
        package[i][WRITE_IN1].stream = VM->VAM_TABLE->at(index).streamInA;
        package[i][WRITE_IN1].buf    = VM->VAM_TABLE->at(index).in1;
        package[i][WRITE_IN1].size   = VM->VAM_TABLE->at(index).size_in1;

        // package[i][WRITE_IN2].type   = WS;
        // package[i][WRITE_IN2].VM     = VM;
        // package[i][WRITE_IN2].card   = card;
        // package[i][WRITE_IN2].index  = index;
        // package[i][WRITE_IN2].stream = VM->VAM_TABLE->at(index).streamInB;
        // package[i][WRITE_IN2].buf    = VM->VAM_TABLE->at(index).in2;
        // package[i][WRITE_IN2].size   = VM->VAM_TABLE->at(index).size_in2;

        package[i][READ_OUT].type   = RS;
        package[i][READ_OUT].VM     = VM;
        package[i][READ_OUT].card   = card;
        package[i][READ_OUT].index  = index;
        package[i][READ_OUT].stream = VM->VAM_TABLE->at(index).streamOut;
        package[i][READ_OUT].buf    = VM->VAM_TABLE->at(index).out;
        package[i][READ_OUT].size   = VM->VAM_TABLE->at(index).size_out;

        #ifdef VERBOSE
          printf("[DEBUG->vstart create] Buf_Reg_Buf, Thread Read Out created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
        threadsRet[i][READ_OUT]  = pthread_create(&threads[i][READ_OUT], NULL, Stream_Threads_Call, (void*) &package[i][READ_OUT]);

        if (package[i][WRITE_IN1].buf != NULL) {
        #ifdef VERBOSE
          printf("[DEBUG->vstart create] Buf_Reg_Buf, Thread Write IN1 created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
          threadsRet[i][WRITE_IN1] = pthread_create(&threads[i][WRITE_IN1], NULL, Stream_Threads_Call, (void*) &package[i][WRITE_IN1]);
        }

        // if (package[i][WRITE_IN2].buf != NULL) {
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart create] Buf_Reg_Buf, Thread Write IN2 created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif
        //   threadsRet[i][WRITE_IN2] = pthread_create(&threads[i][WRITE_IN2], NULL, Stream_Threads_Call, (void*) &package[i][WRITE_IN2]);
        // }
      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,------. ,-----.  ,-----.
      //  |  .--. '|  |) /_ |  |) /_
      //  |  '--'.'|  .-.  \|  .-.  \
      //  |  |\  \ |  '--' /|  '--' /
      //  `--' '--'`------' `------'
      //--------------------------------------------------------------------------------------------------
      case Reg_Buf_Buf: {
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Reg_Buf_Buf, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
        #endif

        // package[i][WRITE_IN1].type   = WS;
        // package[i][WRITE_IN1].VM     = VM;
        // package[i][WRITE_IN1].card   = card;
        // package[i][WRITE_IN1].index  = index;
        // package[i][WRITE_IN1].stream = VM->VAM_TABLE->at(index).streamInA;
        // package[i][WRITE_IN1].buf    = VM->VAM_TABLE->at(index).in1;
        // package[i][WRITE_IN1].size   = VM->VAM_TABLE->at(index).size_in1;

        package[i][WRITE_IN2].type   = WS;
        package[i][WRITE_IN2].VM     = VM;
        package[i][WRITE_IN2].card   = card;
        package[i][WRITE_IN2].index  = index;
        package[i][WRITE_IN2].stream = VM->VAM_TABLE->at(index).streamInB;
        package[i][WRITE_IN2].buf    = VM->VAM_TABLE->at(index).in2;
        package[i][WRITE_IN2].size   = VM->VAM_TABLE->at(index).size_in2;

        package[i][READ_OUT].type   = RS;
        package[i][READ_OUT].VM     = VM;
        package[i][READ_OUT].card   = card;
        package[i][READ_OUT].index  = index;
        package[i][READ_OUT].stream = VM->VAM_TABLE->at(index).streamOut;
        package[i][READ_OUT].buf    = VM->VAM_TABLE->at(index).out;
        package[i][READ_OUT].size   = VM->VAM_TABLE->at(index).size_out;

        #ifdef VERBOSE
          printf("[DEBUG->vstart create] Reg_Buf_Buf, Thread Read Out created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
        threadsRet[i][READ_OUT]  = pthread_create(&threads[i][READ_OUT], NULL, Stream_Threads_Call, (void*) &package[i][READ_OUT]);

        // if (package[i][WRITE_IN1].buf != NULL) {
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart create] Reg_Buf_Buf, Thread Write IN1 created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif
        //   threadsRet[i][WRITE_IN1] = pthread_create(&threads[i][WRITE_IN1], NULL, Stream_Threads_Call, (void*) &package[i][WRITE_IN1]);
        // }

        if (package[i][WRITE_IN2].buf != NULL) {
        #ifdef VERBOSE
          printf("[DEBUG->vstart create] Reg_Buf_Buf, Thread Write IN2 created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
          threadsRet[i][WRITE_IN2] = pthread_create(&threads[i][WRITE_IN2], NULL, Stream_Threads_Call, (void*) &package[i][WRITE_IN2]);
        }
      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,-----.  ,------. ,------.
      //  |  |) /_ |  .--. '|  .--. '
      //  |  .-.  \|  '--'.'|  '--'.'
      //  |  '--' /|  |\  \ |  |\  \
      //  `------' `--' '--'`--' '--'
      //--------------------------------------------------------------------------------------------------
      case Buf_Reg_Reg: {
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Buf_Reg_Reg, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
        #endif

        package[i][WRITE_IN1].type   = WS;
        package[i][WRITE_IN1].VM     = VM;
        package[i][WRITE_IN1].card   = card;
        package[i][WRITE_IN1].index  = index;
        package[i][WRITE_IN1].stream = VM->VAM_TABLE->at(index).streamInA;
        package[i][WRITE_IN1].buf    = VM->VAM_TABLE->at(index).in1;
        package[i][WRITE_IN1].size   = VM->VAM_TABLE->at(index).size_in1;

        // package[i][WRITE_IN2].type   = WS;
        // package[i][WRITE_IN2].VM     = VM;
        // package[i][WRITE_IN2].card   = card;
        // package[i][WRITE_IN2].index  = index;
        // package[i][WRITE_IN2].stream = VM->VAM_TABLE->at(index).streamInB;
        // package[i][WRITE_IN2].buf    = VM->VAM_TABLE->at(index).in2;
        // package[i][WRITE_IN2].size   = VM->VAM_TABLE->at(index).size_in2;

        // package[i][READ_OUT].type   = RS;
        // package[i][READ_OUT].VM     = VM;
        // package[i][READ_OUT].card   = card;
        // package[i][READ_OUT].index  = index;
        // package[i][READ_OUT].stream = VM->VAM_TABLE->at(index).streamOut;
        // package[i][READ_OUT].buf    = VM->VAM_TABLE->at(index).out;
        // package[i][READ_OUT].size   = VM->VAM_TABLE->at(index).size_out;

        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart create] Buf_Buf_Buf, Thread Read Out created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif
        // threadsRet[i][READ_OUT]  = pthread_create(&threads[i][READ_OUT], NULL, Stream_Threads_Call, (void*) &package[i][READ_OUT]);

        if (package[i][WRITE_IN1].buf != NULL) {
        #ifdef VERBOSE
          printf("[DEBUG->vstart create] Buf_Buf_Buf, Thread Write IN1 created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
          threadsRet[i][WRITE_IN1] = pthread_create(&threads[i][WRITE_IN1], NULL, Stream_Threads_Call, (void*) &package[i][WRITE_IN1]);
        }

        // if (package[i][WRITE_IN2].buf != NULL) {
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart create] Buf_Buf_Buf, Thread Write IN2 created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif
        //   threadsRet[i][WRITE_IN2] = pthread_create(&threads[i][WRITE_IN2], NULL, Stream_Threads_Call, (void*) &package[i][WRITE_IN2]);
        // }
      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,------. ,-----.  ,------.
      //  |  .--. '|  |) /_ |  .--. '
      //  |  '--'.'|  .-.  \|  '--'.'
      //  |  |\  \ |  '--' /|  |\  \
      //  `--' '--'`------' `--' '--'
      //--------------------------------------------------------------------------------------------------
      case Reg_Buf_Reg: {
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Reg_Buf_Reg, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
        #endif
        // package[i][WRITE_IN1].type   = WS;
        // package[i][WRITE_IN1].VM     = VM;
        // package[i][WRITE_IN1].card   = card;
        // package[i][WRITE_IN1].index  = index;
        // package[i][WRITE_IN1].stream = VM->VAM_TABLE->at(index).streamInA;
        // package[i][WRITE_IN1].buf    = VM->VAM_TABLE->at(index).in1;
        // package[i][WRITE_IN1].size   = VM->VAM_TABLE->at(index).size_in1;

        package[i][WRITE_IN2].type   = WS;
        package[i][WRITE_IN2].VM     = VM;
        package[i][WRITE_IN2].card   = card;
        package[i][WRITE_IN2].index  = index;
        package[i][WRITE_IN2].stream = VM->VAM_TABLE->at(index).streamInB;
        package[i][WRITE_IN2].buf    = VM->VAM_TABLE->at(index).in2;
        package[i][WRITE_IN2].size   = VM->VAM_TABLE->at(index).size_in2;

        // package[i][READ_OUT].type   = RS;
        // package[i][READ_OUT].VM     = VM;
        // package[i][READ_OUT].card   = card;
        // package[i][READ_OUT].index  = index;
        // package[i][READ_OUT].stream = VM->VAM_TABLE->at(index).streamOut;
        // package[i][READ_OUT].buf    = VM->VAM_TABLE->at(index).out;
        // package[i][READ_OUT].size   = VM->VAM_TABLE->at(index).size_out;

        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart create] Reg_Buf_Reg, Thread Read Out created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif
        // threadsRet[i][READ_OUT]  = pthread_create(&threads[i][READ_OUT], NULL, Stream_Threads_Call, (void*) &package[i][READ_OUT]);

        // if (package[i][WRITE_IN1].buf != NULL) {
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart create] Reg_Buf_Reg, Thread Write IN1 created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif
        //   threadsRet[i][WRITE_IN1] = pthread_create(&threads[i][WRITE_IN1], NULL, Stream_Threads_Call, (void*) &package[i][WRITE_IN1]);
        // }

        if (package[i][WRITE_IN2].buf != NULL) {
        #ifdef VERBOSE
          printf("[DEBUG->vstart create] Reg_Buf_Reg, Thread Write IN2 created, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
          threadsRet[i][WRITE_IN2] = pthread_create(&threads[i][WRITE_IN2], NULL, Stream_Threads_Call, (void*) &package[i][WRITE_IN2]);
        }
      }break;
      //------------------------------------------------------------------------
      //     ,--.        ,---.                ,--.  ,--.
      //   ,-|  | ,---. /  .-' ,--,--.,--.,--.|  |,-'  '-.
      //  ' .-. || .-. :|  `-,' ,-.  ||  ||  ||  |'-.  .-'
      //  \ `-' |\   --.|  .-'\ '-'  |'  ''  '|  |  |  |
      //   `---'  `----'`--'   `--`--' `----' `--'  `--'
      //------------------------------------------------------------------------
      default: {
        printf("[DEBUG->vstart] vstart switch case error, please check number of steps.\r\n");
        return -1;
      }
    }
  }
  // gettimeofday(&end, NULL);
  // timeuse1 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("[DEBUG->vstart Switch1]        : \t%'9d us\r\n", timeuse1);

  // gettimeofday(&start, NULL);
  for (i = 0; i < size; i++) {
    card  = nPR->at(i) >> 4;
    node  = nPR->at(i) & 0xF;
    index = card * ROW + node;
    switch (VM->VAM_TABLE->at(index).node_type) {
      //------------------------------------------------------------------------
      //  ,-----.  ,-----.  ,-----.
      //  |  |) /_ |  |) /_ |  |) /_
      //  |  .-.  \|  .-.  \|  .-.  \
      //  |  '--' /|  '--' /|  '--' /
      //  `------' `------' `------'
      //------------------------------------------------------------------------
      case Buf_Buf_Buf: {
        if (package[i][WRITE_IN1].buf  != NULL)
          pthread_join(threads[i][WRITE_IN1], NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart join] Buf_Buf_Buf, Thread Write IN1 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif

        if (package[i][WRITE_IN2].buf != NULL)
          pthread_join(threads[i][WRITE_IN2], NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart join] Buf_Buf_Buf, Thread Write IN2 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif

        pthread_join(threads[i][READ_OUT],  NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart join] Buf_Buf_Buf, Thread Read  OUT Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif

          // request Mutex for CMD Stream
          // #ifdef VERBOSE
          //   printf("[DEBUG->vstart join] vend thread request mutex for cmd_stream\r\n");
          // #endif
          // pthread_mutex_lock(&VM->vm_mutex);
          // #ifdef VERBOSE
          //   printf("[DEBUG->vstart join] vend thread get mutex...\r\n");
          // #endif
          // Read RSP from CMD Stream
          // cmd_stream = VM->pico[card]->CreateStream(50);
          // tmp = new int[4];
          // VM->pico[card]->ReadStream(cmd_stream, tmp, 16);
          // #ifdef VERBOSE
          // for (i = 0; i < 4; i++)
          //   printf("[DEBUG->vstart join] RSP from cmd_stream [%d] : 0x%08x\r\n", i, tmp[i]);
          // #endif
          // delete[] tmp;
          // VM->pico[card]->CloseStream(cmd_stream);
          // // Releast Mutex on CMD Stream
          // #ifdef VERBOSE
          //   printf("[DEBUG->vstart join] vend thread release mutex...\r\n");
          // #endif
          // pthread_mutex_unlock(&VM->vm_mutex);
      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,-----.  ,-----.  ,------.
      //  |  |) /_ |  |) /_ |  .--. '
      //  |  .-.  \|  .-.  \|  '--'.'
      //  |  '--' /|  '--' /|  |\  \
      //  `------' `------' `--' '--'
      //--------------------------------------------------------------------------------------------------
      case Buf_Buf_Reg: {
        if (package[i][WRITE_IN1].buf  != NULL) {
          pthread_join(threads[i][WRITE_IN1], NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart join] Buf_Buf_Reg, Thread Write IN1 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
        }

        if (package[i][WRITE_IN2].buf != NULL) {
          pthread_join(threads[i][WRITE_IN2], NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart join] Buf_Buf_Reg, Thread Write IN2 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
        }

      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,------. ,------. ,-----.
      //  |  .--. '|  .--. '|  |) /_
      //  |  '--'.'|  '--'.'|  .-.  \
      //  |  |\  \ |  |\  \ |  '--' /
      //  `--' '--'`--' '--'`------'
      //--------------------------------------------------------------------------------------------------
      case Reg_Reg_Buf: {
        pthread_join(threads[i][READ_OUT],  NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart join] Reg_Reg_Buf, Thread Read  OUT Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,------. ,------. ,------.
      //  |  .--. '|  .--. '|  .--. '
      //  |  '--'.'|  '--'.'|  '--'.'
      //  |  |\  \ |  |\  \ |  |\  \
      //  `--' '--'`--' '--'`--' '--'
      //--------------------------------------------------------------------------------------------------
      case Reg_Reg_Reg: {
      // Do Nothing
      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,-----.  ,------. ,-----.
      //  |  |) /_ |  .--. '|  |) /_
      //  |  .-.  \|  '--'.'|  .-.  \
      //  |  '--' /|  |\  \ |  '--' /
      //  `------' `--' '--'`------'
      //--------------------------------------------------------------------------------------------------
      case Buf_Reg_Buf: {
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Buf_Reg_Buf, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
        #endif

        if (package[i][WRITE_IN1].buf  != NULL)
          pthread_join(threads[i][WRITE_IN1], NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart join] Buf_Reg_Buf, Thread Write IN1 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif

        // if (package[i][WRITE_IN2].buf != NULL)
        //   pthread_join(threads[i][WRITE_IN2], NULL);
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart join] Buf_Reg_Buf, Thread Write IN2 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif

        pthread_join(threads[i][READ_OUT],  NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart join] Buf_Reg_Buf, Thread Write OUT Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,------. ,-----.  ,-----.
      //  |  .--. '|  |) /_ |  |) /_
      //  |  '--'.'|  .-.  \|  .-.  \
      //  |  |\  \ |  '--' /|  '--' /
      //  `--' '--'`------' `------'
      //--------------------------------------------------------------------------------------------------
      case Reg_Buf_Buf: {
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Reg_Buf_Buf, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
        #endif

        // if (package[i][WRITE_IN1].buf  != NULL)
        //   pthread_join(threads[i][WRITE_IN1], NULL);
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart join] Reg_Buf_Buf, Thread Write IN1 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif

        if (package[i][WRITE_IN2].buf != NULL)
          pthread_join(threads[i][WRITE_IN2], NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart join] Reg_Buf_Buf, Thread Write IN2 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif

        pthread_join(threads[i][READ_OUT],  NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart join] Reg_Buf_Buf, Thread Write OUT Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif
      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,-----.  ,------. ,------.
      //  |  |) /_ |  .--. '|  .--. '
      //  |  .-.  \|  '--'.'|  '--'.'
      //  |  '--' /|  |\  \ |  |\  \
      //  `------' `--' '--'`--' '--'
      //--------------------------------------------------------------------------------------------------
      case Buf_Reg_Reg: {
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Buf_Reg_Reg, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
        #endif

        if (package[i][WRITE_IN1].buf  != NULL)
          pthread_join(threads[i][WRITE_IN1], NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart join] Buf_Reg_Reg, Thread Write IN1 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif

        // if (package[i][WRITE_IN2].buf != NULL)
        //   pthread_join(threads[i][WRITE_IN2], NULL);
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart join] Buf_Reg_Reg, Thread Write IN2 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif

        // pthread_join(threads[i][READ_OUT],  NULL);
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart join] Buf_Reg_Reg, Thread Write OUT Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif
      }break;
      //--------------------------------------------------------------------------------------------------
      //  ,------. ,-----.  ,------.
      //  |  .--. '|  |) /_ |  .--. '
      //  |  '--'.'|  .-.  \|  '--'.'
      //  |  |\  \ |  '--' /|  |\  \
      //  `--' '--'`------' `--' '--'
      //--------------------------------------------------------------------------------------------------
      case Reg_Buf_Reg: {
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Reg_Buf_Reg, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
        #endif

        // if (package[i][WRITE_IN1].buf  != NULL)
        //   pthread_join(threads[i][WRITE_IN1], NULL);
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart join] Reg_Buf_Reg, Thread Write IN1 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif

        if (package[i][WRITE_IN2].buf != NULL)
          pthread_join(threads[i][WRITE_IN2], NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart join] Reg_Buf_Reg, Thread Write IN2 Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif

        // pthread_join(threads[i][READ_OUT],  NULL);
        // #ifdef VERBOSE
        //   printf("[DEBUG->vstart join] Reg_Buf_Reg, Thread Write OUT Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        // #endif
      }break;
      //------------------------------------------------------------------------
      //     ,--.        ,---.                ,--.  ,--.
      //   ,-|  | ,---. /  .-' ,--,--.,--.,--.|  |,-'  '-.
      //  ' .-. || .-. :|  `-,' ,-.  ||  ||  ||  |'-.  .-'
      //  \ `-' |\   --.|  .-'\ '-'  |'  ''  '|  |  |  |
      //   `---'  `----'`--'   `--`--' `----' `--'  `--'
      //------------------------------------------------------------------------
      default: {
        printf("[DEBUG->vstart] vstart switch case error \r\n");
        return -1;
      }
    }
  }
  // gettimeofday(&end, NULL);
  // timeuse2 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  // printf("[DEBUG->vstart Switch2]        : \t%'9d us\r\n", timeuse2);
  // printf("[DEBUG->vstart Total]          : \t%'9d us\r\n", timeuse1 + timeuse2);
  // printf("[DEBUG->vstart Throughput]     : \t%'7.2f MB/s\r\n", ((SIZE * 1 * 4.0 / 1024 / 1024) / ((timeuse1 + timeuse2) * 1.0 / 1000000)));

  for (i = 0; i < size; i++) {
    delete[] threads[i];
    delete[] threadsRet[i];
    delete[] package[i];
  }
  delete[] threads;
  delete[] threadsRet;
  delete[] package;

  return 0;
}
//==================================================================================================
//  ____    ____  _______ .__   __.  _______
//  \   \  /   / |   ____||  \ |  | |       \
//   \   \/   /  |  |__   |   \|  | |  .--.  |
//    \      /   |   __|  |  . `  | |  |  |  |
//     \    /    |  |____ |  |\   | |  '--'  |
//      \__/     |_______||__| \__| |_______/
//
//==================================================================================================
int vend(vam_vm_t *VM, vector<int> *nPR, int size_out)
{
#ifdef VERBOSE
  printf("\r\n");
#endif

  uint32_t  cmd[4] = {0, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
  char      ibuf[1024];
  int       err;
  int       cmd_stream;
  int       card  ;
  int       node  ;
  int       index ;
  int       i     ;
  int       size  ;
  int      *tmp = NULL;
  pthread_t threads[VSTART_THREADS];
  int       threadsRet[VSTART_THREADS];

  vstart_pk_t package[3]; // Package for two write and one read

  size = nPR->size();
  for (i = 0; i < size; i++) {
    card  = nPR->at(i) >> 4;
    node  = nPR->at(i) & 0xF;
    index = card * ROW + node;

    switch (VM->VAM_TABLE->at(index).node_type) {
      //------------------------------------------------------------------------
      //  ,-----.  ,-----.  ,-----.
      //  |  |) /_ |  |) /_ |  |) /_
      //  |  .-.  \|  .-.  \|  .-.  \
      //  |  '--' /|  '--' /|  '--' /
      //  `------' `------' `------'
      //------------------------------------------------------------------------
      case Buf_Buf_Buf: {
        #ifdef VERBOSE
          printf("[DEBUG->vend] Buf_Buf_Buf, Steps:%d\tIn1:%p\tIn2:%p\tOut:%p\r\n", size, VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
          printf("[DEBUG->vend] Buf_Buf_Buf, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif

        package[READ_OUT].type   = RS;
        package[READ_OUT].VM     = VM;
        package[READ_OUT].card   = card;
        package[READ_OUT].index  = index;
        package[READ_OUT].stream = VM->VAM_TABLE->at(index).streamOut;
        package[READ_OUT].buf    = VM->VAM_TABLE->at(index).out;
        package[READ_OUT].size   = size_out;

        threadsRet[READ_OUT]  = pthread_create(&threads[READ_OUT], NULL, Stream_Threads_Call, (void*) &package[READ_OUT]);
        pthread_join(threads[READ_OUT],  NULL);
        #ifdef VERBOSE
          printf("[DEBUG->vstart] Buf_Buf_Buf, Thread Write OUT Join, nPR:%d, Card:%d, Node:%d, index:%d\r\n", nPR->at(i), card, node, index);
        #endif

        // request Mutex for CMD Stream
        #ifdef VERBOSE
          printf("[DEBUG->vend_TCALL] vend thread request mutex for cmd_stream\r\n");
        #endif
        pthread_mutex_lock(&VM->vm_mutex);
        #ifdef VERBOSE
          printf("[DEBUG->vend_TCALL] vend thread get mutex...\r\n");
        #endif
        // Read RSP from CMD Stream
        cmd_stream = VM->pico[card]->CreateStream(50);
        tmp = new int[4];
        VM->pico[card]->ReadStream(cmd_stream, tmp, 16);
        #ifdef VERBOSE
        for (i = 0; i < 4; i++)
          printf("[DEBUG->vend_TCALL] RSP from cmd_stream [%d] : 0x%08x\r\n", i, tmp[i]);
        #endif
        delete[] tmp;
        VM->pico[card]->CloseStream(cmd_stream);
        // Releast Mutex on CMD Stream
        #ifdef VERBOSE
          printf("[DEBUG->vend_TCALL] vend thread release mutex...\r\n");
        #endif
        pthread_mutex_unlock(&VM->vm_mutex);

      }break;
      //------------------------------------------------------------------------
      //     ,--.        ,---.                ,--.  ,--.
      //   ,-|  | ,---. /  .-' ,--,--.,--.,--.|  |,-'  '-.
      //  ' .-. || .-. :|  `-,' ,-.  ||  ||  ||  |'-.  .-'
      //  \ `-' |\   --.|  .-'\ '-'  |'  ''  '|  |  |  |
      //   `---'  `----'`--'   `--`--' `----' `--'  `--'
      //------------------------------------------------------------------------
      default: {
        printf("[DEBUG->vend] vend switch case error \r\n");
        return -1;
      }
    }
  }

  return 0;
}

// int vstart(vam_vm_t *VM, vector<int> *nPR, int items)
// {
//   char      ibuf[1024];
//   int       err;
//   int       stream;
//   int       card  ;
//   int       node  ;
//   int       index ;
//   int       i     ;
//   int       size  ;

//   size = nPR->size();
//   for (i = 0; i < size; i++) {
//     card  = nPR->at(i) >> 4;
//     node  = nPR->at(i) & 0xF;
//     index = card * ROW + node;

//     #ifdef VERBOSE
//       printf("[DEBUG->vstart] In1:%p, %p, %p\r\n", VM->VAM_TABLE->at(index).in1, VM->VAM_TABLE->at(index).in2, VM->VAM_TABLE->at(index).out);
//     #endif

//     #ifdef VERBOSE
//       printf("[DEBUG->vstart] Writing %i Bytes to 0x%08x\n", items * 4, VM->VAM_TABLE->at(index).streamInA); // Write bytes not words.
//     #endif
//     err = VM->pico[card]->WriteStream(VM->VAM_TABLE->at(index).streamInA, VM->VAM_TABLE->at(index).in1,  items * 4);
//     if (err < 0) {
//       fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
//       return -1;
//     }

//     #ifdef VERBOSE
//       printf("[DEBUG->vstart] Writing %i Bytes to 0x%08x\n", items * 4, VM->VAM_TABLE->at(index).streamInB); // Write bytes not words.
//     #endif
//     err = VM->pico[card]->WriteStream(VM->VAM_TABLE->at(index).streamInB, VM->VAM_TABLE->at(index).in2,  items * 4);
//     if (err < 0) {
//       fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
//       return -1;
//     }

//     #ifdef VERBOSE
//       printf("[DEBUG->vend] Reading %i Bytes from 0x%08x\n", items * 4, VM->VAM_TABLE->at(index).streamOut); // Write bytes not words.
//     #endif
//     err = VM->pico[card]->ReadStream(VM->VAM_TABLE->at(index).streamOut, VM->VAM_TABLE->at(index).out,  items * 4);
//     if (err < 0) {
//       fprintf(stderr, "WriteStream error: %s\n", PicoErrors_FullError(err, ibuf, sizeof(ibuf)));
//       return -1;
//     }

//   }
//   return 0;
// }





  // BITSTREAM_TABLE->item[VSUB].BitAddr[0] = (uint32_t*)acc_vsub_PR0_bit;
  // BITSTREAM_TABLE->item[VSUB].BitAddr[1] = (uint32_t*)acc_vsub_PR1_bit;
  // BITSTREAM_TABLE->item[VSUB].BitAddr[2] = (uint32_t*)acc_vsub_PR2_bit;
  // BITSTREAM_TABLE->item[VSUB].BitAddr[3] = (uint32_t*)acc_vsub_PR3_bit;
  // BITSTREAM_TABLE->item[VSUB].BitAddr[4] = (uint32_t*)acc_vsub_PR4_bit;

  // BITSTREAM_TABLE->item[VSUB].BitSize[0] = (uint32_t) acc_vsub_PR0_bit_len;
  // BITSTREAM_TABLE->item[VSUB].BitSize[1] = (uint32_t) acc_vsub_PR1_bit_len;
  // BITSTREAM_TABLE->item[VSUB].BitSize[2] = (uint32_t) acc_vsub_PR2_bit_len;
  // BITSTREAM_TABLE->item[VSUB].BitSize[3] = (uint32_t) acc_vsub_PR3_bit_len;
  // BITSTREAM_TABLE->item[VSUB].BitSize[4] = (uint32_t) acc_vsub_PR4_bit_len;

  // //////////////////////////////////////////////////////////////////////////////
  // BITSTREAM_TABLE->item[A2PB2].BitAddr[0] = (uint32_t*)acc_a2pb2_PR0_bit;
  // BITSTREAM_TABLE->item[A2PB2].BitAddr[1] = (uint32_t*)acc_a2pb2_PR1_bit;
  // BITSTREAM_TABLE->item[A2PB2].BitAddr[2] = (uint32_t*)acc_a2pb2_PR2_bit;
  // BITSTREAM_TABLE->item[A2PB2].BitAddr[3] = (uint32_t*)acc_a2pb2_PR3_bit;
  // BITSTREAM_TABLE->item[A2PB2].BitAddr[4] = (uint32_t*)acc_a2pb2_PR4_bit;

  // BITSTREAM_TABLE->item[A2PB2].BitSize[0] = (uint32_t) acc_a2pb2_PR0_bit_len;
  // BITSTREAM_TABLE->item[A2PB2].BitSize[1] = (uint32_t) acc_a2pb2_PR1_bit_len;
  // BITSTREAM_TABLE->item[A2PB2].BitSize[2] = (uint32_t) acc_a2pb2_PR2_bit_len;
  // BITSTREAM_TABLE->item[A2PB2].BitSize[3] = (uint32_t) acc_a2pb2_PR3_bit_len;
  // BITSTREAM_TABLE->item[A2PB2].BitSize[4] = (uint32_t) acc_a2pb2_PR4_bit_len;
  // //////////////////////////////////////////////////////////////////////////////
  // BITSTREAM_TABLE->item[VAPBB].BitAddr[0] = (uint32_t*)acc_vapbb_PR0_bit;
  // BITSTREAM_TABLE->item[VAPBB].BitAddr[1] = (uint32_t*)acc_vapbb_PR1_bit;
  // BITSTREAM_TABLE->item[VAPBB].BitAddr[2] = (uint32_t*)acc_vapbb_PR2_bit;
  // BITSTREAM_TABLE->item[VAPBB].BitAddr[3] = (uint32_t*)acc_vapbb_PR3_bit;
  // BITSTREAM_TABLE->item[VAPBB].BitAddr[4] = (uint32_t*)acc_vapbb_PR4_bit;

  // BITSTREAM_TABLE->item[VAPBB].BitSize[0] = (uint32_t) acc_vapbb_PR0_bit_len;
  // BITSTREAM_TABLE->item[VAPBB].BitSize[1] = (uint32_t) acc_vapbb_PR1_bit_len;
  // BITSTREAM_TABLE->item[VAPBB].BitSize[2] = (uint32_t) acc_vapbb_PR2_bit_len;
  // BITSTREAM_TABLE->item[VAPBB].BitSize[3] = (uint32_t) acc_vapbb_PR3_bit_len;
  // BITSTREAM_TABLE->item[VAPBB].BitSize[4] = (uint32_t) acc_vapbb_PR4_bit_len;
  // //////////////////////////////////////////////////////////////////////////////
  // BITSTREAM_TABLE->item[VAAPB].BitAddr[0] = (uint32_t*)acc_vaapb_PR0_bit;
  // BITSTREAM_TABLE->item[VAAPB].BitAddr[1] = (uint32_t*)acc_vaapb_PR1_bit;
  // BITSTREAM_TABLE->item[VAAPB].BitAddr[2] = (uint32_t*)acc_vaapb_PR2_bit;
  // BITSTREAM_TABLE->item[VAAPB].BitAddr[3] = (uint32_t*)acc_vaapb_PR3_bit;
  // BITSTREAM_TABLE->item[VAAPB].BitAddr[4] = (uint32_t*)acc_vaapb_PR4_bit;

  // BITSTREAM_TABLE->item[VAAPB].BitSize[0] = (uint32_t) acc_vaapb_PR0_bit_len;
  // BITSTREAM_TABLE->item[VAAPB].BitSize[1] = (uint32_t) acc_vaapb_PR1_bit_len;
  // BITSTREAM_TABLE->item[VAAPB].BitSize[2] = (uint32_t) acc_vaapb_PR2_bit_len;
  // BITSTREAM_TABLE->item[VAAPB].BitSize[3] = (uint32_t) acc_vaapb_PR3_bit_len;
  // BITSTREAM_TABLE->item[VAAPB].BitSize[4] = (uint32_t) acc_vaapb_PR4_bit_len;

  // //////////////////////////////////////////////////////////////////////////////
  // BITSTREAM_TABLE->item[SQLSELECT].BitAddr[0] = (uint32_t*)acc_sql_select_PR0_bit;
  // BITSTREAM_TABLE->item[SQLSELECT].BitAddr[1] = (uint32_t*)acc_sql_select_PR1_bit;
  // BITSTREAM_TABLE->item[SQLSELECT].BitAddr[2] = (uint32_t*)acc_sql_select_PR2_bit;
  // BITSTREAM_TABLE->item[SQLSELECT].BitAddr[3] = (uint32_t*)acc_sql_select_PR3_bit;
  // BITSTREAM_TABLE->item[SQLSELECT].BitAddr[4] = (uint32_t*)acc_sql_select_PR4_bit;

  // BITSTREAM_TABLE->item[SQLSELECT].BitSize[0] = (uint32_t) acc_sql_select_PR0_bit_len;
  // BITSTREAM_TABLE->item[SQLSELECT].BitSize[1] = (uint32_t) acc_sql_select_PR1_bit_len;
  // BITSTREAM_TABLE->item[SQLSELECT].BitSize[2] = (uint32_t) acc_sql_select_PR2_bit_len;
  // BITSTREAM_TABLE->item[SQLSELECT].BitSize[3] = (uint32_t) acc_sql_select_PR3_bit_len;
  // BITSTREAM_TABLE->item[SQLSELECT].BitSize[4] = (uint32_t) acc_sql_select_PR4_bit_len;
  // //////////////////////////////////////////////////////////////////////////////
  // BITSTREAM_TABLE->item[SQLAVG].BitAddr[0] = (uint32_t*)acc_f_avg_PR0_bit;
  // BITSTREAM_TABLE->item[SQLAVG].BitAddr[1] = (uint32_t*)acc_f_avg_PR1_bit;
  // BITSTREAM_TABLE->item[SQLAVG].BitAddr[2] = (uint32_t*)acc_f_avg_PR2_bit;
  // BITSTREAM_TABLE->item[SQLAVG].BitAddr[3] = (uint32_t*)acc_f_avg_PR3_bit;
  // BITSTREAM_TABLE->item[SQLAVG].BitAddr[4] = (uint32_t*)acc_f_avg_PR4_bit;

  // BITSTREAM_TABLE->item[SQLAVG].BitSize[0] = (uint32_t) acc_f_avg_PR0_bit_len;
  // BITSTREAM_TABLE->item[SQLAVG].BitSize[1] = (uint32_t) acc_f_avg_PR1_bit_len;
  // BITSTREAM_TABLE->item[SQLAVG].BitSize[2] = (uint32_t) acc_f_avg_PR2_bit_len;
  // BITSTREAM_TABLE->item[SQLAVG].BitSize[3] = (uint32_t) acc_f_avg_PR3_bit_len;
  // BITSTREAM_TABLE->item[SQLAVG].BitSize[4] = (uint32_t) acc_f_avg_PR4_bit_len;

