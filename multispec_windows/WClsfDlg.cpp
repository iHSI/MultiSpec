// WClsfDlg.cpp : implementation file
//   
// Revised by Larry Biehl on 03/06/2017
                   
#include "SMulSpec.h"
                      
#include "WClsfDlg.h" 
#include "WCWtDlg.h"

#include	"SExtGlob.h"  

extern void 			ClassifyDialogInitialize (
								DialogPtr							dialogPtr,
								UInt16*								localFeaturesPtr, 
								UInt16*								localTransformFeaturesPtr,
								UInt16*								localClassPtr,
								UInt16*								localClassAreaPtr,
								unsigned char*						localSymbolsPtr,
								SInt16*								classificationProcedurePtr,
								Boolean*								thresholdAllowedFlagPtr,
								SInt16*								covarianceEstimatePtr,
								UInt16*								numberEigenvectorsPtr,
								Boolean*								featureTransformAllowedFlagPtr,
								Boolean*								featureTransformationFlagPtr,
								SInt16*								channelSelectionPtr,
								UInt16*								localNumberChannelsPtr,
								SInt16*								fileNamesSelectionPtr,
								Handle*								targetWindowInfoHandlePtr,
								SInt16*								classAreaSelectionPtr,
								UInt32*								localNumberClassAreasPtr,
								Boolean*								trainingAreaFlagPtr,
								Boolean*								trainingFieldsExistFlagPtr,
								Boolean*								userLeaveOneOutFlagPtr,
								Boolean*								leaveOneOutSettingFlagPtr,
								Boolean*								testAreaFlagPtr,
								Boolean*								imageAreaFlagPtr,
								DialogSelectArea*					dialogSelectAreaPtr,
								SInt16*								classSelectionPtr,
								UInt32*								localNumberClassesPtr,
								SInt16*								weightsSelectionPtr,
								SInt16*								symbolSelectionPtr,
								SInt16*								outputAsciiCodePtr,
//								Boolean*								textWindowFlagPtr,
								Boolean*								createImageOverlayFlagPtr,
								SInt16*								selectImageOverlaySelectionPtr,
								SInt16*								outputFormatCodePtr,
								Boolean*								diskFileFlagPtr,
								SInt16*								paletteSelectionPtr,
								Boolean*								thresholdResultsFlagPtr,
								Boolean*								createProbabilityFileFlagPtr,
								double*								saveThresholdPercentPtr,
								double*								saveAngleThresholdPtr,
								double*								saveCorrelationThresholdPtr,
								double*								saveCEMThresholdPtr,
								SInt16*								listResultsTestCodePtr,
								SInt16*								listResultsTrainingCodePtr,
								SInt16*								parallelPipedCodePtr);
	
extern void 			ClassifyDialogOK (
								SInt16								classificationProcedure,
								SInt16								covarianceEstimate,
								Boolean								featureTransformationFlag,
								SInt16								channelSelection,
								SInt16								localNumberFeatures,
								SInt16*								localFeaturesPtr,
								Handle								targetWindowInfoHandle,
								SInt16								fileNamesSelection,
								SInt16								classAreaSelection,
								SInt16								localNumberClassAreas,
								SInt16*								localClassAreaPtr,
								Boolean								trainingFldsResubstitutionFlag,
								Boolean								trainingFldsLOOFlag,
								Boolean								testFldsFlag,
								Boolean								imageAreaFlag,
								DialogSelectArea*					dialogSelectAreaPtr,
								SInt16								classSelection,
								SInt16								localNumberClasses,
								SInt16*								localClassPtr,
								SInt16								weightsSelection,
								float*								classWeightsPtr,
								SInt16								symbolSelection,
								unsigned char*						localSymbolsPtr,
//								Boolean								outputWindowFlag,
								Boolean								diskFileFlag,
								Boolean								createImageOverlayFlag,
								SInt16								selectImageOverlaySelection,
								SInt16								outputFormatCode,
								Boolean								thresholdFlag,
								double								saveCorrelationThreshold,
								double								saveAngleThreshold,
								double								saveCEMThreshold,
								double								saveThresholdPercent,
								Boolean								probabilityFileFlag,
								SInt16								paletteSelection,
								SInt16								listResultsTestCode,
								SInt16								listResultsTrainingCode,
								SInt16								parallelPipedCode); 
	                
extern SInt16 			ClassifyDialogOnClassificationProcedure (
								DialogPtr							dialogPtr, 
								SInt16								okItem, 
								Boolean*								thresholdAllowedFlagPtr,
								Boolean*								featureTransformAllowedFlagPtr,                     
								SInt16*								weightsSelectionPtr,
								SInt16*								parallelPipedCodePtr,
								SInt16								classificationSelection,
								SInt16*								covarianceEstimatePtr,
								SInt16								numberEigenvectors,
								UInt16*								classifyProcedureEnteredCodePtr,
								Boolean								optionKeyFlag);
	                
extern SInt16 			ClassifyDialogOnTargetFile (
								DialogPtr							dialogPtr,
								SInt16								fileNamesSelection,
								Handle*								targetWindowInfoHandlePtr,
								Boolean*								checkOKFlagPtr,
								DialogSelectArea*					dialogSelectAreaPtr,
								Boolean*								createImageOverlayFlagPtr);
								
extern Boolean 		ClassifyDialogGetFeatureTransformAllowedFlag (
								SInt16								classificationProcedure,
								UInt16								numberEigenvectors);   
								
extern Boolean 		ClassifyDialogGetThresholdAllowedFlag (
								SInt16								classificationProcedure);
	
extern Boolean			ClassifyDialogSetLeaveOneOutItems (
								DialogPtr							dialogPtr,
								SInt16								classificationProcedure,
								SInt16								fileNamesSelection,
								Boolean								userLeaveOneOutFlag,
								Boolean								trainingFieldExistFlag,
								Boolean*								leaveOneOutSettingFlagPtr);
								
extern Boolean 		ClassifyDialogSetThresholdItems (
								DialogPtr							dialogPtr,
								SInt16								classificationProcedure,
								Boolean								classifyImageAreaFlag,
								Boolean								createProbabilityFileFlag,
								Boolean								thresholdResultsFlag,
								Boolean								thresholdAllowedFlag);
								
extern Boolean 		EchoClassifyDialog (void);

extern void 			ListResultsOptionsDialog (
								SInt16*								listResultsTrainingCode, 
								SInt16*								listResultsTestCode ); 
								
extern void 			SetUpPalettePopUpMenu (
								DialogPtr							dialogPtr);
                    
