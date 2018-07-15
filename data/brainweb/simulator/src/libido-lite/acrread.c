/*************************************************************************
* $Id: acrread.c,v 1.1 2005/09/09 08:22:21 bellet Exp $
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
*                                  * PROJET      : libido
* NOM DU MODULE : readacr.c        * TYPE        : fonction
* AUTEUR        : c.odet           * CREATION    :  07/05/1992
* VERSION       :                  * REVISION    :  
* LANGAGE       : C                * 
*                                  *
******************************************** ******************************
*
*  Description : Lit un objet LIBIDO dans un fichier 
*		au format ACRNEMA modifie URA1216
**************************************************************************
*
*  MODULES UTILISES :
*
**************************************************************************
*                     |              |
* REVISIONS :  DATE   | AUTEUR       |  DESCRIPTION
*---------------------|--------------|------------------------------------
*           16/06/92  | B. BARBIER   | Insertion Libido
*           15/9/92   |   ""         | V1.2-5 normalisation des noms
*           01/10/92  | B. Barbier   | V1.2-5 : normalisation des noms
*           08/10/92  | CO           | V1.3 changement de structure
*            10/11/94   JPR            prise en compte des Signaux
*            25/01/95   JPR            prise en compte types non UCHAR
*            21/04/95   CO             complexes  Ima 
             22 04 95   JPR            prise en compte types non Reel  
             29 04 95   JPR            IdAcrInquireInfo  
	     16 06 95   JPR            IdAcrInquirePixelSize 
              3 07 95   JPR            remplacement elem non-ACR 28,105
					par 28,199 pour compatibilite images
					Phillips  
	     25/10/95	JPR		_IdAcrCheckType pour rationnaliser 
					l'usine a gaz... 
	     8/01/96	JPR		Lecture des Objets CONTOUR
	     5/02/96	JPR		UsedNbX ...  
	     6/02/96	JPR		IdAcrInquireType (pour test swap)
		6/5/96	CO		Initialisation NX,NY,NZ dans IdAcrInquireInfo
	     04/09/96   JPR+ GB         VRAIE prise en compte de nbu et hightBit
	      3/2/97	JPR		Prise en compte champ _message  
	      7/2/97	JPR		PROBLEME pour les elements 'chaine de caract' :
					Ils doivent etre alloues 
					AVANT l'appel de IdAcrReadElement 
	19/2/97		BB		Read dans GetSliceThickness
	20/2/97		JPR		Verif entete 
	17/4/97		JPR		Ajout SEQUENCE         
        9/10/97		JPR		IdAcrInquireInfoTot : correction pb sur NT des SEQ
	20/4/98		JPR 		Modif IdIrmGetAndRenameDirContents
					pour pb inscription Xtiple si renomage...
	21/4/98		JPR		Correction confusion CNT et SIG en lecture ACR

	19/5/98		JPR		IdDcmCVReadFile pour DICOM CardioVasc
	20/5/98		JPR		Elagage IdAcrReadFile
	27/5/98		JPR		IdAcrInquireInfoTot2, pour DICOM CardioVasc
	15/12/98	JPR		Erreur ds dimX dimY pour les VRAIES images ACR
					(rectangulaires)
	19/2/99		Djamal Boukerroui IdAcrReadFile : correction code retour si 
							erreur dans IdDcmCVReadFile
			
	19/3/99		JPR		Alloc dynamique pour les _message monstrueux
	29/6/99		JPR		Correction Swap pour CardioVasc sur Dec
	30/6/99		JPR		Modif _IdAcrGetPixelOffset pour SGI
	1/07/99		JPR		Prise en compte RGB DICOM
	24/08/99	JPR		Ajout IdAcrInquireInfoTot3 (renvoie le codeSwap)
	15/10/99	Djamal Boukerroui Pb indicateur de fin du champ _message, a la relecture
	21/10/99	JPR		Pb lgr message si fichier non LibIDO
	24-01-2000	FB		initialisations
	09-08-2000	JPR		Modif lecture lgr pour Philips single Frame (PAPYRUS-like)
	31-08-2000	JPR		Prise en compte Nb Bits stored = 12 dans _IdAcrCheckType
	23-06-2001	JPR 		j'enleve la  prise en compte ERRONNEE de nbu et hightBit


POURRIEZ-VOUS EVITER DE VIRER CES COMMENTAIRES.
MERCI

JPRx

**************************************************************************/


#include <string.h>  // For strlen
#include "idio.h"
#include "idprint.h"
#include "idacr-private.h"
#include "iddcm.h"

static int DEBUG=0;

/* 
Rappels
-------
     
#define TY_CHAR		  	  ( 0<<4)  00
#define TY_FLOAT	  	  ( 1<<4)  10
#define TY_DOUBLE	  	  ( 2<<4)  20
#define TY_COMPLEX	  	  ( 3<<4)  30
#define TY_RGB		  	  ( 4<<4)  40
#define TY_SHORT	  	  ( 5<<4)  50
#define TY_LONG		  	  ( 6<<4)  60
#define TY_UCHAR		  ( 7<<4)  70
#define TY_USHORT         ( 8<<4)  80                             
#define TY_COMPLEX_DOUBLE ( 9<<4)  90                              
#define TY_COMPLEX_FLOAT  (10<<4)  a0                             
#define TY_COMPLEX_CHAR   (11<<4)  b0                             
#define TY_COMPLEX_UCHAR  (12<<4)  c0                            
#define TY_COMPLEX_SHORT  (13<<4)  d0                             
#define TY_COMPLEX_USHORT (14<<4)  e0                            
#define TY_COMPLEX_LONG   (15<<4)  f0                            
#define TY_BIT            (16<<4) 100                             
#define TY_IDPOINT        (17<<4) 110
#define TY_FACE           (18<<4) 120
#define TY_POINTER        (19<<4) 130				 
#qdefine TY_ULONG         (20<<4) 140				 
#define TY_COMPLEX_ULONG  (21<<4) 150

#define TY_IDXPOINT_CHAR  (22<<4) 160			
#define TY_IDXPOINT_UCHAR (23<<4) 170
#define TY_IDXPOINT_FLOAT (24<<4) 180
#define TY_IDXPOINT_DOUBLE(25<<4) 190
#define TY_IDXPOINT_SHORT (26<<4) 1a0
#define TY_IDXPOINT_USHORT(27<<4) 1b0
#define TY_IDXPOINT_LONG  (28<<4) 1c0
#define TY_IDXPOINT_ULONG (29<<4) 1d0			

#define TY_COLOR          (30<<4) 1fo
*/				 

