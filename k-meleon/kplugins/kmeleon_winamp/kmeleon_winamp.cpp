/*
*  Copyright (C) 2000 Brian Harris
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2, or (at your option)
*  any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
// kmeleon_winamp.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "frontend.h"

#define PLUGIN_NAME "Winamp Plugin"

#define KMELEON_PLUGIN_EXPORTS
#include "../kmeleon_plugin.h"

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
   switch (ul_reason_for_call)
   {
   case DLL_PROCESS_ATTACH:
   case DLL_THREAD_ATTACH:
   case DLL_THREAD_DETACH:
   case DLL_PROCESS_DETACH:
      break;
   }

   return TRUE;
}

int Init();
void Create(HWND parent);
void Config(HWND parent);
void Quit();
void DoMenu(HMENU menu, char *param);
void DoRebar(HWND rebarWnd);
int DrawBitmap(DRAWITEMSTRUCT *dis);

long DoMessage(const char *to, const char *from, const char *subject, long data1, long data2);

kmeleonPlugin kPlugin = {
   KMEL_PLUGIN_VER,
   PLUGIN_NAME,
   DoMessage
};

kmeleonFunctions *kFuncs;

long DoMessage(const char *to, const char *from, const char *subject, long data1, long data2)
{
   if (to[0] == '*' || stricmp(to, kPlugin.dllname) == 0) {
      if (stricmp(subject, "Init") == 0) {
         Init();
      }
      else if (stricmp(subject, "Create") == 0) {
         Create((HWND)data1);
      }
      else if (stricmp(subject, "Config") == 0) {
         Config((HWND)data1);
      }
      else if (stricmp(subject, "Quit") == 0) {
         Quit();
      }
      else if (stricmp(subject, "DoMenu") == 0) {
         DoMenu((HMENU)data1, (char *)data2);
      }
      else if (stricmp(subject, "DoRebar") == 0) {
         DoRebar((HWND)data1);
      }
      else if (stricmp(subject, "DrawBitmap") == 0) {
         if (data1 && data2) {
            *(int *)data2 = DrawBitmap((DRAWITEMSTRUCT *)data1);
         }
      }
      else return 0;

      return 1;
   }
   return 0;
}


HIMAGELIST himlHot;

UINT commandIDs;

const numCommands = 5;

DWORD commandTable[numCommands] = {
   WINAMP_BUTTON1, /* prev */
      WINAMP_BUTTON2, /* play */
      WINAMP_BUTTON3, /* pause */
      WINAMP_BUTTON4, /* stop */
      WINAMP_BUTTON5  /* next */
};

int Init(){

   kFuncs = kPlugin.kFuncs;

   // allocate some ids
   commandIDs = kFuncs->GetCommandIDs(numCommands);

   himlHot = ImageList_LoadImage(kPlugin.hDllInstance, MAKEINTRESOURCE(IDB_TOOLBAR_BUTTONS), 12, numCommands, CLR_DEFAULT, IMAGE_BITMAP, LR_DEFAULTCOLOR );

   return true;
}

WNDPROC KMeleonWndProc;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void Create(HWND parent){
   KMeleonWndProc = (WNDPROC) GetWindowLong(parent, GWL_WNDPROC);
   SetWindowLong(parent, GWL_WNDPROC, (LONG)WndProc);
}

void Config(HWND parent){
   MessageBox(parent, "This plugin brought to you by the letter B", "Winamp-Kmeleon plugin", 0);
}

void Quit(){
   ImageList_Destroy(himlHot);
}

void DoMenu(HMENU menu, char *param){
   AppendMenu(menu, MF_STRING, commandIDs + 1, "Play");
   AppendMenu(menu, MF_STRING, commandIDs + 2, "Pause");
   AppendMenu(menu, MF_STRING, commandIDs + 3, "Stop");
   AppendMenu(menu, MF_SEPARATOR, 0, "-");
   AppendMenu(menu, MF_STRING, commandIDs + 0, "Previous");
   AppendMenu(menu, MF_STRING, commandIDs + 4, "Next");
}

