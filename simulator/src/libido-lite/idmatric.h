/*************************************************************************
* $Id: idmatric.h,v 1.1 2005/09/09 08:22:37 bellet Exp $
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

#ifndef __IDMATRIC_H__
#define __IDMATRIC_H__

#include "idima.h"
#include "idsig.h"
#include "idpoint.h"

/*************************************************************************
*  Description : Declarations diverses permettant d'utiliser les types 
*                MATRIX VECTOR
*
*
**************************************************************************/

typedef           void **  PPMATRICE ;                         
typedef           char **  PPMATRICE_CHAR ;                    
typedef  unsigned char **  PPMATRICE_UCHAR  ;                  
typedef          float **  PPMATRICE_FLOAT ;                   
typedef         double **  PPMATRICE_DOUBLE ;                  
typedef        COMPLEX **  PPMATRICE_COMPLEX ;                 
typedef            RGB **  PPMATRICE_RGB ;                     
typedef           RGBA **  PPMATRICE_RGBA ;                   /*CG */
typedef          short **  PPMATRICE_SHORT ;                   
typedef unsigned short **  PPMATRICE_USHORT ;                  
typedef           long **  PPMATRICE_LONG ;                    
typedef unsigned  long **  PPMATRICE_ULONG ;                  
typedef COMPLEX_DOUBLE **  PPMATRICE_COMPLEX_DOUBLE ;          
typedef  COMPLEX_FLOAT **  PPMATRICE_COMPLEX_FLOAT ;           
typedef   COMPLEX_CHAR **  PPMATRICE_COMPLEX_CHAR ;            
typedef  COMPLEX_UCHAR **  PPMATRICE_COMPLEX_UCHAR ;           
typedef  COMPLEX_SHORT **  PPMATRICE_COMPLEX_SHORT ;           
typedef COMPLEX_USHORT **  PPMATRICE_COMPLEX_USHORT ;          
typedef   COMPLEX_LONG **  PPMATRICEE_COMPLEX_LONG ;            
typedef  COMPLEX_ULONG **  PPMATRICE_COMPLEX_ULONG ;

typedef           void **  PPMATRIX;                         
typedef           char **  PPMATRIX_CHAR ;                    
typedef  unsigned char **  PPMATRIX_UCHAR  ;                  
typedef          float **  PPMATRIX_FLOAT ;                   
typedef         double **  PPMATRIX_DOUBLE ;                  
typedef        COMPLEX **  PPMATRIXE_COMPLEX ;                 
typedef            RGB **  PPMATRIX_RGB ;                     
typedef           RGBA **  PPMATRIX_RGBA ;
typedef          short **  PPMATRIX_SHORT ;                   
typedef unsigned short **  PPMATRIX_USHORT ;                  
typedef           long **  PPMATRIX_LONG ;                    
typedef unsigned  long **  PPMATRIX_ULONG ;                  
typedef COMPLEX_DOUBLE **  PPMATRIX_COMPLEX_DOUBLE ;          
typedef  COMPLEX_FLOAT **  PPMATRIX_COMPLEX_FLOAT ;           
typedef   COMPLEX_CHAR **  PPMATRIX_COMPLEX_CHAR ;            
typedef  COMPLEX_UCHAR **  PPMATRIX_COMPLEX_UCHAR ;           
typedef  COMPLEX_SHORT **  PPMATRIX_COMPLEX_SHORT ;           
typedef COMPLEX_USHORT **  PPMATRIX_COMPLEX_USHORT ;          
typedef   COMPLEX_LONG **  PPMATRIXE_COMPLEX_LONG ;            
typedef  COMPLEX_ULONG **  PPMATRIX_COMPLEX_ULONG ;


