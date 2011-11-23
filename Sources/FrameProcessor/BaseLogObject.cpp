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
 * \file      BaseLogObject.cpp
 * \brief     Source file for CBaseLogObject class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for CBaseLogObject class
 */


#include "FrameProcessor_stdafx.h"
#include "BaseLogObject.h"            // For CBaseLogObject class declaration

const int SIZE_CHAR = sizeof(TCHAR);
#define ENOENT          2

#define DEFAULT_FILE_SIZE_IN_MBYTES       10 //MB 
#define MB_VALUE                         1048576

const UINT DEFAULT_FILE_SIZE_IN_BYTES = DEFAULT_FILE_SIZE_IN_MBYTES * MB_VALUE;

#define MAX_LOG_FILE_IN_GRP              9
#define FILE_COUNT_STR                    _T("_%d")

/**
 * Contructor
 */
CBaseLogObject::CBaseLogObject()                       
{
    // Initialise the logging block
    m_sLogInfo.vClear();

    m_omCurrLogFile = _T("");

    m_pLogFile = NULL;
    m_CurrTriggerType = NONE;
    m_nCurrFileCnt = 0;
    m_dTotalBytes = 0;
    InitializeCriticalSection(&m_CritSection);
}

/**
 * Destructor
 */
CBaseLogObject::~CBaseLogObject()
{
    DeleteCriticalSection(&m_CritSection);
}

/**
 * Equal operator
 */
CBaseLogObject& CBaseLogObject::operator=(const CBaseLogObject& RefObj)
{
    RefObj.GetLogInfo(m_sLogInfo);
    m_omCurrLogFile = RefObj.m_omCurrLogFile;
    m_nCurrFileCnt = RefObj.m_nCurrFileCnt;
    m_dTotalBytes = RefObj.m_dTotalBytes;

    Der_CopySpecificData(&RefObj);

    return *this;
}

/**
 * Enable / disable logging
 */
void CBaseLogObject::EnableLogging(BOOL bEnable)
{
    m_sLogInfo.m_bEnabled = bEnable;
}

/**
 * Query - if logging is enable
 */
BOOL CBaseLogObject::IsLoggingEnable(void)
{
    return m_sLogInfo.m_bEnabled;
}

/**
 * Get the log info structure
 */
void CBaseLogObject::GetLogInfo(SLOGINFO& sLoginfo) const
{
    sLoginfo = m_sLogInfo;
}

/**
 * Set Log info structure
 */
void CBaseLogObject::SetLogInfo(const SLOGINFO& sLoginfo)
{
    m_sLogInfo = sLoginfo;

    //Now if the name of the file is changed then only search for other file 
    //in this file cycle else curr file name and size will be same 
    vGetNameAndSizeOfCurrentLogFile();
}

/**
 * Set configuration data
 */
BYTE* CBaseLogObject::SetConfigData(BYTE* pvDataStream)
{
    BYTE* pbSStream = pvDataStream;
    pbSStream = m_sLogInfo.pbSetConfigData(pbSStream);
    pbSStream = Der_SetConfigData(pbSStream);

    // The default value of current log file should be the log file name
    m_omCurrLogFile = m_sLogInfo.m_sLogFileName;

    return pbSStream;
}

/**
 * Get configuration data
 */
BYTE* CBaseLogObject::GetConfigData(BYTE* pvDataStream) const
{
    BYTE* pbTStream = pvDataStream;
    pbTStream = m_sLogInfo.pbGetConfigData(pbTStream);
    pbTStream = Der_GetConfigData(pbTStream);

    return pbTStream;
}

/**
 * To get the total buffer size
 */
UINT CBaseLogObject::unGetBufSize(void) const
{
	return (m_sLogInfo.unGetSize() + Der_unGetBufSize());
}

/**
 * To get the ID
 */
UINT CBaseLogObject::GetID(void)
{
    return m_sLogInfo.m_ushID;
}

/**
 * To log a string
 */
BOOL CBaseLogObject::bLogString(CString& omStr)
{
    if (m_sLogInfo.m_bEnabled == FALSE) 
    {
        return FALSE;
    }

    if (NULL == m_pLogFile)
    {
        ASSERT(FALSE);
        return FALSE;
    }

    EnterCriticalSection(&m_CritSection);
	_ftprintf(m_pLogFile, omStr.GetBuffer(MAX_PATH));
    LeaveCriticalSection(&m_CritSection);

    return TRUE;
}

