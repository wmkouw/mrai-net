/*************************************************************************
* $Id: iderr.h,v 1.1 2005/09/09 08:22:29 bellet Exp $
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
*  Description : Gestion des erreurs dans LIBIDO.
*                VOIR gereer.c
*
**************************************************************************/

#ifndef __IDERR_H__
#define __IDERR_H__

// Provisoirement la, juska ce que tout les IdExit soient tous vires des
// sources de libido... <<Soupir>>
void   IdExit                 (int);

extern int IdErrno;			/* Definie dans gereerr.c */

char * IdErrMsg      (int);

#define IDERR_Y_OUT_OF_IMAGE	201	/* Sortie de l'image en y	*/
#define IDERR_X_OUT_OF_IMAGE	202	/* Sortie de l'image en x	*/
#define IDERR_Y_GREATER_DIMY	203	/* Parametre dy > DIMY Fenetre	*/
#define IDERR_X_GREATER_DIMX	204	/* Parametre dx > DIMX Fenetre	*/

#define IDERR_NOT_SQUARE	206	/* Image non carree		*/

#define IDERR_POINTER_IS_NULL	210	/* Pointeur NULL (parametre) 	*/
#define IDERR_NOT_FLOATLIST	220	/* Liste flottants incorrecte	*/
#define IDERR_LABEL_UNDEF	221	/* Label non defini            	*/
#define IDERR_TOO_LONG_LIST	222	/* Liste avec trop d'elements  	*/
#define IDERR_INVALID_NUMBER	223	/* Parametre Numerique invalide	*/
#define IDERR_NOT_INTLIST	224	/* Liste entiers incorrecte	*/

#define IDERR_WRONG_TYPE	225	/* Mauvais type Libido		*/
#define IDERR_UNDEF_SCREEN	226	/* Code ecran invalide		*/

#define IDERR_WRONG_DIM		227	/* Dimensions invalides		*/
#define IDERR_WRONG_IMAGES	228	/* Images incompatibles		*/
#define IDERR_WRONG_LIBTYPE	229	/* Type libido invalide		*/


#define IDERR_Z_OUT_OF_VOLUME  301	/* Sortie du volume en z	*/
#define IDERR_Y_OUT_OF_VOLUME  302	/* Sortie du volume en y	*/
#define IDERR_X_OUT_OF_VOLUME  303	/* Sortie du volume en x	*/
#define IDERR_Z_GREATER_DIMZ   304	/* Parametre dz > DIMZ Fenetre	*/
#define IDERR_WRONG_VOLUMES    305	/* Volumes incompatibles	*/

#define IDERR_WRONG_SIGNALS	234	/* Signaux incompatibles	*/

#define IDERR_OPEN_FAILED	211	/* Erreur ouverture fichier	*/
#define IDERR_READ_FAILED	235	/* Erreur Lecture fichier	*/
#define IDERR_WRITE_FAILED	236	/* Erreur Ecriture fichier	*/
#define IDERR_NON_ACR_FILE	243	/* Lecture Fichier NON ACR	*/

#define IDERR_ALLOC_MEM		200	/* erreur d'allocation memoiire	*/
#define IDERR_ALLOC_SIG		230	/* erreur Alloc Signal		*/
#define IDERR_ALLOC_IMA		231	/* erreur Alloc Image		*/
#define IDERR_ALLOC_VOL		232	/* erreur Alloc Volume		*/
#define IDERR_ALLOC_SUB		233	/* erreur Alloc Sous-Structure	*/

#define IDERR_ALLOC_PARAM	237	/* Echec allocation PARAM	*/
#define IDERR_ALLOC_CNT		238	/* Echec allocation Contour	*/
#define IDERR_ALLOC_CNT3D	255	/* Echec allocation Contour 3D	*/
#define IDERR_ALLOC_LST		239	/* Echec allocation Liste	*/
#define IDERR_ALLOC_TREE	240	/* Echec allocation Arbre	*/
#define IDERR_ALLOC_FACE	241	/* Echec allocation Face	*/
#define IDERR_ALLOC_ELEM	244	/* Echec allocation Element	*/
#define IDERR_EMPTY_LIST	245	/* Liste Vide 			*/

#define IDERR_WRONG_CONTOURS	242	/* Contours incompatibles	*/
#define IDERR_WRONG_TYPES	246	/* Types incompatibles	*/

#define IDERR_WRONG_FILE_NAME	247	/* Nom Fichier incorrect	*/
#define IDERR_EMPTY_FILE_NAME	248	/* Nom Fichier NULL	*/

#define IDERR_Z_OUT_OF_SEQUENCE  249	/* Sortie  Sequence en z	*/
#define IDERR_Y_OUT_OF_SEQUENCE  250	/* Sortie  Sequence en y	*/
#define IDERR_X_OUT_OF_SEQUENCE  251	/* Sortie  Sequence en x	*/
#define IDERR_T_OUT_OF_SEQUENCE  252	/* Sortie  Sequence en t	*/

#define IDERR_ALLOC_SEQ  	 253	/* Echec allocation Sequence	*/
#define IDERR_WRONG_SEQUENCES	 254	/* Sequences incompatibles	*/
#define IDERR_MIN_EQ_MAX	 256	/* Min = Max			*/


