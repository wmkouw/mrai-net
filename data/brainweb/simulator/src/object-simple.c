/*************************************************************************
* $Id: object-simple.c,v 1.10 2006/12/18 15:10:38 yougz Exp $
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

#include "object-simple.h"

#include <idseq.h>
#include <idacr.h>
#include <string.h>
#include <idvol.h>
#include "display.h"

/*!
*************************************************
*  \ingroup Object
* \brief Create an ro,t1, t2 homogeneous object of one component
* \param dimx x size in voxels
* \param dimy y size in voxels
* \param dimz z size in voxels
* \param t1	t1 value
* \param t2	t2 value
* \param ro	ro value
* \return Pointer on the object	
**************************************************/
OBJECT3D * CreateHomogeneousObject(int dimx,int dimy,int dimz,int t1,int t2,int ro)
{
 OBJECT3D * object;
 int       nbparam,nbcomp;
 int       i,j,k;
  
 nbcomp = 1;  /* We only consider the water proton */
 nbparam= 3;  /* ro, t1, t2 */
 object = AllocObject(dimx,dimy,dimz,nbcomp,nbparam);

 for (i=0;i<dimx;i++) for(j=0;j<dimy;j++) for(k=0;k<dimz;k++)
    SetObjectPoint(object,i,j,k,ro,t1,t2,WATER);
  
 return(object);
}

/*!
**************************************************
* \ingroup Object
* \brief Set an homogeneous sphere within an object
* \param radius		Sphere radius
* \param dimx		x object size in voxel
* \param dimy		y object size in voxel
* \param dimz		z object size in voxel
* \param ox			Sphere centre position along x
* \param oy			Sphere centre position along y 
* \param oz			Sphere centre position along z 
* \param t1s		t1 value inside the sphere
* \param t2s		t2 value inside the sphere
* \param ros		ro value inside the sphere
* \param t1b		t1 value outside the sphere
* \param t2b		t2 value outside the sphere
* \param rob		ro value outside the sphere
* \param fill_all   Filling the outside of the sphere (1=yes / 0=No)
**************************************************/
void SetSphericalObject(OBJECT3D * object,
	int radius,
	int dimx, int dimy, int dimz,
	int ox, int oy, int oz,
	int t1s, int t2s, int ros,
	int t1b, int t2b, int rob, int fill_all)
{
  int       i,j,k;
  
/* Few verifications verifications */
  if ((object->nbparam !=3)||(object->nbcomponent!=1))
  {
	  printf("The number of object values is not correct !\n");
	  exit(0);
  }
  if (!((object->x==dimx)&&(object->y==dimy)&&(object->z==dimz)))
  {
	  printf("The object size is not correct !\n");
	  exit(0);
  }

/* filling the object */
  for (i=0;i<dimx;i++) for(j=0;j<dimy;j++) for(k=0;k<dimz;k++)
  {
	if ((i - ox) * (i - ox) + (j - oy) * (j - oy) +
		    (k - oz) * (k - oz) < radius * radius) {
			SetObjectPoint(object,i,j,k,ros,t1s,t2s,WATER);
		} else {
		    if (fill_all) {
			SetObjectPoint(object,i,j,k,rob,t1b,t2b,WATER);
			}
		}
  }
}

