/*************************************************************************
* $Id: idgen.h,v 1.2 2006/12/18 15:10:39 yougz Exp $
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

#ifndef __IDGEN_H__
#define __IDGEN_H__

/*
 * Types for complex number and RGB data structures
 */
 
typedef struct {double re,im ; }	COMPLEX		;   
typedef struct {double re,im ; }	COMPLEX_DOUBLE	;	 
typedef struct {float re,im; }		COMPLEX_FLOAT	;	 
typedef struct {char re,im; }		COMPLEX_CHAR	;	 
typedef struct {unsigned char re,im; }	COMPLEX_UCHAR	;	     
typedef struct {short re,im; }		COMPLEX_SHORT	;	 
typedef struct {unsigned short re,im; } COMPLEX_USHORT	;	 
typedef struct {long re,im; }		COMPLEX_LONG	;	 
typedef struct {unsigned long re,im; }	COMPLEX_ULONG	;

typedef struct {unsigned char r,g,b;}	 RGB;
typedef struct {unsigned char r,g,b,a;}	 RGBA;

#ifndef  SWIG 
typedef struct {unsigned char c1,c2,c3;} COLOR;
#endif

typedef struct {double x,y ; }		IDXPOINT_DOUBLE	;
typedef struct {float x,y; }		IDXPOINT_FLOAT	;	
typedef struct {char x,y; }		IDXPOINT_CHAR	;	
typedef struct {unsigned char x,y; }	IDXPOINT_UCHAR	;	    
typedef struct {short x,y; }		IDXPOINT_SHORT	;	
typedef struct {unsigned short x,y; }	IDXPOINT_USHORT	;	
typedef struct {long x,y; }		IDXPOINT_LONG	;	
typedef struct {unsigned long x,y; }	IDXPOINT_ULONG	;

/*  JM
Plans of the color image
*/
#define PLAN_1      1
#define PLAN_2      2
#define PLAN_3      3

#define SP_1    1<<(PLAN_1-1)     /* Select PLAN_1 */
#define SP_2    1<<(PLAN_2-1)     /* Select PLAN_2 */
#define SP_3    1<<(PLAN_3-1)     /* Select PLAN_3 */

/*
 * Types of IDO objets
 */

#define IDO_TYPE	0x0f

#define IMA 		0	/* images 2d  */
//// FIXME IDODUDE : how come IMA AND MAT HAVE THE SAME IDO TYPE
#define MAT	        0     	/* Matrices  */ 
#define SEQ	 	1	/* sequences (4D) */
#define VOL 		2	/* volumes 3d */
#define SIG 		3	/* signaux 1d */
#define PNT 		4	/* point 3D */
#define FAC 		5	/* facette 3D */
#define TRI 		6	/* triangulation 3D */
#define LST 		7	/* liste de n'importe quoi IMA..LST */
#define CNT		8       /* contour  */ 
#define CNT3D		9       /* contour 3D  */ 
  
/*
 * Types of the "pixels"
 */

#define TY_SIZE			(0x3f << 4)

#define TY_CHAR			( 0<<4)
#define TY_FLOAT		( 1<<4)
#define TY_DOUBLE		( 2<<4)
#define TY_COMPLEX		( 3<<4)
#define TY_RGB			( 4<<4)
#define TY_SHORT		( 5<<4)
#define TY_LONG			( 6<<4)
#define TY_UCHAR		( 7<<4) 
#define TY_USHORT		( 8<<4)				 

#define TY_COMPLEX_DOUBLE	( 9<<4)				 
#define TY_COMPLEX_FLOAT	(10<<4)				 
#define TY_COMPLEX_CHAR		(11<<4)				 
#define TY_COMPLEX_UCHAR	(12<<4)				 
#define TY_COMPLEX_SHORT	(13<<4)				 
#define TY_COMPLEX_USHORT	(14<<4)				 
#define TY_COMPLEX_LONG		(15<<4)				 
#define TY_BIT			(16<<4)
#define TY_IDPOINT		(17<<4)
#define TY_FACE			(18<<4)
#define TY_POINTER		(19<<4)
#define TY_ULONG		(20<<4)
#define TY_COMPLEX_ULONG	(21<<4)

