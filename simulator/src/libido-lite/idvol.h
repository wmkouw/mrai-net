/*************************************************************************
* $Id: idvol.h,v 1.1 2005/09/09 08:22:43 bellet Exp $
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

#ifndef __IDVOL_H__
#define __IDVOL_H__
/*
 *  Headers for the VOLUME Ido "object".
 */
#include "idgen.h"
#include "idima.h"
#include "idpoint.h"

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

#include "idvol-restricted.h"
       
 
#ifdef SWIG
int  IdVolGetBit(PPPVOLUME_UCHAR,int,int,int);
void IdVolPutBit(PPPVOLUME_UCHAR,int,int,int,int);
#else
#define IdVolGetBit(vol,x,y,z)   (((vol)[z][y][(x)>>3]>>((x)&7) )&1)
#define IdVolPutBit(vol,x,y,z,n)                                   	\
             ( (n)                                                 	\
           ? ( ((vol)[z][y][x>>3])|= (1<<((x)&7) ) )			\
           : ( ((vol)[z][y][x>>3])&=~(1<<((x)&7) ) )  )
#endif

/* 
utilisation: 

	#include <idvol.h>
	PPPVOLUME_UCHAR vouser;
	vouser=(PPPVOLUME_UCHAR)IdVolAlloc(...); 
	.../...  
		vouser[i][j]
		IdVolDimX(vouser) 
	.../...  
	IdVolFree(vouser);
*/

/* Allocation d'un volume identique a un autre */
#ifndef SWIG

#define IdVolAllocLikeVolume(vo) ((!(vo) ) ? 0                              \
                                : IdVolAlloc(IdVolDimX(vo), IdVolDimY(vo),  \
                                             IdVolDimZ(vo), IdVolType(vo) ) )

/* Test d'egalite des dimensions et du type de deux volumes */

#define IdVolSameSizeAndType(vo1,vo2) ((IdVolDimX(vo1)==IdVolDimX(vo2) ) && \
                                       (IdVolDimY(vo1)==IdVolDimY(vo2) ) && \
                                       (IdVolDimZ(vo1)==IdVolDimZ(vo2) ) && \
                                       (IdVolType(vo1)==IdVolType(vo2) ) )

/* Test d'egalite des dimensions de deux volumes */

#define IdVolSameSize(vo1,vo2) ((IdVolDimX(vo1)==IdVolDimX(vo2) )         && \
                                       (IdVolDimY(vo1)==IdVolDimY(vo2) )  && \
                                       (IdVolDimZ(vo1)==IdVolDimZ(vo2)  ))

/* Test d'egalite du type de deux volumes */

#define IdVolSameType(vo1,vo2) ((IdVolDimX(vo1)==IdVolDimX(vo2) )         && \
                                (IdVolType(vo1)==IdVolType(vo2) ) )
#define IdVolType(volume_)       ( (_IdVolPrivate(volume_) )->Type) 
#endif

#ifndef SWIG
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

#else
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
#endif  // SWIG

#ifdef SWIG
PPPVOLUME IdVolAllocLikeVolume(PPPVOLUME);

int IdVolSameSizeAndType(PPPVOLUME,PPPVOLUME);
int IdVolSameSize(PPPVOLUME,PPPVOLUME);
int IdVolSameType(PPPVOLUME,PPPVOLUME);
int IdVolType(PPPVOLUME);

#endif  // SWIG

/************************************************************************/
/* fonctions VOLUME */

PPPVOLUME IdVolAlloc			(int, int, int, int);  
PPPVOLUME IdVolVecteurToVolume ( void *, int, int, int, int );
         
int       IdVolReadRawFileToVol		(char*, PPPVOLUME);             
PPPVOLUME IdVolReadRawFile		(char*, int, int, int, int);    
int       IdVolWriteRawFile		(char*, PPPVOLUME);             
PPPVOLUME IdVolReadRawFileWithOffset 	(char*, int, int, int, int,int); 
PPPVOLUME IdVolClear			(PPPVOLUME);                      
PPPVOLUME_UCHAR
	  IdVolSetValue			(PPPVOLUME_UCHAR, int);
