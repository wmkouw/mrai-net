/*************************************************************************
* $Id: dcmjpeg.c,v 1.1 2005/09/09 08:22:23 bellet Exp $
**************************************************************************
 This software is governed by the CeCILL  license under French law and
  abiding by the rules of distribution of free software.  You can  use, 
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info". 
  
  As a counterpart to the access to the source code and  rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty  and the software's author,  the holder of the
  economic rights,  and the successive licensors  have only  limited
  liability. 
  
  In this respect, the user's attention is drawn to the risks associated
  with loading,  using,  modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean  that it is complicated to manipulate,  and  that  also
  therefore means  that it is reserved for developers  and  experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or 
  data to be ensured and,  more generally, to use and operate it in the 
  same conditions as regards security. 
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
  
  Copyright (c) CREATIS (Centre de Recherche et d'Applications en Traitement de
  l'Image). All rights reserved. See License.txt for details.
  
  Version 1.0  05/09/2005
*************************************************************************/

#include <stdlib.h>
#include "iddcmjpeg.h"
#include "idcommon.h"
	
	
static ClbJpeg* ClbJpegAlloc(void);
static void 	ClbJpegInit (ClbJpeg *); 
static int 	ClbJpegDecodeDiff(ClbJpeg *);
static BOOL 	ClbJpegDecodeData(ClbJpeg *);
static int 	ClbJpegReadBit(ClbJpeg *);
static BOOL 	ClbJpegReadHeader(ClbJpeg *);
static BOOL 	ClbJpegStart(ClbJpeg *,FILE *);	
static BOOL 	ClbJpegFillHuffTable(ClbJpeg *);




void _IdDcmJpegFree(ClbJpeg *jpg)
{
	 free(jpg->DataImg);
	 free(jpg);
}


ClbJpeg * _IdDcmJpegRead (FILE * fp){
ClbJpeg * jpg=NULL;
	jpg=ClbJpegAlloc();
	if(!jpg) {
	   printf("Fail to ClbJpegAlloc \n");
	   return(NULL);
	}
	ClbJpegInit (jpg); 
	if(!ClbJpegStart(jpg, fp)) {
	   printf("Fail to ClbJpegStart \n");
	   return (NULL);
	}
	return (jpg);
}


	
static void ClbJpegInit (ClbJpeg *jpg) {
int n;
	for (n=0;n<256;n++)
	{
		jpg->lHuffTable[n].HufCode=0;
		jpg->lHuffTable[n].HufSz=0;
		jpg->lHuffTable[n].HufVal=0;
	}
	jpg->ValCurByte=0;
	jpg->PosCurBit=10;
	jpg->MarkerFound=0;
	jpg->RestartInterval=0;
}

static ClbJpeg *ClbJpegAlloc(void) {
ClbJpeg * jpg;
	jpg = (ClbJpeg *)malloc(sizeof(ClbJpeg));
	ClbJpegInit(jpg);	
return jpg;
}
 


static BOOL ClbJpegFillHuffTable(ClbJpeg *jpg)
{
	unsigned char c;
	//int testindex=0;
	int n=0;
	int NiDHT=0;
	int indexY=0;
	int k, Code, Si, i;

	for (c=0;c<255;c++)
		jpg->RawDHTstart[c]=0;

	c=fgetc(jpg->infp);


	jpg->MaxHuffSz=0;
	jpg->MaxHuffVal=0;

	for (n=1;n<17;n++)
	{
		jpg->RawDHT[n]=fgetc(jpg->infp);
		NiDHT+=jpg->RawDHT[n];
		if (jpg->RawDHT[n]!=0)
			jpg->MaxHuffSz=n;
	}

	for(n=1;n<16;n++)
	{
		if(jpg->RawDHT[n]>0)
		{
			jpg->RawDHTstart[n]=indexY+1;
			for (i=1;i<(jpg->RawDHT[n]+1);i++)
			{
				indexY+=1;
				c=fgetc(jpg->infp);
				jpg->lHuffTable[indexY].HufVal=c;
				jpg->MaxHuffVal=c;
				jpg->lHuffTable[indexY].HufSz=n;
			}
		}
	}
	k=1;
	Code=0;
	
	Si=jpg->lHuffTable[k].HufSz;

	while(1)
	{
		if (k>=NiDHT) break;
		while( Si==jpg->lHuffTable[k].HufSz)
		{
			jpg->lHuffTable[k].HufCode=Code;
			Code+=1;
			k+=1;
		}
		if (k<NiDHT)
		{
			while (jpg->lHuffTable[k].HufSz>Si)
			{
				Code=Code<<1;
				Si+=1;
			}
		}
	}

	return 1;
}

	

static BOOL ClbJpegStart(ClbJpeg *jpg, FILE *inputfp)
{
	jpg->infp=inputfp;
	if (!ClbJpegReadHeader(jpg)) {
	   printf("Fail to ClbJpegReadHeader\n");
	   return 0;
	}
	//printf("sortie ClbJpegReadHeader\n");
	if (!ClbJpegDecodeData(jpg)) {
	   printf("Fail to ClbJpegDecodeData\n");
	   return 0;
	}	
	//printf("sortie ClbJpegDecodeData\n");
	return 1;
}

