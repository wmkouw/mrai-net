/*************************************************************************
* $Id: imamax.c,v 1.1 2005/09/09 08:22:49 bellet Exp $
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
*  Description : Calcule le maxima d'une image non COMPLEX
*
**************************************************************************/
#include <math.h>
#include <stdio.h>
#include "idima.h"
#include "iderr.h"
#include "idprint.h"

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaMinima   (fonction)
* 
* RESUME: Calcule le Minimum d'une image non COMPLEX 
* 
* DESCRIPTION: Calcule le Minimum d'une image non COMPLEX 
* 
* SYNTAXE:     double min = IdImaMinima(PPIMAGE image);
* 
* RETOUR:      type  : Double
*              role  : valeur min de l'image.
* 
* PARAMETRES:  nom   : ima
*              type  : PPIMAGE
*              role  : Pointeur vers l'image source
* 
* FICHIER:     IMAMAX.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

#define CCMIN(t1)				    \
          mini = (double) ((t1)ima)[0][0];          \
          for (i=0;i<Y;i++) for (j=0;j<X;j++)       \
          {                                         \
           if (((t1)ima)[i][j]<mini)                 \
                mini = (double) ((t1)ima)[i][j];     \
          }

double IdImaMinima (PPIMAGE ima)
{
  double mini=0.0; /* FB 24-01-2000 */
  int i,j,X,Y;

  X=IdImaDimX(ima);
  Y=IdImaDimY(ima);
 switch(IdImaType(ima))
    {
     case IMA_UCHAR:   CCMIN(PPIMAGE_UCHAR);  break;
     case IMA_CHAR:    CCMIN(PPIMAGE_CHAR);   break;
     case IMA_USHORT:  CCMIN(PPIMAGE_USHORT); break;
     case IMA_SHORT:   CCMIN(PPIMAGE_SHORT);  break;
     case IMA_LONG:    CCMIN(PPIMAGE_LONG);   break;
     case IMA_ULONG:   CCMIN(PPIMAGE_ULONG);   break;
     case IMA_FLOAT:   CCMIN(PPIMAGE_FLOAT);  break;
     case IMA_DOUBLE:  CCMIN(PPIMAGE_DOUBLE);  break;
     default:
      {
       IdPrintf("Type %d non traitable par la fonction IdImaMinima !\n",IdImaType(ima));
       IdExit(0);
      }
    } /* Fin du switch */

 return(mini);
}


/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaMaxima   (fonction)
* 
* RESUME: Calcule le Maximum d'une image non COMPLEX
* 
* DESCRIPTION: Calcule le Maximum d'une image non COMPLEX
* 
* SYNTAXE:    double max = IdImaMaxima (PPIMAGE image);
* 
* RETOUR:      type  : double
*              role  : valeur max de l'image.
* 
* PARAMETRES:  nom   : ima
*              type  : PPIMAGE
*              role  : Pointeur vers l'image source
* 
* FICHIER:     IMAMAX.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */
#include <stdio.h>
#include "idima.h"

#define CC(t1) maxi = (double) ((t1)ima)[0][0];            \
	for (i=0;i<Y;i++) for (j=0;j<X;j++)                \
          {                                                \
           if (((t1)ima)[i][j]>maxi)                        \
                maxi = (double) ((t1)ima)[i][j];            \
          }



double IdImaMaxima(PPIMAGE ima)
{
  double maxi;
  int i,j,X,Y;

  X=IdImaDimX(ima);
  Y=IdImaDimY(ima);
  switch(IdImaType(ima))
    {
     case IMA_UCHAR:    CC(PPIMAGE_UCHAR);    break;
     case IMA_CHAR:     CC(PPIMAGE_CHAR);     break;
     case IMA_USHORT:   CC(PPIMAGE_USHORT);   break;
     case IMA_SHORT:    CC(PPIMAGE_SHORT);    break;
     case IMA_LONG:     CC(PPIMAGE_LONG);     break;
     case IMA_ULONG:    CC(PPIMAGE_ULONG);    break;
     case IMA_FLOAT:    CC(PPIMAGE_FLOAT);    break;
     case IMA_DOUBLE:   CC(PPIMAGE_DOUBLE);   break;
     default:
       IdPrintf("Type %d non traitable par la fonction IdImaMaxima !\n",IdImaType(ima));
	IdErrno=IDERR_WRONG_LIBTYPE;
       return(0);
    } /* Fin du switch */

 return(maxi);
}


/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaMinMax   (fonction)
* 
* RESUME: Calcule le Min et le Maximum d'une image non COMPLEX
* 
* DESCRIPTION: Calcule le Min et le Maximum  d'une image non COMPLEX
* 
* SYNTAXE:     void IdImaMinMax (PPIMAGE image, double *min, double *max);
* 
* RETOUR:      type  : Double
*              role  : valeur max de l'image.
* 
* PARAMETRES:  nom   : ima
*              type  : PPIMAGE
*              role  : Pointeur vers l'image source
* 
* FICHIER:     IMAMAX.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */


