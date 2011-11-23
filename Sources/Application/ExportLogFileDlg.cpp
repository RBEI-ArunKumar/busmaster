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
 * \file      ExportLogFileDlg.cpp
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// ExportLogFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ExportLogFileDlg.h"
#include "Common.h"

IMPLEMENT_DYNAMIC(CExportLogFileDlg, CDialog)
CExportLogFileDlg::CExportLogFileDlg(ETYPE_BUS eBus, CWnd* pParent /*=NULL*/)
	: CDialog(CExportLogFileDlg::IDD, pParent)
	, m_omStrLogFileName(_T(""))
	, m_omStrCSVFileName(_T(""))
{
    m_eBus = eBus;
    if (m_eBus == CAN)
    {
        m_pacFields = (CHAR_ARRAY_20*) acFields_CAN;
        m_unNoOfFileds = defNO_OF_FIELDS_CAN;
    }
}

CExportLogFileDlg::~CExportLogFileDlg()
{
}

void CExportLogFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOG_FILE_NAME, m_omStrLogFileName);
	DDX_Text(pDX, IDC_EDIT_CSV_FILE_NAME, m_omStrCSVFileName);
	DDX_Control(pDX, IDC_LST_AVAILABLE, m_omAvailableList);
	DDX_Control(pDX, IDC_LST_SELECTED, m_omSelectedList);
	DDX_Control(pDX, IDC_BUTTON_SELECTALL, m_omSelectAllFields);
	DDX_Control(pDX, IDC_BUTTON_SELECTONE, m_omSelectOneField);
	DDX_Control(pDX, IDC_BUTTON_REMOVEONE, m_omRemoveOneField);
	DDX_Control(pDX, IDC_BUTTON_REMOVEALL, m_omRemoveAllFields);
}


BEGIN_MESSAGE_MAP(CExportLogFileDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_LOG_BROWSE, OnBnClickedBtnLogBrowse)
	ON_BN_CLICKED(IDC_BTN_CSV_BROWSE, OnBnClickedBtnCsvBrowse)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, OnBnClickedButtonSelectall)
	ON_BN_CLICKED(ID_CONVERT, OnBnClickedConvert)
	ON_BN_CLICKED(IDC_CBTN_CANCELEXPORT, OnBnClickedCbtnCancelexport)
	ON_LBN_SELCHANGE(IDC_LST_AVAILABLE, OnLbnSelchangeLstAvailable)
	ON_LBN_SELCHANGE(IDC_LST_SELECTED, OnLbnSelchangeLstSelected)
	ON_BN_CLICKED(IDC_BUTTON_SELECTONE, OnBnClickedButtonSelectone)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEONE, OnBnClickedButtonRemoveone)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEALL, OnBnClickedButtonRemoveall)
END_MESSAGE_MAP()


// CExportLogFileDlg message handlers
/*******************************************************************************
  Function Name  : OnBnClickedBtnLogBrowse
  Input(s)       : -
  Output         : -
  Functionality  : To show file selection dialog for log File
  Member of      : CExportLogFileDlg
  Author(s)      : S.Ramakrishnan
  Date Created   : 14.11.2006
  Modifications  : 
*******************************************************************************/


void CExportLogFileDlg::OnBnClickedBtnLogBrowse()
{
	 UpdateData( TRUE );

    // Create Save File Dialog with overwrite warning
    CFileDialog omSaveAsDlg( TRUE ,                 // Save File dialog
                             defSTR_LOG_FORMAT,     // Default Extension,
                             NULL,
                             OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                             defSTR_LOG_FORMAT_SPECIFIER,
                             NULL );
    // Set Title to Export
    omSaveAsDlg.m_ofn.lpstrTitle  = defSTR_LOG_DIALOG_CAPTION;
    // Show file save dialog
    if ( omSaveAsDlg.DoModal() == IDOK )
    {
        // Save the selected path
        m_omStrLogFileName = omSaveAsDlg.GetPathName();
        UpdateData( FALSE );
    }
}
/*******************************************************************************
  Function Name  : OnBnClickedBtnCsvBrowse
  Input(s)       : -
  Output         : -
  Functionality  : To show file selection dialog for CSV File
  Member of      : CExportLogFileDlg
  Author(s)      : S.Ramakrishnan
  Date Created   : 14.11.2006
  Modifications  : 
*******************************************************************************/

