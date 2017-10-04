/*************************************************************************
* $Id: imagette.c,v 1.1 2005/09/09 08:22:48 bellet Exp $
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

#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "idima.h"
#include "iderr.h"
#include "idprint.h"

static PPIMAGE_DOUBLE imDouble=NULL;


/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaRecadTo8   (fonction)
* 
* RESUME: Recadre (Dynamique) une image non COMPLEX ds une image 8 bits
* 
* DESCRIPTION: Recadre (Dynamique) une image non COMPLEX ds une image 8 bits
* 
* SYNTAXE:    PPIMAGE_UCHAR imDest= IdImaRecadTo8 (PPIMAGE imSource, PPIMAGE_UCHAR imDest, double minImpose, double maxImpose);
* 
* RETOUR    :  type  : PPIMAGE_UCHAR
* 	     role  : ptr sur image recadree.
* 
* PARAMETRES:  nom   : imSource
* 	     type  : PPIMAGE
* 	     role  : Pointeur vers l'image source
* 
*  	     nom   : imDest
* 	     type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers l' Image destination
* 
* 	     nom   : minImpose, maxImpose 
*              type  : double
* 	     role  : min et max imposes pour le recadrage 
* 		   : sans effet si -1.0,-1.0
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

#define FF1(typeima)											\
													\
if ( (minImpose!=-1.) && (maxImpose!=-1.) )								\
{													\
	mini=minImpose;		/* imposes; on les prend */						\
	maxi=maxImpose;											\
													\
	MM= maxi-mini;											\
	if(MM==0.)											\
 	         { 	IdErrPrintf (" Min = Max  = %g; Impossible recadrer \n",maxi);			\
			IdErrno=IDERR_MIN_EQ_MAX;							\
   	 		return 0;									\
	   	 }											\
													\
    	for(i=0;i<IdImaDimY(imS);i++)									\
       	for(j=0;j<IdImaDimX(imS);j++)									\
        { 												\
	pixel = (double)(((typeima)imS)[i][j]);								\
	if     (pixel>maxi) imD[i][j]=255;								\
	else if(pixel<mini) imD[i][j]=0;								\
	  else {											\
    	  	imD[i][j]=(unsigned char) ( (double)( (pixel-mini)*255 ) /MM );				\
		}											\
        }												\
}													\
 else													\
{													\
	maxi= - FLT_MAX;  		/* non imposes, on les cherche */				\
	mini=   FLT_MAX;										\
	for(i=0;i<IdImaDimY(imS);i++)									\
	 for(j=0;j<IdImaDimX(imS);j++)									\
 	  { if((((typeima)imS)[i][j]) > maxi) maxi=(((typeima)imS)[i][j]);				\
   	    if((((typeima)imS)[i][j]) < mini) mini=(((typeima)imS)[i][j]);				\
	  }												\
													\
 /*               Recadrage de l'image       */								\
	MM=maxi-mini;											\
	if(MM==0.)											\
 	         { 	IdErrPrintf (" Min = Max = %g; Impossible recadrer \n",mini);			\
			IdErrno=IDERR_MIN_EQ_MAX;							\
   	 		return 0;									\
	   	 }											\
 													\
	for(i=0;i<IdImaDimY(imS);i++)									\
 	for(j=0;j<IdImaDimX(imS);j++)									\
  		{											\
		imD[i][j]=(unsigned char) ( (double)( ((((typeima)imS)[i][j])-mini)*255 ) /MM );	\
  		}											\
}


PPIMAGE_UCHAR IdImaRecadTo8(
	PPIMAGE imS,
	PPIMAGE_UCHAR imD,
	double minImpose,
	double maxImpose)
{
double mini,maxi;
double MM;
double pixel; 

int i,j;

imD=(PPIMAGE_UCHAR)IdImaCheckSizeAllocType (imS,-1,(PPIMAGE)imD,IMA_UCHAR);

    	      if (!(imD))
 	         { 	IdErrPrintf (" IdImaRecadTo8 : echec Alloc Image 8bits \n");
			IdErrno=IDERR_ALLOC_IMA;
   	 		return 0;
	   	 }

  switch(IdImaType(imS))
    {
     case IMA_CHAR:   FF1(PPIMAGE_CHAR);   break;  
     case IMA_UCHAR:  FF1(PPIMAGE_UCHAR);  break;  
     case IMA_SHORT:  FF1(PPIMAGE_SHORT);  break;  
     case IMA_USHORT: FF1(PPIMAGE_USHORT); break;  
     case IMA_LONG:   FF1(PPIMAGE_LONG);   break; 
     case IMA_ULONG:  FF1(PPIMAGE_ULONG);  break;   
     case IMA_FLOAT:  FF1(PPIMAGE_FLOAT);  break;  
     case IMA_DOUBLE: FF1(PPIMAGE_DOUBLE); break;  
     default:
	IdErrPrintf("Type %d non traitable par la fonction IdImaRecadTo8 !\n", IdImaType(imS));
	IdErrno=IDERR_WRONG_LIBTYPE;
	return(0);
    } 

IdSetFileName(imD,IdGetFileName(imS));
return (imD);
}

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaRecadTo8DCT   (fonction)
* 
* RESUME: Recadre (DCT) une image non COMPLEX ds une image 8 bits
* 
* DESCRIPTION: Recadre (DCT) une image non COMPLEX ds une image 8 bits
* 
* SYNTAXE:    PPIMAGE_UCHAR imDest= IdImaRecadTo8DCT (PPIMAGE imSource, PPIMAGE_UCHAR imDest);
* 
* RETOUR    :  type  : PPIMAGE_UCHAR
* 	     role  : ptr sur image recadree
* 
* PARAMETRES:  nom   : imSource
* 	     type  : PPIMAGE
* 	     role  : Pointeur vers l'image source
* 
*  	     nom   : imDest
* 	     type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers l'Image destination ou NULL si a creer
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

#define FF2(typeima)											\
    	for(i=0;i<IdImaDimY(imS);i++)									\
       	for(j=0;j<IdImaDimX(imS);j++)									\
  	   imDouble[i][j]=fabs((double)((typeima)imS)[i][j]);

