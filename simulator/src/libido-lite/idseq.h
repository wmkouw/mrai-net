/*************************************************************************
* $Id: idseq.h,v 1.1 2005/09/09 08:22:39 bellet Exp $
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

#ifndef __IDSEQ_H__
#define __IDSEQ_H__
/*************************************************************************
*
*  Description : Declarations diverses permettant d'utiliser le type SEQUENCE
*                de la bibliotheque LIBIDO
*
**************************************************************************/
#include "idgen.h"
#include "idvol.h"
#include "idpoint.h"

/* Types de sequence 4D */

#define SEQ_CHAR 	   (SEQ | TY_CHAR)
#define SEQ_UCHAR 	   (SEQ | TY_UCHAR)
#define SEQ_SHORT 	   (SEQ | TY_SHORT)
#define SEQ_USHORT     	   (SEQ | TY_USHORT)                        
#define SEQ_LONG 	   (SEQ | TY_LONG)
#define SEQ_ULONG 	   (SEQ | TY_ULONG)
#define SEQ_FLOAT 	   (SEQ | TY_FLOAT)
#define SEQ_DOUBLE 	   (SEQ | TY_DOUBLE)
#define SEQ_COMPLEX 	   (SEQ | TY_COMPLEX)
#define SEQ_RGB 	   (SEQ | TY_RGB)
#define SEQ_COMPLEX_DOUBLE (SEQ | TY_COMPLEX_DOUBLE)             
#define SEQ_COMPLEX_FLOAT  (SEQ | TY_COMPLEX_FLOAT )             
#define SEQ_COMPLEX_CHAR   (SEQ | TY_COMPLEX_CHAR  )             
#define SEQ_COMPLEX_UCHAR  (SEQ | TY_COMPLEX_UCHAR )             
#define SEQ_COMPLEX_SHORT  (SEQ | TY_COMPLEX_SHORT )             
#define SEQ_COMPLEX_USHORT (SEQ | TY_COMPLEX_USHORT)             
#define SEQ_COMPLEX_LONG   (SEQ | TY_COMPLEX_LONG  )             
#define SEQ_COMPLEX_ULONG  (SEQ | TY_COMPLEX_ULONG )
#define SEQ_BIT            (SEQ | TY_BIT           )             
#define SEQ_POINTER        (SEQ | TY_POINTER       )

/************************************************************************/

typedef           void **** PPPPSEQUENCE;                     
typedef           char **** PPPPSEQUENCE_CHAR;                
typedef  unsigned char **** PPPPSEQUENCE_UCHAR;               
typedef          float **** PPPPSEQUENCE_FLOAT;               
typedef         double **** PPPPSEQUENCE_DOUBLE;              
typedef        COMPLEX **** PPPPSEQUENCE_COMPLEX;             
typedef            RGB **** PPPPSEQUENCE_RGB;                 
typedef          short **** PPPPSEQUENCE_SHORT;               
typedef unsigned short **** PPPPSEQUENCE_USHORT;              
typedef           long **** PPPPSEQUENCE_LONG;                
typedef unsigned  long **** PPPPSEQUENCE_ULONG;
typedef COMPLEX_DOUBLE **** PPPPSEQUENCE_COMPLEX_DOUBLE;      
typedef  COMPLEX_FLOAT **** PPPPSEQUENCE_COMPLEX_FLOAT;       
typedef   COMPLEX_CHAR **** PPPPSEQUENCE_COMPLEX_CHAR;        
typedef  COMPLEX_UCHAR **** PPPPSEQUENCE_COMPLEX_UCHAR;       
typedef  COMPLEX_SHORT **** PPPPSEQUENCE_COMPLEX_SHORT;       
typedef COMPLEX_USHORT **** PPPPSEQUENCE_COMPLEX_USHORT;      
typedef   COMPLEX_LONG **** PPPPSEQUENCE_COMPLEX_LONG;        
typedef  COMPLEX_ULONG **** PPPPSEQUENCE_COMPLEX_ULONG;
typedef PPPPSEQUENCE_UCHAR    PPPPPPSEQUENCE_BIT;
typedef          void **** PPPPSEQUENCE_POINTER;             

