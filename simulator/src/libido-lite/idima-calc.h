/*************************************************************************
* $Id: idima-calc.h,v 1.1 2005/09/09 08:22:31 bellet Exp $
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
  
  Version 1.0  05/09/2005
*************************************************************************/

/*************************************************************************
*
*  Description : Declarations diverses permettant d'utiliser le type IMAGE
*                de la bibliotheque LIBIDO
*
**************************************************************************/

#ifndef __IDIMA_CALC_H__
#define __IDIMA_CALC_H__

#include <stdio.h>
#include "idgen.h"
#include "idsig.h"
#include "idlut.h"
#include "idicont.h"


#include "idima-ido.h"



// Other stuff
int  IdImaGetBit (char **,int,int);
void IdImaPutBit (char **,int,int,int);

////////////////////////////////////////////////////////////////////////////

#include "idima-restricted.h"

PPIMAGE        IdImaClear          (PPIMAGE);
PPIMAGE        IdImaCopy           (PPIMAGE,PPIMAGE);
PPIMAGE        IdImaClone          (PPIMAGE);
PPIMAGE        IdImaAllocLikeImage (void *);
PPIMAGE        IdImaAllocSubImage  (PPIMAGE, int, int, int, int);

#ifndef SWIG
#define  IdImaFreeSubImage(pi)  _IdImaFreeSubImage((PPIMAGE*)&(pi) )
#else
void IdImaFreeSubImage(PPIMAGE);
#endif



void IdImaDisplay	(PPIMAGE_UCHAR ,int,int,int,int ,int,int,int);
void IdImaDisplayColor	(PPIMAGE_UCHAR ,int,int,int,int ,int,int,
			int,PSIGNAL_RGB);
void IdImaDisplayQ	(PPIMAGE_UCHAR ,int);
void IdImaDisplayColorQ	(PPIMAGE_UCHAR,int,PSIGNAL_RGB);


////////////////// Type checking and comparisons
PPIMAGE        IdImaCheckTypeSizeAlloc   (PPIMAGE,int,PPIMAGE);
PPIMAGE        IdImaCheckSizeTypeAlloc   (PPIMAGE,int,PPIMAGE);
PPIMAGE        IdImaCheckSizeAlloc       (PPIMAGE,int,PPIMAGE);
PPIMAGE        IdImaCheckTypeAlloc       (PPIMAGE,int,PPIMAGE);
PPIMAGE        IdImaCheckSizeAllocType   (PPIMAGE,int,PPIMAGE,int);
PPIMAGE        IdImaCheckTypeAllocSize   (PPIMAGE,int,PPIMAGE,int,int);
int            IdImaSameSizeAndType      (void *,void *);
int            IdImaSameSize             (void *,void *);
int            IdImaSameType             (void *,void *);

///////////////// Coercion (or convertion)
PPIMAGE        IdImaCast        (PPIMAGE,PPIMAGE);
PPIMAGE        IdImaCastReal    (PPIMAGE,PPIMAGE);
PPIMAGE        IdImaCastComplex (PPIMAGE,PPIMAGE);

/////////////// IO (various formats) related
PPIMAGE        IdImaReadRawFileWithOffset (char*, int, int, int, int);
PPIMAGE        IdImaReadRawFile           (char*, int, int, int);
int            IdImaReadRawFileToIma	  (char *, PPIMAGE);
int            IdImaReadRawFileToImaWithOffset(char *, PPIMAGE, int);
int            IdImaWriteRawFile          (char *, PPIMAGE);
PPIMAGE_UCHAR  IdImaReadPCXFile           (char *);
int            IdImaWritePCXFile          (char *, PPIMAGE_UCHAR);
PPIMAGE        IdImaReadTGAFile           (char *, int);
int            IdImaWriteTGAFile          (char *, PPIMAGE);
PPIMAGE        IdImaReadBMPFileWithLut    (char *,RGB *);
PPIMAGE        IdImaReadBMPFile           (char *);
PPIMAGE_UCHAR  IdImaReadBMPFileToUCHAR    (char *);
int            IdImaReadBMPLutFromFile    (char *,RGB *);
int            IdImaWriteBMPColorFile     (char *, PPIMAGE, RGB *);
int            IdImaWriteBMPFile          (char *, PPIMAGE);
PPIMAGE        IdImaReadGifFile           (char *,int, PSIGNAL_RGB * OUTPUT);
int            IdImaWriteGifFile          (char *,PPIMAGE_UCHAR, PSIGNAL_RGB);
int            IdImaToPs                  (PPIMAGE_UCHAR,char *,int,int,double,
                                           double,double,double);
