/*
 * Created: August-08-13, 3:47:20 PM
 *  Author: K. C. Lee
 * Copyright (c) 2015 by K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
 */ 

#include "charger.h"

const uint16_t StatusV_IDC[] = {IDC_Volt0,IDC_Volt1};
const uint16_t StatusI_IDC[] = {IDC_Current0,IDC_Current1};
const uint16_t StatusTemp_IDC[] = {IDC_Temp0,IDC_Temp1};
const uint16_t StatusTime_IDC[] = {IDC_Timer0,IDC_Timer1};
const uint16_t State_IDC[] = {IDC_Charger_State0,IDC_Charger_State1};
const uint16_t Button_IDC[] = {IDC_Charger0_But,IDC_Charger1_But};
const int statwidths[] = {340,460,490,570};

const uint8_t  StatusWidthSize = sizeof(statwidths)/sizeof(int);

const char *ChargerState[]=
{ "Unknown",       
  "Battery Select","Fault",        "Bad Battery", "No Battery", 
  "Batt Reversed", "Over Current", "Ready",       "Precharge", 
  "Const Voltage", "Const Current","Batt Full",   "Top up"
 };

void InitChargerDialog(void)
{
  Hmenu = LoadMenu(Hinst,MAKEINTRESOURCE(IDM_MENU2));
    // add menu bar
  if (Hmenu)
    SetMenu(Hwnd,Hmenu);

  Charger_LogMenu(WarnLevel+RPT_NONE);
  
  SendMessage(Hwnd,WM_SETICON,ICON_BIG,
              (LPARAM)LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Icon)));

  // Tab menu
  AddTabs(Hwnd,IDC_Tab,Tabs);
  OpenTab(Hwnd,IDC_Tab,&TabTop,Tabs);

  // Setup periodic timer messages to poll USB
  SetTimer(Hwnd, 1, USB_POLL_FREQ, NULL);

  // catch USB interrupts on a separate thread
  USB_Packet = RegisterWindowMessage("Charger Packet");
  CreateThread(NULL,0,USBTask,Hwnd,0,&USB_ThreadID);
 }

void Charger_Set_Battery(WPARAM wParam)
{ int index;

  switch(HIWORD(wParam))
  {
    // stop charger update from USB when user is selecting battery

    case CBN_DROPDOWN:
      Receive_USB_Data[0] = 0;
      Receive_USB_Data[1] = 0;
      break;

    case CBN_CLOSEUP:
      Receive_USB_Data[0] = SendDlgItemMessage(Charger_Hwnd,IDC_ReceiveData0,BM_GETCHECK,0,0);
      Receive_USB_Data[1] = SendDlgItemMessage(Charger_Hwnd,IDC_ReceiveData1,BM_GETCHECK,0,0);
      break;

    case CBN_SELCHANGE:

      if(!ChargerUpdating)
      {
        index = SendDlgItemMessage(Charger_Hwnd,LOWORD(wParam),CB_GETCURSEL,0,0);
        USB_Cmd(CMD_SetBattery,index?index-1:BATTERY_DB_MAX,(LOWORD(wParam)==IDC_Battery0)?0:1);
       }
   }
 }

void Charger_GUI_Clr(void)
{ int i;

  for(i=0;i<CHARGERS;i++)
  {
    SetDlgItemText(Charger_Hwnd,StatusV_IDC[i],"");
    SetDlgItemText(Charger_Hwnd,StatusI_IDC[i],"");
    SetDlgItemText(Charger_Hwnd,StatusTemp_IDC[i],"");
    SetDlgItemText(Charger_Hwnd,StatusTime_IDC[i],"");
    SetDlgItemText(Charger_Hwnd,State_IDC[i],"");
    SetDlgItemText(Charger_Hwnd,IDC_VIN,"");
    EnableWindow(GetDlgItem(Charger_Hwnd,Button_IDC[i]),0);
    SendDlgItemMessage(Charger_Hwnd,Batt_IDC[i],CB_RESETCONTENT,0,0);
   }

  for(i=STATUS_USB;i<StatusWidthSize;i++)
    SendDlgItemMessage(Hwnd,IDD_StatusBar,SB_SETTEXT,i,(LPARAM)"");
 }

