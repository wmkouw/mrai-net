/*************************************************************************
* $Id: lstbasic.c,v 1.1 2005/09/09 08:22:52 bellet Exp $
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

#include <stdlib.h>
#include "idliste.h"
#include "idprint.h"
#include "iderr.h"

/*************************************************************************
*
*  Description : Ajout et suppression d'un element dans une liste
*
**************************************************************************/
/**
 * \ingroup    lst
 * \brief      Manipulation of bidirect lists or binary trees (creation,
 *             registering...)
 *
 *             Creation of a liste and registering of an element. A list
 *             can be unary (mono ou bidirectional) or represent binary
 *             tree (correponding modes are LST_NEXT et LST_PRED);
 * @param list List to add an new element to.
 * @param obj  Object to add to the list (in the 'Object' field of
 *             the list)
 * @param mode Modality of registering to the list :
 *             LST_BEG | LST_NEXT : liste ou arbre binaire
 *             element rajoute en tete de liste par le
 *             lien Next. 
 *             Ajout a la racine, 'ancien arbre' 
 *             chaine a droite
 *             LST_BEG | LST_PRED : liste ou arbre binaire
 *             element rajoute en tete de liste par le 
 *             lien Pred.
 *             Ajout a la racine, 'ancien arbre' 
 *             chaine a gauche
 *             LST_END | LST_NEXT : liste ou arbre binaire
 *             element rajoute en queue de liste par le lien Next.
 *             Ajout a droite sur une feuille 
 *             LST_END | LST_PRED : liste ou arbre binaire
 *             element rajoute en tete de liste par
 *             le lien Pred.
 *             Ajout a gauche sur une feuille 
 *             LST_BEG | LST_BIDIR : liste bidirectionnelle
 *             element rajoute en tete de liste
 *             LST_END | LST_BIDIR : liste bidirectionnelle
 *             element rajoute en queue de liste
 *             LST_ELEM | LST_PRED : liste ou arbre binaire
 *             element rajoute par le champ Pred avant
 *             l'element el (param.) de la liste
 *             Insertion d'un noeud a gauche
 *             LST_ELEM | LST_NEXT : liste ou arbre binaire
 *             element rajoute par le champ Next apres
 *             l'element el (param.) de la liste
 *             Insertion d'un noeud a droite
 *             LST_ELEM | LST_BIDIR : liste bidirectionnelle
 *             element rajoute apres
 *             l'element el (param.) de la liste
 * @param elem Element of the list after which we register.
 * @return     1 on success, 0 on failure.
 */
int
IdLstAddElement(PLIST list, void *obj, int mode, PLIST_ELEMENT elem)
{
	PLIST_ELEMENT e, p;

	if (!list) {
		IdErrno = IDERR_POINTER_IS_NULL;
		return (0);
	}

	e = calloc(1, sizeof(LIST_ELEMENT));
	if (!e) {
		IdErrno = IDERR_ALLOC_ELEM;
		return (0);
	}

	(_IdLstPrivate(list)->NbElems)++;
	e->Numero = IdLstNbElems(list);
	switch (mode & 0x0f) {
	case LST_BEG:
		if (mode & (LST_NEXT | LST_BIDIR)) {
			p = IdLstFirst(list);
			if (p == 0)
				IdLstLast(list) = e;
			e->Next = p;
			IdLstFirst(list) = e;
			e->Object = obj;
			if (mode & LST_BIDIR)
				if (e->Next)
					e->Next->Pred = e;
		}
		if (mode & LST_PRED) {
			e->Pred = IdLstFirst(list);
			IdLstFirst(list) = e;
			e->Object = obj;
		}
		break;
	case LST_END:
		if (mode & (LST_NEXT | LST_BIDIR)) {

			p = IdLstFirst(list);
			if (p) {
				while (p->Next)
					p = p->Next;
				p->Next = e;
				if (mode & LST_BIDIR)
					e->Pred = p;
			} else
				IdLstFirst(list) = e;
			e->Object = obj;
			IdLstLast(list) = e;
		}
		if (mode & LST_PRED) {
			p = IdLstFirst(list);
			if (p) {
				while (p->Pred)
					p = p->Pred;
				p->Pred = e;
			} else
				IdLstFirst(list) = e;
			e->Object = obj;
		}
		break;
	case LST_ELEM:
		if (mode & (LST_NEXT | LST_BIDIR)) {
			if (elem) {
				if (elem == IdLstLast(list))
					IdLstLast(list) = e;
				e->Next = elem->Next;
				elem->Next = e;
				if (mode & LST_BIDIR) {
					e->Pred = elem;
					if (e->Next)
						e->Next->Pred = e;
				}
			}
			e->Object = obj;
		}
		if (mode & LST_PRED) {
			if (elem) {
				e->Pred = elem->Pred;
				elem->Pred = e;
			}
			e->Object = obj;
		}
		break;

	case LST_ELEM_AV:
		if (mode & (LST_NEXT | LST_BIDIR)) {
			if (elem) {
				if (elem == IdLstFirst(list))
					IdLstFirst(list) = e;
				e->Pred = elem->Pred;
				elem->Pred = e;
				if (mode & LST_BIDIR) {
					e->Next = elem;
					if (e->Pred)
						e->Pred->Next = e;
				}
			}
			e->Object = obj;
		}
		if (mode & LST_PRED) {
			if (elem) {
				e->Next = elem->Next;
				elem->Next = e;
			}
			e->Object = obj;
		}
		break;
	}
	return 1;
}

