/*************************************************************************
* $Id: utilcalc.c,v 1.2 2006/12/18 15:10:39 yougz Exp $
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

#include <stdlib.h>
#include "idmath.h"
#include "idmatric.h"
#include "iderr.h"
#include "idprint.h"
#include "idcalc.h"

#define sqr(x) ((x)*(x) )

/**
 * \brief       Retourne le module d'un complexe
 */
double
IdModule(COMPLEX_FLOAT z)
{ 
  return sqrt((double)(z.re * z.re + z.im * z.im));
}
/**
 * \brief       Retourne la phase d'un complexe
 */
double
IdPhase(COMPLEX_FLOAT z)
{ 
  return atan2((double) z.im, (double) z.re);
}
/********************************************************************************************************/		

/**
 * \brief	Calcule la distance entre deux points
 * @param x1	coordonnee en X du premier point
 * @param Y1	coordonnee en Y du premier point
 * @param x2	coordonnee en X du second point
 * @param Y2	coordonnee en Y du second point
 * @return	distance
 */
double 
IdCalcDistance2Points(double x1, double y1, double x2, double y2)
{
	double carre;
	carre = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
	if (carre == 0.0)
		return (0.0);
	else
		return sqrt(carre);
}


/**
 * \brief	Calcule la distance d'un point a une droite
 * @param x	Coordonnees du point
 * @param y	Coordonnees du point (FIXME : duplicated)
 * @param x1	Coordonnees des deux points de la droite
 * @param y1	Coordonnees des deux points de la droite 
 * 		(FIXME : duplicated)
 * @param x2	Coordonnees des deux points de la droite 
 * 		(FIXME : duplicated)
 * @param y2	Coordonnees des deux points de la droite 
 * 		(FIXME : duplicated)
 * @return	distance
 */
double 
IdCalcDistancePointDroite(double x, double y,
			 double x1, double y1,
			 double x2, double y2)
{
	double dist, num, denom;

	num = abs((x - x1) * (y2 - y1) - (x2 - x1) * (y - y1));
	denom = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	if ((x1 == x2) && (y1 == y2))
		dist = 0.0;
	else
		dist = num / denom;

	return (dist);
}

/**
 * \brief	Calcul d'un angle a partir des cotes de l'angle DROIT.
 * @param dx	taille des cotes de l'angle droit
 * @param dy	taille des cotes de l'angle droit (FIXME : duplicated)
 * @return	distance
 */
double 
IdCalcAngleFromCotes(double dx, double dy)
{
	double pente;
	if (dx == 0) {
		if (dy > 0)
			pente = M_PI / 2;
		else
			pente = 3. * M_PI / 2;
	} else {
		pente = ((float) dy) / ((float) dx);
		if (dx > 0) {
			if (dy > 0)
				pente = atan(pente);
			else
				pente = atan(pente) + 2 * M_PI;
		} else
			pente = M_PI + atan(pente);
	}
	return (pente);
}

/**
 * \brief	Calcul d'un angle B A C a partir des points  B,A,C.
 * \warning	ne marche pas si l'un des points A ou C 
 *		est a la verticale de A...
 * @param xB	Coordonnees des points
 * @param yB	Coordonnees des points (FIXME : duplicated)
 * @param xA	Coordonnees des points (FIXME : duplicated)
 * @param yA	Coordonnees des points (FIXME : duplicated)
 * @param xC	Coordonnees des points (FIXME : duplicated)
 * @param yC	Coordonnees des points (FIXME : duplicated)
 * @return	angle
 */

double 
IdCalcAngleFromPoints(double xB, double yB,
			double xA, double yA,
			double xC, double yC)
{
	double alpha, beta, gamma;
	double xAn, yAn, xBn, yBn, xCn, yCn;

	/*
	 * changement de repere : 
	 * translation de l'origine de l'ancien repere en A 
	 */
	xAn = 0.;
	yAn = 0.;
	xBn = xB - xA;
	yBn = yB - yA;
	xCn = xC - xA;
	yCn = yC - yA;

	/*
	 * beta est l'angle forme entre l'axe X' du nouveau repere 
	 * et le segment [AB] 
	 */
	beta = atan2(yBn, xBn);

	/*
	 * gamma est l'angle forme entre l'axe X' du nouveau repere 
	 * et le segment [AC] 
	 */
	gamma = atan2(yCn, xCn);

	/*
	 * alpha = beta - gamma 
	 */
	alpha = beta - gamma;

	alpha = -alpha;
	return (alpha);
}

