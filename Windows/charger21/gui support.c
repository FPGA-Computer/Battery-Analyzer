/*
 * Created: August-08-13, 3:06:42 PM
 *  Author: K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
*/

#include "charger.h"

char DataPath[MAXPATH],ListPath[MAXPATH];

void Status_USB(uint8_t i)
{ 
  SendDlgItemMessage(Hwnd,IDD_StatusBar,SB_SETTEXT,STATUS_USB,
                    (LPARAM)((i==usb_interrupt)?"USB":(i==usb_command)?"USB":""));
 }

void TimerTask(void)
{ uint8_t update, i;

  update=Poll_USB();

  if(update==1)			// read battery database from charger
  {
    Charger_GUI_Init();
    USB_Cmd(CMD_ReadBatterySize,0,0);
    SendDlgItemMessage(Battery_Hwnd,IDC_BatteryScroll,SBM_SETRANGEREDRAW,0,Batt_DB_Size);

    for(i=0;i<Batt_DB_Size;i++)
      USB_Cmd(CMD_ReadBattery,0,i);

    Battery_GUI_Draw(0);
   }
 }

void WriteGUI(HWND hwnd,int Type, char *LpstrFilter)
{ OPENFILENAME DirBox;

  ZeroMemory(&DirBox, sizeof(DirBox));
  DirBox.lStructSize = sizeof(DirBox);
  DirBox.hwndOwner = hwnd;
  DirBox.lpstrFile = (Type==DataFile)?DataPath:ListPath;
  DirBox.lpstrFilter = (LPCTSTR) LpstrFilter;
  DirBox.nMaxFile = MAX_PATH;
  DirBox.lpstrTitle = (Type==DataFile)?"Save Charger Data as":"Save Battery as";
  DirBox.Flags = OFN_EXPLORER | OFN_LONGNAMES;

  if (GetSaveFileName(&DirBox))
  { 
    if(Type==DataFile)
      Save_ChargerData(DataPath);
   }
 }
