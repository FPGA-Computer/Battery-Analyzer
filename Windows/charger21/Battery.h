/*
 * Battery.h
 *
 * Created: 10/07/2013 5:16:40 PM
 *  Author: K. C. Lee
 * Copyright (c) 2015 by K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
 */ 

#ifndef BATTERY_H_
#define BATTERY_H_

#include "pcb.h"

enum ChargeAlgorithm { ALG_Unknown, ALG_CC_CV, ALG_NiMH, ALG_NiCd,  };

enum ChargeStates
{ Charge_Unknown, Charge_Select,
  Charge_Fault, Charge_BadBattery, Charge_NoBattery, Charge_Reverse, Charge_OverCurrent,
  Charge_Ready, Charge_Precharge, Charge_ConstantV, Charge_ConstantI, Charge_Full, Charge_Trickle
 };

#define BATTERY_NAME_LEN	4
#define Alg_None                Alg_Strings[0]

typedef struct  
{
  char	    Name[BATTERY_NAME_LEN+1];
  int8_t    Algorithm;
  uint16_t  Capacity;           // capacity in mA*Hr
  uint16_t  Precharge_V;        // Minimum voltage for charging
  uint16_t  Charge_V;           // Charging voltage
  uint16_t  TopUp_V;            // Top up voltage

  uint16_t  Precharge_I;        // Precharge current in C
  uint16_t  Charge_I;           // Charge current in C
  uint16_t  Term_I;             // Termination current in C
  uint16_t  TopUp_I;            // Top up current in C

/*
  uint8_t   Temp_Min;           // Temperature min
  uint8_t   Temp_Max;           // Temperature max
*/
  
 } BatteryType;

typedef struct  
{ BatteryType   Battery;
  uint8_t       BattIndex;
  uint8_t       State;
  uint8_t       SavedState;
  int16_t       Prev;
  int16_t       AccumError;
  int16_t       Voltage_Peak;
  int16_t       TempPrev;
  uint16_t      Timer;
  uint32_t      mAS;
 } Charge_State;

#define BUTTON_CH(X)            (2-(X))

// fixed point scaling
#define VOLT(X)                 ((X)*1000.0)
#define AMP(X)                  ((X)*1000.0)
#define C_PERCENT(X,Y)          ((long)X*Y/100)

void Battery_LoadProfile(uint8_t Chan, const void * Addr);
void Charger_Init(void);
void Charger_Off(uint8_t Ch, uint8_t State);
void Charge_Icon(uint8_t State);
void Charge_Status(void);
void Charger_PID(int16_t Actual, int16_t Error, uint8_t Ch, int8_t Gain);
void Charger_CC(int16_t Current, uint8_t Chan);
void Charger_CV(uint16_t Volt, uint8_t Chan);
void ChargerTask(void);
uint8_t Battery_Select(void);

extern xdata Charge_State Charger_CH[CHARGERS];
extern uint8_t Update_Cnt, Timer_Enable;

#define TIMER1_ENABLE           0x01
#define TIMER2_ENABLE           0x02
#define TIMER_ENABLE(X)         (1<<X)

#define TimerEnable(X)          Timer_Enable |= TIMER_ENABLE(X)
#define TimerDisable(X)         Timer_Enable &= ~TIMER_ENABLE(X)

#define PRECHARGE_TIMEOUT       (0.5 * Hour)
#define OVERCHARGE              (1.1 * Hour)

#define OVERVOLT                500

// Improves on stability when both chargers are on
#define I_DeadZone              0x03

#define Int_Error_Range         0x10
#define ErrorLimit_I            0x80

// PID parameters
#define PID_CC_GAIN             128		// 1/200
#define PID_CC_DSCH_GAIN        -512	        // 1/-25
#define PID_CV_GAIN             64		// 1/2000

// CC 110, 10, 50 for 768mA ~ 10% overshoot
// CC 90, 10, 50 for 768mA ~ 5% overshoot
// CC 60, 10, 60 for 768mA ~ 1.4% overshoot

// CC 40, 20, 40 for 768mA

#define PARM_P		        32
#define PARM_I		        2L
#define PARM_D		        16L

enum GAINS { DSCHRG_GAIN, CC_GAIN, CV_GAIN };

extern code const BatteryType Batteries[];
#endif /* BATTERY_H_ */