void DoRebar(HWND rebarWnd){
   TBBUTTON buttons[numCommands];
   int i;
   for (i=0; i<numCommands; i++){
      buttons[i].iBitmap = i;
      buttons[i].idCommand = commandIDs + i;
      buttons[i].iString = i + 1;

      buttons[i].dwData = 0;
      buttons[i].fsState = TBSTATE_ENABLED;
      buttons[i].fsStyle = TBSTYLE_BUTTON;
      buttons[i].bReserved[0] = 0;
   };

   DWORD dwStyle = 0x40 | /*the 40 gets rid of an ugly border on top.  I have no idea what flag it corresponds to...*/
      CCS_NOPARENTALIGN | CCS_NORESIZE |
      TBSTYLE_FLAT | TBSTYLE_TRANSPARENT /* | TBSTYLE_AUTOSIZE | TBSTYLE_LIST | TBSTYLE_TOOLTIPS */;

   // Create the toolbar control to be added.
   HWND hwndTB = CreateToolbarEx(rebarWnd, dwStyle,
      /*id*/ 200,
      /*nBitmaps*/ 2,
      /*hBMInst*/ kPlugin.hDllInstance,
      /*wBMID*/ IDB_TOOLBAR_BUTTONS,
      /*lpButtons*/ buttons,
      /*iNumButtons*/ numCommands,
      /*dxButton*/ 12,
      /*dyButton*/ 12,
      /*dxBitmap*/ 12,
      /*dyBitmap*/ 12,
      /*uStructSize*/ sizeof(TBBUTTON)
      );

   if (!hwndTB){
      MessageBox(NULL, "Failed to create winamp toolbar", NULL, 0);
      return;
   }

   // Register the band name and child hwnd
    kFuncs->RegisterBand(hwndTB, "Winamp", TRUE);
    
   //SetWindowText(hwndTB, "Winamp");

   SendMessage(hwndTB, TB_SETHOTIMAGELIST, 0, (LPARAM)himlHot);

   // Get the height of the toolbar.
   DWORD dwBtnSize = SendMessage(hwndTB, TB_GETBUTTONSIZE, 0,0);

   REBARBANDINFO rbBand;
   rbBand.cbSize = sizeof(REBARBANDINFO);  // Required
   rbBand.fMask  = /*RBBIM_TEXT |*/
      RBBIM_STYLE | RBBIM_CHILD  | RBBIM_CHILDSIZE |
      RBBIM_SIZE | RBBIM_IDEALSIZE;

   rbBand.fStyle = RBBS_CHILDEDGE | RBBS_FIXEDBMP | RBBS_VARIABLEHEIGHT;
   rbBand.lpText     = "Winamp";
   rbBand.hwndChild  = hwndTB;
   rbBand.cxMinChild = 0;
   rbBand.cyMinChild = HIWORD(dwBtnSize);
   rbBand.cyIntegral = 1;
   rbBand.cyMaxChild = rbBand.cyMinChild;
   rbBand.cxIdeal    = LOWORD(dwBtnSize) * numCommands;
   rbBand.cx         = rbBand.cxIdeal;

   // Add the band that has the toolbar.
   SendMessage(rebarWnd, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);

   /*
   UINT bandPos = SendMessage(rebarWnd, RB_GETBANDCOUNT, (WPARAM)0, (LPARAM)0) - 1;
   SendMessage(rebarWnd, RB_MINIMIZEBAND, (WPARAM)bandPos, (LPARAM)0);
   SendMessage(rebarWnd, RB_MAXIMIZEBAND, (WPARAM)bandPos, (LPARAM)true);
   */
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
   if (message == WM_COMMAND){
      WORD command = LOWORD(wParam);
      if (command >= commandIDs && command < (commandIDs + numCommands)){
         HWND hwndWinamp = FindWindow("Winamp v1.x",NULL);
         SendMessage(hwndWinamp, WM_COMMAND, commandTable[command - commandIDs], 0);
         return true;
      }
   }
   return CallWindowProc(KMeleonWndProc, hWnd, message, wParam, lParam);
}

int DrawBitmap(DRAWITEMSTRUCT *dis)
{
   short position = dis->itemID - commandIDs;
   int top = (dis->rcItem.bottom - dis->rcItem.top - 12) / 2;
   top += dis->rcItem.top;

   ImageList_Draw(himlHot, position, dis->hDC, dis->rcItem.left, top, ILD_TRANSPARENT);
   return 14;
}

// so it doesn't munge the function name
extern "C" {

   KMELEON_PLUGIN kmeleonPlugin *GetKmeleonPlugin() {
      return &kPlugin;
   }

}
