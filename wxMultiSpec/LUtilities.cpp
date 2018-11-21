//	 									MultiSpec
//
//					Laboratory for Applications of Remote Sensing
//									Purdue University
//								West Lafayette, IN 47907
//							 Copyright (1988-2017)
//							(c) Purdue Research Foundation
//									All rights reserved.
//
//	File:						LUtilities.cpp
//
//	Authors:					Larry L. Biehl, Tsung Tai Yeh
//
//	Revision number:		3.0
//
//	Revision date:			10/19/2018
//
//	Language:				C
//
//	System:					Linux Operating Systems
//
//	Brief description:	The purpose of the routines in this file is to provide
//								utility type functions in MultiSpec for Linux OS.
//
//	Functions in file:	
//
//------------------------------------------------------------------------------------

#include "SMultiSpec.h"
#include	<ctype.h>  

#include "CImageWindow.h"
#include "CDisplay.h"

#if defined multispec_lin
	#include "wx/combobox.h"
	#include "wx/stdpaths.h"
	#include "LImageView.h"
#endif



extern void GetApplicationStartupPath (
				char*									startupPathPtr,
				SInt32*								sizeOfPathPtr);



		// Prototypes for routines in this file that are only called by other
		// routines in this file.



//------------------------------------------------------------------------------------
//								 Copyright (1988-2017)
//								(c) Purdue Research Foundation
//									All rights reserved.
//
//	Function name:		CopyOffScreenImage
//
//	Software purpose:	This routine copies an off screen bit/pix map
//							to the specified window base on the routine 
//							defined by copyType.
//
//	Parameters in:				
//
//	Parameters out:				
//
//Value Returned:		None
// 
// Called By:			DoUpdateEvent
//
//	Coded By:			Larry L. Biehl			Date: 08/31/1988
//	Revised By:			Larry L. Biehl			Date: 06/01/2017		
// TODO: For Linux

void CopyOffScreenImage (
				CMImageView*						imageViewCPtr,
				CDC*									pDC,
				CMImageWindow*						imageWindowCPtr,
				RgnHandle							invalidRgn,
				Rect*									inSourceRect,
						// see descriptions in cases for switch statement below.
				SInt16								copyType)