void      IdVolArrondi     		(PPPVOLUME);
double    IdVolMaxima    		(PPPVOLUME);
double    IdVolMinima    		(PPPVOLUME); 
int       IdVolMinMax    		(PPPVOLUME, double *OUTPUT,
                                         double *OUTPUT);

double    	IdVolDyn       		(PPPVOLUME);
double    	IdVolMoyenne   		(PPPVOLUME);
double    	IdVolVariance  		(PPPVOLUME);
int		IdVolMoyVar		(PPPVOLUME, double *, double *);
double		IdVolEntropie  		(PPPVOLUME);
PSIGNAL_FLOAT 	IdVolHisto   		(PPPVOLUME);
double 		IdVolMaxPlanX      	(PPPVOLUME,int);
double 		IdVolMaxPlanY       	(PPPVOLUME,int);
double 		IdVolMaxPlanZ       	(PPPVOLUME,int);
PPPVOLUME 	IdVolCutVolume   	(PPPVOLUME,double);
PPPVOLUME 	IdVolCopy        	(PPPVOLUME,PPPVOLUME);
PPPVOLUME 	IdVolCast        	(PPPVOLUME,PPPVOLUME);
PPPVOLUME 	IdVolCastReal    	(PPPVOLUME,PPPVOLUME);
PPPVOLUME 	IdVolCastRealSigne    	(PPPVOLUME,PPPVOLUME);
PPPVOLUME 	IdVolCastComplex 	(PPPVOLUME,PPPVOLUME);
PPPVOLUME 	IdVolAllocSubVolume 	(PPPVOLUME, int, int, int, int,
                                         int, int);
PPPVOLUME	IdVolModifSubVolume	(PPPVOLUME, PPPVOLUME, int, int, int);
PPPVOLUME 	IdVolExtractSubVolume 	(PPPVOLUME, PPPVOLUME, int, int,
                                         int, int, int, int);
void 		IdVolSwap             	(PPPVOLUME, int);

//////// Canny-Deriche volume border detection
int IdVolDericheSmoothX       (PPPVOLUME_FLOAT, double [], PPPVOLUME_FLOAT);
int IdVolDericheSmoothY       (PPPVOLUME_FLOAT, double [], PPPVOLUME_FLOAT);
int IdVolDericheSmoothZ       (PPPVOLUME_FLOAT, double [], PPPVOLUME_FLOAT);
int IdVolDericheDeriveX       (PPPVOLUME_FLOAT, double [], PPPVOLUME_FLOAT);
int IdVolDericheDeriveY       (PPPVOLUME_FLOAT, double [], PPPVOLUME_FLOAT);
int IdVolDericheDeriveZ       (PPPVOLUME_FLOAT, double [], PPPVOLUME_FLOAT);
PPPVOLUME_FLOAT IdVolDericheGradX (PPPVOLUME_FLOAT, double);
PPPVOLUME_FLOAT IdVolDericheGradY (PPPVOLUME_FLOAT, double);
PPPVOLUME_FLOAT IdVolDericheGradZ (PPPVOLUME_FLOAT, double);
PPPVOLUME_UCHAR IdVolDericheGradMagn (PPPVOLUME_FLOAT,
				      PPPVOLUME_FLOAT,
				      PPPVOLUME_FLOAT);

//CLEAN ME PPPVOLUME_UCHAR IdVolGradMagn     (PPPVOLUME_FLOAT,
//CLEAN ME                                    PPPVOLUME_FLOAT, PPPVOLUME_FLOAT);

 
PPPVOLUME_UCHAR IdVolBool          (PPPVOLUME_UCHAR, PPPVOLUME_UCHAR, int);
PPPVOLUME_UCHAR IdVolDilate        (PPPVOLUME_UCHAR, PPPVOLUME_UCHAR, int,int);
PPPVOLUME_UCHAR IdVolNDilate       (PPPVOLUME_UCHAR, PPPVOLUME_UCHAR, int, int,
                                    int);
