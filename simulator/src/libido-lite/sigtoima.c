/*************************************************************************
* $Id: sigtoima.c,v 1.1 2005/09/09 08:22:55 bellet Exp $
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

#include "idima.h"
#include "idsig.h"
#include "idprint.h"

/*
 * Undocumented parameters 
 */
#define PCY 0.5
#define MRG0 2

/**
 * IdImaSignalToImage:
 * @sig: pointeur vers le signal source
 * @off: offset de depart
 * @lh: longueur a afficher
 * @org: origine			---> N'EST PLUS UTILISE
 * @maxi: valeur du Max que l'on veut imposer (-1. :on n'impose rien)
 * @mini: valeur du Min que l'on veut imposer (-1. :on n'impose rien)
 * @dimx: taille en X de l'image produite
 * @dimy: taille en Y de l'image produite
 * @bgnd: valeur du niveau de gris du fond
 * @vpix: valeur du niveau de gris du Pixel
 *
 * transforme un signal non COMPLEX en une image UCHAR affichable.
 * On peut ignorer le debut du signal (offset),
 * On peut ne retenir qu'une partie du signal (lgrSig) 
 * On peut imposer le recadrage entre deux valeurs 
 * (maxImpose, minImpose)
 *
 * Return value: 0 si echec
 */
PPIMAGE_UCHAR 
IdImaSignalToImage(PSIGNAL sig, int off, int lh, int Urg, double maxi,
		   double mini, int dimx, int dimy, int bgnd, int vpix)
{
	int i, j, dx1, dy1, coo, end, ty, previousend=0, xfin;
	double kx, val=0., step;
	PPIMAGE_UCHAR Im;
	double tmp;

	Im = (PPIMAGE_UCHAR) IdImaAlloc(dimx, dimy, IMA_UCHAR);
	if (!Im) {
		IdErrPrintf
		    ("erreur allocation image pour IdImaSignalToIma\n");
		return (0);
	}
	IdImaSetValue(Im, bgnd);

	if (mini == -1.)
		if (maxi == -1.)
			IdSigMinMaxInterv(sig, off, off + lh, &mini,
					  &maxi);
		else
			IdSigMinMaxInterv(sig, off, off + lh, &mini, &tmp);
	else if (maxi == -1)
		IdSigMinMaxInterv(sig, off, off + lh, &tmp, &maxi);

	dx1 = IdImaDimX(Im) - 1;
	dy1 = IdImaDimY(Im) - 1;
	ty = IdSigType(sig);

	/*
	 * mettre l'echelle 'proprement' 
	 */

	for (j = 0; j < (dx1 + 1); j++) {
		Im[0][j] = vpix;
		Im[dy1][j] = vpix;
	}
	for (i = 0; i < (dy1 + 1); i++) {
		Im[i][0] = vpix;
		Im[i][dx1] = vpix;
	}

	if (lh >= dimx)
	 {			/*
				 * on sous-echantillonne le signal 
				 */

		kx = (double) (lh) / dx1;
		for (j = 1; j < dx1; j++) {
			coo = (int) (j * kx + off - 1 + .5);
			switch (ty) {
			case SIG_UCHAR:
				val = ((PSIGNAL_UCHAR) sig)[coo];
				break;
			case SIG_CHAR:
				val = ((PSIGNAL_CHAR) sig)[coo];
				break;
			case SIG_USHORT:
				val = ((PSIGNAL_USHORT) sig)[coo];
				break;
			case SIG_SHORT:
				val = ((PSIGNAL_SHORT) sig)[coo];
				break;
			case SIG_LONG:
				val = ((PSIGNAL_LONG) sig)[coo];
				break;
			case SIG_ULONG:
				val = ((PSIGNAL_ULONG) sig)[coo];
				break;
			case SIG_FLOAT:
				val = ((PSIGNAL_FLOAT) sig)[coo];
				break;
			case SIG_DOUBLE:
				val = ((PSIGNAL_DOUBLE) sig)[coo];
				break;
			}
			end = (int) (((maxi - val) / (maxi - mini)) * dy1);

			if (end < 0)
				end = 0;
			if (j == 1) {
				previousend = end;
			}
			else {
				Im =
				    IdImaDrawLine(Im, vpix, 0, j - 1,
						  previousend, j, end);
				previousend = end;
			}

		}
	}

	/*
	 * fin sous-ech signal 
	 */
	else {			/*
				 * interpolation         
				 */

		step = (double) dimx / (double) (lh - 1);

		for (j = 0; j < lh; j++) {	/*
						 * pour tous les pts du signal 
						 */
			coo = j + off;
			switch (ty) {
			case SIG_UCHAR:
				val = ((PSIGNAL_UCHAR) sig)[coo];
				break;
			case SIG_CHAR:
				val = ((PSIGNAL_CHAR) sig)[coo];
				break;
			case SIG_USHORT:
				val = ((PSIGNAL_USHORT) sig)[coo];
				break;
			case SIG_SHORT:
				val = ((PSIGNAL_SHORT) sig)[coo];
				break;
			case SIG_LONG:
				val = ((PSIGNAL_LONG) sig)[coo];
				break;
			case SIG_ULONG:
				val = ((PSIGNAL_ULONG) sig)[coo];
				break;
			case SIG_FLOAT:
				val = ((PSIGNAL_FLOAT) sig)[coo];
				break;
			case SIG_DOUBLE:
				val = ((PSIGNAL_DOUBLE) sig)[coo];
				break;
			}

			end = (int) (((maxi - val) / (maxi - mini)) * dy1);
			if (end < 0)
				end = 0;

			if (j == 0) {
				previousend = end;
			}

			else {
				if ((xfin = (int) (j * step)) == dimx)
					xfin--;
				Im =
				    IdImaDrawLine(Im, vpix, 0,
						  (int) ((j - 1) * step),
						  previousend, xfin, end);
				previousend = end;
			}
		}

	}
	return (Im);
}