PPIMAGE_UCHAR IdImaRecadTo8DCT(
	PPIMAGE imS,
	PPIMAGE_UCHAR imD)
{
double mini,maxi;
double MM;

int i,j;

imD=(PPIMAGE_UCHAR)IdImaCheckSizeAllocType (imS,-1,(PPIMAGE)imD,IMA_UCHAR);

    	      if (!(imD))
 	         { 	IdErrPrintf ("echec Alloc Image 8bits \n");
			IdErrno=IDERR_ALLOC_IMA;
   	 		return (0);
	   	 }

imDouble=(PPIMAGE_DOUBLE)IdImaAlloc(IdImaDimX(imS),IdImaDimY(imS),IMA_DOUBLE);

    	      if (!(imDouble))
 	         { 	IdErrPrintf (" echec Alloc Image Double\n");
			IdErrno=IDERR_ALLOC_IMA;
   	 		return 0;
	   	 }

  switch(IdImaType(imS))
    {
     case IMA_CHAR:   FF2(PPIMAGE_CHAR);   break;  
     case IMA_UCHAR:  FF2(PPIMAGE_UCHAR);  break;  
     case IMA_SHORT:  FF2(PPIMAGE_SHORT);  break;  
     case IMA_USHORT: FF2(PPIMAGE_USHORT); break;  
     case IMA_LONG:   FF2(PPIMAGE_LONG);   break; 
     case IMA_ULONG:  FF2(PPIMAGE_ULONG);  break;  
     case IMA_FLOAT:  FF2(PPIMAGE_FLOAT);  break;  
     case IMA_DOUBLE: FF2(PPIMAGE_DOUBLE); break;  
    
     default:
	IdErrPrintf("Type %d non traitable par la fonction IdImaRecadTo8DCT !\n", IdImaType(imS));
	IdErrno=IDERR_WRONG_LIBTYPE;
	return(0);
    } 

  	imDouble[0][0]=0.; /* On force la valeur DC a 0 */

    	for(i=0;i<IdImaDimY(imS);i++)
       	for(j=0;j<IdImaDimX(imS);j++)
    	   imDouble[i][j]=log(1+imDouble[i][j]);

	maxi= - FLT_MAX;
	mini=   FLT_MAX;

	for(i=0;i<IdImaDimY(imS);i++)
	 for(j=0;j<IdImaDimX(imS);j++)
 	  { if(((imDouble)[i][j]) > maxi) maxi=((imDouble)[i][j]);
   	    if(((imDouble)[i][j]) < mini) mini=((imDouble)[i][j]);
	  }

 /*               Recadrage de l'image       */
			
	MM= maxi-mini;
	if(MM==0.)   
 		{ IdErrPrintf (" Min = Max  = %g; Impossible recadrer \n",maxi);
			IdErrno=IDERR_MIN_EQ_MAX;
   	 		return 0;
	   	 }
 
	for(i=0;i<IdImaDimY(imS);i++)
 	for(j=0;j<IdImaDimX(imS);j++)
  		{
		imD[i][j]=(unsigned char) ( ( (((imDouble)[i][j])-mini)*255 ) /MM );
  		}

IdImaFree(imDouble);

return (imD);
}

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaRecadTo8LOG   (fonction)
* 
* RESUME: Recadre (LOG) une image non COMPLEX ds une image 8 bits
* 
* DESCRIPTION: Recadre (LOG) une image non COMPLEX ds une image 8 bits
* 
* SYNTAXE:    PPIMAGE_UCHAR imDest= IdImaRecadTo8LOG (PPIMAGE imSource, PPIMAGE_UCHAR imDest);
* 
* RETOUR    :  type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers l'image recadree
* 
* PARAMETRES:  nom   : imSource
* 	     type  : PPIMAGE
* 	     role  : Pointeur vers l'image source
* 
*  	     nom   : imDest
* 	     type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers l'Image destination ou NULL si a creer
* 
* FICHIER:     IMAGETTE.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

#define FF3(typeima)											\
    	for(i=0;i<IdImaDimY(imS);i++)									\
       	for(j=0;j<IdImaDimX(imS);j++)									\
  	   imDouble[i][j]=fabs((double)((typeima)imS)[i][j]);
 
PPIMAGE_UCHAR IdImaRecadTo8LOG(
	PPIMAGE imS,
	PPIMAGE_UCHAR imD)
{
double mini,maxi;
double MM;

int i,j;

imD=(PPIMAGE_UCHAR)IdImaCheckSizeAllocType (imS,-1,(PPIMAGE)imD,IMA_UCHAR);

    	      if (!(imD))
 	         { 	IdErrPrintf (" echec Alloc Image 8bits \n");
			IdErrno=IDERR_ALLOC_IMA;
   	 		return 0;
	   	 }

imDouble=(PPIMAGE_DOUBLE)IdImaAlloc(IdImaDimX(imS),IdImaDimY(imS),IMA_DOUBLE);

    	      if (!(imDouble))
 	         { 	IdErrPrintf (" echec Alloc Image Double\n");
			IdErrno=IDERR_ALLOC_IMA;
   	 		return 0;
	   	 }

  switch(IdImaType(imS))
    {
     case IMA_CHAR:   FF3(PPIMAGE_CHAR);   break;  
     case IMA_UCHAR:  FF3(PPIMAGE_UCHAR);  break;  
     case IMA_SHORT:  FF3(PPIMAGE_SHORT);  break;  
     case IMA_USHORT: FF3(PPIMAGE_USHORT); break;  
     case IMA_LONG:   FF3(PPIMAGE_LONG);   break; 
     case IMA_ULONG:  FF3(PPIMAGE_ULONG);  break;  
     case IMA_FLOAT:  FF3(PPIMAGE_FLOAT);  break;  
     case IMA_DOUBLE: FF3(PPIMAGE_DOUBLE); break;  
    
     default:
	IdErrPrintf("Type %d non traitable par la fonction IdImaRecadTo8LOG !\n", IdImaType(imS));
	IdErrno=IDERR_WRONG_LIBTYPE;
	return(0);
    }

    	for(i=0;i<IdImaDimY(imS);i++)
       	for(j=0;j<IdImaDimX(imS);j++)
    	   imDouble[i][j]=log(1.+imDouble[i][j]);

	maxi= - FLT_MAX;
	mini=   FLT_MAX;

	for(i=0;i<IdImaDimY(imS);i++)
	 for(j=0;j<IdImaDimX(imS);j++)
 	  { if(((imDouble)[i][j]) > maxi) maxi=((imDouble)[i][j]);
   	    if(((imDouble)[i][j]) < mini) mini=((imDouble)[i][j]);
	  }

 /*               Recadrage de l'image       */
			
	MM= maxi-mini;
	if(MM==0.)   
 		{ IdErrPrintf (" Min = Max  = %g; Impossible recadrer \n",maxi);
			IdErrno=IDERR_MIN_EQ_MAX;
   	 		return 0;
	   	 }
 
	for(i=0;i<IdImaDimY(imS);i++)
 	for(j=0;j<IdImaDimX(imS);j++)
  		{
		imD[i][j]=(unsigned char) ( ( (((imDouble)[i][j])-mini)*255 ) /MM );
  		}
 
IdImaFree(imDouble);

return (imD);
}

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaRecadTo8ABS   (fonction)
* 
* RESUME: Recadre (ABS) une image non COMPLEX ds une image 8 bits
* 
* DESCRIPTION: Recadre (ABS) une image non COMPLEX ds une image 8 bits
* 
* SYNTAXE:    PPIMAGE_UCHAR imDest= IdImaRecadTo8ABS (PPIMAGE imSource, PPIMAGE_UCHAR imDest);
* 
* RETOUR     :  type  : PPIMAGE_UCHAR
* 	      role  : Pointeur vers l'image destination
* 
* PARAMETRES :  nom   : imSource
* 	      type  : PPIMAGE
* 	      role  : Pointeur vers l'image source
* 
*  	      nom   : imDest
* 	      type  : PPIMAGE_UCHAR
* 	      role  : Pointeur vers l'Image destination ou NULL si a creer
* 
* FICHIER:     IMAGETTE.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

#define FF4(typeima)											\
    	for(i=0;i<IdImaDimY(imS);i++)									\
       	for(j=0;j<IdImaDimX(imS);j++)									\
  	   imDouble[i][j]=fabs((double)((typeima)imS)[i][j]);


