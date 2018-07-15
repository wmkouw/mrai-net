/*************************************************************************
* $Id: imaclear.c,v 1.1 2005/09/09 08:22:45 bellet Exp $
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
*  Description : initialisation d'images.
*
**************************************************************************/
#include <string.h>  // For memcpy and memset
#include "idima.h"
#include "iderr.h"
#include "idprint.h"

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaClear (fonction)
* 
* RESUME: initialisation d'une image a 0
* 
* DESCRIPTION: Initialisation d'une image a 0.
* 
* SYNTAXE:     PPIMAGE imResult = IdImaClear (PPIMAGE imResult);
* 
* RETOUR:      type  : PPIMAGE
*              role  : Pointeur vers l'image initialisee. Zero si echec.
* 
* PARAMETRES:  nom   : imResult
*              type  : PPIMAGE
*              role  : Pointeur vers l'image a initialiser.
* 
* FICHIER:     imaclear.c
* 
* EXEMPLE:     if ( IdImaClear(image)==0 )
*                 IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
* 
******************************************************** END DESCRIPTION */

PPIMAGE IdImaClear ( PPIMAGE image )
{
  int y;
  int taille_ligne;
                                                 /* Controle de l'image **/
  if ( !image ) {                                /* Alloc. si necessaire ***/
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }else if ( IdLibidoType(image) != IMA) {
    IdErrno = IDERR_WRONG_TYPE;
    return 0;
  }
  taille_ligne = (IdImaType(image)==IMA_BIT)
                ?  ( (IdImaDimX(image)+7)>>3 )
                :  (IdImaDimX(image)*IdSizeOfType(image)) ;
   /* Calcul du resultat *****/
  for ( y=0; y<IdImaDimY(image); y++ )
    memset( image[y], 0, taille_ligne );

IdImaSetUsedNbX(image,0);
IdImaSetUsedNbY(image,0);

  return image;
}


/* FUNCTION DESCRIPTION **************************************************

IdImaSetValueAnyType (fonction)

RESUME: Initialisation d'une image UCHAR a une valeur donnee

DESCRIPTION:  Initialisation d'une image (non COMPLEX) a une valeur donnee

SYNTAXE:     PPIMAGE imResult = IdImaSetValueAnyType (PPIMAGE imResult, double valeur);

RETOUR:      type  : PPIMAG
             role  : Pointeur vers l'image initialisee. Zero si echec.

PARAMETRES:  nom   : imResult
             type  : PPIMAGE
             role  : Pointeur vers l'image a initialiser.

             nom   : valeur
             type  : double
             role  : valeur avec laquelle l'image sera initialisee.

FICHIER:     imaclear.c

EXEMPLE:     if ( IdImaSetValueAnyType(image,128)==0 )
                IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));

******************************************************** END DESCRIPTION */

#define TT(t1)                              \
          for (j=0;j<IdImaDimX(image);j++)  \
          {                                 \
           ((t1)image)[0][j]= valeur;       \
          }

PPIMAGE IdImaSetValueAnyType(image, valeur)
PPIMAGE image;
double valeur;
{
  int y;
  int j = 0;    /* FB 24-01-2000 */
  int taille_ligne;
               /* Controle de l'image **/
  if ( !image ) {
    IdErrno = IDERR_POINTER_IS_NULL;
    return (PPIMAGE)0;
   }

 taille_ligne =  IdImaDimX(image)*IdSizeOfType(image) ;

if ( (IdImaType(image) == IMA_UCHAR) || (IdImaType(image) == IMA_CHAR) ) 
 {

  for ( y=0; y<IdImaDimY(image); y++ )
    memset( image[y], valeur, taille_ligne );
  }

else                          /* autres Types */
 {

 switch(IdImaType(image))
    {
     case IMA_UCHAR:   TT(PPIMAGE_UCHAR); break;
     case IMA_CHAR:    TT(PPIMAGE_CHAR);  break;
     case IMA_USHORT:  TT(PPIMAGE_USHORT); break;
     case IMA_SHORT:   TT(PPIMAGE_SHORT);  break;
     case IMA_LONG:    TT(PPIMAGE_LONG);   break;
     case IMA_ULONG:   TT(PPIMAGE_ULONG);  break;
     case IMA_FLOAT:   TT(PPIMAGE_FLOAT);  break;
     case IMA_DOUBLE:  TT(PPIMAGE_DOUBLE); break;
     default:
      {
       IdPrintf("Type %d non traitable par la fonction IdImaSetValueAnyType !\n",IdImaType(image));
      }
    } /* Fin du switch */ 

 } /*fin else */


  for ( y=0; y<IdImaDimY(image); y++ )
    memcpy( image[y], image[0], taille_ligne );

IdImaSetUsedNbX(image,IdImaDimX(image));
IdImaSetUsedNbY(image,IdImaDimY(image));

  return image;
}


