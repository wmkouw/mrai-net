/*************************************************************************
* $Id: imachekal.c,v 1.1 2005/09/09 08:22:45 bellet Exp $
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
 * Compatibility/allocation of images
 */
#include "idima.h"
#include "iderr.h"

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdImaCheckTypeSizeAlloc  (fonction)
 * 
 * RESUME: Test Compatib. 2 im (type/size); Alloc meme taille/type si 2-ieme non allouee
 * 
 * DESCRIPTION: Test Compatib. 2 im (type/size); Alloc meme taille/type si 2-ieme non allouee.
 * On peut imposer un Type donne (2-ieme parametre). 
 * S'il vaut -1, tous types autorises
 * 
 * SYNTAXE:     PPIMAGE imDest = IdImaCheckTypeSizeAlloc (PPIMAGE imSource, int typeImpose, PPIMAGE imDest);
 * 
 * RETOUR:      type  : PPIMAGE
 * role  : Pointeur vers l'image resultat. Zero si echec.
 * 
 * PARAMETRES:  nom   : imSource
 * type  : PPIMAGE
 * role  : Pointeur vers l'image source.
 * 
 * nom   : typeImpose
 * type  : int
 * role  : type Obligatoire pour l'image Source
 * s'il vaut -1, tous les types sont autorises
 * 
 * nom   : imDest
 * type  : PPIMAGE
 * role  : Pointeur vers l'image resultat
 * Si ce pointeur est nul, l'image est allouee par
 * la fonction.
 * Cette image peut eventuellement etre l'image source.
 * 
 * FICHIER:     ImChekAll.c
 * 
 * EXEMPLE:     if ( (imd=IdImaCheckTypeSizeAlloc(ims,-1,imd))==0 )
 * IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
 * 
 * ******************************************************** END DESCRIPTION 
 */

PPIMAGE
IdImaCheckTypeSizeAlloc(PPIMAGE imSource, int typeImpose, PPIMAGE imDest)
{
	if (!imSource) {
		IdErrno = IDERR_POINTER_IS_NULL;
		return 0;
	}

	if (IdLibidoType(imSource) != IMA) {
		IdErrno = IDERR_WRONG_LIBTYPE;
		return 0;
	}

	if (typeImpose != -1) {
		/*
		 * y a-t-il un type Impose 
		 */
		if (IdImaType(imSource) != typeImpose) {
			IdErrno = IDERR_WRONG_LIBTYPE;
			return 0;
		}
	}

	if (!imDest) {
		/*
		 * Alloc. si necessaire *
		 */
		imDest = (PPIMAGE) IdImaAllocLikeImage(imSource);
		if (!imDest) {
			IdErrno = IDERR_ALLOC_IMA;
			return 0;
		}
		return imDest;
	} else {
		/*
		 * Meme taille et type ? *
		 */

		if (!IdImaSameSizeAndType(imSource, imDest)) {
			IdErrno = IDERR_WRONG_IMAGES;
			return 0;
		} else
			return imDest;
	}
}

PPIMAGE
IdImaCheckSizeTypeAlloc(PPIMAGE SourceIm, int type, PPIMAGE DestIm)
{
	return IdImaCheckTypeSizeAlloc(SourceIm, type, DestIm);
}