PPIMAGE_UCHAR IdImaRecadTo8ABS(
	PPIMAGE imS,
	PPIMAGE_UCHAR imD)
{
double mini,maxi;
double MM;

int i,j;

imD=(PPIMAGE_UCHAR)IdImaCheckSizeAllocType (imS,-1,(PPIMAGE)imD,IMA_UCHAR);

    	      if (!(imD))
 	         { 	IdErrPrintf (" echec Alloc Image 8bits \n");
			IdErrno=IDERR_ALLOC_IMA;
   	 		return 0;
	   	 }

imDouble=(PPIMAGE_DOUBLE)IdImaAlloc(IdImaDimX(imS),IdImaDimY(imS),IMA_DOUBLE);

    	      if (!(imDouble))
 	         { 	IdErrPrintf (" echec Alloc Image Double\n");
			IdErrno=IDERR_ALLOC_IMA;
   	 		return 0;
	   	 }

  switch(IdImaType(imS))
    {
     case IMA_CHAR:   FF4(PPIMAGE_CHAR);   break;  
     case IMA_UCHAR:  FF4(PPIMAGE_UCHAR);  break;  
     case IMA_SHORT:  FF4(PPIMAGE_SHORT);  break;  
     case IMA_USHORT: FF4(PPIMAGE_USHORT); break;  
     case IMA_LONG:   FF4(PPIMAGE_LONG);   break; 
     case IMA_ULONG:  FF4(PPIMAGE_ULONG);  break;  
     case IMA_FLOAT:  FF4(PPIMAGE_FLOAT);  break;  
     case IMA_DOUBLE: FF4(PPIMAGE_DOUBLE); break;  
    
     default:
	IdErrPrintf("Type %d non traitable par la fonction IdImaRecadTo8ABS !\n", IdImaType(imS));
	IdErrno=IDERR_WRONG_LIBTYPE;
	return(0);
    }

	maxi= - FLT_MAX;
	mini=   FLT_MAX;

	for(i=0;i<IdImaDimY(imS);i++)
	 for(j=0;j<IdImaDimX(imS);j++)
 	  { if(((imDouble)[i][j]) > maxi) maxi=((imDouble)[i][j]);
   	    if(((imDouble)[i][j]) < mini) mini=((imDouble)[i][j]);
	  }

 /*               Recadrage de l'image       */
			
	MM= maxi-mini;
	if(MM==0.)   
 		{ IdErrPrintf (" Min = Max  = %g; Impossible recadrer \n",maxi);
			IdErrno=IDERR_MIN_EQ_MAX;
   	 		return 0;
	   	 }
 
	for(i=0;i<IdImaDimY(imS);i++)
 	for(j=0;j<IdImaDimX(imS);j++)
  		{
		imD[i][j]=(unsigned char) ( ( (((imDouble)[i][j])-mini)*255 ) /MM );
  		}
 
IdImaFree(imDouble);

return (imD);
}

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaRecad16To8   (fonction)
* 
* RESUME: recadre une image 16 bits ds une image 8 bits
* 
* DESCRIPTION: recadre une image 16 bits ds une image 8 bits
* 
* SYNTAXE:    PPIMAGE_UCHAR im8= IdImaRecad16To8 (PPIMAGE_USHORT im16, int minImpose, int maxImpose);
* 
* RETOUR    :  type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers l'image destination
* 
* PARAMETRES:  nom   : im16
* 	     type  : PPIMAGE_USHORT
* 	     role  : Pointeur vers l'image source
* 
* 	     nom   : minImpose, maxImpose 
*              type  : int
* 	     role  : min et max imposes pour le recadrage 
* 		   : sans effet si -1,-1
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

PPIMAGE_UCHAR IdImaRecad16To8(
	PPIMAGE_USHORT im16,
	int minImpose,
	int maxImpose)
{
PPIMAGE_UCHAR im8;
unsigned short mini,maxi;
float Fmult,MM;
int i,j;
if (!im16) {IdErrPrintf("erreur image 16 bits \n"); return(0);}

im8=(PPIMAGE_UCHAR)IdImaAlloc(IdImaDimX(im16),IdImaDimY(im16),IMA_UCHAR);
    	      if (!(im8))
 	         { 	IdErrPrintf (" echec Alloc image 8bits \n");
   	 		return 0;
	   	 }
if ( (minImpose!=-1) && (maxImpose!=-1) )
	{
	mini=minImpose;		/* imposes; on les prend */
	maxi=maxImpose;

    for(i=0;i<IdImaDimY(im16);i++)    /* on remonte a Min tt ce qui est < Min */
       for(j=0;j<IdImaDimY(im16);j++) /* on descend a Max tt ce qui est > Max */
        { if     ((int)im16[i][j]>maxi)im16[i][j]=maxi;
          else if((int)im16[i][j]<mini)im16[i][j]=mini;
        }

/*IdPrintf (" min max imposes %d %d\n", minImpose,maxImpose);*/
	}
 else
	{
	maxi=0;  		/* non imposes, on les cherche */
	mini=0xffff;
	for(i=0;i<IdImaDimY(im16);i++)
	 for(j=0;j<IdImaDimX(im16);j++)	
 	  { if(im16[i][j] > maxi) maxi=im16[i][j];
   	    if(im16[i][j] < mini) mini=im16[i][j];
	  }
/*IdPrintf (" min max calcules %d %d\n", mini,maxi);*/
   	}
 /*               Recadrage de l'image       */ 

													
  MM= maxi-mini;											
  if(MM==0.)											
   { 	IdErrPrintf (" Min = Max = %g; Impossible recadrer \n",mini);				
	IdErrno=IDERR_MIN_EQ_MAX;
	return 0;									
   }											
													
     

for(i=0;i<IdImaDimY(im16);i++)
 for(j=0;j<IdImaDimX(im16);j++)	
  { Fmult=(float)((im16[i][j]-mini)*255);
    im8[i][j]=(unsigned char)(Fmult/MM);
  }

IdSetFileName(im8,IdGetFileName(im16));

return (im8);
}

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaRecad16To8IgnoreUpper   (fonction)
* 
* RESUME: recadre une image 16 bits ds une image 8 bits  en IGNORANT les pixels > val donnee
* 
* DESCRIPTION: recadre une image 16 bits ds une image 8 bits en 
* 		IGNORANT les pixels > val donnee
* 		Les pixels >= valSup seront mis a 255 lors du recadrage,
* 		qui se fera sur [min,max] des pixels restants
* 
* SYNTAXE:    PPIMAGE_UCHAR im8= 
* IdImaRecad16To8IgnoreUpper (PPIMAGE_USHORT im16, int minImpose, int maxImpose, int valSup);
* 
* RETOUR    :  type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers l'image destination
* 
* PARAMETRES:  nom   : im16
* 	     type  : PPIMAGE_USHORT
* 	     role  : Pointeur vers l'image source
* 
* 	     nom   : minImpose, maxImpose 
*              type  : int
* 	     role  : min et max imposes pour le recadrage 
* 		   : sans effet si -1,-1
* 
* 	     nom   : valSup 
*              type  : int
* 	     role  : les pixels >= valSup seront mis a 255 lors du recadrage,
* 			qui se fera sur [min,max] des pixels restants
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

