/*************************************************************************
* $Id: idcnt-ido.h,v 1.1 2005/09/09 08:22:26 bellet Exp $
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
#ifndef __IDCNT_IDO_H__
#define __IDCNT_IDO_H__
#include "idgen.h"
#include "idima-base.h"

#define CNT_CHAR 	   (CNT | TY_CHAR)
#define CNT_UCHAR 	   (CNT | TY_UCHAR)
#define CNT_SHORT 	   (CNT | TY_SHORT)
#define CNT_USHORT 	   (CNT | TY_USHORT)                   
#define CNT_LONG 	   (CNT | TY_LONG)
#define CNT_ULONG 	   (CNT | TY_ULONG)
#define CNT_FLOAT 	   (CNT | TY_FLOAT)
#define CNT_DOUBLE 	   (CNT | TY_DOUBLE)
 			 
typedef		           void * PCONTOUR ;     
typedef           IDXPOINT_CHAR * PCONTOUR_CHAR ;                   
typedef  	 IDXPOINT_UCHAR * PCONTOUR_UCHAR  ;                 
typedef          IDXPOINT_FLOAT * PCONTOUR_FLOAT ;                  
typedef         IDXPOINT_DOUBLE * PCONTOUR_DOUBLE ;                            
typedef          IDXPOINT_SHORT * PCONTOUR_SHORT ;                  
typedef 	IDXPOINT_USHORT * PCONTOUR_USHORT ;                 
typedef           IDXPOINT_LONG * PCONTOUR_LONG ;
typedef 	 IDXPOINT_ULONG * PCONTOUR_ULONG ;

typedef struct {
          int   UsedNbX;	
          int   __dum;		// pour alignement frontiere Double Mot
	  int   DimX;
          int   Numero;
	  char  *_message,*_fichier;
	  int   BitsPerPixel;   // nombre de bits reellement utilises
	  int   Type;       	// IMPERATIVEMENT DERNIER DE LA STRUCTURE
} PRIVATE_CONTOUR;                  

// The following are unmatched by their SWIG counter-parts.

#include "idcnt-restricted.h"

#ifndef SWIG
#define _IdCntPrivate(cnt)         (((PRIVATE_CONTOUR*)(cnt))-1) 
   
#define IdCntType(cnt_)            ( _IdCntPrivate(cnt_)->Type) 
#define IdCntDimX(cnt_)            ( _IdCntPrivate(cnt_)->DimX)  
#define IdCntSetX(contour_,i,val)  (contour_)[(i)].x=(val)
#define IdCntSetY(contour_,i,val)  (contour_)[(i)].y=(val)
#define IdCntGetX(contour_,i)	   (contour_)[(i)].x
#define IdCntGetY(contour_,i)      (contour_)[(i)].y
#define IdCntModifyPoint(contour_,i,valx,valy)                              \
			(contour_)[(i)].x=(valx),(contour_)[(i)].y=(valy)
#define IdCntSameSizeAndType(i1,i2)  (     (i1)&&(i2)                       \
				       && (IdCntDimX(i1)==IdCntDimX(i2))    \
				       && (IdCntType(i1)==IdCntType(i2)) )
#define IdCntSameSize(i1,i2)  \
   ( (i1)&&(i2) && (IdCntDimX(i1)==IdCntDimX(i2)) )


////////////////////////////////////////////////////////////////////

#define IdCntNbPts(cnt_)           ( _IdCntPrivate(cnt_)->DimX) 
#define IdCntUsedNbX(cnt_)         ( _IdCntPrivate(cnt_)->UsedNbX)
#define IdCntUsedNbPts(cnt_)       ( _IdCntPrivate(cnt_)->UsedNbX)
#define IdCntSetUsedNbX(cnt_,nb)   ( _IdCntPrivate(cnt_)->UsedNbX=(nb))
#define IdCntSetUsedNbPts(cnt_,nb) ( _IdCntPrivate(cnt_)->UsedNbX=(nb))
#define IdCntGetUsedNbX(cnt_) 	   ( _IdCntPrivate(cnt_)->UsedNbX)
#define IdCntGetUsedNbPts(cnt_)    ( _IdCntPrivate(cnt_)->UsedNbX)
#endif

#ifdef SWIG
   int     IdCntNbPts(PCONTOUR);
   int     IdCntUsedNbX(PCONTOUR);
   int     IdCntUsedNbPts(PCONTOUR);
   int     IdCntSetUsedNbX(PCONTOUR,int);
   int     IdCntSetUsedNbPts(PCONTOUR,int);
   int     IdCntGetUsedNbX(PCONTOUR);
   int     IdCntGetUsedNbPts(PCONTOUR);
#endif



#endif
