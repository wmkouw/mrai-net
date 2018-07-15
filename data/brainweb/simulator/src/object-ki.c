/*************************************************************************
* $Id: object-ki.c,v 1.11 2006/12/19 15:09:53 yougz Exp $
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
#include "object-ki.h"

#include <idseq.h>
#include <idacr.h>
#include <string.h>
#include <idvol.h>
#include "display.h"

/*!
*************************************************
* \ingroup Object
* \brief Set the field default value for a 1D object
* Values are given in tesla and normalised for a 1 Tesla B0 field 	 
* \param object	Pointer on the virtual object
* \param dech	Volume with the field default values in tesla
* \param posy	Y position on the default field volume
* \param posz	Z position on the default field volume
* \param dimx	X dimension of the virtual object
***************************************************/
void SetObjectFieldDefault1D(OBJECT3D* object,int posy,int posz,PPPVOLUME_FLOAT dech,int dimx)
{
 int i;
	
	for(i=0;i<dimx;i++)
		object->fielddefault[0][0][i]=dech[i][posy][posz];
}

/*!
*************************************************
* \ingroup Object
* \brief Set the field default value for a 2D object
* Values are given in tesla and normalised for a 1 Tesla B0 field 	 
* \param object	Pointer on the virtual object
* \param dech	Volume with the field default values in tesla
* \param pos	Slice position in the default field volume
* \param dimx	X dimension of the virtual object
* \param dimy	Y dimension of the virtual object
***************************************************/
void SetObjectFieldDefault2D(OBJECT3D* object,int pos,PPPVOLUME_FLOAT dech,int dimx,int dimy)
{
 int i,j;
 PPIMAGE_FLOAT slice_dech=NULL;
 PPIMAGE_UCHAR slice=NULL;

 /* visualisation of the field default slice which has been chosen */
	slice_dech= (PPIMAGE_FLOAT)IdImaAlloc(dimx,dimy,IMA_FLOAT);
	for (i=0;i<dimx;i++) for(j=0;j<dimy;j++) 
		slice_dech[j][i] = dech[i][j][pos];

	slice= (PPIMAGE_UCHAR)IdImaAlloc(dimx,dimy,IMA_UCHAR);
	IdImaRecadTo8((PPIMAGE)slice_dech,slice,-1.0,-1.0);
	DisplayImage2D(slice);
	
	for (i=0;i<dimx;i++) for(j=0;j<dimy;j++)
		object->fielddefault[0][j][i]= dech[i][j][pos];

 IdImaFree(slice_dech);
 IdImaFree(slice);
}

/*!
*************************************************
* \ingroup	Object
* \brief Set the field default value for a 3D object
* Values are given in tesla and normalised for a 1 Tesla B0 field 	 
* \param object	Pointer on the virtual object
* \param dech	Volume with the field default values in tesla
* \param dimx	X dimension of the virtual object
* \param dimy	Y dimension of the virtual object
* \param dimy	Z dimension of the virtual object
***************************************************/
void SetObjectFieldDefault3D(OBJECT3D * object,PPPVOLUME_FLOAT dech,int dimx, int dimy,int dimz)
{
 int i,j,k;
 for (i=0;i<dimx;i++) for(j=0;j<dimy;j++) for(k=0;k<dimz;k++)
		object->fielddefault[k][j][i]= dech[i][j][k];
}

/*!
*************************************************
* \ingroup	Object
* \brief Set the field default value of an object
* \param object Pointer on the objetc
* \param dech Volume of default field values given in Tesla
***************************************************/
void SetObjectFieldDefault(OBJECT3D * object,PPPVOLUME_FLOAT dech)
{
 int i,j,k;
 for (i=0;i<IdVolDimX(dech);i++) for(j=0;j<IdVolDimY(dech);j++) for(k=0;k<IdVolDimZ(dech);k++)
	object->fielddefault[k][j][i]= dech[k][j][i];
}


/**************************************************
*  \ingroup Object
* \brief Set the ro, t1, t2, and ki values for one point of an object
* \param object Pointer on the object
* \param x x coordinate of the point
* \param y x coordinate of the point
* \param z x coordinate of the point
* \param ro ro value
* \param t1 t1 value
* \param t2 t2 value
* \param ki susceptibility value
**************************************************/
void SetObjectPointSusceptibility(OBJECT3D *object,int x,int y,int z,
				   int ro,int t1,int t2,int ki) 
{   
 object->values[0][z][y][x] = ro;
 object->values[1][z][y][x] = t1;
 object->values[2][z][y][x] = t2;
 object->values[3][z][y][x] = ki;
}

