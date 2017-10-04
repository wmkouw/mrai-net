/*************************************************************************
* $Id: idcnt3d.h,v 1.1 2005/09/09 08:22:27 bellet Exp $
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

#ifndef __IDCNT3D_H__
#define __IDCNT3D_H__
#include "idgen.h"
//#include "idima.h"
#include "idpoint.h"

/*
 * Declarations diverses permettant d'utiliser le type CNT3 of IDO
 */

/* structure CONTOUR3D */

#define CNT3D_FLOAT	(CNT3D | TY_IDPOINT)
typedef			void * PCONTOUR3D ; 
typedef			PIDPOINT * PCONTOUR3D_FLOAT ;       /* PIDPOINT !! */          			 	
typedef struct {
          int   UsedNbX;       /* JPR */
          int   __dum;    /* pour alignement frontiere Double Mot */
	  int   DimX;
          int   Numero;
	  char  *_message,*_fichier;
	  int   BitsPerPixel;        /* nombre de bits reellement utilises */
	  int   Type;       /* IMPERATIVEMENT DERNIER DE LA STRUCTURE */

	} PRIVATE_CONTOUR3D; 
                 

#define _IdCnt3DPrivate(cnt)   (((PRIVATE_CONTOUR3D*)(cnt))-1)  

#include "idcnt3d-restricted.h"
  
#ifndef SWIG
#define IdCnt3DDimX(cnt_)      ( _IdCnt3DPrivate(cnt_)->DimX)  
#define IdCnt3DUsedNbX(cnt_)   ( _IdCnt3DPrivate(cnt_)->UsedNbX)
#define IdCnt3DUsedNbPts(cnt_) ( _IdCnt3DPrivate(cnt_)->UsedNbX)
#define IdCnt3DSetUsedNbX(cnt_,nb)	( _IdCnt3DPrivate(cnt_)->UsedNbX=(nb) )
#define IdCnt3DSetUsedNbPts(cnt_,nb) 	( _IdCnt3DPrivate(cnt_)->UsedNbX=(nb) )
#define IdCnt3DGetUsedNbPts(cnt_)  	( _IdCnt3DPrivate(cnt_)->UsedNbX)
#define IdCnt3DGetUsedNbX(cnt_)    	( _IdCnt3DPrivate(cnt_)->UsedNbX)
#define IdCnt3DType(cnt_)          	( _IdCnt3DPrivate(cnt_)->Type) 

#else
int IdCnt3DDimX(PCONTOUR3D);
int IdCnt3DUsedNbX(PCONTOUR3D);
int IdCnt3DNbPts(PCONTOUR3D);
int IdCnt3DUsedNbPts(PCONTOUR3D);
int IdCnt3DSetUsedNbX(PCONTOUR3D,int);
int IdCnt3DGetUsedNbPts(PCONTOUR3D);
int IdCnt3DGetUsedNbX(PCONTOUR3D);
int IdCnt3DType(PCONTOUR3D);
#endif

#ifndef SWIG
#define IdCnt3DNbPts(cnt_) 		 ( _IdCnt3DPrivate(cnt_)->DimX) 
#define IdCnt3DSetY(contour_,i,val)       (contour_)[(i)]->Y=(val)
#define IdCnt3DSetX(contour_,i,val)       (contour_)[(i)]->X=(val)
#define IdCnt3DSetZ(contour_,i,val)       (contour_)[(i)]->Z=(val)
#define IdCnt3DGetX(contour_,i)		(contour_)[(i)]->X
#define IdCnt3DGetY(contour_,i)		(contour_)[(i)]->Y
#define IdCnt3DGetZ(contour_,i)		(contour_)[(i)]->Z
#define IdCnt3DModifyPoint(contour_,i,valx,valy,valz)	\
			 (contour_)[(i)]->X=(valx),(contour_)[(i)]->Y=(valy),(contour_)[(i)]->Z=(valz)
#define IdCnt3DAllocLikeContour(cn) ((!(cn)) ? 0               		 \
				: IdCnt3DAlloc ( IdCnt3DDimX(cn),IdCnt3DType(cn) ))
#define IdCnt3DSameSizeAndType(i1,i2)  (     (i1)&&(i2)                       \
				       && (IdCnt3DDimX(i1)==IdCnt3DDimX(i2))    \
				       && (IdCnt3DType(i1)==IdCnt3DType(i2)) )
#define IdCnt3DSameSize(i1,i2)  (     (i1)&&(i2)                       \
				    && (IdCnt3DDimX(i1)==IdCnt3DDimX(i2)) )
