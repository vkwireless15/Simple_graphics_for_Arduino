#include "Graphics_IO.h"
#include "SimpleGraphics.h"


//Пользовательские include and define
#include <Arduino.h>

#define WR  2  //PG2
#define RS  7  //PD7
#define RST 0  //PG0
#define CS  1  //PG1

//

extern uint32 DispHeight, DispWidth;
uint32 LCD_FRAME_BUFFER0 = 0;
uint32 LCD_FRAME_BUFFER1 = 0;
uint32 LCD_FRAME_BUFFER3 = 0;

//Пользовательские переменные и т.п.

void LCD_Write_COM(byte cmd)
{
  PORTD &= ~(1 << RS);
  PORTA = 0;
  PORTC = cmd;
  PORTG &= ~(1 << WR);
  PORTG |= (1 << WR);
  PORTD |= (1 << RS);
}

void LCD_Write_DATA(byte dat)
{
  PORTA = 0;
  PORTC = dat;
  PORTG &= ~(1 << WR);
  PORTG |= (1 << WR);
}

void LCD_Write_16bit(unsigned int dat)
{
  PORTA = 0;
  PORTC = 0;
  PORTG &= ~(1 << WR);
  PORTG |= (1 << WR);
}

//

void Init_Graphics_System(uint32 LCD_RAM_START_ADDRESS, uint8 Layers, uint8 ColorType) //Инициализация драйвера дисплея, графического ускорителя и т.п.
{
    LCD_FRAME_BUFFER0 = LCD_RAM_START_ADDRESS;
    //Пользовательский код инициализации дисплея
	DDRC = 0xFF;
	DDRA = 0xFF;
	DDRD |= ( 1 << 7);
	DDRG |= ( 1 << 0) | ( 1 << 1) | ( 1 << 2);
	
	Serial.begin(9600);
	
	pinMode(RST,OUTPUT);
	pinMode(CS,OUTPUT);
	pinMode(WR,OUTPUT);
	pinMode(RS,OUTPUT);
	
    delay(200);
	
	PORTG |= 0x01;  //RST 1
	delay(1);
	PORTG &= ~0x01; //RST 0
	delay(10);
	PORTG |= 0x01;  //RST 1
	
	delay(50);
	PORTG &= ~0x02; //CS 0
	
	LCD_Write_COM(0x11);		// Sleep OUT
	delay(50);
	
	LCD_Write_COM(0x13);
	
	LCD_Write_COM(0xF2);		// ?????
	LCD_Write_DATA(0x1C);
	LCD_Write_DATA(0xA3);
	LCD_Write_DATA(0x32);
	LCD_Write_DATA(0x02);
	LCD_Write_DATA(0xb2);
	LCD_Write_DATA(0x12);
	LCD_Write_DATA(0xFF);
	LCD_Write_DATA(0x12);
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0xF1);		// ?????
	LCD_Write_DATA(0x36);
	LCD_Write_DATA(0xA4);

	LCD_Write_COM(0xF8);		// ?????
	LCD_Write_DATA(0x21);
	LCD_Write_DATA(0x04);

	LCD_Write_COM(0xF9);		// ?????
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x08);
	
	LCD_Write_COM(0xB1);		// Frame rate control
	LCD_Write_DATA(0xC0);
	LCD_Write_DATA(0x00);
	
	LCD_Write_COM(0xB4);		// Display inversion control
	LCD_Write_DATA(0x01);

	LCD_Write_COM(0xC0);		// Power Control 1
	LCD_Write_DATA(0x0d);
	LCD_Write_DATA(0x0d);

	LCD_Write_COM(0xC1);		// Power Control 2
	LCD_Write_DATA(0x43);
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0xC2);		// Power Control 3
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0xC5);		// VCOM Control
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x48);

	LCD_Write_COM(0xB6);		// Display Function Control
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x22);		// 0x42 = Rotate display 180 deg.
	LCD_Write_DATA(0x3B);

	LCD_Write_COM(0xE0);		// PGAMCTRL (Positive Gamma Control)
	LCD_Write_DATA(0x0f);
	LCD_Write_DATA(0x24);
	LCD_Write_DATA(0x1c);
	LCD_Write_DATA(0x0a);
	LCD_Write_DATA(0x0f);
	LCD_Write_DATA(0x08);
	LCD_Write_DATA(0x43);
	LCD_Write_DATA(0x88);
	LCD_Write_DATA(0x32);
	LCD_Write_DATA(0x0f);
	LCD_Write_DATA(0x10);
	LCD_Write_DATA(0x06);
	LCD_Write_DATA(0x0f);
	LCD_Write_DATA(0x07);
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0xE1);		// NGAMCTRL (Negative Gamma Control)
	LCD_Write_DATA(0x0F);
	LCD_Write_DATA(0x38);
	LCD_Write_DATA(0x30);
	LCD_Write_DATA(0x09);
	LCD_Write_DATA(0x0f);
	LCD_Write_DATA(0x0f);
	LCD_Write_DATA(0x4e);
	LCD_Write_DATA(0x77);
	LCD_Write_DATA(0x3c);
	LCD_Write_DATA(0x07);
	LCD_Write_DATA(0x10);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x23);
	LCD_Write_DATA(0x1b);
	LCD_Write_DATA(0x00); 

	LCD_Write_COM(0x20);		// Display Inversion OFF
	LCD_Write_DATA(0x00);//C8 	 

	LCD_Write_COM(0x36);		// Memory Access Control
	LCD_Write_DATA(0x68);

	LCD_Write_COM(0x3A);		// Interface Pixel Format
	LCD_Write_DATA(0x55); 

	LCD_Write_COM(0x2A);		// Column Addess Set
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x01);
	LCD_Write_DATA(0xDF);

	LCD_Write_COM(0x002B);		// Page Address Set
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x01);
	LCD_Write_DATA(0x3f);	 
	delay(50);
	LCD_Write_COM(0x0029);		// Display ON
	LCD_Write_COM(0x002C);		// Memory Write
	
	
	
	//
}