{
   double								magnification;

   LongRect								lDestinationRect,
											lSourceRect;

   Rect									destinationRect,
											sourceRect,
											windowRect;

   CMPalette*							paletteCPtr;
	
	wxPoint								scrollOffset;
	
   Handle								displaySpecsHandle,
											imageBaseAddressH,
											offScreenMapHandle;

   WindowInfoPtr						windowInfoPtr;

   DisplaySpecsPtr					displaySpecsPtr;

   SInt32								hOrigin,
											numberImageOverlays,
											vOrigin;

   UInt32								numberOverlays;

   SInt16								grafPortType,
											xDimension,
											yDimension;

   SInt16								legendWidth,
											titleHeight,
											windowCode;

   Boolean								drawBaseImageFlag,
											drawVectorOverlaysFlag,
											projectWindowFlag;


   if (imageViewCPtr->CheckIfOffscreenImageExists ())
		{
				// Intialize some local variables.

      windowInfoPtr = (WindowInfoPtr) GetHandlePointer (
															imageWindowCPtr->GetWindowInfoHandle ());
		
      legendWidth = 0;

      titleHeight = windowInfoPtr->titleHeight;
      grafPortType = windowInfoPtr->grafPortType;
      numberImageOverlays = windowInfoPtr->numberImageOverlays;
      drawVectorOverlaysFlag = windowInfoPtr->drawVectorOverlaysFlag;
      numberOverlays = windowInfoPtr->numberOverlays;
      drawBaseImageFlag = windowInfoPtr->drawBaseImageFlag;

      offScreenMapHandle = windowInfoPtr->offScreenMapHandle;
      imageBaseAddressH = windowInfoPtr->imageBaseAddressH;

      projectWindowFlag = windowInfoPtr->projectWindowFlag;

      GetWindowClipRectangle (imageViewCPtr, kImageArea, &windowRect);

				// Get handle and pointer to display specification for window

      displaySpecsHandle = imageViewCPtr->m_displayMultiCPtr->mDisplaySpecsHandle;
      displaySpecsPtr = (DisplaySpecsPtr)GetHandlePointer (displaySpecsHandle);

      magnification = displaySpecsPtr->magnification;
      vOrigin = displaySpecsPtr->origin[0];
      hOrigin = displaySpecsPtr->origin[1];
      yDimension = (SInt16) displaySpecsPtr->imageDimensions[0];
      xDimension = (SInt16) displaySpecsPtr->imageDimensions[1];
      
      paletteCPtr = displaySpecsPtr->paletteObject;
      if (((imageViewCPtr != gActiveImageViewCPtr) || gInBackground) &&
              displaySpecsPtr->backgroundPaletteObject != NULL)
         paletteCPtr = displaySpecsPtr->backgroundPaletteObject;

      if (copyType != kClipboardCopy && copyType != kPrinterCopy)
			{
			imageViewCPtr->m_Canvas->CalcUnscrolledPosition (0,
																				0,
																				&scrollOffset.x,
																				&scrollOffset.y);

			windowRect.top += MAX (0 , scrollOffset.y);
			windowRect.bottom += MAX (0, scrollOffset.y);
			windowRect.left += MAX (scrollOffset.x, 0);
			windowRect.right += MAX (scrollOffset.x, 0);

			}	// end "if (copyType != kClipboardCopy && copyType != kPrinterCopy)" 

      else // copyType == kClipboardCopy || copyType == kPrinterCopy)
			{
         scrollOffset.x = 0;
         scrollOffset.y = 0;

			}	// end "else copyType == kClipboardCopy || copyType == kPrinterCopy)"

      if (vOrigin < 0)
         copyType = kDestinationCopy;

				// Set clip region of window to the image portion.						

		//if (copyType != kClipboardCopy)
		//	ClipRect (&windowRect);

				// Branch to appropriate case for copyType.								

      switch (copyType)
			{
         case kClipboardCopy:
						// Map sourceRect to destination that starts at (0,0)
						// This is for copy to clipboard.    

            legendWidth = windowInfoPtr->legendWidth;

            lSourceRect.top = inSourceRect->top;
            lSourceRect.left = inSourceRect->left;
            lSourceRect.bottom = inSourceRect->bottom;
            lSourceRect.right = inSourceRect->right;

            lDestinationRect.top = (SInt32) titleHeight;
            lDestinationRect.left = (SInt32) legendWidth;
            lDestinationRect.bottom = titleHeight + (SInt32)
									((lSourceRect.bottom - lSourceRect.top) * magnification);
            lDestinationRect.right = legendWidth + (SInt32)
									((lSourceRect.right - lSourceRect.left) * magnification);
            break;

         case kSourceCopy:
						// Map to wherever sourceRect * magnification indicates.  This
						// allows one to draw parts of the window at a time.

            lSourceRect.top = inSourceRect->top;
            lSourceRect.left = inSourceRect->left;
            lSourceRect.bottom = inSourceRect->bottom;
            lSourceRect.right = inSourceRect->right;

            lDestinationRect.top =
									(SInt32)(lSourceRect.top * magnification + titleHeight);
            lDestinationRect.left =
									(SInt32)(lSourceRect.left * magnification + legendWidth);
            lDestinationRect.bottom =
									(SInt32)(lSourceRect.bottom * magnification + titleHeight);
            lDestinationRect.right =
									(SInt32)(lSourceRect.right * magnification + legendWidth);
            break;

         case kScrollCopy: // scroll copy
						// Copy portion of offscreen image map that fits in the invalid
						// region of the window. 	This is from a scroll operation.

            //lDestinationRect = (*invalidRgn)->rgnBBox;

         case kDestinationCopy:
						// Copy portion of offscreen map that fits within the input
						// destination area of the window.  This is from a window update
						// event.

            if (copyType == kDestinationCopy)
					{
               lDestinationRect.top = inSourceRect->top;
               lDestinationRect.left = inSourceRect->left;
               lDestinationRect.bottom = inSourceRect->bottom;
               lDestinationRect.right = inSourceRect->right;
               
					}	// end "if (copyType == kDestinationCopy)"

						// Make certain that the legend is not included.

				lDestinationRect.top = (SInt32)(windowRect.top/ magnification);
				lDestinationRect.bottom =
										(SInt32)(windowRect.bottom/ magnification + .9999);
				lDestinationRect.left = (SInt32)(windowRect.left/ magnification);
				lDestinationRect.right =
										(SInt32)(windowRect.right/ magnification + .9999);
				
				lDestinationRect.left = MAX (0, lDestinationRect.left);
				lDestinationRect.top = MAX (0, lDestinationRect.top);
				
				lSourceRect.top = (SInt32)((windowRect.top) / magnification);
				lSourceRect.left = (SInt32)((windowRect.left) / magnification);
				lSourceRect.bottom =
									(SInt32)((windowRect.bottom) / magnification + .9999);
				lSourceRect.right =
									(SInt32)((windowRect.right) / magnification + .9999);
				
				lSourceRect.bottom = MIN (lSourceRect.bottom, yDimension);
				lSourceRect.right = MIN (lSourceRect.right, xDimension);
				lSourceRect.left = MAX (0, lSourceRect.left);
				lSourceRect.top = MAX (0, lSourceRect.top);
				break;	// end "case: kDestinationCopy"

			case kPrinterCopy:
						// Printer. Just map the source to a destination rectangle that
						// starts at 0, 0. The origon has been offset so that the image
						// appears in the middle of the page.									

				lSourceRect.top = inSourceRect->top;
				lSourceRect.left = inSourceRect->left;
				lSourceRect.bottom = inSourceRect->bottom;
				lSourceRect.right = inSourceRect->right;
				legendWidth = windowInfoPtr->legendWidth;

				lDestinationRect.top = (SInt32)
									(titleHeight * imageViewCPtr->m_printerTextScaling + .5);
				lDestinationRect.bottom = (SInt32)
							(titleHeight * imageViewCPtr->m_printerTextScaling +
								magnification * (lSourceRect.bottom - lSourceRect.top) + .5);
				lDestinationRect.left = (SInt32)
								(legendWidth * imageViewCPtr->m_printerTextScaling * 1.4 + .5);
				lDestinationRect.right = (SInt32)
							(legendWidth * imageViewCPtr->m_printerTextScaling * 1.4 +
								magnification * (lSourceRect.right - lSourceRect.left) + .5);
				/*
						// For now just copying windows code
						// Will see later what to do
				 
				lDestinationRect.top = (SInt32) 
								(titleHeight * imageViewCPtr->m_printerTextScaling + .5);
				lDestinationRect.bottom = (SInt32) 
						(titleHeight * imageViewCPtr->m_printerTextScaling +
							magnification * (lSourceRect.bottom - lSourceRect.top) + .5);
				lDestinationRect.left = (SInt32) 
						(legendWidth * imageViewCPtr->m_printerTextScaling * 1.4 + .5);
				lDestinationRect.right = (SInt32) 
						(legendWidth * imageViewCPtr->m_printerTextScaling * 1.4 +
							magnification * (lSourceRect.right - lSourceRect.left) + .5);
				*/
				break;

			}	// end "switch (copyType)"

		/*
		Rect imagerect;
		imagerect.left = 0;
		imagerect.top = 0;
		imagerect.bottom = yDimension * imageViewCPtr->m_Scale;
		imagerect.right = xDimension * imageViewCPtr->m_Scale;
		wxSize windowsize;
		windowsize.x = imageViewCPtr->m_Canvas->size_w;
		windowsize.y = imageViewCPtr->m_Canvas->size_h;
		 
		imageViewCPtr->GetFrame ()->GetClientSize (&windowsize.x, &windowsize.y);
		 
		
		windowRect.top += scrollOffset.y;
		windowRect.bottom += scrollOffset.y;
		windowRect.left += scrollOffset.x;
		windowRect.right += scrollOffset.x;
		
		if (imagerect.right < windowRect.right)
			windowRect.right = imagerect.right;
		 
		if (imagerect.bottom < windowRect.bottom)
			windowRect.bottom = imagerect.bottom;
		*/
		sourceRect.top = (int) lSourceRect.top;
		sourceRect.left = (int) lSourceRect.left;
		sourceRect.bottom = (int) lSourceRect.bottom;
		sourceRect.right = (int) lSourceRect.right;
		/*
		wxRect wxSourceRect = wxRect (lSourceRect.left,
												lSourceRect.top,
												lSourceRect.right-lSourceRect.left+1,
												lSourceRect.bottom-lSourceRect.top+1);
		*/
		destinationRect.top = (int) lDestinationRect.top;
		destinationRect.left = (int) lDestinationRect.left;
		destinationRect.bottom = (int) lDestinationRect.bottom;
		destinationRect.right = (int) lDestinationRect.right;
	
		if (windowInfoPtr->imageType == kMultispectralImageType)
			{
			if (displaySpecsPtr->displayType == k1_ChannelThematicDisplayType)
				{
				int numpentries = paletteCPtr->GetNumberPaletteEntries ();

						// Use palette to assign colors
				
				int tpixels = xDimension * yDimension;
				int pind = 0;
				Handle imagebuffer = malloc (tpixels * 3);
				unsigned char* imagedata = (unsigned char*)imageBaseAddressH;
				unsigned char* imgbufptr = (unsigned char*)imagebuffer;
				
				for (pind = 0; pind<tpixels; pind++)
					{
					paletteCPtr->GetRGB ((int)*(imagedata + 3 * pind),
												imgbufptr,
												imgbufptr + 1,
												imgbufptr + 2);
					imgbufptr = imgbufptr + 3;
					
					}	// end "for (pind=0; pind<tpixels; pind++)"
					
				wxImage ipimage (xDimension, yDimension, (unsigned char*)imagebuffer);

				imageViewCPtr->m_ScaledBitmap = wxBitmap (ipimage);

						// Draw the legend
				
				//if (legendWidth > 0)
				imageViewCPtr->DrawLegend ();
				
				}	// end "if (displaySpecsPtr->displayType == ...)"
				 
			else	// displaySpecsPtr->displayType != k1_ChannelThematicDisplayType
				{
				wxImage ipimage (xDimension,
										yDimension,
										(unsigned char*)imageBaseAddressH,
										true);
				/*
				ipimage.SetPalette (*paletteCPtr);
				(windowInfoPtr->offScreenImage).Create (
															xDimension,
															yDimension,
															(unsigned char*)imageBaseAddressH,
															true);
				windowInfoPtr->offScreenImage = wxImage (
															xDimension,
															yDimension,
															(unsigned char*)imageBaseAddressH, 
															true);
				imageViewCPtr->m_ScaledBitmap = wxBitmap (windowInfoPtr->offScreenImage);
				wxImage ipsubimage = ipimage.GetSubImage (wxSourceRect);
				imageViewCPtr->m_ScaledBitmap = wxBitmap (ipsubimage);
				*/
				imageViewCPtr->m_ScaledBitmap = wxBitmap (ipimage);

				}	// end "else	// displaySpecsPtr->displayType != ..."
				
			}	// end if multispectral image
		
		else	// Thematic image
			{
			int numpentries = paletteCPtr->GetNumberPaletteEntries ();

					  // Use palette to assign colors
			
			int tpixels = xDimension * yDimension;
			int pind = 0;
			Handle imagebuffer = malloc (tpixels * 3);
			unsigned char* imagedata = (unsigned char*) imageBaseAddressH;
			unsigned char* imgbufptr = (unsigned char*) imagebuffer;
			for (pind = 0; pind < tpixels; pind++)
				{
				paletteCPtr->GetRGB ((int) *(imagedata + pind), imgbufptr, imgbufptr + 1, imgbufptr + 2);
				imgbufptr = imgbufptr + 3;
				}
			
			wxImage ipimage (xDimension, yDimension, (unsigned char*) imagebuffer);
			imageViewCPtr->m_ScaledBitmap = wxBitmap (ipimage);

					// Draw the legend
			
			//if (legendWidth > 0)
			//gActiveImageViewCPtr->DrawLegend ();
			imageViewCPtr->DrawLegend ();
			
			}	// end "else Thematic image"
		
		if (numberImageOverlays > 0 || numberOverlays > 0 || projectWindowFlag)
			{
			windowCode = 1;
			if (copyType == kClipboardCopy)
				windowCode = 3;
			else if (copyType == kPrinterCopy)
				windowCode = 2;

			if (windowCode >= 2)
				{
				displaySpecsPtr = (DisplaySpecsPtr)GetHandlePointer (displaySpecsHandle);

				displaySpecsPtr->origin[kVertical] = (SInt16) lSourceRect.top;
				displaySpecsPtr->origin[kHorizontal] = (SInt16) lSourceRect.left;
				
				}	// end "if (windowCode >= 2)"
			
			}	// end "if (numberImageOverlays > 0 || numberOverlays > 0 || ..."
					  
			// Draw the image overlay if they exist.
		
		if (numberImageOverlays > 0)
			DrawImageOverlays (imageViewCPtr,
										  imageWindowCPtr->GetWindowInfoHandle (),
										  NULL,
										  NULL,
										  &destinationRect,
										  &sourceRect, // sourceRect changed inside
										  windowCode);         
		  
				// originally in LImageCanvas.cpp OnPaint
			wxBitmap my_image (imageViewCPtr->m_ScaledBitmap);   
			wxSize bitmapSize = my_image.GetSize ();
			wxMemoryDC displaydc;
			displaydc.SelectObject (imageViewCPtr->m_ScaledBitmap);         
			  
		  
		if (my_image.Ok ())
			{
			double m_zoom = imageViewCPtr->m_Scale;   
			int destinationRectWidth =  destinationRect.right - destinationRect.left;
			int destinationRectHeight = destinationRect.bottom - destinationRect.top;
			//pDC->Clear ();
			//pDC->DrawBitmap (my_image,
			//							0,
			//							1 + ((imageViewCPtr->GetTitleHeight () - 1) / m_zoom),
			//							false);//true
			
			pDC->Blit (destinationRect.left, // DestX
							destinationRect.top, // DestY
							destinationRectWidth, // nDestWidth
							destinationRectHeight, // nDestHeight   
							&displaydc,  
							destinationRect.left, // SrcX
							destinationRect.top, // SrcY
							wxCOPY, // 
							false, // useMask
							wxDefaultCoord, // 
							wxDefaultCoord); // 
			
			}	// end "if (my_image.Ok ())"
		
		displaydc.SelectObject (wxNullBitmap);
		  
			// Draw the vector overlays if they exist.
		
		if (drawVectorOverlaysFlag && numberOverlays > 0)
			DrawArcViewShapes (imageViewCPtr,
											imageWindowCPtr->GetWindowInfoHandle (),
											&windowRect,
											windowCode,
											NULL);
			
				// If this window is the project window, outline the fields if requested.
		if (projectWindowFlag)
			OutlineFieldsControl (gProjectInfoPtr->statsWindowMode,
											  imageViewCPtr,
											  imageWindowCPtr->GetWindowInfoHandle (),
											  windowCode);
		
		if (numberImageOverlays > 0 ||
					numberOverlays > 0 ||
							projectWindowFlag && windowCode >= 2)
			{
			displaySpecsPtr = (DisplaySpecsPtr) GetHandlePointer (displaySpecsHandle);

			displaySpecsPtr->origin[kVertical] = vOrigin;
			displaySpecsPtr->origin[kHorizontal] = hOrigin;

			}	// end "if (numberImageOverlays > 0 || numberOverlays > 0 || ... "
		
				// Draw the selection area, if it exists for this window.			
      /*		
		if (copyType != kClipboardCopy)
			OutlineSelectionArea (theWindow);
		*/
				// Handle title line for side-by-side images.							

      if (titleHeight > 0)
			{
         if (copyType == kClipboardCopy || copyType == kPrinterCopy)
				{
            destinationRect.top = 0;
				destinationRect.left = 0;
				destinationRect.bottom =
							(SInt16)(titleHeight * imageViewCPtr->m_printerTextScaling + .5);

            if (copyType == kClipboardCopy)
					ClipRect (&destinationRect);

            }	// end "if (copyType == kClipboardCopy || copyType == kPrinterCopy)" 

         else // copyType != kClipboardCopy && copyType != kPrinterCopy)
				{
            destinationRect = windowRect;
				destinationRect.top = 0;
				destinationRect.bottom = titleHeight;
				
				ClipRect (&destinationRect);

				destinationRect.top += scrollOffset.y;
				destinationRect.bottom += scrollOffset.y;

				}	// end "else copyType != kClipboardCopy && copyType != kPrinterCopy" 
			
			pDC->SetBrush (wxBrush (*wxWHITE, wxSOLID));

         Handle windowInfoHandle = GetWindowInfoHandle (imageViewCPtr);
			DrawSideBySideTitles (windowInfoHandle,
										  imageViewCPtr,
										  &destinationRect,
										  windowCode);

			ClipRect (&windowRect);
         
			}	// end "if (titleHeight > 0)" 
			 
		gCDCPointer->DestroyClippingRegion ();
		
		}	// end "if (imageViewCPtr->CheckIfOffscreenImageExists ())"

}	// end "CopyOffScreenImage"



