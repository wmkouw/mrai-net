/*************************************************************************
* $Id: acrutil.c,v 1.3 2005/09/26 10:20:08 bellet Exp $
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
* NOM DU MODULE : utilacr.c        * TYPE        : fonction
* AUTEUR        : c.odet           * CREATION    :  07/05/1992
* VERSION       : 1.0              * REVISION    :  16/06/1992
* LANGAGE       : C                * 
*                                  *
**************************************************************************
*
*  Description : fonction utilitaires de gestion 
*               du format ACRNEMA modifie UA1216
*
*       ACR_ELEMENT *_IdAcrMakeElement(int gr, int nu,int size, long type, long init);
*       ACR_ELEMENT *_IdAcrAddElement(ACR_ELEMENT *liste,ACR_ELEMENT *elem);
*       ACR_ELEMENT *_IdAcrFreeListe(ACR_ELEMENT *liste);
*       ACR_ELEMENT *_IdAcrFindElement(ACR_ELEMENT *liste, int gr, int nu);
*       _IdAcrAdjustLength(ACR_ELEMENT *liste);
*       _IdAcrAffListe(ACR_ELEMENT *liste);
*       _AddHisto(char *l)
 
**************************************************************************
*
*  MODULES UTILISES :
*
**************************************************************************
*                     |              |
* REVISIONS :  DATE   | AUTEUR       |  DESCRIPTION
*---------------------|--------------|------------------------------------
*       16/06/92      | B. BARBIER   | Insertion LibIDO
*             /  /    |              |
*	01/10/92      | B. Barbier   |  V1.2-5 : normalisation des noms
	15/12/94    	JPR		modif fonctions de swap
	29/01/95	JPR		acceleration rech. ds l'entete
	12/02/96	JPR		info de Swap
	7/2/97		JPR		PROBLEME pour les elements 'chaine de caract' :
					Ils doivent etre alloues 
					AVANT l'appel de IdAcrReadElement
	10/02/97	JPR		Verif Entete
	7/3/97		JPR		_IdAcrOverWriteElement, _IdAcrGetElementLength
	6/5/98		JPR		_IdAcrMergeListes
	12/5/98		JPR		Modifs pour CardioVasc G.E.
	19.10.99	JPR		Ajout libelles codes ACR / DICOM
	12/12/99	JPR		Essai pour ACR curieux sur DEC
	24-01-2000	FB		Initialisations
	27-01-2000	JPR		impression offset
	28-01-2000	JPR		_IdAcrReadElementR
	21-02-2000	JPR 		Modifs pour lire format GE pique sur MORGON
	12-07-2000	JPR		Modif pour lire images VARIANT
	05-09-2000	JPR		On ne swappe plus que les UL et les US
	07-09-2000	JPR		 ... et les RET
	18-9-2000	JPR		Dorenavant, on ne recherche le type du DICOM Element 
						que pour les groupes pairs
	20-11-2000	JPR		Prise en compte des types OB OW SQ UN (Explicit VR)



***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>		// For var_itoa
#include <errno.h>

#include "idprint.h"
#include <string.h>  		// For strlen

#include "iddicom.h"
#include "idgen.h"
#include "idima.h"
#include "idacr.h"
#include "idacr-restricted.h"

#ifdef WIN32
#include <winsock.h>		// Pour ntohs - BigEndianeries -
#else
#include <netinet/in.h>  	// Pour ntohs - BigEndianeries -
#endif



long _IdAcrReadElement      (int gr, int nu, FILE *fp, void *buff);
long _IdAcrOverWriteElement (int gr, int nu, FILE *fp, void *buff); 
long _IdAcrGetElementLength (int gr, int nu, FILE *fp);
uint32_t _IdAcrRecupLgr(FILE*, int *);
void * _IdAcrReadElement_avec_creation(int gr, int num, FILE * fp, void *buff, int *lgrTrouvee); 
int    _IdStrGetDicomTag (char* libelle, unsigned long *gr, unsigned long *num, char *vr);

short int SWAP_SHORT(short int a);
uint32_t SWAP_LONG(uint32_t a);
long _IdAcrOverWriteElementNumber(int gr, int num, FILE *fp, int nb); 
ACR_ELEMENT *_IdAcrModifyElement(ACR_ELEMENT *liste, ACR_ELEMENT *elem);



static int DEBUG=0;

static int __cas_de_fou_Philips=0;

static int sw;  /* 0 pour pas de swapping 
		   3412      nouvelle org. des octets
                   4321      pour les swapping
                   2143      de long int               */

int __TrueDicom;	/* 1 : contient DICM en 128 */

int __ExplicitVR;	/* 1 si le type est ecrit a l'interieur des 4 octets contenant la longueur  */

int __ID_offset;
int __NumeroGroupePrecedent;
static char VR[3];

static int CardioVasc; /* 0 pour ACR/DICOM 'normal'	
 			  1 pour DICOM CardioVasc	*/

int __Papyrus; /*   0 pour ACR/DICOM-implicit-VR	
 		    1 pour PAPYRUS			*/

int __ID_CardioVascCurieux; /* 0 pour Normal  (commence par (0008, 0000) */
			     /* 1 pour Curieux (commence par (0008, 0005) */
			     /* 1 pour Curieux (commence par (0008, 0002) */

/* =======================================================================
 *	SWAP_SHORT
 *		remet les octets dans un ordre compatible avec celui du processeur
 *		(requiert d'avoir lance CheckSwap AVANT - pas ruse ...-)
 *  ======================================================================= */

short int SWAP_SHORT(short int a) {
	if ( (sw==4321)  || (sw==2143) )  
		a =(((a<<8) & 0x0ff00) | ((a>>8)&0x00ff));
	return (a);
}

/* =======================================================================
 *	SWAP_LONG
 *		remet les octets dans un ordre compatible avec celui du processeur
 *		(requiert d'avoir lance CheckSwap AVANT - pas ruse ...-)
 *  ======================================================================= */

