//
//		Matlab Header Create
//
//		Writes necessary data to "header" of Matlab file.  Create structures
//		in .mat file for number_of_lines, number_of_columns, number_of_bands,
//		and initial information for the actual pixels.  Support BIS only.
//
//		Michael T. Gansler
//
//-----------------------------------------------------------------------------

#include "SMulSpec.h"
	
#	if defined multispec_mac
	
#	endif	// defined multispec_mac   
                             
#	if defined multispec_win
//#		include "CDatFile.h"	 
#	endif	// defined multispec_win 
  
#include	"SExtGlob.h"



			// Prototype descriptions for routines in this file that are only		
			// called by routines in this file.
										
void 								savemat(
										CMFileStream*		outFileStreamPtr, 
										SInt32				type, 
										char					*pname, 
										SInt32				mrows, 
										SInt32				ncols, 
										SDouble				preal); 	// short double
										
 void 							savematHeaderOnly(
										CMFileStream*		outFileStreamPtr, 
										SInt32				type, 
										char					*pname, 
										SInt32				mrows, 
										SInt32				ncols);


//
// savemat - C language routine to save a matrix in a MAT-file.
//
// Here is an example that uses 'savemat' to save two matrices to disk,
// the second of which is complex:
//
//	FILE *fp;
//	double xyz[1000], ar[1000], ai[1000];
//	fp = fopen("foo.mat","wb");
//	savemat(fp, 2000, "xyz", 2, 3, 0, xyz, (double *)0);
//	savemat(fp, 2000, "a", 5, 5, 1, ar, ai);
//	fclose(fp);
//
// Author J.N. Little 11-3-86
//
// [14-Feb-91] jmb -- Added support for MPW C 3.x and THINK C.
//
//	5/7/93 Revised : Michael Gansler
//					 Remove imaginary portions for this application
//

typedef struct {
     long type;	// type
     long mrows;	// row dimension
     long ncols;	// column dimension
     long imagf;	// flag indicating imag part
     long namlen;	// name length (including NULL)
} Fmatrix;


// FileInfoPtr		outFileInfoPtr;		 File pointer
// SInt32			type;				 Type flag: Normally 0 for PC, 1000 for Sun, Mac, and
//											 Apollo, 2000 for VAX D-float, 3000 for VAX G-float
//										 	Add 1 for text variables.
//										 	See LOAD in reference section of guide for more info.
// SInt32			mrows;			 row dimension
// SInt32			ncols;			 column dimension
// char				*pname;			 pointer to matrix name
// SDouble			preal;			 actual real data : 1 SCALAR VALUE

void savemat(
				CMFileStream*						outFileStreamPtr, 
				SInt32								type, 
				char									*pname, 
				SInt32								mrows, 
				SInt32								ncols, 
				SDouble								preal)


{
	Fmatrix								x;
	SInt32								mn;
	UInt32								temp;
	SInt16								errCode;
	
	
	x.type = type;
	x.mrows = mrows;
	x.ncols = ncols;
	x.imagf = (SInt32)0;
	x.namlen = strlen(pname) + 1;
	mn = x.mrows * x.ncols;
	
	temp = (SInt32)sizeof(Fmatrix);
	
			// write header structure
	
	errCode = MWriteData(outFileStreamPtr, &temp, &x, kNoErrorMessages);
	
			// write name of array
				
	temp = (SInt32)(sizeof(char)*x.namlen);
	if (errCode == noErr)
		errCode = MWriteData(outFileStreamPtr, &temp, pname, kNoErrorMessages);
	
			// write array values
		
	temp = (SInt32)(sizeof(SDouble)*mn);
	if (errCode == noErr)
		errCode = MWriteData(outFileStreamPtr, &temp, &preal, kNoErrorMessages);
	
}


//
// savematHeaderOnly - 	C language routine to save a header information
//						for a Matlab matrix.
//
// FileInfoPtr		outFileInfoPtr;		 File pointer 
// SInt32		type;		 Type flag: Normally 0 for PC, 1000 for Sun, Mac, and	
// 							 Apollo, 2000 for VAX D-float, 3000 for VAX G-float   
// 							 Add 1 for text variables.
//							 See LOAD in reference section of guide for more info.
// SInt32		mrows;		 row dimension
// SInt32		ncols;		 column dimension
// char			*pname;		 pointer to matrix name
//
//	5/7/93 Revised : Michael Gansler

void savematHeaderOnly(
				CMFileStream*						outFileStreamPtr, 
				SInt32								type, 
				char									*pname, 
				SInt32								mrows, 
				SInt32								ncols) 
				