/* FUNCTION DESCRIPTION **************************************************

IdImaSetValue (fonction)

RESUME: Initialisation d'une image UCHAR a une valeur donnee

DESCRIPTION:  Initialisation d'une image UCHAR a une valeur donnee

SYNTAXE:     PPIMAGE_UCHAR imResult = IdImaSetValue (PPIMAGE_UCHAR imResult, int valeur);

RETOUR:      type  : PPIMAGE_UCHAR
             role  : Pointeur vers l'image initialisee. Zero si echec.

PARAMETRES:  nom   : imResult
             type  : PPIMAGE_UCHAR
             role  : Pointeur vers l'image a initialiser.

             nom   : valeur
             type  : int
             role  : valeur avec laquelle l'image sera initialisee.

FICHIER:     imaclear.c

EXEMPLE:     if ( IdImaSetValue(image,128)==0 )
       IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));

******************************************************** END DESCRIPTION */

PPIMAGE_UCHAR IdImaSetValue ( image , valeur)
PPIMAGE_UCHAR image;
{
  int y;
  int taille_ligne;
                                                 /* Controle de l'image **/
  if ( !image ) {                                /* Alloc. si necessaire ***/
    IdErrno = IDERR_POINTER_IS_NULL;
    return (PPIMAGE_UCHAR)0;
   } else if ( IdImaType(image) != IMA_UCHAR) {
    IdErrno = IDERR_WRONG_TYPE;
    return (PPIMAGE_UCHAR)0;
  }

  taille_ligne =  IdImaDimX(image)*IdSizeOfType(image) ;
                                                /*  Calcul du resultat *****/
  for ( y=0; y<IdImaDimY(image); y++ )
    memset( image[y], valeur, taille_ligne );

IdImaSetUsedNbX(image,IdImaDimX(image));
IdImaSetUsedNbY(image,IdImaDimY(image));

  return image;
}


/* FUNCTION DESCRIPTION **************************************************

IdImaCopy  (fonction)

RESUME: Copie une image ds une autre

DESCRIPTION: Copie les pixels d'une image dans une autre de meme type et
             memes dimensions.

SYNTAXE:     PPIMAGE imDest = IdImaCopy(PPIMAGE imSource, PPIMAGE imDest);

RETOUR:      type  : PPIMAGE
             role  : Pointeur vers l'image resultat (imd).

PARAMETRES:
             nom   : imSource
             type  : PPIMAGE
             role  : Image d'origine.

             nom   : imDest
             type  : PPIMAGE
             role  : Image destination de meme type et dim. que ims.

FICHIER:     imacast.c

EXEMPLE:     if( !IdImaCopy (ims,imd) ) IdErrPrintf(IdErrMsg(IdErrno));

******************************************************** END DESCRIPTION */
PPIMAGE IdImaCopy (ims,imd)
PPIMAGE ims,imd;
{
  int i;
  if ( !IdImaSameSizeAndType(ims,imd) )  {
    IdErrno = IDERR_WRONG_DIM;
    return 0;
  }
  for(i=0;i<IdImaDimY(imd);i++)
    memcpy (imd[i],ims[i],IdImaDimX(imd)*IdSizeOfType(imd));
  return imd;
}
