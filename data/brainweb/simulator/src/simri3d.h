/*************************************************************************
* $Id: simri3d.h,v 1.19 2006/12/18 15:10:38 yougz Exp $
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
  
  Version 2.0  20/12/2006
*************************************************************************/
#ifndef _SIMRI3D_H
#define _SIMRI3D_H

#include <stdio.h>
#include <math.h>
#ifdef SUNOS5
#define M_PI    3.14159265358979323846
#endif

#ifdef _MSC_VER
#define M_PI    3.14159265358979323846
#endif

#ifndef SWIG

#ifdef _MSC_VER
#define INLINE __inline
#else
#define INLINE inline
#endif

#endif

#define G 267.522128e+6       /* Giromagnetic moment of the proton in rad/sec/Tesla*/
#define G_rad_T 267.522128e+6 /* Giromagnetic moment of the proton in rad/sec/Tesla*/
#define G_kHz_G 4.2577469     /* Giromagnetic moment of the proton in kHz/Gauss    */
#define G_rad_G 26752.2128    /* Giromagnetic moment of the proton in rad/sec/Gauss*/
#define G_Hz_T 4.2577469e+7   /* Giromagnetic moment of the proton in Hz/Tesla     */

#define NONE	0
#define PHASE	1
#define FREQUENCY	2
#define PLUS	1
#define MINUS	-1
#define REPLACE -1
#define TAILLE_NOM	200

#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif

#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif

#ifndef MAX
#define MAX(x,y)        (x > y ? x : y)
#endif
#define RINT(x,y)	(y=floor(x),(x-y > (double) 0.5 ?  y+1 : y))


#ifndef SWIG

#ifdef _USRDLL
#define FPRINTF ConsoleOut
void ConsoleOut(FILE*, ...); /* used for old wrapping in tcl */
#else
#define FPRINTF fprintf
#endif

#endif

#endif