///////////////// Histogram related 
long int       IdImaHistogram                (PPIMAGE_UCHAR, long int *,
                                              int, int, int, int);
PSIGNAL_FLOAT  IdImaHisto                    (PPIMAGE);
PPIMAGE_UCHAR  IdImaHistoCreerIma            (PSIGNAL_FLOAT);
double         IdHistoThresholdCl            (PSIGNAL_FLOAT);
double         IdHistoThresholdGauss         (long *, int, int *);
double         IdHistoThresholdEntropy       (long *, int, int *);
double         IdHistoThresholdVariance      (long * , int , int *);
PPIMAGE_UCHAR  IdImaScaleHistogram           (PPIMAGE_UCHAR, PPIMAGE_UCHAR BOTH,
                                              int, int);
PPIMAGE_UCHAR  IdImaHistogramLinTransform    (PPIMAGE_UCHAR, PPIMAGE_UCHAR,
                                              double, double);
PPIMAGE_UCHAR  IdImaHistogramLogTransform    (PPIMAGE_UCHAR, PPIMAGE_UCHAR);
PPIMAGE_UCHAR  IdImaHistogramCubeRootTransform  
                                             (PPIMAGE_UCHAR, PPIMAGE_UCHAR);
PPIMAGE_UCHAR  IdImaHistogramExponentialTransform
                                             (PPIMAGE_UCHAR, PPIMAGE_UCHAR,
                                              double);
long int       IdImaCumuledHistogram         (PPIMAGE_UCHAR, long int *,
                                              int, int, int, int);
PPIMAGE_UCHAR  IdImaEqualizeHistogram        (PPIMAGE_UCHAR, PPIMAGE_UCHAR);
PPIMAGE_UCHAR  IdImaInverseEqualizeHistogram (PPIMAGE_UCHAR, PPIMAGE_UCHAR);
PPIMAGE        IdImaStrechHisto              (PPIMAGE, PPIMAGE, double, double);
PPIMAGE        IdImaStrechHistoGene          (PPIMAGE, PPIMAGE, double, double,
                                              double, double);
PPIMAGE        IdImaStrechHistoGeneBin       (PPIMAGE, PPIMAGE, double, double,
                                              double, double, double);
PPIMAGE_UCHAR  IdImaHistoBidimOld	     (PPIMAGE_UCHAR, PPIMAGE_UCHAR, 
					      PPIMAGE_UCHAR);

//////////////// Contour related
PPIMAGE_UCHAR  IdImaContour                  (PPIMAGE_UCHAR, PPIMAGE_UCHAR,
                                              int, unsigned char*, int, int);
double         IdImaContourError             (PPIMAGE_UCHAR, PPIMAGE_UCHAR,
                                              long int*);
double         IdImaContourErrorSymetric     (PPIMAGE_UCHAR, PPIMAGE_UCHAR);


////////////////////////
PPIMAGE	       IdImaScale       (PPIMAGE, PPIMAGE);
PPIMAGE_UCHAR  IdImaMerge       (PPIMAGE_UCHAR ,PPIMAGE_UCHAR, int ,int);
int            IdImaConvolve    (PPIMAGE, PPIMAGE_FLOAT, PPIMAGE,
                                 double, double);
PPIMAGE_UCHAR  IdImaDilate      (PPIMAGE_UCHAR , PPIMAGE_UCHAR, int, int);
PPIMAGE_UCHAR  IdImaNDilate     (PPIMAGE_UCHAR , PPIMAGE_UCHAR, int, int, int);
PPIMAGE_UCHAR  IdImaErode       (PPIMAGE_UCHAR , PPIMAGE_UCHAR, int, int);
PPIMAGE_UCHAR  IdImaNErode      (PPIMAGE_UCHAR , PPIMAGE_UCHAR, int, int, int);
PPIMAGE_UCHAR  IdImaThin        (PPIMAGE_UCHAR,PPIMAGE_UCHAR,int,int);
PPIMAGE_UCHAR  IdImaThick       (PPIMAGE_UCHAR,PPIMAGE_UCHAR, int,int,int *);

PPIMAGE_UCHAR  IdImaClahe       (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int, long int);
PPIMAGE_LONG   IdImaCooccu      (PPIMAGE_UCHAR, int, int, int, int, int, int,
                                 int, int);