//------------------------------------------------------------------------------------
//								 Copyright (1988-2017)
//								(c) Purdue Research Foundation
//									All rights reserved.
//
//	Function name:		void CreateClassNameComboBoxList
//
//	Software purpose:	This routine creates a class name list in the input combo
//							box list.
//
//	Parameters in:				
//
//	Parameters out:				
//
//	Value Returned:	None
// 
// Called By:			
//
//	Coded By:			Larry L. Biehl			Date: 02/10/1996
//	Revised By:			Larry L. Biehl			Date: 03/29/2017		

void CreateClassNameComboBoxList (
				wxComboBox*							comboBoxCPtr)

{
	SignedByte							handleStatus;
	UInt16								classIndex,
											classStorage;
	

			// Clear method in wxControlwithitems
	
	comboBoxCPtr->Clear ();

			// Add current list of classes to the combo box.

   if (gProjectInfoPtr->numberStatisticsClasses < kMaxNumberStatClasses - 1)
		comboBoxCPtr->Append (wxT("New"));

			// Make certain that the pointer to class names structure is good.

	HPClassNamesPtr classNamesPtr = (HPClassNamesPtr)GetHandleStatusAndPointer (
											gProjectInfoPtr->classNamesHandle, &handleStatus);

	for (classIndex=0;
				classIndex<gProjectInfoPtr->numberStatisticsClasses;
					classIndex++)
		{
				// Get the class storage number.

		classStorage = gProjectInfoPtr->storageClass[classIndex];
		wxString newstr ((char*) & classNamesPtr[classStorage].name[1], wxConvUTF8);
		comboBoxCPtr->Append (newstr);

		}	// end "for (classIndex=0; classIndex<..."

}	// end "CreateClassNameComboBoxList"



