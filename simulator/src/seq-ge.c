/*************************************************************************
* $Id: seq-ge.c,v 1.8 2006/12/18 15:10:38 yougz Exp $
**************************************************************************
Fonctions that implement grient echo sequence
This sequence runs the following events
- Pulse with teta angle. Teta optimal if cos(teta)=exp(-Tr/T1)
- Waiting
- Phase gradient
- Waiting
- Bipolar gradient: half negative lobe before acquisition
- Acquisition with a positive gradient to get an echo in the center of the acquisition window
- Waiting until TR
To avoid a call to the magnetisation kernel, and earn time
We apply simultaneously the phase gradients and the negative gradient before acquisition
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

#include "seq-ge.h"

#include "event-rfpulse.h"
#include "event-precession.h"
#include "event-acquisition.h"
#include "experience.h"
#include "seq-mpi.h"

void SeqGradientEcho1D_0(SEQPARAM *seqparam,EXPERIENCE3D* expr);
void SeqGradientEcho2D_0(SEQPARAM *seqparam,EXPERIENCE3D* expr);
void SeqGradientEcho3D_0(SEQPARAM *seqparam,EXPERIENCE3D* expr);

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Required MPI overhead function that runs the SeqGradientEcho1D_0 function
* \warning     
* \param seqparam Pointer on the sequence parameter structure (teta,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
* \return      Pointer on the acquired RF volume 
***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE SeqGradientEcho1D(SEQPARAM *seqparam,EXPERIENCE3D* expr)
{
	mpi_job(seqparam, expr, SeqGradientEcho1D_0);
	NormalizeRFSignal(expr);
	return(GetSignalRFComplexFromExperience(expr));
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Required MPI overhead function that runs the SeqGradientEcho2D_0 function
* \warning     
* \param seqparam Pointer on the sequence parameter structure (teta,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
* \return      Pointer on the acquired RF volume 
***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE SeqGradientEcho2D(SEQPARAM *seqparam,EXPERIENCE3D* expr)
{
	mpi_job(seqparam, expr, SeqGradientEcho2D_0);
	NormalizeRFSignal(expr);
	return(GetSignalRFComplexFromExperience(expr));
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Required MPI overhead function that runs the SeqGradientEcho3D_0 function
* \warning     
* \param seqparam Pointer on the sequence parameter structure (teta,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
* \return      Pointer on the acquired RF volume 
***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE SeqGradientEcho3D(SEQPARAM *seqparam,EXPERIENCE3D* expr)
{
	mpi_job(seqparam, expr, SeqGradientEcho3D_0);
	NormalizeRFSignal(expr);
	return(GetSignalRFComplexFromExperience(expr));
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Checking of the GE time parameters
* \param tacq  Acquisition time (in ms) equal to the inverse of the sampling fequency 
* \param te    Echo time TE (in ms)
* \param tr    Rpetition time TR (in ms)
***********************************************************/
void CheckTimesGE(double tacq,double te,double tr)
{
	int f=0;
	if((tacq<te)&&(tr>te+tacq/2.0))f=1;
	if(f==0){
		FPRINTF(stderr,"Bad values for your sequence GE: Tacq=%g, Te=%g, Tr=%g\n",tacq, te, tr);
		exit(0);
	}
}
/*!
**********************************************************
* \ingroup     Sequence
* \brief       Execute a GradientEcho1D sequence 
* \warning     
* \param seqparam Sequence parameter (Teta,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx)
***********************************************************/
void SeqGradientEcho1D_0(SEQPARAM *seqparam,EXPERIENCE3D* expr)
{
  int      ntx;
  int      npy,npz;
  double   fovx;
  double   te,tr,tacq,timp,acq_delay,tg; /* ms */
  double   gx;
  double   teta;	   /* degree */
  int i;
  EVENT* entre, entre_r;
  entre = &entre_r;

  printf("Starting SeqGradientEcho2D \n");  
  ntx = expr->ntx;
  tacq = expr->readout_time*1.e+3;
  fovx = expr->fovx;
  te = seqparam->te;
  tr = seqparam->tr;
  teta = seqparam->teta;
   
  CheckTimesGE(tacq,te,tr);
  timp=0.3;/*RF pulse duration in ms */   
  acq_delay = 0;
  tg=tacq/2.0;
  gx=(ntx-1.)/tacq/G_kHz_G/fovx;
  npy=0;
  npz=0;

  SetSpoilingFlag(entre,ACTIVE); /* XY Spoiling after acquisition */
  DoWaiting(expr,entre,ATT_INIT); /* To get an initial magnetisation M0 */

  for(i=0;i<10;i++) /* Repetition to get a TR effect */
   {
    DoPulseRect(expr,entre,teta,timp);
	DoWaiting(expr,entre,tr);
   }
  ResetToExperience(expr);
  DoPulseRect(expr,entre,teta,timp);
  DoWaiting(expr,entre,te-tacq);
  // Application of the negative gradient before reading (-gx)
  DoGradient( expr,entre,tg,-gx,0,0.0);
  // Readout with application of a positive gradient (gx)
  DoAcqFrequenceX(expr,entre, gx, npy, npz,acq_delay,MINUS);
 
  printf("Ending SeqGradientEcho1D \n");  
}


