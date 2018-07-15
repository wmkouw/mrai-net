/*************************************************************************
* $Id: experience.c,v 1.59 2006/12/18 15:10:37 yougz Exp $
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

#include "experience.h"
#ifdef HAVE_MPI
#include <mpi.h>
#endif

/*!
*************************************************************
* \ingroup Experience
* \brief Allocate a pointer on an EXPERIENCE3D structure
* \return  Pointer on the EXPERIENCE3D structure
**************************************************************/
EXPERIENCE3D * AllocExperience()
{
	EXPERIENCE3D * expr;


	if ((expr = (EXPERIENCE3D *) malloc(sizeof(EXPERIENCE3D))) == NULL)
	{
	fprintf(stderr, "Experience alloc error \n");
	exit(1);
    }

	/* Default setting of the experience structure */
	expr->name[0]='\0';
	expr->nfovx=0; expr->nfovy=0; expr->nfovz=0;
	expr->fovx=0;  expr->fovy=0;  expr->fovz=0;
	expr->obx=0;   expr->oby=0;   expr->obz=0;

	expr->ro=NULL;
	expr->t1=NULL;
	expr->t2=NULL;
	expr->sgn=NULL;
	expr->magstate=NULL;

	expr->xf=0.0; expr->yf=0.0; expr->zf=0.0;
	expr->px=0.0; expr->py=0.0; expr->pz=0.0; expr->pt=0.0;

	expr->G_deche=NULL;
	expr->deltaB0=NULL;
	expr->G_edw=0.0;
	
	expr->to=0.0;

	expr->B0=1.0;	/* Default value set to 1 tesla */
	expr->b0def=0.0;/* Default value set to 0 */

	expr->FLAG_RESONANCE = ON ; /* Pulse are consider on-resonance */
	expr->FLAG_DECH = ON ; 
	return(expr);
}


/*!
*************************************************************
* \ingroup Experience
* \brief Free the EXPERIENCE3D structure pointer
* \param expr  Pointer on the EXPERIENCE3D structure
**************************************************************/
void FreeExperience(EXPERIENCE3D *expr)
{
    int i,j;

	for (i = 0; i < 3; i++) {
	    free(expr->sgn[i][0][0]);
	    for (j = 0; j < expr->ntx; j++)
		free(expr->sgn[i][j]);
	    free(expr->sgn[i]);
	}
	free(expr->sgn);

	for (i = 0; i < 3; i++) {
	    free(expr->magstate[i][0][0]);
	    for (j = 0; j < expr->nfovx; j++)
		free(expr->magstate[i][j]);
	    free(expr->magstate[i]);
	}
	free(expr->magstate);

	free(expr->t1[0][0]);
	free(expr->t2[0][0]);
	free(expr->ro[0][0]);
	free(expr->G_deche[0][0]);
	free(expr->deltaB0[0][0]);
	
	for (i = 0; i < expr->nfovx; i++) {
	    free(expr->t1[i]);
	    free(expr->t2[i]);
	    free(expr->ro[i]);
	    free(expr->G_deche[i]);
	    free(expr->deltaB0[i]);
	}

	free(expr->t1);
	free(expr->t2);
	free(expr->ro);
	free(expr->G_deche);
	free(expr->deltaB0);
	
	free(expr);
}

/*!
*************************************************************
* \ingroup Experience
* \brief Free the RF signal table of the EXPERIENCE3D structure
* \param expr  Pointer on the EXPERIENCE3D structure
**************************************************************/
void FreeExperienceSgn(EXPERIENCE3D *expr)
{
 int i;
 
 for (i = 0; i < 3; i++)  free(expr->sgn[i]);
 free(expr->sgn);
 free(expr);
}

/*!
*************************************************************
* \ingroup Experience
* \brief Zero setting of the 'To' value for the T2* effect
* \param expr  Pointer on the EXPERIENCE3D structure
**************************************************************/
void ResetToExperience(EXPERIENCE3D *expr)
{
 expr->to=0.0;
}

/*!
*************************************************************
* \ingroup Experience
* \brief 'To' table creation for the T2* management
* \param expr  Pointer on the EXPERIENCE3D structure
**************************************************************/
void InitToExperience(EXPERIENCE3D *expr)
{
 expr->to=0.0;
}

/*!
*************************************************************
* \ingroup Experience
* \brief Setting of the RESONANCE FLAG that affects the taking 
* into account of the frequency shift during pulse application
* \param expr  Pointer on the EXPERIENCE3D structure
* \param Flag  Resonance flag value (1 > ON-RESONANCE, 0 > OFF-RESONANCE)
**************************************************************/
void SetResonanceExperience(EXPERIENCE3D *expr, int flag)
{
 expr->FLAG_RESONANCE=flag;
}

/*!
*************************************************************
* \ingroup Experience
* \brief Setting of the FLAG DECH that affects the taking 
* into account of the field inhomogeneity
* \param expr  Pointer on the EXPERIENCE3D structure
* \param Flag  Flag value (1 > active, 0 > inactive)
**************************************************************/
void SetFlagdechExperience(EXPERIENCE3D *expr, int flag)
{
 expr->FLAG_DECH=flag;
}

