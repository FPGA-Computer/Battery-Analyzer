/*
 * Created: August-08-13, 10:24:12 PM
 *  Author: K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
*/

#include "charger.h"

uint16_t Battery_IDC[] =
{ IDC_Capacity, 
  IDC_PreChargeV,IDC_PrechargeI, 
  IDC_ChargeV,IDC_ChargeI,
  IDC_TopUpV,IDC_TopUpI
 };

const uint16_t Batt_IDC[]= {IDC_Battery0,IDC_Battery1};

uint8_t Batt_DB_Size, BatteryUpdating=0, ChargerUpdating=0, Batt_Cur=BATT_SEL_INVALID;
Battery_Database Battery_DB[BATTERY_DB_MAX];

const uint16_t Battery_IDC_Size = sizeof(Battery_IDC)/sizeof(uint16_t);

void Battery_DB_Changes(WPARAM wParam)
{ uint8_t i, index;

  if(!BatteryUpdating)
  {
    index = SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Name,CB_GETCURSEL,0,0);

    for(i=0;i<Battery_IDC_Size;i++)
    { if((LOWORD(wParam)==Battery_IDC[i]) &&
	 (HIWORD(wParam)==EN_CHANGE) &&
         (Battery_GUI_ReadValue(i)!=Battery_DB[index].Parameters[i+1]))
      { 
	changed=1;
	break;
       }
     }

     changed=changed||((LOWORD(wParam)==IDC_Batt_Type)&&
	               (HIWORD(wParam)==CBN_SELCHANGE)&&
	               Battery_DB[index].Parameters[0]!=
	               SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Type,CB_GETCURSEL,0,0));

     EnableWindow(GetDlgItem(Battery_Hwnd,IDC_Update),changed);
   }
 }

void Battery_Name_Changes(WPARAM wParam)
{
  if(HIWORD(wParam)==CBN_SELCHANGE)
  {
    Battery_GUI_Draw(SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Name,CB_GETCURSEL,0,0));
    SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Name,WM_KILLFOCUS,0,0);
   }
  else if (HIWORD(wParam)==CBN_EDITCHANGE)
     EnableWindow(GetDlgItem(Battery_Hwnd,IDC_Update),1);
 }


void Battery_GUI_Init(void)
{ uint16_t i;
  char *Algorithm[]= {"Li-Ion (CC/CV)","NiMH","NiCd","Const. Current","Discharge"};

  SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Name,CB_LIMITTEXT,BATTERY_NAME_LEN,0);

  for(i=0;i< Battery_IDC_Size;i++)
    SendDlgItemMessage(Battery_Hwnd,Battery_IDC[i],EM_SETLIMITTEXT,
                       BATTERY_VPARM(i)?BATTERY_VPARM_LEN:BATTERY_IPARM_LEN,0);

  for(i=0;i<sizeof(Algorithm)/sizeof(char *);i++)
    SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Type,CB_INSERTSTRING,i,(LPARAM)Algorithm[i]);
}

void Battery_GUI_RemoveName(uint16_t Index)
{ int8_t i;

  SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Name,CB_DELETESTRING,Index,0);

  for(i=0;i<sizeof(Batt_IDC)/sizeof(uint16_t);i++)
    SendDlgItemMessage(Battery_Hwnd,Batt_IDC[i],CB_DELETESTRING,Index+1,0);

  Batt_Cur=BATT_SEL_INVALID;
 }

void Battery_GUI_AddName(char *Name, uint16_t Index)
{ int8_t i;

  Battery_GUI_RemoveName(Index);
  SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Name,CB_INSERTSTRING,Index,(LPARAM)Name);

  for(i=0;i<sizeof(Batt_IDC)/sizeof(uint16_t);i++)
  {
    SendDlgItemMessage(Charger_Hwnd,Batt_IDC[i],CB_INSERTSTRING,Index+1,(LPARAM)Name);
   }
 }

// redraw the GUI with database entry
void Battery_GUI_Draw(int8_t Index)
{ uint16_t i;
  char Buffer[BUF_SIZE];

  BatteryUpdating=1;
  sprintf_s(Buffer,sizeof(Buffer),"Battery [%d/%d]:",Batt_DB_Size?Index+1:0,Batt_DB_Size);
  SetDlgItemText(Battery_Hwnd,IDC_Name_Lbl,Buffer);

  for(i=0;i< Battery_IDC_Size;i++)
  {
    if (BATTERY_VPARM(i))
    { sprintf_s(Buffer,sizeof(Buffer),"%2.3f",(float) Battery_DB[Index].Parameters[i+1]/VOLT(1));
      SetDlgItemText(Battery_Hwnd,Battery_IDC[i],Buffer);
    }
    else
      SetDlgItemInt(Battery_Hwnd,Battery_IDC[i],Battery_DB[Index].Parameters[i+1],1);
   }

  BatteryUpdating=0;
  EnableWindow(GetDlgItem(Battery_Hwnd,IDC_Update),0);

  Batt_Cur=Index;
  SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Name,CB_SETCURSEL,Index,0);
  SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Type,CB_SETCURSEL,Battery_DB[Index].Parameters[0],0);
 }

