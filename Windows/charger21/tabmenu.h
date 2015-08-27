/*
 * Created: July-29-13, 10:10:24 PM
 *  Author: K. C. Lee
 * Copyright (c) 2015 by K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
 */ 

// tab control

#ifndef _TABMENU_H_
#define _TABMENU_H_

typedef struct
{ char       *Menu;
  BOOL        CALLBACK (*Proc)(HWND,UINT,WPARAM,LPARAM);
  LPCSTR      Resource;
  HWND        Hwnd;
 } TabItems;

uint16_t AddTabs(HWND hwnd,int dlg, TabItems tab[]);
void  OpenTab(HWND hwnd,int dlg,int *Top,TabItems tab[]);

extern TabItems Tabs[];
extern RECT TabRect;
extern int  TabTop;

#endif
