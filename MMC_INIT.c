 #include "MMC_INIT.h"
//################################spi init#########################################
  void intspi()
  {
     SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV4, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_HIGH, _SPI_LOW_2_HIGH); //MODIFIED
 //SSPCON = 0B00110010;        //low speed osc/64(125kHz),enabled,clock idle=H
}

  unsigned char SPI (unsigned char temp) // send via SPI interface
  {

          SSPBUF = temp;
            while (!BF_bit){} // Wait until send is

     return   SSPBUF; // received simultaneously
  }
  //***************************SEND CMDs*************** **********************

 char Command(unsigned char frame1, unsigned long adrs, unsigned char frame2 )
{
        unsigned char i, res;
        SPI(0xFF);
        SPI((frame1| 0x40) & 0x7F);        //first 2 bits are 01
        SPI((adrs & 0xFF000000) >> 24);                //first of the 4 bytes address
        SPI((adrs & 0x00FF0000) >> 16);
        SPI((adrs & 0x0000FF00) >> 8);
        SPI(adrs & 0x000000FF);
        SPI(frame2 | 1);                                //CRC and last bit 1

        for(i=0;i<10;i++)        // wait for received character
        {
                res = SPI(0xFF);
              if(res != 0xFF)break;
        }
        return res;
}
  //************************************************ **************************

void MMC_Init()
{
        unsigned char i;

        delay_ms(100);
        for(i=0; i < 10; i++){SPI(0xFF);}               // min 74 clocks
        CS=0;                        // Enabled for SPI mode

        if (Command(GO_IDLE_STATE ,0,0x95) !=1) errorLED = 1;        // Reset SD command

        while (Command(SEND_OP_COND,0,0xFF) !=0) {} //wait for idle state


        SSPM1_bit = 0;        // full speed 2MHz
        MMC_Close();

}
//##########################read multi ################################
void READ_Mlti(unsigned long Sectr)
{
   char res;
      CS = 0;
        res = Command(READ_MULTIPLE_BLOCKS,sectr,0xFF);        //read multi-sector
        if(res != 0)errorLED = 1;                        //if command failed
}

 /*********** Send a read command to SD/MMC, without reading the bytes *********/
void MMC_Read_Single(unsigned long sectr)
{
  char res;
  CS=0;


    res = Command(READ_SINGLE_BLOCK,sectr,0xFF);
      if(res != 0)PORTD.F1 = 1;                        //if command failed




}
/**************************** Send cmd16 - set block length *******************/
char MMC_set_blocklen(unsigned long size)
{
  char res;
  CS=0;
       res = Command(SET_BLOCK_LEN, size, 0xFF);                 // Send cmd16 - set block length size


  return res;
}
/******************************** Disable SD/MMC ******************************/
void MMC_Close()
{
                Command(STOP_TRANSMISSION ,0x00,0xFF);        //stop transmit
                SPI(0xFF);
                SPI(0xFF);
                CS = 1;

  }
/******************************************************************************/




/////////////////////////////////////////////////////////////////////////////////////////////////
void MMC_skip(int count){

        for (;count>0;count--){
                 SPI(0xFF);

        }
}


////////////////////////////////////////////////////////////////////////////////////////////////
 //**************************** Initialize FAT variables ************************/