/*!
*************************************************************
* \ingroup Experience
* \brief T1 table creation
* \param expr  Pointer on the EXPERIENCE3D structure
* \param object Pointer on the OBJECT3D structure
* \param component Number of the considered object component
**************************************************************/
void SetT1Experience(EXPERIENCE3D *expr, OBJECT3D *object, int component)
{
	int i,j,k;
	int l;						/* Beginning of the physical values (ro, T1, T2) of the object component*/ 
    l=component*object->nbparam; /* T1 is stored at l+1 position of the object structure */
    /******************************************
	 * T1 table has the same size than the object 
     ******************************************/
	/* Allocation */
	if ((expr->t1 = (float ***) malloc(sizeof(expr->t1[0]) * expr->nfovx))
	== NULL) {
	fprintf(stderr, "Malloc error in T1 table \n");
	exit(1);
    }

    for (i = 0; i < expr->nfovx; i++) {
	if (
	    (expr->t1[i] =
	     (float **) malloc(sizeof(expr->t1[0][0]) * expr->nfovy)) ==
	    NULL) {
	    fprintf(stderr, "Malloc error in T1 table \n");
	    exit(1);
	}
    }
    if (
	(expr->t1[0][0] =
	 (float *) malloc(sizeof(expr->t1[0][0][0]) * expr->nfovx *
			   expr->nfovy * expr->nfovz)) == NULL) {
	fprintf(stderr, "Malloc error in T1 table \n");
	exit(1);
    }
    for (i = 0; i < expr->nfovx; i++)
	for (j = 0; j < expr->nfovy; j++)
	    expr->t1[i][j] =
		expr->t1[0][0] + j * expr->nfovz +
		i * expr->nfovy * expr->nfovz;
	/* Filling */
	for(i=0;i<expr->nfovx;i++) for(j=0;j<expr->nfovy;j++) for(k=0;k<expr->nfovz;k++)
		expr->t1[i][j][k]= object->values[1+l][k][j][i]/(float)1000.0; /* convertion into second */
}

/*!
*************************************************************
* \ingroup Experience
* \brief T2 table creation
* \param expr  Pointer on the EXPERIENCE3D structure
* \param object Pointer on the OBJECT3D structure
* \param component Number of the considered object component
**************************************************************/
void SetT2Experience(EXPERIENCE3D *expr, OBJECT3D *object, int component)
{
	int i,j,k;
	int l;           /* Beginning of the physical values (ro, T1, T2) of the object component*/ 
    l=component*object->nbparam; /* T2 is stored at l+2 position of the object structure*/

	/******************************************
	 * T2 table has the same size than the object *
     ******************************************/
    /* Allocation */
	if ((expr->t2 = (float ***) malloc(sizeof(expr->t2[0]) * expr->nfovx))
	== NULL) {
	fprintf(stderr, "Malloc error in table T2\n");
	exit(1);
    }

    for (i = 0; i < expr->nfovx; i++) {
	if (
	    (expr->t2[i] =
	     (float **) malloc(sizeof(expr->t2[0][0]) * expr->nfovy)) ==
	    NULL) {
	    fprintf(stderr, "Malloc error in table T2 \n");
	    exit(1);
	}
    }
    if (
	(expr->t2[0][0] =
	 (float *) malloc(sizeof(expr->t2[0][0][0]) * expr->nfovx *
			   expr->nfovy * expr->nfovz)) == NULL) {
	fprintf(stderr, "Malloc error in table T2 \n");
	exit(1);
    }
    for (i = 0; i < expr->nfovx; i++)
	for (j = 0; j < expr->nfovy; j++)
	    expr->t2[i][j] =
		expr->t2[0][0] + j * expr->nfovz +
		i * expr->nfovy * expr->nfovz;
	/* Filling */
	for(i=0;i<expr->nfovx;i++) for(j=0;j<expr->nfovy;j++) for(k=0;k<expr->nfovz;k++)
		expr->t2[i][j][k]=object->values[2+l][k][j][i]/(float)1000.0; /* Second conversion */
}

/*!
*************************************************************
* \ingroup Experience
* \brief Ro table creation
* \param expr  Pointer on the EXPERIENCE3D structure
* \param object Pointer on the OBJECT3D structure
* \param component Number of the considered object component
**************************************************************/
void SetRoExperience(EXPERIENCE3D *expr, OBJECT3D *object, int component)
{
	int i,j,k;
	int l;                /* Beginning of the physical values (ro, T1, T2) of the object component*/ 
    l=component*object->nbparam;/* Ro is stored at the l position of the object structure*/

	/******************************************
	 *  Ro table has the same size than the object *
     ******************************************/
	/* Allocation */
	if ((expr->ro = (float ***) malloc(sizeof(expr->ro[0]) * expr->nfovx))
	== NULL) {
	fprintf(stderr, "Malloc error in table Ro\n");
	exit(1);
    }

    for (i = 0; i < expr->nfovx; i++) {
	if (
	    (expr->ro[i] =
	     (float **) malloc(sizeof(expr->ro[0][0]) * expr->nfovy)) ==
	    NULL) {
	    fprintf(stderr, "Malloc error in table Ro\n");
	    exit(1);
	}
    }
    if (
	(expr->ro[0][0] =
	 (float *) malloc(sizeof(expr->ro[0][0][0]) * expr->nfovx *
			   expr->nfovy * expr->nfovz)) == NULL) {
	fprintf(stderr, "Malloc error in table Ro\n");
	exit(1);
    }
    for (i = 0; i < expr->nfovx; i++)
	for (j = 0; j < expr->nfovy; j++)
	    expr->ro[i][j] =
		expr->ro[0][0] + j * expr->nfovz +
		i * expr->nfovy * expr->nfovz;
	/* Filling */
	for(i=0;i<expr->nfovx;i++) for(j=0;j<expr->nfovy;j++) for(k=0;k<expr->nfovz;k++)
		expr->ro[i][j][k]=object->values[0+l][k][j][i];
}

