/*************************************************************************
* $Id: imapart.c,v 1.1 2005/09/09 08:22:49 bellet Exp $
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
*  Description : Creation d'une structure image a partir d'une portion
*                d'une autre image
*
**************************************************************************/
#include <string.h>  // For memcpy
#include <stdlib.h>
#include "idima.h"
#include "iderr.h"
#include "idprint.h"

/* FUNCTION DESCRIPTION **************************************************

IdImaAllocSubImage   (fonction)

RESUME:  Alloc d'1 im. a partir d'1 portion d'1 autre im. ss duplic. de pixels
 
DESCRIPTION: Allocation d'une image a partir d'une portion d'une autre
	     image  sans duplic. de pixels.
	     ATTENTION: L'image d'origine ne doit en aucun cas etre liberee
	     avant cette sous-image. En effet l'information pixel n'est pas
	     dupliquee. Les modifications de la sous-image sont faites dans
	     la zone correspondante de l'image d'origine.
	     Cette fonction peut donc etre tres utile pour faire un
	     traitement sur une portion seulement d'une image.

SYNTAXE:     PPIMAGE sousImage = IdImaAllocSubImage (PPIMAGE ims, int ox, int oy, int lx, int ly);

RETOUR:      type  : PPIMAGE
	     role  : Pointeur vers l'image resultat. Zero si echec.

PARAMETRES:  nom   : ims
	     type  : PPIMAGE
	     role  : Pointeur vers l'image source

	     nom   : ox, oy
	     type  : int
	     role  : Origine en X et Y de la sous-image.

	     nom   : lx, ly
	     type  : int
	     role  : Taille en X et Y de la sous-image.

FICHIER:     imapart.c

EXEMPLE:     if ( (imss=IdImaAllocSubImage(ims,0,0,10,10))==0 )
	       IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
		      // Filtrage median sur la sous-image 
	     imr = IdImaMedian(imss,imr,5);
	     IdImaFreeSubImage(imss);

******************************************************** END DESCRIPTION */

/**
 * \brief		Allocation d'une image a partir d'une portion d'une autre
 *	     image  sans duplic. de pixels.
 *	     ATTENTION: L'image d'origine ne doit en aucun cas etre liberee
 *	     avant cette sous-image. En effet l'information pixel n'est pas
 *	     dupliquee. Les modifications de la sous-image sont faites dans
 *	     la zone correspondante de l'image d'origine.
 *	     Cette fonction peut donc etre tres utile pour faire un
 *	     traitement sur une portion seulement d'une image.
 *
* @param im	PPIMAGRE duquel on veut allouer une sous image
* @param ox	origine en x de la sous image
* @param oy	origine en y de la sous image
* @param lx	longueur en x de la sous image
* @param ly	longueur en y de la sous image
* @return	sous image allouee, sans dupllication des Pixels; NULL si echec
*
*/

PPIMAGE IdImaAllocSubImage ( PPIMAGE im, int ox, int oy, int lx, int ly )
{
  int x;
  int y, j;
  PPIMAGE_UCHAR ims = (PPIMAGE_UCHAR)im;
  PPIMAGE_UCHAR imd = NULL;
  PRIVATE_IMAGE * si;

  if ( (ox < 0) || (ox+lx>IdImaDimX(im)) || (lx<=0) ) {
    IdErrno = IDERR_X_OUT_OF_IMAGE;
    return 0;
  }
  if ( (oy < 0) || (oy+ly>IdImaDimY(im)) || (ly<=0) ) {
    IdErrno = IDERR_Y_OUT_OF_IMAGE;
    return 0;
  }
  if ( im==0 ) {
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }

  if ( IdLibidoType(im) != IMA) {
    IdErrno = IDERR_WRONG_TYPE;
    return(PPIMAGE)0;
  }

  si=(PRIVATE_IMAGE *)calloc(1, sizeof(PRIVATE_IMAGE)+ly*sizeof(void *));
  if(si){

    x = ox*IdSizeOfType(im);
    si->DimX=si->UsedNbX=lx;
    si->DimY=si->UsedNbY=ly;
    si->Type=IdImaType(im);
    si->subObject=1;
    imd = (PPIMAGE_UCHAR)&(si[1]);
    for ( y=oy, j=0; y<oy+ly; y++, j++ )
      imd[j]= &(ims[y][x]);
    IdSetFileName(imd,IdGetFileName(ims));
  }
 else IdErrno = IDERR_POINTER_IS_NULL; /* a mettre le bon code */

				/* Calcul du resultat *****/
  return (PPIMAGE) imd;
}


