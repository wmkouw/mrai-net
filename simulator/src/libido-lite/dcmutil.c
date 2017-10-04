/*************************************************************************
* $Id: dcmutil.c,v 1.4 2005/09/26 10:20:08 bellet Exp $
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

#include "idio.h"
#include "iddicom.h"
#include "iddcm-restricted.h"
#include "iderr.h"
#include <string.h>
#include <stdlib.h>

#ifdef _MSC_VER
#include <winsock.h>		// Pour ntohs - BigEndianeries -
#else
#include <netinet/in.h>  	// Pour ntohs - BigEndianeries -
#endif

#include "idliste.h"

char * _IdStrShowTransfertSyntax (char * codeTransfSynt);

#define LGR_ENTETE_A_LIRE 256				// on ne lit plus que le debut
 
#define DEBUG 0


// ON NE DECORTIQUE PLUS LES SQ (trop long, trop dangereux, inutile)

static char * var_itoa(void * , int );
static char * _cleanString(char *);

static uint32_t 	_IdDcmRecupLgr(ID_DCM_HDR *, int, int*, int *);
static int	_IdDcmCheckSwap(ID_DCM_HDR *);
static int 	_IdDcmLireEtStockerElement(ID_DCM_HDR *, char **, int,
				 int, char *, int);
	 char **	_IdDcmInquireImageInfoXXX (ID_DCM_HDR *, char **);    

static void _setAcrLibido(ID_DCM_HDR *);

// -----------------------------------------------------------------

/**
 * \ingroup       dcm
 * \brief         Alloue un ID_DCM_HDR
 * @return        Pointeur sur structure Allouée, NULL si échec 
 */
	
ID_DCM_HDR * IdDcmHdrAlloc () {
	ID_DCM_HDR *e = (ID_DCM_HDR *) calloc (1, sizeof(ID_DCM_HDR));
	if (!e) return e;
	
	e->deb = (char *) malloc(LGR_ENTETE_A_LIRE);
	if (!e->deb) { 
		free(e);
		printf("Echec alloc ID_DCM_HDR\n");
	}
	
	e->plist = IdLstAlloc();

	if (!e->plist) {
		free(e->deb);
		free(e);
		printf("Echec alloc ID_DCM_HDR->deb\n");
		return (ID_DCM_HDR *)NULL;
	}	
	return(e);
}

// ------------------------------------------------------------------
/**
 * \ingroup       dcm
 * \brief         Libere un ID_DCM_HDR
 * @param e      pointeur sur le ID_DCM_HDR a liberer.
 * @return        void 
 */

void IdDcmHdrFree (ID_DCM_HDR * e) {

	if (e) {
		if (e->deb) free(e->deb);
	 	if (e->plist) IdLstFree(e->plist); // LIBERER LES ELEMENTS DE LA LISTE AVANT
		free(e);
	}
}

/* =======================================================================
 *	_IdDcmSWAP_SHORT
 *		remet les octets dans un ordre compatible avec celui du processeur
 *  ======================================================================= */

short int _IdDcmSWAP_SHORT(short int a,int sw) {
	if ( (sw==4321)  || (sw==2143) )  
		a =(((a<<8) & 0x0ff00) | ((a>>8)&0x00ff));
	return (a);
}

/* =======================================================================
 *	_IdDcmSWAP_LONG
 *		remet les octets dans un ordre compatible avec celui du processeur
 *  ======================================================================= */

uint32_t _IdDcmSWAP_LONG(uint32_t a, int sw) {
/*       ATTENTION: il pourrait y avoir un pb pour les entiers negatifs ...
 *                                                              
 */
   switch (sw) {
	case 4321 :
		a=( 	((a<<24) & 0xff000000) | ((a<<8)  & 0x00ff0000)    | 
			((a>>8)  & 0x0000ff00) | ((a>>24) & 0x000000ff) );
		break;

	case 3412 :
       		a=( 	((a<<16) & 0xffff0000) | ((a>>16) & 0x0000ffff) );
		break;

	case 2143 :
		a=(    ((a<<8) & 0xff00ff00) | ((a>>8) & 0x00ff00ff)  );
		break;
	default :
		printf("\n\n\n *******\n erreur code swap ?!?\n\n\n");
		a=0;
   }
  return(a);
}


#define DEBUG 0

// -----------------------------------------------------------------------------------
/**
 * \ingroup       dcm
 * \brief         renvoie un pointeur sur le ID_DCM_HDR correspondant au fichier
 * @param filename      Nom du fichier ACR / LibIDO / DICOM
 * @return       le ID_DCM_HDR 
 */
 
ID_DCM_HDR * IdDcmGetHeader(char *filename) {

ID_DCM_HDR *e=NULL;
_ID_DCM_ELEM * ple;
PLIST_ELEMENT plist_elem;
unsigned int g, n, samplesPerPixel, raws, columns, bitsAllocated;
char icone, icone_Trouvee;


	e = IdDcmHdrAlloc();
	if(!e) {
		printf("echec alloc HDR \n");
		return(NULL);
	}
	
	if((e->fp=fopen(filename,ID_RFILE_BIN))==0) { 		// OK
		// IdDcmHdrAFree(e);
		printf ("echec ouverture  %s\n",filename);
	 	return (NULL);
        }
 	
 	e->filename = strdup(filename);
	 
	fseek(e->fp, 0L, SEEK_END);
	/*
	 * obtains the current value of the file-position    
	 * indicator for the stream pointed to by stream      
	 */
	e->taille_fich = ftell(e->fp);
	
	if(DEBUG) printf("IdDcmGetHeader : lgr fich %f\n",(float)e->taille_fich); // FORMAT IMPRESSION long int ???
	
	rewind(e->fp);
	 	
  	e->sw= _IdDcmCheckSwap(e);
  	
  	if(e->sw == -1) {		// On est assure que c'est NON DICOM, NON ACR
  		IdDcmHdrFree(e);
  		return (NULL);
  	}
  	
	e->__NumeroGroupePrecedent = 0; // Pour etre sur que le premier sera + grand
	e->grPixelTrouve = 0;
	e->PixelsTrouves = 0;
	
	while ( (ple=_IdDcmReadNextElement(e,e->sw)) ) {
		IdLstAddLast(e->plist, ple);
		// -->
		// --> Pour tester
		//if(e->PixelsTrouves) break;
	}
	
	// une fois le DCM-Header charge en memoire,
	// on recherche les caracteristiques eventuelle
	// d'une 'icone'
		
	icone=0;
	
        plist_elem = IdLstFirst(e->plist);
	while ( plist_elem != NULL) {
	   ple = IdLstPtrObj(plist_elem);
	   g= ple->Gr;
	   n=ple->Num;
	   if (g >  0x0008 && n >  0x2112 ) break; // depasse : pas d'icone

	   if (g == 0x0008 && n == 0x2112) {  // Source Image Sequence :
	   	icone=1;
	   	break;
	   }
	   plist_elem=IdLstNext(plist_elem);   
	}
	
	icone_Trouvee=0;
	if (icone == 1 ) {
	
	   printf ("rech iconeTrouvee\n");

	   while ( plist_elem != NULL) {
	      ple = IdLstPtrObj(plist_elem);
	      g= ple->Gr;
	      n= ple->Num;
	      
	      if (g != 0xfffe && (g >  0x0088 && n >  0x0200)) break; // du soucis a se faire si on sort par ici
	      if (g == 0x0088 && n == 0x0200) {  // Icone Image Sequence
	   	icone_Trouvee=1;
	   	break;
	      }
	      plist_elem=IdLstNext(plist_elem);
	   }   
	}
	
	if (icone_Trouvee == 1 ) {
	
			printf ("rech valeurs\n");

	   while ( plist_elem != NULL) {
	      ple = IdLstPtrObj(plist_elem);
	      g= ple->Gr;
	      if ( g != 0xfffe && g > 0x0088) break; // on a du tous les trouver
	      n=ple->Num;
	      // on est dans la SQ, group vaut 0008 ...	      	
	      if      (n == 0x0002) samplesPerPixel = str2num(ple->valeurElem,unsigned short int);
	      else if (n == 0x0010) raws            = str2num(ple->valeurElem,unsigned short int); 
	      else if (n == 0x0011) columns         = str2num(ple->valeurElem,unsigned short int);
	      else if (n == 0x0100) bitsAllocated   = str2num(ple->valeurElem,unsigned short int);
	   
	      plist_elem  = IdLstNext(plist_elem);
	   } 
	}  
	
	// on ajuste PixelPosition, pour 'sauter' l'icone.
	if (icone_Trouvee) {
	
	// PAS SUFFISANT : il y a des Koshon'ries apres les pixels imagette !
	
	        printf ("samplesPerPixel %d raws %d  columns %d bitsAllocated %d\n", 
	 	   samplesPerPixel, raws, columns, bitsAllocated);
	        printf("pixelPosition %d x(%x) \n", 
	            e->PixelPosition,e->PixelPosition);
	   e->PixelPosition += samplesPerPixel * raws * columns * bitsAllocated / 8;
	        printf("pixelPosition %d x(%x)\n",e->PixelPosition, e->PixelPosition);
	}
	
 	// Positionnement ACR_LIBIDO
 	
	_setAcrLibido(e);
		
	fclose(e->fp);
  	return (e);
}

