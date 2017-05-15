//	 									MultiSpec
//
//					Laboratory for Applications of Remote Sensing
//									Purdue University
//								West Lafayette, IN 47907
//								 Copyright (1988-2010)
//								� Purdue Research Foundation
//									All rights reserved.
//
//	File:						SAuxEcho.cpp
//
//	Authors:					Byeungwoo Jeon
//								Larry L. Biehl
//
//	Revision number:		2.8
//
//	Revision date:			08/15/2010
//
//	Language:				C
//
//	System:					Macintosh and Windows Operating Systems
//
//	Brief description:	
//
//	Functions in file:	void 						free_epix
//								double 					log_lik_ratio
//								short int 				malloc_epix
//								short int 				setup_basic_echo_information
//								void 						setup_echo_image_selection
//								void 						stuffing
//								unsigned	short int 	subtract_log_lik
  
#if defined multispec_win | defined multispec_lin 
	#include	"SMulSpec.h" 
	#include "SExtGlob.h"
#endif	// defined multispec_win    

#include	"SEcho.h"


//------------------------------------------------------------------------------------
//								 	Copyright (1989-2010)
//								� Purdue Research Foundation
//									All rights reserved.
//
//	Function name:		Boolean malloc_epix
//
//	Software purpose:	Allocate memory to "EPIXEL" and "FLDLIK" and set initial icel to 
//							zero. "Common_info" must be set up first before calling this 
//							routine. "THRESHOLD_PHASE1" is memory allocated at 
//							"setup_basic_echo_information"
//							Prerequsite : call setup_basic_echo_information() and 
//							setup_echo_image_selection() (in aux_echo.c)
//
//	Parameters in:					
//
//	Parameters out:				
//
// Value Returned:	Flag indicating whether to continue or not.
//
// Called By:			EchoClassifier in SEchoSpec.cpp
//
//	Coded By:			Byeungwoo Jeon			Date: 01/01/1989
// Revised By:			Byeungwoo Jeon			Date: 04/13/1992
//	Revised By:			Larry L. Biehl			Date: 08/17/2010

Boolean malloc_epix(
				EchoClassifierVar*			echoInfoPtr)