/* structure SEQUENCE */                                           

typedef struct                                                   
	{ 
	  int   UsedNbT;	  
	  int   UsedNbZ;     /* JPR */
	  int   UsedNbY;
          int   UsedNbX;
          int   subObject;   /* JPR */
       	  int 	DimT;
       	  int 	DimZ;
	  int   DimY;
	  int   DimX;
          int   Numero;
	  char  *_message,*_fichier;
	  int   BitsPerPixel;        /* nombre de bits reellement utilises */
	  int   Type; /* IMPERATIVEMENT DERNIER DE LA STRUCTURE */

	} PRIVATE_SEQUENCE;                                        


#include "idseq-restricted.h"
       
#define _IdSeqPrivate(seq)   ( ((PRIVATE_SEQUENCE*)(seq) )-1 ) 
#ifndef SWIG
#define IdSeqDimX(sequence_) ( (_IdSeqPrivate(sequence_) )->DimX)  
#define IdSeqDimY(sequence_) ( (_IdSeqPrivate(sequence_) )->DimY)  
#define IdSeqDimZ(sequence_) ( (_IdSeqPrivate(sequence_) )->DimZ)
#define IdSeqDimT(sequence_) ( (_IdSeqPrivate(sequence_) )->DimT)

#define IdSeqUsedNbX(sequence_) ( (_IdSeqPrivate(sequence_) )->UsedNbX)
#define IdSeqUsedNbY(sequence_) ( (_IdSeqPrivate(sequence_) )->UsedNbY)
#define IdSeqUsedNbZ(sequence_) ( (_IdSeqPrivate(sequence_) )->UsedNbZ) 
#define IdSeqUsedNbT(sequence_) ( (_IdSeqPrivate(sequence_) )->UsedNbT) 

#define IdSeqSetUsedNbX(seq_,nb) ( _IdSeqPrivate(seq_)->UsedNbX=(nb) )
#define IdSeqGetUsedNbX(seq_) 	 ( _IdSeqPrivate(seq_)->UsedNbX)
#define IdSeqSetUsedNbY(seq_,nb) ( _IdSeqPrivate(seq_)->UsedNbY=(nb) )
#define IdSeqGetUsedNbY(seq_) 	 ( _IdSeqPrivate(seq_)->UsedNbY) 
#define IdSeqSetUsedNbZ(seq_,nb) ( _IdSeqPrivate(seq_)->UsedNbZ=(nb) )
#define IdSeqGetUsedNbZ(seq_) 	 ( _IdSeqPrivate(seq_)->UsedNbZ) 
#define IdSeqSetUsedNbT(seq_,nb) ( _IdSeqPrivate(seq_)->UsedNbT=(nb) )
#define IdSeqGetUsedNbT(seq_) 	 ( _IdSeqPrivate(seq_)->UsedNbT) 
                                                               
#define IdSeqGetBit(seq,x,y,z,t)   (((seq)[t][z][y][(x)>>3]>>((x)&7) )&1)
#define IdSeqPutBit(seq,x,y,z,t,n)                                    	\
         ( (n)                                                   	\
           ? ( ((seq)[t][z][y][x>>3])|= (1<<((x)&7) ) )      		\
           : ( ((seq)[t][z][y][x>>3])&=~(1<<((x)&7) ) )  )

#define IdSeqType(sequence_) ( (_IdSeqPrivate(sequence_) )->Type)  

#else
int IdSeqDimX(PPPPSEQUENCE);
int IdSeqDimY(PPPPSEQUENCE);
int IdSeqDimZ(PPPPSEQUENCE);
int IdSeqDimT(PPPPSEQUENCE);

