/*************************************************************************
* $Id: dcmread.c,v 1.2 2005/09/09 08:52:37 bellet Exp $
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
**************************************************************************
*
*  Description : Lit un objet LIBIDO dans un fichier 
*		au format ACR-NEMA,LibIDO,DICOM
**************************************************************************
*/

#include <string.h>  // For strlen
#include <stdlib.h>
#include "idio.h"
//#include <idprint.h>
#include <stdio.h>
#include <stdlib.h>  // For atoi
#include "idgen.h"

#include "idsig.h"
#include "idcnt.h"
#include "idima.h"
#include "idvol.h"
#include "idseq.h"

#include "iderr.h"

#include "iddcm-restricted.h"
#include "iddcmjpeg.h"

#define DEBUG 0

static void _Swap(void* , int , int , int );


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


//static int rgb; // mais ca servait a QUOI qu'elle soit statique ???

// static int sw;

/* JPR, pour eviter de dupliquer le source    */

#define CCS(Type)							\
if(!fread(&((Type)psi)[0],IdSigDimX(psi)*IdSizeOfType(psi),1,e->fp))	\
									\
         {								\
            IdSigFree(psi);					\
            printf("DcmRead : echec en lecture Signal\n");\
	    return(0);						\
	 }

#define CCC(Type)							\
if(!fread(&((Type)pco)[0],IdCntDimX(pco)*IdSizeOfType(pco),1,e->fp))	\
									\
         {								\
            IdCntFree(pco);					\
            printf("DcmRead : echec en lecture Contour\n");	\
	    return(0);						\
	 }

/* pour les anciens codage de contour */
#define CCC2(Type)							\
if(!fread(&((Type)pco)[0],IdCntDimX(pco)*IdSizeOfType(pco)>>1,1,fp))\
									\
         {								\
            IdCntFree(pco);					\
            printf("DcmRead :echec en lecture Contour\n");	\
	    return(0);						\
	 }

#define CCI(Type,TailleType)													\
   {																		\
	if (vector != NULL) { /* on a une contrainte de rapidite */				\
		if(!fread(vector,(int)dim[0]*(int)dim[1]*TailleType,1,e->fp)){	\
				printf("DcmRead :echec en lecture Image dans Vecteur \n");		\
	    			return(0);												\
		}																\
	} else {																\
		for(i=0;i<IdImaDimY(pim);i++){									\
       			if(!fread(((Type)pim)[i],IdImaDimX(pim)*IdSizeOfType(pim),1,e->fp)) { \
            			printf("echec en lecture Image [i : %d] lgr de %d \n",  \
						i,IdImaDimX(pim)*IdSizeOfType(pim));			\
            			IdImaFree(pim);		  									\
	    			return(0);												\
	 		}															\
	 	}																\
	}																	\
   }																		\
													
#define CCV(Type)										\
	for(j=0;j<IdVolDimZ(pvo);j++){					\
	/*if (DEBUG) printf("DcmRead CCV: Lecture Plan %d\n", j); */	\
	  for(i=0;i<IdVolDimY(pvo);i++)					\
	   if(!fread(((Type)pvo)[j][i],IdVolDimX(pvo)*IdSizeOfType(pvo),1,e->fp)) \
	     {											\
              printf("DcmRead CCV : echec en lecture Volume\n");	    \
	      IdVolFree(pvo);							\
              if (DEBUG) printf("DcmRead CCV : Fin  Liberation Volume\n");\
	      return(0);								\
	      }											\
	     }											\
	 if (DEBUG) printf("DcmRead CCV : Fin Lecture Volume\n");

#define CCQ(Type)										\
	for(k=0;k<IdSeqDimT(pse);k++)					\
	for(j=0;j<IdSeqDimZ(pse);j++)					\
	  for(i=0;i<IdSeqDimY(pse);i++)					\
	   if(!fread(((Type)pse)[k][j][i],IdSeqDimX(pse)*IdSizeOfType(pse),1,e->fp))  \
	     {											\
              printf("DcmRead :echec en lecture Sequence\n");	\
               IdSeqFree(pse);							\
              if (DEBUG) printf("DcmRead :Fin  Liberation Sequence\n");\
	      return(0);								\
	     }


//-------------------------------------------------------------------------

//
// IdDcmReadFileIntoVector ( est un sur-ensemble de IdDcmReadFile )
//

/**
 * \brief		Lecture d'un fichier (IMAGE) ACR/LibIDO/DICOM dans un Vecteur existant deja
 *
 * 		A L'ATTENTION DES Kamikazes Creatissiens :
 * 		La fonction IdDcmReadFileIntoVector est une extention de IdDcmReadFile
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
 *		IdDcmReadFileIntoVector((nomImage[i],-1,-1,curPos=+tailleImage);
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

void *IdDcmReadFileIntoVector(char *filename, int type, int premIm, int dernIm, void * vector) {

ID_DCM_HDR *e;
void * o;

	 if (DEBUG) printf("----------------------------entree ds  IdDcmReadFileIntoVector\n");

	// -------------------------------------------------------------
	// On recupere les info 
	// -------------------------------------------------------------
	
	if((e=IdDcmGetHeader(filename))==0) {				// POUR LIRE IMAGES LEONARDO, DECOMMENTER

	//if((e=IdDcmGetHeaderLeonardo(filename))==0) {
		printf ("Gros soucis : %s\n",filename);
		return (NULL);
        }
	if (DEBUG)
		printf("DcmReadFileIntoVector : Sortie IdDcmGetHeader\n");

	o = IdDcmReadFileIntoVectorFromDcmHdr(e,type, premIm, dernIm, vector);
	IdDcmHdrFree(e);
	return (o);
}

 
//-------------------------------------------------------------------------
//
// IdDcmReadFileIntoVectorFromDcmHdr ( est un sur-ensemble de IdDcmReadFile )
//

/**
 * \brief		Lecture d'un fichier (IMAGE) ACR/DICOM dans un Vecteur existant deja
 *
 * 		A L'ATTENTION DES Kamikazes Creatissiens :
 * 		La fonction IdDcmReadFileIntoVector est une extention de IdDcmReadFile
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
 *		IdDcmReadFileIntoVector((nomImage[i],-1,-1,curPos=+tailleImage);
 *	}
 *	// vect pointe sur le vecteur des pixels de l'ensemble des images.
 *	// vect n'EST PAS un 'objet' LibIDO	
 *	// C'est a l'utilisateur de l'exploiter comme il lui convient
 *
 * @param e		ID_DCM_HDR deja cree
 * @param type		Type des images a lire (IMA_UCHAR,IMA_USHORT, ...).
 * @param premIm	Num premiere image a lire (Dicom multiframe uniquement; -1 sinon)
 *			(si -1 ou 0 on commence a la premiere).
 * @param dernIm	Num derniere image a lire a lire (Dicom multiframe uniquement; -1 sinon)
 *			(si -1 ou 0 on fini a la derniere).
 * @param vector    adresse dans le Vecteur, deja alloue, a laquelle sera rangée l'image lue
 * @return		Pointeur vers un tableau unidimentionel 
 * 			(celui passe en parametre; NULL si echec)
 */