float *        IdImaCooc_param  (PPIMAGE_LONG);
int            IdImaSetPlanRGB  (PPIMAGE_RGB , PPIMAGE_UCHAR, int);
PPIMAGE_UCHAR  IdImaGetPlanRGB  (PPIMAGE_RGB, int);
PPIMAGE_UCHAR  IdImaRGBtoUChar  (PPIMAGE_RGB);
PPIMAGE_UCHAR  IdImaVfilter     (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int,
                                 int, int, int, int);


////////////////
PPIMAGE_UCHAR  IdImaUserScaling (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int, float *);
PPIMAGE_UCHAR  IdImaApplyLut    (PPIMAGE_UCHAR, PPIMAGE_UCHAR, IDLUT_UCHAR);

PPIMAGE_UCHAR  IdImaMedian     (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int);
PPIMAGE_UCHAR  IdImaPrewitt    (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int);
PPIMAGE	       IdImaSobel      (PPIMAGE, PPIMAGE,int);
PPIMAGE        IdImaAdd        (PPIMAGE, PPIMAGE, PPIMAGE, double, double);
PPIMAGE	       IdImaSubstract  (PPIMAGE, PPIMAGE, PPIMAGE, double, double);
PPIMAGE        IdImaMaximumOfTwoImages(PPIMAGE, PPIMAGE, PPIMAGE);
PPIMAGE        IdImaMinimumOfTwoImages(PPIMAGE, PPIMAGE, PPIMAGE);
PPIMAGE_UCHAR  IdImaSquaredError (PPIMAGE_UCHAR, PPIMAGE_UCHAR, PPIMAGE_UCHAR,
                                 double *OUTPUT, int);

PPIMAGE_UCHAR  IdImaDrawLine    (PPIMAGE_UCHAR, int, int, int, int, int, int);
PPIMAGE_UCHAR  IdImaDrawPolyLines(PPIMAGE_UCHAR, int, int, int[], int[], int,
                                 int);

PPIMAGE_LONG   IdImaNeighborMat (PPIMAGE_UCHAR, int,int,int,int,int,int,int);
int            IdImaNeighCaract (PPIMAGE_LONG, float *);
int            IdImaFill        (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int);
int            IdImaLabel       (PPIMAGE_UCHAR, PPIMAGE_SHORT,int,FILE *,FILE *,
                                 int);
PPIMAGE_UCHAR  IdImaSetValue            (PPIMAGE_UCHAR, int);
PPIMAGE        IdImaSetValueAnyType     (PPIMAGE, double);

PPIMAGE_UCHAR  IdImaDeriche             (PPIMAGE_UCHAR, double, double);
PPIMAGE_UCHAR  IdImaDericheHyst         (PPIMAGE_UCHAR, double,
                                          double, double);
PPIMAGE_DOUBLE IdImaDericheLaplacian    (PPIMAGE, double);
PPIMAGE_UCHAR  IdImaDericheProcess      (PPIMAGE_UCHAR, PPIMAGE_FLOAT *OUTPUT, 
                                          PPIMAGE_FLOAT * OUTPUT, int,
                                          double, int *);
PPIMAGE_DOUBLE IdImaGeneralRecur        (PPIMAGE, double []);
PPIMAGE_DOUBLE IdImaDerivX              (PPIMAGE, double);
PPIMAGE_DOUBLE IdImaDerivY              (PPIMAGE, double);
PPIMAGE_DOUBLE IdImaDerivXX             (PPIMAGE, double);
PPIMAGE_DOUBLE IdImaDerivYY             (PPIMAGE, double);
PPIMAGE_UCHAR  IdImaNoiseUniform 	 (PPIMAGE_UCHAR, double, double);
PPIMAGE_UCHAR  IdImaNoiseGaussian 	 (PPIMAGE_UCHAR, double, double, int);
PPIMAGE_UCHAR  IdImaNoiseSaltAndPepper  (PPIMAGE_UCHAR, PPIMAGE_UCHAR, double);
PPIMAGE_UCHAR  IdImaGenerateHalfPlane   (PPIMAGE_UCHAR, int);
PPIMAGE_UCHAR  IdImaGenerateDoubleSquare(PPIMAGE_UCHAR, int, int, int);
PPIMAGE_UCHAR  IdImaGenerateSinusSquare (PPIMAGE_UCHAR, int, int);
PPIMAGE_UCHAR  IdImaGenerateCone        (PPIMAGE_UCHAR);
PPIMAGE_UCHAR  IdImaHysteresis          (PPIMAGE_UCHAR, PPIMAGE_UCHAR,
                                          int, int, int);
PPIMAGE_DOUBLE IdImaSmooth              (PPIMAGE,double);


