/*************************************************************************
* $Id: object.c,v 1.12 2006/12/19 15:09:53 yougz Exp $
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
#include "object.h"
#include "object-simple.h"
#include "object-ki.h"
#include "object-compound.h"
#include "object-brain.h"

#include <idseq.h>
#include <idacr.h>
#include <string.h>
#include <idvol.h>
#include "display.h"

/*!
**********************************************************
* \ingroup     Object
* \brief       Create a 2D object for testing a sequence
* \warning
* \param obj   Object choice given by an integer
* \param x  X object size
* \param y  Y object size
* \param db0 Intra voxel field default in T (T2* effect)
* \param b0def Maximum value of the main satic field default (Parabolic form)
* \param flagres On/Off resonance flag
* \param filter =1 to activate Hamming filtering of the k space
* \param fileout Reconstructed image name that will be saved
* \return a pointer on the correspoding object
*****************************************************************/
OBJECT3D* GetTestObject2D(int obj, int x, int y, double b0, double db0, char * objname)
{
	OBJECT3D  *object;
	PPPVOLUME_UCHAR vol, label;
	PPIMAGE_FLOAT value;
	int z;

	z = 1;
	switch (obj)
	{
	case 1:
	{
		object = Create2DTestObjectCircleEllipseSquare(x, y);
		break;
	}
	case 2:
	{
		/* tacq=8, TE=20 TR=2500 gives a nice contrast with this object*/
		object = CreateObjectBiComponent2DCircle(x, y);
		break;
	}
	case 3:
	{
		object = CreateObject2DCircleKi(x, y);
		break;
	}
	case 5:
	{
		object = CreateSphericalObject((int)(x / 2.5), x, y, 1, (int)x / 2, (int)y / 2, 1, 500, 70, 77, 0, 0, 0, 1);
		break;
	}
	/* case 4: /* Brain */
	case 4: /* Brain */
	{
		// tacq=10.0; TE=25.0; TR=500.0;   /* T1 contrast */
		// tacq=10.0; TE=100.0; TR=2000.0; /* T2 contrast */
		// tacq=10.0; TE=22.0; TR=2000.0;  /* ro contrast */
		//vol = ReadLabelVolume("../data/brainbic/brainlabelz126.raw",256,256,1);

		//char src[50];
		//strcpy(src,  "../data/input/");
		// printf("directory = %s \n",src);
		// printf("Object name = %s \n",objname);
		//strcat(src, objname);
		printf("Get 2D object from directory: %s \n", objname);
		vol = ReadLabelVolume(objname, 256, 256, 1);

		switch (x)
		{
		case 256:
		{
			label = GetVolumeVOI(vol, 0, 0, 0, x, y, z);
		}
		break;
		case 128:
		{
			label = GetVolumeVOI(vol, 38, 24, 0, x, y, z);
		}
		break;
		case 64:
		{
			label = GetVolumeVOI(vol, 38, 24, 0, x, y, z);
		}
		break;
		default:
		{
			x = 32; y = 32; z = 1;
			label = GetVolumeVOI(vol, 60, 60, 0, x, y, z);
		}
		break;
		}
		IdVolFree(vol);
		value = GetBrainPhysicalValues(b0);
		object = LabelToObject(label, value);
		IdImaFree(value);
		break;
	}
	default:
	{
		printf("Unknown 2D object number !!\n");
		exit(0);
		break;
	}
	}
	SetObjectSize(object, (float)0.2, (float)0.2, (float)0.002);
	SetObjectDeltaB0(object, db0);

	return(object);
}

