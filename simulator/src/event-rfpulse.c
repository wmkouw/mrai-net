/*************************************************************************
* $Id: event-rfpulse.c,v 1.8 2006/12/18 15:10:36 yougz Exp $
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
/*!
*********************************************************
* \ingroup     Event
* \brief       Apply a rectangular RF pulse
* \warning     
* \param expr  Pointer on the experience structure
* \param angle Pulse flip angle in degree
* \param duration Duration of the pulse in ms
 ***********************************************************/
void DoPulseRect(EXPERIENCE3D * expr,EVENT * event, double angle, double duration)
{
		double agz;

	    if (angle==90.) /* T2* weighting init */
			ResetToExperience(expr);

	    agz = event->agz;	// saving the gradient value because event->agz set to zero in init_evente3D
	    InitEvent(event);
		event->agz = agz;
	    event->ang = angle;
	    event->pulse_duration = duration;
	    Conversion(event);
	    Kernel(expr, event);
}

/*!
*********************************************************
* \ingroup     Event
* \brief       Apply a sinc shaped RF pulse
* \warning     
* \param expr  Pointer on the experience structure
* \param angle Pulse flip angle in degree
* \param duration Duration of the pulse in ms
* \param nb_lobs Number of lobes in the sinc
* \param nb_points Number of points used to define the sinc
 ***********************************************************/
void DoPulseSinc(EXPERIENCE3D * expr,EVENT * event, double angle, double duration, int nb_points, int nb_lobs)
{
	    
  double *fpul;//save the basic rectangular RF pulse used to define sinc shaped pulse
  int ip;
  double b1,fact,som,B,A,trp,b1v,angle_elementaire,agz;
  FILE *fic;

  agz=event->agz;// saving the gradient value because event->agz set to zero in init_evente3D

	// Computation of the table containing the set of nb_points rectangular RF pulse defining the sinc one
	// Of the rectangular pulse have the same duration (duration/nb_points) and their amplitude vary
	// The angle sum is equal to the global angle
  fpul=(double *)malloc(nb_points*sizeof(double));
  som=0;
  A = duration/2.0;
  B=nb_lobs*M_PI/A;
  for(ip=0;ip<nb_points;ip++)
   {
	   trp=ip*duration/nb_points;
	   if(trp==A) fpul[ip]=1;
	   else fpul[ip]=sin(B*(trp-A))/(B*(trp-A));
	   som=som+fpul[ip];
   }
  fact=nb_points/som;
  
   // Temporary saving
  fic=fopen("sinus.txt","w");
  for(ip=0;ip<nb_points;ip++) fprintf(fic,"%f\n",fpul[ip]);
  fclose(fic);

	// Applying the nb_points pulses
  agz=event->agz;
  b1=angle/(G*duration*1e-6)*fact;
	for(ip=0;ip<nb_points;ip=ip++) 
	{
		b1v=b1*fpul[ip];
		angle_elementaire=b1v*G*duration*1e-6/nb_points;
		event->agz=agz;
		DoPulseRect(expr,event, angle_elementaire, duration/nb_points);
	}
  free(fpul);
}


/*!
*********************************************************
* \ingroup     Event
* \brief       Apply an RF pulse defined by a set of rectangular pulses saved in a file
* \warning     
* \param expr  Pointer on the experience structure
* \param angle Pulse flip angle in degree
* \param duration Duration of the pulse in ms
* \param filename Text file name containing the list of RF pulses
*  The first line of this files gives the number of pulses.See examples in ../data
 ***********************************************************/
void DoPulseFile(EXPERIENCE3D * expr,EVENT * event, double angle, double duration, char *filename)
{
	FILE *fic;
	int ip;
	double b1,fact,som,b1v,angle_elementaire,agz;
	double   *fpul;
	int nb_points;

	agz=event->agz; // saving the gradient value because event->agz set to zero in init_evente3D

    fic=fopen(filename,"r");
	if(fic==NULL)
	{
		printf("Unable to open the file %s containing the RF pulses! \n",filename);
		exit(-1);
	}

   fscanf(fic,"%d ",&nb_points);
   fpul=(double *)malloc(nb_points*sizeof(double));
   for(ip=0;ip<nb_points;ip++) fscanf(fic,"%lf ",&fpul[ip]);
	
   fclose(fic);
	// Normalisation
   som=0;
   for(ip=0;ip<nb_points;ip++) som=som+fpul[ip];
   fact=nb_points/som;

	// Apply the pulses
   agz=event->agz;
   som=0;
   b1=angle/(G*duration*1e-6)*fact;
   for(ip=0;ip<nb_points;ip=ip++) 
	{
		b1v=b1*fpul[ip];
		angle_elementaire=b1v*G*duration*1e-6/nb_points;
		event->agz=agz;
		printf("ipnum=%d : %d \n",ip,nb_points);
		DoPulseRect(expr,event, angle_elementaire, duration/nb_points); 
		som=som+angle_elementaire;
	}
   free(fpul);
}

