/*************************************************************************
* $Id: cntutil.c,v 1.1 2005/09/09 08:22:23 bellet Exp $
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
*  Description : Divers Utilitaires sur CONTOUR.
*
**************************************************************************/
#include <math.h>
#include <string.h>  // For memcpy() and memset()
#include <stdlib.h>  // For abs()
#include "idcnt.h"
#include "idcalc.h"
#include "iderr.h"
#include "idprint.h"

/* FUNCTION DESCRIPTION **************************************************

IdCntAddPoint (macro)
  
RESUME: Ajoute un point a un contour USHORT

DESCRIPTION:	ajoute un point a un contour USHORT. UsedNbX est mis a jour. 
		Le contour est re-alloue a une longueur effective plus grande si besoin est. 
		-- ATTENTION -- 
		Si le CNT est associe a un Graph (LibIDO Graphique)
		NE PAS UTILISER cette fonction
		(le Graph ne sera pas informe du 'changement d'adresse'
		 de son CNT en cas de realloc ... )
		Utiliser ds ce cas la fonction LibIDO Graphique IwGraphAddPoint
		qui prend en compte ce genre de pb...


SYNTAXE:     int retCode = IdCntAddPoint(PCONTOUR_USHORT cnt, int x, int y);

RETOUR:      type  : int
	     role  : 

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR_USHORT
	     role  : contour d'origine.

	     nom   : x,y
	     type  : int
	     role  : coordonnes X et Y du point a rajouter

FICHIER:   idcnt.h 

EXEMPLE:  
   
******************************************************** END DESCRIPTION */

int _IdCntAddPoint(contour,valx,valy) 
PCONTOUR_USHORT *contour;
int valx,valy;
{
if( IdCntUsedNbX(*contour) == IdCntDimX(*contour) ) 
		*contour=(PCONTOUR_USHORT)IdCntModifLongueur(*contour, (IdCntDimX(*contour)*3)/2 +1); /* +1 .... A. ANWANDER */
 
if (*contour==NULL) return 0;

IdCntSetX(*contour,IdCntUsedNbX(*contour),valx);
IdCntSetY(*contour,IdCntUsedNbX(*contour),valy);

IdCntUsedNbX(*contour)++; 

return(1);
}

/* FUNCTION DESCRIPTION **************************************************

IdCntInsertPoint (macro)	
  
RESUME: Insere un point ds un contour USHORT

DESCRIPTION:	Insere 1 point ds 1 contour USHORT APRES un point de numero donne.
		-1 signifie que l'on ajoute en tete
		UsedNbX est mis a jour. 
		Le contour est re-alloue a une longueur effective plus grande si besoin est. 
		-- ATTENTION -- 
		Si le CNT est associe a un Graph (LibIDO Graphique)
		NE PAS UTILISER cette fonction
		(le Graph ne sera pas informe du 'changement d'adresse'
		 de son CNT en cas de realloc ... )
		Utiliser ds ce cas la fonction LibIDO Graphique IwGraphAddPoint
		qui prend en compte ce genre de pb...

SYNTAXE:    int retCode = IdCntInsertPoint(PCONTOUR_USHORT cnt, int numPoint, int x, int y);

RETOUR:      type  : int
	     role  : code de retour booleen.

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR_USHORT
	     role  : contour d'origine.

	     nom   : numPoint
	     type  : int
	     role  : numero du point APRES lequel on veut rajouter

	     nom   : x,y
	     type  : int
	     role  : coordonnes X et Y du point a rajouter

FICHIER:   idcnt.h 

EXEMPLE:  
   
******************************************************** END DESCRIPTION */

