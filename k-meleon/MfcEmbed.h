/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): 
 *   Chak Nanga <chak@netscape.com> 
 */

// mozembed.h : main header file for the MOZEMBED application
//

#define NIGHTLY

#ifndef _MFCEMBED_H
#define _MFCEMBED_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Plugins.h"
#include "Preferences.h"
#include "MenuParser.h"
#include "AccelParser.h"
#include "KmeleonConst.h"

#include "resource.h"       // main symbols


class CBrowserFrame;
class CProfileMgr;


/////////////////////////////////////////////////////////////////////////////
// CHiddenWnd:
// The Evil Hidden window is used to keep mozilla running while all browser
// windows are closed during a profile change
// Now also used to receive notification messages from the tray icon

class CHiddenWnd : public CFrameWnd {

public:
   BOOL  StayResident();
   void  GetPersist();

public:
   BOOL  m_bStayResident;
   BOOL  m_bPreloadWindow;
   BOOL  m_bPreloadStartPage;
   BOOL  m_bShowNow;

private:

   // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHiddenWnd)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL


	//{{AFX_MSG(CHiddenWnd)
	afx_msg void OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnClose();
   afx_msg void OnSetPersist(DWORD flags);
   afx_msg void OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
   afx_msg void OnNewWindow();
   afx_msg void ShowBrowser(char *URI=NULL);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
   BOOL           m_bPersisting;
   CBrowserFrame* m_pHiddenBrowser;

};


/////////////////////////////////////////////////////////////////////////////
// CMfcEmbedApp:
// See mozembed.cpp for the implementation of this class
//

class CMfcEmbedApp : public CWinApp,
                     public nsIObserver,
                     public nsIWindowCreator,
                     public nsSupportsWeakReference
{
public:
	CMfcEmbedApp();
	
    NS_DECL_ISUPPORTS
    NS_DECL_NSIOBSERVER
    NS_DECL_NSIWINDOWCREATOR

	CBrowserFrame* CreateNewBrowserFrame(PRUint32 chromeMask = nsIWebBrowserChrome::CHROME_ALL, 
							PRInt32 x = -1, PRInt32 y = -1, 
							PRInt32 cx = -1, PRInt32 cy = -1,
							PRBool bShowWindow = PR_TRUE);
	void RemoveFrameFromList(CBrowserFrame* pFrm, BOOL bCloseAppOnLastFrame = TRUE);
   nsresult OverrideComponents();

//   BOOL IsCmdLineSwitch(const char *pSwitch, BOOL bRemove = FALSE);
//   void ParseCmdLine();

   LPCTSTR GetMainWindowClassName();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcEmbedApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

	CObList m_FrameWndLst;

   CPlugins      plugins;
   CPreferences  preferences;
   CMenuParser   menus;
   CAccelParser  accel;

   HMENU          m_toolbarControlsMenu;
   CBrowserFrame* m_pMostRecentBrowserFrame; // the most recently used frame
   CBrowserFrame* m_pOpenNewBrowserFrame; // used by OnNewBrowser to preserve an initilaized frame

   // Implementation
public:
   //{{AFX_MSG(CMfcEmbedApp)
   afx_msg void OnNewBrowser();
   afx_msg void OnManageProfiles();
   afx_msg void OnPreferences();
   // NOTE - the ClassWizard will add and remove member functions here.
   // DO NOT EDIT what you see in these blocks of generated code !
   //}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL			InitializeProfiles();
	BOOL			CreateHiddenWindow();  
   nsresult    InitializePrefs();
   nsresult    InitializeWindowCreator();

private:
   CProfileMgr *m_ProfileMgr;
   BOOL        m_bAlreadyRunning;
   CString     m_sMainWindowClassName;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _MFCEMBED_H

