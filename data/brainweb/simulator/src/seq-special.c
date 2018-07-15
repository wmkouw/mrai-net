/*************************************************************************
* $Id: seq-special.c,v 1.7 2006/12/18 15:10:38 yougz Exp $
**************************************************************************
* This file contains some specific tests that have been done
* No header for this file, since these "old" functions have 
* not the vocation to be called in their actual design. They
* are given as memory of certain tests or exemples
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
#include "event-rfpulse.h"
#include "event-precession.h"
#include "event-acquisition.h"
#include "seq-mpi.h"
#include "output.h"
#include <idima.h>

/*!
*********************************************************
* \ingroup  sequence
* \brief    Apply a 2D spin echo sequence with selection of the central slice
*	        Old fashion style of coding sequence. Not vocation to be called in their 
*			actual design. They are given as memory of certain tests or exemples
***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE SequenceSE2D_sel(SEQPARAM *seqparam, EXPERIENCE3D* expr, OBJECT3D * objet,double epaisseur_coupe)	 
{
  double   Tacq;
  double   gx_phase,gx_frequence,dgy,agz;
  double   timp;   /* en micros */
  double   tp; /*en ms : duree d'application du gradient de phase */
  double    angle_rf1,angle_rf2; /*en degre*/
  double   t_avant_tp, t_apres_tp, t_avant_acquisition,tech,t_comp_gradz,t_crusher; /*en ms*/
  double   te,tr;
  int	   npy, npz;
  int    ntx, nty, ntz; /* taille du volume RF acquis */
  double fovx,fovy;/* taille du fov en m */
  EVENT* entre, entre_r;
  double   gamma_hg,gamma_rt;
  int ecrit;
  int nb_lobs,nb_pts_rf;

  nb_lobs=3;
  nb_pts_rf = 128;
  gamma_hg = G/2/M_PI/1000/10000; /* G en kHz/gauss # 4,26kHz/G */
  gamma_rt = G/10000;		   /* G en rad/sec/gauss #26000 */
  ecrit= 0;
  entre = &entre_r;

  ntx = expr->ntx;
  nty = expr->nty;
  ntz = expr->ntz;
  Tacq = expr->readout_time*1e3;   // duree d'acquisition en ms
  fovx = expr->fovx;
  fovy = expr->fovy;
   
  te = seqparam->te;
  tr = seqparam->tr;

  /*********************
   * Sequence Spin-Echo 2D
   * Pas de gradient de phase en z
   **********************/
   timp = 2.560; /* temps d'impulsion en ms  */
   angle_rf1 = 90.; /* angle de bascule de la premiere impulsion rf */
   angle_rf2 = 180.; /* angle de bascule de la secondesion rf */
   tp = 4.34; /* temps d'application du gradient de phase en ms */
   tech = expr->pt*1e3; /* temps d'échantillonnage en ms */

   // gx_frequence = gradient en x applique pendant la lecture
   // gx_phase = gradient en x applique en meme temps que le gradient de phase. Ce predephasage en x
   // permet d'avoir ensuite un echo centre pendant l'acquisition. Cet echo sera centre a condition
   // que gx_phase et gx_frequence soient reliés par la relation suivante :

   gx_frequence = 2*M_PI/(fovx*G*tech*1e-3)*1e4;
   gx_phase = gx_frequence/tp*(ntx+1)/2.*tech;       
   dgy=2*M_PI*(nty-1)/nty/(G*fovy*tp*1e-3)*1e4; /* pas du gradient de phase */
   
   // Gradient de sélection de coupe
   // il est calculé soit a partir du nombre de lobes, de la durée d'impulsion et de l'épaisseur de coupe
   // soit à partir de données issues de la doc Siemens (facteur 2.4 ci-après) qui correspond
   // au fichier rf utilisé

   // dans le cas d'une utilsation d'une impulsion en sinus cardinal
   //delta_f=2*nb_lobs/(timp*1e-6);
   //agz = 2*M_PI*delta_f/(G*epaisseur_coupe)*1e4;

   // dans le cas de l'utilisation du fichier de sequence siemens
   agz=2.4*10.0/(epaisseur_coupe*1e3)*10;

   // après la première impulsion rf on attend un certain temps avant d'appliquer le gradient de phase et
   // le gradient en x. On attend le meme temps ensuite avant d'appliquer la seconde impulsion rf
   t_avant_tp = 0.5*(te/2-tp-timp);
   t_apres_tp = 0.5*(te/2-tp-timp);
 
   t_comp_gradz = 0.5175*timp;
   t_crusher = 1;
   // temps d'attente entre la seconde impulsion rf et l'acquisition
   t_avant_acquisition = te/2-(ntx+1)/2.*tech-t_comp_gradz;
  
   npz=0;	
	DoWaiting(expr,entre,ATT_INIT); /* Pour avoir une aimantation initiale */
	for (npy = 0; npy < expr->nty; npy++)
	 {
		if(npy==0) MagstateWatch(expr,"etat_0.txt");
		printf("npy=%d\n",npy);
		printf("rf1\n");
		entre->agz=agz;
		//DoPulseSinc(expr,entre,angle_rf1,timp,nb_pts_rf,nb_lobs);
		DoPulseFile(expr,entre,angle_rf1,timp,"..\\data\\rf\\rf90_2.4_20mm_128_points.txt");
		DoWaiting(expr,entre,t_avant_tp); /*attente */
		DoGradient(expr,entre,(tp-t_comp_gradz)/2,gx_phase,dgy*(npy-nty/2),0.0);
		DoGradient(expr,entre,t_comp_gradz,gx_phase,dgy*(npy-nty/2),-agz); 
		DoGradient(expr,entre,(tp-t_comp_gradz)/2,gx_phase,dgy*(npy-nty/2),0.0); /*gradient de phase + lobe dephaseur en x (pour le centrage de l'echo) */
		DoWaiting(expr,entre,t_apres_tp-t_crusher); /*attente */
		DoGradient(expr,entre,t_crusher,0,0,3*agz); /*gradient de crusher*/

		printf("rf2\n");
		entre->agz=agz;
		//DoPulseSinc(expr,entre,angle_rf2,timp,nb_pts_rf,nb_lobs);
		DoPulseFile(expr,entre,angle_rf2,timp,"..\\data\\rf\\rf180_2.4_20mm_128_points.txt");
		DoGradient(expr,entre,t_crusher,0,0,3*agz); /*gradient de crusher*/
		DoWaiting(expr,entre,t_avant_acquisition-t_crusher); /* attente avant acquisition */
		DoAcqFrequenceX( expr,entre, gx_frequence, npy, npz,tech/2., MINUS); /*acquisition */
		DoGradient(expr,entre,tp,10*agz,0.0,10*agz); /* spoiler */
		DoWaiting(expr,entre,tr);
	 }
	 return(GetSignalRFComplexFromExperience(expr));
}