void *IdDcmReadFileIntoVectorFromDcmHdr(ID_DCM_HDR *e, int type, int premIm, int dernIm, void * vector) {
	
	PSIGNAL      psi;
	PCONTOUR     pco;
	PPIMAGE      pim;
	PPPVOLUME    pvo;
	PPPPSEQUENCE pse;
	int i,j,k ;
  	int nbIm,imNum,nbTotIm = 0,nbFrames;
	unsigned short int ndim,nb,ty,nbu, highBit, SamplesPerPixel;
	unsigned short int dim[4],usedDim[4];
	long int np,lgrASauter;
	unsigned short int  typ, typObj = 0;
	char *bufbid;
	void ** tabIm = NULL;
	short int typePasse;
	unsigned short int mask5 = 0xffff;
	
// -------------------
	int JpegLossless;
	uint32_t ln;
//	ClbJpeg *jpg= NULL;
// --------------------
	
	psi=0;
	pco=0;
	pvo=0;
	pim=0;
	pse=0;
	typePasse=type;
	 if (DEBUG) printf("----------------------------entree ds  IdDcmReadFileIntoVectorFromDcmHdr\n");

	
	if (dernIm < premIm) {
		printf("num Derniere Image %d < num Premiere Image %d\n",
		         premIm,dernIm);
		return(0);
	}

	if (premIm==-1) 
		premIm=0;

	if (DEBUG)
		printf("pre %d der %d\n", premIm,dernIm);

	typ = -1;

	e->ACR_LIBIDO = IdDcmIsAcrLibido(e);

	bufbid=_IdDcmReadElement(0x0028,0x0004,e);

	if(bufbid!=NULL) { 			// RGB
		if (  (memcmp(bufbid,"RGB ",4)==0)    
     			||
      		      (memcmp(bufbid,"B RG",4)==0) 
     			||
      		      (memcmp(bufbid," BGR",4)==0)          
     			||
      		      (memcmp(bufbid,"GR B",4)==0)) {

						// rgb=1; // Mais elle servait a QUOI tout court ??

			printf ("Image RGB  ->%s<-\n", bufbid);
		}
	}

	if((ndim=_IdDcmReadElementUSHORT(0x0028,0x0005,e))==0xFFFF) {
		ndim=2;	// nb de dim de l'objet (IMA par defaut)
	}

	if ((ndim>4)||(ndim<1)) {
		printf ("nb de dim = %d, errone\n",ndim);
		return(0);
	} else {
		if (DEBUG) {
			printf ("\t\t\t ndim (0x0028,0x0005) : %d\n",ndim);
		}
	}
		
	// 10 : nb lignes   allouees
	// 11 : nb colonnes allouees
	// 12 : nb plans    alloues   (     DICOM, non ACR-NEMA )
	// 13 : nb blocs    alloues   ( non DICOM, non ACR-NEMA )
	// 15 : nb lignes   Utilisees ( non DICOM, non ACR-NEMA ) 
	// 16 : nb colonnes Utilisees ( non DICOM, non ACR-NEMA )
	// 17 : nb plans    Utilises  ( non DICOM, non ACR-NEMA )
	// 18 : nb blocs    Utilises  ( non DICOM, non ACR-NEMA )
					
	// Lecture nbFrames 
		
	if(!(bufbid=_IdDcmReadElement(0x0028,0x0008,e))) {
		nbFrames=1; // mono-frame par defaut
	} else {
		nbFrames=atoi(bufbid);
		if (DEBUG)
			printf ("\t\t\t nbFrames (0x0028,0x0008) : %d\n",nbFrames);
	}

	for(i=0;i<ndim;i++) {
			if((dim[i]=_IdDcmReadElementUSHORT(0x0028,0x0010+i,e))==0xFFFF) { 
				IdErrno=IDERR_NON_ACR_FILE;
				printf ("IdDcmReadFile : fichier non ACR (0x0028,0x00%d) "
					"missing\n",10+i);
				return (0);
			}  else {
				if (DEBUG) 
					printf ("\t\t\t ndim (0x0028,0x0005) : dim[%d]=%d\n",i,dim[i]);
			}
		}

	if (e->ACR_LIBIDO!=1) {
		// si c'est une VRAIE image ACR
		// on permutte NX et NY ds l'entete LibIDO
		int secuDim;
		//printf("on permutte NX et NY ds l'entete LibIDO %d \n",e->ACR_LIBIDO);

		secuDim = dim[0];
		dim[0]  = dim[1];
		dim[1]  = secuDim;
	} else {
		//printf("on ne permutte PAS NX et NY ds l'entete LibIDO\n");

		// Lecture UsedNbX, ... (Uniquement pour ACR LibIDO ...)
		
		for(i=0;i<ndim;i++)
			if((usedDim[i]=_IdDcmReadElementUSHORT(0x0028,0x0015+i,e))==0xFFFF)
				usedDim[i]=dim[i];
		if (DEBUG)
			for(i=0;i<ndim;i++)
				printf ("\t\t\t dim[%d]=%d\n",i,usedDim[i]); 
	}

	if((nb=_IdDcmReadElementUSHORT(0x0028,0x0100,e))==0xFFFF) {  // nb bits allocated
		nb=16 ; 
    	} else {
		if (DEBUG) {
			printf ("\t\t\t nb (0x0028,0x0100) : %d\n",nb);
		}
	}

	if((nbu=_IdDcmReadElementUSHORT(0x0028,0x0101,e))==0xFFFF) { // nb bits utiles
		nbu = nb ; 
	} else {
		if (DEBUG) {
			printf ("\t\t\t nbu (0x0028,0x0101) : %d\n",nbu);
		}
        }

	if((highBit=_IdDcmReadElementUSHORT(0x0028,0x0102,e))==0xFFFF) {
		// hight bit pas pris en compte
		highBit = nb-1 ;
	} else {
		if (DEBUG) {
			printf ("\t\t\t highBit (0x0028,0x0102) : %d\n",highBit);
		}
        }
      
	mask5 = mask5 >> (nb-nbu);
	if((ty=_IdDcmReadElementUSHORT(0x0028,0x0103,e))==0xFFFF) { // signe o/n
		IdErrno=IDERR_NON_ACR_FILE;
		printf ("fichier non ACR (0x0028,0x0103) missing\n");
		return (0);
	} else {
		if (DEBUG) {
			printf ("\t\t\t signe (0x0028,0x0103) : %d\n",ty);
	        }
	}

	if (DEBUG) {
		printf("nbit alloues=%d nbit Utiles=%d highbit %d signe=%d\n",
			 nb,nbu,highBit,ty);
		for(i=0;i<ndim;i++)
			printf(" dim[%d]= %d; ", i, dim[i]);
		printf ("\n");
	}

	if (nb==12) {
		printf("No luck for you ... nbit=12!...  Try  : "
			    "	AFFIM BYPASSACR=1 NBIT=12 DIMX=%d DIMY=%d OFFSET= ...\n",
				dim[0],dim[1]);
		nb=8;
		IdErrno=IDERR_NON_ACR_FILE;
		return (0);
	}

	if(ndim==1) typObj=SIG;
	// pas de Pb pour les CNT: _IdAcrCheckType trouvera FORCEMENT
	// l'element (28,199) !
	if(ndim==2) typObj=IMA;
	if(ndim==3) typObj=VOL;
	if(ndim==4) typObj=SEQ;
	
	// Lecture SamplesPerPixel (1 : Gray Scale, 3 : RGB)
	if((SamplesPerPixel=_IdDcmReadElementUSHORT(0x0028,0x0002,e))==0xFFFF) {
		SamplesPerPixel=1;	// (Gray Scale par defaut)
	}

	// on essaye, a titre d'info, de verifier le type, ds le fichier
	if (DEBUG)printf ("appel = IdAcrCheckType\n");
	
	typ=_IdDcmCheckType(e,ty,nb,typObj, SamplesPerPixel);
		if (DEBUG)printf ("Sortie = IdAcrCheckType\n");

	if(ndim==1) typObj = typ & IDO_TYPE; // Les CNT ont AUSSI ndim=1 !...

	if (typ!=typePasse)
		printf ("typeTrouve= %x typePasse= %x\n", typ, typePasse);

	typ=typePasse;  // Si ca ne cadre pas, on considere que la valeur
				// passee est la bonne
         
	np= e->PixelPosition;

	if((e->fp=fopen(e->filename,ID_RFILE_BIN))==0) {
		printf ("echec ouverture  %s\n",e->filename);
		return (0);
        }
 	fseek(e->fp,np,SEEK_CUR);	// On se positionne sur le début présumé des Pixels 
	
	typObj=type&0x000f;  	// Recomputed from result of _IdAcrCheckType
	type  =type&0xfff0;


	if ( premIm != -2) { 	// on n'est pas ds MultiFrame
		lgrASauter = IdTypeSize(type);

		for(i=0;i<ndim;i++)
			lgrASauter = lgrASauter * dim[i];	// taille d'un Frame

		lgrASauter = lgrASauter * premIm;		// si on veut ignorer les Frames de debut
		nbTotIm=nbFrames;
	}

	if (dernIm==-1)
		dernIm=nbTotIm-1;

	nbIm = dernIm - premIm +1;
		
	if ( (JpegLossless=IdDcmIsJpegLossless(e)) ) {		// si Jpeg Lossless, on ne tente pas d'aventure
	/*
		premIm = 1;
		dernIm=nbTotIm-1;
		nbIm = dernIm - premIm +1;
		lgrASauter = 0;
	*/
	}
		
		

	//-------------------------------------------------------------
	// Fin recup. info pour pouvoir Allouer
	//-------------------------------------------------------------

	if (vector == NULL) {
		tabIm = (void **) calloc((nbIm+1),sizeof(void *));
		if(!tabIm) {
			printf("Echec Alloc Table d Images : premIm %d dernIm %d nbIm %d\n",
					 premIm, dernIm,nbIm);
			return NULL;
		}
	}
	if (DEBUG) printf("OK Alloc Table d Images\n");
 
	//-------------------------------------------------------------
	// Pour CHAQUE 'Objet' du Fichier
	//-------------------------------------------------------------
	for(imNum=premIm;imNum<dernIm+1;imNum++) {
		if (DEBUG)
			printf("imNum %d, typObj %x\n",imNum,typObj);
    		switch(typObj) {

		case SIG:
		if (DEBUG)
			printf("SIGNAL\n");

			if(0==(((PSIGNAL *)tabIm)[imNum-premIm]=psi
			   =IdSigAlloc((int)dim[0],typ))) {
				printf("echec alloc Signal\n");
				return(0);
			}
			switch(type) { // 4 derniers bits : SIG, IMA, VOL, SEQ
			case TY_UCHAR:
				CCS(PSIGNAL_UCHAR);
				break;
			case TY_CHAR:
				CCS(PSIGNAL_CHAR);
				break;
			case TY_USHORT:
				CCS(PSIGNAL_USHORT);
				if(e->sw) _Swap(psi,21,(int)dim[0],TY_USHORT);
				break;
			case TY_SHORT:
				CCS(PSIGNAL_SHORT);
				if(e->sw) _Swap(psi,21,(int)dim[0],TY_SHORT);
				break;
			case TY_LONG:
				CCS(PSIGNAL_LONG);
                		if(e->sw) _Swap(psi,e->sw,(int)dim[0],TY_LONG);
   				break;
			case TY_ULONG:
				CCS(PSIGNAL_ULONG);
                		if(e->sw) _Swap(psi,e->sw,(int)dim[0],TY_ULONG);
   				break;
			case TY_FLOAT:
				CCS(PSIGNAL_FLOAT);
				break;
			case TY_DOUBLE:
				CCS(PSIGNAL_DOUBLE);
				break;
  			case TY_RGB:
				CCS(PSIGNAL_RGB);
				break;
  					
			// revoir le pb du Swap des complexes....
			case TY_COMPLEX_UCHAR:
				CCS(PSIGNAL_COMPLEX_UCHAR);
				break;
			case TY_COMPLEX_CHAR:
				CCS(PSIGNAL_COMPLEX_CHAR);
				break;
			case TY_COMPLEX_USHORT:
				CCS(PSIGNAL_COMPLEX_USHORT);
				if(e->sw) _Swap(psi,21,(int)dim[0],TY_COMPLEX_USHORT);
				break;
			case TY_COMPLEX_SHORT:
				CCS(PSIGNAL_COMPLEX_SHORT);
				if(e->sw) _Swap(psi,21,(int)dim[0],TY_COMPLEX_SHORT);
				break;
			case TY_COMPLEX_LONG:
				CCS(PSIGNAL_COMPLEX_LONG);
				if(e->sw) _Swap(psi,e->sw,(int)dim[0],TY_COMPLEX_LONG);
   				break;
			case TY_COMPLEX_ULONG:
				CCS(PSIGNAL_COMPLEX_ULONG);
                		if(e->sw) _Swap(psi,e->sw,(int)dim[0],TY_COMPLEX_ULONG);
   				break;
			case TY_COMPLEX:
				CCS(PSIGNAL_COMPLEX);
				break;	
			case TY_COMPLEX_FLOAT:
				CCS(PSIGNAL_COMPLEX_FLOAT);
				break;	
			case TY_COMPLEX_DOUBLE:
				CCS(PSIGNAL_COMPLEX_DOUBLE);
				break;	
			} // end switch(type)
		
			_IdSigPrivate(psi)->_fichier=
				(char *)malloc(strlen(e->filename)+1);
			if(premIm == -2)
				strcpy(_IdImaPrivate(psi)->_fichier,e->filename);
			else
				sprintf(_IdImaPrivate(psi)->_fichier,"%s[%d]",
					e->filename,imNum);

			_IdSigPrivate(psi)->_message=
				(char *)malloc(strlen(e->_message)+1);
			strcpy(_IdSigPrivate(psi)->_message,e->_message);
			IdSigSetUsedNbX(psi,usedDim[0]);
			break;

		case CNT:
		if (DEBUG)
			printf("CONTOUR\n");

	if(0==(((PCONTOUR *)tabIm)[imNum-premIm]=pco=IdCntAlloc((int)dim[0],typ)))

	         { printf("echec alloc Contour\n");
		   return(0);
		  }

		switch(type)   /* 4 derniers bits : SIG, CNT, IMA, VOL, SEQ */
		{

		case TY_UCHAR:  CCC(PCONTOUR_UCHAR);  	break;
		case TY_CHAR:   CCC(PCONTOUR_CHAR);   	break;
		case TY_USHORT: CCC(PCONTOUR_USHORT);
			if(e->sw)_Swap(pco,e->sw,dim[0],TY_USHORT);		/* VOIR le Pb du SWAP */
							break;
		case TY_SHORT:  CCC(PCONTOUR_SHORT);
                         if(e->sw)_Swap(pco,e->sw,dim[0],TY_SHORT);
							break;
		case TY_LONG:   CCC(PCONTOUR_LONG);
                		if(e->sw)_Swap(pco,e->sw,dim[0],TY_LONG);
   							break;
		case TY_ULONG:   CCC(PCONTOUR_ULONG);
                		if(e->sw)_Swap(pco,e->sw,dim[0],TY_ULONG);
   							break;
 
		case TY_FLOAT:  CCC(PCONTOUR_FLOAT);  	break;
		case TY_DOUBLE: CCC(PCONTOUR_DOUBLE); 	break;

              }	
		
		_IdCntPrivate(pco)->_fichier=(char *)malloc(strlen(e->filename)+1);
	        if(premIm == -2)	strcpy(_IdImaPrivate(pco)->_fichier,e->filename);
		else			sprintf(_IdImaPrivate(pco)->_fichier,"%s[%d]",e->filename,imNum);

		_IdCntPrivate(pco)->_message=(char *)malloc(strlen(e->_message)+1);
		strcpy(_IdCntPrivate(pco)->_message,e->_message);

		IdCntSetUsedNbX(pco,usedDim[0]);
		break;

      case IMA:
	if (DEBUG)
		printf("IMAGE %d %d\n",(int)dim[0],(int)dim[1]);

	if(vector==NULL) {
		if(0==(((PPIMAGE *)tabIm)[imNum-premIm]=
				pim=IdImaAlloc((int)dim[0],(int)dim[1],typ))) { 
			printf("echec alloc Image \n");
	         	return(0);
		}
		if (DEBUG)
				printf("Succes alloc Image nX %d nY %d \n",IdImaDimX(pim),IdImaDimY(pim));
	}

	switch(type)
	      {
		case TY_UCHAR:  CCI(PPIMAGE_UCHAR, TA_TY_UCHAR);  		
						break;

		case TY_CHAR:   CCI(PPIMAGE_CHAR, TA_TY_CHAR);
						break;
	
		case TY_USHORT: 
		
			if(e->__TrueDicom && !IdDcmIsUncompressed(e)) {
				
			   	// skip to the beggining of the pixels
			   	
			      fseek(e->fp,4,SEEK_CUR);
			      fread(&ln,4,1,e->fp); 
			      if(e->sw) 
			      ln=_IdDcmSWAP_LONG(ln,e->sw);
				//if (DEBUG) 
			      printf ("ln %d\n",ln);
			      fseek(e->fp,ln,SEEK_CUR);
			      fseek(e->fp,4,SEEK_CUR);
			      fread(&ln,4,1,e->fp); 
			      if(e->sw) 
				 ln=_IdDcmSWAP_LONG(ln,e->sw);
				//if (DEBUG) 
			      printf ("ln image comprimée %d\n",ln);
			      
			      
			      printf("Sorry, Bits Stored = %d not yet taken into account\n",nbu);
			      return NULL;
			      
			} else { // Old ACR-NEMA or uncompressed
		
				CCI(PPIMAGE_USHORT, TA_TY_USHORT);
                               	if(e->sw) {
                                	if(DEBUG) printf("_Swap\n");

					_Swap(pim[0],e->sw,(int)dim[0]*(int)dim[1],TY_USHORT);
					  if (DEBUG) printf("\t\t\tOn swappe\n");
				} // end if(e->sw)
                  		if (nbu!=nb)  /* GB */
					for(i=0;i<IdImaDimY(pim);i++)
					for(j=0;j<IdImaDimX(pim);j++)
						((PPIMAGE_USHORT)pim)[i][j] = 
							(((PPIMAGE_USHORT)pim)[i][j] >> (nbu-highBit-1)) & mask5;
			} // end else Old ACR-NEMA
			
				
			break;

		case TY_SHORT:  CCI(PPIMAGE_SHORT,TA_TY_SHORT); // les 16 bits 
						if(e->sw)_Swap(pim[0],21,dim[0]*dim[1],TY_SHORT);
 						break;

		case TY_ULONG:   CCI(PPIMAGE_ULONG,TA_TY_ULONG);
						if(e->sw)_Swap(pim[0],e->sw,dim[0]*dim[1],TY_ULONG);
 						break;

		case TY_LONG:   CCI(PPIMAGE_LONG,TA_TY_LONG);
						if(e->sw)_Swap(pim[0],e->sw,dim[0]*dim[1],TY_LONG);
 						break;
			
		case TY_FLOAT:  CCI(PPIMAGE_FLOAT,TA_TY_FLOAT); 	
						break;

		case TY_DOUBLE: CCI(PPIMAGE_DOUBLE,TA_TY_DOUBLE); 	
						break;

		case TY_RGB:    CCI(PPIMAGE_RGB,TA_TY_RGB);    								
						break;
		
		case TY_COMPLEX_UCHAR:  CCI(PPIMAGE_COMPLEX_UCHAR,TA_TY_COMPLEX_UCHAR);  	
						break;

		case TY_COMPLEX_CHAR:   CCI(PPIMAGE_COMPLEX_CHAR,TA_TY_COMPLEX_CHAR);   	
						break;
	
		case TY_COMPLEX_USHORT: CCI(PPIMAGE_COMPLEX_USHORT, TA_TY_COMPLEX_USHORT); 
                                if(e->sw)_Swap(pim[0],21,dim[0]*dim[1],TY_COMPLEX_USHORT);
						break;
	
		case TY_COMPLEX_SHORT:  CCI(PPIMAGE_COMPLEX_SHORT, TA_TY_COMPLEX_SHORT); 
						if(e->sw)_Swap(pim[0],21,dim[0]*dim[1],TY_COMPLEX_SHORT);
 						break;

		case TY_COMPLEX_ULONG:   CCI(PPIMAGE_COMPLEX_ULONG, TA_TY_COMPLEX_ULONG);
						if(e->sw)_Swap(pim[0],e->sw,dim[0]*dim[1],TY_COMPLEX_ULONG);
 						break;

		case TY_COMPLEX_LONG:   CCI(PPIMAGE_COMPLEX_LONG, TA_TY_COMPLEX_LONG);
						if(e->sw)_Swap(pim[0],e->sw,dim[0]*dim[1],TY_COMPLEX_LONG);
 						break;

		case TY_COMPLEX_FLOAT:  CCI(PPIMAGE_COMPLEX_FLOAT, TA_TY_COMPLEX_FLOAT); 	
						break;

		case TY_COMPLEX_DOUBLE: CCI(PPIMAGE_COMPLEX_DOUBLE, TA_TY_COMPLEX_DOUBLE); 	
						break;
		
               }					      /* end switch Type*/
	
		if (vector == NULL) {
			_IdImaPrivate(pim)->_fichier=(char *)malloc(strlen(e->filename)+1);
	       	 	if(premIm == -2)	strcpy (_IdImaPrivate(pim)->_fichier,e->filename);
				else			sprintf(_IdImaPrivate(pim)->_fichier,"%s[%d]",e->filename,imNum);

			_IdImaPrivate(pim)->_message=(char *)malloc(strlen(e->_message)+1);
			strcpy(_IdImaPrivate(pim)->_message,e->_message);

			IdImaSetUsedNbX(pim,usedDim[0]);
			IdImaSetUsedNbY(pim,usedDim[1]);
		}
		if (DEBUG) printf("sortie swich case IMA\n");

      		break;

      case VOL:
		if (DEBUG)
			printf("VOLUME\n");

	/*if(0==(pvo=IdVolAlloc((int)dim[0],(int)dim[1],(int)dim[2],typ)))*/
	
	if(0==(((PPPVOLUME *)tabIm)[imNum-premIm] = pvo = 
				IdVolAlloc((int)dim[0],(int)dim[1],(int)dim[2],typ)))
	      {  printf ("echec alloc Volume\n");
		return(0);
	       } else {
		if (DEBUG)
			printf("Succes Alloc VOLUME %d %d %d \n",(int)dim[0],(int)dim[1],(int)dim[2]);
	       }
	       
     	    switch(type) {
		case TY_UCHAR:  CCV(PPPVOLUME_UCHAR);  	break;
		case TY_CHAR:   CCV(PPPVOLUME_CHAR);   	break;	
		case TY_USHORT: CCV(PPPVOLUME_USHORT);
                               	if(e->sw)_Swap(pvo[0][0],21,dim[0]*dim[1]*dim[2],TY_USHORT);
 							break;	
		case TY_SHORT:  CCV(PPPVOLUME_SHORT); 
                               	if(e->sw)_Swap(pvo[0][0],21,dim[0]*dim[1]*dim[2],TY_SHORT);
 							break;
		case TY_ULONG:   CCV(PPPVOLUME_ULONG);
      				if(e->sw)_Swap(pvo[0][0],e->sw,dim[0]*dim[1]*dim[2],TY_ULONG);
      							break;
  		case TY_LONG:   CCV(PPPVOLUME_LONG);
      				if(e->sw)_Swap(pvo[0][0],e->sw,dim[0]*dim[1]*dim[2],TY_LONG);
 							break;
		case TY_FLOAT:  CCV(PPPVOLUME_FLOAT);  	break;	
		case TY_DOUBLE: CCV(PPPVOLUME_DOUBLE); 	break;	

		case TY_RGB:    CCV(PPPVOLUME_RGB);    	break;	
		
		case TY_COMPLEX_UCHAR:  CCV(PPPVOLUME_COMPLEX_UCHAR);  	break;
		case TY_COMPLEX_CHAR:   CCV(PPPVOLUME_COMPLEX_CHAR);   	break;	
		case TY_COMPLEX_USHORT: CCV(PPPVOLUME_COMPLEX_USHORT);
                               	if(e->sw)_Swap(pvo[0][0],21,dim[0]*dim[1]*dim[2],TY_COMPLEX_USHORT);
 							break;	
		case TY_COMPLEX_SHORT:  CCV(PPPVOLUME_COMPLEX_SHORT); 
                               	if(e->sw)_Swap(pvo[0][0],21,dim[0]*dim[1]*dim[2],TY_COMPLEX_SHORT);
 							break;	
		case TY_COMPLEX_ULONG:   CCV(PPPVOLUME_COMPLEX_ULONG);
      				if(e->sw)_Swap(pvo[0][0],e->sw,dim[0]*dim[1]*dim[2],TY_COMPLEX_ULONG);
      							break;
  		case TY_COMPLEX_LONG:   CCV(PPPVOLUME_COMPLEX_LONG);
      				if(e->sw)_Swap(pvo[0][0],e->sw,dim[0]*dim[1]*dim[2],TY_COMPLEX_LONG);
 							break;
		case TY_COMPLEX	     :  CCV(PPPVOLUME_COMPLEX);  	break;	
		case TY_COMPLEX_FLOAT:  CCV(PPPVOLUME_COMPLEX_FLOAT);  	break;	
		case TY_COMPLEX_DOUBLE: CCV(PPPVOLUME_COMPLEX_DOUBLE); 	break;	
	   }

	   if (DEBUG) printf("fin switch\n");
	   if (DEBUG) printf(" lgr champ 'fichier' %d\n",strlen(e->filename)+1 );
	    _IdVolPrivate(pvo)->_fichier=(char *)malloc(strlen(e->filename)+1);
	    
	    if(!(_IdVolPrivate(pvo)->_fichier ) ) {
	    	printf("DcmRead : Echec Alloc champ 'nom fichier'\n");
	    }
	    if(premIm == -2) strcpy(_IdImaPrivate(pvo)->_fichier,e->filename);
	    else			 sprintf(_IdImaPrivate(pvo)->_fichier,"%s[%d]",e->filename,imNum);

	    _IdVolPrivate(pvo)->_message=(char *)malloc(strlen(e->_message)+1);
	    if(!(_IdVolPrivate(pvo)->_message ) ) {
	    	printf("DcmRead : Echec Alloc champ 'message'\n");
	    }	    
	    strcpy(_IdVolPrivate(pvo)->_message,e->_message);
	    IdVolSetUsedNbX(pvo,usedDim[0]);
	    IdVolSetUsedNbY(pvo,usedDim[1]);
	    IdVolSetUsedNbZ(pvo,usedDim[2]);
	    
	    if (DEBUG) printf("fin lect VOLUME\n");
	    
            break;

      case SEQ:
		if (DEBUG)
			printf("SEQUENCE\n");

	if(0==(((PPPPSEQUENCE * )tabIm)[imNum-premIm]=pse=IdSeqAlloc((int)dim[0],(int)dim[1],(int)dim[2],(int)dim[3],typ)))
		if(0==(pse=IdSeqAlloc((int)dim[0],(int)dim[1],(int)dim[2],(int)dim[3],typ))) {  
			printf ("echec alloc Sequence\n");
			return(0);
	       	}
     	switch(type)
	      {
		case TY_UCHAR:  CCQ(PPPPSEQUENCE_UCHAR);  	break;
		case TY_CHAR:   CCQ(PPPPSEQUENCE_CHAR);   	break;	
		case TY_USHORT: CCQ(PPPPSEQUENCE_USHORT);
                               	if(e->sw)_Swap(pse[0][0][0],21,dim[0]*dim[1]*dim[2]*dim[3],TY_USHORT);
 								break;	
		case TY_SHORT:  CCQ(PPPPSEQUENCE_SHORT); 
                               	if(e->sw)_Swap(pse[0][0][0],21,dim[0]*dim[1]*dim[2]*dim[3],TY_SHORT);
 								break;
		case TY_ULONG:   CCQ(PPPPSEQUENCE_ULONG);
      				if(e->sw)_Swap(pse[0][0][0],e->sw,dim[0]*dim[1]*dim[2]*dim[3],TY_ULONG);
      								break;
  		case TY_LONG:   CCQ(PPPPSEQUENCE_LONG);
      				if(e->sw)_Swap(pse[0][0][0],e->sw,dim[0]*dim[1]*dim[2]*dim[3],TY_LONG);
 								break;
		case TY_FLOAT:  CCQ(PPPPSEQUENCE_FLOAT);  	break;	
		case TY_DOUBLE: CCQ(PPPPSEQUENCE_DOUBLE); 	break;	

		case TY_RGB:    CCQ(PPPPSEQUENCE_RGB);    	break;	
		
		case TY_COMPLEX_UCHAR:  CCQ(PPPPSEQUENCE_COMPLEX_UCHAR);  	break;
		case TY_COMPLEX_CHAR:   CCQ(PPPPSEQUENCE_COMPLEX_CHAR);   	break;	
		case TY_COMPLEX_USHORT: CCQ(PPPPSEQUENCE_COMPLEX_USHORT);
                               	if(e->sw)_Swap(pse[0][0][0],21,dim[0]*dim[1]*dim[2]*dim[3],TY_COMPLEX_USHORT);
 										break;	
		case TY_COMPLEX_SHORT:  CCQ(PPPPSEQUENCE_COMPLEX_SHORT); 
                               	if(e->sw)_Swap(pse[0][0][0],21,dim[0]*dim[1]*dim[2]*dim[3],TY_COMPLEX_SHORT);
 										break;	
		case TY_COMPLEX_ULONG:   CCQ(PPPPSEQUENCE_COMPLEX_ULONG);
      				if(e->sw)_Swap(pse[0][0][0],e->sw,dim[0]*dim[1]*dim[2]*dim[3],TY_COMPLEX_ULONG);
      										break;
  		case TY_COMPLEX_LONG:   CCQ(PPPPSEQUENCE_COMPLEX_LONG);
      				if(e->sw)_Swap(pse[0][0][0],e->sw,dim[0]*dim[1]*dim[2]*dim[3],TY_COMPLEX_LONG);
 										break;
		case TY_COMPLEX	     :  CCQ(PPPPSEQUENCE_COMPLEX);  	  	break;	
		case TY_COMPLEX_FLOAT:  CCQ(PPPPSEQUENCE_COMPLEX_FLOAT);  	break;	
		case TY_COMPLEX_DOUBLE: CCQ(PPPPSEQUENCE_COMPLEX_DOUBLE); 	break;	

	               }

		_IdSeqPrivate(pse)->_fichier=(char *)malloc(strlen(e->filename)+1);
	        if(premIm == -2)	strcpy(_IdImaPrivate(pse)->_fichier,e->filename);
		else			sprintf(_IdImaPrivate(pse)->_fichier,"%s[%d]",e->filename,imNum);

		_IdSeqPrivate(pse)->_message=(char *)malloc(strlen(e->_message)+1);
		strcpy(_IdSeqPrivate(pse)->_message,e->_message);

		IdSeqSetUsedNbX(pse,usedDim[0]);
		IdSeqSetUsedNbY(pse,usedDim[1]);
		IdSeqSetUsedNbZ(pse,usedDim[2]);
		IdSeqSetUsedNbT(pse,usedDim[3]);
      		break;

    } 								/* end switch typObj */

 }	 /* end for : On a parcouru tous les 'objets' du fichier	*/

    fclose(e->fp);
    return tabIm;
}


