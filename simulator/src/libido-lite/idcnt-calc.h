/*************************************************************************
* $Id: idcnt-calc.h,v 1.1 2005/09/09 08:22:26 bellet Exp $
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

/*
 *  Description : CoNTour object (bi dimensional)
 */
#ifndef __IDCNT_CALC_H__
#define __IDCNT_CALC_H__

#include "idgen.h"
#include "idima.h"


#ifndef SWIG
#define OUTPUT
#endif

PCONTOUR_USHORT IdSigBsplineInterpCompCtrlPts     (PCONTOUR_USHORT);
////////////////  ASCII
PCONTOUR IdCntReadAscFileWithOffset	 (char *,int,int,int,int); 
int 	 IdCntReadAscFileToCntWithOffset (char *,PCONTOUR,int,int);
int 	 IdCntGetAscFileLength           (char *,int,int);
int	 IdCntWriteAsciiFile             (char *,PCONTOUR);
////////////////  RAW
PCONTOUR IdCntReadRawFile                (char *, int, int);
int      IdCntReadRawFileToCnt           (char *, PCONTOUR);
PCONTOUR IdCntReadRawFileWithOffset      (char *, int, int, int);
int      IdCntReadRawFileToCntWithOffset (char *, PCONTOUR, int);
////////////////
int      IdCntGetRawFileLength           (char *, int);
int      IdCntWriteRawFile               (char *,PCONTOUR);
int      IdCntDelPoint                   (PCONTOUR, int);
int      IdCntDelPoints                  (PCONTOUR, int, int);
PCONTOUR IdCntVecteurToContour          (void *, int, int, int );
PCONTOUR IdCntModifLongueur             (PCONTOUR,int);
void     IdCntRectEnglob                (PCONTOUR_USHORT, int * OUTPUT,
                                         int * OUTPUT, int * OUTPUT, int * OUTPUT);
int      IdCntMoveContour               (PCONTOUR_USHORT, int , int);
void     IdCntCentreGravite             (PCONTOUR_USHORT contour, int * OUTPUT,
				         int * OUTPUT);
int      IdCntFillPolygon               (PCONTOUR_USHORT, PPIMAGE_UCHAR, int);
int      IdCntFillPolygonXXX            (PCONTOUR_USHORT, PPIMAGE_UCHAR, int);
int      IdCntFillExtPolygon            (PCONTOUR_USHORT, PPIMAGE_UCHAR, int);
PCONTOUR_USHORT
         IdCntPointsOfSegment           (int, int, int, int);
double   IdCntSurfacePoly               (PCONTOUR_USHORT);
PCONTOUR_USHORT
         IdCntSignalToCntImage          (PSIGNAL,int,int,int,int,int,
                                         double,double);
void    IdCntCentreGraviteG             (PCONTOUR, double * OUTPUT,
                                         double * OUTPUT);
double  IdCntCalculAireG                (PCONTOUR);
double  IdCntGetXG                      (PCONTOUR, int);
double  IdCntGetYG                      (PCONTOUR, int);
int     IdCntRotateContour              (PCONTOUR_USHORT, int, int, double); 
int     IdCntRotateContourDouble        (PCONTOUR_DOUBLE, int, int, double);
int     IdCntZoomContourDouble          (PCONTOUR_DOUBLE, int, int, double); 
PCONTOUR 	IdCntConcat             (PCONTOUR, PCONTOUR); 
PCONTOUR_USHORT	IdCntReadContour        (char *); 
//CLEANME void 		IdCntSwap	        (PCONTOUR,int);

int IdImaFillContour(PPIMAGE_UCHAR im, int nivGris);


////////////////////////////////////////////////////////////////////

#ifndef SWIG
#define IdCntAppendPointsOfSegment(cntSource, x, y)             \
    IdCntAddPointsOfSegment ((cntSource),                       \
    IdCntGetX(cntSource,IdCntGetUsedNbX(cntSource)-1),          \
    IdCntGetY(cntSource,IdCntGetUsedNbX(cntSource)-1),(x),(y));
#endif
#ifdef SWIG
int IdCntAppendPointsOfSegment(PCONTOUR_USHORT, int, int);
#endif

#ifndef SWIG
#define IdCntAllocLikeContour(cn) ((!(cn)) ? 0       \
	: IdCntAlloc ( IdCntDimX(cn),IdCntType(cn) ))
#else
PCONTOUR IdCntAllocLikeContour(PCONTOUR);
#endif

/////////////// ASCII
	
