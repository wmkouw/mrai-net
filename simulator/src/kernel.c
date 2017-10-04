/*************************************************************************
* $Id: kernel.c,v 1.6 2006/12/18 15:10:37 yougz Exp $
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

#include "kernel.h"
#ifdef HAVE_MPI
#include <string.h>
#include <math.h>
#include <mpi.h>
#endif

/*!
*************************************************************
* \ingroup Kernel
* \brief Call of the different kernel functions (Precession, pulse, acquisition)
* \warning     
* \param expr Pointer on the experience structure containing the RF volume
* \param event Pointer on the event structure
**************************************************************/
void Kernel(EXPERIENCE3D *expr, EVENT *event)
{
    KernelPulse(expr, event); 
    KernelPrecession(expr, event); 

    switch (event->acq_flag) {
    case NONE:
	break;

    case PHASE:
	KernelPhase(expr, event);
	break;

    case FREQUENCY:

	KernelFrequency1(expr, event);
#ifdef VERBOSE
	fprintf (stderr," line Y,Z = %4d %4d\n",event->nu_pointy,event->nu_pointz); 
#endif
	KernelFrequency2(expr, event);

	break;
    }
}
