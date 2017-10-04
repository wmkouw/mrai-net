/*************************************************************************
* $Id: seq-fid.c,v 1.5 2006/12/18 15:10:38 yougz Exp $
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

#include "seq-fid.h"

#include "event-rfpulse.h"
#include "event-precession.h"
#include "event-acquisition.h"
#include <idima.h>
#include "seq-mpi.h"


void SeqFreeInduction1D_0(SEQPARAM *seqparam,EXPERIENCE3D*expr);

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Required MPI overhead function that runs the SeqFreeInduction1D_0 function
* \warning     
* \param seqparam Pointer on the sequence parameter structure (Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
* \return      Pointer on the acquired RF volume 
***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE SeqFreeInduction1D(SEQPARAM *seqparam,EXPERIENCE3D*expr)
{
	mpi_job(seqparam, expr, SeqFreeInduction1D_0);
	NormalizeRFSignal(expr);
 	return(GetSignalRFComplexFromExperience(expr));
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Free Induction Decay sequence
* \param seqparam Pointer on the sequence parameter structure (Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
***********************************************************/
void SeqFreeInduction1D_0 (SEQPARAM *seqparam,EXPERIENCE3D*expr)
{
  double   acq_delay,timp;	/* in ms */
  EVENT* event, event_r;
  event = &event_r;

  printf("Starting SeqFreeInduction1D \n");  

  timp=0.3; /*RF pulse duration in ms */   

  acq_delay=0.0;

  DoWaiting(expr,event,ATT_INIT); /* To get an initial Magnetization M0 */
 
  DoPulseRect(expr,event, 90.,timp);
  DoAcqFrequenceX(expr,event,0,0,0,acq_delay, MINUS);

  printf("Ending SeqFreeInduction1D \n");  
}


