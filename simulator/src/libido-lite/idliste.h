/*************************************************************************
* $Id: idliste.h,v 1.1 2005/09/09 08:22:35 bellet Exp $
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

#ifndef __IDLISTE_H__
#define __IDLISTE_H__
#include "idgen.h"

/*
 * Declarations permettant d'utiliser le type LIST of IDO
 */

/* modes d'ajout ou de retrait a une liste */
/* 1 seul possible */

#define LST_MODE		0x0f
#define LST_BEG			0x0		/* au debut de la liste */
#define LST_END			0x1		/* a la fin de la liste */
#define LST_ELEM		0x2		/* apres un element */
#define LST_ELEM_AV     0x4		/* AVANT un element */   /*JPR */

/* OU possible sur les valeurs suivantes ex: LST_BEG | LST_PRED | LST_NEXT */
/* la liaison NEXT et en fait toujours realisee 			   */

#define LST_NEXT 	0x10		/* au champ Pred de l'element */
#define LST_PRED 	0x20		/* au champ Next de l'element 
			 			   pour arbre binaire */
#define LST_BIDIR 	0x40 		/* liste bidirect. */



 
/* Types de liste */

/* structure LIST */

typedef struct _elem{
	int		 Numero;
	int 		 TypeElem; /* Utile pour les objets PARAM */
	void 		*Object;
	struct _elem 	*Next;  /* En fait, un PLIST_ELEMENT */ 
	struct _elem 	*Pred;  /* En fait, un PLIST_ELEMENT */ 
	} LIST_ELEMENT;

typedef LIST_ELEMENT *PLIST_ELEMENT;

typedef PLIST_ELEMENT PLIST , *PPLIST;

typedef struct {
	  int   	NbElems;
	  PLIST_ELEMENT First;
	  PLIST_ELEMENT Last;  /* inutilise pour un Arbre */
          int   	Numero;  /* A quoi sert-il ici ??? */
	  char  	*_message,*_fichier;
	  int   	Type; /* LST */ /* IMPERATIVEMENT DERNIER DE LA STRUCTURE */
	} PRIVATE_LIST;

#define   _IdLstPrivate(lst) (((PRIVATE_LIST*)(lst))-1)

#include "idliste-restricted.h"


	/* Bazard Laurent Belatrech */
#ifndef SWIG
#define IdLstNumero(lst)     (_IdLstPrivate(lst)->Numero)
#define IdLstMessage(lst)    (_IdLstPrivate(lst)->_message)
#define IdLstFichier(lst)    (_IdLstPrivate(lst)->_fichier)
#else
int    IdLstNumero(PLIST);
char * IdLstMessage(PLIST);
char * IdLstFichier(PLIST);
#endif




void  IdLstClear(PLIST lst);
PLIST IdLstCopy(PLIST lst);



#ifndef SWIG
#define IdLstNbElems(l)   ( _IdLstPrivate(l)->NbElems)
#define IdLstNbObjects(l) ( _IdLstPrivate(l)->NbElems)
#define IdLstType(l) ( _IdLstPrivate(l)->Type)
#define IdLstGetTypeObj(el) ( (el)->TypeElem)
#define IdLstSetTypeObj(el, type) ( (el)->TypeElem) = (type)
#define IdLstFirst(l) ( _IdLstPrivate(l)->First)
#define IdLstList(l)  ( _IdLstPrivate(l)->First)
 
/* compatibilite avec anciennes versions JPR */
#define IdLstLast(l)  ( _IdLstPrivate(l)->Last)

#define IdLstNext(el) ( ((el) )->Next)
#define IdLstPrevious(el)  ( ((el) )->Pred)
#define IdLstPtrObj(el)  ( ((el))->Object)
#define IdLstAddFirst(list,obj)					\
		  IdLstAddElement((list),((void *)obj),LST_BEG|LST_BIDIR,NULL)
#define IdLstAddLast(list,obj)						\
		  IdLstAddElement((list),((void *)obj),LST_END|LST_BIDIR,NULL)
#endif

#ifdef SWIG
int 	IdLstNbElems	(PLIST);
int 	IdLstNbObjects	(PLIST);
int 	IdLstType		(PLIST);
int 	IdLstGetTypeObj	(PLIST_ELEMENT);
void	IdLstSetTypeObj(PLIST_ELEMENT,int);
PLIST_ELEMENT IdLstFirst(PLIST);
PLIST_ELEMENT IdLstList	(PLIST);
PLIST_ELEMENT IdLstLast	(PLIST);
PLIST_ELEMENT IdLstNext	(PLIST_ELEMENT);
PLIST_ELEMENT IdLstPrevious(PLIST_ELEMENT);
void * IdLstPtrObj		(PLIST_ELEMENT);
int IdLstAddFirst		(PLIST, void *);
int IdLstAddLast		(PLIST, void *);
#endif