/*!
*************************************************************
* \ingroup Experience
* \brief Magnetization state table initialization
* \param expr  Pointer on the EXPERIENCE3D structure
**************************************************************/
void InitMagstateExperience(EXPERIENCE3D *expr)
{
 int i,j,k,v;
    /* Allocation */
	if ((expr->magstate=(double ****)malloc(sizeof(expr->magstate[0])*3)) ==
	NULL) {
	fprintf(stderr, "Malloc error magnetization table\n");
	exit(1);
    }

    for (v = 0; v < 3; v++) {
	if (
	    (expr->magstate[v] =
	     (double ***) malloc(sizeof(expr->magstate[0][0]) *
				 expr->nfovx)) == NULL) {
	    fprintf(stderr, "Malloc error magnetization table\n");
	    exit(1);
	}

	for (i = 0; i < expr->nfovx; i++) {
	    if (
		(expr->magstate[v][i] =
		 (double **) malloc(sizeof(expr->magstate[0][0][0]) *
				    expr->nfovy)) == NULL) {
		fprintf(stderr, "Malloc error magnetization table\n");
		exit(1);
	    }
	}
	if (
	    (expr->magstate[v][0][0] =
	     (double *) malloc(sizeof(expr->magstate[0][0][0][0]) *
			       expr->nfovx * expr->nfovy * expr->nfovz)) ==
	    NULL) {
	    fprintf(stderr, "Malloc error magnetization table\n");
	    exit(1);
	}
	for (i = 0; i < expr->nfovx; i++)
	    for (j = 0; j < expr->nfovy; j++)
		expr->magstate[v][i][j] =
		    expr->magstate[v][0][0] + j * expr->nfovz +
		    i * expr->nfovy * expr->nfovz;
    }
	/* Init */
	for(v=0;v<3;v++) 
		for(i=0;i<expr->nfovx;i++) for(j=0;j<expr->nfovy;j++) for(k=0;k<expr->nfovz;k++)
		expr->magstate[v][i][j][k]=0.0;
}

/*!
*************************************************************
* \ingroup Experience
* \brief Static field definition
* \param expr  Pointer on the EXPERIENCE3D structure
* \param B0  Main static field value
**************************************************************/
void SetB0Experience(EXPERIENCE3D *expr,double B0)
{
 expr->B0 = B0;
}

/*!
*************************************************************
* \ingroup Experience
* \brief Static field default definition
* \param expr  Pointer on the EXPERIENCE3D structure
* \param valmax  Maximum value of the parabolic static field default (centered on the object)
**************************************************************/
void SetB0DefExperience(EXPERIENCE3D *expr,double valmax)
{
 expr->b0def=valmax;
}

/*!
*************************************************************
* \ingroup Experience
* \brief Creation of the field inhomogeneity table
* It is scaled by B0 and will localy modify the resonance frequency of the spin
* \param expr  Pointer on the EXPERIENCE3D structure
* \param object Pointer on the OBJECT3D structure
**************************************************************/
void SetGdechExperience(EXPERIENCE3D *expr,OBJECT3D *object)
{
 int i,j,k;
 int x,y,z;
 double decx,decy,decz;
 float def;
 x= expr->nfovx;
 y= expr->nfovy;
 z= expr->nfovz;

    /* Allocation */
    if ((expr->G_deche = (float ***) malloc(sizeof(expr->G_deche[0]) * x)) == NULL) {
	fprintf(stderr, "Malloc error magstate table\n");
	exit(1);
    }

    for (i = 0; i < x; i++) {
	if ((expr->G_deche[i] = (float **) malloc(sizeof(expr->G_deche[0][0]) * y)) == NULL) {
	    fprintf(stderr, "Malloc error magstate table \n");
	    exit(1);
	}
    }
    if ((expr->G_deche[0][0] = (float *) malloc(sizeof(expr->G_deche[0][0][0]) * x * y * z)) == NULL) {
	fprintf(stderr, "Malloc error G_deche table \n");
	exit(1);
    }
    for (i = 0; i < x; i++)	for (j = 0; j < y; j++)
	    expr->G_deche[i][j] = expr->G_deche[0][0] + j * z +	i * y * z;

    decx= expr->nfovx/2.;
	decy= expr->nfovy/2.;
	decz= expr->nfovz/2.;

	
	switch (expr->FLAG_DECH)
	{
	case 0: /* No default accounting */
		{
		for (i = 0; i < expr->nfovx; i++)
		    for (j = 0; j < expr->nfovy; j++)
				for (k = 0; k < expr->nfovz; k++)
					expr->G_deche[i][j][k] = 0.0;
		break;
		}
	case 1: /* Accounting the default linked only to the object */
		{
		 for (i = 0; i < expr->nfovx; i++)
		    for (j = 0; j < expr->nfovy; j++)
				for (k = 0; k < expr->nfovz; k++)
					expr->G_deche[i][j][k] =  ((float)expr->B0)* object->fielddefault[k][j][i];
		 break; 
		}
	case 2: /* Accounting the default linked only to the main magnet B0 with a parabolic form */
		{
		for (i = 0; i < expr->nfovx; i++)
			for (j = 0; j < expr->nfovy; j++)
				for (k = 0; k < expr->nfovz; k++)
				 expr->G_deche[i][j][k] = ((float)expr->B0)*((i-decx)*(i-decx)+(j-decy)*(j-decy)+(k-decz)*(k-decz))*expr->b0def/(decx*decx+decy*decy+decz*decz);
		 break; 
		}
	case 3: /* Accounting both effects : case 1 + 2 */
		{
		 for (i = 0; i < expr->nfovx; i++)
		    for (j = 0; j < expr->nfovy; j++)
				for (k = 0; k < expr->nfovz; k++)
				{
					def =((float)expr->B0)*((i-decx)*(i-decx)+(j-decy)*(j-decy)+(k-decz)*(k-decz))*expr->b0def/(decx*decx+decy*decy+decz*decz);
					expr->G_deche[i][j][k] = def + ((float)expr->B0)* object->fielddefault[k][j][i];
				}
		 break; 
		}
	default: /* No accounting */
		{
		 for (i = 0; i < expr->nfovx; i++)
		    for (j = 0; j < expr->nfovy; j++)
				for (k = 0; k < expr->nfovz; k++)
					expr->G_deche[i][j][k] = 0.0;
		 break; 
		}
	}
	
}

