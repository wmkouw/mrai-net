/*************************************************************************
* $Id: kernel-rfpulse.c,v 1.6 2006/12/18 15:10:37 yougz Exp $
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
#include "kernel-rfpulse.h"

/*!
*************************************************************
* \ingroup Kernel
* \brief   Simulation of an RF pulse step (Bloch equation)
* \warning     
* \param expr Pointer on the experience structure containing the RF volume
* \param event Pointer on the event structure
**************************************************************/
void KernelPulse(EXPERIENCE3D * expr, EVENT * event)
{
    double w1, w1max, phi, cphi, sphi, x, y, z, dw,edw, alpha, beta, delta, sb, cb, sd, cd;
    double antx, anty, antz,oldantx, oldanty, p1, p2, p3, p5, p6, p9;
    double r1x, r2x, tau;
    double r1y, r2y, r1z, r2z;
    int ix, iy, iz, it, n;
    int xmin;
    int xmax;

#ifdef HAVE_MPI
    if (expr->mpi_slice_x0 == -1) {
	xmin = 0;
	xmax = expr->nfovx;
    } else {
	xmin=expr->mpi_slice_x0;
	xmax=xmin+mpi_get_slice_width(expr, xmin);
    }
#else
    xmin = 0;
    xmax = expr->nfovx;
#endif


    if (event->pulse_duration && event->ang) {
	r1x = G * event->agx;
	r2x = expr->xf + expr->obx - event->agxoff;
	r1y = G * event->agy;
	r2y = expr->yf + expr->oby - event->agyoff;
	r1z = G * event->agz;
	r2z = expr->zf + expr->obz - event->agzoff;


	n = 1;
	alpha = event->ang;
	tau = event->pulse_duration;
	w1max = alpha / tau;             // = omega1 max
	w1 = w1max;

	for (it = 0; it < n; it++)
	    for (ix = xmin; ix < xmax; ix++)
		for (iy = expr->nfovy - 1; iy >= 0; iy--)
	    for (iz = expr->nfovz - 1; iz >= 0; iz--) {
	        antx = expr->magstate[0][ix][iy][iz];   // M_x
			anty = expr->magstate[1][ix][iy][iz];   // M_y
			antz = expr->magstate[2][ix][iy][iz];   // M_z
            oldantx = antx;
            oldanty = anty;
			x = expr->px * ix - r2x;
			y = expr->py * iy - r2y;
			z = expr->pz * iz - r2z;
//		        dw = (r1x * x) + (r1y * y) + (r1z * z) -expr->G_edw + G_rad_T*expr->G_deche[ix][iy][iz];
			if (expr->FLAG_RESONANCE==ON)
			{ /* pulse angle are considered not affected by resonance frequency shift */
			  dw = G_rad_T*(event->agx*x + event->agy*y + event->agz*z);
			}
			else
			{ /* pulse angle are affected by resonance frequency shift */
			  edw = expr->G_edw *(expr->B0+ event->agx*x + event->agy*y + event->agz*z + expr->G_deche[ix][iy][iz]);
			  dw = G_rad_T*(event->agx*x + event->agy*y + event->agz*z + expr->G_deche[ix][iy][iz]) - edw;
			}
			// Introduction of precomputed B1 inhomogeneities due to high B0-field : B1 module has been normalized (max. is 1)       
            if (expr->inhomogeneousB1){
			  w1 = w1max * IdModule(expr->B1t[iz][iy][ix]);                        
              phi = IdPhase(expr->B1t[iz][iy][ix]);
			 }
			beta = -atan2(dw, w1);                   // angle between B1 and Beff (eq.(13) of ref. JMR SIMRI paper)
			delta = -tau * sqrt(dw * dw + w1 * w1);  // flip angle alpha' in eq.(12) of ref. paper above
			sb = sin(beta);
			cb = cos(beta);
			sd = sin(delta);
			cd = cos(delta);
			p5 = cd--; /* p5=cd, cd = cd-1 */
			p1 = 1 + sb * sb * cd;
			p2 = sb * sd;
			p3 = cb * sb * cd;
			p6 = -cb * sd;
			p9 = 1 + cb * cb * cd;
			antx = p1 * oldantx + p2 * oldanty + p3 * antz;
			anty = -p2 * oldantx + p5 * oldanty + p6 * antz;
	        antz = p3 * oldantx - p6 * oldanty + p9 * antz;
            expr->magstate[0][ix][iy][iz] = antx;
            expr->magstate[1][ix][iy][iz] = anty;
 			expr->magstate[2][ix][iy][iz] = antz;
		      }
	/* T2* effect management*/
	expr->to = expr->to + tau;
	if (alpha > M_PI/2) expr->to = expr->to * -1;
    }
}
