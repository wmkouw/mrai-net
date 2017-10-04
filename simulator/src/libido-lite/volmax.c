/*************************************************************************
* $Id: volmax.c,v 1.1 2005/09/09 08:22:56 bellet Exp $
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

#include <stdio.h>
#include <math.h>
#include "idvol.h"
#include "iderr.h"
#include "idprint.h"
/*************************************************************************
*
*  Description : Calcule le maxima d'un plan d'un volume non COMPLEXE
*
**************************************************************************/


/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdVolMaxPlanX   (fonction)
 * 
 * RESUME: Calcule le Maximum d'un plan X d'un Vol non COMPLEX
 * 
 * DESCRIPTION: Calcule le Maximum d'un plan X d'un Vol non COMPLEX
 * 
 * SYNTAXE:     double max = IdVolMaxX (PPPVOLUME volume,int numPlanX);
 * 
 * RETOUR:      type  : Double
 * acces : O
 * role  : valeur max du plan.
 * 
 * PARAMETRES:  nom   : vol
 * type  : PPPVOLUME
 * acces : I, N
 * role  : Pointeur vers le Volume source
 * 
 * nom : x
 * type: entier
 * acces: I
 * role :numero du plan en X dont on calcule le max
 * 
 * VERSION:     V1.00
 * 
 * FICHIER:     maxvol.c     
 * 
 * EXEMPLE:
 * 
 * ******************************************************** END DESCRIPTION 
 */

#define CC(t1)  maxi = (double) ((t1)vol)[0][0][X];	\
          for (i=0;i<Z;i++) for (j=0;j<Y;j++)	\
	  {						\
	   if (((t1)vol)[i][j][X]>maxi)			\
		maxi = (double) ((t1)vol)[i][j][X];	\
	  }

double IdVolMaxPlanX(PPPVOLUME vol, int X)
{
	double maxi;
	int i, j, Y, Z;

	Z = IdVolDimZ(vol);
	Y = IdVolDimY(vol);
	switch (IdVolType(vol)) {
	case VOL_CHAR:
		CC(PPPVOLUME_CHAR);
		break;
	case VOL_UCHAR:
		CC(PPPVOLUME_UCHAR);
		break;
	case VOL_SHORT:
		CC(PPPVOLUME_SHORT);
		break;
	case VOL_USHORT:
		CC(PPPVOLUME_USHORT);
		break;
	case VOL_LONG:
		CC(PPPVOLUME_LONG);
		break;
	case VOL_FLOAT:
		CC(PPPVOLUME_FLOAT);
		break;
	case VOL_DOUBLE:
		CC(PPPVOLUME_DOUBLE);
		break;
	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdVolMaxPlanX !\n",
		     IdVolType(vol));

		return (0);
	} // switch 

	return (maxi);
}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdVolMaxPlanY   (fonction)
 * 
 * RESUME: Calcule le Maximum d'un plan Y d'un Volume non COMPLEX
 * 
 * DESCRIPTION: Calcule le Maximum d'un plan Y d'un Volume non COMPLEX
 * 
 * SYNTAXE:     double maxi = IdVolMaxY (PPPVOLUME volume,int numPlanY);
 * 
 * RETOUR:      type  : Double
 * acces : O
 * role  : valeur max du plan.
 * 
 * PARAMETRES:  nom   : vol
 * type  : PPPVOLUME
 * acces : I, N
 * role  : Pointeur vers le Volume source
 * 
 * nom : y
 * type: entier
 * acces: I
 * role :numero du plan en Y dont on calcule le max
 * 
 * VERSION:     V1.00
 * 
 * FICHIER:     maxvol.c         
 * 
 * EXEMPLE:
 * 
 * ******************************************************** END DESCRIPTION 
 */

#define DD(t1) maxi = (double) ((t1)vol)[0][Y][0];  \
	for (i=0;i<Z;i++) for (k=0;k<X;k++)      \
	  {                                         \
	   if (((t1)vol)[i][Y][k]>maxi)              \
		maxi = (double) ((t1)vol)[i][Y][k];  \
	  }