PPPVOLUME_UCHAR IdVolErode         (PPPVOLUME_UCHAR, PPPVOLUME_UCHAR, int,int);
PPPVOLUME_UCHAR IdVolNErode        (PPPVOLUME_UCHAR, PPPVOLUME_UCHAR, int,
                                    int, int);
PPPVOLUME_UCHAR IdVolDericheProcess(char *, int, double, int *, VOID_FUNCTION_CHARP_INT_POINTER );
PPPVOLUME_UCHAR IdVolExtremSearch  (PPPVOLUME_UCHAR, PPPVOLUME_SHORT *, int); 
PPPVOLUME_UCHAR IdVolGetGradientMagnitude
                                   (PPPVOLUME_SHORT, PPPVOLUME_SHORT,
                                    PPPVOLUME_SHORT);
int             IdVolStatFloat     (PPPVOLUME_FLOAT, float *);
PPPVOLUME_UCHAR IdVolGradientHyster(PPPVOLUME_UCHAR *, PPPVOLUME_SHORT *,
                                    double, int*);
PPPVOLUME_UCHAR IdVolUcharAd       (PPPVOLUME_UCHAR,int,int,int,int,
                                    int,int,int);
PPPVOLUME_FLOAT IdVolFloatAd       (PPPVOLUME_FLOAT,double,int,int,int,
                                    int,int,int);
double           IdVolSnrFloat     (PPPVOLUME_FLOAT,PPPVOLUME_FLOAT);
double		 IdVolSnrDyn255Float
				   (PPPVOLUME_FLOAT, PPPVOLUME_FLOAT);

double           IdVolDistorsionFloat
                                   (PPPVOLUME_FLOAT,PPPVOLUME_FLOAT);
double           IdVolNorm2        (PPPVOLUME);
double           IdVolNorm1        (PPPVOLUME);
PPPVOLUME_DOUBLE IdVolDoubleDCT    (PPPVOLUME_DOUBLE);
PPPVOLUME_DOUBLE IdVolUcharDCT     (PPPVOLUME_UCHAR);
PPPVOLUME_DOUBLE IdVolDoubleIDCT   (PPPVOLUME_DOUBLE);
PPPVOLUME_DOUBLE IdVolDoubleDCTJP  (PPPVOLUME_DOUBLE);
PPPVOLUME_DOUBLE IdVolUcharDCTJP   (PPPVOLUME_UCHAR);
PPPVOLUME_DOUBLE IdVolDoubleIDCTJP (PPPVOLUME_DOUBLE);

PPPVOLUME_UCHAR  IdVolRecad16To8   (PPPVOLUME_USHORT,int,int);
PPPVOLUME_UCHAR  IdVolRecadTo8	   (PPPVOLUME,PPPVOLUME_UCHAR,double,double);

PPPVOLUME  IdVolCheckTypeSizeAlloc (PPPVOLUME,int,PPPVOLUME);
PPPVOLUME  IdVolCheckSizeAlloc     (PPPVOLUME,int,PPPVOLUME);
PPPVOLUME  IdVolCheckTypeAlloc     (PPPVOLUME,int,PPPVOLUME);
PPPVOLUME  IdVolCheckSizeAllocType (PPPVOLUME,int,PPPVOLUME,int);
PPPVOLUME  IdVolCheckTypeAllocSize (PPPVOLUME,int,PPPVOLUME,int,int,int);


int IdVolCalcNbPlanInterp          (PPPVOLUME_UCHAR, double, double, double,
                                    double, double, double, int*, int*, int*);
PPPVOLUME_UCHAR IdVolInsertPlan    (PPPVOLUME_UCHAR, int, int, int);
int IdVolNearestNeighborInterpolation 
                                   (PPPVOLUME_UCHAR, int, int, int);
int IdVolLinearInterpolation       (PPPVOLUME_UCHAR, int, int, int);
int IdVolTriLinearInterpolation    (PPPVOLUME_UCHAR, int, int, int);