extern void 			ClassifyDialogSetPaletteItems (
								DialogPtr							dialogPtr,
								SInt16								outputFormatCode,
								Boolean								createImageOverlayFlag);

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMClassifyDialog dialog


CMClassifyDialog::CMClassifyDialog(CWnd* pParent /*=NULL*/)
	: CMDialog(CMClassifyDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMClassifyDialog)
	m_trainingAreaFlag = FALSE;
	m_imageAreaFlag = FALSE;
//	m_textWindowFlag = FALSE;
	m_thresholdResultsFlag = FALSE;
	m_classWeightsSelection = -1;
	m_thresholdPercent = (float)0.;
	m_diskFileFlag = FALSE;
	m_createImageOverlayFlag = FALSE;
	m_classifyListSelection = -1;      
//	m_classAreaSelection = -1;
//new
	m_classAreaSelection = 0;
//end new
	m_createProbabilityFileFlag = FALSE;
	m_testAreaFlag = FALSE;
	m_fileNamesSelection = -1;
//	m_outputFormatCode = -1;
//new
	m_outputFormatCode = 1;
//end new
	m_paletteSelection = 0;
	m_trainingAreaLOOFlag = FALSE;
	m_cemThreshold = (float)0.;
	m_angleThreshold = (float)0.;
	m_correlationThreshold = (float)0.;
	m_selectImageOverlaySelection = 1;
	//}}AFX_DATA_INIT

	m_selectImageOverlaySelection = 1;
	
	m_createImageOverlayFlag = FALSE;
	
	m_classWeightsPtr = NULL;
	m_classAreaListPtr = NULL;
	m_optionKeyFlag = FALSE;

	m_classifyProcedureEnteredCode = 0;
	
	m_initializedFlag = CMDialog::m_initializedFlag; 
	
	if (m_initializedFlag)
		m_initializedFlag = GetDialogLocalVectors (
														&m_localFeaturesPtr,
														&m_localTransformFeaturesPtr,     
														&m_classListPtr,
														&m_classAreaListPtr, 
														&m_classWeightsPtr,
														NULL,
														NULL,
														NULL);
	
}		// end "CMClassifyDialog"



CMClassifyDialog::~CMClassifyDialog(void)
{  
	ReleaseDialogLocalVectors (m_localFeaturesPtr,
											m_localTransformFeaturesPtr,        
											m_classListPtr,
											m_classAreaListPtr,
											m_classWeightsPtr,
											NULL,
											NULL,
											NULL);
	
}		// end "~CMClassifyDialog"



void CMClassifyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMClassifyDialog)
	DDX_CBIndex(pDX, IDC_ClassCombo, m_classSelection);
	DDX_Check(pDX, IDC_Training, m_trainingAreaFlag);
	DDX_Check(pDX, IDC_ImageArea, m_imageAreaFlag);
//	DDX_Check(pDX, IDC_TextWindow, m_textWindowFlag);            
	DDX_CBIndex(pDX, IDC_WeightCombo, m_classWeightsSelection);
	DDX_CBIndex(pDX, IDC_SymbolCombo, m_symbolSelection);
	DDX_Check(pDX, IDC_DiskFile, m_diskFileFlag);
	DDX_CBIndex(pDX, IDC_ClassificationProcedure, m_classifyListSelection);
	DDX_CBIndex(pDX, IDC_AreasCombo, m_classAreaSelection);
	DDX_Check(pDX, IDC_CreateProbabilityFile, m_createProbabilityFileFlag);
	DDX_Check(pDX, IDC_TestAreas, m_testAreaFlag);
	DDX_CBIndex(pDX, IDC_TargetCombo, m_fileNamesSelection);
	DDX_CBIndex(pDX, IDC_DiskCombo, m_outputFormatCode);
	DDX_CBIndex(pDX, IDC_PaletteCombo, m_paletteSelection);
	DDX_Check(pDX, IDC_TrainingLOO, m_trainingAreaLOOFlag);
	DDX_Text(pDX, IDC_LineEnd, m_LineEnd);
	DDV_MinMaxLong(pDX, m_LineEnd, 1, m_maxNumberLines);
	DDX_Text(pDX, IDC_LineInterval, m_LineInterval);
	DDV_MinMaxLong(pDX, m_LineInterval, 1, m_maxNumberLines);
	DDX_Text(pDX, IDC_LineStart, m_LineStart);                              
	DDV_MinMaxLong(pDX, m_LineStart, 1, m_maxNumberLines); 
	DDX_Text(pDX, IDC_ColumnEnd, m_ColumnEnd);
	DDV_MinMaxLong(pDX, m_ColumnEnd, 1, m_maxNumberColumns);
	DDX_Text(pDX, IDC_ColumnInterval, m_ColumnInterval);
	DDV_MinMaxLong(pDX, m_ColumnInterval, 1, m_maxNumberColumns);
	DDX_Text(pDX, IDC_ColumnStart, m_ColumnStart);
	DDV_MinMaxLong(pDX, m_ColumnStart, 1, m_maxNumberColumns);
	DDX_CBIndex(pDX, IDC_ChannelCombo, m_channelSelection);
	DDX_Check(pDX, IDC_FeatureTransformation, m_featureTransformationFlag); 
	DDX_Check(pDX, IDC_ThresholdResults, m_thresholdResultsFlag); 
	DDX_Check(pDX, IDC_ImageWindowOverlay, m_createImageOverlayFlag);
	DDX_CBIndex(pDX, IDC_ImageOverlayCombo, m_selectImageOverlaySelection);
		
	//}}AFX_DATA_MAP 
	     
	if (!pDX->m_bSaveAndValidate)
		{
		DDX_Text(pDX, IDC_CorrelationThresold, m_angleThreshold);
		DDX_Text(pDX, IDC_CorrelationCoefficient, m_correlationThreshold);
	                                        
		DDX_Text(pDX, IDC_CEMThreshold, m_cemThreshold);       
				
		DDX_Text(pDX, IDC_ThresholdValue, m_thresholdPercent);
		
		}		// end "if (!pDX->m_bSaveAndValidate)" 
	
	if (pDX->m_bSaveAndValidate)
		{                 
				// Only check the threshold values for the values to actually be used.
				
		if (m_thresholdResultsFlag)
			{                                                                        
			CComboBox* comboBoxPtr = (CComboBox*)GetDlgItem(IDC_ClassificationProcedure);
			m_classificationProcedure = 
										(SInt16)comboBoxPtr->GetItemData(m_classifyListSelection); 
		   
		   if (m_classificationProcedure == kCorrelationMode)
		   	{                                              
				DDX_Text(pDX, IDC_CorrelationThresold, m_angleThreshold);          
				DDV_MinMaxDouble(pDX, m_angleThreshold, 0., 180.);
			                                        
				DDX_Text(pDX, IDC_CorrelationCoefficient, m_correlationThreshold);                              
				DDV_MinMaxDouble(pDX, m_correlationThreshold, 0., 1.);
				
				}		// end "if (m_classificationProcedure == kCorrelationMode)"
				
			else if (m_classificationProcedure == kCEMMode)
				{                                           
				DDX_Text(pDX, IDC_CEMThreshold, m_cemThreshold);    
				DDV_MinMaxDouble(pDX, m_cemThreshold, 0., 1.);
				
				}		// end "else if (m_classificationProcedure == kCEMMode)"
			
			else		// m_classificationProcedure != kCEMMode || ...
				{                                                
				DDX_Text(pDX, IDC_ThresholdValue, m_thresholdPercent);      
				DDV_MinMaxDouble(pDX, m_thresholdPercent, 0., 100.); 
				
				}		// end "m_classificationProcedure != kCEMMode || ..."
			
			}		// end "if (m_thresholdResultsFlag)"
		
		}		// end "if (pDX->m_bSaveAndValidate)" 
	
	if (pDX->m_bSaveAndValidate)
		{
				// Verify that the line and column values make sense
		
		if (m_imageAreaFlag)
			VerifyLineColumnStartEndValues (pDX);
/*
		if (m_LineStart > m_LineEnd)
			{
			DDX_Text(pDX, IDC_LineStart, m_LineStart);              
			DDV_MinMaxLong(pDX, m_LineStart, 1, m_LineEnd);
			
			}		// end "m_LineStart > m_LineEnd"
				
		if (m_ColumnStart > m_ColumnEnd)
			{ 
			DDX_Text(pDX, IDC_ColumnStart, m_ColumnStart);
			DDV_MinMaxLong(pDX, m_ColumnStart, 1, m_ColumnEnd);
			
			}		// end "if (m_ColumnStart > m_ColumnEnd)" 
*/		
		CComboBox* comboBoxPtr = (CComboBox*)GetDlgItem(IDC_ClassificationProcedure);
		m_classificationProcedure = 
								(SInt16)comboBoxPtr->GetItemData(m_classifyListSelection); 
			
		}		// end "if (pDX->m_bSaveAndValidate)"
		
}		// end "DoDataExchange"