{
	Boolean					continueFlag;
	
	UInt32					bytes1,
								current_max_field_number,
								ncls;  
   
  	UInt16					nband;
   							

   ncls    = echoInfoPtr->ncls;
   nband   = echoInfoPtr->nband;
   
   current_max_field_number = 
   						EstimateNumberOfHomogeneousFields (echoInfoPtr);
   echoInfoPtr->current_max_field_number = current_max_field_number;

			// Memory Allocation to EPIXEL													

	if (echoInfoPtr->useTempDiskFileFlag)
		bytes1 = 2 * echoInfoPtr->num_col * echoInfoPtr->cell_width;
			
	else		// !echoInfoPtr->useTempDiskFileFlag
		bytes1 = (UInt32)echoInfoPtr->ipixels;
	
	bytes1 *= sizeof (UInt32);	
	echoInfoPtr->epix_ibufPtr = (HSInt32Ptr)MNewPointer(bytes1);		
   continueFlag = (echoInfoPtr->epix_ibufPtr != NULL);
	
	echoInfoPtr->epix_ibuf2Ptr = echoInfoPtr->epix_ibufPtr;
	if (continueFlag && echoInfoPtr->useTempDiskFileFlag)
		echoInfoPtr->epix_ibuf2Ptr = &echoInfoPtr->epix_ibufPtr[
										echoInfoPtr->num_col * echoInfoPtr->cell_width];
	
			// Memory Allocation to FLDLIK												

   if (continueFlag) 
   	{
      bytes1 = (SInt32)3 * echoInfoPtr->ncl_fldlik * ncls * sizeof(FldLik);
      if (bytes1 > 0)
      	{ 
			echoInfoPtr->fldlikPtr = (HFldLikPtr)MNewPointer(bytes1);
   		continueFlag = (echoInfoPtr->fldlikPtr != NULL);
   		
   		}		// end "if (bytes1 > 0)"
   	
   	}	//	"if (continueFlag) "	

			// Memory Allocation to field likelihood table index vector								
 
	if (continueFlag) 
   	{
      bytes1 = (current_max_field_number+1) * sizeof(UInt32);
     	echoInfoPtr->fieldLikelihoodTableIndexPtr = (HUInt32Ptr)MNewPointer(bytes1);
   	continueFlag = (echoInfoPtr->fieldLikelihoodTableIndexPtr != NULL);
   	   
   	}	//	"if (continueFlag)"									
	
			// Memory Allocation to field class number vector							
	
	if (continueFlag) 
   	{
      bytes1 = (current_max_field_number+1) * sizeof(SInt16);
     	echoInfoPtr->fieldClassNumberPtr = (HSInt16Ptr)MNewPointer(bytes1);
   	continueFlag = (echoInfoPtr->fieldClassNumberPtr != NULL);
		
   	}	//	"if (continueFlag)"	

			// Memory Allocation to field_number_table								

	if (continueFlag) 
  		{
      bytes1 = (current_max_field_number+1) * sizeof(UInt32);
     	echoInfoPtr->field_number_table = (HUInt32Ptr)MNewPointer(bytes1);
   	continueFlag = (echoInfoPtr->field_number_table != NULL);
   	  
   	}	//	"if (continueFlag)"	

			// Memory Allocation to work1 vector 										

	if (continueFlag) 
  		{
      bytes1 = ncls;
      if(nband > ncls) 
   		bytes1 = nband;
      bytes1 *= sizeof(double);
      echoInfoPtr->work1 = (double *)MNewPointer(bytes1);
   	continueFlag = (echoInfoPtr->work1 != NULL);
   	  	
		}	//	"if (continueFlag)"	

			// Memory Allocation to work2 vector 										

	if (continueFlag) 
  		{
      bytes1 = (UInt32)nband * (nband+1)/2;
      if( (UInt32)(4*ncls) > bytes1 ) 
   		bytes1 = 4 * ncls;
      bytes1 *= sizeof(double);
      echoInfoPtr->work2 = (HDoublePtr)MNewPointer(bytes1);
   	continueFlag = (echoInfoPtr->work2 != NULL);
   	  		
		}	//	"if (continueFlag)"	

			// Memory Allocation to cellClassPtr 										

	if (continueFlag) 
  		{
      bytes1 = (UInt32)echoInfoPtr->ncl_fldlik * sizeof(SInt16);
      if (bytes1 > 0)
      	{
      	echoInfoPtr->cellClassPtr = (HSInt16Ptr)MNewPointer(bytes1);
   		continueFlag = (echoInfoPtr->cellClassPtr != NULL);
   		
   		}		// end "if (bytes1 > 0)"
   	  	
		}	//	"if (continueFlag)"	

			// Memory Allocation to fieldLikeIndicesPtr 								

	if (continueFlag) 
  		{
      bytes1 = (UInt32)echoInfoPtr->ncl_fldlik * 2 * sizeof(FldLik*);
      if (bytes1 > 0)
      	{
     		echoInfoPtr->fieldLikeIndicesPtr = (HFldLikPtr*)MNewPointer(bytes1);
   		continueFlag = (echoInfoPtr->fieldLikeIndicesPtr != NULL);
   		
   		}		// end "if (bytes1 > 0)"
   	  	
		}	//	"if (continueFlag)"	

			// Memory Allocation to fieldLikeFlagsPtr									

	if (continueFlag) 
  		{
      bytes1 = (UInt32)echoInfoPtr->ncl_fldlik * 2 * sizeof(char);
      if (bytes1 > 0)
      	{
      	echoInfoPtr->fieldLikeFlagsPtr = (HPtr)MNewPointer(bytes1);
   		continueFlag = (echoInfoPtr->fieldLikeFlagsPtr != NULL);
   		
   		}		// end "if (bytes1 > 0)"
   	  	
		}	//	"if (continueFlag)"	
  
   return (continueFlag);

}	// End "malloc_epix"



//------------------------------------------------------------------------------------
//								 	Copyright (1989-2010)
//								� Purdue Research Foundation
//									All rights reserved.
//
//	Function name:		void free_epix
//
//	Software purpose:	Release memory assigned to EchoClassifierVar structure. 
//
//	Parameters in:					
//
//	Parameters out:				
//
// Value Returned:	None
//
// Called By:			EchoClassifier in SEchoSpec.cpp
//
//	Coded By:			Byeungwoo Jeon			Date: 01/01/1989
//	Revised By:			Larry L. Biehl			Date: 08/15/2010

void free_epix(
				EchoClassifierVar*				echoInfoPtr)
				