/*!
**************************************************
* \ingroup     Utility
* \brief       Allocate a 1D float vector initialized to zero
* \warning     
* \param x     Signal size
* \return      Pointer on the allocated signal
**************************************************/
PVECTOR_FLOAT AllocInitVectFloat(int x)
{
PVECTOR_FLOAT vect;
int i;

/*Allocation*/
vect=(PVECTOR_FLOAT) IdVectAlloc(x,VECT_FLOAT);
if (!vect)
  {
   printf("Allocation error in AllocInitVectFloat ! \n");
   exit(1);
  }

/*Initialisation*/
for (i=0;i<x;i++)
	vect[i]=0.0;

return vect;
}

/*!
**************************************************
* \ingroup       Object
* \brief         Allocate and set vales of a Float volume 
* regarding a volume of unsigned char label and a table that gives the values 
* associated to the label
* \param filein  File name of the unsigned char volume of labels 
* \param datatab Signal contained the values associated to the label   
* \warning         
* \return        Pointer on the volume of values
**************************************************/
PPPVOLUME_FLOAT GetParamFromDataFile(char *filein,PSIGNAL_FLOAT datatab)
{
 PPPVOLUME_UCHAR label;
 PPPVOLUME_FLOAT data;
 int nx,ny,nz;
 int x,y,z;

 label = (PPPVOLUME_UCHAR) IdAcrReadFile(filein,VOL_UCHAR);
 nx=IdVolDimX(label);
 ny=IdVolDimY(label);
 nz=IdVolDimZ(label);

 data=(PPPVOLUME_FLOAT)IdVolAlloc(nx,ny,nz,VOL_FLOAT);
 for (x=0;x<nx;x++) for (y=0;y<ny;y++) for(z=0;z<nz;z++) 
	    data[z][y][x]=datatab[label[z][y][x]];
 IdVolFree(label);
 return data;
}

/*!
*************************************************
* \ingroup			Object
* \brief			Create an object which is a 3D homogeneous sphere in ro t1 t2 ki
* \warning The filemane containing the region label and the field default values linked 
* to the susceptibility difference are hard written in the code : 'hightresolution.vol' and 'Defchamps.vol'
* \param dimx       Number of object pixels along x
* \param dimy       Number of object pixels along y
* \param dimz       Number of object pixels along z 
* \return			Pointeur on the created object
**************************************************/
OBJECT3D * CreateObject3DSphereKi(int dimx, int dimy, int dimz)												
{
 OBJECT3D * objet;
 int       nbcomp,nbparam;
 int       i,j,k; 
 PPPVOLUME_FLOAT ro,t1,t2,ki,dech;  
 PVECTOR_FLOAT roval,t1val,t2val,kival;
 char  filelabel[80],filedech[80];
 filelabel[0]='\0';
 filedech[0]='\0';

 strcpy(filelabel,"hightresolution.vol");
 strcpy(filedech,"Defchamps.vol");

 nbcomp = 1;  
 nbparam= 4;  /* ro, t1, t2 ,db */
 objet = AllocObject(dimx,dimy,dimz,nbcomp,nbparam);

 roval=AllocInitVectFloat(2);
 roval[0]=100;                 /* region 0  */
 roval[1]=90;                  /* region 1  */
 ro=GetParamFromDataFile(filelabel,roval);

 t1val=AllocInitVectFloat(2);
 t1val[0]=90 ;                  /* region 0 */
 t1val[1]=400;                   /* region 1 */
 t1=GetParamFromDataFile(filelabel,t1val);

 t2val=AllocInitVectFloat(2);
 t2val[0]=40 ;                  /* region 0 */
 t2val[1]=50;                   /* region 1 */
 t2=GetParamFromDataFile(filelabel,t2val);

 kival=AllocInitVectFloat(2);
 kival[0]=0;					/* region 0 */      
 kival[1]=-9.05e-6;             /* region 1 */  
 ki=GetParamFromDataFile(filelabel,kival);

 for (i=0;i<dimx;i++) for(j=0;j<dimy;j++) for(k=0;k<dimz;k++)
	  SetObjectPointSusceptibility(objet,i,j,k,(int)ro[k][j][i],(int)t1[k][j][i],(int)t2[k][j][i],(int)ki[k][j][i]);

/* Reading the field default values in a file 
 * This file has been obtained with one of the 
 * suceptibility default computation method */
 if (!strcmp(filedech,NULL)) 
	{
	 dech = (PPPVOLUME_FLOAT) IdAcrReadFile(filedech,VOL_FLOAT);
	 SetObjectFieldDefault3D(objet,dech,dimx,dimy,dimz);
	}
	else 
	{
	 printf("Unable to read the field default values %s \n",filedech);
	 exit(1);
	}

 IdVolFree(ro);
 IdVolFree(t1);
 IdVolFree(t2);
 IdVolFree(ki);
 IdVolFree(dech);
 return(objet);
}