int _IdCntInsertPoint(contour,numPoint,valx,valy) 
PCONTOUR_USHORT *contour;
int valx,valy,numPoint;
{
int i;
  if ( !*contour ) {                                  
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }else if ( IdLibidoType(*contour) != CNT) {
    IdErrno = IDERR_WRONG_TYPE;
    return 0;
	}

/*IdPrintf ("numero du point apres lequel inserer :%d\n",numPoint);*/

if (numPoint == IdCntUsedNbX(*contour))
   {  

/*IdPrintf("numPoint= dernier point\n");*/
	i = IdCntAddPoint(*contour,valx,valy);
	if (i==0) { IdErrPrintf("Echec Insertion Point\n");
		    return(0);
		  }
	return(1);
   }			
else if ( numPoint <-1)  { 	IdErrno = IDERR_INVALID_NUMBER;
				return 0;
			   }
else 
{	if ( IdCntDimX(*contour) == IdCntUsedNbX(*contour))
	{ 
		/* Rallonger le Contour */

	 *contour =(PCONTOUR_USHORT) IdCntModifLongueur( *contour,(IdCntDimX(*contour)*3)/2+1);


	} 		
	for (i=IdCntUsedNbX(*contour)-1 ; i>numPoint ; i--)

	{ 
	  IdCntSetX(*contour,i+1,IdCntGetX(*contour,i));
	  IdCntSetY(*contour,i+1,IdCntGetY(*contour,i));
	}

	IdCntSetX(*contour,numPoint+1,valx);
	IdCntSetY(*contour,numPoint+1,valy);

	IdCntSetUsedNbX(*contour,IdCntGetUsedNbX(*contour)+1);
}
return(1);
}

/* FUNCTION DESCRIPTION **************************************************

IdCntMoveContour (fonction)		
  
RESUME: Deplace un contour USHORT ( +DeltaX, +DeltaY sur chaque point)

DESCRIPTION:Deplace un contour USHORT ( +DeltaX, +DeltaY sur chaque point) 

SYNTAXE:     int retCode = IdCntMoveContour(PCONTOUR_USHORT cnt, int DeltaX, int DeltaY);

RETOUR:      type  : int
	     role  : code de retour booleen.

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR_USHORT
	     role  : contour d'origine.

	     nom   : DeltaX,DeltaY
	     type  : int
	     role  : deplacement, en X et Y, des points

FICHIER:   idcnt.h 

EXEMPLE:  
   
******************************************************** END DESCRIPTION */
int IdCntMoveContour(contour,deltaX, deltaY) 
PCONTOUR_USHORT contour;
int deltaX, deltaY;
{
 int i;
  if ( !contour ) {                                  
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
    }

  for (i=0;i<IdCntUsedNbX(contour);i++)
	{ 
	IdCntSetX(contour,i,IdCntGetX(contour,i)+deltaX);
	IdCntSetY(contour,i,IdCntGetY(contour,i)+deltaY);
	}
return 1;
}

/* FUNCTION DESCRIPTION **************************************************

IdCntRotateContour (fonction)	
  
RESUME: Fait tourner d'un angle donne un CNT USHORT autour d'un centre 

DESCRIPTION: Fait tourner d'un angle donne un CNT USHORT autour d'un centre 

SYNTAXE:     int retCode = IdCntRotateContour(PCONTOUR_USHORT cnt, int centreX, int centreY, double angle);

RETOUR:      type  : int
	     role  : code de retour booleen.

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR_USHORT
	     role  : contour d'origine.

	     nom   : centreX,centreY
	     type  : int
	     role  : coordonnees du centre de rotation

	     nom   : angle
	     type  : double
	     role  : angle de rotation

FICHIER:   cntutil.c

EXEMPLE:  
   
******************************************************** END DESCRIPTION */
int IdCntRotateContour(contour, centreX, centreY, angle) 
PCONTOUR_USHORT contour;
int centreX, centreY;
double angle;
{
double x,y;
double sinAlph, cosAlph;
double nouvX,nouvY;

 int i;
  if ( !contour ) {                                  
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
    }
sinAlph=sin(angle);
cosAlph=cos(angle);

  for (i=0;i<IdCntUsedNbX(contour);i++)
	{
 	x= (double)IdCntGetX(contour,i);
 	y= (double)IdCntGetY(contour,i);

	nouvX= cosAlph*(x-(double)centreX) -sinAlph*(y-(double)centreY) + centreX;
	nouvY= sinAlph*(x-(double)centreX) +cosAlph*(y-(double)centreY) + centreY;

	IdCntSetX(contour,i,(unsigned short)nouvX);
	IdCntSetY(contour,i,(unsigned short)nouvY);

 	x= (double)IdCntGetX(contour,i);
 	y= (double)IdCntGetY(contour,i);
	}
return 1;
}