/* ----------------------------------------------------------------------- */
/**
 * \brief		Lecture d'un fichier de type ACR NEMA ou DICOM 
* 			
* @param filename	Nom du fichier ACR-NEMA ou DICOM SINGLE-FRAME
* @param type		Type de l'object a lire (IMA_UCHAR, SIG_CHAR, ...).
* @return		Pointeur vers l'objet lu 
*				( PSIGNAL, PPIMAGE, PPPVOLUME, PPPPSEQUENCE..).  
*/


void *IdDcmReadFile(char * filename, int type) {
void ** tabObj=NULL;

	 if (DEBUG) printf("----------------------------entree ds  IdDcmReadFile\n");

tabObj=IdDcmReadFileIntoVector(filename, type,-2, -2, NULL);

if (tabObj==NULL) 
	return (NULL);
else
	return(tabObj[0]);
}


/* ----------------------------------------------------------------------- */
/**
 * \brief		Lecture d'un fichier de type ACR NEMA ou DICOM 
* 			
* @param e		ID_DCM_HDR *, deja cree, du fichier ACR-NEMA ou DICOM SINGLE-FRAME
* @param type		Type de l'object a lire (IMA_UCHAR, SIG_CHAR, ...).
* @return		Pointeur vers l'objet lu 
*				( PSIGNAL, PPIMAGE, PPPVOLUME, PPPPSEQUENCE..).  
*/

