/*************************************************************************
* $Id: output.c,v 1.8 2006/12/18 15:10:38 yougz Exp $
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
  
  Version 2.0  20/12/2006
*************************************************************************/

#include "output.h"

/*!
*********************************************************
* \ingroup     Output
* \brief       Saving a complex volume (like the RF one) into 2 raw files (Real and imaginary part)
*  The default file names are rf_re.vol and rf_im.vol
* \param volrf Pointer on the RF volume 
***************************************************************/
void WriteVolRF(PPPVOLUME_COMPLEX_DOUBLE volrf)
{
 PPPVOLUME_DOUBLE re,im;
 int i,j,k;
 int X,Y,Z;

 X=IdVolDimX(volrf);
 Y=IdVolDimY(volrf);
 Z=IdVolDimZ(volrf);

 re = (PPPVOLUME_DOUBLE) IdVolAlloc(X,Y,Z,VOL_DOUBLE);
 im = (PPPVOLUME_DOUBLE) IdVolAlloc(X,Y,Z,VOL_DOUBLE);
 for (k=0;k<Z;k++) for (i=0;i<Y;i++) for (j=0;j<X;j++)
 {
  re[k][i][j] = volrf[k][i][j].re;
  im[k][i][j] = volrf[k][i][j].im;
 }
 IdVolWriteRawFile("rf_re.vol",(PPPVOLUME)re);
 IdVolWriteRawFile("rf_im.vol",(PPPVOLUME)im);

 IdVolFree(re);
 IdVolFree(im);

}

/*!
*********************************************************
* \ingroup     Output
* \brief       Saving a volume of float (like the reconstructed image volume)
* into a raw float volume (recfloat.raw) and in a raw uchar volume after a gray level scaling
* \param volrec Pointer on the volume to save
* \param volname File name of the uchar saved volume
***************************************************************/
void WriteVolRecUchar(PPPVOLUME_FLOAT volrec, char * volname)
{
 int X,Y,Z;
 PPPVOLUME_UCHAR voluchar;
 PPIMAGE_UCHAR imauchar;
 double min=0.0;
 double max=0.0;
 double dyn;
 int i,j,k;

 IdVolWriteRawFile("recfloat.raw",(PPPVOLUME)volrec);
 X=IdVolDimX(volrec);
 Y=IdVolDimY(volrec);
 Z=IdVolDimZ(volrec);

 IdVolMinMax((PPPVOLUME)volrec,&min,&max);
 dyn=(max-min);
 if (Z==1) /* Writing a 2D image */
	{
	 imauchar = (PPIMAGE_UCHAR) IdImaAlloc(X,Y,IMA_UCHAR);
	 for (i=0;i<Y;i++) for (j=0;j<X;j++)
		imauchar[i][j]=(unsigned char)(((volrec[0][i][j]-min)/dyn)*255.0+0.5);
	 IdImaWriteRawFile(volname,(PPIMAGE)imauchar);
	 IdImaFree(imauchar);
	}
 else
	{
	 voluchar = (PPPVOLUME_UCHAR) IdVolAlloc(X,Y,Z,VOL_UCHAR);
	 for (k=0;k<Z;k++) for (i=0;i<Y;i++) for (j=0;j<X;j++)
		voluchar[k][i][j]=(unsigned char)(((volrec[k][i][j]-min)/dyn)*255.0+0.5);
	 IdVolWriteRawFile(volname,(PPPVOLUME)voluchar);
	 IdVolFree(voluchar);
	}
}

/*!
*********************************************************
* \ingroup     Output
* \brief       Saving the module of the k space into a float raw data file 
* \param expr Pointer on the experience strcuture which contains the RF data
* \param volname File name
***************************************************************/
void WriteSignalRFModule(EXPERIENCE3D * expr, char * volname)
{
 	PPPVOLUME_FLOAT rfmod;

	rfmod = (PPPVOLUME_FLOAT) GetSignalRFModuleFromExperience(expr);
	IdVolWriteRawFile(volname,(PPPVOLUME_FLOAT)rfmod);
	IdVolFree(rfmod);
}

/*!
*********************************************************
* \ingroup     Output
* \brief       Saving the module of the reconstructed volume using IFFT
* \param expr Pointer on the experience strcuture which contains the RF data
* \param volname File name
***************************************************************/
void WriteRecIFFTVolumeModule(EXPERIENCE3D * expr, char * volname)
{
 	PPPVOLUME_FLOAT module;

	module= (PPPVOLUME_FLOAT) RecVolIFFTModule(expr);
	IdVolWriteRawFile(volname,module);
	
	IdVolFree(module);
}

/*!
*********************************************************
* \ingroup     Output
* \brief       Saving the phase of the reconstructed volume using IFFT
* \param expr Pointer on the experience strcuture which contains the RF data
* \param volname File name
***************************************************************/
void WriteRecIFFTVolumePhase(EXPERIENCE3D * expr, char * volname)
{
 	PPPVOLUME_FLOAT phase;

	phase= (PPPVOLUME_FLOAT) RecVolIFFTPhase(expr);
	IdVolWriteRawFile(volname,phase);
	
	IdVolFree(phase);
}

/*!
*********************************************************
* \ingroup     Output
* \brief       Save the magnetisation state of an object within a file
* \warning Designed to observe the magnetisation state during slice selection
* \param filename File name to save the values using fprintf 
* \param expr Pointer on the experience strcuture which contains magnetisation sate of the object
***************************************************************/
void MagstateWatch(EXPERIENCE3D* expr,char * filename)
{
int g,ii;
FILE *fic;
int x,y;

x=expr->nfovx/2;
y=expr->nfovy/2;
fic=fopen(filename,"w");
for(g=0;g<expr->nfovz;g++)
	{
	for(ii=0;ii<=2;ii++) 
		{
		 fprintf(fic,"%lf ",expr->magstate[ii][x][y][g]);
		}
	fprintf(fic,"%lf ",sqrt(expr->magstate[0][x][y][g]*expr->magstate[0][x][y][g]+expr->magstate[1][x][y][g]*expr->magstate[1][x][y][g]));
	fprintf(fic,"%lf ",atan2(expr->magstate[1][x][y][g],expr->magstate[0][x][y][g])*180/M_PI);
	fprintf(fic,"\n");
	}
fclose(fic);
}
