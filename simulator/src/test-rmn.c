/*************************************************************************
* $Id: test-rmn.c,v 1.28 2006/12/18 15:10:38 yougz Exp $
**************************************************************************
This file contains several functions that run 1D RMN experience like FID,
Train of spin echoes, train of gradient echoes ...
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
#include "test-rmn.h"
#include <idvol.h>
#include <idacr.h>
#include "simri3d.h"
#include "experience.h"
#include "object.h"
#include "object-simple.h"
#include "output.h"
#include "display.h"
#include "event-rfpulse.h"
#include "event-precession.h"
#include "event-acquisition.h"

#ifdef HAVE_MPI
#include <mpi.h>
#endif

/*!
**********************************************************
* \ingroup     Test
* \brief       Test function of a 1D FID on an homogeneous object
* \warning     
* \param objsize Object size
* \param sigsize Reconstructed signal size (should be < object size)
*****************************************************************/
void TestFID1D(int objsize,int sigsize)
  {
    char fic[80];
	int x,y,z;
    double fovx,fovy,fovz;	
	double Tacq;            /* in s */
    int ro,t1,t2;
	double acq_delay; /* in ms */
	int tacq;
    OBJECT3D  *object;
	EXPERIENCE3D* expr;
    EVENT* event, event_r;
    PPPVOLUME_COMPLEX_DOUBLE  volrf;
	PPPVOLUME_FLOAT volrecmod,volz,voly; 
#ifdef HAVE_MPI
    int rank;
#endif
    fic[0]='\0';
	printf("Start TestFID1D \n");	
   	event = &event_r;
	expr = AllocExperience();
	x=objsize; y=1; z=1;
    t1=150; t2=200; ro=10;
	object=CreateHomogeneousObject(x,y,z,t1,t2,ro);
	SetObjectSize(object,(float)0.2,(float)0.0002,(float)0.0002);
	fovx=0.20; fovy=0.00020; fovz=0.00020;

    /* T2* weighting */
	SetObjectDeltaB0(object,0.0000001);

    SetB0Experience(expr,1.5);
    SetObjectExperienceComponent(expr,object,WATER,0);
	SetFovExperience(expr,fovx,1,1,0,0,0);     
    tacq=800; /* in ms */
	Tacq = tacq * 1.e-3; /* conversion in s */
    SetAcqExperience(expr,sigsize,1,1,Tacq);
    acq_delay=0;

	DoWaiting(expr,event,ATT_INIT);

	DoPulseRect(expr,event, 90., 300.);
	DoAcqFrequenceX(expr,event,0,0,0,acq_delay,PLUS);
	
	/* Vizualisation of the T2* decay */
	voly=GetSignalRFComponentFromExperience(expr,1);
    DisplayVolX((PPPVOLUME)voly,0,0,"Antenne Y",0,"");
	IdVolFree(voly);
    /* Vizualisation of the T1 growing*/
	volz=GetSignalRFComponentFromExperience(expr,2);
  
	event = &event_r;
	expr=AllocExperience();

    DisplayVolX((PPPVOLUME)volz,0,0,"Antenne Z",0,"");
	IdVolFree(volz);

	volrf = GetSignalRFComplexFromExperience(expr);
	FreeExperience(expr);

    volrecmod = RecVolIFFTModuleFromVol(volrf);
    volrf=NULL; 
	
#ifdef HAVE_MPI
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if (rank==0)
		WriteVolRecUchar(volrecmod,"imafid1dsimri.acr");
#else
	WriteVolRecUchar(volrecmod,"imafid1dsimri.acr");
#endif
	IdVolFree(volrecmod);
	
	printf("End TestFID1D \n");	
  }