static void _setAcrLibido(ID_DCM_HDR *e) {

_ID_DCM_ELEM * ple;
PLIST_ELEMENT plelem;
PLIST pl;

	// Positionnement ACR_LIBIDO
	if(DEBUG) printf("Entree ds _setAcrLibido\n");

	e->ACR_LIBIDO = 0;
	if ( e->__TrueDicom == 0) {
		// Recognition Code  --> n'existe plus en DICOM V3 ...
		
		pl = e->plist;
		plelem = IdLstFirst(pl);
		while (plelem) {
			ple= IdLstPtrObj(plelem);
			if(DEBUG) printf("gr %04x Num %04x\n", ple->Gr, ple->Num);
			if(ple->Gr >  0x0008) break; 		// On a depasse
			if(ple->Gr == 0x0008) {
				if(ple->Num >  0x0010) break;	// On a depasse
				if(ple->Num == 0x0010) {
					if (  (memcmp(ple->valeurElem,"ACRNEMA_LIBIDO",14)==0)
						// si c'est egal
						|| (memcmp(ple->valeurElem,"CANRME_AILIBOD",14)==0)) {
						// en cas d'objet ACRLibido fait sr 1 autre machine) 
						e->ACR_LIBIDO =1;	
					}  // fin if memcmp
					break;		
				} // fin if ple->Num==0x0010
			} // fin ple->Gr==0x0008
			plelem = IdLstNext(plelem);
		} // fin while 
	} // fin if TrueDicom
	
		if(DEBUG) printf("ACR_LIBIDO = %d\n", e->ACR_LIBIDO);
	return;
}
/* ======================================================================= 
*	_IdDcmRecupLgr
*
*	ACR-NEMA :	On a toujours 
*				GroupNumber   (2 Octets) 
*				ElementNumber (2 Octets) 
*				ElementSize   (4 Octets)
*
*
*	DICOM :		On peut avoir (implicit Value Representation)
*				GroupNumber   (2 Octets) 
*				ElementNumber (2 Octets) 
*				ElementSize   (4 Octets)
*
*			On peut avoir (explicit Value Representation)
*				GroupNumber         (2 Octets) 
*				ElementNumber       (2 Octets) 
*				ValueRepresentation (2 Octets) 
*				ElementSize         (2 Octets)
*
*			ATTENTION : dans le cas ou ValueRepresentation = OB, OW, SQ, UN
*				GroupNumber         (2 Octets) 
*				ElementNumber       (2 Octets) 
*				ValueRepresentation (2 Octets)
*				zone reservee       (2 Octets) 
*				ElementSize         (4 Octets)
*
*
*   ======================================================================= */
/**
 * \ingroup       dcm
 * \brief         recupere la longueur d'un champ DICOM.
 *			(le fichier doit deja avoir ete ouvert,
 *			 _IdAcrCheckSwap(ID_DCM_HDR *e) avoir ete appele)
 *			et la  partie 'group'  ainsi que la  partie 'elem' 
 * 		        de l'acr_element doivent avoir ete lues.
 * @param sw      			code swap
 * @param skippedLength     pointeur sur nombre d'octets que l'on a saute qd la lecture est finie
 * @param longueurLue       pointeur sur longueur (en nombre d'octets) effectivement lue

 * @return        			longueur retenue pour le champ 
 */

static uint32_t _IdDcmRecupLgr(ID_DCM_HDR *e, int sw, int *skippedLength, int *longueurLue) {
uint32_t l_gr; 
unsigned short int l_gr_2;
int i, trouve;
char VR[5];
int lgrLue;

// ID_DCM_HDR *e sert uniquement de passe-plat pour  __ExplicitVR


if (e->__ExplicitVR == 1) {
	lgrLue=fread (&VR, (size_t)2,(size_t)1, e->fp);
	VR[2]=0;

	// ATTENTION :
	// Ce n'est pas parce qu'on a trouve UL la premiere fois qu'on respecte 
	// Explicit VR tout le temps
	// (cf e=film ...)

	//for(i=0,trouve=0;i<nbCode;i++) {
	
	  for(i=0,trouve=0;_ID_dicom_vr[i].dicom_VR != NULL ;i++) {
		if(memcmp(_ID_dicom_vr[i].dicom_VR,VR,(size_t)2)==0) {
			(e->pleCourant)->VR=_ID_dicom_vr[i].dicom_VR;
			trouve=1;	
			break;
		}
	}

	if ( trouve == 0) {

		// On est mal : implicit VR repere
		// mais ce n'est pas un code connu ...
		// On reconstitue la longueur
		
		if(DEBUG) printf("IdDcmRecupLgr : Explicit VR, mais pas trouve de code connu\n");
		memcpy(&l_gr, VR,(size_t)2);

		lgrLue=fread ( ((char*)&l_gr)+2, (size_t)2, (size_t)1, e->fp);

		if(sw) l_gr = _IdDcmSWAP_LONG(((uint32_t)l_gr),sw);
		
		if(DEBUG) printf("IdDcmRecupLgr : lgr deduite : %08x , %d\n",l_gr,l_gr);
		
		*longueurLue=l_gr;
		if ( (int)l_gr == -1) {	
			l_gr=0;
		}
		*skippedLength = 4; 
		if (DEBUG) printf(" 1 : lgr %08x (%d )skippedLength %d\n",l_gr,l_gr, *skippedLength);
		return(l_gr);
	}

	// On repart dans la sequence 'sensee'

	if(DEBUG) printf("VR : [%01x , %01x] (%c%c) en position %d du tableau\n", VR[0],VR[1],VR[0],VR[1],i);
	//printf(" %d , %s\n", i,_ID_dicom_vr[i].dicom_VR);
	
	if ( 	
	   (!memcmp( VR,"OB",(size_t)2 )) || 
	   (!memcmp( VR,"OW",(size_t)2 )) || 
	   (!memcmp( VR,"SQ",(size_t)2 )) ||
	   (!memcmp( VR,"UN",(size_t)2 )) ) {

	// les 2 octets suivants sont reserves

		if(DEBUG) printf("IdDcmRecupLgr : les 2 octets suivants sont reserves\n");
				 	//on les saute		
		fseek(e->fp, 2L,SEEK_CUR);
		
					//on lit la lgr sur QUATRE octets

		lgrLue=fread (&l_gr, (size_t)4,(size_t)1, e->fp);

		if(sw) l_gr = _IdDcmSWAP_LONG(((uint32_t)l_gr),sw);
		*skippedLength = 8;

	} else {
 					//on lit la lgr sur DEUX octets

		lgrLue=fread (&l_gr_2, (size_t)2,(size_t)1, e->fp);

		if(sw) l_gr_2 = _IdDcmSWAP_SHORT((unsigned short)l_gr_2,sw);
		
		*longueurLue=l_gr_2;

		if ( l_gr_2 == 0xffff) {
			l_gr = 0;	
		} else {
			l_gr = l_gr_2;
		}
		*skippedLength = 4;
	}	
  } else {	// Explicit VR = 0	

 				//on lit la lgr sur QUATRE octets

	lgrLue=fread (&l_gr, (size_t)4,(size_t)1, e->fp);

	if(sw)l_gr=_IdDcmSWAP_LONG(((long)l_gr),sw);
	*skippedLength = 4;
  }
  
  *longueurLue=l_gr;
  
  // Traitement des curiosites sur la longueur
  
  if ( (int)l_gr == 0xffffffff)
	l_gr=0; 
	
  if(!memcmp( VR,"SQ",(size_t)2 )) { 	// ca annonce une SEQUENCE d'items ?!
  
 	//l_gr=0;							// on lira donc les items de la sequence 
 	
 	// devrait permettre de SAUTER toures les SQ
 	
 	if (DEBUG) printf(" SQ trouve : lgr %d \n",l_gr);
  }
 	
if (DEBUG) printf(" 2 : lgr %08x (%d) skippedLength %d\n",l_gr,l_gr, *skippedLength);
  return(l_gr);
}

