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
 * \file      BaseLogObject.h
 * \brief     Definition file for CBaseLogObject class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CBaseLogObject class
 */


#if !defined BASELOGOBJECT_H__INCLUDED_
#define BASELOGOBJECT_H__INCLUDED_

#include "DataTypes/Log_Datatypes.h"

class CBaseLogObject
{
private:

    CRITICAL_SECTION m_CritSection; // For thread synchronization
    int m_nCurrFileCnt;             // Keep the number of log file created
    CString m_omCurrLogFile;        // Current file of the cycle
    DWORD m_dTotalBytes;            // To keep the current file size

    //Find the name and size of the file which will be used for logging.
    //ie. file name which contains max file count
    void vGetNameAndSizeOfCurrentLogFile();
    DWORD dwGetFileSize(CString omFileName); // Get size of the file
    CString omAddGroupCountToFileName(int nCount, TCHAR FileName[]);
    CString omRemoveGroupCountFromFileName(CString FileName);
    void vSetNextFileName(void);

protected:
    //All log info
	SLOGINFO m_sLogInfo;

    //File pointer
    FILE* m_pLogFile;

    //Current trigger type
    ELOGTRIGGERSTATE m_CurrTriggerType;

	// To format the header 
	virtual void vFormatHeader(CString& omHeader) = 0;

	// To format the footer 
	virtual void vFormatFooter(CString& omFooter) = 0;

    void vWriteTextToFile(CString& om_LogText);

    // To copy specific data pertaining to the conrete class.
    virtual void Der_CopySpecificData(const CBaseLogObject* pouLogObjSrc) = 0;

    // Set config. data - additional tasks to be done by the concrete class
    virtual BYTE* Der_SetConfigData(BYTE* pvDataStream) = 0;
    // Get config. data - additional tasks to be done by the concrete class
    virtual BYTE* Der_GetConfigData(BYTE* pvDataStream) const = 0;
    // Buffer size calculation - additional tasks to be done by the concrete class
	virtual UINT Der_unGetBufSize(void) const = 0;

public:
    // Standard Default Constructor
	CBaseLogObject();

    // Destructor
	~CBaseLogObject();

    // Equal operator. THERE SHOULDN'T BE ANY EQUAL OPERATOR OVERLOADING FOR
    // THE DERIVED CLASSES
    CBaseLogObject& operator=(const CBaseLogObject& RefObj);

    // To do actions before logging starts
    BOOL bStartLogging(void);

    // To do actions before logging stop
    BOOL bStopLogging(void);

    // To log a string
    BOOL bLogString(CString& omString);

    // Enable / disable filter
    virtual void EnableFilter(BOOL bEnable) = 0;
    // Enable / disable logging
    void EnableLogging(BOOL bEnable);

    // Query - if logging is enable
    BOOL IsLoggingEnable(void);

    //Get the log info structure
    void GetLogInfo(SLOGINFO& sLoginfo) const;
    //Set Log info structure
    void SetLogInfo(const SLOGINFO& sLoginfo);

    // Set configuration data
    BYTE* SetConfigData(BYTE* pvDataStream);
    // Get configuration data
    BYTE* GetConfigData(BYTE* pvDataStream) const;
    // To get the total buffer size
	UINT unGetBufSize(void) const;

    // To get the ID
    UINT GetID(void);
};

#endif // BASELOGOBJECT_H__INCLUDED_