/*!
*************************************************************
* \ingroup Experience
* \brief Creation of the complex B1-field inhomogeneity tables (both transmit and receive B1 tables) 
*  B1 should be normalized (max module = 1)
* \param expr  Pointer on the EXPERIENCE3D structure
**************************************************************/
void SetB1(EXPERIENCE3D *expr)
{
//  ASCII file B1t.txt must contain a complex 3D map of the normalized transmit field (max module = 1).
//  ASCII file B1r.txt must contain a complex 3D map of the normalized receive field. 
//  Each line of these files contains 2 floats representing the real and imaginary parts of the B1 value.
//  The order of the lines is as follows (x, y and z vary from 0 to sizeX, sizeY, sizeZ, e.g. 64 x 64 x 40):
// 
//		1st line corresponds to pixel (x=0, y=0, z=0) in volume corner  
//		2nd line                      (x=1, y=0, z=0)
//              ...
//              64-th line                    (x=63, y=0, z=0)
//              65-th line                    (x=0, y=1, z=0)
//              66-th line                    (x=1, y=1, z=0)
//              ...
//              4096-th line                  (x=63, y=63, z=0)
//              4097-th line                  (x=0, y=0, z=1) 
//               ...                     
//              last line                     (x=63, y=63, z=39) in opposite volume corner
//
//  If ever the object to be imaged has a cylindrical symmetry, B1r does not need to be provided in addition to B1t: 
//  a mirror symmetry is then used to find B1r from B1t, i.e.: Re(B1r(x,y)) = Im(B1t(y,x)) and Im(B1r(x,y)) = Re(B1t(y,x))
  char B1t_File[80] = "../data/B1map/B1t.txt";  // transmit field
  char B1r_File[80] = "../data/B1map/B1r.txt";  // reception field
  FILE *b1t_file, *b1r_file;
  int i,j,k;
  int x,y,z;
  x = expr->ntx;
  y = expr->nty;
  z = expr->ntz;
 
  printf("x = %i, y = %i, z = %i\n", x, y, z);

  b1t_file = fopen(B1t_File, "r");
  if (b1t_file == NULL) {
     printf("Unable to read the B1t(r) file %s \n", B1t_File);
     exit(0);
  }  
  b1r_file = fopen(B1r_File, "r");

 /* B1 Allocation*/

  expr->B1t = (PPPVOLUME_COMPLEX_FLOAT) IdVolAlloc(x,y,z,VOL_COMPLEX_FLOAT);
  if (!expr->B1t)
  {
     printf("Impossible d'allouer B1t(r) ! \n");
     exit(0);
  }
  expr->B1r = (PPPVOLUME_COMPLEX_FLOAT) IdVolAlloc(x,y,z,VOL_COMPLEX_FLOAT);
  if (!expr->B1r)
  {
     printf("Impossible d'allouer B1r(r) ! \n");
     exit(0);
  }

// Set B1(r) to default values
  
  printf("B1 File reading %s\n", B1t_File);
  for (k = 0; k < z; k++) for (j = 0; j < y; j++) for (i = 0; i < x; i++){
    fscanf(b1t_file, "%f %f", &expr->B1t[k][j][i].re, &expr->B1t[k][j][i].im);
  } 
  fclose(b1t_file);

  if (b1r_file == NULL) {
     printf("Unable to read the B1r(r) file %s \n", B1r_File);
     printf("Assume cylindrical symmetry of the phantom/subject, i.e.: Re(B1r(x,y)) = Im(B1t(y,x)) and Im(B1r(x,y)) = Re(B1t(y,x))\n");
     for (k = 0; k < z; k++) for (j = 0; j < y; j++) for (i = 0; i < x; i++){
       expr->B1r[k][i][j].re = expr->B1t[k][j][i].im;
       expr->B1r[k][i][j].im = expr->B1t[k][j][i].re;
     }
  }
  else {  
     printf("Lecture du fichier %s\n", B1r_File);
     for (k = 0; k < z; k++) for (j = 0; j < y; j++) for (i = 0; i < x; i++){
        fscanf(b1r_file, "%f %f", &expr->B1r[k][j][i].re, &expr->B1r[k][j][i].im);
     }
     fclose(b1r_file);
  } 
}
/*!
*************************************************************
* \ingroup Experience
* \brief Creation of the DeltaB0 table that allows T2* weigthing
* DeltaB0 is given in Tesla and weigthed by B0
* \param expr  Pointer on the EXPERIENCE3D structure
* \param object Pointer on the OBJECT3D structure
**************************************************************/
void SetDeltaB0Experience(EXPERIENCE3D *expr,OBJECT3D *object)
{
 int i,j,k;
 int x,y,z;

 x= expr->nfovx;
 y= expr->nfovy;
 z= expr->nfovz;

 /* Allocation */
 if (
	(expr->deltaB0 =
	 (float ***) malloc(sizeof(expr->deltaB0[0]) * x)) ==
	NULL) 
	{
	 fprintf(stderr, "Malloc error of deltaB0 table \n");
	 exit(1);
	}
    for (i = 0; i < x; i++) 
	{
	 if ((expr->deltaB0[i] =
	     (float **)malloc(sizeof(expr->deltaB0[0][0])*y)) == NULL) 
		{
	     fprintf(stderr, "Malloc error of deltaB0 table\n");
	     exit(1);
		}
    }
    
	if ((expr->deltaB0[0][0] =
	   (float *) malloc(sizeof(expr->deltaB0[0][0][0])*x*y*z)) == NULL) 
	{
	 fprintf(stderr, "Malloc error of deltaB0 table\n");
	 exit(1);
    }
    for (i=0;i<x;i++) for (j=0;j<y;j++)
	    expr->deltaB0[i][j] = expr->deltaB0[0][0] + j * z + i * y * z;
	for (i=0;i<x;i++) for(j=0;j<y;j++) for(k=0;k<z;k++) 
	  expr->deltaB0[i][j][k] = ((float)expr->B0) * object->deltaB0[k][j][i];
}