BEGIN_MESSAGE_MAP(CMClassifyDialog, CMDialog)
	//{{AFX_MSG_MAP(CMClassifyDialog)
	ON_CBN_SELENDOK(IDC_ChannelCombo, OnSelendokChannelCombo)
	ON_CBN_SELENDOK(IDC_ClassificationProcedure, OnSelendokClassificationProcedure)
	ON_BN_CLICKED(IDC_DiskFile, OnDiskFile)
	ON_BN_CLICKED(IDC_ThresholdResults, OnThresholdResults)
	ON_CBN_SELENDOK(IDC_DiskCombo, OnSelendokDiskCombo)
	ON_BN_CLICKED(IDC_TextWindow, OnTextWindow)
	ON_BN_CLICKED(IDC_Training, OnTraining)
	ON_BN_CLICKED(IDC_TestAreas, OnTestAreas)
	ON_BN_CLICKED(IDC_ImageArea, OnImageArea)
	ON_BN_CLICKED(IDC_FeatureTransformation, OnFeatureTransformation)
	ON_BN_CLICKED(IDC_CreateProbabilityFile, OnCreateProbabilityFile)
	ON_CBN_SELENDOK(IDC_AreasCombo, OnSelendokAreasCombo)
	ON_CBN_SELENDOK(IDC_PaletteCombo, OnSelendokPaletteCombo)
	ON_CBN_DROPDOWN(IDC_PaletteCombo, OnDropdownPaletteCombo)
	ON_BN_CLICKED(IDC_ListOptions, OnListOptions)
	ON_EN_CHANGE(IDC_CorrelationCoefficient, OnChangeCorrelationCoefficient)
	ON_EN_CHANGE(IDC_CorrelationThresold, OnChangeCorrelationThresold)
	ON_CBN_DROPDOWN(IDC_ClassificationProcedure, OnDropdownClassificationProcedure)
	ON_BN_CLICKED(IDC_TrainingLOO, OnTrainingLOO)
	ON_CBN_SELENDOK(IDC_ClassCombo, OnSelendokClassCombo)
	ON_EN_CHANGE(IDC_ColumnEnd, CheckColumnEnd)
	ON_EN_CHANGE(IDC_ColumnStart, CheckColumnStart)
	ON_EN_CHANGE(IDC_LineEnd, CheckLineEnd)
	ON_EN_CHANGE(IDC_LineStart, CheckLineStart)
	ON_BN_CLICKED(IDEntireImage, ToEntireImage)
	ON_BN_CLICKED(IDSelectedImage, ToSelectedImage)
	ON_CBN_SELENDOK(IDC_TargetCombo, OnSelendokTargetCombo)       
	ON_CBN_SELENDOK(IDC_WeightCombo, OnSelendokClassWeightsCombo)
	ON_BN_CLICKED(IDC_ImageWindowOverlay, OnImageOverlay)
	ON_CBN_SELENDOK(IDC_ImageOverlayCombo, OnSelendokImageOverlayCombo) 
	ON_CBN_CLOSEUP(IDC_ClassificationProcedure, OnCloseupClassificationProcedure)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_StartEndInterval, OnStnClickedStartendinterval)
END_MESSAGE_MAP()  


//------------------------------------------------------------------------------------
//								 Copyright (1988-2003)
//								c Purdue Research Foundation
//									All rights reserved.
//
//	Function name:		void DoDialog
//
//	Software purpose:	The purpose of this routine is to present the classification
//							specification dialog box to the user and copy the
//							revised back to the classify specification structure if
//							the user selected OK.
//
//	Parameters in:		None
//
//	Parameters out:	None
//
//	Value Returned:	None		
// 
//	Called By:			Dialog in MDisMult.cpp
//
//	Coded By:			Larry L. Biehl			Date: 02/27/1996
//	Revised By:			Larry L. Biehl			Date: 04/10/1998	

SInt16 
CMClassifyDialog::DoDialog(void)

