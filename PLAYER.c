
//########################### difines & includes ##############################################
#include "built_in.h"
#include "MMC_INIT.c"


#define t_LED PORTB.F5      //timming LED green
#define errorLED PORTB.F4    // error LED  red
#define Stop PORTB.F3       //stop PB
#define Play PORTB.F1        //play nd PAUSE PB
#define next PORTB.F2          //next file PB
#define PwrOff_Rep PORTB.F0        //// to reset pic  after stop or repet song while playing and the led is on PB

//*****************************************************************************
char swp();
void PAUSE();
void forward();
void ReadSD(void);
//#################################### PAUSE ############################################
void PAUSE()                  //
{                             //
      delay_ms(500);          //
   while(Play)                //
   {                          //
         t_LED = 1;          //
          delay_ms(200);      //
      if(!Play)break;         //
          t_LED = 0;         //
       delay_ms(200);         //
    }                         //
     delay_ms(500);           //
 }                            //
 //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>NEXT SONG>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 void forward()                //
 {                             //
      if(swtch==0)             //
   {                           //
          jump+=32;            //
            MMC_Close();       //
             initroot();       //
            loc = 512;         //
           PORTD.F3 = 0;       //
          READ_Mlti(Curr_File);//read multi
                               //
   }else{                      //
                               //
    MMC_Close();               //
       loc = 512;              //
        PORTD.F3 = 1;          //
                               //
     READ_Mlti(Curr_File);     //read multi
   }                           //
 }                             //
 //----------------------------------swap var values----------------------------------------
 char b=0,k=0,h=1;    //
 char swp()           //
 {                    //           needs some modifications
    b=k;              //
       k=h;           //
         h=b;         //
       swtch=b;       //
       return swtch;  //
 }                    //
                      //
 //***********************************play music from sd/mmc****************************
void ReadSD()
{
       unsigned int i,count;
       unsigned char datt;


           loc = 512;
              delay_ms(1000);   // delay to avoid multi press for PAUSE and next buttons
                  if(strcmp(extention, "WAV ") != 0)
                 {
                     forward();    //next song
                 }

                        READ_Mlti(Curr_File);    //read multi sectors

        while(Stop)
        {      ///////////////////////////////
                while(SPI(0xFF) != 0xFE) {} //      // wait for first byte
                for(i=0;i<256;i++)          //
              { //////////////////////////////

                                        ////////////////////////////////////
                                          if(!PwrOff_Rep){                       //
                                          if(swp()==1){PORTD.F3 = 1;}     //
                                          else{ PORTD.F3 = 0;} }          //
                                        ////////////////////////////////////
                                              if(!Play) PAUSE();          //
                                                                          //
                                          if (count < 23000)t_LED = 1;   //
                                          if (count > 23000)t_LED = 0;   //
                                                                          //
                                          count++;                        //
                                        ////////////////////////////////////
                                                 if(!next)                //
                                             {                            //
                                                   i = 0;                 //
                                                    MMC_Close();          //
                                                 return ;                 //
                                             }                            //
                                        ////////////////////////////////////
                                                 if(Stop == 0)            //
                                             {                            //
                                                        i = 0;            //
                                                        MMC_Close();      //  to stop playing and reset fat
                                                        t_LED = 0;       //
                                                  return ;                //
                                              }                           //
                                        ////////////////////////////////////
                                        if(loc<556)
                                        {             //ignoring 44bytes of subchunks to avoid noise
                                        }
                                        else
                                        {
                        //////////////////////////////////
                        while(!TMR2IF_bit){}            //   wait for the flag to set
                                                        //
                                                        //
                       // LEFT channel                  //
                       datt = SPI(0xFF);                //
                       CCP2X_bit = ((datt>>2) & 1);     //   //shift byte to get the required PWM duty cycle
                       CCPR2L = (datt);                 //
                           //RIGHT Channel              //
                           datt = SPI(0xFF);            //
                           CCP1X_bit = ((datt>>2) & 1); //       //for stereo
                           CCPR1L = (datt);             //
                                                        //
                       TMR2IF_bit = 0;                  //
                      ////////////////////////////////////
                          }
              }
                SPI(0xFF);        //discard of CRC
                SPI(0xFF);
                loc += 512;

                                    ////////////////////////////////////////
                                         if(loc >= BPB_FileSize)          //  to calculate the file size and repeat or next
                                       {                                  //
                                           i = 0;                         //
                                           forward();                     //
                                       }                                  //
                                    ////////////////////////////////////////
        }
          ////////////////
           MMC_Close(); //        close mmc
         /////////////////
}