PPIMAGE_UCHAR   IdImaPers          (PPIMAGE_UCHAR,int,int,int,int,int,int);
PPIMAGE_USHORT  IdImaConnectedComponents
                                   (PPIMAGE_UCHAR, PPIMAGE_USHORT,int,
                                    int *BOTH);
int             IdImaScrap         (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int, int,
                                    int);
double          IdImaMinima        (PPIMAGE);
double          IdImaMaxima        (PPIMAGE);
int             IdImaMinMax        (PPIMAGE, double *OUTPUT, double *OUTPUT);
double          IdImaDyn           (PPIMAGE);
void            IdImaArrondi       (PPIMAGE);
double          IdImaMoyenne       (PPIMAGE);
int             IdImaMoyVar        (PPIMAGE, double *, double *);
double          IdImaVariance      (PPIMAGE);
double          IdImaEntropie      (PPIMAGE);
void            IdImaFloatDct2D    (PPIMAGE_FLOAT,int,int,int);
void            IdImaDoubleDct2D   (PPIMAGE_DOUBLE,int,int,int);
PPIMAGE_UCHAR   IdImaBSplinesBiCubiques (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int);
PPIMAGE   	IdImaErreurQuadratique  (PPIMAGE, PPIMAGE,PPIMAGE);
PPIMAGE   	IdImaErreurPositive     (PPIMAGE, PPIMAGE,PPIMAGE);
PPIMAGE   	IdImaErreurSimple       (PPIMAGE, PPIMAGE,PPIMAGE);
PPIMAGE   	IdImaErreurNoRecad      (PPIMAGE, PPIMAGE,PPIMAGE);
PPIMAGE   	IdImaErreurNegative     (PPIMAGE, PPIMAGE,PPIMAGE);

PPIMAGE   	IdImaSousEchantillonnage(PPIMAGE, PPIMAGE, int, int);
PPIMAGE_UCHAR   IdImaSurEchantillonnage (PPIMAGE_UCHAR, PPIMAGE_UCHAR);
float         * IdImaStat               (PPIMAGE_UCHAR, int);
int           * IdImaCdg                (PPIMAGE_UCHAR, int);
float         * IdImaValeursPropres     (PPIMAGE_UCHAR, int);
float         * IdImaVecteursPropres    (PPIMAGE_UCHAR, int);

void	IdImaPatternIncruste	(PPIMAGE_UCHAR,int,int,int,int);
void 	IdImaBinarise		(PPIMAGE,PPIMAGE_UCHAR,int);
PPIMAGE	IdImaZoom		(PPIMAGE,PPIMAGE);
void 	IdImaSwap		(PPIMAGE,int);
int 	IdImaEllipseIncruste	(PPIMAGE_UCHAR,int,int,int,int,int,int);
int 	IdImaCircleIncruste	(PPIMAGE_UCHAR,int,int,int,int);
int 	IdImaRectangleIncruste	(PPIMAGE_UCHAR,int,int,int,int,int);

PPIMAGE IdImaExtractSubImage    (PPIMAGE, PPIMAGE, int, int, int, int);
PPIMAGE IdImaVecteurToImage     (void *,int,int,int);
PPIMAGE IdImaVecteurToImageOffset(void *,int,int,int,int);


///////////////////// Tangent image related (for whatever it may mean)
/*
#ifndef SWIG
typedef struct {
                float tgalpha;
                unsigned int sens:1;
               } TGDIR;
typedef TGDIR ** PPIMAGE_TGDIR;
#endif
*/ 
PPIMAGE_UCHAR IdImaExtremSearch  (PPIMAGE_UCHAR, PPIMAGE_TGDIR, int);
PPIMAGE_UCHAR IdImaGradientHyster(PPIMAGE_UCHAR, PPIMAGE_TGDIR, double, int *);

///////////////////// B-spline related (for whatever it may mean)
typedef struct B2_SPLINE 
 {
   int nb_pts_eni;
   PPIMAGE_DOUBLE pc;
   int nb_coor;
   PPIMAGE_DOUBLE coor;
   PPIMAGE_DOUBLE  curve;
   int nb_faces;
   } B2spline;

void IdSigBsplineCubiqueReadCtrlPts (char *,B2spline *);
void IdSigBsplineCubiqueWriteSplPts (char *,B2spline *);
void IdSigBsplineCalculCourbe (B2spline *,int,int);
void IdSigBsplineInitCoeff    (int,PSIGNAL_DOUBLE, PSIGNAL_DOUBLE,
                               PSIGNAL_DOUBLE, PSIGNAL_DOUBLE);