#include <stdio.h>
#include <stdlib.h>  // For atoi
#include "idgen.h"
#include "idsig.h"
#include "idcnt.h"
#include "idima.h"
#include "idvol.h"
#include "idseq.h"
#include "idacr.h"
#include "idacr-restricted.h"
#include "iderr.h"


int _IdAcrRecupLgr    (FILE *fp, int *);
int _IdAcrIsAcrLibido (FILE *fp,char *bufbid);

 
short int SWAP_SHORT 		(short int);
long int  SWAP_LONG  		(long int);

static int ACR_LIBIDO;

static int rgb;

static int sw;
static char  * _message;

extern int __ID_CardioVascCurieux;
extern int __ID_offset;
extern int __Papyrus;
extern unsigned short int __NumeroGroupePrecedent;
extern int __TrueDicom;
extern int __ExplicitVR;	


/* FUNCTION DESCRIPTION **************************************************
* 
* IdDcmCVReadFile  (fonction)
* 
* RESUME: Lecture d'un fichier de type DICOM MultiFrame.
* 
* DESCRIPTION: Lecture d'un fichier de type DICOM MultiFrame.
* 
* 		A L'ATTENTION DES Kamikazes Creatissiens :
* 		La fonction IdAcrReadFile (supposee lire des fichiers 'ACR LibIDO')
* 		a evolue au fil des ans, en fonction des choses 
* 		que l'on a considere comme utiles de rajouter dans NOTRE entete.
* 		Puis, on a eu de 'vrais' fichiers ACR-NEMA, V2 puis V3.
* 		Puis des fichiers DICOM single-frame.
* 		Puis des fichiers DICOM multiframe.
* 		A ajouter a ca, des particularites a la limite de la mauvaise foi,
* 		dependant des constructeurs...
* 
* 		DONC :
* 
* 		Ne perdez pas votre temps a essayer de comprendre POURQUOI ...
* 		La reponse est : PARCE QUE !
* 
* 
* SYNTAXE:     PPIMAGE tabIm[] = IdDcmCVReadFile (char * filename, int numPremiereImage, int numDerniereImage);
* 
* RETOUR:      type  : void **
* 	     role  : Pointeur vers un tableau d'objets
* 
* PARAMETRES:
* 	     nom   : filename
* 	     type  : char *
* 	     role  : Nom du fichier DICOM MultiFrame
* 
* 	     nom   : typeObj
* 	     type  : int
* 	     role  : Type de l'object a lire (IMA_UCHAR, SIG_CHAR, ...).
* 
* 	     nom   : numPremiereImage
* 	     type  : int
* 	     role  : Num premiere image a lire (si -1 -ou 0- on commence a la premiere).
* 
* 	     nom   : numDerniereImage
* 	     type  : int
* 	     role  : Num derniere image a lire (si -1 -ou 0- on fini a la derniere).
* 
* FICHIER:     readacr.c
* 
******************************************************** END DESCRIPTION */

/* JPR, pour eviter de dupliquer le source    */

#define CCS(Type)							\
if(!fread(&((Type)psi)[0],IdSigDimX(psi)*IdSizeOfType(psi),1,fp))	\
									\
         {								\
            IdSigFree(psi);					\
            IdErrPrintf("echec en lecture Signal\n");\
	    return(0);						\
	 }

#define CCC(Type)							\
if(!fread(&((Type)pco)[0],IdCntDimX(pco)*IdSizeOfType(pco),1,fp))	\
									\
         {								\
            IdCntFree(pco);					\
            IdErrPrintf("echec en lecture Contour\n");	\
	    return(0);						\
	 }

/* pour les anciens codage de contour */
#define CCC2(Type)							\
if(!fread(&((Type)pco)[0],IdCntDimX(pco)*IdSizeOfType(pco)>>1,1,fp))\
									\
         {								\
            IdCntFree(pco);					\
            IdErrPrintf("echec en lecture Contour\n");	\
	    return(0);						\
	 }

#define CCI(Type,TailleType)													\
   {																		\
	if (vector != NULL) { /* on a une contrainte de rapidite */				\
		if(!fread(vector,(int)dim[0]*(int)dim[1]*TailleType,1,fp)){		\
				IdPrintf("echec en lecture Image dans Vecteur \n");		\
	    			return(0);												\
		}																\
	} else {																\
		for(i=0;i<IdImaDimY(pim);i++){									\
       			if(!fread(((Type)pim)[i],IdImaDimX(pim)*IdSizeOfType(pim),1,fp)) { \
            			IdPrintf("echec en lecture Image [i : %d] lgr de %d \n", \
						i,IdImaDimX(pim)*IdSizeOfType(pim));			\
            			IdImaFree(pim);		  									\
	    			return(0);												\
	 		}								\
	 	}									\
	}										\
   }																		\
													
#define CCV(Type)										\
	for(j=0;j<IdVolDimZ(pvo);j++)					\
	  for(i=0;i<IdVolDimY(pvo);i++)					\
	   if(!fread(((Type)pvo)[j][i],IdVolDimX(pvo)*IdSizeOfType(pvo),1,fp)) \
	     {											\
	      IdVolFree(pvo);							\
              IdErrPrintf("echec en lecture Volume\n");	\
	      return(0);								\
	     }

#define CCQ(Type)										\
	for(k=0;k<IdSeqDimT(pse);k++)					\
	for(j=0;j<IdSeqDimZ(pse);j++)					\
	  for(i=0;i<IdSeqDimY(pse);i++)					\
	   if(!fread(((Type)pse)[k][j][i],IdSeqDimX(pse)*IdSizeOfType(pse),1,fp))  \
	     {											\
	      IdSeqFree(pse);							\
              IdErrPrintf("echec en lecture Sequence\n");\
	      return(0);								\
	     }