{   
   echoInfoPtr->field_number_table = 
   					CheckAndDisposePtr(echoInfoPtr->field_number_table);
   					
   echoInfoPtr->fieldClassNumberPtr = (HSInt16Ptr)CheckAndDisposePtr(
													(Ptr)echoInfoPtr->fieldClassNumberPtr);
	
   echoInfoPtr->fieldLikelihoodTableIndexPtr = (HUInt32Ptr)CheckAndDisposePtr(
													(Ptr)echoInfoPtr->fieldLikelihoodTableIndexPtr);
		
   echoInfoPtr->fldlikPtr = (HFldLikPtr)CheckAndDisposePtr(
   										(Ptr)echoInfoPtr->fldlikPtr);
   	
	echoInfoPtr->epix_ibufPtr = CheckAndDisposePtr(
												echoInfoPtr->epix_ibufPtr);
	echoInfoPtr->epix_ibuf2Ptr = NULL;
   	
   echoInfoPtr->work1 = CheckAndDisposePtr(
   													echoInfoPtr->work1);
   													
   echoInfoPtr->work2 = CheckAndDisposePtr(
   													echoInfoPtr->work2);
   													
   echoInfoPtr->cellClassPtr = CheckAndDisposePtr(
   											echoInfoPtr->cellClassPtr);
   	
   echoInfoPtr->fieldLikeIndicesPtr = (HFldLikPtr*)CheckAndDisposePtr(
   								(Ptr)echoInfoPtr->fieldLikeIndicesPtr);
   	
   echoInfoPtr->fieldLikeFlagsPtr = CheckAndDisposePtr(
												echoInfoPtr->fieldLikeFlagsPtr);
												
	MDeleteFile (echoInfoPtr->tempFileStreamPtr, kNoErrorMessages);
	
	echoInfoPtr->tempFileStreamPtr = DisposeCMFileStream (
												echoInfoPtr->tempFileStreamPtr);
												
	echoInfoPtr->useTempDiskFileFlag = FALSE;
												
}		// end of function "free_epix" 



//====================================================================
//   FUNCTION : setup_basic_echo_information
//   Purpose  : Set up echo_information unrelated to image selection
//   		Common_info must be set up first before calling
//   		this routine.
//   Globals    : none
//   Returns    : error_code
//   Called by  : echo() 			in echo.c	(in ECN)
//   	    	EchoClsfierControl() 	in echo_spec.c	(in Mac)
//   	    	
//   Coded By   : Byeungwoo Jeon		Date: 01/01/89
//   Revised By :	Byeungwoo Jeon		Date: 02/10/2000
//====================================================================

SInt16 setup_basic_echo_information(
				common_classifier_information		*ci_ptr,
				EchoClassifierVar						*e_ptr)
				
{
   double									homogeneityThreshold,
   											minus_natural_log10;
   
   parameter_information				*pi_ptr;
   statistics								*class_stat,
   											*stat_ptr;
   											
 	UInt32									bytes,
 												classIndex,
 												ncls;
   
   SInt16									cell_size,
   											cell_width,
   											degree_of_freedom,
   											nfeature;
         
   Boolean									continueFlag;
      

			//	CHK Here																			
			
   minus_natural_log10 = -2.302585;

			// Set up some of the local variables										

   class_stat	= ci_ptr->class_stat;
   pi_ptr 	= &(ci_ptr->parameter_info);
   ncls 	= ci_ptr->stat_info.ncls;
   nfeature 	= ci_ptr->stat_info.nfeatures;

			// Set up ECHO Variables to the user defined parameters
		   //  Parameters user should specifie are :
		   //    1. Cell_width
		  	// 	2. homogeneityThreshold (phase 1)
			// 	3. annexationThreshold (phase 2)
			// 	4. Loglik Rejection level,
			//	 if variable homogeneityThreshold is used in phase1
		   //  In case of MULTISPEC, following 4 variables are set up at 
		   //	 EchoClassifyDialog()	in	classifiy.c										

   e_ptr->cell_width 		 = pi_ptr->icelwth;
   e_ptr->homogeneityThreshold	 = pi_ptr->thd1;
   e_ptr->annexationThreshold	 = pi_ptr->thd2; 
   e_ptr->fixed_homogeneityThreshold_option 
   	 			 = pi_ptr->fixed_threshold_option;

   cell_width = e_ptr->cell_width;

			// Parameters derived from user specifications given above
   
   e_ptr->cell_size = cell_size = cell_width * cell_width;
   e_ptr->annexationThreshold_derived 
   	= e_ptr->annexationThreshold * minus_natural_log10 * e_ptr->cell_size; 
   
			// Allocate Memory to homogeneityThreshold

   bytes = (UInt32)ncls * sizeof(double);
   e_ptr->thresholds_phase1 = (double*)MNewPointer(bytes);
   continueFlag = (e_ptr->thresholds_phase1 != NULL);
   
			// Phase 1 Threshold Decision 

   if (continueFlag) 
   	{
      if( e_ptr->fixed_homogeneityThreshold_option ) 
      	{ 
			homogeneityThreshold = -1 * (e_ptr->homogeneityThreshold);
			for(classIndex=0; classIndex<ncls; classIndex++)	// Fixed THD1
	   		e_ptr->thresholds_phase1[classIndex] = homogeneityThreshold;
	   		
   		}		// end "if( e_ptr->fixed_homogeneityThreshold_option )"
   
  		else		// !e_ptr->fixed_homogeneityThreshold_option
   		{					// Variable THD1
      
			homogeneityThreshold = e_ptr->homogeneityThreshold / (double)100.;

 			stat_ptr = class_stat;
			for(classIndex=0; classIndex<ncls; classIndex++)
	  			e_ptr->thresholds_phase1[classIndex] = (stat_ptr++)->classConstant;

					//	Determine Threshold Value of each class.
	
			degree_of_freedom = cell_size * nfeature;
			threshold(ncls,degree_of_freedom,e_ptr->thresholds_phase1,
						((double) 1 - homogeneityThreshold),(double) cell_size);

 			stat_ptr = class_stat;
								
      	}		// end "else !e_ptr->fixed_homogeneityThreshold_option"
      
   	}		// end "if (continueFlag)"
 
			// Copy Information from common_classifier_info
   
   e_ptr->ncls 	= ncls;
   e_ptr->nband = ci_ptr->pixel_info.nband;
   e_ptr->nfeature = nfeature;	
   
   return (continueFlag);
   
}	// End "setup_basic_echo_information"