/**
 * \ingroup   ima
 * \brief     renvoie le Min et Max d'un PPIMAGE, quelque soit son type
 * @param pi  Pointeur sur le PPIMAGE a traiter.
 * @param mini  Pointeur sur le min a renvoyer (C'est un DOUBLE)
 * @param maxi  Pointeur sur le max a renvoyer (C'est un DOUBLE)
 */


#define CCMM(t1)						\
          *maxi = *mini = (double) ((t1)ima)[0][0];             \
	  for (i=0;i<Y;i++) for (j=0;j<X;j++)            	\
          {                                                     \
           if (((t1)ima)[i][j]>*maxi)       			\
                { *maxi = (double) ((t1)ima)[i][j]; }		\
          else if (((t1)ima)[i][j]<*mini)            		\
                { *mini = (double) ((t1)ima)[i][j]; }		\
          }

int IdImaMinMax (
	PPIMAGE ima,
	double  *mini,
	double  *maxi)

{
  int i,j,X,Y;

  X=IdImaDimX(ima);
  Y=IdImaDimY(ima);


  switch(IdImaType(ima))
    {
     case IMA_UCHAR:    CCMM(PPIMAGE_UCHAR);    break;
     case IMA_CHAR:     CCMM(PPIMAGE_CHAR);     break;
     case IMA_USHORT:   CCMM(PPIMAGE_USHORT);   break;
     case IMA_SHORT:    CCMM(PPIMAGE_SHORT);    break;
     case IMA_LONG:     CCMM(PPIMAGE_LONG);     break;
     case IMA_ULONG:    CCMM(PPIMAGE_ULONG);    break;
     case IMA_FLOAT:    CCMM(PPIMAGE_FLOAT);    break;
     case IMA_DOUBLE:   CCMM(PPIMAGE_DOUBLE);   break;
     default:
       IdPrintf("Type %d non traitable par la fonction IdImaMinMax !\n",IdImaType(ima));
	IdErrno=IDERR_WRONG_LIBTYPE;
       return(0);
    } /* Fin du switch */
return(1);
 }

#define CCMOY(t1) for (i=0;i<Y;i++) for (j=0;j<X;j++)         \
          {                                                   \
                moy += (double) ((t1)ima)[i][j];              \
          }

/**
 * \ingroup     ima
 * \brief       Compute the mean value of a full image.
 * @param ima   Image of requested mean value.
 * @return      Computed mean value.
 */
double IdImaMoyenne (PPIMAGE ima)
{
  double moy;
  int i,j,X,Y;

  X=IdImaDimX(ima);
  Y=IdImaDimY(ima);
  moy = 0.0;
  switch(IdImaType(ima))
    {
     case IMA_UCHAR:    CCMOY(PPIMAGE_UCHAR);    break;
     case IMA_CHAR:     CCMOY(PPIMAGE_CHAR);     break;
     case IMA_USHORT:   CCMOY(PPIMAGE_USHORT);   break;
     case IMA_SHORT:    CCMOY(PPIMAGE_SHORT);    break;
     case IMA_LONG:     CCMOY(PPIMAGE_LONG);     break;
     case IMA_ULONG:    CCMOY(PPIMAGE_ULONG);    break;
     case IMA_FLOAT:    CCMOY(PPIMAGE_FLOAT);    break;
     case IMA_DOUBLE:   CCMOY(PPIMAGE_DOUBLE);   break;
     default:
       IdPrintf("Type %d non traitable par la fonction IdImaMoyenne !\n",IdImaType(ima));
	IdErrno=IDERR_WRONG_LIBTYPE;
       return(0);
    } /* Fin du switch */

     moy = moy /((double)X*(double)Y);

     return moy;
 }

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaVariance   (fonction)
* 
* RESUME: Calcule la Variance d'une image non COMPLEX
* 
* DESCRIPTION: Calcule la Variance d'une image non COMPLEX
* 
* SYNTAXE:     double var = IdImaVariance (PPIMAGE image);
* 
* RETOUR:      type  : Double
*              role  : variance de l'image.
* 
* PARAMETRES:  nom   : ima
*              type  : PPIMAGE
*              role  : Pointeur vers l'image source
* 
* FICHIER:     IMAMAX.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

#define CCVAR(t1) for (i=0;i<Y;i++) for (j=0;j<X;j++)   \
          {                                             \
           var += pow((moy -(double) ( ((t1)ima)[i][j])),2.0 );     \
          }


