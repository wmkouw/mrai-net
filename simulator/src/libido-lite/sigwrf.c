/*************************************************************************
* $Id: sigwrf.c,v 1.1 2005/09/09 08:22:55 bellet Exp $
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
 * enregistrement d'un signal dans un fichier format RAW
 */
#include <stdio.h>
#include "idsig.h"
#include "iderr.h"
#include "idio.h"

/**
 * IdSigWriteRawFile:
 * @nomfic : Pointeur vers le nom complet du fichier de sauvegarde.
 * @im: Pointeur vers la zone et la structure de description
 * du signal a sauver.
 *
 * Sauve dans un fichier un signal au format RAW.
 *
 * Return value: Indicateur d'erreur: 1 : OK, 0 : Pb.
 */
int 
IdSigWriteRawFile(char *nomfic, PSIGNAL im)
{

	FILE *fp;
	if (!im) {
		IdErrno = 1;
		return (1);
	}
	fp = fopen(nomfic, ID_WFILE_BIN);
	if (!fp) {
		IdErrno = 2;
		return (1);
	}
	fwrite(im, IdSizeOfType(im), IdSigDimX(im), fp);
	fclose(fp);
	return (0);
}