/* FUNCTION DESCRIPTION **************************************************

IdLstNbElems   (macro)

RESUME: Retourne le nombre d'elements de la liste

DESCRIPTION: Retourne le nombre d'elements de la liste

SYNTAXE:     int nbElem = IdLstNbElems(PLIST liste);

RETOUR:      type  : int
	     role  : nombre d'element de la liste

PARAMETRES:  
	     nom   : liste
	     type  : PLIST
	     role  : liste ou arbre binaire

FICHIER:     idliste.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdLstType   (macro)

RESUME: Retourne le type de la liste (inutilise actuellement)

DESCRIPTION: Retourne le type de la liste (inutilise actuellement)

SYNTAXE:     int typList = IdLstType(PLIST liste);

RETOUR:      type  : int 
	     role  : type de la liste

PARAMETRES:  
	     nom   : liste
	     type  : PLIST
	     role  : liste ou arbre binaire

FICHIER:     idliste.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdLstGetTypeObj   (macro)

RESUME: Retourne le type de l'Objet pointe par l'Element

DESCRIPTION:  Retourne le type de l'Objet pointe par l'Element

SYNTAXE:     int typObj = IdLstGetTypeObj(PLIST_ELEMENT element);

RETOUR:      type  : int 
	     role  : type de l'Objet pointe par l'Element

PARAMETRES:  
	     nom   : elem
	     type  : PLIST_ELEMENT 
	     role  : Element de liste 

FICHIER:     idliste.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdLstSetTypeObj   (macro)

RESUME: Positionne le type de l'Objet pointe par l'Element

DESCRIPTION:  Positionne le type de l'Objet pointe par l'Element
	      (Sous la responsabilite de l'utilisateur)

SYNTAXE:     void IdLstSetTypeObj(PLIST_ELEMENT element, int typeObj);

RETOUR:      type  : void
	     role  : 

PARAMETRES:  
	     nom   : elem
	     type  : PLIST_ELEMENT 
	     role  : Element de liste 

	     nom   : type
             role  : Type de l'objet 

FICHIER:     idliste.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdLstFirst   (macro)

RESUME: retourne le pointeur sur le premier element de la liste

DESCRIPTION: retourne le pointeur sur le premier element de la liste

SYNTAXE:     PLIST_ELEMENT elt = IdLstFirst(PLIST liste);

RETOUR:      type  : PLIST_ELEMENT 
	     role  : pointeur sur le premier element de la liste

PARAMETRES:  
	     nom   : liste
	     type  : PLIST
	     role  : liste

FICHIER:     idliste.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdLstLast   (macro)

RESUME: retourne le pointeur sur le dernier element de la liste

DESCRIPTION: retourne le pointeur sur le dernier element de la liste

SYNTAXE:     PLIST_ELEMENT elt = IdLstLast(PLIST liste);

RETOUR:      type  : PLIST_ELEMENT 
	     role  : pointeur sur le premier element de la liste

PARAMETRES:  
	     nom   : liste
	     type  : PLIST
	     role  : liste

FICHIER:     idliste.h

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdLstNext   (macro)

RESUME: retourne le pointeur sur l'element suivant  de la liste

DESCRIPTION: retourne le pointeur sur l'element suivant  de la liste

SYNTAXE:     PLIST_ELEMENT elt = IdLstNext(PLIST_ELEMENT elem);

RETOUR:      type  : PLIST_ELEMENT 
	     role  : pointeur sur l'element suivant de la liste

PARAMETRES:  
	     nom   : el
	     type  : PLIST_ELEMENT 
	     role  : element de la liste dont on veut le suivant.

FICHIER:     idliste.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdLstPrevious   (macro)

RESUME: retourne le pointeur sur l'element precedant  de la liste

DESCRIPTION: retourne le pointeur sur l'element precedant  de la liste

SYNTAXE:     PLIST_ELEMENT elt = IdLstNext(PLIST_ELEMENT elem);

RETOUR:      type  : PLIST_ELEMENT 
	     role  : pointeur sur l'element precedant de la liste

PARAMETRES:
	     nom   : el
	     type  : PLIST_ELEMENT 
	     role  : element de la liste dont on veut le precedant.

FICHIER:     idliste.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdLstPtrObj   (macro)

RESUME: retourne le pointeur sur l'objet reference ds l'element 

DESCRIPTION: retourne le pointeur sur l'objet reference ds l'element

SYNTAXE:     void * objet = IdLstPtrObj(PLIST liste);

RETOUR:      type  : void * 
	     role  : pointeur sur l'objet reference ds l'element

PARAMETRES:  
	     nom   : el
	     type  : PLIST_ELEMENT 
	     role  : element de la liste dont on veut objet reference 

FICHIER:     idliste.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdLstAddFirst   (macro) 

RESUME: Creation et ajout d'un element en tete d'une liste bidirectionnelle

DESCRIPTION: Creation et ajout d'un element en tete d'une liste bidirectionnelle

SYNTAXE:     int retCode =  IdLstAddFirst(PLIST liste, void *objet);

RETOUR:      type  : int
	     role  : Indication d'erreur (inactif actuellement)

PARAMETRES:  
	     nom   : liste
	     type  : PLIST 
	     role  : Liste a laquelle l'element nouveau doit etre rajoute

	     nom   : objet
	     type  : void * 
	     role  : objet a lier a l'element (champ Object de la liste)

FICHIER:     idliste.h

******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************

IdLstAddLast   (macro) 

RESUME: Creation et ajout d'un element en queue d'une liste bidirectionnelle

DESCRIPTION: Creation et ajout d'un element en queue d'une liste bidirectionnelle

SYNTAXE:     int retCode = IdLstAddLast(PLISTE liste, void *objet);

RETOUR:      type  : int
	     role  : Indication d'erreur (inactif actuellement)

PARAMETRES:  
	     nom   : liste
	     type  : PLIST 
	     role  : Liste a laquelle l'element nouveau doit etre rajoute

	     nom   : objet
	     type  : void * 
	     role  : objet a lier a l'element (champ Object de la liste)

FICHIER:     idliste.h

******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************

IdLstFree   (macro)

RESUME: liberation de la memoire occupee par une liste.

DESCRIPTION: liberation de la memoire occupee par une liste. Les objets
		lies a la liste ne sont pas liberes.
		Au retour le pointeur liste est mis a zero.

SYNTAXE:     void IdLstFree(PLISTE liste);

RETOUR:      type  : rien
	     role  : 

PARAMETRES:  
	     nom   : liste
	     type  : PLIST 
	     role  : liste a liberer

FICHIER:     idliste.h

******************************************************** END DESCRIPTION */

