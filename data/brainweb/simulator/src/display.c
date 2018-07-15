/*************************************************************************
* $Id: display.c,v 1.11 2006/12/18 15:10:36 yougz Exp $
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
  software by the user in light of is specific status of free software,
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
#include "display.h"
#include <string.h>

/*! \ingroup Display
 **********************************************************
* \brief Display a 2D image
* This function just calls the library Libido function IdImaDisplay
* and checks if the DISPLAY is not broken (useful for test on grid architecture)
* \param vol Pointer on the volume to display
* \param deb First plane to display
* \param nb  Number of planes to dispaly
* \param visuname name for the figure title
 ***********************************************************/
void DisplayImage2D(PPIMAGE_UCHAR image)
{
 
 #if ! defined(BROKEN_IDIMADISPLAY)
    IdImaDisplay(image,0,0,0,IdImaDimX(image),IdImaDimY(image),0,0);
 #endif

}
/*! \ingroup Display
 **********************************************************
* \brief Display the two first components (ro, t1, t2) of a 3D object
* \param vol Pointer on the volume to display
* \param deb First plane to display
* \param nb  Number of planes to dispaly
* \param visuname name for the figure title
 ***********************************************************/
void DisplayObject3D(OBJECT3D* object,int deb,int nb,char *visuname)
{
 PPPVOLUME_USHORT vol;
 int x,y,z,i,j,k;
 char txt[80];
  
 x=object->x;
 y=object->y;
 z=object->z;
 
 vol =(PPPVOLUME_USHORT) IdVolAlloc(x,y,z,VOL_USHORT);
 if (!vol)
  {
   printf("Unable to allocate a ushort volume in DisplayOBJECT3D !\n");
   exit(0);
  }
 
 /* Display the first component considered to be water */
 for (i=0;i<x;i++) for(j=0;j<y;j++) for(k=0;k<z;k++)  /* T1 */
       vol[k][j][i]=object->values[T1][k][j][i];
 strcpy(txt,visuname);
 strcat(txt,"-T1 water");
 DisplayVol((PPPVOLUME)vol,0,z,0,255,txt,0,"\0");

 for (i=0;i<x;i++) for(j=0;j<y;j++) for(k=0;k<z;k++)  /* T2 */
       vol[k][j][i]=object->values[T2][k][j][i];
 strcpy(txt,visuname);
 strcat(txt,"-T2 water");
 DisplayVol((PPPVOLUME)vol,0,z,0,255,txt,0,"\0");

 for (i=0;i<x;i++) for(j=0;j<y;j++) for(k=0;k<z;k++)  /* RO */
       vol[k][j][i]=object->values[RO][k][j][i];
 strcpy(txt,visuname);
 strcat(txt,"-RO water");
 DisplayVol((PPPVOLUME)vol,0,z,0,255,txt,0,"\0");

/* Display the second component considered to be fat */
if (object->nbcomponent>1)
{
for (i=0;i<x;i++) for(j=0;j<y;j++) for(k=0;k<z;k++)  /* T1 */
       vol[k][j][i]=object->values[4][k][j][i];
 strcpy(txt,visuname);
 strcat(txt,"-T1 Fat");
 DisplayVol((PPPVOLUME)vol,0,z,0,255,txt,0,"\0");

 for (i=0;i<x;i++) for(j=0;j<y;j++) for(k=0;k<z;k++)  /* T2 */
       vol[k][j][i]=object->values[5][k][j][i];
 strcpy(txt,visuname);
 strcat(txt,"-T2 Fat");
 DisplayVol((PPPVOLUME)vol,0,z,0,255,txt,0,"\0");

 for (i=0;i<x;i++) for(j=0;j<y;j++) for(k=0;k<z;k++)  /* RO */
       vol[k][j][i]=object->values[3][k][j][i];
 strcpy(txt,visuname);
 strcat(txt,"-RO Fat");
 DisplayVol((PPPVOLUME)vol,0,z,0,255,txt,0,"\0");
}

 IdVolFree(vol);
}