/* FUNCTION DESCRIPTION **************************************************

IdImaModifSubImage   (fonction)

RESUME:  Modif de l'emplacement d'1 ss im. SANS duplic. de la structure
 
DESCRIPTION: Modif de l'emplacement d'1 ss im. SANS duplic. de la structure
	     ATTENTION: L'image d'origine ne doit en aucun cas etre liberee
	     avant cette sous-image. En effet l'information pixel n'est pas
	     dupliquee. Les modifications de la sous-image sont faites dans
	     la zone correspondante de l'image d'origine.
	     Cette fonction peut donc etre tres utile pour faire un
	     traitement sur une portion seulement d'une image.

SYNTAXE:     PPIMAGE sousImage = IdImaModifSubImage (PPIMAGE ims, PPIMAGE sousImage, int ox, int oy);

RETOUR:      type  : PPIMAGE
	     role  : Pointeur vers l'image resultat. Zero si echec.

PARAMETRES:  nom   : ims
	     type  : PPIMAGE
	     role  : Pointeur vers l'image source

 	     nom   : ims
	     type  : PPIMAGE
	     role  : Pointeur vers l'imagette a modifier

	     nom   : ox, oy
	     type  : int
	     role  : Origine en X et Y de la nouvelle sous-image.

FICHIER:     imapart.c

EXEMPLE:     imss=IdImaModifSubImage(ims,imss,debX,debY);


******************************************************** END DESCRIPTION */

/**
 * \brief		Modif de l'emplacement d'1 sous im. SANS duplic. de la structure
 *	    
 *	     ATTENTION: L'image d'origine ne doit en aucun cas etre liberee
 *	     avant cette sous-image. En effet l'information pixel n'est pas
 *	     dupliquee. Les modifications de la sous-image sont faites dans
 *	     la zone correspondante de l'image d'origine.
 *	     Cette fonction peut donc etre tres utile pour faire un
 *	     traitement sur une portion seulement d'une image.
 *
* @param im	PPIMAGE duquel on veut allouer une sous image
* @param ssim	sousimage que l'on souhaite modifier 
* @param ox	origine en x de la sous image
* @param oy	origine en y de la sous image
* @return	sous image modifiée, sans dupllication de la structure NULL si echec
*
*/

PPIMAGE IdImaModifSubImage ( im, ssim, ox, oy )
PPIMAGE im, ssim;
int     ox, oy;
{
int     lx, ly;

  int x = ox*IdSizeOfType(im);
  int y, j;
  PPIMAGE_UCHAR ims = (PPIMAGE_UCHAR)im;
  PPIMAGE_UCHAR imd = (PPIMAGE_UCHAR)ssim;
/* PRIVATE_IMAGE * si; */

  if ( im==0 ) {
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }
  if ( ssim==0 ) {
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }

  if ( IdLibidoType(im) != IMA) {
    IdErrno = IDERR_WRONG_TYPE;
    return(PPIMAGE)0;
  }

 lx=IdImaDimX(ssim);
 ly=IdImaDimY(ssim);

 if ( (ox < 0) || (ox+lx>IdImaDimX(im)) ) {
    IdErrno = IDERR_X_OUT_OF_IMAGE;
    return 0;
  }
  if ( (oy < 0) || (oy+ly>IdImaDimY(im)) ) {
    IdErrno = IDERR_Y_OUT_OF_IMAGE;
    return 0;
  }

 /* si=(PRIVATE_IMAGE *)calloc(1, sizeof(PRIVATE_IMAGE)+ly*sizeof(void *));*/
 /*   imd = (PPIMAGE_UCHAR)&(si[1]);					  */

    for ( y=oy, j=0; y<oy+ly; y++, j++ )
      imd[j]= &(ims[y][x]);

    IdSetFileName(imd,IdGetFileName(ims));
 
				/* Calcul du resultat *****/
  return (PPIMAGE) imd;
}

