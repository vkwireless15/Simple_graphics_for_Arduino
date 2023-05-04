#include "SimpleGraphics.h"
#include "Graphics_IO.h"
#include "Font.h"



 uint32 DispHeight = 0, DispWidth = 0;
 uint8 ColorType = Color565;
 uint16 CrPosX1 = 0, CrPosX2 = 0, CrPosX3 = 0, CrPosX4 = 0, CrPosX5 = 0, CrPosY1 = 0, CrPosY2 = 0, CrPosY3 = 0, CrPosY4 = 0, CrPosY5 = 0;
 
 uint8 Current_Layer;
	
 uint32 WindowFrameColor;
 uint32 WindowBarColor;
 uint32 WindowBarHColor;
 uint32 WindowBarLColor;
 uint32 WindowColor;
 uint32 WindowTextColor;
 uint8 WindowFrameThickness; 
	
 uint32 ButtonFrameColor;
 uint32 ButtonColor;
 uint32 ButtonSelectedColor;
 uint32 ButtonTextColor;
	
 uint32 LabelColor;
 uint32 LabelSelectedColor;
 uint32 LabelBackgroundColor;
	
 uint32 BarColor;
 uint32 BarColor2;
 uint32 BarColor3;
   
 //User Fn
   
   
 //
 
uint32 Color_A888_(uint32 Color, uint8 Alpha) //преобразует отдельные значения цвета  и прозрачности в формат А888(альфа канал + 24 бита цвета)
{
	return Color | Alpha << 24;
}
uint16 Get565Color(uint32 Color) //Возвращает значение цвета в формате пикселя 565(5 бит - красный цвет, 6 - зеленый, 5 - синий)
{
   uint16 RezColor = 0;
   uint8 R, G, B;
   R = (Color >> 16) & 0xff;
   G = (Color >> 8) & 0xff;
   B = Color & 0xff;

   R = 31 * R / 255;
   G = 63 * G / 255;
   B = 31 * B / 255;

   RezColor |= B;
   RezColor |= (G << 5);
   RezColor |= (R << 11);

   return RezColor;
}
uint32 FontHParameter() //Возвращает значение высоты шрифта
{
	uint8 FontH = calibri[1];
	return FontH;
}
void Set_Backlight(uint8 State, uint32 Brightness) //Управление подсветкой и ее яркостью(не поддерживается упр. яркостью для платы STM32F746Disco)
{
	Backlight(Brightness, State);
}
uint8 is_Disp_Res(int32 x1, int32 x2, int32 y1, int32 y2) //Проверка элемента на выход за пределы дисплея
{
	if((x1 > 0)&&(x2 <= DispWidth))
	{
		if((y1 > 0)&&(y2 <= DispHeight))
		{
			return Ok;
		}
	}
	return Error;
}
void SymbolParameters(uint16 X, uint16 Y, uint16 *NextX, uint16 *NextY, uint8 *CharWt, uint8 *CharHt, char Symbol) //Параметры положения текущего символа и положения следующих возможных.
{
	uint8 FontH = FontHParameter();
	uint8 FirstChar = calibri[2];
	uint8 Cursor = Symbol - FirstChar;
	uint8 CharW = calibri[Cursor + 4];

	*NextX = X + CharW;
	*NextY = Y + FontH;
	*CharWt = CharW;
	*CharHt = FontH;
}
uint32 SymbolLengthPixels(char String[]) //Длина строки в пикселях
{
	uint8 FirstChar = calibri[2];
	uint8 Cursor = 0;
	uint8 CharW = 0;
	uint32 Len = 0;
	uint32 SymCh = 0;

	while(String[SymCh] != 0)
	{
		Cursor = String[SymCh] - FirstChar;
		CharW = calibri[Cursor + 4];
		Len += CharW;
		SymCh++;
	}
	return Len;
}
uint8 RU_EN_UTF16ToASCII(char S1, char S2)
{
	char S = S2 - 16;

	if(S1 == 0xD1 && S2 == 145)
	{
		return 184;
	}

	if(S1 == 0xD0 && S2 == 129)
	{
		return 168;
	}

	if(S <= 175 && S>= 128)
	{
	    return S2 + 48;
    }
	else
	{
		return S + 128;
	}
	return Error;
}
void Get1251(char Str[])
{
	 uint32 W1 = 0, cnt = 0;
	 while(Str[W1] != 0)
	 { W1++ ;}

	 for(uint32	s = 0; s < W1;)
	 {
		 if(Str[s] == 0xD0 || Str[s] == 0xD1)
		 {
			 Str[cnt] = RU_EN_UTF16ToASCII(Str[s], Str[s+1]);
			 if(s!=cnt)
			 {
				 Str[s] = 0;
				 Str[s + 1] = 0;
			 }
			 cnt++;
			 s+=2;
		 }
		 else
		 {
			 Str[cnt] = Str[s];
			 if(s!=cnt)
			 {
				 Str[s] = 0;
			 }
			 cnt++;
			 s++;
		 }
	 }
}
uint8 Inverse(uint8 S)
{
	uint8 K = 0;
	for(uint8 i = 0; i<8; i++)
	{
		if((S & 0x01) != 0)
		{
			K = K | 0x01;
		}
		S = S >> 1;
		if(i<7)
		{
		  K = K << 1;
	    }
	}
	return K;
}   
   
   
   