double IdVolMaxPlanY(PPPVOLUME vol, int Y)
{
	double maxi;
	int i, k, X, Z;

	X = IdVolDimX(vol);
	Z = IdVolDimZ(vol);
	switch (IdVolType(vol)) {
	case VOL_CHAR:
		DD(PPPVOLUME_CHAR);
		break;
	case VOL_UCHAR:
		DD(PPPVOLUME_UCHAR);
		break;
	case VOL_SHORT:
		DD(PPPVOLUME_SHORT);
		break;
	case VOL_USHORT:
		DD(PPPVOLUME_USHORT);
		break;
	case VOL_LONG:
		DD(PPPVOLUME_LONG);
		break;
	case VOL_FLOAT:
		DD(PPPVOLUME_FLOAT);
		break;
	case VOL_DOUBLE:
		DD(PPPVOLUME_DOUBLE);
		break;
	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdVolMaxPlanY !\n",
		     IdVolType(vol));
		IdErrno = IDERR_WRONG_LIBTYPE;
		return (0);
	} // switch

	return (maxi);
}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdVolMaxPlanZ   (fonction)
 * 
 * RESUME: Calcule le Maximum d'un plan Z d'un Volume non COMPLEX
 * 
 * DESCRIPTION: Calcule le Maximum d'un plan Z d'un Volume non COMPLEX
 * 
 * SYNTAXE:     double max = IdVolMaxZ (PPPVOLUME volume,int numPlanZ);
 * 
 * RETOUR:      type  : Double
 * acces : O
 * role  : valeur max du plan.
 * 
 * PARAMETRES:  nom   : vol
 * type  : PPPVOLUME
 * acces : I, N
 * role  : Pointeur vers le Volume source
 * 
 * nom : y
 * type: entier
 * acces: I
 * role :numero du plan en Z dont on calcule le max
 * 
 * VERSION:     V1.00
 * 
 * FICHIER:     maxvol.c    
 * 
 * EXEMPLE:
 * 
 * ******************************************************** END DESCRIPTION 
 */

#define EE(t1) 	maxi = (double) ((t1)vol)[Z][0][0]; 		\
		for (j=0;j<Y;j++) for (k=0;k<X;k++)     	\
	  {                                                     \
	   if (((t1)vol)[Z][j][k]>maxi)             		\
		maxi = (double) ((t1)vol)[Z][j][k]; 		\
	  }

double IdVolMaxPlanZ(PPPVOLUME vol, int Z)
{
	double maxi;
	int j, k, X, Y;

	X = IdVolDimX(vol);
	Y = IdVolDimY(vol);
	switch (IdVolType(vol)) {
	case VOL_CHAR:
		EE(PPPVOLUME_CHAR);
		break;
	case VOL_UCHAR:
		EE(PPPVOLUME_UCHAR);
		break;
	case VOL_SHORT:
		EE(PPPVOLUME_SHORT);
		break;
	case VOL_USHORT:
		EE(PPPVOLUME_USHORT);
		break;
	case VOL_LONG:
		EE(PPPVOLUME_LONG);
		break;
	case VOL_FLOAT:
		EE(PPPVOLUME_FLOAT);
		break;
	case VOL_DOUBLE:
		EE(PPPVOLUME_DOUBLE);
		break;
	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdVolMaxPlanZ !\n",
		     IdVolType(vol));
		IdErrno = IDERR_WRONG_LIBTYPE;
		return (0);
	}			/*
				 * Fin du switch 
				 */

	return (maxi);
}



/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdVolMaxima   (fonction)
 * 
 * RESUME: Calcule le Maximum d'un volume non COMPLEX
 * 
 * DESCRIPTION: Calcule le Maximum d'un volume non COMPLEX
 * 
 * SYNTAXE:     double maxi = IdVolMaxima (PPPVOLUME volume);
 * 
 * RETOUR:      type  : Double
 * acces : O
 * role  : valeur max du volume.
 * 
 * PARAMETRES:  nom   : vol
 * type  : PPPVOLUME
 * acces : I, N
 * role  : Pointeur vers le volume source
 * 
 * VERSION:     V1.00
 * 
 * FICHIER:     maxvol.c     
 * 
 * EXEMPLE:
 * 
 * ******************************************************** END DESCRIPTION 
 */

#define FF(t1) 	maxi = (double) ((t1)vol)[0][0][0];		\
  for (t=0;t<Z;t++) for (i=0;i<Y;i++) for (j=0;j<X;j++) 	\
	  {                                                     \
	   if (((t1)vol)[t][i][j]>maxi)             		\
		maxi = (double) ((t1)vol)[t][i][j]; 		\
	  }


