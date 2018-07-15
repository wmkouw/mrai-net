/*************************************************************************
* $Id: object-compound.c,v 1.10 2006/12/18 15:10:37 yougz Exp $
* Contains functiosn to define objects with multiple components
* At moment, two components for water and fat
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
#include "object-compound.h"
#include <idseq.h>
#include <idacr.h>
#include <string.h>
#include <idvol.h>
#include <display.h>

/*!
*************************************************
* \ingroup      Object
* \brief        Create a 1D object with three parts having linear 
* variation of (ro, t1,t2) fro the water and fat component
* \warning		
* \param nx		x object size in pixel
* \return object Pointer on the created object
**************************************************/	
OBJECT3D * CreateObjectBiComponent1D(int nx)
{
 OBJECT3D * object;
 int xdeb, xfin,rodeb,rofin,t1deb,t1fin,t2deb,t2fin;
 int ny,nz;

 ny=1;nz=1;
 object = (OBJECT3D *) AllocObject(nx,ny,nz,2,3);
 SetObjectChemicalShift(object,DECWATER,WATER);
 SetObjectChemicalShift(object,DECFAT,FAT);
 rodeb=60;rofin=100;
 t1deb=50;t1fin=400;
 t2deb=50;t2fin=90;

 /* 1rst part */
 xdeb=0;xfin=nx/8;
 SetLinearObject1D(object,rodeb, rofin,t1deb,t1fin,t2deb,t2fin,xdeb, xfin, WATER);
 SetLinearObject1D(object,rodeb, rofin,t1deb,t1fin,t2deb,t2fin,xdeb, xfin, FAT);
 /* 2nd part */
 xdeb=nx/8;xfin=nx/4;
 rodeb=100;rofin=100;t1deb=180;t1fin=180;t2deb=90;t2deb=90;
 SetLinearObject1D(object,rofin, rodeb,t1fin,t1deb,t2fin,t2deb,xdeb, xfin, WATER);
 SetLinearObject1D(object,rofin, rodeb,t1fin,t1deb,t2fin,t2deb,xdeb, xfin, FAT);
 /* background */
 xdeb=nx/4; xfin=nx;
 rodeb=50;t1deb=150;t2deb=60;rofin=rodeb;t1fin=t1deb; t2fin=t2deb;
 SetLinearObject1D(object,rofin, rodeb,t1fin,t1deb,t2fin,t2deb,xdeb, xfin, WATER);
 SetLinearObject1D(object,rofin, rodeb,t1fin,t1deb,t2fin,t2deb,xdeb, xfin, FAT);

 return(object);
}

/*!
******************************************************
* \ingroup Object
* \brief   Create a 2D virtual object with one circle of wat, surrounded by a ring of water 
* \warning
* \param        nx x object size in pixel
* \param        ny y object size in pixel
* \return object Pointer on the created object
********************************************************/  
OBJECT3D * CreateObjectBiComponent2DCircle(int nx,int ny)
{
 OBJECT3D * object;
 int nz=1;
 int ro,t1,t2;
 int  ox, oy;
 int i,j,k;
 int rext,rint;
 
 object = (OBJECT3D *) AllocObject(nx,ny,nz,2,3);
 SetObjectChemicalShift(object,DECWATER,WATER);
 SetObjectChemicalShift(object,DECFAT,FAT);
 
/* Background*/
 ox=nx/2; oy=ny/2;
 t1=100; t2=50; ro=0;
 //t1=200; t2=2; ro=2;
 k=0;
 for (i=0;i<nx;i++) for(j=0;j<ny;j++)
	{
	 SetObjectPoint(object,i,j,k,ro,t1,t2,WATER);
	 SetObjectPoint(object,i,j,k,0,t1,t2,FAT);
	}

 /* Water extern circle */
 if (nx<ny)
   {
	rext=nx/4*1.5;
    rint=nx/8*1.5;
   }
 else
   {
	rext=ny/4*1.5;
    rint=ny/8*1.5;
   }
    
 ox=nx/2; oy=ny/2;
 k=0;
 t1=3000; t2=300; ro=100;
 for (i=0;i<nx;i++) for(j=0;j<ny;j++)
	if ((((i - ox)*(i - ox)+(j - oy)*(j - oy)) < (rext * rext) ) && ( ((i - ox)*(i - ox)+(j - oy)*(j - oy)) >= (rint*rint)))
	{ SetObjectPoint(object,i,j,k,ro,t1,t2,WATER);
	  SetObjectPoint(object,i,j,k,0,t1,t2,FAT);
	}

 /* Fat internal circle */
 if (nx<ny) 
   rint=nx/8*1.5;
 else 
   rint=ny/8*1.5;
 
 ox=nx/2; oy=ny/2;
 k=0;
 ro=100;t1=350; t2=70; /* T1, T2 values for B0=1T */
    
 for (i=0;i<nx;i++) for(j=0;j<ny;j++)
    if ((i - ox) * (i - ox) + (j - oy) * (j - oy) < rint * rint)
      {
       SetObjectPoint(object,i,j,k,80,t1,t2,FAT);
	   SetObjectPoint(object,i,j,k,20,3000,300,WATER);
	  }

 return(object);
}