/*!
**********************************************************
* \ingroup     Sequence
* \brief       Execute a GradientEcho2D sequence 
* \warning     
* \param seqparam Sequence parameter (Teta,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
***********************************************************/
void SeqGradientEcho2D_0(SEQPARAM *seqparam,EXPERIENCE3D* expr)
{
  int      ntx,nty;
  double   fovx,fovy;
  double   te,tr,tacq,timp,acq_delay,tg; /* ms */
  double   gx,gy,gz,gp;
  double   teta;	   /* degree */
  int	   npy, npz;
  EVENT* entre, entre_r;
  entre = &entre_r;

  printf("Starting SeqGradientEcho2D \n");  
  ntx = expr->ntx;
  nty = expr->nty;
  tacq = expr->readout_time*1.e+3;
  fovx = expr->fovx;
  fovy = expr->fovy;

  te = seqparam->te;
  tr = seqparam->tr;
  teta = seqparam->teta;
   
  CheckTimesGE(tacq,te,tr);
  timp=0.3;/*RF pulse duration in ms */   
  acq_delay = 0;
  tg=tacq/2.0;
  gx=(ntx-1.)/tacq/G_kHz_G/fovx;
  gy=2*M_PI*(nty-1)/G_rad_G/fovy/(tg/1000);
  gz=0.0;
  npz=0;

  SetSpoilingFlag(entre,ACTIVE); /* XY Spoiling after acquisition */
  DoWaiting(expr,entre,ATT_INIT); /* To get an initial magnetisation M0 */

  for (npy = 0; npy < expr->nty; npy++)
	 {
       ResetToExperience(expr);
	   DoPulseRect(expr,entre,teta,timp);
	   DoWaiting(expr,entre,te-tacq);
       gp = (gy) * (npy - nty / 2.) / nty;  
	   // Simultaneous application of the negative gradient before reading (-gx) and the phase gradient gp
	   DoGradient( expr,entre,tg,-gx,-gp,0.0);
	   // Readout with application of a positive gradient (gx)
       DoAcqFrequenceX(expr,entre, gx, npy, npz,acq_delay,MINUS);
	   // Waiting before next line
	   DoWaiting(expr,entre,tr-te-tacq/2.0);
	 }
 
  printf("Ending SeqGradientEcho2D \n");  
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Execute a GradientEcho3D sequence 
* \warning     
* \param seqparam Sequence parameter (Teta,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty,ntz)
***********************************************************/
void SeqGradientEcho3D_0(SEQPARAM *seqparam,EXPERIENCE3D* expr)
{
  int      ntx,nty,ntz;
  double   fovx,fovy,fovz;
  double   te,tr,tacq,timp,acq_delay,tg; /* ms */
  double   gx,gy,gz,gpy,gpz;
  double   teta;	   /* degree */
  int	   npy,npz;
  EVENT* entre, entre_r;
  entre = &entre_r;

  printf("Starting SeqGradientEcho3D \n");  
  ntx = expr->ntx;
  nty = expr->nty;
  ntz = expr->ntz;
  tacq = expr->readout_time*1.e+3;
  fovx = expr->fovx;
  fovy = expr->fovy;
  fovz = expr->fovz;
  te = seqparam->te;
  tr = seqparam->tr;
  teta = seqparam->teta;
   
  CheckTimesGE(tacq,te,tr);
  timp=0.3;/*RF pulse duration in ms */   
  acq_delay = 0;
  tg=tacq/2.0;
  gx=(ntx-1.)/tacq/G_kHz_G/fovx;
  gy=2*M_PI*(nty-1)/G_rad_G/fovy/(tg/1000);
  gz=2*M_PI*(ntz-1)/G_rad_G/fovz/(tg/1000);

  SetSpoilingFlag(entre,ACTIVE); /* XY Spoiling after acquisition */
  DoWaiting(expr,entre,ATT_INIT); /* To get an initial magnetisation M0 */

  for (npy = 0; npy < expr->nty; npy++)	 {
  for (npz = 0; npz < expr->ntz; npz++)	 {
       ResetToExperience(expr);
	   DoPulseRect(expr,entre,teta,timp);
	   DoWaiting(expr,entre,te-tacq);
       gpy = (gy) * (npy - nty / 2.) / nty;
	   gpz = (gz) * (npz - ntz / 2.) / ntz;
	   // Simultaneous application of the negative gradient before reading (-gx) and the phase gradients gpy,gpz
	   DoGradient( expr,entre,tg,-gx,-gpy,-gpz);
	   // Readout with application of a positive gradient (gx)
       DoAcqFrequenceX(expr,entre, gx,npy,npz,acq_delay,MINUS);
	   // Waiting before next line
	   DoWaiting(expr,entre,tr-te-tacq/2.0);
	 }}
 
  printf("Ending SeqGradientEcho3D \n");  
}