int IdSeqUsedNbX(PPPPSEQUENCE);
int IdSeqUsedNbY(PPPPSEQUENCE);
int IdSeqUsedNbZ(PPPPSEQUENCE);
int IdSeqUsedNbT(PPPPSEQUENCE);

int IdSeqSetUsedNbX(PPPPSEQUENCE,int);
int IdSeqSetUsedNbY(PPPPSEQUENCE,int);
int IdSeqSetUsedNbZ(PPPPSEQUENCE,int);
int IdSeqSetUsedNbT(PPPPSEQUENCE,int);

int IdSeqGetUsedNbX(PPPPSEQUENCE);
int IdSeqGetUsedNbY(PPPPSEQUENCE);
int IdSeqGetUsedNbZ(PPPPSEQUENCE);
int IdSeqGetUsedNbT(PPPPSEQUENCE);

int IdSeqGetBit(PPPPSEQUENCE_UCHAR,int,int,int,int);
int IdSeqPutBit(PPPPSEQUENCE_UCHAR,int,int,int,int,int);

int IdSeqType(PPPPSEQUENCE);
#endif

PPPPSEQUENCE IdSeqVecteurToSequence (void *, int, int, int, int, int );

/* 
 *utilisation: 
 *
 *	#include <idseq.h>
 *	PPPPSEQUENCE_UCHAR vouser;
 *	vouser=(PPPPSEQUENCE_UCHAR)IdSeqAlloc(...); 
 *	.../...  
 *		vouser[i][j][k][l]=...
 *		IdSeqDimX(vouser) 
 *	.../...  
 *	IdSeqFree(vouser);
 */

/* Allocation d'une sequence identique a une autre */

#ifndef SWIG
#define IdSeqAllocLikeSequence(vo) ((!(vo) ) ? 0                            \
                                : IdSeqAlloc(IdSeqDimX(vo), IdSeqDimY(vo), \
                                             IdSeqDimZ(vo), IdSeqDimT(vo), \
					     IdSeqType(vo) ) )

/* Test d'egalite des dimensions et du type de deux sequences */

#define IdSeqSameSizeAndType(vo1,vo2) ((IdSeqDimX(vo1)==IdSeqDimX(vo2) ) && \
                                       (IdSeqDimY(vo1)==IdSeqDimY(vo2) ) && \
                                       (IdSeqDimZ(vo1)==IdSeqDimZ(vo2) ) && \
                                       (IdSeqDimT(vo1)==IdSeqDimT(vo2) ) && \
                                       (IdSeqType(vo1)==IdSeqType(vo2) ) )

/* Test d'egalite des dimensions de deux sequences */

#define IdSeqSameSize(vo1,vo2) 	      ((IdSeqDimX(vo1)==IdSeqDimX(vo2) ) && \
                                       (IdSeqDimY(vo1)==IdSeqDimY(vo2) ) && \
                                       (IdSeqDimZ(vo1)==IdSeqDimZ(vo2) ) && \
                                       (IdSeqDimT(vo1)==IdSeqDimT(vo2) ) )

/* Test d'egalite du type de deux sequences */

#define IdSeqSameType(vo1,vo2)  (IdSeqType(vo1)==IdSeqType(vo2) )

#else
PPPPSEQUENCE 	IdSeqAllocLikeSequence(PPPPSEQUENCE);
int 		IdSeqSameSizeAndType(PPPPSEQUENCE,PPPPSEQUENCE);
int 		IdSeqSameSize(PPPPSEQUENCE,PPPPSEQUENCE);
int 		IdSeqSameType(PPPPSEQUENCE,PPPPSEQUENCE);
#endif



/************************************************************************/


/* fonctions SEQUENCE */

PPPPSEQUENCE IdSeqAlloc	 	   (int, int, int, int, int); 
PPPPSEQUENCE IdSeqAllocSubSequence (PPPPSEQUENCE ,int, int, int, int,
                                    int, int, int ,int);
PPPPSEQUENCE IdSeqModifSubSequence (PPPPSEQUENCE, PPPPSEQUENCE, int, int,
                                    int, int);