double IdVolMaxima(PPPVOLUME vol)
{
	double maxi;
	int i, j, t, X, Y, Z;

	X = IdVolDimX(vol);
	Y = IdVolDimY(vol);
	Z = IdVolDimZ(vol);
	switch (IdVolType(vol)) {
	case VOL_CHAR:
		FF(PPPVOLUME_CHAR);
		break;
	case VOL_UCHAR:
		FF(PPPVOLUME_UCHAR);
		break;
	case VOL_SHORT:
		FF(PPPVOLUME_SHORT);
		break;
	case VOL_USHORT:
		FF(PPPVOLUME_USHORT);
		break;
	case VOL_LONG:
		FF(PPPVOLUME_LONG);
		break;
	case VOL_FLOAT:
		FF(PPPVOLUME_FLOAT);
		break;
	case VOL_DOUBLE:
		FF(PPPVOLUME_DOUBLE);
		break;
	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdVolMaxima !\n",
		     IdVolType(vol));
		IdErrno = IDERR_WRONG_LIBTYPE;
		return (0);
	} // switch

	return (maxi);
}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdVolMoyenne   (fonction)
 * 
 * RESUME: Calcule la Moyenne d'un volume non COMPLEX
 * 
 * DESCRIPTION: Calcule la Moyenne d'un volume non COMPLEX
 * 
 * SYNTAXE:     double moy = IdVolMoyenne(PPPVOLUME volume);
 * 
 * RETOUR:      type  : Double
 * acces : O
 * role  : valeur moyenne du volume.
 * 
 * PARAMETRES:  nom   : vol
 * type  : PPPVOLUME
 * acces : I, N
 * role  : Pointeur vers le volume source
 * 
 * VERSION:     V1.00
 * 
 * FICHIER:     maxvol.c     
 * 
 * EXEMPLE:
 * 
 * ******************************************************** END DESCRIPTION 
 */

#define HH(t1)  for (t=0;t<Z;t++) for (i=0;i<Y;i++) for (j=0;j<X;j++) \
	  {                                                     \
	   moy += (double) ((t1)vol)[t][i][j];			\
	  }


double IdVolMoyenne(PPPVOLUME vol)
{
	double moy;
	int i, j, t, X, Y, Z;

	X = IdVolDimX(vol);
	Y = IdVolDimY(vol);
	Z = IdVolDimZ(vol);

	moy = 0.0;

	switch (IdVolType(vol)) {
	case VOL_CHAR:
		HH(PPPVOLUME_CHAR);
		break;
	case VOL_UCHAR:
		HH(PPPVOLUME_UCHAR);
		break;
	case VOL_SHORT:
		HH(PPPVOLUME_SHORT);
		break;
	case VOL_USHORT:
		HH(PPPVOLUME_USHORT);
		break;
	case VOL_LONG:
		HH(PPPVOLUME_LONG);
		break;
	case VOL_FLOAT:
		HH(PPPVOLUME_FLOAT);
		break;
	case VOL_DOUBLE:
		HH(PPPVOLUME_DOUBLE);
		break;
	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdVolMoyenne !\n",
		     IdVolType(vol));
		IdErrno = IDERR_WRONG_LIBTYPE;
		return (0);
	} // switch

	moy = moy / ((double) X * (double) Y * (double) Z);

	return (moy);
}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdVolVariance   (fonction)
 * 
 * RESUME: Calcule la Variance d'un volume non COMPLEX
 * 
 * DESCRIPTION: Calcule la Variance d'un volume non COMPLEXe
 * 
 * SYNTAXE:     double var = IdVolVariance (PPPVOLUME volume);
 * 
 * RETOUR:      type  : Double
 * acces : O
 * role  : variance du volume.
 * 
 * PARAMETRES:  nom   : vol
 * type  : PPPVOLUME
 * acces : I, N
 * role  : Pointeur vers le volume source
 * 
 * VERSION:     V1.00
 * 
 * FICHIER:     maxvol.c
 * 
 * EXEMPLE:
 * 
 * ******************************************************** END DESCRIPTION 
 */

#define II(t1)								\
		for (t=0;t<Z;t++) for (i=0;i<Y;i++) for (j=0;j<X;j++)	\
	   	var += pow((moy - ((t1)vol)[t][i][j]),2.0);


double IdVolVariance(PPPVOLUME vol)
{
	double moy, var;
	int i, j, t, X, Y, Z;

	X = IdVolDimX(vol);
	Y = IdVolDimY(vol);
	Z = IdVolDimZ(vol);

	moy = IdVolMoyenne(vol);
	var = 0.0;

	switch (IdVolType(vol)) {
	case VOL_CHAR:
		II(PPPVOLUME_CHAR);
		break;
	case VOL_UCHAR:
		II(PPPVOLUME_UCHAR);
		break;
	case VOL_SHORT:
		II(PPPVOLUME_SHORT);
		break;
	case VOL_USHORT:
		II(PPPVOLUME_USHORT);
		break;
	case VOL_LONG:
		II(PPPVOLUME_LONG);
		break;
	case VOL_FLOAT:
		II(PPPVOLUME_FLOAT);
		break;
	case VOL_DOUBLE:
		II(PPPVOLUME_DOUBLE);
		break;
	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdVolVariance !\n",
		     IdVolType(vol));
		IdErrno = IDERR_WRONG_LIBTYPE;
		return (0);
	} // switch

	var = var / ((double) X * (double) Y * (double) Z);

	return (var);
}


