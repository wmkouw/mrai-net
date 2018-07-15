/*************************************************************************
* $Id: kernel-precession.c,v 1.6 2006/12/18 15:10:37 yougz Exp $
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
#include "kernel-precession.h"

/*!
*************************************************************
* \ingroup Kernel
* \brief       simulation of a precession step (Bloch Equation)
* \warning
* \param expr Pointer on the experience structure containing the RF volume
* \param event Pointer on the event structure
**************************************************************/
void KernelPrecession(EXPERIENCE3D * expr, EVENT * event)
{
    double x, y, z,edw,dw;
    double p6x, p6y, p6z, p9x, p9y, p9z;
    double e1, e2, teta, rmnx, r1, r2;
    int ix, iy, iz;
    int xmin;
    int xmax;

#ifdef HAVE_MPI
    if (expr->mpi_slice_x0 == -1) {
	xmin = 0;
	xmax = expr->nfovx;
    } else {
	xmin = expr->mpi_slice_x0;
	xmax = xmin+mpi_get_slice_width(expr, xmin);
    }
#else
    xmin = 0;
    xmax = expr->nfovx;
#endif

    /*
     * waiting a acq_delay time
     */
    if (event->acq_delay) {
	expr->to =(expr->to + event->acq_delay);
	p6x = G * event->ax;
	p9x = expr->xf + expr->obx - event->axoff;
	p6y = G * event->ay;
	p9y = expr->yf + expr->oby - event->ayoff;
	p6z = G * event->az;
	p9z = expr->zf + expr->obz - event->azoff;

	for (ix = xmin; ix < xmax; ix++) {
	    for (iy = expr->nfovy - 1; iy >= 0; iy--) {
		for (iz = expr->nfovz - 1; iz >= 0; iz--) {
		    x = expr->px * ix - p9x;
		    y = expr->py * iy - p9y;
		    z = expr->pz * iz - p9z;
//		    dw = (p6x * x) + (p6y * y) + (p6z * z) - (expr->G_edw) + G_rad_T*expr->G_deche[ix][iy][iz];
			edw = expr->G_edw *(expr->B0+1.*(event->ax*x + event->ay*y + event->az*z) + expr->G_deche[ix][iy][iz]);
			dw = G_rad_T*(event->ax*x + event->ay*y + event->az*z + expr->G_deche[ix][iy][iz])- edw;
		    teta = dw * event->acq_delay;	/* tr= event->acq_delay; */
		    e1 = exp(-event->acq_delay / expr->t1[ix][iy][iz]);
		    e2 = exp(-event->acq_delay / expr->t2[ix][iy][iz]);
		    r1 = e2 * cos(teta);
		    r2 = e2 * sin(teta);
		    rmnx =
			r1 * expr->magstate[0][ix][iy][iz] +
			r2 * expr->magstate[1][ix][iy][iz];
		    expr->magstate[1][ix][iy][iz] =
			-r2 * expr->magstate[0][ix][iy][iz] +
			r1 * expr->magstate[1][ix][iy][iz];
		    expr->magstate[2][ix][iy][iz] =
			e1 * expr->magstate[2][ix][iy][iz] +
			expr->ro[ix][iy][iz] * (1 - e1);
		    expr->magstate[0][ix][iy][iz] = rmnx;
		}
	    }
	}
    }
}