/**
 * \brief	Rotation d'un point autour d'un centre.
 * @param x		Coordonnees des points
 * @param y		Coordonnees des points (FIXME : duplicated)
 * @param centreX	Coordonnees des points (FIXME : duplicated)
 * @param centreY	Coordonnees des points (FIXME : duplicated)
 * @param angle		Coordonnees des points (FIXME : duplicated)
 * @param nouvX		Coordonnees des points (FIXME : duplicated)
 * @param nouvY		Coordonnees des points (FIXME : duplicated)
 * @return		distance
 */
int 
IdCalcRotatePoint(double x, double y, 
		double centreX, double centreY, 
		double angle, 
		double *nouvX, double *nouvY)
{
	double sinAlph, cosAlph;

	if (angle == 0.0) {
		*nouvX = x;
		*nouvY = y;
		return (1);
	}

	sinAlph = sin(angle);
	cosAlph = cos(angle);

	*nouvX =
	    cosAlph * (x - centreX) - sinAlph * (y - centreY) + centreX;
	*nouvY =
	    sinAlph * (x - centreX) + cosAlph * (y - centreY) + centreY;

	return (1);
}


/**
 * \brief	Augmente d'1 %ge donne la Distance d'un point par 
 * 		rapport a un autre.
 * @param x		Coordonnees des points
 * @param y		Coordonnees des points (FIXME : duplicated)
 * @param centreX	Coordonnees des points (FIXME : duplicated)
 * @param centreY	Coordonnees des points (FIXME : duplicated)
 * @param percent	Pourcentage de modif distance
 * @param nouvX		FIXME : undocumented
 * @param nouvY		FIXME : undocumented
 * @return		distance
 */
int 
IdCalcZoomPoint(double x, double y, 
		double centreX, double centreY, 
		double percent, double *nouvX, double *nouvY)
{
	double angle;
	double oldDist, newDist;

	if (percent == 0.0) {
		*nouvX = x;
		*nouvY = y;
		return (1);
	}

	angle = atan2((double) (y - centreY), (double) (x - centreX));

	oldDist = IdCalcDistance2Points((double) centreX, (double) centreY,
					(double) x, (double) y);
	newDist = oldDist * percent;

	*nouvX = centreX + newDist * cos(angle);
	*nouvY = centreY + newDist * sin(angle);

	return (1);
}

static double sqrarg;
#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)

/**
 * \brief	computes sqrt(a + b) without destructive underflow or overflow
 * @param a	valeur
 * @param b	valeur (FIXME : duplicated)
 * @return	res
 */
double 
IdCalcPythagore(double a, double b)
{
	double absa, absb;

	absa = fabs(a);
	absb = fabs(b);
	if (absa > absb)
		return (absa * sqrt(1.0 + SQR(absb / absa)));
	else
		return (absb ==
			0.0 ? 0.0 : absb * sqrt(1.0 + SQR(absa / absb)));
}

/**
 * \brief	racines d'un polynome de 2eme degre a coefs reels
 *		ax^2+bx+c 
 * @param a	coef
 * @param b	coef (FIXME : duplicated)
 * @param c	coef (FIXME : duplicated)
 * @return	racines
 */
PVECTOR_COMPLEX 
IdCalcRacinesPoly2(double a, double b, double c)
{
	double d, signe, den;
	COMPLEX q;
	PVECTOR_COMPLEX racines;

	racines = (PVECTOR_COMPLEX) IdVectAlloc(2, VECT_COMPLEX);
	if (!racines) {
		IdErrPrintf("\n Erreur allocation solution du 2nd degre");
		IdExit(1);
	}

	d = (double) (b * b - 4.0 * a * c);
	if (b >= 0.0)
		signe = 1.0;
	else
		signe = -1.0;

	if (d >= 0.0) {
		q.re = -(b + signe * sqrt(d)) / 2;
		q.im = 0.0;
	} else {
		q.re = -b / 2;
		q.im = -signe * sqrt(-d) / 2;
	}
	den = q.im * q.im + q.re * q.re;
	racines[0].re = q.re / a;
	racines[0].im = q.im / a;
	racines[1].re = c * q.re / den;
	racines[1].im = -c * q.im / den;

	return (racines);
}

/**
 * \brief	racines d'un polynome de 3eme degre a coefs reels
 *		ax^3+bx^2+cx+d = 0 
 * @param a	coef
 * @param b	coef (FIXME : duplicated)
 * @param c	coef (FIXME : duplicated)
 * @param d	coef (FIXME : duplicated)
 * @return	racines
 */