/* Next : 257 */

#define IDERR_BAD_DIRECTION     		800
#define IDERR_THRESHOLD_OUT_OF_RANGE		801
#define IDERR_NULL_POINTER_ARGUMENT		802
#define IDERR_WRONG_IMAGE_TYPE			803
#define IDERR_WRONG_PARAMETER			804
#define IDERR_OUT_OF_RANGE			805
#define IDERR_CALCULATION			806
#define IDERR_WRONG_MATRIX_DIMENSION		807
#define IDERR_WRONG_IMAGE_SIZE			808
#define IDERR_WINDOW_SIZE_LESS_THAN_ZERO	810
#define IDERR_POINTER_IS_ZERO			811
#define IDERR_NO_IMAGE_ALLOCATED		812
#define IDERR_WINDOW_SIZE_NOT_POWER_TO_2	809
#define IDERR_PARAMETER_NOT_IN_RANGE		813
#define IDERR_PARAMETER_MUST_BE_BOOLEAN		814

#endif


/* DEFINITION DESCRIPTION **************************************************

iderr.h  (codes_erreurs)

RESUME: Codes d'erreur retournes par les fonctions LibIdo

DESCRIPTION : Codes d'erreur retournes par les differentes fonctions LibIDO.
              Voir IdErrMsg, IdErrPrintf.

  0				, "Pas d'erreur"		,
  IDERR_NOT_FLOATLIST		, "Liste flottants incorrecte"	,
  IDERR_LABEL_UNDEF		, "Label non defini"		,
  IDERR_TOO_LONG_LIST		, "Liste avec trop d'elements"	,
  IDERR_INVALID_NUMBER		, "Param Numerique invalide"	,
  IDERR_NOT_INTLIST		, "Liste entiers incorrecte"	,
  IDERR_ALLOC_MEM		, "Echec allocation memoire"	,
  IDERR_ALLOC_SIG		, "Echec allocation Signal"	,
  IDERR_ALLOC_IMA		, "Echec allocation Image"	,
  IDERR_ALLOC_VOL		, "Echec allocation Volume"	,
  IDERR_ALLOC_PARAM		, "Echec allocation PARAM"	,
  IDERR_ALLOC_CNT		, "Echec allocation Contour"	,
  IDERR_ALLOC_CNT3D		, "Echec allocation Contour3D"	,
  IDERR_ALLOC_LST		, "Echec allocation Liste"	,
  IDERR_ALLOC_TREE		, "Echec allocation Arbre"	,
  IDERR_ALLOC_FACE		, "Echec allocation Face"	,
  IDERR_ALLOC_LST		, "Echec allocation Liste"	,
  IDERR_ALLOC_ELEM		, "Echec allocation Element"	,
  IDERR_EMPTY_LIST		, "Liste Vide"			,
  IDERR_ALLOC_SUB		, "Echec allocation Sous-Struct",
  IDERR_Y_OUT_OF_IMAGE		, "Sortie de l'image en y"	,
  IDERR_X_OUT_OF_IMAGE		, "Sortie de l'image en x"	,
  IDERR_Z_OUT_OF_VOLUME		, "Sortie du volume en z"	,
  IDERR_Y_OUT_OF_VOLUME		, "Sortie du volume en y"	,
  IDERR_X_OUT_OF_VOLUME		, "Sortie du volume en x"	,
  IDERR_Y_GREATER_DIMY		, "Parametre dy > DIMY Fenetre"	,
  IDERR_X_GREATER_DIMX		, "Parametre dx > DIMX Fenetre"	,
  IDERR_Z_GREATER_DIMZ		, "Parametre dz > DIMZ Fenetre"	,
  IDERR_NOT_SQUARE		, "Image non carree"		,
  IDERR_POINTER_IS_NULL		, "Pointeur NULL"		,
  IDERR_WRONG_TYPE		, "Mauvais type Libido"		,
  IDERR_WRONG_LIBTYPE		, "Type Libido incorrect"	,
  IDERR_UNDEF_SCREEN		, "Code ecran invalide"		,
  IDERR_WRONG_DIM		, "Dimensions invalides"	,
  IDERR_WRONG_SIGNALS		, "Signaux incompatibles"	,
  IDERR_WRONG_IMAGES		, "Images incompatibles"	,
  IDERR_WRONG_VOLUMES		, "Volumes incompatibles"	,
  IDERR_WRONG_CONTOURS		, "Contours incompatibles"	,
  IDERR_WRONG_TYPES		, "Types incompatibles"         ;
  IDERR_OPEN_FAILED		, "Erreur Ouverture fichier"	,
  IDERR_READ_FAILED		, "Erreur Lecture fichier"	,
  IDERR_WRITE_FAILED		, "Erreur Ecriture fichier"	,
  IDERR_NON_ACR_FILE		, "Lecture Fichier NON ACR"     ,
  IDERR_WRONG_FILE_NAME		, "Nom Fichier incorrect"	,
  IDERR_EMPTY_FILE_NAME		, "Nom Fichier NULL"	

                           
******************************************************** END DESCRIPTION */