/*!
*************************************************
* \ingroup      Object
* \brief        Create a 2D virtual object with one circle of water on the top left,
* and one ellipse of fat at bottom right
* \warning		
* \param        nx x object size in pixel
* \param        ny y object size in pixel
* \return object Pointer on the created object
**************************************************/	
OBJECT3D * CreateObjectBiComponent2DCircleEllipse(int nx,int ny)
{
 OBJECT3D * object;
 int nz=1;
 int ro,t1,t2;
 int  ox, oy,rx,ry;
 register int i,j,k;
 int radius;
 
 object = (OBJECT3D *) AllocObject(nx,ny,nz,2,3);
 SetObjectChemicalShift(object,DECWATER,WATER);
 SetObjectChemicalShift(object,DECFAT,FAT);

 /* Background */
 t1=100; t2=50; ro=100;
 k=0;
 for (i=0;i<nx;i++) for(j=0;j<ny;j++)
	   SetObjectPoint(object,i,j,k,ro,t1,t2,WATER);
 
 /* Water circle */
 if (nx<ny) radius=nx/8;
 else       radius=ny/8;
 ox=nx/4; oy=ny/4;
 k=0;
 t1=0; t2=0; ro=0;
 for (i=0;i<nx;i++) for(j=0;j<ny;j++)
    if ((i - ox) * (i - ox) + (j - oy) * (j - oy) < radius * radius)
	   	 SetObjectPoint(object,i,j,k,ro,t1,t2,WATER);
 if (nx<ny) radius=nx/8;
 else       radius=ny/8;
 ox=nx/4; oy=ny/4;
 k=0;
 t1=400; t2=50; ro=100;
 for (i=0;i<nx;i++) for(j=0;j<ny;j++)
    if ((i - ox) * (i - ox) + (j - oy) * (j - oy) < radius * radius)
	   	 SetObjectPoint(object,i,j,k,ro,t1,t2,WATER);
 
 /* Fat ellipse */
 rx=nx/4;  ry=ny/8;
 ox=3*nx/4; oy=3*ny/4;
 k=0;
 ro=1; t1=2500; t2=200;
 for (i=0;i<nx;i++) for(j=0;j<ny;j++)
    if ((i-ox)*(i-ox)/(1.0*rx*rx)+(j-oy)*(j-oy)/(1.0*ry*ry)<1)
   	     SetObjectPoint(object,i,j,k,ro,t1,t2,FAT);
	
 return(object);
}

/*!
**************************************************************************************
* \ingroup      Object
* \brief        Create an object with a samll sphere of fat within a larger one of water
* \warning		pour la componentule d'WATER et/ou graisse avec/sans deplacement chimique 
* \param        nx x object size in pixel
* \param        ny y object size in pixel
* \param        nz z object size in pixel 
* \return object Pointer on the created object
****************************************************************************************/
OBJECT3D * CreateObjectBiComponent3DSphere(int nx,int ny,int nz)
{
 OBJECT3D * object;
 
 object = (OBJECT3D *) AllocObject(nx,ny,nz,2,3);  
 SetObjectChemicalShift(object,DECWATER,WATER);
 SetObjectChemicalShift(object,DECFAT,FAT);
                   
 /* Water extern sphere */
 SetObjectSphereComponent(object, (int)(nx/3),
	     nx, ny, nz,
	     nx / 2, ny / 2, nz /2 , 0, 0, 0, 90, 40, 100, 0,WATER);
 SetObjectSphereComponent(object, (int)(nx/3),
	     nx, ny, nz,
	     nx / 2, ny / 2, nz /2 , 400, 50, 100, 90, 40, 100, 1,WATER);

 /* Fat intern sphere */
 SetObjectSphereComponent(object, (int)(nx/5),
	     nx, ny, nz,
	     nx / 2, ny / 2, nz /2 , 0, 0 , 0, 90, 40, 100, 0,WATER);
 SetObjectSphereComponent(object, (int)(nx/5),
	     nx, ny, nz,
	     nx / 2, ny / 2, nz /2 , 180, 90 , 1, 90, 40, 100, 0,FAT);
 SetObjectSphereComponent(object, (int)(nx/5),
	     nx, ny, nz,
	     nx / 2, ny / 2, nz /2 , 180, 90 , 1, 90, 40, 100, 0,WATER);

 return (object);
}