void *IdDcmReadFileFromDcmHdr(ID_DCM_HDR *e, int type) {
void ** tabObj=NULL;

	 if (DEBUG) printf("----------------------------entree ds  IdDcmReadFileFromDcmHdr\n");

tabObj=IdDcmReadFileIntoVectorFromDcmHdr(e, type,-2, -2, NULL);

if (tabObj==NULL) 
	return (NULL);
else
	return(tabObj[0]);
}

//-------------------------------------------------------------------------
//
// IdDcmMultiFrameReadFile est une encapsulation de IdDcmReadFileIntoVector
//

/**
 * \brief		Lecture d'un fichier de type ACR/LibIDO/DICOM dans un vecteur existant deja
 * 			La fonction IdDcmMultiFrameReadFile est une extention de IdDcmReadFile
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



void *IdDcmMultiFrameReadFile(char * filename, int type, int numPremIm, int numDernIm) {
void ** tabObj=NULL;

	 if (DEBUG) printf("----------------------------entree ds  IdDcmMultiFrameReadFile\n");

tabObj=IdDcmReadFileIntoVector(filename,type, numPremIm, numDernIm, NULL);

if (tabObj==NULL) 
	return (NULL);
else
	return(tabObj[0]);
}

// -------------------------------------------------------------------------------

void *IdDcmMultiFrameReadFileFromDcmHdr(ID_DCM_HDR *e, int type, int numPremIm, int numDernIm) {
void ** tabObj=NULL;

	 if (DEBUG) printf("----------------------------entree ds  IdDcmMultiFrameReadFileFromDcmHdr\n");

tabObj=IdDcmReadFileIntoVectorFromDcmHdr(e,type, numPremIm, numDernIm, NULL);

if (tabObj==NULL) 
	return (NULL);
else
	return(tabObj[0]);
}


/* ------------------------------------------------------------------------------------ */

