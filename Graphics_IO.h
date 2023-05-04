#ifndef GRAPHICS_IO_H
#define GRAPHICS_IO_H

#include "SimpleGraphics.h"

void Init_Graphics_System(uint32 LCD_RAM_START_ADDRESS, uint8 Layers, uint8 ColorType);
void Fill_all(uint16 Color);
void Fill_rectangle(uint32 Color, int32 StartX, int32 StopX, int32 StartY, int32 StopY);
void MemPoint(int32 x, int32 y, int32 Color);
void Backlight(uint32 Brightness, uint8 State);
uint8 GetCursorPosition();

#endif
