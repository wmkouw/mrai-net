/*************************************************************************
* $Id: imafill.c,v 1.1 2005/09/09 08:22:47 bellet Exp $
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

#include <stdio.h>
#include <string.h>   // For strcpy
#include "idima.h"
#include "iderr.h"
#include "idprint.h"
#include "idio.h"
/*******************************************************************************
*
* Description :  Avec des images UCHAR
*		Remplit les trous des objets connexes d'une image binaire
*        Parametres de sortie : Aucun
*
*******************************************************************************/

/* FUNCTION DESCRIPTION **************************************************

IdImaFill   (fonction)

RESUME: Rempli les trous ds les objets connexes d'1 image binaire

DESCRIPTION: 
	Remplit les trous dans les objets connexes d'une image binaire
		(Objets blancs sur fond noir).
	A noter: un cadre noir de largeur 1 pixel remplace le bord de l'image.
	PLUS MAINTENANT ! ...

	ATTENTION : L'image source etait modifiee, ainsi que l'image destination.
			Ce n'etait pas precise dans la Doc, c'etait peut-etre un Bug....
			Ce n'est plus le cas maintenant (21/4/99)

SYNTAXE:     int retCode=IdImaFill (PPIMAGE_UCHAR imSource, PPIMAGE_UCHAR imDest, int connexite);

RETOUR:      type  : int
	     role  : -1 si echec, sinon le nombre de trous remplis.

PARAMETRES:  nom   : ims
	     type  : PPIMAGE_UCHAR
	     role  : Pointeur vers l'image source

	     nom   : imd
	     type  : PPIMAGE_UCHAR
	     role  : Pointeur vers l'image resultat
		     Si ce pointeur est nul, l'image est allouee par
		     la fonction.  NON  CE POINTEUR NE PEUT PAS ETRE NULL  MW

	     nom   : connexite
	     type  : int
	     role  : connexite des objets:
			connexite = 1 : 8-connexite 
			connexite = 0 : 4-connexite

FICHIER:     imafill.c

EXEMPLE:     if ( (status=IdImaFill(ims,imd,1)) == -1 )
               IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));

******************************************************** END DESCRIPTION */
int
IdImaFill(ims,imd,connexite)
PPIMAGE_UCHAR ims,imd;
int connexite;
{
PPIMAGE_SHORT imo;
PPIMAGE_UCHAR imX,imY;
FILE *fich_res,*fich_mod;
char nom_res[100],nom_mod[100];
int nb,surf_min=1,x,y;
short val;
unsigned char tval;
                                                 /* Controle des l'images **/
  if ( !ims || IdImaType(ims)!=IMA_UCHAR ){
    IdErrno = IDERR_WRONG_TYPE;
    return -1;
  }
  if ( !imd || IdImaType(imd)!=IMA_UCHAR ){
    IdErrno = IDERR_WRONG_TYPE;
    return -1;
  }
  if ( (connexite != 0) && (connexite != 1) ){
    IdErrno = IDERR_INVALID_NUMBER;
    return -1;
  }

/* On sauve le cadre de largeur un pixel autour de l'image             *****/
/* On repousse vers l'interieur les pixels blancs qui touchent le bord *****/
/* On incruste un cadre noir de largeur un pixel autour de l'image     *****/

imX=(PPIMAGE_UCHAR)IdImaAlloc(IdImaDimX(ims),2,IMA_UCHAR);
imY=(PPIMAGE_UCHAR)IdImaAlloc(2,IdImaDimY(ims),IMA_UCHAR);

	for(x=0;x<IdImaDimX(ims);x++) 
		{ imX[0][x] = ims[0][x];
		  imX[1][x] = ims[IdImaDimY(ims)-1][x];

		  if (ims[0][x]==255) 
				{
				 ims[1][x]=255; 
				 ims[0][x]=0;
				}
		  if (ims[IdImaDimY(ims)-1][x]==255)
				{ 
				 ims[IdImaDimY(ims)-2][x]=255;
				 ims[IdImaDimY(ims)-1][x]=0;
				}
		}

	for(y=1;y<IdImaDimY(ims)-1;y++) 
		{ imY[y][0] = ims[y][0];
		  imY[y][1] = ims[y][IdImaDimX(ims)-1];

		  if (ims[y][0]==255) 
				{
				 ims[y][1]=255; 
				 ims[y][0]=0;
				}
		  if (ims[y][IdImaDimX(ims)-1]==255)
				{ 
				 ims[y][IdImaDimX(ims)-2]=255;
				 ims[y][IdImaDimX(ims)-1]=0;
				}
		}	

/***** Transformation de l'image en negatif *****/
	for(y=0;y<IdImaDimY(ims);y++)
		for(x=0;x<IdImaDimX(ims);x++){
			tval=ims[y][x];
			ims[y][x]= ~tval;
			}

/***** Allocation de l'image label et initialisation a zero *****/
	imo=(PPIMAGE_SHORT) IdImaAlloc(IdImaDimX(ims),IdImaDimY(ims),IMA_SHORT);
	if ( !imo ) {
		IdErrno = IDERR_ALLOC_MEM;
		return -1;
		}
	for(y=0;y<IdImaDimY(imo);y++)
		for(x=0;x<IdImaDimX(imo);x++)
			imo[y][x]=0;

/***** Recherche des objets (trous) *****/

/* ! ! ! Nom de fichiers fixes, 1 seul appel a la fois ! ! ! */

#if ( _SUNC_ )
	strcpy(nom_mod,getenv("HOME")); strcat(nom_mod,"/tmp/ftemp.mod");
	strcpy(nom_res,getenv("HOME")); strcat(nom_res,"/tmp/ftemp.res");
#else
	strcpy(nom_mod,"ftemp.mod");
	strcpy(nom_res,"ftemp.res");
#endif
	if(! (fich_mod=fopen(nom_mod,ID_WFILE_TEXT)) ) {
		IdErrno = IDERR_OPEN_FAILED;
		IdErrPrintf("ERREUR: %s: %s\n",IdErrMsg(IdErrno),nom_mod);
		return -1;
		}
	if(! (fich_res=fopen(nom_mod,ID_WFILE_TEXT)) ) {
		IdErrno = IDERR_OPEN_FAILED;
		IdErrPrintf("ERREUR: %s: %s\n",IdErrMsg(IdErrno),nom_res);
		return -1;
		}

	nb=IdImaLabel(ims,imo,surf_min,fich_res,fich_mod,1-connexite);

/* Les connexites des objets et des trous sont complementaires (4 - 8) */

	if(nb == -1) IdErrPrintf("ERREUR: %s\n",IdErrMsg(IdErrno));
	fclose(fich_res);fclose(fich_mod);

/***** Remplissage des trous *****/

	for(y=0;y<IdImaDimY(imd);y++)
	for(x=0;x<IdImaDimX(imd);x++)
		{
		val=imo[y][x];
		if (val==0) imd[y][x]=0;	/*   val=0  : objets */
		else if (val<nb) imd[y][x]=0;	/* 0<val<nb : trous  */
		else imd[y][x]=255;		/*   val=nb : fond   */
		}

/***** Liberation memoire *****/
	IdImaFree(imo);

/***** Retour en normal negatif de imd *****/

	for(y=0;y<IdImaDimY(imd);y++)
		for(x=0;x<IdImaDimX(imd);x++){
			tval=imd[y][x];
			imd[y][x]= ~tval;
/* MW */
/*         		tval=ims[y][x];
         		ims[y][x]= ~tval;
*/
			}

if (imd!=ims)					/* On ne restaure l'image Source QUE si elle est differente 	*/
						/* de l'image destination ... JPRx 				*/
	for(y=0;y<IdImaDimY(imd);y++)
		for(x=0;x<IdImaDimX(imd);x++){
        	 	tval=ims[y][x];
         		ims[y][x]= ~tval;
			}

/* On restaure le cadre de largeur un pixel autour de l'image *****/

	for(x=0;x<IdImaDimX(ims);x++) 
		{ imd[0][x] = imX[0][x];
		  imd[IdImaDimY(ims)-1][x] = imX[1][x];

                  ims[0][x] = imX[0][x];
		  ims[IdImaDimY(ims)-1][x] = imX[1][x];
		}

	for(y=1;y<IdImaDimY(ims)-1;y++) 
		{ imd[y][0] = imY[y][0];
		  imd[y][IdImaDimX(ims)-1] = imY[y][1];

                  ims[y][0] = imY[y][0];
		  ims[y][IdImaDimX(ims)-1] = imY[y][1];
		}

IdImaFree(imX);
IdImaFree(imY);

return nb ;
}