#define IdSeqFree(pi) 		 _IdSeqFree((PPPPSEQUENCE*)&(pi) )
#define IdSeqFreeSubSequence(pi) _IdSeqFreeSubSequence((PPPPSEQUENCE*)&(pi) )
PPPPSEQUENCE IdSeqTabVolToSequence  (PPPVOLUME *,int);
PPPVOLUME *  IdSeqSequenceToTabVol  (PPPPSEQUENCE);

PPPPSEQUENCE       IdSeqClear       (PPPPSEQUENCE); 
PPPPSEQUENCE_UCHAR IdSeqSetValue    (PPPPSEQUENCE_UCHAR,int);                      
PPPPSEQUENCE IdSeqCopy        	     (PPPPSEQUENCE,PPPPSEQUENCE);
PPPPSEQUENCE IdSeqCast        	     (PPPPSEQUENCE,PPPPSEQUENCE);
PPPPSEQUENCE IdSeqCastReal    	     (PPPPSEQUENCE,PPPPSEQUENCE);
PPPPSEQUENCE IdSeqCastComplex	     (PPPPSEQUENCE,PPPPSEQUENCE);
         
PPPPSEQUENCE IdSeqCheckTypeSizeAlloc (PPPPSEQUENCE,int,PPPPSEQUENCE);
PPPPSEQUENCE IdSeqCheckSizeAlloc     (PPPPSEQUENCE,int,PPPPSEQUENCE);
PPPPSEQUENCE IdSeqCheckTypeAlloc     (PPPPSEQUENCE,int,PPPPSEQUENCE);
PPPPSEQUENCE IdSeqCheckSizeAllocType (PPPPSEQUENCE,int,PPPPSEQUENCE,int);
PPPPSEQUENCE IdSeqCheckTypeAllocSize (PPPPSEQUENCE,int,PPPPSEQUENCE,int,int,
                                      int,int);
void 	     IdSeqSwap               (PPPPSEQUENCE,int);
PPPPSEQUENCE IdSeqReadRawFile        (char *,int,int,int,int,int);
int          IdSeqReadRawFileToSeq   (char *, PPPPSEQUENCE);
PPPPSEQUENCE IdSeqReadRawFileWithOffset	(char *,int,int,int,int,int,int);
int          IdSeqReadRawFileToSeqWithOffset (char *, PPPPSEQUENCE, int);
int	     IdSeqWriteRawFile(char *, PPPPSEQUENCE);


/* RIEN DE TOUT CA N'EST FAIT ....
 *
 * PPPSEQUENCE_UCHAR IdSeqRecadTo8 (PPPPSEQUENCE,PPPPSEQUENCE_UCHAR,
 *                                 double,double);
 * PPPPSEQUENCE *     IdSeqSequenceToTabVol (PPPPSEQUENCE);
 *
 */

#endif


