/*
 * Created: July-23-13, 8:12:35 PM
 *  Author: K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
*/

#include "charger.h"

usb_dev_handle *USB_Dev_Handle;
HWND Hwnd, Charger_Hwnd, Battery_Hwnd, Console_Hwnd;
HINSTANCE Hinst;
HMENU Hmenu;

uint8_t Receive_USB_Data[]= {1,1}, Exit=0, changed=0;

UINT USB_Packet;

void ExitApp(void)
{
  Exit=1;

  if(USB_Dev_Handle)
    usb_close(USB_Dev_Handle);

  EndDialog(Hwnd,0);
 }

BOOL CALLBACK ConsoleProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{ 
  switch(msg)
  { case WM_INITDIALOG:
      InitDbg(hwnd);
      break;

    default:
      return FALSE;
   }
  return TRUE;
 }

BOOL CALLBACK BatteryProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{ 
  Battery_Hwnd=hwnd;

  if(Batt_DB_Size>=BATTERY_DB_MAX)
    EnableWindow(GetDlgItem(Battery_Hwnd,IDC_ADD),0);

  switch(msg)
  { case WM_INITDIALOG:
      Battery_GUI_Init();
      break;

    case WM_COMMAND:		// Buttons
      switch(LOWORD(wParam))
      {
	case IDC_Batt_Name:
          Battery_Name_Changes(wParam);
          break;

        case IDC_Update:
          Battery_GUI_Update();
          break;

        case IDC_ADD:
          Battery_GUI_Add();
	  break;

        case IDC_Remove:
          Battery_GUI_Remove();
          break;

        default:
          Battery_DB_Changes(wParam);
          break;
       }

    default:
      return FALSE;
   }
  return TRUE;
 }


BOOL CALLBACK ChargerProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{ 
  Charger_Hwnd=hwnd;

  switch(msg)
  { case WM_INITDIALOG:
      Charger_GUI_Init();
      break;

    case WM_COMMAND:   // Buttons
      switch(LOWORD(wParam))
      {
        case IDC_ReceiveData0:
          Receive_USB_Data[0] = SendDlgItemMessage(hwnd,IDC_ReceiveData0,BM_GETCHECK,0,0);
          break;

        case IDC_ReceiveData1:
          Receive_USB_Data[1] = SendDlgItemMessage(hwnd,IDC_ReceiveData1,BM_GETCHECK,0,0);
          break;

        case IDC_Battery0:
        case IDC_Battery1:
          Charger_Set_Battery(wParam);
	  break;

        case IDC_Charger0_But:
          USB_Cmd(CMD_StartStop,0,0);
	  break;

	case IDC_Charger1_But:
          USB_Cmd(CMD_StartStop,0,1);
	  break;

	case IDC_ZOOM_ALL:
	  break;
       }

    default:
      return FALSE;
   }
  return TRUE;
 }

BOOL CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{ 
  int code;

  Hwnd = hwnd;
  
  switch(msg)
  { 
    case WM_COMMAND:        													// Buttons
      switch(LOWORD(wParam))
      {

        case CM_New:
          code= MessageBox(Hwnd,"Okay to erase charger data?",
	                   "Erase data confirmation",
	                   MB_OKCANCEL|MB_ICONSTOP|MB_APPLMODAL);
          if(code==IDOK)                 
            Free_ChargerData();
	  break;

        case CM_Open:
	  break;
        case CM_SaveAs:
          WriteGUI(hwnd,DataFile,"*.txt");
	  break;

        case CM_Exit:
          ExitApp();
          break;

        case CM_SaveLog:
	  SaveLogFile(hwnd);
	  break;

        case CM_Clr_Log:
	  SendDlgItemMessage(hwnd,IDC_DBG,LB_RESETCONTENT,0,0);
          break;

        case CM_Log_Everything:
        case CM_Log_Debug:
        case CM_Log_Verbose:
        case CM_Log_Warnings:
        case CM_Log_Errors:
        case CM_Log_None:
           Charger_LogMenu(LOWORD(wParam));
	  break;
      }
      break;

    case WM_TIMER:			/* Handle Timer messages */
      TimerTask();
      break;

    case WM_INITDIALOG:
      InitChargerDialog();
      break;

    case WM_SIZE:
      SendMessage(GetDlgItem(Hwnd,IDD_StatusBar),WM_SIZE,0,0);
      break;

    case WM_CLOSE:
      ExitApp();
      break;

    case WM_NOTIFY:
      code = ((LPNMHDR)lParam)->code;

      if ((code==TCN_SELCHANGE)&& (LOWORD(wParam)==IDC_Tab))
      { OpenTab(hwnd,IDC_Tab,&TabTop,Tabs);
        break;
       }
      else
        return(FALSE);

      break;

    default:
      if(msg==USB_Packet)
      { 
        Charger_GUI_Update((ChargerStatus *) wParam);
	break;
       }
      else
        return FALSE;
   }
  return TRUE;
 }

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
  Hinst= hInstance;
  return(DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,DialogProc));
 }