/***************************************************/


/* FUNCTION DESCRIPTION **************************************************

IdImaFreeSubImage   (macro)

RESUME: Liberation d'1 image constituee comme 1 portion d'1 autre image

DESCRIPTION: Liberation d'une image constituee comme une portion d'une autre
	     image.
	     ATTENTION: L'image d'origine ne doit en aucun cas etre liberee
	     avant cette sous-image. En effet l'information pixel n'est pas
	     dupliquee. Les modifications de la sous-image sont faites dans
	     la zone correspondante de l'image d'origine.

SYNTAXE:     void IdImaFreeSubImage (PPIMAGE im);

RETOUR:      type  : void

PARAMETRES:  nom   : im
	     type  : PPIMAGE
	     role  : Pointeur vers la sous-image a liberer.

FICHIER:     imapart.c

EXEMPLE:     if ( (imss=IdImaAllocSubImage(ims,0,0,10,10))==0 )
	       IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
		      / * Filtrage median sur la sous-image * /
	     imr = IdImaMedian(ims,imr,5);
	     IdImaFreeSubImage(imss);

******************************************************** END DESCRIPTION */
void _IdImaFreeSubImage(pi)
PPIMAGE * pi;
{
  if(*pi){
	     /* les lignes ne doivent pas etre liberees, puisqu'elles
		appartiennent a l'image d'origine !!! */
    if(_IdImaPrivate(*pi)->_message)free((_IdImaPrivate(*pi)->_message));
    _IdImaPrivate(*pi)->_message=0;
    if(_IdImaPrivate(*pi)->_fichier)free((_IdImaPrivate(*pi)->_fichier));
    _IdImaPrivate(*pi)->_fichier=0;
    free(_IdImaPrivate(*pi));
    *pi=0;
  }
}

/* FUNCTION DESCRIPTION **************************************************

IdImaArrayToImage   (fonction)

RESUME: Alloc d'1 im a partir d'1 tableau 2D SANS duplication de pixels
	     
DESCRIPTION: Allocation d'une image a partir d'un tableau 2D SANS duplication des pixels
	     ATTENTION: Le tableau d'origine ne doit en aucun cas etre liberee
	     avant l'image. En effet l'information pixel n'est pas
	     dupliquee. Les modifications de l'image sont faites dans
	     la zone correspondante du tableau d'origine.
	     Cette fonction peut donc etre tres utile pour transformer un 
	     tableau 2D en une image Libido sans recopier les pixels.

SYNTAXE:     PPIMAGE image = IdImaArrayToImage (void ** tab2D, int lx, int ly);

RETOUR:      type  : PPIMAGE
	     role  : Pointeur vers l'image resultat. Zero si echec.

PARAMETRES:  nom   : tab2D
	     type  : void **
	     role  : Pointeur vers l'image source

	     nom   : lx, ly
	     type  : int
	     role  : Taille en X et Y du tableau

FICHIER:     imapart.c

EXEMPLE:    

******************************************************** END DESCRIPTION */

/**
 * \brief		Alloc d'1 im a partir d'1 tableau 2D SANS duplication de pixels
 *	    
 *
* @param im	tableau 2D sur lequel on veut plaquer une structure PPIMAGE
* @param ty	type (IMA_USHORT, ..) de l'image
* @param ox	dimension en x de l' image
* @param oy	dimension en y de l' image
* @return	Pointeur sur structure PPIMAGE
*
*/