/* FUNCTION DESCRIPTION **************************************************

IdSeqDimX (macro)

RESUME: Donne la taille sur l'axe X de la Sequence LibIDO consideree.

DESCRIPTION: Donne la taille sur l'axe X de la Sequence LibIDO consideree.

SYNTAXE:     int dimx = IdSeqDimX ( PPPPSEQUENCE seq);

RETOUR:      type  : int
	     role  : taille en X de la sequence.

PARAMETRES:  nom   : seq
	     type  : objet sequence LibIDO 
	     role  : sequence LibIDO a scruter

FICHIER:     idseq.h

EXEMPLE:     

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdSeqDimT (macro)

RESUME: Donne la taille sur l'axe T de la Sequence LibIDO considereee.

DESCRIPTION: Donne la taille sur l'axe T de la Sequence LibIDO considereee.

SYNTAXE:     int dimT= IdSeqDimT ( PPPPSEQUENCE seq);

RETOUR:      type  : int
	     role  : taille en T de la sequence.

PARAMETRES:  nom   : seq
	     type  : objet sequence LibIDO 
	     role  : sequence LibIDO a scruter

FICHIER:     idseq.h

EXEMPLE:     

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdSeqDimY (macro)

RESUME: Donne la taille sur l'axe Y de la Sequence LibIDO considereee.

DESCRIPTION: Donne la taille sur l'axe Y de la Sequence LibIDO considereee.

SYNTAXE:     int dimY= IdSeqDimY (PPPPSEQUENCE seq);

RETOUR:      type  : int
	     role  : taille en Y de la sequence.

PARAMETRES:  nom   : seq
	     type  : objet sequence LibIDO 
	     role  : sequence LibIDO a scruter

FICHIER:     idseq.h

EXEMPLE:     

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdSeqDimZ (macro)

RESUME: Donne la taille sur l'axe Z de la Sequence LibIDO consideree.

DESCRIPTION: Donne la taille sur l'axe Z de la Sequence LibIDO consideree.

SYNTAXE:     int dimz= IdSeqDimZ (PPPPSEQUENCE seq);

RETOUR:      type  : int
	     role  : taille en Z de la sequence.

PARAMETRES:  nom   : seq
	     type  : objet sequence LibIDO 
	     role  : sequence LibIDO a scruter

FICHIER:     idseq.h

EXEMPLE:     

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdSeqGetUsedNbX (macro)

RESUME: Donne le nombre de pixels UTILISES sur l'axe x (horizontal) d'une Sequence.

DESCRIPTION: Donne le nombre de pixels UTILISES sur l'axe x (horizontal) d'une Sequence.

SYNTAXE:     int dimx = IdSeqGetUsedNbX (PPPPSEQUENCE Sequence);

RETOUR:      type  : int
             role  : taille en x de l'Sequence

PARAMETRES:  nom   : Sequence
             type  : objet Sequence Libido
             role  : Sequence LibIDO a scruter.

FICHIER:     idseq.h

EXEMPLE:     

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdSeqGetUsedNbY (macro)

RESUME: Donne le nombre de pixels UTILISES sur l'axe y (vertical) d'une Sequence.

DESCRIPTION: Donne le nombre de pixels UTILISES sur l'axe y (vertical) d'une Sequence.

SYNTAXE:     int dimy = IdSeqGetUsedNby (PPPPSEQUENCE Sequence);

RETOUR:      type  : int
             role  : taille en y de l'Sequence

PARAMETRES:  nom   : Sequence
             type  : objet Sequence Libido
             role  : Sequence LibIDO a scruter.

FICHIER:     idseq.h

EXEMPLE:     

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdSeqGetUsedNbZ (macro)

RESUME: Donne le nombre de pixels UTILISES sur l'axe z d'une Sequence.

DESCRIPTION: Donne le nombre de pixels UTILISES sur l'axe z d'une Sequence.

SYNTAXE:     int dimZ = IdSeqGetUsedNbZ (PPPPSEQUENCE Sequence);

RETOUR:      type  : int
             role  : taille en Z de la Sequence

PARAMETRES:  nom   : Sequence
             type  : objet Sequence Libido
             role  : Sequence LibIDO a scruter.

FICHIER:     idseq.h

EXEMPLE:     

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdSeqGetUsedNbT (macro)

RESUME: Donne le nombre de pixels UTILISES sur l'axe t d'une Sequence.

DESCRIPTION: Donne le nombre de pixels UTILISES sur l'axe t d'une Sequence.

SYNTAXE:     int dimT = IdSeqGetUsedNbZ (PPPPSEQUENCE Sequence);

RETOUR:      type  : int
             role  : taille en T de la Sequence

PARAMETRES:  nom   : Sequence
             type  : objet Sequence Libido
             role  : Sequence LibIDO a scruter.

FICHIER:     idseq.h

EXEMPLE:     

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdSeqSetUsedNbX (macro)

RESUME: Positionne le nombre de pixels UTILISES sur l'axe x (horizontal) d'une Sequence.

DESCRIPTION: Positionne le nombre de pixels UTILISES sur l'axe x (horizontal) d'une Sequence. A MANIER AVEC PRECAUTION !!!

SYNTAXE:     IdSeqSetUsedNbX (PPPPSEQUENCE Sequence, int nbXUtilises);

RETOUR:      rien

PARAMETRES:  nom   : Sequence
             type  : objet Sequence Libido
             role  : Sequence LibIDO a scruter.

	     nom   : nbXUtilises
             type  : int
             role  : nb pts UTILISES en x.

FICHIER:     idseq.h

EXEMPLE:     

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdSeqSetUsedNbY (macro)

RESUME: Positionne le nombre de pixels UTILISES sur l'axe y (vertical) d'une Sequence.

DESCRIPTION: Positionne le nombre de pixels UTILISES sur l'axe y (vertical) d'une Sequence. A MANIER AVEC PRECAUTION !!!

SYNTAXE:     IdSeqSetUsedNbY (PPPPSEQUENCE Sequence, int nbYUtilises);

RETOUR:      rien
             
PARAMETRES:  nom   : Sequence
             type  : objet Sequence Libido
             role  : Sequence LibIDO a scruter.

	     nom   : nbXUtilises
             type  : int
             role  : nb pts UTILISES en y.

FICHIER:     idseq.h

EXEMPLE:     

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdSeqSetUsedNbZ (macro)

RESUME: Positionne le nombre de pixels UTILISES sur l'axe z  d'une Sequence.

DESCRIPTION: Positionne le nombre de pixels UTILISES sur l'axe z  d'une Sequence. A MANIER AVEC PRECAUTION !!!

SYNTAXE:     IdSeqSetUsedNbZ (PPPPSEQUENCE Sequence, int nbZUtilises);

RETOUR:      rien
             
PARAMETRES:  nom   : Sequence
             type  : objet Sequence Libido
             role  : Sequence LibIDO a scruter.

	     nom   : nbXUtilises
             type  : int
             role  : nb pts UTILISES en Z.

FICHIER:     idseq.h

EXEMPLE:     

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdSeqSetUsedNbT (macro)

RESUME: Positionne le nombre de pixels UTILISES sur l'axe t  d'une Sequence.

DESCRIPTION: Positionne le nombre de pixels UTILISES sur l'axe t  d'une Sequence. A MANIER AVEC PRECAUTION !!!

SYNTAXE:     IdSeqSetUsedNbT (PPPPSEQUENCE Sequence, int nbZUtilises);

RETOUR:      rien
             
PARAMETRES:  nom   : Sequence
             type  : objet Sequence Libido
             role  : Sequence LibIDO a scruter.

	     nom   : nbXUtilises
             type  : int
             role  : nb pts UTILISES en T.

FICHIER:     idseq.h

EXEMPLE:     

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdSeqType (macro)

RESUME: Donne le type d'une sequence LibIDO

DESCRIPTION: Donne le type d'une sequence LibIDO

SYNTAXE:     int typ_seq = IdSeqType (PPPPSEQUENCE seq);

RETOUR:      type  : int
	     acces : O
	     role  : Type de la Sequence LibIDO. #defini par le tableau suivant :

             #define SEQ_CHAR           (SEQ | TY_CHAR)          
             #define SEQ_UCHAR          (SEQ | TY_UCHAR)         
             #define SEQ_SHORT          (SEQ | TY_SHORT)         
             #define SEQ_USHORT         (SEQ | TY_USHORT)        
             #define SEQ_LONG           (SEQ | TY_LONG)          
             #define SEQ_FLOAT          (SEQ | TY_FLOAT)         
             #define SEQ_DOUBLE         (SEQ | TY_DOUBLE)        
             #define SEQ_COMPLEX        (SEQ | TY_COMPLEX)       
             #define SEQ_RGB            (SEQ | TY_RGB)           
             #define SEQ_COMPLEX_DOUBLE (SEQ | TY_COMPLEX_DOUBLE)
             #define SEQ_COMPLEX_FLOAT  (SEQ | TY_COMPLEX_FLOAT )
             #define SEQ_COMPLEX_CHAR   (SEQ | TY_COMPLEX_CHAR  )
             #define SEQ_COMPLEX_UCHAR  (SEQ | TY_COMPLEX_UCHAR )
             #define SEQ_COMPLEX_SHORT  (SEQ | TY_COMPLEX_SHORT )
             #define SEQ_COMPLEX_USHORT (SEQ | TY_COMPLEX_USHORT)
             #define SEQ_COMPLEX_LONG   (SEQ | TY_COMPLEX_LONG  )
             #define SEQ_BIT            (SEQ | TY_BIT )          
                                                    
PARAMETRES:  nom   : seq
	     type  : Objet sequence LibIDO 
	     role  : Sequence LibIDO a scruter.

FICHIER:     idseq.h

******************************************************** END DESCRIPTION */


