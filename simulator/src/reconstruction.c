/*************************************************************************
* $Id: reconstruction.c,v 1.19 2006/12/18 15:10:38 yougz Exp $
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

#include "reconstruction.h"

#include <stdio.h>
#include <math.h>
#include <idsig.h>
#include <idima.h>
#include <idvol.h>
#include <iderr.h>
#include <idprint.h>
/*!
**********************************************************
* \ingroup     Reconstruction
* \brief       Filtering of the RF volume
* \warning     
* \param volrf Pointer on the Complex RF volume to filter
* \param type  Filter type : 0 (Hanning), others not implanted
***********************************************************/
void VolRFFiltering(PPPVOLUME_COMPLEX_DOUBLE volrf,int type)
{
	int i,j,k;
	double x,y,z;
	double w,d;

	x= (double) IdVolDimX(volrf);
	y= (double) IdVolDimY(volrf);
	z= (double) IdVolDimZ(volrf);

switch(type)
{
case 0: /* Hanning */
	{
     if ((x!=1)&&(y!=1)&&(z!=1)) /*3D*/
	 {
	  for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	  {
       d=sqrt((i-x/2.)*(i-x/2.)/x/x*4+(j-y/2.)*(j-y/2.)/y/y*4+(k-z/2)*(k-z/2.)/z/z*4);
	   if (d>1.0) d=1.0;
	   w=0.5 + 0.5*cos(M_PI*d);
	   volrf[k][j][i].re=volrf[k][j][i].re * w;
	   volrf[k][j][i].im=volrf[k][j][i].im * w;
	  }
	 }
     if ((x!=1)&&(y!=1)&&(z==1)) /*2D*/
	 {
	  for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	  {
       d=sqrt((i-x/2.)*(i-x/2.)/x/x*4+(j-y/2.)*(j-y/2.)/y/y*4);
	   if (d>1.0) d=1.0;
	   w=0.5 + 0.5*cos(M_PI*d);
	   volrf[k][j][i].re=volrf[k][j][i].re * w;
	   volrf[k][j][i].im=volrf[k][j][i].im * w;
	  }
	 }
     if ((x!=1)&&(y==1)&&(z==1)) /*1D*/
	 {
	  for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	  {
       d=sqrt((i-x/2.)*(i-x/2.)/x/x*4);
	   w=0.5 + 0.5*cos(M_PI*d);
	   volrf[k][j][i].re=volrf[k][j][i].re * w;
	   volrf[k][j][i].im=volrf[k][j][i].im * w;
	  }
	 }
	} /* End Hanning */
case 2:
	{
	}
case 3:
	{
	}
} /* End switch */
}

/*!
**********************************************************
* \ingroup     Reconstruction
* \brief       Volume reconstruction applying the IFFT on the RF volume
* \warning     The RF volume is desallocated because of the implementation of the IdVolIFFT function
* \param volrf Pointer on the complex RF volume
* \return      Pointer on the complex reconstructed volume
 ***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE RecVolIFFTComplexeFromVol (PPPVOLUME_COMPLEX_DOUBLE volrf)
{
	PPPVOLUME_COMPLEX_DOUBLE volrec;
	int i,j,k;
	int x,y,z;

	volrf= (PPPVOLUME_COMPLEX_DOUBLE) IdVolIFFT((PPPVOLUME_COMPLEX) volrf);
	/* Careful : volrf output of IFFT is different from the volrf input. Thus it is desalloacted in this function */
	
	/* x permutation of the IFFT  */
	x=IdVolDimX(volrf);
	y=IdVolDimY(volrf);
	z=IdVolDimZ(volrf);
	volrec = (PPPVOLUME_COMPLEX_DOUBLE) IdVolAlloc(x,y,z,VOL_COMPLEX_DOUBLE);
	if(!volrec)
	 {
	  printf("Impossible d'allouer un volume dans RecVolIFFTComplexe !\n");
	  exit(0);
     }
    for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	 {
	  volrec[k][j][i].re=volrf[k][j][x-1-i].re;
	  volrec[k][j][i].im=volrf[k][j][x-1-i].im;
	 }

	 IdVolFree(volrf);
    return(volrec);
}

/*!
**********************************************************
* \ingroup     Reconstruction
* \brief       Volume reconstruction of the module applying the IFFT on the RF volume
* \warning     The RF volume is desallocated because of the implementation of the IdVolIFFT function
* \param volrf Pointer on the complex RF volume
* \return      Pointer on the module of the reconstructed volume
 ***********************************************************/
PPPVOLUME_FLOAT RecVolIFFTModuleFromVol (PPPVOLUME_COMPLEX_DOUBLE volrf)
{
	PPPVOLUME_COMPLEX_DOUBLE volrec;
	PPPVOLUME_FLOAT module;
	int i,j,k;
	int it,jt,kt;
	int x,y,z;
	double a,b;

	volrec= (PPPVOLUME_COMPLEX_DOUBLE) RecVolIFFTComplexeFromVol(volrf);
	x = IdVolDimX(volrec); y=IdVolDimY(volrec); z=IdVolDimZ(volrec);

	module = (PPPVOLUME_FLOAT) IdVolAlloc(x,y,z,VOL_FLOAT);

	/* Module computation with dimx*dimy*dimz permutations */
	for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	{
		it = (i + (x / 2)) % x;
		jt = (j + (y / 2)) % y;
		kt = (k + (z / 2)) % z;
		a = volrec[k][j][i].re;
		b = volrec[k][j][i].im;
		module[kt][jt][it] = (float) sqrt((a*a) + (b*b));
	}
	
	IdVolFree(volrec);
	
	return(module);
}