{  
	SInt16								returnCode;
	
	Boolean								continueFlag = FALSE;
								

	                          
			// Make sure intialization has been completed.
							                         
	if ( !m_initializedFlag )
																			return(FALSE);
	
	returnCode = DoModal ();
	
	if (returnCode == IDOK)
		{
		continueFlag = TRUE;  
				
				// Classification area													
				
		m_dialogSelectArea.lineStart = m_LineStart;
		m_dialogSelectArea.lineEnd = m_LineEnd;
		m_dialogSelectArea.lineInterval = m_LineInterval;
																
		m_dialogSelectArea.columnStart = m_ColumnStart;
		m_dialogSelectArea.columnEnd = m_ColumnEnd;
		m_dialogSelectArea.columnInterval = m_ColumnInterval;
				
		ClassifyDialogOK (m_classificationProcedure,
									m_covarianceEstimate,
									m_featureTransformationFlag,
									m_channelSelection,
									(SInt16)m_localActiveNumberFeatures,
									(SInt16*)m_localActiveFeaturesPtr,
									m_targetWindowInfoHandle,
									m_fileNamesSelection,
									m_classAreaSelection,
									(SInt16)m_localNumberClassAreas,
									(SInt16*)m_classAreaListPtr,
									m_trainingAreaFlag,
									m_trainingAreaLOOFlag,
									m_testAreaFlag,
									m_imageAreaFlag,
									&m_dialogSelectArea,
									m_classSelection,
									(SInt16)m_localNumberClasses,
									(SInt16*)m_classListPtr,
									m_classWeightsSelection+1,
									m_classWeightsPtr,
									m_symbolSelection,
									m_localSymbolsPtr,
//									m_textWindowFlag,
									m_diskFileFlag,
									m_createImageOverlayFlag,
									m_selectImageOverlaySelection+1,
									m_outputFormatCode + 1,
									m_thresholdResultsFlag,
									m_correlationThreshold,
									m_angleThreshold,
									m_cemThreshold,
									m_thresholdPercent,
									m_createProbabilityFileFlag,
									m_paletteSelection + 1,
									m_listResultsTestCode,
									m_listResultsTrainingCode,
									m_parallelPipedCode);
					                                 
		}		// end "if (returnCode == IDOK)"
	
			// Set the variable containing the pointer to the overlay combo box
			// to NULL.
				
	gPopUpImageOverlayMenu = NULL;
		
	return (continueFlag);
		
}		// end "DoDialog" 

    

BOOL CMClassifyDialog::OnInitDialog(void)

{  
	DialogPtr								dialogPtr = this;
	
	SInt16									channelSelection,
												classAreaSelection,
												classSelection,
												fileNamesSelection,
												outputFormatCode,
												paletteSelection,
												selectImageOverlaySelection,
												symbolSelection,
												weightsSelection;
	
	
	CDialog::OnInitDialog();
	
			// Make sure that we have the bitmaps for the entire image buttons.
		
	VERIFY(toEntireButton.AutoLoad(IDEntireImage, this));
	VERIFY(toSelectedButton.AutoLoad(IDSelectedImage, this));  
	
	ClassifyDialogInitialize (this,
										m_localFeaturesPtr,
										m_localTransformFeaturesPtr,
										m_classListPtr,
										m_classAreaListPtr,
										m_localSymbolsPtr,
										&m_classificationProcedure,
										&m_thresholdAllowedFlag,
										&m_covarianceEstimate,
										&m_numberEigenvectors,
										&m_featureTransformAllowedFlag,
										(Boolean*)&m_featureTransformationFlag,
										&channelSelection,
										&m_localActiveNumberFeatures,
										&fileNamesSelection,
										&m_targetWindowInfoHandle,
										&classAreaSelection,
										&m_localNumberClassAreas,
										(Boolean*)&m_trainingAreaFlag,
										&m_trainingFieldsExistFlag,
										&m_savedLeaveOneOutFlag,
										(Boolean*)&m_trainingAreaLOOFlag,
										(Boolean*)&m_testAreaFlag,
										(Boolean*)&m_imageAreaFlag,
										&m_dialogSelectArea,
										&classSelection,
										&m_localNumberClasses,
										&weightsSelection,
										&symbolSelection,
										&m_outputAsciiCode,
//										(Boolean*)&m_textWindowFlag,
										(Boolean*)&m_createImageOverlayFlag,
										&selectImageOverlaySelection,
										&outputFormatCode,
										(Boolean*)&m_diskFileFlag,
										&paletteSelection,
										(Boolean*)&m_thresholdResultsFlag,
										(Boolean*)&m_createProbabilityFileFlag,
										&m_saveThresholdPercent,
										&m_saveAngleThreshold,
										&m_saveCorrelationThreshold,
										&m_saveCEMThreshold,
										&m_listResultsTestCode,
										&m_listResultsTrainingCode,
										&m_parallelPipedCode); 
	
			// Set feature/transform feature parameters  

	InitializeDialogFeatureParameters (m_featureTransformationFlag,
													m_localActiveNumberFeatures, 
													gProjectInfoPtr->numberStatisticsChannels, 
													gTransformationMatrix.numberFeatures,
													m_localFeaturesPtr,
													m_localTransformFeaturesPtr,
													&m_localNumberFeatures,
													&m_localNumberTransformFeatures,
													&m_localActiveFeaturesPtr);
	
			// Set up classification popup menu list
	
	m_correlationComboListItem = kCorrelationMode - 1;	
	if (gProjectInfoPtr->statisticsCode != kMeanCovariance)
		m_correlationComboListItem = 1;
		
			// Get the classification list selection that matches the input
			// classification procedure.
			                             
	m_classifyListSelection = GetComboListSelection(IDC_ClassificationProcedure, 
																	m_classificationProcedure);
		
	if (m_classifyListSelection == -1)
		m_classifyListSelection = 0;
	                                                                   			
			//	Set the channels/features list item								
			                                             
	m_availableFeaturePtr = gProjectInfoPtr->channelsPtr;
	m_channelSelection = channelSelection;
	
			// Target File
			
	m_fileNamesSelection = fileNamesSelection - 1;
	
			// Class areas to classify.
			
	m_classAreaSelection = classAreaSelection;
				
			// Selected area to classify.
                  
	m_LineStart = m_dialogSelectArea.lineStart;
	m_LineEnd = m_dialogSelectArea.lineEnd;
	m_LineInterval = m_dialogSelectArea.lineInterval;
	m_ColumnStart = m_dialogSelectArea.columnStart;
	m_ColumnEnd = m_dialogSelectArea.columnEnd;
	m_ColumnInterval = m_dialogSelectArea.columnInterval; 
						
			// Classes to use.
			
	m_classSelection = classSelection;
	
			// Class weights.				             
			// Adjust for 0 base index. 
	
	m_classWeightsSelection = weightsSelection - 1;

	if (weightsSelection > 0)
		HideDialogItem (this, IDC_WeightsEqual);
	
	else		// weightsSelection <= 0
		HideDialogItem (this, IDC_WeightCombo);                                            
//	((CComboBox*)GetDlgItem(IDC_WeightCombo))->DeleteString(1);
	
			// Symbols to use.             
			// Adjust for 0 base index.
			// User defined symbos are not available yet.																	
			                                                                  
	m_symbolSelection = symbolSelection - 1; 
	HideDialogItem (this, IDC_SymbolCombo);                                            
	((CComboBox*)GetDlgItem(IDC_SymbolCombo))->DeleteString(1);  
	 
			// Classification output to disk file.
			// Adjust for 0 base index.
	
	m_outputFormatCode = abs(outputFormatCode);	
	m_outputFormatCode -= 1;
		
	if (!(gClassifySpecsPtr->outputStorageType & 0x0006)) 
		gOutputFormatCode = -gOutputFormatCode;
		
	if (!m_diskFileFlag)
		MHideDialogItem (this, IDC_DiskCombo);

	m_selectImageOverlaySelection = selectImageOverlaySelection - 1;
	
			// Palette to use.             
			// Adjust for 0 base index.
		
	m_paletteSelection = paletteSelection - 1;
	
			// Threshold output data. 															
			// Threshold percent (to be used for output files).						
		
	m_thresholdPercent = (float)m_saveThresholdPercent;  
	
			// Thresholds for Correlation Classifier.
			
	m_angleThreshold = (float)m_saveAngleThreshold;
	m_correlationThreshold = (float)m_saveCorrelationThreshold;
	
			// Thresholds for CEM Classifier.
			
	m_cemThreshold = (float)m_saveCEMThreshold; 
	                 
	if (UpdateData(FALSE) )                   
		PositionDialogWindow ();
	
			// Set default text selection to first edit text item	
		                                                     
	SelectDialogItemText (this, IDC_LineStart, 0, SInt16_MAX);        
	
	return FALSE;  // return TRUE  unless you set the focus to a control
	
}		// end "OnInitDialog"    