/* DEFINITION DESCRIPTION **************************************************

idseq.h (constantes)  

RESUME: constantes SEQ_??? pour definir le type de voxel des sequences a allouer

DESCRIPTION : constantes SEQ_??? utilisees dans les fonctions d'allocation 
              de sequences permettant de definir le type des voxels.           
              
           : IdSeqAlloc, IdSeqType, IdLibidoDataType.
           
           : SEQ_CHAR   : SEQ_DOUBLE         : SEQ_COMPLEX_UCHAR  :
           : SEQ_UCHAR  : SEQ_COMPLEX        : SEQ_COMPLEX_SHORT  :
           : SEQ_SHORT  : SEQ_RGB            : SEQ_COMPLEX_USHORT :
           : SEQ_USHORT : SEQ_COMPLEX_DOUBLE : SEQ_COMPLEX_LONG   :
           : SEQ_LONG   : SEQ_COMPLEX_FLOAT  : SEQ_BIT            :
           : SEQ_FLOAT  : SEQ_COMPLEX_CHAR   :
                    
              Chaque type (ie UCHAR) est decrit dans : IdLibidoDataType
                                                            
******************************************************** END DESCRIPTION */


/* DEFINITION DESCRIPTION **************************************************

idseq.h (types) 

RESUME: types predefinis des sequences (associes a IdLibidoDataType)

DEFINITION : types predefinis pour l'allocation des sequences, associes aux types
             predefinis par IdLibidoDataType.
             
          : IdSeqAlloc, IdLibidoDataType.

  : PPPPSEQUENCE         : PPPPSEQUENCE_RGB            : PPPPSEQUENCE_COMPLEX_CHAR   :
  : PPPPSEQUENCE_CHAR    : PPPPSEQUENCE_SHORT          : PPPPSEQUENCE_COMPLEX_UCHAR  :
  : PPPPSEQUENCE_UCHAR   : PPPPSEQUENCE_USHORT         : PPPPSEQUENCE_COMPLEX_SHORT  :
  : PPPPSEQUENCE_FLOAT   : PPPPSEQUENCE_LONG           : PPPPSEQUENCE_COMPLEX_USHORT :
  : PPPPSEQUENCE_DOUBLE  : PPPPSEQUENCE_COMPLEX_DOUBLE : PPPPSEQUENCE_COMPLEX_LONG   :
  : PPPPSEQUENCE_COMPLEX : PPPPSEQUENCE_COMPLEX_FLOAT  : PPPPPPSEQUENCE_BIT            :
                          
 PPPPSEQUENCE seul est de type void.

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdSeqGetBit (macro)

RESUME: Retourne la valeur du voxel de coordonnees x,y,z,b.

DESCRIPTION: Retourne la valeur du voxel de coordonnees x,y,z,b.

SYNTAXE:     int val = IdSeqGetBit(PPPPSEQUENCE_BIT seq,int x, int y, int z, int b);

RETOUR:      type  : int 
	     role  : 0 ou 1, selon la valeur du bit de la sequence.

PARAMETRES:  
	     nom   : seq
	     type  : PPPPSEQUENCE_BIT 
	     role  : Sequence de bit.

	     nom   : x, y, z, b
	     type  : int 
	     role  : Coordonnees x, y, z, b de la sequence.

FICHIER:     idseq.h

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdSeqPutBit (macro)

RESUME: Fixe la valeur d'un voxel d'un sequence binaire (PPPPSEQUENCE_BIT).

DESCRIPTION: Fixe la valeur d'un voxel d'un sequence binaire (PPPPSEQUENCE_BIT).

SYNTAXE:     void  IdSeqPutBit(PPPPSEQUENCE_BIT seq, int x, int y, int z, int valBit);

RETOUR:      type  : void 
	     role  : non defini.

PARAMETRES:  
	     nom   : seq
	     type  : PPPPSEQUENCE_BIT 
	     role  : Sequence de bit.

	     nom   : x, y, z
	     type  : int 
	     role  : Coordonnees x, y et z de la sequence.

	     nom   : n
	     type  : int 
	     role  : Imperativement 0 ou 1, pour regler la valeur du bit.

FICHIER:     idseq.h

******************************************************** END DESCRIPTION */
                          
