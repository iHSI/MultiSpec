//                                     MultiSpec
//
//                   Copyright 1988-2020 Purdue Research Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use
// this file except in compliance with the License. You may obtain a copy of the
// License at:  https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the specific
// language governing permissions and limitations under the License.
//
// MultiSpec is curated by the Laboratory for Applications of Remote Sensing at
// Purdue University in West Lafayette, IN and licensed by Larry Biehl.
//
//	File:						WChannelsDialog.cpp : implementation file
//
//	Authors:					Larry L. Biehl
//
//	Revision date:			01/04/2018
//
//	Language:				C++
//
//	System:					Windows Operating System
//
//	Brief description:	This file contains functions that relate to the
//								CMChannelsDlg class.
//
//------------------------------------------------------------------------------------

#include "SMultiSpec.h"
#include "SImageWindow_class.h"

#include "WChannelsDialog.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char BASED_CODE THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP (CMChannelsDlg, CMOneColDlg)
	//{{AFX_MSG_MAP (CMChannelsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP ()



CMChannelsDlg::CMChannelsDlg (
				CWnd* 								pParent /*=NULL*/)
		: CMOneColDlg (pParent)

{  
	m_layerInfoPtr = NULL;
	m_fileInfoPtr = NULL; 
	m_useTransformFlag = FALSE; 
	m_availableFeaturePtr = NULL;
	                        
	m_initializedFlag = CMOneColDlg::m_initializedFlag;
	
}	// end "CMChannelsDlg"



void CMChannelsDlg::DoDataExchange (
				CDataExchange* 					pDX)

{
	CMOneColDlg::DoDataExchange (pDX);
	//{{AFX_DATA_MAP (CMChannelsDlg)
	//}}AFX_DATA_MAP
	
}	// end "DoDataExchange"



Boolean CMChannelsDlg::DoDialog (
				SInt16*								numberOutputFeaturesPtr, 
				SInt16* 								selectedFeaturePtr, 
				LayerInfoPtr						layerInfoPtr,
				FileInfoPtr							fileInfoPtr, 
				SInt16								listType, 
				Boolean								useTransformFlag, 
				SInt16* 								availableFeaturePtr, 
				SInt16								numberInputVecItems,
				SInt16								currentSelection)

{
	INT_PTR								returnCode;
	
	Boolean								OKFlag = FALSE;

	
			// Make sure intialization has been completed.
							                         
	if (!m_initializedFlag)
																						return (FALSE);
	
			// This is a 0 base list.
			
	m_indexStart = 0;
	
	m_numberOutputItems = *numberOutputFeaturesPtr;    
	m_selectedItemsPtr = selectedFeaturePtr;
	m_layerInfoPtr = layerInfoPtr;
	m_fileInfoPtr = fileInfoPtr; 
	m_listType = listType; 
	m_useTransformFlag = useTransformFlag;
	m_availableFeaturePtr = availableFeaturePtr; 
	m_numberInputVecItems = numberInputVecItems;
	m_currentSelection = currentSelection;                     
																					
	returnCode = DoModal ();
	
	if (returnCode == IDOK)
		{                                                  
				// Get items to use.	    
				
		if (m_listType == kSelectItemsList || m_listType == kSelectPCList)
			{                             
			*numberOutputFeaturesPtr = m_numberSelections;

			SaveSubsetList ((SInt16)m_numberSelections, 
									m_selectedItemsPtr,
									m_availableFeaturePtr, 
									(SInt16)m_numberInputVecItems);
			
			}	// end "if (m_listType == kSelectChannelsList || ..."
			
		OKFlag = TRUE; 
		
		}	// end "if (returnCode == IDOK)"
		
	CheckAndUnlockHandle (gTransformationMatrix.eigenFeatureHandle);
		
	return (OKFlag);
		
}	// end "DoDialog" 



BOOL CMChannelsDlg::OnInitDialog (void)

