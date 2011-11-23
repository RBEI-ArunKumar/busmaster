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
 * \file      SectionNames.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#ifndef _SECTIONNAMES__H_
#define _SECTIONNAMES__H_

enum eSECTION_ID // These IDs should be maintained with load sequence 
{
    DATABASE_SECTION_ID = 0x0,    
    DIL_SECTION_ID,
    FILTER_SECTION_ID,
    MAINFRAME_SECTION_ID,
    SIGWATCH_SECTION_ID,    
    //GRAPH_SECTION_ID,
    LOG_SECTION_ID,
    SIMSYS_SECTION_ID,    
    REPLAY_SECTION_ID,
    MSGWND_SECTION_ID,    
    TXWND_SECTION_ID,
	//WAVEFORMDATA_SECTION_ID,
	BUS_STATISTICS_SECTION_ID,
    TEST_SETUP_EDITOR_SECTION_ID,
    TEST_SUITE_EXECUTOR_SECTION_ID,
    SECTION_TOTAL
};

//This Names shold be maintained with eSECTION_ID sequence
static TCHAR SectionName[SECTION_TOTAL][MAX_PATH] = {
                                                        _T("DatabaseConfigSectionName"),                                                        
                                                        _T("DilConfigSectionName"),
                                                        _T("FilterConfigSectionName"),
                                                        _T("MainFrameConfigSectionName"),
                                                        _T("SigWatchConfigSectionName"),                                                        
                                                        //_T("GraphConfigSectionName"),
                                                        _T("LogConfigSectionName"),                                                        
                                                        _T("SimSysConfigSectionName"),                                                        
                                                        _T("ReplayConfigSectionName"),
                                                        _T("MsgWndConfigSectionName"),                                                        
                                                        _T("TxWndConfigSectionName"),
														//_T("WaveformConfigSectionName"),
														_T("BusStatsConfigSectionName"),
														_T("TestSetupEditorSectionName"),
														_T("TestSuiteExecutorSectionName")
                                                    };
#endif // _SECTIONNAMES__H_
