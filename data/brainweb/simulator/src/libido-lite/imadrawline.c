/*************************************************************************
* $Id: imadrawline.c,v 1.1 2005/09/09 08:22:47 bellet Exp $
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
*  Description  : Trace de lignes dans une image.
*
**************************************************************************/
#include <string.h>		// For memcpy
#include <stdlib.h>
#include "idcommon.h"
#include "idima.h"
#include "iderr.h"
#include "idprint.h"
#include "idcnt.h"

static int CalculVoxelDroite(int, int, int, int, int, int *, int *);

/*
 * -----------------------------------------------------
 * 
 * Cette fonction, statique, est prevue pour LE RESTER
 * Ne la recopiez pas chez vous ...
 * Il y a ds les fonctions sur les CNT tout ce qu'il faut pour travailler
 * Si vous trouvez qu'il en manque une, dites-le nous, nous la rajouterons...
 * Toutes les recopies 'non controlees' de cette fonction dans des directories 
 * utilisateur ont ete jusqu'a present des erreures d'analyse ...
 * 
 * ----------------------------------------------------- 
 */

static int
CalculVoxelDroite(int premier, int ox, int oy, int fx, int fy, int *x, int *y)
{

	static int Dx;		// Ecart en x entre debut et fin 
	static int Dy;		//   "      y   "     "       "  
	static short Incx;	// Increment en x                
	static short Incy;	// Increment en y                
	static int s;
	static int Dx2;		// Double de Dx                  
	static int Dy2;		// Double de Dy                  
	static int Dxy;		// Double de |Dx-Dy|             
	static int X;		// coordonnee X courante         
	static int Y;		// coordonnee Y courante         
	static int Voxel;	// Compteur de voxel retourne    

#define SIGN(x)  ((x)<0?-1:((x)>0?1:0))

	Voxel++;
	/*
	 * Initialisation du trace de droite 
	 */
	if (premier) {
		X = ox;
		/*
		 * Sens de variation de la droite suivant X
		 */
		Incx = SIGN(fx - X);
		/*
		 * Variation totale suivant X 
		 */
		Dx = ABS(fx - X);
		Dx2 = 2 * Dx;

		Y = oy;
		/*
		 * Sens de variation de la droite 
		 */
		Incy = SIGN(fy - Y);
		/*
		 * Variation totale suivant Y 
		 */
		Dy = ABS(fy - Y);
		Dy2 = 2 * Dy;

		/*
		 * Servent a determiner si on va en diagonnale ou pas 
		 */
		Dxy = SIGN(Dx - Dy) * (Dy2 - Dx2);
		if (Dx > Dy)
			s = Dy2 - Dx;
		else
			s = Dx2 - Dy;

		Voxel = 0;
		/*
		 * variation plus rapide suivant X 
		 */
	} else if (Dx > Dy) {
		if (Voxel > Dx)
			/*
			 * fin du trace 
			 */
			return 0;
		if (s >= 0) {
			Y += Incy;
			s += Dxy;
		} else {
			s += Dy2;
		}
		X += Incx;
		/*
		 * variation plus rapide suivant Y 
		 */
	} else if (Dx < Dy) {
		if (Voxel > Dy)
			/*
			 * fin du trace 
			 */
			return 0;
		if (s >= 0) {
			X += Incx;
			s += Dxy;
		} else {
			s += Dx2;
		}
		Y += Incy;
		/*
		 * variation égale suivant X et Y 
		 */
	} else {
		if (Voxel > Dy)
			/*
			 * fin du trace 
			 */
			return 0;
		X += Incx;
		Y += Incy;
	}

	*x = X;
	*y = Y;
	return 1;
}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdImaDrawLine (fonction)
 * 
 * RESUME: Trace une ligne dans une image UCHAR.
 * 
 * DESCRIPTION: Trace une ligne dans une image UCHAR.
 * 
 * SYNTAXE:     PPIMAGE_UCHAR imSource = IdImaDrawLine ( PPIMAGE_UCHAR imSource, int value, int modeTrace, int ox, int oy, int fx, int fy);
 * 
 * RETOUR:      type  : PPIMAGE_UCHAR
 * role  : Pointeur vers l'image resultat. Zero si echec.
 * 
 * PARAMETRES:  nom   : imSource
 * type  : PPIMAGE_UCHAR
 * role  : Pointeur vers l'image source
 * 
 * nom   : value
 * type  : int
 * role  : Valeur de trace.
 * 
 * nom   : modeTrace
 * type  : int
 * role  : Type de trace : 0 = trace direct
 * 1 = AND
 * 2 = OR
 * 3 = XOR
 * 
 * nom   : ox, oy, fx, fy
 * type  : int
 * role  : (ox, oy) et (fx,fy) correspondent aux points de debut
 * et de fin du segment a tracer.
 * 
 * FICHIER:     flignes.c
 * 
 * EXEMPLE:     if ( (imd=IdImaDrawLine(ims,255,0,0,3,0,5,4))==0 )
 * IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
 * Trace une ligne avec une valeur de 255, entre les points
 * (3,0) et (5,4).
 * 
 * ******************************************************** END DESCRIPTION 
 */

