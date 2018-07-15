/*************************************************************************
* $Id: event.c,v 1.5 2006/12/18 15:10:36 yougz Exp $
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

#include "event.h"

/*!
*************************************************************
* \ingroup Event
* \brief Conversion function helping in entering parameters
* \param Pointer on the EVENT structure
**************************************************************/
void Conversion(EVENT * event)
{
    event->agx *= 1.e-4;	/* Conversion in Tesla */
    event->ax *= 1.e-4;
    event->agy *= 1.e-4;
    event->ay *= 1.e-4;
    event->agz *= 1.e-4;
    event->az *= 1.e-4;
    event->ang *= M_PI / 180;  /* Conversion in rad/s */
    event->pulse_duration *= 1.e-3; /* Conversion in second */
    event->acq_delay *= 1.e-3; /* Conversion in second */

}

/*!
*************************************************************
* \ingroup Event
* \brief Init of the EVENT structure to O
* \param Pointer on the EVENT structure
**************************************************************/
void InitEvent(EVENT * event)
{
    event->agx = 0.0;		/* Pulse gradient */
    event->agy = 0.0;		/* Pulse gradient*/
    event->agz = 0.0;		/* Pulse gradient*/
    event->ax = 0.0;		/* Precession gradient */
    event->ay = 0.0;		/* Precession gradient  */
    event->az = 0.0;		/* Precession gradient  */
    event->agxoff = 0.0;	/* Pulse gradient offset in meter*/
    event->agyoff = 0.0;	/* Pulse gradient offset in meter */
    event->agzoff = 0.0;	/* Pulse gradient offset in meter */
    event->axoff = 0.0;		/* Precession gradien offset */
    event->ayoff = 0.0;		/* Precession gradien offset */
    event->azoff = 0.0;		/* Precession gradien offset */
    event->ang = 0.0;		/* Pulse angle */
    event->pulse_duration = 0.0;	/* Pulse duration */
    event->acq_delay = 0.0;	/* Delay before readout */
    event->acq_flag = NONE;	/* Acquisition type*/
    event->op_flag = PLUS;	/* Accumulation sign */
    event->nu_pointx = 0;	/* Point x position if phase acquisition */
    event->nu_pointy = 0;	/* Point y position if phase acquisition */
    event->nu_pointz = 0;	/* Point z position if phase acquisition */

    return;
}

/*!
*************************************************************
* \ingroup Event
* \brief Mise a jour du flag de spoiling post-acquisition
* \param Pointer on the EVENT structure
* \param flag 0=no spoiling, 1=spoiling activation
**************************************************************/
void SetSpoilingFlag(EVENT * event,int flag)
{
	event->spoiling_flag = flag ; 
}
