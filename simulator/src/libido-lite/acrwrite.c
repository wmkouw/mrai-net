/*************************************************************************
* $Id: acrwrite.c,v 1.1 2005/09/09 08:22:21 bellet Exp $
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
*
*  Description : Ecrit un objet LIBIDO dans un fichier
*		au format ACRNEMA modifie UA1216
*
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // For strlen
#include <ctype.h>   // For isprint


#include "idsig.h"
#include "idcnt.h"
#include "idima.h"
#include "idvol.h"
#include "idseq.h"
#include "idacr.h"
#include "idacr-restricted.h"
#include "iderr.h"
#include "idio.h"
#include "idprint.h"

/*   ---------------------------------    RAPPELS   -------------------------


#define NO 0  	NON ACRNEMA : traitement special pour ecriture 
#define BI 1  	16 bits 
#define BD 2  	32 bits 
#define AN 3  	ASCII numerique 
#define AT 4 	ASCII texte

typedef struct acr_element {
	short int group;
	short int number;
	uint32_t length;
	long int value;     		pointeur ou valeur(max 32 bits) 
	struct acr_element *suiv;
	int type;
	} ACR_ELEMENT;

 --------------------------------------------------------------------------  */

#define DEBUG 0

char *_IdRecCode="ACRNEMA_LIBIDO_1.1";
// char *Histo="";

static ACR_ELEMENT *header=0;


/* FUNCTION DESCRIPTION **************************************************

IdAcrWriteFile   (fonction)

RESUME: Ecriture d'un objet libido ds un fichier ACR 

DESCRIPTION:

Ecriture d'un objet libido PSIGNAL, PCONTOUR, PPIMAGE, PPPVOLUME ou PPPPSEQUENCE
dans un fichier au format ACR_LibIDO. Si le fichier existait deja, son ancien
contenu est perdu.

SYNTAXE:     int retCode = IdAcrWriteFile(char *nomfic,void *obj)

RETOUR:
		type : int
		role : 1 si pas d'erreur, 0 si erreur

PARAMETRES:
	     nom   : nomfic
	     type  : char *
	     role  : nom du fichier a creer

	     nom   : obj
	     type  : void * (en fait :	PSIGNAL, PCONTOUR, PPIMAGE,
					PPPVOLUME, PPPPSEQUENCE)
	     role  : pointeur sur l'objet libido a ecrire dans le fichier

FICHIER:     WRITEACR.C

******************************************************** END DESCRIPTION */


/**
 * \ingroup        acr
 * \brief	   Ecriture d'un objet libido ds un fichier ACR avec ajouts d'ACR_ELEMENT
 *
 * @param nf  		 nom du fichier a creer 
 * @param obj  		 pointeur sur une structure libido
 * @return               code retour (booleen)
 *
 */


int IdAcrWriteFile(char *nf,void *obj)
//char *nf;  /* nom du fichier a creer */
//void *obj; /* pointeur sur une structure libido 
//	      PSIGNAL PCONTOUR PPIMAGE PPPVOLUME PPPSEQUENCE */

{
return IdAcrWriteFileExtended(nf,obj,NULL);
			/* Oui, je sais...
			   Il eut ete + efficace de mettre un #define...
			   Mais ca obligeait tout le Labo de recompiler
			   TOUS les Sources... 

			Et puis FROG m'aurait dit qu'il faut mettre une fonction, et pas une macro.
				4/12/2000
			*/
}


