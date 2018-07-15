/*************************************************************************
* $Id: object.h,v 1.10 2006/12/18 15:10:38 yougz Exp $
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
#ifndef _OBJECT_H
#define _OBJECT_H

#include <stdlib.h>
#include <idseq.h>
#include <idvol.h>
#include <idsig.h>
#include <idmatric.h>

/*!\def WATER 0 Number of water component */
/*!\def FAT 1 Number of fat component*/
#define WATER 0       
#define FAT 1
/*!\def DECWATER 0.0 Chemical shift from water  :-)*/
/*!\def DECFAT 150.0 Chemical shift (in Hertz) from fat to water at 1 Tesla */
#define DECWATER 0.0
#define DECFAT 150.0

#define RO 0
#define T1 1
#define T2 2



/*!\typedef OBJECT3D Data structure for th virtual object */
typedef struct _Object3D {
    /*! Object size in voxels  */
	int x,y,z;	      
     /*! Object size in meter */
	float lx,ly,lz;  
    /*! Number of considered components . Default value : One (Water) */
	int nbcomponent; 
	/*! Number of the active component for the acquisition */
	int component;
	/*! Number of physical values by component. Default value =3 (ro, t1, t2) */
	int nbparam;
    /*! DeltaB0 in Tesla: T2* weight for a reference value of B0=1T */
	PPPVOLUME_FLOAT deltaB0;
    /*! Nbparam volumes per component, T1 et T2 values are integer and are given in ms. */
	PPPPSEQUENCE_USHORT values; 
    /*! Chemical shift value for each component */
	PSIGNAL_FLOAT chemshift; 
	/*!Magnetic field default associated to the magnetic susceptibility. Given in Tesla for a B0 value of 1 tesla */
	PPPVOLUME_FLOAT fielddefault;
} OBJECT3D; 

/****************************************
* Function prototypes
*****************************************/
OBJECT3D* GetTestObject2D         (int,int,int,double,double,char *);
OBJECT3D* GetTestObject3D         (int,int,int,int,double,double,char *);
OBJECT3D* AllocObject             (int,int,int,int,int);
void      FreeObject              (OBJECT3D*);
void      WriteObject             (OBJECT3D*,char *);
void      SaveObjectAsVolFloat    (OBJECT3D*);
OBJECT3D* ReadObject              (char *);
void      SetObjectSize           (OBJECT3D*, float, float, float);
void      SetObjectDeltaB0        (OBJECT3D*, double);
void      SetObjectChemicalShift (OBJECT3D*,float,int);
void      SetObjectActiveComponent(OBJECT3D*, int);
void      SetObjectPoint               (OBJECT3D*,int,int,int,int,int,int,int);
#endif