/* =======================================================================
	_IdDcmReadNextElement

			lit l'acr_element courant 
			le fichier doit deja avoir ete ouvert, 
                 
   ======================================================================= */


/**
 * \ingroup       dcm
 * \brief         lit le dicom_element suivant.
 *			(le fichier doit deja avoir ete ouvert,
 *			 _IdAcrCheckSwap(ID_DCM_HDR *e) avoir ete appele)
 * @param e      ID_DCM_HDR  dans lequel effectuer la recherche.
 * @param sw    	code swap.
 * @return        	En cas de succes, 1 
 *               	0 en cas d'echec.
 */



_ID_DCM_ELEM * _IdDcmReadNextElement(ID_DCM_HDR * e, int sw) {
unsigned short g;
unsigned short n;
uint32_t l;
long int posFich;
int skL;
size_t lgrLue;
_ID_DCM_ELEM *nouvDcmElem;

	if (DEBUG) printf(" ===> entree ds _IdDcmReadNextElement\n");
		
	if(e->offsetCourant ==	e->taille_fich) { // On a atteint la fin du fichier
		if (DEBUG) printf(" On a atteint la fin du fichier\n");
		return(NULL);
	} else {
		if (DEBUG) {
				posFich = ftell(e->fp);
				printf("lgrFich %f positionDsFich %f offset courant %f x(%x)\n", 
							(float)e->taille_fich,
							(float)posFich,
							(float)e->offsetCourant,
							 e->offsetCourant);
		}
	} 
	
	nouvDcmElem = (_ID_DCM_ELEM *)malloc(sizeof(_ID_DCM_ELEM));
	if (!nouvDcmElem) {
		printf("Echec alloc _ID_DCM_ELEM *nouvDcmElem\n");
		return(NULL);
	}
	
	e->pleCourant = nouvDcmElem;
	
	// ------------------------- Lecture Num group : g

	lgrLue=fread (&g, (size_t)2,(size_t)1, e->fp);
	
	if (feof(e->fp))  {
		if (DEBUG) printf("_IdDcmReadNextElement : eof trouve\n");			
		return (NULL);
	}
	if (ferror(e->fp)){
		if (DEBUG) printf(" IdDcmReadNextElement : echec lecture NumGr\n");
		return (NULL);
	}

	if (DEBUG) printf("_IdDcmReadNextElement : "
					" gr  %04x\n",g );
						
	if (sw)	g=_IdDcmSWAP_SHORT(((short)g),sw);
	
	nouvDcmElem->Gr=g;
	e->__NumeroGroupePrecedent =g;
	
	// ------------------------- Lecture Num Elem : n

	lgrLue=fread (&n, (size_t)2,(size_t)1, e->fp);
	
	if (feof(e->fp))  {
		if (DEBUG) printf("_IdDcmReadNextElement : eof trouve\n");			
		return (NULL);
	}
	if (ferror(e->fp)){
		if (DEBUG) printf(" IdDcmReadNextElement : echec lecture NumElem\n");
		return (NULL);
	}
	
	if (DEBUG) printf("_IdDcmReadNextElement : "
					" num %04x\n",n );

	if(sw)n=_IdDcmSWAP_SHORT(((short)n),sw);
	nouvDcmElem->Num=n;

	// ------------------------- Lecture longueur element : l
								
	l = _IdDcmRecupLgr(e, sw, &skL, &nouvDcmElem->LgrLueElem);
			
	if(g==0xfffe) l=0;  // pour sauter les indicateurs de 'SQ'

	nouvDcmElem->LgrElem=l;

	if (DEBUG) if (n!=0) printf("_IdDcmReadNextElement : "
					" gr %04x\tnum %04x\tlong %08x (%d)\n",
					g,n,l,l);				
					
	// ------------------------- Lecture Valeur element 
		
	nouvDcmElem->valeurElem	= malloc(l+1);
	if(nouvDcmElem->valeurElem) {
		nouvDcmElem->valeurElem[l]= 0;
	} else {	
		if (DEBUG) printf(" IdDcmReadNextElement : echec Alloc valeurElem lgr : %d\n",l);
		return (NULL);
	}	
	
	// ------------------------ On amene en mémoire y compris les Elem 'tres longs'
			
	lgrLue=fread (nouvDcmElem->valeurElem, (size_t)l,(size_t)1, e->fp);	
	
	e->offsetCourant +=  2 + 2 + skL; // gr +  num + lgr
	nouvDcmElem->Offset = e->offsetCourant;
	e->offsetCourant += l;			// debut elem suivant


	// ------------------------- Doit-on le Swapper ?
	

	if ((n==0) && sw)  {	// n=0 : lgr du groupe : uint32_t
			*(uint32_t *) nouvDcmElem->valeurElem =
					_IdDcmSWAP_LONG  ((*(uint32_t *) nouvDcmElem->valeurElem),sw);  
			nouvDcmElem->Swap = 1;
	} else {
		if(sw) {
			if ( (g/2)*2-g==0) { /* on ne teste pas les groupes impairs */

				if ((l==4)||(l==2)) {  /*pour ne s'interroger que sur les elements 
										de lgr =2 ou =4 */

					// On a deja trouve la VR lors de RecupLgr
					// On la reutilise
					
					if (    (strcmp(nouvDcmElem->VR,"UL") ==0) 
			  	  	 	 || (strcmp(nouvDcmElem->VR,"US") ==0)
			  	  	  	 || (strcmp(nouvDcmElem->VR,"SL") ==0) 
			  	  	  	 || (strcmp(nouvDcmElem->VR,"SS") ==0) 
          				  	 || (g == 0x0028 && 
						   ( n == 0x0005 || n == 0x0200) )	) { // seuls (28,5) de vr RET
									   		    //  et (28,200) sont des entiers
									   		    // ... jusqu'a preuve du contraire
						nouvDcmElem->Swap = 1;

						if(l==4) { 
						   	*(uint32_t *) nouvDcmElem->valeurElem=
						   		_IdDcmSWAP_LONG  ((*(uint32_t *) nouvDcmElem->valeurElem),sw); 
							nouvDcmElem->valInt =
						   		_IdDcmSWAP_LONG  ((*(uint32_t *) nouvDcmElem->valeurElem),sw); 		
						} else {
							if(l==2) 
						    *(unsigned short *)   nouvDcmElem->valeurElem=
						    	_IdDcmSWAP_SHORT ((*(unsigned short *)nouvDcmElem->valeurElem),sw);
						    	
							nouvDcmElem->valShort =
						    	_IdDcmSWAP_SHORT ((*(unsigned short *)nouvDcmElem->valeurElem),sw);
						 }
					}
				} /* fin if l==2 ==4 */
			} /* fin if g pair */
		} /* fin sw */	
	}

	// ------------------------- A-t-on trouve l'info donnant le 'num groupe' des Pixels ?
	
								
	if (!e->grPixelTrouve) {			// on n a pas encore trouve les pixels
	
		if ( (g == 0x0028 && n > 0x0200) || g > 0x0028 ) { // on a depasse (28,200)
			e->grPixel  = 0x7FE0;
			e->numPixel = 0x0010;
			e->grPixelTrouve = 1;
			if (DEBUG) printf("-----------------par defaut, en %04x %04x :-------grPixel %04x numPixel %04x\n",
					g,n,e->grPixel,e->numPixel);				
		} else {					// on est sur (28,200)
			if (g == 0x0028) {
				if (n == 0x0200) {
					e->grPixelTrouve = 1;
					
					*(unsigned short *)nouvDcmElem->valeurElem= 
						_IdDcmSWAP_SHORT ((*(unsigned short *)nouvDcmElem->valeurElem),sw);
					
					e->grPixel = str2num(nouvDcmElem->valeurElem, short int);
										
					
					if (e->grPixel == 0xE07F) { 
						e->grPixel=0x7FE0;
					}					
					
					if (DEBUG) printf("-----------28 200 trouve-------------GrPixel %04x\n",
										e->grPixel);
					
					if (e->grPixel != 0x7FE0)		// Vieux pb Philips
						e->numPixel = 0x1010;		// encore utile ??
					else
						e->numPixel = 0x0010;
					if (DEBUG) printf("------------------------grPixel %04x numPixel %04x\n",
											e->grPixel,e->numPixel);
				}
			}	
		}	
	} else {					// on vient de trouver les pixels
		if (g == e->grPixel) {
			if (n == e->numPixel) {
				e->PixelPosition = nouvDcmElem->Offset; 
				e->PixelsTrouves = 1;
			if (DEBUG) printf(" \t===x=> Pixels Trouves en %d\n",e->PixelPosition);				
			}	
		}
	}		
	e->nbElem ++;
	if (DEBUG) printf("nb Elem %d\n",e->nbElem);
	return(nouvDcmElem);
}
 
 
 #define DEBUG 0
 
 /* ============================================================================

	SPESHEUL FUNKSHEUNZ TO READ NAADIN ZOBOROMOK LEONARDO IMAGES
	
	N.B. : LEONARDO, c'est la console Siemens .
		Pendant plusieurs mois, les entetes des images etaient verolees
		Des longueurs de Champs etaient fausses
		Ces deux fonctions ne servent qu'a pouvoir relire ces images
		(on ne les a pas conservées sous une autre forme :-(

  ============================================================================= */
  
  

