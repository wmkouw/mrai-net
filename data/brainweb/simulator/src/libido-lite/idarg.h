/*************************************************************************
* $Id: idarg.h,v 1.1 2005/09/09 08:22:25 bellet Exp $
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
*  Description : Gestion simplifiee des arguments passes sur la ligne
*                de commande.
* idarg.h DOIT OBLIGATOIREMENT figurer UNE FOIS ET UNE SEULE dans TOUT
* filtre utilisant la Bibliotheque LibIDO (FIXME IDODUDE : c'est toujours
* d'actualite ce commentaire ?)
*
**************************************************************************/

#ifndef  ___ARG_LIB__
#define  ___ARG_LIB__
#include <stdlib.h> // For atof

#define ARGMAXCOUNT 100                 /* Nombre maximum d'arguments */


					/* autorises ******************/
					/* Nom de fichier par defaut **/
#define ARG_DEFAULT_PARAMOUT    "fileout.par"
#define ARG_DEFAULT_LOGFILE     "libido.log"

#ifndef FALSE
#define FALSE (1==0)
#define TRUE  (1==1)
#endif

#ifndef  SWIG 
#define START_USAGE(usage)   static char * usage[] = {
#define FINISH_USAGE         0};
#endif

char * IdArgInit              (int, char **); /* Routine d'initialisation     */
int    IdArgDefined           (char *);       /* Teste si Param est defini    */
char * IdArgValue             (char *);       /* Retourne la valeur de Param  */
char * IdArgUnused            (void);         /* Retourne un arg. non utilise */
int    IdArgSave              (char *);       /* Sauvegarde de parametres out */
int    IdArgUsage             (char **);      /* Affichage usage              */
int    IdArgPrintUnusedLabels (void);         /* Imprime la liste des labels
                                                 non utilises */
float *IdArgGetListOfFloat    (char *,int);   /* Lecture d'une liste de float */
int   *IdArgGetListOfInt      (char *,int);   /* Lecture d'une liste de int   */
char **IdArgGetListOfString   (char *,int);   /* Lecture d'une liste de chaine*/
float *IdArgGetListOfFloatWyW (char *,int *);
int   *IdArgGetListOfInt_N    ( char *, int, int * OUTPUT);
                                              /* Lecture d'une liste de int
                                                 avec le nb                   */
float *IdArgGetListOfFloat_N  (char *,int, int * OUTPUT);  
     /* Lecture d'une liste de float avec le nb     */
char **IdArgGetListOfStringWyW(char *, int * OUTPUT);
     /* Lecture d'une liste de chaine de lgr qcq */
int   *IdArgGetListOfIntWyW   (char *, int * OUTPUT); 
     /* Lecture d'une liste de int de lgr qcq    */

int   * IdArgGetIntEnum     (char *, int * OUTPUT);
     /* Lecture d'une liste de d'intervalles int   */
float * IdArgGetFloatEnum   (char *, int * OUTPUT);
     /* Lecture d'une liste de d'intervalles float */

int    IdArgGetType        (char *,int);   /* Saisie d'argument de typeData  */
int    IdArgWantType       (char *,char**);/* Saisie d'argument de typeData  */
int    IdArgGetTypeObj     (char *,int);   /* Saisie d'argument de typeObjet */
int    IdArgWantTypeObj    (char *,char**);/* Saisie d'argument de typeObjet */

int    IdArgGetLabel       (char *, char *, int);
int    IdArgWantLabel      (char *, char *, char **);

                                            /* C.M.  7-JUI-1992 10:24:02 */
/* FUNCTION DESCRIPTION **************************************************
* 
* IdArgGetInt (macro)
* 
* RESUME: Saisie de valeurs passees en argument a un filtre, avec val. par defaut.
* 
* DESCRIPTION: Saisie de valeurs passees en argument a un filtre.
*              Si le parametre n'est pas present, retourne la valeur
*              par defaut.
* 	     Voir aussi : IdArgWant...
* 
* SYNTAXE:     int i = IdArgGetInt (char * label, int default);
* 
* RETOUR:      type  : int
*              role  :
* 
* PARAMETRES:
*              nom   : label
*              type  : char *
*              role  : Label du parametre recherche.
* 
*              nom   : defaut
*              type  : variable (cf. Role)
*              role  : Valeur par defaut retournee si le label n'est
*                      pas trouve.
* 
* 
* EXEMPLE:     int dimx = IdArgGetInt ( "DIMX", 256 );
* 
******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************
* 
* IdArgGetFloat (macro)
* 
* RESUME: Saisie de valeurs passees en argument a un filtre, avec val. par defaut.
* 
* DESCRIPTION: Saisie de valeurs passees en argument a un filtre.
*              Si le parametre n'est pas present, retourne la valeur
*              par defaut.
* 		Voir Aussi IdArgWant ...
* 
* SYNTAXE:     float f = IdArgGetFloat( char * label, float default);
* 
* RETOUR:      type  : float 
*              role  :
* 
* PARAMETRES:  nom   : label
*              type  : char *
*              role  : Label du parametre recherche.
* 
*              nom   : defaut
*              type  : variable (cf. Role)
*              role  : Valeur par defaut retournee si le label n'est
*                      pas trouve.
* 
* 
* EXEMPLE:     int dimx = IdArgGetInt ( "DIMX", 256 );
* 
******************************************************** END DESCRIPTION */

