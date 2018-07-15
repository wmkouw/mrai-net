/*************************************************************************
* $Id: cntallo.c,v 1.1 2005/09/09 08:22:22 bellet Exp $
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
*  Description : Allocation et liberation structure et donnee CONTOUR
*
**************************************************************************/

#include <string.h>  // For memcpy
#include <stdlib.h>
#include "idcnt.h"
#include "idprint.h"
#include "iderr.h"

/* FUNCTION DESCRIPTION ****************************************************

IdCntFree    (macro)

RESUME: Liberation de la memoire occupee par un CONTOUR

DESCRIPTION: Liberation de la memoire occupee par un contour. La variable de
	     type PCONTOUR passe en parametre est mise a zero (voir macro
	     IdCntFree dans idcnt.h).

SYNTAXE:     void IdCntFree(PCONTOUR contour);

RETOUR:
	     Pas de valeur de retour.

PARAMETRES:
	     nom    : contour
	     type   : PCONTOUR
	     role   : contour a liberer.

FICHIER:     allocnt.c

********************************************************* END DESCRIPTION */

void _IdCntFree(PCONTOUR *pi)
{
if(*pi)
	{
	if(_IdCntPrivate(*pi)->_message)free((_IdCntPrivate(*pi)->_message));
	_IdCntPrivate(*pi)->_message=0;

	if(_IdCntPrivate(*pi)->_fichier)free((_IdCntPrivate(*pi)->_fichier));
	_IdCntPrivate(*pi)->_fichier=0;

	free(_IdCntPrivate(*pi));
	*pi=0;
	}
}

/* FUNCTION DESCRIPTION ****************************************************

IdCntAlloc (function)

RESUME: Allocation memoire d'un objet libido de type CONTOUR

DESCRIPTION: Allocation memoire d'un objet libido de type CONTOUR (CNT). La
	     taille correspond a la dimension du contour. Le type est
	     CNT_xxx et peut prendre les valeurs du tableau des types
	     d'elements LIBIDO, par exemple CNT_UCHAR, CNT_FLOAT, ...etc.
	     Si l'allocation memoire a pu se faire
	     correctement, la fonction retourne une valeur non nulle de
	     type PCONTOUR. En cas d'erreur la fonction retourne NULL. Il
	     est indispensable de tester cette valeur de retour avant
	     d'utiliser le CNT. Il est conseille de faire un changement
	     de type sur la valeur de retour pour eviter les WARNINGS des
	     compilateurs.
	     Voir: idcnt.h : types et constantes.

SYNTAXE:     PCONTOUR cnt1 = IdCntAlloc ( int taille, int typeCnt);

RETOUR:	     nom    : cnt1
	     type   : PCONTOUR
	     role   : pointeur sur l'objet CNT

PARAMETRES:
	     nom    : taille
	     type   : int
	     role   : taille du contour a creer

	     nom    : typeCnt
	     type   : int
	     role   : type du contour a creer (CNT_xxx ex: CNT_FLOAT)

FICHIER:     allocnt.c

********************************************************* END DESCRIPTION */

PCONTOUR IdCntAlloc(int sx, int ty)
		/* retourne PCONTOUR si OK, 0 si erreur */
		/* ty = type de donnees: CNT_FLOAT ...  */
{
  PRIVATE_CONTOUR *si;
  PCONTOUR pi;

  if(!(ty & IDO_TYPE)==CNT) {
    IdErrPrintf("Allocation impossible. Objet de type incompatible.");
    IdErrno=IDERR_WRONG_TYPE;
    return((PCONTOUR)0);
  }

  si=(PRIVATE_CONTOUR *)calloc(1, sizeof(PRIVATE_CONTOUR) + sx * IdTypeSize(ty));
  if(si){
    si->DimX=sx;
    si->Type=ty;
    si->_fichier=0;  /* JPR */
    si->_message=0;  /* JPR */
    pi=(PCONTOUR)&(si[1]);
  }else{
    IdErrPrintf("Echec d'allocation de contour. Memoire saturee.");
    IdErrno=IDERR_ALLOC_CNT;
    return((PCONTOUR)0);  /* JPR */
   }

  return(pi);
}