/*!
******************************************************************
* \ingroup     Test
* \brief       Test function of a 1D spin echo on an homogeneous object
* \warning     
* \param objsize Object size
* \param sigsize Reconstructed signal size (should be < object size)
*****************************************************************/
void TestEchoSpin1D(int objsize,int sigsize)
  {
    char fic[80];
	int x,y,z;
    double fovx,fovy,fovz;	
	int ro,t1,t2;
	double acq_delay; /* in ms */
	double TE,t;
	double timp; /* in ms */
	int tacq; /* in ms */
    OBJECT3D  *object;
	EXPERIENCE3D* expr;
    EVENT* event, event_r;
    PPPVOLUME_COMPLEX_DOUBLE  volrf;
	PPPVOLUME_FLOAT volrecmod,voly; 
#ifdef HAVE_MPI
    int rank;
#endif
  
	event = &event_r;
	expr=AllocExperience();

    fic[0]='\0';
	printf("Sart EchoSpin1D \n");	

	x=objsize; y=1; z=1;
    t1=500; t2=300; ro=77;
	object=CreateHomogeneousObject(x,y,z,t1,t2,ro);
	SetObjectSize(object,(float)0.2,(float)0.0002,(float)0.0002);
	fovx=0.20; fovy=0.00020; fovz=0.00020;
    
	SetObjectDeltaB0(object,1.e-5);
    SetB0Experience(expr,1.);
    SetObjectExperienceComponent(expr,object,WATER,0);
	SetFovExperience(expr,fovx,1,1,0,0,0);     
    tacq=10; /* en ms */
	SetAcqExperience(expr,sigsize,1,1,(tacq * 1.e-3));
    acq_delay=0.;
    
	timp=300.;
	TE=20.;
	t=TE/2; /* equals TE/2 in ms */

	DoWaiting(expr,event,ATT_INIT);
	printf("Mo=%lf \n",expr->magstate[2][objsize/2][0][0]*objsize);

	DoPulseRect(expr,event, 90.,timp);
	DoWaiting(expr,event,t-timp*1.e-3);
	DoPulseRect(expr,event, 180.,timp);
	DoWaiting(expr,event,t+timp*1.e-3-(tacq/2.+tacq/2./sigsize));
  
	DoAcqFrequenceX(expr,event,0,0,0,acq_delay,MINUS);
	
	voly=GetSignalRFComponentFromExperience(expr,1);
    DisplayVolX((PPPVOLUME)voly,0,0,"Antenne Y",0,"");
	IdVolFree(voly);

	volrf = GetSignalRFComplexFromExperience(expr);
	FreeExperience(expr);

    volrecmod = RecVolIFFTModuleFromVol(volrf);
    volrf=NULL; 
	
#ifdef HAVE_MPI
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if (rank==0)
		WriteVolRecUchar(volrecmod,"imafid1dsimri.acr");
#else
	WriteVolRecUchar(volrecmod,"imafid1dsimri.acr");
#endif
	IdVolFree(volrecmod);
	
	printf("End TestEchoSpin1D \n");	
  }