/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdVolMoyVar   (fonction)
 * 
 * RESUME: Renvoie la Moyenne et la Variance d'un volume non COMPLEX
 * 
 * DESCRIPTION: Renvoie la Moyenne et la Variance d'un volume non COMPLEX
 * 
 * SYNTAXE:     int retCode = IdVolMoyVar (PPPVOLUME volume, double * Moy, double * Var);
 * 
 * RETOUR:                       type  : int
 * acces : O
 * role  : 0: echec 1 : OK
 * 
 * PARAMETRES:           nom   : vol
 * type  : PPPVOLUME
 * acces : I, N
 * role  : Pointeur vers le volume source
 * 
 * nom   : moy
 * type  : double *
 * acces : I, N
 * role  : Pointeur vers la Moy
 * 
 * nom   : var
 * type  : double *
 * acces : I, N
 * role  : Pointeur vers la Var
 * 
 * VERSION:     V1.00
 * 
 * FICHIER:     maxvol.c
 * 
 * EXEMPLE:
 * 
 * ******************************************************** END DESCRIPTION 
 */

#define JJ(t1)  for (t=0;t<Z;t++) for (i=0;i<Y;i++) for (j=0;j<X;j++)	\
	   	*moy += (double) ((t1)vol)[t][i][j];			\
		*moy = *moy /((double)X*(double)Y*(double)Z);		\
		for (t=0;t<Z;t++) for (i=0;i<Y;i++) for (j=0;j<X;j++)	\
	   	*var += pow((*moy - ((t1)vol)[t][i][j]),2.0);


int IdVolMoyVar(PPPVOLUME vol, double *moy, double *var)
{
	int i, j, t, X, Y, Z;

	X = IdVolDimX(vol);
	Y = IdVolDimY(vol);
	Z = IdVolDimZ(vol);

	*moy = 0.0;
	*var = 0.0;

	switch (IdVolType(vol)) {
	case VOL_CHAR:
		JJ(PPPVOLUME_CHAR);
		break;
	case VOL_UCHAR:
		JJ(PPPVOLUME_UCHAR);
		break;
	case VOL_SHORT:
		JJ(PPPVOLUME_SHORT);
		break;
	case VOL_USHORT:
		JJ(PPPVOLUME_USHORT);
		break;
	case VOL_LONG:
		JJ(PPPVOLUME_LONG);
		break;
	case VOL_FLOAT:
		JJ(PPPVOLUME_FLOAT);
		break;
	case VOL_DOUBLE:
		JJ(PPPVOLUME_DOUBLE);
		break;
	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdVolMoyVar !\n",
		     IdVolType(vol));
		IdErrno = IDERR_WRONG_LIBTYPE;
		return (0);
	} // switch

	*var = *var / ((double) X * (double) Y * (double) Z);

	return (1);
}

/**
 * \ingroup vol
 * \brief       Compute minimum and maximum of a volume (non COMPLEX)
 * @param vol   Pointer to concerned volume.
 * @param mini  Computed minimum.
 * @param maxi  Computed maximum.
 * @return      1 on succes, 0 on failure
 */
int IdVolMinMax(PPPVOLUME vol, double *mini, double *maxi)
{
	#define GG(t1) 	*mini = *maxi = (double) ((t1)vol)[0][0][0];  \
	 for (t=0;t<Z;t++) for (i=0;i<Y;i++) for (j=0;j<X;j++)        \
		  {                                                   \
		   if (((t1)vol)[t][i][j]>*maxi)                      \
			*maxi = (double) ((t1)vol)[t][i][j];          \
		   if (((t1)vol)[t][i][j]<*mini)                      \
			*mini = (double) ((t1)vol)[t][i][j];          \
		  }

	int i, j, t, X, Y, Z;

	X = IdVolDimX(vol);
	Y = IdVolDimY(vol);
	Z = IdVolDimZ(vol);

	switch (IdVolType(vol)) {
	case VOL_CHAR:
		GG(PPPVOLUME_CHAR);
		break;
	case VOL_UCHAR:
		GG(PPPVOLUME_UCHAR);
		break;
	case VOL_SHORT:
		GG(PPPVOLUME_SHORT);
		break;
	case VOL_USHORT:
		GG(PPPVOLUME_USHORT);
		break;
	case VOL_LONG:
		GG(PPPVOLUME_LONG);
		break;
	case VOL_FLOAT:
		GG(PPPVOLUME_FLOAT);
		break;
	case VOL_DOUBLE:
		GG(PPPVOLUME_DOUBLE);
		break;
	default:
		IdErrPrintf
		    ("Type %d non traitable par la fonction IdVolMinMax !\n",
		     IdVolType(vol));
		IdErrno = IDERR_WRONG_LIBTYPE;
		return (0);
	} // switch

	return (1);
}