PPPVOLUME IdVolAddBord 		   (PPPVOLUME,double,int,int,int,int,int,int);
PPPVOLUME IdVolObjToVolume         (void *,PPPVOLUME);
PPPVOLUME_FLOAT IdVolWanYu         (PPPVOLUME_UCHAR, PPPVOLUME_FLOAT, int,
                                    int, int, int, int, int, int, int, int,
                                    double);
int IdVolGradFloatToShort 	   (char *,PPPVOLUME_SHORT *,VOID_FUNCTION_CHARP_INT_POINTER);

PPPVOLUME IdVolSeuil 		 (PPPVOLUME, PPPVOLUME, double, double, double);
PPPVOLUME IdVolStrechHisto 	 (PPPVOLUME, PPPVOLUME, double, double);
PPPVOLUME IdVolStrechHistoGene   (PPPVOLUME, PPPVOLUME, double, double,
                                  double,double);

PPPVOLUME IdVolStrechHistoGeneBin(PPPVOLUME, PPPVOLUME, double, double, double,
                                  double,double);


#ifdef SWIG
#define IdVolSubImageFromPlan(a,b,c,d,e) 	\
        IdVolSubImageXYFromPlan((a),(b),(c),(d),(e) )
#define IdVolModifSubImage(a,b,c,d,e)      	\
	IdVolModifSubImageXY ((a),(b),(c),(d),(e) )
#endif

int     IdVolSkelRempTrous      (PPPVOLUME_UCHAR, PPPVOLUME_UCHAR,
                                 PPPVOLUME_UCHAR, int *OUTPUT, int *OUTPUT,
                                 int *OUTPUT, float *OUTPUT, float *OUTPUT);
int     IdVolSkel               (PPPVOLUME_UCHAR,PSIGNAL_FLOAT * OUTPUT);
int     IdVolSkelBarb           (PPPVOLUME_UCHAR,PSIGNAL_FLOAT * OUTPUT);

PPPVOLUME_UCHAR IdVolRecadTo8DCT(PPPVOLUME,PPPVOLUME_UCHAR);
PPPVOLUME_UCHAR IdVolRecadTo8LOG(PPPVOLUME,PPPVOLUME_UCHAR);
PPPVOLUME_UCHAR IdVolRecadTo8ABS(PPPVOLUME,PPPVOLUME_UCHAR);

PPPVOLUME_COMPLEX IdVolFFT      (PPPVOLUME);
PPPVOLUME_COMPLEX IdVolIFFT     (PPPVOLUME_COMPLEX);

PPPVOLUME_UCHAR   IdVolAutoThresholdChow (PPPVOLUME , int);

#define IdVolVolumeToTabIma(v) IdVolVolumeToTabImaXY(v)

PPPVOLUME IdVolSubstract (PPPVOLUME, PPPVOLUME, PPPVOLUME, double, double);
PPPVOLUME IdVolSousEch   (PPPVOLUME, int, int, int);

int       IdVolMinMaxSubVolNoLoad (char *, int, int, int, int, int, int,
                                   int, int, int, int, int, long int,
                                   double * OUTPUT, double * OUTPUT);
int       IdVolMinMaxNoLoad       (char *, int, int, int, int, int, long int,
                                   double * OUTPUT, double * OUTPUT);
PPPVOLUME IdVolSubArrayToVolume   (void ***, int, int, int, int, int, int,
                                   int, int, int, int);
PPPVOLUME IdVolArrayToVolume      (void ***, int, int, int, int);

 
PPIMAGE   IdVolGetPlane 		(PPPVOLUME, int, int); 
int       IdVolSetPlane     		(PPPVOLUME,int,int,PPIMAGE);
PPPVOLUME       IdVolTabImaToVolume(PPIMAGE *, int);
PPIMAGE          IdVolMipEasy      (PPPVOLUME, int);
PPIMAGE          IdVolMipEasySubVol(PPPVOLUME, int, int, int, int, int,
                                    int, int);
