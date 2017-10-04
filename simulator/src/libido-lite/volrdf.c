/*************************************************************************
* $Id: volrdf.c,v 1.1 2005/09/09 08:22:57 bellet Exp $
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
 * Read a Volume out of a raw file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // For strlen
#include "idvol.h"
#include "idio.h"
#include "idprint.h"

/* FUNCTION DESCRIPTION **************************************************

IdVolReadRawFile  (fonction)

RESUME: Lect. d'1 fich. Volume RAW et allocation du Volume

DESCRIPTION: Charge en memoire un fichier Volume au format RAW.
             L'espace memoire est alloue par cette fonction.

SYNTAXE:     PPPVOLUME vol = IdVolReadRawFile(char *filename, int dimx, int dimy, int dimz, int typeVol);

RETOUR:      type  : PPPVOLUME 
             role  : Pointeur vers le volume charge en memoire.

PARAMETRES:
             nom   : filename
             type  : char *
             role  : Pointeur vers le nom complet du fichier volume
                     a charger.

             nom   : dimx,dimy,dimz
             type  : int
             role  : Dimensions en x,y,z du volume a charger.

             nom   : typeVol
             type  : int
             role  : Type des voxels.

FICHIER:     rdfvol.c

******************************************************** END DESCRIPTION */
PPPVOLUME IdVolReadRawFile(nomfic,sx,sy,sz,ty)
char *nomfic;
int sx,sy,sz,ty;
{
  int      i, j;
  FILE   * fp;
  PPPVOLUME  vo;

  fp=fopen(nomfic,ID_RFILE_BIN);
  if(fp) {
    vo=IdVolAlloc(sx,sy,sz,ty);
    if(vo){
      sx = (IdVolType(vo)==VOL_BIT)               /* CM V1.01 */
           ? ((IdVolDimX(vo)+7)>>3)
           : IdVolDimX(vo);

      for(i=0;i<sz;i++) for(j=0;j<sy;j++)
        if( !(fread(vo[i][j],IdSizeOfType(vo),sx,fp)) ) {
		IdPrintf("Echec Lecture volume : %s\n",nomfic);
		break;
	}

      _IdVolPrivate(vo)->_fichier=(char *)malloc(strlen(nomfic)+1);
      strcpy(_IdVolPrivate(vo)->_fichier,nomfic);
    }
    fclose(fp);
    return(vo);
  }else
    return ( (PPPVOLUME )0 );
}

/* FUNCTION DESCRIPTION **************************************************

IdVolReadRawFileToVol  (fonction)

RESUME: Lect. d'un fichier Volume au format RAW ds un VOLUME existant.

DESCRIPTION: Charge en memoire un fichier Volume au format RAW.
             L'espace memoire n'est pas alloue par cette fonction.

SYNTAXE:     int retCode = IdVolReadRawFileToVol(char *filename, PPPVOLUME vol);

RETOUR:      type  : int
             role  : Indicateur d'erreur: 1 : OK
                                          0 : Pb. Volume non charge
					      Fichier non trouve.

PARAMETRES:
             nom   : filename
             type  : char *
             role  : Pointeur vers le nom complet du fichier volume
                     a charger.

             nom   : vol
             type  : PPPVOLUME 
	     role  : Pointeur vers la zone et la structure de description
		     du volume a charger. Ceci suppose que ce pointeur est
		     le resultat de la fonction IdVolAlloc.

FICHIER:     rdfvol.c

******************************************************** END DESCRIPTION */
int IdVolReadRawFileToVol ( nomfic, vo )
char   * nomfic;
PPPVOLUME  vo;
{
  int    i, j;
  FILE * fp;

  fp=fopen(nomfic,ID_RFILE_BIN);
  if(fp){
    if(vo){
      int sx = (IdVolType(vo)==VOL_BIT)
               ? ((IdVolDimX(vo)+7)>>3)
               : IdVolDimX(vo);

      for(i=0;i<IdVolDimZ(vo);i++)for(j=0;j<IdVolDimY(vo);j++)
        fread(vo[i][j],IdSizeOfType(vo),sx,fp);

      _IdVolPrivate(vo)->_fichier=(char *)malloc(strlen(nomfic)+1);
      strcpy(_IdVolPrivate(vo)->_fichier,nomfic);
    }
    fclose(fp);
    return(1);
  }else
    return(0);
}
/* FUNCTION DESCRIPTION ******************************************* JPR **

IdVolReadRawFileWithOffset   (fonction)

DESCRIPTION: Lecture d'un fichier volume au format RAW 
	  avec saut d'une entete et creation de la structure VOLUME
	  correspondante.

SYNTAXE:     PPPVOLUME vol = IdVolReadRawFileWithOffset(char *filename, int dimx, int dimy, int dimz, int typeVol, int tailleEntete);

RETOUR:      type  : PPPVOLUME
	     role  : pointeur sur le VOLUME cree.

PARAMETRES:  
	     nom   : nomfic
	     type  : nom de fichier
	     role  : fichier contenant le VOLUME au format RAW

	     nom   : dimx
	     type  : entier
	     role  : taille en X du VOLUME

	     nom   : dimy
	     type  : entier
	     role  : taille en Y du VOLUME

	     nom   : dimz
	     type  : entier
	     role  : taille en Z du VOLUME

	     nom   : typeVol
	     type  : entier
	     role  : type du VOLUME ( VOL_UCHAR, etc...

	     nom   : tailleEntete
	     type  : entier
	     role  : taille, en octets, de l'entete de l'VOLUME

FICHIER:     rdfvol.c

EXEMPLE:     PPPVOLUME_UCHAR im;
	     im=IdVolReadRawFileWithOffset("lena.vol",512,512,512,VOL_UCHAR,0);


******************************************************** END DESCRIPTION */