/* =======================================================================
	_IdDcmReadNextElementLeonardo

			lit l'acr_element courant (Special images foirées Leonardo)
			le fichier doit deja avoir ete ouvert, 
                 
   ======================================================================= */

/**
 * \ingroup       dcm
 * \brief         lit le dicom_element suivant.
 *			(le fichier doit deja avoir ete ouvert,
 *			 _IdAcrCheckSwap(ID_DCM_HDR *e) avoir ete appele)
 * @param e      ID_DCM_HDR  dans lequel effectuer la recherche.
 * @param sw    	code swap.
 * @return        	En cas de succes, 1 
 *               	0 en cas d'echec.
 */
 
//_ID_DCM_ELEM * _IdDcmReadNextElement(ID_DCM_HDR * e, int sw) {

_ID_DCM_ELEM * _IdDcmReadNextElementLeonardo(ID_DCM_HDR * e, int sw) {
unsigned short g;
unsigned short n;
uint32_t l;
long int posFich;
int skL;
size_t lgrLue;
int i;
_ID_DCM_ELEM *nouvDcmElem;

	if (DEBUG) printf(" ===> entree ds _IdDcmReadNextElement\n");
		
	if(e->offsetCourant ==	e->taille_fich) { // On a atteint la fin du fichier
		if (DEBUG) printf(" On a atteint la fin du fichier\n");
		return(NULL);
	} else {
		if (DEBUG) {
				posFich = ftell(e->fp);
				printf("lgrFich %f positionDsFich %f offset courant %f\n", 
							(float)e->taille_fich,
							(float)posFich,
							(float)e->offsetCourant);
		}
	} 
	
	nouvDcmElem = (_ID_DCM_ELEM *)malloc(sizeof(_ID_DCM_ELEM));
	if (!nouvDcmElem) {
		printf("Echec alloc _ID_DCM_ELEM *nouvDcmElem\n");
		return(NULL);
	}
	
	e->pleCourant = nouvDcmElem;
	
	// ------------------------- Lecture Num group : g

	lgrLue=fread (&g, (size_t)2,(size_t)1, e->fp);
	
	if (feof(e->fp))  {
		if (DEBUG) printf("_IdDcmReadNextElement : eof trouve\n");			
		return (NULL);
	}
	if (ferror(e->fp)){
		if (DEBUG) printf(" IdDcmReadNextElement : echec lecture NumGr\n");
		return (NULL);
	}

	if (DEBUG) printf("_IdDcmReadNextElement : "
					" gr  %04x\n",g );
						
	if (sw)	g=_IdDcmSWAP_SHORT(((short)g),sw);
	
	nouvDcmElem->Gr=g;
	e->__NumeroGroupePrecedent =g;
	
	// ------------------------- Lecture Num Elem : n

	lgrLue=fread (&n, (size_t)2,(size_t)1, e->fp);
	
	if (feof(e->fp))  {
		if (DEBUG) printf("_IdDcmReadNextElement : eof trouve\n");			
		return (NULL);
	}
	if (ferror(e->fp)){
		if (DEBUG) printf(" IdDcmReadNextElement : echec lecture NumElem\n");
		return (NULL);
	}
	
	if (DEBUG) printf("_IdDcmReadNextElement : "
					" num %04x\n",n );

	if(sw)n=_IdDcmSWAP_SHORT(((short)n),sw);
	nouvDcmElem->Num=n;

	// ------------------------- Lecture longueur element : l
								
	l = _IdDcmRecupLgr(e, sw, &skL, &nouvDcmElem->LgrLueElem);
	

		// ---------------- images delirantes LEONARDO
	
		if( (nouvDcmElem->Gr == 0x0009) && ( (nouvDcmElem->Num == 0x1113) || (nouvDcmElem->Num == 0x1114) ) )
		{
		
			if (DEBUG) printf ("gr %04x  num %04x LgrElem %d\n",
						nouvDcmElem->Gr, nouvDcmElem->Num, nouvDcmElem->LgrElem);
			l=4;
			nouvDcmElem->LgrElem =4;
			nouvDcmElem->LgrLueElem =4;			
		}
		
		// ---------------fin LEONARDO ---------------------------------------------------
		
	if(g==0xfffe) l=0;  // pour sauter les indicateurs de 'SQ'

	nouvDcmElem->LgrElem=l;

	if (DEBUG) if (n!=0) printf("_IdDcmReadNextElement : "
					" gr %04x\tnum %04x\tlong %08x (%d)\n",
					g,n,l,l);				
					
	// ------------------------- Lecture Valeur element 
		
	nouvDcmElem->valeurElem	= malloc(l+1);
	if(nouvDcmElem->valeurElem) {
		nouvDcmElem->valeurElem[l]= 0;
	} else {	
		if (DEBUG) printf(" IdDcmReadNextElement : echec Alloc valeurElem lgr : %d\n",l);
		return (NULL);
	}	
	
		// ------------------------ On amene en mémoire y compris les Elem 'tres longs'
			
	lgrLue=fread (nouvDcmElem->valeurElem, (size_t)l,(size_t)1, e->fp);	
	
	e->offsetCourant +=  2 + 2 + skL; // gr +  num + lgr
	nouvDcmElem->Offset = e->offsetCourant;
	e->offsetCourant += l;							// debut elem suivant
	
	// ------------------------- Doit-on le Swapper ?
	

	if ((n==0) && sw)  {	// n=0 : lgr du groupe : uint32_t
			*(uint32_t *) nouvDcmElem->valeurElem =
					_IdDcmSWAP_LONG  ((*(uint32_t *) nouvDcmElem->valeurElem),sw);  
			nouvDcmElem->Swap = 1;
	} else {
		if(sw) {
			if ( (g/2)*2-g==0) { /* on ne teste pas les groupes impairs */

				if ((l==4)||(l==2)) {  /*pour eviter de swapper les chaines 
										de lgr 2 ou 4 */

					// On a deja trouve la VR lors de RecupLgr
					// On la reutilise
					
					if (    (strcmp(nouvDcmElem->VR,"UL") ==0) 
			  	  	 	 || (strcmp(nouvDcmElem->VR,"US") ==0)
			  	  	  	 || (strcmp(nouvDcmElem->VR,"SL") ==0) 
			  	  	  	 || (strcmp(nouvDcmElem->VR,"SS") ==0) 
          				  	 || (g == 0x0028 && 
						   ( n == 0x0005 || n == 0x0200) )	) { // seuls (28,5) de vr RET
									   						 	//  et (28,200) sont des entiers
									   						 	// ... jusqu'a preuve du contraire
						nouvDcmElem->Swap = 1;

						if(l==4) { 
						   	*(uint32_t *) nouvDcmElem->valeurElem=
						   		_IdDcmSWAP_LONG  ((*(uint32_t *) nouvDcmElem->valeurElem),sw); 
							nouvDcmElem->valInt =
						   		_IdDcmSWAP_LONG  ((*(uint32_t *) nouvDcmElem->valeurElem),sw); 		
						} else {
							if(l==2) 
						    *(unsigned short *)   nouvDcmElem->valeurElem=
						    	_IdDcmSWAP_SHORT ((*(unsigned short *)nouvDcmElem->valeurElem),sw);
							nouvDcmElem->valShort =
						    	_IdDcmSWAP_SHORT ((*(unsigned short *)nouvDcmElem->valeurElem),sw);
						 }
					}
				} /* fin if l==2 ==4 */
			} /* fin if g pair */
		} /* fin sw */	
	}
	
	// ------------------------- A-t-on trouve l'info donnant le 'num groupe' des Pixels ?
								
	if (!e->grPixelTrouve) {			// on n a pas encore trouve les pixels
		if (g > 0x0028) {
				if (n > 0x0200 || g == 0x7FE0 ) {  // on a depasse (28,200)
					e->grPixel  = 0x7FE0;
					e->numPixel = 0x0010;
					e->grPixelTrouve = 1;
					if (DEBUG) printf("------------------------grPixel %04x numPixel %04x\n",
										e->grPixel,e->numPixel);
				}
						
		} else {					// on est sur (28,200)
			if (g == 0x0028) {
				if (n == 0x0200) {
					e->grPixelTrouve = 1;
					for(i=0;i<4;i++)
						*((char*)(&e->grPixel)+i) = *(nouvDcmElem->valeurElem+i); 
					
					
					if (DEBUG) printf("------------------------GrPixel %04x\n",
										e->grPixel);
					
					if (e->grPixel != 0x7FE0)		// Vieux pb Philips
						e->numPixel = 0x1010;		// encore utile ??
					else
						e->numPixel = 0x0010;
						if (DEBUG) printf("------------------------grPixel %04x numPixel %04x\n",
											e->grPixel,e->numPixel);
				}
			}	
		}	
	} else {					// on vient de trouver les pixels
		if (g == e->grPixel) {
			if (n == e->numPixel) {
				e->PixelPosition = nouvDcmElem->Offset; 
				e->PixelsTrouves = 1;
			/*if (DEBUG)*/printf(" \t=$$$$$$$==> Pixels Trouves en %d\n",e->PixelPosition);					
			}	
		}
	}
	e->nbElem ++;
	return(nouvDcmElem);
}
 
 
 // -----------------------------------------------------------------------------------