#define MAT_CHAR           (MAT | TY_CHAR)
#define MAT_UCHAR          (MAT | TY_UCHAR)
#define MAT_SHORT          (MAT | TY_SHORT)
#define MAT_USHORT         (MAT | TY_USHORT)                       
#define MAT_LONG           (MAT | TY_LONG)
#define MAT_ULONG          (MAT | TY_ULONG)
#define MAT_FLOAT          (MAT | TY_FLOAT)
#define MAT_DOUBLE         (MAT | TY_DOUBLE)
#define MAT_COMPLEX        (MAT | TY_COMPLEX)
#define MAT_COMPLEX_DOUBLE (MAT | TY_COMPLEX_DOUBLE)               
#define MAT_COMPLEX_FLOAT  (MAT | TY_COMPLEX_FLOAT )               
#define MAT_COMPLEX_CHAR   (MAT | TY_COMPLEX_CHAR  )               
#define MAT_COMPLEX_UCHAR  (MAT | TY_COMPLEX_UCHAR )               
#define MAT_COMPLEX_SHORT  (MAT | TY_COMPLEX_SHORT )               
#define MAT_COMPLEX_USHORT (MAT | TY_COMPLEX_USHORT)               
#define MAT_COMPLEX_LONG   (MAT | TY_COMPLEX_LONG  )               
#define MAT_COMPLEX_ULONG  (MAT | TY_COMPLEX_ULONG  )
#define MAT_BIT            (MAT | TY_BIT )
#define MAT_POINTER        (MAT | TY_POINTER)


typedef           void *  PVECTOR ;                        
typedef           char *  PVECTOR_CHAR ;                   
typedef  unsigned char *  PVECTOR_UCHAR  ;                  
typedef          float *  PVECTOR_FLOAT ;                   
typedef         double *  PVECTOR_DOUBLE ;                  
typedef        COMPLEX *  PVECTOR_COMPLEX ;                 
typedef          short *  PVECTOR_SHORT ;                   
typedef unsigned short *  PVECTOR_USHORT ;                  
typedef           long *  PVECTOR_LONG ;                    
typedef  unsigned long *  PVECTOR_ULONG ;
typedef COMPLEX_DOUBLE *  PVECTOR_COMPLEX_DOUBLE ;          
typedef  COMPLEX_FLOAT *  PVECTOR_COMPLEX_FLOAT ;           
typedef   COMPLEX_CHAR *  PVECTOR_COMPLEX_CHAR ;            
typedef  COMPLEX_UCHAR *  PVECTOR_COMPLEX_UCHAR ;           
typedef  COMPLEX_SHORT *  PVECTOR_COMPLEX_SHORT ;           
typedef COMPLEX_USHORT *  PVECTOR_COMPLEX_USHORT ;           
typedef   COMPLEX_LONG *  PVECTOR_COMPLEX_LONG ;            
typedef  COMPLEX_ULONG *  PVECTOR_COMPLEX_ULONG ;
typedef          void **  PVECTOR_POINTER ;
 

typedef           void *  PVECTEUR  ;                        
typedef           char *  PVECTEUR_CHAR ;                   
typedef  unsigned char *  PVECTEUR_UCHAR  ;                  
typedef          float *  PVECTEUR_FLOAT ;                   
typedef         double *  PVECTEUR_DOUBLE ;                  
typedef        COMPLEX *  PVECTEUR_COMPLEX ;                 
typedef          short *  PVECTEUR_SHORT ;                   
typedef unsigned short *  PVECTEUR_USHORT ;                  
typedef           long *  PVECTEUR_LONG ;                    
typedef  unsigned long *  PVECTEUR_ULONG ;
typedef COMPLEX_DOUBLE *  PVECTEUR_COMPLEX_DOUBLE ;          
typedef  COMPLEX_FLOAT *  PVECTEUR_COMPLEX_FLOAT ;           
typedef   COMPLEX_CHAR *  PVECTEUR_COMPLEX_CHAR ;            
typedef  COMPLEX_UCHAR *  PVECTEUR_COMPLEX_UCHAR ;           
typedef  COMPLEX_SHORT *  PVECTEUR_COMPLEX_SHORT ;           
typedef COMPLEX_USHORT *  PVECTEUR_COMPLEX_USHORT ;           
typedef   COMPLEX_LONG *  PVECTEUR_COMPLEX_LONG ;            
typedef  COMPLEX_ULONG *  PVECTEUR_COMPLEX_ULONG ;
typedef          void **  PVECTEUR_POINTER ;