void IdSigBsplineInitDerive1Coeff(int,PSIGNAL_DOUBLE,PSIGNAL_DOUBLE,
		                PSIGNAL_DOUBLE,PSIGNAL_DOUBLE);
void IdSigBsplineInitDerive2Coeff(int,PSIGNAL_DOUBLE,PSIGNAL_DOUBLE,
		                PSIGNAL_DOUBLE,PSIGNAL_DOUBLE);
void IdSigBsplineAlloc        (int, PSIGNAL_DOUBLE * OUTPUT,
                               PSIGNAL_DOUBLE * OUTPUT,
                               PSIGNAL_DOUBLE * OUTPUT,
                               PSIGNAL_DOUBLE * OUTPUT);
void IdSigBsplineFree         (PSIGNAL_DOUBLE , PSIGNAL_DOUBLE ,
                               PSIGNAL_DOUBLE , PSIGNAL_DOUBLE);
void IdSigBsplineCloseCourbe  (B2spline *);

//////////////////////
PPIMAGE_UCHAR IdImaRecalage   (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int *, int *, int);
int IdImaTransform            (int *, int *, int, PSIGNAL_DOUBLE,
                               PSIGNAL_DOUBLE);
PPIMAGE_UCHAR IdImaAppTransfo (PPIMAGE_UCHAR, PPIMAGE_UCHAR, PSIGNAL_DOUBLE,
                               PSIGNAL_DOUBLE, int);
PPIMAGE_UCHAR IdImaRecad16To8 (PPIMAGE_USHORT, int, int);
PPIMAGE_UCHAR IdImaRecad16To8IgnoreUpper
                              (PPIMAGE_USHORT, int, int, int);
PPIMAGE_USHORT IdImaRecad16To16IgnoreUpper
                              (PPIMAGE_USHORT, int, int, int);
PPIMAGE_UCHAR  IdImaRecadTo8  (PPIMAGE,PPIMAGE_UCHAR, double,double);
PPIMAGE_USHORT IdImaRecadTo16 (PPIMAGE,PPIMAGE_USHORT,double,double);

PPIMAGE_UCHAR IdImaCreImagette  (PPIMAGE_UCHAR *,int,int,int);
PPIMAGE_UCHAR IdImaCreImagetteXY(PPIMAGE_UCHAR *,int,int,int,int,int);
PPIMAGE_UCHAR IdImaConcat       (PPIMAGE_UCHAR ,PPIMAGE_UCHAR);
PPIMAGE_UCHAR IdImaScotchImages (PPIMAGE_UCHAR *,int);

PPIMAGE_UCHAR IdImaScotchImagesXYHoriz   (PPIMAGE_UCHAR *, int, int, int);
PPIMAGE_UCHAR IdImaScotchImagesXYVertic  (PPIMAGE_UCHAR *, int, int, int);
PPIMAGE_UCHAR IdImaMakeImagetteXYHoriz   (PPIMAGE_UCHAR *, int, int, int, int,
                                          int);
PPIMAGE_UCHAR IdImaMakeImagetteXYVertic  (PPIMAGE_UCHAR *, int, int, int, int,
                                          int);
PPIMAGE_UCHAR IdImaSignalToImage	 (PSIGNAL, int, int, int, double,
                                          double, int, int, int, int);
PPIMAGE_UCHAR IdImaSignalToExistingImage (PSIGNAL, PPIMAGE_UCHAR, int, int,
                                          int, double, double, int);
PPIMAGE_UCHAR IdImaDrawPlotBox 		 (int, int, int, int, int, int, int,
                                          int);
PPIMAGE_UCHAR IdImaPutPlotBoxToImage 	 (PPIMAGE_UCHAR, int, int, int, int,
                                          int, int, int, int, int);

PPIMAGE       IdImaArrayToImage          (void **, int, int , int);
PSIGNAL_FLOAT IdImaProfil                (PPIMAGE_UCHAR,int,int,int,int);
PPIMAGE_UCHAR IdImaModifDeContraste      (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int);
PPIMAGE_UCHAR IdImaRehauss               (PPIMAGE_UCHAR ,PPIMAGE_UCHAR, double,
                                          int);

PPIMAGE IdImaSupprPtsIsoles  (PPIMAGE,PPIMAGE);
PPIMAGE IdImaSeuilMinMax     (PPIMAGE,PPIMAGE,double,double);
PPIMAGE IdImaSupprPtsInNoise (PPIMAGE,PPIMAGE,double);

int IdStatDataRead (char *,PPIMAGE_FLOAT * OUTPUT, PPIMAGE_FLOAT ** OUTPUT,
                    PSIGNAL_LONG *OUTPUT, PSIGNAL_LONG *OUTPUT);
