/*************************************************************************
* $Id: imaallo.c,v 1.1 2005/09/09 08:22:44 bellet Exp $
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

#include "idima.h"
#include "idima-private.h"
#include "iderr.h"
#include "idprint.h"
#include <stdlib.h>

void _IdFreeLinesOfPlane(void **pi, int n);


/*************************************************************************
*
*  Description : Dynamic allocatio/free of IMAGE object
*
**************************************************************************/

void _IdFreePlanesOfBloc(void ***, int, int);

void _IdFreeLinesOfPlane(void **pi, int n)
{
#ifdef _TURBOC_
	int i;
	for (i = 0; i < n; i++)
		if (pi[i]) {
			free(pi[i]);
			pi[i] = 0;
		}
#else
	if (pi[0]) {
		free(pi[0]);
		pi[0] = 0;
	}
#endif
}

int _IdAllocLinesOfPlane(void *piv, int sx, int sy, int si)
{

	char **pi = (char **) piv;
	int i = 0;

#ifdef _TURBOC_
	for (i = 0; i < sy; i++) {
		if ((pi[i] = (char *) calloc(1, sx * si)) == 0) {
			_IdFreeLinesOfPlane((void **) pi, sy);
			return 0;
		}
	}

#else
	/*
	 * autres compilateurs sauf TURBOC ni _MSC_VER 
	 */
	if ((pi[0] = (char *) calloc(1, sx * sy * si)))
		for (i = 1; i < sy; i++)
			pi[i] = pi[0] + sx * si * i;
	else {
		IdErrPrintf("EchecAlloc ligne %d (lgr %d : %d*%d*%d).", i,
			    sx * sy * si, sx, sy, si);
		return 0;
	}

#endif
	return 1;
}

void **_IdAllocPlan(int sx, int sy, int si)
{
	char **pi;

	pi = (char **) calloc(sy, sizeof(void *));
	if (pi) {
		if (!_IdAllocLinesOfPlane(pi, sx, sy, si)) {
			IdErrPrintf
			    ("Echec _IdAllocLinesOfPlane. Memoire saturee.");
			free(pi);
			pi = (char **) 0;
		}
	}
	return ((void **) pi);
}

void _IdFreePlan(void **pi, int n)
{
	if (pi) {
		_IdFreeLinesOfPlane(pi, n);
		free(pi);
	}
}

// Special SEQUENCES
static int _IdAllocPlanesOfBloc(void *piv, int sx, int sy, int sz, int si)
{

	char ***pi = (char ***) piv;
	int i;
	for (i = 0; i < sz; i++) {
		if (0 == (pi[i] = (char **) _IdAllocPlan(sx, sy, si))) {
			IdErrPrintf
			    ("Echec Alloc Plan %d (sx %d sy %d si %d).", i,
			     sx, sy, si);
			_IdFreePlanesOfBloc((void ***) pi, sz, sy);
			return 0;
		}
	}
	return 1;
}

void _IdFreePlanesOfBloc(void ***pi, int nbp, int nbl)
{
	int i;
	for (i = 0; i < nbp; i++)
		if (pi[i]) {
			_IdFreeLinesOfPlane(pi[i], nbl);
			pi[i] = 0;
		}
}


void ***_IdAllocBloc(int sx, int sy, int sz, int si)
{
	char ***pi;

	pi = (char ***) calloc(sz, sizeof(void **));
	if (pi) {
		if (!_IdAllocPlanesOfBloc(pi, sx, sy, sz, si)) {
			IdErrPrintf
			    ("Echec Alloc dans  _IdAllocPlanesOfBloc (sx %d sy %d sz %d si %d",
			     sx, sy, sz, si);
			free(pi);
			pi = (char ***) 0;
		}

	} else
		IdErrPrintf("Echec Alloc tabPointeurs pour Bloc");

	return ((void ***) pi);
}


void _IdFreeBloc(void ***pi, int nbp, int nbl)
{
	if (pi) {
		_IdFreePlanesOfBloc(pi, nbp, nbl);
		free(pi);
	}
}

/**
 * \ingroup   ima
 * \brief     Free dynamically allocated PPIMAGE "object"
 * @param pi  Pointer to PPIMAGE to free.
 */