//-------------------------------------------------------------------------
//
// IdAcrReadFileIntoVector est un sur-ensemble de IdAcrReadFile
//

/**
 * \brief		Lecture d'un fichier (IMAGE) ACR/DICOM dans un Vecteur existant deja
 *
 * 		A L'ATTENTION DES Kamikazes Creatissiens :
 * 		La fonction IdAcrReadFileIntoVector est une extention de IdAcrReadFile
 *		LIMITEE pour le moment aux fichiers IMAGES (non SIGNAL, non VOLUME, non SEQUENCE)
 *		Elle n'a d'interet que lorsqu'on lit une serie de fichiers *dans un vecteur*
 *		et permettra, par exemple de court-circuiter VtkReader, particulierement peu efficace
 *
 *		ATTENTION : elle NE CREE PAS l'entete privee de PPIMAGE !!!
 *					de plus, l'adresse de debut dans le vecteur est a la charge de l'utilisateur ...
 *  		==> a n'utiliser qu'avec precaution
 *
 *		Exemple d'utilisation :
 *
 *	int nbImages;
 *	int tailleImage=dimx*dimy*sizeof(...));
 *	char * vect, *curPos;;
 *	curPos = vect = (char *)malloc(tailleImage*nbImages);
 *	for(i=0;i<nbImages;i++) {
 *		IdAcrReadFileIntoVector(nomImage[i],-1,-1,curPos=+tailleImage);
 *	}
 *	// vect pointe sur le vecteur des pixels de l'ensemble des images.
 *	// vect n'EST PAS un 'objet' LibIDO	
 *	// C'est a l'utilisateur de l'exploiter comme il lui convient
 *
 * @param filename	Nom du fichier DICOM ACR/DICOM
 * @param type		Type des images a lire (IMA_UCHAR,IMA_USHORT, ...).
 * @param premIm	Num premiere image a lire (Dicom multiframe uniquement; -1 sinon)
 *			(si -1 ou 0 on commence a la premiere).
 * @param dernIm	Num derniere image a lire a lire (Dicom multiframe uniquement; -1 sinon)
 *			(si -1 ou 0 on fini a la derniere).
 * @param vector    adresse dans le Vecteur, deja alloue, a laquelle sera rangée l'image lue
 * @return		Pointeur vers un tableau unidimentionel 
 * 			(celui passe en parametre; NULL si echec)
 */

void *IdAcrReadFileIntoVector(char *filename, int type, int premIm, int dernIm, void * vector) {
	
    return IdDcmReadFileIntoVector(filename,  type,  premIm,  dernIm, vector) ;
    return (NULL);
}



/* FUNCTION DESCRIPTION **************************************************
* 
* IdAcrInquireInfoTot4  (fonction)
* 
* RESUME: Retourne diverses info stockees dans l'entete ACRNEMA.
* 
* DESCRIPTION: Retourne des info sur l'objet stocke dans un fichier ACRNEMA.
* 	Remplace IdAcrInquireInfo, qui etait incomplet
* 
* SYNTAXE:     int retCode = IdAcrInquireInfoTot4 (char *filename, int *typObj, *typLibido, int *typData, int *NX, int *NY, int *NZ, int *NT, int *tailleEntete, int *nbFrames, int* nb, int *nbu, int *hb, int *sign);
* 
* RETOUR:      type  : int
* 	     role  : 0: echec 1: succes.
* 
* PARAMETRES:
* 	     nom   : filename
* 	     type  : char *
* 	     role  : Nom du fichier ACRNEMA
* 
* 	     nom   : typeObj
* 	     type  : int *
* 	     role  : type d'objet (SIG, IMA, VOL, CNT, ...
* 
* 	     nom   : typLibido
* 	     type  : int *
* 	     role  : type d'objet (TY_DOUBLE, TY_UCHAR, TY_COMPLEX_DOUBLE ...
* 
* 			Si l'on veut fabriquer, a partir de ce type LibIDO un objet d'un autre type d'objet
* 			(p ex, on a lu un VOLUME, on veut fabriquer une IMAGE de meme type)
* 			On fera :
* 			typeIma=typeLibido | IMA;
* 
* 	     nom   : typData
* 	     type  : int *
* 	     role  : type de Donnees LibIDO de l'objet (SIG_DOUBLE, IMA_UCHAR, VOL_COMPLEX_DOUBLE, ...    			
* 	     nom   : NX
* 	     type  : int *
* 	     role  : taille en X de l'objet (SIG, IMA, VOL, SEQ seulement)
* 
* 	     nom   : NY
* 	     type  : int *
* 	     role  : taille en Y de l'objet (IMA, VOL, SEQ seulement)
* 
* 	     nom   : NZ
* 	     type  : int *
* 	     role  : taille en Z de l'objet (VOL, SEQ seulement)
* 
* 	     nom   : NT
* 	     type  : int *
* 	     role  : taille en T de l'objet (SEQ seulement)
* 
* 	     nom   : tailleEntete
* 	     type  : int *
* 	     role  : taille, en octets, de l'entete a sauter
* 
* 	     nom   : nbFrames
* 	     type  : int *
* 	     role  : nb d'images, ds le cas DICOM CardioVasc
*
* 	     nom   : codeSwap
* 	     type  : int *
* 	     role  : code de swap des octets
* 
* 	     nom   : nb
* 	     type  : int *
* 	     role  : nombre de bits alloues
*
* 	     nom   : nbu
* 	     type  : int *
* 	     role  : nombre de bits utiles
*
* 	     nom   : hb
* 	     type  : int *
* 	     role  : Bit de poids fort
*
* 	     nom   : sign
* 	     type  : int *
* 	     role  : signe des pixels
*
*
* 
* FICHIER:     readacr.c
* 
******************************************************** END DESCRIPTION */


