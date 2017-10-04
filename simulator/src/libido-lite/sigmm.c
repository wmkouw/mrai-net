/*************************************************************************
* $Id: sigmm.c,v 1.1 2005/09/09 08:22:53 bellet Exp $
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
 * Calcule le max et le min d'un signal non COMPLEX
 */
#include <stdio.h>
#include <float.h>
#include "idsig.h"
#include "iderr.h"
#include "idprint.h"


/**
 * IdSigMax:
 * @sig: pointeur vers le signal source.
 *
 * Calcule le Maximum d'un signal non COMPLEX
 *
 * Return value: valeur max du signal
 */
double 
IdSigMax(PSIGNAL sig)
{
	#define CC(t1) {  for (i=0;i<X;i++) \
		if (((t1)sig)[i]>maxi) \
			{maxi = (double) ((t1)sig)[i]; \
			} \
		}

	double maxi;
	int i, X;

	X = IdSigDimX(sig);
	maxi = -999999.99;
	switch (IdSigType(sig)) {
	case SIG_CHAR:
		CC(PSIGNAL_CHAR);
		break;
	case SIG_UCHAR:
		CC(PSIGNAL_UCHAR);
		break;
	case SIG_SHORT:
		CC(PSIGNAL_SHORT);
		break;
	case SIG_USHORT:
		CC(PSIGNAL_USHORT);
		break;
	case SIG_LONG:
		CC(PSIGNAL_LONG);
		break;
	case SIG_ULONG:
		CC(PSIGNAL_ULONG);
		break;
	case SIG_FLOAT:
		CC(PSIGNAL_FLOAT);
		break;
	case SIG_DOUBLE:
		CC(PSIGNAL_DOUBLE);
		break;
	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdSigMax !\n",
		     IdSigType(sig));
		IdExit(0);
	}

	return (maxi);
}

#define BB(t1) {  for (i=0;i<X;i++) \
	           if (((t1)sig)[i]<mini) \
	               {mini = (double) ((t1)sig)[i]; \
                       } \
               }

/**
 * IdSigMin:
 * @sig: pointeur vers le signal source.
 *
 * Calcule le Minimum d'un signal non COMPLEX
 *
 * Return value: valeur min du signal
 */
double 
IdSigMin(PSIGNAL sig)
{
	double mini;
	int i, X;

	X = IdSigDimX(sig);
	mini = 999999.99;
	switch (IdSigType(sig)) {
	case SIG_CHAR:
		BB(PSIGNAL_CHAR);
		break;
	case SIG_UCHAR:
		BB(PSIGNAL_UCHAR);
		break;
	case SIG_SHORT:
		BB(PSIGNAL_SHORT);
		break;
	case SIG_USHORT:
		BB(PSIGNAL_USHORT);
		break;
	case SIG_LONG:
		BB(PSIGNAL_LONG);
		break;
	case SIG_ULONG:
		BB(PSIGNAL_ULONG);
		break;
	case SIG_FLOAT:
		BB(PSIGNAL_FLOAT);
		break;
	case SIG_DOUBLE:
		BB(PSIGNAL_DOUBLE);
		break;
	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdSigMin !\n",
		     IdSigType(sig));
		IdExit(0);
	}
	return (mini);
}

#define BB2(t1) {  for (i=0;i<X;i++) { \
	           if ((double)((t1)sig)[i]<*mini) \
	               {*mini = (double) ((t1)sig)[i]; \
                       } \
	           if ((double)((t1)sig)[i]>*maxi) \
	               {*maxi = (double) ((t1)sig)[i]; \
                       } 	} \
               }

/**
 * IdSigMinMax:
 * @sig: pointeur vers le signal source
 * @min: pointeur vers le min du signal
 * @max: pointeur vers le max du signal
 *
 * Renvoie le Minimum et le Maximum d'un signal non COMPLEX
 *
 * Return value: code retour (0: echec 1: OK)
 */
int 
IdSigMinMax(PSIGNAL sig, double *mini, double *maxi)
{
	int i, X, retCode = 1;
	X = IdSigDimX(sig);

	*mini = FLT_MAX;
	*maxi = -FLT_MAX;
	switch (IdSigType(sig)) {
	case SIG_CHAR:
		BB2(PSIGNAL_CHAR);
		break;
	case SIG_UCHAR:
		BB2(PSIGNAL_UCHAR);
		break;
	case SIG_SHORT:
		BB2(PSIGNAL_SHORT);
		break;
	case SIG_USHORT:
		BB2(PSIGNAL_USHORT);
		break;
	case SIG_LONG:
		BB2(PSIGNAL_LONG);
		break;
	case SIG_ULONG:
		BB2(PSIGNAL_ULONG);
		break;
	case SIG_FLOAT:
		BB2(PSIGNAL_FLOAT);
		break;
	case SIG_DOUBLE:
		BB2(PSIGNAL_DOUBLE);
		break;
	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdSigMinMax !\n",
		     IdSigType(sig));
		retCode = 0;
	}
	return (retCode);
}

#define BB3(t1) {  for (i=deb;i<fin;i++) { \
	           if ((double)((t1)sig)[i]<*mini) \
	               {*mini = (double) ((t1)sig)[i]; \
                       } \
	           if ((double)((t1)sig)[i]>*maxi) \
	               {*maxi = (double) ((t1)sig)[i]; \
                       } 	} \
               }

/**
 * IdSigMinMaxInterv:
 * @sig: pointeur vers le signal source
 * @deb: debut dans le signal de la recherche du Min,Max
 * @fin: fin dans  le signal de la recherche du Min,Max
 * @mini: pointeur vers le min du signal
 * @maxi: pointeur vers le max du signal
 *
 * Renvoie le Minimum et le Maximum d'un signal non COMPLEX entre 2 bornes
 *
 * Return value: code retour (0: echec 1: OK)
 */
int 
IdSigMinMaxInterv(PSIGNAL sig, int deb, int fin, double *mini, double *maxi)
{
	int i, X, retCode = 1;

	X = IdSigDimX(sig);

	if (deb < 0)
		deb = 0;
	if (fin > X)
		fin = X;

	*mini = FLT_MAX;
	*maxi = -FLT_MAX;
	switch (IdSigType(sig)) {
	case SIG_CHAR:
		BB3(PSIGNAL_CHAR);
		break;
	case SIG_UCHAR:
		BB3(PSIGNAL_UCHAR);
		break;
	case SIG_SHORT:
		BB3(PSIGNAL_SHORT);
		break;
	case SIG_USHORT:
		BB3(PSIGNAL_USHORT);
		break;
	case SIG_LONG:
		BB3(PSIGNAL_LONG);
		break;
	case SIG_ULONG:
		BB3(PSIGNAL_ULONG);
		break;
	case SIG_FLOAT:
		BB3(PSIGNAL_FLOAT);
		break;
	case SIG_DOUBLE:
		BB3(PSIGNAL_DOUBLE);
		break;
	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdSigMinMaxInterv !\n",
		     IdSigType(sig));
		retCode = 0;
	}
	return (retCode);
}