static BOOL ClbJpegReadHeader(ClbJpeg *jpg)
{
	unsigned char gr;
	unsigned char el;
	unsigned char l2;
	unsigned char l1;
	unsigned int sztag;
	long ouca=0;
	BOOL HeaderEnd=0;
	BOOL isLossLess=0;
	
	int tp;

	gr=fgetc(jpg->infp);  //FF
	el=fgetc(jpg->infp);  //D8

	while(!HeaderEnd)
	{
		gr=fgetc(jpg->infp);  
		if(gr!=0xFF) {
		   printf("gr!=0xFF (=%02x)\n",gr);
		   return 0;
		}
		el=fgetc(jpg->infp);

		if ( (el==0xFF) || (el==0x01) || (el==0xD8) ||(el==0xD9) ||( (el>=0xD0) && (el<=0xD7) ))
			;
		else
		{
			l1=fgetc(jpg->infp);
			l2=fgetc(jpg->infp);
			sztag=(l1*256)+l2-2; //tag lengh
			ouca=ftell(jpg->infp);

			if (el==0xC3)
			{
				jpg->lSof.precision=fgetc(jpg->infp);

				l1=fgetc(jpg->infp);
				l2=fgetc(jpg->infp);
				jpg->lSof.Himg=(l1*256)+l2;

				l1=fgetc(jpg->infp);
				l2=fgetc(jpg->infp);
				jpg->lSof.Wimg=(l1*256)+l2;

				jpg->lSof.NbComponent=fgetc(jpg->infp);

				jpg->lSof.SofTabPos=ftell(jpg->infp);

				isLossLess=TRUE;
			}

			if (el==0xC4)
			{
				ClbJpegFillHuffTable(jpg);
			}
			if (el==0xDA)
			{
				jpg->lSos.CompCount=fgetc(jpg->infp);
				for (tp=0;tp<jpg->lSos.CompCount;tp++)
				{
					jpg->lSos.CompId=fgetc(jpg->infp);
					jpg->lSos.CompDc=fgetc(jpg->infp);
				}
				jpg->lSos.SpectralSelStart=fgetc(jpg->infp);
				jpg->lSos.SpectralSelEnd=fgetc(jpg->infp);
				jpg->lSos.SuccessiveAp=fgetc(jpg->infp);
				jpg->lSos.Sospttrans=(jpg->lSos.SuccessiveAp & 16);
				HeaderEnd=1;
			}
			if (el==0xDD)
			{
				l1=fgetc(jpg->infp);
				l2=fgetc(jpg->infp);
				jpg->RestartInterval=(l1*256)+l2;
			}

			fseek(jpg->infp,(ouca+sztag),0);
		}
	}

	if (!isLossLess) {
	   printf("NOT isLossLess\n");
	   return 0;
	}
	return 1;

}

static int ClbJpegReadBit(ClbJpeg *jpg)
{
	int r=0;
	unsigned char c;
	if(jpg->PosCurBit>8) // need lire octet suivant
	{
		jpg->ValCurByte=fgetc(jpg->infp);
		if (jpg->ValCurByte==0xFF)
		{
			c=fgetc(jpg->infp);// est 00 ou restart marker: a skiper
			if (c!=0)
			{
				jpg->ValCurByte=fgetc(jpg->infp);
				jpg->PosCurBit=1;
				jpg->MarkerFound=1;
				return 0; 
			}
		}
		jpg->PosCurBit=2;
		return (jpg->ValCurByte>>7);
	}
	else
	{
		r=(1&(jpg->ValCurByte>>(8-jpg->PosCurBit)));
		jpg->PosCurBit+=1;
		return r;
	}
}