#endif
/* FUNCTION DESCRIPTION **************************************************

IdCnt3DNbPts  (macro)

RESUME:  donne le points d'un CONTOUR3D (PCONTOUR3D) (identique a IdCnt3DDimX ...)

DESCRIPTION:  	donne le points d'un CONTOUR3D (identique a IdCnt3DDimX ...)
		(celui precise lors de l'allocation ou de la modif de longueur)
	
SYNTAXE:     int nbrPts = IdCnt3DNbPts (PCONTOUR3D cnt1);

PARAMETRES:  nom   : cnt1
	     type  : PCONTOUR3D
	     role  : Pointeur vers le contour source

FICHIER:     idcnt3d.h
EXEMPLE:

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdCnt3DType (macro)

RESUME: Donne le type d'un contour (PCONTOUR3D) LibIDO.

DESCRIPTION: Donne le type d'un contour LibIDO.

SYNTAXE:     int typ_cn = IdCnt3DType (PCONTOUR3D cn);

RETOUR:      type  : int
	     role  : Type du contour Libido. #defini par le tableau suivant :

             #define CNT3D_FLOAT          (CNT3D | TY_FLOAT)          

Les autres, pas definis...

             #define CNT3D_CHAR           (CNT3D | TY_CHAR)           
             #define CNT3D_UCHAR          (CNT3D | TY_UCHAR)          
             #define CNT3D_SHORT          (CNT3D | TY_SHORT)          
             #define CNT3D_USHORT         (CNT3D | TY_USHORT)         
             #define CNT3D_LONG           (CNT3D | TY_LONG)           
             #define CNT3D_FLOAT          (CNT3D | TY_FLOAT)          
             #define CNT3D_DOUBLE         (CNT3D | TY_DOUBLE)         
                                                     

PARAMETRES:  nom   : cn
	     type  : Objet contour3D LibIDO 
	     role  : contour3D LibIDO a scruter

FICHIER:     idcnt3d.h

EXEMPLE:     

******************************************************** END DESCRIPTION */



/* FUNCTION DESCRIPTION **************************************************

IdCnt3DGetUsedNbPts  (macro)

RESUME:  donne le nb de points UTILISES d'un CONTOUR3D (PCONTOUR3D)

DESCRIPTION:  	donne le nb de points UTILISES d'un CONTOUR3D
	
SYNTAXE:     int nbPtsUtilises = IdCnt3DGetUsedNbPts (PCONTOUR3D cnt);

PARAMETRES:  nom   : ima
	     type  : PCONTOUR3D
	     acces : I, N
	     role  : Pointeur vers le contour source

FICHIER:     idcnt3d.h

EXEMPLE:

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdCnt3DSetY (macro)
  
RESUME: Positionne la valeur de la coord. Y d'un point d'un contour (PCONTOUR3D)

DESCRIPTION: Positionne la valeur de la coord. Y d'un point d'un contour. 
 Le point DOIT deja exister.
 
SYNTAXE:     void IdCntSetY(PCONTOUR3D cnt, int numPoint, nombre coordY);

RETOUR:      type  : rien
	     role  : 

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR3D
	     role  : contour d'origine.

	     nom   : i
	     type  : entier
	     role  : numero du point a modifier

	     nom   : val
	     type  : entier, ou double, ou ...
	     role  : coordonne Y du point a modifier

FICHIER:  idcnt3d.h  

EXEMPLE:     
******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdCnt3DSetX (macro)
  
RESUME: Positionne la valeur de la coord. X d'un point d'un contour3D (PCONTOUR3D)

DESCRIPTION: Positionne la valeur de la coordonnee X d'un point d'un contour3D. 
 Le point DOIT deja exister.
 
SYNTAXE:     void IdCnt3DSetX(PCONTOUR3D cnt, int numPoint , nombre coordX);

RETOUR:      type  : rien
	     role  : 

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR3D
	     role  : contour d'origine.

	     nom   : i
	     type  : entier
	     role  : numero du point a modifier

	     nom   : val
	     type  : entier, ou double, ou ...
	     role  : coordonne X du point a modifier

FICHIER:    idcnt3d.h

EXEMPLE:
     
******************************************************** END DESCRIPTION */



/* FUNCTION DESCRIPTION **************************************************

IdCnt3DSetZ (macro)
  
RESUME: Positionne la valeur de la coord. Z d'un point d'un contour3D (PCONTOUR3D)

DESCRIPTION: Positionne la valeur de la coordonnee Z d'un point d'un contour3D. 
 Le point DOIT deja exister.
 
SYNTAXE:     void IdCnt3DSetZ(PCONTOUR3D cnt, int numPoint , nombre coordX);

RETOUR:      type  : rien
	     role  : 

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR3D
	     role  : contour d'origine.

	     nom   : i
	     type  : entier
	     role  : numero du point a modifier

	     nom   : val
	     type  : entier, ou double, ou ...
	     role  : coordonnee Z du point a modifier

FICHIER:    idcnt3d.h

EXEMPLE:  
   
******************************************************** END DESCRIPTION */