void Graphics_Init(DisplayConfig *dcf) //Инициализация самой бибиллиотеки а также инициализация графических устройств(дисплей, графические ускорители, тач-панели)
{ 
	DispHeight = dcf->Display_Height;
	DispWidth = dcf->Display_Width;
	ColorType = dcf->Color_Type;
	
	WindowFrameColor = dcf->Window_Frame_Color;
    WindowBarColor = dcf->Window_Bar_Color;
    WindowBarHColor = dcf->Window_Bar_H_Color;
    WindowBarLColor = dcf->Window_Bar_L_Color;
    WindowColor = dcf->Window_Color;
    WindowTextColor = dcf->Window_Text_Color;
    WindowFrameThickness = dcf->Window_Frame_Thickness;
	
    ButtonFrameColor = dcf->Button_Frame_Color;
    ButtonColor = dcf->Button_Color;
    ButtonSelectedColor = dcf->Button_Selected_Color;
    ButtonTextColor = dcf->Button_Text_Color;
	
    LabelColor = dcf->Label_Color;
    LabelSelectedColor = dcf->Label_Selected_Color;
    LabelBackgroundColor = dcf->Label_Background_Color;
	
	BarColor = dcf-> Bar_Color;
    BarColor2 = dcf-> Bar_Color2;
    BarColor3 = dcf-> Bar_Color3;
	
	Init_Graphics_System(dcf->Start_RAM_Address, dcf->Layers, dcf->Color_Type);
}
void Fill_Display(uint32 Color) //Заливает весь дисплей определенным цветом
{

	if(ColorType == Color565)
	{ Fill_all(Get565Color(Color)); }

	if(ColorType == Color888)
	{ Fill_all(Color | 0xFF << 24); }

	if(ColorType == Color_A888)
	{ Fill_all(Color); }
}
void Pixel(uint16 x, uint16 y, uint32 Color, uint16 Thickness)
{
  uint16 i, j, tolsx, tolsy;
  x--; y--;
  tolsy = y+Thickness;
  tolsx = x+Thickness;
  for(i = y; i<tolsy; i++)
  {
	  for(j = x; j<tolsx; j++)
	  {
		  if(i < DispHeight && j < DispWidth)
		  {
			  if(ColorType == Color565)
			  { MemPoint(j,i,Get565Color(Color)); }

			  if(ColorType == Color888)
			  { MemPoint(j,i,Color | 0xFF << 24); }

			  if(ColorType == Color_A888)
			  { MemPoint(j,i,Color); }
	      }
	  }
  }
}
void Fill_Rectangle(uint32 Color, int32 StartX, int32 StopX, int32 StartY, int32 StopY) //заливает цветом прямоугольную область
{
	if(((StartX > 0) && (StartX <= DispWidth)) || ((StopX > 0) && (StopX <= DispWidth)))
	{
		if(((StartY > 0) && (StartY <= DispHeight)) || ((StopY > 0) && (StopY <= DispHeight)))
		{
			if(StartX <= 0)
			{ StartX = 1;}
			if(StopX > DispWidth)
			{ StartX = DispWidth;}

			if(StartY <= 0)
			{ StartY = 1;}
			if(StopY > DispHeight)
			{ StartY = DispHeight;}

			if(ColorType == Color565)
			{ Fill_rectangle(Get565Color(Color), StartX, StopX, StartY, StopY); }

			if(ColorType == Color888)
			{ Fill_rectangle(Color | 0xFF << 24, StartX, StopX, StartY, StopY); }

			if(ColorType == Color_A888)
			{ Fill_rectangle(Color | 0xFF << 24, StartX, StopX, StartY, StopY); }
		}
	}
}
void HLine(uint32 Color, int16 x1, int16 x2, int16 y1, uint8 Thickness)//
{
    if(Thickness > 0)
    {
    	Thickness--;
    	Fill_Rectangle(Color, x1, x2, y1, y1+Thickness);
    }
}
void VLine(uint32 Color, int16 x1, int16 y1, int16 y2, uint8 Thickness)
{
	if(Thickness > 0)
	{
		Thickness--;
		Fill_Rectangle(Color, x1, x1+Thickness, y1, y2);
	}
}
void FramePanel(uint32 BorderColor, uint32 FloodColor, int16 x1, int16 x2, int16 y1, int16 y2, uint8 Thickness)
{
	Thickness --;
	Fill_Rectangle(FloodColor,x1,x2,y1,y2);
    HLine(BorderColor,x1,x2,y1,Thickness + 1);
    HLine(BorderColor,x1,x2,y2 - Thickness,Thickness +1);
    VLine(BorderColor,x1,y1,y2,Thickness + 1);
    VLine(BorderColor,x2 - Thickness,y1,y2,Thickness + 1);
}
void Line(int16 x1, int16 y1, int16 x2, int16 y2, uint32 Color, uint16 Thickness)//++
{
   uint16 y = 0, k = 0;
   if(x1 <= x2)
   {
	   if(y1 <= y2)
	   {
		   if(x2 - x1 != 0)
		   {
		       k = (y2 - y1)/(x2 - x1);
		       if((y2 - y1)%(x2 - x1) != 0)
		       {
		    	   if(Thickness == 1)
		    	   {
		    	       k++;
		           }
		       }
		   }
		   else
		   {
			   k = 1;
			   x2++;
		   }

		   if(k == 0)
		   { k = 1; }

		   for(uint16 xp = x1; xp <= x2; xp++)
		   {
			   y = (xp - x1) * (y2 - y1) / (x2 - x1) + y1;
			   for(uint16 yp = 0; yp < k; yp++)
			   {
				   Pixel(xp, y, Color, Thickness);
				   y++;
				   if(y > DispHeight)
				   {break; }
			   }
		   }
	   }
	   else
	   {
		   if(x2 - x1 != 0)
		   {
		       k = (y1 - y2)/(x2 - x1);
		       if((y1 - y2)%(x2 - x1) != 0)
		       {
		    	   if(Thickness == 1)
	    	       {
	    	           k++;
	               }
		       }
		   }
		   else
		   {
			   k = 1;
			   x2++;
		   }

		   if(k == 0)
		   { k = 1; }

		   for(uint16 xp = x1; xp <= x2; xp++)
		   {
			   y = (xp - x1) * (y2 - y1) / (x2 - x1) + y1;
			   for(uint16 yp = 0; yp < k; yp++)
			   {
				   Pixel(xp, y, Color, Thickness);
				   y--;
				   if(y > DispHeight)
				   {break; }
			   }
		   }
	   }
   }
   else
   {
	   if(y1 <= y2)
	   {
		   if(x1 - x2 != 0)
		   {
		       k = (y2 - y1)/(x1 - x2);
		       if((y2 - y1)%(x1 - x2) != 0)
		       {
		    	   if(Thickness == 1)
		    	   {
		    	       k++;
		           }
               }
		   }
		   else
		   {
			   k = 1;
			   x1++;
		   }

		   if(k == 0)
		   { k = 1; }

		   for(uint16 xp = x2; xp <= x1; xp++)
		   {
			   y = (xp - x1) * (y2 - y1) / (x2 - x1) + y1;
			   for(uint16 yp = 0; yp < k; yp++)
			   {
				   Pixel(xp, y, Color, Thickness);
				   y++;
				   if(y > DispHeight)
				   {break; }
			   }
		   }
	   }
	   else
	   {
		   if(x2 - x1 != 0)
		   {
		       k = (y1 - y2)/(x2 - x1);
		       if((y1 - y2)%(x2 - x1) != 0)
		       {
		    	   if(Thickness == 1)
	    	       {
	    	           k++;
	               }
		       }
		   }
		   else
		   {
			   k = 1;
			   x2++;
		   }

		   if(k == 0)
		   { k = 1; }

		   for(uint16 xp = x2; xp <= x1; xp++)
		   {
			   y = (xp - x1) * (y2 - y1) / (x2 - x1) + y1;
			   for(uint16 yp = 0; yp < k; yp++)
			   {
				   Pixel(xp, y, Color, Thickness);
				   y++;
				   if(y > DispHeight)
				   {break; }
			   }
		   }
	   }
   }
}
void Circle (int16 x1, int16 y1, int16 r,uint32 Color, uint16 Thickness)
{
    int x = -r, y = 0, err = 2-2*r, e2;
	do {
		Pixel(x1-x, y1+y,Color,Thickness);
		Pixel(x1+x, y1+y,Color,Thickness);
		Pixel(x1+x, y1-y,Color,Thickness);
		Pixel(x1-x, y1-y,Color,Thickness);
		e2 = err;
		if (e2 <= y) {
			err += ++y*2+1;
			if (-x == y && e2 <= x) e2 = 0;
		}
		if (e2 > x) err += ++x*2+1;
	} while (x <= 0);
}
void FillCircle(uint16 x1, uint16 y1, uint16 r, uint32 Color)//
{
  uint16 r1 = r;
  while(r1 != 0)
  {
      Circle(x1,y1,r1,Color,2);
      r1--;
  }
}
void VGradA(int16 x1, int16 x2, int16 y1, int16 y2, int32 ColorH, int32 ColorL)//
{
    float HRed, HGreen, HBlue;
    float LRed, LGreen, LBlue;
    y1-=1;
    x1-=1;

	if(ColorType == Color565)
	{ ColorH = Get565Color(ColorH); ColorL = Get565Color(ColorL);}

	if(ColorType == Color888)
	{ ColorH = ColorH | 0xFF << 24; ColorL = ColorL | 0xFF << 24; }

	if(x1 < 1){x1 = 1;}
	if(x2 > DispWidth){ x2 = DispWidth;}
	if(y1 < 1){ y1 = 1;}
	if(y2 > DispHeight){ y2 = DispHeight;}

    if(ColorType != Color565)
    {
       HBlue = ColorH & 0x000000ff;
       ColorH = ColorH>>8;
       HGreen = ColorH & 0x000000ff;
       ColorH = ColorH>>8;
       HRed = ColorH & 0x000000ff;

       LBlue = ColorL & 0x000000ff;
       ColorL = ColorL>>8;
       LGreen = ColorL & 0x000000ff;
       ColorL = ColorL>>8;
       LRed = ColorL & 0x000000ff;
    }
    else
    {
        HBlue = ColorH & 0x0000001f;
        ColorH = ColorH>>5;
        HGreen = ColorH & 0x0000003f;
        ColorH = ColorH>>6;
        HRed = ColorH & 0x0000001f;

        LBlue = ColorL & 0x0000001f;
        ColorL = ColorL>>5;
        LGreen = ColorL & 0x0000003f;
        ColorL = ColorL>>6;
        LRed = ColorL & 0x0000001f;
    }

    uint32 Res_Color, promej_zn;
    float Mix;

    for(int16 i = y1; i<y2; i++)
    {
     Mix = (float)(i - y1) / (float)(y2 - y1);

     if(ColorType != Color565)
     {
        Res_Color = HRed * (1 - Mix) + LRed * (Mix);
        Res_Color <<= 8;

        promej_zn = HGreen * (1 - Mix) + LGreen * (Mix);
        Res_Color |= promej_zn;
        Res_Color <<= 8;
        promej_zn = HBlue * (1 - Mix) + LBlue * (Mix);
        Res_Color |= promej_zn;
     }
     else
     {
         Res_Color = HRed * (1 - Mix) + LRed * (Mix);
         Res_Color <<= 6;
         promej_zn = HGreen * (1 - Mix) + LGreen * (Mix);
         Res_Color |= promej_zn;
         Res_Color <<= 5;
         promej_zn = HBlue * (1 - Mix) + LBlue * (Mix);
         Res_Color |= promej_zn;
     }

     for(int16 j = x1; j<x2; j++)
     {
          MemPoint(j,i,Res_Color);
     }
   }
}
void HGradA(int16 x1, int16 x2, int16 y1, int16 y2, int32 ColorH, int32 ColorL)//++
{
    float HRed, HGreen, HBlue;
    float LRed, LGreen, LBlue;
    y1-=1;
    x1-=1;

	if(ColorType == Color565)
	{ ColorH = Get565Color(ColorH); ColorL = Get565Color(ColorL);}

	if(ColorType == Color888)
	{ ColorH = ColorH | 0xFF << 24; ColorL = ColorL | 0xFF << 24; }

	if(x1 < 1){x1 = 1;}
	if(x2 > DispWidth){ x2 = DispWidth;}
	if(y1 < 1){ y1 = 1;}
	if(y2 > DispHeight){ y2 = DispHeight;}

    if(ColorType != Color565)
    {
       HBlue = ColorH & 0x000000ff;
       ColorH = ColorH>>8;
       HGreen = ColorH & 0x000000ff;
       ColorH = ColorH>>8;
       HRed = ColorH & 0x000000ff;

       LBlue = ColorL & 0x000000ff;
       ColorL = ColorL>>8;
       LGreen = ColorL & 0x000000ff;
       ColorL = ColorL>>8;
       LRed = ColorL & 0x000000ff;
    }
    else
    {
        HBlue = ColorH & 0x0000001f;
        ColorH = ColorH>>5;
        HGreen = ColorH & 0x0000003f;
        ColorH = ColorH>>6;
        HRed = ColorH & 0x0000001f;

        LBlue = ColorL & 0x0000001f;
        ColorL = ColorL>>5;
        LGreen = ColorL & 0x0000003f;
        ColorL = ColorL>>6;
        LRed = ColorL & 0x0000001f;
    }

    uint32 Res_Color, promej_zn;
    float Mix;

    for(uint16 i = x1; i<x2; i++)
    {
     if(i > x2 - 1)
     {i = DispWidth;}

     Mix = (float)(i - x1) / (float)(x2 - x1);

     if(ColorType != Color565)
     {
         Res_Color = HRed * (1 - Mix) + LRed * (Mix);
         Res_Color <<= 8;
         promej_zn = HGreen * (1 - Mix) + LGreen * (Mix);
         Res_Color |= promej_zn;
         Res_Color <<= 8;
         promej_zn = HBlue * (1 - Mix) + LBlue * (Mix);
         Res_Color |= promej_zn;
     }
     else
     {
         Res_Color = HRed * (1 - Mix) + LRed * (Mix);
         Res_Color <<= 6;
         promej_zn = HGreen * (1 - Mix) + LGreen * (Mix);
         Res_Color |= promej_zn;
         Res_Color <<= 5;
         promej_zn = HBlue * (1 - Mix) + LBlue * (Mix);
         Res_Color |= promej_zn;
     }


     for(int16 j = y1; j<y2; j++)
     {
         MemPoint(i,j,Res_Color);
     }
   }
}
void VGradB(int16 x1, int16 x2, int16 y1, int16 y2, uint32 ColorH, uint32 ColorC, uint32 ColorL)//++
{
 VGradA(x1,x2,y1,y1+((y2 - y1)/2),ColorH,ColorC);
 VGradA(x1,x2,y1+((y2 - y1)/2),y2,ColorC,ColorL);
}
void HGradB(int16 x1, int16 x2, int16 y1, int16 y2, uint32 ColorH, uint32 ColorC, uint32 ColorL)//++
{
 HGradA(x1,x1+((x2 - x1)/2),y1,y2,ColorH,ColorC);
 HGradA(x1+((x2 - x1)/2),x2,y1,y2,ColorC,ColorL);
}
void Progress_bar(uint16 XStart, uint16 XEnd, uint16 YStart, uint16 YEnd, uint16 StartPos, uint16 StopPos, uint16 CurrPos, uint32 BorderColor, uint32 FloodColor, uint32 BarColor, uint16 Thickness, uint8 Orient)
{
  uint16 Tr = Thickness - 1;
  float STPOS;
  float POSCOUNT;
  uint16 XPOS = 0, YPOS = 0;
  uint16 POSN = CurrPos - StartPos;

  if(Orient == Horisontal)
  {
      if(CurrPos == 1)
      {
         STPOS = StopPos - StartPos;
         POSCOUNT = (XEnd - XStart)/STPOS;
         XPOS = (uint16)((POSN - StartPos * POSCOUNT)+ XStart + 1 + Tr);
      }

      if(CurrPos == StartPos)
      {
         STPOS = StopPos - StartPos;
         POSCOUNT = (XEnd - XStart)/STPOS;
         XPOS = (uint16)((POSN * POSCOUNT)+ XStart + Tr);
      }

      if(CurrPos > StartPos)
      {
         STPOS = StopPos - StartPos;
         POSCOUNT = (XEnd - XStart)/STPOS;
         XPOS = (uint16)((POSN * POSCOUNT)+ XStart - Tr);
      }
  }
  else
  {
      STPOS = StopPos - StartPos;
      POSCOUNT = (YEnd - YStart)/STPOS;
      YPOS = (uint16)(YEnd - (POSN * POSCOUNT));
      YPOS += Thickness;
  }

  if(Orient == Horisontal)
  {
      FramePanel(BorderColor,FloodColor,XStart,XEnd,YStart, YEnd,Thickness);
      Fill_Rectangle(BarColor,XStart + 1 + Tr,XPOS-1,YStart + 1 + Tr,YEnd - Tr - 1);
  }
  else
  {
	  FramePanel(BorderColor,FloodColor,XStart,XEnd,YStart, YEnd,Thickness);
	  Fill_Rectangle(BarColor, XStart + 1 + Tr,XEnd - Tr - 1, YPOS, YEnd - Tr - 1);
  }
}
void Progress_bar_gradient(uint16 XStart, uint16 XEnd, uint16 YStart, uint16 YEnd, uint16 StartPos, uint16 StopPos, uint16 CurrPos, uint32 BorderColor, uint32 FloodColor, uint32 BarColorH, uint32 BarColorC, uint16 Thickness, uint8 Orient)//++
{
	  uint16 Tr = Thickness - 1;
	  float STPOS;
	  float POSCOUNT;
	  uint16 XPOS = 0, YPOS = 0;
	  uint16 POSN = CurrPos - StartPos;

	  if(Orient == Horisontal)
	  {
	      if(CurrPos == 1)
	      {
	         STPOS = StopPos - StartPos;
	         POSCOUNT = (XEnd - XStart)/STPOS;
	         XPOS = (uint16)((POSN - StartPos * POSCOUNT)+ XStart + 1 + Tr);
	      }

	      if(CurrPos == StartPos)
	      {
	         STPOS = StopPos - StartPos;
	         POSCOUNT = (XEnd - XStart)/STPOS;
	         XPOS = (uint16)((POSN * POSCOUNT)+ XStart + Tr);
	      }

	      if(CurrPos > StartPos)
	      {
	         STPOS = StopPos - StartPos;
	         POSCOUNT = (XEnd - XStart)/STPOS;
	         XPOS = (uint16)((POSN * POSCOUNT)+ XStart);
	      }
	  }
	  else
	  {
	      STPOS = StopPos - StartPos;
	      POSCOUNT = (YEnd - YStart)/STPOS;
	      YPOS = (uint16)(YEnd - (POSN * POSCOUNT));
	      YPOS += Thickness;
	  }

	  if(Orient == Horisontal)
	  {
	      FramePanel(BorderColor,FloodColor,XStart,XEnd,YStart, YEnd,Thickness);
	      VGradB(XStart + 1 + Tr,XPOS-1,YStart + 1 + Tr,YEnd - 1 - Tr,BarColorH,BarColorC,BarColorH);
	  }
	  else
	  {
		  FramePanel(BorderColor,FloodColor,XStart,XEnd,YStart, YEnd,Thickness);
		  HGradB(XStart + 1 + Tr, XEnd - Tr - 1, YPOS, YEnd - Tr - 1, BarColorH, BarColorC, BarColorH);
	  }
}
void Symbol(uint16 X, uint16 Y, uint32 Color, char Symbol)
{
	uint8 FontH = calibri[1];
	uint8 FirstChar = calibri[2];
	uint8 NumOfChar = calibri[3];
	uint8 Cursor = Symbol - FirstChar;
	uint16 SymbStartPos = 4 + NumOfChar;
	uint8 CharW = calibri[Cursor + 4];
	uint16 i, j, k = 0;
	uint32 CharBit = 0;

	for(i = 4; i < Cursor + 4; i++)
	{
	   	if(calibri[i] <= 8)
	   	{
	   	   SymbStartPos += FontH;
	   	}

	   	if(calibri[i] > 8 && calibri[i] <= 16)
		{
	   		SymbStartPos += FontH * 2;
		}

	   	if(calibri[i] > 16 && calibri[i] <= 24)
		{
	   		SymbStartPos += FontH * 3;
		}
	   	if(calibri[i] > 24 && calibri[i] <= 32)
		{
	   		SymbStartPos += FontH * 4;
		}
	}

	for(i = Y; i < Y + FontH; i++)
	{
		if(CharW <= 8)
		{
			CharBit = Inverse(calibri[SymbStartPos + k]);
			CharBit = CharBit << 24;
			k++;
		}
		if(CharW > 8 && CharW <= 16)
		{
			CharBit = Inverse(calibri[SymbStartPos + k]);
			k++;
			CharBit = CharBit << 8;
			CharBit = CharBit | Inverse(calibri[SymbStartPos + k]);
			CharBit = CharBit << 16;
			k++;
		}
		if(CharW > 16 && CharW <= 24)
		{
			CharBit = Inverse(calibri[SymbStartPos + k]);
			k++;
			CharBit = CharBit << 8;
			CharBit = CharBit | Inverse(calibri[SymbStartPos + k]);
			k++;
			CharBit = CharBit << 8;
			CharBit = CharBit | Inverse(calibri[SymbStartPos + k]);
			CharBit = CharBit << 8;
			k++;
		}
		if(CharW > 24 && CharW <= 32)
		{
			CharBit = Inverse(calibri[SymbStartPos + k]);
			k++;
			CharBit = CharBit << 8;
			CharBit = CharBit | Inverse(calibri[SymbStartPos + k]);
			k++;
			CharBit = CharBit << 8;
			CharBit = CharBit | Inverse(calibri[SymbStartPos + k]);
			k++;
			CharBit = CharBit << 8;
			CharBit = CharBit | Inverse(calibri[SymbStartPos + k]);
			k++;
		}


		for(j = X; j < X + CharW; j++)
		{
            if((CharBit & 0x80000000) != 0)
            {
      		    if(i < DispHeight && j < DispWidth)
      		    {
      			    if(ColorType == Color565)
      			    { MemPoint(j,i,Get565Color(Color)); }

      			    if(ColorType == Color888)
      			    { MemPoint(j,i,Color | 0xFF << 24); }

      			    if(ColorType == Color_A888)
      			    { MemPoint(j,i,Color); }
      	        }
            }
            CharBit = CharBit << 1;
		}
	}
}
void Label (uint16 X, uint16 Y, uint32 Color, char String[])//++
{
  uint16 Nx = 0, Ny = 0;
  uint16 Sym = 0;
  uint8 ChW = 0;
  uint8 ChH = 0;
  while(String[Sym]!=0)
  {
  	Sym++;
  }
  for(uint16 i = 0; i<Sym; i++)
  {
  	if(String[i]>= 0x20 && String[i]<= 0xFF)
  	{

  		Symbol(X, Y, Color, String[i]);
  		SymbolParameters(X, Y, &Nx, &Ny,&ChW,&ChH, String[i]);

  		if(X + ChW < DispWidth)
  		{
  		   X = Nx;
  	    }
  		else
  		{
  			break;
  		}

  	}
  }

}
void TextBox(uint16 x1, uint16 x2, uint16 y1,uint16 Thickness, uint32 BColor, uint32 FloodColor, uint32 TextColor, char Text[])//++
{
  FramePanel(BColor,FloodColor,x1, x2, y1, y1+19, Thickness);
  Label(x1+2,y1+1, TextColor, Text);
}
void TrackBar(int16 XStart, int16 XEnd, int16 YStart, int16 YEnd, int16 StartPos, int16 StopPos, int16 CurrPos, int32 BorderColor, uint32 FloodColor, uint32 TrackerColor, uint8 Orient, uint8 Rad)
{
	  float STPOS;
	  float POSCOUNT;
	  uint16 XPOS = 0, YPOS = 0;
	  uint16 POSN = CurrPos - StartPos;

	  if(Orient == Horisontal)
	  {
	      if(CurrPos == 1)
	      {
	         STPOS = StopPos - StartPos;
	         POSCOUNT = (XEnd - XStart)/STPOS;
	         XPOS = (uint16)((POSN - StartPos * POSCOUNT)+ XStart + 1);
	      }

	      if(CurrPos == StartPos)
	      {
	         STPOS = StopPos - StartPos;
	         POSCOUNT = (XEnd - XStart)/STPOS;
	         XPOS = (uint16)((POSN * POSCOUNT)+ XStart);
	      }

	      if(CurrPos > StartPos)
	      {
	         STPOS = StopPos - StartPos;
	         POSCOUNT = (XEnd - XStart)/STPOS;
	         XPOS = (uint16)((POSN * POSCOUNT)+ XStart);
	      }
	  }
	  else
	  {
	      STPOS = StopPos - StartPos;
	      POSCOUNT = (YEnd - YStart)/STPOS;
	      YPOS = (uint16)(YEnd - (POSN * POSCOUNT));
	  }

	  if(Orient == Horisontal)
	  {
	      FramePanel(BorderColor,FloodColor,XStart,XEnd,YStart, YEnd,1);
	      FillCircle(XPOS,YStart + (YEnd - YStart) / 2, Rad, TrackerColor);
	  }
	  else
	  {
		  FramePanel(BorderColor,FloodColor,XStart,XEnd,YStart, YEnd,1);
		  FillCircle(XStart + ((XEnd - XStart) / 2),YPOS,Rad,TrackerColor);
	  }
}
void Form(uint16 x1, uint16 x2, uint16 y1, uint16 y2, uint16 y_S, uint16 Thickness, uint32 ColorL, uint32 ColorS, uint32 ColorW,uint32 ColorT,char FormName[])//++
{
  uint16 k = Thickness - 1,txty;
  txty = y1+((y1+y_S - y1)/2 - 8);

  Fill_Rectangle(ColorW,x1,x2,y1,y2);
  Fill_Rectangle(ColorS,x1,x2,y1,y1+y_S);
  VLine(ColorL,x1,y1,y2,Thickness);
  VLine(ColorL,x2-k,y1,y2,Thickness);
  HLine(ColorL,x1,x2,y1,Thickness);
  HLine(ColorL,x1,x2,y2-k,Thickness);
  HLine(ColorL,x1+k,x2 - k,y_S+y1,Thickness);
  Label(x1+k+2,txty+k,ColorT,FormName);
}
void Button(uint16 x1, uint16 x2, uint16 y1, uint16 y2, uint16 Thickness, uint32 BorderColor, uint32 BtColor, char Text[],uint32 TextColor)//++
{
  uint16 txtX, txtY, g = 0;
  txtY = y1+((y2 - y1)/2 - 8);
  g = SymbolLengthPixels(Text);
  txtX = x1+((x2 - x1)/2)-(g/2);
  FramePanel(BorderColor,BtColor,x1,x2,y1,y2,Thickness);
  Label(txtX, txtY, TextColor,Text);
}
void GradientFormA(uint16 x1, uint16 x2, uint16 y1, uint16 y2, uint16 y_S, uint16 Thickness, uint32 ColorL, uint32 ColorSH, uint32 ColorSL, uint32 ColorW,uint32 ColorT,char FormName[])//++
{
  uint16 k = Thickness - 1,txty;
  txty = y1+(y_S / 2 - 8);
  Fill_Rectangle(ColorW,x1,x2,y1,y2);
  HGradA(x1,x2,y1,y1+y_S,ColorSH,ColorSL);
  VLine(ColorL,x1,y1,y2,Thickness);
  VLine(ColorL,x2-k,y1,y2,Thickness);
  HLine(ColorL,x1,x2,y1,Thickness);
  HLine(ColorL,x1,x2,y2-k,Thickness);
  HLine(ColorL,x1+k,x2 - k,y_S+y1,Thickness);
  Label(x1+k+2,txty+k,ColorT,FormName);
}
void GradientFormB(uint16 x1, uint16 x2, uint16 y1, uint16 y2, uint16 y_S, uint16 Thickness, uint32 ColorL, uint32 ColorSH, uint32 ColorSL, uint32 ColorW,uint32 ColorT,char FormName[])//++
{
  uint16 k = Thickness - 1,txty;
  txty = y1+(y_S/2 - 8);
  Fill_Rectangle(ColorW,x1,x2,y1,y2);
  VGradA(x1,x2,y1,y1+y_S,ColorSH,ColorSL);
  VLine(ColorL,x1,y1,y2,Thickness);
  VLine(ColorL,x2-k,y1,y2,Thickness);
  HLine(ColorL,x1,x2,y1,Thickness);
  HLine(ColorL,x1,x2,y2-k,Thickness);
  HLine(ColorL,x1+k,x2 - k,y_S+y1,Thickness);
  Label(x1+k+2,txty+k,ColorT,FormName);
}
void CheckBox(int16 x1, int16 x2, int16 y1, int16 y2, uint32 BrColor, uint32 BackColor, uint32 ChColor, uint8 Checked, uint32 TextColor, char Name[])
{
	FramePanel(BrColor,BackColor,x1,x2,y1,y2,2);
	if(Checked == 1)
	{

		Line(x1+3, y1+6, x1+((x2-x1)/2), y2-3, ChColor, 2);
		Line(x1+((x2-x1)/2), y2-3, x2-3, y1+3, ChColor, 2);
	}
	Label(x2 + 3, y1, TextColor, Name);
}
void Radiobutton(uint16 x, uint16 y, uint16 Radius, uint32 ExCirColor, uint32 InCirColor, uint32 BackColor, uint8 Transp_key, uint16 Checked)
{
	if(Transp_key == 0)
	{
//		FramePanel(BackColor,x-Radius,x+Radius+1,y-Radius,y+Radius+1);
	}

	if(Checked == 1)
	{
		Circle(x,y,Radius,ExCirColor,2);
		FillCircle(x,y,Radius - 4,InCirColor);

	}
	else
	{
		Circle(x,y,Radius,ExCirColor,2);
	}
}
void Graph(uint16 X1, uint16 X2, uint16 Y1, uint16 Y2, uint32 FillColor, uint32 FrColor, uint8 Thickness, uint32 LinColor, uint32 StepX, uint32 StepY, int32 XMax, int32 XMin, int32 YMax, int32 YMin)
{
	float XC, YC, StX, StY, XX, YY;;

	XC = ((float)XMax - (float)XMin) / (float)StepX;
	YC = ((float)YMax - (float)YMin) / (float)StepY;
	FramePanel(FrColor, FillColor, X1, X2, Y1, Y2, Thickness);

	StX = (((float)X2 - (float)Thickness) - ((float)X1 + (float)Thickness)) / XC;
	StY = (((float)Y2 - (float)Thickness) - ((float)Y1 + (float)Thickness)) / YC;
	XX = (float)X1 + (float)Thickness;
	YY = (float)Y2 - (float)Thickness;

	VLine(LinColor, (uint16)XX, Y1+Thickness, Y2-Thickness, 1);
	XX += StX;
	for(uint16 x = 0; x < XC; x++)
	{
		if(x < XC - 1)
		{
		    VLine(LinColor, (uint16)XX, Y1+Thickness, Y2-Thickness, 1);
		    XX += StX;
	    }
		else
		{
		    VLine(LinColor, X2 - Thickness, Y1+Thickness, Y2-Thickness, 1);
		    XX += StX;
		}
	}
	HLine(LinColor, X1+Thickness, X2-Thickness, (uint16)YY, 1);
	YY -= StY;
	for(uint16 y = 0; y < YC; y++)
	{
		if((uint16)YY >= Y1 && (uint16)YY <= Y1 + Thickness)
		{
		    HLine(LinColor, X1+Thickness, X2-Thickness, Y1 + Thickness, 1);
	    }
		else
		{
			HLine(LinColor, X1+Thickness, X2-Thickness, (uint16)YY, 1);
		}
		YY -= StY;
	}
}
void GraphLine(uint16 X1, uint16 X2, uint16 Y1, uint16 Y2, uint32 Color, uint8 Thickness, uint32 StepX, uint32 StepY, uint32 XMax, uint32 XMin, int YMax, int YMin, int data[], uint32 Points, uint8 LineThickness)
{
	uint16 Points_cnt = 0, Yctrl = 0;
	float LineX = 0, NextX = 0, LineY = 0, NextY = 0;
	float XC = 0, StX = 0, YY = 0, Ydif = 0, StY = 0, StartY = 0;
    uint8 owf_flag = 1;
	YY = Y2 - Y1;
	Ydif = YMax - YMin;

	if((X2 - X1) >= (XMax - XMin))
	{
		XC = (float)XMax;
		StX = (float)(X2 - X1) / (float)(XMax - XMin) * 0.9902;
	}

	LineX = (float)X1 + (float)Thickness + (float)(LineThickness - 1);
	NextX = LineX;
	StartY = (float)Y2 - (float)(Thickness) - (float)LineThickness;

	LineY = StartY - ((YY / Ydif * (float)data[XMin]) * 0.974);

	Yctrl = LineY;

	if(Yctrl < (Y1 + Thickness))
	{
		Yctrl = Y1 + Thickness;
	}

	NextY = Yctrl;

	for(uint16 x = XMin; x < XC; x++)
	{
		StY = (YY / Ydif * (float)data[x]) * 0.974; //расчет дисплейного Y, 0.974 - корректировка погрешности
		NextX += StX;
		NextY = StartY - StY;

		if((data[x] >= YMin) && (data[x] <= YMax))
		{
		    if(Points_cnt < Points)
		    {
		    	if(NextX >= X2 - Thickness)
		    	{NextX = X2 - LineThickness;}

		        Line((uint16)LineX, (uint16)LineY, (uint16)NextX, (uint16)NextY, Color, LineThickness);
		        owf_flag = 0;
	        }
		    Points_cnt++;
			LineX = NextX;
			LineY = NextY;
	    }
		else
		{
			if(owf_flag == 0)
			{
				NextY = Y1 + Thickness;
				Line((uint16)LineX, (uint16)LineY, (uint16)NextX, (uint16)NextY, Color, LineThickness);
				owf_flag = 1;
			}

			if(data[x] > YMax)
			{
			    LineX = NextX;
			    LineY = Y1 + Thickness;
		    }
			else
			{
			    LineX = NextX;
			    LineY = StartY;
			}
		}
	}
}
void RichTextBox(uint16 x1, uint16 x2, uint16 y1, uint16 y2,uint16 Thickness, uint32 BColor, uint32 FloodColor, uint32 TextColor, char Text[] )//++
{
    uint16 Sym_count = 0;
    uint16 Nx = 0, Ny = 0;
    uint16 x = 0, y = 0;
    uint8 ChWt = 0;
    uint8 ChH = 0;

    x = x1 + Thickness;
    y = y1;
    while(Text[Sym_count]!=0)
    {
      Sym_count++;
    }

    FramePanel(BColor,FloodColor,x1,x2,y1,y2,Thickness);

    for(int i = 0; i < Sym_count; i++)
    {
//       if((Text[i] >= 0x20) | (Text[i] == 10))
//       {
//          if(Text[i] == 10)
//          {
//     	     y = Ny;
//     	     x = x1 + Tolshina;
//          }
//          else
//          {
//        	  Symbol(x, y,TextColor, Text[i]);
//
//        	  if(y + ChH + Tolshina >= y2)
//        	  {
//        	    break;
//        	  }
//
//        	  if(Nx + ChWt + Tolshina >= x2)
//        	  {
//        	     y = Ny;
//        	     x = x1 + Tolshina;
//        	  }
//        	  else
//        	  {
//        	    x = Nx;
//        	  }
//        	  SymbolParameters(x, y, &Nx, &Ny, &ChWt, &ChH, Text[i]);
//          }

//       }
    }
}