int IdDcmIsAcrLibido(ID_DCM_HDR *e) {

	return(e->ACR_LIBIDO);
}



// ------------------------------------------------------------------------------------


/**
 * \ingroup       acr
 * \brief         localise un dicom_element dans un ID_DCM_HDR
 *            	(le ID_DCM_HDR doit deja avoir ete cree,
 *		  et _IdDcmHdrCheckSwap(ID_DCM_HDR  *e) avoir ete appelle)
 * @param gr  partie 'group' du Tag du dicom_element a lire.
 * @param num partie 'elem'  du Tag du dicom_element a lire.
 * @param e      ID_DCM_HDR dans lequel effectuer la recherche.
 * @return        Adresse memoire de l'element localisé (NULL si pas trouve)
 */

char * _IdDcmReadElement(int gr, int num, ID_DCM_HDR  *e) {

_ID_DCM_ELEM *ple;	
PLIST_ELEMENT plelem; 
plelem = IdLstFirst(e->plist);

if (DEBUG)  printf("\t\t\t\t\trecherche (1) gr %x  num %x\n",gr,num); 

	while(plelem) {
		ple= IdLstPtrObj(plelem);	// le _ID_DCM_ELEM  pointé par le PLIST_ELEMENT

		if (ple->Gr == gr) break;
		if (ple->Gr > gr) {
			return(NULL);
		}
		plelem = IdLstNext(plelem);
	}
	while(plelem) {
		ple= IdLstPtrObj(plelem);	// le _ID_DCM_ELEM  pointé par le PLIST_ELEMENT

		if(ple->Num == num) break;
		if(ple->Num > num) {
			return(NULL);
		}
		plelem = IdLstNext(plelem);
	}
	
	return(ple->valeurElem);
}