/**
 * \ingroup       dcm
 * \brief         renvoie un pointeur sur le ID_DCM_HDR correspondant au fichier
 * @param filename      Nom du fichier ACR / LibIDO / DICOM
 * @return       le ID_DCM_HDR 
 */
 
ID_DCM_HDR * IdDcmGetHeaderLeonardo(char *filename) {

ID_DCM_HDR *e=NULL;
_ID_DCM_ELEM * ple;

	e = IdDcmHdrAlloc();
	if(!e) {
		printf("echec alloc HDR \n");
		return(NULL);
	}
	
	if((e->fp=fopen(filename,ID_RFILE_BIN))==0) { 		// OK
		// IdDcmHdrAFree(e);
		printf ("echec ouverture  %s\n",filename);
	 	return (NULL);
        }
 	
 	e->filename = strdup(filename);
	 
	fseek(e->fp, 0L, SEEK_END);
	/*
	 * obtains the current value of the file-position    
	 * indicator for the stream pointed to by stream      
	 */
	e->taille_fich = ftell(e->fp);
	
	if(DEBUG) printf("IdDcmGetHeader : lgr fich %f\n",(float)e->taille_fich); // FORMAT IMPRESSION long int ???
	
	rewind(e->fp);
	 	
  	e->sw= _IdDcmCheckSwap(e);
	e->__NumeroGroupePrecedent = 0; // Pour etre sur que le premier sera + grand
	e->grPixelTrouve = 0;
	e->PixelsTrouves = 0;

	
	while ( (ple=_IdDcmReadNextElementLeonardo(e,e->sw)) ) {
		IdLstAddLast(e->plist, ple);
	}
	
 	// Positionnement ACR_LIBIDO
 	
	_setAcrLibido(e);
		
	fclose(e->fp);
  	return (e);
}


 
/* ============================================================================

	END OF SPESHEUL FUNKSHEUNZ TO READ NAADIN ZOBOROMOK LEONARDO IMAGES

  ============================================================================= */
  
  

/* =======================================================================

	_IdDcmCheckSwap
		La seule maniere sure que l'on aie pour determiner 
		si on est en 	LITTLE_ENDIAN, 	   BIG-ENDIAN, 
						BAD-LITTLE-ENDIAN, BAD-BIG-ENDIAN
		est de trouver l'element qui donne la longueur d'un 'GROUP'
		(on sait que la longueur de cet element vaut 0x00000004)
		et de regarder comment cette longueur est codee en memoire  

		Le probleme vient de ce que parfois, il n'y en a pas ...

		On fait alors le pari qu'on a a faire a du LITTLE_ENDIAN propre.
		(Ce qui est la norme -pas respectee- depuis ACR-NEMA)
		Si ce n'est pas le cas, on ne peut rien faire.

		(il faudrait avoir des fonctions auxquelles 
		 on passe le code Swap en parametre, pour faire des essais 'manuels')

   ======================================================================= */