/*! \ingroup Display
*********************************************************************
* \brief Display the two first components (ro, t1, t2) of a 2D object
* \param vol Pointer on the volume to display
* \param visuname name for the figure title
**********************************************************************/
void DisplayObject2D (OBJECT3D* object,char * visuname)
{
	register int i,j;
	int x,y,z;
	PPIMAGE_USHORT imashort;
	double min,max;
	PPIMAGE_UCHAR imauchar;
	char txt[80];

	txt[0]='\0';

	x=object->x;
	y=object->y;
    z=object->z;

	if (z!=1)
      printf("Watchout, your object is not 2D \n!");

	imashort = (PPIMAGE_USHORT) IdImaAlloc(x,y,IMA_SHORT);
	if(!imashort)
	 {
	  printf("Impossible d'allouer imashort dans Displayobject2D !\n");
	  exit(0);
	 }

	imauchar = (PPIMAGE_UCHAR) IdImaAlloc(x,y,IMA_UCHAR);
	if(!imauchar)
	 {
	  printf("Impossible d'allouer imauchar dans Displayobject2D !\n");
	  exit(0);
	 }

	/* Display the first component considered to be water */
	for (i=0;i<x;i++) for(j=0;j<y;j++)  /* T1 */
       imashort[j][i]=object->values[T1][0][j][i];
	min = IdImaMinima((PPIMAGE)imashort);
	max = IdImaMaxima((PPIMAGE)imashort);
	if (max!=min)
	 {
	  for (i=0;i<x;i++) for(j=0;j<y;j++)
	     imauchar[i][j]= (int)((imashort[i][j]-min)/(max-min)*255);
	 }
    else 
	 {
	 	  for (i=0;i<x;i++) for(j=0;j<y;j++)
	     imauchar[i][j]= (int) min;
	 }
    strcpy(txt,visuname);
    strcat(txt,"-T1 water");
	IdSetFileName(imauchar,txt);
	DisplayImage2D(imauchar);	
	for (i=0;i<x;i++) for(j=0;j<y;j++)  /* T2 */
       imashort[j][i]=object->values[T2][0][j][i];
	min = IdImaMinima((PPIMAGE)imashort);
	max = IdImaMaxima((PPIMAGE)imashort);
	if (max!=min)
	 {
	  for (i=0;i<x;i++) for(j=0;j<y;j++)
	     imauchar[i][j]= (int)((imashort[i][j]-min)/(max-min)*255);
	 }
    else 
	 {
	 	  for (i=0;i<x;i++) for(j=0;j<y;j++)
	     imauchar[i][j]= (int) min;
	 }
    strcpy(txt,visuname);
    strcat(txt,"-T2 water");
	IdSetFileName(imauchar,txt);
	DisplayImage2D(imauchar);	
	for (i=0;i<x;i++) for(j=0;j<y;j++)  /*  RO */
       imashort[j][i]=object->values[RO][0][j][i];
	min = IdImaMinima((PPIMAGE)imashort);
	max = IdImaMaxima((PPIMAGE)imashort);
	if (max!=min)
	 {
	  for (i=0;i<x;i++) for(j=0;j<y;j++)
	     imauchar[i][j]= (int)((imashort[i][j]-min)/(max-min)*255);
	 }
    else 
	 {
	 	  for (i=0;i<x;i++) for(j=0;j<y;j++)
	     imauchar[i][j]= (int) min;
	 }
    strcpy(txt,visuname);
    strcat(txt,"-RO water");
	IdSetFileName(imauchar,txt);
	DisplayImage2D(imauchar);	
 
	/* Display the second component considered to be fat */
if (object->nbcomponent>1)
{
for (i=0;i<x;i++) for(j=0;j<y;j++)  /* Recopie de T1 */
       imashort[j][i]=object->values[T1+3][0][j][i];
	min = IdImaMinima((PPIMAGE)imashort);
	max = IdImaMaxima((PPIMAGE)imashort);
	if (max!=min)
	 {
	  for (i=0;i<x;i++) for(j=0;j<y;j++)
	     imauchar[i][j]= (int)((imashort[i][j]-min)/(max-min)*255);
	 }
    else 
	 {
	 	  for (i=0;i<x;i++) for(j=0;j<y;j++)
	     imauchar[i][j]= (int) min;
	 }
    strcpy(txt,visuname);
    strcat(txt,"-T1 fat");
	IdSetFileName(imauchar,txt);
	DisplayImage2D(imauchar);	
	for (i=0;i<x;i++) for(j=0;j<y;j++)  /* T2 */
       imashort[j][i]=object->values[T2+3][0][j][i];
	min = IdImaMinima((PPIMAGE)imashort);
	max = IdImaMaxima((PPIMAGE)imashort);
	if (max!=min)
	 {
	  for (i=0;i<x;i++) for(j=0;j<y;j++)
	     imauchar[i][j]= (int)((imashort[i][j]-min)/(max-min)*255);
	 }
    else 
	 {
	 	  for (i=0;i<x;i++) for(j=0;j<y;j++)
	     imauchar[i][j]= (int) min;
	 }
    strcpy(txt,visuname);
    strcat(txt,"-T2 fat");
	IdSetFileName(imauchar,txt);
	DisplayImage2D(imauchar);	
	for (i=0;i<x;i++) for(j=0;j<y;j++)  /* RO */
       imashort[j][i]=object->values[RO+3][0][j][i];
	min = IdImaMinima((PPIMAGE)imashort);
	max = IdImaMaxima((PPIMAGE)imashort);
	if (max!=min)
	 {
	  for (i=0;i<x;i++) for(j=0;j<y;j++)
	     imauchar[i][j]= (int)((imashort[i][j]-min)/(max-min)*255);
	 }
    else 
	 {
	 	  for (i=0;i<x;i++) for(j=0;j<y;j++)
	     imauchar[i][j]= (int) min;
	 }
    strcpy(txt,visuname);
    strcat(txt,"-RO fat");
	IdSetFileName(imauchar,txt);
	DisplayImage2D(imauchar);	
}
	IdImaFree(imauchar);
	IdImaFree(imashort);
}