{
	ChannelDescriptionPtr			channelDescriptionPtr;
	CListBox* 							listBoxPtr = NULL;
	DialogPtr							dialogPtr = this;
	FileInfoPtr							localFileInfoPtr;
	
	char									*spacer = ": ";
	
	SInt32								estimatedLengthListDescription;
	
	SInt16								fileInfoIndex;
									
									
	CMOneColDlg::OnInitDialog ();


			// Initialize local variables.														
			
	channelDescriptionPtr = NULL;
	fileInfoIndex = -1;
	localFileInfoPtr = NULL;
	
	if (m_useTransformFlag)
		{
		m_numberInputVecItems = gTransformationMatrix.numberFeatures; 
		MHLock (gTransformationMatrix.eigenFeatureHandle);
		m_availableFeaturePtr = NULL;
		m_fileInfoPtr = NULL;       
		
		m_listEnd = (UInt16)m_numberInputVecItems; 
		
		}	// end "if (m_useTransformFlag)"
		
	LoadSubsetList (&m_numberOutputItems,
							m_selectedItemsPtr,
							m_availableFeaturePtr,
							(SInt16)m_numberInputVecItems,
							&m_currentSelection);
	
			// Get estimate of the length of the description in the list.			
			// Use 22 for the description list = 4 for channel number and 18 		
			// for the channel description.													
		
	estimatedLengthListDescription = (SInt32)m_numberInputVecItems * 22; 
                                                              
	SetWindowText ((LPCTSTR)_T("Select Channels"));
	LoadDItemString (IDC_ListTitle, (Str255*)"\0Channel List:");
	
			// If this is just a display list, hide all but OK button and list.	
			// Put "Channel Descriptions" or "Selected Channels" as title of 		
			// the list.																			
			
	if (m_listType == kItemsListOnly || m_listType == kSelectedItemsListOnly)
		{
		MHideDialogItem (dialogPtr, IDCANCEL);
		MHideDialogItem (dialogPtr, IDC_AllSelected);
		MHideDialogItem (dialogPtr, IDC_NoneSelected);
		
		MHideDialogItem (dialogPtr, IDC_ListTitle);
		MHideDialogItem (dialogPtr, IDC_AllSelected);
		MHideDialogItem (dialogPtr, IDC_FirstLabel);
		MHideDialogItem (dialogPtr, IDC_First);
		MHideDialogItem (dialogPtr, IDC_LastLabel);
		MHideDialogItem (dialogPtr, IDC_Last);
		MHideDialogItem (dialogPtr, IDC_IntervalLabel);
		MHideDialogItem (dialogPtr, IDC_Interval); 
		MHideDialogItem (dialogPtr, IDC_EnterNewRange);
		MHideDialogItem (dialogPtr, IDC_RangeOutline); 
		
		if (m_listType == kItemsListOnly)                                     
			SetWindowText ((LPCTSTR)_T("Channel Descriptions"));
			
		if (m_listType == kSelectedItemsListOnly)                         
			SetWindowText ((LPCTSTR)_T("Selected Channels"));
		
		}	// end "if (listType == kItemsListOnly || ...)" 
		
	if (m_listType == kItemsListOnly)
		{
		MHideDialogItem (dialogPtr, IDC_SelectionCountLabel);
		MHideDialogItem (dialogPtr, IDC_SelectionCount);
		
		}	// end "if (m_listType == kItemsListOnly)"
			
	if (m_useTransformFlag)
		{                                                                            
		SetWindowText ((LPCTSTR)_T("Select Transformation Features"));
		LoadDItemString (IDC_ListTitle, (Str255*)"\0Feature List:");
		
		}	// end "if (m_useTransformFlag)" 

	if (UpdateData (FALSE))
		{ 
		PositionDialogWindow (); 
		
		listBoxPtr = (CListBox*)GetDlgItem (IDC_List1);
		
		}	// end "if (UpdateData (FALSE))" 

	if (listBoxPtr != NULL)
		{  
		SInt32 numberSelectedItems = AddChannelsToDialogList (
																	listBoxPtr,
																	&m_numberOutputItems,
																	m_selectedItemsPtr,
																	m_layerInfoPtr,
																	m_fileInfoPtr,
																	m_listType,
																	m_availableFeaturePtr,
																	(SInt32)m_numberInputVecItems,
																	m_currentSelection);
																
				// Set the number of selected items.									
				
		::LoadDItemValue (dialogPtr, IDC_SelectionCount, numberSelectedItems);
			
		}	// end "if (listBoxPtr != NULL)"
	
			// Set default text selection to first edit text item 
		                                       
	SelectDialogItemText (this, IDC_First, 0, SInt16_MAX);
	
	return FALSE;  // return TRUE  unless you set the focus to a control
	
}	// end "OnInitDialog"