void 
CMClassifyDialog::HideShowClassAreasItem(void)
				
{   	
	if ( !m_trainingAreaFlag && !m_trainingAreaLOOFlag && !m_testAreaFlag)
		MHideDialogItem (this, IDC_AreasCombo);
		
	else		// m_trainingAreaFlag || m_trainingAreaLOOFlag ...
		MShowDialogItem (this, IDC_AreasCombo);
	
}		// end "ShowThresholdItems"

  

void CMClassifyDialog::OnSelendokChannelCombo()
{                                                           
	HandleChannelsMenu(IDC_ChannelCombo, 
								m_featureTransformationFlag,
								(SInt16)gProjectInfoPtr->numberStatisticsChannels,
								2,
								TRUE);
	
}		// end "OnSelendokChannelCombo"

  

void CMClassifyDialog::OnSelendokClassWeightsCombo()
{                                                           
	HandleClassWeightsMenu ((SInt16)m_localNumberClasses, 
									(SInt16*)m_classListPtr,
									m_classWeightsPtr,
									gProjectInfoPtr->covarianceStatsToUse == kEnhancedStats,
									IDC_WeightCombo,
									&m_classWeightsSelection);
	
}		// end "OnSelendokClassWeightsCombo"



void CMClassifyDialog::OnSelendokClassificationProcedure()

{  
	CComboBox* 			comboBoxPtr;
	
	SInt16				classificationProcedure,
							savedClassifyListSelection,
							weightsSelection;
	                  
	Boolean				returnFlag = TRUE;
	
	
	savedClassifyListSelection = m_classifyListSelection;
	                                    
	DDX_CBIndex(m_dialogFromPtr, 
					IDC_ClassificationProcedure, 
					m_classifyListSelection); 
					
			// Get the actual classification procedure code.
			
	comboBoxPtr = (CComboBox*)GetDlgItem(IDC_ClassificationProcedure);
	classificationProcedure = 
					(SInt16)comboBoxPtr->GetItemData(m_classifyListSelection);

			// Get the current weights selection. Force to 1 base.

	weightsSelection = m_classWeightsSelection + 1;
					
	classificationProcedure = ClassifyDialogOnClassificationProcedure ( 
																this,
																IDOK,
																&m_thresholdAllowedFlag,
																&m_featureTransformAllowedFlag,                     
																&weightsSelection,
																&m_parallelPipedCode,
																classificationProcedure,
																&m_covarianceEstimate,
																m_numberEigenvectors,
																&m_classifyProcedureEnteredCode,
																m_optionKeyFlag);
																
	m_classWeightsSelection = weightsSelection - 1;
								
//	if (m_covarianceEstimate != kNoCovarianceUsed)
//		{                                        
//		SetComboItemText (IDC_ClassificationProcedure, 
//									m_correlationComboListItem, 
//									(char*)"Correlation (SAM)...");
//								
//		comboBoxPtr->SetItemData(m_correlationComboListItem, kCorrelationMode);
//		 								
//		}		// end "if (m_covarianceEstimate != kNoCovarianceUsed)"
		
	if (classificationProcedure != 0)
		{
		m_classificationProcedure = classificationProcedure;
			
		ClassifyDialogSetLeaveOneOutItems (this,
														m_classificationProcedure,
														m_fileNamesSelection,
														m_savedLeaveOneOutFlag,
														m_trainingFieldsExistFlag,
														(Boolean*)&m_trainingAreaLOOFlag);
						                     
		CheckFeatureTransformationDialog ( this, 
														m_featureTransformAllowedFlag,
														IDC_FeatureTransformation, 
														IDC_ChannelPrompt, 
														(SInt16*)&m_featureTransformationFlag);
					
		ClassifyDialogSetThresholdItems ( this,
													m_classificationProcedure,
													m_imageAreaFlag,
													m_createProbabilityFileFlag,
													m_thresholdResultsFlag,
													m_thresholdAllowedFlag);

		if (weightsSelection > 0)
			{
			HideDialogItem (this, IDC_WeightsEqual);
			ShowDialogItem (this, IDC_WeightCombo);
			
			DDX_CBIndex(m_dialogToPtr, 
					IDC_WeightCombo, 
					m_classWeightsSelection);

			}		// end "if (weightsSelection > 0)"
		
		else		// weightsSelection <= 0
			{
			ShowDialogItem (this, IDC_WeightsEqual);
			HideDialogItem (this, IDC_WeightCombo);
			
			}		// end "else weightsSelection <= 0"
							
		}		// end "if (classificationProcedure != 0)"
		
	else		// classificationProcedure == 0
		{                      
		m_classifyListSelection = savedClassifyListSelection;
/*	                                    
		DDX_CBIndex(m_dialogToPtr, 
						IDC_ClassificationProcedure, 
						m_classifyListSelection); 
						 
		m_classificationProcedure = 
					(SInt16)comboBoxPtr->GetItemData(m_classifyListSelection);
*/		
		}		// end "else m_classificationProcedure == 0" 
	                                    
	DDX_CBIndex(m_dialogToPtr, 
					IDC_ClassificationProcedure, 
					m_classifyListSelection); 
						 
	m_classificationProcedure = 
				(SInt16)comboBoxPtr->GetItemData(m_classifyListSelection);

	m_optionKeyFlag = FALSE;
	
}		// end "OnSelendokClassificationProcedure"   
 
  

