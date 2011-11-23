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
 * \file      ExportLogFileDlg.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// CExportLogFileDlg dialog
#include "LogToExcel.h"

class CExportLogFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CExportLogFileDlg)

public:
	CExportLogFileDlg(ETYPE_BUS eBus, CWnd* pParent = NULL);   // standard constructor
	virtual ~CExportLogFileDlg();

// Dialog Data
	enum { IDD = IDD_DLG_EXPORT_LOG_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogBrowse();
	CString m_omStrLogFileName;
	afx_msg void OnBnClickedBtnCsvBrowse();
	CString m_omStrCSVFileName;
	afx_msg void OnBnClickedButtonSelectall();
	virtual BOOL OnInitDialog();
private: 
    ETYPE_BUS m_eBus;
    CHAR_ARRAY_20* m_pacFields;
    UINT m_unNoOfFileds;
	void vPopulateAvailableList();
	void vEnableDisableControls();

public:
	CListBox m_omAvailableList;
	CListBox m_omSelectedList;
	CButton m_omSelectAllFields;
	CButton m_omSelectOneField;
	CButton m_omRemoveOneField;
	CButton m_omRemoveAllFields;
	afx_msg void OnBnClickedConvert();
	afx_msg void OnBnClickedCbtnCancelexport();
	afx_msg void OnLbnSelchangeLstAvailable();
	afx_msg void OnLbnSelchangeLstSelected();
	afx_msg void OnBnClickedButtonSelectone();
	afx_msg void OnBnClickedButtonRemoveone();
	afx_msg void OnBnClickedButtonRemoveall();
};