/*!
*************************************************************
* \ingroup Experience
* \brief Copy the (Ro,T1,T2) values of one object componentinto the EXPERIENCE3D structure
* \warning     
* \param expr  Pointer on the EXPERIENCE3D structure
* \param object Pointer on the OBJECT3D structure
* \param component Component number, for instance (0=WATER  1=FAT)
* \param FLAG_DECH Accounting flag for field default (0=NO  1=YES object 2=YES B0 3=1+2) 
* \param B0 Value in Tesla of the main field B0, Impact the field default value
**************************************************************/
void SetObjectExperienceComponent(EXPERIENCE3D *expr, OBJECT3D *object,int component,int FLAG_DECH)
{
	if ((object->nbcomponent>2)||(object->nbparam>4))
	{
		printf("This kind of object is not manageable at the moment!\n");
		exit(0);
	}
	/* Chemical shift in rad/s */

      if (component==0)  
        expr->G_edw = object->chemshift[WATER] * 2. * M_PI ; 
      else if (component==1)  
        expr->G_edw = object->chemshift[FAT] * 2. * M_PI;
    else
	{
		printf("Component not defined !\n");
		exit(1);
	}

	expr->FLAG_DECH = FLAG_DECH ; /* Field default accounting */
	
	/* Fov and size */ 
	/* By default, the fov equals the object size */
	expr->lx = object->lx; 
	expr->ly = object->ly;
	expr->lz = object->lz;	
	expr->fovx = object->lx; 
	expr->fovy = object->ly;
	expr->fovz = object->lz;	
    
	/* Number of object point */
	expr->nfovx=object->x;
	expr->nfovy=object->y;
	expr->nfovz=object->z;

	/* The step px,py,pz (used by the computation kernel to compute dw) are 
	 computed using the object size */
    expr->px = expr->nfovx == 1 ? 1e-12 : expr->lx / (expr->nfovx - 1);
    expr->py = expr->nfovy == 1 ? 1e-12 : expr->ly / (expr->nfovy - 1);
    expr->pz = expr->nfovz == 1 ? 1e-12 : expr->lz / (expr->nfovz - 1);
	expr->xf = expr->lx / 2.;	/* xf=object center */
    expr->yf = expr->ly / 2.;
    expr->zf = expr->lz / 2.;
	

	InitToExperience(expr);
	SetT1Experience(expr,object,component);
	SetT2Experience(expr,object,component);
	SetRoExperience(expr,object,component);
	InitMagstateExperience(expr);
	SetDeltaB0Experience(expr,object);
	SetGdechExperience(expr,object);
}


/*!
*************************************************************
* \ingroup Experience
* \brief Give a name to a given EXPERIENCE3D structure
* \param expr Pointer on the EXPERIENCE3D structure
* \param name EXPERIENCE3D name
**************************************************************/
void SetNameExperience(EXPERIENCE3D *expr, char *name)
{
	strcpy(expr->name,name);
}

/*!
*************************************************************
* \ingroup Experience
* \brief Creation of the RF signal table
* \param expr  Pointer on the EXPERIENCE3D structure
**************************************************************/
void InitRFSigExperience(EXPERIENCE3D *expr)
{
	int i,j,k,v;
    /********************************
	* The RF table has the same size than the output RF volume
	********************************/
	/* Allocation */
    if ((expr->sgn = (float ****) malloc(sizeof(float ***) * 3)) == NULL) {
	fprintf(stderr, "Malloc error of the RF signal table\n");
	exit(1);
    }

    for (v = 0; v < 3; v++) {
	if (
	    (expr->sgn[v] =
	     (float ***) malloc(sizeof(float **) * expr->ntx)) == NULL) {
	    fprintf(stderr, "Malloc error of the RF signal table \n");
	    exit(1);
	}

	for (i = 0; i < expr->ntx; i++) {
	    if (
		(expr->sgn[v][i] =
		 (float **) malloc(sizeof(float *) * expr->nty)) == NULL) {
		fprintf(stderr, "Malloc error of the RF signal table \n");
		exit(1);
	    }
	}
	if (
	    (expr->sgn[v][0][0] =
	     (float *) malloc(sizeof(float) * expr->ntx * expr->nty *
			      expr->ntz)) == NULL) {
	    fprintf(stderr, "Malloc error of the RF signal table\n");
	    exit(1);
	}
	for (i = 0; i < expr->ntx; i++)
	    for (j = 0; j < expr->nty; j++)
		expr->sgn[v][i][j] =
		    expr->sgn[v][0][0] + j * expr->ntz +
		    i * expr->nty * expr->ntz;
    }
     /* Setting */
    for (i = 0; i < expr->ntx; i++)
	for (j = 0; j < expr->nty; j++)
	    for (k = 0; k < expr->ntz; k++) {
		expr->sgn[0][i][j][k] = 0;
		expr->sgn[1][i][j][k] = 0;
		expr->sgn[2][i][j][k] = 0;
	    }
}