PPIMAGE_UCHAR IdImaRecad16To8IgnoreUpper(
	PPIMAGE_USHORT im16,
	int minImpose,
	int maxImpose,
	int valSup)
{
PPIMAGE_UCHAR im8;
unsigned short mini,maxi;
float Fmult,MM;
int i,j;
if (!im16) {IdErrPrintf("erreur image 16 bits \n"); return(0);}

im8=(PPIMAGE_UCHAR)IdImaAlloc(IdImaDimX(im16),IdImaDimY(im16),IMA_UCHAR);
    	      if (!(im8))
 	         { 	IdErrPrintf (" echec Alloc image 8bits \n");
   	 		return 0;
	   	 }
if ( (minImpose!=-1) && (maxImpose!=-1) )
	{
	mini=minImpose;		/* imposes; on les prend */
	maxi=maxImpose;

    for(i=0;i<IdImaDimY(im16);i++)    /* on remonte a Min tt ce qui est < Min */
       for(j=0;j<IdImaDimY(im16);j++) /* on descend a Max tt ce qui est > Max */
        { 
	if     ((int)im16[i][j]>=valSup) continue;
	if     ((int)im16[i][j]>maxi)im16[i][j]=maxi;
          else if((int)im16[i][j]<mini)im16[i][j]=mini;
        }

	}
 else
	{
	maxi=0;  		/* non imposes, on les cherche */
	mini=0xffff;
	for(i=0;i<IdImaDimY(im16);i++)
	 for(j=0;j<IdImaDimX(im16);j++)	
 	  { 
	    if((int)im16[i][j]>=valSup) continue;
	    if(im16[i][j] > maxi) maxi=im16[i][j];
   	    if(im16[i][j] < mini) mini=im16[i][j];
	  }
   	}
 /*               Recadrage de l'image       */      


													
	MM= maxi-mini;											
	if(MM==0.)
 	         { 	IdErrPrintf (" Min = Max; Impossible recadrer \n");				
			IdErrno=IDERR_MIN_EQ_MAX;							
   	 		return 0;									
	   	 }
				

for(i=0;i<IdImaDimY(im16);i++)
 for(j=0;j<IdImaDimX(im16);j++)	
  { 
    if((int)im16[i][j]>=valSup) { im8[i][j]=255; continue; }
    Fmult=(float)((im16[i][j]-mini)*255);
    im8[i][j]=(unsigned char)(Fmult/MM);
  }

IdSetFileName(im8,IdGetFileName(im16));

return (im8);
}


/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaRecad16To16IgnoreUpper   (fonction)
* 
* RESUME: recadre une image 16 bits ds une image 16 bits  en IGNORANT les pixels > val donnee
* 
* DESCRIPTION: recadre une image 16 bits ds une image 16 bits  (!!!)en 
* 		IGNORANT les pixels > val donnee
* 		Les pixels >= valSup seront mis a 4095 lors du recadrage,
* 		qui se fera sur [min,max] des pixels restants
* 
* SYNTAXE:    PPIMAGE_USHORT imd16= 
* IdImaRecad16To16IgnoreUpper (PPIMAGE_USHORT ims16, int minImpose, int maxImpose, int valSup);
* 
* RETOUR    :  type  : PPIMAGE_USHORT
* 	     role  : Pointeur vers l'image destination
* 
* PARAMETRES:  nom   : ims16
* 	     type  : PPIMAGE_USHORT
* 	     role  : Pointeur vers l'image source
* 
* 	     nom   : minImpose, maxImpose 
*              type  : int
* 	     role  : min et max imposes pour le recadrage 
* 		   : sans effet si -1,-1
* 
* 	     nom   : valSup 
*              type  : int
* 	     role  : les pixels >= valSup seront mis a 4095 lors du recadrage,
* 			qui se fera sur [min,max] des pixels restants
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

PPIMAGE_USHORT IdImaRecad16To16IgnoreUpper(
	PPIMAGE_USHORT im16,
	int minImpose,
	int maxImpose,
	int valSup)
{
PPIMAGE_USHORT im8;
unsigned short mini,maxi;
float Fmult,MM;
int i,j;
if (!im16) {IdErrPrintf("erreur image 16 bits \n"); return(0);}

/* ATTENTION AUX COMMENTAIRES : im8 est USHORT ... */

im8=(PPIMAGE_USHORT)IdImaAlloc(IdImaDimX(im16),IdImaDimY(im16),IMA_USHORT);
    	      if (!(im8))
 	         { 	IdErrPrintf (" echec Alloc image 16 bits \n");
   	 		return 0;
	   	 }
if ( (minImpose!=-1) && (maxImpose!=-1) )
	{
	mini=minImpose;		/* imposes; on les prend */
	maxi=maxImpose;

    for(i=0;i<IdImaDimY(im16);i++)    /* on remonte a Min tt ce qui est < Min */
       for(j=0;j<IdImaDimY(im16);j++) /* on descend a Max tt ce qui est > Max */
        { 
	if       ((int)im16[i][j]>=valSup)	continue;
	if       ((int)im16[i][j]>maxi)		im16[i][j]=maxi;
          else if((int)im16[i][j]<mini)		im16[i][j]=mini;
        }

	}
 else
	{
	maxi=0;  		/* non imposes, on les cherche */
	mini=0xffff;
	for(i=0;i<IdImaDimY(im16);i++)
	 for(j=0;j<IdImaDimX(im16);j++)	
 	  { 
	    if((int)im16[i][j]>=valSup) continue;
	    if(im16[i][j] > maxi) maxi=im16[i][j];
   	    if(im16[i][j] < mini) mini=im16[i][j];
	  }
   	}
 /*               Recadrage de l'image       */
													
	MM= maxi-mini;											
	if(MM==0.)
 	         { 	IdErrPrintf (" Min = Max; Impossible recadrer \n");				
			IdErrno=IDERR_MIN_EQ_MAX;							
   	 		return 0;									
	   	 }
				

for(i=0;i<IdImaDimY(im16);i++)
 for(j=0;j<IdImaDimX(im16);j++)	
  { 
    if((int)im16[i][j]>=valSup) { im8[i][j]=4095; continue; }
    Fmult=(float)((im16[i][j]-mini)*4095);
    im8[i][j]=(unsigned short)(Fmult/MM);
  }

IdSetFileName(im8,IdGetFileName(im16));

return (im8);
}

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaRecadTo16   (fonction)
* 
* RESUME: Recadre (Dynamique) une image non COMPLEX ds une image 16 bits
* 
* DESCRIPTION: Recadre (Dynamique) une image non COMPLEX ds une image 16 bits
* 
* SYNTAXE:    PPIMAGE_USHORT imDest= IdImaRecadTo16 (PPIMAGE imSource, PPIMAGE_USHORT imDest, double minImpose, double maxImpose);
* 
* RETOUR    :  type  : PPIMAGE_USHORT
* 	     role  : ptr sur image recadree.
* 
* PARAMETRES:  nom   : imSource
* 	     type  : PPIMAGE
* 	     role  : Pointeur vers l'image source
* 
*  	     nom   : imDest
* 	     type  : PPIMAGE_USHORT
* 	     role  : Pointeur vers l' Image destination
* 
* 	     nom   : minImpose, maxImpose 
*              type  : double
* 	     role  : min et max imposes pour le recadrage 
* 		   : sans effet si -1.0,-1.0
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

#define FF6(typeima)											\
													\