void GetApplicationStartupPath (
				char*									startupPathPtr,
				SInt32*								sizeOfPathPtr)

{
	char*									executableFilePathPtr;

   SInt32								i,
											length,
											stringStartIndex;


	*sizeOfPathPtr = 0;

	//wxStandardPaths std;
	wxString exepath = (wxStandardPaths::Get ()).GetExecutablePath ();
	strncpy (executableFilePathPtr,
				(const char*)exepath.mb_str (wxConvUTF8),
				exepath.Len ());
	//executableFilePathPtr = (char*)exepath.mb_str (wxConvUTF8);

	if (executableFilePathPtr != NULL) 
		{
				// Remove the '"' from the beginning and end of the string.

      stringStartIndex = 0;
      if (executableFilePathPtr[0] == '"')
			stringStartIndex = 1;

		strcpy (startupPathPtr, &executableFilePathPtr[stringStartIndex]);

				// Now remove the executable name. Go backward from
				// end of string to find the first '\'.

		length = (SInt32)strlen (startupPathPtr);
		for (i = length; i >= 0; i--) 
			{
			if (startupPathPtr[i] == '\\')
				break;

			}	// end "for (i=length; i>=0; i--)"

      startupPathPtr[i + 1] = 0;
		*sizeOfPathPtr = i + 1;
		/*
				// Now change the '\' characters to '/'. This is done
				// for gdal routines. This approach will need to be changed
				// if other libraries need it differently.
				// Determined that this is not needed.

		for (i=0; i<*sizeOfPathPtr; i++)
			{
			if (startupPathPtr[i] == '\\')
			startupPathPtr[i] = '/';

			}	// end "for (i=0; i<*sizeOfPathPtr; i++)"
		*/
		}	// end "if (executableFilePathPtr != NULL)"

}	// end "GetApplicationStartupPath" 