/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdImaCheckSizeAlloc  (fonction)
 * 
 * RESUME: Test Compatib. 2 im (size); Alloc meme taille/type si 2-ieme non allouee.
 * 
 * DESCRIPTION: Test Compatib. 2 im (size); Alloc meme taille/type si 2-ieme non allouee.
 * On peut imposer un Type donne (2-ieme parametre. 
 * S'il vaut -1, tous types autorises)
 * 
 * SYNTAXE:     PPIMAGE imDest = IdImaCheckSizeAlloc (PPIMAGE imSource, int typeImpose, PPIMAGE imDest, int typeImDest);
 * 
 * RETOUR:      type  : PPIMAGE
 * role  : Pointeur vers l'image resultat. Zero si echec.
 * 
 * PARAMETRES:  nom   : imSource
 * type  : PPIMAGE
 * role  : Pointeur vers l'image source.
 * 
 * nom   : typeImpose
 * type  : int
 * role  : type Obligatoire pour l'image Source
 * s'il vaut -1, tous les types sont autorises
 * 
 * nom   : imDest
 * type  : PPIMAGE
 * role  : Pointeur vers l'image resultat
 * Si ce pointeur est nul, l'image est allouee par
 * la fonction.
 * Cette image peut eventuellement etre l'image source.
 * 
 * nom   : typeImDest
 * type  : int
 * role  : Type de l'image resultat
 * 
 * 
 * FICHIER:     ImChekAll.c
 * 
 * EXEMPLE:     if ( (imd=IdImaCheckSizeAlloc(ims,-1,imd,IMA_COMPLEX_DOUBLE))==0 )
 * IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
 * 
 * ******************************************************** END DESCRIPTION 
 */

PPIMAGE
IdImaCheckSizeAlloc(PPIMAGE imSource, int typeImpose, PPIMAGE imDest)
{
	if (!imSource) {
		IdErrno = IDERR_POINTER_IS_NULL;
		return 0;
	}

	if (IdLibidoType(imSource) != IMA) {
		IdErrno = IDERR_WRONG_LIBTYPE;
		return 0;
	}
	if (typeImpose != -1) {
		/*
		 * y a-t-il un type Impose 
		 */
		if (IdImaType(imSource) != typeImpose) {
			IdErrno = IDERR_WRONG_LIBTYPE;
			return 0;
		}
	}

	if (!imDest) {
		/*
		 * Alloc. si necessaire *
		 */
		imDest = (PPIMAGE) IdImaAlloc(IdImaDimX(imSource),
					      IdImaDimY(imSource),
					      IdImaType(imSource));
		if (!imDest) {
			IdErrno = IDERR_ALLOC_IMA;
			return 0;
		}
		return imDest;
	} else {
		/*
		 * Meme taille ? *
		 */
		if (!IdImaSameSize(imSource, imDest)) {
			IdErrno = IDERR_WRONG_IMAGES;
			return 0;
		} else
			return imDest;
	}
}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdImaCheckTypeAlloc  (fonction)
 * 
 * RESUME: Test Compatib. 2 im (type); Alloc meme taille/type si 2-ieme non alloueee
 * 
 * DESCRIPTION: Test Compatib. 2 im (type); Alloc meme taille/type si 2-ieme non alloueee
 * On peut imposer un Type donne (2-ieme parametre. 
 * S'il vaut -1, tous types autorises)
 * 
 * SYNTAXE:     PPIMAGE imDest = IdImaCheckTypeAlloc (PPIMAGE imSource, int typeImpose, PPIMAGE imDest);
 * 
 * RETOUR:      type  : PPIMAGE
 * role  : Pointeur vers l'image resultat. Zero si echec.
 * 
 * PARAMETRES:  nom   : imSource
 * type  : PPIMAGE
 * role  : Pointeur vers l'image source.
 * 
 * nom   : typeImpose
 * type  : int
 * role  : type Obligatoire pour l'image Source
 * s'il vaut -1, tous les types sont autorises
 * 
 * nom   : imDest
 * type  : PPIMAGE
 * role  : Pointeur vers l'image resultat
 * Si ce pointeur est nul, l'image est allouee par
 * la fonction.
 * Cette image peut eventuellement etre l'image source.
 * 
 * 
 * FICHIER:     ImChekAll.c
 * 
 * EXEMPLE:     if ( (imd=IdImaCheckTypeAlloc(ims,-1,imd))==0 )
 * IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
 * 
 * ******************************************************** END DESCRIPTION 
 */

PPIMAGE
IdImaCheckTypeAlloc(PPIMAGE imSource, int typeImpose, PPIMAGE imDest)
{
	if (!imSource) {
		IdErrno = IDERR_POINTER_IS_NULL;
		return 0;
	}

	if (IdLibidoType(imSource) != IMA) {
		IdErrno = IDERR_WRONG_LIBTYPE;
		return 0;
	}

	if (typeImpose != -1) {
		/*
		 * y a-t-il un type Impose 
		 */
		if (IdImaType(imSource) != typeImpose) {
			IdErrno = IDERR_WRONG_LIBTYPE;
			return 0;
		}
	}

	if (!imDest) {
		/*
		 * Alloc. si necessaire *
		 */
		imDest = (PPIMAGE) IdImaAlloc(IdImaDimX(imSource),
					      IdImaDimY(imSource),
					      IdImaType(imSource));
		if (!imDest) {
			IdErrno = IDERR_ALLOC_IMA;
			return 0;
		}
		return imDest;
	} else {
		/*
		 * Meme type ? *
		 */
		if (!IdImaSameType(imSource, imDest)) {
			IdErrno = IDERR_WRONG_IMAGES;
			return 0;
		} else
			return imDest;
	}
}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdImaCheckSizeAllocType  (fonction)
 * 
 * RESUME: Test Compatib. 2 im (size); Alloc type choisi/meme taille si 2-ieme non allouee.
 * 
 * DESCRIPTION: Test Compatib. 2 im (size); Alloc type choisi/meme taille si 2-ieme non allouee.
 * On PEUT imposer un Type donne pour l'image origine (2-ieme parametre. 
 * S'il vaut -1, tous types autorises)
 * On DOIT preciser le type de l'image destination.
 * 
 * SYNTAXE:     PPIMAGE imDest = IdImaCheckSizeAlloc (PPIMAGE imSource, int typeImpose, PPIMAGE imDest, int typeImDest);
 * 
 * RETOUR:      type  : PPIMAGE
 * role  : Pointeur vers l'image resultat. Zero si echec.
 * 
 * PARAMETRES:  nom   : imSource
 * type  : PPIMAGE
 * role  : Pointeur vers l'image source.
 * 
 * nom   : typeImpose
 * type  : int
 * role  : type Obligatoire pour l'image Source
 * s'il vaut -1, tous les types sont autorises
 * 
 * nom   : imDest
 * type  : PPIMAGE
 * role  : Pointeur vers l'image resultat
 * Si ce pointeur est nul, l'image est allouee par
 * la fonction.
 * Cette image peut eventuellement etre l'image source.
 * 
 * nom   : typeImDest
 * type  : int
 * role  : Type de l'image resultat
 * 
 * FICHIER:     ImChekAll.c
 * 
 * EXEMPLE:     if ( (imd=IdImaCheckSizeAlloc(ims,-1,imd,IMA_COMPLEX_DOUBLE))==0 )
 * IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
 * 
 * ******************************************************** END DESCRIPTION 
 */

PPIMAGE
IdImaCheckSizeAllocType(PPIMAGE imSource, int typeImpose,
			PPIMAGE imDest, int typeImDest)
{
	if (!imSource) {
		IdErrno = IDERR_POINTER_IS_NULL;
		return 0;
	}

	if (IdLibidoType(imSource) != IMA) {
		IdErrno = IDERR_WRONG_LIBTYPE;
		return 0;
	}

	if (typeImpose != -1) {
		/*
		 * y a-t-il un type Impose 
		 */
		if (IdImaType(imSource) != typeImpose) {
			IdErrno = IDERR_WRONG_LIBTYPE;
			return 0;
		}
	}

	if (!imDest) {
		/*
		 * Alloc. si necessaire
		 */
		imDest = (PPIMAGE) IdImaAlloc(IdImaDimX(imSource),
					      IdImaDimY(imSource),
					      typeImDest);
		if (!imDest) {
			IdErrno = IDERR_ALLOC_IMA;
			return 0;
		}
		return imDest;
	} else {
		/*
		 * Meme taille ? *
		 */
		if (!IdImaSameSize(imSource, imDest)) {
			IdErrno = IDERR_WRONG_IMAGES;
			return 0;
		} else
		 if (IdImaType(imDest) != typeImDest) {
			IdErrno = IDERR_WRONG_LIBTYPE;
			return 0;
		}

		return imDest;
	}
}

/*
 * FUNCTION DESCRIPTION **************************************************
 * 
 * IdImaCheckTypeAllocSize  (fonction)
 * 
 * RESUME: Test Compatib. 2 im (type); Alloc meme type,taille choisie si 2-ieme non allouee
 * 
 * DESCRIPTION: Test Compatib. 2 im (type);  Alloc meme type,taille choisie si 2-ieme non allouee.
 * On peut imposer un Type donne pour l'image Source (2-ieme parametre. 
 * S'il vaut -1, tous types autorises)
 * 
 * SYNTAXE:     PPIMAGE imDest = IdImaCheckTypeAllocSize (PPIMAGE imSource, int typeImpose, PPIMAGE imDest, int nouvTailleX, int nouvTailleY);
 * 
 * RETOUR:      type  : PPIMAGE
 * role  : Pointeur vers l'image resultat. Zero si echec.
 * 
 * PARAMETRES:  nom   : imSource
 * type  : PPIMAGE
 * role  : Pointeur vers l'image source.
 * 
 * nom   : typeImpose
 * type  : int
 * role  : type Obligatoire pour l'image Source
 * s'il vaut -1, tous les types sont autorises
 * 
 * nom   : imDest
 * type  : PPIMAGE
 * role  : Pointeur vers l'image resultat
 * Si ce pointeur est nul, l'image est allouee par
 * la fonction.
 * Cette image peut eventuellement etre l'image source.
 * 
 * nom   : nouvTailleX
 * type  : int
 * role  : Taille en X de l'image resultat, 
 * si elle n'est pas deja allouee
 * 
 * nom   : nouvTailleY
 * type  : int
 * role  : Taille en Y de l'image resultat, 
 * si elle n'est pas deja allouee
 * 
 * FICHIER:     ImChekAll.c
 * 
 * EXEMPLE:     if ( (imd=IdImaCheckTypeAllocSize(ims,-1,imd,128,256))==0 )
 * IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
 * 
 * ******************************************************** END DESCRIPTION 
 */

PPIMAGE
IdImaCheckTypeAllocSize(PPIMAGE imSource, int typeImpose,
			PPIMAGE imDest, int nouvTailleX, int nouvTailleY)
{
	if (!imSource) {
		IdErrno = IDERR_POINTER_IS_NULL;
		return 0;
	}

	if (IdLibidoType(imSource) != IMA) {
		IdErrno = IDERR_WRONG_LIBTYPE;
		return 0;
	}

	if (typeImpose != -1) {
		if (IdImaType(imSource) != typeImpose) {
			IdErrno = IDERR_WRONG_LIBTYPE;
			return 0;
		}
	}

	if (!imDest) {
		/*
		 * Alloc. si necessaire *
		 */
		imDest = (PPIMAGE) IdImaAlloc(nouvTailleX, nouvTailleY,
					      IdImaType(imSource));
		if (!imDest) {
			IdErrno = IDERR_ALLOC_IMA;
			return 0;
		}
		return imDest;
	} else {
		/*
		 * Meme type ? *
		 */

		if (!IdImaSameType(imSource, imDest)) {
			IdErrno = IDERR_WRONG_IMAGES;

			return 0;
		} else
			return imDest;
	}
}
