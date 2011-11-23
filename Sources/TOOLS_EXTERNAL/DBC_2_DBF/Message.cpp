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
 * \file      Message.cpp
 * \brief     Implementation file for the Message class.
 * \author    Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the Message class.
 */

#include "StdAfx.h"
#include "App.h"
#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
 * \brief Constructor
 */
CMessage::CMessage()
{
    m_acName[0] = '\0';
    m_txNode = "\0";
    m_cDataFormat = CSignal::SIG_DF_INTEL;
    m_cFrameFormat = MSG_FF_STANDARD;
    m_ucLength = 8;
    m_ucNumOfSignals = 1;
    m_uiMsgID = 0;
    m_listSignals.RemoveAll();
}

/**
 * \brief Destructor
 */
CMessage::~CMessage()
{
    if(!m_listSignals.IsEmpty())
    {
        m_listSignals.RemoveAll();
    }
}

/**
 * \brief overloaded operator =
 */
CMessage& CMessage::operator=(CMessage& message)
{
    // if there are some elements in the signal list clear them first
    if(!m_listSignals.IsEmpty())
    {
        m_listSignals.RemoveAll();
    }

    // now copy the other elements of the new message to this
    strcpy(m_acName,message.m_acName);
    m_txNode = message.m_txNode;
    m_cDataFormat = message.m_cDataFormat;
    m_cFrameFormat = message.m_cFrameFormat;
    m_ucLength = message.m_ucLength;
    m_ucNumOfSignals = message.m_ucNumOfSignals;
    m_uiMsgID = message.m_uiMsgID;
    m_listSignals.AddTail(&message.m_listSignals);
    return (*this);
}

/**
 * \brief Extracts the message data
 *
 * Extracts the message data from the given Line and populates
 * the message structure.
 */
int CMessage::Format(char *pcLine)
{
    char* pcToken;
    // get the MSG ID
    pcToken = strtok(pcLine," :");
    m_uiMsgID = strtoul(pcToken, NULL, 0);

    // get the message name
    pcToken = strtok(NULL," :");
    strcpy(m_acName,pcToken);

    // set the message length
    pcToken = strtok(NULL,_T(" :"));
    m_ucLength = (unsigned char)atoi(pcToken);
    CConverter::ucMsg_DLC = m_ucLength; 

    //get the Tx'ing Node Name
    pcToken = strtok(NULL,_T(" :\n"));
    if(strcmp(pcToken,_T("Vector__XXX")))
        m_txNode = pcToken;
    else
        m_txNode = _T("");

    // set the Data format
    m_cDataFormat = CSignal::SIG_DF_INTEL;

    // set the number of signals 
    m_ucNumOfSignals = 0;	

    return 1;
}

/**
 * \brief      writes the Messages in the given list to the output file
 * \param[in]  fileOutput Pointer to the Output file
 * \param[in]  m_listMessages List of Message
 * \param[in]  writeErr If true write error signals also else write only correct signals
 *             associated with the message
 *
 * Writes the Messages in the given list to the output file.
 */
bool CMessage::writeMessageToFile( CStdioFile &fileOutput,CList<CMessage,CMessage&> &m_listMessages,bool writeErr)
{
    bool bResult = true;
    char acLine[defCON_MAX_LINE_LEN];
    POSITION pos = m_listMessages.GetHeadPosition();

    //Write all the message
    while(pos != NULL)
    {
        CMessage& msg = m_listMessages.GetNext(pos);
        sprintf(acLine,"%s %s,%u,%u,%u,%c,%c,%s\n",T_START_MSG,msg.m_acName,msg.m_uiMsgID,msg.m_ucLength,msg.m_ucNumOfSignals,msg.m_cDataFormat,msg.m_cFrameFormat,msg.m_txNode);
        fileOutput.WriteString(acLine);

        CSignal sig; 
        //write all related signals to the messages
        bResult &= sig.WriteSignaltofile (fileOutput,msg.m_listSignals,msg.m_ucLength,msg.m_cDataFormat,writeErr);
        fileOutput.WriteString(T_END_MSG"\n\n");
    }
    return bResult;
}
