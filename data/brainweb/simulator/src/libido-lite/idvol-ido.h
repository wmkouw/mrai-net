/*************************************************************************
* $Id: idvol-ido.h,v 1.1 2005/09/09 08:22:42 bellet Exp $
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

#ifndef __IDVOL_IDO_H__
#define __IDVOL_IDO_H__


/* Types de volume 3d */

#define VOL_CHAR 	(VOL | TY_CHAR)
#define VOL_UCHAR 	(VOL | TY_UCHAR)
#define VOL_SHORT 	(VOL | TY_SHORT)
#define VOL_USHORT 	(VOL | TY_USHORT)                         
#define VOL_LONG 	(VOL | TY_LONG)
#define VOL_ULONG 	(VOL | TY_ULONG)
#define VOL_FLOAT 	(VOL | TY_FLOAT)
#define VOL_DOUBLE 	(VOL | TY_DOUBLE)
#define VOL_COMPLEX 	(VOL | TY_COMPLEX)
#define VOL_RGB 	(VOL | TY_RGB)
#define VOL_COMPLEX_DOUBLE (VOL | TY_COMPLEX_DOUBLE)
#define VOL_COMPLEX_FLOAT  (VOL | TY_COMPLEX_FLOAT )
#define VOL_COMPLEX_CHAR   (VOL | TY_COMPLEX_CHAR  )
#define VOL_COMPLEX_UCHAR  (VOL | TY_COMPLEX_UCHAR )
#define VOL_COMPLEX_SHORT  (VOL | TY_COMPLEX_SHORT )
#define VOL_COMPLEX_USHORT (VOL | TY_COMPLEX_USHORT)  
#define VOL_COMPLEX_LONG   (VOL | TY_COMPLEX_LONG  ) 
#define VOL_COMPLEX_ULONG  (VOL | TY_COMPLEX_ULONG )
#define VOL_BIT            (VOL | TY_BIT           )
#define VOL_POINTER        (VOL | TY_POINTER       )

/***************************************************************/

typedef           void *** PPPVOLUME;
typedef           char *** PPPVOLUME_CHAR; 
typedef  unsigned char *** PPPVOLUME_UCHAR; 
typedef          float *** PPPVOLUME_FLOAT;                
typedef         double *** PPPVOLUME_DOUBLE;               
typedef        COMPLEX *** PPPVOLUME_COMPLEX;              
typedef            RGB *** PPPVOLUME_RGB;                  
typedef          short *** PPPVOLUME_SHORT;                
typedef unsigned short *** PPPVOLUME_USHORT;               
typedef           long *** PPPVOLUME_LONG;                 
typedef unsigned  long *** PPPVOLUME_ULONG;
typedef COMPLEX_DOUBLE *** PPPVOLUME_COMPLEX_DOUBLE;       
typedef  COMPLEX_FLOAT *** PPPVOLUME_COMPLEX_FLOAT;        
typedef   COMPLEX_CHAR *** PPPVOLUME_COMPLEX_CHAR;         
typedef  COMPLEX_UCHAR *** PPPVOLUME_COMPLEX_UCHAR;        
typedef  COMPLEX_SHORT *** PPPVOLUME_COMPLEX_SHORT;        
typedef COMPLEX_USHORT *** PPPVOLUME_COMPLEX_USHORT;       
typedef   COMPLEX_LONG *** PPPVOLUME_COMPLEX_LONG;         
typedef  COMPLEX_ULONG *** PPPVOLUME_COMPLEX_ULONG;
typedef PPPVOLUME_UCHAR    PPPVOLUME_BIT;                  
typedef          void **** PPPVOLUME_POINTER;

/* structure VOLUME */

typedef struct
	{ 	  
	  int   UsedNbZ;
	  int   UsedNbY;
          int   UsedNbX;
          int   subObject;
       	  int 	DimZ;
	  int   DimY;
	  int   DimX;
          int   Numero;
	  char  *_message,*_fichier;
	  int   BitsPerPixel;        /* nombre de bits reellement utilises */
	  int   Type; /* IMPERATIVEMENT DERNIER DE LA STRUCTURE */

	} PRIVATE_VOLUME;                                         

#define _IdVolPrivate(vol)       ( ((PRIVATE_VOLUME*)(vol) )-1 )  


#define IdVolDimX(volume_)       ( (_IdVolPrivate(volume_) )->DimX)  
#define IdVolDimY(volume_)       ( (_IdVolPrivate(volume_) )->DimY)  
#define IdVolDimZ(volume_)       ( (_IdVolPrivate(volume_) )->DimZ)

#define IdVolUsedNbX(volume_)    ( (_IdVolPrivate(volume_) )->UsedNbX)
#define IdVolUsedNbY(volume_)    ( (_IdVolPrivate(volume_) )->UsedNbY)
#define IdVolUsedNbZ(volume_)    ( (_IdVolPrivate(volume_) )->UsedNbZ) 

#define IdVolSetUsedNbX(vol_,nb) ( _IdVolPrivate(vol_)->UsedNbX=(nb) )
#define IdVolGetUsedNbX(vol_) 	 ( _IdVolPrivate(vol_)->UsedNbX)
#define IdVolSetUsedNbY(vol_,nb) ( _IdVolPrivate(vol_)->UsedNbY=(nb) )
#define IdVolGetUsedNbY(vol_) 	 ( _IdVolPrivate(vol_)->UsedNbY) 
#define IdVolSetUsedNbZ(vol_,nb) ( _IdVolPrivate(vol_)->UsedNbZ=(nb) )
#define IdVolGetUsedNbZ(vol_) 	 ( _IdVolPrivate(vol_)->UsedNbZ) 

#define IdVolType(volume_)       ( (_IdVolPrivate(volume_) )->Type) 

#ifdef SWIG
int IdVolDimX(PPPVOLUME);
int IdVolDimY(PPPVOLUME);
int IdVolDimZ(PPPVOLUME);

int IdVolUsedNbX(PPPVOLUME);
int IdVolUsedNbY(PPPVOLUME);
int IdVolUsedNbZ(PPPVOLUME);

int IdVolGetUsedNbX(PPPVOLUME);
int IdVolGetUsedNbY(PPPVOLUME);
int IdVolGetUsedNbZ(PPPVOLUME);

void IdVolSetUsedNbX(PPPVOLUME,int);
void IdVolSetUsedNbY(PPPVOLUME,int);
void IdVolSetUsedNbZ(PPPVOLUME,int);
#endif  
// SWIG



#endif

