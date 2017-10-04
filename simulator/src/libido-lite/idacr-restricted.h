/*************************************************************************
* $Id: idacr-restricted.h,v 1.1 2005/09/09 08:22:25 bellet Exp $
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

#ifndef _IDACR_RESTRICTED_H
#define _IDACR_RESTRICTED_H

#include "idcommon.h"

/* Not For Rats Functions  ('Hidden' Functions)
 * ======================
 *
 *	Utilisees seulement en interne
 * 	N'ont pas etre connues des end-users
 * 	Ne seront pas SWIGees
 */

typedef struct acr_element {
        unsigned short int group;
        unsigned short int number;
        uint32_t length;
        uint32_t skippedLength; /* pour compatibilite DICOM VR   */
        long int value;    /* pointeur sur chaine de caracteres */
                           /* ou valeur entiere (max 32 bits)   */
                           /* des soucis a prevoir              */
                           /* avec les DEC Alpha, ptrs 64 bits  */
        struct acr_element *suiv;
        int type;
        } ACR_ELEMENT;



int	IdAcrWriteListeAcr(char *nf, ACR_ELEMENT *header);

ACR_ELEMENT * IdAcrMakeListFromInfo (char **,char **);
ACR_ELEMENT * IdAcrInquireHeader    (char *, int);

char ** _IdAcrInquireScanElscintInfoXXX (FILE *, char **);
char ** _IdAcrInquirePatientInfoXXX     (FILE *, char **);
char ** _IdAcrInquireIRMInfoXXX         (FILE *, char **);

int	_IdAcrTestSwap 	(uint32_t);
int	_IdAcrCheckSwap (FILE *);

ACR_ELEMENT *_IdAcrMakeElement 	(int, int, long int, int, long int);
ACR_ELEMENT *_IdAcrAddElement 	(ACR_ELEMENT *, ACR_ELEMENT *);
ACR_ELEMENT *_IdAcrAppendElement (ACR_ELEMENT *, ACR_ELEMENT *);
ACR_ELEMENT *_IdAcrFreeListe 	(ACR_ELEMENT *);
ACR_ELEMENT *_IdAcrMergeListes 	(ACR_ELEMENT *, ACR_ELEMENT *);

ACR_ELEMENT *_IdAcrReadNextElement(FILE *, int);
ACR_ELEMENT *_IdAcrFindElement  (ACR_ELEMENT *, int, int);

void    _IdAcrAdjustLength      (ACR_ELEMENT *);
void    _IdAcrAffListe          (ACR_ELEMENT *);
void    _IdAcrAffListe2         (ACR_ELEMENT *, int, int);
void    _IdAcrAffListeBref      (ACR_ELEMENT *);
int     _IdAcrAddHisto          (char *);
long	_IdAcrReadElement 	(int, int, FILE *, void *);
void * _IdAcrReadElementFromLabel(char *libelle, FILE * fp,
                                  char * vr, void *buff);

// FIXME IDODUDE long	_IdAcrReadElementR 	(int, int, FILE *, void *);
long 	_IdAcrOverWriteElement  (int, int, FILE *, void *); 
long 	_IdAcrGetElementLength  (int, int, FILE *); 
long 	_IdAcrModifElementLength(int, int, FILE *, uint32_t);

short int _IdAcrCheckType           (FILE *, char *, int, int, int);
long     _IdAcrGetPixelOffset       (FILE *);



/* Don't use any Longer Functions
 * ==============================
 *
 * 	Ne doivent plus etre utilisees par personne
 *	Ne sont laissees que pour compatibilite avec versions anterieures
 * 	Ne seront pas SWIGees
 */

int   IdAcrInquireInfo       (char *, unsigned short int * OUTPUT,
                              unsigned short int * OUTPUT, int * OUTPUT,
                              int * OUTPUT,  int * OUTPUT, int * OUTPUT);
int   IdAcrInquireInfoTot    (char *, unsigned short int * OUTPUT,
                              unsigned short int * OUTPUT,
                              unsigned short int * OUTPUT, int * OUTPUT,
                              int * OUTPUT,  int * OUTPUT, int * OUTPUT,
                              int * OUTPUT);
int   IdAcrInquireInfoTot2   (char *, unsigned short int * OUTPUT,
                              unsigned short int * OUTPUT,
                              unsigned short int * OUTPUT, int * OUTPUT,
                              int * OUTPUT,  int * OUTPUT, int * OUTPUT,
                              int * OUTPUT,  int * OUTPUT);

int   IdAcrInquireInfoTot3 (char *, unsigned short int * OUTPUT,
                           unsigned short int * OUTPUT,
                           unsigned short int * OUTPUT, int * OUTPUT,
                           int * OUTPUT,  int * OUTPUT, int * OUTPUT,
                           int * OUTPUT,  int * OUTPUT, int * OUTPUT);

#endif