PPIMAGE IdVolSubImageXYFromPlan (PPPVOLUME, int, int, int, int, int);
PPIMAGE IdVolModifSubImageXY    (PPPVOLUME, PPIMAGE,  int, int, int);
PPIMAGE IdVolSubImageXZFromPlan (PPPVOLUME, int, int, int, int, int);
PPIMAGE IdVolModifSubImageXZ    (PPPVOLUME, PPIMAGE,  int, int, int);
PPIMAGE *         IdVolVolumeToTabImaXY  (PPPVOLUME);
PPIMAGE *         IdVolVolumeToTabImaXZ  (PPPVOLUME);
PPPVOLUME_UCHAR	IdVolImageACM  (PPIMAGE_UCHAR);
PPIMAGE_UCHAR	IdVolImageACMCreeGraphe(PPIMAGE_UCHAR, PPPVOLUME_UCHAR);
PPIMAGE_UCHAR 	IdVolImageCheminMinimal(PPPVOLUME_UCHAR, int, int, int, int);

double IdVolMinPlanX(PPPVOLUME vol, int X);
double IdVolMinPlanY(PPPVOLUME vol, int Y);
double IdVolMinPlanZ(PPPVOLUME vol, int Y);

PPPVOLUME IdVolRotate90(PPPVOLUME);
PPPVOLUME_UCHAR IdVolUcharSup(PPPVOLUME_UCHAR, int, int, int, int, int, int);
PPPVOLUME_FLOAT IdVolFloatSup(PPPVOLUME_FLOAT, int, int, int, int, int, int);


/*######################################################################*/

/* Fonctions internes */

#ifndef SWIG
#define IdVolFree(pi)          _IdVolFree((PPPVOLUME*)&(pi))
#define IdVolFreeSubVolume(pi) _IdVolFreeSubVolume((PPPVOLUME*)&(pi))
#else
void IdVolFree			(PPPVOLUME);
void IdVolFreeSubVolume	(PPPVOLUME);
#endif

/************************************************************************/

#endif


/* FUNCTION DESCRIPTION **************************************************

IdVolDimX (macro)

RESUME: Donne la taille sur l'axe X du Volume LibIDO considere.

DESCRIPTION: Donne la taille sur l'axe X du Volume LibIDO considere.

SYNTAXE:     int dimx = IdVolDimX ( PPPVOLUME vol);

RETOUR:      type  : int
	     role  : taille en X du volume.

PARAMETRES:  nom   : vol
	     type  : objet volume LibIDO 
	     role  : volume LibIDO a scruter

FICHIER:     IDVOL.H

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdVolDimY (macro)

RESUME: Donne la taille sur l'axe Y du Volume LibIDO considere.

DESCRIPTION: Donne la taille sur l'axe Y du Volume LibIDO considere.

SYNTAXE:     int dimY= IdVolDimY ( PPPVOLUME vol);

RETOUR:      type  : int
	     role  : taille en Y du volume.

PARAMETRES:  nom   : vol
	     type  : objet volume LibIDO 
	     role  : volume LibIDO a scruter

FICHIER:     IDVOL.H

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdVolDimZ (macro)

RESUME: Donne la taille sur l'axe Z du Volume LibIDO considere.

DESCRIPTION: Donne la taille sur l'axe Z du Volume LibIDO considere.

SYNTAXE:     int dimz= IdVolDimZ ( PPPVOLUME vol);

RETOUR:      type  : int
	     role  : taille en Z du volume.

PARAMETRES:  nom   : vol
	     type  : objet volume LibIDO 
	     role  : volume LibIDO a scruter

FICHIER:     IDVOL.H

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdVolType (macro)

RESUME: Donne le type d'un volume LibIDO

DESCRIPTION: Donne le type d'un volume LibIDO

SYNTAXE:     int typ_vol = IdVolType (PPPVOLUME vol);

RETOUR:      type  : int
	     role  : Type du Volume LibIDO. #defini par le tableau suivant :

             #define VOL_CHAR           (VOL | TY_CHAR)          
             #define VOL_UCHAR          (VOL | TY_UCHAR)         
             #define VOL_SHORT          (VOL | TY_SHORT)         
             #define VOL_USHORT         (VOL | TY_USHORT)        
             #define VOL_LONG           (VOL | TY_LONG)          
             #define VOL_FLOAT          (VOL | TY_FLOAT)         
             #define VOL_DOUBLE         (VOL | TY_DOUBLE)        
             #define VOL_COMPLEX        (VOL | TY_COMPLEX)       
             #define VOL_RGB            (VOL | TY_RGB)           
             #define VOL_COMPLEX_DOUBLE (VOL | TY_COMPLEX_DOUBLE)
             #define VOL_COMPLEX_FLOAT  (VOL | TY_COMPLEX_FLOAT )
             #define VOL_COMPLEX_CHAR   (VOL | TY_COMPLEX_CHAR  )
             #define VOL_COMPLEX_UCHAR  (VOL | TY_COMPLEX_UCHAR )
             #define VOL_COMPLEX_SHORT  (VOL | TY_COMPLEX_SHORT )
             #define VOL_COMPLEX_USHORT (VOL | TY_COMPLEX_USHORT)
             #define VOL_COMPLEX_LONG   (VOL | TY_COMPLEX_LONG  )
             #define VOL_BIT            (VOL | TY_BIT )          
                                                    
PARAMETRES:  nom   : vol
	     type  : Objet volume LibIDO 
	     role  : Volume LibIDO a scruter.

FICHIER:     IDVOL.H

******************************************************** END DESCRIPTION */


