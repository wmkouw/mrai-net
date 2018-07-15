/*************************************************************************
* $Id: volmin.c,v 1.1 2005/09/09 08:22:56 bellet Exp $
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
 *  Description :  volume non COMPLEXE
 */

#include <stdio.h>
#include <float.h>
#include "idvol.h"
#include "iderr.h"
#include "idprint.h"


/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdVolMinPlanX   (fonction)
 * 
 * RESUME: Calcule le Minimum d'un plan X d'un Vol non COMPLEX
 * 
 * DESCRIPTION: Calcule le Minimum d'un plan X d'un Vol non COMPLEX
 * 
 * SYNTAXE:    double min = IdVolMinX (PPPVOLUME volume,int numPlanX);
 * 
 * RETOUR:      type  : double
 * role  : valeur min du plan.
 * 
 * PARAMETRES:  nom   : vol
 * type  : PPPVOLUME
 * role  : Pointeur vers le Volume source
 * 
 * nom : x
 * type: entier
 * role :numero du plan en X dont on calcule le min
 * 
 * FICHIER: minvol.c    
 * 
 * EXEMPLE:
 * 
 * ******************************************************** END DESCRIPTION 
 */


/**
 * \ingroup        vol
 * \brief	   Calcule le Minimum d'un plan X d'un Vol non COMPLEX
 *
 * @param vol 	Pointeur vers le Volume source
 * @param x	numero du plan en X dont on calcule le min
 * @return	valeur min du plan.
 *
 */


#define CC(t1)	mini = (double) ((t1)vol)[0][0][X];	\
   	for (i=0;i<Z;i++) for (j=0;j<Y;j++)	\
	  {						\
	   if (((t1)vol)[i][j][X]<mini)			\
		mini = (double) ((t1)vol)[i][j][X];	\
	  }

double
IdVolMinPlanX(PPPVOLUME vol, int X)
{
	double mini;
	int i, j, Y, Z;

	Z = IdVolDimZ(vol);
	Y = IdVolDimY(vol);
	mini = FLT_MAX;
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
	case VOL_ULONG:
		CC(PPPVOLUME_ULONG);
		break;
	case VOL_FLOAT:
		CC(PPPVOLUME_FLOAT);
		break;
	case VOL_DOUBLE:
		CC(PPPVOLUME_DOUBLE);
		break;
	default:
		IdPrintf
		    ("Type %d non traitable par la fonction IdVolMinPlanX !\n",
		     IdVolType(vol));
		IdExit(0);
	} // switch

	return (mini);
}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdVolMinPlanY   (fonction)
 * 
 * RESUME: Calcule le Minimum d'un plan Y d'un Volume non COMPLEX
 * 
 * DESCRIPTION: Calcule le Minimum d'un plan Y d'un Volume non COMPLEX
 * 
 * SYNTAXE:    double min = IdVolMinY (PPPVOLUME volume, int numPlanY);
 * 
 * RETOUR:      type  : Double
 * acces : O
 * role  : valeur min du plan.
 * 
 * PARAMETRES:  nom   : vol
 * type  : PPPVOLUME
 * acces : I, N
 * role  : Pointeur vers le Volume source
 * 
 * nom : y
 * type: entier
 * acces: I
 * role :numero du plan en Y dont on calcule le min
 * 
 * VERSION:     V1.00
 * 
 * FICHIER:    minvol.c 
 * 
 * EXEMPLE:
 * 
 * ******************************************************** END DESCRIPTION 
 */


/**
 * \ingroup        vol
 * \brief	   Calcule le Minimum d'un plan Y d'un Vol non COMPLEX
 *
 * @param vol 	Pointeur vers le Volume source
 * @param x	numero du plan en Y dont on calcule le min
 * @return	valeur min du plan.
 *
 */



#include <stdio.h>
#include "idvol.h"

#define DD(t1)	mini = (double) ((t1)vol)[0][Y][0];  \
 	for (i=0;i<Z;i++) for (k=0;k<X;k++)      \
	  {                                         \
	   if (((t1)vol)[i][Y][k]<mini)              \
		mini = (double) ((t1)vol)[i][Y][k];  \
	  }