/* FUNCTION DESCRIPTION **************************************************

IdSeqAllocLikeSequence (macro)

RESUME: Alloc. d'1 sequence de memes dimensions et type qu'1 autre

DESCRIPTION: Allocation d'un sequence de memes dimensions et meme type
	     qu'une autre.

SYNTAXE:     PPPPSEQUENCE seq2 = IdSeqAllocLikeSequence (PPPPSEQUENCE seq1);

RETOUR:      type  : PPPPSEQUENCE
	     role  : Pointeur vers le sequence alloue (a convertir en un type
		     valide : PPPPSEQUENCE_UCHAR par exemple ).

PARAMETRES:  nom   : seq1
	     type  : PPPPSEQUENCE, ou PPPPSEQUENCE_???
	     role  : Pointeur vers le sequence 'modele'.

FICHIER:     idseq.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdSeqSameSizeAndType (macro)

RESUME: Comparaison du type et des dimensions de deux sequences.

DESCRIPTION: Comparaison du type et des dimensions de deux sequences.

SYNTAXE:     int Booleen = IdSeqSameSizeAndType(PPPPSEQUENCE seq1, PPPPSEQUENCE seq2);

RETOUR:      type  : int
	     role  : Vrai si les deux sequences ont la meme dimension et
		     le meme type.

PARAMETRES:  nom   : seq1, seq2
	     type  : PPPPSEQUENCE, ou PPPPSEQUENCE_???
	     role  : Pointeurs vers les sequences a comparer.

FICHIER :    idseq.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdSeqSameSize (macro)

RESUME: Comparaison des dimensions de deux sequences.

DESCRIPTION: Comparaison des dimensions de deux sequences.

SYNTAXE:     int Booleen = IdSeqSameSize(PPPPSEQUENCE seq1, PPPPSEQUENCE seq2);

RETOUR:      type  : int
	     role  : Vrai si les deux sequences ont les memes dimensions

PARAMETRES:  nom   : seq1, seq2
	     type  : PPPPSEQUENCE, ou PPPPSEQUENCE_???
	     role  : Pointeurs vers les sequences a comparer.

FICHIER :    idseq.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdSeqSameType (macro)

RESUME: Comparaison des types de deux sequences.

DESCRIPTION: Comparaison des types de deux sequences.

SYNTAXE:     int Booleen = IdSeqSameType(PPPPSEQUENCE seq1, PPPPSEQUENCE seq2);

RETOUR:      type  : int
	     role  : Vrai si les deux sequences ont le meme type

PARAMETRES:  nom   : seq1, seq2
	     type  : PPPPSEQUENCE, ou PPPPSEQUENCE_???
	     role  : Pointeurs vers les sequences a comparer.

FICHIER :    idseq.h

******************************************************** END DESCRIPTION */