/*!
******************************************************************
* \ingroup     Test
* \brief       Test function of a 1D gradient echo on an homogeneous object
* \warning     
* \param objsize Object size
* \param sigsize Reconstructed signal size (should be < object size)
*****************************************************************/
void TestEchoGradient1D(int objsize,int sigsize)
  {
    char fic[80];
	int x,y,z;
    double fovx,fovy,fovz;	
	int ro,t1,t2;
	double acq_delay; /* in ms */
	double tgp,TE,teta,gx;
	double timp; /* in ms */
	int tacq; /* in ms */
    OBJECT3D  *object;
	EXPERIENCE3D* expr;
    EVENT* event, event_r;
    PPPVOLUME_COMPLEX_DOUBLE  volrf;
	PPPVOLUME_FLOAT volrecmod,voly; 
#ifdef HAVE_MPI
    int rank;
#endif
  
	event = &event_r;
	expr=AllocExperience();

    fic[0]='\0';
	printf("Sart EchoGradient1D \n");	

	x=objsize; y=1; z=1;
    t1=500; t2=300; ro=77;
	object=CreateHomogeneousObject(x,y,z,t1,t2,ro);
	SetObjectSize(object,(float)0.2,(float)0.0002,(float)0.0002);
	fovx=0.20; fovy=0.00020; fovz=0.00020;
    
	SetObjectDeltaB0(object,1.e-6);
    SetB0Experience(expr,1.);
    SetObjectExperienceComponent(expr,object,WATER,0);
	SetFovExperience(expr,fovx,1,1,0,0,0);     
    tacq=10; /* in ms */
	SetAcqExperience(expr,sigsize,1,1,(tacq * 1.e-3));
    acq_delay=0.;
	gx=(sigsize-2)/tacq/G_kHz_G/fovx;
	gx = gx * 1.0; /* Gradient effect weighting on the decay */
	timp=300.;

	TE=20.;
	tgp=TE-timp*1.e-3-tacq/2.0-tacq/2.0; 
    teta=90.0;

	DoWaiting(expr,event,ATT_INIT);
	printf("Mo=%lf \n",expr->magstate[2][objsize/2][0][0]*objsize);
	
	DoPulseRect(expr,event,teta,timp);
	DoWaiting(expr,event,tgp);
	DoGradient(expr,event,tacq/2.0,-gx,0.0,0.0);
	DoAcqFrequenceX(expr,event,gx,0,0,acq_delay,PLUS);
	
	voly=GetSignalRFComponentFromExperience(expr,1);
    DisplayVolX((PPPVOLUME)voly,0,0,"Antenne Y",0,"");
	IdVolFree(voly);

	volrf = GetSignalRFComplexFromExperience(expr);
	FreeExperience(expr);

    volrecmod = RecVolIFFTModuleFromVol(volrf);
    volrf=NULL; 
	
#ifdef HAVE_MPI
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if (rank==0)
		WriteVolRecUchar(volrecmod,"imafid1dsimri.acr");
#else
	WriteVolRecUchar(volrecmod,"imafid1dsimri.acr");
#endif
	IdVolFree(volrecmod);
	
	printf("End TestEchoGradient1D \n");	
  }

/*!
**********************************************************
* \ingroup     Test
* \brief       Test function of a spin echo train on an homogeneous object 
* \warning     
* \param objsize Object size
* \param sigsize Reconstructed signal size (should be < object size)
*****************************************************************/
void TestTrainEchoSpin1D(int objsize,int sigsize,int nbecho)
  {
    char fic[80];
	int x,y,z,i;
    double fovx,fovy,fovz;
	int ro,t1,t2;
	double acq_delay; /* in ms */
	double TE,t;
	double timp; /* in ms */
	int tacq; /* in ms */
    OBJECT3D  *object;
	EXPERIENCE3D* expr;
    EVENT* event, event_r;
	PPPVOLUME_FLOAT voly; 
#ifdef HAVE_MPI
    int rank;
#endif
  
	event = &event_r;
	expr=AllocExperience();

    fic[0]='\0';
	printf("Start TrainEchoSpin1D \n");	

	x=objsize; y=1; z=1;
    t1=250; t2=300; ro=10;
	object=CreateHomogeneousObject(x,y,z,t1,t2,ro);
	SetObjectSize(object,(float)0.2,(float)0.0002,(float)0.0002);
	fovx=0.20; fovy=0.00020; fovz=0.00020;
    
	SetObjectDeltaB0(object,0.00001);
    SetB0Experience(expr,1.);
    SetObjectExperienceComponent(expr,object,WATER,0);
	SetFovExperience(expr,fovx,1,1,0,0,0);     
    tacq=10; /* in ms */
	SetAcqExperience(expr,sigsize,1,1,(tacq * 1.e-3));
    acq_delay=0.;
    
	timp=3.;
	TE=100.;
	t=TE/2; /* corresponds to TE/2 in ms */

	DoWaiting(expr,event,ATT_INIT);
	
	DoPulseRect(expr,event, 90.,timp);
	DoWaiting(expr,event,t-timp*1.e-3);
	
	for (i=1;i<=nbecho;i++)
	{
	 DoPulseRect(expr,event, 180.,timp);
	 DoWaiting(expr,event,t+timp*1.e-3-(tacq/2.+tacq/2./sigsize));
  	 DoAcqFrequenceX(expr,event,0,0,0,acq_delay,MINUS);
	 DoWaiting(expr,event,t+timp*1.e-3-(tacq/2.+tacq/2./sigsize));

	 voly=GetSignalRFComponentFromExperience(expr,1);
	 IdVolFree(voly);
	}
	printf("End TestTrainEchoSpin1D \n");	
  }

