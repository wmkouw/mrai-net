/*************************************************************************
* $Id: kernomfic.c,v 1.1 2005/09/09 08:22:51 bellet Exp $
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
*  Description : Gestion des noms de fichier stockes dans les objets
*
**************************************************************************/
#include <string.h>		// For strlen
#include <stdlib.h>
#include "idima.h"
#include "idvol.h"
#include "idsig.h"
#include "idprint.h"
#include "idseq.h"
#include "idcnt.h"
#include "idcnt3d.h"


/* FUNCTION DESCRIPTION **************************************************
 * * 
 * * IdGetFileName   (fonction)
 * * 
 * * RESUME: recupere le nom de fichier stocke ds un objet
 * * 
 * * DESCRIPTION: recupere le nom de fichier stocke ds un objet
 * * 
 * * SYNTAXE:     char *nomFichier = IdGetFileName(void *objet);
 * * 
 * * RETOUR:      type  : char *
 * *         role  : pointeur sur le nom du fichier. 0 si l'objet ne 
 * *            provient pas d'un fichier ou en cas d'erreur sur obj.
 * * 
 * * PARAMETRES:  
 * *         nom   : objet
 * *         type  : void *
 * *         role  : pointeur sur un objet libido provenant d'un fichier
 * * 
 * * FICHIER:     nomfic.c
 * * 
 * * EXEMPLE:
 * * 
 * ******************************************************** END DESCRIPTION */

char *IdGetFileName(void *obj)
{
	if (obj)
		switch (IdLibidoType(obj)) {
		case IMA:
			return (_IdImaPrivate(obj)->_fichier);
		case VOL:
			return (_IdVolPrivate(obj)->_fichier);
		case SIG:
			return (_IdSigPrivate(obj)->_fichier);
		case CNT:
			return (_IdCntPrivate(obj)->_fichier);
		case SEQ:
			return (_IdSeqPrivate(obj)->_fichier);
		case CNT3D:
			return (_IdCntPrivate(obj)->_fichier);
		}

	return ((char *) NULL);
}

/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdSetFileName   (fonction)
 * 
 * RESUME: positionne le nom de fichier d'un objet libido
 * 
 * DESCRIPTION: positionne le nom de fichier d'un objet libido
 * 
 * SYNTAXE:     int retCode = IdSetFileName(void *obj, char *nomFich);
 * 
 * RETOUR:      type  : entier 
 * role  : indicateur d'erreur
 * 
 * PARAMETRES:  
 * nom   : obj
 * type  : void * 
 * role  : pointeur sur l'objet libido
 * 
 * nom   : nf
 * type  : char * 
 * role  : nom de fichier
 * 
 * FICHIER:     nomfic.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */

int IdSetFileName(void *obj, char *nf)
{
	if (obj)
		switch (IdLibidoType(obj)) {
		case IMA:
			if (_IdImaPrivate(obj)->_fichier)
				free((_IdImaPrivate(obj)->_fichier));
			if (nf == 0)
				break;	/* JPR   */
			_IdImaPrivate(obj)->_fichier =
			    (char *) malloc(strlen(nf) + 1);
			strcpy(_IdImaPrivate(obj)->_fichier, nf);
			return (1);

		case VOL:
			if (_IdVolPrivate(obj)->_fichier)
				free((_IdVolPrivate(obj)->_fichier));
			if (nf == 0)
				break;	/* JPR   */
			_IdVolPrivate(obj)->_fichier =
			    (char *) malloc(strlen(nf) + 1);
			strcpy(_IdVolPrivate(obj)->_fichier, nf);
			return (1);

		case SIG:
			if (_IdSigPrivate(obj)->_fichier)
				free((_IdSigPrivate(obj)->_fichier));
			if (nf == 0)
				break;	/* JPR   */
			_IdSigPrivate(obj)->_fichier =
			    (char *) malloc(strlen(nf) + 1);
			strcpy(_IdSigPrivate(obj)->_fichier, nf);
			return (1);

		case CNT:
			if (_IdCntPrivate(obj)->_fichier)
				free((_IdCntPrivate(obj)->_fichier));
			if (nf == 0)
				break;	/* JPR   */
			_IdCntPrivate(obj)->_fichier =
			    (char *) malloc(strlen(nf) + 1);
			strcpy(_IdCntPrivate(obj)->_fichier, nf);
			return (1);

		case CNT3D:
			if (_IdCnt3DPrivate(obj)->_fichier)
				free((_IdCnt3DPrivate(obj)->_fichier));
			if (nf == 0)
				break;	/* JPR   */
			_IdCnt3DPrivate(obj)->_fichier =
			    (char *) malloc(strlen(nf) + 1);
			strcpy(_IdCnt3DPrivate(obj)->_fichier, nf);
			return (1);

		case SEQ:
			if (_IdSeqPrivate(obj)->_fichier)
				free((_IdSeqPrivate(obj)->_fichier));
			if (nf == 0)
				break;
			_IdSeqPrivate(obj)->_fichier =
			    (char *) malloc(strlen(nf) + 1);
			strcpy(_IdSeqPrivate(obj)->_fichier, nf);
			return (1);
		}
	return (0);
}