static int
_IdDcmCheckSwap(ID_DCM_HDR * e) {
uint32_t  s, x=4;  // x : pour ntohs

int sw;
int lgrLue;
char * entCur;
char deb[LGR_ENTETE_A_LIRE];
short int shortS;


// On teste le processeur

	if (x==(x)) {	/* si le HostByteOrder est le meme que le NetworkByteOrder  */ 
		e->net2host = 1;
	} else {
		e->net2host = 0;
	}
	
	//printf("\t\t\t\te->net2host %d \n",e->net2host);
	
/* On commence par verifier si c'est du DICOM 'actuel' */
/*                                      -------------  */

	lgrLue = fread(deb,1,LGR_ENTETE_A_LIRE,e->fp);

	entCur = deb+128;
	if(memcmp(entCur, "DICM", (size_t)4) == 0) {
		e->__TrueDicom=1;
		if (DEBUG) printf ("_IdDcmCheckSwap : C est du DICOM actuel \n");
	} else {
		e->__TrueDicom=0;
		if (DEBUG) printf ("_IdDcmCheckSwap : Ce n'est PAS du DICOM actuel\n"); 
	}

	if(e->__TrueDicom) {
		entCur = deb+136;	/* on saute le File Preamble (souvent a ZERO) : 128 Octets */
					/* le DICM, et le 0002, 0000 ou le 0002, 0001              */
   		if( (memcmp(entCur, "UL", (size_t)2) == 0) ||
   		    (memcmp(entCur, "OB", (size_t)2) == 0) )
   			{
   			/* les 2 premiers octets de la lgr peuvent valoir UL --> Explicit VR */
			e->__ExplicitVR = 1;
			if (DEBUG)	printf ("_IdDcmCheckSwap : Explicit VR\n");
   			} else {
				e->__ExplicitVR = 0;
			if (DEBUG)	printf ("_IdDcmCheckSwap : PAS Explicit VR\n");
   			}

   		if (e->net2host == 0) {	/* si le HostByteOrder est different du NetworkByteOrder  */ 
			sw = 0;			/* on est sur PC ou DEC --> LITTLE-ENDIAN -> Rien a faire */
			if (DEBUG) printf("HostByteOrder = NetworkByteOrder\n");
	
   		} else {		/* on est sur une Sun ou une SGI	*/
			sw = 4321;
			if (DEBUG) printf("HostByteOrder != NetworkByteOrder\n");
   		}
  		 
  		rewind(e->fp);
  		fseek (e->fp, 132L, SEEK_SET); //On se positionne sur le debut des info
  		e->offsetCourant=132;
  		return sw;

	} /* fin TrueDicom */
	
/* -----  SINON ----- */
	
/* Si c'est de l'ACR 'propre', la lgr du premier element du groupe est FORCEMENT 4 */

	entCur=deb + 4;
	s=str2num(entCur,int);
		 
    	switch (s) {
		case 0x00040000 :
			sw=3412; if(DEBUG) printf("s : %08x sw : %d\n",s,sw);
			break;
		case 0x04000000 :
			sw=4321; if(DEBUG) printf("s : %08x sw : %d\n",s,sw);
			break;
		case 0x00000400 :
			sw=2143; if(DEBUG) printf("s : %08x sw : %d\n",s,sw);
			break;
		case 0x00000004 :
			sw=0;    if(DEBUG) printf("s : %08x sw : %d\n",s,sw);
			break;
		default :
			sw = -1;
      			if (DEBUG) printf (" Pas trouve l info de Swap; On va parier\n");
 	}
 		
/* Si c'est de l'ACR 'pas propre', il manque la lgr du groupe 		*/
				
	if(sw==-1) {						
		/* On n'a pas trouve l'info de swap (lgr de l'elem (0008,0000) = 4) */
		/* Si c'est du VRAI ACR NEMA  on DOIT trouver le numero du premier groupe : 0008	*/
		/* Les seules possibilités sont (Good)BigEndian,(Good)LittleEndian		
		*/
	  		
	  	shortS=str2num(deb,short int);
	  	
    			switch (shortS) {
				case 0x0008 :
					sw=0; 
					if(DEBUG) printf("shortS : %04x sw : %d\n",shortS,sw);
					break;
				case 0x0800 :
					sw=4321; 
					if(DEBUG) printf("shortS : %08x sw : %d\n",shortS,sw);
					break;
				default :
				sw = -1;
      				if (DEBUG) printf (" Meme pas trouve 0008 (premier groupe); c est du RAW\n");
 		}
	}
  	rewind(e->fp);		// les info commencent au debut
  	e->offsetCourant=0;
	return (sw);
}


//
/* ================================================================== */
//

/**
 * \ingroup       dcm
 * \brief	renvoie dans un tableau de chaines de caracteres
 		la description des info pertinentes d'une image ACR /DCM
 		(la soixantaine de champs utiles)
 		a partir du nom du fichier
 * 
 * @param filename	Nom du fichier.
 * @return		ID_DCM_HDR
 * 
 * 
 * Pour avoir la liste a jour des elements renvoyes dans ce tableau,
 * Consulter src/idacr.h et src/dicom.c
 * 
*/
/* Pour rajouter une entree ici,

	penser a modifier le fichier dicom.c
	penser a modifier la fonction idacr.h

	pour maintenir la coherence.
*/

char **
IdDcmInquireImageInfoFromFile(char *filename) {

	ID_DCM_HDR *e;
	char **imageInfo;

 	e = IdDcmGetHeader(filename);

	if (!e) {
		printf("fichier %s : gros problème :\n", filename);
		IdErrno = IDERR_NON_ACR_FILE;
		return (0);
	}
	imageInfo = _IdDcmInquireImageInfoXXX(e, NULL);
	
	// LIBERER LE HEADER

	return (imageInfo);
}


//
/* ================================================================== */
//

/**
 * \ingroup       dcm
 * \brief	renvoie dans un tableau de chaines de caracteres
 *		la description des info pertinentes d'une image ACR /DCM
 *		a partir d'un ID_DCM_HDR deja fabrique	
 *
 * @param e	ID_DCM_HDR de l'image
 * @return	Tableau :
 *
 * Pour avoir la liste a jour des elements renvoyes dans ce tableau,
 * Consulter src/idacr.h et src/dicom.c
 * 
*/


/* Pour rajouter une entree ici,

	penser a modifier le fichier dicom.c
	penser a modifier la fonction idacr.h

	pour maintenir la coherence.
*/

char **
IdDcmInquireImageInfoFromDcmHdr(ID_DCM_HDR *e)
{
	char **imageInfo;

	if (!e) {
		printf("ID_DCM_HDR pas alloue\n");
		IdErrno = IDERR_NON_ACR_FILE;
		return (0);
	}

	imageInfo = _IdDcmInquireImageInfoXXX(e, NULL);

	return (imageInfo);
}

/* ----------------------------------------------------------------------- */

	 char **
_IdDcmInquireImageInfoXXX(ID_DCM_HDR *e, char **imageInfo)
{

int boolTrouveQqChose=0,trouv,k;

DICOM_INFO *t;
t = _ID_dicom_info;
	
	if (!imageInfo)
		imageInfo =
		    (char **) calloc (1, _ID_Number_of_Items + 1);

	for(k=0;k<_ID_Number_of_Items;k++) {
		trouv=_IdDcmLireEtStockerElement(e, imageInfo,t[k].dicom_group, t[k].dicom_elem,
				   t[k].dicom_info_libelle, t[k].dicom_info_ind);
		if(trouv) boolTrouveQqChose=1;
	}

        if(boolTrouveQqChose)
		return (imageInfo);
	else
		return(0);
}

/* -----------------------------------------------------------------------
*	ID_DCM_HDR *e : 		 doit avoir ete cree   dans l'appelant
*	 char **TableauImageInfo : doit avoir ete alloue dans l'appelant; 
*				 ses elements sont alloues dans la fonction
*	int numGroupe :		 gr et el du Tag DICOM
*	int numElem
*	char *nomElem :		 libelle imprime si element pas trouve
*	int position :		 indice dans TableauImageInfo
  -----------------------------------------------------------------------*/
