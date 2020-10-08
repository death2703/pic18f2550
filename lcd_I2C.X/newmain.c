/*
 * File:   newmain.c
 * Author: death
 *
 * Created on 8 de octubre de 2020, 02:27 PM
*/

#include "conf.h"
#include "LCD_I2C.h"

void main(void)
{
    I2C_Master_Init();
    LCD_Init(0x4E);
    ADCON1bits.PCFG = 0b1111; //Todos los pines digitales
    OSCCONbits.IRCF = 0b111;  //Configura oscilador interno (FOSC = 8Mhz)
    OSCCONbits.SCS = 0b10;    // Oscilador del sistema = Fosc interno
    TRISB = 0xff;             //Puerto B digital
    LCD_Set_Cursor(0, 0);
    LCD_Write_String("PROTON FROM HELL");
    LCD_Set_Cursor(0, 1);
    LCD_Write_String("LIKE & FOLLOW");
    __delay_ms(3000);
    LCD_CMD(LCD_CLEAR);
    while (1)
    {
        LCD_Set_Cursor(0, 0);
        LCD_Write_String("LCD I2C CON");
        LCD_Set_Cursor(0, 1);
        LCD_Write_String("PIC18f2550");
    }
}
