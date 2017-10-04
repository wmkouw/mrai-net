/*************************************************************************
* $Id: test-sequence.c,v 1.32 2006/12/18 15:10:38 yougz Exp $
**************************************************************************
* TestSequence function is an excellent example of how to run a simulation
* with the object definition, the experience definition, the sequence call
* and the reconstruction step. It gives also variation about adding noise
* and filtering at several step
* One should copy such function to make similar tests in 3D
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
#include "test-sequence.h"
#include "idvol.h"
#include "idacr.h"
#include "simri3d.h"
#include "experience.h"
#include "object.h"
#include "object-simple.h"
#include "object-compound.h"
#include "object-brain.h"
#include "object-ki.h"
#include "output.h"
#include "display.h"
#include "seq-se.h"
#include "sequence.h"

#ifdef HAVE_MPI
#include <mpi.h>
#endif

/*!
**********************************************************
* \ingroup     Test
* \brief       Generic 2D test function
* \warning     The object size must be equal or greater than the image size
*  Both sizes should be power of 2 for the FFT reconstruction
* \param obj   Object choice given by an integer
* \param objsize Object size (x=y)
* \param imasize Image size (x=y)
* \param tacq Readout time in ms
* \param TE Echo time in ms
* \param TR Repetition time in ms
* \param teta RF pulse angle (used only for gradient echo sequence)
* \param b0 Main static field value in T
* \param db0 Intra voxel field default in T (T2* effect)
* \param b0def Maximum value of the main satic field default (Parabolic form)
* \param flagres On/Off resonance flag
* \param filter =1 to activate Hamming filtering of the k space
* \param fileout Reconstructed image name that will be saved
* \param seqname Sequence name (Name list given in sequence.c)
* \param display Display flag: 1 > display the reconstructed image
*****************************************************************/
void TestSequence2D(int obj, int objsize, int imasize,
	double tacq, double TE, double TR, double teta,
	double b0, double db0, double b0def, int flagres, int flagdech, int filter,
	char * fileout, char * objname, char * seqname, int display, int inhomogeneousB1)
{
	char fic[80];
	int i, j, k;
	int ntx, nty, ntz;		/* K space size */
	double fovx, fovy, fovz;	/* Fov size in meter */
	OBJECT3D  *object;
	EXPERIENCE3D *expr;
	SEQPARAM seqparam;
	PPPVOLUME_COMPLEX_DOUBLE  volrf;
	PPPVOLUME_FLOAT volrecmod;
#ifdef HAVE_MPI
	int rank;
#endif
	fic[0] = '\0';

	printf("TestSequence2D start\n");
	printf("B0 = %f\n", b0);

	if (objsize < imasize)
	{
		printf("The object has more pixel than the image !! \n");
		exit(0);
	}

	/* Object */
	object = GetTestObject2D(obj, objsize, objsize, b0, db0, objname);
	//	AddNoiseToObject(object,5.0);
	//	ApplyMeanToObject(object,3);

	/* Experience */
	expr = AllocExperience();
	ntx = imasize; nty = ntx; ntz = 1;
	fovx = 0.20; fovy = 0.20; fovz = 0.0020;
	SetFovExperience(expr, fovx, fovy, fovz, 0.0, 0.0, 0.0);
	SetAcqExperience(expr, ntx, nty, ntz, tacq*1e-3);
	SetResonanceExperience(expr, flagres);
	SetFlagdechExperience(expr, flagdech);
	SetB0Experience(expr, b0);
	SetB0DefExperience(expr, b0def);
	expr->inhomogeneousB1 = inhomogeneousB1;

	/* Sequence */
	SetSeqParamTE(&seqparam, TE);
	SetSeqParamTR(&seqparam, TR);
	SetSeqParamTeta(&seqparam, teta); /* pour test GE2D */

	/* Acquisition */
	volrf = RunSequence(seqname, &seqparam, object, expr);
	//	WriteVolRF(volrf);
	FreeObject(object);
	//    AddGaussianNoiseToRFVolume(volrf,0.001);
	if (filter == 1)    VolRFFiltering(volrf, HANNING);

	/* Reconstruction */
	volrecmod = RecVolIFFTModuleFromVol(volrf);
	volrf = NULL;/* Should be an IdVolFree function call, but volrf pointer if already free within IdVolIFFT function ! */

	/* Save & Display */
#ifdef HAVE_MPI
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if (rank==0)  WriteVolRecUchar(volrecmod,fileout);
#else
	if (display == 1) DisplayVolXY((PPPVOLUME)volrecmod, 0, "Recvol2D");
	WriteVolRecUchar(volrecmod, fileout);
#endif
	printf("min=%e max=%e \n", IdVolMinima(volrecmod), IdVolMaxima(volrecmod));
	IdVolFree(volrecmod);

	printf("TestSequence2D End\n");
}