static int 
_IdDcmLireEtStockerElement(ID_DCM_HDR *e, char **TableauImageInfo, int numGroupe,
			int numElem, char *nomElem, int position) {
	int z;
  	char *chaine;

	/*
	 * Lecture Element 
	 */
	 //if (!( chaine = _IdDcmReadElement(numGroupe, numElem, e) )) {
	if (!( chaine = _IdDcmReadElementNoSQ(numGroupe, numElem, e) )) {

		if (DEBUG)
			printf
			    ("%d : _IdDcmLireEtStockerElement : "
			     "%s (%04x,%04x) pas trouve \n",
			     position, nomElem, numGroupe, numElem);

		return (0);
	} else {
		if (DEBUG)
			printf
			    ("%d : _IdDcmLireEtStockerElement : "
			     "%s (%04x,%04x) \t[%s] \n",
			     position, nomElem, numGroupe, numElem,chaine);

		z = strlen(chaine);
		while (chaine[z] == ' ') {
			chaine[z] = 0;
			z--;
		}

		TableauImageInfo[position] =
		    (char *) malloc(1 + strlen(chaine));
		strcpy(TableauImageInfo[position], chaine);
		if (DEBUG)
			printf
			    ("%d : %s (_IdDcmLireEtStockerElement : "
			     "%04x,%04x)\t[%s]\n",
			     position, nomElem, numGroupe, numElem,
			     chaine);

		return (1);
	}
}

// Converts an integer represented with num_byte bytes within the
// buffer buff to a string.
// Refer to comp.lang.faq FAQ, question 12.21 : How can I tell how much
// destination buffer space I'll need for an arbitrary sprintf call? How can
// I avoid overflowing the destination buffer with sprintf? 

static char * var_itoa(void * buff, int num_byte)
{
	char * retbuf;

	// We double the recommended value since we also have to deal
	// with long integers. Pfff...
#define CHAR_BIT 8
	retbuf = malloc(2 * ( (sizeof(int) * CHAR_BIT + 2) / 3 + 1 + 1));
	switch (num_byte) {
	case 2:
		sprintf(retbuf, "%d",  *((int*)buff));
		break;
	case 4:
		sprintf(retbuf, "%dl", *((int*)buff));
		break;
	default:
		sprintf(retbuf, "var_itoa?????");
	}
	return retbuf;
}


/* =======================================================================
	int _IdStrGetDicomTag(char * label, int *gr, int * num, char *vr)

                     recherche le Dicom tag d'un libelle donne
			par inspection du Dictionnaire DICOM
                          (retourne 0 si pas trouve)                        
   ======================================================================= */
   


static int _IdStrGetDcmTag (char* libelle, unsigned long *gr, unsigned long *num, char **vr) {

DICOM_ELEMENTS *t;
int i;

t=_ID_dicom_elements;

for (i=0; t[i].dicom_group != 0xffff; i++) {
	if( strcmp(t[i].dicom_libelle, libelle) ==0 )

	 {
		*gr= t[i].dicom_group;
		*num=t[i].dicom_elem;
		 *vr =t[i].dicom_type;
		return(1);
	}
}
return(0);

}


/**
 * \ingroup       dcm
 * \brief         Tente de lire un dicom_element a partir de son libelle. Le
 *                resultat est range dans buff.
 * @param libelle Libelle de l'acr_element a lire. Il s'agit du dernier
 *                champ de la variable globale _ID_dicom_elements, tel que
 *                "Group Length", "Priority" ou encore "AE Title".
 * @param e      ID_DCM_HDR dans lequel effectuer la recherche.
 * @param vr      DICOM_VR (cf la variable globale _ID_dicom_vr) retourne',
 *                pour permettre a l'appelant d'interpreter le contenu de buff.
 * @param buff    Ou placer l'acr_element en cas de succes. Si NULL, 
 *                l'allocation est faite localement.
 * @return        En cas de succes, le contenu de l'acr_element lu. Null
 *                en cas d'echec.
 */
 
 
char *
_IdDcmReadElementFromLabel(char *libelle, ID_DCM_HDR *e, char * vr, void *buff) { 
	
	int i;
	unsigned long gr;
	unsigned long num;
	char * caster;
	
	i = _IdStrGetDcmTag (libelle, &gr, &num, &vr);
	
	if (i == 0) {
		if (DEBUG) printf("champ [%s] pas trouve\n",libelle);
		caster = malloc(4);
		sprintf(caster, "???");
		return (caster);
		//return (NULL);
	}
	buff = _IdDcmReadElement(gr, num, e);
	if (buff == 0) {
		caster = malloc(4);
		sprintf(caster, "???");
		return (caster);
	}
	if ( (strcmp(vr,"UL") != 0)
	  && (strcmp(vr,"US") != 0)
	  && (strcmp(vr,"SL") != 0)
          && (strcmp(vr,"SS") != 0)
	  && (gr != 0x0028 || 
		( num != 0x0005 && num != 0x0200) ) ) { 
								// les champs de vr RET sont des caract
						        // sauf (28,5) et (28,200) Pixel Location
		return((void *) strdup(buff));
	}
	if ( (strcmp(vr, "US") == 0)
	  || (strcmp(vr, "SS") == 0)
          || (gr == 0x0028 && 
		(num == 0x0005  || num == 0x0200) )	) {	// les champs de vr RET sont des caract
						           				// sauf (28,5) et (28,200) Pixel Location
		// Buff should contain an int16
		caster = var_itoa(buff, 2);
		return(caster);
	}
        if ( (strcmp(vr,"UL") == 0)
          || (strcmp(vr,"SL") == 0) ) {
		// Buff should contain a uint32_t
		caster = var_itoa(buff, 2);
		return(caster);
	}
	if (DEBUG) printf("impossible de passer ici, mais le compilo rale\n");
	return (NULL);

}


//
/* ============================================================================= */
//



/* ----------------------------------------------------------------------- */
static void 
_IdImprimerElement(char **TableauImageInfo, int numGroupe,
		       int numElem, char *nomElem, int position)
{
	if (!TableauImageInfo[position])
		printf("%d : %s \t(%04x,%04x)  pas trouve \n", position,
			 nomElem, numGroupe, numElem);
	else
		printf("%d : %s \t(%04x,%04x)\t[%s]\n", position,
			 nomElem, numGroupe, numElem,
			 TableauImageInfo[position]);
}

/* ----------------------------------------------------------------------- */

/**
* \ingroup       dcm
* \brief	imprime la description des info 'pertinentes' d'une image DICOM
* 		obtenue par IdDcmInquireImageInfoFromXXX (File/DcmHdr)
*
* @param TableauImageInfo	Tableau des info.
* 
* Pour avoir la liste a jour des DICOM Elements renvoyes par IdDcmInquireImageInfo
* Consultez le fichier src/dicom.c
*
* @return		1 : OK
*			0 : pointeur sur Tableau NULL
*/

int
IdDcmPrintImageInfo(char **TableauImageInfo) {

int k;
DICOM_INFO *t;
t = _ID_dicom_info;

	if (!TableauImageInfo) return(0);

	for(k=0;k<_ID_Number_of_Items;k++) {
		_IdImprimerElement(TableauImageInfo, t[k].dicom_group, 
						t[k].dicom_elem, 
				   		t[k].dicom_info_libelle, 
						t[k].dicom_info_ind);
	}
	return (1);
}



/* ----------------------------------------------------------------------- */
static char * _cleanString(char *v) {
	
	char *d;
	int i, l;
	l = strlen(v);
	for (	i=0,d=v; 
		i<l ; 
		i++,d++) 
		{
		if (!isprint(*d))
			*d = '.';
	}
	
	return v;
}
/* ----------------------------------------------------------------------- */