/* FUNCTION DESCRIPTION **************************************************

IdAcrWriteFileExtended   (fonction)

RESUME: Ecriture d'un objet libido ds un fichier ACR avec ajouts d'ACR_ELEMENT

DESCRIPTION:

Ecriture d'un objet libido PSIGNAL, PCONTOUR, PPIMAGE, PPPVOLUME, ou PPPPSEQUENCE
dans un fichier au format ACRNEMA. 
Si le fichier existait deja, son ancien
contenu est perdu.
A la liste des ACR_ELEMENT de LibIDO est ajoutee une liste d'ACR_ELEMENT
cree par l'utilisateur, sous son propre controle 
--> NOT FOR BOZOS ...

SYNTAXE:     int retCode = IdAcrWriteFileExtended(char *nomfic,void *obj, ACR_ELEMENT * liste);

RETOUR:
		type : int
		role : 1 si pas d'erreur, 0 si erreur

PARAMETRES:
	     nom   : nomfic
	     type  : char *
	     role  : nom du fichier a creer

	     nom   : obj
	     type  : void * (en fait :	PSIGNAL, PCONTOUR, PPIMAGE,
					PPPVOLUME, PPPPSEQUENCE)
	     role  : pointeur sur l'objet libido a ecrire dans le fichier

	     nom   : listeAAjouter
	     type  : ACR_ELEMENT *
	     role  : liste d'ACR_ELEMENT, 
			fabriquee par l'utilisateur
			qui sera stockee sur disque 
			en plus des ACR_ELEMENT
			de LibIDO 
FICHIER:     WRITEACR.C

******************************************************** END DESCRIPTION */

/**
 * \ingroup        acr
 * \brief	   Ecriture d'un objet libido ds un fichier ACR avec ajouts d'ACR_ELEMENT
 *
 * @param nf  		 nom du fichier a creer 
 * @param obj  		 pointeur sur une structure libido
 * @param listeAAjouter  liste d'ACR_ELEMENT, fabriquee par l'utilisateur 
 * @return               code retour (booleen)
 *
 */


int IdAcrWriteFileExtended(char *nf, void *obj, ACR_ELEMENT *listeAAjouter)

