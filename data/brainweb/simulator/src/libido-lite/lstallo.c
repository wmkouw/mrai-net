/*************************************************************************
* $Id: lstallo.c,v 1.1 2005/09/09 08:22:51 bellet Exp $
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
/*************************************************************************
*
*  Description : Allocation and destruction of LIST related objects
*
**************************************************************************/
 
/**
 * \ingroup    lst
 * \brief      Destruction of an element of a list
 * \warning    Destruction only works for LINKED lists. Binary trees
 *             should be destructed by hand...
 * @param p    List from which the element is desctructed
 * @param d    Code targeted for destruction of binary trees (UNUSED)
 */
void
_IdFreeElement(PLIST_ELEMENT p, int d)
{
	if (p) {
		_IdFreeElement(p->Next, d);
		if (d)
			_IdFreeElement(p->Pred, d);
		free(p);
	}
}

/**
 * \ingroup    lst
 * \brief      Destruction of a list
 * @param lst  List to desctruct.
 */
void
_IdLstFree(PLIST * lst)
{
	PLIST_ELEMENT p;
	int m;

	if (*lst) {
		if (_IdLstPrivate(*lst)->_message)
			free((_IdLstPrivate(*lst)->_message));
		_IdLstPrivate(*lst)->_message = 0;
		if (_IdLstPrivate(*lst)->_fichier)
			free((_IdLstPrivate(*lst)->_fichier));
		_IdLstPrivate(*lst)->_fichier = 0;

		if (IdLstNbElems(*lst) != 0) {
			p = IdLstList(*lst);
			m = 1;
			if (p->Next)
				if (p->Next->Pred == p)
					m = 0;

			_IdFreeElement(p, m);
		}
		free(_IdLstPrivate(*lst));
		*lst = 0;
	}
}

/**
 * \ingroup lst
 * \brief   Allocation of an element.
 * @return  Allocated element
 */

PLIST_ELEMENT _IdLstAllocElement(void)
{
	void *p = calloc(sizeof(LIST_ELEMENT), 1);

	if (!p)
		IdErrPrintf
		    ("Echec d'allocation d'element de liste. Memoire saturee.");
	return (PLIST_ELEMENT) p;
}
 
/**
 * \ingroup lst
 * \brief   Allocation of a list.
 * \warning One should test the returned value before using it...
 * @return  On succes, return a pointeur the first free element of 
 *          the allocated list. On failure, return 0.
 */
PLIST
IdLstAlloc(void)
{
	PRIVATE_LIST *si;

	si = (PRIVATE_LIST *) calloc(sizeof(PRIVATE_LIST), 1);
	if (si) {
		si->Type = LST;
		si->_message = 0;
		si->_fichier = 0;
	} else {
		IdErrPrintf
		    ("Echec d'allocation 'partie privee' de liste.");
	}

	return ((PLIST) & (si[1]));
}

/**
 * \ingroup   lst
 * \brief     Get an element from a list.
 * @param pl  List among which to search.
 * @param obj Object corresponding to the element that we search.
 * @return    On succes, return a pointeur to the searched element among 
 *            list. On failure, return 0.
 */
PLIST_ELEMENT
IdLstSearchElemObj(PLIST pl, void *obj)
{
	PLIST_ELEMENT el;
	el = IdLstFirst(pl);
	while (el) {
		if (el->Object == obj) {
			return ((PLIST_ELEMENT) el);
		}
		el = el->Next;
	}
	return (0);
}

/**
 * \ingroup lst
 * \brief     Get an element from a list by using a comparison key
 * @param pl  List among which to search.
 * @param UserCompareFonc
 *            Pointer to caller provided comparison function. If pointer
 *            is void then the comparison function is defaulted to
 *            equality (FIXME IDODUDE: equality of what???) This
 *            function is not aware of the structure of the pointed
 *            object and what the key represents.It belongs to the caller
 *            to pay attention to the given comparison function pointer
 *            and to make sure it corresponds to the structure of key
 *            arguement.
 * @param key Pointer to comparison key. This function is not aware
 *            of the size and type of the key. It belongs to the caller
 *            to pay attention to the given pointer and make sure it
 *            designates a memory location where the function finds
 *            the necessary 'pieces' of information...
 * @return    On succes, return a pointeur to the searched element
 *            (that itself points to object obj) among the given
 *            list. On failure, return 0.
 */