int IdStatMean_N   (PPIMAGE_FLOAT,PSIGNAL_FLOAT *OUTPUT);
int IdStatMean_1   (PSIGNAL_FLOAT,double * OUTPUT);
int IdStatStdDev_N (PPIMAGE_FLOAT,PSIGNAL_FLOAT,PSIGNAL_FLOAT * OUTPUT);
int IdStatStdDev_1 (PSIGNAL_FLOAT,double,double * OUTPUT);
int IdStatCorrel_N (PPIMAGE_FLOAT,PSIGNAL_FLOAT,PSIGNAL_FLOAT,
                    PPIMAGE_FLOAT * OUTPUT);
int IdStatCorrel_1 (PSIGNAL_FLOAT,PSIGNAL_FLOAT,double,double, double,
                    double,double * OUTPUT);
int IdStatInertTot (PPIMAGE_FLOAT,PSIGNAL_FLOAT,PPIMAGE_FLOAT * OUTPUT);
int IdStatInertWit (PPIMAGE_FLOAT *,PSIGNAL_FLOAT *,int,
                    PPIMAGE_FLOAT * OUTPUT);
int IdStatInertBet (PSIGNAL_FLOAT *,PSIGNAL_FLOAT,PSIGNAL_LONG,
                    PPIMAGE_FLOAT * OUTPUT);
int IdStatFisher   (PPIMAGE_FLOAT,PPIMAGE_FLOAT,double *OUTPUT);

PPIMAGE_UCHAR IdImaNagao9            (PPIMAGE_UCHAR, PPIMAGE_UCHAR );
PPIMAGE_RGB   IdImaReadAvsFile       (char *);
PPIMAGE_UCHAR IdImaSkelet            (PPIMAGE_UCHAR,PPIMAGE_UCHAR, int);
PPIMAGE_UCHAR IdImaModToUchar        (PPIMAGE_COMPLEX);
PPIMAGE_UCHAR IdImaUcharAd           (PPIMAGE_UCHAR,int,int,int,int,int);
PPIMAGE_FLOAT IdImaFloatAd           (PPIMAGE_FLOAT,int,int,int,int,int);
PPIMAGE_UCHAR IdImaUcharSup          (PPIMAGE_UCHAR,int,int,int,int);
PPIMAGE_FLOAT IdImaFloatSup          (PPIMAGE_FLOAT,int,int,int,int);
double        IdImaCorFloatCentree   (PPIMAGE_FLOAT,PPIMAGE_FLOAT);
double        IdImaCovFloatCentree   (PPIMAGE_FLOAT,PPIMAGE_FLOAT);
double        IdImaContrastUchar     (PPIMAGE_UCHAR);
double        IdImaSnrFloat          (PPIMAGE_FLOAT,PPIMAGE_FLOAT);
double        IdImaPSnrFloat         (PPIMAGE_FLOAT,PPIMAGE_FLOAT);
double        IdImaPPSnrFloat        (PPIMAGE_FLOAT,PPIMAGE_FLOAT);
double        IdImaDistortionFloat   (PPIMAGE_FLOAT,PPIMAGE_FLOAT);
double        IdImaCovFloat          (PPIMAGE_FLOAT,PPIMAGE_FLOAT);
double        IdImaCorFloat          (PPIMAGE_FLOAT,PPIMAGE_FLOAT);
double        IdImaNorm2             (PPIMAGE);
double        IdImaNorm1             (PPIMAGE);

PPIMAGE_FLOAT IdImaCentrerFloat      (PPIMAGE_FLOAT,PPIMAGE_FLOAT);


//////////// Fast Fourier Transform related

PPIMAGE_UCHAR   IdImaFFTrecadLog (PPIMAGE_COMPLEX, PPIMAGE_UCHAR, int);
PPIMAGE_COMPLEX IdImaFFT         (PPIMAGE_UCHAR);
int             IdImaFFT_Filter  (PPIMAGE_COMPLEX,int,int,int);
PPIMAGE_COMPLEX IdImaFFTpermut   (PPIMAGE_COMPLEX);
PPIMAGE_COMPLEX IdImaIFFT        (PPIMAGE_COMPLEX);

//////////// C. Mathieu's Morphology related
int           IdImaWriteVisilogFile (char *,PPIMAGE_UCHAR);
PPIMAGE_UCHAR IdImaReadVisilogFile  (char*, int);
PPIMAGE_UCHAR IdImaRemoveEndingSegments
                                    (PPIMAGE_UCHAR, PPIMAGE_UCHAR);
