/*************************************************************************
* $Id: volwrf.c,v 1.1 2005/09/09 08:22:57 bellet Exp $
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
 * Write a volume in a RAW formatted file.
 */
#include <stdio.h>
#include "idvol.h"
#include "idio.h"

/**
 * \ingroup      vol
 * \brief        Save a Volume object to file with RAW format.
 * @param nomfic Pointer to the complete name of the file to write to.
 * @param vo     Pointer to the zone and the structure of the volume
 *               to be written.
 * @return       1 on success, 0 on failure (can't open file to write to)
 */
int
IdVolWriteRawFile (char *nomfic, PPPVOLUME vo)
{
	int i, j;
	FILE *fp;

	fp = fopen(nomfic, ID_WFILE_BIN);
	if (fp) {
		if (vo) {
			int sx = (IdVolType(vo) == VOL_BIT)
			    ? ((IdVolDimX(vo) + 7) >> 3)
			    : IdVolDimX(vo);
			for (i = 0; i < IdVolDimZ(vo); i++)
				for (j = 0; j < IdVolDimY(vo); j++) {
					fwrite(vo[i][j], IdSizeOfType(vo),
					       sx, fp);
				}
		}
		fclose (fp);
		return (1);
	} else
		return (0);
}
