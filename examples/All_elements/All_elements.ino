#include "SimpleGraphics.h"

void setup() {
  DisplayConfig ds;
  ds.Color_Type = Color565;
  ds.Display_Height = 320;
  ds.Display_Width = 480;
  ds.Layers = 1;            
  ds.Start_RAM_Address = 0;
  
  ds.Window_Frame_Color = red;
  ds.Window_Bar_Color = lightBlue2;
  ds.Window_Bar_H_Color = blue;
  ds.Window_Bar_L_Color = lightBlue4;
  ds.Window_Color = white;
  ds.Window_Text_Color = black;
  ds.Window_Frame_Thickness = 2;
  
  ds.Button_Frame_Color = lightBlack2;
  ds.Button_Color = gray;
  ds.Button_Selected_Color = lightBlack3;
  ds.Button_Text_Color = black;
 
  ds.Label_Color = black;
  ds.Label_Selected_Color = lightBlack2;
  ds.Label_Background_Color = blue;
  
  ds.Bar_Color = darkGreen2;
  ds.Bar_Color2 = darkViolet2;
  ds.Bar_Color3 = lightViolet2;
  
  Graphics_Init(&ds);
}

void loop() {
LCD_FramePanel();
LCD_Label(30, 5, "Progress bar >>>");

for(int i = 0; i < 10; i++)
{
  LCD_Progress_bar(50, 430, 135, 155, 0, 9, i, Horisontal);
  LCD_Progress_bar_gradient(50, 430, 160, 180, 0, 9, i, Horisontal);
  delay(100);
}
delay(3000);
LCD_FramePanel();
LCD_Label(110, 5, "Progress bar >>>");

for(int j = 0; j < 10; j++)
{
  LCD_Progress_bar(50, 70, 20, 300, 0, 9, j, Vertical);
  LCD_Progress_bar_gradient(80, 100, 20, 300, 0, 9, j, Vertical);
  delay(100);
}
delay(3000);
LCD_FramePanel();
LCD_Label(30, 290, "Simple Progress bar >>>");

LCD_Progress_bar(50, 430, 10, 30, 0, 10, 0, Horisontal);
LCD_Progress_bar(50, 430, 35, 55, 0, 10, 1, Horisontal);
LCD_Progress_bar(50, 430, 60, 80, 0, 10, 2, Horisontal);
LCD_Progress_bar(50, 430, 85, 105, 0, 10, 3, Horisontal);
LCD_Progress_bar(50, 430, 110, 130, 0, 10, 4, Horisontal);
LCD_Progress_bar(50, 430, 135, 155, 0, 10, 5, Horisontal);
LCD_Progress_bar(50, 430, 160, 180, 0, 10, 6, Horisontal);
LCD_Progress_bar(50, 430, 185, 205, 0, 10, 7, Horisontal);
LCD_Progress_bar(50, 430, 210, 230, 0, 10, 8, Horisontal);
LCD_Progress_bar(50, 430, 235, 255, 0, 10, 9, Horisontal);
LCD_Progress_bar(50, 430, 260, 280, 0, 10, 10, Horisontal);
delay(10000);
LCD_FramePanel();
LCD_Label(30, 290, "Gradient Progress bar >>>");

LCD_Progress_bar_gradient(50, 430, 10, 30, 0, 10, 0, Horisontal);
LCD_Progress_bar_gradient(50, 430, 35, 55, 0, 10, 1, Horisontal);
LCD_Progress_bar_gradient(50, 430, 60, 80, 0, 10, 2, Horisontal);
LCD_Progress_bar_gradient(50, 430, 85, 105, 0, 10, 3, Horisontal);
LCD_Progress_bar_gradient(50, 430, 110, 130, 0, 10, 4, Horisontal);
LCD_Progress_bar_gradient(50, 430, 135, 155, 0, 10, 5, Horisontal);
LCD_Progress_bar_gradient(50, 430, 160, 180, 0, 10, 6, Horisontal);
LCD_Progress_bar_gradient(50, 430, 185, 205, 0, 10, 7, Horisontal);
LCD_Progress_bar_gradient(50, 430, 210, 230, 0, 10, 8, Horisontal);
LCD_Progress_bar_gradient(50, 430, 235, 255, 0, 10, 9, Horisontal);
LCD_Progress_bar_gradient(50, 430, 260, 280, 0, 10, 10, Horisontal);
delay(10000);

LCD_FramePanel();
LCD_Label(30, 5, "Textbox >>>");
LCD_TextBox(30, 130, 40, "Text, Num");
delay(10000);



}
