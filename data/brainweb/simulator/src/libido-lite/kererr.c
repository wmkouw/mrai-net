/*************************************************************************
* $Id: kererr.c,v 1.1 2005/09/09 08:22:50 bellet Exp $
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

/*
 * Gestion des erreurs LIBIBO par IdErrno
 */

#include "iderr.h"

int IdErrno = 0;

typedef struct {
	int code;
	char *msg;
} MSGERROR;

static MSGERROR taberror[] = {
	{0, "Pas d'erreur"},

	{IDERR_NOT_FLOATLIST, "Liste flottants incorrecte"},
	{IDERR_LABEL_UNDEF, "Label non defini"},
	{IDERR_TOO_LONG_LIST, "Liste avec trop d'elements"},
	{IDERR_INVALID_NUMBER, "Param Numerique invalide"},
	{IDERR_NOT_INTLIST, "Liste entiers incorrecte"},

	{IDERR_ALLOC_MEM, "Echec allocation memoire"},
	{IDERR_ALLOC_SIG, "Echec allocation Signal"},
	{IDERR_ALLOC_IMA, "Echec allocation Image"},
	{IDERR_ALLOC_VOL, "Echec allocation Volume"},
	{IDERR_ALLOC_PARAM, "Echec allocation PARAM"},
	{IDERR_ALLOC_CNT, "Echec allocation Contour"},
	{IDERR_ALLOC_LST, "Echec allocation Liste"},
	{IDERR_ALLOC_TREE, "Echec allocation Arbre"},
	{IDERR_ALLOC_FACE, "Echec allocation Face"},
	{IDERR_ALLOC_ELEM, "Echec allocation Element"},
	{IDERR_ALLOC_CNT3D, "Echec allocation Contour 3D"},
	{IDERR_EMPTY_LIST, "Liste Vide		"},

	{IDERR_ALLOC_SUB, "Echec allocation Sous-Struct"},

	{IDERR_Y_OUT_OF_IMAGE, "Sortie de l'image en y"},
	{IDERR_X_OUT_OF_IMAGE, "Sortie de l'image en x"},

	{IDERR_Z_OUT_OF_VOLUME, "Sortie du volume en z"},
	{IDERR_Y_OUT_OF_VOLUME, "Sortie du volume en y"},
	{IDERR_X_OUT_OF_VOLUME, "Sortie du volume en x"},

	{IDERR_Y_GREATER_DIMY, "Parametre dy > DIMY Fenetre"},
	{IDERR_X_GREATER_DIMX, "Parametre dx > DIMX Fenetre"},
	{IDERR_Z_GREATER_DIMZ, "Parametre dz > DIMZ Fenetre"},

	{IDERR_NOT_SQUARE, "Image non carree"},

	{IDERR_POINTER_IS_NULL, "Pointeur NULL"},

	{IDERR_WRONG_TYPE, "Mauvais type Libido"},
	{IDERR_WRONG_LIBTYPE, "Type Libido incorrect"},

	{IDERR_UNDEF_SCREEN, "Code ecran invalide"},

	{IDERR_WRONG_DIM, "Dimensions invalides"},
	{IDERR_WRONG_SIGNALS, "Signaux incompatibles"},
	{IDERR_WRONG_IMAGES, "Images incompatibles"},
	{IDERR_WRONG_VOLUMES, "Volumes incompatibles"},
	{IDERR_WRONG_CONTOURS, "Contours incompatibles"},
	{IDERR_WRONG_TYPES, "Types incompatibles"},

	{IDERR_OPEN_FAILED, "Erreur Ouverture fichier"},
	{IDERR_READ_FAILED, "Erreur Lecture fichier"},
	{IDERR_WRITE_FAILED, "Erreur Ecriture fichier"},
	{IDERR_NON_ACR_FILE, "Fichier Non  ACR	"},

	{IDERR_WRONG_FILE_NAME, "Nom Fichier incorrect"},
	{IDERR_EMPTY_FILE_NAME, "Nom Fichier NULL"},

	{IDERR_Z_OUT_OF_SEQUENCE, "Sortie du Sequence en z"},
	{IDERR_Y_OUT_OF_SEQUENCE, "Sortie du Sequence en y"},
	{IDERR_X_OUT_OF_SEQUENCE, "Sortie du Sequence en x"},
	{IDERR_T_OUT_OF_SEQUENCE, "Sortie du Sequence en t"},

	{IDERR_ALLOC_SEQ, "Echec allocation Sequence"},
	{IDERR_WRONG_SEQUENCES, "Sequences Incompatibles"},
	{IDERR_MIN_EQ_MAX, "Min = Max"},

	{IDERR_BAD_DIRECTION, "Bad Direction"},
	{IDERR_THRESHOLD_OUT_OF_RANGE, "Threshold out of range"},
	{IDERR_NULL_POINTER_ARGUMENT, "NULL_POINTER_ARGUMENT"},
	{IDERR_WRONG_IMAGE_TYPE, "WRONG_IMAGE_TYPE"},
	{IDERR_WRONG_PARAMETER, "WRONG_PARAMETER"},
	{IDERR_OUT_OF_RANGE, "OUT_OF_RANGE"},
	{IDERR_CALCULATION, "CALCULATION"},
	{IDERR_WRONG_MATRIX_DIMENSION, "WRONG_MATRIX_DIMENSION"},
	{IDERR_WRONG_IMAGE_SIZE, "WRONG_IMAGE_SIZE"},
	{IDERR_WINDOW_SIZE_LESS_THAN_ZERO, "WINDOW_SIZE_LESS_THAN_ZERO"},
	{IDERR_POINTER_IS_ZERO, "POINTER_IS_ZERO"},
	{IDERR_NO_IMAGE_ALLOCATED, "NO_IMAGE_ALLOCATED"},
	{IDERR_WINDOW_SIZE_NOT_POWER_TO_2, "WINDOW_SIZE_NOT_POWER_TO_2"},
	{IDERR_PARAMETER_NOT_IN_RANGE, "PARAMETER_NOT_IN_RANGE"},
	{IDERR_PARAMETER_MUST_BE_BOOLEAN, "PARAMETER_MUST_BE_BOOLEAN"},

	{0, 0}	/* Watch out : keep this as the last entry */
};

/**
 * \ingroup    ker
 * \brief      Give the error message corresponding to the error code
 *             (IdErrno) given as argument.
 * @param code Error code (in IdErrno)
 * \warning    The error code must be found in IdErrno. 
 * @return     Pointer to the string describing the error code.
 */

char *IdErrMsg(int code)
{
	MSGERROR *ptab = taberror;
	while (ptab->msg) {
		if (ptab->code == code)
			return ptab->msg;
		ptab++;
	}
	return "Unknown Error Code";
}
