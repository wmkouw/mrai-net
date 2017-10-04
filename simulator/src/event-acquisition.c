/*************************************************************************
* $Id: event-acquisition.c,v 1.7 2006/12/18 15:10:36 yougz Exp $
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

#include "event-acquisition.h"
/*!
*********************************************************
* \ingroup     Event
* \brief       One line acquisition with a frequency encoding along x direction
* \warning     
* \param expr  Pointer on the EXPERIENCE3D structure
* \param ax    Reading gradient amplitude in Gauss/meter
* \param npy   Line number in the k space, 
* \param npz   Plane number in the k space, where the readout signal is stored
* \param delai Precession time (ms) before acquisition. The gradient application includes this time
*              Equivalent of a DoGradient before a DoAcquisition  
* \param sign Regarding this value (MINUS or PLUS) the readout signal is added or substracted to the k space
***********************************************************/
void DoAcqFrequenceX(EXPERIENCE3D * expr,EVENT * event, double ax, int npy, int npz, double delai, int sign)
{
	    InitEvent(event);
	    event->nu_pointy = npy;
	    event->nu_pointz = npz;
	    event->ax = ax;
	    event->acq_delay = delai;	/* Te/2-(Tacq/2+Tacq/(2*(N-1))) */
	    event->acq_flag = FREQUENCY;
	    event->op_flag = sign;

	    Conversion(event);
	    Kernel(expr, event);
}