SInt16 GetComboListSelection (
				wxWindow*							cWindowPtr,
				UInt16								dialogItemNumber,
				SInt32								selectedListItemData)
{
	wxComboBox*							comboBoxPtr;
	wxWindow*							wptr;

	wxChoice*							choiceptr;
	UInt32								index,
											numberComboItems;

	SInt16								comboListSelection;


	comboListSelection = -1;

	//comboBoxPtr = (wxComboBox*) cWindowPtr->FindWindow (dialogItemNumber);
	wptr = cWindowPtr->FindWindow (dialogItemNumber);
           
   if (wxDynamicCast (wptr, wxComboBox) != NULL)
		{
				//if it's wxComboBox
		
      comboBoxPtr = (wxComboBox*)wptr;
      if (comboBoxPtr != NULL)
			{
         numberComboItems = (UInt32) comboBoxPtr->GetCount ();
         
         for (index=0; index<numberComboItems; index++)
				{
            if (comboBoxPtr->GetClientData (index))
					{
               SInt64 windowIndex64 =
											(SInt64)((int*)comboBoxPtr->GetClientData (index));
               SInt32 windowIndex = (SInt32)windowIndex64;
               if (windowIndex == selectedListItemData)
						comboListSelection = (SInt16) index;
					
               }	// end "if (comboBoxPtr->GetClientData (index))"
				
				}	// end "for (index=0; index<numberComboItems; index++)"
         
			}	// end "if (comboBoxPtr != NULL)"
		
		}	// end "if (wxDynamicCast (wptr, wxComboBox) != NULL)"
	
	else if (wxDynamicCast (wptr, wxChoice) != NULL)
		{
				// If it's wxChoice
		
      choiceptr = (wxChoice*)wptr;
      if (choiceptr != NULL)
			{
         numberComboItems = (UInt32) choiceptr->GetCount ();
         for (index = 0; index < numberComboItems; index++)
				{
            if (choiceptr->GetClientData (index))
					{
               SInt64 windowIndex64= (SInt64)((int*)choiceptr->GetClientData (index));
               SInt32 windowIndex = (SInt32) windowIndex64;   
               if (windowIndex == selectedListItemData)
						comboListSelection = (SInt16) index;

               }	// end "if (choiceptr->GetClientData (index))"
				
				}	// end "for (index=0; index<numberComboItems; index++)"

			}	// end "if (comboBoxPtr != NULL)"
		
		}	// end "else if (wxDynamicCast (wptr, wxChoice) != NULL)"
   
   return (comboListSelection);

}	// end "GetComboListSelection"



