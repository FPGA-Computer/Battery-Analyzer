/*
 * Created: July-23-13, 7:58:33 PM
 *  Author: K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
*/

// libusb.lib: Need to set up compiler to compile in cdecl !!

#include <wchar.h>
#include "charger.h"

DWORD USB_ThreadID;
uint8_t InitUSB;

usb_dev_handle *Open_USB_Dev(void)
{
  struct usb_bus *busses;
  struct usb_bus *bus;
  struct usb_device *dev;
  usb_dev_handle *usb_handle;

  /* Find the device */
  usb_init();
  usb_find_busses();
  usb_find_devices();
  busses = usb_get_busses();

  for (bus=busses; bus; bus=bus->next)
  {
    for (dev=bus->devices; dev; dev=dev->next)
    {
      // WCHAR Vendor_Str[]={ USB_VENDOR_NAME, 0 };
      // WCHAR Device_Str[]={ USB_DEVICE_NAME, 0 };
      // WCHAR *ID_String[] ={ Vendor_Str,Device_Str };
      // WCHAR DescriptStr[USB_CONFIG_STR];

      unsigned char Vendor_ID[]={ USB_VID };
      unsigned char Product_ID[] = { USB_PID };

      // for V-USB, match VID/PID and actual Strings as per V-USB "Free PID" requirement

      if ((dev->descriptor.idVendor == *((uint16_t *) Vendor_ID)) &&
          (dev->descriptor.idProduct == *((uint16_t *) Product_ID)))
      {
        usb_handle=usb_open(dev);
  
        if(usb_handle)
        { 
	  
	  /* not using V-USB any more	  
	  Match=1;

	  for(i=1;i<=2;i++)
	  {
	    len=usb_get_string(usb_handle,i,0x0409,(char *)DescriptStr, sizeof(DescriptStr));
            DescriptStr[len/2]=0;

            Match = Match && !wcscmp(DescriptStr+1,ID_String[i-1]);
           }

          if(Match) */
	  { 
	    return usb_handle;
	   }
         }
       }
     }
   }
  return NULL;
 }

int8_t Poll_USB(void)
{ static char PrevStatus = 0x02;
  char USB_Presence;

  InitUSB = !USB_Dev_Handle;

  if(!USB_Dev_Handle)
  {
    USB_Dev_Handle = Open_USB_Dev();
    USB_Presence = (USB_Dev_Handle)?1:0;

    if(PrevStatus!=USB_Presence)
    {
      SendDlgItemMessage(Hwnd,IDD_StatusBar,SB_SETTEXT,STATUS_CONNECT,
	                 (LPARAM)(USB_Presence?"Connected to Charger.":"Charger not found."));
      PrevStatus=USB_Presence;

      if(!USB_Presence)
	Charger_GUI_Clr();

      return(USB_Presence);
     }
   }

  return(-1);
 }

void USB_ErrorMsg(char *Func,int Error)
{ char *USB_ErrorCode[]=
  { "LIBUSB_ERROR_OTHER",		// -99
    "LIBUSB_ERROR_IO", 			// -1
    "LIBUSB_ERROR_INVALID_PARAM",	// -2
    "LIBUSB_ERROR_ACCESS",		// -3
    "LIBUSB_ERROR_NO_DEVICE",		// -4
    "LIBUSB_ERROR_NOT_FOUND",		// -5
    "LIBUSB_ERROR_BUSY",		// -6
    "LIBUSB_ERROR_TIMEOUT",		// -7
    "LIBUSB_ERROR_OVERFLOW",		// -8
    "LIBUSB_ERROR_PIPE",		// -9
    "LIBUSB_ERROR_INTERRUPTED",		// -10
    "LIBUSB_ERROR_NO_MEM",		// -11
    "LIBUSB_ERROR_NOT_SUPPORTED"  	// -12
   };

  if(Error==-116)
    CondDebug(RPT_ERR,"%s: Error: %d %s",Func,Error,USB_ErrorCode[-7]);
  else if(Error<0)
    CondDebug(RPT_ERR,"%s: Error: %d %s",Func,Error,USB_ErrorCode[(-Error>(sizeof(USB_ErrorCode)/sizeof(char*)))?0:-Error]);
  else
    CondDebug(RPT_ERR,"%s: Data transfer: %d bytes != requested",Func,Error);
 }

// Swap byte ordering for 8051
void SwapEndian(uint16_t *ptr, int size)
{
  int i, l, h;

  for(i=0;i<size;i++)
  {
    h = ptr[i] & 0xff,
    l = ptr[i] >>8;
    ptr[i] = (h<<8)|l;
   }
}