PPIMAGE IdImaArrayToImage ( tab2D, ty, dimx , dimy )
void ** tab2D;
int ty;   		/* type de donnees. ex : IMA_UCHAR... */
int     dimx, dimy;
{
 /* int x = dimx*IdTypeSize(ty); */
  int j;
  PPIMAGE imd=NULL;
  PRIVATE_IMAGE * si;

   if ( tab2D==0 ) {
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }

  si=(PRIVATE_IMAGE *)calloc(1, sizeof(PRIVATE_IMAGE)+dimy*sizeof(void *));
  if(si){
    si->DimX=dimx;
    si->DimY=dimy;
    si->Type=ty;
    imd = (PPIMAGE)&(si[1]);
    for ( j=0; j<dimy; j++ )
      imd[j]= tab2D[j];

  }
				/* Calcul du resultat *****/
  return  imd;
}



/* FUNCTION DESCRIPTION **************************************************

IdImaVecteurToImage   (fonction)

RESUME: Alloc d'1 im a partir d'1 tableau 1D SANS duplication de pixels
	     
DESCRIPTION: Allocation d'une image a partir d'un tableau 1D SANS duplication des pixels
	     ATTENTION: Le tableau 1D d'origine ne doit en aucun cas etre libere
	     avant l'image. En effet l'information pixel n'est pas
	     dupliquee. Les modifications de l'image sont faites dans
	     la zone correspondante du tableau d'origine.
	     Cette fonction peut donc etre tres utile pour transformer un 
	     tableau 1D en une image Libido sans recopier les pixels.

SYNTAXE:     PPIMAGE image = IdImaVecteurToImage ( void tableau1D[], int type, int dimx, int dimy);

RETOUR:      type  : PPIMAGE
	     role  : Pointeur vers l'image resultat. Zero si echec.

PARAMETRES: 
	     nom   : tableau1D
	     type  : tableau
	     role  : Vecteur de valeurs

	     nom   : type
	     type  : int
	     role  : Type LibIDO des donnees
 
	     nom   : dimx, dimy
	     type  : int
	     role  : Taille en X et Y de l'image

FICHIER:     imapart.c

******************************************************** END DESCRIPTION */

/**
 * \brief		Alloc d'une structure PPIMAGE a partir d'un tableau 1D SANS duplication de pixels
 *	    
 *
* @param im	tableau 1D sur lequel on veut plaquer une structure PPIMAGE
* @param ty	type (IMA_USHORT, ..) de l'image
* @param ox	dimension en x de l' image
* @param oy	dimension en y de l' image
* @return	Pointeur sur structure PPIMAGE
*
*/

PPIMAGE IdImaVecteurToImage ( tab1D, ty, dimx , dimy )
void *tab1D;
int ty;   		/* type de donnees: IMA_UCHAR... */
int     dimx, dimy;
{
  int lgLig = dimx*IdTypeSize(ty);
  int j;
  PPIMAGE imd=NULL;
  PRIVATE_IMAGE * si;

   if ( tab1D==0 ) {
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }

/* controler que NX * NY <= lgr vecteur .... JPR  */


  si=(PRIVATE_IMAGE *)calloc(1, sizeof(PRIVATE_IMAGE)+dimy*sizeof(void *));
  if(si){
    si->DimX=dimx;
    si->DimY=dimy;
    si->Type= (unsigned short)ty;
    imd = (PPIMAGE)&(si[1]);

    for ( j=0; j<dimy; j++ )
      imd[j]=(char *)tab1D + j*lgLig;

  }
  return  imd;
}
/***************************************************/