/**
 * \brief		Retourne diverses info stockees dans l'entete ACRNEMA ou DICOM
 *
 * @param filename	Nom du fichier.
 * @param typObj	Pointeur vers le Unsigned Short qui recevra le type d'OBJET 
 *				(SIG, IMA, VOL, SEQ, CNT, etc ..)
 * @param typLibido	Pointeur vers le Unsigned Short qui recevra le type LibIDO 
 *				(ex : SIG_COMPLEX_FLOAT, IMA_USHORT, VOL_DOUBLE) 
 * @param typData	Pointeur vers le Unsigned Short qui recevra le type de Donnees 
 *				(ex : TY_COMPLEX_FLOAT, TY_USHORT, TY_DOUBLE) 
 * @param NX	Pointeur vers l'integer qui recevra le nombre de Colonnes 
 * @param NY	Pointeur vers l'integer qui recevra le nombre de Lignes 
 * @param NZ	Pointeur vers l'integer qui recevra le nombre de Colonnes 
 * @param NT	Pointeur vers l'integer qui recevra le nombre d' Instants 
 * @param offset Pointeur vers l'integer qui recevra la valeur de l'offset des Pixels 
 * @param nbFrames Pointeur vers l'integer qui recevra le nombre de Frames 
 * @param codeSwap Pointeur vers l'integer qui la valeur du code de Swap pour les entiers
 *				1234 : pas de swap
 *				4321
 *				2143 : Bad Big Endian
 *				3412 : Bad Little Endian
 * @param nb  Pointeur vers l'integer qui recevra le nombre de bits alloues 
 * @param nbu Pointeur vers l'integer qui recevra le nombre de bits utilises 
 * @param nb  Pointeur vers l'integer qui recevra le nombre le bit de poids fort 
 * @param ty  Pointeur vers l'integer qui recevra le signe 
 *
 * @return	1 : l'objet est ACR-NEMA ou DICOM
 *		0 : sinon
 */



int IdAcrInquireInfoTot4 ( char * filename,
			unsigned short int *typObj,
			unsigned short int *typLibido,
			unsigned short int *typData,
			int *NX, 
			int *NY, 
			int *NZ, 
			int *NT,
			int *offset,
			int *nbFrames,
			int *codeSwap, 
			int *bitsAllocated,
			int *bitsStored,
			int *highBit,
			int *signe)
{
  FILE *fp;
  short int ndim,nb,nbu,hb,ty;
  short int dim[4];
  int i 	/* ,sw */ ;		 /* sw en Global */
  char bufbid[150];
  long int nbCarLus;

if((fp=fopen(filename,ID_RFILE_BIN))==0) {
	IdErrno=IDERR_OPEN_FAILED;
	IdErrPrintf ("echec ouverture  %s\n",filename);
	return (0);
 } else
     	sw=_IdAcrCheckSwap(fp);

 if(sw==-1) { 
	IdErrno=IDERR_NON_ACR_FILE;
    	fclose ( fp );
	return (0);
 }

 *codeSwap=sw;

if ( __ID_CardioVascCurieux == 1) {
	printf("ID_CardioVascCurieux : 2\n");
 	fseek(fp,0L,SEEK_SET); /* pas trouve 08,00; on se met au debut */
} else {
	ACR_LIBIDO = _IdAcrIsAcrLibido (fp,bufbid);	
}

   *typData=0;
   nbCarLus=_IdAcrReadElement(0x0028,0x0004,fp,bufbid);

   if(nbCarLus!=0) { /* RGB  */

		  bufbid[nbCarLus]=0;

		  if (  (memcmp(bufbid,"RGB ",4)==0)    
     			||
      		        (memcmp(bufbid,"B RG",4)==0) 
     			||
      		        (memcmp(bufbid," BGR",4)==0)          
     			||
      		        (memcmp(bufbid,"GR B",4)==0) )  {

			*typData=TY_RGB;
			IdPrintf ("Image RGB !\n");
		}

    }
	                               /* JPR  27-05-98*/

		/* Lecture dimension de l'espace */
  if (!_IdAcrReadElement(0x0028,0x0005,fp,&ndim)) {
	ndim=2; 
  }
	                               /* JPR */
  if(ndim>4) {
	IdPrintf (" Nb de Dimensions incorrect : %d\n", ndim);
        fclose ( fp );
	return(0);
  }

		/* Lecture nbFrames pour CardioVasc */

  if ((nbCarLus=_IdAcrReadElement(0x0028,0x0008,fp,bufbid))==0) {
	*nbFrames=1;
  } else
	*nbFrames=atoi(bufbid); /*  c'est des caracteres type : IS, pas un short int	*/

  for(i=0;i<ndim;i++)
      if(!_IdAcrReadElement(0x0028,0x0010+i,fp,&dim[i])) {

	  IdErrno=IDERR_NON_ACR_FILE;
          fclose ( fp );
	  return (0);
      }

  if (ACR_LIBIDO!=1) { /* si c'est une VRAIE image ACR  	  */
		       /* on permutte NX et NY ds l'entete LibIDO */
	int secuDim;
	secuDim = dim[0];
	dim[0]  = dim[1];
	dim[1]  = secuDim;
  }

  *NT = *NZ = *NY = *NX = 1; /* CO 6/5/96 */
  switch(ndim) {
	case 4: *NT=dim[3];
	case 3: *NZ=dim[2];
	case 2: *NY=dim[1];
	case 1: *NX=dim[0];
  } 

  switch (ndim) {
      case  1: *typObj=SIG; break;
      case  2: *typObj=IMA; break;
      case  3: *typObj=VOL; break;
      case  4: *typObj=SEQ; break;
      default: {
	  IdErrno=IDERR_NON_ACR_FILE;
	  fclose ( fp );
	  return (0);
      }
 }
 				         /* nombre de bits par pixels (Bits Allocated) */
    if(!_IdAcrReadElement(0x0028,0x0100,fp,&nb)) {

	  IdErrno=IDERR_NON_ACR_FILE;
          fclose ( fp );
	  return (0);
     }	
     *bitsAllocated = nb;
   				         /* nombre de bits utiles (Bits Stored) */
    if(!_IdAcrReadElement(0x0028,0x0101,fp,&nbu)) {

	nbu=nb;
    }
    *bitsStored = nbu;
   				         /* Bit de poids fort (High Bit ) */
    if(!_IdAcrReadElement(0x0028,0x0102,fp,&hb)) {

	hb=nbu-1;
    }
    *highBit = hb;

					   /* type 'signe' ou 'non signe ' **/
    if(!_IdAcrReadElement(0x0028,0x0103,fp,&ty)) {

	ty=0;	
    }
    *signe = ty;
/*
    if (*typData != TY_RGB) {
    	*typData=_IdAcrCheckType(fp, bufbid, (int)ty, (int)nb, *typObj);
    }
    *typLibido = *typData & TY_SIZE;
*/

    *typLibido=_IdAcrCheckType(fp, bufbid, (int)ty, (int)nb, *typObj);

    *typData = *typLibido & TY_SIZE;


   /* IdPrintf (" typData %x\n",*typData); */

    *offset = _IdAcrGetPixelOffset(fp);

    fclose ( fp );

  return 1;
}

