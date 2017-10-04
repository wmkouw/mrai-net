/*************************************************************************
* $Id: strcomp.c,v 1.1 2005/09/09 08:22:56 bellet Exp $
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
 * Comparaison de chaines de caracteres sans tenir compte de 
 * la casse.
 */
#include "idstr.h"

#define UP(c) (('a'<=(c) && 'z'>=(c))?((c)+'A'-'a'):(c))

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdStrCmpNoCase (fonction)
 * 
 * RESUME: Meme role que strcmp, sans tenir compte de la casse (maj./min.).
 * 
 * DESCRIPTION: Meme role que strcmp, sans tenir compte de la casse (maj./min.).
 * 
 * SYNTAXE:     int comp = IdStrCmpNoCase (char * a, char * b);
 * 
 * RETOUR:      type  : comp
 * role  : code de comparaison : 0: a=b   >0: a>b  <0: a<b 
 * 
 * PARAMETRES:  
 * nom   : a, b
 * type  : char * 
 * role  : chaines a comparer.
 * 
 * FICHIER:     strcomp.c
 * 
 * ******************************************************** END DESCRIPTION 
 */

int 
IdStrCmpNoCase(char *s1, char *s2)
{
	do {
		int c;
		if ((c = UP(*s1) - UP(*s2)) != 0)
			return c;
		s1++;
		s2++;
	} while ((*s1) || (*s2));
	return 0;
}

#undef UP