void CBaseLogObject::vWriteTextToFile(CString& om_LogText)
{
    DWORD dwBytes2Write = om_LogText.GetLength()* SIZE_CHAR; //no of bytes

    if ((m_dTotalBytes + dwBytes2Write) >= DEFAULT_FILE_SIZE_IN_BYTES) //megabytes
    {
        //triggering type is saved to be used for next file
        ELOGTRIGGERSTATE LastTriggerType = m_CurrTriggerType;
        bStopLogging();
        //Set the next file name of the series
        vSetNextFileName();
        //If file is append mode then change it to overwrite mode bfore startlogging
        //So that old data will be deleted
        eMode eFileMode = m_sLogInfo.m_eFileMode;
        //The file mode is changed to overwrite so that when the same file
        //is opened in cycle by bStartLogging(), it should overwrite
        if (m_sLogInfo.m_eFileMode == APPEND_MODE)
        {
            m_sLogInfo.m_eFileMode = OVERWRITE_MODE;
        }
        bStartLogging();
        //Save the triggering type
        m_CurrTriggerType = LastTriggerType;
        //Revert back to the original mode
        m_sLogInfo.m_eFileMode = eFileMode;
        //theApp.GetDefaultLogFile();
        m_dTotalBytes = 0;
    }

    EnterCriticalSection(&m_CritSection);
	_ftprintf(m_pLogFile, om_LogText.GetBuffer(MAX_PATH));
    LeaveCriticalSection(&m_CritSection);

    //Get the file size
    m_dTotalBytes += dwBytes2Write;
}

void CBaseLogObject::vSetNextFileName(void)
{
    //If it is not default file then remove "_File count no."
    if (_tcscmp(m_omCurrLogFile.GetBuffer(MAX_PATH), m_sLogInfo.m_sLogFileName) &&
        m_omCurrLogFile.GetLength() > 2)
    {
        m_omCurrLogFile.Left(m_omCurrLogFile.GetLength() - 2);
    }

    if ( ++m_nCurrFileCnt > MAX_LOG_FILE_IN_GRP)
    {
        //If it reaches max start again
        m_nCurrFileCnt = 0;
        m_omCurrLogFile = m_sLogInfo.m_sLogFileName;
    }
    else
    {
        //Add the file count with "_"
        m_omCurrLogFile = omAddGroupCountToFileName(m_nCurrFileCnt, 
                                                m_sLogInfo.m_sLogFileName);
    }
}

CString CBaseLogObject::omAddGroupCountToFileName(int nCount, TCHAR sFileName[])
{
    CString omFileName = sFileName;
    //Add the file count with "_"
    CString omStrAdd;
    omStrAdd.Format(FILE_COUNT_STR, nCount);
    //Remove Extension
    int nExt = omFileName.Find(L'.');
    CString omExt = omFileName.Right(omFileName.GetLength() - nExt);
    omFileName = omFileName.Left(nExt);
    //Now add the file count
    omFileName = omFileName + omStrAdd + omExt;
    return omFileName;
}

CString CBaseLogObject::omRemoveGroupCountFromFileName(CString FileName)
{
    //Remove Extension
    int nExt = FileName.Find(L'.');
    CString omExt = FileName.Right(FileName.GetLength() - nExt);
    FileName = FileName.Left(nExt);
    //Now remove last 2 chars
    FileName = FileName.Left(FileName.GetLength() - 2);
    FileName += omExt;
    return FileName;
}

/**
 * \brief Start logging
 * \req RS_12_23 Start logging
 *
 * To do actions before logging starts.
 */