/*!
*********************************************************
* \ingroup     Experience
* \brief       Define the acquisition parameter of an experience
* \warning     
* \param expr  Pointer on the EXPERIENCE3D  structure 
* \param ntx   x dimension of the k space
* \param nty   y dimension of the k space
* \param ntz   z dimension of the k space
* \param duree Acquisition time of one line of the k space given in seconds
 ***********************************************************/
void SetAcqExperience (EXPERIENCE3D *expr,int ntx, int nty, int ntz, 
					   double tacq)
{
    
    expr->ntx = ntx;
    expr->nty = nty;
    expr->ntz = ntz;
    if (tacq == 0) {
	fprintf(stderr, "Error : acquisition time equals zero\n");
	exit(1);
    }
	else
		expr->readout_time = tacq;

    expr->pt = expr->readout_time / (expr->ntx - 1);
	InitRFSigExperience(expr);
}


/*!
*************************************************************
* \ingroup Experience
* \brief  RF signal part allocation
* \param ntx   x dimension of the k space
* \param nty   y dimension of the k space
* \param ntz   z dimension of the k space
* \return Pointer on the EXPERIENCE3D  structure
**************************************************************/
EXPERIENCE3D * AllocExperienceSgn(int ntx,int nty,int ntz)
{
   EXPERIENCE3D  * expr;

   expr = AllocExperience();
   expr->ntx = ntx;		
   expr->nty = nty;
   expr->ntz = ntz;

   InitRFSigExperience(expr);
   return(expr);
}

/*!
*************************************************************
* \ingroup Experience
* \brief Definition of the EXPERIENCE3D FOV
* \param expr Pointer on the EXPERIENCE3D  structure
* \param fovx Fov dimension in meter for x  
* \param fovy Fov dimension in meter for y  
* \param fovz Fov dimension in meter for z  
* \param obx  Excentering of the object in meter for x
* \param oby  Excentering of the object in meter for y
* \param obz  Excentering of the object in meter for z
**************************************************************/
void SetFovExperience (EXPERIENCE3D *expr, double fovx,
					   double fovy,double fovz,
					   double obx,double oby,
					   double obz)
{
    expr->fovx = fovx;
    expr->fovy = fovy;
    expr->fovz = fovz;
    expr->obx = obx;
    expr->oby = oby;
    expr->obz = obz;
}

/*!
*************************************************************
* \ingroup Experience
* \brief Get the RF volume from the EXPERIENCE3D structure
* \param expr Pointer on the EXPERIENCE3D  structure
* \return Pointer on the RF volume
*************************************************************/
PPPVOLUME_COMPLEX_DOUBLE GetSignalRFComplexFromExperience(EXPERIENCE3D *expr)
{
	PPPVOLUME_COMPLEX_DOUBLE volcomplex;
    int i,j,k;
	int x,y,z;
#ifdef HAVE_MPI
	int myrank;
#endif

	x= expr->ntx;
	y= expr->nty;
	z= expr->ntz;

	volcomplex = (PPPVOLUME_COMPLEX_DOUBLE) IdVolAlloc(x,y,z,VOL_COMPLEX_DOUBLE);
	
	for (k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	{
		volcomplex[k][j][i].re = expr->sgn[1][i][j][k];	/* Y+iX */
		volcomplex[k][j][i].im = expr->sgn[0][i][j][k];
	}
#ifdef HAVE_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	if (myrank == 0) {
	    float avg[2]={0.,0.};
	    float min[2]={0.,0.};
	    float max[2]={0.,0.};
	    float std_dev[2]={0.,0.};
	    float e[2];
	    int l;

	    min[0] = max[0] = expr->sgn[0][0][0][0];
	    min[1] = max[1] = expr->sgn[1][0][0][0];
	    for (i=0;i<x;i++)
	    for (j=0;j<y;j++)
	    for (k=0;k<z;k++)
	    for (l=0;l<2;l++) {
		e[l] = expr->sgn[l][i][j][k];
		avg[l] += e[l];

		if (e[l] > max[l]) max[l]=e[l];
		if (e[l] < min[l]) min[l]=e[l];
	    }
	    avg[0] = avg[0] / (x*y*z);
	    avg[1] = avg[1] / (x*y*z);

	    for (i=0;i<x;i++)
	    for (j=0;j<y;j++)
	    for (k=0;k<z;k++)
	    for (l=0;l<2;l++) {
		e[l] = expr->sgn[l][i][j][k];
		std_dev[l] += (e[l] - avg[l])*(e[l] - avg[l]);
	    }
	    std_dev[0] = sqrt(std_dev[0] / (x*y*z));
	    std_dev[1] = sqrt(std_dev[1] / (x*y*z));

	    printf("summary:\n");
	    printf("im: min=%f, max=%f, avg=%f, std_dev=%f\n",
		min[0], max[0], avg[0], std_dev[0]);
	    printf("re: min=%f, max=%f, avg=%f, std_dev=%f\n",
		min[1], max[1], avg[1], std_dev[1]);
	}
#endif

	return (volcomplex);
}