if ( (minImpose!=-1.) && (maxImpose!=-1.) )								\
{													\
	mini=minImpose;		/* imposes; on les prend */						\
	maxi=maxImpose;											\
													\
	MM= maxi-mini;											\
	if(MM==0.)											\
 	         { 	IdErrPrintf (" Min = Max  = %g; Impossible recadrer \n",maxi);			\
			IdErrno=IDERR_MIN_EQ_MAX;							\
   	 		return 0;									\
	   	 }											\
													\
    	for(i=0;i<IdImaDimY(imS);i++)									\
       	for(j=0;j<IdImaDimX(imS);j++)									\
        { 												\
	pixel = (double)(((typeima)imS)[i][j]);								\
	if     (pixel>maxi) imD[i][j]=32767;								\
	else if(pixel<mini) imD[i][j]=0;								\
	  else {											\
    	  	imD[i][j]=(unsigned short) ( (double)( (pixel-mini)*32767. ) /MM );			\
		}											\
        }												\
}													\
 else													\
{													\
	maxi= - FLT_MAX;  		/* non imposes, on les cherche */				\
	mini=   FLT_MAX;										\
	for(i=0;i<IdImaDimY(imS);i++)									\
	 for(j=0;j<IdImaDimX(imS);j++)									\
 	  { if((((typeima)imS)[i][j]) > maxi) maxi=(((typeima)imS)[i][j]);				\
   	    if((((typeima)imS)[i][j]) < mini) mini=(((typeima)imS)[i][j]);				\
	  }												\
													\
 /*               Recadrage de l'image       */								\
	MM=maxi-mini;											\
	if(MM==0.)											\
 	         { 	IdErrPrintf (" Min = Max = %g; Impossible recadrer \n",mini);			\
			IdErrno=IDERR_MIN_EQ_MAX;							\
   	 		return 0;									\
	   	 }											\
 													\
	for(i=0;i<IdImaDimY(imS);i++)									\
 	for(j=0;j<IdImaDimX(imS);j++)									\
  		{											\
		imD[i][j]=(unsigned short) ( (double)( ((((typeima)imS)[i][j])-mini)*32767. ) /MM );	\
  		}											\
}


PPIMAGE_USHORT IdImaRecadTo16(
	PPIMAGE imS,
	PPIMAGE_USHORT imD,
	double minImpose,
	double maxImpose)
{
double mini,maxi;
double MM;
double pixel; 

int i,j;

imD=(PPIMAGE_USHORT)IdImaCheckSizeAllocType (imS,-1,(PPIMAGE)imD,IMA_USHORT);

    	      if (!(imD))
 	         { 	IdErrPrintf (" echec Alloc Image 16bits \n");
			IdErrno=IDERR_ALLOC_IMA;
   	 		return 0;
	   	 }

  switch(IdImaType(imS))
    {
     case IMA_CHAR:   FF6(PPIMAGE_CHAR);   break;  
     case IMA_UCHAR:  FF6(PPIMAGE_UCHAR);  break;  
     case IMA_SHORT:  FF6(PPIMAGE_SHORT);  break;  
     case IMA_USHORT: FF6(PPIMAGE_USHORT); break;  
     case IMA_LONG:   FF6(PPIMAGE_LONG);   break; 
     case IMA_ULONG:  FF6(PPIMAGE_ULONG);  break;   
     case IMA_FLOAT:  FF6(PPIMAGE_FLOAT);  break;  
     case IMA_DOUBLE: FF6(PPIMAGE_DOUBLE); break;  
     default:
	IdErrPrintf("Type %d non traitable par la fonction IdImaRecadTo16 !\n", IdImaType(imS));
	IdErrno=IDERR_WRONG_LIBTYPE;
	return(0);
    } 

IdSetFileName(imD,IdGetFileName(imS));
return (imD);
}


/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaConcat   (fonction)
* 
* RESUME: concatenne 2 im. 8 bits ds une image 8 bits SANS duplication de pixels
* 
* DESCRIPTION: concatenne 2 im. 8 bits ds une image 8 bits SANS duplication de pixels. 
* 	Si l'une des 2 images est = NULL la fonction retourne un pointeur sur l'autre.
* 
* SYNTAXE:    PPIMAGE_UCHAR imRes= IdImaConcat (PPIMAGE_UCHAR im1,PPIMAGE_UCHAR im2);
* 
* RETOUR    :  type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers les images destination
* 
* PARAMETRES:  nom   : im1, im2
* 	     type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers les images source
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

PPIMAGE_UCHAR IdImaConcat(
	PPIMAGE_UCHAR im1,
	PPIMAGE_UCHAR im2)
{
int nbLigIm1=0,nbLigIm2=0,nbLigIm3,nbColIm1,nbColIm2;
int j,y,j2;
  PRIVATE_IMAGE * si;
  PPIMAGE_UCHAR im3=NULL;
if(im1)
  	{	if (( IdLibidoType(im1) != IMA) )

		{    IdErrno = IDERR_WRONG_TYPE;
    	             return(PPIMAGE_UCHAR)0;
		}
		nbLigIm1=IdImaDimY(im1);
		nbColIm1=IdImaDimX(im1);
	}
	else 	nbColIm1=0;

if(im2)
  	{	if (( IdLibidoType(im2) != IMA) )

		{    IdErrno = IDERR_WRONG_TYPE;
    	             return(PPIMAGE_UCHAR)0;
		}
		nbLigIm2=IdImaDimY(im2);
		nbColIm2=IdImaDimX(im2);

	}
	else 	nbColIm2=0;

if ( !im1 && !im2 ) return (PPIMAGE_UCHAR)0;
if ( im1 && im2 )
{
	if (nbColIm1!=nbColIm2) 
		{	IdErrPrintf("erreur taille images\n");
 			return(PPIMAGE_UCHAR)0;
		}
	if (IdImaType(im1)!=IdImaType(im2))
		{	IdErrPrintf("erreur: types images #\n");
			return(PPIMAGE_UCHAR)0;
		}
}

 nbLigIm3=nbLigIm1+nbLigIm2;

 si=(PRIVATE_IMAGE *)calloc(1, sizeof(PRIVATE_IMAGE) + nbLigIm3*sizeof(void *));
  if(si){
     	if(im1) si->DimX=IdImaDimX(im1); else si->DimX=IdImaDimX(im2);
    	si->DimY=nbLigIm3;
     	if(im1) si->Type=IdImaType(im1); else si->Type=IdImaType(im2);
    	im3 = (PPIMAGE_UCHAR)&(si[1]);

	if (im1)
		{
    		for ( j=0 ;j<nbLigIm1; j++ )
      			im3[j]= im1[j];
   		j2=j;
		}
		else j2=0;
	if (im2)
		{
   		for ( y=0, j=j2; y<nbLigIm2; y++, j++ )
      			im3[j]= im2[y];
       		}
	}
	else IdErrPrintf("echec alloc im3\n");
return (im3);

}

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaCreImagette   (fonction)
* 
* RESUME: cree une imagette (ligne) a partir d'un tableau de pointeurs sur images
* 
* DESCRIPTION:  cree une imagette a partir d'un tableau de pointeurs sur images
* 
* 
* SYNTAXE:    PPIMAGE_UCHAR im8= IdImaCreImagette ( PPIMAGE_UCHAR tabImages[], int nbImages,int tailleX , int tailleY);
* 
* RETOUR    :  type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers le tableau d'images destination
* 
* PARAMETRES:  nom   : tabImages 
* 	     type  : PPIMAGE_UCHAR *
* 	     role  : Pointeur vers le tableau d'images source
* 
* 	     nom   : nbImages 
* 	     type  : int 
* 	     role  : nb d'images ds le tableau
* 
* 	     nom   : tailleX 
* 	     type  : int 
* 	     role  : taille en X des images sous-echantillonnees
* 
* 	     nom   : tailleY 
* 	     type  : int 
* 	     role  : taille en Y des images sous-echantillonnees
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