#define TY_IDXPOINT_CHAR	(22<<4)
#define TY_IDXPOINT_UCHAR	(23<<4)
#define TY_IDXPOINT_FLOAT	(24<<4)
#define TY_IDXPOINT_DOUBLE	(25<<4)
#define TY_IDXPOINT_SHORT	(26<<4)
#define TY_IDXPOINT_USHORT	(27<<4)
#define TY_IDXPOINT_LONG	(28<<4)
#define TY_IDXPOINT_ULONG	(29<<4)

#define TY_COLOR		(30<<4)
#define TY_RGBA			(31<<4)

/*
 * Size of the pixels (size = TAille in French
 */

#define TA_TY_CHAR		sizeof(char)
#define TA_TY_UCHAR		sizeof(unsigned char)
#define TA_TY_FLOAT		sizeof(float)
#define TA_TY_DOUBLE		sizeof(double)
#define TA_TY_COMPLEX		(sizeof(COMPLEX))
#define TA_TY_RGB		(sizeof(RGB))
#define TA_TY_RGBA		(sizeof(RGBA))
#define TA_TY_SHORT		(sizeof(short))
#define TA_TY_USHORT		(sizeof(unsigned short))	 
#define TA_TY_LONG		(sizeof(long))
#define TA_TY_ULONG		(sizeof(unsigned long))
#define TA_TY_COMPLEX_DOUBLE	(sizeof(COMPLEX_DOUBLE ))	 
#define TA_TY_COMPLEX_FLOAT	(sizeof(COMPLEX_FLOAT  ))	 
#define TA_TY_COMPLEX_CHAR	(sizeof(COMPLEX_CHAR   ))	 
#define TA_TY_COMPLEX_UCHAR	(sizeof(COMPLEX_UCHAR  ))	 
#define TA_TY_COMPLEX_SHORT	(sizeof(COMPLEX_SHORT  ))	 
#define TA_TY_COMPLEX_USHORT	(sizeof(COMPLEX_USHORT ))	 
#define TA_TY_COMPLEX_LONG	(sizeof(COMPLEX_LONG   ))	 
#define TA_TY_COMPLEX_ULONG	(sizeof(COMPLEX_ULONG  ))
#define TA_TY_BIT		(sizeof(char))
#define TA_TY_POINTER		(sizeof(void *))
#define TA_TY_IDPOINT		(sizeof(IDPOINT))
#define TA_TY_FACE		(sizeof(FACE))

#define TA_TY_IDXPOINT_CHAR	(sizeof(IDXPOINT_CHAR))
#define TA_TY_IDXPOINT_UCHAR	(sizeof(IDXPOINT_UCHAR))
#define TA_TY_IDXPOINT_FLOAT	(sizeof(IDXPOINT_FLOAT))
#define TA_TY_IDXPOINT_DOUBLE	(sizeof(IDXPOINT_DOUBLE))
#define TA_TY_IDXPOINT_SHORT	(sizeof(IDXPOINT_SHORT))
#define TA_TY_IDXPOINT_USHORT	(sizeof(IDXPOINT_USHORT))
#define TA_TY_IDXPOINT_LONG	(sizeof(IDXPOINT_LONG))
#define TA_TY_IDXPOINT_ULONG	(sizeof(IDXPOINT_ULONG))

#define TA_TY_COLOR		(sizeof(COLOR))

#ifdef SWIG

int IdLibidoTypeObj(void *);
int IdLibidoType(void *);
int IdLibidoDataType(void *);
int IdSizeOfType(void *);
int IdTypeSize(int);

#else

#define IdLibidoTypeObj(obj)    ( ( (int*)(obj) )[-1] & IDO_TYPE )
#define IdLibidoType(obj)       ( ( (int*)(obj) )[-1] & IDO_TYPE )
#define IdLibidoDataType(obj)   ( ( (int*)(obj) )[-1] & TY_SIZE  )

