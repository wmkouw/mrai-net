/*************************************************************************
* $Id: object-brain.c,v 1.8 2006/12/18 15:10:37 yougz Exp $
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

Version 2.0  20/12/2006
*************************************************************************/
#include "object-brain.h"
#include <idseq.h>
#include <idacr.h>
#include <string.h>
#include <idvol.h>
#include <idima.h>
#include "display.h"
#include "noise.h"

/*!
**************************************************
* \ingroup     Object
* \brief       Read a uchar volume asscoiated to ki label
* \param file  File name
* \param x     x dimension of the data volume
* \param y     y dimension of the data volume
* \param z     z dimension of the data volume
* \return      Pointer on the label volume
**************************************************/
PPPVOLUME_UCHAR ReadLabelVolume(char * filein, int x, int y, int z)
{
	PPPVOLUME_UCHAR label;

	label = (PPPVOLUME_UCHAR)IdVolReadRawFile(filein, x, y, z, VOL_UCHAR);
	if (!label)
	{
		printf("Unable to read %s \n", filein);
		exit(0);
	}

	return label;
}

/*!
**************************************************
* \ingroup     Object
* \brief       Allocate and get a volume of interest (VOI) within a given volume of ki labels
* \param       px Starting point on x dimension
* \param       py Starting point on y dimension
* \param       pz Starting point on z dimension
* \param	   dimx Number of plane to get along the x dimension
* \param	   dimy Number of plane to get along the y dimension
* \param	   dimz Number of plane to get along the z dimension
* \return      Pointer on the VOI
**************************************************/
PPPVOLUME_UCHAR GetVolumeVOI(PPPVOLUME_UCHAR label, int px, int py, int pz, int dimx, int dimy, int dimz)
{
	PPPVOLUME_UCHAR vol;
	int i, j, k;

	vol = (PPPVOLUME_UCHAR)IdVolAlloc(dimx, dimy, dimz, VOL_UCHAR);
	for (k = 0; k < dimz; k++) for (j = 0; j < dimy; j++) for (i = 0; i < dimx; i++)
		vol[k][j][i] = label[k + pz][j + py][i + px];

	return vol;
}

/*!
**************************************************************************
* \ingroup     Object
* \brief       Create un object from a volume of label and physical values
* \param       label Volume of label
* \param       value Set of phyical values including their variance
* \warning
* \return      Pointer on the virtual object
**************************************************************************/
OBJECT3D* LabelToObject(PPPVOLUME_UCHAR label, PPIMAGE_FLOAT value)
{
	int x, y, z;
	int ro, t1, t2, lab;
	OBJECT3D *object;
	int i, j, k;

	x = IdVolDimX(label);
	y = IdVolDimY(label);
	z = IdVolDimZ(label);

	object = AllocObject(x, y, z, 1, 3);

	for (k = 0; k < z; k++) for (j = 0; j < y; j++) for (i = 0; i < x; i++)
	{
		lab = label[k][j][i];
		ro = (int)(value[lab][0] + RandGaussian(value[lab][1], 0.0, 0));
		t1 = (int)(value[lab][2] + RandGaussian(value[lab][3], 0.0, 0));
		t2 = (int)(value[lab][4] + RandGaussian(value[lab][5], 0.0, 0));
		SetObjectPoint(object, i, j, k, ro, t1, t2, WATER);
	}
	return object;
}

