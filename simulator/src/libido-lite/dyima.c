/*************************************************************************
* $Id: dyima.c,v 1.1 2005/09/09 08:22:24 bellet Exp $
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

#if defined(HAVE_X11) || defined(_MSC_VER)
#include "idima.h"
#include "idima-private.h"

//// FIXME IDODUDE, of please DOXYGEN ME, my dearest IDODUDE, oh yeeeeeessss

/* FUNCTION DESCRIPTION ****************************************************
* 
* IdImaDisplayColorQ  (macro)
* 
* RESUME:  affiche une image 8 bits avec une palette couleur
* 
* DESCRIPTION:  	affiche une zone d'une image 8 bits  dans le display
* 		avec une palette couleur 
* 
* SYNTAXE:   void IdImaDisplayColorQ (PPIMAGE_UCHAR im,int modAff, PSIGNAL_RGB pal);
* 
* RETOUR    :  type  : rien
* 	     role  : cf resume.
* 
* PARAMETRES:  nom   : im
* 	     type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers l'image source
* 
*              nom   : modAff
* 	     type  : int
* 	     role  : param d'affichage (depend du systeme)
*                      pair: meme fenetre impair: nouvelle fenetre
* 		     images 8 bits:
*                      0-1 128 gris/couleurs garantis
*                      2-3 256 gris/couleurs non garantis
* 		     4-5 256 gris/couleurs garantis
* 		     images 24 bits:
*                    16 millions de couleurs garanties (sur Serveur True Color)
* 
* 		     ATTENTION : pour des 'DISPLAY' pairs les images doivent 
* 				avoir TOUTES la meme taille (NX, NY)
* 		     ATTENTION : les 'DISPLAY' pairs n'ont pas d'effet sur PC
* 
* 		     Voir LibIDO Graphique (UTILIW/visuseri)
* 			si ca vous pose des pb
*             
*              nom   : pal
* 	     type  : PSIGNAL_RGB
* 	     role  : pointeur vers la palette
* 
* FICHIER:     DYIMAX.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION ****************************************************
* 
* IdImaDisplayColor  (macro)
* 
* RESUME:  Positionne une image 8 bits avec une palette couleur sur un ecran
* 
* DESCRIPTION:  Positionne une zone d'une image 8 bits  commencant en (x,y) de 
* 		largeurs (nx,ny) a la position (ox,oy) dans le display
* 		avec une palette couleur 
* 
* 		UTILISEZ PLUTOT IdImaDisplayColorQ  (+ simple)
* 
* SYNTAXE:     void IdImaDisplayColor (PPIMAGE_UCHAR im,int modAff ,int offsetX, int offsetY,int tailleFenX,int tailleFenY, int posX, int posY, PSIGNAL_RGB pal);
* 
* RETOUR    :  type  : rien
* 	     role  : cf resume.
* 
* 
* PARAMETRES:  nom   : im
* 	     type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers l'image source
* 
*              nom   : modAff
* 	     type  : int
* 	     role  : param d'affichage (depend du systeme)
*                      pair: meme fenetre impair: nouvelle fenetre
* 		     images 8 bits:
*                      0-1 128 gris/couleurs garantis
*                      2-3 256 gris/couleurs non garantis
* 		     4-5 256 gris/couleurs garantis
* 		     images 24 bits:
*                    16 millions de couleurs garanties (sur Serveur True Color)
* 
* 		     ATTENTION : pour des 'DISPLAY' pairs les images doivent 
* 				avoir TOUTES la meme taille (NX, NY)
* 		     ATTENTION : les 'DISPLAY' pairs n'ont pas d'effet sur PC
* 
* 		     Voir LibIDO Graphique (UTILIW/visuseri)
* 			si ca vous pose des pb
* 
* 
*              nom   : offsetX,offsetY
* 	     type  : int
* 	     role  : offset de depart en x,y
*            
*              nom   : tailleFenX,tailleFenY
* 	     type  : int
* 	     role  : taille en X, Y de l'image produite
* 
*              nom   : posX,posY
* 	     type  : int
* 	     role  : position en x,y dans le display
*                       
*              nom   : pal
* 	     type  : PSIGNAL_RGB
* 	     role  : pointeur vers la palette
* 
* FICHIER:     DYIMAX.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION ****************************************************
* 
* IdImaDisplayQ  (macro)
* 
* RESUME:  Affiche une image 8 bits en 128 niveaux de gris
* 
* DESCRIPTION:  	Affiche une image 8 bits dans le display 
* 
* SYNTAXE:     void IdImaDisplayQ (PPIMAGE_UCHAR im,int modAff);      
* 
* RETOUR    :  type  : rien
* 	     role  : cf resume.
*             
* PARAMETRES:  nom   : im
* 	     type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers l'image source
* 
*              nom   : modAff
* 	     type  : int
* 	     role  : param d'affichage (depend du systeme)
*                      pair: meme fenetre impair: nouvelle fenetre
* 		     images 8 bits:
*                      0-1 128 gris/couleurs garantis
*                      2-3 256 gris/couleurs non garantis
* 		     4-5 256 gris/couleurs garantis
* 		     images 24 bits:
*                    16 millions de couleurs garanties (sur Serveur True Color)
* 
* 		     ATTENTION : pour des 'DISPLAY' pairs les images doivent 
* 				avoir TOUTES la meme taille (NX, NY)
* 		     ATTENTION : les 'DISPLAY' pairs n'ont pas d'effet sur PC
* 
* 		     Voir LibIDO Graphique (UTILIW/visuseri)
* 			si ca vous pose des pb  
* 
* FICHIER:     DYIMAX.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION ****************************************************
* 
* IdImaDisplay  (macro)
* 
* RESUME:  Positionne 1 zone d'1 image 8 bits en 128 niveaux de gris
* 
* DESCRIPTION: 	Positionne une zone d'une image 8 bits  commencant en (x,y) de 
* 		largeurs (nx,ny) a la position (ox,oy) dans le display
* 
* 		UTILISEZ PLUTOT IdImaDisplayQ  (+ simple)
* 
* SYNTAXE:     void IdImaDisplay (PPIMAGE_UCHAR im,int modAff ,int offsetX, int offsetY,int tailleFenX,int tailleFenY, int posX, int posY);
* 
* RETOUR    :  type  : rien
* 	     role  : cf resume.
* 
* PARAMETRES:  nom   : im
* 	     type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers l'image source
* 
*              nom   : modAff
* 	     type  : int
* 	     role  : param d'affichage (depend du systeme)
*                      pair: meme fenetre impair: nouvelle fenetre
* 		     images 8 bits:
*                      0-1 128 gris/couleurs garantis
*                      2-3 256 gris/couleurs non garantis
* 		     4-5 256 gris/couleurs garantis
* 		     images 24 bits:
*                    16 millions de couleurs garanties (sur Serveur True Color)
* 
* 		     ATTENTION : pour des 'DISPLAY' pairs les images doivent 
* 				avoir TOUTES la meme taille (NX, NY)
* 		     ATTENTION : les 'DISPLAY' pairs n'ont pas d'effet sur PC
* 
* 		     Voir LibIDO Graphique (UTILIW/visuseri)
* 			si ca vous pose des pb
* 
*              nom   : offsetX,offsetY
* 	     type  : int
* 	     role  : offset de depart en x,y
*            
*              nom   : tailleFenX,tailleFenY
* 	     type  : int
* 	     role  : taille en X, Y de l'image produite
* 
*              nom   : posX,posY
* 	     type  : int
* 	     role  : position en x,y dans le display
*                       
*              nom   : pal
* 	     type  : PSIGNAL_RGB
* 	     role  : pointeur vers la palette.        
* 
* FICHIER:     DYIMAX.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

/**
 * \ingroup     ima
 * \brief       cette fonction n'a ete laisee ici que pour compatiblite 
 *              avec des versions anterieures et doit etre remplacee 
 *              par IdImaDisplayColorQ
*/

