/*************************************************************************
* $Id: sigfft.c,v 1.1 2005/09/09 08:22:53 bellet Exp $
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
 * transfomation de fourier rapide (FFT) des signaux
 * monodimensionnels complexes
 */
#include <math.h>
#include "idsig.h"
#include "idprint.h"

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define REAL double


/**
 * IdSigFFT
 * @sig: signal d'entree et resultat de la FFT en sortie.
 * @isign: 1 pour FFT directe, -1 pour FFT inverse.
 *
 * FFT d'un signal COMPLEX. Le resultat remplace le signal entre.
 */
void 
IdSigFFT(PSIGNAL_COMPLEX sig, int isign)
{
	int n, nn, mmax, m, j, istep, i;
	REAL wtemp, wr, wpr, wpi, wi, theta;
	REAL tempr, tempi;

	if (!sig) {
		IdErrPrintf("IdSigFFT: Parametrer is NULL\n");
		return;
	}

	if (IdLibidoType(sig) != SIG) {
		IdErrPrintf("IdSigFFT: Parametrer is NOT a SIGNAL\n");
		return;
	}
	if ((IdLibidoDataType(sig) != TY_COMPLEX) &&
	    (IdLibidoDataType(sig) != TY_COMPLEX_DOUBLE))
	 {
		IdErrPrintf
		    ("IdSigFFT: Parametrer is NOT a PSIGNAL_COMPLEX_DOUBLE\n");
		return;
	}

	nn = IdSigDimX(sig);
	n = nn;
	j = 1;
	for (i = 1; i < n; i++) {
		if (j > i) {
			SWAP(sig[j - 1].re, sig[i - 1].re);
			SWAP(sig[j - 1].im, sig[i - 1].im);
		}
		m = n >> 1;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	mmax = 1;
	while (n > mmax) {
		istep = 2 * mmax;
		theta = 6.28318530717959 / (-isign * mmax * 2);
		/*
		 * recipes in C
		 * +isign
		 */
		wtemp = sin(0.5 * theta);
		wpr = -2.0 * wtemp * wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		for (m = 0; m < mmax; m++) {
			for (i = m; i < n; i += istep) {
				j = i + mmax;
				tempr = wr * sig[j].re - wi * sig[j].im;
				tempi = wr * sig[j].im + wi * sig[j].re;
				sig[j].re = sig[i].re - tempr;
				sig[j].im = sig[i].im - tempi;
				sig[i].re += tempr;
				sig[i].im += tempi;
			}
			wtemp = wr;
			wr = (wtemp) * wpr - wi * wpi + wr;
			wi = wi * wpr + wtemp * wpi + wi;
		}
		mmax = istep;
	}
	if (isign == -1)
		for (i = 0; i < n; i++) {
			sig[i].re /= nn;
			sig[i].im /= nn;
		}
}