/*!
**********************************************************
* \ingroup     Object
* \brief       Create a 3D object for testing a sequence
* \warning
* \param obj   Object choice given by an integer
* \param x  X object size
* \param y  Y object size
* \param z  Z object size
* \param db0 Intra voxel field default in T (T2* effect)
* \param b0def Maximum value of the main satic field default (Parabolic form)
* \param flagres On/Off resonance flag
* \param filter =1 to activate Hamming filtering of the k space
* \param fileout Reconstructed image name that will be saved
* \return a pointer on the correspoding object
*****************************************************************/
OBJECT3D* GetTestObject3D(int obj, int x, int y, int z, double b0, double db0, char * objname)
{
	OBJECT3D  *object;
	PPPVOLUME_UCHAR vol, label;
	PPIMAGE_FLOAT value;

	switch (obj)
	{
	case 1:
	{
		object = CreateObject3DSphereEllipse(x, y, z);
		break;
	}
	case 2:
	{
		object = CreateObjectBiComponent3DSphere(x, y, z);
		break;
	}
	/* case 4: /* Brain */
	case 4: /* Brain */
	{
		// tacq=10.0; TE=25.0; TR=500.0;   /* T1 contrast */
		// tacq=10.0; TE=100.0; TR=2000.0; /* T2 contrast */
		// tacq=10.0; TE=22.0; TR=2000.0;  /* ro contrast */
		//vol = ReadLabelVolume("../data/brainbic/brainlabelz126.raw",256,256,1);

		//char src[50];
		//strcpy(src,  "../data/input/");
		// printf("directory = %s \n",src);
		// printf("Object name = %s \n",objname);
		//strcat(src, objname);
		printf("Get 3D object from directory: %s \n", objname);
		vol = ReadLabelVolume(objname, 256, 256, 256);

		switch (x)
		{
		case 256:
		{
			label = GetVolumeVOI(vol, 0, 0, 0, x, y, z);
		}
		break;
		case 128:
		{
			label = GetVolumeVOI(vol, 60, 60, 110, x, y, z);
		}
		break;
		case 64:
		{
			label = GetVolumeVOI(vol, 60, 60, 110, x, y, z);
		}
		break;
		case 16:
		{
			label = GetVolumeVOI(vol, 60, 60, 110, x, y, z);
		}
		break;
		default:
		{
			x = 32; y = 32; z = 32;
			label = GetVolumeVOI(vol, 60, 60, 110, x, y, z);
		}
		break;
		}
		IdVolFree(vol);
		value = GetBrainPhysicalValues(b0);
		object = LabelToObject(label, value);
		IdImaFree(value);
		break;
	}

	default:
	{
		printf("Unknown 3D object number !!\n");
		exit(0);
		break;
	}
	}
	SetObjectSize(object, (float)0.2, (float)0.2, (float)0.2);
	SetObjectDeltaB0(object, db0);

	return(object);
}


/*!
**************************************************
* \ingroup Object
* \brief   Save a virtual object within a set of float volumes
* \warning A maximum of two components (water and fat) is considered for now
* \param   object Pointer on the virtual object to save
**************************************************/
void SaveObjectAsVolFloat(OBJECT3D *object)
{
	PPPVOLUME_FLOAT volfloat;
	int x, y, z;
	int i, j, k;
	char nom[80];
	nom[0] = '\0';

	x = object->x;
	y = object->y;
	z = object->z;

	volfloat = (PPPVOLUME_FLOAT)IdVolAlloc(x, y, z, VOL_FLOAT);
	if (!volfloat)
	{
		printf("Impossible d'allouer volfloat ! \n");
		exit(0);
	}

	for (k = 0; k < z; k++) for (j = 0; j < y; j++) for (i = 0; i < x; i++)
		volfloat[k][j][i] = (float)object->values[0][k][j][i];
	strcpy(nom, "volWATERro.vol");
	IdAcrWriteFile(nom, volfloat);

	for (k = 0; k < z; k++) for (j = 0; j < y; j++) for (i = 0; i < x; i++)
		volfloat[k][j][i] = (float)object->values[1][k][j][i];
	strcpy(nom, "volWATERt1.vol");
	IdAcrWriteFile(nom, volfloat);

	for (k = 0; k < z; k++) for (j = 0; j < y; j++) for (i = 0; i < x; i++)
		volfloat[k][j][i] = (float)object->values[2][k][j][i];
	strcpy(nom, "volWATERt2.vol");
	IdAcrWriteFile(nom, volfloat);

	if (object->component == 2)
	{
		for (k = 0; k < z; k++) for (j = 0; j < y; j++) for (i = 0; i < x; i++)
			volfloat[k][j][i] = (float)object->values[3][k][j][i];
		strcpy(nom, "volFATro.vol");
		IdAcrWriteFile(nom, volfloat);


		for (k = 0; k < z; k++) for (j = 0; j < y; j++) for (i = 0; i < x; i++)
			volfloat[k][j][i] = (float)object->values[4][k][j][i];
		strcpy(nom, "volFATt1.vol");
		IdAcrWriteFile(nom, volfloat);

		for (k = 0; k < z; k++) for (j = 0; j < y; j++) for (i = 0; i < x; i++)
			volfloat[k][j][i] = (float)object->values[5][k][j][i];
		strcpy(nom, "volFATt2.vol");
		IdAcrWriteFile(nom, volfloat);
		IdVolFree(volfloat);
	}
}