PPIMAGE_UCHAR
IdImaDrawLine(PPIMAGE_UCHAR ims, int value, int code,
		int ox, int oy, int fx, int fy)
{
	int x, y;
	int init;
	/*
	 * Controle de l'image *
	 */
	if (!ims || IdImaType(ims) != IMA_UCHAR) {
		IdErrno = IDERR_WRONG_TYPE;
		return 0;
	}

	if ((ox < 0) || (ox >= IdImaDimX(ims))
	    || (fx < 0) || (fx >= IdImaDimX(ims))
	    || (oy < 0) || (oy >= IdImaDimY(ims))
	    || (fy < 0) || (fy >= IdImaDimY(ims))
	    || (code > 3) || (code < 0)) {
		IdErrno = IDERR_INVALID_NUMBER;
		return 0;
	}
	/*
	 * Calcul du resultat ****
	 */
	switch (code) {

	case 1:{
			for (init =
			     CalculVoxelDroite(1, ox, oy, fx, fy, &x, &y);
			     init;
			     init =
			     CalculVoxelDroite(0, ox, oy, fx, fy, &x, &y)) {
				ims[y][x] &= value;
			}
		}
		break;

	case 2:{
			for (init =
			     CalculVoxelDroite(1, ox, oy, fx, fy, &x, &y);
			     init;
			     init =
			     CalculVoxelDroite(0, ox, oy, fx, fy, &x, &y)) {
				ims[y][x] |= value;
			}
		}
		break;

	case 3:{
			for (init =
			     CalculVoxelDroite(1, ox, oy, fx, fy, &x, &y);
			     init;
			     init =
			     CalculVoxelDroite(0, ox, oy, fx, fy, &x, &y)) {
				ims[y][x] ^= value;
			}
		}
		break;

	case 0:
	default:{
			for (init =
			     CalculVoxelDroite(1, ox, oy, fx, fy, &x, &y);
			     init;
			     init =
			     CalculVoxelDroite(0, ox, oy, fx, fy, &x, &y)) {
				ims[y][x] = value;
			}
		}
	}

	return ims;

}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdImaDrawPolyLines (fonction)
 * 
 * RESUME: Trace d'une suite de lignes dans une image UCHAR.
 * 
 * DESCRIPTION: Trace d'une suite de lignes dans une image UCHAR.
 * 
 * SYNTAXE:     PPIMAGE_UCHAR imSource = IdImaDrawPolyLines ( PPIMAGE_UCHAR imSource, int value, int modeTrace, int ptx[],int pty[],int nb,int closepoly );
 * 
 * RETOUR:      type  : PPIMAGE_UCHAR
 * role  : Pointeur vers l'image resultat. Zero si echec.
 * 
 * PARAMETRES:  nom   : image
 * type  : PPIMAGE_UCHAR
 * role  : Pointeur vers l'image source
 * 
 * nom   : value
 * type  : int
 * role  : Valeur de trace.
 * 
 * nom   : modeTrace
 * type  : int
 * role  : Type de trace :
 * 0 = trace direct
 * 1 = AND
 * 2 = OR
 * 3 = XOR
 * 
 * nom   : ptx, pty
 * type  : int *
 * role  : Coordonnees des points decrivant les lignes
 * 
 * nom   : nb
 * type  : int
 * role  : nombre de pointsde la ligne
 * 
 * nom   : closepoly
 * type  : int
 * role  : Faut-il fermer le polygone ?
 * 0 = Non
 * 1 = Oui
 * FICHIER:     flignes.c
 * 
 * EXEMPLE:     Trace d'un carre ferme entre les pts (50,50) et (100,100) :
 * static int ptx[]= {50,100,100,50};
 * static int pty[]= {50,50,100,100};
 * if ( IdImaDrawPolyLines(ims,255,0,0,ptx,pty,1) ==0 )
 * IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
 * 
 * ******************************************************** END DESCRIPTION 
 */