#define IdSizeOfType(obj)	( (IdLibidoTypeObj(obj)!=CNT) ?  _IdSizeOfType ( IdLibidoDataType(obj) >> 4)	\
							     : (_IdSizeOfType ( IdLibidoDataType(obj) >> 4)) <<1\
				)

#define IdTypeSize(ty)      	( ((ty & IDO_TYPE)!=CNT) ?	_IdSizeOfType ( (ty & TY_SIZE)       >> 4)	\
							     : (_IdSizeOfType ( (ty & TY_SIZE)       >> 4)) <<1	\
				)
#endif

/************************************************************************/

unsigned short int IdTypeToComplex   (int);
unsigned short int IdTypeFromComplex (int);

int      IdCalcArrondInt (double); 
float    IdCalcFloatFromSun (float *);
double   IdModule(COMPLEX_FLOAT);
double   IdPhase(COMPLEX_FLOAT);

/************************************************************************/
/* fonctions "message" */

int     IdSetMessage (void *,char *);
char *  IdGetMessage (void *);

/************************************************************************/
/* fonctions "fichier" */

int     IdSetFileName (void *,char *);
char *  IdGetFileName (void *);


/*######################################################################*/

/* Fonctions internes */

void **_IdAllocPlan  (int, int, int);
void   _IdFreePlan   (void **, int);
int    _IdSizeOfType (int);

/************************************************************************/

#endif

