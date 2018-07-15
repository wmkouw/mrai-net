/*************************************************************************
* $Id: idacr.h,v 1.1 2005/09/09 08:22:25 bellet Exp $
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

#ifndef __IDACR_H__
#define __IDACR_H__
/*************************************************************************
*                                  * PROJET      : libido
* NOM DU MODULE : idacr.h          * TYPE        : fichier inclus
* AUTEUR        : c.odet           * CREATION    :  07/05/1992
* VERSION       : 1.0              * REVISION    :  16/06/1992
* LANGAGE       : C                * 
*                                  * 
**************************************************************************
*
*  Description : definition et structure pour format ACRNEMA modifie URA1216
**************************************************************************
*
*  MODULES UTILISES :
*
**************************************************************************
*                     |              |
* REVISIONS :  DATE   | AUTEUR       |  DESCRIPTION
*---------------------|--------------|------------------------------------
*           16/06/92  | B. BARBIER   | Insertion Libido
*           01/10/92  | B. Barbier   | V1.2-5 : normalisation des noms
*           16/10/93  | C.Mathieu    | V2.5-2 : Ajout de prototypes
	    7/5/99	JPR		Mise a niveau pour SWIG
	    5/3/2000	JPR		Ajouts pour IRMImageInfo
**************************************************************************/

#include <stdio.h>
#include "idswig.h"

#ifdef _MSC_VER
#ifndef __CYGWIN__
#include <Basetsd.h>
#endif
#endif

#include "idacr-restricted.h"

/* ========================================== */

/*  indices dans la Table des IRM Image Info  */

/* si une entree est rajoutee ici,

	Penser a modifier le fichier dicom.c
		pour maintenir la coherence.
	Faire les ajouts de preference en fin de liste
		pour garder une compatibilite ascendante
		meme pour les utilisateurs qui auraient goretise
		c-a-d utilise les indices 'en dur' au lieu 
		de ceux donnes par le 'enum'
*/
	

enum {
_ID_File_Name = 0,
_ID_SOP_Class_UID,
_ID_SOP_Instance_UID,
_ID_Study_Date,
_ID_Series_Date,
_ID_Acquisition_Date,
_ID_Image_Date,
_ID_Study_Time,
_ID_Series_Time,
_ID_Acquisition_Time,
_ID_Image_Time,
_ID_Modality,
_ID_Manufacturer,
_ID_Institution_Name,
_ID_Study_Description,
_ID_Series_Description,
_ID_Admitting_Diagnoses_Description,
_ID_Patient_Name,
_ID_Patient_ID,
_ID_Body_Part_Examined,
_ID_Scanning_Sequence,
_ID_Sequence_Variant,
_ID_Scan_Options,
_ID_MR_Acquisition_Type,
_ID_Sequence_Name,
_ID_Slice_Thickness,
_ID_Repetition_Time,
_ID_Echo_Time,
_ID_Inversion_Time,
_ID_Number_of_Averages,
_ID_Imaging_Frequency,
_ID_Imaged_Nucleus,
_ID_Echo_Number,
_ID_Magnetic_Field_Strength,
_ID_Spacing_Between_Slices,
_ID_Echo_Train_Length,
_ID_Percent_Sampling,
_ID_Percent_Phase_Field_of_View,
_ID_Receiving_Coil,
_ID_Patient_Position,
_ID_Study_Instance_UID,
_ID_Series_Instance_UID,
_ID_Study_ID,
_ID_Series_Number,
_ID_Acquisition_Number,
_ID_Image_Number,
_ID_Patient_Orientation,
_ID_Image_Position,
_ID_Image_Position_Patient,
_ID_Image_Orientation,
_ID_Image_Orientation_Patient,
_ID_Location,
_ID_Frame_of_Reference_UID,
_ID_Slice_Location,
_ID_Image_Comments,
_ID_Pixel_Spacing,
_ID_Window_Center,
_ID_Window_Width,
_ID_Image_Type,
_ID_Temporal_Position_Identifier,
_ID_Comments,
_ID_Rescale_Intercept,
_ID_Rescale_Slope,
_ID_Rescale_Type,
_ID_Protocol_Name,
_ID_Number_of_Temporal_Positions,


_ID_Number_of_Items
};

/* ========================================== */
/* type de valeur */

#define NO 0 /* NON ACRNEMA : traitement special pour ecriture */
#define BI 1 /* 16 bits */
#define BD 2 /* 32 bits */
#define AN 3 /* ASCII numerique */
#define AT 4 /* ASCII texte */

void *  IdAcrReadFile               (char *, int);
void *  IdDcmCVReadFile             (char *, int, int, int);
void *  IdAcrReadFileIntoVector	    (char *, int , int, int, void *);

int     IdAcrWriteFile              (char *, void *);

int     IdAcrGetIdtype              (char *);
int     IdAcrGetXSize               (char *);
int     IdAcrGetYSize               (char *);
int     IdAcrGetZSize               (char *);
int     IdAcrGetTSize               (char *);
int     IdAcrGetSwapCode            (char *);
int     IdAcrGetOffset              (char *);
int     IdAcrGetNbFrames            (char *);
int     IdAcrGetNBitsAllocated      (char *);
int     IdAcrGetNBitsUsed           (char *);
int     IdAcrGetHighBit             (char *);
int     IdAcrGetSign           	    (char *);

int     IdAcrIsAcrReadable          (char *);

int 	IdAcrInquirePixelSize       (char * ,float * OUTPUT, float * OUTPUT);
int 	IdAcrInquireSliceThickness  (char * ,float * OUTPUT);
char **	IdAcrInquirePatientInfo     (char *);
char **	IdAcrInquireIRMInfo         (char *);
char **	IdAcrInquireScanElscintInfo (char *);
char **	IdAcrInquireExamIRMInfo     (char *);
char ** IdAcrInquireIRMImageInfo    (char *);

int     IdAcrPrintIRMInfo           (char **);
int     IdAcrPrintIRMImageInfo      (char **);
int     IdAcrPrintScanElscintInfo   (char **);
int     IdAcrPrintPatientInfo       (char **);
FILE *  IdSwigFopen                 (char *);
void    IdSwigFclose                (FILE*);
int     IdAcrInquireInfoTot4        (char *, unsigned short int * OUTPUT,
                                     unsigned short int * OUTPUT,
                                     unsigned short int * OUTPUT, int * OUTPUT,
                                     int * OUTPUT, int * OUTPUT, int * OUTPUT,
                                     int * OUTPUT, int * OUTPUT, int * OUTPUT,
                                     int * OUTPUT, int * OUTPUT, int * OUTPUT,
				       		 int * OUTPUT);
				     
int     IdAcrWriteFileExtended (char *, void *, ACR_ELEMENT *); 

char * IdAcrReadDicomElementFromLabel(char *, FILE *);
long   IdAcrGetPixelOffsetFromFP     (FILE *);
long   IdAcrGetPixelOffsetFromFile   (char *);

char ***IdIrmGetAndRenameDirContents(char *, char *, int);



int 	IdAcrPrintExamIRMInfo(char **ExamIRMInfo);


char ***IdStrGetDicomDirectoryInfo  (char *, char *, int * OUTPUT);
char  **IdStrGetExamInfo            (char *, char *);
char  **IdStrGetSerieInfo           (char *, char *);
char ***IdStrGetSeriesExamInfo      (char *, char *, int *OUTPUT);
char ***IdStrGetDirContents3        (char *, char *, int *OUTPUT);
int 	IdStrRenameDirContents 		(char *, char *, int *OUTPUT);

#endif
