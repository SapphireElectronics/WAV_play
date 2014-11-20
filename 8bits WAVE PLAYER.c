  /* author: Mustapha Rahhou
   8bits wave player v 2.0.0.1
   using Easy Hi-Q converter
   Compiler:mC pro v4.5.0
   MCU:PIC16F877A + MMC card up to 2 GB Fat16
   you can use this library for any sound project
   included one song for test
   The library supports LFN .
   Email:mus.me@hotmail.com
   date:28 jan 2011

   Best Regards   */



 #include "PLAYER.c"
 
  void sleep(){
    PORTD.F2=1;
    delay_ms(3000);
    PORTD = 0;
   MMC_Close();
   PWM1_Stop();

   delay_ms(10);


   {
if(INTCON.INTF == 1)   // if the RB0/INT External Interrupt flag is set...
  {
    INTCON.INTF =0;
  }
  }
INTCON.INTE = 1;    // bit 4 RB0/INT External Interrupt Enable
INTCON.GIE = 1;     // global interrput enable
    asm sleep;
    asm nop;
    asm nop;
    delay_ms(3000);
    PORTD.F3=1;
    delay_ms(3000);
   asm CLRF PCLATH ;
   asm GOTO 0x0000;
    }

void main()
{
    long PWR_OFF;
  loc=512;
  jump=0;
  t_LED = 0;
  errorLED = 0;
     ADCON0=0;
     ADCON1=0x07;

    TRISC=0b00010000;
    TRISB = 0b00001111;
    TRISD=0x00;
    TRISE=0x00;
    TRISA=0b00000001;
    PORTA=0x00;
    PORTD=0x00;
    OPTION_REG.F7=1;
        CS=1;
        delay_ms(100);
        CCP1CON = 0B00001100;        //PWM mode
        CCP2CON = 0B00001100;        //PWM mode
        PR2 = 227;
        T2CON = 0B100;        //prescale 1, post scale 1, timer2 on


           delay_ms(100);
    intspi();       // initialize spi

          delay_ms(200);
    MMC_Init();     // inisialize sd/mmc
          delay_ms(200);
    FAT_init();        //inisialize fat
          delay_ms(200);
    initroot();   // inisializ root derictory

    ReadSD();  // auto play when you switch on the player
//**********************************************************************

while(1)
 {

     if(!next)
     {
       jump+=32;
       initroot();
       ReadSD();
     }

        if(!Play ) ReadSD();

         if(!Stop )
         { jump=0;
           initroot();
         }

      while(!PwrOff_Rep)
      {
         PWR_OFF++;


     if(PWR_OFF >= 300000)  // long press to power off  and press to power on
     {
          PWR_OFF=0;
           sleep();
     }
     }
 }
}