/*************************************************************************
* $Id: idvol-restricted.h,v 1.1 2005/09/09 08:22:42 bellet Exp $
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

#ifndef _IDVOL_RESTRICTED_H
#define _IDVOL_RESTRICTED_H



/* Not For Rats Functions  ('Hidden' Functions)
 * ======================
 *
 *	Utilisees selement en interne
 * 	N'ont pas etre connues des end-users
 * 	Ne seront pas SWIGees
 */
void   _IdVolFree                         (PPPVOLUME*);
void   _IdVolFreeSubVolume                (PPPVOLUME *);


/* Don't use any Longer Functions
 * ==============================
 *
 * 	Ne doivent plus etre utilisees par personne
 *	Ne sont laissees que pour compatibilite avec versions anterieures
 * 	Ne seront pas SWIGees
 */


// VIDE


/* ------------------------------------------------------------- */

#endif