/* FUNCTION DESCRIPTION **************************************************

IdImaVecteurToImageOffset   (fonction)

RESUME: Alloc d'1 im a partir d'une portion d'1 tableau 1D SANS duplication de pixels avec saut entete
	     
DESCRIPTION: Allocation d'une image a partir d'une portion d'un tableau 1D SANS duplication des pixels avec saut entete

	     ATTENTION: L'image d'origine ne doit en aucun cas etre liberee
	     avant ce tableau. En effet l'information pixel n'est pas
	     dupliquee. Les modifications de l'image sont faites dans
	     la zone correspondante du tableau d'origine.
	     Cette fonction peut donc etre tres utile pour transformer un 
	     tableau 1D en une image Libido sans recopier les pixels.

SYNTAXE:     PPIMAGE image = IdImaVecteurToImageOffset ( void tableau1D[], int type, int dimx, int dimy, int offset);

RETOUR:      type  : PPIMAGE
	     role  : Pointeur vers l'image resultat. Zero si echec.

PARAMETRES:  
	     nom   : tableau1D
	     type  : void *
	     role  : Vecteur de valeurs

	     nom   : type
	     type  : int
	     role  : Type LibIDO des donnees
 
	     nom   : dimx, dimy
	     type  : int
	     role  : Taille en X et Y de l'image

	     nom   : offset
	     type  : int
	     role  : Nombre d'elements du tableau que l'on veut 'sauter' 
			pour fabriquer le Signal
 
FICHIER:     imapart.c

******************************************************** END DESCRIPTION */

/**
 * \brief		Alloc d'une structure PPIMAGE a partir d'un tableau 1D 
 *				SANS duplication de pixels avec saut entete
 *	    
* @param im	tableau 1D sur lequel on veut plaquer une structure PPIMAGE
* @param ty	type (IMA_USHORT, ..) de l'image
* @param ox	dimension en x de l' image
* @param oy	dimension en y de l' image
* @param offset	taille de l'entete a sauter
* @return	Pointeur sur structure PPIMAGE
*
*/


PPIMAGE IdImaVecteurToImageOffset ( tab1D, ty, dimx , dimy, offset )
void *tab1D;
int ty;   		/* type de donnees: IMA_UCHAR... */
int     dimx, dimy;
int offset;
{
 /* int x = dimx*IdTypeSize(ty); */
  int j;
  PPIMAGE imd=0;
  PRIVATE_IMAGE * si;

   if ( tab1D==0 ) {
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }

/* controler que NX * NY <= lgr vecteur .... JPR  */

  si=(PRIVATE_IMAGE *)calloc(1, sizeof(PRIVATE_IMAGE)+dimy*sizeof(void *));
  if(si){
    si->DimX=dimx;
    si->DimY=dimy;
    si->Type=(unsigned short)ty;
    imd = (PPIMAGE)&(si[1]);

    for ( j=0; j<dimy; j++ ){
       /* ou n'importe quoi *   */
      imd[j]=(char *)tab1D +offset + j*IdTypeSize(ty)*dimx;}
   }

  return  imd;
}

/* FUNCTION DESCRIPTION **************************************************

IdImaExtractSubImage   (fonction)

RESUME:  Extrait d'1 im. a partir d'1 portion d'1 autre image
  
DESCRIPTION: Extraction (AVEC dupplication des pixels) d'une image a partir 
	     	d'une portion d'une autre image. 
		Si l'image a extraire est deja alouee, elle devra
	     	avoir la bonne taille. 

SYNTAXE:     PPIMAGE sousImage = IdImaExtractSubImage (PPIMAGE ims, PPIMAGE sousImage, int ox, int oy, int lx, int ly);

RETOUR:      type  : PPIMAGE
	     role  : Pointeur vers l'image resultat. Zero si echec.

PARAMETRES:  nom   : ims
	     type  : PPIMAGE
	     role  : Pointeur vers l'image source

	     nom   : sousImage
	     type  : PPIMAGE
	     role  : Pointeur vers le sous-image

	     nom   : ox, oy
	     type  : int
	     role  : Origine en X et Y de la sous-image.

	     nom   : lx, ly
	     type  : int
	     role  : Taille en X et Y de la sous-image.

FICHIER:     imapart.c

EXEMPLE:     if ( (imss=IdImaExtractSubImage(ims,0,0,10,10))==0 )
	       IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
		      / * Filtrage median sur la sous-image * /
	     imr = IdImaMedian(imss,imr,5);
	     IdImaFree(imss);

******************************************************** END DESCRIPTION */