/*!
**************************************************
* \ingroup Object
* \brief   Virtual object allocation
* \warning A default size of 1mx1mx1m is given
* \param x Number of voxels on the x dimension
* \param y Number of voxels on the y dimension
* \param z Number of voxels on the z dimension
* \param ncomp Number of components
* \param nbparam Number of physical values by component
* \return Pointer on the alocated object
**************************************************/
OBJECT3D* AllocObject(int x, int y, int z, int nbcomp, int nbparam)
{
	OBJECT3D *object;
	int i, j, k, t;
	int nbvol;

	nbvol = nbcomp*nbparam;

	object = (OBJECT3D *)malloc(sizeof(OBJECT3D));
	if (!object)
	{
		printf("Unable to allocate an OBJECT3D ! \n");
		exit(0);
	}

	object->values = (PPPPSEQUENCE_USHORT)IdSeqAlloc(x, y, z, nbvol, SEQ_USHORT);
	if (!object->values)
	{
		printf("Unable to allocate a SEQUENCE ! \n");
		exit(0);
	}

	for (t = 0; t < nbvol; t++)
		for (i = 0; i < x; i++) for (j = 0; j < y; j++) for (k = 0; k < z; k++)
		{
			object->values[t][k][j][i] = 0;
		}

	object->chemshift = (PSIGNAL_FLOAT)IdSigAlloc(nbcomp, SIG_FLOAT);
	if (!object->chemshift)
	{
		printf("Unable to allocate SIGNAL! \n");
		exit(0);
	}
	for (t = 0; t < nbcomp; t++)
	{
		object->chemshift[t] = 0.0;
	}

	object->deltaB0 = (PPPVOLUME_FLOAT)IdVolAlloc(x, y, z, VOL_FLOAT);
	if (!object->deltaB0)
	{
		printf("Unable to allocate VOLUME! \n");
		exit(0);
	}

	for (i = 0; i < x; i++) for (j = 0; j < y; j++) for (k = 0; k < z; k++)
	{
		object->deltaB0[k][j][i] = 0.0;
	}

	object->fielddefault = (PPPVOLUME_FLOAT)IdVolAlloc(x, y, z, VOL_FLOAT);
	if (!object->fielddefault)
	{
		printf("Unable to allocate VOLUME! \n");
		exit(0);
	}

	for (i = 0; i < x; i++) for (j = 0; j < y; j++) for (k = 0; k < z; k++)
	{
		object->fielddefault[k][j][i] = 0.0;
	}

	object->x = x; object->y = y; object->z = z;
	object->nbcomponent = nbcomp;
	object->nbparam = nbparam;
	object->component = 0; /* Default active component = 0 */

	/* We give to the object a size of 1x1x1 meter */
	object->lx = 1.0; object->ly = 1.0; object->lz = 1.0;

	return(object);
}

/*!
**************************************************
* \ingroup Object
* \brief   Object free
* \param object Pointer on the object to free
**************************************************/
void FreeObject(OBJECT3D * object)
{
	IdSeqFree(object->values);
	IdVolFree(object->fielddefault);
	IdVolFree(object->deltaB0);
	if (object->chemshift != NULL) IdSigFree(object->chemshift);
	free(object);
}

/*!
*************************************************
*  \ingroup Object
* \brief Define the size in meter of a virtual object
* \param object Pointer on the object
* \param lx x size in meter
* \param ly y size in meter
* \param lz z size in meter
* Note that the same object could be considered at different sizes
**************************************************/
void SetObjectSize(OBJECT3D *object, float lx, float ly, float lz)
{
	object->lx = lx;
	object->ly = ly;
	object->lz = lz;
}

