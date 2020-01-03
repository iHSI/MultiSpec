//	 									MultiSpec
//
//					Laboratory for Applications of Remote Sensing
//									Purdue University
//								West Lafayette, IN 47907
//								 Copyright (1995-2020)
//							(c) Purdue Research Foundation
//									All rights reserved.
//
//	File:						WFeatureExtractionDialog.h
//	Implementation:		WFeatureExtractionDialog.cpp
//
//	Authors:					Larry L. Biehl
//
//	Language:				C++
//
//	System:					Windows Operating System
//
//	Brief description:	Header file for the CMFeatureExtractionDialog class.
//
//	Written By:				Larry L. Biehl			Date: ??/??/1995?
//	Revised By:				Larry L. Biehl			Date: 12/05/2019
//
//------------------------------------------------------------------------------------

#pragma once

#include "WDialog.h"


class CMFeatureExtractionDialog : public CMDialog
{
	// Construction
	public:
		CMFeatureExtractionDialog (	// standard constructor
				CWnd* 								pParent = NULL);
	
		~CMFeatureExtractionDialog ();	// standard desctructor
	
		Boolean DoDialog (
				FeatureExtractionSpecsPtr		featureExtractionSpecsPtr);


		// Dialog Data
		//{{AFX_DATA (CMFeatureExtractionDialog)
		enum { IDD = IDD_FeatureExtraction };
	
		float									m_interclassThrehold,
												m_optimizeThreshold,
												m_withinClassThreshold;
	
		long									m_maxPixelsPerClass,
												m_minThresholdNumber;
	
		int									m_algorithmCode,
												m_interClassWeightsSelection,
												m_preprocessSelection,
												m_weightsSelection;
	
		BOOL									m_listTransformationFlag,
												m_optimizeClassesFlag,
												m_specialOptionsFlag;
		//}}AFX_DATA

	// Implementation
	protected:
		virtual void DoDataExchange (
				CDataExchange* 					pDX);	// DDX/DDV support

		// Generated message map functions
		//{{AFX_MSG (CMFeatureExtractionDialog)
		afx_msg void OnCbnSelendokPreprocessingcombo ();
	
		afx_msg void OnDecisionBoundary ();
	
		virtual BOOL OnInitDialog ();
	
		afx_msg void OnOptimizeClasses ();
	
		afx_msg void OnSelendokChannelCombo ();
	
		afx_msg void OnSelendokClassPairWeightsCombo ();
	
		afx_msg void OnSelendokClassWeightsCombo ();
	
		afx_msg void OnSpecialOptions ();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP ()
	
	
		FeatureExtractionSpecsPtr		m_featureExtractionSpecsPtr;
		float*								m_classWeightsPtr;
		SInt16*								m_localClassPairWeightsListPtr;
	
		SInt16								m_localDefaultClassPairWeight;
	
};	// end class CMFeatureExtractionDialog