void Fill_all(uint16 Color) //Заливка всего дисплея цветом
{
    int x1 = 0, x2 = DispWidth - 1, y1 = 0, y2 = DispHeight - 1;
	
	LCD_Write_COM(0x2a); 
	LCD_Write_DATA(x1>>8);
	LCD_Write_DATA(x1);
	LCD_Write_DATA(x2>>8);
	LCD_Write_DATA(x2);
	LCD_Write_COM(0x2b); 
	LCD_Write_DATA(y1>>8);
	LCD_Write_DATA(y1);
	LCD_Write_DATA(y2>>8);
	LCD_Write_DATA(y2);
	LCD_Write_COM(0x2c); 
	
	PORTA = Color >> 8;
    PORTC = Color & 0xff;
	
	for(long i=0; i < 153600; i++)
	{
	    PORTG &= ~(1 << WR);
        PORTG |= (1 << WR);
	}
}

void Fill_rectangle(uint32 Color, int32 StartX, int32 StopX, int32 StartY, int32 StopY) //Заливка прямоугольной формы
{
    int x1 = StartX - 1, x2 = StopX - 1, y1 = StartY - 1, y2 = StopY - 1;
	long pix = (StopX - x1) * (StopY - y1);	
		
	if(pix != 0)
	{
	  LCD_Write_COM(0x2a); 
	  LCD_Write_DATA(x1>>8);
	  LCD_Write_DATA(x1);
	  LCD_Write_DATA(x2>>8);
	  LCD_Write_DATA(x2);
	  LCD_Write_COM(0x2b); 
	  LCD_Write_DATA(y1>>8);
	  LCD_Write_DATA(y1);
	  LCD_Write_DATA(y2>>8);
	  LCD_Write_DATA(y2);
	  LCD_Write_COM(0x2c); 
	
	  PORTA = Color >> 8;
      PORTC = Color & 0xff;
	  
	  for(long i=0; i < pix; i++)
	  {
	     PORTG &= ~(1 << WR);
         PORTG |= (1 << WR);
	  }
	}
}

void MemPoint(int32 x, int32 y, int32 Color) //Отрисовка одного пикселя
{
	  LCD_Write_COM(0x2a); 
	  LCD_Write_DATA((x)>>8);
	  LCD_Write_DATA(x);
	  LCD_Write_DATA(x>>8);
	  LCD_Write_DATA(x);
	  LCD_Write_COM(0x2b); 
	  LCD_Write_DATA((y)>>8);
	  LCD_Write_DATA(y);
	  LCD_Write_DATA(y>>8);
	  LCD_Write_DATA(y);
	  LCD_Write_COM(0x2c); 
	
	  PORTA = Color >> 8;
      PORTC = Color & 0xff;
	  
	  PORTG &= ~(1 << WR);
      PORTG |= (1 << WR);
}

void Backlight(uint32 Brightness, uint8 State) //Управление подсветкой
{
	if(State == LOW)
	{
		
	}
	else
	{
		
	}
}

uint8 GetCursorPosition()
{
}