/**
 * \brief		Extraction (AVEC dupplication des pixels) d'une image a partir 
 *	     	d'une portion d'une autre image. 
 *		Si l'image a extraire est deja alouee, elle devra
 *	     	avoir la bonne taille
 *	    
* @param im	image de laquelle lequel on veut extraire une ss omage
* @param sousImRes	sous-image deja allouée NULL si pas allouée
* @param ox	dimension en x de la sous image
* @param oy	dimension en y de la sous image
* @param lx	taille en x de la sous image
* @param ly	taille en y de la sous image
* @return	Pointeur sur structure PPIMAGE
*
*/

PPIMAGE IdImaExtractSubImage ( im, sousImRes, ox, oy, lx, ly )
PPIMAGE im;
PPIMAGE sousImRes;
int     ox, oy;
int     lx, ly;
{
PPIMAGE sousIm;

  if ( (ox < 0) || (ox+lx>IdImaDimX(im)) || (lx<=0) ) {
    IdErrno = IDERR_X_OUT_OF_IMAGE;
    return NULL;
  }
  if ( (oy < 0) || (oy+ly>IdImaDimY(im)) || (ly<=0) ) {
    IdErrno = IDERR_Y_OUT_OF_IMAGE;
    return NULL;
  }
  if ( im==0 ) {
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }

  if ( IdLibidoType(im) != IMA) {
    IdErrno = IDERR_WRONG_TYPE;
    return(PPIMAGE)NULL;
  }

sousIm=IdImaAllocSubImage(im, ox, oy, lx, ly );
if(!sousIm) 	{ 	IdErrno = IDERR_ALLOC_SUB;
			return NULL; /* positionner IdErrNo... */
		}

/* Si sousImRes n'existe pas, on l'alloue meme taille/type que sousIm */

sousImRes=(PPIMAGE) IdImaCheckTypeAllocSize (sousIm,-1,sousImRes,lx,ly);

 if (!sousImRes){ 	IdErrPrintf ("Impossible Allouer Sous-Image\n");
			/* IdErrno positionne par IdImaCheckTypeSizeAlloc */
			return NULL;
		}
	
sousImRes=IdImaCopy(sousIm,sousImRes);

/*
 JAMAIS d'erreur, car filtree par IdImaCheckTypeSizeAlloc 
*/

IdImaFreeSubImage(sousIm);
return (sousImRes);
}

/* FUNCTION DESCRIPTION **************************************************

IdImaSignalFromLine   (fonction)

RESUME:  Alloc d'1 SIGNAL a partir d'1 portion de ligne (+rapide que recopie des pixels)
 
DESCRIPTION: Alloc d'1 SIGNAL a partir d'1 portion de ligne (+rapide que recopie des pixels).
		Le Signal sera alloue avec le meme type LibIDO que l'image.

SYNTAXE:     PSIGNAL signal = IdImaSignalFromLine (PPIMAGE ims, int ox, int oy, int lx);

RETOUR:      type  : PSIGNAL
	     role  : Pointeur vers le SIGNAL resultat. Zero si echec.

PARAMETRES:  nom   : ims
	     type  : PPIMAGE
	     role  : Pointeur vers l'image source

	     nom   : ox, oy
	     type  : int
	     role  : Origine en X et Y du SIGNAL.

	     nom   : lx, ly
	     type  : int
	     role  : Longueurdu SIGNAL

FICHIER:     imapart.c

EXEMPLE:    

******************************************************** END DESCRIPTION */

