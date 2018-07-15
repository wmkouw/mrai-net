/*************************************************************************
* $Id: imabasic.c,v 1.1 2005/09/09 08:22:45 bellet Exp $
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

#include "idima.h"

/**
 * \ingroup      ima
 * \brief        Returns the number of pixels along the y axis (vertical) 
 *               actually USED i.e. the current usefull height of the image.
 *               This is to be distinguished from IdImaDimY(image) which
 *               is the full height of the image as represented in memory.
 *               IdImaUsedNbY is typically usefull when working on a
 *               subimage.
 * @param image  image whose USEFULL height is queried
 * @return       vertical number of USED pixels of image
 */
int
IdImaUsedNbY(void * image)
{
	return  ( _IdImaPrivate(image)->UsedNbY);
}

/**
 * \ingroup      ima
 * \brief        Alias of IdImaUsedNbY.
 * @param image  image whose USEFULL height is queried
 * @return       vertical number of USED pixels of image
 */
int
IdImaGetUsedNbY(void * image)
{
	return IdImaUsedNbY(image);
}

/**
 * \ingroup      ima
 * \brief        Returns the number of pixels along the x axis (horizontal) 
 *               actually USED i.e. the current usefull width of the image.
 *               This is to be distinguished from IdImaDimX(image) which
 *               is the full width of the image as represented in memory.
 *               IdImaUsedNbX is typically usefull when working on a
 *               subimage.
 * @param image  image whose USEFULL width is queried
 * @return       horizontal number of USED pixels of image
 */
int
IdImaUsedNbX(void * image)
{
	return  ( _IdImaPrivate(image)->UsedNbX);
}

/**
 * \ingroup      ima
 * \brief        Alias of IdImaUsedNbX.
 * @param image  image whose USEFULL width is queried
 * @return       horizontal number of USED pixels of image
 */
int
IdImaGetUsedNbX(void * image)
{
	return IdImaUsedNbX(image);
}

/**
 * \ingroup      ima
 * \brief        Returns the height of the image i.e. the vertical
 *               (y axis) number of pixels.
 * @param image  image whose height is queried
 * @return       vertical number of pixels of image
 */
int
IdImaDimY(void * image)
{
	return  ( _IdImaPrivate(image)->DimY);
}

/**
 * \ingroup      ima
 * \brief        Returns the width of the image i.e. the horizontal
 *               (x axis) number of pixels.
 * @param image  image whose width is queried
 * @return       horizontal number of pixels of image
 */
int
IdImaDimX(void * image)
{
	return  ( _IdImaPrivate(image)->DimX);
}

/**
 * \ingroup      ima
 * \brief        Returns the number of bits per pixel of image
 * @param image  queried image
 * @return       Number of bits per pixel of image
 */
int
IdImaBitsPerPixel(void * image)
{
	return  ( _IdImaPrivate(image)->BitsPerPixel);
}

/**
 * \ingroup      ima
 * \brief        Returns the Ido type of the image (e.g. IMA_CHAR, IMA_UCHAR,
 *               IMA_SHORT, IMA_DOUBLE...
 * @param image  image whose Type is queried
 * @return       Ido type of image
 */
int
IdImaType(void * image)
{
	return  ( _IdImaPrivate(image)->Type);
}

/**
 * \ingroup       ima
 * \brief         Sets the number of used pixels in the vertical
 *                direction of a given image.
 * @param image   image whose UsedNbY is to be modified
 * @param usedNby UsedNbY to be set
 */
void
IdImaSetUsedNbY(void * image, int usedNby)
{
	_IdImaPrivate(image)->UsedNbY = usedNby;
}

/**
 * \ingroup       ima
 * \brief         Sets the number of used pixels in the horizontal
 *                direction of a given image.
 * @param image   image whose UsedNbX is to be modified
 * @param usedNbx UsedNbX to be set
 */
void
IdImaSetUsedNbX(void * image, int usedNbx)
{
	_IdImaPrivate(image)->UsedNbX = usedNbx;
}

