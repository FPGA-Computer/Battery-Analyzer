/*
 * Created: August-08-13, 3:54:36 PM
 *  Author: K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
*/

#ifndef _BATTERY_GUI_H_
#define _BATTERY_GUI_H_

typedef struct
{ 
  char 		Name[BATTERY_NAME_LEN+1];
  uint16_t	Parameters[BATTERY_PARAMETERS];
 } Battery_Database;

#define BATT_PARM_ALG		0
#define BATTERY_VPARM_LEN	6
#define BATTERY_IPARM_LEN	5

#define BATT_SEL_INVALID	255

#define BATTERY_VPARM(X)	((X)&0x01)

extern uint16_t Battery_IDC[];
extern const uint16_t Batt_IDC[];
extern uint8_t Batt_DB_Size,BatteryUpdating,ChargerUpdating;
extern Battery_Database Battery_DB[BATTERY_DB_MAX];
extern const uint16_t Battery_IDC_Size;

void Battery_DB_Changes(WPARAM wParam);
void Battery_Name_Changes(WPARAM wParam);

void Battery_GUI_Init(void);
void Battery_GUI_AddName(char *Name, uint16_t Index);
void Battery_GUI_Draw(int8_t Index);
void Battery_GUI_Add(void);
void Battery_GUI_Update(void);
void Battery_GUI_Remove(void);

uint16_t Battery_GUI_ReadValue(uint8_t i);
Battery_Database *Battery_GUI_Read(void);

#endif