PPIMAGE_UCHAR IdImaRemoveIsolatedPoints
                                    (PPIMAGE_UCHAR, PPIMAGE_UCHAR);
PPIMAGE_UCHAR IdImaNThin            (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int);
PPIMAGE_UCHAR IdImaNShrink          (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int);
PPIMAGE_UCHAR IdImaConditionalMarking
                                    (PPIMAGE_UCHAR, PPIMAGE_UCHAR,
                                     PPIMAGE_UCHAR, PSIGNAL_UCHAR,
                                     PSIGNAL_UCHAR, long* OUTPUT);
PPIMAGE_UCHAR IdImaMark             (PPIMAGE_UCHAR, PPIMAGE_UCHAR,
                                      PPIMAGE_UCHAR,int);
PPIMAGE_UCHAR IdImaGradientMorphologique
                                    (PPIMAGE_UCHAR, PPIMAGE_UCHAR , int, int);
PPIMAGE_UCHAR IdImaPlaneCorrection  (PPIMAGE_UCHAR, PPIMAGE_UCHAR);

/* Christophe Mathieu's Color */
int            IdImaSetPlanColor    (PPIMAGE_COLOR, PPIMAGE_UCHAR, int);
int            IdImaSetNumPlanColor (PPIMAGE_COLOR, int, int); 
PPIMAGE_UCHAR  IdImaGetPlanColor    (PPIMAGE_COLOR, int);
PPIMAGE_UCHAR  IdImaColorToUChar    (PPIMAGE_COLOR);
PPIMAGE_COLOR  IdImaEqualizeHistogramColor
                                    (PPIMAGE_COLOR, PPIMAGE_COLOR, int);
PPIMAGE_COLOR  IdImaMedianColor     (PPIMAGE_COLOR, PPIMAGE_COLOR,int,int);
PPIMAGE_COLOR  IdImaContourColor    (PPIMAGE_COLOR, PPIMAGE_COLOR,
                                     int, unsigned char*, int, int,
                                     int, unsigned char*, int, int,
                                     int, unsigned char*, int, int, int);
PPIMAGE_COLOR  IdImaNoiseUniformColor(PPIMAGE_COLOR,
                		      double,double,double,double,
                                      double,double);
PPIMAGE_COLOR IdImaPrewittColor      (PPIMAGE_COLOR,PPIMAGE_COLOR,int,int);
PPIMAGE_COLOR IdImaSobelColor        (PPIMAGE_COLOR,PPIMAGE_COLOR,int,int);
PPIMAGE_COLOR IdImaConvertRGBToIHS   (PPIMAGE_COLOR, PPIMAGE_COLOR);
PPIMAGE_COLOR IdImaConvertIHSToRGB   (PPIMAGE_COLOR, PPIMAGE_COLOR);
PPIMAGE_UCHAR IdImaGenerateBorder    (PPIMAGE_UCHAR, int, int);
PPIMAGE_UCHAR IdImaKillBorderObjects (PPIMAGE_UCHAR,PPIMAGE_UCHAR,int);
PPIMAGE_UCHAR IdImaSelectBorderObjects(PPIMAGE_UCHAR,PPIMAGE_UCHAR,int);
PPIMAGE_UCHAR IdImaScaleInterpolate  (PPIMAGE_UCHAR,PPIMAGE_UCHAR);

// En attendant de faire qq chose de generique . JPRx
PPIMAGE_USHORT IdImaScaleInterpolateUSHORT  (PPIMAGE_USHORT,PPIMAGE_USHORT);


////////////////////////////////////////////////////////////
PPIMAGE_UCHAR IdImaSeparatedMean     (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int, int);
PPIMAGE_UCHAR IdImaAutoThreshold     (PPIMAGE_UCHAR, PPIMAGE_UCHAR, int, int);
PPIMAGE_UCHAR IdImaAutoThresholdChow (PPIMAGE, int);

PPIMAGE_UCHAR IdImaRGBToLuminance    (PPIMAGE_RGB, PPIMAGE_UCHAR);
PPIMAGE_UCHAR IdImaRGBLutToLuminance (PPIMAGE_UCHAR,PPIMAGE_UCHAR,RGB*);