uint32_t SWAP_LONG(uint32_t a) {
/*       ATTENTION: il aura surement un pb pour les entiers negatifs ...
 *                                                       JPR                   
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
		IdPrintf("\n\n\n *******\n erreur code swap ?!?\n\n\n");
		a=0;
   }
  return(a);
}

/* 
*  =======================================================================
*
*	_IdAcrTestSwap
*
*  ======================================================================= 
*/

int
_IdAcrTestSwap(uint32_t s) {

/*                     la valeur de sw montre 
 *                     la nouvelle organisation des octets 
 *                     dans  un long int
 *                     0 = pas de swap                             	 
*/
    switch (s) {
	case 0x00040000 :
		sw=3412;
		if(DEBUG) IdPrintf("s : %08x sw : %d\n",s,sw);
		break;
	case 0x04000000 :
		sw=4321;
		if(DEBUG) IdPrintf("s : %08x sw : %d\n",s,sw);
		break;
	case 0x00000400 :
		sw=2143;
		if(DEBUG) IdPrintf("s : %08x sw : %d\n",s,sw);
		break;
	case 0x00000004 :
		sw=0;
		if(DEBUG) IdPrintf("s : %08x sw : %d\n",s,sw);
		break;
	default :
		sw = -1;
      		 if (DEBUG) IdPrintf (  "_IdAcrTestSwap (1): Pas trouve "
					"l info de Swap : s= %x -> sw=-1 \n",s);
    }
   return (sw);
}

/* =======================================================================

	_IdAcrCheckSwap
		La seule maniere sure que l'on aie pour determiner 
		si on est en 	LITTLE_ENDIAN, 	   BIG-ENDIAN, 
				BAD-LITTLE-ENDIAN, BAD-BIG-ENDIAN
		est de trouver l'element qui donne la longueur d'un 'GROUP'
		(on sait que la longueur de cet element vaut 0x00000004)
		et de regarder comment cette longueur est codee en memoire  

		Le probleme vient de ce que parfois, il n'y en a pas ...

		On fait alors le pari qu'on a a faire a du LITTLE_ENDIAN propre.
		Si ce n'est pas le cas, om ne peut rien faire.

		(il faudrait avoir des fonctions auxquelles 
		 on passe le codeSwap en parametre, pour faire des essais 'manuels')

   ======================================================================= */
int _IdAcrCheckSwap(FILE * fp) {
uint32_t  bili2,x=4;
char DICM[16];

__Papyrus		= 0;
CardioVasc		= 0;
__ID_CardioVascCurieux	= 0;

/* On commence par verifier si c'est du DICOM 'actuel' */
/*                                      -------------  */

fseek(fp,128L,SEEK_SET); 
fread(&DICM,4,1,fp); 
DICM[4] = 0;

if(memcmp(DICM, "DICM", (size_t)4) == 0)
	{
	__TrueDicom=1;
	__ID_offset=132L; /* a voir ... */
	if (DEBUG) IdPrintf ("_IdAcrCheckSwap : C est du DICOM actuel ->%s<-\n",DICM);
	}
	else
	{
	__TrueDicom=0;
	__ID_offset=0L;  /* a voir ... */
	if (DEBUG) IdPrintf ("_IdAcrCheckSwap : Ce n'est PAS du DICOM actuel ->%s<-\n",DICM); 
	}

if(__TrueDicom)
{

 rewind(fp);
 fseek(fp,136L,SEEK_SET);/* on saute le File Preamble (souvent a ZERO) : 128 Octets */
			 /* le DICM, et le 0002, 0000 				    */

   fread(&DICM,4,1,fp);	/* les 2 premiers octets de la lgr peuvent valoir UL 
   			(ou OB si le premier est (0008, 0001)		-> Explicit VR */
   if(memcmp(DICM, "UL", (size_t)2) == 0) {
	__ExplicitVR = 1;
	if (DEBUG)	IdPrintf ("_IdAcrCheckSwap : Explicit VR\n");
   } else {
	__ExplicitVR = 0;
   }

   if (x!=(x)) {	/* si le HostByteOrder est different du NetworkByteOrder  */ 
		sw = 0;	/* on est sur PC ou DEC --> LITTLE-ENDIAN -> Rien a faire */
		if (DEBUG) IdPrintf("HostByteOrder = NetworkByteOrder\n");
	
   } else {		/* on est sur une Sun ou une SGI	*/
		sw = 4321;
		if (DEBUG) IdPrintf("HostByteOrder != NetworkByteOrder\n");
   }

   rewind(fp); 							
   fseek(fp,132L,SEEK_SET);
   return sw;

} /* fin TrueDicom */



/* -----  SINON ----- */


	
/* Si c'est de l'ACR 'propre', la lgr du premier element du groupe est FORCEMENT 4 */
							
 fseek(fp,4L,SEEK_SET);    /* SEEK_SET = 0                   */		
 fread(&bili2,4,1,fp);     /* lgr groupe : 0000 0004   JPR   */ 
	if (DEBUG) IdPrintf( "_IdAcrCheckSwap (2) : bili2 %08x\n", bili2);	
		
 sw=_IdAcrTestSwap(bili2);		

 __ID_offset=0;

/* Si c'est de l'ACR 'pas propre', il manque la lgr du groupe */
									
if(sw==-1) 
{								
									
/* pour tenter de lire DICOM CardioVasc G.E.	*/
									
    rewind(fp);           						
    fseek(fp,136L,SEEK_SET);  /* SEEK_SET = 0                     */	
    fread(&bili2,4,1,fp);     /* lgr groupe : 0000 0004   JPR     */ 
   if (DEBUG) IdPrintf( "_IdAcrCheckSwap (3)  : bili2 %08x\n", bili2);	
    sw=_IdAcrTestSwap(bili2);

    if (sw != -1) 
	{

	CardioVasc = 1;							
        rewind(fp); 							
        fseek(fp,132L,SEEK_SET);
	__ID_offset=132;
	IdPrintf("\n\n\n\n NORMALEMENT, ON NE PASSE PLUS ICI (si c'est le cas, prevenez-moi JPRx)\n");
	IdPrintf("\n\nsw = %d\n\n\n",sw);
	if (DEBUG) IdPrintf( " <<CardioVasc (ou autre 'curiosite' ...)>>\n");
        }
	else
	{		
		/* On n'a pas trouve l'info de swap 	28/11/2000 JPR						*/
		/* Si c'est du VRAI ACR NEMA  si on est sur une DEC ou un PC swap=0, SUN ou SGI SWAP=4321 	*/
		/* si c'est du RAW, ca degagera + tard								*/
	  
		if (DEBUG) IdPrintf("On force la chance \n");

		if (x!=(x))  /* si le HostByteOrder est different du NetworkByteOrder  */ 
			sw = 0;   /* on est sur PC ou DEC --> LITTLE-ENDIAN -> Rien a faire */
		else
			sw = 4321; /* on est sur Sun ou SGI */
	}
}

rewind(fp);
return (sw);
}



/* =======================================================================
	int _IdStrGetDicomTag(char * label, int *gr, int * num, char *vr)

                     recherche le Dicom tag d'un libelle donne
			par inspection du Dictionnaire DICOM
                          (retourne 0 si pas trouve)                        
   ======================================================================= */

int _IdStrGetDicomTag (char* libelle, unsigned long *gr, unsigned long *num, char *vr) 
{

DICOM_ELEMENTS *t;
int i;

t=_ID_dicom_elements;

for (i=0; t[i].dicom_group != 0xffff; i++) {
	if( strcmp(t[i].dicom_libelle, libelle) ==0 )

	 {
		*gr= t[i].dicom_group;
		*num=t[i].dicom_elem;
		 vr =t[i].dicom_type;
		return(1);
	}
}
return(0);

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

/**
 * \ingroup       acr
 * \brief         Tente de lire un acr_element a partir de son libelle. Le
 *                resultat est range dans buff.
 * @param libelle Libelle de l'acr_element a lire. Il s'agit du dernier
 *                champ de la variable globale _ID_dicom_elements, tel que
 *                "Group Length", "Priority" ou encore "AE Title".
 * @param fp      File descriptor dans lequel effectuer la recherche.
 * @param vr      DICOM_VR (cf la variable globale _ID_dicom_vr) retourne',
 *                pour permettre a l'appelant d'interpreter le contenu de buff.
 * @param buff    Ou placer l'acr_element en cas de succes. Si NULL, 
 *                l'allocation est faite localement.
 * @return        En cas de succes, le contenu de l'acr_element lu. Null
 *                en cas d'echec.
 */

void *
_IdAcrReadElementFromLabel(char *libelle, FILE * fp, char * vr, void *buff) 
{ 
	int lgrTrouvee;
	int i;
	unsigned long gr;
	unsigned long num;
	char * caster;
	
	i = _IdStrGetDicomTag (libelle, &gr, &num, vr);
	
	if (i == 0) {
		if (DEBUG) IdPrintf("champ [%s] (%08x , %08x) pas trouve\n");
		caster = malloc(4);
		sprintf(caster, "???");
		return (void*)caster;
		//return (NULL);
	}
	buff = _IdAcrReadElement_avec_creation(gr, num, fp, buff, &lgrTrouvee);
	if (lgrTrouvee == 0) {
		caster = malloc(4);
		sprintf(caster, "???");
		return (void*)caster;
	}
	if ( (strcmp(vr,"UL") != 0)
	  && (strcmp(vr,"US") != 0)
	  && (strcmp(vr,"SL") != 0)
          && (strcmp(vr,"SS") != 0)
	  && (gr != 0x0028 || 
		( num != 0x0005 && num != 0x0200) ) ) { // les champs de vr RET sont des caract
						        // sauf (28,5) et (28,200) Pixel Location
		// Buff should contain a string. End the string properly
		caster = (char*)buff;
		// FIXME : in fact we should have written
		// caster[lgrTrouvee + 1] = '\0' but better 'eating' one
		// character than depend on alloaction of buff.
		caster[lgrTrouvee] = '\0';
		return((void *) buff);
	}
	if ( (strcmp(vr, "US") == 0)
	  || (strcmp(vr, "SS") == 0) ) {
		// Buff should contain an int16
		caster = var_itoa(buff, 2);
		return((void *) caster);
	}
        if ( (strcmp(vr,"UL") == 0)
          || (strcmp(vr,"SL") == 0)
          || (gr == 0x0028 && 
		(num == 0x0005  || num == 0x0200) )	) { // les champs de vr RET sont des caract
						            // sauf (28,5) et (28,200) Pixel Location
  
		// Buff should contain a uint32_t
		caster = var_itoa(buff, 2);
		return((void *) caster);
	}
	if (DEBUG) IdPrintf("impossible de passer ici, mais le compilo rale\n");
	return (NULL);

}




/* =======================================================================
	_IdAcrReadElement

                     lit un acr_element et le range dans buff
	                  retourne la taille de l'acr_element 
                          (non lu si buff =0) 
(le fichier doit deja avoir ete ouvert, 
et _IdAcrCheckSwap(FILE * fp) avoir ete appelle)

		ATTENTION : C'est a l'utilisateur de savoir si l'element lu 
				est un entier ou une chaine de caract	                       
   ======================================================================= */


/**
 * \ingroup       acr
 * \brief         lit un acr_element et le range dans buff
 *                retourne la taille de l'acr_element
 *		  (non lu si buff =0)
 *			(le fichier doit deja avoir ete ouvert,
 *			et _IdAcrCheckSwap(FILE * fp) avoir ete appelle)
 *			ATTENTION : C'est a l'utilisateur de savoir si l'element lu
 *			est un entier ou une chaine de caract
 * @param gr  partie 'group' du Tag de l'acr_element a lire.
 * @param num partie 'elem' du Tag de l'acr_element a lire.
 * @param fp      File descriptor dans lequel effectuer la recherche.
 * @param buff    Ou placer l'acr_element en cas de succes. 
 *		  Si NULL, l'allocation est faite par la fonction
 * @return        Longueur de l'ACR_ELEMENT lu 
 */


long _IdAcrReadElement(int gr, int num, FILE * fp, void *buff)
{
int lgrTrouvee;

    _IdAcrReadElement_avec_creation (gr, num, fp, buff, &lgrTrouvee);

return(lgrTrouvee);
}


/* =======================================================================
	_IdAcrReadElement_avec_creation

			lit un acr_element
				le range dans buff s'il est alloue, 
				l'alloue sinon
			retourne buff dans tous les cas

		ATTENTION : C'est a l'utilisateur de savoir si l'element lu 
				est un entier ou une chaine de caract

	Cette fonction est rendue necessaire, car _IdAcrReadElement, 
	utilisee helas partout, n'alloue pas.

(le fichier doit deja avoir ete ouvert, 
et _IdAcrCheckSwap(FILE * fp) avoir ete appelle)

		
   ======================================================================= */



/**
 * \ingroup       acr
 * \brief         lit un acr_element et le range dans buff s'il est alloue
 *                l'alloue sinon et retourne buff dans tous les cas
 *		  (non lu si buff =0)
 *		  (le fichier doit deja avoir ete ouvert,
 *		  et _IdAcrCheckSwap(FILE * fp) avoir ete appelle)
 *		  ATTENTION : C'est a l'utilisateur de savoir si l'element lu
 *		  est un entier ou une chaine de caract
 * @param gr  partie 'group' du Tag de l'acr_element a lire.
 * @param num partie 'elem' du Tag de l'acr_element a lire.
 * @param fp      File descriptor dans lequel effectuer la recherche.
 * @param buff    Ou placer l'acr_element en cas de succes. 
 *		  Si NULL, l'allocation est faite par la fonction
 * @param buff    Ou placer l'acr_element en cas de succes. 
 * @lgrTrouvee    Longueur de l'ACR_ELEMENT lu 
 *
 * @return        Adresse memoire de l'element lu 
 */

void * _IdAcrReadElement_avec_creation(int gr, int num, FILE * fp, void *buff, int *lgrTrouvee) 
{

DICOM_ELEMENTS *t = NULL;
uint32_t l, l_gr;
int flag;
int skL;
int cherche_vr=0;
unsigned short g;
unsigned short n;

if (DEBUG)  IdPrintf("\t\t\t\t\trecherche (1) gr %x  num %x (swapping %d)\n",gr,num,sw); 

__cas_de_fou_Philips=0;

l=0;
flag=0;
while(fread(&g,2,1,fp))
	{
	if(sw)g=SWAP_SHORT(((short)g));
	fread(&n,2,1,fp);
	if(sw)n=SWAP_SHORT(((short)n));
	// Lecture de la longueur de l'element
	l = _IdAcrRecupLgr(fp, &skL);


	if (DEBUG)  IdPrintf("  -------------> gr %x  num %x  long %x : %d\n",g,n,l,l);    

        if(gr>g)           /* on est plus haut que l'element cherche     */
           	flag=1;    /* si on ne le trouve pas des le premier coup */
                           /* c'est qu'il n'y est pas			 */ 
           if (0) // pour voir si pb de lgr groupe
	 /*if((gr>g)&&(n==0)) */ /* on est sur un debut de groupe		 */
		{
		fread(&l_gr,4,1,fp);		//on lit la longueur du groupe
		if(sw) l_gr = SWAP_LONG(((uint32_t)l_gr));

		fseek(fp,l_gr,SEEK_CUR);  		/* on saute le groupe  		*/

		if (DEBUG)
			{
			IdPrintf("                   on saute au gr sv :%d car\n",l_gr);
			}
               }

	else if((g==gr)&&(n==num))    /* on est sur l'element recherche   */
                                      /* ------------------------------   */
		{
		if (buff == NULL)		// On alloue, en essayant de ne pas se vautrer ...

			{			// desole pour la cascade de if,
						// mais il n'y a pas d'operateur AND THEN en C ... 

			if ( (g/2)*2-g==0 )	// on ne recherche que pour les groupes pairs
			 {

				if ((l==2) ) {
					cherche_vr=1;	// on signale qu'on a deja cherche ds dict. Dicom
	    				for (t=_ID_dicom_elements; t->dicom_group!=0xffff; t++) {
						if( (t->dicom_group==g) && (t->dicom_elem==n) ) break;
					}

					if ( (strcmp(t->dicom_type,"UL") ==0) 
			 	  	  || (strcmp(t->dicom_type,"US") ==0) )
						buff = malloc(2);	    /* on alloue un int 16  */
				}
				else  	
				if ((l==4) ) {
					cherche_vr=1;
	    				for (t=_ID_dicom_elements; t->dicom_group!=0xffff; t++) {
						if( (t->dicom_group==g) && (t->dicom_elem==n) ) break;
					}

					if ( (strcmp(t->dicom_type,"SL") ==0) 
			 	 	  || (strcmp(t->dicom_type,"SS") ==0) )
					buff = malloc(4);			/* on alloue un int 32  */
					}
				}
					// On peut esperer que les subtilites c-dessus ne servent jamais...

				buff = malloc(l+1); // c'est une chaine de caracteres 
			}

		fread(buff,(int)l,1,fp);


		if(sw) {
			if ( (g/2)*2-g==0 ) { /* on ne teste pas les groupes impairs */

				if ((l==4)||(l==2)) {  /*pour eviter de swapper les chaines 
							de lgr 2 ou 4 */

					if(cherche_vr ==0) {  //si on n'a jamais inspecte le dict. Dicom

						if (DEBUG) IdPrintf("Consultation Dictionaire DICOM"
								   " g %08x n %08x l %d\n",g,n,l);
	    					for (t=_ID_dicom_elements; t->dicom_group!=0xffff; t++) {
							if( (t->dicom_group==g) && (t->dicom_elem==n) ) break;
						}
					}

					if ( (strcmp(t->dicom_type,"UL") ==0) 
			  	  	  || (strcmp(t->dicom_type,"US") ==0) 
			  	  	  || (strcmp(t->dicom_type,"SL") ==0)
			  	  	  || (strcmp(t->dicom_type,"SS") ==0)  
	  				  || (gr == 0x0028 && 
						( num == 0x0005 || num == 0x0200) ) )
					 { 			// les champs de vr RET sont des caract
						        	// sauf (28,5) et (28,200) Pixel Location

						if(l==4) {
						     *(uint32_t *) buff=SWAP_LONG  ((*(uint32_t *) buff)); 
						}

						else if(l==2)  {
						     *(short *)   buff=SWAP_SHORT ((*(short *)   buff));
						}

						if(DEBUG) if ((l==4)||(l==2)) { IdPrintf("on swappe\n");}
					}
				} /* fin if l==2 ==4 */
			} /* fin if g pair */
		} /* fin sw */

	//return l;
	*lgrTrouvee=l;
	return ((void *)buff);
	}

	else if((g>gr)||((gr==g)&&(n>num)))     /* on a depasse l'element */
						/* ---------------------- */
		{
		if ( g > 0xff00 ) { // on est dans le cas de fou Philips 
				    // ou des Tags ne sont pas dans l'ordre
				if (DEBUG) IdPrintf("\t\t\t\t\t\t =============Tags pas dans l'ordre (cas de fou) "
							" gr %04x %d g %04x %d\n",gr,gr,g,g);
				__cas_de_fou_Philips=1;
				fseek(fp,l,SEEK_CUR);
		} else {
	
			if(flag)            /*on a deja re-essaye depuis le debut */
				{           /* l'element cherche n'y est pas      */
				fseek(fp,l,SEEK_CUR);/*on se positionne sur l'element sv */
				*lgrTrouvee=0;
				return((void *)NULL);    /*  On sort                JPR      */
				} 
           			  
			rewind(fp);
			if ((CardioVasc == 1) || (__TrueDicom == 1))
					fseek(fp,132L,SEEK_SET); /* pour CardioVasc G.E. ??? a revoir */

			flag=1; 	    	 	/* On va re-essayer depuis le debut  */
			}
		}

	else {                       		/* on saute a l'element suivant      */
				     		/* ----------------------------      */
		fseek(fp,l,SEEK_CUR);
             }
	}
//
*lgrTrouvee=0;
return ((void *)NULL);
}



/* =======================================================================
	_IdAcrReadNextElement

                     lit l'acr_element courant 

  mode= 1 : on charge EGALEMENT les Elements 'longs' (>5000) ,
        0 : on NE CHARGE PAS    les Elements 'longs' (>5000) ,	 

(le fichier doit deja avoir ete ouvert, 
et _IdAcrCheckSwap(FILE * fp) avoir ete appelle)

                  
   ======================================================================= */


/**
 * \ingroup       acr
 * \brief         lit l'acr_element courant.
 *			(le fichier doit deja avoir ete ouvert,
 *			et _IdAcrCheckSwap(FILE * fp) avoir ete appelle)
 * @param libelle Libelle de l'acr_element a lire. Il s'agit du dernier
 *                champ de la variable globale _ID_dicom_elements, tel que
 *                "Group Length", "Priority" ou encore "AE Title".
 * @param fp      File descriptor dans lequel effectuer la recherche.
 * @param mode    mode= 1 : on charge EGALEMENT les Elements 'longs' (>5000),
 *                      0 : on NE CHARGE PAS    les Elements 'longs' (>5000) .
 * @return        En cas de succes, pointeur sur l'ACR_ELEMENT lu. 
 *               Null en cas d'echec.
 */


ACR_ELEMENT * _IdAcrReadNextElement(FILE *fp, int mode) 
{
unsigned short g;
unsigned short n;
uint32_t l;
uint32_t type=0;
int skL;
ACR_ELEMENT *p;
DICOM_ELEMENTS *t;
char * buff = NULL;

	if (DEBUG) IdPrintf(" ===> entree ds _IdAcrReadNextElement\n");
	fread(&g,2,1,fp);
	if (feof(fp))   {if (DEBUG) IdPrintf("_IdAcrReadNextElement : eof trouve\n");return (NULL);}
	if (ferror(fp)) {           IdPrintf("echec lecture NumGr\n");               return (NULL);}

	if (sw)          g=SWAP_SHORT(((short)g));
/*
	if (g < __NumeroGroupePrecedent) {
		IdPrintf("Gros malaise : num de groupe pas dans l'ordre : (%04x) apres (%04x)\n",g, __NumeroGroupePrecedent );
	}
*/
	__NumeroGroupePrecedent =g;

	fread(&n,2,1,fp);
	if (ferror(fp)) {IdPrintf("echec lecture NumElem\n");return (NULL);}
	if (feof(fp))   {IdPrintf("echec lecture NumElem\n");return (NULL);}

	if(sw)n=SWAP_SHORT(((short)n));

	// Lecture de la longueur de l'element

	l = _IdAcrRecupLgr(fp,&skL);

	if (DEBUG) if (n!=0) IdPrintf("_IdAcrReadNextElement : "
					" gr %x\tnum %x\tlong %x (%d)",
					g,n,l,l); 

	if (l < 5000 || mode ==1)
	{
	buff=malloc(l+1);
	buff[l]=0;
		
	if(buff) fread(buff,(int)l,1,fp);
	if (ferror(fp)) {IdErrPrintf("echec lecture Element\n");return (NULL);}
	if (feof(fp))   {IdErrPrintf("echec lecture Element\n");return (NULL);}

	if ((n==0) && sw)  *(uint32_t *) buff=SWAP_LONG  ((*(uint32_t *) buff)); // n=0 : lgr du groupe 
	else
		if(sw) {
			 if ( (g/2)*2-g==0) { /* on ne teste pas les groupes impairs */
			// if (1) { // on teste MEME les groupes impairs ...

				if ((l==4)||(l==2)) {   //pour eviter d'explorer de Dict. DICOM
										// dans le cas de  chaines 
										// de lgr 2 ou 4 
					if (DEBUG) IdPrintf("Consultation Dictionary DICOM g %04x n %0xx l %d\n",g,n,l);

	    				for (t=_ID_dicom_elements; t->dicom_group!=0xffff; t++) {
						if( (t->dicom_group==g) && (t->dicom_elem==n) ) break;
					}
					if ( (strcmp(t->dicom_type,"UL") ==0) 
			  	  	  || (strcmp(t->dicom_type,"US") ==0)
			  	  	  || (strcmp(t->dicom_type,"SL") ==0) 
			  	  	  || (strcmp(t->dicom_type,"SS") ==0) 
          				  || (g == 0x0028 && 
						( n == 0x0005 || n == 0x0200) )	) { // seuls (28,5) de vr RET
										    //  et (28,200) sont des entiers

						if(l==4) 
						     *(uint32_t *) buff=SWAP_LONG  ((*(uint32_t *) buff)); 
						else if(l==2) 
						     *(unsigned short *)   buff=SWAP_SHORT ((*(unsigned short *)   buff));
					}
				} /* fin if l==2 ==4 */
			} /* fin if g pair */
		} /* fin sw */


	switch (l) {
		case 2:  type = BI; break;
	 	case 4:  type = BD; break;
	 	default: if(l>5000) type=NO; else type = AT;
	}

	} /* fin (l < 5000 || mode ==1) */

	else
	{
	if (DEBUG) IdPrintf(" --> Element too long. Skipped\n");
        fseek(fp,l,SEEK_CUR);
	}

if (DEBUG) 
   {
	if (n==0) IdPrintf("   gr %x\tnum %x\tlong %d lgr_grp = %d\n",
			    	g,n,l,*(uint32_t *) buff);

	else if (l==2) {	((char *)buff)[2]=0; 
				IdPrintf("\t\t[%s]  \tval = %d \tx(%04x)\n", 
				buff, *(unsigned short int *) buff, *(unsigned short int *) buff); 
			}

	else if (l==4) { 	((char *)buff)[4]=0; 
				IdPrintf("\t\t[%s]  \tval = %d \tx(%08x)\n", 
				buff, *(uint32_t *) buff     , *(uint32_t *) buff); 
			}

	else if (l<5000 ) 
			{
				((char *)buff)[l]=0;
				IdPrintf("\t\t[%s]\n", (char *)buff);
			}
	
	else IdPrintf(" --> Element too long. Not Printed\n");
   }

if((p=(ACR_ELEMENT *)calloc(1, sizeof(ACR_ELEMENT)))!=0) 

	{
	p->group= (unsigned short int)g;
	p->number=(unsigned short int)n;
	p->length=(uint32_t)l;
	p->skippedLength=(uint32_t)skL; /* pour AffListe2 */
	p->type=  type;
	p->suiv=  0;
	p->value= (long int)(buff);
	}
     else
	IdPrintf("PB allocation memoire ACR_ELEMENT\n");

if (DEBUG) IdPrintf(" ===> sortie de _IdAcrReadNextElement\n");

return(p);

}

/* =======================================================================
	_IdAcrGetElementLength

                     Recherche un acr_element et renvoie sa longueur
                          (0 si pas trouve)   

(le fichier doit deja avoir ete ouvert, 
et _IdAcrCheckSwap(FILE * fp) avoir ete appelle)

                     
   ======================================================================= */

/**
 * \ingroup       acr
 * \brief         Recherche un acr_element et renvoie sa longueur.
 *			(le fichier doit deja avoir ete ouvert,
 *			et _IdAcrCheckSwap(FILE * fp) avoir ete appelle)
 * @param gr  partie 'group' du Tag de l'acr_element a lire.
 * @param num partie 'elem' du Tag de l'acr_element a lire.
 * @param fp      File descriptor dans lequel effectuer la recherche.
 * @return        En cas de succes, longueur du champ lu. 
 *                0 en cas d'echec.
 */


long _IdAcrGetElementLength(int gr, int num, FILE *fp) 
{
uint32_t l, l_gr;
int flag;
unsigned short g;
unsigned short n;
int skL;

if (DEBUG)  IdPrintf("recherche (2) gr %x  num %x (swapping %d)\n",gr,num,sw); 

l=0;
flag=0;
while(fread(&g,2,1,fp))
	{
	if(sw)g=SWAP_SHORT(((short)g));
	fread(&n,2,1,fp);
	if(sw)n=SWAP_SHORT(((short)n));
	l = _IdAcrRecupLgr(fp,&skL);


	if (DEBUG)  IdPrintf("  -------------> gr %x  num %x  long %x : %d\n",g,n,l,l);    

        if(gr>g)           /* on est plus haut que l'element cherche      */
           	flag=1;    /* si on ne le trouve pas des le premier coup  */
                           /* c'est qu'il n'y est pas          JPR        */            
	if((gr>g)&&(n==0))  /* on est sur un debut de groupe   JPR        */
		{

		fread(&l_gr,4,1,fp);		//on lit la longueur du groupe
		if(sw) l_gr = SWAP_LONG(((uint32_t)l_gr));

		fseek(fp,l_gr,SEEK_CUR);
		if (DEBUG)  IdPrintf("                   on saute au gr sv :%d car\n",l_gr);        
                }

	else if((g==gr)&&(n==num))    /* on est sur l'element recherche   */
                                      /* ------------------------------   */
		{
		return l;		/* on renvoie sa longueur	*/
		}

	else if((g>gr)||((gr==g)&&(n>num)))     /* on a depasse l'element */
		{
		if(flag)            /*on a deja re-essaye depuis le debut */
			{           /* l'element cherche n'y est pas      */
			fseek(fp,l,SEEK_CUR);/*on se positionne sur l'element sv */
			return(0);    /*  On sort                JPR      */
			}            			  
		fseek(fp,l,SEEK_SET);
		flag=1; 	     /* On va re-essayer depuis le debut  */

		if ((CardioVasc == 1) || (__TrueDicom == 1))
				fseek(fp,132L,SEEK_SET); /* pour CardioVasc G.E. */

		}

	else {                       /* on saute a l'element suivant      */
		fseek(fp,l,SEEK_CUR);
             }
	}
return(0);
}


/* ======================================================================= 
*	_IdAcrRecupLgr
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
 * \ingroup       acr
 * \brief         recupere la longueur d'un champ DICOM.
 *			(le fichier doit deja avoir ete ouvert,
 *			 _IdAcrCheckSwap(FILE * fp) avoir ete appelle)
 *			et la  partie 'group'  ainsi que la  partie 'elem' 
 * 		        de l'acr_element doivent avoir ete lues.
 * @param fp      File descriptor dans lequel effectuer la recherche.
 * @param skippedLength     nombre d'octets que l'on saute qd la lecture est finie
 * @return        longueur du champ 
 */

uint32_t _IdAcrRecupLgr(FILE *fp, int *skippedLength)
{
uint32_t l_gr; 
unsigned short int l_gr_2;
int i, trouve;

/*
*
*   ATTENTION :
*
* c'est TRES gorret
* mais je n'ai pas le temps d'inventer qq chose de propre :-(
*
*/

int nbCode=26; // nombre d'elements dans la table de type DICOM_VR definie dans dicom.c


  if (__ExplicitVR == 1) {

	fread(VR,2,1,fp);
	VR[2]=0;

// ATTENTION :
// Ce n'est pas parce qu'on a trouve UL la premiere fois qu'on respecte 
// Explicit VR tout le temps
// (cf e=film ...)
// A completer par le test de ttes les valeurs possibles :-(

	for(i=0,trouve=0;i<nbCode;i++) {
		//printf(" %d , %s\n", i,_ID_dicom_vr[i].dicom_VR);
		if(memcmp(_ID_dicom_vr[i].dicom_VR,VR,(size_t)2)==0) {
			trouve=1;
			break;
		}
	}

	if ( trouve == 0) {

		// On est mal : implicit VR repere
		// mais ce n'est pas un code connu ...
		// On reconstitue la longueur
		
		if(DEBUG) IdPrintf("Explicit VR, mais pas trouve de code connu\n");
		memcpy(&l_gr, VR,(size_t)2);
		fread( ((char*)&l_gr)+2,2,1,fp);
		if(sw) l_gr = SWAP_LONG(((uint32_t)l_gr));
		if(DEBUG) IdPrintf("lgr deduite : %08x , %d\n",l_gr,l_gr);
		
		if ( (int)l_gr == -1) {
			l_gr=0;
		}
		*skippedLength = 4; 
		return(l_gr);
	}

	// On repart dans la sequence 'sensee'

	if(DEBUG) IdPrintf("VR : [%01x , %01x] en position %d du tableau\n", VR[0],VR[1],i);
	//printf(" %d , %s\n", i,_ID_dicom_vr[i].dicom_VR);
	if ( 	
	   (!memcmp( VR,"OB",(size_t)2 )) || 
	   (!memcmp( VR,"OW",(size_t)2 )) || 
	   (!memcmp( VR,"SQ",(size_t)2 )) ||
	   (!memcmp( VR,"UN",(size_t)2 )) ) {

	// les 2 octets suivants sont reserves

		if(DEBUG) IdPrintf("les 2 octets suivants sont reserves\n");
		fread(VR,2,1,fp); 	//on les saute
		fread(&l_gr,4,1,fp);	//on lit la lgr sur QUATRE octets
		if(sw) l_gr = SWAP_LONG(((uint32_t)l_gr));
		*skippedLength = 8;

	} else {
		fread(&l_gr_2,2,1,fp);	//on lit la lgr sur DEUX octets
		if(sw) l_gr_2 = SWAP_SHORT((unsigned short)l_gr_2);
		
		if ( l_gr_2 == 0xffff) {
			l_gr = 0;	
		} else {
			l_gr = l_gr_2;
		}
		*skippedLength = 4;
	}	
  } else {	// Explicit VR = 0	

	fread(&l_gr,4,1,fp);	//on lit la lgr sur QUATRE octets
	if(sw)l_gr=SWAP_LONG(((long)l_gr));
	*skippedLength = 4;
  }
	
if (l_gr == 0xffffffff) 
	l_gr = 0;

return(l_gr);
}




/* ===============================================
*
*
*            ATTENTION :
*			Les trois fonctions qui suivent NE MARCHENT PAS
*			pour du DICOM en Explicit Value Representation
*			La VRAIE solution sera de faire un VRAI DICOM writer (?!)
*
 ================================================= */





/* =======================================================================
 	_IdAcrOverWriteElementNumber

	Remplace le numero d'un acr_element existant par une valeur donnee
	(0 si pas trouve) 
	A MANIER AVEC PRECAUTION

	ATTENTION : le fichier doit avoir ete ouvert en ID_RFILE_P_BIN ...  (rb+)                        
   ======================================================================= */

long _IdAcrOverWriteElementNumber(int gr, int num, FILE *fp, int nb) 
{
unsigned short g;
unsigned short n;
unsigned short numero;

uint32_t l, l_gr;
int flag;
int i, skL;
numero=nb; /* on passe en USHORT */

if (DEBUG)  IdPrintf("recherche (3) gr %x  num %x (swapping %d)\n",gr,num,sw); 

l=0;
flag=0;
while(fread(&g,2,1,fp))
	{
	if(sw)g=SWAP_SHORT(((short)g));
	fread(&n,2,1,fp);
	if(sw)n=SWAP_SHORT(((short)n));
	l = _IdAcrRecupLgr(fp,&skL);

if (DEBUG)  IdPrintf("  -------------> gr %x  num %x  long %x : %d\n",g,n,l,l);    

        if(gr>g)           /* on est plus haut que l'element cherche      */
           	flag=1;    /* si on ne le trouve pas des le premier coup  */
                           /* c'est qu'il n'y est pas          JPR        */            
	if((gr>g)&&(n==0))  /* on est sur un debut de groupe   JPR        */
		{
		fread(&l_gr,4,1,fp);
		if(sw)l_gr=SWAP_LONG(((long)l_gr)); /* on lit sa longueur */
		fseek(fp,l_gr,SEEK_CUR);
		if (DEBUG)  IdPrintf("                   on saute au gr sv :%d car\n",l_gr);
		return(0);        
             }

	else if((g==gr)&&(n==num))    /* on est sur l'element recherche   */
                                      /* ------------------------------   */
		{
			/* on se repositionne sur le numero Element */
			/* ATTENTION : ne marchera pas pour les Explicit VR skL !=4 ... */
			/* a rectifier 							*/
		fseek(fp, -6, SEEK_CUR);

		if(sw)numero=SWAP_SHORT(((short)numero)); /* A VOIR ... du VRAI DICOM est tjs en littleEndian */

		for (i=0;i<2;i++) {
			putc( ((char *)&numero)[i] , fp);
		}
		
		return l;
		}

	else if((g>gr)||((gr==g)&&(n>num)))     /* on a depasse l'element */
		{
		if(flag)            /*on a deja re-essaye depuis le debut */
			{           /* l'element cherche n'y est pas      */
			fseek(fp,l,SEEK_CUR);/*on se positionne sur l'element sv */
			return(0);    /*  On sort                JPR      */
			}            			  
		fseek(fp,l,SEEK_SET);
		flag=1; 	     /* On va re-essayer depuis le debut  */
		}
	else {                       /* on saute a l'element suivant      */
		fseek(fp,l,SEEK_CUR);
             }
	}
return(0);
}

/* =======================================================================
	_IdAcrOverWriteElement

	Ecrase un acr_element existant par une valeur prise dans dans buff
	retourne la taille de l'acr_element qui a ete remplace
	(0 si pas trouve) 

	ATTENTION : le fichier doit avoir ete ouvert en ID_RFILE_P_BIN ...  (rb+) 

	ATTENTION : Un probleme majeur risque de se poser avec les int32 et les int16
			(la question du swap en ecriture n'a pas ete traitee)

		 DONC : pour le moment, se limiter aux chaines de caracteres 

		ATTENTION : 	pour des raisons non encore elucidees,
				Si la lgr est superieure a celle sur disque
				malgre les precautions prises
				ca se passe MAL                      
   ======================================================================= */

long _IdAcrOverWriteElement(int gr, int num, FILE *fp, void *buff) 

{
unsigned short g;
unsigned short n;
long l, l_gr, ptr;
int flag, skL;
int i;
int retCode;

if (DEBUG)  IdPrintf("_IdAcrOverWriteElement :recherche (4) gr %x  num %x (swapping %d)\n",gr,num,sw); 

l=0;
flag=0;
while(fread(&g,2,1,fp))
	{
	if(sw)g=SWAP_SHORT(((short)g));
	fread(&n,2,1,fp);
	if(sw)n=SWAP_SHORT(((short)n));
	// Lecture de la longueur de l'element
	l = _IdAcrRecupLgr(fp, &skL);
	if (DEBUG)  IdPrintf("  _IdAcrOverWriteElement-------------> gr %x  num %x  long %x : %d\n",g,n,l,l);    

        if(gr>g)           /* on est plus haut que l'element cherche      */
           	flag=1;    /* si on ne le trouve pas des le premier coup  */
                           /* c'est qu'il n'y est pas          JPR        */ 
	if (0)           
	/*if((gr>g)&&(n==0))*/  /* on est sur un debut de groupe   JPR        */
		{
		fread(&l_gr,4,1,fp);
		if(sw)l_gr=SWAP_LONG(((long)l_gr)); /* on lit sa longueur */
		fseek(fp,l_gr,SEEK_CUR);
		if (DEBUG)  IdPrintf("                   on saute au gr sv :%d car\n",l_gr);        
             }

	else if((g==gr)&&(n==num))    /* on est sur l'element recherche   */
                                      /* ------------------------------   */
		{
		if (DEBUG)  IdPrintf("_IdAcrOverWriteElement-------------------------on re-ecrit (%04x,%04x) lgr %d\n",g,n,l); 

		fgetpos(fp,(fpos_t *)&ptr);
		fsetpos(fp,(fpos_t *)&ptr);
       
		for (i=0;i<l;i++)
			{// printf("%d %c\n", i,((char *)buff)[i]);
			retCode=fputc(((char *)buff)[i],fp);
			//printf("%d %08x\n",retCode,retCode);
			if(retCode == EOF) {
				IdPrintf("Erreur re-ecriture\n");
				switch(errno) {
					case EAGAIN : IdPrintf("EAGAIN\n"); break;
					case EBADF : IdPrintf("EBADF\n"); break;
					case EFBIG : IdPrintf("EFBIG\n"); break;
					case EINTR : IdPrintf("EINTR\n"); break;
					case EIO : IdPrintf("EIO\n"); break;
					case ENOSPC : IdPrintf("ENOSPC\n"); break;
					case EPIPE : IdPrintf("EPIPE\n"); break;
					case ENOMEM : IdPrintf("ENOMEM\n"); break;
					case ENXIO : IdPrintf("ENXIO\n"); break;
					}
				}
			}

		fgetpos(fp,(fpos_t *)&ptr);
		fsetpos(fp,(fpos_t *)&ptr);

		return l;
		}

	else if((g>gr)||((gr==g)&&(n>num)))     /* on a depasse l'element */
		{
		if(flag)            /*on a deja re-essaye depuis le debut */
			{           /* l'element cherche n'y est pas      */
			fseek(fp,l,SEEK_CUR);/*on se positionne sur l'element sv */
			return(0);    /*  On sort                JPR      */
			}            			  
		fseek(fp,l,SEEK_SET);
		flag=1; 	     /* On va re-essayer depuis le debut  */
		}
	else {                       /* on saute a l'element suivant      */
		fseek(fp,l,SEEK_CUR);
             }
	}
return(0);
}

/* =======================================================================
	_IdAcrModifElementLength

                     Recherche un acr_element et MODIFIE sa longueur

	ATTENTION : CASSE TOUT si cet element n'est pas celui des PIXELS...

	ATTENTION : le fichier doit avoir ete ouvert en ID_RFILE_P_BIN ...  (rb+)                          
   ======================================================================= */

long _IdAcrModifElementLength(int gr, int num, FILE *fp, uint32_t newLength) 
{
unsigned short g;
unsigned short n;
uint32_t l, l_gr, lgrEcr;
int flag,skL;

if (DEBUG)  IdPrintf("recherche (5) gr %x  num %x (swapping %d)\n",gr,num,sw); 

l=0;
flag=0;
while(fread(&g,2,1,fp))
	{
	if(sw)g=SWAP_SHORT(((short)g));
	fread(&n,2,1,fp);
	if(sw)n=SWAP_SHORT(((short)n));

	// Lecture de la longueur de l'element
	l = _IdAcrRecupLgr(fp, &skL);

if (DEBUG)  IdPrintf("  -------------> gr %x  num %x  long %x : %d\n",g,n,l,l);    

        if(gr>g)           /* on est plus haut que l'element cherche      */
           	flag=1;    /* si on ne le trouve pas des le premier coup  */
                           /* c'est qu'il n'y est pas          JPR        */            
	if((gr>g)&&(n==0))  /* on est sur un debut de groupe   JPR        */
		{
		fread(&l_gr,4,1,fp);
		if(sw)l_gr=SWAP_LONG(((long)l_gr)); /* on lit sa longueur */
		fseek(fp,l_gr,SEEK_CUR);

if (DEBUG)  IdPrintf("                   on saute au gr sv :%d car\n",l_gr);        
             }

	else if((g==gr)&&(n==num))    /* on est sur l'element recherche   */
                                      /* ------------------------------   */
		{
		fseek (fp, -4, SEEK_CUR);
		lgrEcr = fwrite(&newLength ,4,1,fp);
		return(lgrEcr);
		}

	else if((g>gr)||((gr==g)&&(n>num)))     /* on a depasse l'element */
		{
		if(flag)            /*on a deja re-essaye depuis le debut */
			{           /* l'element cherche n'y est pas      */
			fseek(fp,l,SEEK_CUR);/*on se positionne sur l'element sv */
			return(0);    /*  On sort                JPR      */
			}            			  
		fseek(fp,l,SEEK_SET);
		flag=1; 	     /* On va re-essayer depuis le debut  */
		}

	else {                       /* on saute a l'element suivant      */
		fseek(fp,l,SEEK_CUR);
             }
	}
return(0);
}


/* ===============================================
*
*
*            A partir d ici, plus de probleme
*
*
 ================================================= */





/* =======================================================================
	_IdAcrMakeElement

		creation d'un acr_element d'entete ACRNEMA 
   ======================================================================= */

ACR_ELEMENT *_IdAcrMakeElement(int gr, int nu, long int size, int type, long int init)

/* ATTENTION : init pointeur sur une chaine de char(types AN,AT)
 *		ou valeur (types BI,BD) 
*/
{
ACR_ELEMENT *p;

if (DEBUG) IdPrintf("MakeElement gr %x nu %x size %d type %x init %d\n",gr,nu,size,type,init);

if(size & 1)size+=1; /* size doit etre paire */

if((p=(ACR_ELEMENT *)calloc(1, sizeof(ACR_ELEMENT)))!=0) 

	{
	p->group= (short int)gr;
	p->number=(short int)nu;
	p->length=(uint32_t)size;
	p->type=  type;
	p->suiv=  0;
	p->value= init;
	}
else
	IdPrintf("PB allocation memoire 1\n");
return(p);
}

/* =======================================================================
 *	_IdAcrAddElement
 *
 *		insertion d'un acr_element dans la liste d'entete ACRNEMA 
 *  ======================================================================= */

ACR_ELEMENT *_IdAcrAddElement(ACR_ELEMENT *liste, ACR_ELEMENT *elem)
{
ACR_ELEMENT *p,*pp;
int flag;

if(!elem)return(0);

/*if (DEBUG) IdPrintf ("AddElement %p %p \n",liste,elem);*/

flag=0; /* pas d'insertion effectuee */

if (DEBUG) IdPrintf ("_IdAcrAddElement on ajoute : group : %04x elem : %04x lgr : %d ", elem->group, elem->number, elem->length);
if (DEBUG) IdPrintf("\n");

/* liste vide */
if(liste==0)
	return(elem);
/* liste non vide */
p=liste;
pp=0;



while( (p!=0) && (!flag)) {

	// laiser les 4 lignes commentees
	//if (DEBUG) { 	IdPrintf ("\t\t_IdAcrAddElement on Explore : group : %04x elem : %04x lgr : %d", 
	//			p->group, p->number, p->length);
	//		IdPrintf("\n");
	//}

	if(  (p->group < elem->group) || 
	     ((p->group == elem->group) && (p->number < elem->number))) {
		pp=p;p=p->suiv;

	} else {
		if( (p->group > elem->group) ||
		    ((p->group == elem->group) && (p->number > elem->number))) {       
			flag=1;
			if(pp) {
				pp->suiv=elem;
				elem->suiv=p;
			} else {
				elem->suiv=p;
				liste=elem;
			}
		} else  {          /*  element identique */
			IdPrintf("ACR_ELEMENT deja present.  : (%x, %x)\n",p->group,p->number);
			flag=1;
		}
	}
}

if(!flag)
	pp->suiv=elem;

return(liste);
}



/* =======================================================================
 *	_IdAcrAppendElement
 *
 *		insertion d'un acr_element EN FIN d'une liste d'entete ACRNEMA 
 *		(sans elimination des doublons, sans test de la sequence croissante)
 *  ======================================================================= */

ACR_ELEMENT *_IdAcrAppendElement(ACR_ELEMENT *liste, ACR_ELEMENT *elem)
{
ACR_ELEMENT *p,*pp;

if(!elem)return(0);

/*if (DEBUG) IdPrintf ("_IdAcrAppendElement %p %p \n",liste,elem);*/

if (DEBUG) IdPrintf ("_IdAcrAppendElement on ajoute : group : %04x elem : %04x lgr : %d ", elem->group, elem->number, elem->length);
if (DEBUG) IdPrintf("\n");

/* liste vide */
if(liste==0)
	return(elem);
/* liste non vide */
p=liste;
pp=0;

while (p!=0) {
	pp=p;p=p->suiv;
}

pp->suiv=elem;

return(liste);
}


/* =======================================================================
 *	_IdAcrModifyElement
 *
 *		Remplacement d'un acr_element dans la liste d'entete ACRNEMA (pour Marcela)
 *  ======================================================================= */

ACR_ELEMENT *_IdAcrModifyElement(ACR_ELEMENT *liste, ACR_ELEMENT *elem)
{
ACR_ELEMENT *p, *pp, *prec;
int flag;

if(!elem)return(0);

if (DEBUG) IdPrintf ("_IdAcrModifyElement %p %p \n",liste,elem);

flag=0; /* pas d'insertion effectuee */

if (DEBUG) IdPrintf ("_IdAcrModifyElement on ajoute : group : %04x elem : %04x lgr : %d ", elem->group, elem->number, elem->length);
if (DEBUG) IdPrintf("\n");

/* liste vide */

if(liste==0) {
	IdPrintf("Tentative de modif d'un ACR Elem dans une ACR List vide\n");
	return(NULL);
}
/* liste non vide */

p=   liste;
prec=liste;
pp=  0;

while( (p!=0) && (!flag))
	{
	if (DEBUG) IdPrintf ("\t\t_IdAcrModifyElement on Explore : group : %04x elem : %04x lgr : %d", 
				p->group, p->number, p->length);
	if (DEBUG) IdPrintf("\n");

	if(     (p->group < elem->group) || 
		((p->group == elem->group) && (p->number < elem->number)))
		{pp=p; prec=p; p=p->suiv;}

	else	{
		if( (p->group > elem->group) ||
		((p->group == elem->group) && (p->number == elem->number)))
			{
			elem->suiv=p->suiv;
			prec->suiv=elem;
			free(p);
			}
		else    {
			IdPrintf("ACR_ELEMENT absent. Modif Impossible : (%x, %x)\n",p->group,p->number);
			flag=1;
			}
		}
	}
return(liste);
}


/* =======================================================================
 *	_IdAcrFreeListe
 *
 *		liberation de la place utilisee par l'entete ACRNEMA 
 * ======================================================================= */


ACR_ELEMENT *_IdAcrFreeListe(ACR_ELEMENT *liste)
{
ACR_ELEMENT *p,*pp;
p=liste;
while(p)
	{
	pp=p->suiv; 
	free(p);
	p=pp;
	}
return(0);
}

/* =======================================================================
 *	_IdAcrMergeListes
 *
 *		Interclassement de 2 listes d'ACR_ELEMENT dans une entete ACRNEMA 
 *   ======================================================================= */

ACR_ELEMENT *_IdAcrMergeListes(ACR_ELEMENT *listeOriginale, ACR_ELEMENT *listeAajouter)
{
ACR_ELEMENT *p,*pp;
p=listeAajouter;
while(p)
	{
	pp=p->suiv;                                              
	listeAajouter=_IdAcrAddElement(listeOriginale,p);
	if(listeAajouter==0) return(0);
	p=pp;                                                    
	}
return(listeOriginale);
}

/* =======================================================================
 *	_IdAcrFindElement
 *
 *		Recherche EN MEMOIRE d'un Element de la (future) ACRNEMA 
 *  ======================================================================= */

ACR_ELEMENT *_IdAcrFindElement(ACR_ELEMENT *liste, int gr, int nu)
{
ACR_ELEMENT *p;
p=liste;
while(p && ((gr != p->group) || (nu != p->number)))
	{
	p=p->suiv;
	}
return(p);
}

/* =======================================================================
 *	_IdAcrAdjustLength
 *
 *		Aligne la taille de al liste d'ACR_ELEMENT
 *		sur unr frontiere DICOM compatible
 *  ======================================================================= */

void _IdAcrAdjustLength(ACR_ELEMENT *liste)
{
ACR_ELEMENT *p;
long g;
int s;
long s_tot;
s=0;
s_tot=0;
g = -1; /*BB 18-JUN-1992 08:53:54 : g=-1;*/
p=liste;
while(p)
	{
	if(g != p->group)
		{
		if(g != -1)
			{
			(_IdAcrFindElement(liste,g,0))->value=s;
			s_tot+=s+12;
			s=0;
			}
		g=p->group;
		}
	else
		{
		if(p->number) s += p->length + 8;
		}
	p=p->suiv;
	}

(_IdAcrFindElement(liste,g,0))->value=s;
s_tot+=s+12;
(_IdAcrFindElement(liste,8,1))->value=s_tot-24;
}

/* =======================================================================
 *       _IdAcrAffListe (pour liste creee par LibIDO)
 *  ======================================================================= */

void _IdAcrAffListe(ACR_ELEMENT *liste)
{
ACR_ELEMENT *p;
p=liste;
while(p)
        {
 	IdPrintf(" %04x\t%04x long.=%d \tvaleur ",
                 p->group,p->number,p->length); 
        switch(p->type)
                {
                case BI:IdPrintf("%d\n",  p->value);            break;
                case BD:IdPrintf("%d\n",  p->value);            break;
                case AN:IdPrintf("[%s]\n",(char *)p->value);    break;
                case AT:IdPrintf("[%s]\n",(char *)p->value);    break;
                case NO:IdPrintf("%x\n",  p->value);            break;
                }
        p=p->suiv;
        }
}

/* =======================================================================
 *	_IdAcrAffListeBref (pour Liste cree par _IdAcrInquireHeader)
 *  ======================================================================= */

void _IdAcrAffListeBref(ACR_ELEMENT *liste)
{
ACR_ELEMENT *p;
p=liste;
while(p)
	{
	 IdPrintf(" %04x\t%04x long.=%d \tvaleur ",
		 p->group,p->number,p->length); 
	switch(p->type)
		{
		case BI:IdPrintf("%d [%8x]\n", *(short int *)(p->value),  *(short int *)(p->value));	break;
		case BD:IdPrintf("%d [%8x]\n", *(uint32_t *)(p->value), *(uint32_t *)(p->value));	break;
		case AN:IdPrintf("[%s]\n",(char *)p->value);	break;
		case AT:IdPrintf("[%s]\n",(char *)p->value);	break;
		case NO:IdPrintf("%8x\n",  p->value);		break;
		default:IdPrintf(" ?? %d [%8x]\n",  p->value, p->value); break;
		}
	p=p->suiv;
	}
}

/* =======================================================================
 *	_IdAcrAffListe2 (Reserve a l'affichage du resultat de IdAcrInquireHeader)
 *  ======================================================================= */

void _IdAcrAffListe2(ACR_ELEMENT * liste, int npriv, int noffset)
{
ACR_ELEMENT *p;
DICOM_ELEMENTS *t = NULL;
long  _offset =0;


_offset = __ID_offset + 2 + 2 + 4;


p=liste;

IdPrintf("\n\n\t\tATTENTION : Les chaines de caracteres de longueur 2 ou 4 \n"
	"\t(des groupes de num IMPAIR) peuvent avoir ete 'swappees'\n\n");

while(p) {
	 // if (DEBUG)IdPrintf(" ?? gr %04x num %04x long %d\n", p->group, p->number, p->length);

		if (p->number==0) { 
			if(!npriv) IdPrintf("\n"); 
		 } else { 
			for (t=_ID_dicom_elements; t->dicom_group!=0xffff; t++) {
				if( (t->dicom_group==p->group) && (t->dicom_elem==p->number) ) break;
			}
             	}

		if(!(p->group%2) || !npriv) {

			if (noffset) 
				IdPrintf(" gr %04x  num %04x  long %d",
			  		 p->group, p->number, p->length);
			else

			IdPrintf(" gr %04x  num %04x  long %d  _offset %d",
			   p->group, p->number, p->length, _offset);
		}

		_offset += 2 +2 + p->skippedLength + p->length;

		if (p->number!=0) { 
			if(!(p->group%2) || !npriv) IdPrintf("\t%s\t%s\t\t",  
						t->dicom_type, t->dicom_libelle); 
		} 

		if (p->number==0) { 
			if(!(p->group%2) || !npriv) IdPrintf("   lgr du grp = %d\n   -------\n",
					*(long *)(p->value)); 
		}
 

/* MANQUE le retrait, avant impression en %s, des caract non ASCII */

		else 	if (p->length==2)  {
				if ( (strcmp(t->dicom_type, "US") == 0)
	  		  	  || (strcmp(t->dicom_type, "SS") == 0) 
          		  	  || (t->dicom_group == 0x0028 && 
				     (t->dicom_elem == 0x0005  || t->dicom_elem == 0x0200) ) )  {
 
					if(!(p->group%2) || !npriv) { 
						IdPrintf("\t  \t \t %d \tx(%04x)\n", 
						*(short int *)(p->value) , *(short int *)p->value); 
					}
				}
			else

				if(!(p->group%2) || !npriv){ IdPrintf("\t[%s]  \t %d \tx(%04x)\n", 
					(char*)p->value,*(short int *)p->value, *(short int *)p->value);}
			
		}

		else 	if (p->length==4)  {
        			if ( (strcmp(t->dicom_type,"UL") == 0)
          		  	  || (strcmp(t->dicom_type,"SL") == 0) ) {
					if(!(p->group%2) || !npriv) { 
						IdPrintf("\t  \t \t %d \tx(%08x)\n", 
							(int *)p->value, *(short int *)p->value); 
					}
				} else {
					if(!(p->group%2) || !npriv) { 
						IdPrintf("\t[%s]  \t %d \tx(%08x)\n", 
						(char*)p->value,*(long *)p->value, *(long *)p->value); 
					}
				}
			
			}

		else { 
			if (p->length > 5000) { 
				if(!(p->group%2) || !npriv) 
					IdPrintf(" --> Too Long. Not Printed...\n");
			} else {
				if(!(p->group%2) || !npriv) IdPrintf(" \t[%s]\n",p->value);
			}
	     }
	p=p->suiv;
	}
}



/* =======================================================================
	_IdAcrAddHisto
   ======================================================================= */

int
_IdAcrAddHisto(char *l)
{
FILE *fphisto;

if((fphisto=fopen("_t_histo.tmp","a"))!=0)
	{
	fseek(fphisto,0L,SEEK_END);
	fwrite(l,strlen(l),1,fphisto);
	fclose(fphisto);
	}
  return 0;
}

