/*
 * Created: August-08-13, 10:24:12 PM
 *  Author: K. C. Lee
 * Copyright (c) 2015 by K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
 */ 

#ifndef _CHARGER_GUI_
#define _CHARGER_GUI_

extern RECT Coord[];
extern const uint16_t StatusV_IDC[];
extern const uint16_t StatusI_IDC[];
extern const uint16_t StatusTemp_IDC[];
extern const uint16_t StatusTime_IDC[];
extern const uint16_t State_IDC[];
extern const uint16_t Button_IDC[];
extern const int statwidths[];
extern const char *ChargerState[];

extern const uint8_t CoordSize;
extern const uint8_t  StatusWidthSize;

void InitChargerDialog(void);
void Charger_Set_Battery(WPARAM wParam);
void Charger_Resize_Window(LPARAM lParam);
void Charger_GUI_Update(ChargerStatus *ChargerPacket);
void Charger_GUI_Clr(void);
void Charger_GUI_Init(void);
void Charger_LogMenu(int Index);

#endif
