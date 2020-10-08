#include <xc.h>
#include "LCD_I2C.h"

unsigned char RS, i2c_add, BackLight_State = LCD_BACKLIGHT;

//---------------[ I2C Rutina ]-------------------
//--------------------------------------------------

void I2C_Master_Init()
{
  SSPCON1 = 0b00101000;
  SSPSTAT = 0;
  SSPADD = ((_XTAL_FREQ / 4) / I2C_BaudRate) - 1;
  SCL_D = 1;
  SDA_D = 1;
}

void I2C_Master_Wait()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F))
    ;
}

void I2C_Master_Start()
{
  I2C_Master_Wait();
  SEN = 1;
}

void I2C_Master_RepeatedStart()
{
  I2C_Master_Wait();
  RSEN = 1;
}

void I2C_Master_Stop()
{
  I2C_Master_Wait();
  PEN = 1;
}

void I2C_ACK(void)
{
  ACKDT = 0; // 0 -> ACK
  I2C_Master_Wait();
  ACKEN = 1; // Send ACK
}

void I2C_NACK(void)
{
  ACKDT = 1; // 1 -> NACK
  I2C_Master_Wait();
  ACKEN = 1; // Send NACK
}

unsigned char I2C_Master_Write(unsigned char data)
{
  I2C_Master_Wait();
  SSPBUF = data;
  while (!SSPIF)
    ;
  SSPIF = 0;
  return ACKSTAT;
}

unsigned char I2C_Read_Byte(void)
{
  //---[ Recibe y regresa un Byte]---
  I2C_Master_Wait();
  RCEN = 1;
  while (!SSPIF)
    ;
  SSPIF = 0;
  I2C_Master_Wait();
  return SSPBUF;
}
//======================================================

//---------------[ LCD Routines ]----------------
//-----------------------------------------------

void LCD_Init(unsigned char I2C_Add)
{
  i2c_add = I2C_Add;
  IO_Expander_Write(0x00);
  __delay_ms(30);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(LCD_RETURN_HOME);
  __delay_ms(5);
  LCD_CMD(0x20 | (LCD_TYPE << 2));
  __delay_ms(50);
  LCD_CMD(LCD_TURN_ON);
  __delay_ms(50);
  LCD_CMD(LCD_CLEAR);
  __delay_ms(50);
  LCD_CMD(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
  __delay_ms(50);
}
void IO_Expander_Write(unsigned char Data)
{
  I2C_Master_Start();
  I2C_Master_Write(i2c_add);
  I2C_Master_Write(Data | BackLight_State);
  I2C_Master_Stop();
}
void LCD_Write_4Bit(unsigned char Nibble)
{

  Nibble |= RS;
  IO_Expander_Write(Nibble | 0x04);
  IO_Expander_Write(Nibble & 0xFB);
  __delay_us(50);
}

void LCD_CMD(unsigned char CMD)
{
  RS = 0;
  LCD_Write_4Bit(CMD & 0xF0);
  LCD_Write_4Bit((CMD << 4) & 0xF0);
}

void LCD_Write_Char(char Data)
{
  RS = 1;
  LCD_Write_4Bit(Data & 0xF0);
  LCD_Write_4Bit((Data << 4) & 0xF0);
}

void LCD_Write_String(char *Str)
{
  for (int i = 0; Str[i] != '\0'; i++)
    LCD_Write_Char(Str[i]);
}

void LCD_Set_Cursor(unsigned char COL, unsigned char ROW)
{
  switch (ROW)
  {
  case 1:
    LCD_CMD(0xC0 + COL);
    break;
  case 2:
    LCD_CMD(0x94 + COL);
    break;
  case 3:
    LCD_CMD(0xD4 + COL);
    break;
  // Case 1
  default:
    LCD_CMD(0x80 + COL);
  }
}

void Backlight()
{
  BackLight_State = LCD_BACKLIGHT;
  IO_Expander_Write(0);
}

void noBacklight()
{
  BackLight_State = LCD_NOBACKLIGHT;
  IO_Expander_Write(0);
}

void LCD_SL()
{
  LCD_CMD(0x18);
  __delay_us(40);
}

void LCD_SR()
{
  LCD_CMD(0x1C);
  __delay_us(40);
}

void LCD_Clear(void)
{
  LCD_CMD(0x01);
  __delay_us(40);
}
void LCD_Custom_Char(unsigned char loc, unsigned char *msg)
{
  unsigned char i;
  if (loc < 8)
  {
    LCD_CMD(0x40 + (loc * 8));
    for (i = 0; i < 8; i++)
      LCD_Write_Char(msg[i]);
  }
}