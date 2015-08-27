/*
 * Created: July-24-13, 8:46:03 AM
 *  Author: K. C. Lee
 * Copyright (c) 2015 by K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
 */ 

#ifndef DEBUG_H_
#define DEBUG_H_

enum { RPT_NONE, RPT_ERR, RPT_WARN, RPT_INFO, RPT_DEBUG, RPT_DEBUG_NOPREFIX };		
#define RPT_EVERYTHING		RPT_DEBUG_NOPREFIX

#define RPT_ERR_Prefix		"Error: "
#define RPT_WARN_Prefix		" Warn: "
#define RPT_INFO_Prefix		" Info: "
#define RPT_DEBUG_Prefix	"Debug: "
#define RPT_DEBUG_NoPrefix	""

// Debug
void Puts(char *Msg);
/*void Debug(const char *Format,...); */
void SetTextf(HWND Hwnd,int idc, const char *Format,...);
void CondDebug(int level,const char *Format,...);
void InitDbg(HWND Hwnd);
void SaveLogFile(HWND Hwnd);
extern int WarnLevel;

#define DebugSize	256

// Hex dump line length
#define HEXLINE		16
void DumpBuffer(uint8_t *Buffer, uint16_t Size);

#endif
