/*
 * Created: July-29-13, 10:10:24 PM
 *  Author: K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
*/

#include "charger.h"

int  TabTop=1;

TabItems Tabs[]=
{ { "Charger",ChargerProc,MAKEINTRESOURCE(IDC_CHARGER)},
  { "Batteries",BatteryProc,MAKEINTRESOURCE(IDD_BATTERY)},
  { "Console Log",ConsoleProc,MAKEINTRESOURCE(IDD_CONSOLE)},
  { NULL,NULL,NULL }
 };

uint16_t AddTabs(HWND hwnd,int dlg, TabItems tab[])
{ TC_ITEM item;
  RECT TabRect;

  int i, cyMargin;
  long dwDlgBase;

  ZeroMemory(&item,sizeof(item));

  item.mask= TCIF_TEXT|TCIF_PARAM;

  for(i=0;tab[i].Menu;i++)
  { item.pszText = tab[i].Menu;
    item.cchTextMax = strlen(tab[i].Menu)+1;
    item.lParam=i;

    SendDlgItemMessage(hwnd,dlg,TCM_INSERTITEM,(WPARAM)i,(LPARAM)&item);
   }

  SendDlgItemMessage(hwnd,dlg,TCM_ADJUSTRECT,(WPARAM)FALSE,(LPARAM)&TabRect);

  dwDlgBase = GetDialogBaseUnits();
//  cxMargin = LOWORD(dwDlgBase) / 4;
  cyMargin = HIWORD(dwDlgBase) / 8;
  OffsetRect(&TabRect,0,cyMargin*9);

  for(i=0;tab[i].Menu;i++)
  { tab[i].Hwnd=CreateDialog(Hinst,tab[i].Resource,hwnd,tab[i].Proc);
   }
  return(i-1);
 }

void OpenTab(HWND hwnd,int dlg,int *Top,TabItems tab[])
{ int sel;

  SetWindowPos(tab[*Top].Hwnd,HWND_BOTTOM,0,0,0,0,
                 SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);

  sel=SendDlgItemMessage(hwnd,dlg,TCM_GETCURSEL,0,0);
  *Top=(sel==-1)?0:sel;      // Default to first tab

  SetWindowPos(tab[*Top].Hwnd,HWND_TOP,0,0,0,0,
               SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
 }