//char *nf;  /* nom du fichier a creer */
//void *obj; /* pointeur sur une structure libido 
//	      PSIGNAL PCONTOUR PPIMAGE PPPVOLUME PPPPSEQUENCE */
//ACR_ELEMENT *listeAAjouter;  liste d'ACR_ELEMENT, fabriquee par l'utilisateur 
//				fabriquee par l'utilisateur
//				qui sera stockee sur disque 
//				en plus des ACR_ELEMENT
//				de LibIDO 
{
FILE *fp,*fphisto;
PSIGNAL   	sip = NULL;      /*  FB 20-01-2000  */
PCONTOUR  	cop = NULL;      /*  FB 20-01-2000  */
PPIMAGE   	imp = NULL;      /*  FB 20-01-2000  */
PPPVOLUME 	vop = NULL;      /*  FB 20-01-2000  */
PPPPSEQUENCE 	sep = NULL;      /*  FB 20-01-2000  */

int i,j,k,ty;
int ndim;
short int sh;
uint32_t i32;
ACR_ELEMENT *p;
// long lo;
int retval=1;

if(obj==NULL) { IdErrPrintf(" writeacr --> Pointeur NULL !!!\n");
 		IdErrno=IDERR_POINTER_IS_NULL; 
		return (0);
	       }

fphisto=fopen("_t_histo.tmp","r");

/*							lgr du groupe	*/

header=_IdAcrAddElement(header,_IdAcrMakeElement(0x0008,0x0000,4,BD,0));
header=_IdAcrAddElement(header,_IdAcrMakeElement(0x0008,0x0001,4,BD,0));
header=_IdAcrAddElement(header,
	_IdAcrMakeElement(0x0008,0x0010,strlen(_IdRecCode),AT,
			(long int)_IdRecCode));
header=_IdAcrAddElement(header,_IdAcrMakeElement(0x0028,0x0000,4,BD,0));

     if( IdLibidoType(obj)==SIG) { ndim=1; 
				   imp=(PPIMAGE)((void*)(sip=(PSIGNAL)obj));   
				 }
else if( IdLibidoType(obj)==IMA) { ndim=2; 
				   imp=    (PPIMAGE)obj;   
				 }
else if( IdLibidoType(obj)==CNT) { ndim=1;
				   cop=(PCONTOUR)obj;
				   imp=(PPIMAGE)obj;  
				 }
else if( IdLibidoType(obj)==VOL) { ndim=3; 
				   imp=(PPIMAGE)(vop=(PPPVOLUME)obj); 
				 }
else if( IdLibidoType(obj)==SEQ) { ndim=4;
				   sep=(PPPPSEQUENCE)obj;
				   imp=(PPIMAGE)obj; 
				 }          

else {IdErrPrintf("Type d'Objet non traite par IdAcrWriteFile (%04x)!!!\n",IdLibidoType(obj));
			IdErrno=IDERR_WRONG_TYPE;
			return (0);}


header=_IdAcrAddElement(header,_IdAcrMakeElement(0x0028,0x0005,2,BI,ndim));

// Il ne *devrait* plus y avoir Histo nulle part
/*
if(!fphisto)
	lo=strlen(Histo);
else
	{
	fseek(fphisto,0L,2);
	lo=ftell(fphisto);
	Histo=(char *)calloc(1,(int)lo+2);
	fseek(fphisto,0L,0);
	fread(Histo,(int)lo,1,fphisto);
	}
if(lo & 1){Histo[(int)lo]=' ';lo++;}
header=_IdAcrAddElement(header,_IdAcrMakeElement(0x0028,0x0050,(int)lo,AT,
		(long int)Histo));
*/


/*					nb bits alloues */
header=_IdAcrAddElement(header,
	_IdAcrMakeElement(0x0028,0x0100,2,BI,IdSizeOfType(imp)*8));

/*					nb bits utiles	*/
header=_IdAcrAddElement(header,
	_IdAcrMakeElement(0x0028,0x0101,2,BI,IdSizeOfType(imp)*8));

/*					high bit	*/
header=_IdAcrAddElement(header,
	_IdAcrMakeElement(0x0028,0x0102,2,BI,IdSizeOfType(imp)*8-1));
ty=0;

     if(IdLibidoDataType(imp)==TY_CHAR)   ty=1;
else if(IdLibidoDataType(imp)==TY_SHORT)  ty=1;
else if(IdLibidoDataType(imp)==TY_LONG)   ty=1; 
else if(IdLibidoDataType(imp)==TY_FLOAT)  ty=1;
else if(IdLibidoDataType(imp)==TY_DOUBLE) ty=1;

else if(IdLibidoDataType(imp)==TY_COMPLEX_CHAR)   ty=1;  /* JPR  */
else if(IdLibidoDataType(imp)==TY_COMPLEX_SHORT)  ty=1;
else if(IdLibidoDataType(imp)==TY_COMPLEX_LONG)   ty=1; 
else if(IdLibidoDataType(imp)==TY_COMPLEX_FLOAT)  ty=1;
else if(IdLibidoDataType(imp)==TY_COMPLEX_DOUBLE) ty=1;  /* JPR */


/*					signe o/n	*/
header=_IdAcrAddElement(header,
	_IdAcrMakeElement(0x0028,0x0103,2,BI,ty));

header=_IdAcrAddElement(header,
	_IdAcrMakeElement(0x7FE0,0x0000,4,BD,0));

/* objet de type PSIGNAL */		/* JPR */

     if( IdLibidoType(obj)==SIG)
	{
	sip=(PSIGNAL )obj;
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0010,2,BI,IdSigDimX(sip)));

	header=_IdAcrAddElement(header,				/* JPR */
		_IdAcrMakeElement(0x0028,0x0015,2,BI,IdSigUsedNbX(sip)));

	header=_IdAcrAddElement(header,				/* JPR */
		_IdAcrMakeElement(0x0028,0x00199,2,BI,((int *)sip)[-1]));

	header=_IdAcrAddElement(header,_IdAcrMakeElement(0x7FE0,0x0010,
		IdSigDimX(sip)*IdSizeOfType(sip),
		NO,(long int)sip));
	if ( _IdCntPrivate(sip)->_message)	/* JPR */		
	header=_IdAcrAddElement(header,
	_IdAcrMakeElement(0x0028,0x0198, strlen(_IdCntPrivate(sip)->_message), AT, (long int)(_IdCntPrivate(sip)->_message))); /*CO 22-02-97 */
	}