void LCD_FramePanel()
{
   FramePanel(WindowFrameColor, WindowColor, 1, DispWidth, 1, DispHeight, WindowFrameThickness);
}
void LCD_Progress_bar(uint16 X1, uint16 X2, uint16 Y1, uint16 Y2, uint16 StartPos, uint16 StopPos, uint16 CurrPos, uint8 Orient)
{
   Progress_bar(X1, X2, Y1, Y2, StartPos, StopPos, CurrPos, WindowFrameColor, WindowColor, BarColor, WindowFrameThickness, Orient);
}
void LCD_Progress_bar_gradient(uint16 X1, uint16 X2, uint16 Y1, uint16 Y2, uint16 StartPos, uint16 StopPos, uint16 CurrPos, uint8 Orient)
{
   Progress_bar_gradient(X1, X2, Y1, Y2, StartPos, StopPos, CurrPos, WindowFrameColor, WindowColor, BarColor2, BarColor3, WindowFrameThickness, Orient);
}
void LCD_Label(uint16 X, uint16 Y, char String[])
{
   Label (X, Y, LabelColor, String);
}
void LCD_TextBox(uint16 x1, uint16 x2, uint16 y1, char Text[])
{
    TextBox(x1, x2, y1, WindowFrameThickness, WindowFrameColor, WindowColor, WindowTextColor, Text);
}
void LCD_Form(char FormName[])
{
   Form(1, DispWidth, 1, DispHeight, 25, WindowFrameThickness, WindowFrameColor, WindowBarColor, WindowColor, WindowTextColor, FormName);
}
void LCD_Button(uint16 x1, uint16 x2, uint16 y1, uint16 y2, char Text[])
{
   Button(x1, x2, y1, y2, WindowFrameThickness, ButtonFrameColor, ButtonColor, Text, ButtonTextColor);
}
void LCD_Gradient_Form_A(char FormName[])
{
   GradientFormA(1, DispWidth, 1, DispHeight, 25, WindowFrameThickness, WindowFrameColor, WindowBarHColor, WindowBarLColor, WindowColor, WindowTextColor, FormName);
}
void LCD_Gradient_Form_B(char FormName[])
{
   GradientFormB(1, DispWidth, 1, DispHeight, 25, WindowFrameThickness, WindowFrameColor, WindowBarHColor, WindowBarLColor, WindowColor, WindowTextColor, FormName);
}