void Charger_LogMenu(int Index)
{
  CheckMenuItem(Hmenu,WarnLevel+RPT_NONE,MF_BYCOMMAND|MF_UNCHECKED);
  CheckMenuItem(Hmenu,Index,MF_BYCOMMAND|MF_CHECKED);

  WarnLevel=Index-RPT_NONE;
 }

void Print_Time(char *Str, uint16_t Time)
{ uint8_t Hour, Min, Sec;

  Min = Time/60;
  Sec = Time%60;

  Hour = Min/60;
  Min %= 60;

  sprintf(Str,"%02d:%02d:%02d",Hour,Min,Sec);
 }

void Charger_GUI_Init(void)
{ int i;

  for(i=0;i<CHARGERS;i++)
  {
    SendDlgItemMessage(Charger_Hwnd,Batt_IDC[i],CB_RESETCONTENT,0,0);
    SendDlgItemMessage(Charger_Hwnd,Batt_IDC[i],CB_INSERTSTRING,0,(LPARAM)" ");
   }
  SendDlgItemMessage(Charger_Hwnd,IDC_ReceiveData0,BM_SETCHECK,Receive_USB_Data[0],0);
  SendDlgItemMessage(Charger_Hwnd,IDC_ReceiveData1,BM_SETCHECK,Receive_USB_Data[1],0);

  SendDlgItemMessage(Hwnd,IDD_StatusBar,SB_SETPARTS,StatusWidthSize,(LPARAM)statwidths);
 }

void Charger_GUI_Update(ChargerStatus *ChargerPacket)
{ uint8_t i;
  char Status[BUF_SIZE];

  ChargerUpdating=1;

  for(i=0;i<CHARGERS;i++)
    if(Receive_USB_Data[i])
    {
      SendDlgItemMessage(Charger_Hwnd,Batt_IDC[i],CB_SETCURSEL,
                         (ChargerPacket->Ch[i].BattIndex==Batt_DB_Size+1)?
	                 0:ChargerPacket->Ch[i].BattIndex+1,0);

      sprintf(Status,"%4.3fV",(float)(ChargerPacket->Ch[i].Voltage/VOLT(1)));
      SetDlgItemText(Charger_Hwnd,StatusV_IDC[i],Status);

      sprintf(Status,"%3dmA",ChargerPacket->Ch[i].Current);
      SetDlgItemText(Charger_Hwnd,StatusI_IDC[i],Status);

      sprintf(Status,"%2.1f°C",(float)ChargerPacket->Ch[i].Temp/10.0);
      //SetDlgItemText(Charger_Hwnd,StatusTemp_IDC[i],Status);
      SetDlgItemText(Charger_Hwnd,StatusTemp_IDC[i],"");

      Print_Time(Status,ChargerPacket->Ch[i].Time);
      SetDlgItemText(Charger_Hwnd,StatusTime_IDC[i],Status);

      SetDlgItemText(Charger_Hwnd,State_IDC[i],ChargerState[ChargerPacket->Ch[i].State]);

      if(ChargerPacket->Ch[i].State==Charge_Ready)
      {
	EnableWindow(GetDlgItem(Charger_Hwnd,Button_IDC[i]),1);
        SetDlgItemText(Charger_Hwnd,Button_IDC[i],"Start");
       }
      else if((ChargerPacket->Ch[i].State>=Charge_Precharge)&&
	      (ChargerPacket->Ch[i].State<=Charge_Trickle))
      {
	EnableWindow(GetDlgItem(Charger_Hwnd,Button_IDC[i]),1);
	SetDlgItemText(Charger_Hwnd,Button_IDC[i],"Stop");
       }
      else if((ChargerPacket->Ch[i].State>=Charge_Fault)&&
	      (ChargerPacket->Ch[i].State<=Charge_OverCurrent))
      {
	EnableWindow(GetDlgItem(Charger_Hwnd,Button_IDC[i]),1);
	SetDlgItemText(Charger_Hwnd,Button_IDC[i],"Clear");
       }
      else
        EnableWindow(GetDlgItem(Charger_Hwnd,Button_IDC[i]),0);
     }

  sprintf(Status,"Vin:%2.1fV",ChargerPacket->Vin/100.0);

  SendDlgItemMessage(Hwnd,IDD_StatusBar,SB_SETTEXT,STATUS_VIN,(LPARAM)Status);
  ChargerUpdating=0;
 }