//====================================================================
//   FUNCTION : SETUP_ECHO_IMAGE_SELECTION
//   Purpose  : Set up parameters related to the size of image selection
//   	Common_info must be set up first before calling this routine.
//   Globals  : none
//   Returns  : none
//   Called by:  EchoClassifier () 	in echospec.c (Multispec)
//   	       echo ()			in echo.c     (ECN)
//   Coded By   : Byeungwoo Jeon		Date : 01/01/89
//   Revised by : Byeungwoo Jeon		Date : 09/28/91
//====================================================================

void setup_echo_image_selection(
				common_classifier_information		*c_info,
				EchoClassifierVar						*echo_info)

{
   SInt32							num_row,
   									num_col;
   									
   UInt32							ipixels,
   									ncls;
   									
   SInt16							cell_width;
   
   
			// Set up local variables   

   cell_width	= echo_info->cell_width;
   ncls		= echo_info->ncls;
   num_row	= c_info->pixel_info.num_row;
   num_col	= c_info->pixel_info.num_col;
   ipixels	= c_info->pixel_info.ipixels;
   
			//		Set up ECHO variables  
    
   echo_info->nrw_fldlik = (SInt32)(num_row / cell_width);
   echo_info->ncl_fldlik = (SInt32)(num_col / cell_width);
   
   echo_info->nrw_icel	 = echo_info->nrw_fldlik * cell_width;
   echo_info->ncl_icel   = echo_info->ncl_fldlik * cell_width;
   
   echo_info->nrw_rem_icel = num_row - echo_info->nrw_icel;
   echo_info->ncl_rem_icel = num_col - echo_info->ncl_icel;
   
   echo_info->lastrow_echo = echo_info->nrw_icel - cell_width;
   echo_info->lastcol_echo = echo_info->ncl_icel - cell_width;
   
   echo_info->icel_linlen  = (UInt32)num_col * cell_width;
   echo_info->line_fldlik  = (UInt32)echo_info->ncl_fldlik * ncls;
   
			// Set up Output Parameters of ECHO
	   
   echo_info->field_size 		  = 0;
   echo_info->number_of_fields 		  = 0;
   echo_info->number_of_homogeneous_cells = 0;

			// Copy Information from common_classifier_info
   
   echo_info->num_row	= num_row;
   echo_info->num_col 	= num_col;
   echo_info->ipixels	= ipixels;

}	// End "setup_echo_image_selection"



//===========================================================
//   FUNCTION : stuffing
//   Purpose  :
//   Called by : phase1()	in	phase1.c
//==========================================================

void stuffing(
				HSInt32Ptr				epix_ptr, 
				SInt32					value, 
				UInt16					cell_length, 
				UInt16					cell_width, 
				SInt32					line_epix)

{
	HSInt32Ptr					epix_ptr1;
    
	UInt32				 		kx,
   								ky;
   								
   								
	for(ky=0; ky<cell_length; ky++) 
	   {
		epix_ptr1 = epix_ptr;
		for(kx=0; kx<cell_width; kx++) 
			*epix_ptr1++ = value;
			
		epix_ptr += line_epix;
		
	   }	// end "for(ky=0; ky<cell_length; ky++)"

}		// End "stuffing"