// ------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------


/**
 * \ingroup       acr
 * \brief         localise un dicom_element dans un ID_DCM_HDR
 *		en ne tenant pas compte des ruptures de séquence
 		-dues aux SQ- dans les Dicom Tags
 *            	(le ID_DCM_HDR doit deja avoir ete cree,
 *		  et _IdDcmHdrCheckSwap(ID_DCM_HDR  *e) avoir ete appelle)
 * @param gr  partie 'group' du Tag du dicom_element a lire.
 * @param num partie 'elem'  du Tag du dicom_element a lire.
 * @param e      ID_DCM_HDR dans lequel effectuer la recherche.
 * @return        Adresse memoire de l'element localisé (NULL si pas trouve)
 */

char * _IdDcmReadElementNoSQ(int gr, int num, ID_DCM_HDR  *e) {

_ID_DCM_ELEM *ple;	
PLIST_ELEMENT plelem; 
int trouve = 0;

plelem = IdLstFirst(e->plist);

if (DEBUG)  printf("\t\t\t\t\trecherche IdDcmReadElementNoSQ (1) gr %x  num %x\n",gr,num); 

	while(plelem) {
		ple= IdLstPtrObj(plelem);	// le _ID_DCM_ELEM  pointé par le PLIST_ELEMENT

		if (ple->Gr == gr) {
			trouve =1;
			break;
		}
		plelem = IdLstNext(plelem);
	}
	
	if (trouve ==0) return (NULL);
	
	trouve = 0;
	
	while(plelem) {
		ple= IdLstPtrObj(plelem);	// le _ID_DCM_ELEM  pointé par le PLIST_ELEMENT

		if(ple->Num == num)  {
			trouve =1;
			break;
		}
		plelem = IdLstNext(plelem);
	}
	
	if (trouve ==0) {
		return (NULL);
	 } else {
		return(ple->valeurElem);
	}
}

// ------------------------------------------------------------------------------------

/**
 * \ingroup       acr
 * \brief         localise un dicom_element dans un ID_DCM_HDR
 *            	(le ID_DCM_HDR doit deja avoir ete cree,
 *		  et _IdDcmHdrCheckSwap(ID_DCM_HDR  *e) avoir ete appelle)
 * @param gr  partie 'group' du Tag du dicom_element a lire.
 * @param num partie 'elem'  du Tag du dicom_element a lire.
 * @param e      ID_DCM_HDR dans lequel effectuer la recherche.
 * @return        valeur de l'element localisé (0xFFFF si pas trouve 
 * 		- cette valeur ne peut pas exister ds une entete -
 */