/* FUNCTION DESCRIPTION **************************************************
* 
* IdArgGetString   (macro)
* 
* RESUME: Saisie de valeurs passees en argument a un filtre, avec val. par defaut.
* 
* DESCRIPTION: Saisie de valeurs passees en argument a un filtre.
*              Si le parametre n'est pas present, retourne la valeur
*              par defaut.
* 		Voir Aussi IdArgWant ...
* 
* SYNTAXE:     char *s = IdArgGetString (char * label, char * default);
* 
* RETOUR:      type  : char *
*              role  :
* 
* PARAMETRES:
*              nom   : label
*              type  : char *
*              role  : Label du parametre recherche.
* 
*              nom   : defaut
*              type  : variable (cf. Role)
*              role  : Valeur par defaut retournee si le label n'est
*                      pas trouve.
* 
* EXEMPLE:     int dimx = IdArgGetInt ( "DIMX", 256 );
* 
******************************************************** END DESCRIPTION */


#define IdArgGetInt(label,default)       ( (IdArgDefined(label))         \
                                         ? (atoi(IdArgValue(label)))     \
                                         : (default) )
#define IdArgGetFloat(label,default)     ( (IdArgDefined(label))         \
                                         ? (atof(IdArgValue(label)))     \
                                         : (default) )
#define IdArgGetString(label,default)    ((IdArgDefined(label))          \
                                         ? (IdArgValue(label))           \
                                         : (default) )

#ifdef  SWIG

int 	IdArgGetInt(char*, int);
float  	IdArgGetFloat(char*, float);
char*	IdArgGetString(char*, char*);

#endif

/* FUNCTION DESCRIPTION **************************************************
* 
* IdArgWantInt (macro)
* 
* RESUME: Saisie de valeurs passees en argument a un filtre.
* 
* DESCRIPTION: Saisie de valeurs passees en argument a un filtre.
*              Si le label du parametre n'est pas present, appelle la fonction
*              IdArgUsage.
* 
* SYNTAXE:     int i = IdArgWantInt(char * label, char *usage[]);
* 
* RETOUR:      type  : int 
*              role  :
* 
* PARAMETRES:
*              nom   : label
*              type  : char *
*              role  : Label du parametre recherche.
* 
*              nom   : usage
*              type  : char *[] (cf. IdArgUsage)
*              role  : Texte d'usage filtre si le label n'est pas trouve.
* 
* VOIR AUSSI:  IdArgGet...
* 
* EXEMPLE:
*              START_USAGE(usage)
*                ...
*                "DIMX   : Dimension en X",
*                ...
*              FINISH_USAGE
* 
*              ...
*              int dimx = IdArgWantInt ( "DIMX", usage );
*              ...
* 
******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************
* 
* IdArgWantString   (macro)
* 
* RESUME: Saisie de valeurs passees en argument a un filtre.
* 
* DESCRIPTION: Saisie de valeurs passees en argument a un filtre.
*              Si le label du parametre n'est pas present, appelle la fonction
*              IdArgUsage.
* 
* SYNTAXE:     char *s = IdArgWantString (char * label, char *usage[]);
* 
* RETOUR:      type  : char *
*              role  :
* 
* PARAMETRES:
*              nom   : label
*              type  : char *
*              role  : Label du parametre recherche.
* 
*              nom   : usage
*              type  : char *[] (cf. IdArgUsage)
*              role  : Texte d'usage filtre si le label n'est pas trouve.
* 
* EXEMPLE:
*              START_USAGE(usage)
*                ...
*                "DIMX   : Dimension en X",
*                ...
*              FINISH_USAGE
* 
*              ...
*              int dimx = IdArgWantInt ( "DIMX", usage );
*              ...
* 
******************************************************** END DESCRIPTION */


/* FUNCTION DESCRIPTION **************************************************
* 
* IdArgWantFloat (macro)
* 
* RESUME: Saisie de valeurs passees en argument a un filtre.
* 
* DESCRIPTION: Saisie de valeurs passees en argument a un filtre.
*              Si le label du parametre n'est pas present, appelle la fonction
*              IdArgUsage.
* 
* SYNTAXE:     float f = IdArgWantFloat(char *label, char *usage[]);
* 
* RETOUR:      type  : float 
*              role  :
* 
* PARAMETRES:
*              nom   : label
*              type  : char *
*              role  : Label du parametre recherche.
* 
*              nom   : usage
*              type  : char *[] (cf. IdArgUsage)
*              role  : Texte d'usage filtre si le label n'est pas trouve.
* 
* VOIR AUSSI:  IdArgGet...
* 
* EXEMPLE:
*              START_USAGE(usage)
*                ...
*                "DIMX   : Dimension en X",
*                ...
*              FINISH_USAGE
* 
*              ...
*              int dimx = IdArgWantInt ( "DIMX", usage );
*              ...
* 
******************************************************** END DESCRIPTION */


#define IdArgWantInt(label,usage)       ( (IdArgDefined(label) )	\
                                         ? (atoi(IdArgValue(label) ) )	\
                                         : (IdArgUsage(usage),1) )

#define IdArgWantFloat(label,usage)      ( (IdArgDefined(label) )     	\
                                         ? (atof(IdArgValue(label) ) )	\
                                         : (IdArgUsage(usage),1.0) )

#define IdArgWantString(label,usage)     ( (IdArgDefined(label) )	\
                                         ? (IdArgValue(label) )		\
                                         : (IdArgUsage(usage),(char*)0) )

#ifdef  SWIG
int	IdArgWantInt(char*,char**);
float	IdArgWantFloat(char*,char**);
char*	IdArgWantString(char*,char**);
#endif

/* Fonctions internes ******************************************************/

int _IdArgFree (void);              /* Menage en fin d'execution   */

/***************************************************************************/
#endif
