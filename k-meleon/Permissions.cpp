/*
*  Copyright (C) 2005 Dorian Boissonnade 
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
*
*/

#include "stdafx.h"
#include "Permissions.h"

//IMPLEMENT_DYNAMIC(CPermissionsDlg, CDialog)
CPermissionsDlg::CPermissionsDlg(char* type, CWnd* pParent /*=NULL*/)
	: CDialog(CPermissionsDlg::IDD, pParent)
{
	m_type = type;
	m_permissions = new CPermissions(type);
}

CPermissionsDlg::~CPermissionsDlg()
{
	while (!m_PermissionsList.IsEmpty())
	{
		delete(m_PermissionsList.GetHead());
		m_PermissionsList.RemoveHead();
	}

	delete m_permissions;
}

void CPermissionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PERMISSIONS, m_cPermissionsList);
}

void CPermissionsDlg::FillList()
{
	CPermission* permission;

	LVITEM lvItem;
	lvItem.mask		= LVIF_PARAM;
	lvItem.iSubItem	= 0;
	lvItem.state = 0;
	lvItem.stateMask = 0;
	lvItem.pszText = 0;
	lvItem.iItem	= 0xffff;

	POSITION pos = m_PermissionsList.GetHeadPosition();
	for (int i=0;i < m_PermissionsList.GetCount();i++)
	{
		lvItem.lParam = (LPARAM)pos;
		int index = m_cPermissionsList.InsertItem(&lvItem);
		if (index==-1) continue;

		permission = m_PermissionsList.GetNext(pos);

		USES_CONVERSION;
		ListView_SetItemText(m_cPermissionsList.GetSafeHwnd(), index, 0, (LPTSTR)A2CT(permission->m_host.get()))
		
		CString state;
		if (permission->m_state) 
			state.LoadString(IDS_AUTHORIZED);
		else
			state.LoadString(IDS_DENIED);

		ListView_SetItemText(m_cPermissionsList.GetSafeHwnd(), index, 1, state.GetBuffer(0))
	}

	RECT rect;
	m_cPermissionsList.GetClientRect(&rect);
	int width = (rect.right - rect.left);
	m_cPermissionsList.SetColumnWidth(0, width-80);
	m_cPermissionsList.SetColumnWidth(1, 80);

	m_cPermissionsList.SortItems(SortPermissionsList, (LPARAM) &m_PermissionsList);
}

int CALLBACK CPermissionsDlg::SortPermissionsList(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   CList<CPermission*>* pPermissionList = (CList<CPermission*>*) lParamSort;

   CPermission* permission1 = pPermissionList->GetAt((POSITION)lParam1);
   CPermission* permission2 = pPermissionList->GetAt((POSITION)lParam2);

   return strcmp(permission1->m_host.get(), permission2->m_host.get());
}

BEGIN_MESSAGE_MAP(CPermissionsDlg, CDialog)
	ON_BN_CLICKED(IDC_BLOCK, OnBnClickedBlock)
	ON_BN_CLICKED(IDC_ALLOW, OnBnClickedAllow)
	ON_BN_CLICKED(IDC_DELETE_PERMISSIONS, OnBnClickedDeletePermissions)
	ON_BN_CLICKED(IDC_DELETE_ALL_PERMISSIONS, OnBnClickedDeleteAllPermissions)
END_MESSAGE_MAP()


// Gestionnaires de messages CPermissionsDlg
BOOL CPermissionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cPermissionsList.InsertColumn(0, _T("Site"), LVCFMT_LEFT, 0, 0);
	m_cPermissionsList.InsertColumn(1, _T("State"), LVCFMT_LEFT, 0, 1);

	m_cPermissionsList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_permissions->list(m_PermissionsList);
	FillList();

	CString headstr;
	if (strcmp(m_type, "cookie") == 0)
		headstr.LoadString(IDS_PERMISSION_COOKIE);
	else if (strcmp(m_type, "image") == 0)
		headstr.LoadString(IDS_PERMISSION_IMAGE);
	else if (strcmp(m_type, "popup") == 0)
	{
		headstr.LoadString(IDS_PERMISSION_POPUP);
		CWnd* b = GetDlgItem(IDC_BLOCK);
		if (b) b->ShowWindow(SW_HIDE);
	}

	SetDlgItemText(IDC_HEAD, headstr);
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPermissionsDlg::OnNewPermission(int state)
{
	CString csUrl;
	GetDlgItemText(IDC_URL, csUrl);
	
	if (!csUrl.IsEmpty())
	{
		USES_CONVERSION;

		char* url = T2A(csUrl.GetBuffer(0));
		if (m_permissions->set(url, state))
		{
			while (!m_PermissionsList.IsEmpty())
			{
				delete(m_PermissionsList.GetHead());
				m_PermissionsList.RemoveHead();
			}
			m_cPermissionsList.DeleteAllItems();

			// Getting the complete list again is I think the safer way.
			m_permissions->list(m_PermissionsList);
			FillList();
		/*
			CPermission* permission = new CPermission(url, 0);
			POSITION pos = m_PermissionsList.AddHead(permission);
			
			LVITEM lvItem;
			lvItem.mask		= LVIF_PARAM;
			lvItem.iSubItem	= 0;
			lvItem.state = 0;
			lvItem.stateMask = 0;
			lvItem.pszText = 0;
			lvItem.iItem	= 0xffff;
			lvItem.lParam = (LPARAM)pos;
			int index = m_cPermissionsList.InsertItem(&lvItem);
			ListView_SetItemText(m_cPermissionsList.GetSafeHwnd(), index, 0, permission->m_host)
			char* state;
			if (permission->m_state) 
				state = _T("Authorized");
			else	
				state = _T("Denied");

			ListView_SetItemText(m_cPermissionsList.GetSafeHwnd(), index, 1, state)*/
		}
		else
			AfxMessageBox(IDS_FAILED_PERMISSION, MB_OK| MB_ICONERROR);
	}

}
void CPermissionsDlg::OnBnClickedBlock()
{
	OnNewPermission(0);
}

void CPermissionsDlg::OnBnClickedAllow()
{
	OnNewPermission(1);
}

void CPermissionsDlg::OnBnClickedDeletePermissions()
{
	UINT uSelectedCount = m_cPermissionsList.GetSelectedCount();
	int nItem = -1;

	if (uSelectedCount>0)
	{
		UINT i;
		for (i=0;i < uSelectedCount;i++)
		{
			nItem = m_cPermissionsList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			POSITION p = (POSITION)m_cPermissionsList.GetItemData(nItem);
			CPermission* permission = m_PermissionsList.GetAt(p);
			
			if (!m_permissions->del(permission))
			{
				AfxMessageBox(IDS_FAILED_DELETE_PERMISSION,MB_OK|MB_ICONERROR);
				return;
			}
		
			m_PermissionsList.RemoveAt(p);
			delete permission;
		}

		for (i=0;i < uSelectedCount;i++)
		{
			nItem = m_cPermissionsList.GetNextItem(-1, LVNI_SELECTED);
			m_cPermissionsList.DeleteItem(nItem);
		}
	}
}

void CPermissionsDlg::OnBnClickedDeleteAllPermissions()
{
	if (AfxMessageBox(IDS_CONFIRM_DELETEALLPERMISSIONS, MB_OKCANCEL | MB_ICONWARNING) == IDOK) 
	{
		while (!m_PermissionsList.IsEmpty())
		{
			CPermission* permission = m_PermissionsList.GetHead();
			m_permissions->del(permission);
			delete(permission);
			m_PermissionsList.RemoveHead();
		}
		m_cPermissionsList.DeleteAllItems();
	}
}