void GetMenuItemText (
				MenuHandle							menuHandle,
				SInt16								menuItem,
				UCharPtr								stringPtr)

{
   //SInt16 stringLength = GetMenuString (menuItem, stringPtr, 255, MF_BYPOSITION);

}	// end "GetMenuItemText" 



//------------------------------------------------------------------------------------
//								 Copyright (1988-2017)
//								(c) Purdue Research Foundation
//									All rights reserved.
//
//	Function name:		void GetScreenRect
//
//	Software purpose:	The purpose of this routine is to obtain the rectangle that
//							describes the size of the screen with the mouse cursor.
//
//	Parameters in:					
//
//	Parameters out:				
//
//	Value Returned:
//
// Called By:																
//										
//	Global Data:
//
//	Coded By:			Larry L. Biehl			Date: 06/29/1995
//	Revised By:			Larry L. Biehl			Date: 06/29/1995	
// TODO: For Linux

void GetScreenRect (
				Rect*									screenRectPtr)

{
	
}	// end "GetScreenRect"



//------------------------------------------------------------------------------------
//								 Copyright (1988-2017)
//								(c) Purdue Research Foundation
//									All rights reserved.
//
//	Function name:		void GetScrollOffset
//
//	Software purpose:	The purpose of this routine is to obtain the scroll offset
//							for the window represented by the input window information
//							handle.
//
//	Parameters in:					
//
//	Parameters out:				
//
//	Value Returned:
//
// Called By:																
//										
//	Global Data:
//
//	Coded By:			Larry L. Biehl			Date: 05/23/1996
//	Revised By:			Larry L. Biehl			Date: 07/21/1998	
// TODO: For Linux

