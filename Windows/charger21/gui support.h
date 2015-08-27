/*
 * Created: August-08-13, 3:07:24 PM
 *  Author: K. C. Lee
 * Copyright (c) 2015 by K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
 */ 

#ifndef _GUI_SUPPORT_H_
#define _GUI_SUPPORT_H_

#define STATUS_GRAPH	0
#define STATUS_CONNECT	1
#define STATUS_USB	2
#define STATUS_VIN	3

enum USB_Status { usb_idle, usb_interrupt, usb_command };

enum File_Types { DataFile };

void Status_USB(uint8_t i);
void TimerTask(void);
void WriteGUI(HWND hwnd,int Type, char *LpstrFilter);

#endif
