/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      ConfigDialogsDIL.h
 * \brief     Definition file for CConfigDialogsDILApp class.
 * \authors   Ratnadip Choudhury, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CConfigDialogsDILApp class.
 */

// ConfigDialogsDIL.h : main header file for the ConfigDialogsDIL DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'CongDlg_stdafx.h' before including this file for PCH
#endif

#include "ConfigDlg_resource.h"		// main symbols


// CConfigDialogsDILApp
// See ConfigDialogsDIL.cpp for the implementation of this class
//

class CConfigDialogsDILApp : public CWinApp
{
public:
	CConfigDialogsDILApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