#define VECT_CHAR 	    (SIG | TY_CHAR)
#define VECT_UCHAR 	    (SIG | TY_UCHAR)
#define VECT_SHORT 	    (SIG | TY_SHORT)
#define VECT_USHORT 	    (SIG | TY_USHORT)                      
#define VECT_LONG 	    (SIG | TY_LONG)
#define VECT_ULONG 	    (SIG | TY_ULONG)
#define VECT_FLOAT 	    (SIG | TY_FLOAT)
#define VECT_DOUBLE 	    (SIG | TY_DOUBLE)
#define VECT_COMPLEX 	    (SIG | TY_COMPLEX)
#define VECT_COMPLEX_DOUBLE (SIG | TY_COMPLEX_DOUBLE)
#define VECT_COMPLEX_FLOAT  (SIG | TY_COMPLEX_FLOAT )
#define VECTG_COMPLEX_CHAR  (SIG | TY_COMPLEX_CHAR  )              
#define VECT_COMPLEX_UCHAR  (SIG | TY_COMPLEX_UCHAR )
#define VECT_COMPLEX_SHORT  (SIG | TY_COMPLEX_SHORT )
#define VECT_COMPLEX_USHORT (SIG | TY_COMPLEX_USHORT)              
#define VECT_COMPLEX_LONG   (SIG | TY_COMPLEX_LONG  )              
#define VECT_COMPLEX_ULONG  (SIG | TY_COMPLEX_ULONG )
#define VECT_POINTER        (SIG | TY_POINTER)




#ifndef SWIG 
#define IdMatDimX(a)		IdImaDimX((a) )
#define IdMatDimY(a)		IdImaDimY((a) )
#define IdMatUsedNbX(image_) ( _IdImaPrivate(image_)->UsedNbX) 
#define IdMatUsedNbY(image_) ( _IdImaPrivate(image_)->UsedNbY)
#define IdMatGetUsedNbX(imag_)    ( _IdImaPrivate(imag_)->UsedNbX)
#define IdMatSetUsedNbX(imag_,nb) ( _IdImaPrivate(imag_)->UsedNbX=(nb) )
#define IdMatGetUsedNbY(imag_)    ( _IdImaPrivate(imag_)->UsedNbY)
#define IdMatSetUsedNbY(imag_,nb) ( _IdImaPrivate(imag_)->UsedNbY=(nb) )
#define IdMatType(image_) ( _IdImaPrivate(image_)->Type)
#define IdMatAlloc(a,b,c)  	IdImaAlloc( (a),(b),(c) )
#define IdVectAlloc(a,b) 	IdSigAlloc( (a),(b) )
#define IdMatFree(a) 		IdImaFree( (a) )
#define IdVectFree(a) 		IdSigFree( (a) )
#else
int IdMatDimX(PPMATRICE);
int IdMatDimY(PPMATRICE);
int IdMatUsedNbX(PPMATRICE);
int IdMatUsedNbY(PPMATRICE);
int IdMatGetUsedNbX(PPMATRICE);
int IdMatGetUsedNbY(PPMATRICE);
int IdMatSetUsedNbX(PPMATRICE,int);
int IdMatSetUsedNbY(PPMATRICE,int);
int IdMatType(PPMATRICE);
PPMATRICE IdMatAlloc(int,int,int);
PVECTEUR  IdVectAlloc(int,int);
void IdMatFree(PPMATRICE);
void IdVectFree(PVECTEUR);
#endif