/*!
**********************************************************
* \ingroup     Test
* \brief       Test function of a gradient echo train on an homogeneous object 
*  Each echo has a decay in T2** and the train echo decays in T2*
* \warning     
* \param objsize Object size
* \param sigsize Reconstructed signal size (should be < object size)
*****************************************************************/
void TestTrainEchoGradient1D(int objsize,int sigsize,int nbecho)
  {
    char fic[80];
	int x,y,z,i;
    double fovx,fovy,fovz;
	int ro,t1,t2,s;
	double acq_delay; /* in ms */
	double TE;
	double timp,gx,teta,tgp; /* in ms */
	int tacq; /* in ms */
    OBJECT3D  *object;
	EXPERIENCE3D* expr;
    EVENT* event, event_r;
	PPPVOLUME_FLOAT voly; 
#ifdef HAVE_MPI
    int rank;
#endif
  
	event = &event_r;
	expr=AllocExperience();

    fic[0]='\0';
	printf("Start TrainEchoGradient1D \n");	

	x=objsize; y=1; z=1;
    t1=500; t2=300; ro=77;
	object=CreateHomogeneousObject(x,y,z,t1,t2,ro);
	SetObjectSize(object,(float)0.2,(float)0.0002,(float)0.0002);
	fovx=0.20; fovy=0.00020; fovz=0.00020;
    
	SetObjectDeltaB0(object,1.e-6);
    SetB0Experience(expr,1.);
	/* */
    SetObjectExperienceComponent(expr,object,WATER,0);
	SetFovExperience(expr,fovx,1,1,0,0,0);     
    tacq=10; /* en ms */
	SetAcqExperience(expr,sigsize,1,1,(tacq * 1.e-3));
    acq_delay=0.;
	gx=(sigsize-2)/tacq/G_kHz_G/fovx;
	timp=300.;

	TE=20.;
	tgp=TE-timp*1.e-3-tacq/2.0-tacq/2.0; 
    teta=90.0;

	DoWaiting(expr,event,ATT_INIT);
	printf("Mo=%lf \n",expr->magstate[2][objsize/2][0][0]*objsize);
	
	DoPulseRect(expr,event,teta,timp);
	DoWaiting(expr,event,tgp);
	DoGradient(expr,event,tacq/2.0,-gx,0.0,0.0);
	DoAcqFrequenceX(expr,event,gx,0,0,acq_delay,PLUS);
    voly=GetSignalRFComponentFromExperience(expr,1);
	IdVolFree(voly);	
	s=1;
	for (i=1;i<=nbecho;i++)
	{
	 s= s*(-1);
	 DoAcqFrequenceX(expr,event,s*gx,0,0,acq_delay,PLUS);
	 getchar();
	 voly=GetSignalRFComponentFromExperience(expr,1);
	 IdVolFree(voly);
	}
	
	printf("End TestTrainEchoGradient1D \n");	
  }

