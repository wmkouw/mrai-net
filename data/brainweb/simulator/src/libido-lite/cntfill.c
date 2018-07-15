/*************************************************************************
* $Id: cntfill.c,v 1.1 2005/09/09 08:22:22 bellet Exp $
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

#include <math.h>
#include "idcnt.h"
#include "idima.h"

/**
 * \ingroup       cnt
 * \brief         Draws a polygon or polyline (depending on the closed parameter)
 *                on an image out of a CNT_USHORT with a given origin.
 * @param contour Pointer to source contour.
 * @param ox      Offset along the X axis.
 * @param oy      Offset along the Y axis.
 * @param image   Pointer to destination image.
 * @param code    Type of drawing : 0 means direct drawing, 1 means AND,
 *                2 for OR and 3 for XOR (refer to IdImaDrawLine).
 * @param closed  0 for a polyline, 1 for a polygone (closes the polyline by
 *                drawing an additional line starting at last point of contour
 *                and ending at first point)
 * @param greylevel Grey level of the line to draw.
 * @return        0 on failure, 1 on success.
 */
int
_IdCntDrawPolygonOffset(PCONTOUR_USHORT contour,
			int ox, int oy,
			PPIMAGE_UCHAR image,
			int code, int closed, int greylevel)
{
	int i;
	int NbPts = IdCntUsedNbX(contour);
	if (NbPts < 2) {
		return 0;
	}

	for (i = 0; i < NbPts - 1; i++) {
		if (!IdImaDrawLine
		    (image, greylevel, code, IdCntGetX(contour, i) - ox,
		     IdCntGetY(contour, i) - oy, IdCntGetX(contour,
							   i + 1) - ox,
		     IdCntGetY(contour, i + 1) - oy)) {
			return 0;
		}
	}
	if (closed == 1)
		if (!IdImaDrawLine
		    (image, greylevel, code,
		     IdCntGetX(contour, NbPts - 1) - ox, IdCntGetY(contour,
								   NbPts -
								   1) - oy,
		     IdCntGetX(contour, 0) - ox, IdCntGetY(contour,
							   0) -
		     oy)) {return 0;
		}

	return (1);
}

/**
 * \ingroup       cnt
 * \brief         Draws a poly line (continuous collection of linear segments)
 *                on an image out of a CNT_USHORT.
 * @param contour Pointer to source contour.
 * @param image   Pointer to destination image.
 * @param code    Type of drawing : 0 means direct drawing, 1 means AND,
 *                2 for OR and 3 for XOR (refer to IdImaDrawLine).
 * @param greylevel Grey level of the line to draw.
 * @return        0 on failure, 1 on sucess.
 */
int 
IdCntDrawPolyLine(PCONTOUR_USHORT contour,
		PPIMAGE_UCHAR image,
		int code,
		int greylevel)
{
	return _IdCntDrawPolygonOffset(contour,
			0,0, image, code, 0, greylevel);
}

/**
 * \ingroup       cnt
 * \brief         Draws an "offseted" poly line (continuous collection of
 *                linear segments) on an image out of a CNT_USHORT.
 * @param contour Pointer to source contour.
 * @param ox      Offset along the X axis.
 * @param oy      Offset along the Y axis.
 * @param image   Pointer to destination image.
 * @param code    Type of drawing : 0 means direct drawing, 1 means AND,
 *                2 for OR and 3 for XOR (refer to IdImaDrawLine).
 * @param greylevel Grey level of the line to draw.
 * @return        0 on failure, 1 on sucess.
 */
int 
IdCntDrawPolyLineOffset(PCONTOUR_USHORT contour,
			int ox, int oy,
			PPIMAGE_UCHAR image,
			int code,
			int greylevel)
{
	return _IdCntDrawPolygonOffset(contour,
			ox, oy, image, code, 0, greylevel);
}

/**
 * \ingroup       cnt
 * \brief         Draws a polygon (closed continuous collection of linear
 *                segments) on an image out of a CNT_USHORT.
 * @param contour Pointer to source contour.
 * @param image   Pointer to destination image.
 * @param code    Type of drawing : 0 means direct drawing, 1 means AND,
 *                2 for OR and 3 for XOR (refer to IdImaDrawLine).
 * @param greylevel Grey level of the line to draw.
 * @return        0 on failure, 1 on sucess.
 */
int
IdCntDrawPolygon(PCONTOUR_USHORT contour,
		PPIMAGE_UCHAR image,
		int code, int greylevel)
{
	return _IdCntDrawPolygonOffset(contour,
			0,0, image, code, 1, greylevel);
}

/**
 * \ingroup       cnt
 * \brief         Draws an "offseted" polygon (closed continuous collection
 *                of linear segments) on an image out of a CNT_USHORT
 * @param contour Pointer to source contour.
 * @param ox      Offset along the X axis.
 * @param oy      Offset along the Y axis.
 * @param image   Pointer to destination image.
 * @param code    Type of drawing : 0 means direct drawing, 1 means AND,
 *                2 for OR and 3 for XOR (refer to IdImaDrawLine).
 * @param greylevel Grey level of the line to draw.
 * @return        0 on failure, 1 on sucess.
 */
int
IdCntDrawPolygonOffset(PCONTOUR_USHORT contour,
			int ox, int oy,
			PPIMAGE_UCHAR image,
			int code, int greylevel)
{
	return _IdCntDrawPolygonOffset(contour,
			ox, oy, image, code, 1, greylevel);
}