/*!
**********************************************************
* \ingroup     Test
* \brief       Generic 3D test function
* \warning     The object size must be equal or greater than the image size
*  Both sizes should be power of 2 for the FFT reconstruction
* \param obj   Object choice given by an integer
* \param objsize Object size (x=y=z)
* \param imasize Image size (x=y=z)
* \param tacq Readout time in ms
* \param TE Echo time in ms
* \param TR Repetition time in ms
* \param teta RF pulse angle (used only for gradient echo sequence)
* \param b0 Main static field value in T
* \param db0 Intra voxel field default in T (T2* effect)
* \param b0def Maximum value of the main satic field default (Parabolic form)
* \param flagres On/Off resonance flag
* \param filter =1 to activate Hamming filtering of the k space
* \param fileout Reconstructed image name that will be saved
* \param seqname Sequence name (Name list given in sequence.c)
* \param display Display flag: 1 > display the reconstructed image
*****************************************************************/
void TestSequence3D(int obj, int objsize, int imasize, double tacq, double TE, double TR, double teta,
	double b0, double db0, double b0def, int flagres, int flagdech, int filter,
	char * fileout, char * objname, char * seqname, int display, int inhomogeneousB1)
{
	char fic[80];
	int ntx, nty, ntz;		/* Taille du volume RF acquis */
	double fovx, fovy, fovz;	/* Taille du fov en cm */
	OBJECT3D  *object;
	EXPERIENCE3D *expr;
	SEQPARAM seqparam;
	PPPVOLUME_COMPLEX_DOUBLE  volrf;
	PPPVOLUME_FLOAT volrecmod;
#ifdef HAVE_MPI
	int rank;
#endif
	fic[0] = '\0';

	printf("TestSequence3D start\n");

	if (objsize < imasize)
	{
		printf("L'object est plus petit que l'image !! \n");
		exit(0);
	}

	/* Object */
	object = GetTestObject3D(obj, objsize, objsize, objsize, b0, db0, objname);
	printf("Object loaded \n");

	//	AddNoiseToObject(object,5.0);
	//	ApplyMeanToObject(object,3);

	/* Experience */
	expr = AllocExperience();
	ntx = imasize; nty = ntx; ntz = ntx;
	fovx = 0.20; fovy = 0.20; fovz = 0.20;
	SetFovExperience(expr, fovx, fovy, fovz, 0.0, 0.0, 0.0);
	SetAcqExperience(expr, ntx, nty, ntz, tacq*1e-3);
	SetResonanceExperience(expr, flagres);
	SetFlagdechExperience(expr, flagdech);
	SetB0Experience(expr, b0);
	SetB0DefExperience(expr, b0def);
	expr->inhomogeneousB1 = inhomogeneousB1;
	printf("Experiences set \n");

	/* Sequence */
	SetSeqParamTE(&seqparam, TE);
	SetSeqParamTR(&seqparam, TR);
	SetSeqParamTeta(&seqparam, teta);
	printf("Sequences set \n");

	/* Acquisition */
	volrf = RunSequence(seqname, &seqparam, object, expr);
	printf("Sequences run \n");
	FreeObject(object);
	printf("Acquisition excecuted \n");

	//	WriteVolRF(volrf);
	//    AddGaussianNoiseToRFVolume(volrf,0.001);
	if (filter == 1)    VolRFFiltering(volrf, HANNING);

	/* Reconstruction */
	volrecmod = RecVolIFFTModuleFromVol(volrf);
	volrf = NULL; /* Should be an IdVolFree function call, but volrf pointer if already free within IdVolIFFT function ! */

	/* Save & Display */
#ifdef HAVE_MPI
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if (rank==0)
		WriteVolRecUchar(volrecmod,fileout);
#else
	if (display == 1) DisplayVolXY((PPPVOLUME)volrecmod, 0, "Recvol2D");
	WriteVolRecUchar(volrecmod, fileout);
#endif
	IdVolFree(volrecmod);

	printf("TestSequence3D End\n");
}
