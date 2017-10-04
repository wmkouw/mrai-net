/*************************************************************************
* $Id: idima-base.h,v 1.1 2005/09/09 08:22:30 bellet Exp $
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
*  Description : Declarations diverses permettant d'utiliser le type IMAGE
*                de la bibliotheque LIBIDO
*
*
*
*===> Ce .h est destine a disparaitre lorsque Id xxx Alloc et Id xxx Free auront ete vires de IdDcmReadFile
*
**************************************************************************/

#ifndef __IDIMA_BASE_H__
#define __IDIMA_BASE_H__

#include <stdio.h>
#include "idgen.h"

#include "idsig-ido.h"
#include "idima-ido.h"


////////////////////////////////////////////////////////////////////////////

#include "idima-restricted.h"

/////////////// Allocation, freeing..


#ifndef SWIG
#define  IdImaFree(pi)          _IdImaFree((PPIMAGE*)&(pi) )
#else 
void IdImaFree(PPIMAGE pi)
{
   _IdImaFree((PPIMAGE*)&(pi) ) // Temporary modif for SWIG.2 pb
                                // \fixme : image pointer is no longer
                                //          set to ZERO after IdImaFree ...   

};
#endif

PPIMAGE        IdImaAlloc          (int, int, int);




#endif