static BOOL ClbJpegDecodeData(ClbJpeg *jpg)
{
	int iX,iY;
	int lbInc=0;
	unsigned int mask; 
	  
	int lPredicted=(1<<(jpg->lSof.precision-1-jpg->lSos.Sospttrans));

	jpg->ValCurByte=jpg->lSos.SuccessiveAp;
	jpg->PosCurBit=9;
	
	if (jpg->lSof.precision==8)
		mask=0xFF;
	if (jpg->lSof.precision==12)
		mask=0xFFF;
	if (jpg->lSof.precision==16)
		mask=0xFFFF;

	 jpg->DataImg=(int*)malloc(jpg->lSof.Himg*jpg->lSof.Wimg*sizeof(*jpg->DataImg));
	memset( jpg->DataImg,0,(jpg->lSof.Himg*jpg->lSof.Wimg*sizeof(*jpg->DataImg)));

	if (!jpg->RestartInterval)
	{
		for(iX=0;iX<jpg->lSof.Wimg;iX++) // lit première ligne
		{
			lbInc+=1;
			if (lbInc>1)
				lPredicted= jpg->DataImg[lbInc-1];
			 jpg->DataImg[lbInc]=lPredicted+ClbJpegDecodeDiff(jpg);

			if (  jpg->DataImg[lbInc] > ((1<<(jpg->lSof.precision))-1) )
				 jpg->DataImg[lbInc]= jpg->DataImg[lbInc]&mask;
			if ( jpg->DataImg[lbInc]<0) 
				 jpg->DataImg[lbInc]= jpg->DataImg[lbInc]&mask;
		}

		for (iY=1;iY<jpg->lSof.Himg;iY++) //lit la suite
		{
			lbInc+=1;
			if (lbInc>(jpg->lSof.Himg*jpg->lSof.Wimg-1)) break;
			lPredicted= jpg->DataImg[lbInc-jpg->lSof.Wimg];	// se base % premier é ligne d'avant
			 jpg->DataImg[lbInc]=lPredicted+ClbJpegDecodeDiff(jpg);

			if (  jpg->DataImg[lbInc] > ((1<<(jpg->lSof.precision))-1) ) 
				 jpg->DataImg[lbInc]= jpg->DataImg[lbInc]&mask;
			if ( jpg->DataImg[lbInc]<0) 
				 jpg->DataImg[lbInc]= jpg->DataImg[lbInc]&mask;

			for(iX=1;iX<jpg->lSof.Wimg;iX++)
			{
				lbInc+=1;
				if (lbInc>(jpg->lSof.Himg*jpg->lSof.Wimg-1)) break;
				if (jpg->lSos.SpectralSelStart==7) // si spectral
					lPredicted=( jpg->DataImg[lbInc-1]+ jpg->DataImg[lbInc-jpg->lSof.Wimg])>>1;
				else
					lPredicted= jpg->DataImg[lbInc-1];		// se base%pixel juste avant
				 jpg->DataImg[lbInc]=lPredicted+ClbJpegDecodeDiff(jpg);

				if (  jpg->DataImg[lbInc] > ((1<<(jpg->lSof.precision))-1) ) 
					 jpg->DataImg[lbInc]= jpg->DataImg[lbInc]&mask;
				if ( jpg->DataImg[lbInc]<0) 
					 jpg->DataImg[lbInc]= jpg->DataImg[lbInc]&mask;

			}
		}
	}
	else // il y a un define interval
	{
		while(1)
		{
			jpg->MarkerFound=0;
			lPredicted=(1<<(jpg->lSof.precision - 1 - jpg->lSos.Sospttrans));
			for (iY=0;iY<jpg->RestartInterval;iY++) 
			{
				 jpg->DataImg[lbInc]=lPredicted+ClbJpegDecodeDiff(jpg);

				if (  jpg->DataImg[lbInc] > ((1<<(jpg->lSof.precision))-1) ) 
					 jpg->DataImg[lbInc]= jpg->DataImg[lbInc]&mask;
				if ( jpg->DataImg[lbInc]<0) 
					 jpg->DataImg[lbInc]= jpg->DataImg[lbInc]&mask;

				lbInc+=1;
				if (lbInc>(jpg->lSof.Himg*jpg->lSof.Wimg-1)) return 1;

				if (jpg->lSos.SpectralSelStart==7) // si spectral
					lPredicted=( jpg->DataImg[lbInc-1]+ jpg->DataImg[lbInc-jpg->lSof.Wimg])>>1;
				else
					lPredicted= jpg->DataImg[lbInc-1];
			}
			while (!jpg->MarkerFound)
			{
				ClbJpegReadBit(jpg); // skip bits restant avant restart marker
			}
		}
	}
	return 1;
}

static int ClbJpegDecodeDiff(ClbJpeg *jpg)
{
	int lInput;
	int lInputBits;
	int lHufVal;
	int lDiff;
	int lI;
	int resultat;
	lHufVal = 666;
	lInput = 0;
	lInputBits = 0;
	
	
	while (1)
	{
		lInputBits+=1;
		lInput=(lInput<<1)+ClbJpegReadBit(jpg);
		if (jpg->RawDHT[lInputBits]!=0)
		{
			for(lI=jpg->RawDHTstart[lInputBits];lI<(jpg->RawDHTstart[lInputBits]+jpg->RawDHT[lInputBits]);lI++)
			{
				if (lInput==jpg->lHuffTable[lI].HufCode)
					lHufVal=jpg->lHuffTable[lI].HufVal;
			}
		}
		if (lInputBits>=jpg->MaxHuffSz)
			lHufVal=jpg->MaxHuffVal;
		if (lHufVal<255) break;
	}
	if (lHufVal==0) resultat= 0;

	if ( (lHufVal>0) && (lHufVal<16))
	{
		lDiff=0;
		if( ClbJpegReadBit(jpg)==1)
		{
			for (lI=1;lI<lHufVal;lI++)
			{
				lDiff=(lDiff<<1)+ClbJpegReadBit(jpg);
			}

			resultat= (lDiff+(1<<(lHufVal-1)));
		}
		else
		{
			for (lI=1;lI<lHufVal;lI++)
				lDiff=(lDiff<<1)+1-ClbJpegReadBit(jpg);
			resultat= -(lDiff+(1<<(lHufVal-1)));
		}
	}

	return resultat;
	
}