PSIGNAL IdImaSignalFromLine ( im, ox, oy, lx )
PPIMAGE im;
int     ox, oy;
int     lx;
{
  int x;

  PPIMAGE_UCHAR ims = (PPIMAGE_UCHAR)im;
  PSIGNAL_UCHAR imd = NULL;
  PRIVATE_SIGNAL * si;

  if ( im==0 ) {
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }

  if ( IdLibidoType(im) != IMA) {
    IdErrno = IDERR_WRONG_TYPE;
    return(PPIMAGE)0;
  }

  if ( (ox < 0) || (ox+lx>IdImaDimX(im)) || (lx<=0) ) {
    IdErrno = IDERR_X_OUT_OF_IMAGE;
    return 0;
  }

  if (oy < 0) {
    IdErrno = IDERR_Y_OUT_OF_IMAGE;
    return 0;
  }

x = ox*IdSizeOfType(im);

  si=(PRIVATE_SIGNAL *)calloc (1,sizeof(PRIVATE_SIGNAL)+lx*IdSizeOfType(im));
  if(si){
    si->DimX=si->UsedNbX=lx;
    si->Type=IdLibidoDataType(im) | SIG;
    imd = (PSIGNAL_UCHAR)&(si[1]);
    memcpy(imd,ims[oy]+x,lx*IdSizeOfType(im));
    IdSetFileName(imd,IdGetFileName(ims));
  }

else IdErrno = IDERR_POINTER_IS_NULL; /* a mettre le bon code */

				/* Calcul du resultat *****/
  return (PSIGNAL) imd;
}

/* FUNCTION DESCRIPTION **************************************************

IdImaReuseSigFromLine   (fonction)

RESUME: Copie d'un morceau de ligne d'une image dans un signal deja alloue 
 
DESCRIPTION: remplissage d'1 SIGNAL a partir d'1 portion de ligne (plus rapide
             que copie pixel a pixel)

SYNTAXE:     PSIGNAL signal = IdImaReuseSigFromLine (PPIMAGE ims, int ox, int oy, PSIGNAL sig);

RETOUR:      type  : PSIGNAL
	     role  : Pointeur vers le SIGNAL resultat. Zero si echec.

PARAMETRES:  nom   : ims
	     type  : PPIMAGE
	     role  : Pointeur vers l'image source

	     nom   : ox, oy
	     type  : int
	     role  : Origine en X et Y du SIGNAL.

	     nom   : sig 
	     type  : PSIGNAL
	     role  : signal a remplir 

FICHIER:     imapart.c


******************************************************** END DESCRIPTION */

PSIGNAL IdImaReuseSigFromLine ( im, ox, oy, sig )
PPIMAGE im;
PSIGNAL sig;
int     ox, oy;
{
/* JPR ?? */
/*
  PPIMAGE_UCHAR ims = (PPIMAGE_UCHAR)im;
  PSIGNAL_UCHAR sigs = (PSIGNAL_UCHAR)sig;
*/
  int x;

  if ( im==0 || sig==0) {
    IdErrno = IDERR_POINTER_IS_NULL;
    return(PSIGNAL)0;
  }

  if ( (IdLibidoType(im) != IMA) || (IdLibidoType(sig) != SIG) 
         || (IdLibidoDataType(im)!=IdLibidoDataType(sig))) {
    IdErrno = IDERR_WRONG_TYPE;
    return(PSIGNAL)0;
  }

  if ( (ox < 0) || ((ox+IdSigDimX(sig))>IdImaDimX(im)) ) {
    IdErrno = IDERR_X_OUT_OF_IMAGE;
    return 0;
  }

  if ( (oy < 0) || (oy>=IdImaDimY(im)) ) {
    IdErrno = IDERR_Y_OUT_OF_IMAGE;
    return(PSIGNAL)0;
  }
  x = ox*IdSizeOfType(im);
  memcpy(sig,(char *)im[oy]+x*IdSizeOfType(im),IdSigDimX(sig)*IdSizeOfType(im));
  IdSetFileName(sig,IdGetFileName(im));

  return (PSIGNAL) sig;
}

