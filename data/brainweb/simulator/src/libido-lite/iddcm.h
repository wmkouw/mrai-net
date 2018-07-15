/*************************************************************************
* $Id: iddcm.h,v 1.1 2005/09/09 08:22:28 bellet Exp $
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

#ifndef _IDDCM_H
#define _IDDCM_H

#include "idcommon.h"
#include <stdio.h>
#include "idswig.h"
#include "idliste.h"

#include "idima-ido.h"


#ifdef _MSC_VER
#ifndef __CYGWIN__
#include <Basetsd.h>
#endif
#endif

// =======================================================================================


typedef struct {
	int Gr;				// numero de groupe
	int Num;			// numero elem
	size_t LgrLueElem;	// Longueur Lue
	size_t LgrElem;		// Longueur retenue pour l'exploration
	int Swap;			// 1 si l'elem a ete swappé -> Est-ce positionne correctement ?
	int Offset;			// offset ( a partir début fichier)
	char *VR;			// VR de l'element
	char *DicomDescr;	// Definition trouvee dans Dicom Dictionnary
	int TypeDonnee;		// 0 : char ; 1 : short int; 2 : long int;	
	char *valeurElem;	// pointeur sur le début de la valeur (dans tous les cas)
	uint32_t	valInt;		// n'a de sens que si Swap != 0 et lgrLueElem=4  // GROS SOUCIS
	uint16_t valShort;	// n'a de sens que si Swap != 0 et lgrLueElem=2	 // utiliser str2num(valeurElem,...
	
} _ID_DCM_ELEM;


typedef struct {

	char * filename;		// nom du fichier
	FILE * fp;
	long int taille_fich;
	int nbElem;		// Nbre de dicom elements dans l'entete.

	char * _message;
	
	int sw; // code Swap (0, 4321, 2143, 3412)
	int net2host; // =1 : meme ordre; =0 ordre inverse
	int ACR_LIBIDO;
	int __ExplicitVR;
	int __TrueDicom;
	
	int grPixelTrouve;
	unsigned short int grPixel;
	unsigned short int numPixel;
	int PixelsTrouves;	
	long int PixelPosition;
	
	unsigned short int __NumeroGroupePrecedent;
	char * deb;				// zone qui recevra de debut de l'entete 
	long int offsetCourant;
	_ID_DCM_ELEM *pleCourant;
	
	PLIST plist;
	
} ID_DCM_HDR;




// =======================================================================================

#define str2num(str, typeNum) *((typeNum *)(str))

ID_DCM_HDR * 	IdDcmHdrAlloc	(void);
void 		IdDcmHdrFree	(ID_DCM_HDR *);


#define IdDcmGetPixelOffset(e) ((e)->PixelPosition)
#define IdDcmGetPFileName(e)   ((e)->filename)


ID_DCM_HDR  	*IdDcmGetHeader		(char *);
int 		 IdDcmIsAcrLibido	(ID_DCM_HDR *);
int		IdDcmIsJpegLossless	(ID_DCM_HDR *);
int 		IdDcmIsUncompressed 	(ID_DCM_HDR *);
ID_DCM_HDR 	*IdDcmIsDcmReadable	(char *);

void 		*IdDcmReadFile			    (char * ,       int );
void 		*IdDcmReadFileFromDcmHdr	     (ID_DCM_HDR *, int );
void 		*IdDcmMultiFrameReadFile             (char *,       int , int , int );
void 		*IdDcmMultiFrameReadFileFromDcmHdr   (ID_DCM_HDR *, int , int , int );
void 		*IdDcmReadFileIntoVector             (char *, 	    int , int , int , void * );
void 		*IdDcmReadFileIntoVectorFromDcmHdr   (ID_DCM_HDR *, int , int , int , void * );

int		IdDcmWriteFile				(ID_DCM_HDR * , PPIMAGE );

char **		IdDcmInquireImageInfoFromFile	(char *);
char **		IdDcmInquireImageInfoFromDcmHdr	(ID_DCM_HDR *);
int		IdDcmPrintImageInfo				(char **);
void		IdDcmAffDcmHdr					(ID_DCM_HDR *, int, int);

											
int     IdDcmInquireInfoFromFile    (char *, unsigned short int * OUTPUT,
                                     unsigned short int * OUTPUT,
                                     unsigned short int * OUTPUT, int * OUTPUT,
                                     int * OUTPUT, int * OUTPUT, int * OUTPUT,
                                     int * OUTPUT, int * OUTPUT, int * OUTPUT,
                                     int * OUTPUT, int * OUTPUT, int * OUTPUT,
				       		 int * OUTPUT);

int     IdDcmInquireInfoFromDcmHdr  (ID_DCM_HDR *, unsigned short int * OUTPUT,
                                     unsigned short int * OUTPUT,
                                     unsigned short int * OUTPUT, int * OUTPUT,
                                     int * OUTPUT,  int * OUTPUT, int * OUTPUT,
                                     int * OUTPUT,  int * OUTPUT, int * OUTPUT,
                                     int * OUTPUT,  int * OUTPUT, int * OUTPUT,
				       		 int * OUTPUT);
				       		 
char * _IdDcmReadElementFromLabel(char *libelle, ID_DCM_HDR * e,
                                  char * vr, void *buff);
                                  

#endif