/*! \ingroup Display
*****************************************************************
* \brief Display the two first components (ro, t1, t2) of a 1D object
* \param vol Pointer on the volume to display
* \param visuname name for the figure title
******************************************************************/
void DisplayObject1D (OBJECT3D* object, char* visuname)
{
	int i;
	int x,y,z;
	PSIGNAL_USHORT sig;
	PPIMAGE_UCHAR ima;
	char txt[80];

    txt[0]='\0';

	x=object->x;
    y=object->y;
	z=object->z;

	if ((y!=1)||(z!=1))
      printf("Watchout ! Your object is not 1D \n!");
	
	sig = (PSIGNAL_USHORT) IdSigAlloc(x,SIG_USHORT);
	if (!sig)
	  {
	   printf("Unable to allocate sig in DisplayObject1D !\n");
	   exit(0);
	  }

	/* Display the first component considered to be water */
    for (i=0;i<x;i++)  /* T1 */
        sig[i]=object->values[1][0][0][i];
	ima=IdImaSignalToImage(sig,0,x,0,-1,-1,x,128,255,0);
    strcpy(txt,visuname);
    strcat(txt,"-T1 Water");
	IdSetFileName(ima,txt);
	DisplayImage2D(ima);	
	IdImaFree(ima);
	for (i=0;i<x;i++)  /* T2 */
        sig[i]=object->values[2][0][0][i];
 	ima=IdImaSignalToImage(sig,0,x,0,-1,-1,x,128,255,0);;
    strcpy(txt,visuname);
    strcat(txt,"-T2 Water");
	IdSetFileName(ima,txt);
	DisplayImage2D(ima);	
	IdImaFree(ima);
	for (i=0;i<x;i++)  /* RO */
        sig[i]=object->values[0][0][0][i];
    ima=IdImaSignalToImage(sig,0,x,0,-1,-1,x,128,255,0);
    strcpy(txt,visuname);
    strcat(txt,"-RO Water");
	IdSetFileName(ima,txt);
	DisplayImage2D(ima);	
	IdImaFree(ima);

	/* Display the second component considered to be fat */
    if (object->nbcomponent>1)
	{
    for (i=0;i<x;i++)  /* T1 */
        sig[i]=object->values[4][0][0][i];
	ima=IdImaSignalToImage(sig,0,x,0,-1,-1,x,128,255,0);
    strcpy(txt,visuname);
    strcat(txt,"-T1 Fat");
	IdSetFileName(ima,txt);
	DisplayImage2D(ima);	
	IdImaFree(ima);
	for (i=0;i<x;i++)  /* T2 */
        sig[i]=object->values[5][0][0][i];
 	ima=IdImaSignalToImage(sig,0,x,0,-1,-1,x,128,255,0);
    strcpy(txt,visuname);
    strcat(txt,"-T2 Fat");
	IdSetFileName(ima,txt);
	DisplayImage2D(ima);	
	IdImaFree(ima);
	for (i=0;i<x;i++)  /* RO */
        sig[i]=object->values[3][0][0][i];
    ima=IdImaSignalToImage(sig,0,x,0,-1,-1,x,128,255,0);
    strcpy(txt,visuname);
    strcat(txt,"-RO Fat");
	IdSetFileName(ima,txt);
	DisplayImage2D(ima);	
	IdImaFree(ima);
    }

	IdSigFree(sig);
}