/* prototypes fonctions LIST */

#ifndef SWIG
#define  IdLstFree(l)        		_IdLstFree( (PLIST*)&(l) )
#define  IdLstDestroyNoControl(l,f)	_IdLstDestroyNoControl( (PLIST*)&(l),(f) )

#else
void IdLstFree(PLIST);
void IdLstDestroyNoControl(PLIST, VOID_FUNCTION_VOIDP_POINTER);
#endif

PLIST         IdLstAlloc         (void);
PLIST_ELEMENT IdLstSearchElemObj (PLIST, void *);

typedef  int  (*INT_FUNCTION_PLISTELEMENT_VOIDP_POINTER)(PLIST_ELEMENT,void*);

PLIST_ELEMENT IdLstFindElem   (PLIST, 
                               INT_FUNCTION_PLISTELEMENT_VOIDP_POINTER,
                               void *);
void          IdLstDestroyElemNoControl	(PLIST, PLIST_ELEMENT, 
                                        VOID_FUNCTION_VOIDP_POINTER );

/* a l'attention de ceux qui se tromperaient ds le nom */

#ifndef SWIG
#define  IdLstFindElemObj(a,b,c) IdLstFindElem( (a),(b),(c) )

#else
PLIST_ELEMENT IdLstFindElemObj      	(PLIST, INT_FUNCTION_POINTER, void *);
#endif

int	      IdLstRemoveFirst   (PLIST);
int	      IdLstRemoveLast    (PLIST);
int	      IdLstRemoveElem    (PLIST, PLIST_ELEMENT);

#ifndef SWIG
#define IdLstAddAfter(list,obj,el)					\
	IdLstAddElement((list),((void *)obj),LST_ELEM|LST_BIDIR,(el) )

#define IdLstAddBefore(list,obj,el)					\
	IdLstAddElement((list),((void *)obj),LST_ELEM_AV|LST_BIDIR,(el) )

#else
int IdLstAddAfter(PLIST,void *,PLIST_ELEMENT);
int IdLstAddBefore(PLIST,void *,PLIST_ELEMENT);
#endif

	
/************************************************************************/

/* Fonctions internes */




/************************************************************************/

#endif