/*!
*************************************************
*  \ingroup Object
* \brief Create a spherical object with homogeneous ro, T1, t2 values
* \param radius		Sphere radius
* \param dimx		x object size in voxel
* \param dimy		y object size in voxel
* \param dimz		z object size in voxel
* \param ox			Sphere centre position along x
* \param oy			Sphere centre position along y 
* \param oz			Sphere centre position along z 
* \param t1s		t1 value inside the sphere
* \param t2s		t2 value inside the sphere
* \param ros		ro value inside the sphere
* \param t1b		t1 value outside the sphere
* \param t2b		t2 value outside the sphere
* \param rob		ro value outside the sphere
* \param fill_all   Filling the outside of the sphere (1=yes / 0=No)
* \return			Pointer on the object
**************************************************/
OBJECT3D * CreateSphericalObject(
	int radius,
	int dimx, int dimy, int dimz,
	int ox, int oy, int oz,
	int t1s, int t2s, int ros,
	int t1b, int t2b, int rob, int fill_all)
{
 OBJECT3D * object;
 int       nbparam,nbcomp;
  
 nbcomp = 1;  /* We only consider the water protons */
 nbparam= 3;  /* ro, t1, t2 */
 object = AllocObject(dimx,dimy,dimz,nbcomp,nbparam);
 SetSphericalObject(object,radius,dimx,dimy,dimz,ox,oy,oz,t1s,t2s,ros,t1b,t2b,rob,fill_all);
 return(object);
}


