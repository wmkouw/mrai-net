/*************************************************************************
* $Id: imazoom.c,v 1.1 2005/09/09 08:22:50 bellet Exp $
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
*  Description : Module offrant la possibilite de changer la taille d'une
*                image (IdImaZoom) .
*
**************************************************************************/
#include "idima.h"
#include "iderr.h"
#include "idprint.h"


/* FUNCTION DESCRIPTION **************************************************

IdImaZoom   (fonction)

RESUME: Cree une copie d'une image en changeant ses dimensions.

DESCRIPTION: Cree une copie d'une image en changeant ses dimensions.

SYNTAXE:     PPIMAGE imRes = IdImaZoom ( PPIMAGE imRes, PPIMAGE imSource );

RETOUR:      type  : PPIMAGE
             acces : O
             role  : Pointeur vers l'image resultat, ou 0 si echec.

PARAMETRES:
             nom   : imout
             type  : PPIMAGE
             acces : I,O,N
             role  : Pointeur vers la nouvelle image. Doit etre alloue
                     avant l'appel a la fonction.

             nom   : imin
             type  : PPIMAGE
             acces : I,N
             role  : Pointeur vers l'image d'origine.

VERSION:     V1.00

FICHIER:     imazoom.c

EXEMPLE:     if( IdImaZoom(imout,imin) ) {
               IdErrPrintf("Erreur lors du changement d'echelle");
               IdExit(1);
             }

******************************************************** END DESCRIPTION */
PPIMAGE IdImaZoom (imout,imin)
PPIMAGE imout, imin;
{
/*                                       JPR  */
#define CC(t1) for (j=0; j<toy; j++)                                         \
           for (i=0; i<tox; i++)                                             \
           ((t1)imout)[j][i]=((t1)imin)[(int)(j*rapporty)][(int)(i*rapportx)];



  int curx=IdImaDimX (imin);
  int cury=IdImaDimY (imin);
  int tox=IdImaDimX (imout);
  int toy=IdImaDimY (imout);
  int i,j;

  float rapportx=((float)(curx))/tox;
  float rapporty=((float)(cury))/toy;
/* IdPrintf(" zoom x %f zoomy %f \n",rapportx,rapporty); */

if(IdImaType(imin)==IdImaType(imout))
{
 switch(IdImaType(imin))
 {
 case IMA_UCHAR:  CC(PPIMAGE_UCHAR);  break;
 case IMA_CHAR:   CC(PPIMAGE_CHAR);   break;
 case IMA_USHORT: CC(PPIMAGE_USHORT); break;
 case IMA_SHORT:  CC(PPIMAGE_SHORT);  break;
 case IMA_LONG:   CC(PPIMAGE_LONG);   break;
 case IMA_FLOAT:  CC(PPIMAGE_FLOAT);  break;
 case IMA_DOUBLE: CC(PPIMAGE_DOUBLE); break;
 case IMA_RGB:    CC(PPIMAGE_RGB); break;
 default:
  IdErrno=IDERR_WRONG_LIBTYPE;
  IdPrintf("Type %d non traite par IdImaZoom\n",IdImaType(imin));
   return 0;
 }
 return imout;
}
else
{  IdErrno=IDERR_WRONG_IMAGES;
   IdPrintf("images de type differents\n");
   return 0;
}

}

