/*************************************************************************
* $Id: idpoint.h,v 1.1 2005/09/09 08:22:37 bellet Exp $
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
*  Description : Declarations permettant d'utiliser le type IDPOINT
*                de la bibliotheque LIBIDO
*
**************************************************************************/
#ifndef __IDPOINT_H__
#define __IDPOINT_H__

#include "idgen.h"

/* Types de point 3D */

#define PNT_FLOAT 	   (PNT | TY_FLOAT)

/* structure IDPOINT */
typedef struct __IdPoint__{
        int Number;
        float X,Y,Z;
        int NbDatas;
        float *Datas; /* pointeur vers tableau de donnees
                        (user specific)*/
        } IDPOINT;

typedef IDPOINT *PIDPOINT; 

/************************************************************************/

/* prototypes des fonctions POINT */

PIDPOINT IdPntAlloc     (int);            /* nb float pour data */
PIDPOINT IdPntRemplit   (PIDPOINT,double,double,double,int);
PIDPOINT *IdPntTabAlloc (int,int);
void     IdPntCopy      (PIDPOINT,PIDPOINT); 

#ifndef SWIG
#define  IdPntFree(pi)        		_IdPntFree((PIDPOINT *)&(pi) )
#define  IdPntGetDataLength(pi)		(pi)->NbDatas
#define  IdPntGetData(pi)    		(pi)->Datas
#define  IdPntTabFree(pp,nbp,libere) 	_IdPntTabFree ((PIDPOINT **)&(pp),(nbp),(libere) )

/* Bazar Laurent Belatrech	*/

#define IdPntX(p)  (p).X
#define IdPntY(p)  (p).Y
#define IdPntZ(p)  (p).Z
#define IdPntSamePnt(p1,p2) (    IdPntX(p1)==IdPntX(p2)      \
			      && IdPntY(p1)==IdPntY(p2)      \
			      && IdPntZ(p1)==IdPntZ(p2) )
#else 

void 	IdPntFree(PIDPOINT);
int 	IdPntGetDataLength(PIDPOINT);
float *	IdPntGetData(PIDPOINT);
void 	IdPntTabFree(PIDPOINT*, int,int);
float 	IdPntX(IDPOINT);
float 	IdPntY(IDPOINT);
float 	IdPntZ(IDPOINT);
int 	IdPntSamePnt(IDPOINT,IDPOINT);
#endif

/************************************************************************/

/* Fonctions internes */



/************************************************************************/

/* FUNCTION DESCRIPTION **************************************************

IdPntGetDataLength  (macro)

RESUME: Renvoie le nombre d'elements du vecteur de Donnees associe a 1 IDPOINT

DESCRIPTION: Renvoie le nombre d'elements du vecteur de Donnees associe a 1 IDPOINT

SYNTAXE:     int nb_float = IdPntGetDataLength(PIDPOINT p1);

RETOUR:      nom   : nb_float
	     type  : int
	     role  : nombre de donnees utilisateur(en float) allouees
		     cf structure point dans idpoint.h
PARAMETRES:
	     type  : PIDPOINT
	     role  : pointeur sur le point alloue

FICHIER:     idpoint.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdPntGetData  (macro)

RESUME: Renvoie le vecteur de Donnees associe a 1 IDPOINT

DESCRIPTION: Renvoie le vecteur de Donnees associe a 1 IDPOINT

SYNTAXE:     float Datas[] = IdPntGetData(PIDPOINT p1);

RETOUR:      nom   : Datas
	     type  : float []
	     role  : tableau de donnees utilisateur(en float) allouees
		     cf structure point dans idpoint.h
PARAMETRES:
	     type  : PIDPOINT
	     role  : pointeur sur le point alloue

FICHIER:     idpoint.h

******************************************************** END DESCRIPTION */


#endif