/* objet de type PCONTOUR */		/* JPR */

else if( IdLibidoType(obj)==CNT)
	{

	cop=(PCONTOUR )obj;
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0010,2,BI,IdCntDimX(cop)));

/* printf ("IdCntUsedNbX(cop) %x\n",IdCntUsedNbX(cop)); */
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0015,2,BI,IdCntUsedNbX(cop)));

	header=_IdAcrAddElement(header,				
		_IdAcrMakeElement(0x0028,0x00199,2,BI,((int *)cop)[-1]));

	header=_IdAcrAddElement(header,_IdAcrMakeElement(0x7FE0,0x0010,
		IdCntDimX(cop)*IdSizeOfType(cop),
		NO,(long int)cop));
	if ( _IdCntPrivate(cop)->_message)	/* JPR */		
	header=_IdAcrAddElement(header,
	_IdAcrMakeElement(0x0028,0x0198, strlen(_IdCntPrivate(cop)->_message), AT, (long int)(_IdCntPrivate(cop)->_message))); /*CO 22-02-97 */

	}


/* objet de type PPIMAGE */

else if( IdLibidoType(obj)==IMA)
	{
	imp=(PPIMAGE )obj;
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0010,2,BI,IdImaDimX(imp)));
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0011,2,BI,IdImaDimY(imp)));

	header=_IdAcrAddElement(header,				/* JPR */
		_IdAcrMakeElement(0x0028,0x0015,2,BI,IdImaUsedNbX(imp)));

	header=_IdAcrAddElement(header,				/* JPR */
		_IdAcrMakeElement(0x0028,0x0016,2,BI,IdImaUsedNbY(imp)));

	header=_IdAcrAddElement(header,				/* JPR */
		_IdAcrMakeElement(0x0028,0x00199,2,BI,((int *)imp)[-1]));

	header=_IdAcrAddElement(header,_IdAcrMakeElement(0x7FE0,0x0010,
		IdImaDimX(imp)*IdImaDimY(imp)*IdSizeOfType(imp),
		NO,(long int)imp));

	if ( _IdCntPrivate(imp)->_message)	/* JPR */		
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0198, 
				strlen(_IdCntPrivate(imp)->_message), 
				AT, 
				(long int)(_IdCntPrivate(imp)->_message))); /*CO 22-02-97 */

	}

/* objet de type PPPVOLUME */

else if( IdLibidoType(obj)==VOL)
	{ 
	vop=(PPPVOLUME )obj;
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0010,2,BI,IdVolDimX(vop)));
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0011,2,BI,IdVolDimY(vop)));
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0012,2,BI,IdVolDimZ(vop)));

	header=_IdAcrAddElement(header,				/* JPR */
		_IdAcrMakeElement(0x0028,0x0015,2,BI,IdVolUsedNbX(vop)));

	header=_IdAcrAddElement(header,				/* JPR */
		_IdAcrMakeElement(0x0028,0x0016,2,BI,IdVolUsedNbY(vop)));

	header=_IdAcrAddElement(header,				/* JPR */
		_IdAcrMakeElement(0x0028,0x0017,2,BI,IdVolUsedNbZ(vop)));

	header=_IdAcrAddElement(header,				/* JPR */
		_IdAcrMakeElement(0x0028,0x00199,2,BI,((int *)vop)[-1]));

	header=_IdAcrAddElement(header,_IdAcrMakeElement(0x7FE0,0x0010,
		IdVolDimZ(vop)*IdVolDimX(vop)*IdVolDimY(vop)*
			IdSizeOfType(vop),
		NO,(long int)vop));

	if ( _IdCntPrivate(vop)->_message)	/* JPR */	
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0198, 
			strlen(_IdCntPrivate(vop)->_message), 
			AT, 
			(long int)(_IdCntPrivate(vop)->_message))); /*CO 22-02-97 */

	}