/* FUNCTION DESCRIPTION **************************************************

IdCnt3DGetX (macro)
  
RESUME: Recupere la valeur de la coord. X d'un point d'un contour 3D (PCONTOUR3D)

DESCRIPTION: Recupere la valeur de la coordonnee X d'un point d'un contour 3D. 
 Le point DOIT deja exister.
 
SYNTAXE:     double valCoordX = IdCnt3DGetX(PCONTOUR3D cnt,int numPoint);

RETOUR:      type  : entier, ou double, ou ...
	     role  : 

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR3D
	     role  : contour d'origine.

	     nom   : i
	     type  : entier
	     role  : numero du point dont on veut la coordonnee X

FICHIER:    idcnt3d.h

EXEMPLE: 
    
******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdCnt3DGetY  (macro)
  
RESUME: Recupere la valeur de la coord. Y d'un point d'un contour 3D (PCONTOUR3D)

DESCRIPTION: Recupere la valeur de la coordonnee Y d'un point d'un contour 3D. 
 Le point DOIT deja exister.
 
SYNTAXE:    nombre valCoordy = IdCnt3DGetY(PCONTOUR3D cnt,int numPoint);

RETOUR:      type  : entier, ou double, ou ...
	     role  : 

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR3D
	     role  : contour d'origine.

	     nom   : i
	     type  : entier
	     role  : numero du point dont on veut la coordonnee y

FICHIER:    idcnt3d.h

EXEMPLE: 
    
******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdCnt3DGetZ  (macro)
  
RESUME: Recupere la valeur de la coord. Z d'un point d'un contour 3D (PCONTOUR3D)

DESCRIPTION: Recupere la valeur de la coordonnee Y d'un point d'un contour 3D. 
 Le point DOIT deja exister.
 
SYNTAXE:    nombre valCoordZ = IdCnt3DGetZ(PCONTOUR3D cnt,int numPoint);

RETOUR:      type  : entier, ou double, ou ...
	     role  : 

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR3D
	     role  : contour d'origine.

	     nom   : i
	     type  : entier
	     role  : numero du point dont on veut la coordonnee Z

FICHIER:    idcnt3d.h

EXEMPLE: 
    
******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdCnt3DModifyPoint (macro)
  
RESUME: Modifie la valeur des coord. X Y Z d'1 point d'un contour 3D (PCONTOUR3D)

DESCRIPTION: Positionne la valeur des coord. X Y d'un point d'un contour 3D. 
 Le point DOIT deja exister (aucun controle !!!).
 
SYNTAXE:     void IdCnt3DSetY(PCONTOUR3D cnt, int numPoint, nombre coordX, nombre coordY);

RETOUR:      type  : rien
	     role  : 

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR3D
	     role  : contour d'origine.

	     nom   : i
	     type  : entier
	     role  : numero du point a modifier

	     nom   : coordX, coordY, coordZ
	     type  : entier, ou double, ou ...
	     role  : coordonne Y du point a modifier

FICHIER:  idcnt.h  

EXEMPLE: 
    
******************************************************** END DESCRIPTION */

						

/* FUNCTION DESCRIPTION **************************************************

IdCnt3DAllocLikeContour  (macro)

RESUME: Alloc. d'1 contour 3D (PCONTOUR3D) de memes taille et type qu'1 autre

DESCRIPTION: Allocation d'un contour 3D de memes taille et meme type
	     qu'un autre.

SYNTAXE:     PCONTOUR3D cnt2 = IdCnt3DAllocLikeContour (PCONTOUR3D cnt1 );

RETOUR:      type  : PCONTOUR3D
	     role  : Pointeur vers le contour 3D alloue (a convertir en un type
		     valide : PCONTOUR3D_FLOAT par exemple ).


PARAMETRES:  nom   : contour1
	     type  : PCONTOUR3D, ou PCONTOUR3D_???
	     role  : Pointeur vers le contour 'modele'.

FICHIER:     idcnt3d.h

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdCnt3DSameSizeAndType (macro)

RESUME: Comparaison du type et de la taille de deux contours 3D (PCONTOUR3D).

DESCRIPTION: Comparaison du type et de la taille de deux contours 3D.

SYNTAXE:     int retCode = IdCnt3DSameSizeAndType(PCONTOUR3D cnt1, PCONTOUR3D cnt2 ); 

RETOUR:      type  : int
	     role  : Vrai si les deux contours EXISTENT et ont la meme taille et
		     le meme type.

PARAMETRES:  nom   : cnt1, cnt2
	     type  : PCONTOUR3D, ou PCONTOUR3D_???
	     role  : Pointeurs vers les contours a comparer.

FICHIER :    idcnt3d.h

******************************************************** END DESCRIPTION */