PVECTOR_COMPLEX 
IdCalcRacinesPoly3(double a, double b, double c, double d)
{
	PVECTOR_COMPLEX racines;
	double q2, Q, R, A=0., B=0., C=0., S, T;
	double teta, teta3, A3, Q3, KPI = (double) M_PI * 2 / 3, signe;

	if (!a) {
		IdErrPrintf("\n Erreur degre du polynome");
		IdExit(1);
	} else {
		A = b / a;
		B = c / a;
		C = d / a;
	}

	racines = (PVECTOR_COMPLEX) IdSigAlloc(3, VECT_COMPLEX);

	if (!racines) {
		IdErrPrintf("\n Erreur allocation solution 3eme degre");
		IdExit(1);
	}

	R = (A * A * A * 2 - A * B * 9 + C * 27) / 54;
	Q = (A * A - B * 3) / 9;
	Q3 = Q * Q * Q;
	A3 = A / 3;

	if (R * R < Q3) {
		teta = acos(R / sqrt(Q3));
		teta3 = teta / 3;
		q2 = -sqrt(Q) * 2;

		racines[0].re = q2 * cos(teta3) - A3;
		racines[0].im = (double) 0.0;

		racines[1].re = q2 * cos(teta3 + KPI) - A3;
		racines[1].im = (double) 0.0;

		racines[2].re = q2 * cos(teta3 - KPI) - A3;
		racines[2].im = (double) 0.0;

	} else {
		if (R >= (double) 0.0)
			signe = (double) 1.0;
		else {
			signe = (double) (-1.0);
			R = -R;
		}
		S =
		    -signe * pow((R + sqrt(R * R - Q3)),
				 (double) (1.0 / 3.0));
		if (S != (double) 0.0)
			T = Q / S;
		else
			T = (double) 0.0;

		racines[0].re = S + T - A3;
		racines[0].im = (double) 0.0;

		racines[1].re = -(S + T) / 2 - A3;
		racines[1].im = (S - T) * sqrt((double) 3.0) / 2;

		racines[2].re = -(S + T) / 2 - A3;
		racines[2].im = -(S - T) * sqrt((double) 3.0) / 2;
	}

	return (racines);
}

/**
 * \brief	Nombre d'elements designes par une Int Enum (voir IdStrIntEnum)
 *		Exemple :
 *		tabInterv :  22,25,35,35,43,45
 *		nbInterv  :     3
 *		retour    :     8
 * @param tabInterv	table d'intervalles (ex : 22,25,35,35,43,45)
 * @param nbInterv	nb d'intervalles (3, dans l'exemple). 
 * @return		Nombre d'elements designes (8 dans l'exemple)
 */
int 
IdCalcNbElemIntEnum(int *tabInterv, int nbInterv)
{
	int nbElem = 0;
	int i;
	for (i = 0; i < nbInterv * 2; i += 2)
		nbElem = nbElem + tabInterv[i + 1] - tabInterv[i] + 1;
	return (nbElem);
}

/**
 * \brief	Procedure d'arrondi double->int
 * @param x	valeur a arrondir
 * @return	arrondi
 */
int 
IdCalcArrondInt(double x)
{
	int sgn;
	double xabs = fabs(x);

	sgn = (x < 0.0) ? -1 : 1;
	if ((xabs - floor(xabs)) >= 0.5)
		return (sgn * (int) ceil(xabs));
	else
		return (sgn * (int) floor(xabs));

}

/**
 * \brief		transforme un Float SUN (ancienne version!!!) en 
 * 			Float de la machine ou on est.
 * @param origine	pointeur sur la valeur a convertir.
 * @return		arrondi
 */
float IdCalcFloatFromSun(float *origine)
{
	typedef struct {
		unsigned signe:1;
		unsigned caracteristique:7;
		unsigned mantisse:24;
	} FORME_SUN;

	FORME_SUN number;
	float result;
	int i;

	/*
	 * Pour du code propre il faudrait invoquer memcpy...
	 */
	for (i = 0; i < 4; i++)
		*((char *) (&number) + i) = *((char *) origine + i);

	if (number.signe == 0 && number.caracteristique == 0
	    && number.mantisse == 0)
		return (0.0);
	result = (float) number.mantisse / (1 << 24) *
	    pow(16.0, number.caracteristique - 64.0);
	return ((number.signe == 0) ? result : -result);
}

/**
 * \brief		transforme un Float IEEE 745-1985 en Float de la 
 * 			machine ou on est.
 * @param origine	pointeur sur la valeur a convertir
 * @return		valeur codee en float natif
 */