void FAT_init()
{
 unsigned  int ii,xx;
  SSPCON = 0B00110010;        //low speed osc/64(125kHz),enabled,clock idle=H
     MMC_Close();




                      READ_Mlti(0);
      while(buff[0]!=0xEB)
 {
                      while(SPI(0xFF) != 0xFE) {}       // wait for first byte

      for(ii = 0; ii < 512; ii++)
  {
      if(ii==0)
   {
        for(xx=0;xx<25;xx++)
     {
         buff[xx]=(SPI(0xFF)&0xFF);

      }
    }else {SPI(0xFF);}
   }
       SPI(0xFF);        //discard of CRC
       SPI(0xFF);


 }
          MMC_Close();




  Delay_ms(10);





    PORTD.F2 = (buff[0]);   // not important


 //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
           Lo(BPB_BytesPerSec) = (buff[11]);
        Hi(BPB_BytesPerSec) = (buff[12]);


        BPB_SecPerClus = buff[13];


        Lo(BPB_RsvdSecCnt) = (buff[14]);
        Hi(BPB_RsvdSecCnt) = (buff[15]);



        BPB_NumFATs = buff[16];


        Lo(root_entries) = (buff[17]);
        Hi(root_entries) = (buff[18]);

        Lo(BPB_SecperFats) = (buff[22]);
        Hi(BPB_SecperFats) = (buff[23]);


  rootdir_sector =((((unsigned long)BPB_SecperFats * BPB_NumFATs)+ BPB_RsvdSecCnt)* BPB_BytesPerSec);

  Clustrsize = ((unsigned long)BPB_BytesPerSec*BPB_SecPerClus);
  size_of_root =((unsigned int)root_entries*32);
   PORTD=0;            // not important
   SSPM1_bit = 0;        // full speed 2MHz
  delay_ms(100);
}




  /////**************rootDir  **********************************************
void readdirbuff()
{
    unsigned  int ii,x;
    SSPM1_bit = 0;       //full speed 2MHz

     PORTD=0;    // not important


      rtdircount = 0;

          READ_Mlti((unsigned long)rootdir_sector);

           while(1)
  {
                      while(SPI(0xFF) != 0xFE) {}       // wait for first byte

        for(ii=0; ii<512; ii++)
       {

               if(rtdircount==jump)
         {
             for(x=0;x<32;x++)
           {
                                          IRP_bit =1;
             buff1[x] =(SPI(0xFF)&0xFF);
                                          IRP_bit =0;
             ii++;

            }

         }

            else{ SPI(0xFF);

            }


                 rtdircount++;
       }
            SPI(0xFF);        //discard of CRC
            SPI(0xFF);
                          if (rtdircount > jump)break;
                  if (rtdircount >= size_of_root)break;
  }

    MMC_Close();


                                         IRP_bit =1;
         PORTD.B2 = (buff1[0]);                        // not important
                                         IRP_bit =0;

}
/////////////////////////////////////////////////////////////////////////////////////////////
  void initroot()
{
        readdirbuff();

       while(buff1[0] == 0xE5)
   {
       jump+=32;
       readdirbuff();
   }

   switch (buff1[0])                                // calculating the root dir...
   {
       case 0x00:jump=0;readdirbuff();break;
       case 0x42:jump+=64;readdirbuff();break;
       case 0x43:jump+=96;readdirbuff();break;
       case 0x44:jump+=128;readdirbuff();break;
       case 0x45:jump+=160;readdirbuff();break;
       case 0x46:jump+=192;readdirbuff();break;
       case 0x47:jump+=224;readdirbuff();break;
       case 0x48:jump+=256;readdirbuff();break;
       case 0x49:jump+=288;readdirbuff();break;
       case 0x4A:jump+=320;readdirbuff();break;
       case 0x4B:jump+=352;readdirbuff();break;
       case 0x4C:jump+=384;readdirbuff();break;
       case 0x4D:jump+=416;readdirbuff();break;
       case 0x4E:jump+=448;readdirbuff();break;
       case 0x4F:jump+=480;readdirbuff();break;
    }
     //........................to find 1st song auto...................................................
     delay_ms(10);

          IRP_bit =1;

          extention[3] = (buff1[11]);   // get the file extention
          extention[2] = (buff1[10]);
          extention[1] = (buff1[9]);
          extention[0] = (buff1[8]);


          Lo(FILE_LOC) = (buff1[26]);    // file location
          Hi(FILE_LOC) = (buff1[27]);


           Lo(BPB_FileSize) = (buff1[28]);      // file size
           Hi(BPB_FileSize) =  (buff1[29]);
           Higher(BPB_FileSize) =  (buff1[30]);
           Highest(BPB_FileSize)= (buff1[31]);

           IRP_bit =0;

      Curr_File = ((unsigned long)rootdir_sector+(Clustrsize*FILE_LOC)-Clustrsize);  // calculate the file start


    SSPM1_bit = 0;        // full speed 2MHz
    PORTD=0;        // not important

}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++