unsigned short _IdDcmReadElementUSHORT(int gr, int num, ID_DCM_HDR  *e) {

_ID_DCM_ELEM *ple;
PLIST_ELEMENT plelem; 

if(!e) {
	printf("DCM_HDR non alloué\n");
	return(0xFFFF);
}

plelem = IdLstFirst(e->plist);
if (!plelem) {
	printf("ID_DCM_HDR : Liste vide\n");
	return(0xFFFF);
}
	
if (DEBUG)  printf("\t\t\t_IdDcmReadElementUSHORT\t\trecherche (1) gr %x  num %x\n",gr,num); 

	while(plelem) {
		ple= IdLstPtrObj(plelem);	// le _ID_DCM_ELEM  pointé par le PLIST_ELEMENT
		
		//if (DEBUG) printf(" on est sur gr %04x \n",ple->Gr);

		if (ple->Gr == gr) break;
		if ( (ple->Gr >  gr) && (ple->Gr !=0xFFFE) ) {
			if (DEBUG) printf(" gr %04x trouve\n",ple->Gr);
			return(0xFFFF);
		}
		plelem = IdLstNext(plelem);
	}
	
	while(plelem) {
		ple= IdLstPtrObj(plelem);	// le _ID_DCM_ELEM  pointé par le PLIST_ELEMENT
		
		//if (DEBUG) printf(" on est sur num %04x \n",ple->Num);
	
		if(ple->Num == num) break;
		if(ple->Num >  num) {
			if (DEBUG) printf(" num %04x trouve\n",ple->Num);
			return(0xFFFF);
		}
		plelem = IdLstNext(plelem);
	}
											// POURQUOI ple->valShort N'EST PAS INITIALISE?
	return (str2num(ple->valeurElem,unsigned short int));
}



int _IdDcmGetElementLength(int gr, int num, ID_DCM_HDR  *e) {

_ID_DCM_ELEM *ple;
PLIST_ELEMENT plelem; 

plelem = IdLstFirst(e->plist);

if (DEBUG)  printf("\t\t\t\t\trecherche Lgr(1) gr %x  num %x\n",gr,num); 

	while(plelem) {
		ple= IdLstPtrObj(plelem);	// le _ID_DCM_ELEM  pointé par le PLIST_ELEMENT

		if (ple->Gr == gr) break;
		if ( (ple->Gr >  gr) && (ple->Gr !=0xFFFE) ) {
			if (DEBUG) printf(" gr %04x trouve\n",ple->Gr);
			//return(0xFFFFFFFF);
			return(0);
		}
		plelem = IdLstNext(plelem);
	}
	
	while(plelem) {
		ple= IdLstPtrObj(plelem);	// le _ID_DCM_ELEM  pointé par le PLIST_ELEMENT
	
		if(ple->Num == num) break;
		if(ple->Num >  num) {
			if (DEBUG) printf(" num %04x trouve\n",ple->Num);
			//return(0xFFFFFFFF);
			return(0);
		}
		plelem = IdLstNext(plelem);
	}
	return (ple->LgrElem);
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

short int _IdDcmCheckType(ID_DCM_HDR  *e, int ity, int inb, int itypObj, int SamplesPerPixel)
{
long int lgrMessage;
short int typ;
short int ty=ity;
short int nb=inb;
short int typObj=itypObj;
char * bufbid;

	if (nb==12) nb=8; ///  ?!?
	
if (DEBUG) printf ("ACR LIBIDO %d\n",e->ACR_LIBIDO); 
	typ= -1;
	if (  e->ACR_LIBIDO ) {
 		if (DEBUG) printf ("IdDcmCheckType : on a trouve ACR LIBIDO\n"); 
		bufbid=_IdDcmReadElement(0x0028,0x0199,e);/* on cherche 28,199 (LibIDO type)*/
		if (!bufbid) 
			typ=-1;
		else {
			typ=str2num(bufbid, short int);
    			if (DEBUG)  printf ("IdDcmCheckType : type lu sur disque %x\n",typ); 
    		}
	}

	if (typ==-1) {      /* on n a rien trouve 					*/
					/* (ce n'EST PAS un fichier ACR-LibIDO nouvelle version)  */
                            /* debut usine a gaz    */
                            
               if (SamplesPerPixel == 3) nb=24;
                            
	     	     if((nb==TA_TY_UCHAR*8) && (ty==0))typ = typObj | TY_UCHAR; 
		else if((nb==TA_TY_CHAR*8)  && (ty==1))typ = typObj | TY_CHAR;
		else if((nb==TA_TY_USHORT*8)&& (ty==0))typ = typObj | TY_USHORT;
		else if((nb==TA_TY_SHORT*8) && (ty==1))typ = typObj | TY_SHORT;
		else if((nb==TA_TY_ULONG*8) && (ty==0))typ = typObj | TY_ULONG;
		else if((nb==TA_TY_LONG*8)  && (ty==1))typ = typObj | TY_LONG;
	
		 

		//   ATTENTION : les representations internes des types FLOAT et DOUBLE
		//	  sont machine-dependent
	
      		//les anciens float et doubles n etaient pas signes.... */

		else if ((nb==TA_TY_FLOAT*8)  )				typ = typObj | TY_FLOAT;
		else if ((nb==TA_TY_DOUBLE*8) )				typ = typObj | TY_DOUBLE; 
    		else if ((nb==3*TA_TY_UCHAR*8)&&(ty==0))	typ = typObj | TY_RGB;
		else if ((nb==TA_TY_COMPLEX_DOUBLE*8))		typ = typObj | TY_COMPLEX_DOUBLE;
		
		printf("typ %x\n",typ);
		

  		if (DEBUG)  printf ("IdDcmCheckType : Entete : nb=%d typObj=%x; Type deduit=%x\n",
				nb,typObj,typ);  

		/* ce N'EST PAS un fichier LibIDO	*/

		e->_message=(char *)malloc(10); /* pour eviter des soucis ulterieurs */
		lgrMessage=0;
            
  	} else {   	/* fin usine a gaz    */
 
		/* C'EST un fichier LibIDO	*/
		/* on regarde si 'message' (28,198)contient qq chose */
		lgrMessage= _IdDcmGetElementLength(0x0028,0x0198,e);
		
		if (lgrMessage !=0) { 
			if (DEBUG) printf ("lgrMessage %ld\n", lgrMessage);  
			e->_message=(char *)malloc(lgrMessage+1);
			if (!e->_message) {
				printf("Echec alloc _message (lgr %d)\n",lgrMessage);
				e->_message=(char *)malloc(10); /* pour eviter des soucis ulterieurs */
			} else

     				e->_message=_IdDcmReadElement(0x0028,0x0198,e);
		}
		else
			e->_message=(char *)malloc(10); /* pour eviter des soucis ulterieurs */
  	}
  	e->_message[lgrMessage]='\0';
  	return typ;
}


//-------------------------------------------------------------------------------------------

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
 *				3412 : Bad Little Endian
 * @param nb  Pointeur vers l'integer qui recevra le nombre de bits alloues 
 * @param nbu Pointeur vers l'integer qui recevra le nombre de bits utilises 
 * @param nb  Pointeur vers l'integer qui recevra le nombre le bit de poids fort 
 * @param ty  Pointeur vers l'integer qui recevra le signe 
 *
 * @return	1 : l'objet est ACR-NEMA ou DICOM
 *		0 : sinon
 */

int IdDcmInquireInfoFromFile ( char * filename,
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
  ID_DCM_HDR *e;
  int i;
         if (DEBUG) printf ("entree ds IdDcmInquireInfoFromFile\n");

	if((e=IdDcmGetHeader(filename))==0) {
		printf ("Gros soucis sur : %s\n",filename);
		return (0);
        }
         if (DEBUG) printf ("sortie de IdDcmGetHeader\n");
         
        i = IdDcmInquireInfoFromDcmHdr (e,
			 		typObj,
			 		typLibido,
					typData,
					NX, 
					NY, 
					NZ, 
					NT,
					offset,
					nbFrames,
					codeSwap, 
					bitsAllocated,
					bitsStored,
					highBit,
					signe);
					
	if (DEBUG) printf ("sortie de IdDcmInquireInfoFromDcmHdr\n");

	IdDcmHdrFree(e);
 	return (i);
 	
 }
 
 
//-------------------------------------------------------------------------------------------

/**
 * \brief		Retourne diverses info stockees dans l'entete ACRNEMA ou DICOM
 *
 * @param e	ID_DCM_HDR 
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


int IdDcmInquireInfoFromDcmHdr ( ID_DCM_HDR * e,
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
  unsigned short int ndim;
  unsigned short int dim[4];
  int i, SamplesPerPixel;
  char *bufbid;
		
	e->ACR_LIBIDO = IdDcmIsAcrLibido(e);

	bufbid=_IdDcmReadElement(0x0028,0x0004,e);

	if(bufbid!=NULL) { 			// RGB
		if (  (memcmp(bufbid,"RGB ",4)==0)    
     			||
      		      (memcmp(bufbid,"B RG",4)==0) 
     			||
      		      (memcmp(bufbid," BGR",4)==0)          
     			||
      		      (memcmp(bufbid,"GR B",4)==0)) {

			// rgb=1; // Mais elle servait a QUOI tout court ??

			printf ("Image RGB  ->%s<-\n", bufbid);
		}
	}

	if((ndim=_IdDcmReadElementUSHORT(0x0028,0x0005,e))==0xFFFF) {
		ndim=2;	// nb de dim de l'objet (IMA par defaut)
	}

	if ((ndim>4)||(ndim<1)) {
		printf ("nb de dim = %d, errone\n",ndim);
		return(0);
	} else {
		if (DEBUG) {
			printf ("\t\t\t ndim (0x0028,0x0005) : %d\n",ndim);
		}
	}
	
	// Lecture SamplesPerPixel (1 : Gray Scale, 3 : RGB)
	
	if((SamplesPerPixel=_IdDcmReadElementUSHORT(0x0028,0x0002,e))==0xFFFF) {
		SamplesPerPixel=1;	// (Gray Scale par defaut)
	}	

	// Lecture nbFrames 
		
	if(!(bufbid=_IdDcmReadElement(0x0028,0x0008,e))) {
		*nbFrames=1; // mono-frame par defaut
	} else {
		*nbFrames=atoi(bufbid);
		if (DEBUG)
			printf ("\t\t\t nbFrames (0x0028,0x0008) : %d\n",*nbFrames);
	}

	for(i=0;i<ndim;i++)
		if((dim[i]=_IdDcmReadElementUSHORT(0x0028,0x0010+i,e))==0xFFFF) { 
			IdErrno=IDERR_NON_ACR_FILE;
			printf ("IdDcmInquireInfoFromDcmHdr : fichier non ACR (0x0028,0x00%d) "
				"missing\n",10+i);
			return (0);
		}  else {
			if (DEBUG) 
				printf ("\t\t\t dim (0x0028,0x001?) : dim[%d]=%d\n",i,dim[i]);
		}


	if (e->ACR_LIBIDO!=1) {
	// si c'est une VRAIE image ACR
	// on permutte NX et NY ds l'entete LibIDO
		int secuDim;
		secuDim = dim[0];
		dim[0]  = dim[1];
		dim[1]  = secuDim;
	}

  	*NT = *NZ = *NY = *NX = 1;
  	
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
	  		return (0);
      		}
 	}
 	
	if((*bitsAllocated=_IdDcmReadElementUSHORT(0x0028,0x0100,e))==0xFFFF) { 
		*bitsAllocated=16 ; 		 // nb bits allocated
    	} else {
		if (DEBUG) {
			printf ("\t\t\t nb (0x0028,0x0100) : %d\n",*bitsAllocated);
		}
	}

	if((*bitsStored=_IdDcmReadElementUSHORT(0x0028,0x0101,e))==0xFFFF) { 
		*bitsStored = *bitsAllocated ; 	// nb bits utiles
	} else {
		if (DEBUG) {
			printf ("\t\t\t nbu (0x0028,0x0101) : %d\n",*bitsStored);
		}
        }

	if((*highBit=_IdDcmReadElementUSHORT(0x0028,0x0102,e))==0xFFFF) {
		// hight bit pas pris en compte
		*highBit = *bitsStored-1 ;
	} else {
		if (DEBUG) {
			printf ("\t\t\t highBit (0x0028,0x0102) : %d\n",*highBit);
		}
        }
      	
	if((*signe=_IdDcmReadElementUSHORT(0x0028,0x0103,e))==0xFFFF) { // signe o/n
		IdErrno=IDERR_NON_ACR_FILE;
		printf ("fichier non ACR (0x0028,0x0103) missing\n");
		return (0);
	} else {
		if (DEBUG) {
			printf ("\t\t\t signe (0x0028,0x0103) : %d\n",*signe);
	        }
	}

	if (DEBUG) {
		printf("nbit alloues=%d nbit Utiles=%d highbit %d signe=%d\n",
			 *bitsAllocated,*bitsStored,*highBit,*signe);
		for(i=0;i<ndim;i++)
			printf(" dim[%d]= %d; ", i, dim[i]);
		printf ("\n");
	}   
/*
    if (*typData != TY_RGB) {
    	*typData=_IdAcrCheckType(fp, bufbid, (int)ty, (int)nb, *typObj);
    	
    }
    *typLibido = *typData & TY_SIZE;
*/ 
   
    *typLibido=_IdDcmCheckType(e,*signe,*bitsAllocated,*typObj, SamplesPerPixel); 
    *typData = *typLibido & TY_SIZE;
    
    
    *offset  = e->PixelPosition;
    
	if (DEBUG) {
		printf("PixelPosition %d\n", e->PixelPosition); 
	} 
    
   *codeSwap = e->sw; 

  return 1;
}



