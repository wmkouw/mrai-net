/*************************************************************************
* $Id: volfft.c,v 1.1 2005/09/09 08:22:56 bellet Exp $
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

/*
 * FFT Tridimensionnelle d'un volume USHORT -> VolFFT
 * IFFT Tridimensionnelle d'un volume COMPLEX double precision -> VolIFFT
 */

#include <stdio.h>
#include <math.h>
#include "idsig.h"
#include "idima.h"
#include "idvol.h"
#include "iderr.h"
#include "idprint.h"

static PPPVOLUME_COMPLEX 
recon_3d(PPPVOLUME_COMPLEX volr)
{
	int i, j, k, dimx, dimy, dimz;
	PPPVOLUME_COMPLEX volx;

	dimx = IdVolDimX(volr) / 2;
	dimy = IdVolDimY(volr) / 2;
	dimz = IdVolDimZ(volr) / 2;

	volx =
	    (PPPVOLUME_COMPLEX) IdVolAlloc(IdVolDimX(volr),
					   IdVolDimY(volr),
					   IdVolDimZ(volr), VOL_COMPLEX);
	if (volx == NULL)
		IdErrPrintf("\n PROBLEME D'ALLOCATION ");

	for (i = 0; i < dimz; i++)
		for (j = 0; j < dimy; j++)
			for (k = 0; k < dimx; k++)

			 {
				volx[i][j][k] =
				    volr[i + dimz][j + dimy][k + dimx];
				volx[i][j + dimy][k] =
				    volr[i + dimz][j][k + dimx];
				volx[i + dimz][j + dimy][k] =
				    volr[i][j][k + dimx];
				volx[i + dimz][j][k] =
				    volr[i][j + dimy][k + dimx];

				volx[i][j][k + dimx] =
				    volr[i + dimz][j + dimy][k];
				volx[i][j + dimy][k + dimx] =
				    volr[i + dimz][j][k];
				volx[i + dimz][j + dimy][k + dimx] =
				    volr[i][j][k];
				volx[i + dimz][j][k + dimx] =
				    volr[i][j + dimy][k];
			}
	IdVolFree(volr);
	return (volx);
}

static void 
FFT1Dbis(PPPVOLUME_COMPLEX volc)
{
	int i, j, k, Tx;
	PSIGNAL_COMPLEX lign;
	double sqtx;

	Tx = IdVolDimX(volc);
	sqtx = sqrt((double) (Tx));
	lign = (PSIGNAL_COMPLEX) IdSigAlloc(Tx, SIG_COMPLEX);

	for (i = 0; i < IdVolDimZ(volc); i++)
		for (j = 0; j < IdVolDimY(volc); j++) {
			/*
			 * remplissage de la ligne 
			 */
			for (k = 0; k < Tx; k++) {
				lign[k] = volc[i][j][k];
			}
			/*
			 * calcul de la FFT de la ligne 
			 */
			IdSigFFT(lign, 1);
			/*
			 * copie dans la ligne correspondante de volc 
			 */
			for (k = 0; k < Tx; k++) {
				volc[i][j][k].re = lign[k].re / (sqtx);
				volc[i][j][k].im = lign[k].im / (sqtx);
			}
		}
	IdSigFree(lign);
}


static void 
IFFT1Dbis(PPPVOLUME_COMPLEX volc)
{
	int i, j, k, Tx;
	PSIGNAL_COMPLEX lign;
	double sqtx;

	Tx = IdVolDimX(volc);
	sqtx = sqrt((double) (Tx));
	lign = (PSIGNAL_COMPLEX) IdSigAlloc(Tx, SIG_COMPLEX);

	for (i = 0; i < IdVolDimZ(volc); i++)
		for (j = 0; j < IdVolDimY(volc); j++) {
			/*
			 * remplissage de la ligne 
			 */
			for (k = 0; k < Tx; k++) {
				lign[k] = volc[i][j][k];
			}
			/*
			 * calcul de la IFFT de la ligne 
			 */
			IdSigFFT(lign, -1);
			/*
			 * copie dans la ligne correspondante de volc 
			 */
			for (k = 0; k < Tx; k++) {
				volc[i][j][k].re = lign[k].re * (sqtx);
				volc[i][j][k].im = lign[k].im * (sqtx);
			}
		}
	IdSigFree(lign);
}


static void 
transposition_3d(PPPVOLUME_COMPLEX volc, PPPVOLUME_COMPLEX voli)
{
	int i, j, k;
	for (i = 0; i < IdVolDimY(volc); i++)
		for (j = 0; j < IdVolDimX(volc); j++)
			for (k = 0; k < IdVolDimZ(volc); k++)

				voli[i][j][k] = volc[k][i][j];
	return;
}