/*!
**********************************************************
* \ingroup     Display
* \brief       Display a volume plane by plane
* \warning     Does not work for Complex volume
* \param vol Pointer on the volume to display
* \param deb First plane to display
* \param nb  Number of planes to dispaly
* \param recad Gray level scaling used for display (linear (0), Abs (1) log (2))
* \param n Scaling factor.
* \param visuname Name of the figure title
* \param save Set to 1 if you want to save the displayed signal
* \param filename Name of the file used to save the signal
***********************************************************/
void DisplayVol (PPPVOLUME vol,int deb, int nb, int recad, int N, char *visuname, int save, char *filename)
{
 typedef unsigned char uchar;
 typedef unsigned short ushort;
 typedef unsigned long ulong;
 char             blabla[400];
 int              X,Y,Z;
 PPPVOLUME_UCHAR  voluchar;
 PPIMAGE_UCHAR    ima;
 int              i,j,k;
 int              nbx,nby;
 int              dimx,dimy;
 int              x,y,compt;
 double           mini,maxi;
 double           val;

 blabla[0]='\0';

 X=IdVolDimX(vol); Y=IdVolDimY(vol); Z=IdVolDimZ(vol);

 voluchar = (PPPVOLUME_UCHAR) IdVolAlloc(X,Y,Z,VOL_UCHAR);
 if(!voluchar)
  {
   printf("Allocation failure of voluchar in DisplayVol !\n");
   exit(0);
  }

#define ABSO 1
#define LOG 2

#define RABS(type,tt) for(k=0;k<Z;k++) for(j=0;j<Y;j++) for(i=0;i<X;i++) \
                       ((type)vol)[k][j][i] = (tt) fabs((double)((type)vol)[k][j][i]);  

#define RLOG(type,tt) for(k=0;k<Z;k++) for(j=0;j<Y;j++) for(i=0;i<X;i++) \
                       ((type)vol)[k][j][i] = (tt) log(1.0+fabs((double)((type)vol)[k][j][i]));  

#define RECAD(type) for(k=0;k<Z;k++) for(j=0;j<Y;j++) for(i=0;i<X;i++) \
                       {	\
					    val = (double) ((type)vol)[k][j][i];	\
                        val = (val-mini) / (maxi-mini) * N;	\
						if (val>255.0) val = 255.0;	\
						voluchar[k][j][i]=(unsigned char) val;	\
					   }
if (recad==ABSO)
 {
  switch(IdVolType(vol))
   { 
    case VOL_UCHAR:
    {
     RABS(PPPVOLUME_UCHAR,uchar);
    }
    break;
    case VOL_USHORT:
    {
	RABS(PPPVOLUME_USHORT,ushort);
    }
    break;
    case VOL_SHORT:
    {
	RABS(PPPVOLUME_SHORT,short);
    }
    break;
    case VOL_ULONG:
    {
	RABS(PPPVOLUME_ULONG,ulong);
    }
    break;
    case VOL_LONG:
    {
	RABS(PPPVOLUME_LONG,long);
    }
    break;
    case VOL_FLOAT:
    {
	RABS(PPPVOLUME_FLOAT,float);
    }
    break;
    case VOL_DOUBLE:
    {
	RABS(PPPVOLUME_DOUBLE,double);
    }
    break;
    default:
    {
     printf("This Volume type is not supported by DisplayVol !\n");
     exit(0);
    }
    break;
   }
  }

if (recad==LOG)
 {
  switch(IdVolType(vol))
   { 
    case VOL_UCHAR:
    {
     RLOG(PPPVOLUME_UCHAR,uchar);
    }
    break;
    case VOL_USHORT:
    {
	RLOG(PPPVOLUME_USHORT,ushort);
    }
    break;
    case VOL_SHORT:
    {
	RLOG(PPPVOLUME_SHORT,short);
    }
    break;
    case VOL_ULONG:
    {
	RLOG(PPPVOLUME_ULONG,ulong);
    }
    break;
    case VOL_LONG:
    {
	RLOG(PPPVOLUME_LONG,long);
    }
    break;
    case VOL_FLOAT:
    {
	RLOG(PPPVOLUME_FLOAT,float);
    }
    break;
    case VOL_DOUBLE:
    {
	RLOG(PPPVOLUME_DOUBLE,double);
    }
    break;
    default:
    {
     printf("This Volume type is not supported by DisplayVol !\n");
     exit(0);
    }
    break;
   }
  }

 mini = IdVolMinima(vol); maxi = IdVolMaxima(vol); 
 
  switch(IdVolType(vol))
   { 
    case VOL_UCHAR:
    {
     RECAD(PPPVOLUME_UCHAR);
    }
    break;
    case VOL_USHORT:
    {
	RECAD(PPPVOLUME_USHORT);
    }
    break;
    case VOL_SHORT:
    {
	RECAD(PPPVOLUME_SHORT);
    }
    break;
    case VOL_ULONG:
    {
	RECAD(PPPVOLUME_ULONG);
    }
    break;
    case VOL_LONG:
    {
	RECAD(PPPVOLUME_LONG);
    }
    break;
    case VOL_FLOAT:
    {
	RECAD(PPPVOLUME_FLOAT);
    }
    break;
    case VOL_DOUBLE:
    {
	RECAD(PPPVOLUME_DOUBLE);
    }
    break;
    default:
    {
     printf("This Volume type is not supported by DisplayVol!\n");
     exit(0);
    }
    break;
   }

 if (nb+deb>Z)
   {
    printf("Nb > profondeur du volume !!! \n");
    exit(0);
   }

 nbx = (int) floor(sqrt((double)nb));
 nby = nbx;
 dimx = nbx * X;
 dimy = nby * Y;

 ima = (PPIMAGE_UCHAR) IdImaAlloc(dimx,dimy,IMA_UCHAR);
 if (!ima)
   {
    printf("Impossible d'allouer l'image de visu !\n");
    exit(0);
   }
 for (i=0;i<dimy;i++) for(j=0;j<dimx;j++)
    ima[i][j]=0;
 compt = deb;
 for (y=0;y<nby;y++) for (x=0;x<nbx;x++)
   {
    if (compt<nb+deb)
      {
       for(i=0;i<Y;i++) for(j=0;j<X;j++)
	  ima[i+y*Y][j+x*X] = voluchar[compt][i][j];
      }
    compt++;
   }
 IdVolFree(voluchar);
 sprintf(blabla," %d -> %d",deb,deb+nb-1);
 strcat(blabla,visuname);
 IdSetFileName(ima,visuname);
 DisplayImage2D(ima);	
 if (save==1)
   IdAcrWriteFile(filename,ima);
 IdImaFree(ima);
}