//------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CMChannelCombinationsDlg, CMOneColDlg)
	//{{AFX_MSG_MAP (CMChannelCombinationsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP ()

 

CMChannelCombinationsDlg::CMChannelCombinationsDlg (
				CWnd* 								pParent /*=NULL*/)
		: CMOneColDlg (pParent)
		
{  
	m_useTransformFlag = FALSE; 
	m_availableFeaturePtr = NULL;
	                        
	m_initializedFlag = CMOneColDlg::m_initializedFlag;
	
}	// end "CMChannelCombinationsDlg"



void CMChannelCombinationsDlg::DoDataExchange (
				CDataExchange* 					pDX)

{
	CMOneColDlg::DoDataExchange (pDX);
	//{{AFX_DATA_MAP (CMChannelCombinationsDlg)
	//}}AFX_DATA_MAP
	
}	// end "DoDataExchange"



//-----------------------------------------------------------------------------
//                   Copyright 1988-2020 Purdue Research Foundation
//
//	Function name:		void DoDialog
//
//	Software purpose:	The purpose of this routine is to present the channel
//							combinations dialog box to the user and copy the
//							revised back to the display specification structure if
//							the user selected OK.
//
//	Parameters in:		None
//
//	Parameters out:	None
//
//	Value Returned:	None		
// 
//	Called By:			
//
//	Coded By:			Larry L. Biehl			Date: 04/30/1998
//	Revised By:			Larry L. Biehl			Date: 05/26/2017	

Boolean CMChannelCombinationsDlg::DoDialog (
				SInt16								channelCombinationSelection,
				UInt16*								numberOutputChannelCombinationsPtr, 
				UInt16* 								channelCombinationsPtr, 
				UInt32								numberInputChannelCombinations, 
				UInt16*								allChanCombinationsPtr,
				UInt32								contiguousChannelsPerGroup,
				Boolean								useTransformFlag)

{  
	INT_PTR								returnCode;
	
	Boolean								OKFlag = FALSE;
	
	
			// Make sure intialization has been completed.
							                         
	if (!m_initializedFlag)
																					return (FALSE);
	
			// This is a 1 base list.
			
	m_indexStart = 1; 
	m_listType = kSelectItemsList; 
	
	m_channelCombinationSelection = channelCombinationSelection;
	m_numberOutputItems = *numberOutputChannelCombinationsPtr;    
	m_selectedItemsPtr = (SInt16*)channelCombinationsPtr;
	m_numberInputVecItems = numberInputChannelCombinations;
	m_availableFeaturePtr = (SInt16*)allChanCombinationsPtr; 
	m_contiguousChannelsPerGroup = (SInt16)contiguousChannelsPerGroup; 
	m_useTransformFlag = useTransformFlag;                    
																					
	returnCode = DoModal ();
	
	if (returnCode == IDOK)
		{                                                  
				// Get items to use.	 
				
		ChannelCombinationsDialogOK (m_numberSelections,                                   
												numberOutputChannelCombinationsPtr,
												channelCombinationsPtr,         
												allChanCombinationsPtr);
			
		OKFlag = TRUE; 
		
		}	// end "if (returnCode == IDOK)"
		
	return (OKFlag);
		
}	// end "DoDialog" 



BOOL CMChannelCombinationsDlg::OnInitDialog (void)

{		
	CListBox* 							listBoxPtr = NULL;
									
									
	CMOneColDlg::OnInitDialog ();


			// Initialize local variables.
	
	ChannelCombinationsDialogInitialize (this,
														m_contiguousChannelsPerGroup,
														m_useTransformFlag,
														m_numberInputVecItems);

	if (UpdateData (FALSE))
		{ 
		PositionDialogWindow (); 
		
		listBoxPtr = (CListBox*)GetDlgItem (IDC_List1);
		
		}	// end "if (UpdateData (FALSE))" 
		
	ChannelCombinationsDialogLoadList (this,
													listBoxPtr,
													m_numberOutputItems,
													(UInt16*)m_selectedItemsPtr,
													m_numberInputVecItems,
													(UInt16*)m_availableFeaturePtr,
													m_channelCombinationSelection);

			// Set default text selection to first edit text item 
		                                       
	SelectDialogItemText (this, IDC_First, 0, SInt16_MAX);     
	
	return FALSE;  // return TRUE  unless you set the focus to a control
	
}	// end "OnInitDialog"   
