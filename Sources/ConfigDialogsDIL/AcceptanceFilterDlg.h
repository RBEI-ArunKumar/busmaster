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
 * \file      AcceptanceFilterDlg.h
 * \brief     This file contain the declaration of CAcceptanceFilterDlg class
 * \authors   Amitesh Bharti, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the declaration of CAcceptanceFilterDlg class
 */
#if !defined(AFX_ACCEPTANCEFILTERDLG_H__80C9C246_59BF_11D7_80AD_0010B599CE39__INCLUDED_)
#define AFX_ACCEPTANCEFILTERDLG_H__80C9C246_59BF_11D7_80AD_0010B599CE39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AcceptanceFilterDlg.h : header file
//
#include "ConfigDlg_resource.h"
/////////////////////////////////////////////////////////////////////////////
// CAcceptanceFilterDlg dialog

class CAcceptanceFilterDlg : public CDialog//CPropertyPage
{
    //DECLARE_DYNCREATE(CAcceptanceFilterDlg)
// Construction
public:
    // standard constructor
    CAcceptanceFilterDlg(CWnd* pParent = NULL, 
                         PSCONTROLER_DETAILS psAccFilterInfo = NULL); 
    virtual ~CAcceptanceFilterDlg();
// Dialog Data
    //{{AFX_DATA(CAcceptanceFilterDlg)
    enum { IDD = IDD_DLG_ACCEPTANCE };
    CRadixEdit  m_omEditAccMaskByte4;
    CRadixEdit  m_omEditAccMaskByte3;
    CRadixEdit  m_omEditAccMaskByte2;
    CRadixEdit  m_omEditAccMaskByte1;
    CRadixEdit  m_omEditAccCodeByte4;
    CRadixEdit  m_omEditAccCodeByte3;
    CRadixEdit  m_omEditAccCodeByte2;
    CRadixEdit  m_omEditAccCodeByte1;
    CString m_omStrAccCodeByte1;
    CString m_omStrAccCodeByte2;
    CString m_omStrAccCodeByte3;
    CString m_omStrAccCodeByte4;
    CString m_omStrAccMaskByte1;
    CString m_omStrAccMaskByte2;
    CString m_omStrAccMaskByte3;
    CString m_omStrAccMaskByte4;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAcceptanceFilterDlg)
    public:
    virtual void OnOK();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CAcceptanceFilterDlg)
    afx_msg void OnCbtnAccetanceOk();
    virtual BOOL OnInitDialog();
    afx_msg void OnMaxtextEditAcceptanceCode1();
    afx_msg void OnMaxtextEditAcceptanceCode2();
    afx_msg void OnMaxtextEditAcceptanceCode3();
    afx_msg void OnMaxtextEditAcceptanceCode4();
    afx_msg void OnMaxtextEditAcceptanceMask1();
    afx_msg void OnMaxtextEditAcceptanceMask2();
    afx_msg void OnMaxtextEditAcceptanceMask3();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    BOOL m_bAccFilterMode;
    PSCONTROLER_DETAILS m_psControllerInfo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCEPTANCEFILTERDLG_H__80C9C246_59BF_11D7_80AD_0010B599CE39__INCLUDED_)
