/*************************************************************************
* $Id: kerprint.c,v 1.1 2005/09/09 08:22:51 bellet Exp $
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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>  // For strlen
#include "idprint.h"

/* FUNCTION DESCRIPTION **************************************************
* 
* IdPrintf   (fonction)
* 
* RESUME: affiche un message
* 
* DESCRIPTION: Fonction printf de LIBIDO, permettant, en fonction de
*          l'environnement de l'utilisateur d'orienter le flux de sortie
*          sur le fichier adapte.
* 
* 
* SYNTAXE:     int nbArgImprimes = IdPrintf ( char *formatImpression, ... );
* 
* RETOUR:      type  : int
* 	     role  : Nombre d'arguments reellement imprimes
* 
* PARAMETRES:
* 	     nom   : formatImpression
* 	     type  : char *
* 	     role  : chaine de format (comme pour printf)
* 
* EXEMPLE:     IdPrintf("\nexemple: %d, %c, %s->fin", 1,'a',"fin exemple");
* 
******************************************************** END DESCRIPTION */


#ifdef   _IWWIN95_
#define  _CONSOLE_ 0
#define  _UNIX_    0
#else
#define  _CONSOLE_ 1
#define  _UNIX_    0
#endif


#if defined(_UNIX_) || defined(_CONSOLE_)

#define IdStdOut stdout
#define IdStdErr stderr

int
IdPrintf (char *format, ...)
{
   int             nb;
   va_list         ap;

   va_start(ap,format);
   nb = vfprintf ( IdStdOut, format, ap );

   va_end(ap);
   return nb;
}

/* FUNCTION DESCRIPTION **************************************************

IdErrPrintf   (fonction)

RESUME: affiche un message d'erreur

DESCRIPTION: Fonction printf de LIBIDO, permettant, en fonction de
         l'environnement de l'utilisateur, et d'orienter le flux d'erreur
         sur le fichier adapte.


SYNTAXE:     int nbArgImprimes = IdErrPrintf (char * format, ... );

RETOUR:      type  : int
	     role  : Nombre d'arguments reellement imprimes

PARAMETRES:
	     nom   : format
	     type  : char *
	     role  : chaine de format (comme pour printf)

EXEMPLE:     IdErrPrintf ( "\nerr en ligne", __LINE__ );

******************************************************** END DESCRIPTION */


int
IdErrPrintf (char *format, ...)
{
   int             nb;
   va_list         ap;

   va_start(ap,format);
   printf ("\n ERREUR LIBIDO :");
   nb = vfprintf (IdStdErr,format,ap);
   va_end(ap);
   return nb;
}

#else

/*			sur PC, ca se gate :		*/
/*			en IWWIN95 on fabrique une fonction	*/

#ifdef  _IWWIN95_

#include <windows.h>

static long int __tt__;
static char 		msgbuf[10000];

int IdPrintf (char * format, ... )
{
	va_list         ap;

	va_start(ap,format);	 /* C'est APRES cet element que va_arg     */
				 /* va commencer a depiler les parametres  */
	(void) vsprintf(msgbuf,format,ap);
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), msgbuf, strlen(msgbuf),
					&__tt__, NULL);
        va_end(ap);
	return 1;
}


int IdErrPrintf (char * format, ... )
{
	va_list         ap;

	va_start(ap,format);	 /* C'est APRES cet element que va_arg     */
				 /* va commencer a depiler les parametres  */
	(void) vsprintf(msgbuf,format,ap);
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), msgbuf, strlen(msgbuf),
					&__tt__, NULL);
        va_end(ap);
	return 1;
}

#endif
#endif