/*!
*************************************************
* \ingroup			Object
* \brief			Create an object which is a 2D homogeneous circle in ro t1 t2 ki (part of a sphere)
* \warning The filemane containing the region label and the field default values linked 
* to the susceptibility difference are hard written in the code :"../data/sphere_64_10.vol" and
* "../data/Defchamps_sphere_64_10.vol"
* \param dimx       Number of object pixels along x
* \param dimy       Number of object pixels along y
* \return			Pointeur on the created object
**************************************************/
OBJECT3D * CreateObject2DCircleKi(int dimx, int dimy)
{
 OBJECT3D * objet;
 int       nbcomp,nbparam;
 int       i,j,k;
 int       z;     
 PPPVOLUME_FLOAT ro,t1,t2,ki,dech;  
 PVECTOR_FLOAT roval,t1val,t2val,kival;
 char   filelabel[80], filedech[80];
 filelabel[0]='\0';
 filedech[0]='\0';

 strcpy(filelabel,"../data/ki/sphere_64_10.vol");
 strcpy(filedech,"../data/ki/defchamps_sphere_64_10.vol");
       
 nbcomp = 1;  
 nbparam= 4;  /* ro, t1, t2 ,db */

 objet = AllocObject(dimx,dimy,1,nbcomp,nbparam);
 
 roval=AllocInitVectFloat(2);
 roval[0]=1000;               /* region 0 */
 roval[1]=0;                  /* region 1 */
 ro=GetParamFromDataFile(filelabel,roval);
 IdVectFree(roval);

 t1val=AllocInitVectFloat(2);
 t1val[0]=2000 ;                 /* region 0 */
 t1val[1]=400;                   /* region 1 */
 t1=GetParamFromDataFile(filelabel,t1val);
 IdVectFree(t1val);

 t2val=AllocInitVectFloat(2);
 t2val[0]=200 ;                 /* region 0 */
 t2val[1]=50;                   /* region 1 */
 t2=GetParamFromDataFile(filelabel,t2val);
 IdVectFree(t2val);

 /* remplissage avec du ki */
 kival=AllocInitVectFloat(2);
 kival[0]=-9.05e-6;			/* region 0 */
 kival[1]=0;	            /* region 1 */
 ki=GetParamFromDataFile(filelabel,kival);
 IdVectFree(kival);

 k=0;
 z=dimx/2; /* Chosen slice */ 
 for (i=0;i<dimx;i++) for(j=0;j<dimy;j++)
  SetObjectPointSusceptibility(objet,i,j,k,(int)ro[z][j][i],(int)t1[z][j][i],(int)t2[z][j][i],(int)ki[z][j][i]);

 
/* Reading the field default values in a file 
 * This file has been obtained with one of the 
 * suceptibility default computation method */
 dech = (PPPVOLUME_FLOAT)IdAcrReadFile(filedech,VOL_FLOAT);
 if (!dech)
	{
	 printf("Unable to read the field default values %s \n",filedech);
	 exit(1);
	}
 SetObjectFieldDefault2D(objet,z,dech,dimx,dimy);

 IdVolFree(ro);
 IdVolFree(t1);
 IdVolFree(t2);
 IdVolFree(ki);
 IdVolFree(dech);
 return(objet);
}

