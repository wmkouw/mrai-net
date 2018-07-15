/*************************************************************************
* $Id: noise.c,v 1.7 2006/12/18 15:10:37 yougz Exp $
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

#include "noise.h"
#include <math.h>
#include <idseq.h>
#include <stdlib.h>

#define PI 3.14159265358979
/*!
**********************************************************************
\ingroup Noise
\brief Gaussian Noise generator by the Box-Muller Method
\param s Standard deviation
\param m Mean
\param initseed seed for random init, or not if (0)
\return random value
**********************************************************************/
double RandGaussian(double s, double m, int initseed)
{
 double r1,r2;
 double x;

 if (initseed!=0) srand((unsigned)(initseed*time(NULL)));

 r1=rand()/(double)RAND_MAX;
 r2=rand()/(double)RAND_MAX;
 
 x= (s*sqrt(-2.0*log(r1))*cos(2*PI*r2)) + m;
 
 return(x);
}

/*!
**********************************************************************
\ingroup Noise
\brief Uniform Noise generator 
\param initseed seed for random init, or not if (0)
\return random value [0,1]
**********************************************************************/
double RandUniform(int initseed)
{
 double x;

 if (initseed!=0) srand((unsigned)(initseed*time(NULL)));

 x=rand()/(double)RAND_MAX;
  
 return(x);
}

/*!
**********************************************************************
\ingroup Noise
\brief Add noise to K space volume
\param volrf Pointer on the K space volume
\param s Noise variance 
**********************************************************************/
void AddGaussianNoiseToRFVolume(PPPVOLUME_COMPLEX_DOUBLE volrf,double s)
{
 int i,j,k;
 double x,y,z;
 double r1,r2;
 
 x= (double) IdVolDimX(volrf);
 y= (double) IdVolDimY(volrf);
 z= (double) IdVolDimZ(volrf);
 
 for(k=0;k<z;k++) for(j=0;j<y;j++)
 {
	 r1=RandGaussian(s,0,k*j); /* to change seed beetween each 1D signal */
	 for(i=0;i<x;i++) /* 1 signal RF 1D */
	 {
	  r1=RandGaussian(s,0,0);
	  r2=RandGaussian(s,0,0);
	  volrf[k][j][i].re=volrf[k][j][i].re + r1;
	  volrf[k][j][i].im=volrf[k][j][i].im + r2;
	 }
 }
}

/*!
**********************************************************************
\ingroup Noise
\brief Add noise to K space volume
\param volrf Pointer on the K space volume
\param p in % to get the noise variance from the maximum of the rf signal
**********************************************************************/
void AddGaussianNoisePercentToRFVolume(PPPVOLUME_COMPLEX_DOUBLE volrf,double p)
{
 int i,j,k;
 double x,y,z;
 double r1,r2;
 double m,s;
 m=0.0;
 
 x= (double) IdVolDimX(volrf);
 y= (double) IdVolDimY(volrf);
 z= (double) IdVolDimZ(volrf);

 for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
 {
  if (volrf[k][j][i].re >m) m=volrf[k][j][i].re;
  if (volrf[k][j][i].im >m) m=volrf[k][j][i].im;
 }
 s = m*p;
 
 for(k=0;k<z;k++) for(j=0;j<y;j++)
 {
	 r1=RandGaussian(s,0,k*j); /* to change seed beetween each 1D signal */
	 for(i=0;i<x;i++) /* 1 signal RF 1D */
	 {
	  r1=RandGaussian(s,0,0);
	  r2=RandGaussian(s,0,0);
	  volrf[k][j][i].re=volrf[k][j][i].re + r1;
	  volrf[k][j][i].im=volrf[k][j][i].im + r2;
	 }
 }
}

/*!
**********************************************************************
\ingroup Noise
\brief Add noise to Object
\param object Pointer on the virtual object
\param s Noise variance 
**********************************************************************/
void AddNoiseToObject(OBJECT3D *object,double s)
{
 int i,j,k,n;
 double x,y,z;
 double r0,r1,r2;
 int nbparam;
 
 x=(double) object->x; 
 y=(double) object->y;
 z=(double) object->z;
 nbparam = object->nbparam;

 r0=RandGaussian(1,0,1); /* Initialize seed */
 
 for (n=0;n<object->nbcomponent;n++)
	{
	for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++) 
	{
     r0=RandGaussian(s,0,0);
	 r1=RandGaussian(s,0,0);
	 r2=RandGaussian(s,0,0);
	 if (object->values[nbparam*n+0][k][j][i]!=0) 
		 object->values[nbparam*n+0][k][j][i] += (short) r0;
	 object->values[nbparam*n+1][k][j][i] += (short) r1;
	 object->values[nbparam*n+2][k][j][i] += (short) r2;
	}
	} 
}

