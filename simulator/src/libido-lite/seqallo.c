/*************************************************************************
* $Id: seqallo.c,v 1.1 2005/09/09 08:22:53 bellet Exp $
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

#include <stdio.h>
#include <stdlib.h>

#include "idseq-base.h"
#include "idima-private.h"

#define DEBUG 0

/*************************************************************************
*
*  Description : Allocation / Destruction d'une structure SEQuence compacte .
*
**************************************************************************/

/**
 * \ingroup  seq
 * \brief    Allocation d'une structure SEQuence compacte (seqxels consecutifs).
 * \warning  les sexels de cette SEQuence ne sont pas initialises.
 * @param sx X dimension of the sequence to allocate.
 * @param sy Y dimension of the sequence to allocate.
 * @param sz Z dimension of the sequence to allocate.
 * @param sb T (time) dimension of the sequence to allocate.
 * @param ty Type des seqxels
 * @return   pointeur sur la structure; NULL si echec
 */
 
PPPPSEQUENCE
IdSeqAlloc(int sx, int sy, int sz, int sb, int ty)
{
	PRIVATE_SEQUENCE *pseq;
	PPPPSEQUENCE pv = NULL;
	int ibloc, iplan, iligne;

	if (!(ty & IDO_TYPE) == SEQ) {
		printf
		    ("Allocation impossible. Objet de type incompatible.");
		return (PPPPSEQUENCE) 0;
	}

	pseq = (PRIVATE_SEQUENCE *)
	    calloc(1, sizeof(PRIVATE_SEQUENCE) + sb * sizeof(void ***));

	if (pseq ==NULL) {
			printf
		    ("Echec d'allocation PRIVATE_SEQUENCE.");
		    return (PPPPSEQUENCE) NULL;
	}
	
	if (DEBUG) printf("pseq %p\n",pseq);

	
	pseq->_fichier = 0;
	pseq->_message = 0;
	pv = (PPPPSEQUENCE) & pseq[1];

	IdSeqDimX(pv) = sx;
	IdSeqDimY(pv) = sy;
	IdSeqDimZ(pv) = sz;
	IdSeqDimT(pv) = sb;

	IdSeqUsedNbX(pv) = 0;
	IdSeqUsedNbY(pv) = 0;
	IdSeqUsedNbZ(pv) = 0;
	IdSeqUsedNbT(pv) = 0;

	IdSeqType(pv)   = ty;
	pseq->subObject = 0;
	
		/*
		 * Gestion du cas des sequences de bits
		 */
	sx = ((ty & TY_SIZE) == TY_BIT) ? ((sx + 7) >> 3) : sx;

	pv[0] = (void ***) malloc( sb * sz * sizeof(void *));
	
	if (pv[0] == NULL) {
		printf("Echec Alloc Sequence (pointeurs sur Plans)\n");
		free(pseq);
		return (PPPPSEQUENCE) NULL;
	}
	if (DEBUG) printf("IdSeqAlloc : pv[0] : %p\n", pv[0]);
	
	pv[0][0] = (void **) malloc( sb * sz * sy * sizeof(void *) );

	if (pv[0][0] == NULL) {
		printf("Echec Alloc Sequence (pointeurs sur Lignes)\n");
		free(pv[0]);
		free(pseq);		
		return (PPPPSEQUENCE) NULL;
	}
	
	if (DEBUG) printf("IdSeqAlloc : pv[0][0] : %p\n", pv[0][0]);

	pv[0][0][0] = (void *) malloc( sb * sz * sy * sx * IdSizeOfType(pv));

	if (pv[0][0][0] == NULL) {
		printf("Echec Alloc Sequence (seqxels)\n");
		free(pv[0][0]);				
		free(pv[0]);				
		free(pseq);
		return (PPPPSEQUENCE) NULL;
	}
	
	if (DEBUG) printf("IdSeqAlloc : pv[0][0][0] : %p\n", pv[0][0][0]);

	for (ibloc = 0; ibloc < sb; ibloc++) {
		pv[ibloc]= pv[0] + ibloc * sz;
		if (DEBUG) printf("ibloc : pv[%d] %p\n", ibloc, pv[ibloc]);
	}
			
	for (ibloc = 0; ibloc < sb; ibloc++) {	
		for (iplan = 0; iplan < sz; iplan++) {
			pv[ibloc][iplan]= pv[0][0] + (ibloc*sz + iplan) * sy;
			if (DEBUG) printf("	plan : pv[%d][%d] %p \n", ibloc, iplan,pv[ibloc][iplan]);
			
		}
	}

	for (ibloc = 0; ibloc < sb; ibloc++) {	
		for (iplan = 0; iplan < sz; iplan++) {
			for (iligne=0; iligne<sy; iligne++) {		
				pv[ibloc][iplan][iligne] = (( (char *)pv[0][0][0]) 
						+ ((ibloc*sz + iplan) * sy + iligne )*sx*IdSizeOfType(pv) );
				if (DEBUG) printf("		iligne :  pv[%d][%d][%d] %p\n", 
								ibloc, iplan, iligne, pv[ibloc][iplan][iligne]);
			}
		}
	}
	return (pv);
}


/**
 * \ingroup  seq
 * \brief    Liberation d'une SEQuence compacte (tous les blocs sont consecutifs en memoire)
 * @param pv SEQuence a liberer.
 */
void
_IdSeqFree(PPPPSEQUENCE * pv)
{
	if (*pv) {
		if (_IdSeqPrivate(*pv)->subObject == 0) {
			/*
			 *Liberation des seqxels si PAS un ss-objet 
			 */
			if(DEBUG) printf("Liberation des seqxels car PAS un ss-objet\n"); 
			free((*pv)[0][0][0]);
		}
		
		if(DEBUG) printf("Liberation Pointeurs Plans\n"); 		
		free((*pv)[0][0]);
		if(DEBUG) printf("Liberation Pointeurs Volumes\n"); 
		free((*pv)[0]);
		if(DEBUG) printf("fin Liberation Pointeurs Volumes\n");
		
		if (_IdSeqPrivate(*pv)->_message) {
			if(DEBUG) printf("Liberation _message\n"); 
			free((_IdSeqPrivate(*pv)->_message));
		}
			
		if (_IdSeqPrivate(*pv)->_fichier) {
			if(DEBUG) printf("Liberation _fichier\n"); 
			free((_IdSeqPrivate(*pv)->_fichier));
		}
		if(DEBUG) printf("Liberation _IdSeqPrivate\n"); 
		free(_IdSeqPrivate(*pv));
		*pv = 0;
	}
}