/*!
*************************************************************
* \ingroup Experience
* \brief RF Signal normalisation taking into account the object discretisation (number of voxels and size)
* \param expr Pointer on the EXPERIENCE3D  structure
*************************************************************/
void NormalizeRFSignal(EXPERIENCE3D *expr)
{
	int i,j,k,n;
	int x,y,z;
	double c;
	
	/* px=lx/x, take into account the size and the number of points */
	c = expr->px * expr->py * expr->pz;	
   	x= expr->ntx; y= expr->nty;	z= expr->ntz;

	for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++) for (n=0;n<3;n++)
		expr->sgn[n][i][j][k] = expr->sgn[n][i][j][k] / (float) c;
}

/*!
*************************************************************
* \ingroup Experience
* \brief Get one one of the RF component obtained after an experience
* \param expr Pointer on the EXPERIENCE3D  structure
* \param int One RF component (0->X, 1->Y, (2->Z,non available in real world)) 
* \return Pointer on the component required
*************************************************************/
PPPVOLUME_FLOAT GetSignalRFComponentFromExperience(EXPERIENCE3D *expr,int comp)
{
	PPPVOLUME_FLOAT vol;
	register int i,j,k;
	int x,y,z;
	
	if ((comp >2) || (comp<0))
	{
		printf("This component does not exist!\n");
		exit(1);
	}
	
	x= expr->ntx; y= expr->nty;	z= expr->ntz;

	vol = (PPPVOLUME_FLOAT) IdVolAlloc(x,y,z,VOL_FLOAT);
    for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
		 vol[k][j][i] = (float) expr->sgn[comp][i][j][k];
		
	return (vol);
}

/*!
*************************************************************
* \ingroup Experience
* \brief Get an image (in unsigned char) of the RF signals (X or Y) stored in the k space. Useful for a quick visualization.
* \param expr Pointer on the EXPERIENCE3D  structure
* \param haut Number of signals considered
* \param dir Direction considered (0 > X, 1 >Y)
* \return Pointer on the unsigned char image of the k space part
**************************************************************/
PPIMAGE_UCHAR GetkSpaceImage2DFromExperience(EXPERIENCE3D *expr, int haut, int dir)
{
	PPIMAGE_FLOAT imafloat;
	PSIGNAL_FLOAT sigx;
	PPIMAGE_UCHAR imasig;
	PPIMAGE_UCHAR ima;
    int i,j;
	int x,y,z;
	int ligne;

	x= expr->ntx;
	y= expr->nty;
	z= expr->ntz;

	if (z!=1)
	{
		printf("Only available for 2D k space \n");
		exit(1);
	}

	if (!((dir ==0) || (dir==1)))
	{
		printf("The direction parameter is not correct : 0->X, 1->Y \n");
		exit(1);
	}


	imafloat = (PPIMAGE_FLOAT) IdImaAlloc(x,y,IMA_FLOAT);
	for(j=0;j<y;j++) for(i=0;i<x;i++)
		imafloat[j][i] = expr->sgn[dir][i][j][0];/* recuperation of the signals from the antennas 'dir' */
    
	ima = (PPIMAGE_UCHAR) IdImaAlloc(x,y*haut,IMA_UCHAR);
	for(ligne=0;ligne<y;ligne++) 
	{
		sigx = (PSIGNAL_FLOAT) IdSigAlloc(x,SIG_FLOAT);
		for (i=0;i<x;i++) sigx[i] = imafloat[ligne][i];
		imasig = IdImaSignalToImage(sigx,0,x,0,-1,-1,x,haut,255,0);
        for(i=0;i<x;i++) for (j=0;j<haut;j++)
			ima[(ligne*haut)+j][i] = imasig[j][i];
		IdImaFree(imasig);
	}

	IdSigFree(sigx);
	IdImaFree(imafloat);
	
	return (ima);
}


/*!
*************************************************************
* \ingroup Experience
* \brief Get the module of the RF signal
* \param expr Pointer on the EXPERIENCE3D  structure
* \return Pointer on the volume in float
*************************************************************/
PPPVOLUME_FLOAT GetSignalRFModuleFromExperience(EXPERIENCE3D *expr)
{
	PPPVOLUME_COMPLEX_DOUBLE volcomplex;
	PPPVOLUME_FLOAT module;
	register int i,j,k;
	int x,y,z;
	double a,b;

	x= expr->ntx;
	y= expr->nty;
	z= expr->ntz;

	volcomplex = (PPPVOLUME_COMPLEX_DOUBLE) GetSignalRFComplexFromExperience (expr);
	
	module = (PPPVOLUME_FLOAT) IdVolAlloc(x,y,z,VOL_FLOAT);
    for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	{
		a = volcomplex[k][j][i].re;
		b = volcomplex[k][j][i].im;
		module[k][j][i] = (float) sqrt(a*a + b*b);
	}
	
	IdVolFree(volcomplex);
	return (module);
}