void IdMatLubksb (PPMATRICE_DOUBLE,PVECTOR_LONG,PVECTOR_DOUBLE);
int IdMatLudcmp  (PPMATRICE_DOUBLE,PVECTOR_LONG,double *);
int IdMatInv     (PPMATRICE_FLOAT,PPMATRICE_FLOAT *);
int IdMatInvd    (PPMATRICE_FLOAT,PPMATRICE_FLOAT *,float *);
int IdMatProd    (PPMATRICE_FLOAT,PPMATRICE_FLOAT,PPMATRICE_FLOAT *);
int IdMatTransp  (PPMATRICE_FLOAT,PPMATRICE_FLOAT *);
int IdMatJacobi  (PPMATRICE_DOUBLE, PVECTOR_DOUBLE, PPMATRICE_DOUBLE, int *);

void IdMatAffMat           (PPMATRICE_FLOAT);
void IdVectAffVect         (PVECTOR_FLOAT);
int IdMatVectProduct       (PPMATRICE_FLOAT,PVECTOR_FLOAT,PVECTOR_FLOAT);
int IdMatTranspose         (PPMATRICE_FLOAT,PPMATRICE_FLOAT);
double IdVectScalarProduct (PVECTOR_FLOAT,PVECTOR_FLOAT);
int IdVectCrossProduct     (PVECTOR_FLOAT,PVECTOR_FLOAT,PVECTOR_FLOAT);
int IdMatProduct 	   (PPMATRICE_FLOAT, PPMATRICE_FLOAT, PPMATRICE_FLOAT);
double IdVectNorm          (PVECTOR_FLOAT);
int IdVectScalarDiv        (PVECTOR_FLOAT,double,PVECTOR_FLOAT);
int IdVectNormalize        (PVECTOR_FLOAT,PVECTOR_FLOAT);
int IdVectEq               (PVECTOR_FLOAT,PVECTOR_FLOAT);
int IdMatEq                (PPMATRICE_FLOAT,PPMATRICE_FLOAT);
int IdMatUnit              (PPMATRICE_FLOAT);


PPMATRICE_FLOAT IdMatMatricePassage (IDPOINT,IDPOINT,IDPOINT);
void IdMatPassageImageReelle        (IDPOINT, PIDPOINT, PPMATRICE_FLOAT);
void IdMatPassageReelleImage        (PIDPOINT, IDPOINT, PPMATRICE_FLOAT);
void IdVectTranslatePoint           (PIDPOINT, PVECTOR_FLOAT);
void IdVectTranslatePointInv        (PIDPOINT, PVECTOR_FLOAT);
void IdMatCreatMatTranslation       (PPMATRICE_FLOAT, PVECTOR_FLOAT);
void IdMatCreatMatRotation          (PPMATRICE_FLOAT, PVECTOR_FLOAT,
                                     PVECTOR_FLOAT, PVECTOR_FLOAT);
void IdMatCreatMatScaling           (PPMATRICE_FLOAT, double, double, double);
void IdVectVectToHomogene           (PVECTOR_FLOAT, PVECTOR_FLOAT);
void IdVectHomogeneToVect           (PVECTOR_FLOAT, PVECTOR_FLOAT);

PVECTOR_COMPLEX IdCalcRacinesPoly2 (double, double, double);
PVECTOR_COMPLEX IdCalcRacinesPoly3 (double, double, double, double);

int IdMatEstimKalman       (PVECTOR_FLOAT, PPMATRICE_FLOAT, PVECTOR_FLOAT,
                            PPMATRICE_FLOAT, PPMATRICE_FLOAT, PVECTOR_FLOAT,
                            PPMATRICE_FLOAT);
int IdMatPrediKalman       (PVECTOR_FLOAT, PPMATRICE_FLOAT, PPMATRICE_FLOAT,
                            PPMATRICE_FLOAT, PVECTOR_FLOAT, PPMATRICE_FLOAT);
int IdMatEstimKalmanEtendu (PPMATRICE_FLOAT, PPMATRICE_FLOAT, PPMATRICE_FLOAT,
                            PPMATRICE_FLOAT, PPMATRICE_FLOAT);