void 
CMClassifyDialog::CheckColumnEnd(void)

{  
	SignedByte							handleStatus;
	
							                 
	if (!m_settingSelectedEntireButton)
		{
		m_dialogSelectArea.imageWindowInfoPtr = 
			(WindowInfoPtr)GetHandleStatusAndPointer( m_targetWindowInfoHandle,
																		&handleStatus,
																		kNoMoveHi);
																		
		CMDialog::CheckColumnEnd(); 
	
		MHSetState (m_targetWindowInfoHandle, handleStatus);  
	
		m_dialogSelectArea.imageWindowInfoPtr = NULL;
		
		}		// end "if (!m_settingSelectedEntireButton)" 
	
}		// end "CheckColumnEnd" 
  

void 
CMClassifyDialog::CheckColumnStart()

{        
	SignedByte							handleStatus;
	
							                 
	if (!m_settingSelectedEntireButton)
		{
		m_dialogSelectArea.imageWindowInfoPtr = 
			(WindowInfoPtr)GetHandleStatusAndPointer( m_targetWindowInfoHandle,
																		&handleStatus,
																		kNoMoveHi);
																		
		CMDialog::CheckColumnStart(); 
	
		MHSetState (m_targetWindowInfoHandle, handleStatus); 
	
		m_dialogSelectArea.imageWindowInfoPtr = NULL;
		
		}		// end "if (!m_settingSelectedEntireButton)" 
	
}		// end "CheckColumnStart"
 
  

void CMClassifyDialog::CheckLineEnd()
{  
	SignedByte							handleStatus;
	
							                 
	if (!m_settingSelectedEntireButton)
		{
		m_dialogSelectArea.imageWindowInfoPtr = 
			(WindowInfoPtr)GetHandleStatusAndPointer( m_targetWindowInfoHandle,
																		&handleStatus,
																		kNoMoveHi);
																		
		CMDialog::CheckLineEnd(); 
	
		MHSetState (m_targetWindowInfoHandle, handleStatus);
	
		m_dialogSelectArea.imageWindowInfoPtr = NULL;
		
		}		// end "if (!m_settingSelectedEntireButton)" 
	
}		// end "CheckLineEnd" 
  

void CMClassifyDialog::CheckLineStart()
{  
	SignedByte							handleStatus;
	
							                 
	if (!m_settingSelectedEntireButton)
		{
		m_dialogSelectArea.imageWindowInfoPtr = 
			(WindowInfoPtr)GetHandleStatusAndPointer( m_targetWindowInfoHandle,
																		&handleStatus,
																		kNoMoveHi);
																		
		CMDialog::CheckLineStart(); 
	
		MHSetState (m_targetWindowInfoHandle, handleStatus); 
	
		m_dialogSelectArea.imageWindowInfoPtr = NULL;
		
		}		// end "if (!m_settingSelectedEntireButton)" 
	
}		// end "CheckLineStart" 



void CMClassifyDialog::CheckOutputFormatItems()
 
{				
			// Make certain that disk file output formats and thresholding 	
			// are consistant with the requested classification 					
			// specifications.  
			       
	DDX_CBIndex(m_dialogFromPtr, 
					IDC_DiskCombo, 
					m_outputFormatCode);
					
	m_outputFormatCode++; 

	ClassifyDialogSetPaletteItems (this,
												m_outputFormatCode,
												m_createImageOverlayFlag);
/*		
	if (m_diskFileFlag && m_outputFormatCode == kClassifyERDAS74OutputFormat)
		{                 
		ShowDialogItem (this, IDC_PalettePrompt);
		ShowDialogItem (this, IDC_PaletteCombo);
		
		}		// end "if (m_outputFormatCode != 2)"
		
	else		// !m_diskFileFlag || m_outputFormatCode != 2
		{                 
		HideDialogItem (this, IDC_PalettePrompt);
		HideDialogItem (this, IDC_PaletteCombo);
		
		}		// end "else !m_diskFileFlag || m_outputFormatCode != 2"
*/					                    
	if ( m_outputFormatCode != 1 )
		m_outputAsciiCode = (m_outputAsciiCode & 0xfffd);
		
	else		// m_outputFormatCode == 1 
		m_outputAsciiCode = (m_outputAsciiCode | 0x0002);
					
				// Check threshold items.  
					
	ClassifyDialogSetThresholdItems ( this,
													m_classificationProcedure,
													m_imageAreaFlag,
													m_createProbabilityFileFlag,
													m_thresholdResultsFlag,
													m_thresholdAllowedFlag);
	
}		// end "CheckOutputFormatItems" 

  

void CMClassifyDialog::OnDiskFile(void)

{                
	DDX_Check(m_dialogFromPtr, 
					IDC_DiskFile, 
					m_diskFileFlag);         
		
	if (!m_diskFileFlag) 
		HideDialogItem (this, IDC_DiskCombo);         
		
	else		// !m_diskFileFlag) 
		ShowDialogItem (this, IDC_DiskCombo);
		  
	CheckOutputFormatItems ();
	
}		// end "OnDiskFile"



void CMClassifyDialog::OnThresholdResults(void)

{ 		       
	DDX_Check(m_dialogFromPtr, 
					IDC_ThresholdResults, 
					m_thresholdResultsFlag);
					
	CheckOutputFormatItems ();
	
}		// end "OnThresholdResults"



void CMClassifyDialog::OnSelendokDiskCombo()
{
	CheckOutputFormatItems ();
	
}



void CMClassifyDialog::OnTextWindow()
{
	// TODO: Add your control notification handler code here
	
} 



void CMClassifyDialog::OnTraining(void)

{                                                                                        
	DDX_Check(m_dialogFromPtr, 
					IDC_Training, 
					m_trainingAreaFlag); 
	
	HideShowClassAreasItem();
	
	CheckAreaSettings ();                    
	
}		// end "OnTraining" 



void CMClassifyDialog::OnTrainingLOO()
{                                                                                                                      
	DDX_Check(m_dialogFromPtr, 
					IDC_TrainingLOO, 
					m_trainingAreaLOOFlag); 
	
	HideShowClassAreasItem();
	
	CheckAreaSettings ();     
	
}		// end "OnTrainingLOO"