/* FUNCTION DESCRIPTION **************************************************

IdLibidoTypeObj  (macro)

RESUME: Donne le type d'objet libido considere (IMA,SIG,VOL, ...)

DESCRIPTION: Donne le type d'objet libido considere.

SYNTAXE:     int type = IdLibidoTypeObj (void *objet);

RETOUR:      type  : int
	     role  : La valeur est decrite dans le tableau ci-dessous.

                     #define IDO_TYPE	0x0f

                     #define IMA 	0	images 2d  
                     #define SEQ	1	sequences (4D)  
                     #define VOL 	2	volumes 3d 
                     #define SIG 	3	signaux 1d 
                     #define PNT 	4	point 3D 
                     #define FAC 	5	facette 3D 
                     #define TRI 	6	triangulation 3D 
                     #define LST 	7	liste de n'importe quoi IMA..LST
                     #define CNT 	8	Contour (x,y)
                     #define CNT3D 	9	Contour 3D (ensemble de IDPOINT)


PARAMETRES:  nom   : objet
	     type  : objet libido (voir les differents types possibles
                     dans le tableau ci-dessous)

                     PPIMA_???
                     PPPPSEQ_???
                     PPPVOL_???
                     PSIG_???
                     PPOINT_???
                     PFACE_???
                     PTRIANGLE_???
                     PLIST_???

                     ??? pouvant varier de UCHAR a COMPLEX..., 
                     voir IdLibidoDataType.

	     role  : 
      
FICHIER:     IDGEN.H

EXEMPLE:     

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdLibidoDataType  (macro)

RESUME: Type des donnees d'1 objet LibIDO.  ex: IdLibidoDataType(pVol1) --> USHORT.

DESCRIPTION: Type des donnees d'1 objet LibIDO.  ex: IdLibidoDataType(pVol1) --> USHORT

SYNTAXE:     int type = IdLibidoDataType (void *objet);

RETOUR:      type  : int
	     acces : O
	     role  : La valeur est decrite dans le tableau ci-dessous :


       #define TY_SIZE           (0x3f << 4)
             
       #define TY_CHAR           ( 0<<4) : char
       #define TY_FLOAT          ( 1<<4) : float
       #define TY_DOUBLE         ( 2<<4) : double
       #define TY_COMPLEX        ( 3<<4) : complex 
       #define TY_RGB            ( 4<<4) : RGB     (---- ------- - -----)
       #define TY_SHORT          ( 5<<4) : short
       #define TY_LONG           ( 6<<4) : long
       #define TY_UCHAR          ( 7<<4) : unsigned char
       #define TY_USHORT         ( 8<<4) : unsigned short
       #define TY_COMPLEX_DOUBLE ( 9<<4) : complex double 
       #define TY_COMPLEX_FLOAT  (10<<4) : complex float  
       #define TY_COMPLEX_CHAR   (11<<4) : complex char   
       #define TY_COMPLEX_UCHAR  (12<<4) : complex unsigned char  
       #define TY_COMPLEX_SHORT  (13<<4) : complex short  
       #define TY_COMPLEX_USHORT (14<<4) : complex unsigned short 
       #define TY_COMPLEX_LONG   (15<<4) : complex long   
       #define TY_BIT            (16<<4) : bit
       #define TY_POINT          (17<<4) : point   (voir idpoint.h : types)
       #define TY_FACE           (18<<4) : facette (---- idface.h  : types)
       #define TY_POINTER        (19<<4) : pointeur 
       #define TY_ULONG          (20<<4) : unsigned long
       #define TY_COMPLEX_ULONG  (21<<4) : complex usigned long  
            
PARAMETRES:  nom   : objet
	     type  : objet libido (voir les differents types possibles
                     dans le tableau ci-dessous)

                     PPIMA_???
                     PPPPSEQ_???
                     PPPVOL_???
                     PSIG_???
                     PPOINT_???
                     PFACE_???
                     PTRIANGLE_???
                     PLIST_???

                     ??? pouvant varier de UCHAR a COMPLEX..., 
                     voir IdLibidoDataType.

	     role  : 
      
FICHIER:     IDGEN.H

EXEMPLE:     

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdSizeOfType  (macro)

RESUME: Taille en octets du type de donnees d'un objet LIBIDO ex: IdSizeOfType(pIma1) -->4

DESCRIPTION: Donne la taille en octets du type de donnees d'un objet 
             LibIDO.
		ex: IdSizeOfType(pIma1) -->4

SYNTAXE:     int taille = IdSizeOfType (void* objet);

RETOUR:      type  : int 
	     role  : taille en octets de l'objet LibIDO

PARAMETRES:  nom   : objet
	     type  : objet libido (voir les differents types possibles
                     dans le tableau ci-dessous)

                     PPIMA_???
                     PPPPSEQ_???
                     PPPVOL_???
                     PSIG_???
                     PPOINT_???
                     PFACE_???
                     PTRIANGLE_???
                     PLIST_???

                     ??? pouvant varier de UCHAR a COMPLEX..., 
                     voir IdLibidoDataType.

	     role  : 
      
FICHIER:     IDGEN.H

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdTypeSize  (macro)

RESUME: Donne la taille en octets d'un type de donnes possible LibIDO.

DESCRIPTION: Donne la taille en octets d'un type de donnes possible LibIDO.

SYNTAXE:     int nbOctets = IdTypeSize (int TYPE_LIBIDO);

RETOUR:      type  : int
	     role  : taille du type considere

PARAMETRES:  nom   : TYPE_LIBIDO
	     type  : constante a prendre dans le tableau suivant :

               TY_CHAR      TY_UCHAR            TY_COMPLEX_USHORT TY_COMPLEX_ULONG
               TY_FLOAT     TY_USHORT           TY_COMPLEX_LONG   
               TY_DOUBLE    TY_COMPLEX_DOUBLE   TY_BIT            
               TY_COMPLEX   TY_COMPLEX_FLOAT    TY_POINT          
               TY_RGB       TY_COMPLEX_CHAR     TY_FACE           
               TY_SHORT     TY_COMPLEX_UCHAR    TY_POINTER
               TY_LONG      TY_COMPLEX_SHORT    TY_ULONG
                   
	     role  : permet de connaitre la taille d'un type de donnees
                     (pouvant etre different sur differentes machines)

FICHIER:     IDGEN.H

******************************************************** END DESCRIPTION */
