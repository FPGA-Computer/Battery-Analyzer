/*
 * Created: July-24-13, 8:46:04 AM
 *  Author: K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
*/


#include "charger.h"
#include <ctype.h>

HWND DBGHwnd;
int WarnLevel = RPT_EVERYTHING;
char *LogMsg[] = { "None","Errors","Warnings","Verbose","Debug","Everything" };

void InitDbg(HWND Hwnd)
{ HFONT hFont;
  int index;
  
  DBGHwnd=Hwnd;

  for(index=0;index<sizeof(LogMsg)/sizeof(char *);index++)
    SendDlgItemMessage(Hwnd,IDC_ReportLevel,CB_ADDSTRING,0,(LPARAM)LogMsg[index]);

  SendDlgItemMessage(Hwnd,IDC_ReportLevel,CB_SETCURSEL,RPT_EVERYTHING,0);

  // Use fixed size font for hex dump
  hFont = CreateFont(-12,8,0,0,FW_LIGHT,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
                     CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,
                     "Courier New");
  SendDlgItemMessage(Hwnd,IDC_DBG,WM_SETFONT,(WPARAM)hFont,1);
 }

void SaveLogFile(HWND Hwnd)
{ char *logfile, Buffer[DebugSize];
  FILE *outfile;
  int i,n;

  logfile=OpenFileGUI(Hwnd,NULL,"Logfile save as",
          "Text File (*.txt)\0*.txt\0Log File (*.log)\0*.log\0All files (*.*)\0*.*\0",1);

  if(logfile)
  {
    outfile=fopen(logfile,"w");

    if(outfile)
    {
      n=SendDlgItemMessage(Hwnd,IDC_DBG,LB_GETCOUNT,0,0);
      for(i=0;i<n;i++)
      { SendDlgItemMessage(Hwnd,IDC_DBG,LB_GETTEXT,i,(LPARAM)Buffer);
        fprintf(outfile,"%s\n",Buffer);
       }

      fclose(outfile);
     }
   }
 }

void CondDebug(int Level, const char *Format,...)
{ char Buffer[DebugSize];
  char *Prefix[]= {"",RPT_ERR_Prefix,RPT_WARN_Prefix,RPT_INFO_Prefix,RPT_DEBUG_Prefix,RPT_DEBUG_NoPrefix};
  int n;

  va_list args;
  va_start (args,Format);

  if(WarnLevel>=Level)
  {
    n=snprintf(Buffer,sizeof(Buffer),"%s",Prefix[Level]);
    vsnprintf(&Buffer[n],sizeof(Buffer)-n,Format,args);
    SendDlgItemMessage(DBGHwnd,IDC_DBG,LB_ADDSTRING,0,(LPARAM)Buffer);
   }

  va_end (args);
 }

void DumpBuffer(uint8_t *Buffer, uint16_t Size)
{ uint8_t j, Col=0;

  char LineBuf[HEXLINE*4+16];

  for(j=0;j<Size;j++)
    Col+=snprintf(LineBuf+Col,sizeof(LineBuf)-Col,"%02hhX ",Buffer[j]);

  LineBuf[Col]=0;

  CondDebug(RPT_DEBUG_NOPREFIX,LineBuf);
 }