void
IdImaDisplayColor (PPIMAGE_UCHAR im,
        int display,
        int x, int y, int nx, int ny, int ox, int oy,
        PSIGNAL_RGB pal)
{
        _IdImaDisplayColor(im, display, x, y, nx, ny, ox, oy, pal);
}

/**
 * \ingroup     ima
 * \brief       cette fonction n'a ete laisee ici que pour compatiblite 
 *              avec des versions anterieures et doit etre remplacee 
 *              par IdImaDisplayQ
*/

void
IdImaDisplay (PPIMAGE_UCHAR im,
        int display,
        int x, int y, int nx, int ny, int ox, int oy)
{
        _IdImaDisplayColor(im, display, x, y, nx, ny, ox, oy, NULL);
}




/**
 * \ingroup     ima
 * \brief       affiche une image UCHAR en niveau de gris ou RGB
 * @param im   image a afficher
 * @param display  ne marche qu'en UN*X (enfin ... TOUS les UNIX, sauf LINUX) 
 *			si =1 ou =5 affiche dans une nouvelle fenetre
 *		    	si =0 ou =4 affiche dans la  meme fenetre
 *
 *			si =0 ou =1 'prend' 128 niveaux de gris
 * 			si =4 ou =5 'prend' 256 niveaux de gris
 *				--> et donc fait flasher les ecrans 8 bits :-(
 */
 
void
IdImaDisplayQ (PPIMAGE_UCHAR im, int display)
{
        _IdImaDisplayColor(im, display,
                        0, 0, IdImaDimX(im), IdImaDimY(im), 0, 0, NULL);
}

/**
 * \ingroup     ima
 * \brief       affiche une image UCHAR en 'fausses couleurs'
 * @param im   image a afficher
 * @param display  ne marche qu'en UN*X (enfin ... TOUS les UNIX, sauf LINUX) 
 *			si =1 ou =5 affiche dans une nouvelle fenetre
 *		    	si =0 ou =4 affiche dans la  meme fenetre
 *
 *			si =0 ou =1 'prend' 128 niveaux de gris
 * 			si =4 ou =5 'prend' 256 niveaux de gris
 *				--> et donc fait flasher les ecrans 8 bits :-(
 * @param pal   palette de fausses couleurs sous forme d'un PSIGNAL_RGB
 *
 */

void
IdImaDisplayColorQ (PPIMAGE_UCHAR im, int display, PSIGNAL_RGB pal)
{
        _IdImaDisplayColor(im, display,
                        0, 0, IdImaDimX(im), IdImaDimY(im), 0, 0, pal);
}
#else /* HAVE_X11 || MSC_VER */
/* Here we put dummy functions to satisfy filters programs compilation
 * even when X11 is not available
 */
#include "idima.h"
void IdImaDisplayColor (PPIMAGE_UCHAR im,
        int display,
        int x, int y, int nx, int ny, int ox, int oy,
        PSIGNAL_RGB pal)
{}
void IdImaDisplay (PPIMAGE_UCHAR im,
        int display, int x, int y, int nx, int ny, int ox, int oy)
{}
void IdImaDisplayQ (PPIMAGE_UCHAR im, int display)
{}
void IdImaDisplayColorQ (PPIMAGE_UCHAR im, int display, PSIGNAL_RGB pal)
{}
#endif /* HAVE_X11 || MSC_VER */