BOOL CBaseLogObject::bStartLogging(void)
{
    BOOL bResult = FALSE;
    if ((m_pLogFile == NULL) && (m_sLogInfo.m_bEnabled))
    {
        // This function should be called every time logging is started
        m_CurrTriggerType = m_sLogInfo.m_sLogTrigger.m_unTriggerType;
        TCHAR Mode[2] =  _T(" ");
		Mode[0] = (m_sLogInfo.m_eFileMode == APPEND_MODE) ? L'a' : L'w';
        EnterCriticalSection(&m_CritSection);
        m_pLogFile = _tfopen(m_omCurrLogFile, Mode);
        //In case user has deleted the content of the file
        m_dTotalBytes = dwGetFileSize(m_omCurrLogFile);
        if (m_pLogFile != NULL)
        {
            CString omHeader = _T("");
			vFormatHeader(omHeader);
            _ftprintf(m_pLogFile,  _T("%s"), omHeader.GetBuffer(MAX_PATH));
            bResult = TRUE;
        }
        LeaveCriticalSection(&m_CritSection);
    }
    return bResult;
}

/**
 * \brief Stop logging
 * \req RS_12_24 Stop logging
 *
 * To do actions before logging stop.
 */
BOOL CBaseLogObject::bStopLogging()
{
	BOOL bResult = FALSE;

    if ((m_pLogFile != NULL) && (m_sLogInfo.m_bEnabled))
    {
        m_CurrTriggerType = NONE;

		CString omFooter = _T("");
		vFormatFooter(omFooter);

		_ftprintf(m_pLogFile,  _T("%s\n"), omFooter.GetBuffer(MAX_PATH));
        fclose(m_pLogFile);
        m_pLogFile = NULL;
        bResult = TRUE;
    }

	return bResult;
}

#define MIN_NAME_LENGTH    5

/**
 * Find the name and size of the file which will be used for logging.
 * ie. file name which contains max file count
 */
void CBaseLogObject::vGetNameAndSizeOfCurrentLogFile()
{
    //Two conditions to search for the current file present at the main file 
    //path folder
    //1.If logging is initialised
    //2.if file name is modified ie.m_omCurrLogFile belongs to diff. main file cycle

    //Find file only if current file is not same as main file
    if (m_omCurrLogFile.Compare(m_sLogInfo.m_sLogFileName))
    {
        //Check if the curr file doesn't belong to main file group
        if (m_omCurrLogFile.GetLength() >= MIN_NAME_LENGTH)
        {
            CString omTempFile = m_omCurrLogFile.Left(m_omCurrLogFile.GetLength() - 2);
            if ( !omTempFile.Compare(m_sLogInfo.m_sLogFileName) )
            {
                //If curr file belongs to main file group then do nothing
                return;
            }
        }

        if (ENOENT != _taccess(m_sLogInfo.m_sLogFileName, 0))
        {
            CString strNextFile = omAddGroupCountToFileName(1, m_sLogInfo.m_sLogFileName);
            if (ENOENT != _taccess(strNextFile, 0))
            {
                //If first file is not present then current file is main file
                m_omCurrLogFile = m_sLogInfo.m_sLogFileName;
            }
            else
            {
                //Search for the file in a group with highest count
                m_nCurrFileCnt = 0;
                for (int i = 2; (i <= MAX_LOG_FILE_IN_GRP) && (m_nCurrFileCnt != 0); i++)
                {
                    strNextFile = omAddGroupCountToFileName(i, m_sLogInfo.m_sLogFileName);
                    if (ENOENT != _taccess(strNextFile, 0))
                    {
                        //If this file is not found means the last file is target file
                        m_nCurrFileCnt = --i;
                    }
                }
                m_omCurrLogFile = omAddGroupCountToFileName(m_nCurrFileCnt, 
                                                            m_sLogInfo.m_sLogFileName);
            }
            //Now find the size of the file
            m_dTotalBytes = dwGetFileSize(m_omCurrLogFile);
        }
        else
        {
            //If main file is not present initialise the current file 
            //with main file
            m_omCurrLogFile = m_sLogInfo.m_sLogFileName;
            m_dTotalBytes = 0;
            m_nCurrFileCnt = 0;
        }
    }
}

/**
 * Get size of the file
 */
DWORD CBaseLogObject::dwGetFileSize(CString omFileName)
{
    DWORD dwFileSize = 0;
    CFile omfile;
    CFileException omFileException;
    if (omfile.Open(omFileName, CFile::modeRead | CFile::shareExclusive, &omFileException))
    {
        dwFileSize = (DWORD) (omfile.GetLength());
        omfile.Close();
    }
    return dwFileSize;
}