/*!
**********************************************************************
\ingroup Noise
\brief Add noise to Object
\param object Pointer on the virtual object
\param p in % to get the noise variance from the maximum of object component
**********************************************************************/
void AddNoisePercentToObject(OBJECT3D *object,double p)
{
 int i,j,k,n;
 double x,y,z;
 double r0,r1,r2,sro,st1,st2;
 short maxro,maxt1,maxt2;
 int nbparam;
 
 x=(double) object->x; 
 y=(double) object->y;
 z=(double) object->z;
 nbparam = object->nbparam;
 
 r0=RandGaussian(1,0,1); /* Initialize seed */
 
 for (n=0;n<object->nbcomponent;n++)
	{
	 maxro=0;maxt1=0;maxt2=0;
	 for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	 {
	  if (object->values[nbparam*n+0][k][j][i]>maxro)
		  maxro=object->values[nbparam*n+0][k][j][i];
	  if (object->values[nbparam*n+1][k][j][i]>maxro)
		  maxt1=object->values[nbparam*n+1][k][j][i];
	  if (object->values[nbparam*n+2][k][j][i]>maxro) 
		  maxt2=object->values[nbparam*n+2][k][j][i];
	 }
	sro = maxro*p; st1 = maxt1*p; st2 = maxt2*p;
	for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	{
     r0=RandGaussian(sro,0,0);
	 r1=RandGaussian(st1,0,0);
	 r2=RandGaussian(st2,0,0);
	 if (object->values[nbparam*n+0][k][j][i]!=0) 
		 object->values[nbparam*n+0][k][j][i] += (short) r0;
	 object->values[nbparam*n+1][k][j][i] += (short) r1;
	 object->values[nbparam*n+2][k][j][i] += (short) r2;
	}
	} 
 
}

/*!
**********************************************************************
\ingroup Noise
\brief Apply an mean average filter ro each object component
\param object Pointer on the virtual object
\param N Size of the filter, must be odd, 3 recommended
**********************************************************************/
void ApplyMeanToObject(OBJECT3D *object,int N)
{
 int x,y,z;
 int p,q,r;
 int i,j,k;
 int n,n2;
 int nbparam;
 unsigned short val0,val1,val2;

 if (N%2 == 0) 
 {
  printf("Only an odd length mean average filter is supported !\n");
  exit(1);
 }

 x=object->x; 
 y=object->y;
 z=object->z;
 nbparam = object->nbparam;

 n2 = (N-1)/2;

 for (n=0;n<object->nbcomponent;n++)
	{
	 if ((x!=1)&&(y!=1)&&(z!=1)) /*3D*/
	 {
		 N = N*N*N;
		 for(k=n2;k<z-n2;k++) for(j=n2;j<y-n2;j++) for(i=n2;i<x-n2;i++)
		 {
			 val0=0; val1=0; val2=0;
			 for(r=-n2;(r<=n2)&&(r!=0);r++) for(q=-n2;(q<=n2)&&(q!=0);q++) for(p=-n2;(p<=n2)&&(p!=0);p++)
			 {
				 val0 += object->values[nbparam*n+0][k+r][j+q][i+p];
				 val1 += object->values[nbparam*n+1][k+r][j+q][i+p];
				 val2 += object->values[nbparam*n+2][k+r][j+q][i+p];
			 }
			 object->values[nbparam*n+0][k][j][i]= val0 / N;
			 object->values[nbparam*n+1][k][j][i]= val1 / N;
			 object->values[nbparam*n+2][k][j][i]= val2 / N;
			 
		 }
	 }
	 if ((x!=1)&&(y!=1)&&(z==1)) /*2D*/
	 {
		 N = N*N;
		 for(k=0;k<z;k++) for(j=n2;j<y-n2;j++) for(i=n2;i<x-n2;i++)
		 {
			 val0=0; val1=0; val2=0;
			 for(q=-n2;q<=n2;q++) for(p=-n2;p<=n2;p++)
			 {
				 val0 += object->values[nbparam*n+0][k][j+q][i+p];
				 val1 += object->values[nbparam*n+1][k][j+q][i+p];
				 val2 += object->values[nbparam*n+2][k][j+q][i+p];
			 }
			 object->values[nbparam*n+0][k][j][i]= val0 / N;
			 object->values[nbparam*n+1][k][j][i]= val1 / N;
			 object->values[nbparam*n+2][k][j][i]= val2 / N;
		 }
	 }
	 if ((x!=1)&&(y==1)&&(z==1)) /*1D*/
	 {
		 for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=n2;i<x-n2;i++)
		 {
			 val0=0; val1=0; val2=0;
			 for(p=-n2;p<=n2;p++)
			 {
				 val0 += object->values[nbparam*n+0][k][j][i+p];
				 val1 += object->values[nbparam*n+1][k][j][i+p];
				 val2 += object->values[nbparam*n+2][k][j][i+p];
			 }
			 object->values[nbparam*n+0][k][j][i]= val0 / N;
			 object->values[nbparam*n+1][k][j][i]= val1 / N;
			 object->values[nbparam*n+2][k][j][i]= val2 / N;
		 }
	 }
	}
 }