#ifndef SWIG
#define  IdCntReadAscFile(a,b,c) \
   IdCntReadAscFileWithOffset((a), (b), (c), 0, 0)
#define  IdCntReadAscFileToCnt(a,b) \
   IdCntReadAscFileToCntWithOffset((a), (b), 0, 0)
#else
   int IdCntReadAscFile(char *,int,int);
   int IdCntReadAscFileToCnt(char *,PCONTOUR);
#endif


int IdCntDrawPolyLine       (PCONTOUR_USHORT,PPIMAGE_UCHAR,int,int);
int IdCntDrawPolygon        (PCONTOUR_USHORT,PPIMAGE_UCHAR,int,int);
int IdCntDrawPolyLineOffset (PCONTOUR_USHORT,int,int,PPIMAGE_UCHAR,int,int);
int IdCntDrawPolygonOffset  (PCONTOUR_USHORT,int,int,PPIMAGE_UCHAR,int,int);

#ifndef SWIG
#define IdCntAddPointsOfSegment(contour_,a,b,c,d)  		\
	_IdCntAddPointsOfSegment(&(contour_),(a),(b),(c),(d))
#else
int IdCntAddPointsOfSegment(PCONTOUR_USHORT, int, int, int, int);
#endif

#ifndef SWIG
#define IdCntInsertPointsOfSegment(contour_,a,b,c) 	\
 	_IdCntInsertPointsOfSegment(&(contour_),(a),(b),(c))
#else
int IdCntInsertPointsOfSegment(PCONTOUR_USHORT, int, int, int);
#endif

#ifndef SWIG
#define IdCntConnexPts(contour_,a)  	_IdCntConnexPts(&(contour_),(a)) 
#else
int IdCntConnexPts(PCONTOUR_USHORT, int);
#endif

#ifndef SWIG
#define IdCntClean(contour_)  	_IdCntClean(&(contour_))
#else
int    IdCntClean (PCONTOUR_USHORT);
#endif

#ifndef SWIG
#define IdCntCleanFromStart(contour_, i)  	_IdCntCleanFromStart(&(contour_), (i))
#else
int IdCntCleanFromStart(PCONTOUR_USHORT, int);
#endif

#ifndef SWIG
#define  IdCntAddPointG(contour,valx,valy) \
           _IdCntAddPointG((PCONTOUR*)&(contour), (valx),(valy))
#else
int IdCntAddPointG(PCONTOUR, double, double);
#endif