#define SEEK_SET 0

PPPVOLUME IdVolReadRawFileWithOffset(nomfic,sx,sy,sz,ty,offset) 
char *nomfic;
int sx,sy,sz,ty,offset;
{
  int i,j;
  FILE *fp;
  PPPVOLUME vo;

/* IdPrintf("sx=%d sy=%d sz %d ty=%d offset=%d \n",sx,sy,sz,ty,offset); */

 fp=fopen(nomfic,ID_RFILE_BIN);
  if(fp) {
    vo=IdVolAlloc(sx,sy,sz,ty);
    if(vo){
         if(offset)  fseek(fp,offset,SEEK_SET);      /* JPR  V1.01_001*/
      sx = (IdVolType(vo)==VOL_BIT)
           ? ((IdVolDimX(vo)+7)>>3)
           : IdVolDimX(vo);

      for(i=0;i<sz;i++) for(j=0;j<sy;j++)
        fread(vo[i][j],IdSizeOfType(vo),sx,fp);

      _IdVolPrivate(vo)->_fichier=(char *)malloc(strlen(nomfic)+1);
      strcpy(_IdVolPrivate(vo)->_fichier,nomfic);
    }
    fclose(fp);
    return(vo);
  }else
    return ( (PPPVOLUME )0 );
}


/* FUNCTION DESCRIPTION ******************************************* JPR **

IdVolReadRawFileToVolWithOffset   (fonction) PAS FAIT ...

DESCRIPTION: Lecture d'un fichier volume au format RAW dans un PPPVOLUME existant avec saut d'une entete 

SYNTAXE:     PPPVOLUME vol = IdVolReadRawFileToVolWithOffset(char *filename, PPPPVOLUME volExist, int dimx, int dimy, int dimz, int typeVol, int tailleEntete);

RETOUR:      type  : PPPVOLUME
	     role  : pointeur sur le VOLUME cree.

PARAMETRES:  
	     nom   : nomfic
	     type  : nom de fichier
	     role  : fichier contenant le VOLUME au format RAW

	     nom   : dimx
	     type  : entier
	     role  : taille en X du VOLUME

	     nom   : dimy
	     type  : entier
	     role  : taille en Y du VOLUME

	     nom   : dimz
	     type  : entier
	     role  : taille en Z du VOLUME

	     nom   : typeVol
	     type  : entier
	     role  : type du VOLUME ( VOL_UCHAR, etc...

	     nom   : tailleEntete
	     type  : entier
	     role  : taille, en octets, de l'entete de l'VOLUME

FICHIER:     rdfvol.c

EXEMPLE:     PPPVOLUME_UCHAR im;
	     im=IdVolReadRawFileWithOffset("lena.vol",512,512,512,VOL_UCHAR,0);


******************************************************** END DESCRIPTION */