PPIMAGE_UCHAR
IdImaDrawPolyLines(PPIMAGE_UCHAR imSource, int value, int modeTrace,
		int *ptx, int *pty, int nb, int closepoly)
{
	int n;
	for (n = nb - 1; n > 0; n--) {
		imSource = IdImaDrawLine(imSource, value, modeTrace,
					 ptx[n - 1], pty[n - 1], ptx[n],
					 pty[n]);
	}
	if (closepoly)
		imSource = IdImaDrawLine(imSource, value, modeTrace,
					 ptx[nb - 1], pty[nb - 1], ptx[0],
					 pty[0]);

	return imSource;
}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdCntPointsOfSegment (fonction)
 * 
 * RESUME: Renvoie les coordonnees des pixels sur un segment entre 2 points.
 * 
 * DESCRIPTION: Renvoie les coordonnees des pixels sur un segment entre 2 points.
 * 
 * SYNTAXE:     PCONTOUR_USHORT cnt = IdCntPointsOfSegment ( int ox, int oy, int fx, int fy);
 * 
 * RETOUR:      type  : PCONTOUR_USHORT
 * role  : Pointeur vers le CONTOUR resultat. Zero si echec.
 * 
 * PARAMETRES:  
 * 
 * nom   : ox, oy, fx, fy
 * type  : int
 * role  : (ox, oy) et (fx,fy) correspondent aux points de debut
 * et de fin du segment.
 * 
 * FICHIER:     flignes.c
 * 
 * EXEMPLE:     if ( (cnt1=IdCntPointsOfSegment(3,0,5,4))==0 )
 * IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
 * Renvoie les coordonnees des pixels sur un segment entre les points
 * (3,0) et (5,4).
 * 
 * ******************************************************** END DESCRIPTION 
 */

PCONTOUR_USHORT
IdCntPointsOfSegment(int ox, int oy, int fx, int fy)
{
	int x, y;
	int init;
	PCONTOUR_USHORT cnt;

	if ((ox < 0) || (fx < 0) || (oy < 0) || (fy < 0)) {
		IdErrno = IDERR_INVALID_NUMBER;
		return 0;
	}

	/*
	 * Si pas suffisant, IdCntAddPoint rallonge 
	 */
	cnt = (PCONTOUR_USHORT) IdCntAlloc(512, CNT_USHORT);

	if (!cnt) {
		IdErrno = IDERR_ALLOC_CNT;
		IdErrPrintf("Echec Alloc Contour \n");
		return 0;
	}
	/*
	 * Calcul du resultat ****
	 */

	for (init = CalculVoxelDroite(1, ox, oy, fx, fy, &x, &y); init;
	     init = CalculVoxelDroite(0, ox, oy, fx, fy, &x, &y)) {
		IdCntAddPoint(cnt, x, y);
	}

	return cnt;

}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdCntAddPointsOfSegment (Macro)
 * 
 * RESUME: Rajoute a la fin d'un CNT les coordonnees des pixels sur un segment entre 2 points.
 * 
 * DESCRIPTION: Rajoute a la fin d'un CNT les coordonnees des pixels sur un segment entre 2 points.
 * 
 * SYNTAXE:     int retCode  = IdCntAddPointsOfSegment (PCONTOUR_USHORT cntSource int ox, int oy, int fx, int fy);
 * 
 * RETOUR:      type  : int 
 * role  : code retour. Zero si echec, 1 si OK.
 * 
 * PARAMETRES:  
 * 
 * nom   : ox, oy, fx, fy
 * type  : int
 * role  : (ox, oy) et (fx,fy) correspondent aux points de debut
 * et de fin du segment.
 * 
 * nom   : cnt
 * type  : PCONTOUR_USHORT
 * role  : 
 * 
 * FICHIER:     flignes.c
 * 
 * EXEMPLE:     if ( (cnt1=IdCntAddPointsOfSegment(cnt1,3,0,5,4))==0 )
 * IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
 * Renvoie les coordonnees des pixels sur un segment entre les points
 * (3,0) et (5,4).
 * 
 * ******************************************************** END DESCRIPTION 
 */
