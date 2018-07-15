/*************************************************************************
* $Id: idface.h,v 1.1 2005/09/09 08:22:29 bellet Exp $
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

/*************************************************************************
*
*  Description : Declarations permettant d'utiliser le type FAC
*                de la bibliotheque LIBIDO
*
**************************************************************************/
#ifndef __IDFACE_H__
#define __IDFACE_H__

#include "idgen.h"
#include "idpoint.h"

/* Types de face 3d */

#define FAC_FLOAT          (FAC | TY_FLOAT)

/* structure FACE */

typedef struct {
        int Number;
        IDPOINT *P1,*P2,*P3,*P4; /* deviendront des numero sur disque */
        float NormX,NormY,NormZ; /* normale */
        int NbDatas;
        float *Datas;/* pointeur vers des donnees texturales
                        (user specific)*/
        } FACE, *PFACE;

/***********************************************************************/
#include "idface-restricted.h"
/* prototypes de fonctions */

FACE *IdFacAddPnt (FACE *, IDPOINT *);


#define  IdFacFree(f)        	    _IdFacFree((PFACE*)&(f))
#define  IdFacTabFree(f,n,l)        _IdFacTabFree((PFACE*)&(f),(n),(l))

/*
 * #define IdFacSameFaces(f1,f2)  (     (f1).P1==(f2).P1    	\
 *				&&   (f1).P2==(f2).P2    	\
 *				&&   (f1).P3==(f2).P3 )
 */

/* JPR */

#define IdFacSameFaces(f1,f2)  (     (f1)->P1==(f2)->P1    	\
				&&   (f1)->P2==(f2)->P2    	\
				&&   (f1)->P3==(f2)->P3 )

#define  IdFacPntIn(f,p)         ( (PIDPOINT)(f)->P1==(p) 	\
				|| (PIDPOINT)(f)->P2==(p) 	\
				|| (PIDPOINT)(f)->P3==(p) )

#ifdef SWIG
void IdFacFree		(PFACE);
void IdFacTabFree	(PFACE *, int, int);
int  IdFacSameFaces	(PFACE,PFACE);
int  IdFacPntIn		(PFACE,PIDPOINT);
#endif

PFACE      IdFacAlloc      	(int);		/*nb datas*/
PFACE *    IdFacTabAlloc        (int,int);


/************************************************************************/

/* Fonctions internes */



/************************************************************************/


#endif



