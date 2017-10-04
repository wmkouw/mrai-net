/*************************************************************************
* $Id: kernel-phase-acq.c,v 1.6 2006/12/18 15:10:37 yougz Exp $
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

#include "kernel-phase-acq.h"
#ifdef HAVE_MPI
#include <mpi.h>
#include "seq-mpi.h" /* for MPI TAGS definition */
#endif

/*!
*************************************************************
* \ingroup Kernel
* \brief   Simulation for a phase acquisition of one point(Bloch Equation)
* \warning     
* \param expr Pointer on the experience structure containing the RF volume
* \param event Pointer on the event structure
**************************************************************/
void KernelPhase(EXPERIENCE3D * expr, EVENT * event)
{
    int ix, iy, iz;
    float sgn[3];
    int	xmin;
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
    /* 
     * Do a one point acquisition
     */

    sgn[0] = 0.;
    sgn[1] = 0.;
    sgn[2] = 0.;

    if (event->op_flag == PLUS)
	for (ix = xmin; ix < xmax; ix++)
	    for (iy = 0; iy < expr->nfovy; iy++)
		for (iz = 0; iz < expr->nfovz; iz++) {
		    sgn[0] += (float) expr->magstate[0][ix][iy][iz];
		    sgn[1] += (float) expr->magstate[1][ix][iy][iz];
		    sgn[2] += (float) expr->magstate[2][ix][iy][iz];
    } else
	for (ix = xmin; ix < xmax; ix++)
	    for (iy = 0; iy < expr->nfovy; iy++)
		for (iz = 0; iz < expr->nfovz; iz++) {
		    sgn[0] -= (float) expr->magstate[0][ix][iy][iz];
		    sgn[1] -= (float) expr->magstate[1][ix][iy][iz];
		    sgn[2] -= (float) expr->magstate[2][ix][iy][iz];
		}
    expr->sgn[0][event->nu_pointx][event->nu_pointy][event->nu_pointz]+=sgn[0];
    expr->sgn[1][event->nu_pointx][event->nu_pointy][event->nu_pointz]+=sgn[1];
    expr->sgn[2][event->nu_pointx][event->nu_pointy][event->nu_pointz]+=sgn[2];
}