void CExportLogFileDlg::OnBnClickedBtnCsvBrowse()
{
	    UpdateData( TRUE );
    // Create Save File Dialog with overwrite warning
    CFileDialog omSaveAsDlg( FALSE,                 // Save File dialog
                             defSTR_XLS_FORMAT,     // Default Extension,
                             NULL,
                             OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                             defSTR_XLS_FORMAT_SPECIFIER,
                             NULL );
    // Set Title to Export
    omSaveAsDlg.m_ofn.lpstrTitle  = defSTR_CSV_DIALOG_CAPTION;
    // Show file save dialog
    if ( omSaveAsDlg.DoModal() == IDOK )
    {
        // Save the selected path
        m_omStrCSVFileName = omSaveAsDlg.GetPathName();
        UpdateData( FALSE );
    }

}
void CExportLogFileDlg::vEnableDisableControls()
{
	BOOL bSelectAllButtonEnable = FALSE;
    BOOL bRemoveAllButtonEnable = FALSE;
    BOOL bSelectOneButtonEnable = FALSE;
    BOOL bRemoveOneButtonEnable = FALSE;

    // Select Button
    if( m_omAvailableList.GetCount() != 0 && m_omAvailableList.GetCurSel() != -1 )
    {
        bSelectAllButtonEnable = TRUE;
		bSelectOneButtonEnable = TRUE;
    }

	if( m_omSelectedList.GetCount() != 0 && m_omSelectedList.GetCurSel() != -1 )
    {
        bRemoveAllButtonEnable = TRUE;
		bRemoveOneButtonEnable = TRUE;
    }
	// update Button Status
    m_omSelectAllFields.EnableWindow( bSelectAllButtonEnable );
    m_omSelectOneField.EnableWindow( bSelectOneButtonEnable );
    m_omRemoveOneField.EnableWindow( bRemoveOneButtonEnable );
    m_omRemoveAllFields.EnableWindow( bRemoveAllButtonEnable );
}
void CExportLogFileDlg::vPopulateAvailableList()
{    
	// Insert filed text
    for( UINT unIndex = 0; unIndex < m_unNoOfFileds; unIndex++)
    {
        m_omAvailableList.InsertString( unIndex, m_pacFields[unIndex]);
        m_omAvailableList.SetItemData( unIndex, unIndex );
    }
}

void CExportLogFileDlg::OnBnClickedButtonSelectall()
{    
	int nSelectedItem = m_omAvailableList.GetCurSel();
	DWORD_PTR nFieldIndex;
	if( nSelectedItem != -1 )
    {
		nSelectedItem=0;
		while(m_omAvailableList.GetCount()!=0)
		{
            nFieldIndex = m_omAvailableList.GetItemData( nSelectedItem );
            m_omAvailableList.DeleteString( nSelectedItem );
            m_omAvailableList.SetCurSel( nSelectedItem );
            // Add it in to the selected list
            int nNewItemIndex = m_omSelectedList.InsertString(
                                           m_omSelectedList.GetCount(),
                                           m_pacFields[ nFieldIndex ] );
            // Set Field index as item data
            m_omSelectedList.SetItemData( nNewItemIndex, nFieldIndex );
            // Set the selection
            m_omSelectedList.SetCurSel( nNewItemIndex );
        }
	}
	// Update UI buttons status
	vEnableDisableControls();
}
BOOL CExportLogFileDlg::OnInitDialog()
{   
    CString omTitle("Export Log File - ");
    if (m_eBus == CAN)
    {
        omTitle += _T("CAN");
    }
    SetWindowText(omTitle);
	CDialog::OnInitDialog();
	vPopulateAvailableList();
	vEnableDisableControls();
	return TRUE;  
}
void CExportLogFileDlg::OnBnClickedConvert()
{
	// Log File name and Excel File name

	
	CString strLogFileName = m_omStrLogFileName;  
	CString strCSVFileName = m_omStrCSVFileName;  


	// filename have been entered

	
	if( strLogFileName != "" && strCSVFileName != "" )
	{
		if( m_omSelectedList.GetCount() != 0 ) 
		{	
			//fields have been selected
			//open the files
            CLogToExcel oExport( strLogFileName.GetBuffer(MAX_PATH),strCSVFileName.GetBuffer(MAX_PATH), m_unNoOfFileds, m_pacFields);
			//set the selected fields
			oExport.fnSetSelectedFields( &(this->m_omSelectedList) );
			//convert log file to excel file
			if( !oExport.bConvert() )
            {
				MessageBox(EXPORTTOEXCEL_SYNTAXERROR,APPLICATION_NAME,MB_OK);
            }
			else
			{
				MessageBox(EXPORTTOEXCEL_SUCCESSMSG,APPLICATION_NAME,MB_OK);
				//OnOK();
			}
		}
		else
			    MessageBox(EXPORTTOEXCEL_FIELDSELECTIONERROR,APPLICATION_NAME,MB_OK);
	}
	else
				MessageBox(EXPORTTOEXCEL_FILEERROR,APPLICATION_NAME,MB_OK);


}
void CExportLogFileDlg::OnBnClickedCbtnCancelexport()
{
	OnCancel(); 
}