void GetScrollOffset (
				Handle								windowInfoHandle,
				LongPoint*							longPointPtr)

{
	WindowInfoPtr						windowInfoPtr;
	
	
	longPointPtr->v = 0;
	longPointPtr->h = 0;

	windowInfoPtr = (WindowInfoPtr)GetHandlePointer (windowInfoHandle);
	if (windowInfoPtr != NULL)
		{
		if (windowInfoPtr->cImageWindowPtr != NULL)
			{
			CMImageView* imageViewCPtr =
											windowInfoPtr->cImageWindowPtr->mImageViewCPtr;
			if (imageViewCPtr != NULL)
				{
				wxPoint scrollOffset = imageViewCPtr->m_Canvas->GetScrollPosition ();
				longPointPtr->v = scrollOffset.y;
				longPointPtr->h = scrollOffset.x;
				
				}	// end "if (imageViewCPtr != NULL)"
			
			}	// end "if (windowInfoPtr->cImageWindowPtr != NULL)"
		
		}	// end "if (windowInfoPtr != NULL)"
	
}	// end "GetScrollOffset"



//-----------------------------------------------------------------------------
//								 Copyright (1988-2017)
//								(c) Purdue Research Foundation
//									All rights reserved.
//
//	Function name:		void NumToString
//
//	Software purpose:	This routine is a Windows substitute for the
//							function of the same name in the Mac Toolbox.
//
//	Parameters in:				
//
//	Parameters out:				
//
//	Value Returned:	None
// 
// Called By:
//
//	Coded By:			Larry L. Biehl			Date: 06/27/1995
//	Revised By:			Larry L. Biehl			Date: 06/27/1995	

