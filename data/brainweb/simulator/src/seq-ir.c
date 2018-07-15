/*************************************************************************
* $Id: seq-ir.c,v 1.4 2006/12/18 15:10:38 yougz Exp $
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
#include "seq-ir.h"
#include "seq-mpi.h"
#include "event-acquisition.h"
#include "experience.h"
#include "event.h"
#include "event-rfpulse.h"
#include "event-precession.h"

void SeqInversionRecuperation1D_0(SEQPARAM *seqparam,EXPERIENCE3D*expr);
void SeqInversionRecuperation2D_0(SEQPARAM *seqparam,EXPERIENCE3D*expr);
void SeqInversionRecuperation3D_0(SEQPARAM *seqparam,EXPERIENCE3D*expr);

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Required MPI overhead function that runs the SeqInversionRecuperation1D_0 function
* \warning     
* \param seqparam Pointer on the sequence parameter structure (Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
* \return      Pointer on the acquired RF volume 
***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE SeqInversionRecuperation1D(SEQPARAM *seqparam,EXPERIENCE3D*expr)
{
	mpi_job(seqparam, expr, SeqInversionRecuperation1D_0);
	NormalizeRFSignal(expr);
 	return(GetSignalRFComplexFromExperience(expr));
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Required MPI overhead function that runs the SeqInversionRecuperation2D_0 function
* \warning     
* \param seqparam Pointer on the sequence parameter structure (Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
* \return      Pointer on the acquired RF volume 
***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE SeqInversionRecuperation2D(SEQPARAM *seqparam,EXPERIENCE3D*expr)
{
	mpi_job(seqparam , expr, SeqInversionRecuperation2D_0);
	NormalizeRFSignal(expr);
 	return(GetSignalRFComplexFromExperience(expr));
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Required MPI overhead function that runs the SeqInversionRecuperation3D_0 function
* \warning     
* \param seqparam Pointer on the sequence parameter structure (Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
* \return      Pointer on the acquired RF volume 
***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE SeqInversionRecuperation3D(SEQPARAM *seqparam,EXPERIENCE3D*expr)
{
	mpi_job(seqparam, expr, SeqInversionRecuperation3D_0);
	NormalizeRFSignal(expr);
 	return(GetSignalRFComplexFromExperience(expr));
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Checking of the SE time parameters
* \param tacq  Acquisition time (in ms) equal to the inverse of the sampling fequency 
* \param te    Echo time TE (in ms)
* \param tr    Rpetition time TR (in ms)
* \param ti    Inversion time TI (in ms)
***********************************************************/
void CheckTimesIR(double tacq,double te,double tr,double ti)
{
	int f=0;
	if((tacq<te)&&(tr>te+tacq/2.0))f=1;
	if(f==0){
		FPRINTF(stderr,"Bad values for your sequence SE: Tacq=%g, Te=%g, Tr=%g\n",tacq, te, tr);
		exit(0);
	}
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Define and execute a 2D Inversion Recuperation sequence
* \warning     
* \param seqparam Pointer on the sequence parameter structure (Ti,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx)
 ***********************************************************/
void SeqInversionRecuperation1D_0(SEQPARAM *seqparam,EXPERIENCE3D*expr)
{
  int      ntx;
  double   fovx;
  double   te,tr,tacq,ti; /* in ms */
  double   gx,gy,gz;
  double   acq_delay,timp;	/* in ms */
  int	   npy, npz;
  int i;
  EVENT* event, event_r;
  event = &event_r;

   printf("Starting SeqInversionRecuperation1D\n");  
   ntx = expr->ntx;
   tacq = expr->readout_time*1.e+3;
   fovx = expr->fovx;
   
   te = seqparam->te;
   tr = seqparam->tr;
   ti = seqparam->ti;
   
   CheckTimesIR(tacq,te,tr,ti);

   acq_delay = (te/2) - (tacq/2. + tacq/2./(ntx-1));
   timp=0.3; /*RF pulse duration in ms */
   
   gx=(ntx-2.)/tacq/G_kHz_G/fovx;
   gy=0.0;
   gz=0.0;
   npy=0;npz=0;

   SetSpoilingFlag(event,ACTIVE); /* XY Spoiling after each line acquisition */
   DoWaiting(expr,event,ATT_INIT); /* To get an initial Magnetization M0 */
   ResetToExperience(expr);

   for(i=0;i<10;i++) /* Repetition cycle to take TR into account */
   {
	DoPulseRect(expr,event, 180.,timp);
	DoWaiting(expr,event,ti);
	DoPulseRect(expr,event, 90., timp);
    DoGradient(expr,event, te/2.,gx,gy,gz);
    DoPulseRect(expr,event,180.,timp);
    DoGradient(expr,event, acq_delay+tacq,gx,gy,gz);
	DoWaiting(expr,event,tr-te-tacq/2.0);
   }
   
   DoPulseRect(expr,event, 180.,timp);
   DoWaiting(expr,event,ti);
   DoPulseRect(expr,event, 90.,timp);
   DoGradient(expr,event, te/2.,gx,gy,gz);
   DoPulseRect(expr,event,180.,timp);
   DoAcqFrequenceX( expr,event, gx, npy, npz,acq_delay, MINUS);

   printf("Ending SeqInversionRecuperation1D\n");  
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Define and execute a 2D Inversion Recuperation sequence
* \warning     
* \param seqparam Pointer on the sequence parameter structure (Ti,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
 ***********************************************************/
void SeqInversionRecuperation2D_0(SEQPARAM *seqparam,EXPERIENCE3D*expr)
{
  int      ntx,nty;
  double   fovx,fovy;
  double   te,tr,ti,tacq,timp,acq_delay,tg; /* ms */
  double   gx,gy,gz,gp;
  int	   npy, npz;
  EVENT* event, event_r;
  event = &event_r;

  printf("Starting SeqInversionRecuperation2D \n");  
  ntx = expr->ntx;
  nty = expr->nty;
  tacq = expr->readout_time*1.e+3;
  fovx = expr->fovx;
  fovy = expr->fovy;

  ti = seqparam->ti;
  te = seqparam->te;
  tr = seqparam->tr;

  CheckTimesIR(tacq,te,tr,ti);
  timp=0.3; /* Pulse duration in ms */
  acq_delay = (te/2) - (tacq/2. + tacq/2./(ntx-1));
  tg = te/2.;
  gx = (ntx-2.)/tacq/G_kHz_G/fovx;	    
  gy = 2*M_PI*(nty-1)/G_rad_G/fovy/(tg/1000); 
  gz = 0.0;
  npz= 0;

  SetSpoilingFlag(event,ACTIVE); /* XY Spoiling after each line acquisition */

  DoWaiting(expr,event,ATT_INIT); /* To get an initial Magnetization M0 */
  for (npy = 0; npy < expr->nty; npy++)
	 {
	    ResetToExperience(expr);
		DoPulseRect(expr,event, 180.,timp);
		DoWaiting(expr,event,ti);

		DoPulseRect(expr,event, 90.,timp);
		gp = (gy) * (npy - (nty) / 2) / (nty);
		DoGradient(expr,event,tg,gx,gp,gz);

		DoPulseRect(expr,event,180.,timp);

		DoAcqFrequenceX( expr,event, gx, npy, npz,acq_delay, MINUS);

		DoWaiting(expr,event,tr-te-tacq/2.0);
	 }
  printf("Ending SeqInversionRecuperation2D\n");  
}


/*!
**********************************************************
* \ingroup     Sequence
* \brief       Define and execute a 2D Inversion Recuperation sequence
* \warning     
* \param seqparam Pointer on the sequence parameter structure (Ti,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty,ntz)
 ***********************************************************/
void SeqInversionRecuperation3D_0(SEQPARAM *seqparam,EXPERIENCE3D*expr)
{
  int      ntx,nty,ntz;
  double   fovx,fovy,fovz;
  double   te,tr,tacq,ti; /* ms */
  double   gx,gy,gz;
  double   acq_delay,timp;	/* ms */
  int	   npy,npz;
  EVENT* event, event_r;
  event = &event_r;
  
  printf("Starting SeqInversionRecuperation3D\n");  
   ntx = expr->ntx;
   nty = expr->nty;
   ntz = expr->ntz;
   tacq = expr->readout_time*1.e+3;
   fovx = expr->fovx;
   fovy = expr->fovy;
   fovz = expr->fovz;

   te = seqparam->te;
   tr = seqparam->tr;
   ti = seqparam->ti;
   
   CheckTimesIR(tacq,te,tr,ti);

   acq_delay = (te/2) - (tacq/2. + tacq/2./(ntx-1));
   
   gx=(ntx-2.)/tacq/G_kHz_G/fovx;	    
   gy=2*M_PI*(nty-1)/G_rad_G/fovy/(te/2./1000); 
   gz=2*M_PI*(ntz-1)/G_rad_G/fovz/(te/2./1000); 
   timp=0.3; /* Pulse duration in ms */

   SetSpoilingFlag(event,ACTIVE); /* XY Spoiling after each line acquisition */

   DoWaiting(expr,event,ATT_INIT); /* To get an initial Magnetization M0 */
	for (npy = 0; npy < expr->nty; npy++) {
	for (npz = 0; npz < expr->ntz; npz++) {
        ResetToExperience(expr);
		DoPulseRect(expr,event, 180.,timp);
		DoWaiting(expr,event,ti);
		DoPulseRect(expr,event, 90., timp);
		DoGradient( expr,event, te/2.,gx,
			   (gy) * (npy - nty / 2) / nty,
			       (gz) * (npz - ntz / 2) / ntz);
		DoPulseRect(expr,event,180.,timp);
		DoAcqFrequenceX( expr,event, gx, npy, npz,acq_delay, MINUS);
		DoWaiting(expr,event,tr-te-tacq/2.0);
	}}
	printf("Ending SeqInversionRecuperation3D\n");  

}