/**
 * \ingroup      ima
 * \brief        Sets an arbitrary height to an given image i.e. the vertical
 *               (y axis) number of pixels.
 * \warning      This function should be used with extreme caution. They are
 *               very few good reasons to use it and even the IdoDude once
 *               said "Using IdImaSetDimY is calling for trouble".
 * @param image  image whose height is to be modified
 * @param height height to be set
 */
void
IdImaSetDimY(void * image, int height)
{
	_IdImaPrivate(image)->DimY = height;
}

/**
 * \ingroup      ima
 * \brief        Sets an arbitrary width to an given image i.e. the horizontal
 *               (x axis) number of pixels.
 * \warning      This function should be used with extreme caution. They are
 *               very few good reasons to use it and even the IdoDude once
 *               said "Using IdImaSetDimX is calling for trouble".
 * @param image  image whose width is to be modified
 * @param width  width to be set
 */
void
IdImaSetDimX(void * image, int width)
{
	_IdImaPrivate(image)->DimX = width;
}

/**
 * \ingroup      ima
 * \brief        Sets an arbitrary BitsPerPixel to a given image
 * @param image  image whose BitsPerPixel is to be modified
 * @param bits   Number of bits per pixel to be set
 */
void
IdImaSetBitsPerPixel(void * image, int bits)
{
	_IdImaPrivate(image)->BitsPerPixel = bits;
}

/**
 * \ingroup      ima
 * \brief        Sets an arbitrary type to a given image
 * @param image  image whose Type is to be modified
 * @param type   type to be set
 */
void
IdImaSetType(void * image, int type)
{
	_IdImaPrivate(image)->Type = type;
}

/**
 * \ingroup      ima
 * \brief        Returns the value of the pixel coordinates x,y of
 *               an image of type PPIMAGE_BIT.
 * @param image  image whose pixel is queried.
 * @param x      x coordinate of required pixel
 * @param y      y coordinate of required pixel
 * @return       value (0 or 1) at specified pixel
 */
int
IdImaGetBit(char ** image, int x, int y)
{
	return (((image)[y][(x)>>3]>>((x)&7) )&1);
}

/**
 * \ingroup      ima
 * \brief        Sets the value of the pixel of coordinates x,y
 *               an image of type PPIMAGE_BIT.
 * @param image  image whose pixel is to be set.
 * @param x      x coordinate of required pixel
 * @param y      y coordinate of required pixel
 * @param val    value to be set at pixel (0 or 1)
 */
void
IdImaPutBit(char ** image, int x, int y, int val)
{
	val ? ( ((image)[y][x>>3])|= (1<<( x &7) ) )
	    : ( ((image)[y][x>>3])&=~(1<<( x &7) ) );
}

/**
 * \ingroup      ima
 * \brief        Determines wether two images are of same dimensions.
 * @param image1 first image to compare
 * @param image2 second image to compare
 * @return       True when the two images have same dimensions.
 *               False otherwise.
 */
int
IdImaSameSize(void *image1, void *image2)
{
	return (   ( image1 && image2 )
                && ( IdImaDimX(image1) == IdImaDimX(image2) )
                && ( IdImaDimY(image1) == IdImaDimY(image2) ) );
}

/**
 * \ingroup      ima
 * \brief        Determines wether two images are of same Type.
 * @param image1 first  image to compare
 * @param image2 second image to compare
 * @return       True when the two images have same Type.
 *               False otherwise.
 */
int
IdImaSameType(void *image1, void *image2)
{
	return (   ( image1 && image2 )
                && ( IdImaType(image1) == IdImaType(image2) ) );
}

/**
 * \ingroup      ima
 * \brief        Determines wether two images are of same Type and
 *               same dimensions.
 * @param image1 first image to compare
 * @param image2 second image to compare
 * @return       True when the two images have same Type and dimensions.
 *               False otherwise.
 */
int
IdImaSameSizeAndType(void *image1, void *image2)
{
	return (   ( image1 && image2 )
                && ( IdImaDimX(image1) == IdImaDimX(image2) )
                && ( IdImaDimY(image1) == IdImaDimY(image2) )
                && ( IdImaType(image1) == IdImaType(image2) ) );
}