else if( IdLibidoType(obj)==SEQ)	/* JPR	*/
	{ 
	sep=(PPPPSEQUENCE )obj;
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0010,2,BI,IdSeqDimX(sep)));
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0011,2,BI,IdSeqDimY(sep)));
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0012,2,BI,IdSeqDimZ(sep)));
	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0013,2,BI,IdSeqDimT(sep)));

	header=_IdAcrAddElement(header,	
		_IdAcrMakeElement(0x0028,0x0015,2,BI,IdSeqUsedNbX(sep)));

	header=_IdAcrAddElement(header,	
		_IdAcrMakeElement(0x0028,0x0016,2,BI,IdSeqUsedNbY(sep)));

	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0017,2,BI,IdSeqUsedNbZ(sep)));

	header=_IdAcrAddElement(header,
		_IdAcrMakeElement(0x0028,0x0018,2,BI,IdSeqUsedNbT(sep)));

	header=_IdAcrAddElement(header,	
		_IdAcrMakeElement(0x0028,0x00199,2,BI,((int *)sep)[-1]));

	header=_IdAcrAddElement(header,_IdAcrMakeElement(0x7FE0,0x0010,
		IdSeqDimT(sep)*IdSeqDimZ(sep)*IdSeqDimX(sep)*IdSeqDimY(sep)*
			IdSizeOfType(sep),
		NO,(long int)sep));

	if ( _IdCntPrivate(sep)->_message)	
	header=_IdAcrAddElement(header,
	_IdAcrMakeElement(0x0028,0x0198, strlen(_IdCntPrivate(sep)->_message), AT, (long int)(_IdCntPrivate(sep)->_message)));

	}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

if (listeAAjouter != NULL) 		/* JPR, pour IdAcrWriteFileExtended	*/
	{
		if (DEBUG) 
			{
			IdPrintf ("header %p listeAAjouter %p\n",header,listeAAjouter);
			_IdAcrAffListe(header);
			IdPrintf ("\n\n\n");
			_IdAcrAffListe(listeAAjouter);
			IdPrintf ("\n\n\n");
			}

	header=_IdAcrMergeListes(header,listeAAjouter);
	if(!header) 
		IdPrintf("Gros soucis lors de l'incorporation des ACR_ELEMENT utilisateurs... \n");
	/* _IdAcrFreeListe(listeAAjouter); */ /* C ODET */
	}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */


if (DEBUG) {	IdPrintf ("\n\nAvant _IdAcrAdjustLength\n");
		 _IdAcrAffListe(header);
		 IdPrintf ("\n\n\n");
	   }

_IdAcrAdjustLength(header);


if (DEBUG) {	IdPrintf ("\n\nApres _IdAcrAdjustLength\n");
		 _IdAcrAffListe(header);
		 IdPrintf ("\n\n\n");
	   }

