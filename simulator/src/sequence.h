/*************************************************************************
* $Id: sequence.h,v 1.18 2006/12/18 15:10:38 yougz Exp $
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
#ifndef _SEQUENCE_H
#define _SEQUENCE_H

#include <stdio.h>
#include <idvol.h>

#include <experience.h>
#include <object.h>

/*!\typedef SEQPARAM Data structure containing the sequence parameters */
typedef struct _SeqParam {
    /*! Echo time in ms */
	double te;
	/*! Repetition time in ms */
	double tr;
     /*! Inversion time in ms for inversion/recuperation sequence*/
	double ti;
     /*! RF pulse angle in degree for gradient echo sequence */
	double teta;
     /*! Turbo factor for multiple echo sequences */
	double tfact;
} SEQPARAM; 

/*!\typedef PSEQ pointer on sequence function */
typedef PPPVOLUME_COMPLEX_DOUBLE (*PSEQ)(SEQPARAM *,EXPERIENCE3D*);

/***************************************************
* Basic function prototypes
***************************************************/
void SetSeqParamTE(SEQPARAM *,double);
void SetSeqParamTR(SEQPARAM *,double);
void SetSeqParamTI(SEQPARAM *,double);
void SetSeqParamTeta(SEQPARAM *,double);
void SetSeqParamTfact(SEQPARAM *,double);

/***************************************************
* Prototypes of all the sequence functions
***************************************************/
/* Definition in seq-se.c */
PPPVOLUME_COMPLEX_DOUBLE SeqSpinEcho1D(SEQPARAM *,EXPERIENCE3D*);
PPPVOLUME_COMPLEX_DOUBLE SeqSpinEcho2D(SEQPARAM *,EXPERIENCE3D*);
PPPVOLUME_COMPLEX_DOUBLE SeqSpinEcho3D(SEQPARAM *,EXPERIENCE3D*);
/* Definition in seq-ge.c */
PPPVOLUME_COMPLEX_DOUBLE SeqGradientEcho1D(SEQPARAM *,EXPERIENCE3D*);
PPPVOLUME_COMPLEX_DOUBLE SeqGradientEcho2D(SEQPARAM *,EXPERIENCE3D*);
PPPVOLUME_COMPLEX_DOUBLE SeqGradientEcho3D(SEQPARAM *,EXPERIENCE3D*);
/* Definition in seq-tf.c */
PPPVOLUME_COMPLEX_DOUBLE SeqTrueFisp1D(SEQPARAM *,EXPERIENCE3D*);
PPPVOLUME_COMPLEX_DOUBLE SeqTrueFisp2D(SEQPARAM *,EXPERIENCE3D*);
PPPVOLUME_COMPLEX_DOUBLE SeqTrueFisp3D(SEQPARAM *,EXPERIENCE3D*);
/* Definition in seq-ir.c */
PPPVOLUME_COMPLEX_DOUBLE SeqInversionRecuperation1D(SEQPARAM *,EXPERIENCE3D*);
PPPVOLUME_COMPLEX_DOUBLE SeqInversionRecuperation2D(SEQPARAM *,EXPERIENCE3D*);
PPPVOLUME_COMPLEX_DOUBLE SeqInversionRecuperation3D(SEQPARAM *,EXPERIENCE3D*);
/* Definition in seq-fid.c */
PPPVOLUME_COMPLEX_DOUBLE SeqFreeInduction1D(SEQPARAM *,EXPERIENCE3D*);
/* Definition in seq-tse.c */
PPPVOLUME_COMPLEX_DOUBLE SeqTurboSpinEcho2D(SEQPARAM *,EXPERIENCE3D*);


PPPVOLUME_COMPLEX_DOUBLE SeqTestSE2D(SEQPARAM *,EXPERIENCE3D*);

PPPVOLUME_COMPLEX_DOUBLE RunSequence(char *,SEQPARAM *,OBJECT3D *,EXPERIENCE3D*);
PSEQ GetSequenceByName (char *);

/*!\typedef SEQFUNC Structure associating a sequence name with a sequence code and sequence function pointer */
typedef struct _SeqFunc {
    /*! Sequence code */
	int seqnum;
	/*! Sequence name */
	char seqname[80];
     /*! Pointer on the C correponding function */
	PSEQ pseq;
} SEQFUNC; 

#endif
