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

#ifndef __MENUPARSER_H__
#define __MENUPARSER_H__

#include "StdAfx.h"

#include "Parser.h"

class CMenuParser : public CParser{
protected:
  CMap<CString, LPCTSTR, CMenu *, CMenu *&> menus;
  CMap<CMenu *, CMenu *&, int, int&> menuOffsets;

  int Parse(char *p);

public:
	CMenuParser();
   CMenuParser(CString &filename);

	~CMenuParser();

   int Load(CString &filename);
   void Destroy();

   CMenu *GetMenu(TCHAR * menuName);
   int GetOffset(CMenu *menu);

   void SetCheck(UINT id, BOOL checked = TRUE);
};

#endif // __MENUPARSER_H__
