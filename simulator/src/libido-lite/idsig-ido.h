/*************************************************************************
* $Id: idsig-ido.h,v 1.1 2005/09/09 08:22:40 bellet Exp $
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
*  Description : Declarations diverses permettant d'utiliser le type SIGNAL
*                de la bibliotheque LIBIDO
*
**************************************************************************/



#ifndef __IDSIG_IDO_H__
#define __IDSIG_IDO_H__


#include "idgen.h"
#include "idswig.h"

/* types des signaux 1d */

#define SIG_CHAR 	   (SIG | TY_CHAR)
#define SIG_UCHAR 	   (SIG | TY_UCHAR)
#define SIG_SHORT 	   (SIG | TY_SHORT)
#define SIG_USHORT 	   (SIG | TY_USHORT)                      
#define SIG_LONG 	   (SIG | TY_LONG)
#define SIG_ULONG 	   (SIG | TY_ULONG)
#define SIG_FLOAT 	   (SIG | TY_FLOAT)
#define SIG_DOUBLE 	   (SIG | TY_DOUBLE)
#define SIG_COMPLEX 	   (SIG | TY_COMPLEX)
#define SIG_RGB 	   (SIG | TY_RGB)
#define SIG_COMPLEX_DOUBLE (SIG | TY_COMPLEX_DOUBLE)
#define SIG_COMPLEX_FLOAT  (SIG | TY_COMPLEX_FLOAT )
#define SIG_COMPLEX_CHAR   (SIG | TY_COMPLEX_CHAR  )              
#define SIG_COMPLEX_UCHAR  (SIG | TY_COMPLEX_UCHAR )
#define SIG_COMPLEX_SHORT  (SIG | TY_COMPLEX_SHORT )
#define SIG_COMPLEX_USHORT (SIG | TY_COMPLEX_USHORT)              
#define SIG_COMPLEX_LONG   (SIG | TY_COMPLEX_LONG  )              
#define SIG_COMPLEX_ULONG  (SIG | TY_COMPLEX_ULONG )
#define SIG_POINTER        (SIG | TY_POINTER)

/* structure SIGNAL */

typedef           void *  PSIGNAL ;                        
typedef           char *  PSIGNAL_CHAR ;                   
typedef  unsigned char *  PSIGNAL_UCHAR  ;                  
typedef          float *  PSIGNAL_FLOAT ;                   
typedef         double *  PSIGNAL_DOUBLE ;                  
typedef        COMPLEX *  PSIGNAL_COMPLEX ;                 
typedef            RGB *  PSIGNAL_RGB ;                     
typedef          short *  PSIGNAL_SHORT ;                   
typedef unsigned short *  PSIGNAL_USHORT ;                  
typedef           long *  PSIGNAL_LONG ;                    
typedef  unsigned long *  PSIGNAL_ULONG ;
typedef COMPLEX_DOUBLE *  PSIGNAL_COMPLEX_DOUBLE ;          
typedef  COMPLEX_FLOAT *  PSIGNAL_COMPLEX_FLOAT ;           
typedef   COMPLEX_CHAR *  PSIGNAL_COMPLEX_CHAR ;            
typedef  COMPLEX_UCHAR *  PSIGNAL_COMPLEX_UCHAR ;           
typedef  COMPLEX_SHORT *  PSIGNAL_COMPLEX_SHORT ;           
typedef COMPLEX_USHORT *  PSIGNAL_COMPLEX_USHORT ;           
typedef   COMPLEX_LONG *  PSIGNAL_COMPLEX_LONG ;            
typedef  COMPLEX_ULONG *  PSIGNAL_COMPLEX_ULONG ;
typedef          void **  PSIGNAL_POINTER;
                       
typedef struct {
          int   UsedNbX;       /* JPR */
          int   __dum;    /* pour alignement frontiere Double Mot */
	  int   DimX;
          int   Numero;
	  char  *_message,*_fichier;
	  int   BitsPerPixel;        /* nombre de bits reellement utilises */
	  int   Type;       /* IMPERATIVEMENT DERNIER DE LA STRUCTURE */

	} PRIVATE_SIGNAL;                                          


#ifndef  SWIG
#define  _IdSigPrivate(sig) 	(((PRIVATE_SIGNAL*)(sig) )-1)

#define IdSigDimX(sig_) 	( _IdSigPrivate(sig_)->DimX)          
#define IdSigUsedNbX(sig_) 	( _IdSigPrivate(sig_)->UsedNbX)

#define IdSigSetUsedNbX(sig_,nb) ( _IdSigPrivate(sig_)->UsedNbX=(nb) )
#define IdSigGetUsedNbX(sig_) 	 ( _IdSigPrivate(sig_)->UsedNbX)

#define IdSigType(sig_) 	( _IdSigPrivate(sig_)->Type) 

#else 
int  IdSigDimX(PSIGNAL);
int  IdSigUsedNbX(PSIGNAL);
int  IdSigGetUsedNbX(PSIGNAL);
int  IdSigSetUsedNbX(PSIGNAL,int);
#endif



#endif