/* FUNCTION DESCRIPTION **************************************************

IdCntRotateContourDouble (fonction)	
  
RESUME: Fait tourner d'un angle donne un CNT DOUBLE autour d'un centre 

DESCRIPTION: Fait tourner d'un angle donne un CNT DOUBLE autour d'un centre 

SYNTAXE:     int retCode = IdCntRotateContourDouble(PCONTOUR_DOUBLE cnt, int centreX, int centreY, double angle);

RETOUR:      type  : int
	     role  : code de retour booleen.

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR_DOUBLE
	     role  : contour d'origine.

	     nom   : centreX,centreY
	     type  : int
	     role  : coordonnees du centre de rotation

	     nom   : angle
	     type  : double
	     role  : angle de rotation

FICHIER:   cntutil.c

EXEMPLE:  
   
******************************************************** END DESCRIPTION */
int IdCntRotateContourDouble(contour, centreX, centreY, angle) 
PCONTOUR_DOUBLE contour;
int centreX, centreY;
double angle;
{
double x,y;
double sinAlph, cosAlph;
double nouvX,nouvY;

 int i;
  if ( !contour ) {                                  
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
    }
sinAlph=sin(angle);
cosAlph=cos(angle);

/*IdPrintf ("alpha %g sinApha %g cosAlpha %g\n",angle, sinAlph, cosAlph);*/

  for (i=0;i<IdCntUsedNbX(contour);i++)
	{
 	x= IdCntGetX(contour,i);
 	y= IdCntGetY(contour,i);

	nouvX= cosAlph*(x-(double)centreX) - sinAlph*(y-(double)centreY) + centreX;
	nouvY= sinAlph*(x-(double)centreX) + cosAlph*(y-(double)centreY) + centreY;

	IdCntSetX(contour,i,nouvX);
	IdCntSetY(contour,i,nouvY);

 	x= IdCntGetX(contour,i);
 	y= IdCntGetY(contour,i);
	}
return 1;
}




/* FUNCTION DESCRIPTION **************************************************

IdCntZoomContourDouble (fonction)	
  
RESUME: Zoome d'un %ge donne un CNT DOUBLE autour du centre du Rect Englob

DESCRIPTION: Zoome d'un %ge donne un CNT DOUBLE autour du centre du Rect Englob
		Si on connait les coord de centre du Rect Englob,
		on les passe en param ,
		sinon on passe -1., -1. et la fct les calcule

SYNTAXE:     int retCode = IdCntZoomContourDouble(PCONTOUR_DOUBLE cnt, int centreX, int centreY, double percent);

RETOUR:      type  : int
	     role  : code de retour booleen.

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR_DOUBLE
	     role  : contour d'origine.

	     nom   : centreX,centreY
	     type  : int
	     role  : coordonnees du centre du Rect Englob
			-1.,-1. si on ne les connait pas

	     nom   : percent
	     type  : double
	     role  : pourcentage de zoom

FICHIER:   cntutil.c

EXEMPLE:  
   
******************************************************** END DESCRIPTION */
int IdCntZoomContourDouble(contour, centreX, centreY, percent) 
PCONTOUR_DOUBLE contour;
int centreX, centreY;
double percent;
{
double x,y;
double angle, oldDist, newDist;
double nouvX,nouvY;

 int i;
  if ( !contour ) {                                  
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
    }

percent= 1.+ (percent/100.);

if ( (centreX == -1.) && (centreX == -1.) ) /* a tester ... JPR */
	{
	int x0,y0, x1,y1;
	IdCntRectEnglob((PCONTOUR_USHORT)contour, &x0, &y0, &x1, &y1 );
	centreX = abs( x1-x0) /2;
	centreY = abs( y1-y0) /2;
	}

  for (i=0;i<IdCntUsedNbX(contour);i++)
	{

 	x= IdCntGetX(contour,i);
 	y= IdCntGetY(contour,i);

	angle= atan2 ((double)(y - centreY), (double)(x - centreX)); 

	oldDist=IdCalcDistance2Points((double)centreX,	(double)centreY,
				      (double)x, 	(double)y	);
	newDist=oldDist*percent;

	nouvX= centreX + newDist*cos(angle);
	nouvY= centreY + newDist*sin(angle);

	IdCntSetX(contour,i,nouvX);
	IdCntSetY(contour,i,nouvY);

	}
return 1;
}