PPIMAGE_UCHAR  IdImaDistance       (PPIMAGE_UCHAR, PPIMAGE_UCHAR,  int);
PPIMAGE_USHORT IdImaDistanceUSHORT (PPIMAGE_UCHAR, PPIMAGE_USHORT, int);
PPIMAGE_UCHAR  IdImaThresholdDouble(PPIMAGE_UCHAR, PPIMAGE_UCHAR, int,int,int);
PPIMAGE        IdImaAddBord        (PPIMAGE,double,int,int,int,int);
PPIMAGE_UCHAR  IdImaHistoBidim     (PPIMAGE_UCHAR, PPIMAGE_UCHAR,
                                    PPIMAGE_UCHAR);
PPIMAGE        IdImaModifSubImage  (PPIMAGE, PPIMAGE, int, int);
PPIMAGE        IdImaSeuil          (PPIMAGE, PPIMAGE, double, double, double);

PPIMAGE_UCHAR  IdImaHough          (PPIMAGE_UCHAR, int, double, double,
                                    double, double, double, int, int );
PSIGNAL        IdImaSignalFromLine (PPIMAGE, int, int, int);
PSIGNAL        IdImaReuseSigFromLine (PPIMAGE, int, int, PSIGNAL);
PPIMAGE        IdImaBool           (PPIMAGE, PPIMAGE, PPIMAGE, int);
PPIMAGE_UCHAR  IdImaRecadTo8ABS    (PPIMAGE,PPIMAGE_UCHAR);
PPIMAGE_UCHAR  IdImaRecadTo8DCT    (PPIMAGE,PPIMAGE_UCHAR);
PPIMAGE_UCHAR  IdImaRecadTo8LOG    (PPIMAGE,PPIMAGE_UCHAR);

///////////// Geometry related
PPIMAGE        IdImaRotate180      (PPIMAGE,PPIMAGE);
PPIMAGE        IdImaFlipV          (PPIMAGE,PPIMAGE);
PPIMAGE        IdImaFlipH          (PPIMAGE,PPIMAGE);
PPIMAGE_UCHAR  IdImaRotate         (PPIMAGE_UCHAR, PPIMAGE_UCHAR,
                                    double, double, double, double,
                                    int, int);

PPIMAGE_UCHAR  IdImaApplyLut16     (PPIMAGE_USHORT, PPIMAGE_UCHAR,
                                    unsigned short int *);
PPIMAGE_UCHAR  IdImaGradMax        (PPIMAGE_UCHAR, PPIMAGE_UCHAR, double);
PPIMAGE        IdImaDiffusionAnisotropique 
                                   (PPIMAGE, PPIMAGE, double, double,
                                    int,int,int);
#endif

/* DEFINITION DESCRIPTION **************************************************

RESUME: constantes IMA_??? utilisees pour definir le type des pixels d'1 image

DESCRIPTION : constantes IMA_??? utilisees dans les fonctions d'allocation 
              d'images permettant de definir le type des pixels.           
              
            : IdImaAlloc, IdImaType, IdLibidoDataType.

            : IMA_CHAR   : IMA_DOUBLE         : IMA_COMPLEX_UCHAR  :
            : IMA_UCHAR  : IMA_COMPLEX        : IMA_COMPLEX_SHORT  :
            : IMA_SHORT  : IMA_RGB            : IMA_COMPLEX_USHORT :
            : IMA_USHORT : IMA_COMPLEX_DOUBLE : IMA_COMPLEX_LONG   :
            : IMA_LONG   : IMA_COMPLEX_FLOAT  : IMA_BIT            :
            : IMA_FLOAT  : IMA_COMPLEX_CHAR   :
                                         
              Chaque type (ie UCHAR) est decrit dans : IdLibidoDataType
                                                            
RESUME: types predefinis d'images (associes a IdLibidoDataType)

DEFINITION : types predefinis pour l'allocation des images, associes aux types
             predefinis par IdLibidoDataType                
             
          : IdImaAlloc, IdLibidoDataTYpe.

    : PPIMAGE         : PPIMAGE_RGB            : PPIMAGE_COMPLEX_CHAR   :
    : PPIMAGE_CHAR    : PPIMAGE_SHORT          : PPIMAGE_COMPLEX_UCHAR  :
    : PPIMAGE_UCHAR   : PPIMAGE_USHORT         : PPIMAGE_COMPLEX_SHORT  :
    : PPIMAGE_FLOAT   : PPIMAGE_LONG           : PPIMAGE_COMPLEX_USHORT :
    : PPIMAGE_DOUBLE  : PPIMAGE_COMPLEX_DOUBLE : PPIMAGE_COMPLEX_LONG   :
    : PPIMAGE_COMPLEX : PPIMAGE_COMPLEX_FLOAT  : PPIMAGE_BIT            :
                        
                      
	 PPIMAGE seul est de type void.

******************************************************** END DESCRIPTION */
