/*
 * File:   newmain.c
 * Author: death
 *
 * Created on 3 de octubre de 2020, 09:59 PM
 */


#include "conf.h"

int led_run[8]={1,2,4,8,16,32,64,128};

void main(void) {
    ADCON1bits.PCFG = 0b1111;
    OSCCONbits.IRCF = 0b111; //Configura oscilador interno (FOSC = 8Mhz)
    OSCCONbits.SCS = 0b10; // Oscilador del sistema = Fosc interno
    TRISB = 0x0;
    while(1){
        for(int i = 0;i<8;i++ ){
            LATB = led_run[i];
            __delay_ms(500);
        }
        LATB = 0;
        __delay_ms(500);
        for(int i = 0;i<10;i++ ){
            LATB = 255;
            __delay_ms(500);
            LATB = 0;
            __delay_ms(500);
        }
    }
}