// FUNCTION DESCRIPTION **************************************************
// 
// IdCntNbPts  (macro)
// 
// RESUME:  donne le points d'un CONTOUR (identique a IdCntDimX ...)
// 
// DESCRIPTION:  	donne le points d'un CONTOUR (identique a IdCntDimX ...)
// 		(celui precise lors de l'allocation ou de la modif de longueur)
// 	
// SYNTAXE:     int nbrPts = IdCntNbPts (PCONTOUR cnt1);
// 
// PARAMETRES:  nom   : cnt1
// 	     type  : PCONTOUR
// 	     role  : Pointeur vers le contour source
// 
// FICHIER:     idcnt.h
// 
// EXEMPLE:
// 
// ******************************************************** END DESCRIPTION
// 
// FUNCTION DESCRIPTION **************************************************
// 
// IdCntType (macro)
// 
// RESUME: Donne le type d'un contour LibIDO.
// 
// DESCRIPTION: Donne le type d'un contour LibIDO.
// 
// SYNTAXE:     int typ_cn = IdCntType (PCONTOUR cn);
// 
// RETOUR:      type  : int
// 	     role  : Type du contour Libido. #defini par le tableau suivant :
// 
//              #define CNT_CHAR           (CNT | TY_CHAR)           
//              #define CNT_UCHAR          (CNT | TY_UCHAR)          
//              #define CNT_SHORT          (CNT | TY_SHORT)          
//              #define CNT_USHORT         (CNT | TY_USHORT)         
//              #define CNT_LONG           (CNT | TY_LONG)           
//              #define CNT_FLOAT          (CNT | TY_FLOAT)          
//              #define CNT_DOUBLE         (CNT | TY_DOUBLE)         
//                                                      
// PARAMETRES:  nom   : im
// 	     type  : Objet contour LibIDO 
// 	     role  : contour LibIDO a scruter
// 
// FICHIER:     idcnt.h
// 
// EXEMPLE:     
// 
// ******************************************************** END DESCRIPTION
//
// FUNCTION DESCRIPTION **************************************************
// 
// IdCntGetUsedNbPts  (macro)
// 
// RESUME:  donne le nb de points UTILISES d'un CONTOUR
// 
// DESCRIPTION:  	donne le nb de points UTILISES d'un CONTOUR
// 	
// SYNTAXE:     int nbPtsUtilises = IdCntGetUsedNbPts (PCONTOUR cnt);
// 
// PARAMETRES:  nom   : ima
// 	     type  : PCONTOUR
// 	     role  : Pointeur vers le contour source
// 
// FICHIER:     idcnt.h
// 
// EXEMPLE:
// 
// ******************************************************** END DESCRIPTION
//
// FUNCTION DESCRIPTION **************************************************
// 
// IdCntSetY  (macro)
//   
// RESUME: Positionne la valeur de la coord. Y d'un point d'un contour
// 
// DESCRIPTION: Positionne la valeur de la coord. Y d'un point d'un contour. 
//  Le point DOIT deja exister.
//  
// SYNTAXE:     void IdCntSetY(PCONTOUR cnt, int numPoint, nombre coordY);
// 
// RETOUR:      type  : rien
// 	     role  : 
// 
// PARAMETRES:
// 	     nom   : cnt
// 	     type  : PCONTOUR
// 	     role  : contour d'origine.
// 
// 	     nom   : i
// 	     type  : entier
// 	     role  : numero du point a modifier
// 
// 	     nom   : val
// 	     type  : entier, ou double, ou ...
// 	     role  : coordonne Y du point a modifier
// 
// FICHIER:  idcnt.h  
// 
// EXEMPLE:     
// ******************************************************** END DESCRIPTION
// 
// FUNCTION DESCRIPTION **************************************************
// 
// IdCntSetX  (macro)
//   
// RESUME: Positionne la valeur de la coord. X d'un point d'un contour
// 
// DESCRIPTION: Positionne la valeur de la coordonnee X d'un point d'un contour. 
//  Le point DOIT deja exister.
//  
// SYNTAXE:     void IdCntSetX(PCONTOUR cnt, int numPoint , nombre coordX);
// 
// RETOUR:      type  : rien
// 	     role  : 
// 
// PARAMETRES:
// 	     nom   : cnt
// 	     type  : PCONTOUR
// 	     role  : contour d'origine.
// 
// 	     nom   : i
// 	     type  : entier
// 	     role  : numero du point a modifier
// 
// 	     nom   : val
// 	     type  : entier, ou double, ou ...
// 	     role  : coordonne X du point a modifier
// 
// FICHIER:    idcnt.h
// 
// EXEMPLE:     
// ******************************************************** END DESCRIPTION
// 
// FUNCTION DESCRIPTION **************************************************
// 
// IdCntGetX (macro)
//   
// RESUME: Recupere la valeur de la coord. X d'un point d'un contour
// 
// DESCRIPTION: Recupere la valeur de la coordonnee X d'un point d'un contour. 
//  Le point DOIT deja exister.
//  
// SYNTAXE:     nombre valCoordX = IdCntGetX(PCONTOUR cnt,int numPoint);
// 
// RETOUR:      type  : entier, ou double, ou ...
// 	     role  : 
// 
// PARAMETRES:
// 	     nom   : cnt
// 	     type  : PCONTOUR
// 	     role  : contour d'origine.
// 
// 	     nom   : i
// 	     type  : entier
// 	     role  : numero du point dont on veut la coordonnee X
// 
// FICHIER:    idcnt.h
// 
// EXEMPLE: 
//     
// ******************************************************** END DESCRIPTION
//
// FUNCTION DESCRIPTION **************************************************
// 
// IdCntGetY  (macro)
//   
// RESUME: Recupere la valeur de la coord. Y d'un point d'un contour
// 
// DESCRIPTION: Recupere la valeur de la coordonnee Y d'un point d'un contour. 
//  Le point DOIT deja exister.
//  
// SYNTAXE:    nombre valCoordy = IdCntGetY(PCONTOUR cnt,int numPoint);
// 
// RETOUR:      type  : entier, ou double, ou ...
// 	     role  : 
// 
// PARAMETRES:
// 	     nom   : cnt
// 	     type  : PCONTOUR
// 	     role  : contour d'origine.
// 
// 	     nom   : i
// 	     type  : entier
// 	     role  : numero du point dont on veut la coordonnee y
// 
// FICHIER:    idcnt.h
// 
// EXEMPLE: 
//     
// ******************************************************** END DESCRIPTION
//
// FUNCTION DESCRIPTION **************************************************
// 
// IdCntModifyPoint  (macro)
//   
// RESUME: Modifie la valeur des coord. X Y d'1 point d'un contour
// 
// DESCRIPTION: Positionne la valeur des coord. X Y d'un point d'un contour. 
//  Le point DOIT deja exister (aucun controle !!!).
//  
// SYNTAXE:     void IdCntSetY(PCONTOUR cnt, int numPoint, nombre coordX, nombre coordY);
// 
// RETOUR:      type  : rien
// 	     role  : 
// 
// PARAMETRES:
// 	     nom   : cnt
// 	     type  : PCONTOUR
// 	     role  : contour d'origine.
// 
// 	     nom   : i
// 	     type  : entier
// 	     role  : numero du point a modifier
// 
// 	     nom   : coordX, coordY
// 	     type  : entier, ou double, ou ...
// 	     role  : coordonne Y du point a modifier
// 
// FICHIER:  idcnt.h  
// 
// EXEMPLE:     
// ******************************************************** END DESCRIPTION
// 				
// FUNCTION DESCRIPTION **************************************************
// 
// IdCntAllocLikeContour  (macro)
// 
// RESUME: Alloc. d'1 contour de memes taille et type qu'1 autre
// 
// DESCRIPTION: Allocation d'un contour de memes taille et meme type
// 	     qu'un autre.
// 
// SYNTAXE:     PCONTOUR cnt2 = IdCntAllocLikeContour (PCONTOUR cnt1 );
// 
// RETOUR:      type  : PCONTOUR
// 	     role  : Pointeur vers le contour alloue (a convertir en un type
// 		     valide : PCONTOUR_UCHAR par exemple ).
// 
// PARAMETRES:  nom   : contour1
// 	     type  : PCONTOUR, ou PCONTOUR_???
// 	     role  : Pointeur vers le contour 'modele'.
// 
// FICHIER:     idcnt.h
// 
// ******************************************************** END DESCRIPTION
// 
// FUNCTION DESCRIPTION **************************************************
// 
// IdCntSameSizeAndType (macro)
// 
// RESUME: Comparaison du type et de la taille de deux contours.
// 
// DESCRIPTION: Comparaison du type et de la taille de deux contours.
// 
// SYNTAXE:     int retCode = IdCntSameSizeAndType(PCONTOUR cnt1, PCONTOUR cnt2 ); 
// 
// RETOUR:      type  : int
// 	     role  : Vrai si les deux contours EXISTENT et ont la meme taille et
// 		     le meme type.
// 
// PARAMETRES:  nom   : cnt1, cnt2
// 	     type  : PCONTOUR, ou PCONTOUR_???
// 	     role  : Pointeurs vers les contours a comparer.
// 
// FICHIER :    idcnt.h
// 
// ******************************************************** END DESCRIPTION
// 
// FUNCTION DESCRIPTION **************************************************
// 
// IdCntSameSize (macro)
// 
// RESUME: Comparaison de la taille de deux contours.
// 
// DESCRIPTION: Comparaison de la taille de deux contours.
// 
// SYNTAXE:     int retCode = IdCntSameSize(PCONTOUR cnt1, PCONTOUR cnt2 ); 
// 
// RETOUR:      type  : int
// 	     role  : Vrai si les deux contours EXISTEMNT et ont la meme taille
// 
// PARAMETRES:  nom   : cnt1, cnt2
// 	     type  : PCONTOUR, ou PCONTOUR_???
// 	     role  : Pointeurs vers les contours a comparer.
// 
// FICHIER :    idcnt.h
// 
// ******************************************************** END DESCRIPTION
// 
// FUNCTION DESCRIPTION **************************************************
// 
// IdCntSameType (macro)
// 
// RESUME: Comparaison du type de deux contours.
// 
// DESCRIPTION: Comparaison du type de deux contours.
// 
// SYNTAXE:     int retCode = IdCntSameType(PCONTOUR cnt1, PCONTOUR cnt2 ); 
// 
// RETOUR:      type  : int
// 	     role  : Vrai si les deux contours EXISTENT et ont le meme type.
// 
// PARAMETRES:  nom   : cnt1, cnt2
// 	     type  : PCONTOUR, ou PCONTOUR_???
// 	     role  : Pointeurs vers les contours a comparer.
// 
// FICHIER :    idcnt.h
// 
// ******************************************************** END DESCRIPTION


#endif
