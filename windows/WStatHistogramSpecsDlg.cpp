// WStatHistogramSpecsDlg.cpp : implementation file
//
            
#include "SMulSpec.h"

#include "WChanDlg.h"
#include "WStatHistogramSpecsDlg.h"

#include	"SExtGlob.h"

extern void 		StatHistogramDialogInitialize (
							DialogPtr							dialogPtr,
							FileInfoPtr							fileInfoPtr,
							SInt16								statsWindowMode,
							UInt16*								localFeaturesPtr, 
							UInt16*								localTransformFeaturesPtr,
							Boolean*								histogramClassFlagPtr,
							SInt32*								lineIntervalPtr,
							SInt32*								columnIntervalPtr,
							Boolean*								featureTransformAllowedFlagPtr,
							Boolean*								featureTransformationFlagPtr,
							SInt16*								channelSelectionPtr,
							UInt16*								localNumberChannelsPtr,
							SInt16*								histogramOutputCodePtr,
							Boolean*								includeEmptyBinsFlagPtr,
							Boolean*								blankValuesFlagPtr,
							SInt16*								groupCodePtr,
							SInt16*								columnMatrixCodePtr,
							Boolean*								overlayDFAllowedFlagPtr,
							Boolean*								overlayDensityFunctionFlagPtr);	
												
extern void 		StatHistogramDialogOK (
							Boolean								histogramClassFlag,
							SInt32								lineInterval,
							SInt32								columnInterval,
							Boolean								featureTransformationFlag,
							SInt16								channelSelection,
							UInt16								localNumberFeatures,
							UInt16*								localFeaturesPtr,
							Boolean								listDataFlag,
							Boolean								includeEmptyBinsFlag,
							Boolean								blankValuesFlag,
							Boolean								plotHistogramFlag,
							Boolean								groupChannelsFlag,
							Boolean								groupFieldsClassesFlag,
							SInt16								columnMatrixCode,
							Boolean								overlayDensityFunctionFlag);

extern void 		StatHistogramDialogClassesParameters (
							DialogPtr							dialogPtr,
							Boolean								listFlag,
							Boolean								overlayDFAllowedFlag,
							Boolean								overlayDensityFunctionFlag);
							
extern void 		StatHistogramDialogFieldsParameters (
							DialogPtr							dialogPtr);
							
extern void 		StatHistogramDialogListParameters (
							DialogPtr							dialogPtr,
							Boolean								histogramClassFlag,
							SInt16								columnMatrixCode);
							
extern void 		StatHistogramDialogPlotParameters (
							DialogPtr							dialogPtr,
							Boolean								histogramClassFlag,
							Boolean								overlayDensityFunctionFlag);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMStatHistogramSpecsDlg dialog


CMStatHistogramSpecsDlg::CMStatHistogramSpecsDlg(CWnd* pParent /*=NULL*/)
	: CMDialog(CMStatHistogramSpecsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMStatHistogramSpecsDlg)
	m_featureTransformationFlag = FALSE;
	m_includeEmptyBinsFlag = FALSE;
	m_blankValuesFlag = FALSE;
	m_overlayDensityFunctionFlag = FALSE;
	m_lineInterval = 1;
	m_columnInterval = 1;
	m_histogramClassFieldCode = 0;
	m_listPlotCode = 0;
	m_channelsClassesCode = 0;
	m_matrixColumnCode = 0;
	//m_channelSelection = -1;
	//}}AFX_DATA_INIT

	m_initializedFlag = CMDialog::m_initializedFlag;
	
	if (m_initializedFlag)
		m_initializedFlag = GetDialogLocalVectors (
														&m_localFeaturesPtr,
														&m_localTransformFeaturesPtr,     
														NULL,
														NULL, 
														NULL,
														NULL,
														NULL,
														NULL);
}



CMStatHistogramSpecsDlg::~CMStatHistogramSpecsDlg(void)
{  
	ReleaseDialogLocalVectors (m_localFeaturesPtr,
											m_localTransformFeaturesPtr,        
											NULL,
											NULL,
											NULL,
											NULL,
											NULL,
											NULL);
	
}		// end "~CMStatHistogramSpecsDlg"


void CMStatHistogramSpecsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMStatHistogramSpecsDlg)
	DDX_Check(pDX, IDC_UseFeatureTransformation, m_featureTransformationFlag);
	DDX_Check(pDX, IDC_IncludeEmptyCheck, m_includeEmptyBinsFlag);
	DDX_Check(pDX, IDC_BlankCheck, m_blankValuesFlag);
	DDX_Check(pDX, IDC_OverlayCheck, m_overlayDensityFunctionFlag);
	DDX_Text(pDX, IDC_LineInterval, m_lineInterval);
	DDV_MinMaxLong(pDX, m_lineInterval, 1, 999999);
	DDX_Text(pDX, IDC_ColumnInterval, m_columnInterval);
	DDV_MinMaxLong(pDX, m_columnInterval, 1, 999999);
	DDX_Radio(pDX, IDC_ClassesRadio, m_histogramClassFieldCode);
	DDX_Radio(pDX, IDC_ListRadio, m_listPlotCode);
	DDX_Radio(pDX, IDC_ChannelsRadio, m_channelsClassesCode);
	DDX_Radio(pDX, IDC_MatrixRadio, m_matrixColumnCode);
	DDX_CBIndex(pDX, IDC_ChannelsCombo, m_channelSelection);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMStatHistogramSpecsDlg, CMDialog)
	//{{AFX_MSG_MAP(CMStatHistogramSpecsDlg)
	ON_BN_CLICKED(IDC_ClassesRadio, OnClassesRadio)
	ON_BN_CLICKED(IDC_FieldsRadio, OnFieldsRadio)
	ON_BN_CLICKED(IDC_ListRadio, OnListRadio)
	ON_BN_CLICKED(IDC_PlotRadio, OnPlotRadio)
	ON_BN_CLICKED(IDC_UseFeatureTransformation, OnFeatureTransformation)
	ON_CBN_SELENDOK(IDC_ChannelsCombo, OnSelendokChannelCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



//-----------------------------------------------------------------------------
//								 Copyright (1988-1998)
//								c Purdue Research Foundation
//									All rights reserved.
//
//	Function name:		void DoDialog
//
//	Software purpose:	The purpose of this routine is to present the display
//							specification dialog box to the user and copy the
//							revised back to the display specification structure if
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
//	Coded By:			Larry L. Biehl			Date: 09/26/2002
//	Revised By:			Larry L. Biehl			Date: 09/26/2002	

Boolean 
CMStatHistogramSpecsDlg::DoDialog(
				StatHistogramSpecsPtr			statHistogramSpecsPtr,
				FileInfoPtr							fileInfoPtr)

{ 
	// oul: changed from SInt16 to SInt64
	SInt64								returnCode; 

	Boolean								continueFlag = FALSE;
	
	
			// Make sure intialization has been completed.
							                         
	if ( !m_initializedFlag )
																			return(FALSE);
																			
	m_statHistogramSpecsPtr = statHistogramSpecsPtr;
	m_fileInfoPtr = fileInfoPtr;			
																					
	returnCode = DoModal ();
	
	if (returnCode == IDOK)
		{ 
		StatHistogramDialogOK ((m_histogramClassFieldCode==0),
										m_lineInterval,
										m_columnInterval,
										m_featureTransformationFlag,
										m_channelSelection,
										m_localActiveNumberFeatures,
										m_localActiveFeaturesPtr,
										(m_listPlotCode==0),
										m_includeEmptyBinsFlag,
										m_blankValuesFlag,
										(m_listPlotCode==1),
										(m_channelsClassesCode==0),
										(m_channelsClassesCode==1),
										m_matrixColumnCode+1,
										m_overlayDensityFunctionFlag);

		continueFlag = TRUE;
		
		}		// end "if (returnCode == IDOK)"
		
	return (continueFlag);
		
}		// end "DoDialog"

/////////////////////////////////////////////////////////////////////////////
// CMStatHistogramSpecsDlg message handlers

BOOL CMStatHistogramSpecsDlg::OnInitDialog() 
{
	SInt16				channelSelection,
							groupCode,
							matrixColumnCode,
							outputCode;
	
	Boolean				histogramClassFlag;


	CMDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	StatHistogramDialogInitialize (this,
												m_fileInfoPtr,
												gProjectInfoPtr->statsWindowMode,
												m_localFeaturesPtr, 
												m_localTransformFeaturesPtr,
												&histogramClassFlag,
												&m_lineInterval,
												&m_columnInterval,
												&m_featureTransformAllowedFlag,
												(Boolean*)&m_featureTransformationFlag,
												&channelSelection,
												&m_localActiveNumberFeatures,
												&outputCode,
												(Boolean*)&m_includeEmptyBinsFlag,
												(Boolean*)&m_blankValuesFlag,
												&groupCode,
												&matrixColumnCode,
												&m_overlayDFAllowedFlag,
												(Boolean*)&m_overlayDensityFunctionFlag); 
	
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

	m_channelSelection = channelSelection;
	
	m_histogramClassFieldCode = 1;
	if (histogramClassFlag)
		m_histogramClassFieldCode = 0;

	m_listPlotCode = outputCode - 1;

	m_channelsClassesCode = groupCode - 1;

	m_savedMatrixColumnCode = matrixColumnCode - 1;
	m_matrixColumnCode = m_savedMatrixColumnCode;
	if (m_listPlotCode == 1)
		m_matrixColumnCode = -1;
		
	if (UpdateData(FALSE) )                   
		PositionDialogWindow ();
	
			// Set default text selection to first edit text item	
		                                                     
	SelectDialogItemText (this, IDC_LineInterval, 0, SInt16_MAX); 
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CMStatHistogramSpecsDlg::OnClassesRadio() 
{
	DDX_Radio(m_dialogFromPtr, IDC_ListRadio, m_listPlotCode);

	StatHistogramDialogClassesParameters (this,
														(m_listPlotCode==0),
														m_overlayDFAllowedFlag,
														m_overlayDensityFunctionFlag);
	
}		// end "OnClassesRadio"


void CMStatHistogramSpecsDlg::OnFeatureTransformation()
{                                
	DDX_Check(m_dialogFromPtr, 
					IDC_UseFeatureTransformation, 
					m_featureTransformationFlag);
	
	CheckFeatureTransformationDialog ( this,
													m_featureTransformAllowedFlag, 
													IDC_UseFeatureTransformation, 
													IDC_ChannelsPrompt, 
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


void CMStatHistogramSpecsDlg::OnFieldsRadio() 
{
	StatHistogramDialogFieldsParameters (this);
	
}		// end "OnFieldsRadio"



void CMStatHistogramSpecsDlg::OnListRadio() 
{
	DDX_Radio(m_dialogFromPtr, IDC_ClassesRadio, m_histogramClassFieldCode);

	StatHistogramDialogListParameters (this,
													(m_histogramClassFieldCode==0),
													m_savedMatrixColumnCode+1);
	
}		// end "OnListRadio"



void CMStatHistogramSpecsDlg::OnPlotRadio() 
{
	DDX_Radio(m_dialogFromPtr, IDC_ClassesRadio, m_histogramClassFieldCode);

	DDX_Radio(m_dialogFromPtr, IDC_MatrixRadio, m_matrixColumnCode);
	if (m_matrixColumnCode >= 0)
		m_savedMatrixColumnCode = m_matrixColumnCode;
	
	StatHistogramDialogPlotParameters (this,
													(m_histogramClassFieldCode==0),
													m_overlayDensityFunctionFlag);
	
}		// end "OnPlotRadio"


void CMStatHistogramSpecsDlg::OnSelendokChannelCombo()
{                                                                                                   
	HandleChannelsMenu(IDC_ChannelsCombo, 
								m_featureTransformationFlag,
								(SInt16)gProjectInfoPtr->numberStatisticsChannels,
								2,
								TRUE);
	
}		// end "OnSelendokChannelCombo"