int IdMatPrediKalmanEtendu (PPMATRICE_FLOAT, PPMATRICE_FLOAT, PPMATRICE_FLOAT,
                            PPMATRICE_FLOAT);


/* FUNCTION DESCRIPTION ****************************************************

IdMatAlloc (macro)

RESUME: Allocation memoire d'un objet libido de type PPMATRICE

DESCRIPTION: Allocation memoire d'un objet libido de type PPMATRICE.
		(Meme fonctionnement que IdImaAlloc)

SYNTAXE:    PPMATRICE matrice = IdMatAlloc ( int taille_x, int taille_y, int typeMat);

RETOUR:      type  : PPMATRICE
	     role  : pointeur sur l'objet et les donnees images

PARAMETRES : nom   : taille_x 
	     type  : int
	     role  : taille en x (nb colonnes) de la matrice a creer

	     nom   : taille_y 
	     type  : int
	     role  : taille en y (nb de lignes) de la matrice a creer

	     nom   : type
	     type  : int
	     role  : type de la matrice a creer (MAT_xxx ex: MAT_UCHAR)

FICHIER:     idmatric.c

********************************************************* END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdMatDimX (macro)

RESUME: Donne la taille en X d'une matrice.

DESCRIPTION: Donne la taille en X d'une matrice.

SYNTAXE:     int dimx = IdMatDimX (PPMATRICE matrice);

RETOUR:      type  : int
             role  : taille en x de la matrice

PARAMETRES:  nom   : image
             type  : objet matrice Libido
             role  : Image LibIDO a scruter.

EXEMPLE:     

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdMatDimY (macro)

RESUME: Donne la taille en Y d'une matrice.

DESCRIPTION: Donne la taille en Y d'une matrice.

SYNTAXE:     int dimy = IdMatDimY (PPMATRICE matrice);

RETOUR:      type  : int
             role  : taille en y de l'matrice

PARAMETRES:  nom   : matrice
             type  : objet matrice Libido
             role  : matrice LibIDO a scruter.

EXEMPLE:     

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdMatType (macro)

RESUME: Donne le type d'une matrice LibIDO.

DESCRIPTION: Donne le type d'une matrice LibIDO.

SYNTAXE:     int typ_im = IdMatType (PPMATRICE matrice);

RETOUR:      type  : int
             role  : Type de la matrice LibIDO. #defini par le tableau suivant :


             #define MAT_CHAR           (MAT | TY_CHAR)           
             #define MAT_UCHAR          (MAT | TY_UCHAR)          
             #define MAT_SHORT          (MAT | TY_SHORT)          
             #define MAT_USHORT         (MAT | TY_USHORT)         
             #define MAT_LONG           (MAT | TY_LONG)           
             #define MAT_FLOAT          (MAT | TY_FLOAT)          
             #define MAT_DOUBLE         (MAT | TY_DOUBLE)         
             #define MAT_COMPLEX        (MAT | TY_COMPLEX)        
             #define MAT_COMPLEX_DOUBLE (MAT | TY_COMPLEX_DOUBLE) 
             #define MAT_COMPLEX_FLOAT  (MAT | TY_COMPLEX_FLOAT ) 
             #define MAT_COMPLEX_CHAR   (MAT | TY_COMPLEX_CHAR  ) 
             #define MAT_COMPLEX_UCHAR  (MAT | TY_COMPLEX_UCHAR ) 
             #define MAT_COMPLEX_SHORT  (MAT | TY_COMPLEX_SHORT ) 
             #define MAT_COMPLEX_USHORT (MAT | TY_COMPLEX_USHORT) 
             #define MAT_COMPLEX_LONG   (MAT | TY_COMPLEX_LONG  ) 
             #define MAT_BIT            (MAT | TY_BIT )           
                                                     

PARAMETRES:  nom   : im
             type  : Objet matrice LibIDO 
             role  : Image matrice a scruter

EXEMPLE:     

******************************************************** END DESCRIPTION */

#endif
