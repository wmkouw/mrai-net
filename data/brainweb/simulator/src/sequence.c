/*************************************************************************
* $Id: sequence.c,v 1.26 2006/12/18 15:10:38 yougz Exp $
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
#include "sequence.h"
#include <idvol.h>
#include <string.h>
#include "event-rfpulse.h"
#include "event-precession.h"
#include "event-acquisition.h"

/*!
**********************************************************
* \ingroup     Sequence
* \brief	   Make the association between a sequence name and the corresponding function pointer
* \warning
* \param seqname Sequence name
* \return Pointer on the corresponding sequence function
 ***********************************************************/
PSEQ GetSequenceByName (char *seqname)
{
 int i;
 int ok;

 #define NSEQ 14
 SEQFUNC seqlist[NSEQ]={
	{13,"TS2D",SeqTurboSpinEcho2D},
	{12,"FID1",SeqFreeInduction1D},
	{11,"IR3D",SeqInversionRecuperation3D},
	{10,"IR2D",SeqInversionRecuperation2D},
	{9,"IR1D",SeqInversionRecuperation1D},
	{8,"TF3D",SeqTrueFisp3D},
	{7,"TF2D",SeqTrueFisp2D},
	{6,"TF1D",SeqTrueFisp1D},
	{5,"GE3D",SeqGradientEcho3D},
	{4,"GE2D",SeqGradientEcho2D},
	{3,"GE1D",SeqGradientEcho1D},
	{2,"SE3D",SeqSpinEcho3D},
	{1,"SE2D",SeqSpinEcho2D},
	{0,"SE1D",SeqSpinEcho1D}
	};
 i=0;
 ok=-1;

 while ((i<NSEQ)&&(ok==-1))
 {
  if (strcmp(seqname,seqlist[i].seqname)==0)
	  ok=i;
  i++;
 }
 printf("ok=%d\n",ok);
 return (seqlist[ok].pseq);
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Apply an MRI sequence with given sequence parameters on a given object using given experiment parameters
* \warning
* \param seqname Sequence name
* \param seqparam Pointer on the sequence parameter structure
* \param objet Pointer on the object
* \param expr Pointer on the experience structure
* \return Pointer on the RF volume obtained
 ***********************************************************/
PPPVOLUME_COMPLEX_DOUBLE
	RunSequence(char * seqname, SEQPARAM *seqparam, OBJECT3D * objet, EXPERIENCE3D* expr)
{
 int i,j,k;
 PPPVOLUME_COMPLEX_DOUBLE volrf,volrfacu;
 PSEQ pseq;

/*******************************************************************************************************/
// Alexis Amadon, December 2005
// Introduce B1(r) inhomogeneities

if (expr->inhomogeneousB1)
   SetB1(expr);

/*******************************************************************************************************/


 pseq = GetSequenceByName(seqname);


 volrfacu = (PPPVOLUME_COMPLEX_DOUBLE) IdVolAlloc(expr->ntx,expr->nty,expr->ntz,VOL_COMPLEX_DOUBLE);
 if (!volrfacu)
   {
	FPRINTF(stderr,"Unable to allocate volrfacu!!\n");
    exit(0);
   }
 for(i=0;i<expr->ntx;i++) for (j=0;j<expr->nty;j++) for (k=0;k<expr->ntz;k++)
  {
	volrfacu[k][j][i].re = 0.0;
	volrfacu[k][j][i].im = 0.0;
  }

 for (i=0;i<objet->nbcomponent;i++)
   {
    SetObjectActiveComponent(objet,i);
    SetObjectExperienceComponent(expr,objet,i,expr->FLAG_DECH);
	volrf = pseq(seqparam,expr);
    AddComplexVolRF(expr,volrfacu,volrf);
	IdVolFree(volrf);
	}

 return(volrfacu);
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Define the Echo time within the sequence parameter structure
* \warning
* \param seqparam Pointer on the sequence parameter structure
* \param te    Echo time in ms
***********************************************************/
void SetSeqParamTE(SEQPARAM *seqparam,double te)
{
	seqparam->te = te;
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Define the Repetition time within the sequence parameter structure
* \warning
* \param seqparam Pointer on the sequence parameter structure
* \param tr Repetition time in ms
***********************************************************/
void SetSeqParamTR(SEQPARAM *seqparam,double tr)
{
	seqparam->tr = tr;
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Define the Inversion time within the sequence parameter structure
* \warning
* \param seqparam Pointer on the sequence parameter structure
* \param ti    Inversion time in ms
***********************************************************/
void SetSeqParamTI(SEQPARAM *seqparam,double ti)
{
	seqparam->ti = ti;
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Define the RF pulse angle within the sequence parameter structure
* \warning
* \param seqparam Pointer on the sequence parameter structure
* \param teta RF pulse angle in degree
***********************************************************/
void SetSeqParamTeta(SEQPARAM *seqparam,double teta)
{
	seqparam->teta = teta;
}

/*!
**********************************************************
* \ingroup     Sequence
* \brief       Define the turbo factor within the sequence parameter structure
* \warning
* \param seqparam Pointer on the sequence parameter structure
* \param tfact Turbo factor value
***********************************************************/
void SetSeqParamTfact(SEQPARAM *seqparam,double tfact)
{
	seqparam->tfact = tfact;
}