if((fp=fopen(nf,ID_WFILE_BIN))!=0)
	{
	p=header;
	while(p)
		{
		switch(p->type)
			{
			case BI:
				fwrite(p,8,1,fp);
				sh=(short int)p->value;
				fwrite(&sh,2,1,fp);

				if(DEBUG) IdPrintf(" BI element (en %p) %4x,%4x long.=%d valeur ",
							p, p->group,p->number,p->length);
				if (DEBUG) IdPrintf("%d\n",p->value);			
	
				break;
			case BD:
				fwrite(p,8,1,fp);
				i32=p->value; 
				fwrite(&i32,4,1,fp);
 
				if(DEBUG) IdPrintf(" BD  element (en %p) %4x,%4x long.=%d valeur ",
							p, p->group,p->number,p->length);
				if (DEBUG) IdPrintf("%d\n",p->value);
				break;
			case AT:
				fwrite(p,8,1,fp);
				fwrite((char *)(p->value),(size_t)p->length,1,fp);

				if(DEBUG) IdPrintf(" AT  element (en %p) %4x,%4x long.=%d valeur ",
							p, p->group,p->number,p->length);
				if (DEBUG) IdPrintf("%s\n",(char *)p->value);
				break;
			case AN:
				fwrite(p,8,1,fp);
				fwrite((char *)(p->value),(size_t)p->length,1,fp);

				if(DEBUG) IdPrintf(" AN  element (en %p) %4x,%4x long.=%d valeur ",
							p, p->group,p->number,p->length);
				if (DEBUG) IdPrintf("%s\n",(char *)p->value);

				break;
			case NO:
				fwrite(p,8,1,fp);

				if(DEBUG) IdPrintf(" NO  element (en %p) %4x,%4x long.=%d valeur ",
							p, p->group,p->number,p->length);
				if (DEBUG) IdPrintf("%x\n",p->value);


				switch (IdLibidoType(obj))             /* JPR  */
				{
				case SIG:
				  	fwrite(sip,
				    		IdSigDimX(sip)*IdSizeOfType(sip),1,fp);
					break;

				case CNT:
				  	fwrite(cop,
				    		IdCntDimX(cop)*IdSizeOfType(cop),1,fp);
					break;
				
				case IMA:
					for(i=0;i<IdImaDimY(imp);i++)
					fwrite(imp[i],
				    		IdImaDimX(imp)*IdSizeOfType(imp),1,fp);
					break;

				case VOL:
					for(j=0;j<IdVolDimZ(vop);j++)
				 	for(i=0;i<IdVolDimY(vop);i++)
				  	fwrite(vop[j][i],
				    		IdVolDimX(vop)*IdSizeOfType(vop),1,fp);
					break;

				case SEQ:
					for(k=0;k<IdSeqDimT(sep);k++)
					for(j=0;j<IdSeqDimZ(sep);j++)
				 	for(i=0;i<IdSeqDimY(sep);i++)
				  	fwrite(sep[k][j][i],
				    		IdVolDimX(sep)*IdSizeOfType(sep),1,fp);
					break;
				default:
					IdPrintf("TypeObj %x non supporte par LibIDO\n",IdLibidoType(obj));

				}
			}
		p=p->suiv;
		}
	fclose(fp);
	}

else 	{
	IdErrno = IDERR_OPEN_FAILED;
	retval  = 0;
     	}


header=_IdAcrFreeListe(header);

/*
if(fphisto)
	{
	free(Histo);
	fclose(fphisto);
	}
*/
return(retval);
}

/* FUNCTION DESCRIPTION **************************************************

IdAcrWriteListeAcr   (fonction)

RESUME: Ecriture d'une liste d'ACR_ELEMENT ds un fichier ACR  

DESCRIPTION:

Ecriture d'une liste d'ACR_ELEMENT
(creee par l'utilisateur, sous son propre controle --> NOT FOR BOZOS ...)
dans un fichier au format ACRNEMA. 
Si le fichier existait deja, son ancien
contenu est perdu.

SYNTAXE:     int retCode = IdAcrWriteListeAcr(char *nomfic, ACR_ELEMENT * liste);

RETOUR:
		type : int
		role : 1 si pas d'erreur, 0 si erreur

PARAMETRES:
	     nom   : nomfic
	     type  : char *
	     role  : nom du fichier a creer

	     nom   : liste
	     type  : ACR_ELEMENT *
	     role  : liste d'ACR_ELEMENT, 
			fabriquee par l'utilisateur
			qui sera stockee sur disque 
		
FICHIER:     WRITEACR.C

******************************************************** END DESCRIPTION */

/**
 * \ingroup        acr
 * \brief	   Ecriture d'un objet libido ds un fichier ACR avec ajouts d'ACR_ELEMENT
 *
 * @param nf  		 nom du fichier a creer 
 * @param obj  		 pointeur sur une structure libido
 * @return               code retour (booleen)
 *
 */

