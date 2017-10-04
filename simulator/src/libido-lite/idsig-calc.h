/*************************************************************************
* $Id: idsig-calc.h,v 1.1 2005/09/09 08:22:40 bellet Exp $
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

#ifndef __IDSIG_CALC_H__
#define __IDSIG_CALC_H__

/*
 *  Description : SIGNAL related functions
 */
#include "idgen.h"
#include "idswig.h"

#include "idsig-ido.h"
                                        

/*
#define  _IdSigPrivate(sig) 	(((PRIVATE_SIGNAL*)(sig) )-1)

#define IdSigDimX(sig_) 	( _IdSigPrivate(sig_)->DimX)          
#define IdSigUsedNbX(sig_) 	( _IdSigPrivate(sig_)->UsedNbX)

#define IdSigSetUsedNbX(sig_,nb) ( _IdSigPrivate(sig_)->UsedNbX=(nb) )
#define IdSigGetUsedNbX(sig_) 	 ( _IdSigPrivate(sig_)->UsedNbX)

#define IdSigType(sig_) 	( _IdSigPrivate(sig_)->Type) 

#ifdef  SWIG
int  IdSigDimX(PSIGNAL);
int  IdSigUsedNbX(PSIGNAL);
int  IdSigGetUsedNbX(PSIGNAL);
int  IdSigSetUsedNbX(PSIGNAL,int);
#endif


*/

 
/************************************************************************/
/* fonctions SIGNAL */
void _IdSigFree (PSIGNAL *);

#define IdSigFree(si) _IdSigFree((PSIGNAL *)&(si) )  /* Anne Chretien */

#ifdef  SWIG
void  IdSigFree(PSIGNAL);
#endif

/* PSIGNAL  IdSigAllocSubSignal  (int, int); IMPOSSIBLE */
/* PSIGNAL  IdSigFreesSubSignal  (int, int); INUTILE    */

PSIGNAL        IdSigAlloc        (int, int);
void           IdSigFFT          (PSIGNAL_COMPLEX , int);
PSIGNAL_FLOAT  IdSigFloatDct1D   (PSIGNAL_FLOAT,int,int);
PSIGNAL_DOUBLE IdSigDoubleDct1D  (PSIGNAL_DOUBLE,int,int); 

PSIGNAL IdSigReadAscFile                 (char *, int ,int); 
int     IdSigReadAscFileToSig            (char *, PSIGNAL); 
int     IdSigReadAscFileToSigWithOffset  (char *, PSIGNAL,  int, int); 
PSIGNAL IdSigReadAscFileWithOffset       (char *, int, int, int, int); 

PSIGNAL IdSigReadRawFile                 (char *,int ,int); 
int     IdSigReadRawFileToSig            (char *,PSIGNAL); 
PSIGNAL IdSigReadRawFileWithOffset       (char *,int ,int ,int); 
int     IdSigReadRawFileToSigWithOffset  (char *,PSIGNAL, int); 
int     IdSigWriteRawFile                (char *,PSIGNAL); 
int IdPalWriteAsciiFile					 (char *, PSIGNAL_RGB);

double  IdSigMax          (PSIGNAL);
double  IdSigMin          (PSIGNAL);
int     IdSigMinMax       (PSIGNAL, double * OUTPUT, double * OUTPUT);
int     IdSigMinMaxInterv (PSIGNAL, int, int, double * OUTPUT, double * OUTPUT);
PSIGNAL IdSigClear        (PSIGNAL);

int     IdSigGetRawFileLength  (char *,int);
int     IdSigGetAscFileLength  (char * ,int ,int);
void    IdSigSwap              (PSIGNAL,int);
PSIGNAL IdSigVecteurToSignal   (void *, int, int, int );
PSIGNAL IdSigModifLongueur     (PSIGNAL,int );

PSIGNAL IdSigCopy 	       (PSIGNAL,PSIGNAL);
PSIGNAL IdSigCastComplex       (PSIGNAL,PSIGNAL);
PSIGNAL IdSigCastReal          (PSIGNAL,PSIGNAL);
PSIGNAL IdSigCast              (PSIGNAL,PSIGNAL);

PSIGNAL_RGB IdPalGreyCol            (void);
PSIGNAL_RGB IdPalGrey64             (void);
PSIGNAL_RGB IdPalFalsCol            (void);
PSIGNAL_RGB IdPalGreyRedCol         (void);
PSIGNAL_RGB IdPalHotMetal           (void);
PSIGNAL_RGB IdPalHotMetal128        (void);
PSIGNAL_RGB IdPalHotMetal64         (void);
PSIGNAL_RGB IdPalHotMetal32         (void);
PSIGNAL_RGB IdPalHotMetal16         (void);
PSIGNAL_RGB IdPalHotMetal8          (void);
PSIGNAL_RGB IdPalAnalyseStd         (void);
PSIGNAL_RGB IdPalAnalyseRet         (void);
PSIGNAL_RGB IdPalGreyRedToYellowCol (int);

#ifndef  SWIG 
#define IdPalFalseCol IdPalFalsCol
#endif