/* FoNCTION DESCRIPTIoN **************************************************
* 
* _IdAcrGetPixelOffset  (fonction)
* 
* ReSUME: Retourne l'offset pour atteindre le groupe des Pixels stocke dans l'entete ACRNEMA.
* 
* DeSCRIPTION: Retourne l'offset pour atteindre le groupe des Pixels stocke dans l'entete ACRNEMA..
* 
* SyNTAXE:     long offset = _IdAcrGetPixelOffset(fp)
* 
* 
* FICHIER:     readacr.c
* 
*     fseek(fp,4L,SEEK_SET);     SEEK_SET = 0                   
*     fread(&bili2,4,1,fp);      lgr groupe : 0000 0004   JPR       
*     sw=_IdAcrTestSwap(bili2);                           JPR   
* 
* 
* EXEMPLE:     
*        
******************************************************** END DESCRIPTIoN */


long _IdAcrGetPixelOffset(FILE *fp)
{
	unsigned short g,gr;
	unsigned short n,num;
	int l;
	int flag;
	int skL;
	int lgrTot=__ID_offset;
	
	int __cas_de_fou_Philips =0;

	if (DEBUG)
		IdPrintf (" _IdAcrGetPixelOffset : __ID_offset = %d\n",__ID_offset);


	/* --- pour remettre le pointeur de fichier d'applomb	--- */
	/* --- et pour positionner __ID_offset			--- */
	/* pas tres elegant --> trouver autre chose ? 		--- */
     	sw=_IdAcrCheckSwap(fp);

	if (DEBUG)
		IdPrintf (" \n_IdAcrGetPixelOffset : sortie _IdAcrCheckSwap\n\n");

	if ( __ID_CardioVascCurieux == 1)
		{
		printf("ID_CardioVascCurieux : 3\n");
		if (DEBUG)
			IdPrintf(" _IdAcrGetPixelOffset : __ID_CardioVascCurieux = 1\n");
		fseek(fp,0L,SEEK_SET);
		}

	// -------

	if(!_IdAcrReadElement(0x0028,0x0200,fp,&gr))
                gr=0x7FE0;   // Par defaut
	if (DEBUG)
		IdPrintf (" _IdAcrGetPixelOffset : numGrPixels %x\n",gr);

	if (!(gr==0x7FE0))
		num=0x1010;  // Special PHILIPS
	else
		num=0x0010;

	if (__ID_CardioVascCurieux == 1) {
		num=0x0010;    // Special PHILIPS
		printf("ID_CardioVascCurieux : 4\n");
	}

	if (DEBUG)
		IdPrintf (" _IdAcrGetPixelOffset : numEltPixels %x\n",num);

	// -------

	if (DEBUG)
		IdPrintf(" _IdAcrGetPixelOffset : Recherche gr %x  num"
			"%x (swapping %d)\n",gr,num,sw); 

	/* --- pour remettre le pointeur de fichier d'applomb	--- */
	/* pas tres elegant --> trouver autre chose ? 		--- */
     	sw=_IdAcrCheckSwap(fp);

	if (DEBUG)
		IdPrintf (" _IdAcrGetPixelOffset : sortie _IdAcrCheckSwap\n");

	if ( __ID_CardioVascCurieux == 1)
		{
		printf("ID_CardioVascCurieux : 5\n");
		if (DEBUG)
			IdPrintf(" _IdAcrGetPixelOffset : __ID_CardioVascCurieux = 1\n");
		fseek(fp,0L,SEEK_SET);
		}

	if (DEBUG)
		IdPrintf (" \n_IdAcrGetPixelOffset : On lance le comptage\n\n");

	l=0;
	flag=0;


	while(fread(&g,2,1,fp)) {
		if(sw)
			g=SWAP_SHORT(((short)g));
		fread(&n,2,1,fp);
		if(sw)
			n=SWAP_SHORT(((short)n));
		l= _IdAcrRecupLgr(fp, &skL);
		if (DEBUG)
			IdPrintf(" _IdAcrGetPixelOffset : fread g %x "
				"n %x l %d 0x%08x\n",g,n,l,l);
      
		if((g==gr)&&(n==num)) {
			// on est sur l'element recherche
			lgrTot = lgrTot + 2 +2 + skL; 
	
			if (DEBUG)
				IdPrintf("_IdAcrGetPixelOffset : sortie avec offset  %d\n",
							lgrTot); 
			return lgrTot;

		} else if( 	( (g>gr)||((gr==g)&&(n>num)) ) &&
				( g < 0xff00 )		// cas de fou Phiplips
								     ) {
			// on a depasse l'element
				IdPrintf("_IdAcrGetPixelOffset : Kross Katastrof : on a depasse" 
					 "  le groupe des Pixels :-( \n");
		} else {  // on saute a l'element suivant
			if (g>0xff00) {
				if (DEBUG)IdPrintf("_IdAcrGetPixelOffset : cas de fou Phiplips\n");
				__cas_de_fou_Philips=1;
			}
			fseek(fp,l,SEEK_CUR);
			lgrTot = lgrTot + l +2 +2 +skL;
			if (DEBUG)
				IdPrintf("_IdAcrGetPixelOffset : on saute a "
					"l ele sv : lgrTot (part) %d\n", 
					lgrTot); 
		}
	} // While
	return(0);
}

/* ----------------------------------------------------------------------- */

/**
 * \brief	Retourne l'offset pour atteindre le groupe des Pixels 
 *		stocke dans l'entete ACRNEMA ou DICOM
 *
 * @param filename	Nom du Fichier.
 *
 * @return		longueur de l'entete.
 */