/* FUNCTION DESCRIPTION **************************************************

IdCntDelPoint (fonction)
  
RESUME: Supprime un point a un contour 

DESCRIPTION:	Supprime un point a un contour. UsedNbX est mis a jour. 

SYNTAXE:     int retCode = IdCntDelPoint(PCONTOUR cnt, int numPoint);

RETOUR:      type  : int
	     role  : code de retour booleen.

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR
	     role  : contour d'origine.

	     nom   : numPoint
	     type  : int
	     role  : numero du point a supprimer

FICHIER:   cntutil.c 

EXEMPLE:  
   
******************************************************** END DESCRIPTION */

int IdCntDelPoint(contour,numPoint) 
PCONTOUR contour;
int numPoint;
{
char * debut;
 
  if ( !contour ) {                                  
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }else if ( IdLibidoType(contour) != CNT) {
    IdErrno = IDERR_WRONG_TYPE;
    return 0;
	}

if( (numPoint > IdCntUsedNbX(contour) ) || ( numPoint <0) ) { return 0; }
 
debut = (char*) contour + numPoint * IdSizeOfType(contour);

memcpy 	(	debut,
		debut+IdSizeOfType(contour),
		(IdCntGetUsedNbX(contour)-numPoint-1)*IdSizeOfType(contour)
	);

IdCntSetUsedNbX(contour,IdCntGetUsedNbX(contour)-1);

return(1);
}

/* FUNCTION DESCRIPTION **************************************************

IdCntDelPoints (fonction)
  
RESUME: Supprime un ensemble de points a un contour 

DESCRIPTION:	Supprime un ensemble de points a un contour. UsedNbX est mis a jour. 

SYNTAXE:     int retCode = IdCntDelPoints(PCONTOUR cnt, int numPoint1, int numPoint2);

RETOUR:      type  : int
	     role  : code de retour booleen.

PARAMETRES:
	     nom   : cnt
	     type  : PCONTOUR
	     role  : contour d'origine.

	     nom   : numPoint1
	     type  : int
	     role  : numero du premier point a supprimer

	     nom   : numPoint2
	     type  : int
	     role  : numero du dernier point a supprimer

FICHIER:   cntutil.c 

EXEMPLE:  
   
******************************************************** END DESCRIPTION */

int IdCntDelPoints(contour,numPoint1,numPoint2) 
PCONTOUR contour;
int numPoint1,numPoint2;
{
char * debut;
int nbpSupr;
 
  if ( !contour ) {                                  
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }else if ( IdLibidoType(contour) != CNT) {
    IdErrno = IDERR_WRONG_TYPE;
    return 0;
	}
if(numPoint1>numPoint2) { return 0; }

if( (numPoint1 > IdCntUsedNbX(contour)-1 ) || ( numPoint1 <0) ) { return 0; }
if( (numPoint2 > IdCntUsedNbX(contour)-1 ) || ( numPoint2 <0) ) { return 0; }

nbpSupr= numPoint2-numPoint1+1;

debut = (char*) contour + numPoint1 * IdSizeOfType(contour);

memcpy	(	debut,
		debut+IdSizeOfType(contour)*nbpSupr, 
		(IdCntGetUsedNbX(contour)-numPoint2-1)*IdSizeOfType(contour)
	);

IdCntSetUsedNbX(contour,IdCntGetUsedNbX(contour)-nbpSupr);

return(1);
}

