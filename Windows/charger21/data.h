/*
 * Battery.h
 *
 * Created: July-24-13, 10:52:47 PM
 *  Author: K. C. Lee
 * Copyright (c) 2015 by K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
 */

#ifndef _DATA_H_
#define _DATA_H_

#include <stdint.h>
#include "pcb.h"

typedef struct
{
  uint8_t	State;
  uint8_t	BattIndex;
  uint16_t	Voltage;
  int16_t	Current;
  uint16_t	Charge_L;
  uint16_t	Charge_H;
  uint16_t	Time;
  uint16_t	Temp;
 } BatteryStatus;

typedef struct
{
  BatteryStatus Ch[CHARGERS];
  uint16_t	Vin;
 } ChargerStatus;

#define CHARGE_DATASIZE_INCR	(4032/sizeof(Battery_Database))

extern UINT USB_Packet;
extern const int statwidths[];
extern const uint8_t  StatusWidthSize;
extern BatteryStatus *ChargerData[];

extern int ChargerDataSize[];
extern int ChargerReallocSize[];

void Free_ChargerData(void);
int Save_ChargerData(char *file);

#endif