long IdAcrGetPixelOffsetFromFile(char *filename)
{
	FILE *fp;
	long result;
	if((fp=fopen(filename,ID_RFILE_BIN))==0) {
		IdErrPrintf ("echec ouverture  %s\n", filename);
		return (0);
        }
	result = _IdAcrGetPixelOffset(fp);
	fclose(fp);
	return result;
}


/* ----------------------------------------------------------------------- */


/**
 * \brief		Lecture d'un fichier de type ACR NEMA ou DICOM 
*
* 		A L'ATTENTION DES Kamikazes Creatissiens :
* 		La fonction IdDcmCVReadFile est une extention de IdAcrReadFile
* 		qui a evolue au fil des ans, en fonction des choses 
* 		que l'on a considere comme utiles de rajouter dans NOTRE entete.
* 		Puis, on a eu de 'vrais' fichiers ACR-NEMA, V2 puis V3.
* 		Puis des fichiers DICOM single-frame.
* 		Puis des fichiers DICOM multiframe.
* 		A ajouter a ca, des particularites a la limite 
*		de la mauvaise foi, dependant des constructeurs...
* 		
* 		DONC :
* 
*		Contrairement aux autres fonctions, la lecture de ce source est
*		TOUT PARTICULIEREMENT deconseillee !
* 		Ne perdez pas votre temps a essayer de comprendre POURQUOI ...
* 		La reponse est : PARCE QUE !
*
* 			
* @param filename	Nom du fichier ACR-NEMA ou DICOM SINGLE-FRAME
* @param type		Type de l'object a lire (IMA_UCHAR, SIG_CHAR, ...).
* @return		Pointeur vers l'objet lu 
*				( PSIGNAL, PPIMAGE, PPPVOLUME, PPPPSEQUENCE..).  
*/


void *IdAcrReadFile(char * filename, int type)
{
void ** tabObj=NULL;

tabObj=IdAcrReadFileIntoVector(filename, type,-2, -2, NULL);

if (tabObj==NULL) 
	return (NULL);
else
	return(tabObj[0]);
}


//-------------------------------------------------------------------------
//
// IdDcmCVReadFile est une encapsulation de IdAcrReadFileIntoVector
//

/**
 * \brief		Lecture d'un fichier de type ACR/DICOM dans un vecteur existant deja
 *
 * 		A L'ATTENTION DES Kamikazes Creatissiens :
 * 		La fonction IdAcrReadFileIntoVector est une extention de IdAcrReadFile
 * 		qui a evolue au fil des ans, en fonction des choses 
 * 		que l'on a considere comme utiles de rajouter dans NOTRE entete.
 * 		Puis, on a eu de 'vrais' fichiers ACR-NEMA, V2 puis V3.
 * 		Puis des fichiers DICOM single-frame.
 * 		Puis des fichiers DICOM multiframe.
 * 		A ajouter a ca, des particularites a la limite 
 *		de la mauvaise foi, dependant des constructeurs...
 * 		
 * 		DONC :
 * 
 *		Contrairement aux autres fonctions, la lecture de ce source est
 *		TOUT PARTICULIEREMENT deconseillee !
 * 		Ne perdez pas votre temps a essayer de comprendre POURQUOI ...
 * 		La reponse est : PARCE QUE !
 *
 * 			
 * @param filename	Nom du fichier ACR / DICOM
 * @param type		Type des images a lire (IMA_UCHAR,IMA_USHORT, ...).
 * @param premIm	Num premiere image a lire 
 *			(si -1 ou 0 on commence a la premiere).
 * @param dernIm	Num derniere image a lire a lire 
 *			(si -1 ou 0 on fini a la derniere).
 * @return		Pointeur vers un tableau de PPIMAGE
 */



void *IdDcmCVReadFile(char * filename, int type, int numPremIm, int numDernIm)
{
void ** tabObj=NULL;

tabObj=IdAcrReadFileIntoVector(filename,type, numPremIm, numDernIm, NULL);

if (tabObj==NULL) 
	return (NULL);
else
	return(tabObj[0]);
}


/* FUNCTION DESCRIPTION **************************************************

IdAcrInquireHeader  (fonction)

RESUME: Retourne TOUTES les info stockees dans l'entete ACRNEMA.

DESCRIPTION: Retourne TOUTES les info sur l'objet stocke dans un fichier ACRNEMA.
	     sous forme d'une liste d'ACR_ELEMENT.
	     la 'valeur' de chaque element est suivie d'un ZERO binaire, 
	     car il est IMPOSSIBLE ( pour IdAcrInquireHeader) de savoir, 
	     dans le cas d'un element d'un groupe prive (impair),
	       si un element de longueur 2 est un UNSIGNED SHORT INT ou un CHAR *...
	       si un element de longueur 4 est un UNSIGNED LONG      ou un CHAR *...

	    L'utilisateur desirant connaitre connaitre la valeur d'un element particulier
	    saura, en consultant le 'Conformance Statement' de l'imageur sur lequel il travaille
	    le type de cet element.
	    il procedera alors comme suit:
	 
	unsigned short int gr =0x0010;        ..  par exemple ..
	unsigned short int num=0x0010;
	ACR_ELEMENT *elem=_IdAcrFindElement(liste,gr,num);
	uint32_t lgr = elem->length;

	    Selon que :

 	    c'est un char * de lgr !=2, !=4
		On l'utilisera directement : 	printf("%s\n",elem->value);

 	    c'est un char * de lgr =4	
		On le Swappera :		*(long *)elem->value=SWAP_LONG((*(long*)elem->value));
						printf("%s\n",elem->value);
 	    c'est un char * de lgr =2	
		On le Swappera :		*(long *)elem->value=SWAP_SHORT((*(long*)elem->value));
						printf("%s\n",elem->value);
	    c'est un short int
		On le castera :			unsigned short int x = *(short int *) elem->value;

	    c'est un long int
		On le castera :			unsigned long x  = *(unsigned long *) elem->value;

Bon...
On a vu pire...			

SYNTAXE:     ACR_ELEMENT *liste = IdAcrInquireHeader (char *filename, int mode);

RETOUR:      type  : ACR_ELEMENT *
	     role  : liste d'ACR_ELEMENT

PARAMETRES:
	     nom   : filename
	     type  : char *
	     role  : Nom du fichier ACRNEMA

	     nom   : mode
	     type  : integer
	     role:
  		mode= 0 : on NE CHARGE PAS    les Elements 'longs' (>5000) ,
  			  1 : on charge EGALEMENT les Elements 'longs' (>5000) ,  		      
   		      2 : on NE charge PAS les Groupes Shadow (ni Elts Longs)
 

FICHIER : readacr.c


******************************************************** END DESCRIPTION */

