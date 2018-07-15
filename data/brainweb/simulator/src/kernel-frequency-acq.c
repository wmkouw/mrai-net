/*************************************************************************
* $Id: kernel-frequency-acq.c,v 1.6 2006/12/18 15:10:37 yougz Exp $
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

#include "kernel-frequency-acq.h"
#include <math.h>
#ifdef HAVE_MPI
#include <mpi.h>
#include "seq-mpi.h" /* for MPI TAGS definition */
#endif

/*!
**********************************************************
* \ingroup     Kernel 
* \brief       Simulation for the first point acquisition using a frequency encoding (Bloch Equation)
* No precession before the first point caquisition
* \warning     
* \param expr Pointer on the experience structure containing the RF volume
* \param event Pointer on the event structure
 ***********************************************************/
void KernelFrequency1(EXPERIENCE3D * expr, EVENT * event)
{
    int ix, iy, iz;
    float sgn[3];
	double v;
    int xmin;
    int xmax;

#ifdef HAVE_MPI
    xmin= expr->mpi_slice_x0;
    xmax= xmin+mpi_get_slice_width(expr, xmin);
#else
    xmin = 0;
    xmax = expr->nfovx;
#endif

    sgn[0] = 0.;
    sgn[1] = 0.;
    sgn[2] = 0.;

    if (event->op_flag == PLUS)
	{
	 for (ix=xmin;ix<xmax;ix++)
		for(iy=expr->nfovy -1;iy>=0;iy--)
			for(iz=expr->nfovz -1;iz>=0;iz--)
			{ /* Spins summation */
			  /* T2* in the transverse plane xy */
		     v = exp(-fabs(expr->to*G_Hz_T*expr->deltaB0[ix][iy][iz]));
 			 sgn[0] += (float) expr->magstate[0][ix][iy][iz]* (float)v;
		     sgn[1] += (float) expr->magstate[1][ix][iy][iz]* (float)v;
		     sgn[2] += (float) expr->magstate[2][ix][iy][iz];
			}
	}
	else if (event->op_flag == MINUS)
	{
	 for (ix=xmin;ix<xmax;ix++)
		for(iy=expr->nfovy -1;iy>=0;iy--)
			for(iz=expr->nfovz -1;iz>=0;iz--)
			{ /* Spins summation */
			  /* T2* in the transverse plane xy */
		     v = exp(-fabs(expr->to*G_Hz_T*expr->deltaB0[ix][iy][iz]));
		     sgn[0] -= (float) expr->magstate[0][ix][iy][iz]* (float) v;
		     sgn[1] -= (float) expr->magstate[1][ix][iy][iz]* (float) v;
		     sgn[2] -= (float) expr->magstate[2][ix][iy][iz];
			}
	}

/* First k space point saving */
#ifndef OSCILLO
    expr->sgn[0][0][event->nu_pointy][event->nu_pointz] += sgn[0];
    expr->sgn[1][0][event->nu_pointy][event->nu_pointz] += sgn[1];
    expr->sgn[2][0][event->nu_pointy][event->nu_pointz] += sgn[2];
#else
    expr->sgn[0][0][event->nu_pointy][event->nu_pointz] = sgn[0];
    expr->sgn[1][0][event->nu_pointy][event->nu_pointz] = sgn[1];
    expr->sgn[2][0][event->nu_pointy][event->nu_pointz] = sgn[2];
#endif
}

