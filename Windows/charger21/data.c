/*
 * Created: July-24-13, 10:46:03 PM
 *  Author: K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
*/

#include "charger.h"

BatteryStatus *ChargerData[CHARGERS]={NULL};

int ChargerDataSize[CHARGERS]= {0};
int ChargerReallocSize[CHARGERS]= {0};

void Free_ChargerData(void)
{ uint8_t i;

  for(i=0;i<CHARGERS;i++)
  {

    if(ChargerData[i])
    {
      free(ChargerData[i]);
      ChargerData[i]=NULL;
     }

    ChargerDataSize[i]=0;
    ChargerReallocSize[i]=0;
   }
 }

void Add_ChargeData(ChargerStatus *ChargerPacket)
{ 
  uint8_t i;

  for(i=0;i<CHARGERS;i++)
  {
    if(ChargerDataSize[i] >= ChargerReallocSize[i])
    {
      ChargerReallocSize[i] += CHARGE_DATASIZE_INCR;
      ChargerData[i]=realloc(ChargerData[i],ChargerReallocSize[i]);
     }

    memcpy(&ChargerData[i][ChargerDataSize[i]],&ChargerPacket->Ch[i],sizeof(BatteryStatus));
  
    ChargerDataSize[i]++; 
   }
 }

int Save_ChargerData(char *path)
{ FILE *outfile;

  int i, j;

  for(i=0,j=1;i<CHARGERS;i++)
    j=j && ChargerData[i];

  if(!j)
    return(FALSE);

  outfile=fopen(path,"w");

  if(!outfile)
    return(FALSE);

  fprintf(outfile,"Battery 0: %s Battery 1: %s\n\n",
          Battery_DB[ChargerData[0]->BattIndex].Name,
          Battery_DB[ChargerData[1]->BattIndex].Name);

  fprintf(outfile,"Time 0 [sec]\tState0\tV0 [V]\tI0 [mA]\tT0 [C]\tQ0 [mAs]\t"
                  "Time 1 [sec]\tState1\tV1 [V]\tI1 [mA]\tT1 [C]\tQ1 [mAs]\t");

  /*
  for(i=0,ptr=ChargeData;i<ChargeDataSize;i++,ptr++)
  { 
    for(j=0;j<CHARGERS;j++)
      fprintf(outfile,"%d\t%d\t%4.3f\t%d\t%d\t%f",
	      ptr->Ch[j].Time,ptr->Ch[j].State,
              ptr->Ch[j].Voltage/VOLT(1),ptr->Ch[j].Current,ptr->Ch[j].Temp,
	      ((long)ptr->Ch[j].Charge_L|(ptr->Ch[j].Charge_H<<16))/5.0);

    fprintf(outfile,"\n");
   }

*/

  fclose(outfile);
  return(TRUE);
 }


#ifdef TEST
void font(void)
{

RECT rc;  
int angle; 
HFONT hfnt, hfntPrev; 
LPSTR lpszRotate = "String to be rotated."; 
 
/* Allocate memory for a LOGFONT structure. */ 
 
PLOGFONT plf = (PLOGFONT) LocalAlloc(LPTR, sizeof(LOGFONT)); 
 
 
/* Specify a font typeface name and weight. */ 
 
lstrcpy(plf->lfFaceName, "Arial"); 
plf->lfWeight = FW_NORMAL; 
 
/* Retrieve the client-rectangle dimensions. */ 
 
GetClientRect(hwnd, &rc); 
 
/* 
 * Set the background mode to transparent for the 

 * text-output operation. 
 */ 
 
SetBkMode(hdc, TRANSPARENT); 
 
/* 
 * Draw the string 36 times, rotating 10 degrees 
 * counter-clockwise each time. 
 */ 
 
for (angle = 0; angle < 3600; angle += 100) { 
    plf->lfEscapement = angle; 
    hfnt = CreateFontIndirect(plf); 
    hfntPrev = SelectObject(hdc, hfnt); 
    TextOut(hdc, rc.right / 2, rc.bottom / 2, 
        lpszRotate, lstrlen(lpszRotate)); 
    SelectObject(hdc, hfntPrev); 
    DeleteObject(hfnt); 

} 
 
/* Reset the background mode to its default. */ 
 
SetBkMode(hdc, OPAQUE); 
 
/* Free the memory allocated for the LOGFONT structure. */ 
 
LocalFree((LOCALHANDLE) plf); 
 }

void Marker(LONG x, LONG y, HWND hwnd)  
{ 
    HDC hdc; 
 
    hdc = GetDC(hwnd); 
        MoveToEx(hdc, (int) x - 10, (int) y, (LPPOINT) NULL); 
        LineTo(hdc, (int) x + 10, (int) y); 
        MoveToEx(hdc, (int) x, (int) y - 10, (LPPOINT) NULL); 
        LineTo(hdc, (int) x, (int) y + 10); 
    ReleaseDC(hwnd, hdc); 
}
#endif