/*!
*************************************************
* \ingroup			Object
* \brief			Create an object which is a 1D homogeneous segment in ro t1 t2 ki (part of a sphere)
* \warning The filemane containing the region label and the field default values linked 
* to the susceptibility difference are hard written in the code :"../data/hightresolution.vol" and
* "../data/Defchamps.vol"
* \param dimx       Number of object pixels along x
* \return			Pointeur on the created object**************************************************/
OBJECT3D * CreateObject1DSectionKi(int dimx)
{
 OBJECT3D * objet;
 int       nbcomp,nbparam;
 int       i,j,k; 
 int       y,z;   
 PPPVOLUME_FLOAT ro,t1,t2,ki; 
 PPPVOLUME_FLOAT dech;
 PVECTOR_FLOAT roval,t1val,t2val,kival;
 char   filelabel[80],filedech[80];
 filelabel[0]='\0';
 filedech[0]='\0';

 strcpy(filelabel,"../data/hightresolution.vol");
 strcpy(filedech,"../data/Defchamps.vol");

 nbcomp = 1; 
 nbparam= 4;  /* ro, t1, t2 ,db */

 objet = AllocObject(dimx,1,1,nbcomp,nbparam);
 
 roval=AllocInitVectFloat(2);
 roval[0]=0;                    /* region 0 */
 roval[1]=100;                  /* region 1 */
 ro=GetParamFromDataFile(filelabel,roval);
 IdVectFree(roval);
 
 t1val=AllocInitVectFloat(2);
 t1val[0]=400 ;                 /* region 0 */
 t1val[1]=2000;                 /* region 1 */
 t1=GetParamFromDataFile(filelabel,t1val);
 IdVectFree(t1val);

 t2val=AllocInitVectFloat(2);
 t2val[0]=50 ;                  /* region 0 */
 t2val[1]=200;                  /* region 1 */
 t2=GetParamFromDataFile(filelabel,t2val);
 IdVectFree(t2val);

 kival=AllocInitVectFloat(2);
 kival[0]=-9.05e-6;		 /* region 0 */
 kival[1]=0;             /* region 1 */
 ki=GetParamFromDataFile(filelabel,kival);
 IdVectFree(kival);

 y=dimx/2; /* chosen line */
 z=dimx/2; /* chosen slice */ 
 
 j=0;
 k=0;
 for (i=0;i<dimx;i++) 
 {
  SetObjectPointSusceptibility(objet,i,j,k,(int)ro[z][y][i],(int)t1[z][y][i],(int)t2[z][y][i],(int)ki[z][y][i]);
 }

 /* Reading the field default values in a file 
 * This file has been obtained with one of the 
 * suceptibility default computation method */
 if (strcmp(filedech,"NULL")) 
	{   
	 dech = (PPPVOLUME_FLOAT) IdAcrReadFile(filedech,VOL_FLOAT);
 	 SetObjectFieldDefault1D(objet,y,z,dech,dimx);
	}
	else
	{
	 printf("Unable to read the field default values %s \n",filedech);
	 exit(1);
	}
 IdVolFree(ro);
 IdVolFree(t1);
 IdVolFree(t2);
 IdVolFree(ki);
 IdVolFree(dech);
 return(objet);
}

/*!
*************************************************
* \ingroup			Object
* \brief			Create an object with two regions homogeneous in ro, t1, t2, ki values
* \warning
* \param dimx       Number of object pixels along x
* \param dimy       Number of object pixels along y
* \param dimz       Number of object pixels along z 
* \return			Pointeur on the created object
**************************************************/

OBJECT3D * CreateObject2DKi(float RoExt, float RoInt, float T1Ext,float T1Int, float T2Ext, float T2Int, float KiExt, float KiInt, int dimx, int dimy, char *filelabel, char *filedech)
{
 OBJECT3D * objet;
 int       nbcomp,nbparam;
 int       i,j,k;
 int       z;   
 PPPVOLUME_FLOAT ro,t1,t2,ki,dech;  
 PVECTOR_FLOAT roval,t1val,t2val,kival;
      
 nbcomp = 1;  
 nbparam= 4;  /* ro, t1, t2 ,db */
 
 objet = AllocObject(dimx,dimy,1,nbcomp,nbparam);

 roval=AllocInitVectFloat(2);
 roval[0]=RoExt;                  /* region 0 */
 roval[1]=RoInt;                  /* region 1 */
 ro=GetParamFromDataFile(filelabel,roval);
 IdVectFree(roval);

 t1val=AllocInitVectFloat(2);
 t1val[0]=T1Ext;                  /* region 0 */
 t1val[1]=T1Int;                  /* region 1 */
 t1=GetParamFromDataFile(filelabel,t1val);
 IdVectFree(t1val);

 t2val=AllocInitVectFloat(2);
 t2val[0]=T2Ext ;                  /* region 0 */
 t2val[1]=T2Int;                   /* region 1 */
 t2=GetParamFromDataFile(filelabel,t2val);
 IdVectFree(t2val);

 kival=AllocInitVectFloat(2);
 kival[0]=KiExt;				   /* region 0 */ 
 kival[1]=KiInt;                   /* region 1 */
 ki=GetParamFromDataFile(filelabel,kival);
 IdVectFree(kival);

 k=0;
 z=dimx/2; /*Slice selection*/
 for (i=0;i<dimx;i++) for(j=0;j<dimy;j++)
 {	   
  SetObjectPointSusceptibility(objet,i,j,k,(int)ro[z][j][i],(int)t1[z][j][i],(int)t2[z][j][i],(int)ki[z][j][i]);
 }

 /* Reading the field default values in a file 
 * This file has been obtained with one of the 
 * suceptibility default computation method */
 dech = (PPPVOLUME_FLOAT)IdAcrReadFile(filedech,VOL_FLOAT);
 if (!dech)
	{
	 printf("Unable to read the field default values %s \n",filedech);
	 exit(1);
	}
 SetObjectFieldDefault2D(objet,z,dech,dimx,dimy);

 IdVolFree(ro);
 IdVolFree(t1);
 IdVolFree(t2);
 IdVolFree(ki);
 IdVolFree(dech);
 return(objet);
}