uint16_t Battery_GUI_ReadValue(uint8_t i)
{ 
  uint8_t Index;
  char Buffer[BUF_SIZE];

  Index = SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Name,CB_GETCURSEL,0,0);
  GetDlgItemText(Battery_Hwnd,Battery_IDC[i],Buffer,sizeof(Buffer));

  return(BATTERY_VPARM(i)?atof(Buffer)*VOLT(1):atoi(Buffer));
}

Battery_Database *Battery_GUI_Read(void)
{ 
  static Battery_Database Battery;
  uint8_t i;

  GetDlgItemText(Battery_Hwnd,IDC_Batt_Name,Battery.Name,sizeof(Battery.Name));

  Battery.Parameters[0] = SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Type,CB_GETCURSEL,0,0);

  for(i=0;i<Battery_IDC_Size;i++)
    Battery.Parameters[i+1]=Battery_GUI_ReadValue(i);

  return(&Battery);
 }

void Battery_GUI_Add(void)
{ 
  char Buffer[BUF_SIZE];

  GetDlgItemText(Battery_Hwnd,IDC_Batt_Name,Buffer,sizeof(Buffer));

  if((SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Name,
	                 CB_FINDSTRINGEXACT,0,(LPARAM)Buffer)!=CB_ERR)||!Buffer[0])

    Battery_GUI_AddName("New",Batt_DB_Size);
  else
    Battery_GUI_AddName(Buffer,Batt_DB_Size);

  Batt_Cur=Batt_DB_Size;

  SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Name,CB_SETCURSEL,Batt_DB_Size,0);
  GetDlgItemText(Battery_Hwnd,IDC_Batt_Name,Battery_DB[Batt_DB_Size].Name,
                 sizeof(BATTERY_NAME_LEN+1));

  FillMemory(Battery_DB[Batt_DB_Size].Parameters,sizeof(int16_t)*BATTERY_PARAMETERS,0);      
  Battery_GUI_Draw(Batt_DB_Size++);
  USB_Cmd(CMD_SaveBatterySize,Batt_DB_Size,0);
 }

void Battery_GUI_Update(void)
{
  int index;

  index = Batt_Cur;

  if(Batt_Cur!=BATT_SEL_INVALID)
  {
    memcpy(&Battery_DB[Batt_Cur],Battery_GUI_Read(),sizeof(Battery_Database));
    Battery_GUI_AddName(Battery_DB[Batt_Cur].Name,Batt_Cur);

    USB_Cmd(CMD_WriteBattery,0,Batt_Cur);
    USB_Cmd(CMD_SaveBattery,0,Batt_Cur);
    changed=0;
    EnableWindow(GetDlgItem(Battery_Hwnd,IDC_Update),changed);
   }
 }

void Battery_GUI_Remove(void)
{ int i, index;

  if(Batt_DB_Size)
  {
    index=SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Name,CB_GETCURSEL,0,0);

    Batt_DB_Size--;

    if(index!=Batt_DB_Size)
    { 
      memcpy(&Battery_DB[index],&Battery_DB[index+1],sizeof(Battery_Database)*(Batt_DB_Size-index));
     }
    else
    { 
      FillMemory(&Battery_DB[index],sizeof(Battery_Database),0); 
      index--;
     }

    for(i=index;i<Batt_DB_Size;i++)
    {
      USB_Cmd(CMD_WriteBattery,0,i);
      USB_Cmd(CMD_SaveBattery,0,i);        
     }

    USB_Cmd(CMD_SaveBatterySize,Batt_DB_Size,0);

    Charger_GUI_Init();
    SendDlgItemMessage(Battery_Hwnd,IDC_Batt_Name,CB_RESETCONTENT,0,0);

    for(i=0;i<Batt_DB_Size;i++)
      Battery_GUI_AddName(Battery_DB[i].Name,i);

    Battery_GUI_Draw((index<0)?0:index);

   //         USB_Cmd(CMD_SaveBatterySize,Batt_DB_Size,0);
  }
 }

