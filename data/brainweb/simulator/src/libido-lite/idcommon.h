/*************************************************************************
* $Id: idcommon.h,v 1.4 2005/09/26 10:24:56 yougz Exp $
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

#ifdef CMAKE_HAVE_STDINT_H
   #include <stdint.h>
#else
#ifdef CMAKE_HAVE_INTTYPES_H
   // Old system only have this
   #include <inttypes.h>   // For uint8_t uint16_t and uint32_t
#else
#ifndef WIN32
#include <stdint.h>
#endif
   #define TRUE 1
   #define FALSE 0
   #define ABS(a)	   (((a) < 0) ? -(a) : (a))
#endif
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
   typedef  signed char         int8_t;
   typedef  signed short        int16_t;
   typedef  signed int          int32_t;
   typedef  unsigned char       uint8_t;
   typedef  unsigned short      uint16_t;
   typedef  unsigned int        uint32_t;
   #define UINT32_MAX    (4294967295U)
#endif