long int IdSigHistogram     (PSIGNAL_UCHAR, long int *, int, int);

PSIGNAL_UCHAR IdSigMedian    (PSIGNAL_UCHAR, PSIGNAL_UCHAR, int);
PSIGNAL_UCHAR  IdSigConvolve (PSIGNAL_UCHAR,PSIGNAL_FLOAT,PSIGNAL_UCHAR,
                              double,double);

PSIGNAL IdSigCheckTypeSizeAlloc (PSIGNAL,int,PSIGNAL);
PSIGNAL IdSigCheckSizeAlloc     (PSIGNAL,int,PSIGNAL);
PSIGNAL IdSigCheckTypeAlloc     (PSIGNAL,int,PSIGNAL);
PSIGNAL IdSigCheckSizeAllocType (PSIGNAL,int,PSIGNAL,int);
PSIGNAL IdSigCheckTypeAllocSize (PSIGNAL,int,PSIGNAL,int);

PSIGNAL_RGB IdPalReadAsciiFile  (char *);
int         IdPalWriteAsciiFile (char *, PSIGNAL_RGB);
PSIGNAL     IdSigMedianGene     (PSIGNAL, PSIGNAL, int);
PSIGNAL_USHORT IdSigLutRMNWL    (PSIGNAL_USHORT,int,int);
PSIGNAL_USHORT IdSigLutRMNMinMax(PSIGNAL_USHORT,int,int);
PSIGNAL_UCHAR IdSigLut8WL       (PSIGNAL_UCHAR, int,int);
PSIGNAL_UCHAR IdSigLut8MinMax   (PSIGNAL_UCHAR, int,int);

void IdSigGammaFit              (double*, double*, double*, double*,
                                 short int*, double* OUTPUT, double**);

/*************************************************************************/

/* fonctions internes */



#define IdSigSameSizeAndType(i1,i2)  (     (i1)&&(i2)                       \
                                       && (IdSigDimX(i1)==IdSigDimX(i2) )   \
                                       && (IdSigType(i1)==IdSigType(i2) ) )
#ifdef  SWIG
int  IdSigSameSizeAndType(PSIGNAL,PSIGNAL);
#endif


#define IdSigSameSize(i1,i2)  (     (i1)&&(i2)				\
                                       && (IdSigDimX(i1)==IdSigDimX(i2) ) )   
#ifdef  SWIG
int  IdSigSameSize(PSIGNAL,PSIGNAL);
#endif

#define IdSigSameType(i1,i2)  (    (i1)&&(i2)                       \
                                && (IdSigType(i1)==IdSigType(i2) ) )

#ifdef  SWIG
int  IdSigSameType(PSIGNAL,PSIGNAL);
#endif


#define IdSigAllocLikeSignal(im) ((!(im)) ? 0			\
                                : IdSigAlloc ( IdSigDimX(im),	\
                                             IdSigType(im) ) )
#ifdef  SWIG
PSIGNAL  IdSigAllocLikeSignal(PSIGNAL);
#endif



/* FUNCTION DESCRIPTION **************************************************

IdSigDimX (macro)

RESUME: donne la taille d'un objet signal LibIDO (nombre d'echantillons)

DESCRIPTION: donne la taille d'un objet signal LibIDO (nombre d'echantillons)

SYNTAXE:     int taille = IdSigDimX (SIGNAL sig);

RETOUR:      type  : int
	     role  : taille du signal.

PARAMETRES:  nom   : sig
	     type  : objet signal LibIDO 
	     role  : Signal LibIDO a scruter.

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdSigType (macro)

RESUME: Donne le type d'un signal LibIDO.

DESCRIPTION: Donne le type d'un signal LibIDO.

SYNTAXE:     int typ_sig = IdSigType (SIGNAL sig);

RETOUR:      type  : int
	     role  : Type du signal LibIDO. #defini par le tableau suivant :

             #define SIG_CHAR           (SIG | TY_CHAR)
             #define SIG_UCHAR          (SIG | TY_UCHAR)
             #define SIG_SHORT          (SIG | TY_SHORT)
             #define SIG_USHORT         (SIG | TY_USHORT)  
             #define SIG_ULONG          (SIG | TY_ULONG)
             #define SIG_LONG           (SIG | TY_LONG)
             #define SIG_FLOAT          (SIG | TY_FLOAT)
             #define SIG_DOUBLE         (SIG | TY_DOUBLE)
             #define SIG_COMPLEX        (SIG | TY_COMPLEX)
             #define SIG_RGB            (SIG | TY_RGB)
             #define SIG_COMPLEX_DOUBLE (SIG | TY_COMPLEX_DOUBLE) 
             #define SIG_COMPLEX_FLOAT  (SIG | TY_COMPLEX_FLOAT )       
             #define SIG_COMPLEX_CHAR   (SIG | TY_COMPLEX_CHAR  )       
             #define SIG_COMPLEX_UCHAR  (SIG | TY_COMPLEX_UCHAR )       
             #define SIG_COMPLEX_SHORT  (SIG | TY_COMPLEX_SHORT )       
             #define SIG_COMPLEX_USHORT (SIG | TY_COMPLEX_USHORT)       
             #define SIG_COMPLEX_LONG   (SIG | TY_COMPLEX_LONG  )
             #define SIG_COMPLEX_ULONG  (SIG | TY_COMPLEX_ULONG )        
             #define SIG_POINTER        (SIG | TY_POINTER       )                                                           
PARAMETRES:  nom   : sig
	     type  : Objet Signal LibIDO 
	     role  : Signal a scruter

******************************************************** END DESCRIPTION */