void CMClassifyDialog::OnTestAreas(void)

{                                                                                       
	DDX_Check(m_dialogFromPtr, 
					IDC_TestAreas, 
					m_testAreaFlag);  
	
	HideShowClassAreasItem();
	
	CheckAreaSettings ();                    
	
}		// end "OnTestAreas" 



void CMClassifyDialog::OnImageArea(void)

{  
	SignedByte						handleStatus;
	
	                                                              
	DDX_Check(m_dialogFromPtr, 
					IDC_ImageArea, 
					m_imageAreaFlag);                                                     

	HideShowAreaItems(m_imageAreaFlag);
		
			// Determine if this is the entire area and set the 
			// to entire image icon. 
	
	if (m_imageAreaFlag)
		{  				                 
		m_dialogSelectArea.imageWindowInfoPtr = 
			(WindowInfoPtr)GetHandleStatusAndPointer( m_targetWindowInfoHandle,
																	&handleStatus,
																	kNoMoveHi);
		
		SetEntireImageButtons (
						NULL, 
						m_LineStart, 
						m_LineEnd, 
						m_ColumnStart, 
						m_ColumnEnd);  

		MHSetState (m_targetWindowInfoHandle, handleStatus); 
	
		m_dialogSelectArea.imageWindowInfoPtr = NULL;
		
		}		// end ""
										
					
	ClassifyDialogSetThresholdItems ( this,
													m_classificationProcedure,
													m_imageAreaFlag,
													m_createProbabilityFileFlag,
													m_thresholdResultsFlag,
													m_thresholdAllowedFlag);
	
	CheckAreaSettings ();
	
}		// end "OnImageArea"



void CMClassifyDialog::CheckAreaSettings(void)

{  
	Boolean		enableFlag = FALSE;
	
	                                                                                     
	if (m_trainingAreaFlag || 
				m_trainingAreaLOOFlag || 
							m_testAreaFlag || 
										m_imageAreaFlag)
		enableFlag = TRUE;
	
	GetDlgItem(IDOK)->EnableWindow(enableFlag); 
	
}		// end "CheckAreaSettings"   



void CMClassifyDialog::OnFeatureTransformation()
{                                
	DDX_Check(m_dialogFromPtr, 
					IDC_FeatureTransformation, 
					m_featureTransformationFlag);
	
	CheckFeatureTransformationDialog ( this,
													m_featureTransformAllowedFlag, 
													IDC_FeatureTransformation, 
													IDC_ChannelPrompt, 
													(SInt16*)&m_featureTransformationFlag); 

	m_channelSelection = UpdateDialogFeatureParameters (
												m_featureTransformationFlag, 
												&m_localActiveNumberFeatures,
												&m_localActiveFeaturesPtr, 
												m_localNumberFeatures,
												m_localFeaturesPtr,
												gProjectInfoPtr->numberStatisticsChannels, 
												m_localNumberTransformFeatures,
												m_localTransformFeaturesPtr,
												gTransformationMatrix.numberFeatures);  
	
}		// end "OnFeatureTransformation" 



void CMClassifyDialog::OnCreateProbabilityFile()
{                                                           
	DDX_Check(m_dialogFromPtr, 
					IDC_CreateProbabilityFile, 
					m_createProbabilityFileFlag);
	
}		// end "OnCreateProbabilityFile"



void 
CMClassifyDialog::OnSelendokAreasCombo()

{                                                                                            
	HandleClassesMenu(&m_localNumberClassAreas,
								(SInt16*)m_classAreaListPtr,
								1,      
								(SInt16)gProjectInfoPtr->numberStatisticsClasses,
								IDC_AreasCombo,
								&m_classAreaSelection);
	
}		// end "OnSelendokAreasCombo"

  

void 
CMClassifyDialog::OnSelendokPaletteCombo()

{                                                           
	int		lastPaletteSelection;
	
	
	lastPaletteSelection = m_paletteSelection;                                                        
	DDX_CBIndex(m_dialogFromPtr, IDC_PaletteCombo, m_paletteSelection);
	
	if (m_paletteSelection+1 == kFalseColors)
		{
		if ( !FalseColorPaletteDialog() )     
			{                 
			if (lastPaletteSelection != m_paletteSelection)
				{
				m_paletteSelection = lastPaletteSelection;                                            
				DDX_CBIndex(m_dialogToPtr, IDC_PaletteCombo, m_paletteSelection);
				
				}		// end "if (lastPaletteSelection != m_paletteSelection)"
				
			m_paletteSelection = lastPaletteSelection;
			
			}		// end "else !FalseColorPaletteDialog()"
			
		}		// end "if (m_paletteSelection+1 == kFalseColors)"
	
}		// end "OnSelendokPaletteCombo"



void 
CMClassifyDialog::OnDropdownPaletteCombo()

{                                                                                            
	SetUpPalettePopUpMenu (this); 
	
	m_paletteSelection = gProjectInfoPtr->imagePalettePopupMenuSelection - 1;					                             
	DDX_CBIndex(m_dialogToPtr, IDC_PaletteCombo, m_paletteSelection);
	
}		// end "OnDropdownPaletteCombo"



void 
CMClassifyDialog::OnListOptions()

{        
	SetDLogControlHilite (this, IDOK, 255);  
		      
	ListResultsOptionsDialog (
					&m_listResultsTrainingCode, 
					&m_listResultsTestCode); 
					                           
	SetDLogControlHilite (this, IDOK, 0);
	
}		// end "OnListOptions" 



void 
CMClassifyDialog::OnChangeCorrelationCoefficient()

{        
	DDX_Text(m_dialogFromPtr, IDC_CorrelationCoefficient, m_correlationThreshold);
	                                                   
//	if (thresholdValue > 1)
//		RealNumberErrorAlert (saveCorrelationThreshold, dialogPtr, 46, 4);
		
	if (m_correlationThreshold >= 0 && m_correlationThreshold <= 1)
		{
		m_saveCorrelationThreshold = m_correlationThreshold;
		m_angleThreshold = (float)(
						acos(m_saveCorrelationThreshold) * kRadiansToDegrees);
		m_saveAngleThreshold = m_angleThreshold;
		DDX_Text(m_dialogToPtr, IDC_CorrelationThresold, m_angleThreshold);
				
		}		// end "if (m_correlationThreshold >= 0 && ..."
	
}		// end "OnChangeCorrelationCoefficient"



