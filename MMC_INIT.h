
#define CS PORTC.F0         // output for chip select Sd/MMC
#define t_LED PORTB.F5
#define errorLED PORTB.F4
//########################## MMC/SD COMMANDS ##############################################
#define GO_IDLE_STATE            0x00       //
#define SEND_OP_COND             0x01        //
#define SEND_IF_COND                 0x08       //
#define SEND_CSD                 0x09       //
#define STOP_TRANSMISSION        0x0C       //
#define SEND_STATUS              0x0D       //
#define SET_BLOCK_LEN            0x10         //
#define READ_SINGLE_BLOCK        0x11          //
#define READ_MULTIPLE_BLOCKS     0x12   //
#define WRITE_SINGLE_BLOCK       0x18 //
#define WRITE_MULTIPLE_BLOCKS    0x19 //
#define ERASE_BLOCK_START_ADDR   0x20 //
#define ERASE_BLOCK_END_ADDR     0x21 //
#define ERASE_SELECTED_BLOCKS    0x26 //   //
#define SD_SEND_OP_COND          0x29   //ACMD
#define APP_CMD                  0x37    //
#define READ_OCR                 0x3A      //
#define CRC_ON_OFF               0x38        //
///##################################################################################
void intspi();
unsigned char SPI (unsigned char dat);
char Command(unsigned char frame1, unsigned long adrs, unsigned char frame2 );
char MMC_Init();
void READ_Mlti(unsigned long Sectr);
void MMC_Read_Single(unsigned long sectr);
char MMC_set_blocklen(unsigned long size);
void MMC_Close();


void FAT_init();
void initroot();
void MMC_skip();
//************************************************ ********************
//unsigned long  FILE_EXT;
unsigned int   FILE_LOC;
unsigned int   BPB_BytesPerSec;
unsigned char  BPB_SecPerClus;
unsigned int   BPB_RsvdSecCnt;
unsigned int   BPB_NumFATs;
unsigned int   BPB_SecperFats;
unsigned long  Clustrsize;
unsigned int   root_entries;
unsigned int   size_of_root,rtdircount;
unsigned  int  jump;
unsigned char buff[26];
unsigned char buff1[32]absolute 0x120;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

unsigned long  loc;
unsigned long  Curr_File;
unsigned long  rootdir_sector;
unsigned long  BPB_FileSize; // file size  bytes  123 > 127      in root dir   in 127 byts
  extern  int  jump;
         char  swtch;
 unsigned char extention[5];
