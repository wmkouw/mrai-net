/*************************************************************************
* $Id: idseq-ido.h,v 1.1 2005/09/09 08:22:39 bellet Exp $
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

#ifndef __IDSEQ_IDO_H__
#define __IDSEQ_IDO_H__
/*************************************************************************
*
*  Description : Declarations diverses permettant d'utiliser le type SEQUENCE
*                de la bibliotheque LIBIDO
*
**************************************************************************/
#include "idgen.h"
#include "idseq-base.h"

/* Types de sequence 4D */

#define SEQ_CHAR 	   	(SEQ | TY_CHAR)
#define SEQ_UCHAR 	   	(SEQ | TY_UCHAR)
#define SEQ_SHORT 	   	(SEQ | TY_SHORT)
#define SEQ_USHORT     	(SEQ | TY_USHORT)                        
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

#define _IdSeqPrivate(seq)   ( ((PRIVATE_SEQUENCE*)(seq) )-1 ) 

#include "idseq-restricted.h"
       
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

#define IdSeqType(sequence_) ( (_IdSeqPrivate(sequence_) )->Type)  
                                                               
#define IdSeqGetBit(seq,x,y,z,t)   (((seq)[t][z][y][(x)>>3]>>((x)&7) )&1)

#define IdSeqPutBit(seq,x,y,z,t,n)                                    	\
         ( (n)                                                   	\
           ? ( ((seq)[t][z][y][x>>3])|= (1<<((x)&7) ) )      		\
           : ( ((seq)[t][z][y][x>>3])&=~(1<<((x)&7) ) )  )


#ifdef SWIG
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


#endif


