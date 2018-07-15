/*************************************************************************
* $Id: seq-tse.c,v 1.7 2006/12/18 15:10:38 yougz Exp $
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
#include "seq-tse.h"
#include "seq-se.h"

#include "event-rfpulse.h"
#include "event-precession.h"
#include "event-acquisition.h"
#include "seq-mpi.h"

void SeqTurboSpinEcho2D_0(SEQPARAM *seqparam,EXPERIENCE3D*expr);

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Required MPI overhead function that runs the SeqSpinEcho2D_0 function
* \warning     
* \param seqparam Pointer on the sequence parameter structure (Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
* \return      Pointer on the acquired RF volume 
***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE SeqTurboSpinEcho2D(SEQPARAM *seqparam,EXPERIENCE3D*expr)
{
	mpi_job(seqparam , expr, SeqTurboSpinEcho2D_0);
	NormalizeRFSignal(expr);
 	return(GetSignalRFComplexFromExperience(expr));
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Define and execute a 2D Turbo Spin Echo sequence
* \warning     
* \param seqparam Pointer on the sequence parameter structure (turbo, Te,Tr)
* \param expr     Pointer on the experience structure (tacq,ntx,nty)
***********************************************************/
void SeqTurboSpinEcho2D_0(SEQPARAM *seqparam,EXPERIENCE3D*expr)
{
  int      ntx,nty;
  double   fovx,fovy;
  double   te,tr,tacq,timp,acq_delay,tgp; /* ms */
  double   gx,gy,gz,gp;
  int	   npz;
  int      tfact;
  int      i;
  int      Nenc,M,Nex,m,n;
  int      kmn1,kmn4,kmn5,k;

  EVENT* event, event_r;
  event = &event_r;

  printf("Starting SeqTurboSpinEcho2D \n");  
  ntx = expr->ntx;
  nty = expr->nty;
  tacq = expr->readout_time*1.e+3;
  fovx = expr->fovx;
  fovy = expr->fovy;

  te = seqparam->te;
  tr = seqparam->tr;
  tfact = (int) seqparam->tfact;

  CheckTimesSE( tacq, te, tr);
  timp=0.3; /*RF pulse duration in ms */
  acq_delay = 0;
  tgp = te/2.- tacq/2;
  gx = (ntx-2.)/tacq/G_kHz_G/fovx;	    
  gy = 2*M_PI*(nty-1)/G_rad_G/fovy/(tgp/1000); 
  gz = 0.0;
  npz= 0;

  SetSpoilingFlag(event,ACTIVE); /* XY Spoiling after each line acquisition */
  i=0;
  DoWaiting(expr,event,ATT_INIT); /* To get an initial Magnetization M0 */
  for(i=0;i<10;i++) /* Boucle de repetition pour un Mz saturee des la premiere acquisition*/
   {
    DoPulseRect(expr,event, 90.,timp);
    DoWaiting(expr,event,te/2.0);
    DoPulseRect(expr,event,180.,timp);
    DoWaiting(expr,event,tr-te/2.0);
   }
  
   Nenc = nty;   /* Number of lines of the k space to acquire */
   M=tfact;      /* Number of echo per 90° RF pulse */
   Nex = Nenc/M; /* Number of excitation to repeat */
   printf("%d lines of the k space acquired in %d excitations of %d echos each \n",Nenc,Nex,M);

   for (m=1; m<= Nex; m++)  /* Excitation m */
	 {
       DoPulseRect(expr,event,90,timp);
	   DoGradient(expr,event,tacq/2,gx,0,0);
       DoWaiting(expr,event,te/2-tacq/2);
	   for (n=1;n<=M;n++)        /* Echo n */
	   {
		DoPulseRect(expr,event,180.,timp);
        /* kmn1 >> interleaving the strong magnitude echoes in the k space # k space subsampling, image periodisation */
		kmn1=n-1+(m-1)*M-Nenc/2+Nenc/2;   
		/* kmn4 -> k space filling by a step of echo energy > a maximum of energy is contained in the HF half space */
		kmn4=m-1+(n-1)*Nex-Nenc/2+Nenc/2; 
		/* kmn5 -> K space weighting by a step centered on the zero frequency */
		if (m>Nex/2) kmn5= -(m-Nex/2+(n-1)*Nex/2)+Nenc/2; 
		else    	 kmn5= (m-1+(n-1)*Nex/2)+Nenc/2;
		k=kmn5; /* K space filling choice */
		gp = (gy) * (k - Nenc / 2) / Nenc;  
 		DoGradient(expr,event,tgp,0,-gp,0);
		DoAcqFrequenceX(expr,event,gx,k,npz,acq_delay,MINUS);
		DoGradient(expr,event,tgp,0,+gp,0);
	    i++;
	   }
	  DoWaiting(expr,event,tr-((tfact)*te)-te/2.0);
	 }
  printf("Ending SeqTurboSpinEcho2D \n");  
}