/* FUNCTION DESCRIPTION **************************************************

IdCntConcat (fonction)		
  
RESUME: Concatene 2 objets CNT. UsedNbX est mis a jour 

DESCRIPTION:	Concatene 2 objets CNT. UsedNbX est mis a jour. 

SYNTAXE:     PCONTOUR CntResult = IdCntConcat(PCONTOUR cnt1, PCONTOUR cnt2);

RETOUR:      type  : PCONTOUR
	     role  : contour concatene.

PARAMETRES:
	     nom   : cnt1
	     type  : PCONTOUR
	     role  : contour d'origine.

	     nom   : cnt2
	     type  : PCONTOUR
	     role  : contour a rajouter

FICHIER:   idcnt.h 

EXEMPLE:  
   
******************************************************** END DESCRIPTION */

PCONTOUR IdCntConcat(contour1,contour2) 
PCONTOUR contour1,contour2;
{
char * fin;
int nouvlong; 
  if ( !contour1 ) {                                  
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }else if ( IdLibidoType(contour1) != CNT) {
    IdErrno = IDERR_WRONG_TYPE;
    return 0;
	}
 
  if ( !contour2 ) {                                  
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }else if ( IdLibidoType(contour2) != CNT) {
    IdErrno = IDERR_WRONG_TYPE;
    return 0;
	}
if (!IdCntSameType(contour1,contour2)){
    IdErrno = IDERR_WRONG_TYPE;
    return 0;
	}

nouvlong= IdCntUsedNbX(contour1)+IdCntUsedNbX(contour2);
if (IdCntDimX(contour1) < nouvlong)
		contour1 = IdCntModifLongueur (contour1,(nouvlong*3)/2);
 
fin = (char*) contour1 + (IdCntUsedNbX(contour1))*IdSizeOfType(contour1);

memcpy (fin,contour2,IdCntUsedNbX(contour2)*IdSizeOfType(contour2));
 
IdCntSetUsedNbX(contour1,nouvlong);

return(contour1);
}

/* FUNCTION DESCRIPTION **************************************************

IdCntClear (fonction)

RESUME: initialisation d'un Contour a 0

DESCRIPTION: Initialisation d'un Contour a 0

SYNTAXE:     PCONTOUR CntResult = IdCntClear (PCONTOUR contResult);

RETOUR:      type  : PCONTOUR
	     role  : Pointeur vers le contour initialise. Zero si echec.

PARAMETRES:  nom   : contResult
	     type  : PCONTOUR
	     role  : Pointeur vers le contour a initialiser.

FICHIER:     cntutil.c

EXEMPLE:     if ( IdCntClear(contour)==0 )
	       IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));

******************************************************** END DESCRIPTION */

PCONTOUR IdCntClear ( cont )
PCONTOUR cont;
{
/*  int y;*/
  int taille_cont;
						 /* Controle du contour **/
  if ( !cont ) {                                  
    IdErrno = IDERR_POINTER_IS_NULL;
    return 0;
  }else if ( IdLibidoType(cont) != CNT) {
    IdErrno = IDERR_WRONG_TYPE;
    return 0;
  }
  taille_cont = (IdCntDimX(cont)*IdSizeOfType(cont)) ;
						 /* Calcul du resultat *****/
      memset( cont, 0, taille_cont );

      IdCntSetUsedNbX(cont,0);

  return cont;
}
/* FUNCTION DESCRIPTION **************************************************

IdCntCopy  (fonction)

RESUME: copie un Contour ds un autre, meme type/longueur

DESCRIPTION: Copie les points d'un Contour dans un autre de meme type et longueur.

SYNTAXE:     PCONTOUR cntDest = IdCntCopy(PCONTOUR cntSource, PCONTOUR cntDest);

RETOUR:      type  : PCONTOUR
	     role  : Pointeur vers le Contour resultat (cntDest).

PARAMETRES:
	     nom   : cntSource
	     type  : PCONTOUR
	     role  : Contour d'origine.

	     nom   : cntDest
	     type  : PCONTOUR
	     role  : Contour destination de meme type et dim. que cntSource.

FICHIER:     cntutil.c

EXEMPLE:     if( !IdCntCopy (ims,imd) ) IdErrPrintf(IdErrMsg(IdErrno));

******************************************************** END DESCRIPTION */
PCONTOUR IdCntCopy (ims,imd)
PCONTOUR ims,imd;
{
/*  int i;*/
  if ( !IdCntSameSizeAndType(ims,imd) )  {
    IdErrno = IDERR_WRONG_DIM;
    return 0;
  }
     memcpy (imd,ims,IdCntDimX(imd)*IdSizeOfType(imd));
     IdCntSetUsedNbX(imd,IdCntGetUsedNbX(ims));
  return imd;
}

