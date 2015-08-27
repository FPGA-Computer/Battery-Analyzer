/*
 * usb_common.h
 *
 * Created: 19/07/2013 9:24:01 PM
 *  Author: K. C. Lee
 * Copyright (c) 2015 by K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
 */

#ifndef USB_COMMON_H_
#define USB_COMMON_H_

#include <stdint.h>
#include "pcb.h"

#define USB_PACKET_SIZE		        64
#define USB_INT_ENDPOINT	        0x81

#define USB_VID                         0x09,0x12
#define USB_PID                         0x21,0xdc

#define Packet_Chargers_Shift           0
#define Packet_Chargers_Mask            0x07
#define Packet_Chargers(X)              (X & Packet_Chargers_Mask)

#define Packet_Temp_Mask                0x7f
#define Packet_ChargeState_Shift        7
#define Packet_ChargeState_Mask         0x1f
#define Packet_ChargeState(X)           (X << Packet_ChargeState_Shift)
#define Packet_Battery_Shift            12
#define Packet_Battery_Mask             0x0f
#define Packet_Battery(X)               ((X)<<Packet_Battery_Shift)

typedef struct
{
   uint16_t      Voltage;
   int16_t       Current;
   uint16_t      Temp;
   uint16_t      Time;
   uint16_t      Charge_L;
   uint16_t      Charge_H;
 } BattInfo21;

typedef struct
{ uint16_t       Vin;
  uint16_t       flags;
  BattInfo21     BatteryInfo[CHARGERS];
 } HID_Report21;

// This is limited by EEPROM space of 254 bytes
#define BATTERY_DB_MAX 10

enum VendorCmd
 {
   CMD_ReadBattery,
   CMD_WriteBattery,
   CMD_SaveBattery,
   CMD_ReadBatterySize,
   CMD_SaveBatterySize,
   CMD_SetBattery,
   CMD_StartStop
 };


#endif /* USB_COMMON_H_ */