/* DEFINITION DESCRIPTION **************************************************

RESUME: constantes SIG_??? utilisees pour allouer des signaux

DESCRIPTION : constantes SIG_??? utilisees dans les fonctions d'allocation 
              de signaux permettant de definir le type des echantillons.

           : IdSigAlloc, IdSigType, IdLibidoDataType.

           : SIG_CHAR   : SIG_DOUBLE         : SIG_COMPLEX_CHAR   :
           : SIG_UCHAR  : SIG_COMPLEX        : SIG_COMPLEX_UCHAR  :
           : SIG_SHORT  : SIG_RGB            : SIG_COMPLEX_SHORT  :
           : SIG_USHORT : SIG_COMPLEX_DOUBLE : SIG_COMPLEX_USHORT :
           : SIG_LONG   : SIG_COMPLEX_FLOAT  : SIG_COMPLEX_LONG   :
           : SIG_FLOAT  : SIG_POINTER
                     
           Chaque type (ie UCHAR) est decrit dans : IdLibidoDataType
                     
******************************************************** END DESCRIPTION */


/* DEFINITION DESCRIPTION **************************************************


RESUME: types predefinis pour les signaux (associes a IdLibIdoDataType)

DESCRIPTION : types predefinis pour l'allocation des signaux, associes aux
              types predefinis par IdLibIdoDataType.

           : IdSigAlloc, IdLibidoDataType.

    : PSIGNAL         : PSIGNAL_RGB            : PSIGNAL_COMPLEX_CHAR   :
    : PSIGNAL_CHAR    : PSIGNAL_SHORT          : PSIGNAL_COMPLEX_UCHAR  :
    : PSIGNAL_UCHAR   : PSIGNAL_USHORT         : PSIGNAL_COMPLEX_SHORT  :
    : PSIGNAL_FLOAT   : PSIGNAL_LONG           : PSIGNAL_COMPLEX_USHORT :
    : PSIGNAL_DOUBLE  : PSIGNAL_COMPLEX_DOUBLE : PSIGNAL_COMPLEX_LONG   :
    : PSIGNAL_COMPLEX : PSIGNAL_COMPLEX_FLOAT  : PSIGNAL_POINTER
                          
       PSIGNAL seul est de type void.

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdSigSameSizeAndType (macro)

RESUME: Comparaison du type et des dimensions de deux signaux.

DESCRIPTION: Comparaison du type et des dimensions de deux signaux.

SYNTAXE:     int retCode = IdSigSameSizeAndType(PSIGNAL signal1, PSIGNAL signal2);

RETOUR:      type  : int
             role  : Vrai si les deux signaux ont la meme dimension le meme type.

PARAMETRES:  nom   : signal1, signal2
             type  : PSIGNAL, ou PSIGNAL_???
             role  : Pointeurs vers les signaux a comparer.

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdSigSameSize (macro)

RESUME: Comparaison des dimensions de deux signaux.

DESCRIPTION: Comparaison des dimensions de deux signaux.

SYNTAXE:     int retCode = IdSigSameSize(PSIGNAL signal1, PSIGNAL signal2);

RETOUR:      type  : int
             role  : Vrai si les deux signaux ont les memes dimensions

PARAMETRES:  nom   : signal1, signal2
             type  : PSIGNAL, ou PSIGNAL_???
             role  : Pointeurs vers les signaux a comparer.

FICHIER :    .h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdSigSameType (macro)

RESUME: Comparaison du type de deux signaux.

DESCRIPTION: Comparaison du type de deux signaux.

SYNTAXE:     int retCode = IdSigSameType(PSIGNAL signal1, PSIGNAL signal2);

RETOUR:      type  : int
             role  : Vrai si les deux signaux ont le meme type.

PARAMETRES:  nom   : signal1, signal2
             type  : PSIGNAL, ou PSIGNAL_???
             role  : Pointeurs vers les signaux a comparer.

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdSigAllocLikeSignal (macro)

RESUME: Alloc. d'1 signal de memes dimensions et type qu'1 autre

DESCRIPTION: Allocation d'un signal de memes dimensions et meme type qu'un autre.

SYNTAXE:     PSIGNAL imDest =  IdSigAllocLikeSignal (PSIGNAL imSource);

RETOUR:      type  : PSIGNAL
             role  : Pointeur vers le signal alloue (a convertir en un type valide : PSIGNAL_UCHAR par exemple ).

PARAMETRES:  nom   : signal1
             type  : PSIGNAL, ou PSIGNAL_???
             role  : Pointeur vers le signal 'modele'.

******************************************************** END DESCRIPTION */


#endif