int
_IdCntAddPointsOfSegment(PCONTOUR_USHORT *cnt, int ox, int oy,
		int fx, int fy)
{
	int x, y;
	int init;

	if ((ox < 0) || (fx < 0) || (oy < 0) || (fy < 0)) {
		IdErrno = IDERR_INVALID_NUMBER;
		return 0;
	}

	if (!(*cnt)) {
		IdErrno = IDERR_POINTER_IS_NULL;
		IdErrPrintf("Contour non Alloue\n");
		return 0;
	}
	/*
	 * Calcul du resultat ****
	 */

	for (init = CalculVoxelDroite(1, ox, oy, fx, fy, &x, &y); init;
	     init = CalculVoxelDroite(0, ox, oy, fx, fy, &x, &y)) {
		IdCntAddPoint(*cnt, x, y);
	}

	return 1;

}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdCntAppendPointsOfSegment (macro)
 * 
 * RESUME: Rajoute a la fin d'un CNT les coordonnees des pixels sur un segment jusqu'a 1 point.
 * 
 * DESCRIPTION: Rajoute a la fin d'un CNT les coordonnees des pixels sur un segment jusqu'a 1 point.
 * 
 * SYNTAXE:     int retCode  = IdCntAppendPointsOfSegment (PCONTOUR_USHORT cntSource int x, int y);
 * 
 * RETOUR:      type  : int 
 * role  : code retour. Zero si echec, 1 si OK.
 * 
 * PARAMETRES:  
 * nom   : x, y
 * type  : int
 * role  : (x, y) correspondent a l'extremite du segment.
 * 
 * nom   : cnt
 * type  : PCONTOUR_USHORT
 * role  : 
 * 
 * FICHIER:     flignes.c
 * 
 * EXEMPLE:     if ( (cnt1=IdCntAppendPointsOfSegment(cnt1,3,8))==0 )
 * IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
 * 
 * ******************************************************** END DESCRIPTION 
 */



/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdCntInsertPointsOfSegment (macro)
 * 
 * RESUME: Insere ds 1 CNT les coordonnees des pixels sur 1 segmt a partir d'1 point.
 * 
 * DESCRIPTION: Insere ds 1 CNT les coordonnees des pixels sur 1 segmt a partir d'1 point.
 * 
 * SYNTAXE:     int retCode  = IdCntInsertPointsOfSegment (PCONTOUR_USHORT cntSource ,int numPoint, int fx, int fy);
 * 
 * RETOUR:      type  : int 
 * role  : code retour. Zero si echec, 1 si OK.
 * 
 * PARAMETRES:  
 * 
 * nom   : cnt
 * type  : PCONTOUR_USHORT
 * role  : 
 * 
 * nom   : numPoint
 * type  : int
 * role  : indice du Point a partir duquel on insere 
 * 
 * nom   : fx, fy
 * type  : int
 * role  :  (fx,fy) correspondent aux points de fin du segment.
 * 
 * FICHIER:     cntutil.c
 * 
 * EXEMPLE:    
 * 
 * ******************************************************** END DESCRIPTION 
 */

int
_IdCntInsertPointsOfSegment(PCONTOUR_USHORT *cnt,
		int numPoint, int fx, int fy)
{
	int x, y;
	int init, nbPtsAvLaFin, lgr, lgr2;
	char *zoneSecu = NULL;
	char *debut;

	if ((fy < 0) || (fx < 0)) {
		IdErrno = IDERR_INVALID_NUMBER;
		return 0;
	}

	if (!(*cnt)) {
		IdErrno = IDERR_POINTER_IS_NULL;
		IdErrPrintf("Contour non Alloue\n");
		return 0;
	}

	if (numPoint > IdCntUsedNbX(*cnt)) {
		IdErrno = IDERR_POINTER_IS_NULL;
		IdErrPrintf("numPoint > taille Cnt\n");
		return 0;
	}

	debut = (char *) *cnt + (numPoint + 1) * IdSizeOfType(*cnt);
	nbPtsAvLaFin = IdCntUsedNbX(*cnt) - numPoint - 1;
	lgr = IdSizeOfType(*cnt) * nbPtsAvLaFin;

	zoneSecu = (char *) malloc(lgr);

	memcpy(zoneSecu, debut, lgr);
	IdCntSetUsedNbX(*cnt, numPoint + 1);

	/*
	 * Calcul du resultat ****
	 */
	for (init = CalculVoxelDroite(1, IdCntGetX(*cnt, numPoint),
				      IdCntGetY(*cnt, numPoint),
				      fx, fy, &x, &y);
	     init;
	     init = CalculVoxelDroite(0, IdCntGetX(*cnt, numPoint),
				      IdCntGetY(*cnt, numPoint),
				      fx, fy, &x, &y)) {
		IdCntAddPoint(*cnt, x, y);
	}

	lgr2 = (IdCntUsedNbX(*cnt) + nbPtsAvLaFin);

	if (IdCntDimX(*cnt) < (IdCntUsedNbX(*cnt) + nbPtsAvLaFin)) {

		/*
		 * 20 % de + que necessaire pr eviter trop de realloc 
		 */
		*cnt = (PCONTOUR_USHORT)IdCntModifLongueur(
				(PCONTOUR)*cnt, lgr2 * 1.2);
	}

	memcpy((char *) *cnt + IdCntUsedNbX(*cnt) * IdSizeOfType(*cnt),
	       zoneSecu, lgr);

	IdCntSetUsedNbX(*cnt, IdCntUsedNbX(*cnt) + nbPtsAvLaFin);

	return 1;

}
