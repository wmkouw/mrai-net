/*************************************************************************
* $Id: kerextern.c,v 1.1 2005/09/09 08:22:51 bellet Exp $
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
 * IDO global variables
 */
#ifndef __IDEXTERN_H__
#define __IDEXTERN_H__

#include "idpoint.h"
#include "idface.h"

const int    __tab_ta_[] = /* dans l'ordre des definitions type de pixel */
	{
	TA_TY_CHAR,
	TA_TY_FLOAT,
	TA_TY_DOUBLE,
	TA_TY_COMPLEX,
	TA_TY_RGB,
	TA_TY_SHORT,
	TA_TY_LONG,
	TA_TY_UCHAR,
	TA_TY_USHORT,
	TA_TY_COMPLEX_DOUBLE,
	TA_TY_COMPLEX_FLOAT,
	TA_TY_COMPLEX_CHAR,
	TA_TY_COMPLEX_UCHAR,
	TA_TY_COMPLEX_SHORT,
	TA_TY_COMPLEX_USHORT,
	TA_TY_COMPLEX_LONG,
	TA_TY_BIT,
	TA_TY_IDPOINT,
	TA_TY_FACE,
	TA_TY_POINTER,
	TA_TY_ULONG,
	TA_TY_COMPLEX_ULONG,

	TA_TY_IDXPOINT_CHAR,
	TA_TY_IDXPOINT_UCHAR,
	TA_TY_IDXPOINT_FLOAT,
	TA_TY_IDXPOINT_DOUBLE,
	TA_TY_IDXPOINT_SHORT,
	TA_TY_IDXPOINT_USHORT,
	TA_TY_IDXPOINT_LONG,
	TA_TY_IDXPOINT_ULONG,
   	TA_TY_RGBA
	}
	;

int _IdSizeOfType (type)
int type;
{
  return __tab_ta_[type];
}
#endif