void NumToString (
				SInt32								numberValue,
				UCharPtr								stringPtr)

{
   sprintf ((CharPtr)&stringPtr[1], "%ld", numberValue);

	stringPtr[0] = strlen ((CharPtr)&stringPtr[1]);

}	// end "NumToString"   


/*
//------------------------------------------------------------------------------------
//								 Copyright (1988-2017)
//								(c) Purdue Research Foundation
//									All rights reserved.
//
//	Function name:		void NumToString
//
//	Software purpose:	This routine is a Windows substitute for the
//							function of the same name in the Mac Toolbox.
//
//	Parameters in:				
//
//	Parameters out:				
//
//	Value Returned:	None
// 
// Called By:
//
//	Coded By:			Larry L. Biehl			Date: 06/27/1995
//	Revised By:			Larry L. Biehl			Date: 06/27/1995	
							               	                
void	NumToString (
				UInt32								numberValue,
				UCharPtr								stringPtr)
 
{
	sprintf ((CharPtr)&stringPtr[1], "%lu", numberValue);
 
	stringPtr[0] = strlen ((CharPtr)&stringPtr[1]);

}	// end "NumToString"
*/



//------------------------------------------------------------------------------------
//								 Copyright (1988-2017)
//								(c) Purdue Research Foundation
//									All rights reserved.
//
//	Function name:		CharPtr PtoCstr
//
//	Software purpose:	This routine is a Windows substitute for the
//							function of the same name in the Mac Toolbox.
//
//	Parameters in:				
//
//	Parameters out:				
//
//	Value Returned:	None
// 
// Called By:
//
//	Coded By:			Larry L. Biehl			Date: 04/19/1995
//	Revised By:			Larry L. Biehl			Date: 04/19/1995			

CharPtr PtoCstr (
				UCharPtr								textStringPtr)

{
   if (textStringPtr != NULL)
		{
      size_t bytesToMove = *textStringPtr;

      if (bytesToMove > 0)
			{
         memmove (textStringPtr, &textStringPtr[1], bytesToMove);
			textStringPtr[bytesToMove] = '\0';

			}	// end "if (bytesToMove > 0)"

		}	// end "if (textStringPtr != NULL)"

   return (CharPtr) textStringPtr;

}	// end "PtoCstr"



//------------------------------------------------------------------------------------
//								 Copyright (1988-2017)
//								(c) Purdue Research Foundation
//									All rights reserved.
//
//	Function name:		CharPtr StringToNum
//
//	Software purpose:	This routine is a Windows substitute for the
//							function of the same name in the Mac Toolbox.
//
//	Parameters in:				
//
//	Parameters out:				
//
//	Value Returned:	None
// 
// Called By:	
//
//	Coded By:			Larry L. Biehl			Date: 04/19/1995
//	Revised By:			Larry L. Biehl			Date: 12/21/2016			

void StringToNum (
				ConstStr255Param					theString,
				SInt32*								theNumPtr)

{
	CharPtr stringPtr = (CharPtr) theString;

			// Force a c string terminator.

	stringPtr[stringPtr[0]+1] = '\0';
	*theNumPtr = atol (&stringPtr[1]);

}	// end "StringToNum"



SInt16 TextWidth (
				UCharPtr								stringPtr,
				SInt16								firstByte,
				SInt16								byteCount)

{
	wxSize								size;

	if (gCDCPointer != NULL)
		{
		wxString string = wxString::FromAscii (&stringPtr[firstByte]);
		size = gCDCPointer->GetTextExtent (string);
		
		}	// end "if (gCDCPointer != NULL)"

	else	// gCDCPointer == NULL
		size.x = byteCount * 6;

	return ((SInt16) size.x);
	
}	// end "TextWidth"


void SetUpToolTip (
				wxControl*							control,
				SInt16								stringnumber)

{
   GetSpecifiedStringNumber (kFileIOStrID, stringnumber, (UCharPtr)gTextString, TRUE);
	wxString wdcard ((char*)&gTextString[1], wxConvUTF8);
   control->SetToolTip (wdcard);
}