/* DEFINITION DESCRIPTION **************************************************

idvol.h (constantes)  

RESUME: constantes VOL_??? pour definir le type de voxel des volumes a allouer

DESCRIPTION : constantes VOL_??? utilisees dans les fonctions d'allocation 
              de volumes permettant de definir le type des voxels.           
              
           : IdVolAlloc, IdVolType, IdLibidoDataType.
           
           : VOL_CHAR   : VOL_DOUBLE         : VOL_COMPLEX_UCHAR  :
           : VOL_UCHAR  : VOL_COMPLEX        : VOL_COMPLEX_SHORT  :
           : VOL_SHORT  : VOL_RGB            : VOL_COMPLEX_USHORT :
           : VOL_USHORT : VOL_COMPLEX_DOUBLE : VOL_COMPLEX_LONG   :
           : VOL_LONG   : VOL_COMPLEX_FLOAT  : VOL_BIT            :
           : VOL_FLOAT  : VOL_COMPLEX_CHAR   :
                    
              Chaque type (ie UCHAR) est decrit dans : IdLibidoDataType
                                                            

******************************************************** END DESCRIPTION */


/* DEFINITION DESCRIPTION **************************************************

idvol.h (types) 

RESUME: types predefinis des volumes (associes a IdLibidoDataType)

DEFINITION : types predefinis pour l'allocation des volumes, associes aux types
             predefinis par IdLibidoDataType.
             
          : IdVolAlloc, IdLibidoDataType.

  : PPPVOLUME         : PPPVOLUME_RGB            : PPPVOLUME_COMPLEX_CHAR   :
  : PPPVOLUME_CHAR    : PPPVOLUME_SHORT          : PPPVOLUME_COMPLEX_UCHAR  :
  : PPPVOLUME_UCHAR   : PPPVOLUME_USHORT         : PPPVOLUME_COMPLEX_SHORT  :
  : PPPVOLUME_FLOAT   : PPPVOLUME_LONG           : PPPVOLUME_COMPLEX_USHORT :
  : PPPVOLUME_DOUBLE  : PPPVOLUME_COMPLEX_DOUBLE : PPPVOLUME_COMPLEX_LONG   :
  : PPPVOLUME_COMPLEX : PPPVOLUME_COMPLEX_FLOAT  : PPPVOLUME_BIT            :
                          
   PPPVOLUME seul est de type void.

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdVolGetBit (macro)

RESUME: Retourne la valeur du voxel de coordonnees x,y,z.

DESCRIPTION: Retourne la valeur du voxel de coordonnees x,y,z.

SYNTAXE:     int val = IdVolGetBit(PPVOLUME_BIT vol,int x, int y, int z);

RETOUR:      type  : int 
	     role  : 0 ou 1, selon la valeur du bit du volume.

PARAMETRES:  
	     nom   : vol
	     type  : PPVOLUME_BIT 
	     role  : Volume de bit.

	     nom   : x, y, z
	     type  : int 
	     role  : Coordonnees x, y et z du volume.

FICHIER:     idvol.h

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdVolPutBit (macro)

RESUME: Fixe la valeur d'un voxel d'un volume binaire (PPVOLUME_BIT).

DESCRIPTION: Fixe la valeur d'un voxel d'un volume binaire (PPVOLUME_BIT).

SYNTAXE:     void  IdVolPutBit(PPVOLUME_BIT vol, int x, int y, int z, int valBit);

RETOUR:      type  : void 
	     role  : non defini.

PARAMETRES:  
	     nom   : vol
	     type  : PPVOLUME_BIT 
	     role  : Volume de bit.

	     nom   : x, y, z
	     type  : int 
	     role  : Coordonnees x, y et z du volume.

	     nom   : n
	     type  : int 
	     role  : Imperativement 0 ou 1, pour regler la valeur du bit.

FICHIER:     idvol.h

******************************************************** END DESCRIPTION */
                          