/*!
/**********************************************************
* \ingroup     Display
* \brief       Display an XY plane of a 3D volume
* \param vol Pointer on the volume to display
* \param z Plane number
* \param visuname Name of the figure title
***********************************************************/
void DisplayVolXY (PPPVOLUME vol,int z,char *visuname)
{
 DisplayVol(vol,z,1,0,255,visuname,0,"\0");
}

/*!
/**********************************************************
* \ingroup     Display
* \brief       Display an X line of a 3D volume, and can save the corresponding signal
* \param vol Pointer on the volume to display
* \param y Column number
* \param z Plane number
* \param visuname Name of the figure title
* \param save Set to 1 if you want to save the displayed signal
* \param filename Name of the file used to save the signal
***********************************************************/
void DisplayVolX (PPPVOLUME vol,int y,int z,char *visuname,int save,char *filename)
{
	int i;
	int x;
	PSIGNAL_DOUBLE sig;
	PPIMAGE_UCHAR ima;
	char txt[80];

	txt[0]='\0';
	sprintf(txt,"-Y:%d-Z:%d",y,z);
	strcat(txt,visuname);
	
	x=IdVolDimX(vol);

	sig = (PSIGNAL_DOUBLE) IdSigAlloc(x,SIG_DOUBLE);
	if (!sig)
	  {
	   printf("Impossible d'allouer sig !\n");
	   exit(0);
	  }
	switch (IdVolType(vol)) 
	{
     case VOL_DOUBLE:
      for (i=0;i<x;i++) sig[i]= (double) ((PPPVOLUME_DOUBLE)vol)[z][y][i];
     break;
     case VOL_FLOAT:
      for (i=0;i<x;i++) 
	  {
		  sig[i]= (double) ((PPPVOLUME_FLOAT)vol)[z][y][i];
	  }
     break;
	 case VOL_UCHAR:
      for (i=0;i<x;i++) sig[i]= (double) ((PPPVOLUME_UCHAR)vol)[z][y][i];
     break;
	 default:
	 { 
	  printf("Volume type not yet managed !\n");
	  exit(0);
	 }
	 break;
	}
	printf("sig[0]=%lf \n",sig[0]);
	printf("sig[%d]=%lf \n",x/4,sig[x/4]);
	printf("sig[%d]=%lf \n",x/2-1,sig[x/2-1]);
	printf("sig[%d]=%lf \n",x/2,sig[x/2]);
	printf("sig[%d]=%lf \n",x/2+1,sig[x/2+1]);
	printf("sig[%d]=%lf \n",x-1,sig[x-1]);
	
	if (save==1) IdSigWriteRawFile(filename,(PSIGNAL) sig);

 	ima=IdImaSignalToImage(sig,0,x,0,-1,-1,x,128,255,0);
	IdSigFree(sig);
	IdSetFileName(ima,txt);
    DisplayImage2D(ima);	
	IdImaFree(ima);
}