/* FUNCTION DESCRIPTION **************************************************

IdCnt3DSameSize (macro)

RESUME: Comparaison de la taille de deux contours 3D (PCONTOUR3D).

DESCRIPTION: Comparaison de la taille de deux contours 3D.

SYNTAXE:     int retCode = IdCnt3DSameSize(PCONTOUR3D cnt1, PCONTOUR3D cnt2); 

RETOUR:      type  : int
	     role  : Vrai si les deux contours EXISTENT et ont la meme taille

PARAMETRES:  nom   : cnt1, cnt2
	     type  : PCONTOUR3D, ou PCONTOUR3D_???
	     role  : Pointeurs vers les contours 3D a comparer.

FICHIER :    idcnt3d.h

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdCnt3DSameType (macro)

RESUME: Comparaison du type de deux contours 3D (PCONTOUR3D).

DESCRIPTION: Comparaison du type de deux contours 3D.

SYNTAXE:     int retCode = IdCnt3DSameType(PCONTOUR3D cnt1, PCONTOUR3D cnt2 ); 

RETOUR:      type  : int
	     role  : Vrai si les deux contours 3D EXISTENT et ont le meme type.

PARAMETRES:  nom   : cnt1, cnt2
	     type  : PCONTOUR3D, ou PCONTOUR3D_???
	     role  : Pointeurs vers les contours 3D a comparer.

FICHIER :    idcnt3d.h

******************************************************** END DESCRIPTION */

PCONTOUR3D IdCnt3DAlloc 		(int, int);

PCONTOUR3D IdCnt3DClear    		(PCONTOUR3D);
PCONTOUR3D IdCnt3DCopy     		(PCONTOUR3D,PCONTOUR3D);

#ifndef SWIG
#define IdCnt3DSameType(i1,i2)  ( (i1)&&(i2)                       	\
			       && (IdCnt3DType(i1)==IdCnt3DType(i2)) )

#define IdCnt3DFree(contour_)  				\
		_IdCnt3DFree((PCONTOUR3D*)&(contour_))

#define IdCnt3DDestroyNoControl(contour_) 		\
 		_IdCnt3DDestroyNoControl((PCONTOUR3D*)&(contour_))

#define IdCnt3DAddPoint(contour_,x,y,z,n)		\
		_IdCnt3DAddPoint(&(contour_), (x), (y), (z), (n) )

#define IdCnt3DAddPnt(contour_,p)			\
			_IdCnt3DAddPnt(&(contour_),(p) )
#define IdCnt3DInsertPnt(contour_,numPoint,p)		\
		_IdCnt3DInsertPnt(&(contour_),(numPoint),(p))

#define IdCnt3DInsertPoint(contour_,numPoint,x,y,z,n)	\
		_IdCnt3DInsertPoint(&(contour_),(numPoint), (x), (y), (z), (n) )
#endif

#ifdef SWIG
int 	IdCnt3DSameSizeAndType(PCONTOUR3D,PCONTOUR3D);
int 	IdCnt3DSameType(PCONTOUR3D,PCONTOUR3D);
int 	IdCnt3DSameSize(PCONTOUR3D,PCONTOUR3D);
void 	IdCnt3DDestroyNoControl (PCONTOUR3D);
int 	IdCnt3DAddPoint	(PCONTOUR3D_FLOAT, double, double, double, int);
int 	IdCnt3DAddPnt	(PCONTOUR3D_FLOAT, PIDPOINT);
int 	IdCnt3DInsertPnt(PCONTOUR3D_FLOAT, int, PIDPOINT); 
int 	IdCnt3DInsertPoint(PCONTOUR3D_FLOAT, int, double, double, double, int); 
PCONTOUR3D	IdCnt3DAllocLikeContour	(PCONTOUR3D);
void 	   IdCnt3DFree			(PCONTOUR3D);
/* int IdCnt3DClean	(PCONTOUR3D_FLOAT); */
#endif

PCONTOUR3D IdCnt3DVecteurToContour (PIDPOINT[], int, int, int );
PCONTOUR3D IdCnt3DModifLongueur    (PCONTOUR3D,int);
int        IdCnt3DMoveContour      (PCONTOUR3D_FLOAT, double , double, double);

int      IdCnt3DDelPoint 	(PCONTOUR3D,int); 
   


/* int    _IdCnt3DClean (PCONTOUR3D_FLOAT *); */
/* define IdCnt3DClean(contour_)  	_IdCnt3DClean(&(contour_)) */
/* Ou est passe le source de ce truc la ?!? JPR */

PCONTOUR3D 	IdCnt3DConcat (PCONTOUR3D,PCONTOUR3D); 

#endif
