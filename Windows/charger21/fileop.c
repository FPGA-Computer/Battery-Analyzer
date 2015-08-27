/*
 * Created: July-24-13, 8:59:25 AM
 *  Author: K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
*/

#include "charger.h"

char *OpenFileGUI(HWND hwnd,char *Drive, char *Title, LPCTSTR LpstrFilter, char Save)
{ OPENFILENAME DirBox;
  static char Path[MAXPATH]={0}, PrevDrive[MAXPATH]={0};

  if(!LpstrFilter || (Drive && _stricmp(PrevDrive,Drive)))
  {
    Path[0]=0;													// Reset path if drives changed
    lstrcpyn(PrevDrive,Drive,sizeof(PrevDrive));
   }

  ZeroMemory(&DirBox, sizeof(DirBox));
  DirBox.lStructSize = sizeof(DirBox);
  DirBox.hwndOwner = hwnd;
  DirBox.lpstrFile = Path;
  DirBox.lpstrInitialDir = Drive;
  DirBox.lpstrFilter = LpstrFilter;
  DirBox.nMaxFile = MAX_PATH;
  DirBox.lpstrTitle = Title;
  DirBox.Flags = OFN_EXPLORER | OFN_LONGNAMES /* | OFN_NOCHANGEDIR*/ ;

  if (Save)
  { DirBox.Flags |= OFN_OVERWRITEPROMPT;
    return((GetSaveFileName(&DirBox))?Path:NULL);
   } 
  else
    return((GetOpenFileName(&DirBox))?Path:NULL);
 }

