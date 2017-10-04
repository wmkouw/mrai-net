/*************************************************************************
* $Id: experience.h,v 1.46 2006/12/18 15:10:37 yougz Exp $
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

#ifndef _EXPERIENCE_H
#define _EXPERIENCE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <idseq.h>
#include <idvol.h>
#include "simri3d.h"
#include "object.h"

#define ATT_INIT 1000000 /* Initial waiting time in order to get numerically 
                            an equilibrium M0z value at the beginning of a sequence */
                         
#define ON 1
#define OFF 0

/*************************************************
* Structure which groups together object information and experience conditions
**************************************************/
typedef struct _Experience3D {
    char name[TAILLE_NOM];
    
	/*!MRI image (k-space) size*/
	int ntx, nty, ntz;		
    /*! Acquisition time in seconds (Band-Width inverse) */
	double readout_time;		
    /*! Number of voxels used to define the object (> or = to the MRI image size) */
	int nfovx, nfovy, nfovz;			
#ifdef HAVE_MPI
    /*! The working zone within the object for an MPI node, 
        limited to the interval [x0 .. x0-width[  */
        int mpi_slice_x0;
	int mpi_slice_width;
#endif
    /*! Object size in meter*/
	float lx,ly,lz;
    /*! Fov size in meter */
	double fovx,fovy,fovz;
    /*! Object center shifting in meter */
	double obx,oby,obz;	
    /*!T1 table with the object size */
    float ***t1;		
	/*!T2 table with the object size */
    float ***t2;
    /*!RO table with the object size */
	float ***ro;
	/*!Time variable used for the T2* management */
	double to;
    /*! Intra spin DeltaB0 value in Tesla, T2* effect */						   
	float *** deltaB0;		
    /*! RF signal table viewed in x,y and z directions <-> sgn[3][ntx][ny][nz] */
    float ****sgn;		
    /*! Spin magnetization vector state table */
	double ****magstate;	
    /* Object center posiition */
    double xf,yf,zf;
    /*! Spatial step in meter = tailleobjet/(nx-1) */
	double px,py,pz;			
    /*! Time step = sampling period of the RF signal = tacq/(ntx-1) */
    double pt;			
    /*!Flag for field default accounting (1=active)*/
	int FLAG_DECH;		
	/*!Field default inter-voxel (susceptibility ...)in Tesla */
	float ***G_deche;	
    /*!Main static field value in Tesla*/
	double B0;			
    /*!Flag for B1 accounting (1=active)*/
	int inhomogeneousB1;
    // Introduce complex B1(r) computed from external RF coil simulator.
    /*!B1 transmission field */
	PPPVOLUME_COMPLEX_FLOAT B1t;
	/*!B1 reception field, B1(r) should be normalized so that its maximum module is one */
	PPPVOLUME_COMPLEX_FLOAT B1r;
    /*!Maximum value of the parabolic static field default (centered on the object) */
	double b0def;			
	/*!Water relative chemical shift value (rad/s) for the considered object component */	
	double G_edw;
	/*!FLAG to take off resonance into account (OFF) or not (ON) within RF pulse */
	double FLAG_RESONANCE;

} EXPERIENCE3D;

/***************************************************
* Functions prototype 
***************************************************/
EXPERIENCE3D * AllocExperience();
EXPERIENCE3D * AllocExperienceSgn(int ,int , int);
void FreeExperience (EXPERIENCE3D *);
void FreeExperienceSgn (EXPERIENCE3D *);
void SetNameExperience  (EXPERIENCE3D *, char *);
void ResetToExperience(EXPERIENCE3D *);
void InitToExperience (EXPERIENCE3D *);
void SetT1Experience   (EXPERIENCE3D *,OBJECT3D *,int);
void SetT2Experience   (EXPERIENCE3D *,OBJECT3D *,int);
void SetRoExperience   (EXPERIENCE3D *,OBJECT3D *,int);
void InitMagstateExperience(EXPERIENCE3D *);
void SetB0Experience   (EXPERIENCE3D *,double);
void SetB0DefExperience(EXPERIENCE3D *,double);
void SetDeltaB0Experience(EXPERIENCE3D *,OBJECT3D *);
void SetGdechExperience(EXPERIENCE3D *,OBJECT3D *);
void SetResonanceExperience(EXPERIENCE3D *,int);
void SetFlagdechExperience (EXPERIENCE3D *,int);
void SetObjectExperienceComponent(EXPERIENCE3D *, OBJECT3D *,int,int);
void SetFovExperience  (EXPERIENCE3D *,double,double,double,
	 				                  double,double,double);
void InitRFSigExperience(EXPERIENCE3D *);
void SetAcqExperience   (EXPERIENCE3D *,int,int,int,double);
void NormalizeRFSignal  (EXPERIENCE3D *);

PPIMAGE_UCHAR			 GetkSpaceImage2DFromExperience    (EXPERIENCE3D *,int, int);
PPPVOLUME_FLOAT          GetSignalRFComponentFromExperience(EXPERIENCE3D *,int);
PPPVOLUME_COMPLEX_DOUBLE GetSignalRFComplexFromExperience  (EXPERIENCE3D *);
PPPVOLUME_FLOAT          GetSignalRFModuleFromExperience   (EXPERIENCE3D *);
PPPVOLUME_FLOAT          GetSignalRFPhaseFromExperience    (EXPERIENCE3D *);
EXPERIENCE3D *           GetSumSignalRFComplex(PPPVOLUME_COMPLEX_DOUBLE ,
											PPPVOLUME_COMPLEX_DOUBLE     );
EXPERIENCE3D *           GetDiffSignalRFComplex(PPPVOLUME_COMPLEX_DOUBLE ,
											PPPVOLUME_COMPLEX_DOUBLE     );
void AddComplexVolRF(EXPERIENCE3D *, PPPVOLUME_COMPLEX_DOUBLE, PPPVOLUME_COMPLEX_DOUBLE);

/*****************************************************************************************/
// Alexis Amadon, December 2005
void SetB1(EXPERIENCE3D *expr);
/******************************************************************************************/

#endif