double IdImaVariance( PPIMAGE ima)
{
  double moy,var;
  int i,j,X,Y;

  X = IdImaDimX(ima);
  Y = IdImaDimY(ima);
  moy = IdImaMoyenne(ima);
  var = 0.0;

  switch(IdImaType(ima))
    {
     case IMA_UCHAR:    CCVAR(PPIMAGE_UCHAR);    break;
     case IMA_CHAR:     CCVAR(PPIMAGE_CHAR);     break;
     case IMA_USHORT:   CCVAR(PPIMAGE_USHORT);   break;
     case IMA_SHORT:    CCVAR(PPIMAGE_SHORT);    break;
     case IMA_LONG:     CCVAR(PPIMAGE_LONG);     break;
     case IMA_ULONG:    CCVAR(PPIMAGE_ULONG);    break;
     case IMA_FLOAT:    CCVAR(PPIMAGE_FLOAT);    break;
     case IMA_DOUBLE:   CCVAR(PPIMAGE_DOUBLE);   break;
     default:
       IdPrintf("Type %d non traitable par la fonction IdImaVariance !\n",IdImaType(ima));
	IdErrno=IDERR_WRONG_LIBTYPE;
       return(0);
    } /* Fin du switch */

var = var /((double)X*(double)Y);

 return(var);
}

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaMoyVar   (fonction)
* 
* RESUME: Calcule la Moyenne et la Variance d'une image non COMPLEX
* 
* DESCRIPTION: Calcule la Moyenne et la Variance d'une image non COMPLEX
* 
* SYNTAXE:     int retCode = IdImaMoyVar (PPIMAGE image, double *moy, double *var);
* 
* RETOUR:      type  : int
*              role  : code retour (1:OK 0:echec)
* 
* PARAMETRES:  nom   : ima
*              type  : PPIMAGE
*              role  : Pointeur vers l'image source
* 
*  		 	 nom   : moy
* 			 type  : double *
* 			 role  : Pointeur vers la Moy
* 
* 		 	 nom   : var
* 			 type  : double *
* 			 role  : Pointeur vers la Var
* * 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */

/**
 * \ingroup   ima
 * \brief     renvoie lea Moyenne et la Variance d'un PPIMAGE, quelque soit son type
 * @param pi  Pointeur sur le PPIMAGE a traiter.
 * @param moy  Pointeur sur la moyenne  a renvoyer (C'est un DOUBLE)
 * @param var  Pointeur sur la variance a renvoyer (C'est un DOUBLE)
 */


#define CCMV(t1) for (i=0;i<Y;i++) for (j=0;j<X;j++)            \
	   	*moy += (double) ((t1)ima)[i][j];		\
		*moy = *moy /((double)X*(double)Y);		\
		 for (i=0;i<Y;i++) for (j=0;j<X;j++)		\
	   	*var += pow((*moy - ((t1)ima)[i][j]),2.0);

int IdImaMoyVar (
	PPIMAGE ima,
	double  *moy,
	double  *var)
{
int i,j,X,Y;
  X=IdImaDimX(ima);
  Y=IdImaDimY(ima);

 *moy = 0.0;
 *var = 0.0;
  switch(IdImaType(ima))
    {
     case IMA_UCHAR:    CCMV(PPIMAGE_UCHAR);    break;
     case IMA_CHAR:     CCMV(PPIMAGE_CHAR);     break;
     case IMA_USHORT:   CCMV(PPIMAGE_USHORT);   break;
     case IMA_SHORT:    CCMV(PPIMAGE_SHORT);    break;
     case IMA_LONG:     CCMV(PPIMAGE_LONG);     break;
     case IMA_ULONG:    CCMV(PPIMAGE_ULONG);    break;
     case IMA_FLOAT:    CCMV(PPIMAGE_FLOAT);    break;
     case IMA_DOUBLE:   CCMV(PPIMAGE_DOUBLE);   break;
     default:
       IdPrintf("Type %d non traitable par la fonction IdImaMoyVar !\n",IdImaType(ima));
	IdErrno=IDERR_WRONG_LIBTYPE;
      return(0);
    } /* Fin du switch */

 *var = *var /((double)X*(double)Y);
 return(1);

 }



/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaDyn   (fonction)
* 
* RESUME: Calcule la dynamique d'une image non COMPLEX (reel)
* 
* DESCRIPTION: Calcule la dynamique d'une image non COMPLEX (reel)
* 
* SYNTAXE:     double dyn = IdImaDyn (PPIMAGE image);
* 
* RETOUR:      type  : double
*              role  : Dynamique de l'image.
* 
* PARAMETRES:  nom   : ima
*              type  : PPIMAGE
*              role  : Pointeur vers l'image source
* 
* FICHIER:     IMAMAX.c
* 
* EXEMPLE:
* 
******************************************************** END DESCRIPTION */


/**
 * \ingroup     ima
 * \brief       Calcule la Dynamique (max - min) d'une image, quelque soit son type
 * @param ima   PPIMAGE dont on calcule la dynamique
 * @return      dynamique calculee  (c'est un DOUBLE)
 */

double IdImaDyn (PPIMAGE ima)
{
 double dyn;

 dyn = (double)IdImaMaxima(ima) -(double)IdImaMinima(ima);
 return(dyn);
}