float 
IdCalcFloatFromIEEE754(float *origine)
{
	float result;
	unsigned int mantisse, signe, caracteristique;

	signe = (((char *) origine)[0] & 0x80) >> 7;

	mantisse = ((char *) origine)[0] & 0x7f;
	mantisse = mantisse << 1;

	caracteristique = ((char *) origine)[1] & 0x80;
	if (caracteristique != 0)
		mantisse++;
	caracteristique = (((char *) origine)[1] & 0x7f) << 17;
	caracteristique +=
	    (((char *) origine)[2] << 9) + ((char *) origine)[3];

	if (signe == 0 && caracteristique == 0 && mantisse == 0)
		return (0.0);

	result = ((float) caracteristique / (1 << 24) + 1.)
	    * pow(2.0, mantisse - 127.0);

	return ((signe == 0) ? result : -result);
}



#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

/**
 * \brief	Calcul Nombre Aleatoire
 * 		(C) Copr. 1986-92 Numerical Recipes Software ]2#. 
 * @param idum	FIXME : undocumented
 * @return	FIXME : undocumented
 */
float 
IdCalcRand1(long *idum)
{
	int j;
	long k;
	static long iy = 0;
	static long iv[NTAB];
	float temp;

	if (*idum <= 0 || !iy) {
		if (-(*idum) < 1)
			*idum = 1;
		else
			*idum = -(*idum);
		for (j = NTAB + 7; j >= 0; j--) {
			k = (*idum) / IQ;
			*idum = IA * (*idum - k * IQ) - IR * k;
			if (*idum < 0)
				*idum += IM;
			if (j < NTAB)
				iv[j] = *idum;
		}
		iy = iv[0];
	}
	k = (*idum) / IQ;
	*idum = IA * (*idum - k * IQ) - IR * k;
	if (*idum < 0)
		*idum += IM;
	j = iy / NDIV;
	iy = iv[j];
	iv[j] = *idum;
	if ((temp = AM * iy) > RNMX)
		return RNMX;
	else
		return temp;
}

#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX


/**
 * \brief	swappe les octets d'un short int.
 * @param a	pointeur sur la valeur a convertir
 * @param sw	code de swap  :
 * 		1234 : pas de swap (les octets sont deja ds l'ordre) 
 *		3412, 4321, 2143 : on les remet ds l'ordre
 * @return 	valeur codee en short int
 */
short int 
IdCalcSwapInt16(short int *a, int sw)
{
	short int b=0;

	if (!sw)
		return (*a);

	if (sw == 4321)
		b = (((*a << 8) & 0x0ff00) | ((*a >> 8) & 0x00ff));
	return (b);
}

/**
 * \brief	swappe les octets d'un int32.
 * @return	valeur codee en short int
 * @param a	pointeur sur la valeur a convertir
 * @param sw	code de swap :
 * 		1234 : pas de swap (les octets sont deja ds l'ordre) 
 *		3412, 4321, 2143 : on les remet ds l'ordre
 */
int 
IdCalcSwapInt32(int * a, int sw)
{
	int b = 0;
	/*
	 * attention: il aura surement un pb pour les entiers negatifs ...
	 */
	if (!sw)
		return (*a);

	if (sw == 4321)
		b = (((*a << 24) & 0xff000000) | ((*a << 8) & 0x00ff0000) |
		     ((*a >> 8) & 0x0000ff00) | ((*a >> 24) & 0x000000ff));
	if (sw == 3412)
		b =
		    (((*a << 16) & 0xffff0000) |
		     ((*a >> 16) & 0x0000ffff));

	if (sw == 2143)
		b = (((*a << 8) & 0xff00ff00) | ((*a >> 8) & 0x00ff00ff));

	return (b);
}






/*
 * Fonction de calcul de la distance d'un point (x, y, z) au plan 
 * centre de coefficients directeurs px, py, pz 
 */


/**
 * \brief	Fonction de calcul de la distance d'un point (x, y, z) 
 *              au plan centre de coefficients directeurs px, py, pz
 *       
 * @param x	coord x du point
 * @param y	coord y du point
 * @param z	coord z du point
 * @param px	coefficient directeur x du plan
 * @param py	coefficient directeur y du plan
 * @param pz	coefficient directeur z du plan
 * @return 	distance
 */


double
IdGetDistanceFromPlane(int x, int y, int z, int px, int py, int pz)
{
	double rval;

	rval =
	    fabs(((double) x * px) + ((double) y * py) +
		 ((double) z * pz));
	return rval / sqrt(sqr((double) px) + sqr((double) py) +
			   sqr((double) pz));
}