/*!
**************************************************
* \ingroup Object
* \brief Set an homogeneous ellipse within an object
* \param rx	        x radius of the ellipse
* \param ry	        y radius of the ellipse
* \param rz	        z radius of the ellipese
* \param dimx		x object size in voxel
* \param dimy		y object size in voxel
* \param dimz		z object size in voxel
* \param ox			Ellipse centre position along x
* \param oy			Ellipse centre position along y 
* \param t1s		t1 value inside the ellipse
* \param t2s		t2 value inside the ellipse
* \param ros		ro value inside the ellipse
* \param t1b		t1 value outside the ellipse
* \param t2b		t2 value outside the ellipse
* \param rob		ro value outside the ellipse
* \param fill_all   Filling the outside of the ellipse (1=yes / 0=No)
**************************************************/
void SetEllipticalObject(OBJECT3D *object,
	int rx, int ry, int rz,
	int dimx, int dimy, int dimz,
	int ox, int oy, int oz,
	int t1s, int t2s, int ros,
	int t1b, int t2b, int rob, int fill_all)
{
  int       i,j,k;
  
/* Few verifications  */
  if ((object->nbparam !=3)||(object->nbcomponent!=1))
  {
	  printf("The number of object values is not correct !\n");
	  exit(0);
  }
  if (!((object->x==dimx)&&(object->y==dimy)&&(object->z==dimz)))
  {
	  printf("The object size is not correct !\n");
	  exit(0);
  }

/* Filling the object */
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


/*!
**************************************************
*\ingroup Object
* \brief Create an homogeneous elliptical object
* \param rx	        x radius of the ellipse
* \param ry	        y radius of the ellipse
* \param rz	        z radius of the ellipese
* \param dimx		x object size in voxel
* \param dimy		y object size in voxel
* \param dimz		z object size in voxel
* \param ox			Ellipse centre position along x
* \param oy			Ellipse centre position along y 
* \param oz			Ellipse centre position along z
* \param t1s		t1 value inside the ellipse
* \param t2s		t2 value inside the ellipse
* \param ros		ro value inside the ellipse
* \param t1b		t1 value outside the ellipse
* \param t2b		t2 value outside the ellipse
* \param rob		ro value outside the ellipse
* \param fill_all   Filling the outside of the ellipse (1=yes / 0=No)
* \return			Pointer on the object
**************************************************/
OBJECT3D * CreateEllipticalObject(
	int rx, int ry, int rz,
	int dimx, int dimy, int dimz,
	int ox, int oy, int oz,
	int t1s, int t2s, int ros,
	int t1b, int t2b, int rob, int fill_all)
{
  OBJECT3D * object;
  int       nbparam,nbcomp;
  
  nbcomp = 1;  /* We only consider the water protons */
  nbparam= 3;  /* ro, t1, t2 */
  object = AllocObject(dimx,dimy,dimz,nbcomp,nbparam);
  SetEllipticalObject(object,rx,ry,rz,dimx,dimy,dimz,ox,oy,oz,t1s,t2s,ros,t1b,t2b,rob,fill_all);

  return(object);
}

/*!
**************************************************
* \ingroup			Object
* \brief			Set an homogeneous spherical cylinder within an object
* \param object     Pointer on the object
* \param radius	    Cylinder radius
* \param dimx		x object size in voxel
* \param dimy		y object size in voxel
* \param dimz		z object size in voxel
* \param ox			Axis position along x
* \param oy			Axis position along y 
* \param t1s		t1 value inside the cylinder
* \param t2s		t2 value inside the cylinder
* \param ros		ro value inside the cylinder
* \param t1b		t1 value outside the cylinder
* \param t2b		t2 value outside the cylinder
* \param rob		ro value outside the cylinder
* \param fill_all   Filling the outside of the cylinder (1=yes / 0=No)
* \return			Pointeur vers l'object
**************************************************/	
void SetSphericalCylindricObject(OBJECT3D * object,
	int radius,
	int dimx, int dimy, int dimz,
	int ox, int oy,
	int t1s, int t2s, int ros,
	int t1b, int t2b, int rob, int fill_all)
{
  int       i,j,k;
  
/* Few verifications  */
  if ((object->nbparam !=3)||(object->nbcomponent!=1))
  {
	  printf("The number of object values is not correct !\n");
	  exit(0);
  }
  if (!((object->x==dimx)&&(object->y==dimy)&&(object->z==dimz)))
  {
	  printf("The object size is not correct !\n");
	  exit(0);
  }

/* Filling the object */
  for (i=0;i<dimx;i++) for(j=0;j<dimy;j++) for(k=0;k<dimz;k++)
  {
	if ((i - ox) * (i - ox) + (j - oy) * (j - oy) < radius * radius) {
			SetObjectPoint(object,i,j,k,ros,t1s,t2s,WATER);
		} else {
		    if (fill_all) {
			SetObjectPoint(object,i,j,k,rob,t1b,t2b,WATER);
			}
		}
  }
}

/*!
**************************************************
* \ingroup			Object
* \brief			Set an homogeneous elliptical cylinder within an object
* \param object     Pointer on the object
* \param rx			Cylinder radius along x
* \param ry			Cylinder radius along y
* \param dimx		x object size in voxel
* \param dimy		y object size in voxel
* \param dimz		z object size in voxel
* \param ox			Axis position along x
* \param oy			Axis position along y 
* \param t1s		t1 value inside the cylinder
* \param t2s		t2 value inside the cylinder
* \param ros		ro value inside the cylinder
* \param t1b		t1 value outside the cylinder
* \param t2b		t2 value outside the cylinder
* \param rob		ro value outside the cylinder
* \param fill_all   Filling the outside of the cylinder (1=yes / 0=No)
**************************************************/	
void SetEllipticalCylindricObject(OBJECT3D *object,
	int rx, int ry,
	int dimx, int dimy, int dimz,
	int ox, int oy,
	int t1s, int t2s, int ros,
	int t1b, int t2b, int rob,int fill_all)
{
  int       i,j,k;
  
/* Few verifications  */
  if ((object->nbparam !=3)||(object->nbcomponent!=1))
  {
	  printf("The number of object values is not correct !\n");
	  exit(0);
  }
  if (!((object->x==dimx)&&(object->y==dimy)&&(object->z==dimz)))
  {
	  printf("The object size is not correct !\n");
	  exit(0);
  }

/* Filling the object */
  for (i=0;i<dimx;i++) for(j=0;j<dimy;j++) for(k=0;k<dimz;k++)
  {
		if ((i-ox)*(i-ox)/(1.0*rx*rx)+(j-oy)*(j-oy)/(1.0*ry*ry)<1)
		{
			SetObjectPoint(object,i,j,k,ros,t1s,t2s,WATER);
		} else 
		{
		    if (fill_all) {
			SetObjectPoint(object,i,j,k,rob,t1b,t2b,WATER);
			}
		}
  }
}

/*!
**************************************************
* \ingroup	Object
* \brief	Clear a set of planes in an object
* \param	object	Pointer on the object
* \param    first Fisrt plane 
* \param    last  Last plane
* \param dimx		x object size in voxel
* \param dimy		y object size in voxel
* \param dimz		z object size in voxel
**************************************************/
void ClearZplane(OBJECT3D *object,int first, int last,
				 int dimx, int dimy, int dimz)
{
  int       i,j,k;
  for (k=first;k<=last;k++)
  for (i=0;i<dimx;i++) for(j=0;j<dimy;j++)
  {
	SetObjectPoint(object,i,j,k,1,1,1,WATER);
  }
}

/*!
*************************************************
* \ingroup      Object
* \brief        Set a linear variation of the ro, t1, t2, values in the object
* \warning		for one component WATER or FAT 
* \param object  Pointer on the object
* \param rodeb  ro value on the left side of the object
* \param rofin  ro value on the right side of the object
* \param t1deb  t1 value on the left side of the object
* \param t1fin  t1 value on the right side of the object
* \param t2deb  t2 value on the left side of the object
* \param t2fin  t2 value on the right side of the object
* \param xdeb	Fisrt voxel considered
* \param xfin   Last voxel considered
* \param component Component number
**************************************************/	
void SetLinearObject1D(OBJECT3D* object,int rodeb,int rofin,int t1deb,int t1fin,int t2deb,int t2fin,
						int xdeb,int xfin,int component)
{
 int roa,t1a,t2a;
 int rob,t1b,t2b;
 int i;
 int t1,t2,ro;
   
 roa= (int)( (rofin-rodeb)/(xfin-xdeb) ); t1a=(int)( (t1fin-t1deb)/(xfin-xdeb) ); t2a=(int)( (t2fin-t2deb)/(xfin-xdeb) );
 rob=rofin-roa*xfin; t1b=t1fin-t1a*xfin;t2b=t2fin-t2a*xfin;
 for (i=xdeb;i<xfin;i++) 
	{
	 ro=(int)(roa*i+rob) ; t1=(int)(t1a*i+t1b) ; t2=(int)(t2a*i+t2b);
	 SetObjectPoint(object,i,0,0,ro,t1,t2,component);
	}
}

/*!
*************************************************
* \ingroup      Object
* \brief        Create a 2D test object with a circle at the left top,
 an ellipse at the right botom, and a square at the center.
* \warning		Only for the component 0, WATER
* \param nx		x object dimension in voxels
* \param ny     y object dimension in voxels 
* \return object Pointer on the object
**************************************************/	
OBJECT3D * Create2DTestObjectCircleEllipseSquare(int nx,int ny)
{
 OBJECT3D * object;
 int nz=1;
 int ro,t1,t2;
 int rx,ry, ox, oy;
 int i,j,k;
 int radius;
	
 object = (OBJECT3D *) AllocObject(nx,ny,nz,1,3);
    
 /* Background filling */
 //t1=100; t2=50; ro=0;
 t1=500; t2=70; ro=50;
 k=0;
 for (i=0;i<nx;i++) for(j=0;j<ny;j++)
      SetObjectPoint(object,i,j,k,ro,t1,t2,WATER);
 
 /* Circle filling */
 if (nx<ny) radius=nx/8;
 else       radius=ny/8;
 ox=nx/4; oy=ny/8;
 k=0;
 //t1=400; t2=50; ro=100;
 t1=500; t2=70; ro=100;
 for (i=0;i<nx;i++) for(j=0;j<ny;j++)
	if ((i - ox) * (i - ox) + (j - oy) * (j - oy) < radius * radius)
	   	 SetObjectPoint(object,i,j,k,ro,t1,t2,WATER);

/* Square filling */
 t1=2500; t2=70; ro=100; 
 for (i=4*nx/10;i<6*nx/10;i++) for(j=4*ny/10;j<6*ny/10;j++)
	SetObjectPoint(object,i,j,k,ro,t1,t2,WATER);
 
 /* Ellipse filling */
 rx=nx/4;  ry=ny/8;
 ox=3*nx/4; oy=3*ny/4;
 k=0;
 //t1=200; t2=50; ro=100;
 t1=500; t2=150; ro=100; 
 for (i=0;i<nx;i++) for(j=0;j<ny;j++)
	if ((i-ox)*(i-ox)/(1.0*rx*rx)+(j-oy)*(j-oy)/(1.0*ry*ry)<1)
		 SetObjectPoint(object,i,j,k,ro,t1,t2,WATER);
	
 return(object);
}


/*!
**************************************************************************************
* \ingroup Object
* \brief   Create a 2D object with a rectangle presenting a linear gradient of proton density
* \param nx	x object dimension in voxels
* \param ny y object dimension in voxels
* \param lx x rectangle dimension
* \param ly y rectangle dimension 
* \return object Pointer on the object
****************************************************************************************/
OBJECT3D * Create2DTestObjectRectangleRoGradient(int nx,int ny, int lx, int ly)
{

 OBJECT3D * object;
 int nz=1;
 int ro,t1,t2;
 int i,j,k;
 int slope;  /* in ro/pixel*/
 int x1,x2,y1,y2,ro_point;
 
	
 object = (OBJECT3D *) AllocObject(nx,ny,nz,1,3);

 t1=100; t2=50; ro=0; slope= 10; 
 k=0;
 x1=(int)(0.5*(nx-lx));
 x2=(int)(0.5*(nx+lx));
 y1=(int)(0.5*(ny-ly));
 y2=(int)(0.5*(ny+ly));

 /* Background filling*/
 for (i=0;i<nx;i++) for(j=0;j<ny;j++)
      SetObjectPoint(object,i,j,k,ro,t1,t2,WATER);


 /* Rectangle filling */
 
 for (i=x1;i<x2;i++) for(j=y1;j<y2;j++)
	{
	     ro_point=(int)(ro+slope*(i-x1));
	   	 SetObjectPoint(object,i,j,k,ro_point,t1,t2,WATER);
	}
 return(object);
}


/**************************************************
* \ingroup object
* \brief Set a cone within an object
* \param object     Pointer on the object
* \param dimx		x object size in voxel
* \param dimy		y object size in voxel
* \param dimz		z object size in voxel
* \param ox			Cone centre position along x
* \param oy			Cone centre position along y 
* \param r			Basis circle radius at z=0
* \param h		    Cone high
* \param t1s		t1 value inside the cone
* \param t2s		t2 value inside the cone
* \param ros		ro value inside the cone
* \param t1b		t1 value outside the cone
* \param t2b		t2 value outside the cone
* \param rob		ro value outside the cone
* \param fill_all   Filling the outside (Yes=1, No=0) 
**************************************************/
void SetObjectCone(OBJECT3D * object,
	int dimx, int dimy, int dimz,
	int r,  int h,
	int ox, int oy,
	int t1s, int t2s, int ros,
	int t1b, int t2b, int rob, int fill_all)
{
  int       i,j,k;
  float radius;
  

/* Few verifications  */
  if ((object->nbparam !=3)||(object->nbcomponent!=1))
  {
	  printf("The number of object values is not correct !\n");
	  exit(0);
  }
  if (!((object->x==dimx)&&(object->y==dimy)&&(object->z==dimz)))
  {
	  printf("The object size is not correct !\n");
	  exit(0);
  }

  for(k=0;k<h;k++)
  {
		radius = -(float)k/(float)h*r+r;

		for(i=0;i<dimx;i++) for(j=0;j<dimy;j++)

		{
			if (((i-ox)*(i-ox) + (j-oy)*(j-oy))< (radius * radius)) 
			{
			SetObjectPoint(object,i,j,k,ros,t1s,t2s,WATER);
			} 
			else 
			{
				if (fill_all) 
				{
				SetObjectPoint(object,i,j,k,rob,t1b,t2b,WATER);
			}
			}
		 }
	}


}