/* FUNCTION DESCRIPTION **************************************************

IdVolAllocLikeVolume (macro)

RESUME: Alloc. d'1 volume de memes dimensions et type qu'1 autre

DESCRIPTION: Allocation d'un volume de memes dimensions et meme type
	     qu'une autre.

SYNTAXE:     PPPVOLUME vol2 = IdVolAllocLikeVolume (PPPVOLUME vol1);


RETOUR:      type  : PPPVOLUME
	     role  : Pointeur vers le volume alloue (a convertir en un type
		     valide : PPPVOLUME_UCHAR par exemple ).


PARAMETRES:  nom   : vol1
	     type  : PPPVOLUME, ou PPPVOLUME_???
	     role  : Pointeur vers le volume 'modele'.

FICHIER:     idvol.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdVolSameSizeAndType (macro)

RESUME: Comparaison du type et des dimensions de deux volumes.

DESCRIPTION: Comparaison du type et des dimensions de deux volumes.

SYNTAXE:     int Booleen = IdVolSameSizeAndType(PPPVOLUME vol1, PPPVOLUME vol2);

RETOUR:      type  : int
	     role  : Vrai si les deux volumes ont la meme dimension et
		     le meme type.

PARAMETRES:  nom   : vol1, vol2
	     type  : PPPVOLUME, ou PPPVOLUME_???
	     role  : Pointeurs vers les volumes a comparer.

FICHIER :    idvol.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdVolSameSize (macro)

RESUME: Comparaison des dimensions de deux volumes.

DESCRIPTION: Comparaison des dimensions de deux volumes.

SYNTAXE:     int Booleen = IdVolSameSize(PPPVOLUME vol1, PPPVOLUME vol2);

RETOUR:      type  : int
	     role  : Vrai si les deux volumes ont les memes dimensions

PARAMETRES:  nom   : vol1, vol2
	     type  : PPPVOLUME, ou PPPVOLUME_???
	     role  : Pointeurs vers les volumes a comparer.

FICHIER :    idvol.h

******************************************************** END DESCRIPTION */



/* FUNCTION DESCRIPTION **************************************************

IdVolSameType (macro)

RESUME: Comparaison des types de deux volumes.

DESCRIPTION: Comparaison des types de deux volumes.

SYNTAXE:     int Booleen = IdVolSameType(PPPVOLUME vol1, PPPVOLUME vol2);

RETOUR:      type  : int
	     role  : Vrai si les deux volumes ont le meme type

PARAMETRES:  nom   : vol1, vol2
	     type  : PPPVOLUME, ou PPPVOLUME_???
	     role  : Pointeurs vers les volumes a comparer.

FICHIER :    idvol.h

******************************************************** END DESCRIPTION */
