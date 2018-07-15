/*************************************************************************
* $Id: seq-mpi.h,v 1.5 2006/12/18 15:10:38 yougz Exp $
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

#ifndef _SEQ_MPI_H
#define _SEQ_MPI_H

#include "simri3d.h"
#include "experience.h"
#include "sequence.h"

typedef void (*task_cb)(SEQPARAM *,EXPERIENCE3D*);
void mpi_job (SEQPARAM *seqparam, EXPERIENCE3D * expr, task_cb task);

#ifdef HAVE_MPI

#define WORKTAG		    1
#define DIETAG		    2

#define SLICE_WIDTH	    2

void mpi_job_master (SEQPARAM *seqparam, EXPERIENCE3D * expr);
void mpi_job_slave (SEQPARAM *seqparam, EXPERIENCE3D * expr, task_cb task);
int  mpi_get_slice_width (EXPERIENCE3D *, int);
#endif

#endif