void CExportLogFileDlg::OnLbnSelchangeLstAvailable()
{
	// TODO: Add your control notification handler code here
	vEnableDisableControls();
}

void CExportLogFileDlg::OnLbnSelchangeLstSelected()
{
	vEnableDisableControls();
}

void CExportLogFileDlg::OnBnClickedButtonSelectone()
{
	// Get the selected item index
    int nSelectedItem = m_omAvailableList.GetCurSel();
    // If it is valid
    if( nSelectedItem != -1 )
    {
        // Get the filed index
        DWORD_PTR nFieldIndex = m_omAvailableList.GetItemData( nSelectedItem );
        if( nFieldIndex >= 0 && nFieldIndex < m_unNoOfFileds )
        {
            // Remove that item
            m_omAvailableList.DeleteString( nSelectedItem );
            // Set the focus to the next item
            if( nSelectedItem == m_omAvailableList.GetCount() )
            {
                nSelectedItem--;
            }
            m_omAvailableList.SetCurSel( nSelectedItem );
            // Add it in to the selected list
            int nNewItemIndex = m_omSelectedList.InsertString(
                                           m_omSelectedList.GetCount(),
                                           m_pacFields[ nFieldIndex ]);
            // Set Field index as item data
            m_omSelectedList.SetItemData( nNewItemIndex, nFieldIndex );
            // Set the selection
            m_omSelectedList.SetCurSel( nNewItemIndex );
            // Update UI buttons status
            vEnableDisableControls();
        }
    }

}

void CExportLogFileDlg::OnBnClickedButtonRemoveone()
{
	// Get the selected item index
    int nSelectedItem = m_omSelectedList.GetCurSel();
    // If it is a valid index
    if( nSelectedItem != -1 )
    {
        // Get the selected item data. That is index of fields array
        DWORD_PTR nFieldIndex = m_omSelectedList.GetItemData( nSelectedItem );
        // Remove the item fr the selected list
        m_omSelectedList.DeleteString( nSelectedItem );
        // Set the item selection to next
        if( nSelectedItem == m_omSelectedList.GetCount() )
        {
            nSelectedItem--;
        }
        m_omSelectedList.SetCurSel( nSelectedItem );

        // Insert this item in to the available list
        int nNewItemIndex = m_omAvailableList.GetCount();
        for( int nIndex = 0; nIndex < m_omAvailableList.GetCount(); nIndex++ )
        {
            if( m_omAvailableList.GetItemData( nIndex ) > nFieldIndex )
            {
                nNewItemIndex = nIndex;
                // Break the loop
                nIndex = m_omAvailableList.GetCount();
            }
        }
        if( nNewItemIndex != -1 )
        {
            m_omAvailableList.InsertString( nNewItemIndex,
                                            m_pacFields[ nFieldIndex ] );
        }
        else
        {
            nNewItemIndex = m_omAvailableList.AddString( 
                                        m_pacFields[ nFieldIndex ] );
        }
        // Set item data
        m_omAvailableList.SetItemData( nNewItemIndex, nFieldIndex );
        // Set the new item as selected
        m_omAvailableList.SetCurSel( nNewItemIndex );
        // Update UI Controls
        vEnableDisableControls();
    }
}

void CExportLogFileDlg::OnBnClickedButtonRemoveall()
{
	int nSelectedItem = m_omSelectedList.GetCurSel();
	DWORD_PTR nFieldIndex;
	if( nSelectedItem != -1 )
    {
		nSelectedItem=0;
		while(m_omSelectedList.GetCount()!=0)
		{
			// Get the selected item data. That is index of fields array
			nFieldIndex = m_omSelectedList.GetItemData( nSelectedItem );
			// Remove the item fr the selected list
			m_omSelectedList.DeleteString( nSelectedItem );
			// Set the item selection to next
			m_omSelectedList.SetCurSel( nSelectedItem );
			// Insert this item in to the available list
			int nNewItemIndex = m_omAvailableList.GetCount();
			for( int nIndex = 0; nIndex < m_omAvailableList.GetCount(); nIndex++ )
			{
				if( m_omAvailableList.GetItemData( nIndex ) > nFieldIndex )
				{
					nNewItemIndex = nIndex;
					// Break the loop
					nIndex = m_omAvailableList.GetCount();
				}
			}
			if( nNewItemIndex != -1 )
			{
				m_omAvailableList.InsertString( nNewItemIndex,
												m_pacFields[ nFieldIndex ] );
			}
			else
			{
				nNewItemIndex = m_omAvailableList.AddString( 
											m_pacFields[ nFieldIndex ] );
			}
			// Set item data
			m_omAvailableList.SetItemData( nNewItemIndex, nFieldIndex );
			// Set the new item as selected
			m_omAvailableList.SetCurSel( nNewItemIndex );
			// Update UI Controls
		}
    }
	vEnableDisableControls();
}