/*!
**********************************************************
* \ingroup     Test
* \brief       Test function of siùmulated echo with 3 RF pulses
* \warning The actual model doesn't enable simulated echo simulation
* \param objsize Object size
* \param sigsize Reconstructed signal size (should be < object size)
*****************************************************************/
void TestSimulatedEcho(int objsize,int sigsize)
  {
    char fic[80];
	int x,y,z;
    double fovx,fovy,fovz;
	int ro,t1,t2;
	double acq_delay; /* in ms */
	double TE,t,TM;
	double timp; /* in ms */
	double alpha1,alpha2,alpha3;
	int tacq; /* in ms */
    OBJECT3D  *object;
	EXPERIENCE3D* expr;
    EVENT* event, event_r;
	PPPVOLUME_FLOAT voly; 
#ifdef HAVE_MPI
    int rank;
#endif

	
	event = &event_r;
	expr=AllocExperience();

    fic[0]='\0';
	printf("Start TestSimulatedEcho \n");	

	x=objsize; y=1; z=1;
    t1=250; t2=300; ro=10;
	object=CreateHomogeneousObject(x,y,z,t1,t2,ro);
	SetObjectSize(object,(float)0.2,(float)0.0002,(float)0.0002);
	fovx=0.20; fovy=0.00020; fovz=0.00020;
    
	SetObjectDeltaB0(object,0.00001);
    SetB0Experience(expr,1.5);
    SetObjectExperienceComponent(expr,object,WATER,0);
	SetFovExperience(expr,fovx,1,1,0,0,0);     
    tacq=10; /* in ms */
	SetAcqExperience(expr,sigsize,1,1,(tacq * 1.e-3));
    
    acq_delay=0.;
	timp=3.;
	alpha1=90.;
	alpha2=180;
	alpha3=180;

	TE=80.;
	TM=100.; /* Mixin gtime */
	t=TE/2; /* corresponds to TE/2 in ms */

	DoWaiting(expr,event,ATT_INIT);
	
	DoPulseRect(expr,event,alpha1,timp);
	DoWaiting(expr,event,t-timp*1.e-3); /* waiting of TE/2 */
	DoPulseRect(expr,event,alpha2,timp);
    DoWaiting(expr,event,t+timp*1.e-3-(tacq/2.+tacq/2./sigsize));
	DoAcqFrequenceX(expr,event,0,0,0,acq_delay,MINUS);
		 voly=GetSignalRFComponentFromExperience(expr,1);
		 //DisplayVolX((PPPVOLUME)voly,0,0,"Echo spin 1-2",0,"");
		 IdVolFree(voly);

	DoWaiting(expr,event,TM-(t+tacq/2.));  /* 55 */
	DoPulseRect(expr,event,alpha3,timp);
    DoWaiting(expr,event,t+timp*1.e-3-(tacq/2.+tacq/2./sigsize)); /* 35 */
	DoAcqFrequenceX(expr,event,0,0,0,acq_delay,MINUS);
		 voly=GetSignalRFComponentFromExperience(expr,1);
		 //DisplayVolX((PPPVOLUME)voly,0,0,"Echo stimule 1-2-3",0,"");
		 IdVolFree(voly);
    
	DoWaiting(expr,event,TM-t-(tacq/2.)-t-tacq/2.0);  /*10*/
	DoAcqFrequenceX(expr,event,0,0,0,acq_delay,MINUS);
		 voly=GetSignalRFComponentFromExperience(expr,1);
		 //DisplayVolX((PPPVOLUME)voly,0,0,"Echo spin 1-2-3",0,"");
		 IdVolFree(voly);
    
	DoWaiting(expr,event,t-tacq/2.0-(tacq/2.+tacq/2./sigsize)); /* 30 */
	DoAcqFrequenceX(expr,event,0,0,0,acq_delay,MINUS);
		 voly=GetSignalRFComponentFromExperience(expr,1);
		 //DisplayVolX((PPPVOLUME)voly,0,0,"Echo spin 2-3",0,"");
		 IdVolFree(voly);
    
	DoWaiting(expr,event,t-tacq/2.0-(tacq/2.+tacq/2./sigsize)); /* 30 */
	DoAcqFrequenceX(expr,event,0,0,0,acq_delay,MINUS);
		 voly=GetSignalRFComponentFromExperience(expr,1);
		 //DisplayVolX((PPPVOLUME)voly,0,0,"Echo spin 1-3",0,"");
		 IdVolFree(voly);

	printf("End TestSimulatedEcho \n");	
  }