static void _Swap(void* im, int swap, int lgr, int type) {                     
unsigned long s32, i;
unsigned short fort,faible;

if (DEBUG) printf ("Entree ds _Swap codeSwap %d,  lgr %d\n",swap, lgr);

if((type==TY_SHORT)||(type==TY_USHORT))
    
	switch(swap) {
		case 0:
		case 12:
 		case 1234:
			break;
		
 		case 21:
 		case 3412:
 		case 2143:
 		case 4321:

 			for(i=0;i<lgr;i++)
  				((unsigned short int*)im)[i]= ((((unsigned short int*)im)[i])>>8)
                            			| ((((unsigned short int*)im)[i])<<8);
 			break;
 			
 		default:
 			printf("valeur de SWAP (16 bits)non autorisee : %d\n", swap);
	} 
 
if( (type==TY_LONG)||(type==TY_ULONG)||(type==TY_FLOAT)  )

	switch (swap) {
    		case 0:
     		case 1234:
    			 break;

    		case 4321:
    			 for(i=0;i<lgr;i++) {
        			faible=  ((unsigned long int*)im)[i]&0x0000ffff;    /* 4321 */
       				fort  =((unsigned long int*)im)[i]>>16;
        			fort=  (fort>>8)   | (fort<<8);
        			faible=(faible>>8) | (faible<<8);
        			s32=faible;
        			((unsigned long int*)im)[i]=(s32<<16)|fort;
       			}
    			break;

    		case 2143:
    			for(i=0;i<lgr;i++) {
       				faible=  ((unsigned long int*)im)[i]&0x0000ffff;    /* 2143 */
       				fort=((unsigned long int*)im)[i]>>16;
       				fort=  (fort>>8)   | (fort<<8);
       				faible=(faible>>8) | (faible<<8);
       				s32=fort; 
       				((unsigned long int*)im)[i]=(s32<<16)|faible;
      			}
    			break;
  
    		case 3412:
    			for(i=0;i<lgr;i++) {
      				faible=  ((unsigned long int*)im)[i]&0x0000ffff;    /* 3412 */
       				fort=((unsigned long int*)im)[i]>>16;                  
       				s32=faible; 
       				((unsigned long int*)im)[i]=(s32<<16)|fort;
      			}                 
    			break; 
    			        
   		default:
   			printf("valeur de SWAP (32 bits) non autorisee : %d\n", swap);
	} 
if(DEBUG) printf("sortie _Swap\n"); 
return;
}