PPIMAGE_UCHAR IdImaCreImagette(
	PPIMAGE_UCHAR *TabImage,
	int n,
	int nx,
	int ny)
{
PPIMAGE_UCHAR imgtte;
PPIMAGE_UCHAR imaint;
int i,j,l;
int TAILLE_X = nx;
int TAILLE_Y = ny;
imgtte=(PPIMAGE_UCHAR)IdImaAlloc(TAILLE_X*n,TAILLE_Y,TY_UCHAR);
if(!imgtte) {IdErrPrintf("echec allocation imagette\n"); return (NULL);}

imaint=(PPIMAGE_UCHAR)IdImaAlloc(TAILLE_X,TAILLE_Y,TY_UCHAR);
if(!imaint) {IdErrPrintf("echec allocation image reduite\n"); return (NULL);}

for (l=0;l<n;l++)
	{ 	IdImaZoom((PPIMAGE)imaint,(PPIMAGE)(TabImage[l]));

	for (i=0;i<IdImaDimY(imaint);i++)	
	  for (j=0;j<IdImaDimX(imaint);j++)
	  {  	imgtte[i][l*IdImaDimX(imaint)+j]=imaint[i][j];
	  }
        }
IdImaFree(imaint);	
return(imgtte);
}

/* ------------------------------------------------------------------------*/
PPIMAGE_UCHAR _IdImaCreImagetteClear(
	PPIMAGE_UCHAR *TabImage,
	int n,		// Number of allocated positions
	int neffectif,	// Number of images to be stored
	int nx,
	int ny)
{
PPIMAGE_UCHAR imgtte;
PPIMAGE_UCHAR imaint;
int i,j,l;
int TAILLE_X = nx;
int TAILLE_Y = ny;
imgtte=(PPIMAGE_UCHAR)IdImaAlloc(TAILLE_X*n,TAILLE_Y,TY_UCHAR);
if(!imgtte) {IdErrPrintf("echec allocation imagette\n"); return (NULL);}
IdImaClear((PPIMAGE)imgtte);

imaint=(PPIMAGE_UCHAR)IdImaAlloc(TAILLE_X,TAILLE_Y,TY_UCHAR);
if(!imaint) {IdErrPrintf("echec allocation image reduite\n"); return (NULL);}

for (l=0;l<neffectif;l++)
	{ 	IdImaZoom((PPIMAGE)imaint,(PPIMAGE)(TabImage[l]));


	for (i=0;i<IdImaDimY(imaint);i++)	
	  for (j=0;j<IdImaDimX(imaint);j++)
	  {  	imgtte[i][l*IdImaDimX(imaint)+j]=imaint[i][j];
	  }

        }
IdImaFree(imaint);
return(imgtte);
}

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaCreImagetteXY   (fonction)
* 
* RESUME: cree une imagette(matrice) a partir d'un tableau de pointeurs sur images
* 
* DESCRIPTION:  cree une imagette a partir d'un tableau de pointeurs sur images
* 
* SYNTAXE:    PPIMAGE_UCHAR im8= IdImaCreeImagetteXY (PPIMAGE_UCHAR tabImages[] , int nbImages, int nbImLig, int nbImCol, int Xsize, int Ysize);
* 
* PARAMETRES:  nom   : tabImages 
* 	     type  : PPIMAGE_UCHAR *
* 	     role  : Pointeur vers le tableau d'images source
* 
* 	     nom   : nbImages 
* 	     type  : int 
* 	     role  : nb d'images ds le tableau
* 
* 	     nom   : nbImLig 
* 	     type  : int 
* 	     role  : nombre d'imagettes par LIGNE (si -1, on ajuste)
* 
* 	     nom   : nbImCol 
* 	     type  : int 
* 	     role  :  nombre d'imagettes par COLONNE (si -1, on ajuste)
* 
* 	     nom   : Xsize 
* 	     type  : int 
* 	     role  : taille en X des images sous-echantillonnees
* 
* 	     nom   : Ysize 
* 	     type  : int 
* 	     role  :  taille en Y des images sous-echantillonnees
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

PPIMAGE_UCHAR IdImaCreImagetteXY (
	PPIMAGE_UCHAR *tabimage,
	int nb,		// Number of images
	int XSIZE,	// X-size of reduced images
	int YSIZE,	// Y-size of reduced images
	int nx,		// Number of images within each line
	int ny)		// Number of images within each column

{
PPIMAGE_UCHAR imagettes=0;       /* image finale a retourner          */
PPIMAGE_UCHAR ligne_imagettes=0; /* im auxiliaire lignes d'imagettes  */
int  m,k,FLAG_COL=0;

     if ( (nx!=-1)&&(ny!=-1) )  if (nb>(nx*ny))   nx=(int)sqrt((double)nb);

     if ( (nx==-1)&&(ny==-1) ) { nx=nb; ny=1; FLAG_COL=1;}
else if ( (ny==-1)&&(nx!=-1) ) { ny=(int)ceil( ((double)nb/(double)nx) ); }
else if ( (nx==-1)&&(ny!=-1) ) { nx=(int)ceil( ((double)nb/(double)ny) ); }


if(nb<nx) k=nb; else k=nx;

ligne_imagettes =(PPIMAGE_UCHAR) IdImaCreImagette (&tabimage[0], k, XSIZE, YSIZE);

if (FLAG_COL)   return(ligne_imagettes);
if (k==nb) return(ligne_imagettes);

imagettes=ligne_imagettes;

FLAG_COL=0;
for (m=nx; m<nb; m+=nx)
{
if ((m+nx)>nb){ FLAG_COL=1; k=nx -(nx*ny-nb) ;break;}

	       	ligne_imagettes =(PPIMAGE_UCHAR) IdImaCreImagette (&tabimage[m], nx, XSIZE, YSIZE);
		imagettes=(PPIMAGE_UCHAR)IdImaConcat(imagettes,ligne_imagettes);

		}

if (FLAG_COL)
		{
	       	ligne_imagettes =(PPIMAGE_UCHAR) _IdImaCreImagetteClear (&tabimage[m], nx,k, XSIZE, YSIZE);
		imagettes=(PPIMAGE_UCHAR)IdImaConcat(imagettes,ligne_imagettes);
		}

return imagettes;
}

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaMakeImagetteXYHoriz   (fonction)
* 
* RESUME: Colle ds une matrice (G->D, H->B) des images les unes a la suite des autres
* 
* DESCRIPTION:  Cree une imagette (matrice) a partir d'un tableau de pointeurs sur images.
* 		Les images sont 'rangees' de gauche a droite, puis de haut en bas.
* 
* SYNTAXE:    PPIMAGE_UCHAR im8= IdImaMakeImagetteXYHoriz (PPIMAGE_UCHAR tabImages[] , int nbImages, int nbImCol, int nbImLig, int Xsize, int Ysize);
* 
* PARAMETRES:  nom   : tabImages 
* 	     type  : PPIMAGE_UCHAR *
* 	     role  : Pointeur vers le tableau d'images source
* 
* 	     nom   : nbImages 
* 	     type  : int 
* 	     role  : nb d'images ds le tableau
* 
* 	     nom   : nbImCol
* 	     type  : int 
* 	     role  : nombre d'imagettes (en X) par COLONNE (si -1, on ajuste)
* 
* 	     nom   : nbImLig
* 	     type  : int 
* 	     role  : nombre d'imagettes (en Y) par LIGNE (si -1, on ajuste)
* 
* 	     nom   : Xsize 
* 	     type  : int 
* 	     role  : taille en X des images sous-echantillonnees
* 
* 	     nom   : Ysize 
* 	     type  : int 
* 	     role  :  taille en Y des images sous-echantillonnees
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

