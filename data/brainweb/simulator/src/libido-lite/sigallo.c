/*************************************************************************
* $Id: sigallo.c,v 1.1 2005/09/09 08:22:53 bellet Exp $
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
 * Allocation and liberation of SIGNAL data structure
 */
#include <string.h>
#include <stdlib.h>
#include "idsig.h"
#include "iderr.h"
#include "idprint.h"

void _IdSigFree(PSIGNAL * pi)
{
	if (*pi) {
		if (_IdSigPrivate(*pi)->_message)
			free((_IdSigPrivate(*pi)->_message));
		_IdSigPrivate(*pi)->_message = 0;
		if (_IdSigPrivate(*pi)->_fichier)
			free((_IdSigPrivate(*pi)->_fichier));
		_IdSigPrivate(*pi)->_fichier = 0;
		free(_IdSigPrivate(*pi));
		*pi = 0;
	}
}


/**
 * IdSigAlloc:
 * @sx: taille du signal a creer
 * @ty: type du signal a creer (SIG_xxx ex: SIG_FLOAT)
 *
 * Allocation memoire d'un objet libido de type SIGNAL. La
 * taille en  correspond a la dimension du signal. Le type est
 * SIG_xxx et peut prendre les valeurs du tableau des type
 * d'elements LIBIDO, par exemples SIG_UCHAR, SIG_FLOAT,
 * SIG_COMPLEX ...etc. Si l'allocation memoire a pu se faire
 * correctement, la fonction retourne une valeur non nulle de
 * type PISIGNAL. En cas d'erreur la fonction retourne 0. Il
 * est indispensable de tester cette valeur de retour avant
 * d'utiliser l'image. Il est conseille de faire un changement
 * de type sur la valeur de retour pour eviter les WARNINGS des
 * compilateurs.
 *
 * Return value: pointeur sur l'objet et les donnees signal
 */
PSIGNAL 
IdSigAlloc(int sx, int ty)
{
	PRIVATE_SIGNAL *si;
	PSIGNAL pi;

	if (!(ty & IDO_TYPE) == SIG) {
		IdErrPrintf
		    ("Allocation impossible. Objet de type incompatible.");
		IdErrno = IDERR_WRONG_TYPE;
		return ((PSIGNAL) 0);
	}

	si =
	    (PRIVATE_SIGNAL *) calloc(1, sizeof(PRIVATE_SIGNAL) +
					 sx * IdTypeSize(ty));
	if (si) {
		si->DimX = sx;
		si->Type = ty;
		si->_fichier = 0;
		si->_message = 0;
		pi = (PSIGNAL) & (si[1]);
	} else {
		IdErrPrintf
		    ("Echec d'allocation de signal. Memoire saturee.");
		IdErrno = IDERR_ALLOC_SIG;
		return ((PSIGNAL) 0);
	}
	return (pi);
}

/**
 * IdSigVecteurToSignal:
 * @vecteur: adresse du vecteur source
 * @ty: type du signal
 * @ox: Origine (ds le vecteur) du signal
 * @lx: Taille du signal
 *
 * Allocation d'un signal a partir d'une portion d'un Vecteur.
 *
 * Return value: Pointeur vers le signal resultat. Zero si echec.
 */
PSIGNAL 
IdSigVecteurToSignal(void *vecteur, int ty, int ox, int lx)
{
	PRIVATE_SIGNAL *si;
	PSIGNAL pi;

	if (!(ty & IDO_TYPE) == SIG) {
		IdErrPrintf
		    ("Allocation impossible. Objet de type (%x) incompatible.",
		     ty);
		return ((PSIGNAL) 0);
	}

	si =
	    (PRIVATE_SIGNAL *) calloc(1, sizeof(PRIVATE_SIGNAL) +
					 lx * IdTypeSize(ty));
	if (si) {
		si->DimX = lx;
		si->Type = ty;
		pi = (PSIGNAL) & (si[1]);
		memcpy(&si[1], ((char *) vecteur) + ox * IdTypeSize(ty),
		       lx * IdTypeSize(ty));
	} else {
		IdErrPrintf
		    ("Echec d'allocation de signal. Memoire saturee.");
		return ((PSIGNAL) 0);
	}
	return (pi);
}

/**
 * IdSigModifLongueur:
 * @sig: pointeur vers le signal
 * @nouvLong: nouvelle taille du signal
 *
 * Modif longueur d'un objet libido de type SIGNAL. 
 * Si la nouvelle longueur est < a l'ancienne, la fin est perdue.
 * Si la nouvelle longueur est > a l'ancienne, la partie rajoutee 
 * n'EST PAS initialisee   
 *               
 * Si l'allocation de la memoire supplememtaire a pu se faire
 * correctement, la fonction retourne une valeur non nulle de
 * type PISIGNAL. En cas d'erreur la fonction retourne 0. Il
 * est indispensable de tester cette valeur de retour avant
 * d'utiliser l'image. Il est conseille de faire un changement
 * de type sur la valeur de retour pour eviter les WARNINGS des
 * compilateurs.
 *
 * Return value: pointeur sur l'objet et les donnees signal, NULL si echec
 */
PSIGNAL 
IdSigModifLongueur(PSIGNAL sig, int nouvLong)
{
	PRIVATE_SIGNAL *si;
	PSIGNAL pi;

	if (IdLibidoType(sig) != SIG) {
		IdErrPrintf
		    ("Modif Longueur impossible. Objet non Signal.");
		return ((PSIGNAL) 0);
	}

	si = (PRIVATE_SIGNAL *) realloc(
					  (void *) (_IdSigPrivate(sig)),
					  sizeof(PRIVATE_SIGNAL) +
					  nouvLong * IdSizeOfType(sig)
	    );

	if (si) {
		si->DimX = nouvLong;
		pi = (PSIGNAL) & (si[1]);
	} else {
		IdErrPrintf
		    ("Echec Modif Longueur signal. Memoire saturee.");
		return ((PSIGNAL) 0);
	}

	return (pi);
}
