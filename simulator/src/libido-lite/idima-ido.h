/*************************************************************************
* $Id: idima-ido.h,v 1.1 2005/09/09 08:22:32 bellet Exp $
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
*  Description : Declarations diverses permettant d'utiliser le type IMAGE
*                de la bibliotheque LIBIDO
*
**************************************************************************/



#ifndef __IDIMA_IDO_H__
#define __IDIMA_IDO_H__



/* Types d'images 2d */

#define IMA_CHAR           (IMA | TY_CHAR)
#define IMA_UCHAR          (IMA | TY_UCHAR)
#define IMA_SHORT          (IMA | TY_SHORT)
#define IMA_USHORT         (IMA | TY_USHORT)                      
#define IMA_LONG           (IMA | TY_LONG)
#define IMA_ULONG          (IMA | TY_ULONG)
#define IMA_FLOAT          (IMA | TY_FLOAT)
#define IMA_DOUBLE         (IMA | TY_DOUBLE)
#define IMA_COMPLEX        (IMA | TY_COMPLEX)
#define IMA_RGB            (IMA | TY_RGB)
#define IMA_RGBA           (IMA | TY_RGBA)
#define IMA_COMPLEX_DOUBLE (IMA | TY_COMPLEX_DOUBLE)              
#define IMA_COMPLEX_FLOAT  (IMA | TY_COMPLEX_FLOAT )              
#define IMA_COMPLEX_CHAR   (IMA | TY_COMPLEX_CHAR  )              
#define IMA_COMPLEX_UCHAR  (IMA | TY_COMPLEX_UCHAR )              
#define IMA_COMPLEX_SHORT  (IMA | TY_COMPLEX_SHORT )              
#define IMA_COMPLEX_USHORT (IMA | TY_COMPLEX_USHORT)              
#define IMA_COMPLEX_LONG   (IMA | TY_COMPLEX_LONG  )              
#define IMA_COMPLEX_ULONG  (IMA | TY_COMPLEX_ULONG )
#define IMA_BIT            (IMA | TY_BIT )
#define IMA_POINTER        (IMA | TY_POINTER)
#define IMA_COLOR          (IMA | TY_COLOR)


/* structure IMAGE */

typedef           void **  PPIMAGE ;
typedef           char **  PPIMAGE_CHAR ;
typedef  unsigned char **  PPIMAGE_UCHAR  ;
typedef          float **  PPIMAGE_FLOAT ;
typedef         double **  PPIMAGE_DOUBLE ;
typedef        COMPLEX **  PPIMAGE_COMPLEX ;
typedef            RGB **  PPIMAGE_RGB ;
typedef           RGBA **  PPIMAGE_RGBA ;
typedef          short **  PPIMAGE_SHORT ;
typedef unsigned short **  PPIMAGE_USHORT ;
typedef           long **  PPIMAGE_LONG ;
typedef unsigned  long **  PPIMAGE_ULONG ;                  
typedef COMPLEX_DOUBLE **  PPIMAGE_COMPLEX_DOUBLE ;
typedef  COMPLEX_FLOAT **  PPIMAGE_COMPLEX_FLOAT ;
typedef   COMPLEX_CHAR **  PPIMAGE_COMPLEX_CHAR ;
typedef  COMPLEX_UCHAR **  PPIMAGE_COMPLEX_UCHAR ;
typedef  COMPLEX_SHORT **  PPIMAGE_COMPLEX_SHORT ;
typedef COMPLEX_USHORT **  PPIMAGE_COMPLEX_USHORT ;
typedef   COMPLEX_LONG **  PPIMAGE_COMPLEX_LONG ;
typedef  COMPLEX_ULONG **  PPIMAGE_COMPLEX_ULONG ;
typedef     PPIMAGE_UCHAR  PPIMAGE_BIT ;
typedef        void   ***  PPIMAGE_POINTER;
typedef       COLOR    **  PPIMAGE_COLOR;


typedef struct {
          int   UsedNbY;
          int   UsedNbX;
          int   subObject;
          int   DimY;
          int   DimX;
          int   Numero;
          char  *_message,*_fichier;
          int   BitsPerPixel;       /* nombre de bits reellement utilises */
          int   Type;       		/* IMPERATIVEMENT DERNIER DE LA STRUCTURE */

        } PRIVATE_IMAGE;

#define   _IdImaPrivate(ima) (((PRIVATE_IMAGE*)(ima) )-1)


// PRIVATE_IMAGE accessors
int  IdImaUsedNbY      (void *);
int  IdImaGetUsedNbY   (void *);    // Alias of IdImaUsedNbY
int  IdImaUsedNbX      (void *);
int  IdImaGetUsedNbX   (void *);    // Alias of IdImaUsedNbX
int  IdImaDimY         (void *);
int  IdImaDimX         (void *);
int  IdImaBitsPerPixel (void *);
int  IdImaType         (void *);

void IdImaSetUsedNbY       (void *, int);
void IdImaSetUsedNbX       (void *, int);
void IdImaSetDimY          (void *, int);
void IdImaSetDimX          (void *, int);
void IdImaSetBitsPerPixel  (void *, int);
void IdImaSetType          (void *, int);


#endif