void _IdImaFree(PPIMAGE * pi)
{
	if (*pi) {
		if (_IdImaPrivate(*pi)->subObject == 0)
			/*
			 * On ne libere pas les lignes si sous-objet
			 */
			//_IdFreeLinesOfPlane(*pi, IdImaDimY(*pi));
			free((*pi)[0]);  // devrait aller + vite
								//  revoir l'utilite de _IdFreeLinesOfPlane

		if (_IdImaPrivate(*pi)->_message)
			free((_IdImaPrivate(*pi)->_message));
		_IdImaPrivate(*pi)->_message = 0;
		if (_IdImaPrivate(*pi)->_fichier)
			free((_IdImaPrivate(*pi)->_fichier));
		_IdImaPrivate(*pi)->_fichier = 0;
		free(_IdImaPrivate(*pi));
		*pi = 0;
	}
}

/**
 * \ingroup   ima
 * \brief     Dynamic allocation of PPIMAGE "object"
 * \warning   It is the callers responsability to check the pointer returned
 *            by IdImaAlloc.
 * @param sx  Width (horizontal dimension) of the image to be allocated
 * @param sy  Height (vertical dimension) of the image to be allocated
 * @param ty  Ido type of the image e.g. IMA_UCHAR, IMA_FLOAT, IMA_RGB...
 * @return    (PPIMAGE)0 on failure. Pointer (casted to PPIMAGE) of newly
 *            allocated image on success.
 */
PPIMAGE IdImaAlloc(int sx,	// Number of columns (width)
		   int sy,	// Number of lignes  (height)
		   int ty)
{
	PRIVATE_IMAGE *si;
	PPIMAGE pi = NULL;
	if (!(ty & IDO_TYPE) == IMA) {
		IdErrPrintf
		    ("Allocation impossible. Objet de type incompatible(%x)\n",
		     ty);
		IdErrno = IDERR_WRONG_TYPE;
		return (PPIMAGE) 0;
	}
	si =
	    (PRIVATE_IMAGE *) calloc(1, sizeof(PRIVATE_IMAGE) +
					sy * sizeof(void *));
	if (si) {
		si->DimX = sx;
		si->DimY = sy;
		si->Type = ty;
		si->subObject = 0;
		si->UsedNbX = 0;
		si->UsedNbY = 0;
		si->_fichier = 0;
		si->_message = 0;
		pi = (PPIMAGE) & (si[1]);
		if (ty == IMA_BIT) {
			sx = (sx + 7) >> 3;
			ty = IMA_UCHAR;
		}
		if (!_IdAllocLinesOfPlane(pi, sx, sy, IdSizeOfType(pi))) {
			IdImaFree(pi);
			IdErrPrintf
			    ("Echec d'allocation d'image. Memoire saturee.");
			IdErrno = IDERR_ALLOC_MEM;
			return (PPIMAGE) 0;
		}
	} else {
		IdErrPrintf
		    ("Echec d'allocation PRIVATE_IMAGE. Memoire saturee.");
	}
	return (pi);
}

/**
 * \ingroup   ima
 * \brief     Cloning (by using dynamic allocation) of PPIMAGE "object"
 *            IdImaClone is an alias of IdImaAllocLikeImage.
 * \warning   It is the callers responsability to check the pointer return
 *            by IdImaClone.
 * @param sim Source IMage to be cloned
 * @return    (PPIMAGE)0 on failure. Pointer (casted to PPIMAGE) of newly
 *            allocated image on success.
 */
PPIMAGE IdImaClone(PPIMAGE sim)
{
	return IdImaAllocLikeImage(sim);
}

/**
 * \ingroup   ima
 * \brief     Allocate a new PPIMAGE of same dimensions (width and height)
 *            and same type of the incoming argument.
 * \warning   It is the callers responsability to check the pointer return
 *            by IdImaClone.
 * @param sim Source IMage to be cloned
 * @return    (PPIMAGE)0 on failure. Pointer (casted to PPIMAGE) of newly
 *            allocated image on success.
 */
PPIMAGE IdImaAllocLikeImage (void * sim)
{
	if ( !sim ) {
		IdErrPrintf ("IdImaAllocLikeImage : can't clone NULL.");
		return (PPIMAGE)0;
	}
	return IdImaAlloc(IdImaDimX(sim), IdImaDimY(sim), IdImaType(sim));
}