/**
 * \brief	 Retourne TOUTES les info sur l'objet stocke dans un fichier ACRNEMA.
 *			sous forme d'une liste d'ACR_ELEMENT.
 *     		la 'valeur' de chaque element est suivie d'un ZERO binaire, 
 *	     	car il est IMPOSSIBLE ( pour IdAcrInquireHeader) de savoir, 
 *	     	dans le cas d'un element d'un groupe prive (impair),
 *	       	si un element de longueur 2 est un UNSIGNED SHORT INT ou un CHAR *...
 *	       	si un element de longueur 4 est un UNSIGNED LONG      ou un CHAR *...
 *
 *	    L'utilisateur desirant connaitre connaitre la valeur d'un element particulier
 *	    saura, en consultant le 'Conformance Statement' de l'imageur sur lequel il travaille
 *	    le type de cet element.
 *	    il procedera alors comme suit:
 *	 
 *	unsigned short int gr =0x0010;        ..  par exemple ..
 *	unsigned short int num=0x0010;
 *	ACR_ELEMENT *elem=_IdAcrFindElement(liste,gr,num);
 * 	uint32_t lgr = elem->length;
 *
 * 	    Selon que :
 *
 * 	    c'est un char * de lgr !=2, !=4
 * 		On l'utilisera directement : 	printf("%s\n",elem->value);
 *
 * 	    c'est un char * de lgr =4	
 *		On le Swappera :		*(long *)elem->value=SWAP_LONG((*(long*)elem->value));
 *						printf("%s\n",elem->value);
 * 	    c'est un char * de lgr =2	
 *		On le Swappera :		*(long *)elem->value=SWAP_SHORT((*(long*)elem->value));
 *						printf("%s\n",elem->value);
 *	    c'est un short int
 *		On le castera :			unsigned short int x = *(short int *) elem->value;
 *
 *	    c'est un long int
 *		On le castera :			unsigned long x  = *(unsigned long *) elem->value;
 *		
 *
 * @param filename	Nom du Fichier ACR-NEMA ou DICOM.
 * @param mode	  	= 1 : on charge EGALEMENT les Elements 'longs' (>5000) ,
 *  		    = 0 : on NE CHARGE PAS    les Elements 'longs' (>5000) ,
 * @return	Pointeur vers une ensemble chaine d'ACR_ELEMENT.
 */



ACR_ELEMENT * IdAcrInquireHeader(char *filename, int mode)
{
FILE * fp;
ACR_ELEMENT *p, *liste=NULL;

	if((fp=fopen(filename,ID_RFILE_BIN))==0) { 
		IdErrPrintf ("echec ouverture  %s\n",filename);
	 	return (NULL);
        }
  	sw= _IdAcrCheckSwap(fp);

  	if (__ID_CardioVascCurieux) {
		fseek(fp,0L,SEEK_SET);
		printf("ID_CardioVascCurieux : 6\n");
	}

  	if(sw==-1) { 
		IdErrPrintf ("fichier %s non ACR\n",filename);
		IdErrno=IDERR_NON_ACR_FILE;
		return (NULL);
        }
	__NumeroGroupePrecedent = 0; /*Pour etre sur que le premier sera + grand */

	while ( (p=_IdAcrReadNextElement(fp,mode)) ) {
		if (mode==2) {
			if(!(p->group%2)) {
				liste=_IdAcrAppendElement(liste,p);
			}
		} else {
			liste=_IdAcrAppendElement(liste,p);
		}
	}
  	return (liste);
}


/* 
* ------------------------------------------------------------------------------------------------------ 
*/

/* ------------------------------------------------------------ */
/* A partir d'un fichier Acr deja ouvert, 			*/
/* ds lequel on deja lu bufbid (8,10)				*/
/*                et	ty (signe : ( 28,103)			*/
/* on cherche le type stocke  ( 28,105 ou 28,199 )		*/
/*         ou le type deduit  ( a partir de nb et de ty )	*/
/*								*/
/*    retourne -1 si rien trouve ou rien deduit			*/
/* ------------------------------------------------------------ */