{
	Fmatrix								x;
	SInt32								mn;
	UInt32								temp;
	SInt16								errCode;
						
	
	x.type = type;
	x.mrows = mrows;
	x.ncols = ncols;
	x.imagf = (SInt32)0;
	x.namlen = strlen(pname) + 1;
	mn = x.mrows * x.ncols;
	
			// write header structure
				
	temp = (SInt32)sizeof(Fmatrix);
	errCode = MWriteData(outFileStreamPtr, &temp, &x, kNoErrorMessages);
	
			// write name of array
						
	temp = (SInt32)(sizeof(char)*x.namlen);
	if (errCode == noErr)
		errCode = MWriteData(outFileStreamPtr, &temp, pname, kNoErrorMessages);
		
}		// end "savematHeaderOnly"



//	Revised By:			Larry L. Biehl			Date: 12/22/2003

void WriteMatlabHeader(
				FileInfoPtr 						outFileInfoPtr)

{
	SInt64								numberHeaderBytes;
	CMFileStream*						fileStreamPtr;
	SInt32								type;
	SInt16								errCode;
	double								rows, 
											columns;
	
	
	if (gBigEndianFlag)	
		type = 1000;
	else		// !gBigEndianFlag	
		type = 0;
							
	fileStreamPtr = GetFileStreamPointer (outFileInfoPtr);

	savemat(fileStreamPtr, type, (char*)"number_of_bands", 
									1, 1,(SDouble)outFileInfoPtr->numberChannels);
	savemat(fileStreamPtr, type, (char*)"number_of_rows", 
									1, 1,(SDouble)outFileInfoPtr->numberLines);
	savemat(fileStreamPtr, type, (char*)"number_of_columns", 
									1, 1,(SDouble)outFileInfoPtr->numberColumns);
	
	columns = (outFileInfoPtr->numberLines) * (outFileInfoPtr->numberColumns);
	rows = outFileInfoPtr->numberChannels;
	savematHeaderOnly (fileStreamPtr, type, 
									(char*)"pixels", (SInt32)rows, (SInt32)columns);
									  
	errCode = MGetMarker (fileStreamPtr, 
									&numberHeaderBytes, 
									kNoErrorMessages);
									
	if (errCode == noErr)
		outFileInfoPtr->numberHeaderBytes = (UInt32)numberHeaderBytes;

}		// end "WriteMatlabHeader"

/*
//
//		ConvertToMatlabFormat : called by reformat.c to reformat output buffer
//		to Matlab data size (SDouble)
//
//		Michael Gansler

void ConvertToMatlabFormat(
				HUCharPtr							savedOutBufferPtr, 
				SInt16								numberBytes, 
				Boolean								signedDataFlag, 
				SInt32								numberSamples)
				

{
	SDouble*								matlabBufferPtr;
	
	SInt32								i;


	matlabBufferPtr = (SDouble*)savedOutBufferPtr;
	matlabBufferPtr = &matlabBufferPtr [numberSamples-1]; 

	if (numberBytes == 1)
		{
		HUCharPtr				ioOut1BufferPtr;
	
		ioOut1BufferPtr = (HUCharPtr)savedOutBufferPtr;
		ioOut1BufferPtr = &ioOut1BufferPtr [numberSamples-1];
		
		for (i=numberSamples; i>0; i--)
			{
			*matlabBufferPtr = *ioOut1BufferPtr;
			ioOut1BufferPtr--;
			matlabBufferPtr--;
			
			}		// end "for (i=numberSamples; i>0; i--)"
	
		}		// end "if (numberBytes == 1)"
	
	if (numberBytes == 2)
		{
		if (signedDataFlag)
			{
			HSInt16Ptr				ioOut2BufferPtr;
		
			ioOut2BufferPtr = (HSInt16Ptr)savedOutBufferPtr;
			ioOut2BufferPtr = &ioOut2BufferPtr [numberSamples-1];
			
			for (i=numberSamples; i>0; i--)
				{
				*matlabBufferPtr = *ioOut2BufferPtr;
				ioOut2BufferPtr--;
				matlabBufferPtr--;
									
				}		// end "for (i=numberSamples; i>0; i--)"
				
			}		// end "if (signedDataFlag)"
			
		else		// !signedDataFlag
			{
			HUInt16Ptr		ioOut2BufferPtr;
		
			ioOut2BufferPtr = (HUInt16Ptr)savedOutBufferPtr;
			ioOut2BufferPtr = &ioOut2BufferPtr [numberSamples-1];
			
			for (i=numberSamples; i>0; i--)
				{
				*matlabBufferPtr = *ioOut2BufferPtr;
				ioOut2BufferPtr--;
				matlabBufferPtr--;
									
				}		// end "for (i=numberSamples; i>0; i--)"
			
			}		// end "else !signedDataFlag"
	
		}		// end "if (numberBytes == 2)"

}		// end "ConvertToMatlabFormat"
*/
	