/**
 * \ingroup    lst
 * \brief      Suppression of the first element of a bidirectional list
 * @param l    List from which we suppress 
 * @return     1 on success, 0 on failure.
 */
int
IdLstRemoveFirst(PLIST l)
{
	PLIST_ELEMENT e;
	if (IdLstNbElems(l) == 0) {
		/*
		 * empty list
		 */
		IdErrno = IDERR_EMPTY_LIST;
		return (0);
	}
	(_IdLstPrivate(l)->NbElems)--;

	e = IdLstFirst(l);
	IdLstFirst(l) = IdLstNext(e);

	if (IdLstNext(e) == NULL)
		/*
		 * si c'est le dernier 
		 */
		IdLstLast(l) = NULL;
	else
		IdLstPrevious(IdLstNext(e)) = NULL;

	/*
	 * les liens de chainage ont deja ete supprimes 
	 */
	free(e);
	return (1);
}

/**
 * \ingroup    lst
 * \brief      Suppression of the last element of a bidirectional list
 * @param l    List from which we suppress 
 * @return     1 on success, 0 on failure.
 */
int
IdLstRemoveLast(PLIST l)
{
	PLIST_ELEMENT e;
	if (IdLstNbElems(l) == 0) {
		/*
		 * liste vide 
		 */
		IdErrno = IDERR_EMPTY_LIST;
		return (0);
	}
	(_IdLstPrivate(l)->NbElems)--;

	e = IdLstLast(l);
	IdLstLast(l) = IdLstPrevious(e);

	if (IdLstPrevious(e) == NULL)
		/*
		 * si c'est le Premier 
		 */
		IdLstFirst(l) = NULL;
	else
		IdLstNext(IdLstPrevious(e)) = NULL;

	/*
	 * les liens de chainage ont deja ete supprimes 
	 */
	free(e);
	return (1);
}

/**
 * \ingroup    lst
 * \brief      Suppression of an element of a bidirectional list.
 * @param l    List from which we suppress the element.
 * @param e    Element to suppress.
 * @return     1 on success, 0 on failure.
 */
int
IdLstRemoveElem(PLIST l, PLIST_ELEMENT e)
{
	PLIST_ELEMENT pr, sv;

	if (IdLstNbElems(l) == 0) {
		/*
		 * liste vide 
		 */
		IdErrno = IDERR_EMPTY_LIST;
		return (0);
	}
	pr = IdLstPrevious(e);
	sv = IdLstNext(e);

	if (pr == NULL)
		IdLstRemoveFirst(l);
		/*
		 * si c est le Premier 
		 */
	else if (sv == NULL)
		IdLstRemoveLast(l);
		/*
		 * si c est le Dernier 
		 */

	else {
		/*
		 * Element quelconque  
		 */
		IdLstNext(pr) = IdLstNext(e);
		IdLstPrevious(sv) = IdLstPrevious(e);
		(_IdLstPrivate(l)->NbElems)--;

		/*
		 * les liens de chainage ont deja ete supprimes 
		 */
		free(e);
	}
	return (1);
}

/**
 * \ingroup    lst
 * \brief      Creation of an element and insertion-AFTER a given
 *             element-inside a bidirectional *             list.
 * @param l    List inside which we insert the new element.
 * @param pobj Pointer to object that shall be linked we new element.
 * @param e    Element AFTER which insertion should occur.
 * @return     1 on success, 0 on failure.
 */
int
LstAddAfter(PLIST l, void *pobj, PLIST_ELEMENT e)
{

	PLIST_ELEMENT elem;

	if (e == IdLstLast(l))
		IdLstAddLast(l, pobj);
	else {
		/*
		 * Si l'element n'est pas le dernier 
		 */
		elem = calloc(1, sizeof(LIST_ELEMENT));
		if (!elem) {
			IdErrPrintf("(Erreur allocation nouvel element\n");
			IdErrno = IDERR_ALLOC_ELEM;
			return 0;
		}
		(_IdLstPrivate(l)->NbElems)++;
		elem->Numero = IdLstNbElems(l);
		elem->Object = pobj;
		IdLstNext(elem) = IdLstNext(e);
		IdLstPrevious(elem) = e;
		IdLstPrevious(IdLstNext(e)) = elem;
		IdLstNext(e) = elem;
	}
	return 1;
}

/**
 * \ingroup    lst
 * \brief      Creation of an element and insertion-BEFORE a given
 *             element-inside a bidirectional *             list.
 * @param l    List inside which we insert the new element.
 * @param pobj Pointer to object that shall be linked we new element.
 * @param e    Element BEFORE which insertion should occur.
 * @return     1 on success, 0 on failure.
 */
int
LstAddBefore(PLIST l, void *pobj, PLIST_ELEMENT e)
{

	PLIST_ELEMENT elem;

	if (e == IdLstFirst(l))
		IdLstAddFirst(l, pobj);
	else {
		/*
		 * Si l'element n'est pas le premier 
		 */
		elem = calloc(1, sizeof(LIST_ELEMENT));
		if (!elem) {
			IdErrPrintf("Erreur allocation nouvel element\n");
			IdErrno = IDERR_ALLOC_ELEM;
			return 0;
		}
		(_IdLstPrivate(l)->NbElems)++;

		elem->Numero = IdLstNbElems(l);
		elem->Object = pobj;
		IdLstNext(elem) = e;
		IdLstPrevious(elem) = IdLstPrevious(e);
		IdLstNext(IdLstPrevious(e)) = elem;
		IdLstPrevious(e) = elem;
	}
	return 1;
}