void IdDcmAffDcmHdr(ID_DCM_HDR *e, int npriv, int noffset) {


DICOM_ELEMENTS *t = NULL;
PLIST pl;
PLIST_ELEMENT plelem; 
_ID_DCM_ELEM *ple;
	
	pl=e->plist;
	plelem = IdLstFirst(pl);

	while (plelem) {
	
		ple= IdLstPtrObj(plelem);	// le _ID_DCM_ELEM  pointé par le PLIST_ELEMENT

		if (ple->Num==0) { 
			if(!npriv) printf("\n"); 
		 } else { 
			for (t=_ID_dicom_elements; t->dicom_group!=0xffff; t++) {
				if( (t->dicom_group==ple->Gr) && (t->dicom_elem==ple->Num) ) break;
			}
             	}
		if(!(ple->Gr%2) || !npriv) {

			if (noffset) 
				printf(" gr %04x  num %04x  long %d",
			  		 ple->Gr, ple->Num, ple->LgrLueElem);
			else
				printf(" gr %04x  num %04x  long %d  _offset %d x(%x) o(%o12)",
			   		ple->Gr, ple->Num, ple->LgrLueElem, ple->Offset,ple->Offset,ple->Offset );
		}

		if (ple->Num!=0) { 
		
			if(ple->VR == NULL) {
				if(!(ple->Gr%2) || !npriv) printf("\t%s\t%s\t\t",  
							t->dicom_type, t->dicom_libelle);
				} else {
				if(!(ple->Gr%2) || !npriv) printf("\t%s\t%s\t\t",  
							ple->VR, t->dicom_libelle);				
				}
				
				if ( (ple->Gr==0x0002) && (ple->Num==0x0010) ) {
					char * s = _IdStrShowTransfertSyntax(ple->valeurElem);
					printf(" : [%s] ",s);
				}
		} 
		
		if (ple->Num==0) { 	
			if(!(ple->Gr%2) || !npriv){
					printf("   lgr du grp = %d\n   -------\n",
						str2num(ple->valeurElem,int) );
			}
		} else {	
			switch (ple->LgrElem) { 
			
			case 2:						// LGR = 2

				if (     (strcmp(t->dicom_type, "US") == 0)
	  		  		  || (strcmp(t->dicom_type, "SS") == 0) 
          		  		  || (t->dicom_group == 0x0028 
          		  		  && 
          		  		  	(	t->dicom_elem == 0x0005  
				  	  	  || t->dicom_elem == 0x0200) ) ) {
   
						if(!(ple->Gr%2) || !npriv) { 
							printf("\t  \t \t %d \tx(%04x)\n",
								str2num(ple->valeurElem,unsigned short int),
								str2num(ple->valeurElem,unsigned short int) );	
						}
				} else {

					if(!(ple->Gr%2) || !npriv){ 
						printf("\t[%s]  \t %d \tx(%04x)\n", 
							_cleanString(ple->valeurElem),
							str2num(ple->valeurElem,unsigned short int), 
							str2num(ple->valeurElem,unsigned short int));
					}
				}
				break;		
		
			case 4 :			// LGR = 4
 
        			if (   (strcmp(t->dicom_type,"UL") == 0)
          		  	    || (strcmp(t->dicom_type,"SL") == 0) ) {
						if(!(ple->Gr%2) || !npriv) { 
							printf("\t  \t \t %d \tx(%08x)\n", 
								str2num(ple->valeurElem,int), 
								str2num(ple->valeurElem,int));
						}	
				} else {
						if  (strcmp(t->dicom_type,"FL") == 0) {
							if(!(ple->Gr%2) || !npriv) { 
								printf("\t  \t \t %f\t\n", 
									str2num(ple->valeurElem,float));							
							}
						} else {
								if(!(ple->Gr%2) || !npriv) { 
									printf(" \t[%s]\n",_cleanString(ple->valeurElem));
								}
						}
				}
				break;
				
				
			case 8 :		// lgr = 8 (double)
			
        			if (strcmp(t->dicom_type,"FD") == 0) {
						if(!(ple->Gr%2) || !npriv) { 
							printf("\t  \t \t %g \t\n", 
								str2num(ple->valeurElem,double));
						}	
				} else {	
							if(!(ple->Gr%2) || !npriv) { 
								printf("\t[%s]\n",_cleanString(ple->valeurElem));
							}
				}
				break;				
						
			
			default :								// AUTRES LGR

				if (ple->LgrElem > 5000) { 
					if(!(ple->Gr%2) || !npriv) 
						printf(" --> Too Long (%08x). Not Printed...\n",ple->LgrElem);
				} else {
					if(!(ple->Gr%2) || !npriv) 
						printf(" \t[%s]\n",_cleanString(ple->valeurElem));
				}

	     		}
		}
	plelem = IdLstNext(plelem);
	}
}

// -----------------------------------------------------------------------------------------------

/**
 * \ingroup        dcm
 * \brief	   Verifie le caractère Acr/Dcm/LibIdo d'un fichier.
 *
 * @param filename	Nom du Fichier
 * @return     		ID_DCM_HDR (ca sera fait pour plus tard) si lisible
 *			O sinon.
 *
 */

ID_DCM_HDR * IdDcmIsDcmReadable(char * filename) {

// on suppose que la creation du DcmHeader est suffisante 
// pour que le fichier soit DcmReadble
// (abusif, car un DICOMDIR permet de creer un DcmHeader
// et n'est pas DcmReadable -pas de Pixels -...)

   ID_DCM_HDR * a =IdDcmGetHeader(filename);
   return a;
}


// ----------------------------------------------------------------------------------------------
 
 
 /**
 * \ingroup        dcm
 * \brief	   Verifie le caractère Jpeg Lossless d'un fichier, a partir du DCM_HDR.
 *
 * @param e		ID_DCM_HDR  deja cree	
 * @return     		1 si Jpeg Losless
 *			O sinon.
 *
 */
 
int IdDcmIsJpegLossless (ID_DCM_HDR * e) {
char * Derivation_Description;
char * Transfert_Syntax_UID;
char * valeur = "Compress BN JPEG Lossless";

	if (!e) 
		return (0);
		
	Transfert_Syntax_UID = _IdDcmReadElement(0x0002, 0x0010, e);
	if (Transfert_Syntax_UID != NULL) {
		if ( (memcmp(Transfert_Syntax_UID + strlen(Transfert_Syntax_UID)-2, "70", 2) == 0)
			||
		      (memcmp(Transfert_Syntax_UID + strlen(Transfert_Syntax_UID)-2, "55", 2) == 0) ) {
		 	 return (1);
		 } else {	
				Derivation_Description = _IdDcmReadElement(0x0008, 0x2111, e);
				if (Derivation_Description != NULL) {
					if (memcmp (Derivation_Description, valeur, strlen(valeur)) == 0)
						return (1);
				}	
		}
	}
	return (0);
}


// ----------------------------------------------------------------------------------------------
 
 
 /**
 * \ingroup        dcm
 * \brief	   Verifie le caractère non compresse 
 *                 d'un fichier, a partir du DCM_HDR.
 *
 * @param e		ID_DCM_HDR  deja cree	
 * @return     		1 si non compresse
 *			O sinon.
 *
 */
 
int IdDcmIsUncompressed (ID_DCM_HDR * e) {
char * Transfert_Syntax_UID;
int l;

	if (!e) // never allocated
		return (0);
		
	if (!e->__TrueDicom) // old ACR-NEMA
		return (1);
		
	// IsImplicitVRLittleEndianTransferSyntax         1.2.840.10008.1.2
	// IsExplicitVRLittleEndianTransferSyntax         1.2.840.10008.1.2.1
	// IsDeflatedExplicitVRLittleEndianTransferSyntax 1.2.840.10008.1.2.1.99
	// IsExplicitVRBigEndianTransferSyntax            1.2.840.10008.1.2.2
		
	Transfert_Syntax_UID = _IdDcmReadElement(0x0002, 0x0010, e);
	
	if (Transfert_Syntax_UID != NULL) {
		l = strlen(Transfert_Syntax_UID);
		if ( (memcmp(Transfert_Syntax_UID,"1.2.840.10008.1.2",      l) == 0)
			||
		     (memcmp(Transfert_Syntax_UID,"1.2.840.10008.1.2.1",    l) == 0)
			||
		     (memcmp(Transfert_Syntax_UID,"1.2.840.10008.1.2.1.99", l) == 0)
			||
		     (memcmp(Transfert_Syntax_UID,"1.2.840.10008.1.2.2",    l) == 0)
		     		     
		   ) 	 
	          return (1);  // uncompressed
	       else
	          return (0);  // others
	  } else {
	     return (1); // No Transfer Syntax found. Hope it's uncompressed
	  }   
}