void 
CMClassifyDialog::OnChangeCorrelationThresold()
{                                 
	DDX_Text(m_dialogFromPtr, IDC_CorrelationThresold, m_angleThreshold);
	                          	                         
//	if (thresholdValue > 180)
//		RealNumberErrorAlert (saveAngleThreshold, dialogPtr, 47, 1); 
		
	if (m_angleThreshold >= 0 && m_angleThreshold <= 180)
		{
		m_saveAngleThreshold = m_angleThreshold;
		m_correlationThreshold = (float)cos(
										m_angleThreshold * kDegreesToRadians); 
		m_saveCorrelationThreshold = m_correlationThreshold;   
		DDX_Text(m_dialogToPtr, IDC_CorrelationCoefficient, m_correlationThreshold);
						
		}		// end "if (thresholdValue >= 0 && ..."
	
}		// end "OnChangeCorrelationThresold" 

   

void CMClassifyDialog::OnDropdownClassificationProcedure()
{
	if (GetKeyState (VK_RBUTTON) < 0 || GetKeyState(VK_SHIFT) & 0x8000 ||
							m_covarianceEstimate != kNoCovarianceUsed)
		{
		SetComboItemText (IDC_ClassificationProcedure, 
									m_correlationComboListItem, 
									(UCharPtr)"Correlation (SAM)...");

		m_optionKeyFlag = TRUE;

		}		// end "if (GetKeyState (VK_RBUTTON) < 0 || ..."


	else		// no option requested
		{
		SetComboItemText (IDC_ClassificationProcedure, 
									m_correlationComboListItem, 
									(UCharPtr)"Correlation (SAM)");

		m_optionKeyFlag = FALSE;

		}		// end "else no option requested"

	CComboBox* comboBoxPtr = (CComboBox*)GetDlgItem(IDC_ClassificationProcedure);
	comboBoxPtr->SetItemData(m_correlationComboListItem, kCorrelationMode);

	DDX_CBIndex(m_dialogToPtr,
						IDC_ClassificationProcedure, 
						m_classifyListSelection);
								
}		// end "OnDropdownClassificationProcedure"



void 
CMClassifyDialog::ToEntireImage(void)

{                                   
	SignedByte							handleStatus;
	
							                 
	m_dialogSelectArea.imageWindowInfoPtr = 
		(WindowInfoPtr)GetHandleStatusAndPointer( m_targetWindowInfoHandle,
																	&handleStatus,
																	kNoMoveHi);
																	
	CMDialog::ToEntireImage(); 

	MHSetState (m_targetWindowInfoHandle, handleStatus);
	
	m_dialogSelectArea.imageWindowInfoPtr = NULL;
	
}		// end "ToEntireImage"



void 
CMClassifyDialog::ToSelectedImage(void)

{                               
	SignedByte							handleStatus;
	
							                 
	m_dialogSelectArea.imageWindowInfoPtr = 
		(WindowInfoPtr)GetHandleStatusAndPointer( m_targetWindowInfoHandle,
																	&handleStatus,
																	kNoMoveHi);
																	
	CMDialog::ToSelectedImage(); 

	MHSetState (m_targetWindowInfoHandle, handleStatus);
	
	m_dialogSelectArea.imageWindowInfoPtr = NULL;
	
}		// end "ToSelectedImage" 



void CMClassifyDialog::OnSelendokTargetCombo()

{  
	int					savedFileNamesSelection;
	
	Boolean				checkOKFlag,
							createImageOverlayFlag;
	
	
	savedFileNamesSelection = m_fileNamesSelection;                                                                      
	DDX_CBIndex(m_dialogFromPtr, IDC_TargetCombo, m_fileNamesSelection);
		
	if (m_fileNamesSelection != savedFileNamesSelection)
		{        
		createImageOverlayFlag = m_createImageOverlayFlag;
		ClassifyDialogOnTargetFile ( this,
													m_fileNamesSelection+1,
													&m_targetWindowInfoHandle,
													&checkOKFlag,
													&m_dialogSelectArea,
													&createImageOverlayFlag);
		m_createImageOverlayFlag = createImageOverlayFlag;
													
				// Recheck the image area items. Some may have gotten displayed
				// when they should be hidden.
		
		if (!m_imageAreaFlag)		                               
			HideShowAreaItems(m_imageAreaFlag);
		
		if (checkOKFlag)
			{
			DDX_Check(m_dialogFromPtr, IDC_Training, m_trainingAreaFlag);
			DDX_Check(m_dialogFromPtr, IDC_TestAreas, m_testAreaFlag); 
			DDX_Check(m_dialogFromPtr, IDC_TrainingLOO, m_trainingAreaLOOFlag);
			DDX_Check(m_dialogFromPtr, IDC_ImageArea, m_imageAreaFlag);
												
			CheckAreaSettings ();
			
			}		// end "if (checkOKFlag)"
		
		}		// end "if (m_fileNamesSelection != savedFileNamesSelection)"
	
}		// end "OnSelendokTargetCombo" 

  
  
void CMClassifyDialog::OnImageOverlay()

{   
	DDX_Check(m_dialogFromPtr, IDC_ImageWindowOverlay, m_createImageOverlayFlag);

	if (m_createImageOverlayFlag)
		ShowDialogItem (this, IDC_ImageOverlayCombo);
	else		// m_createImageOverlayFlag
		HideDialogItem (this, IDC_ImageOverlayCombo);
					
	ClassifyDialogSetPaletteItems (this,
												m_outputFormatCode,
												m_createImageOverlayFlag);
		
}		// end "OnImageOverlay"

  

void 
CMClassifyDialog::OnSelendokImageOverlayCombo()

{                                                           
	DDX_CBIndex(m_dialogFromPtr, IDC_ImageOverlayCombo, m_selectImageOverlaySelection);
	
}		// end "OnSelendokImageOverlayCombo"



void CMClassifyDialog::OnCloseupClassificationProcedure() 
{
/*	if (m_optionKeyFlag)
		{                                                       
		SetComboItemText (IDC_ClassificationProcedure, 
									m_correlationComboListItem, 
									(char*)"Correlation (SAM)");
								          
		CComboBox* comboBoxPtr = (CComboBox*)GetDlgItem(IDC_ClassificationProcedure);
		comboBoxPtr->SetItemData(m_correlationComboListItem, kCorrelationMode);

		DDX_CBIndex(m_dialogToPtr,
							IDC_ClassificationProcedure, 
							m_classifyListSelection);

		m_optionKeyFlag = FALSE;

		}		// end "if (m_optionKeyFlag)"
*/	
}		// end "OnCloseupClassificationProcedure"



void CMClassifyDialog::OnStnClickedStartendinterval()
{
	// TODO: Add your control notification handler code here
}
