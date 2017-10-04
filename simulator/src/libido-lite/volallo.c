/*************************************************************************
* $Id: volallo.c,v 1.1 2005/09/09 08:22:56 bellet Exp $
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

#include <stdlib.h>
#include "idvol.h"
#include "idprint.h"


#define DEBUG 0
 
/*************************************************************************
*
*  Description : Allocation / Destruction d'une structure VOLume compacte  
*
**************************************************************************/

/**
 * \ingroup  vol
 * \brief    Allocation d'une structure VOLUME compacte (voxels consecutifs)
 * @param sx  dimension X du volume a allouer.
 * @param sy  dimension Y du volume a allouer
 * @param sz  dimension X du volume a allouer.
 * @param ty Type des voxels
 * @return   pointeur sur la structure; NULL si echec
 */
PPPVOLUME
IdVolAlloc(int sx, int sy, int sz, int ty)
{
PRIVATE_VOLUME *pvol;
PPPVOLUME      pv  = NULL;
int plan, ligne;

	if (!(ty & IDO_TYPE) == VOL) {
		printf
		    ("Allocation impossible. Objet de type incompatible.");
		return (PPPVOLUME) NULL;
	}

	pvol = (PRIVATE_VOLUME *)
	    calloc(1, sizeof(PRIVATE_VOLUME) + sz * sizeof(char **));

	if (pvol == NULL) 	{
		printf("Echec Alloc Volume (partie privee)\n");
		free(pvol);
		return (PPPVOLUME) NULL;
	}
	if (DEBUG) printf("pvol %p\n",pvol);

	pvol->_fichier = 0;
	pvol->_message = 0;
	pv = (PPPVOLUME) & pvol[1];
	IdVolDimX(pv) = sx;
	IdVolDimY(pv) = sy;
	IdVolDimZ(pv) = sz;
	IdVolUsedNbX(pv) = 0;
	IdVolUsedNbY(pv) = 0;
	IdVolUsedNbZ(pv) = 0;
	IdVolType(pv) = ty;
	pvol->subObject = 0;
	
		/*
		 * Gestion du cas des volumes de bits
	ls 	 */
	sx = ((ty & TY_SIZE) == TY_BIT) ? ((sx + 7) >> 3) : sx;

	pv[0] = (void **) malloc( sz * sy * sizeof(void *));

	if (pv[0] == NULL) {
		printf("Echec Alloc Volume (pointeurs sur Lignes)\n");
		free(pvol);
		return (PPPVOLUME) NULL;
	}
	
	if (DEBUG) printf("pv[0] %p\n",pv[0]);

	pv[0][0] = (void *) malloc( sz * sy * sx * IdSizeOfType(pv));

	if (pv[0][0] == NULL) {
		printf("Echec Alloc Volume (voxels)\n");
		free(pvol);
		return (PPPVOLUME) NULL;
	}

	for (plan = 0; plan < sz; plan++) {
		pv[plan]= pv[0] + plan * sy;
		if (DEBUG) printf("\tpv[%d] %p\n",plan, pv[plan]);		
		for (ligne=0; ligne<sy; ligne++) {	
			pv[plan][ligne] = (((char *)pv[0][0]) + (plan*sy + ligne )*sx*IdSizeOfType(pv) );
			if (DEBUG) printf("\t pv[%d][%d] %p\n",plan, ligne, pv[plan][ligne]);			
		}
	}
return (pv);
}

 
/**
 * \ingroup  vol
 * \brief    Liberation d'un VOLume compact (tous les plans sont consecutifs en memoire)
 * @param pv Volume a liberer.
 */
void
_IdVolFree(PPPVOLUME * pv)
{
	if (*pv) {
		if (_IdVolPrivate(*pv)->subObject == 0) {
			/*
			 *Liberation des voxels si PAS un ss-objet 
			 */
			free((*pv)[0][0]);
		}
		free((*pv)[0]);

		if (_IdVolPrivate(*pv)->_message)
			free((_IdVolPrivate(*pv)->_message));
		//_IdVolPrivate(*pv)->_message = 0; // inutile
		if (_IdVolPrivate(*pv)->_fichier)
			free((_IdVolPrivate(*pv)->_fichier));
		//_IdVolPrivate(*pv)->_fichier = 0; // inutile

		free(_IdVolPrivate(*pv));
		*pv = 0;
	}
}
