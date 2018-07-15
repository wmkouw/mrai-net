/*************************************************************************
* $Id: idicont.h,v 1.1 2005/09/09 08:22:30 bellet Exp $
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
* PROJET        : LIBIDO           *
* NOM DU MODULE : idicont.h        * TYPE        : fichier inclus
* AUTEUR        : Clarysse P.      * CREATION    : 15/02/92
* VERSION       : 1.0              * REVISION    :    /  /
* LANGAGE       : C                * 
*                                  * 
**************************************************************************
* Insertion dans libido :
* par :                            le :
* virification libido   :
* par :                            le :
**************************************************************************
*
*  Description :  Declaration relatives au filtre IMACONT.C, FIMAEXTR.C,
*                 FIMAHYST.C de recherche de contours dans une image
*                 de gradient.
*
**************************************************************************
*
*  MODULES UTILISES :
*
**************************************************************************
*                     |              |
* REVISIONS :  DATE   | AUTEUR       |  DESCRIPTION
*---------------------|--------------|------------------------------------
*
**************************************************************************/
#ifndef __IDICONT_H__
#define __IDICONT_H__

#define FLT_ZERO   1e-18  /* Valeur Zero */
#define  NDGMAX    255    /* Niveau des points de contour */

/* Structure de base de type point */
struct point {
              short x;
              short y;
              short z;
              short val;
             };

typedef struct point * POINT_PTR;

/* Structure de noeud */
struct node {
             struct node * father;
             struct node * son;
             struct point * info;
            };

typedef struct node * NODE;


/* Structure de l'image tangente */

typedef struct __tgdir__{
                float tgalpha;
#ifdef SWIG
                unsigned int sens;
#else
                unsigned int sens:1;
#endif
               } TGDIR;

typedef TGDIR ** PPIMAGE_TGDIR;


/* Macros */
/* Fonction carree */
#define sqr(x) ((x) * (x))

/* Test par rapport au zero  machine */
#define TestZero(val) (fabs((double)(val)) < FLT_ZERO)

/* Test d'inclusion d'une variable entre 2 bornes */
#define TestDansInterval(x, i, s) ((x)>=(i) && (x)<=(s))

/* prototypes de FIMAEXTR.C */


PPIMAGE_TGDIR TangentImage     (PPIMAGE_FLOAT, PPIMAGE_FLOAT);
void          TangentImageFree (PPIMAGE_TGDIR, int);
int           CoTangentImage   (PPIMAGE_TGDIR, int, int);

#ifndef SWIG
PPIMAGE_UCHAR IdImaExtremSearch  (PPIMAGE_UCHAR, PPIMAGE_TGDIR, int);
#endif

       void  _IdFreeLinesOfPlane         (void**, int);

/* Prototypes de FIMAHYST.C */

int  BtScanTree    (NODE, PPIMAGE_UCHAR, PPIMAGE_UCHAR, PPIMAGE_TGDIR,
                    double, int [],long* BOTH, PPIMAGE_UCHAR);
int  PathTrace     (NODE, PPIMAGE_UCHAR);
int  ExistingPoint (NODE, struct point *, PPIMAGE_UCHAR);
NODE BtCreateNode  (NODE, size_t);
NODE BtFreeNode    (NODE);
#endif

