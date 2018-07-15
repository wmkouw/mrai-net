/*************************************************************************
* $Id: idio.h,v 1.1 2005/09/09 08:22:35 bellet Exp $
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
 * DESCRIPTION : Constantes pour l'acces aux fichiers.
 *
 * ID_RFILE_BIN    : read binary   ("rb")
 * ID_RFILE_TEXT   : read text
 * ID_WFILE_BIN    : write binary
 * ID_WFILE_TEXT   : write text
 * ID_AFILE_BIN    : append binary
 * ID_AFILE_TEXT   : append text
 * ID_RFILE_P_BIN  : read binary   + ("rb+")
 * ID_RFILE_P_TEXT : read text     +
 * ID_WFILE_P_BIN  : write binary  +
 * ID_WFILE_P_TEXT : write text    +
 * ID_AFILE_P_BIN  : append binary +
 * ID_AFILE_P_TEXT : append text   +
 *  
 */

#if defined(_MSC_VER)
#define ID_RFILE_BIN    "rb"
#define ID_RFILE_TEXT   "rt"
#define ID_WFILE_BIN    "wb"
#define ID_WFILE_TEXT   "wt"
#define ID_AFILE_BIN    "ab"
#define ID_AFILE_TEXT   "at"
#define ID_RFILE_P_BIN  "rb+"
#define ID_RFILE_P_TEXT "rt+"
#define ID_WFILE_P_BIN  "wb+"
#define ID_WFILE_P_TEXT "wt+"
#define ID_AFILE_P_BIN  "ab+"
#define ID_AFILE_P_TEXT "at+"
#else
#define ID_RFILE_BIN    "rb"
#define ID_RFILE_TEXT   "r"
#define ID_WFILE_BIN    "wb"
#define ID_WFILE_TEXT   "w"
#define ID_AFILE_BIN    "ab"
#define ID_AFILE_TEXT   "a"
#define ID_RFILE_P_BIN  "rb+"
#define ID_RFILE_P_TEXT "r+"
#define ID_WFILE_P_BIN  "wb+"
#define ID_WFILE_P_TEXT "w+"
#define ID_AFILE_P_BIN  "ab+"
#define ID_AFILE_P_TEXT "a+"
#endif