/* FUNCTION DESCRIPTION **************************************************

IdCntVecteurToContour   (fonction)

RESUME: Alloc. d'1 CNT a partir d'1 morceau d'1 Vecteur de points IDXPOINT 

DESCRIPTION: Allocation d'un CNT a partir d'une portion d'un Vecteur de points IDXPOINT.

SYNTAXE:     PCONTOUR cnt1 = IdCntVecteurToContour (void * VectSource, int typeCnt, int origineX, int lgrX);

RETOUR:      nom   : cnt1 
	     type  : PCONTOUR
	     role  : Pointeur vers le contour resultat. Zero si echec.

PARAMETRES:  nom   : VectSource
	     type  : void *
	     role  : adresse du vecteur source

	     nom   : typeCnt
	     type  : int
	     role  : type du contour.		    

	     nom   : origineX
	     type  : int
	     role  : Origine (ds le vecteur) du contour.

	     nom   : lgrX
	     type  : int
	     role  : nb de points du contour.

FICHIER:    allocnt.c

EXEMPLE:

     cnts=(PCONTOUR_UCHAR)IdCntVecteurToContour(im(i),CNT_UCHAR,0,IdCntDimX(im))	 
      
******************************************************** END DESCRIPTION */

PCONTOUR 
IdCntVecteurToContour (void *vecteur, int ty, int ox, int lx)
{
  PRIVATE_CONTOUR *si;
  PCONTOUR pi;

  if(!(ty & IDO_TYPE)==CNT) {
    IdErrPrintf("Allocation impossible. Objet de type (%x) incompatible.",ty);
    IdErrno=IDERR_WRONG_TYPE;
    return((PCONTOUR)0);
  }

  si=(PRIVATE_CONTOUR *)calloc(1, sizeof(PRIVATE_CONTOUR) + lx * IdTypeSize(ty));
	if(si){
		si->DimX=lx;
		si->Type=ty;
		pi=(PCONTOUR)&(si[1]);
		memcpy(&si[1],((char*)vecteur)+ox* IdTypeSize(ty),
	        	lx * IdTypeSize(ty));
  	} else {
    		IdErrPrintf("Echec d'allocation de contour. Memoire saturee.");
		IdErrno=IDERR_ALLOC_CNT;
                return((PCONTOUR)0);
	}
	return(pi);
}

/* FUNCTION DESCRIPTION ****************************************************
* 
* IdCntModifLongueur (function)
* 
* RESUME: Modif longueur d'un objet libido de type CONTOUR
* 
* DESCRIPTION: CETTE FONCTION NE DOIT PAS ETRE UTILISEE
* 		(IdCntAddPoint modifie la longueur du CNT s'il y a besoin
* 		 et met a jour le nb de points utilises !!!)
* 
* 		Modif longueur d'un objet libido de type CONTOUR. 
* 		Si la nouvelle longueur est < a l'ancienne, la fin est perdue.
* 		Si la nouvelle longueur est > a l'ancienne, la partie rajoutee 
* 		n'EST PAS initialisee	
* 		
* 	     Si l'allocation de la memoire supplementaire a pu se faire
* 	     correctement, la fonction retourne une valeur non nulle de
* 	     type PCONTOUR. En cas d'erreur la fonction retourne 0. Il
* 	     est indispensable de tester cette valeur de retour avant
* 	     d'utiliser l'image. Il est conseille de faire un changement
* 	     de type sur la valeur de retour pour eviter les WARNINGS des
* 	     compilateurs.
* 
* SYNTAXE:   PCONTOUR cnt = IdCntModifLongueur ( PCONTOUR cnt,int nouvelleTaille);
* 
* RETOUR:
* 	     nom    : cnt
* 	     type   : PCONTOUR
* 	     role   : pointeur sur l'objet CNT
* 		      NULL si echec
* 
* PARAMETRES:
* 	     nom   : cnt
* 	     type  : PCONTOUR
* 	     role  : Pointeur vers le contour dont on veut modifier la taille.
* 
* 	     nom    : nouvelleTaille
* 	     type   : int
* 	     role   : nouvelle taille du contour
* 
* FICHIER:     allocnt.c
* 
********************************************************* END DESCRIPTION */

PCONTOUR IdCntModifLongueur (PCONTOUR cnt, int nouvLong)
		/* retourne PCONTOUR si OK 0 si erreur */
{
  PRIVATE_CONTOUR *si;
  PCONTOUR pi;

  if(IdLibidoType(cnt)!=CNT) {
    IdErrPrintf("Modif Longueur impossible. Objet non CONTOUR.");
    IdErrno=IDERR_WRONG_TYPE;
    return((PCONTOUR)0);
  }
  
 si=(PRIVATE_CONTOUR *)realloc(
(void *)( _IdCntPrivate(cnt)),
	 sizeof(PRIVATE_CONTOUR) + nouvLong * IdSizeOfType(cnt)
	);

  if(si){
    si->DimX=nouvLong;
    pi=(PCONTOUR)&(si[1]);

  }else{
    IdErrPrintf("Echec Modif Longueur contour. Memoire saturee.");
    IdErrno=IDERR_ALLOC_CNT;
    return((PCONTOUR)0);  
   }

  return(pi);
}