short int _IdAcrCheckType(FILE * fp, char * bufbid, int ity, int inb, int itypObj)
{
long int lgrMessage;
short int typ;
short int ty=ity;
short int nb=inb;
short int typObj=itypObj;


if (nb==12) nb=8;

typ= -1;
if (  (memcmp(bufbid,"ACRNEMA_LIBIDO",14)==0)      /* si c'est egal                  */
     ||
      (memcmp(bufbid,"CANRME_AILIBOD",14)==0)      /* (en cas d'objet ACRLibido fait sr 1 autre machine) */
    ) {
 	if (DEBUG) IdPrintf ("on a trouve ACR LIBIDO\n"); 

	if (!_IdAcrReadElement(0x0028,0x0105,fp,&typ)) 	   		/* on cherche 28,105               */
     		_IdAcrReadElement(0x0028,0x0199,fp,&typ) ;    	/* si pas trouve on cherche 28,199 */
    	if (DEBUG)  IdPrintf ("type lu sur disque %x\n",typ); 
}

if (typ==-1) {                /* on n a rien trouve (ce n'EST PAS un fichier ACR-LibIDO nouvelle version)  */
   
                              /* debut usine a gaz    */

	     if((nb==TA_TY_UCHAR*8) && (ty==0))typ = typObj | TY_UCHAR;     	/*  JPR */
	else if((nb==TA_TY_CHAR*8)  && (ty==1))typ = typObj | TY_CHAR;
	else if((nb==TA_TY_USHORT*8)&& (ty==0))typ = typObj | TY_USHORT;
	else if((nb==TA_TY_SHORT*8) && (ty==1))typ = typObj | TY_SHORT;
	else if((nb==TA_TY_ULONG*8) && (ty==0))typ = typObj | TY_ULONG;
	else if((nb==TA_TY_LONG*8)  && (ty==1))typ = typObj | TY_LONG;

	/*   ATTENTION : les representations internes des types FLOAT et DOUBLE
		  sont machine-dependent
	*/

      /* les anciens float et doubles n etaient pas signes.... */

	else if ((nb==TA_TY_FLOAT*8) /* && (ty==1) */ )	typ = typObj | TY_FLOAT;
	else if ((nb==TA_TY_DOUBLE*8)/* && (ty==1) */ )	typ = typObj | TY_DOUBLE;     /*  JPR */
    	else if ((nb==3*TA_TY_UCHAR*8)&&(ty==0))		typ = typObj | TY_RGB;        /*BB  6-JUL-1992 */
	else if ((nb==TA_TY_COMPLEX_DOUBLE*8))			typ = typObj | TY_COMPLEX_DOUBLE;

  	if (DEBUG)  IdPrintf ("Entete : nb=%d typObj=%x; Type deduit=%x\n",
				nb,typObj,typ);  

	/* ce N'EST PAS un fichier LibIDO	*/

	_message=(char *)malloc(10); /* pour eviter des soucis ulterieurs */
	lgrMessage=0;
            
  } else {   	/* fin usine a gaz    */
 
		/* C'EST un fichier LibIDO	*/
		/* on regarde si 'message' contient qq chose */

	lgrMessage= _IdAcrGetElementLength(0x0028,0x0198,fp);
	if (lgrMessage !=0) { 
		if (DEBUG) IdPrintf ("lgrMessage %ld\n", lgrMessage);  
		_message=(char *)malloc(lgrMessage+1);
		if (!_message) {
				IdErrPrintf("Impossible d allouer de la place pour _message ...\n");
				_message=(char *)malloc(10); /* pour eviter des soucis ulterieurs */
		}
		else

     		_IdAcrReadElement(0x0028,0x0198,fp,_message) ;    /*  on cherche 28,198  JPR */
	}
	else
		_message=(char *)malloc(10); /* pour eviter des soucis ulterieurs */
  }
  _message[lgrMessage]='\0';	/* Djamal Boukerroui */

  return typ;
}






/* FUNCTION DESCRIPTION **************************************************

IdAcrInquireInfo  (fonction)
IdAcrInquireInfoTot  (fonction)
IdAcrInquireInfoTot2  (fonction)
IdAcrInquireInfoTot3  (fonction)

RESUME: Retourne diverses info stockees dans l'entete ACRNEMA.

DESCRIPTION: NE PLUS UTILISER !! Retourne des info sur l'objet stocke dans un fichier ACRNEMA.

	Remplace par IdAcrInquireInfoTot4, car incomplet
	On doit le laisser, car est peut etre utilise par de vieux programmes

******************************************************** END DESCRIPTION */

int IdAcrInquireInfo ( char * filename,unsigned short int *typObj,unsigned short int *typData,int *NX,int *NY,int *NZ,int *offset )
{
unsigned short int typLibido;
int NT;
int nbFrames;
int codeSwap;
int nb;
int nbu;
int hb;
int signe;
return ( IdAcrInquireInfoTot4 ( filename,typObj,&typLibido,typData,NX,NY,NZ,&NT,offset,&nbFrames,&codeSwap,&nb,&nbu,&hb,&signe ) );

}

/* -------------------- */

int IdAcrInquireInfoTot ( char * filename, unsigned short int *typObj, unsigned short int *typLibido, unsigned short int *typData, int *NX, int *NY, int *NZ, int *NT, int *offset )
{
int nbFrames;
int codeSwap;
int nb;
int nbu;
int hb;
int signe;
return ( IdAcrInquireInfoTot4 ( filename,typObj,typLibido,typData,NX,NY,NZ,NT,offset,&nbFrames,&codeSwap,&nb,&nbu,&hb,&signe ) );
}

/* -------------------- */

int IdAcrInquireInfoTot2 ( char * filename, unsigned short int *typObj, unsigned short int *typLibido, unsigned short int *typData, int *NX, int *NY, int *NZ, int *NT, int *offset, int *nbFrames )
{
int codeSwap;
int nb;
int nbu;
int hb;
int signe;
return ( IdAcrInquireInfoTot4 ( filename,typObj,typLibido,typData,NX,NY,NZ,NT,offset,nbFrames,&codeSwap,&nb,&nbu,&hb,&signe ) );
}

/* -------------------- */

int IdAcrInquireInfoTot3 ( char * filename, unsigned short int *typObj, unsigned short int *typLibido, unsigned short int *typData, int *NX, int *NY, int *NZ, int *NT, int *offset, int *nbFrames, int *codeSwap )
{
int nb;
int nbu;
int hb;
int signe;
return ( IdAcrInquireInfoTot4 ( filename,typObj,typLibido,typData,NX,NY,NZ,NT,offset,nbFrames,codeSwap,&nb,&nbu,&hb,&signe ) );
}


/* -------------------- */

int _IdAcrIsAcrLibido(FILE *fp,char *bufbid)
{
	if ( __TrueDicom == 0) {
		// Recognition Code  --> n'existe plus en DICOM V3 ...
		//
		// ACR_NEMA 1.0 ou ACRNEMA_LIBIDO_1.1
		// On le lit ici pour eviter de revenir en arriere

		if(_IdAcrReadElement(0x0008,0x0010,fp,bufbid)!=0) { 

			if (  (memcmp(bufbid,"ACRNEMA_LIBIDO",14)==0)
				// si c'est egal
				|| (memcmp(bufbid,"CANRME_AILIBOD",14)==0))
				// en cas d'objet ACRLibido fait sr 1 autre machine) 
					return(1);
		}
	}

	return(0);
}


/* -------------------- */

void _IdAcrPositionSwap(FILE* fp)
{
    sw = _IdAcrCheckSwap(fp);
}
