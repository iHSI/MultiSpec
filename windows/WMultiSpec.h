//	 									MultiSpec
//
//					Laboratory for Applications of Remote Sensing
//									Purdue University
//								West Lafayette, IN 47907
//								 Copyright (1995-2020)
//							(c) Purdue Research Foundation
//									All rights reserved.
//
//	File:						WMultiSpec.h
//	Implementation:		WMultiSpec.cpp
//
//	Authors:					Larry L. Biehl
//
//	Language:				C++
//
//	System:					Windows Operating System
//
//	Brief description:	Main header file for the MultSpec application
//
//	Written By:				Larry L. Biehl			Date: ??/??/1995?
//	Revised By:				Larry L. Biehl			Date: 02/23/2017
//
//------------------------------------------------------------------------------------

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif         

#pragma once
       
#undef _mac_
#undef multispec_mac

#ifndef _win_
	#define _win_
#endif

#ifndef multispec_win
	#define multispec_win
#endif
                                            
#include "WAbout.h"                 


class CMultiSpecApp : public CWinApp
{
	public:
		CMultiSpecApp ();

		Boolean DoIdle (
				SInt32								lCount);

		void ExitApplication (void);
	
		CMultiDocTemplate* GetGraphDocTemplate (void);
	
		Handle GetOpenImageFileInfoHandle (void);
	
		CMultiDocTemplate* GetStatisticsDocTemplate (void);
	
		SInt16 GetZoomCode (void);
	
		SInt32 OpenImageFileWin (
				LocalAppFile*						localAppFilePtr,
				CDocument**							documentPtrPtr,
				Boolean								fromOpenProjectImageWindowFlag,
				UInt16								hdfDataSetSelection);
	
		void SetControlDelayFlag (
				Boolean								delayFlag);
	
		void SetPrintOrientation (
				Handle								windowInfoHandle);

		Handle SetUpNewImageDocument (
				Handle								fileInfoHandle,
				SInt16								fileImageType,
				SInt16								windowType);
	
		void SetZoomCode (
				SInt16								zoomCode);
	

		UInt32 								m_nextControlTime;
	
	// Implementation
	private:
		virtual int ExitInstance (void);
	
		virtual BOOL InitInstance (void);
	
		virtual BOOL OnIdle (
				SInt32								lCount);
	
		CDocument* OpenDocumentFile (
				LPCSTR								lpszFileName);

		//{{AFX_MSG (CMultiSpecApp)
		afx_msg void OnAppAbout ();
	
		afx_msg void OnAppExit ();
	
		afx_msg void OnFileOpen ();
	
		afx_msg void OnFilePrintSetup ();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP ()
	
	
		CAboutDlg 							m_splash;
	
		CMultiDocTemplate					*m_graphDocTemplatePtr,
												*m_imageDocTemplatePtr,
												*m_statisticsDocTemplatePtr;
	
		Handle								m_openImageFileInfoHandle;

		UInt32 								m_dwSplashTime;
	
		SInt16								m_imageZoomCode;
	
		Boolean								m_controlDelayFlag;
	
};	// end class CMultiSpecApp
   
#ifndef _DEBUG  // debug version in multspec.cpp
	inline CMultiDocTemplate* CMultiSpecApp::GetGraphDocTemplate ()
  		{ return (CMultiDocTemplate*)m_graphDocTemplatePtr; } 
  		 
	inline Handle CMultiSpecApp::GetOpenImageFileInfoHandle ()
  		{ return m_openImageFileInfoHandle; }   

	inline CMultiDocTemplate* CMultiSpecApp::GetStatisticsDocTemplate ()
  		{ return (CMultiDocTemplate*)m_statisticsDocTemplatePtr; }
#endif	// end "!_DEBUG"
