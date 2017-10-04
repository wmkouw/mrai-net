/*************************************************************************
* $Id: idstr.h,v 1.1 2005/09/09 08:22:40 bellet Exp $
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

#ifndef __IDSTR_H__
#define __IDSTR_H__
#include "idswig.h"

char  *  IdStrCreateNewSuffix  (char*, char*);
char  *  IdStrRemoveSuffix     (char*);
int      IdStrCmpNoCase        (char*, char*);
char  *  IdStrCreateUpperCase  (char*);
char  *  IdStrSetUpperCase     (char*);
int      IdStrCountChar        (char*, int);
char  *  IdStrFileName         (char *);
char  *  IdStrDirName          (char *);
int   *  IdStrIntEnum          (char*, int * OUTPUT);
float *  IdStrFloatEnum        (char*, int * OUTPUT);
int	 IdStrSwap	       (char *);
int *    IdStrSort             (void **, int, INT_FUNCTION_VOIDP_VOIDP_POINTER);
char **  IdStrGetDirContents   (char*, char*);
int      IdStrGetDirNbFiles    (char*, char*);
int      IdStrPatternMatching  (char*, char*);
char *  _IdStrGetDirectoryName (char * );
char *   IdStrDirFindFirstFile  (char * );
char *   IdStrDirFindNextFile   (char * );
char *   IdStrMakeFullPathName  (char * , char *);

char * IdStrLibidoDataType (int);
char * IdStrLibidoTypeObj  (int);
char * IdStrCleanName      (char * );

char * _IdStrShowTransfertSyntax (char *);
#endif