/*!
**********************************************************
* \ingroup     Kernel
* \brief       simulation for a frequency acquisition (Bloch Equation)
* \warning     
* \param expr Pointer on the experience structure containing the RF volume
* \param event Pointer on the event structure
***********************************************************/
void KernelFrequency2(EXPERIENCE3D * expr, EVENT * event)
{
    double v;
    double x, y, z, dw,edw;
    double p6x, p6y, p6z, p9x, p9y, p9z;
    double e1, e2, teta, rmnx, r1, r2;
    double *e0xyz, *e1xyz, *e2xyz;
    int ix, iy, iz, it;
    float *sgn[3];
    int xmin;
    int xmax;

#ifdef HAVE_MPI
    xmin=expr->mpi_slice_x0;
    xmax=xmin+mpi_get_slice_width(expr, xmin);
#else
    xmin = 0;
    xmax = expr->nfovx;
#endif

    p6x = G * event->ax;
    p9x = expr->xf + expr->obx - event->axoff;
    p6y = G * event->ay;
    p9y = expr->yf + expr->oby - event->ayoff;
    p6z = G * event->az;
    p9z = expr->zf + expr->obz - event->azoff;

    /* RF signal allocation */
    sgn[0] = (float *)malloc (expr->ntx * sizeof(float));
    sgn[1] = (float *)malloc (expr->ntx * sizeof(float));
    sgn[2] = (float *)malloc (expr->ntx * sizeof(float));

    /* RF signal point acquisition */
	for (it = 1; it < expr->ntx; it++)
	{
	 sgn[0][it] = 0.;
	 sgn[1][it] = 0.;
	 sgn[2][it] = 0.;
	 /* T2* account */
  	 expr->to = expr->to + expr->pt;

	 for (ix = xmin; ix < xmax; ix++) {
	    x = expr->px * ix - p9x;
	    for (iy = expr->nfovy - 1; iy >= 0; iy--) {
	           y = expr->py * iy - p9y;
		   for (iz = expr->nfovz - 1; iz >= 0; iz--) 
		   { /* Update of the spin states (Bloch) before the it point acquisition */
		    z = expr->pz * iz - p9z;

		    e0xyz = &(expr->magstate[0][ix][iy][iz]);
		    e1xyz = &(expr->magstate[1][ix][iy][iz]);
		    e2xyz = &(expr->magstate[2][ix][iy][iz]);
		   
			/* dw is given in rad/s */
//			dw =(p6x * x)+(p6y * y)+(p6z * z)-(expr->G_edw)+G_rad_T*expr->G_deche[ix][iy][iz];
			edw = expr->G_edw *(expr->B0+1.*(event->ax*x + event->ay*y + event->az*z) + expr->G_deche[ix][iy][iz]);
			dw = G_rad_T*(event->ax*x + event->ay*y + event->az*z + expr->G_deche[ix][iy][iz]) - edw;
			teta = dw * expr->pt;
			
			e1 = exp(-expr->pt / expr->t1[ix][iy][iz]);
		    e2 = exp(-expr->pt / expr->t2[ix][iy][iz]);

		    r1 = e2 * cos(teta);
		    r2 = e2 * sin(teta);

		    rmnx = r1 * e0xyz[0] + r2 * e1xyz[0];
		    e1xyz[0] = -r2 * e0xyz[0] + r1 * e1xyz[0];
		    e2xyz[0] = e1 * e2xyz[0] + expr->ro[ix][iy][iz] * (1 - e1);
		    e0xyz[0] = rmnx;

		    v = exp(-fabs(expr->to*G_Hz_T*expr->deltaB0[ix][iy][iz]));
		    if (event->op_flag == PLUS)
			{ /* Spins summation */
			  /* T2* in the transverse plane xy */
			 sgn[0][it] += (float) e0xyz[0] * (float) v;
			 sgn[1][it] += (float) e1xyz[0] * (float) v;
			 sgn[2][it] += (float) e2xyz[0];
		    } 
			else if (event->op_flag == MINUS)
			{ /* Spins summation */
			  /* T2* in the transverse plane xy */
			 sgn[0][it] -= (float) e0xyz[0] * (float) v;
			 sgn[1][it] -= (float) e1xyz[0] * (float) v;
			 sgn[2][it] -= (float) e2xyz[0];
		    }

		   }
		}
	    }
    }

#ifndef OSCILLO
    for (it = 1; it < expr->ntx; it++) {
	expr->sgn[0][it][event->nu_pointy][event->nu_pointz] += sgn[0][it];
	expr->sgn[1][it][event->nu_pointy][event->nu_pointz] += sgn[1][it];
	expr->sgn[2][it][event->nu_pointy][event->nu_pointz] += sgn[2][it];
    }
#else
    for (it = 1; it < expr->ntx; it++) {
	expr->sgn[0][it][event->nu_pointy][event->nu_pointz] = sgn[0][it];
	expr->sgn[1][it][event->nu_pointy][event->nu_pointz] = sgn[1][it];
	expr->sgn[2][it][event->nu_pointy][event->nu_pointz] = sgn[2][it];
    }
#endif

    free(sgn[0]);
    free(sgn[1]);
    free(sgn[2]);

	if (event->spoiling_flag==1)
	/* Numerical spoiling : Canceling the transverse magnetisation */
	 for (ix=xmin;ix<xmax;ix++)
		for(iy=expr->nfovy -1;iy>=0;iy--)
			for(iz=expr->nfovz -1;iz>=0;iz--)
			{  
		     expr->magstate[0][ix][iy][iz]=0.0;
		     expr->magstate[1][ix][iy][iz]=0.0;
			}

}