PPIMAGE_UCHAR IdImaMakeImagetteXYHoriz (
	PPIMAGE_UCHAR *tabimage,
	int nb,		// Number of images
	int XSIZE,	// X-size of reduced images
	int YSIZE,	// Y-size of reduced images
	int nx,		// Number of images within each line
	int ny)		// Number of images within each column

{
PPIMAGE_UCHAR imagettes=NULL;       /* image finale a retourner          */
PPIMAGE_UCHAR imin=NULL,imout=NULL;

int  k;
int i,j,l;

int nlp;	/* Nb de lignes pleines */

float rapportx, rapporty;

     if ( (nx!=-1)&&(ny!=-1) )  if (nb>(nx*ny))  
				{  nx=ny=(int)ceil(sqrt((double)nb));
				   if (nx*(ny-1)>=nb) ny--;
				}

     if ( (nx==-1)&&(ny==-1) ) { nx=ny=(int)ceil(sqrt((double)nb));
				if (nx*(ny-1)>=nb) ny--;
				}
else if ( (ny==-1)&&(nx!=-1) ) { ny=(int)ceil( ((double)nb/(double)nx) ); 
				}
else if ( (nx==-1)&&(ny!=-1) ) { nx=(int)ceil( ((double)nb/(double)ny) );  
				}

if(nb<nx) k=nb; else k=nx;


imagettes= (PPIMAGE_UCHAR)IdImaAlloc(nx*XSIZE, ny*YSIZE, IMA_UCHAR);

if (!imagettes)	{ IdErrPrintf("Echec Alloc imagettes\n");
		 IdErrno=IDERR_ALLOC_IMA;
		 return (PPIMAGE_UCHAR)NULL;
		}

if (nb<(nx*ny)) 
	{ nlp=nb/nx;
	 imout=(PPIMAGE_UCHAR) IdImaAllocSubImage((PPIMAGE) imagettes, 0,nlp , nx*XSIZE, (nx-nlp)*YSIZE);
 	IdImaClear((PPIMAGE)imout);
	IdImaFreeSubImage(imout);
	}

imout=(PPIMAGE_UCHAR) IdImaAllocSubImage((PPIMAGE) imagettes, 0,0 , XSIZE, YSIZE);

if (!imout) {	IdErrPrintf("Echec Alloc sous-im\n"); 
		IdErrno=IDERR_ALLOC_IMA;
		return (PPIMAGE_UCHAR)NULL;
		}

for (l=0; l<nb; l++)  /* pour chaque image de la table */
{
imin = tabimage[l];
rapportx=((float)(IdImaDimX(imin)))/XSIZE;
rapporty=((float)(IdImaDimY(imin)))/YSIZE;

imout=(PPIMAGE_UCHAR) IdImaModifSubImage( (PPIMAGE)imagettes, (PPIMAGE)imout, (l%nx)*XSIZE, (l/nx)*YSIZE);

if (!imout) {	IdErrPrintf("Echec Modif sous-im\n"); 
		IdErrno=IDERR_ALLOC_IMA;
		return (PPIMAGE_UCHAR)NULL;
		}
                                       
	for (j=0; j<IdImaDimY (imout); j++)
           for (i=0; i<IdImaDimX (imout); i++)                                             
	{
	imout[j][i]=((PPIMAGE_UCHAR)imin)[(int)(j*rapporty)][(int)(i*rapportx)];
	}
}

IdImaFreeSubImage(imout);
return imagettes;
}

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaMakeImagetteXYVertic   (fonction)
* 
* RESUME: Colle ds une matrice ( H->B, G->D) des images les unes a la suite des autres
* 
* DESCRIPTION:  Cree une imagette (matrice) a partir d'un tableau de pointeurs sur images.
* 		Les images sont 'rangees' de haut en bas , puis de gauche a droite.
* 
* SYNTAXE:    PPIMAGE_UCHAR im8= IdImaMakeImagetteXYVertic (PPIMAGE_UCHAR tabImages[] , int nbImages, int nbImCol,int nbImLig,  int Xsize, int Ysize);
* 
* PARAMETRES:  nom   : tabImages 
* 	     type  : PPIMAGE_UCHAR *
* 	     role  : Pointeur vers le tableau d'images source
* 
* 	     nom   : nbImages 
* 	     type  : int 
* 	     role  : nb d'images ds le tableau
* 
* 	     nom   : nbImCol
* 	     type  : int 
* 	     role  : nombre d'imagettes (en X) par COLONNE (si -1, on ajuste)
* 
* 	     nom   :  nbImLig
* 	     type  : int 
* 	     role  :  nombre d'imagettes (en Y) par LIGNE (si -1, on ajuste)
* 
* 	     nom   : Xsize 
* 	     type  : int 
* 	     role  : taille en X des images sous-echantillonnees
* 
* 	     nom   : Ysize 
* 	     type  : int 
* 	     role  :  taille en Y des images sous-echantillonnees
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

PPIMAGE_UCHAR IdImaMakeImagetteXYVertic (
	PPIMAGE_UCHAR *tabimage,
	int nb,		// Number of images
	int XSIZE,	// X-size of reduced images
	int YSIZE,	// Y-size of reduced images
	int nx,		// Number of images within each line
	int ny)		// Number of images within each column
{
PPIMAGE_UCHAR imagettes=NULL;       /* image finale a retourner          */
PPIMAGE_UCHAR imin=NULL,imout=NULL;

int  k;
int i,j,l;

int nlp;	/* Nb de lignes pleines */

  float rapportx, rapporty;

     if ( (nx!=-1)&&(ny!=-1) )  if (nb>(nx*ny))  
				{  nx=ny=(int)ceil(sqrt((double)nb));
				  if (nx*(ny-1)>=nb) ny--;
				}

     if ( (nx==-1)&&(ny==-1) ) 	{ nx=ny=(int)ceil(sqrt((double)nb));
					if (nx*(ny-1)>=nb) ny--;
				}
else if ( (ny==-1)&&(nx!=-1) ) { ny=(int)ceil( ((double)nb/(double)nx) ); }
else if ( (nx==-1)&&(ny!=-1) ) { nx=(int)ceil( ((double)nb/(double)ny) ); }


if(nb<nx) k=nb; else k=nx;

imagettes= (PPIMAGE_UCHAR)IdImaAlloc(nx*XSIZE, ny*YSIZE, IMA_UCHAR);
if (!imagettes)	{
		 IdErrPrintf("Echec Alloc Imagettes\n");
		 IdErrno=IDERR_ALLOC_IMA;
		 return (PPIMAGE_UCHAR)NULL;
		}

if (nb<(nx*ny)) 
	{ nlp=nb/nx;
	 imout=(PPIMAGE_UCHAR) IdImaAllocSubImage((PPIMAGE) imagettes, 0,nlp , nx*XSIZE, (nx-nlp)*YSIZE);
 	IdImaClear((PPIMAGE)imout);
	IdImaFreeSubImage(imout);
	}

imout=(PPIMAGE_UCHAR) IdImaAllocSubImage( (PPIMAGE)imagettes, 0,0 , XSIZE, YSIZE);

if (!imagettes)	{
		 IdErrPrintf("Echec Alloc sous-im\n");
		 IdErrno=IDERR_ALLOC_IMA;
		 return (PPIMAGE_UCHAR)NULL;
		}

for (l=0; l<nb; l++)  /* pour chaque image de la table */
{
imin = tabimage[l];
rapportx=((float)(IdImaDimX(imin)))/XSIZE;
rapporty=((float)(IdImaDimY(imin)))/YSIZE;

imout=(PPIMAGE_UCHAR) IdImaModifSubImage((PPIMAGE)imagettes, (PPIMAGE)imout, (l/ny)*XSIZE, (l%ny)*YSIZE);

if (!imout) 	{ IdErrPrintf("Echec Modif sous-im\n"); 
		 IdErrno=IDERR_ALLOC_IMA;
		 return (PPIMAGE_UCHAR)NULL;
		}
                                       
	for (j=0; j<IdImaDimY (imout); j++)                                         
           for (i=0; i<IdImaDimX (imout); i++)                                             
	{
	imout[j][i]=((PPIMAGE_UCHAR)imin)[(int)(j*rapporty)][(int)(i*rapportx)];
	}
}

IdImaFreeSubImage(imout);
return imagettes;
}


