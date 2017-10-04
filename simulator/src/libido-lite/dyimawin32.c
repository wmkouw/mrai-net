/*************************************************************************
* $Id: dyimawin32.c,v 1.3 2005/09/22 15:14:37 yougz Exp $
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

#if defined(_MSC_VER)

#include "idima.h"
#include "iderr.h"
#include "idacr.h"

#include <dos.h>
#include <windows.h>



/*************************************************************************
*                     |              |
* REVISIONS :  DATE   | AUTEUR       |  DESCRIPTION
*---------------------|--------------|------------------------------------
*           12/11/92  | Ch. Mathieu  |  Prise en charge de tous les
*                     |              |  parametres de IdImaDisplay()
*           26/11/92  | Ch. Mathieu  |  Fonction de zoom.
**************************************************************************/

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

char buff[180];

void _IdImaDisplayColor(PPIMAGE_UCHAR ims,
                       int display,
		       int x, int y, int nx, int ny, int ox, int oy,
		       PSIGNAL_RGB pal)
{
UINT retCode;
#ifdef _IWWIN95_
   static int res;
   LPSTARTUPINFO lpstartinfo;
   PROCESS_INFORMATION procinfo;
#endif
   // CAVEAT EMPTOR : all this code assumes the libido Win32 installation
   // process was done :
   // 1/ on disk C:
   // 2/ inside the C:\Creatis\Libido directory and that
	//    C:\Creatis\Libido\bin\affimwin.exe is correct binary
   // 3/ a C:\Creatis\Libido\tmp directory exists
   // Those assumptions are NOT verified and no debug message is printed
   // on console. You've been warned... 

   char *fn;
   IdAcrWriteFile("_imtmp_.acr",ims);
   strcpy(buff,"affimwin filein=_imtmp_.acr name=");
   fn=IdGetFileName(ims);
   if(fn)strcat(buff,fn);
   if (pal) {
      IdPalWriteAsciiFile("_paltmp_.asc",pal);
      strcat(buff,"filepal=_paltmp_.asc");
   }

#ifdef _IWWIN95_
   lpstartinfo=(LPSTARTUPINFO)calloc(1,sizeof(STARTUPINFO));
   lpstartinfo->cb=sizeof(STARTUPINFO);

   res=CreateProcess(NULL,buff,NULL,NULL,FALSE,0,NULL,NULL,
                    lpstartinfo,&procinfo);
   res=res*1;
#else
	retCode=WinExec(buff,SW_SHOW);
	   	if(retCode<31) {
   		strcpy(buff,"affimwin" 
			    "filein=_imtmp_.acr name=");
		if(fn) strcat(buff,fn);
   		if (pal) {
      			strcat(buff,"filepal=_paltmp_.asc");
		}
	}	
#endif

   return;
}


#endif