int IdAcrWriteListeAcr(char *nf,ACR_ELEMENT *header)

//char *nf;  /* nom du fichier a creer */
//ACR_ELEMENT *header;          liste d'ACR_ELEMENT, 
//				fabriquee par l'utilisateur
//				qui sera stockee sur disque 
			   
{
FILE *fp;

short int sh;
uint32_t i32;
ACR_ELEMENT *p;

int retval=1;

_IdAcrAdjustLength(header);

if (DEBUG) {	IdPrintf ("\n\nApres _IdAcrAdjustLength\n");
		 _IdAcrAffListe(header);
		 IdPrintf ("\n\n\n");
	   }

if((fp=fopen(nf,ID_WFILE_BIN))!=0)
	{
	p=header;
	while(p)
		{
		switch(p->type)
			{
			case BI:
				fwrite(p,8,1,fp);
				sh=(short int)p->value;
				fwrite(&sh,2,1,fp);

				if(DEBUG) IdPrintf(" BI element (en %p) %4x,%4x long.=%d valeur ",
							p, p->group,p->number,p->length);
				if (DEBUG) IdPrintf("%d\n",p->value);			
	
				break;
			case BD:
				fwrite(p,8,1,fp);
				i32=p->value; 
				fwrite(&i32,4,1,fp);
 
				if(DEBUG) IdPrintf(" BD  element (en %p) %4x,%4x long.=%d valeur ",
							p, p->group,p->number,p->length);
				if (DEBUG) IdPrintf("%d\n",p->value);
				break;
			case AT:
				fwrite(p,8,1,fp);
				fwrite((char *)(p->value),(size_t)p->length,1,fp);

				if(DEBUG) IdPrintf(" AT  element (en %p) %4x,%4x long.=%d valeur ",
							p, p->group,p->number,p->length);
				if (DEBUG) IdPrintf("%s\n",(char *)p->value);
				break;
			case AN:
				fwrite(p,8,1,fp);
				fwrite((char *)(p->value),(size_t)p->length,1,fp);

				if(DEBUG) IdPrintf(" AN  element (en %p) %4x,%4x long.=%d valeur ",
							p, p->group,p->number,p->length);
				if (DEBUG) IdPrintf("%s\n",(char *)p->value);

				break;
			case NO:
				fwrite(p,8,1,fp);
				fwrite((char *)(p->value),(size_t)p->length,1,fp);


				if(DEBUG) IdPrintf(" NO  element (en %p) %4x,%4x long.=%d valeur ",
							p, p->group,p->number,p->length);
				if (DEBUG) IdPrintf("%x\n",p->value);

/*

				switch (IdLibidoType(obj))
				{
				case SIG:
				  	fwrite(sip,
				    		IdSigDimX(sip)*IdSizeOfType(sip),1,fp);
					break;

				case CNT:
				  	fwrite(cop,
				    		IdCntDimX(cop)*IdSizeOfType(cop),1,fp);
					break;
				
				case IMA:
					for(i=0;i<IdImaDimY(imp);i++)
				  	fwrite(imp[i],
				    		IdImaDimX(imp)*IdSizeOfType(imp),1,fp);
					break;

				case VOL:
					for(j=0;j<IdVolDimZ(vop);j++)
				 	for(i=0;i<IdVolDimY(vop);i++)
				  	fwrite(vop[j][i],
				    		IdVolDimX(vop)*IdSizeOfType(vop),1,fp);
					break;

				case SEQ:
					for(k=0;k<IdSeqDimT(sep);k++)
					for(j=0;j<IdSeqDimZ(sep);j++)
				 	for(i=0;i<IdSeqDimY(sep);i++)
				  	fwrite(sep[k][j][i],
				    		IdVolDimX(sep)*IdSizeOfType(sep),1,fp);
					break;

				}
*/
			}
		p=p->suiv;
		}
	fclose(fp);
	}

else 	{
	IdErrno = IDERR_OPEN_FAILED;
	retval  = 0;
     	}

return(retval);
}
