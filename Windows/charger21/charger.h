/*
 * Created: July-23-13, 8:12:35 PM
 *  Author: K. C. Lee
 * Copyright (c) 2015 by K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
 */ 

#ifndef CHARGER_H_
#define CHARGER_H_

// Directory path limit
#define MAXPATH		1024
#define BUF_SIZE	80

#include <windows.h>
#include <WinNT.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

//#include "windowex.h"
#include "pcb.h"
#include "usb.h"
#include "charger rc.h"
#include "usb host.h"
#include "debug.h"
#include "fileop.h"
#include "tabmenu.h"

// USB charger - embedded shared code starts here

// Get rid of 8051 extensions
#define code
#define xdata

// ----------------------------------------------
#include "usb common.h"
#include "battery.h"
//-----------------------------------------------
#undef code
#undef xdata

#define BATTERY_PARAMETERS ((sizeof(BatteryType)-BATTERY_NAME_LEN-1+1)/2)

#include "data.h"
#include "gui support.h"
#include "Battery gui.h"
#include "Charger gui.h"


extern HWND Hwnd, Charger_Hwnd, Battery_Hwnd, Console_Hwnd;
extern HMENU Hmenu;
extern HINSTANCE Hinst;
extern uint8_t Exit, changed, Receive_USB_Data[];

BOOL CALLBACK ConsoleProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ChargerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK BatteryProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