/**
 * \brief	Module de calcul de la transformee de fourier d'un volume 3D
 *		par appel du module de la transformee 1D
 *		l'algorithme utilise etant la FFT,la taille de l'image
 *		doit etre une puissance de deux
 *		Att:Ce module cree une structure image -> penser aux
 *		liberations memoire a l'appel
 *
 * @param vols	volume source ? (FIXME : undocumented)
 * @return	volume destination ? (FIXME : undocumented)
 */

#define FF3(typevol) \
    	for(k=0;k<Tz;k++) \
    	for(i=0;i<Ty;i++) \
       	for(j=0;j<Tx;j++) \
  	   {	volr[k][i][j].re=((typevol)vols)[k][i][j]; \
   		volr[k][i][j].im=0; \
	   }


PPPVOLUME_COMPLEX 
IdVolFFT(PPPVOLUME vols)
{
	PPPVOLUME_COMPLEX voli;
	PPPVOLUME_COMPLEX volr;
	int i, j, k, Tx, Ty, Tz;

	Tx = IdVolDimX(vols);
	Ty = IdVolDimY(vols);
	Tz = IdVolDimZ(vols);

	volr = (PPPVOLUME_COMPLEX) IdVolAlloc(Tx, Ty, Tz, VOL_COMPLEX);

	switch (IdVolType(vols)) {
	case VOL_CHAR:
		FF3(PPPVOLUME_CHAR);
		break;
	case VOL_UCHAR:
		FF3(PPPVOLUME_UCHAR);
		break;
	case VOL_SHORT:
		FF3(PPPVOLUME_SHORT);
		break;
	case VOL_USHORT:
		FF3(PPPVOLUME_USHORT);
		break;
	case VOL_LONG:
		FF3(PPPVOLUME_LONG);
		break;
	case VOL_ULONG:
		FF3(PPPVOLUME_ULONG);
		break;
	case VOL_FLOAT:
		FF3(PPPVOLUME_FLOAT);
		break;
	case VOL_DOUBLE:
		FF3(PPPVOLUME_DOUBLE);
		break;

	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdVolFFT !\n",
		     IdVolType(vols));
		IdErrno = IDERR_WRONG_LIBTYPE;
		return (0);
	}

	FFT1Dbis(volr);
	voli = (PPPVOLUME_COMPLEX) IdVolAlloc(Tz, Tx, Ty, VOL_COMPLEX);
	transposition_3d(volr, voli);
	IdVolFree(volr);

	FFT1Dbis(voli);
	volr = (PPPVOLUME_COMPLEX) IdVolAlloc(Ty, Tz, Tx, VOL_COMPLEX);
	transposition_3d(voli, volr);
	IdVolFree(voli);

	FFT1Dbis(volr);
	voli = (PPPVOLUME_COMPLEX) IdVolAlloc(Tx, Ty, Tz, VOL_COMPLEX);
	transposition_3d(volr, voli);

	voli = recon_3d(voli);

	IdVolFree(volr);
	return (voli);
}

/**
 * \brief	Module de calcul de la transformee de fourier inverse 
 * 		d'un volume par appel du module de la transformee inverse 
 * 		1D l'algorithme utilise etant la IFFT,la taille du volume
 *		doit etre une puissance de deux
 * \warning	les pointeurs sur volumes source et 
 * 		destination doivent etre identiques.
 *
 * @param volsource	volume source ? (FIXME : undocumented)
 * @return		volume destinated ? (FIXME : undocumented)
 */

PPPVOLUME_COMPLEX 
IdVolIFFT(PPPVOLUME_COMPLEX volsource)
{
	PPPVOLUME_COMPLEX voli;
	int Tx, Ty, Tz;


	Tx = IdVolDimX(volsource);
	Ty = IdVolDimY(volsource);
	Tz = IdVolDimZ(volsource);

	/*
	 * transformee de Fourier premiere passe 
	 */
	IFFT1Dbis(volsource);
	voli = (PPPVOLUME_COMPLEX) IdVolAlloc(Tz, Tx, Ty, VOL_COMPLEX);
	transposition_3d(volsource, voli);
	IdVolFree(volsource);

	/*
	 * transformee de Fourier deuxieme passe 
	 */

	IFFT1Dbis(voli);
	volsource =
	    (PPPVOLUME_COMPLEX) IdVolAlloc(Ty, Tz, Tx, VOL_COMPLEX);
	transposition_3d(voli, volsource);
	IdVolFree(voli);

	/*
	 * transformee de Fourier troisieme passe 
	 */

	IFFT1Dbis(volsource);
	voli = (PPPVOLUME_COMPLEX) IdVolAlloc(Tx, Ty, Tz, VOL_COMPLEX);
	transposition_3d(volsource, voli);
	/*
	 * voli=recon_3d(voli); 
	 */
	IdVolFree(volsource);

	return (voli);
}