double
IdVolMinPlanY(PPPVOLUME vol, int Y)
{
	double mini=0.;
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
		IdPrintf
		    ("Type %d non traitable par la fonction IdVolMinPlanY !\n",
		     IdVolType(vol));
		IdExit(0);
	} // switch

	return (mini);
}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdVolMinPlanZ   (fonction)
 * 
 * RESUME: Calcule le Minimum d'un plan Z d'un Volume non COMPLEX
 * 
 * DESCRIPTION: Calcule le Minimum d'un plan Z d'un Volume non COMPLEX
 * 
 * SYNTAXE:    double min = IdVolMinZ (PPPVOLUME volume,int numPlanZ);
 * 
 * RETOUR:      type  : Double
 * acces : O
 * role  : valeur min du plan.
 * 
 * PARAMETRES:  nom   : vol
 * type  : PPPVOLUME
 * acces : I, N
 * role  : Pointeur vers le Volume source
 * 
 * nom : y
 * type: entier
 * acces: I
 * role :numero du plan en Z dont on calcule le min
 * 
 * VERSION:     V1.00
 * 
 * FICHIER:    minvol.c 
 * 
 * EXEMPLE:
 * 
 * ******************************************************** END DESCRIPTION 
 */

/**
 * \ingroup        vol
 * \brief	   Calcule le Minimum d'un plan Z d'un Vol non COMPLEX
 *
 * @param vol 	Pointeur vers le Volume source
 * @param x	numero du plan en Z dont on calcule le min
 * @return	valeur min du plan.
 *
 */


#include <stdio.h>
#include "idvol.h"

#define EE(t1) 	mini = (double) ((t1)vol)[Z][0][0]; \
 for (j=0;j<Y;j++) for (k=0;k<X;k++) \
	  {                                         \
	   if (((t1)vol)[Z][j][k]<mini)             \
		mini = (double) ((t1)vol)[Z][j][k]; \
	  }

double
IdVolMinPlanZ(PPPVOLUME vol, int Z)
{
	double mini=0.;
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
		IdPrintf
		    ("Type %d non traitable par la fonction IdVolMinPlanZ !\n",
		     IdVolType(vol));
		IdExit(0);
	} // switch

	return (mini);
}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdVolMinima   (fonction)
 * 
 * RESUME: Calcule le Minimum d'un volume non COMPLEX
 * 
 * DESCRIPTION: Calcule le Minimum d'un volume non COMPLEX
 * 
 * SYNTAXE:     double min = IdVolMinima (PPPVOLUME volume);
 * 
 * RETOUR:      type  : Double
 * acces : O
 * role  : valeur min du volume.
 * 
 * PARAMETRES:  nom   : vol
 * type  : PPPVOLUME
 * acces : I, N
 * role  : Pointeur vers le volume source
 * 
 * VERSION:     V1.00
 * 
 * FICHIER:     minvol.c     
 * 
 * EXEMPLE:
 * 
 * ******************************************************** END DESCRIPTION 
 */

/**
 * \ingroup        vol
 * \brief	   Calcule le Minimum  d'un Vol non COMPLEX
 *
 * @param vol 	Pointeur vers le Volume source
 * @return	valeur min du plan.
 *
 */



#include <stdio.h>
#include "idvol.h"

#define FF(t1) 	mini = (double) ((t1)vol)[0][0][0]; \
 	for (t=0;t<Z;t++) for (i=0;i<Y;i++) for (j=0;j<X;j++) \
	  {                                                     \
	   if (((t1)vol)[t][i][j]<mini)             \
		mini = (double) ((t1)vol)[t][i][j]; \
	  }


double
IdVolMinima(PPPVOLUME vol)
{
	double mini=0.;
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
		IdPrintf
		    ("Type %d non traitable par la fonction IdVolMinima !\n",
		     IdVolType(vol));
		IdExit(0);
	} // switch

	return (mini);
}
