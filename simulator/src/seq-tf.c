/*************************************************************************
* $Id: seq-tf.c,v 1.8 2006/12/18 15:10:38 yougz Exp $
**************************************************************************
Functions that implement a TrueFisp Sequence
This sequence chains the following events
- RF pulse 
- Waiting
- Phase Gradient
- Waiting
- Bipolar Gradient : Half negative lobe before acquisition
- Acquisition with a positive gradient
- Rewind of all the gradients (phase and readout)
******************************************************************************
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

#include "seq-tf.h"
#include "event-rfpulse.h"
#include "event-precession.h"
#include "event-acquisition.h"
#include "experience.h"
#include "seq-mpi.h"

void SeqTrueFisp1D_0(SEQPARAM *seqparam,EXPERIENCE3D* expr);
void SeqTrueFisp2D_0(SEQPARAM *seqparam,EXPERIENCE3D* expr);
void SeqTrueFisp3D_0(SEQPARAM *seqparam,EXPERIENCE3D* expr);

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Required MPI overhead function that runs the SeqTrueFisp1D_0 function
* \warning     
* \param seqparam Pointer on the sequence parameter structure (teta,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx)
* \return      Pointer on the acquired RF volume 
***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE
SeqTrueFisp1D(SEQPARAM *seqparam,EXPERIENCE3D* expr)
{
	mpi_job(seqparam, expr, SeqTrueFisp1D_0);
	NormalizeRFSignal(expr);
	return(GetSignalRFComplexFromExperience(expr));
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Required MPI overhead function that runs the SeqTrueFisp2D_0 function
* \warning     
* \param seqparam Pointer on the sequence parameter structure (teta,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
* \return      Pointer on the acquired RF volume 
***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE
SeqTrueFisp2D(SEQPARAM *seqparam,EXPERIENCE3D* expr)
{
	mpi_job(seqparam, expr, SeqTrueFisp2D_0);
	NormalizeRFSignal(expr);
	return(GetSignalRFComplexFromExperience(expr));
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Required MPI overhead function that runs the SeqTrueFisp3D_0 function
* \warning     
* \param seqparam Pointer on the sequence parameter structure (teta,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty,ntz)
* \return      Pointer on the acquired RF volume 
***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE
SeqTrueFisp3D(SEQPARAM *seqparam,EXPERIENCE3D* expr)
{
	mpi_job(seqparam, expr, SeqTrueFisp3D_0);
	NormalizeRFSignal(expr);
	return(GetSignalRFComplexFromExperience(expr));
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Verification des parametres d'une sequence True Fisp
* \param tacq  Temps d'acquisition en ms
* \param tr    Temps de repetition (TR) en ms
* \param twait Temps de lattence en ms
***********************************************************/
void CheckTimesTF(double tacq,double tr,double twait)
{
	int f=0;
	if((tr>=2*tacq)&&(twait>=0.))f=1;
	if(f==0){
		FPRINTF(stderr,"Bad values for your sequence TF: Tacq=%g, Tr=%g, Twait=%g, \n",tacq, tr, twait);
		exit(0);
	}
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Execute a TrueFisp1D sequence 
* \warning     
* \param seqparam Sequence parameter (Teta,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx)
***********************************************************/
void SeqTrueFisp1D_0(SEQPARAM *seqparam,EXPERIENCE3D* expr)
{
  int      i,ntx,phase,altphase;
  double   fovx;
  double   tr,tacq,timp,acq_delay,tgrad,twait; /* ms */
  double   gx;
  double   teta;	   /* degree */
  int	   npy, npz;
  EVENT* entre, entre_r;
  entre = &entre_r;

   printf("Starting SeqTrueFisp1D \n");  

   ntx = expr->ntx;
   tacq = expr->readout_time*1.e+3;
   fovx = expr->fovx;

   tr = seqparam->tr;
   teta = seqparam->teta;
   
   CheckTimesTF(tacq,tr/2.,tr);
   timp=0.3; /*RF pulse duration in ms */   
   acq_delay = 0;
   tgrad=(tacq)/2.;
   twait=(tr-2*tgrad-tacq)/2;

   gx=(ntx-1)/tacq/G_kHz_G/fovx;	    
   npy=0;npz=0;

   SetSpoilingFlag(entre,INACTIVE); /* No XY Spoiling after acquisition for true fisp*/
   DoWaiting(expr,entre,ATT_INIT); /* To get an initial magnetisation M0 */

   altphase=-1;/* Phase alternance at each acquisition */
   phase = 1;
  /* Iterated excitations to get an equilibrium state */
  for (i = 0; i<10*ntx; i++)
	 {
	  DoPulseRect(expr,entre,teta * phase,timp);
	  DoWaiting(expr,entre,tr);
	  phase= phase * altphase;
	 }
  ResetToExperience(expr);
  DoPulseRect(expr,entre,teta * phase,timp);
  DoWaiting(expr,entre,twait); 
  DoGradient( expr,entre,tgrad,-gx,0.0,0.0);
  DoAcqFrequenceX(expr,entre, +gx, npy, npz,acq_delay, phase);
  
  printf("Ending SeqTrueFisp1D \n");  
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Execute a TrueFisp2D sequence 
* \warning     
* \param seqparam Sequence parameter (Teta,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
***********************************************************/
void SeqTrueFisp2D_0(SEQPARAM *seqparam,EXPERIENCE3D* expr)
{
  int      ntx,nty, phase, altphase;
  double   fovx,fovy;
  double   tr,tacq,timp,acq_delay,tgrad,twait; /* ms */
  double   gx,gy,gz,gpy;
  double   teta;	   /* degree */
  int	   npy, npz;
  EVENT* entre, entre_r;
  entre = &entre_r;

  printf("Starting SeqTrueFisp2D \n");  
  ntx = expr->ntx;
  nty = expr->nty;
  tacq = expr->readout_time*1.e+3;
  fovx = expr->fovx;
  fovy = expr->fovy;

  tr = seqparam->tr;
  teta = seqparam->teta;
  
  timp=0.3; /*RF pulse duration in ms */   
  acq_delay = 0;
  tgrad=(tacq)/2.;
  twait=(tr-2*tgrad-tacq)/2;
   
  CheckTimesTF(tacq,tr,twait);
   
  gx=(ntx-1.)/tacq/G_kHz_G/fovx;	    
  gy=2*M_PI*(nty-1)/G_rad_G/fovy/(tgrad/1000);
  gz=0.0;
  npz=0;

  SetSpoilingFlag(entre,INACTIVE); /* No XY Spoiling after acquisition for true fisp*/
  DoWaiting(expr,entre,ATT_INIT); /* To get an initial magnetisation M0 */

  altphase=-1; /* Phase alternance at each acquisition */
  phase = 1;
  
  /* Iterated excitations to get an equilibrium state */
  for (npy = 0; npy < 10 * expr->nty; npy++)
	 {
	  DoPulseRect(expr,entre,teta * phase,timp);
	  DoWaiting(expr,entre,tr);
	  phase= phase * altphase;
	 }
   
  for (npy = 0; npy < expr->nty; npy++)
	 {
 	   ResetToExperience(expr);
	   DoPulseRect(expr,entre,teta * phase,timp);
	   DoWaiting(expr,entre,twait); 
       gpy = (gy) * (npy - nty / 2.) / nty;  
	   // Simultaneous application of the negative gradient before reading (-gx) and the phase gradients gpy
       DoGradient( expr,entre,tgrad,-gx,-gpy,0.0);
       DoAcqFrequenceX(expr,entre, +gx, npy, npz,acq_delay, phase);
       DoGradient( expr,entre,tgrad,-gx,+gpy,0.0);
	   DoWaiting(expr,entre,twait); 
	   phase= phase * altphase;
	 }
  printf("Ending SeqTrueFisp2D\n");  
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Execute a TrueFisp3D sequence 
* \warning     
* \param seqparam Sequence parameter (Teta,Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty,ntz)
***********************************************************/
void SeqTrueFisp3D_0(SEQPARAM *seqparam,EXPERIENCE3D* expr)
{
  int      ntx,nty,ntz, phase, altphase;
  double   fovx,fovy,fovz;
  double   tr,tacq,timp,acq_delay,tgrad,twait; /* ms */
  double   gx,gy,gz,gpy,gpz;
  double   teta;	   /* degree */
  int	   npy,npz;
  EVENT* entre, entre_r;
  entre = &entre_r;

  printf("Starting SeqTrueFisp3D \n");  
  ntx = expr->ntx;
  nty = expr->nty;
  ntz = expr->ntz;
  tacq = expr->readout_time*1.e+3;
  fovx = expr->fovx;
  fovy = expr->fovy;
  fovz = expr->fovz;

  tr = seqparam->tr;
  teta = seqparam->teta;
  
  timp=0.3;/*RF pulse duration in ms */   
  acq_delay = 0;
  tgrad=(tacq)/2.;
  twait=(tr-2*tgrad-tacq)/2;
   
  CheckTimesTF(tacq,tr,twait);
   
  gx=(ntx-1.)/tacq/G_kHz_G/fovx;	    
  gy=2*M_PI*(nty-1)/G_rad_G/fovy/(tgrad/1000);
  gz=2*M_PI*(ntz-1)/G_rad_G/fovz/(tgrad/1000);

  SetSpoilingFlag(entre,INACTIVE); /* No XY Spoiling after acquisition for true fisp*/
  DoWaiting(expr,entre,ATT_INIT); /* To get an initial magnetisation M0 */

  altphase=-1; /* Phase alternance at each acquisition */
  phase = 1;
  
  /* Iterated excitations to get an equilibrium state */
  for (npy = 0; npy < 10 * expr->nty; npy++)
	 {
	  DoPulseRect(expr,entre,teta * phase,timp);
	  DoWaiting(expr,entre,tr);
	  phase= phase * altphase;
	 }
   
  for (npy = 0; npy < expr->nty; npy++) {
  for (npz = 0; npz < expr->ntz; npz++)
	 {
 	   ResetToExperience(expr);
	   DoPulseRect(expr,entre,teta * phase,timp);
	   DoWaiting(expr,entre,twait); 
       gpy = (gy) * (npy - nty / 2.) / nty;  
	   gpz = (gz) * (npz - ntz / 2.) / ntz;  
	   // Simultaneous application of the negative gradient before reading (-gx) and the phase gradients gpy,gpz
       DoGradient( expr,entre,tgrad,-gx,-gpy,-gpz);
       DoAcqFrequenceX(expr,entre, +gx, npy, npz,acq_delay, phase);
       DoGradient( expr,entre,tgrad,-gx,+gpy,+gpz);
	   DoWaiting(expr,entre,twait); 
	   phase= phase * altphase;
	 }}
  printf("Ending SeqTrueFisp3D\n");  
}