//===================================================================
//   FUNCTION : log_lik_ratio
//   Purpose  : Compute Lik. ratio
//   Coded By   : Byeungwoo Jeon		Date : 01/01/89
//   Revised by : Byeungwoo Jeon		Date : 09/28/91
//   Revised by : Larry Biehl			Date : 05/04/96
//====================================================================

double log_lik_ratio(
				HFldLikPtr								field_likPtr,
				HFldLikPtr								cell_likPtr,
				HFldLikPtr								auxlikPtr,
				UInt16									fieldClass,
				UInt16									cellClass,
				UInt16*									auxClassPtr,
				UInt32									numberClasses)
				

{
 	double					denominator,
       						smax;
   
   UInt32					classIndex;
   

	if (fieldClass == cellClass)
		{
	   for(classIndex=1; classIndex<=numberClasses; classIndex++) 
	   	{
	      *auxlikPtr = *field_likPtr + *cell_likPtr;
	
	      auxlikPtr++;
	      field_likPtr++;
	      cell_likPtr++;
	      
	   	}	// end " for(classIndex=1; classIndex<=numberClasses; ..."
	   
	   *auxClassPtr = fieldClass;	
	   smax = 0.;
		
		}		// end "if (fieldClass == cellClass)"
		
	else		// fieldClass != cellClass
		{
	   *auxClassPtr = 1;
	   smax    = *auxlikPtr = *field_likPtr + *cell_likPtr;
	   auxlikPtr++;
	   field_likPtr++;
	   cell_likPtr++;
	
	   for(classIndex=2; classIndex<=numberClasses; classIndex++) 
	   	{
	      *auxlikPtr = *field_likPtr + *cell_likPtr;
	      if(*auxlikPtr > smax)  
	      	{
		   	smax    = *auxlikPtr;
		   	*auxClassPtr = (UInt16)classIndex;
		   	
	      	}	// "if(*auxlik > smax)"
	
	      auxlikPtr++;
	      field_likPtr++;
	      cell_likPtr++;
	      
	   	}	// end "for(classIndex=2; classIndex<=ncls; classIndex++)"
		
		if (*auxClassPtr == fieldClass)
			{
					// The maximum class is fieldClass
   		
			cell_likPtr -= numberClasses;		
			smax = *(cell_likPtr + fieldClass - 1) - *(cell_likPtr + cellClass - 1);
			
			}		// end "if (smax2 > smax1)"
			
		else if (*auxClassPtr == cellClass)
			{
					// The maximum class is cellClass
					
			field_likPtr -= numberClasses;	
			smax = *(field_likPtr + cellClass - 1) - *(field_likPtr + fieldClass - 1);
			
			}		// end "else smax2 <= smax"
			
		else		// *auxClassPtr != cellClass != fieldClass
			{
			cell_likPtr -= numberClasses;	
			field_likPtr -= numberClasses;
	   	denominator = *(cell_likPtr + cellClass - 1) + 
	   														*(field_likPtr + fieldClass -1);
	   	smax -= denominator;
	   	
	   	}		// end "else		// *auxClassPtr != cellClass != fieldClass"
	   
	 	}		// end "else fieldClass != cellClass"		

   return (smax);

}	// End "log_lik_ratio"



//===================================================================
//   FUNCTION : subtract_log_lik
//   Purpose  : subtract input cell likelihood values from input field
//   		likelihood values and compute new field class.
//   Coded By   : Larry Biehl		Date : 05/08/92
//   Revised by : Larry Biehl		Date : 05/02/96
//====================================================================

UInt16 subtract_log_lik(
				HFldLikPtr						fldlik,
				HFldLikPtr						cell_lik,
				UInt32							ncls)
				
{
 	double					smax;
   
  	UInt32					classIndex;
   UInt16					auxcls;
   

   auxcls = 1;
   *fldlik -= *cell_lik;
   smax    = *fldlik;
   fldlik++;
   cell_lik++;

   for(classIndex=2; classIndex<=ncls; classIndex++) 
   	{
      *fldlik -= *cell_lik;
      if(*fldlik > smax)  
      	{
	   	smax    = *fldlik;
	   	auxcls = (UInt16)classIndex;
	   
      	}	// "if(*fldlik > smax)"

      fldlik++;
      cell_lik++;
      
   }	// "for(classIndex=2; classIndex<=ncls; classIndex++)"

   return(auxcls);

}	// End "subtract_log_lik"