/*!
*************************************************
* \ingroup Object
* \brief Define the component that will be considered for acquisition
* \param object Pointer on thevirtual object
* \param component Component number
**************************************************/
void SetObjectActiveComponent(OBJECT3D *object, int component)
{
	object->component = component;
}

/*!
*************************************************
* \ingroup Object
* \brief Set the T2* weight by definig an intra spin deltab0 value
* For now, this value is constant all over the volume
* \param object Pointer on the object
* \param deltab0 Weight value in Tesla
**************************************************/
void SetObjectDeltaB0(OBJECT3D *object, double deltab0)
{
	int i, j, k;
	for (i = 0; i < object->x; i++) for (j = 0; j < object->y; j++) for (k = 0; k < object->z; k++)
	{
		object->deltaB0[k][j][i] = (float)deltab0;
	}
}

/*!
*************************************************
*  \ingroup Object
* \brief Fix the chemical shift value of each component
* \param object Pointer on the object
* \param edw Value (in Hz) of the chemical shift
* \param component Numero de la molecule
***************************************************/
void SetObjectChemicalShift(OBJECT3D * object, float edw, int component)
{
	object->chemshift[component] = edw;
}


/*!
*************************************************
* \ingroup Object
* \brief Set the ro t1 t2 values of one point (voxel) of an object component
* \param object Pointer on the object
* \param x x coordinate
* \param y y coordinate
* \param z z coordinate
* \param ro ro vlue
* \param t1 t1 value
* \param t2 t2 value
* \param component Component number
**************************************************/
void SetObjectPoint(OBJECT3D *object, int x, int y, int z,
	int ro, int t1, int t2, int component
	)
{
	int i;
	i = component*object->nbparam;
	object->values[0 + i][z][y][x] = ro;
	object->values[1 + i][z][y][x] = t1;
	object->values[2 + i][z][y][x] = t2;
}

/*!
*************************************************
* \ingroup Object
* \brief Save an object within a file
* \warning Only the param values are saved, not the size; the deltaB0 ...
* \param object Pointer on the virtual object
* \param name file name
**************************************************/
void WriteObject(OBJECT3D * object, char * name)
{
	IdAcrWriteFile(name, (PPPPSEQUENCE)object->values);
}

/*!
*************************************************
* \ingroup Object
* \brief Allocate and read a virtual object from file.
* \warning Some object parameters are not saved in the file, they take their default values
* \param name File name containing the object values
* \return Pointer on the object
**************************************************/
OBJECT3D* ReadObject(char * name)
{
	PPPPSEQUENCE_USHORT seq;
	OBJECT3D * object;
	int x, y, z, nbparam, nbcomp, nbvol;
	int t, i, j, k;
	float lx, ly, lz;

	seq = (PPPPSEQUENCE_USHORT)IdAcrReadFile(name, SEQ_USHORT);
	x = IdSeqDimX(seq);
	y = IdSeqDimY(seq);
	z = IdSeqDimZ(seq);
	nbvol = IdSeqDimT(seq);

	if (nbvol > 3)
	{
		nbparam = 3; /* Default value : ro,t1,t2 */
		nbcomp = 2; /* Default value : 2, WATER and FAT*/
	}
	else
	{
		nbparam = 3; /* Default value : ro,t1,t2 */
		nbcomp = 1; /* Default value : 1, WATER */
	}
	lx = 1.0; ly = 1.0; lz = 1.0;

	object = (OBJECT3D *)AllocObject(x, y, z, nbcomp, nbparam);
	for (t = 0; t < nbvol; t++)
		for (i = 0; i < x; i++) for (j = 0; j < y; j++) for (k = 0; k < z; k++)
		{
			/* Not very efficient, better possible using memcpy ! */
			object->values[t][k][j][i] = seq[t][k][j][i];
		}
	IdSeqFree(seq);

	object->nbcomponent = nbcomp;
	object->nbparam = nbparam;
	object->lx = lx; object->ly = ly; object->lz = lz;
	object->x = x;   object->y = y;   object->z = z;

	return(object);
}