/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdGetMessage   (fonction)
 * 
 * RESUME: recupere le 'message' stocke ds un objet
 * 
 * DESCRIPTION: recupere le nom de fichier stocke ds un objet
 * 
 * SYNTAXE:     char *message = IdGetMessage(void *objet);
 * 
 * RETOUR:      type  : char *
 * role  : pointeur sur le 'message'. 0 si pas de 'message'
 * ou en cas d'erreur sur obj.
 * PARAMETRES:  
 * nom   : objet
 * type  : void *
 * role  : pointeur sur un objet libido 
 * 
 * FICHIER:     nomfic.c
 * 
 * EXEMPLE:
 * 
 * ******************************************************** END DESCRIPTION */


char *IdGetMessage(void *obj)
{
	if (obj)
		switch (IdLibidoType(obj)) {
		case IMA:
			return (_IdImaPrivate(obj)->_message);
		case VOL:
			return (_IdVolPrivate(obj)->_message);
		case SIG:
			return (_IdSigPrivate(obj)->_message);
		case SEQ:
			return (_IdSeqPrivate(obj)->_message);
		case CNT:
			return (_IdCntPrivate(obj)->_message);
		case CNT3D:
			return (_IdCnt3DPrivate(obj)->_message);
		default:
//			IdPrintf("IdGetMessage : l'objet passe n'a pas un type LibIDO connu\n");
			return((char *)NULL);
		}

	return (0);
}

/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdSetMessage   (fonction)
 * 
 * RESUME: positionne le 'message' dans un objet LibIDO
 * 
 * DESCRIPTION: positionne le 'message' dans un objet LibIDO
 * 
 * SYNTAXE:     int retCode = IdSetMessage(void *obj,char *nomFich);
 * 
 * RETOUR:      type  : entier 
 * role  : indicateur d'erreur
 * 
 * PARAMETRES:  
 * nom   : obj
 * type  : void * 
 * role  : pointeur sur l'objet libido
 * 
 * nom   : nf
 * type  : char * 
 * role  : 'message'
 * 
 * FICHIER:     nomfic.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */

int IdSetMessage(void *obj, char *nf)
{
	if (obj)
		switch (IdLibidoType(obj)) {
		case IMA:
			if (_IdImaPrivate(obj)->_message)
				free((_IdImaPrivate(obj)->_message));
			if (nf == 0)
				break;
			_IdImaPrivate(obj)->_message =
			    (char *) malloc(strlen(nf) + 1);
			strcpy(_IdImaPrivate(obj)->_message, nf);
			return (1);
		case VOL:
			if (_IdVolPrivate(obj)->_message)
				free((_IdVolPrivate(obj)->_message));
			if (nf == 0)
				break;
			_IdVolPrivate(obj)->_message =
			    (char *) malloc(strlen(nf) + 1);
			strcpy(_IdVolPrivate(obj)->_message, nf);
			return (1);
		case SIG:
			if (_IdSigPrivate(obj)->_message)
				free((_IdSigPrivate(obj)->_message));
			if (nf == 0)
				break;
			_IdSigPrivate(obj)->_message =
			    (char *) malloc(strlen(nf) + 1);
			strcpy(_IdSigPrivate(obj)->_message, nf);
			return (1);
		case SEQ:
			if (_IdSeqPrivate(obj)->_message)
				free((_IdSeqPrivate(obj)->_message));
			if (nf == 0)
				break;
			_IdSeqPrivate(obj)->_message =
			    (char *) malloc(strlen(nf) + 1);
			strcpy(_IdSeqPrivate(obj)->_message, nf);
			return (1);
		case CNT:
			if (_IdCntPrivate(obj)->_message)
				free((_IdCntPrivate(obj)->_message));
			if (nf == 0)
				break;
			_IdCntPrivate(obj)->_message =
			    (char *) malloc(strlen(nf) + 1);
			strcpy(_IdCntPrivate(obj)->_message, nf);
			return (1);
		case CNT3D:
			if (_IdCnt3DPrivate(obj)->_message)
				free((_IdCnt3DPrivate(obj)->_message));
			if (nf == 0)
				break;
			_IdCnt3DPrivate(obj)->_message =
			    (char *) malloc(strlen(nf) + 1);
			strcpy(_IdCnt3DPrivate(obj)->_message, nf);
			return (1);


		}
	return (0);
}