/*!
**************************************************
* \ingroup      Object
* \brief        Create an object with a sphere of water and an ellipse of water
* \warning		
* \param        nx x object size in pixel
* \param        ny y object size in pixel
* \param        nz z object size in pixel 
* \return object Pointer on the created object
**************************************************/
OBJECT3D * CreateObject3DSphereEllipse(int nx, int ny, int nz)
{
    OBJECT3D * object;
	object = (OBJECT3D *) AllocObject(nx,ny,nz,1,3);

	SetObjectSphereComponent(object, (int)(nx/5),
	     nx, ny, nz,
	     nx / 4, ny / 4, nz / 4,
             500, 90, 70,
             10, 50, 2,1,WATER); /* GM */

    SetObjectEllipseComponent(object, (int)(nx/2.5), (int)(nx/4), (int)(nx/8),
	      nx, ny, nz,
	      nx / 2, ny / 2, nz / 2, 
              350, 75, 60, 
              10, 50, 2,0,WATER); /* WM */
 
	return(object);
}

/*!
**************************************************
* \ingroup			Object
* \brief			Define an homogeneous sphere of one component within an object
* \param object     Pointer on the object
* \param radius		sphere radius
* \param dimx		x sphere size
* \param dimy		y sphere size
* \param dimz		z sphere size
* \param ox			x position of the sphere
* \param oy			y position of the sphere 
* \param oz			z position of the sphere 
* \param t1s		t1 value in the sphere
* \param t2s		t2 value in the sphere
* \param ros		ro value in the sphere
* \param t1b		t1 value out of the sphere
* \param t2b		t2 value out of the sphere
* \param rob		ro value out of the sphere
* \param fill_all   Set values outside the sphere: 1 (Yes), 0 (No)
* \param component  0>> Water 1 >> Fat
**************************************************/	
void SetObjectSphereComponent(OBJECT3D * object,
	int radius,
	int dimx, int dimy, int dimz,
	int ox, int oy, int oz,
	int t1s, int t2s, int ros,
	int t1b, int t2b, int rob, int fill_all,int component)
{
  register int       i,j,k;
  
/* Few verifications */
  if ((object->nbparam !=3)||(object->nbcomponent>2))
  {
	  printf("Le nombre de composantes de l'object est incoherent !\n");
	  exit(0);
  }
  if (!((object->x==dimx)&&(object->y==dimy)&&(object->z==dimz)))
  {
	  printf("Taille d'object incompatible !\n");
	  exit(0);
  }

  for (i=0;i<dimx;i++) for(j=0;j<dimy;j++) for(k=0;k<dimz;k++)
  {
	if ((i - ox) * (i - ox) + (j - oy) * (j - oy) +
		    (k - oz) * (k - oz) <= radius * radius) {
			SetObjectPoint(object,i,j,k,ros,t1s,t2s,component);
		} else {
		    if (fill_all) {
			SetObjectPoint(object,i,j,k,rob,t1b,t2b,component);
			}
		}
  }
}

/*!
**************************************************
* \ingroup			Object
* \brief			Define an homogeneous ellipse of one component within an object 
* \param object     Pointer on the object
* \param rx		    x ellipse radius
* \param ry		    y ellipse radius
* \param rz		    z ellipse radius
* \param dimx		x ellipse size
* \param dimy		y ellipse size
* \param dimz		z ellipse size
* \param ox			x position of the ellipse
* \param oy			y position of the ellipse 
* \param oz			z position of the ellipse 
* \param t1s		t1 value in the ellipse
* \param t2s		t2 value in the ellipse
* \param ros		ro value in the ellipse
* \param t1b		t1 value out of the ellipse
* \param t2b		t2 value out of the ellipse
* \param rob		ro value out of the ellipse
* \param fill_all   Set values outside the ellipse: 1 (Yes), 0 (No)
* \param component  0>> Water 1 >> Fat
**************************************************/
void SetObjectEllipseComponent(OBJECT3D *object,
	int rx, int ry, int rz,
	int dimx, int dimy, int dimz,
	int ox, int oy, int oz,
	int t1s, int t2s, int ros,
	int t1b, int t2b, int rob, int fill_all,int component)
{
   register int       i,j,k;

/* Few verifications */
  if ((object->nbparam !=3)||(object->nbcomponent>21))
  {
	  printf("Le nombre de composantes de l'object plus grand que 2 !\n");
	  exit(0);
  }
  if (!((object->x==dimx)&&(object->y==dimy)&&(object->z==dimz)))
  {
	  printf("Taille d'object incompatible !\n");
	  exit(0);
  }

  for (i=0;i<dimx;i++) for(j=0;j<dimy;j++) for(k=0;k<dimz;k++)
  {
		if ((i-ox)*(i-ox) / (1.0*rx*rx) + (j-oy)*(j-oy) / (1.0*ry*ry)
			+ (k-oz)*(k-oz) / (1.0*rz*rz) < 1) {
			SetObjectPoint(object,i,j,k,ros,t1s,t2s,WATER);
		} else {
		    if (fill_all) {
			SetObjectPoint(object,i,j,k,rob,t1b,t2b,WATER);
			}
		}
  }
}