/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaScotchImages   (fonction)
* 
* RESUME: Colle en ligne des images les unes a la suite des autres
* 
* DESCRIPTION:  Colle en ligne des images (de MEMES DIMENSIONS) les unes a la suite des autres
* 	
* SYNTAXE:    PPIMAGE_USHORT im8= IdImaScotchImages (PPIMAGE_USHORT tabImages[],int nbImages);
* 
* PARAMETRES:  nom   : tabImages 
* 	     type  : PPIMAGE_USHORT *
* 	     role  : Pointeur vers le tableau d'images source
* 
* 	     nom   : nbImages 
* 	     type  : int 
* 	     role  : nb d'images ds le tableau
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

PPIMAGE_UCHAR IdImaScotchImages(
	PPIMAGE_UCHAR *TabImage,
	int n)
{
PPIMAGE_UCHAR imagette;
PPIMAGE_UCHAR imaint;
int i,j,l,l1,NX,NY;

NX=IdImaDimX(TabImage[0]);
NY=IdImaDimY(TabImage[0]);

for (l=1;l<n;l++)
	{ 
        if(TabImage[l]==0)  { IdErrPrintf("erreur images : pointeur NULL\n");
			      return (0);
			    }
	if(IdImaDimY(TabImage[l])!=NY)  { IdErrPrintf("erreur tailles images\n");
					 return (0);
					}
	}

imagette=(PPIMAGE_UCHAR)IdImaAlloc(NX*n,NY,TY_UCHAR);
if(!imagette) {IdErrPrintf("echec allocation imagette\n"); return (NULL);}

l1=0;
for (l=0;l<n;l++)
	{ 
		imaint=TabImage[l];
		for (i=0;i<NY;i++)
       		{
	  		for (j=0;j<IdImaDimX(imaint);j++)
	  		{   			
	        	imagette[i][l1+j]=imaint[i][j];  
	  		}
		}
		l1=l1+IdImaDimX(imaint)*IdSizeOfType(imagette);	
        }
	
return(imagette);
}


/* FoNCTION DESCRIPTION **************************************************
* 
* IdImaScotchImagesXYHoriz   (fonction)
* 
* RESUME: Colle ds une matrice (G->D, H->B) des images les unes a la suite des autres
* 
* DESCRIPTION:  Colle des images (de MEME TAILLE) dans une matrice
* 	Les images sont 'rangees' de gauche a droite, puis de haut en bas.
* 
* SYNTAXE:    im8= IdImaScotchImagesXYHoriz (tabImages,nbImages,nbImCol,nbImLig);
* 
* PARAMETRES:  nom   : tabImages 
* 	     type  : PPIMAGE_USHORT *
* 	     role  : Pointeur vers le tableau d'images source
* 
* PARAMETRES:  nom   : nbImages 
* 	     type  : int 
* 	     role  : nb d'images ds le tableau
* 
* 	     nom   : nbImCol 
* 	     type  : int 
* 	     role  :  nombre d'imagettes par COLONNE (si -1, on ajuste)
* 
* 	     nom   : nbImLig 
* 	     type  : int 
* 	     role  : nombre d'imagettes par LIGNE (si -1, on ajuste)
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTIoN */

PPIMAGE_UCHAR IdImaScotchImagesXYHoriz (
	PPIMAGE_UCHAR *tabimage,
	int nb,		// nb nombres d'images
	int nx,		// nx nombre d'images par ligne
	int ny)		// ny nombre d'images par colonne

{
PPIMAGE_UCHAR matImages=NULL;       /* image finale a retourner          */

int XSIZE, YSIZE;

int l;




if (!tabimage)	{
		return 0;
		}

if (nb<=0)	{
		return 0;
		}

if (tabimage[0] ==0)	{
			return 0;
			}

XSIZE=IdImaDimX(tabimage[0]);
YSIZE=IdImaDimY(tabimage[0]);

for (l=1;l<nb;l++)
 {
	if (!IdImaSameSizeAndType(tabimage[0],tabimage[l])) 	
		{
		return 0;
		}
 }
matImages=IdImaMakeImagetteXYHoriz ( tabimage, nb, XSIZE, YSIZE, nx, ny);
return matImages;
}



/* FoNCTION DESCRIPTION **************************************************
* 
* IdImaScotchImagesXYVertic   (fonction)
* 
* RESUME: Colle ds une matrice (H->B, G->D ) des images les unes a la suite des autres
* 
* DESCRIPTION:  Colle des images (de MEME TAILLE) dans une matrice
* 	Les images sont 'rangees' de haut en bas, puis de gauche a droite.
* 
* SYNTAXE:    im8= IdImaScotchImagesXYVertic (tabImages,nbImages,nbImCol,nbImLig);
* 
* PARAMETRES:  nom   : tabImages 
* 	     type  : PPIMAGE_USHORT *
* 	     role  : Pointeur vers le tableau d'images source
* 
* PARAMETRES:  nom   : nbImages 
* 	     type  : int 
* 	     role  : nb d'images ds le tableau
* 
* 	     nom   : nbImCol 
* 	     type  : int 
* 	     role  :  nombre d'imagettes par COLONNE (si -1, on ajuste)
* 
* 	     nom   : nbImLig 
* 	     type  : int 
* 	     role  : nombre d'imagettes par LIGNE (si -1, on ajuste)
* 
* FICHIER:     imagette.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTIoN */


PPIMAGE_UCHAR IdImaScotchImagesXYVertic (
	PPIMAGE_UCHAR *tabimage,
	int nb,		// nb nombres d'images
	int nx,		// nx nombre d'images par ligne
	int ny)		// ny nombre d'images par colonne

{
PPIMAGE_UCHAR matImages=NULL;       /* image finale a retourner          */

int XSIZE, YSIZE;

int l;




if (!tabimage)	{
		return 0;
		}

if (nb<=0)	{
		return 0;
		}

if (tabimage[0] ==0)	{
			return 0;
			}

XSIZE=IdImaDimX(tabimage[0]);
YSIZE=IdImaDimY(tabimage[0]);

for (l=1;l<nb;l++)
 {
	if (!IdImaSameSizeAndType(tabimage[0],tabimage[l]))	
		{
		return 0;
		}
 }
matImages=IdImaMakeImagetteXYVertic ( tabimage, nb, XSIZE, YSIZE, nx, ny);
return matImages;
}