DWORD WINAPI USBTask( LPVOID Dummy )
{ int i, chargers, transfered;
  HID_Report21 Packet;
  static ChargerStatus Charger;

  while(!Exit)
  { 

    while(InitUSB)
    {
      if (USB_Dev_Handle && 
          !usb_set_configuration(USB_Dev_Handle,1) && 
          !usb_claim_interface(USB_Dev_Handle,0))

        InitUSB=0;
      else    
        Sleep(500);
     }


    if((Receive_USB_Data[0] || Receive_USB_Data[1]) && USB_Dev_Handle)
    {
      transfered = usb_interrupt_read(USB_Dev_Handle,USB_INT_ENDPOINT,
	                             (char *)&Packet,sizeof(Packet),
	                             USB_INTR_POLL_INTERVAL*10);
      Status_USB(usb_interrupt);

      if(transfered==sizeof(Packet))
      {
        // 8051 is big-endian

        #ifdef BIG_ENDIAN
          SwapEndian((uint16_t *)&Packet, sizeof(Packet)/sizeof(uint16_t));
        #endif

        chargers = (Packet.flags>>Packet_Chargers_Shift) & Packet_Chargers_Mask;
        Charger.Vin = Packet.Vin;

        for(i=0;i<chargers;i++)
        {
          Charger.Ch[i].State     = (Packet.BatteryInfo[i].Temp >>Packet_ChargeState_Shift)
	                            & Packet_ChargeState_Shift;
	  Charger.Ch[i].BattIndex = (Packet.BatteryInfo[i].Temp >>Packet_Battery_Shift)
	                            &Packet_Battery_Mask;

	  Charger.Ch[i].Temp      = Packet.BatteryInfo[i].Temp & Packet_Temp_Mask;
	  Charger.Ch[i].Voltage   = Packet.BatteryInfo[i].Voltage;
	  Charger.Ch[i].Current   = Packet.BatteryInfo[i].Current;
          Charger.Ch[i].Time 	  = Packet.BatteryInfo[i].Time;
          Charger.Ch[i].Charge_L  = Packet.BatteryInfo[i].Charge_L;
	  Charger.Ch[i].Charge_H  = Packet.BatteryInfo[i].Charge_H;
	}

        PostMessage(Hwnd,USB_Packet,(WPARAM)&Charger,0);
        Status_USB(usb_idle);
       }
      else if(transfered!=-116)
      { 
	// CondDebug(RPT_ERR,"usb_interrupt_read() = %d",transfered);
        USB_Dev_Handle = NULL;
       }
     }
    else
      Sleep(200);

   }

  return(0);
 }

void USB_Cmd(uint8_t Cmd, uint16_t Value, uint16_t Index)
{ 
  uint16_t Size;
  uint8_t Buffer[255];

  if(!USB_Dev_Handle)
    return;

  Status_USB(usb_command);

  switch(Cmd)
  {
    case CMD_ReadBattery:
      Size=usb_control_msg(USB_Dev_Handle,USB_TYPE_VENDOR|USB_RECIP_DEVICE|USB_ENDPOINT_IN,
	                   CMD_ReadBattery,Value,Index,(char*)Buffer,sizeof(Buffer),CMD_TIMEOUT);
      if(Size>0)
      {
        // copy from byte aligned struct to x86 

        strncpy(Battery_DB[Index].Name,(char*)Buffer,BATTERY_NAME_LEN);
	Battery_DB[Index].Name[BATTERY_NAME_LEN]=0;
        Battery_GUI_AddName(Battery_DB[Index].Name,Index);

        Battery_DB[Index].Parameters[BATT_PARM_ALG] = Buffer[BATTERY_NAME_LEN];

        memcpy((void*)&Battery_DB[Index].Parameters[BATT_PARM_ALG+1],
	       (void*)&Buffer[BATTERY_NAME_LEN+1],
	       sizeof(int16_t)*(BATTERY_PARAMETERS-1));
/*
        CondDebug(RPT_DEBUG_NOPREFIX,"%s %d %d %d %d %d %d %d %d",
                  Battery_DB[Index].Name,
	          Battery_DB[Index].Parameters[0],Battery_DB[Index].Parameters[1],
	          Battery_DB[Index].Parameters[2],Battery_DB[Index].Parameters[3],	  
	          Battery_DB[Index].Parameters[4],Battery_DB[Index].Parameters[5],
	          Battery_DB[Index].Parameters[6],Battery_DB[Index].Parameters[7]);
*/
       }

      break;

    case CMD_WriteBattery:

      // copy from x86 to byte aligned struct

      strncpy((char*)Buffer,Battery_DB[Index].Name,BATTERY_NAME_LEN);
      Buffer[BATTERY_NAME_LEN]=Battery_DB[Index].Parameters[BATT_PARM_ALG];

      memcpy((void*)&Buffer[BATTERY_NAME_LEN+1],
	     (void*)&Battery_DB[Index].Parameters[BATT_PARM_ALG+1],
	     sizeof(int16_t)*(BATTERY_PARAMETERS-1));

      Size=usb_control_msg(USB_Dev_Handle,USB_TYPE_VENDOR|USB_RECIP_DEVICE|USB_ENDPOINT_OUT,
	                   CMD_WriteBattery,Value,Index,(char*)Buffer,sizeof(Buffer),CMD_TIMEOUT);
      break;

    case CMD_ReadBatterySize:
      Size=usb_control_msg(USB_Dev_Handle,USB_TYPE_VENDOR|USB_RECIP_DEVICE|USB_ENDPOINT_IN,
	                   CMD_ReadBatterySize,Value,Index,(char*)&Batt_DB_Size,sizeof(Batt_DB_Size),CMD_TIMEOUT);
//    CondDebug(RPT_DEBUG_NOPREFIX,"%d %d",Size, Batt_DB_Size);

      break;

    case CMD_SaveBatterySize: 
    case CMD_SaveBattery:
    case CMD_SetBattery:
    case CMD_StartStop:

      usb_control_msg(USB_Dev_Handle,USB_TYPE_VENDOR|USB_RECIP_DEVICE|USB_ENDPOINT_OUT,
	              Cmd,Value,Index,NULL,0,CMD_TIMEOUT);
      break;
   }

  Status_USB(usb_idle);
}