/*!
**********************************************************
* \ingroup     Reconstruction
* \brief       Volume reconstruction of the phase applying the IFFT on the RF volume
* \warning     The RF volume is desallocated because of the implementation of the IdVolIFFT function
* \param volrf Pointer on the complex RF volume
* \return      Pointer on the phase of the reconstructed volume
 ***********************************************************/
PPPVOLUME_FLOAT RecVolIFFTPhaseFromVol (PPPVOLUME_COMPLEX_DOUBLE volrf)
{
	PPPVOLUME_COMPLEX_DOUBLE volrec;
	PPPVOLUME_FLOAT phase;
	int i,j,k;
	int it,jt,kt;
	int x,y,z;
	double a,b;

	volrec= (PPPVOLUME_COMPLEX_DOUBLE) RecVolIFFTComplexeFromVol(volrf);
	x = IdVolDimX(volrec); y=IdVolDimY(volrec); z=IdVolDimZ(volrec);

	phase = (PPPVOLUME_FLOAT) IdVolAlloc(x,y,z,VOL_FLOAT);

	/* Phase computation with dimx*dimy*dimz permutations */
	for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	{
		it = (i + (x / 2)) % x;
		jt = (j + (y / 2)) % y;
		kt = (k + (z / 2)) % z;
		a = volrec[k][j][i].re;
		b = volrec[k][j][i].im;
		phase[kt][jt][it] = (float) atan(b/a);
	}
	
	IdVolFree(volrec);
	
	return(phase);
}


/*!
**********************************************************
* \ingroup     Reconstruction
* \brief       Volume reconstruction applying the IFFT on the RF volume contained by the experience structure
* \warning     
* \param expr Pointer on the experience strcuture containing the complex RF volume
* \return      Pointer on the phase of the reconstructed volume
 ***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE RecVolIFFTComplexe (EXPERIENCE3D *expr)
{
	PPPVOLUME_COMPLEX_DOUBLE vol;
	PPPVOLUME_COMPLEX_DOUBLE volrec;
	int i,j,k;
	int x,y,z;

	vol= (PPPVOLUME_COMPLEX_DOUBLE) GetSignalRFComplexFromExperience(expr);
	vol= (PPPVOLUME_COMPLEX_DOUBLE) IdVolIFFT((PPPVOLUME_COMPLEX) vol);
	
	/* X permutation */
	x=IdVolDimX(vol);
	y=IdVolDimY(vol);
	z=IdVolDimZ(vol);
	volrec = (PPPVOLUME_COMPLEX_DOUBLE) IdVolAlloc(x,y,z,VOL_COMPLEX_DOUBLE);
	if(!volrec)
	 {
	  printf("Impossible d'allouer un volume dans RecVolIFFTComplexe !\n");
	  exit(0);
     }
    for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	 {
	  volrec[k][j][i].re=vol[k][j][x-1-i].re;
	  volrec[k][j][i].im=vol[k][j][x-1-i].im;
	 }
    IdVolFree(vol);
	return(volrec);
}


/*!
**********************************************************
* \ingroup     Reconstruction
* \brief       Volume reconstruction of the module applying the IFFT on the RF volume contained by the experience structure
* \warning 
* \param expr Pointer on the experience strcuture containing the complex RF volume
* \return      Pointer on the module of the reconstructed volume
 ***********************************************************/
PPPVOLUME_FLOAT RecVolIFFTModule (EXPERIENCE3D *expr)
{
	PPPVOLUME_COMPLEX_DOUBLE volrec;
	PPPVOLUME_FLOAT module;
	int i,j,k;
	int it,jt,kt;
	int x,y,z;
	double a,b;

	volrec= (PPPVOLUME_COMPLEX_DOUBLE) RecVolIFFTComplexe(expr);
	x = IdVolDimX(volrec); y=IdVolDimY(volrec); z=IdVolDimZ(volrec);

	module = (PPPVOLUME_FLOAT) IdVolAlloc(x,y,z,VOL_FLOAT);

	/* Module computation with dimx*dimy*dimz permutations */
	for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	{
		it = (i + (x / 2)) % x;
		jt = (j + (y / 2)) % y;
		kt = (k + (z / 2)) % z;
		a = volrec[k][j][i].re;
		b = volrec[k][j][i].im;
		module[kt][jt][it] = (float) sqrt((a*a) + (b*b));
	}
	
	IdVolFree(volrec);
	
	return(module);
}

/*!
**********************************************************
* \ingroup     Reconstruction
* \brief       Volume reconstruction of the phase applying the IFFT on the RF volume contained by the experience structure
* \warning     
* \param expr Pointer on the experience strcuture containing the complex RF volume
* \return      Pointer on the phase of the reconstructed volume
 ***********************************************************/
PPPVOLUME_FLOAT RecVolIFFTPhase (EXPERIENCE3D *expr)
{
	PPPVOLUME_COMPLEX_DOUBLE volrec;
	PPPVOLUME_FLOAT phase;
	int i,j,k;
	int it,jt,kt;
	int x,y,z;
	double a,b;

	volrec= (PPPVOLUME_COMPLEX_DOUBLE) RecVolIFFTComplexe(expr);
	x = IdVolDimX(volrec); y=IdVolDimY(volrec); z=IdVolDimZ(volrec);

	phase = (PPPVOLUME_FLOAT) IdVolAlloc(x,y,z,VOL_FLOAT);

	/* Phase computation with dimx*dimy*dimz permutations */
	for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	{
		it = (i + (x / 2)) % x;
		jt = (j + (y / 2)) % y;
		kt = (k + (z / 2)) % z;
		a = volrec[k][j][i].re;
		b = volrec[k][j][i].im;
		phase[kt][jt][it] = (float) atan(b/a);
	}
	
	IdVolFree(volrec);
	
	return(phase);
}