/*!
**************************************************************************
* \ingroup     Object
* \brief       Define physical values of brain tissue types
*  Ref : Yoder et al. SPIE MEdical Imaging 2002
* 10 tissues are considered with the following description
* label / name / Ro / Ro standard deviation / T1 / T1sd / T2 / T2sd / Ki / Kisd
* 0 /Air/0/0 /0/0 /0/0 /0/0
* 1 /CSF/2569/ /329/ /1/ /-9.05/ /
* 2 /Grey Matter/833/ /83/ /0.86/ /-9.05/ /
* 3 /White Matter/500/ /70/ /0.77/ /-9.05/ /
* 4 /Fat/350/ /70/ /1/ /-7.5/ /
* 5 /Muscle/900/ /47/ /1/ /-9.5/ /
* 6 /Skin/2569/ /329/ /1/ /-9.05/ /
* 7 /Skull/0/0/0/0/0/0/-8.86/ /
* 8 /Glial Matter/833/ /83/ /0.86/ /-9.05/ /
* 9 /Connective/500/ /70/ /0.77/ /-9.05/ /
* \param B0 Static field value (unused at the moment, assumed to be 1T)
* \return Image containing the physical set of values
**************************************************************************/
PPIMAGE_FLOAT GetBrainPhysicalValues(double b0)
{
	int i, j;
	PPIMAGE_FLOAT value;
	switch ((int)b0) {
	case 2:
		value = (PPIMAGE_FLOAT)IdImaAlloc(8, 10, IMA_FLOAT);
		for (j = 0; j < 10; j++) {
			for (i = 0; i < 8; i++) {

				value[j][i] = 0.0;

				/* Air */
				value[0][0] = 0.0; value[0][1] = 0.0; /* Ro , Ro sd */
				value[0][2] = 0.0; value[0][3] = 0.0; /* T1 , T1 sd */
				value[0][4] = 0.0; value[0][5] = 0.0; /* T2 , T2 sd */
				value[0][6] = 0.0; value[0][7] = 0.0; /* Ki , Ki sd */
				/* CSF */
				value[1][0] = 100.0;    value[1][1] = 0.0; /* Ro , Ro sd */
				value[1][2] = 4326.0;   value[1][3] = 0.0; /* T1 , T1 sd */
				value[1][4] = 503.0;    value[1][5] = 64.0; /* T2 , T2 sd */
				value[1][6] = -9.05e-6; value[1][7] = 0.0; /* Ki , Ki sd */
				/* Grey Matter */
				value[2][0] = 86.0;     value[2][1] = 0.4; /* Ro , Ro sd */
				value[2][2] = 1124.0;   value[2][3] = 24.0; /* T1 , T1 sd */
				value[2][4] = 95.0;     value[2][5] = 8.0; /* T2 , T2 sd */
				value[2][6] = -9.05e-6; value[2][7] = 0.0; /* Ki , Ki sd */
				/* White Matter */
				value[3][0] = 77.0;    value[3][1] = 3.00; /* Ro , Ro sd */
				value[3][2] = 884.0;   value[3][3] = 50.0; /* T1 , T1 sd */
				value[3][4] = 72.0;    value[3][5] = 4.0; /* T2 , T2 sd */
				value[3][6] = -9.5e-6; value[3][7] = 0.0; /* Ki , Ki sd */
				/* Fat */
				value[4][0] = 100.0;    value[4][1] = 0.0; /* Ro , Ro sd */
				value[4][2] = 343.0;    value[4][3] = 37.0; /* T1 , T1 sd */
				value[4][4] = 58.0;     value[4][5] = 4.0; /* T2 , T2 sd */
				value[4][6] = -7.5e-6;  value[4][7] = 0.0; /* Ki , Ki sd */
				/* Muscle */
				value[5][0] = 100.0;   value[5][1] = 0.0; /* Ro , Ro sd */
				value[5][2] = 629.0;   value[5][3] = 50.0; /* T1 , T1 sd */
				value[5][4] = 44.0;    value[5][5] = 6.0; /* T2 , T2 sd */
				value[5][6] = -9.5e-6; value[5][7] = 0.0; /* Ki , Ki sd */
				/* Skin */
				value[6][0] = 100.0;    value[6][1] = 0.0; /* Ro , Ro sd */
				value[6][2] = 230.0;    value[6][3] = 8.0; /* T1 , T1 sd */
				value[6][4] = 35.0;     value[6][5] = 4.0; /* T2 , T2 sd */
				value[6][6] = -9.05e-6; value[6][7] = 0.0; /* Ki , Ki sd */
				/* Skull */
				value[7][0] = 0.0;      value[7][1] = 0.0; /* Ro , Ro sd */
				value[7][2] = 200.0;    value[7][3] = 0.0; /* T1 , T1 sd */
				value[7][4] = 0.0;      value[7][5] = 0.0; /* T2 , T2 sd */
				value[7][6] = -8.86e-6; value[7][7] = 0.0; /* Ki , Ki sd */
				/* Glial Matter */
				value[8][0] = 86.0;     value[8][1] = 0.0; /* Ro , Ro sd */
				value[8][2] = 1124.0;   value[8][3] = 24.0; /* T1 , T1 sd */
				value[8][4] = 95.0;     value[8][5] = 8.0; /* T2 , T2 sd */
				value[8][6] = -9.05e-6; value[8][7] = 0.0; /* Ki , Ki sd */
				/* Connective */
				value[9][0] = 77.0;     value[9][1] = 0.0; /* Ro , Ro sd */
				value[9][2] = 1124.0;   value[9][3] = 24.0; /* T1 , T1 sd */
				value[9][4] = 95.0;     value[9][5] = 8.0; /* T2 , T2 sd */
				value[9][6] = -9.05e-6; value[9][7] = 0.0; /* Ki , Ki sd */
			}
		}
		return (value);
	case 3:
		value = (PPIMAGE_FLOAT)IdImaAlloc(8, 10, IMA_FLOAT);
		for (j = 0; j < 10; j++) {
			for (i = 0; i < 8; i++) {

				value[j][i] = 0.0;

				/* Air */
				value[0][0] = 0.0; value[0][1] = 0.0; /* Ro , Ro sd */
				value[0][2] = 0.0; value[0][3] = 0.0; /* T1 , T1 sd */
				value[0][4] = 0.0; value[0][5] = 0.0; /* T2 , T2 sd */
				value[0][6] = 0.0; value[0][7] = 0.0; /* Ki , Ki sd */
				/* CSF */
				value[1][0] = 100.0;    value[1][1] = 0.0; /* Ro , Ro sd */
				value[1][2] = 4313.0;   value[1][3] = 0.0; /* T1 , T1 sd */
				value[1][4] = 503.0;    value[1][5] = 64.0; /* T2 , T2 sd */
				value[1][6] = -9.05e-6; value[1][7] = 0.0; /* Ki , Ki sd */
				/* Grey Matter */
				value[2][0] = 86.0;     value[2][1] = 0.4; /* Ro , Ro sd */
				value[2][2] = 1820.0;   value[2][3] = 114.0; /* T1 , T1 sd */
				value[2][4] = 99.0;     value[2][5] = 7.0; /* T2 , T2 sd */
				value[2][6] = -9.05e-6; value[2][7] = 0.0; /* Ki , Ki sd */
				/* White Matter */
				value[3][0] = 77.0;    value[3][1] = 3.00; /* Ro , Ro sd */
				value[3][2] = 1084.0;  value[3][3] = 45.0; /* T1 , T1 sd */
				value[3][4] = 69.0;    value[3][5] = 3.0; /* T2 , T2 sd */
				value[3][6] = -9.5e-6; value[3][7] = 0.0; /* Ki , Ki sd */
				/* Fat */
				value[4][0] = 100.0;    value[4][1] = 0.0; /* Ro , Ro sd */
				value[4][2] = 382.0;    value[4][3] = 13.0; /* T1 , T1 sd */
				value[4][4] = 68.0;     value[4][5] = 4.0; /* T2 , T2 sd */
				value[4][6] = -7.5e-6;  value[4][7] = 0.0; /* Ki , Ki sd */
				/* Muscle */
				value[5][0] = 100.0;   value[5][1] = 0.0; /* Ro , Ro sd */
				value[5][2] = 832.0;   value[5][3] = 62.0; /* T1 , T1 sd */
				value[5][4] = 50.0;    value[5][5] = 4.0; /* T2 , T2 sd */
				value[5][6] = -9.5e-6; value[5][7] = 0.0; /* Ki , Ki sd */
				/* Skin */
				value[6][0] = 100.0;    value[6][1] = 0.0; /* Ro , Ro sd */
				value[6][2] = 306.0;    value[6][3] = 18.0; /* T1 , T1 sd */
				value[6][4] = 22.0;     value[6][5] = 0.0; /* T2 , T2 sd */
				value[6][6] = -9.05e-6; value[6][7] = 0.0; /* Ki , Ki sd */
				/* Skull */
				value[7][0] = 0.0;      value[7][1] = 0.0; /* Ro , Ro sd */
				value[7][2] = 223.0;    value[7][3] = 11.0; /* T1 , T1 sd */
				value[7][4] = 0.0;      value[7][5] = 0.0; /* T2 , T2 sd */
				value[7][6] = -8.86e-6; value[7][7] = 0.0; /* Ki , Ki sd */
				/* Glial Matter */
				value[8][0] = 86.0;     value[8][1] = 0.0; /* Ro , Ro sd */
				value[8][2] = 1820.0;   value[8][3] = 114.0; /* T1 , T1 sd */
				value[8][4] = 99.0;     value[8][5] = 7.0; /* T2 , T2 sd */
				value[8][6] = -9.05e-6; value[8][7] = 0.0; /* Ki , Ki sd */
				/* Connective */
				value[9][0] = 77.0;     value[9][1] = 0.0; /* Ro , Ro sd */
				value[9][2] = 1820.0;   value[9][3] = 114.0; /* T1 , T1 sd */
				value[9][4] = 99.0;     value[9][5] = 7.0; /* T2 , T2 sd */
				value[9][6] = -9.05e-6; value[9][7] = 0.0; /* Ki , Ki sd */
			}
		}
		return (value);
	default:
		value = (PPIMAGE_FLOAT)IdImaAlloc(8, 10, IMA_FLOAT);
		for (j = 0; j < 10; j++) {
			for (i = 0; i < 8; i++) {

				value[j][i] = 0.0;

				/* Air */
				value[0][0] = 0.0; value[0][1] = 0.0; /* Ro , Ro sd */
				value[0][2] = 0.0; value[0][3] = 0.0; /* T1 , T1 sd */
				value[0][4] = 0.0; value[0][5] = 0.0; /* T2 , T2 sd */
				value[0][6] = 0.0; value[0][7] = 0.0; /* Ki , Ki sd */
				/* CSF */
				value[1][0] = 100.0;    value[1][1] = 0.0; /* Ro , Ro sd */
				value[1][2] = 2569.0;   value[1][3] = 170.0; /* T1 , T1 sd */
				value[1][4] = 329.0;    value[1][5] = 13.0; /* T2 , T2 sd */
				value[1][6] = -9.05e-6; value[1][7] = 0.0; /* Ki , Ki sd */
				/* Grey Matter */
				value[2][0] = 86.0;     value[2][1] = 0.4; /* Ro , Ro sd */
				value[2][2] = 833.0;    value[2][3] = 40.0; /* T1 , T1 sd */
				value[2][4] = 83.0;     value[2][5] = 8.0; /* T2 , T2 sd */
				value[2][6] = -9.05e-6; value[2][7] = 0.0; /* Ki , Ki sd */
				/* White Matter */
				value[3][0] = 77.0;    value[3][1] = 3.00; /* Ro , Ro sd */
				value[3][2] = 500.0;   value[3][3] = 23.0; /* T1 , T1 sd */
				value[3][4] = 70.0;    value[3][5] = 3.0; /* T2 , T2 sd */
				value[3][6] = -9.5e-6; value[3][7] = 0.0; /* Ki , Ki sd */
				/* Fat */
				value[4][0] = 100.0;    value[4][1] = 0.0; /* Ro , Ro sd */
				value[4][2] = 350.0;    value[4][3] = 0.0; /* T1 , T1 sd */
				value[4][4] = 70.0;     value[4][5] = 0.0; /* T2 , T2 sd */
				value[4][6] = -7.5e-6;  value[4][7] = 0.0; /* Ki , Ki sd */
				/* Muscle */
				value[5][0] = 100.0;   value[5][1] = 0.0; /* Ro , Ro sd */
				value[5][2] = 900.0;   value[5][3] = 0.0; /* T1 , T1 sd */
				value[5][4] = 47.0;    value[5][5] = 0.0; /* T2 , T2 sd */
				value[5][6] = -9.5e-6; value[5][7] = 0.0; /* Ki , Ki sd */
				/* Skin */
				value[6][0] = 100.0;    value[6][1] = 0.0; /* Ro , Ro sd */
				value[6][2] = 2569.0;   value[6][3] = 0.0; /* T1 , T1 sd */
				value[6][4] = 329.0;    value[6][5] = 0.0; /* T2 , T2 sd */
				value[6][6] = -9.05e-6; value[6][7] = 0.0; /* Ki , Ki sd */
				/* Skull */
				value[7][0] = 0.0;      value[7][1] = 0.0; /* Ro , Ro sd */
				value[7][2] = 0.0;      value[7][3] = 0.0; /* T1 , T1 sd */
				value[7][4] = 0.0;      value[7][5] = 0.0; /* T2 , T2 sd */
				value[7][6] = -8.86e-6; value[7][7] = 0.0; /* Ki , Ki sd */
				/* Glial Matter */
				value[8][0] = 86.0;     value[8][1] = 0.0; /* Ro , Ro sd */
				value[8][2] = 833.0;    value[8][3] = 0.0; /* T1 , T1 sd */
				value[8][4] = 83.0;     value[8][5] = 0.0; /* T2 , T2 sd */
				value[8][6] = -9.05e-6; value[8][7] = 0.0; /* Ki , Ki sd */
				/* Connective */
				value[9][0] = 77.0;     value[9][1] = 0.0; /* Ro , Ro sd */
				value[9][2] = 500.0;    value[9][3] = 0.0; /* T1 , T1 sd */
				value[9][4] = 70.0;     value[9][5] = 0.0; /* T2 , T2 sd */
				value[9][6] = -9.05e-6; value[9][7] = 0.0; /* Ki , Ki sd */
			}
		}
		return (value);
	}
}
