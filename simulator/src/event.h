/*************************************************************************
* $Id: event.h,v 1.6 2006/12/18 15:10:37 yougz Exp $
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

#ifndef _EVENT_H
#define _EVENT_H
#include <stdlib.h>
#include <string.h>
#include "simri3d.h"

#define ACTIVE 1
#define INACTIVE 0

/************************
* Structure de donnees
*************************/
typedef struct _Event {
    /*! Gradient value during a pulse */
	double agx,agy,agz;
    /*! Gradient offset during a pulse */
	double agxoff,agyoff,agzoff;
    /*! Gradient value during precession */
	double ax,ay,az;			
    /*! Gradient offset during precession */
	double axoff,ayoff,azoff;		
    /*! Pulse angle */
	double ang;			
    /*! Pulse duration */
	double pulse_duration;		
    /*!Delay before acquisition */
	double acq_delay;		
    /*!Acquisition type PHASE or FREQUENCY*/
    int acq_flag;		
    /*!Accumulation sign PLUS or MINUS*/
	int op_flag;	
	/*!Spoiling flag value */
	int spoiling_flag;	
    /*! K-space point position in case of phase acquisition */
	int nu_pointx,nu_pointy,nu_pointz;
    } EVENT;

/************************
* Prototypes des focntions
*************************/
void InitEvent      (EVENT *);
void Conversion     (EVENT *);
void SetSpoilingFlag(EVENT *,int);

#endif
