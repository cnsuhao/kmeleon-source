/*
*  Copyright (C) 2000 Christophe Thibault
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

#if !defined(AFX_KMELEON_H__F83C8E0B_F33E_11D2_A713_0090274409AC__INCLUDED_)
#define AFX_KMELEON_H__F83C8E0B_F33E_11D2_A713_0090274409AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define FIRST_FAVORITE_COMMAND	0xe00
#define LAST_FAVORITE_COMMAND	0xfff

#include "resource.h"       // main symbols

#include "Preferences.h"
#include "Plugins.h"
#include "MenuParser.h"

/////////////////////////////////////////////////////////////////////////////
// CKMeleonApp:
//

class CKMeleonApp :	public CWinApp //,	public CBCGWorkspace
{
public:
	CKMeleonApp();
	~CKMeleonApp();

	CPreferences   preferences;
  CPlugins       plugins;

  CMenuParser    menus;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKMeleonApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	virtual void LoadCustomState ();
	virtual void SaveCustomState ();

	void createNewBrowser();

  virtual BOOL OnIdle( LONG count );

// Implementation
  afx_msg void OnFileNew();
	//{{AFX_MSG(CKMeleonApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CKMeleonApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KMELEON_H__F83C8E0B_F33E_11D2_A713_0090274409AC__INCLUDED_)