/*!
*************************************************************
* \ingroup Experience
* \brief Get the phase of the RF signal
* \param expr Pointer on the EXPERIENCE3D  structure
* \return Pointer on the volume in float
*************************************************************/
PPPVOLUME_FLOAT GetSignalRFPhaseFromExperience(EXPERIENCE3D *expr)
{
	PPPVOLUME_COMPLEX_DOUBLE volcomplex;
	PPPVOLUME_FLOAT phase;
	register int i,j,k;
	int x,y,z;
	double a,b;

	x= expr->ntx;
	y= expr->nty;
	z= expr->ntz;

	volcomplex = (PPPVOLUME_COMPLEX_DOUBLE) GetSignalRFComplexFromExperience (expr);
	
	phase = (PPPVOLUME_FLOAT) IdVolAlloc(x,y,z,VOL_FLOAT);
    for(k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	{
		a = volcomplex[k][j][i].re;
		b = volcomplex[k][j][i].im;
		phase[k][j][i] = (float) atan(b/a);
	}
	
	IdVolFree(volcomplex);
	return (phase);
}

/*!
*************************************************************
* \ingroup Utility
* \brief Do the summation of two complex RF volume and put it in an EXPERIENCE3D structure
* \param volrf_1 Pointer on the first complex RF volume
* \param volrf_2 Pointer on the second complex RF volume
* \return Pointer on the EXPERIENCE3D  structure containing the RF volume summation
************************************************************************************/
EXPERIENCE3D * GetSumSignalRFComplex(PPPVOLUME_COMPLEX_DOUBLE volrf_1,
								  PPPVOLUME_COMPLEX_DOUBLE volrf_2  )
{
	int i,j,k;
	int x,y,z;
    EXPERIENCE3D * expr;
	
    x=IdVolDimX(volrf_1);
    y=IdVolDimY(volrf_1);
    z=IdVolDimZ(volrf_1); 
	
    expr=AllocExperienceSgn(x,y,z);
    
	for (k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	{
	
		expr->sgn[1][i][j][k]= (float)(volrf_1[k][j][i].re + volrf_2[k][j][i].re);  /* Y1+Y2+i(X1+X2) */
        expr->sgn[0][i][j][k]= (float)(volrf_1[k][j][i].im + volrf_2[k][j][i].im);
	}
	expr->ntx=x;
    expr->nty=y;
	expr->ntz=z;

 return (expr);
}

/*!
*************************************************************
* \ingroup Utility
* \brief Do the difference of two complex RF volume and put it in an EXPERIENCE3D structure
* \param volrf_1 Pointer on the first complex RF volume
* \param volrf_2 Pointer on the second complex RF volume
* \return Pointer on the EXPERIENCE3D  structure containing the RF volume difference
**************************************************************/
EXPERIENCE3D * GetDiffSignalRFComplex(PPPVOLUME_COMPLEX_DOUBLE volrf_1,
								  PPPVOLUME_COMPLEX_DOUBLE volrf_2)
{
	int i,j,k;
	int x,y,z;
    EXPERIENCE3D * expr;

    x=IdVolDimX(volrf_1);
    y=IdVolDimY(volrf_1);
    z=IdVolDimZ(volrf_1); 
	
    expr=AllocExperienceSgn(x,y,z);

	for (k=0;k<z;k++) for(j=0;j<y;j++) for(i=0;i<x;i++)
	{
		expr->sgn[1][i][j][k]= (float)(volrf_1[k][j][i].re - volrf_2[k][j][i].re);  /* Y1-Y2+i(X1-X2) */
        expr->sgn[0][i][j][k]= (float)(volrf_1[k][j][i].im - volrf_2[k][j][i].im);
	}

	expr->ntx=x;
    expr->nty=y;
	expr->ntz=z;

	return (expr);
}

/*!
**********************************************************
* \ingroup     Utility
* \brief       Accumulation of two complex volumes
* \param expr	Pointer on the expereience structure to get the B1 information
* \param volacu  Accumulator volume
* \param vol     Volume to accumulate
***********************************************************/
void AddComplexVolRF(EXPERIENCE3D* expr, PPPVOLUME_COMPLEX_DOUBLE volacu, PPPVOLUME_COMPLEX_DOUBLE vol)
{
 int x,y,z;
 int i,j,k;

 if ((!volacu) || (!vol))
  {
	FPRINTF(stderr,"Volumes to add not allocated !!\n");
    exit(0);
  }
 
 x = IdVolDimX(volacu);
 y = IdVolDimY(volacu);
 z = IdVolDimZ(volacu);

 if ((x!=IdVolDimX(vol))||(y!=IdVolDimY(vol))||(z!=IdVolDimZ(vol)))
 {
	FPRINTF(stderr,"Volumes to add have differents sizes !!\n");
    exit(0);
 }

/*******************************************************************************************************/
// Alexis Amadon, January 2006
// Measured signal should integrate magnetization weighted by RF coil sensitivity profile:
// add RF coil complex sensitivity profile B1r-*(r) as a weighting factor of magnetization : signal = integral[M+(r) * B1r-*(r)] ;
// here B1r stands for Reception field (the one found by running 2nd EM simulation with swapped excitation phases on x and y);
// the minus sign stands for the circularly-polarized component that turns opposite to the spins' precession;
// the * sign stands for the complex conjugate
  if (expr->inhomogeneousB1){
    for(i=0;i<x;i++) for (j=0;j<y;j++) for (k=0;k<z;k++)
    {
      volacu[k][j][i].re += expr->B1r[k][j][i].re * vol[k][j][i].re + expr->B1r[k][j][i].im * vol[k][j][i].im;
      volacu[k][j][i].im += expr->B1r[k][j][i].re * vol[k][j][i].im - expr->B1r[k][j][i].im * vol[k][j][i].re;
    }
  }
  else {
    for(i=0;i<x;i++) for (j=0;j<y;j++) for (k=0;k<z;k++)
    {
      volacu[k][j][i].re = volacu[k][j][i].re + vol[k][j][i].re;
      volacu[k][j][i].im = volacu[k][j][i].im + vol[k][j][i].im;
    }
  }
}