PLIST_ELEMENT
IdLstFindElem(PLIST pl,
	    int (*UserCompareFonc) (PLIST_ELEMENT, void *),
	    void *key)
{
	PLIST_ELEMENT el;
	el = IdLstFirst(pl);
	while (el) {
		if (UserCompareFonc(el, key) == 0)
			return (el);
		el = el->Next;
	}
	return (0);
}

/**
 * \ingroup    lst
 * \brief      Destruction of a list AND the OBJECTS it refers to.
 * \warning    Make sure the given destruction function corresponds to
 *             the type of objects that are stored in this list...
 * @param lst  List to destruct.
 * @param foncDestr
 *             Pointer to destruction function of a pointed object.
 */
void
_IdLstDestroyNoControl(PLIST * lst,
		    VOID_FUNCTION_VOIDP_POINTER foncDestr)
{
	PLIST_ELEMENT e;
	if (!*lst)
		return;
	e = IdLstFirst(*lst);
	while (e != NULL) {
		foncDestr(IdLstPtrObj(e));
		e = IdLstNext(e);
	}
	_IdLstFree(lst);
}

/**
 * \ingroup    lst
 * \brief      Deletion of the elements of a list BUT not the OBJECTS it
 *             refers to.
 * @param lst  List from which to delete.
 * \warning    If no other reference exists to the pointed object then
 *             this creates a memory leak...
 * @param lst  List to destruct.
 */
void
IdLstClear(PLIST lst)
{
	PLIST_ELEMENT e, suiv;
	if (!lst)
		return;
	e = IdLstFirst(lst);
	while (e != NULL) {
		suiv = IdLstNext(e);
		IdLstRemoveElem(lst, e);
		e = suiv;
	}
}

/**
 * \ingroup    lst
 * \brief      Deletion of an element of a list AND the refering object.
 * \warning    If no other reference exists to the pointed object then
 *             this creates a memory leak. Also, make sure the given
 *             destruction function corresponds to the type of objects
 *             that are stored in this list.
 * @param lst  List from which to delete.
 * @param foncDestr
 *             Pointer to destruction function of a pointed object.
 */
void
IdLstDestroyElemNoControl(PLIST lst,
		       PLIST_ELEMENT elem,
		       VOID_FUNCTION_VOIDP_POINTER foncDestr)
{
	if (!lst)
		return;
	if (!elem)
		return;
	foncDestr(IdLstPtrObj(elem));
	IdLstRemoveElem(lst, elem);
}

/**
 * \ingroup    lst
 * \brief      Duplication of a list AND it's elements BUT NOT the
 *             refered objects.
 * @param lst  List to duplicate from.
 * @return     Pointeur to the duplicated list.
 */
PLIST
IdLstCopy(PLIST lst)
{
	PLIST_ELEMENT ed;
	PRIVATE_LIST *si;
	PLIST lstResult;
	int retCode;

	if (!lst)
		return ((PLIST) NULL);

	si = (PRIVATE_LIST *) calloc(sizeof(PRIVATE_LIST), 1);
	if (si) {
		si->Type = LST;
		si->_message = 0;
		si->_fichier = 0;
	} else {
		IdErrPrintf
		    ("Echec allocation 'partie privee' de liste.\n");
		return ((PLIST) NULL);
	}

	lstResult = (PLIST) & (si[1]);

	ed = IdLstFirst(lst);

	while (ed != NULL) {
		retCode = IdLstAddLast(lstResult, IdLstPtrObj(ed));
		if (!retCode) {
			IdErrPrintf
			    ("Echec allocation 'Element' de liste.\n");
			return ((PLIST) NULL);
		}
		ed = IdLstNext(ed);
	}

	return (lstResult);
}
 