/* FUNCTION DESCRIPTION **************************************************

IdCntCast  (fonction)

RESUME: converti un Contour d'un type reel ds un autre

DESCRIPTION: Convertit un Contour d'un type reel dans un autre. Les
             types sources complexes ne sont pas pris en charge.
             Dans ce cas, la variable IdErrno prend une valeur non-nulle.

SYNTAXE:     PCONTOUR imDest = IdCntCast(PCONTOUR imSource, PCONTOUR imDest);

RETOUR:      type  : PCONTOUR
	     role  : Pointeur vers le Contour resultat (imd) ou zero si echec.

PARAMETRES:
	     nom   : imSource
	     type  : PCONTOUR
	     role  : Contour d'origine.

	     nom   : imDest
	     type  : PCONTOUR
	     role  : Contour destination. Doit etre alloue en utilisant la
                     fonction IdCntAlloc() avec le bon type CNT_???.

FICHIER:     cntutil.c

EXEMPLE:     if( !IdCntCast (ims,imd) )
               IdErrPrintf(IdErrMsg(IdErrno));

******************************************************** END DESCRIPTION */
PCONTOUR IdCntCast (ims,imd)
PCONTOUR ims,imd;
{
  int i;
  if ( !IdCntSameSize (ims,imd)) 
  {
    IdErrno = IDERR_WRONG_DIM;
    return 0;
  }
  if (IdCntType(imd)==IdCntType(ims)){
    IdCntCopy(ims,imd);
    return imd;
  }

#define CRR(t1,t2)      for(i=0;i<IdCntDimX(ims);i++) 			\
                          { ((t2)imd)[i].x = ((t1)ims)[i].x;		\
                            ((t2)imd)[i].y = ((t1)ims)[i].y;		\
                          }

#define CFR(PPt)  switch ( IdCntType(imd) ) { 				\
	case CNT_CHAR		: CRR(PPt,PCONTOUR_CHAR); 	break; \
	case CNT_UCHAR		: CRR(PPt,PCONTOUR_UCHAR); 	break; \
        case CNT_SHORT		: CRR(PPt,PCONTOUR_SHORT); 	break; \
	case CNT_USHORT		: CRR(PPt,PCONTOUR_USHORT); 	break; \
	case CNT_LONG		: CRR(PPt,PCONTOUR_LONG); 	break; \
	case CNT_ULONG		: CRR(PPt,PCONTOUR_ULONG); 	break; \
	case CNT_FLOAT  	: CRR(PPt,PCONTOUR_FLOAT); 	break; \
	case CNT_DOUBLE 	: CRR(PPt,PCONTOUR_DOUBLE); 	break; \
	default			: IdErrno=IDERR_WRONG_TYPE; 		\
				 IdPrintf("Type non Traite1 %x\n",IdCntType(imd) );		\
				return 0;  }

  switch ( IdCntType(ims) ) {
    case CNT_UCHAR  : CFR(PCONTOUR_UCHAR); break;
    case CNT_CHAR   : CFR(PCONTOUR_CHAR); break;
    case CNT_SHORT  : CFR(PCONTOUR_SHORT); break;
    case CNT_USHORT : CFR(PCONTOUR_USHORT); break;
    case CNT_LONG   : CFR(PCONTOUR_LONG); break;
    case CNT_ULONG  : CFR(PCONTOUR_ULONG); break;
    case CNT_FLOAT  : CFR(PCONTOUR_FLOAT); break;
    case CNT_DOUBLE : CFR(PCONTOUR_DOUBLE); break;
    default	    : IdErrno=IDERR_WRONG_TYPE;
				 IdPrintf("Type non Traite2 %x\n",IdCntType(ims));
				return 0;  }    
  IdCntSetUsedNbX(imd,IdCntGetUsedNbX(ims));
  return imd;
}

