/*************************************************************************
* $Id: arg.c,v 1.1 2005/09/09 08:22:21 bellet Exp $
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
*
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>  // For strlen
#include "idarg.h"
#include "iderr.h"
#include "idprint.h"
#include "idio.h"

#define  ARG_LONG_MAX          1000 /* JPR, au lieu de 100 */

/* VERS     1.02 *********************************************************/
#define  ARG_LABEL_LOGFILE       "LOG"
#define  ARG_LABEL_PARAMOUT      "paramout"
static char * ArgParamOut;              /* Nom du fichier de sortie param*/
/* Fin ******************************************************************/


static char * ArgUsed = 0;              /* Arguments utilises            */
static char * ArgLab[ARGMAXCOUNT];      /* Liste des labels d'arguments  */
static char * ArgStr[ARGMAXCOUNT];      /* Liste des chaines d'arguments */
static int    ArgCount = 0;             /* Nombre d'arguments passes     */
static char * Appel = 0;                /* globale pour liberation par   */
					/* argfree    */   /* CO 14/4/92 */

/* Prototypes ***************************************** Version 1.03 CM **/

static char * Majuscule       (char*);
static int    FiltreLong      (char*);
static char * LoadedParam     (char*,FILE*);
static int    ArgLoadFromFile (char*);
static void   ArgStdArgs      (void);

/* Version 1.03 **********************************************************/
/**************************************************************************
*                                                                         *
* Nom de la fonction : Majuscule                                          *
* Role ............. : Met une chaine de caracteres en majuscules.        *
* Parametres ....... : Pointeur vers la chaine.                           *
* Valeur retournee . : pointeur vers cette meme chaine en majuscule.      *
*                                                                         *
**************************************************************************/
static char *
Majuscule (chaine )
  char * chaine;
{
  char * ptr, *ptr2, *ptr3;
  ptr2 = malloc(strlen(chaine)*sizeof(char)+1);/*bb 30Mai95*/
  ptr3=ptr2;

  for ( ptr = chaine ; *ptr!='\0' ; ptr ++ ) 
   {  *ptr3 = toupper ( * ptr ); ptr3++; 
   }
  *ptr3='\0';
 
  return ptr2;
}

/**************************************************************************
*                                                                         *
* Nom de la fonction : FiltreLong                                         *
* Role ............. : Arrete le programme si l'argument est trop long.   *
*                      ARG_LONG_MAX definit cette longueur.               *
* Parametres ....... : Pointeur vers l'argument.                          *
* Valeur retournee . : Faux s'il n'y a pas d'erreur.                      *
*                      Vrai s'il y a une erreur.                          *
**************************************************************************/
static int
FiltreLong (arg  )
  char * arg;
{
  int  n = 0 ;
  while ( (n++<ARG_LONG_MAX) && (*(arg++) != '\0') ) ;
  return (n>=ARG_LONG_MAX) ;
}



/* VERS 1.02 *********************************************************/
/*------------------------------------------------------------------------
 | Role       : Lit un parametre depuis un fichier
 | Retour     : Type   : char *
 |              Role   : pointeur vers le label
 | Parametres : param  : char *
 |              Role   : one ou il faut stocker le parametre
 |              fd     : FILE *
 |              Role   : descripteur du fichier (suppose ouvert)
 |
 +------------------------------------------------------------------------*/
static char *
LoadedParam ( param, fd )
  char * param;
  FILE *fd;
{
  int    carlu;
  char * car = param;
  int    quote = FALSE;
  int    nbcar = 0;

					  /* On supprime les espaces ****/
					  /* du debut de chaine *********/
  while ( isspace(carlu=fgetc (fd)) );
  if (carlu==EOF) return 0;
					  /* On cherche une " eventuelle*/
  if ( carlu=='\"' ) {
    carlu=fgetc(fd);
    quote=TRUE;
					  /* On lit tous les caracteres */
  }
  while (  (carlu!=EOF)
	&& (  ( (!quote)&&(!isspace(carlu)) )
	    ||( (quote)&& !(carlu=='\"')   ) ) ) {
    *(car++) = (char) carlu;
    nbcar ++;
					  /* sans depasser la taille max*/
    if ( nbcar >= ARG_LONG_MAX ) {
      IdErrPrintf("\nErreur: Argument trop long ( > %d )dans fichier de parametre.",ARG_LONG_MAX);
      break;
    }
    carlu = fgetc(fd);
  }
  *car = '\0';
  return param;
}


/* VERS 1.02 *********************************************************/
/*------------------------------------------------------------------------
 | Role       : Lecture d'arguments dans un fichier de parametres
 |              (cette fonction est recursive).
 | Retour     : Type   : int
 |              Role   : retourne le nombre de lettres necessaires
 |                       pour stocker en memoire tous les parametres
 | Parametres : filename : char *
 |              Role     : nom du fichier de parametre
 |
 +------------------------------------------------------------------------*/
static int
ArgLoadFromFile ( filename )
char    * filename ;
{
  int   nbl = 0;
  char  param[ARG_LONG_MAX+1];
  FILE  * fch;

  fch = fopen ( filename, ID_RFILE_TEXT );
  while ( LoadedParam (param, fch ) ) {
    int n = strlen(param);
    if ( param[0]=='@' ) {
      nbl  += ArgLoadFromFile ( &param[1] );
    }else{
      ArgLab [ArgCount] = strcpy ((char *) malloc(n+1), param ) ;
      nbl += n + 1 ;
      ArgCount++;
      if ( ArgCount >= ARGMAXCOUNT ) break;
    }
  }
  fclose ( fch );
  return nbl;
}

/*------------------------------------------------------------------------
 | Role       : Gestion des parametres standards de la ligne d'argument.
 | Retour     : Type   : void
 | Parametres : aucun
 +------------------------------------------------------------------------*/
static void
ArgStdArgs()
{
  char * logfile;
  FILE * fd;

  if ( (ArgParamOut=IdArgValue(ARG_LABEL_PARAMOUT))==0 )         /* V1.02 */
    ArgParamOut = ARG_DEFAULT_PARAMOUT;                          /* V1.02 */

  if ( (logfile = IdArgValue(ARG_LABEL_LOGFILE))!=0) 
  {
    if ( *logfile == '\0' ) logfile = ARG_DEFAULT_LOGFILE;
    fd = fopen ( logfile, "a+" );
    if ( fd ) 
    {
      fprintf ( fd, "%s\n", Appel );
      fclose  ( fd );
    }
  }
}

/* FUNCTION DESCRIPTION **************************************************
*
*IdArgInit   (fonction)
*
* RESUME: Initialise le gestionnaire d'arguments des filtres LIBIDO.
*
* DESCRIPTION: Initialise le gestionnaire d'arguments des filtres LIBIDO.
*              Pour plus d'informations, se reporter a la description des
*              filtres de LIBIDO (manuel de l'utilisateur, et developpement).
*
* SYNTAXE:    char * ligneAppel = IdArgInit (int argc, char **argv) ;
*
* RETOUR:     type  : char *
*             role  : Pointeur vers une chaine contenant la ligne complete
*                     d'appel du filtre. Les fichiers de parametres sont
*                     developpees dans cette lignes.
* PARAMETRES:
*            nom   : argc
*            type  : int
*            role  : Nombre d'arguments passes a la fonction main.
*
*            nom   : argv
*            type  : char **
*            role  : tableau de pointeur vers les arguments passes a
*                    la fonction main (cf: Le Langage C-Kernighan & Richie).
*
* FICHIER:     arg.c
*
* EXEMPLE:
*     #include <idarg.h>
*     int
*     main ( argc, argv )
*     int    argc;
*     char * argv[];
*     {
*	char * ligne_appel;
*	ligne_appel = IdArgInit ( argc, argv );
*	IdPrintf("\nAppel : \n\n%s\n\n", ligne_appel );
*	IdExit();
*     }
*
******************************************************** END DESCRIPTION */

/**************************************************************************
* Explication ...... : recopie des arguments de la chaine de commande     *
*                      dans les variables statiques globales de arg.c :   *
*                        ArgCount : nombre de parametres                  *
*                        ArgStr[ 0..ArgCount-1] : arguments               *
**************************************************************************/
char * IdArgInit (argc,argv)   /* On laisse comme ca  JPR */
int argc;
char * argv[];
{
  int i;
  int nblettre;

 
/* Lecture des parametres de la ligne de commande *************************/
  for ( ArgCount=0, nblettre=1 , i=0; i<argc; i++) {
    if ( FiltreLong(argv[i]) ) 
		{ 
		IdErrPrintf("Argument trop long ( > %d)...\n",ARG_LONG_MAX); 
		return NULL;
		}
    if ( argv[i][0] == '@' ){                                    /* V1.02 */
      nblettre  += ArgLoadFromFile ( &argv[i][1] );              /* V1.02 */
    }else{                                                       /* V1.02 */
      ArgLab [ArgCount] = strcpy ( (char *)malloc(strlen(argv[i])+1), argv[i] ) ;
      nblettre  += 1 + strlen(ArgLab[ArgCount]);                 /* V1.02 */
      ArgCount++;                                                /* V1.02 */
    }
    if (ArgCount >= ARGMAXCOUNT )                                /* V1.02 */
		{
		 IdErrPrintf("Trop d'Arguments ( + de %d)\n", ARGMAXCOUNT ); 
     		 return 0;
		}
  }

/* Initialisation d'un tableau memorisant les parametres deja utilises ****/
  ArgUsed = (char *) calloc ( 1, ArgCount );

/* Construction de la chaine complete des parametres d'appel **************/
  Appel = (char *)  calloc ( 1, nblettre );

  for ( *Appel = '\0', i=0; i<ArgCount; i++) {
    strcat ( Appel, ArgLab [i] ) ;
    strcat ( Appel, " " ) ;
  }

/* Separation du label de leur valeur *************************************/
  for ( i=0; i<ArgCount; i++) {
    char * egaloufin = ArgLab[i] ;
    while ( (*egaloufin != '\0') && (*egaloufin != '=') ) egaloufin ++ ;
    if ( *egaloufin ) *(egaloufin++) = '\0';
    ArgStr[i]= egaloufin;
  }

/* Mise en majuscule des labels ( casse non-sensible pour label ) *********/
  for ( i=0; i<ArgCount; i++)
    ArgLab[i] = Majuscule ( ArgLab[i] ) ;

/* Les arguments standards sont geres par ArgStdArgs **********************/
 ArgStdArgs(); 

 return Appel ;
}

/* FUNCTION DESCRIPTION **************************************************

IdArgDefined   (fonction)

RESUME: verifie si un parametre existe dans la ligne de commande

DESCRIPTION: verifie si un parametre existe dans la ligne de commande

SYNTAXE:     int retCode = IdArgDefined(char *label);

RETOUR:      type  : int
	     role  : Vrai si le parametre 'label' existe.
		     En fait, la valeur retournee est 0 si le
		     label n'est pas trouve, sinon, c'est le numero
		     d'emplacement ou il a ete recontre pour la derniere
		     fois.

PARAMETRES:
	     nom   : label
	     type  : char *
	     role  : Nom du label du parametre recherche.

FICHIER:     arg.c

EXEMPLE:     if ( !IdArgDefined("FILEIN") )
		IdErrPrintf ("\nErreur: parametre non trouve");

******************************************************** END DESCRIPTION */
int
IdArgDefined(SearchParam )
  char * SearchParam;
{
  int i, Trouve ;
  char *Temp;

  Temp =Majuscule ( SearchParam ) ;

  for ( Trouve = FALSE, i = ArgCount-1; i>0; i-- ) {            /* V1.04 */
    Trouve = ! strcmp( ArgLab[i], Temp ) ;
    if ( Trouve ) {
      int j;
      ArgUsed[i] = TRUE ;                                       /* V1.04 */
      for ( j=1; j<i; j++) {                                    /* V1.04 */
	if ( (!ArgUsed[j])&&(!strcmp(ArgLab[i],ArgLab[j])) )    /* V1.04 */
	  ArgUsed[j] = TRUE ;
      }
      return i ;
    }
  }

  return FALSE ;
}

/* FUNCTION DESCRIPTION **************************************************

IdArgValue   (fonction)

RESUME: Recupere la valeur du parametre, lue sur la ligne de commande

DESCRIPTION: Recupere la valeur du parametre demande, qui doit etre
	     lu sur la ligne de commande.

SYNTAXE:     char *valeurParam = IdArgValue(char *label);

RETOUR:      type  : char *
	     role  : Valeur, sous forme de chaine de caractere, du
		     parametre dont le label est specifie.
		     La chaine pointee ne doit pas etre modifiee.
		     Elle doit, si necessaire, etre d'abord recopiee
		     dans une autre chaine de caractere.
		     Si le label n'est pas trouve, la fonction retourne
		     le pointeur NULL.

PARAMETRES:
	     nom   : label
	     type  : char *
	     role  : Nom du label du parametre recherche.

FICHIER:     arg.c

EXEMPLE:
	     {
	       char * ficsource;
	       char * ficdestination;
	       int    fic_defaut = 0:

	       nomfic = IdArgValue("FILEIN");

	       if ( (ficdestination = IdArgValue("FILEOUT")==0 ){
		 ficdestination = IdStrCreateNewSuffix(ficsource,".out");
		 fic_defaut = 1;
	       }


...
	       if (fic_defaut) free( ficdestination );
	     }

******************************************************** END DESCRIPTION */
char *
IdArgValue ( Param )
  char * Param;
{
  int Trouve ;

if ( (Trouve = IdArgDefined ( Param )) != FALSE )
    return ArgStr[Trouve] ;
  else { return NULL ;}
}

/* FUNCTION DESCRIPTION **************************************************

IdArgUnused   (fonction)

RESUME: Recherche du premier label encore non utilise.

DESCRIPTION: Recherche du premier label non encore utilise.
	     Ce label devient alors un label utilise.
	     Si tous les labels sont deja utilises, retourne NULL.

SYNTAXE:     char *label = IdArgUnused();

RETOUR:      type  : char *
	     role  : Pointeur vers une chaine de caratere contenant
		     le premier label non utilise.
		     retourne NULL si tous les labels ont ete utilises.

FICHIER:     arg.c

EXEMPLE:     la fonction suivante permetrait d'imprimer tous les labels
	     de la chaine d'appel.

	     int imprime_labels ()
	     {
	       char * label;
	       int i=0;
	       while ( (label=IdArgUnused())!=0 )
		 IdPrintf ("Label %3d : %s   :  %s",
				++i,  label,  IdArgValue(label));
	       return i;
	     }

******************************************************** END DESCRIPTION */
char *
IdArgUnused ( )
{
  int i ;
  for ( i=ArgCount-1; i>0; i-- ){                               /* V1.04 */
    if ( ! ArgUsed[i] ) {
      IdArgDefined(ArgLab[i]);                                  /* V1.04 */
      return ArgLab[i] ;
    }
  }
  return NULL ;
}

/* FUNCTION DESCRIPTION **************************************************

IdArgPrintUnusedLabels   (fonction)

RESUME: Imprime la liste des labels non utilises.

DESCRIPTION: Imprime la liste des labels non  utilises.

SYNTAXE:     int nbLabel = IdArgPrintUnusedLabels();

RETOUR:      type  : int
	     role  : nombre de labels non utilises.

FICHIER:     arg.c

EXEMPLE:    

******************************************************** END DESCRIPTION */
int IdArgPrintUnusedLabels ()
{
char * label;
int i=0;
while ( (label=IdArgUnused())!=0 )
	{       if (i==0) IdPrintf ("\nLabels Inutilises\n================\n");
 		IdPrintf ("Label : %s = %s\n", label,  IdArgValue(label));
		i++;
	}
 return i;
}

/* FUNCTION DESCRIPTION **************************************************
* 
* IdArgUsage   (fonction)
* 
* RESUME: Imprime le mode d'emploi d'un programme, et termine l'exec
* 
* DESCRIPTION: Imprime le mode d'emploi d'un programme, et termine
* 	     l'execution, en appelant la fonction IdExit().
* 	     IdArgUsage peut s'utiliser avec START_USAGE et
* 	     FINISH_USAGE (cf.exemple).
* 
* SYNTAXE:     int retCode =IdArgUsage(char **usage)
* 
* RETOUR:      type  : int (cf. role)
* 	     role  : permettre l'utilisation de IdArgUsage() dans le
* 		     corps meme d'une expression typee:
* 		     Exemple : (a=IdArgValue()?1:IdArgusage(usage))
* 
* PARAMETRES:
* 	     nom   : usage
* 	     type  : char **
* 	     role  : tableau de pointeur vers les lignes de la
* 		     documentation du filtre. La definition de usage
* 		     est facilitee par les macro-commandes predefinies
* 		     dans idarg.h : START_USAGE() et FINISH_USAGE.
* 		     (cf.exemple)
* 
* FICHIER:     arg.c
* 
* EXEMPLE:
* 	     #include <idarg.h>
* 
* 	     START_USAGE(usage)                //usage : variable static 
* 	       "usage : <nom> ...",
* 	       "\007",                           // erreur = bip(\007) ! 
* 	       "affiche cette aide :",
* 	       "    programme exemple",
* 	     FINISH_USAGE                     // fin definition 'usage'  
* 
* 	     int main()
* 	     {
* 	       IdArgUsage(usage);                      // affiche la doc 
* 	     }
* 
******************************************************** END DESCRIPTION */
int IdArgUsage(char ** usage_text )
{
  while ( *usage_text ) 
    IdPrintf("\n%s", *(usage_text++) );
  IdPrintf ( "\n" ) ; 
  IdExit (1);
  return (0);
}

// liberation de la memoire utilisee par ArgInit 
int _IdArgFree()
{
  int i;

  for(i=0;i<ArgCount;i++)
    if(ArgLab[i])free(ArgLab[i]);
  if(ArgUsed)free(ArgUsed);
  if(Appel)free(Appel);
  return 0;
}

/* FUNCTION DESCRIPTION **************************************************

IdArgSave    (fonction)

RESUME: sauvegarde une chaine de caract. dans un fichier de parametres

DESCRIPTION: sauvegarde une chaine de caractere dans un fichier de
	     parametres. Le nom du fichier est celui specifie sur la ligne
	     d'appel par : PARAMOUT=???
	     ou, par defaut, celui donne par ARG_DEFAULT_PARAMOUT
	     (chaine de caractere definie dans idarg.h).

SYNTAXE:     int retCode = IdArgSave ( char *parametre );

RETOUR:      type  : int
	     role  : 0 : echec ; !=0 succes.
		     En cas d'echec, IdErrno vaut l'un des valeurs
		     suivantes :
		       IDERR_POINTER_IS_NULL : Pointeur NULL (parametre)
		       IDERR_OPEN_FAILED     : Echec ouverture fichier

PARAMETRES:
	     nom   : parametre
	     type  : char *
	     role  : Chaine de caractere definissant le parametre:
		       "label=valeur"

FICHIER:     arg.c

EXEMPLE:
	     IdErrno = 0;
	     IdArgSave("FILEIN=lena128.img");
	     IdArgSave("DIM=128");
	     IdArgSave("LOG=mylog.log");
	     if (IdErrno) IdErrPrintf ("\nEchec de sauvegarde de parametres");

******************************************************** END DESCRIPTION */
/*------------------------------------------------------------------------
 | Role       : Sauvegarde dans un fichier de parametres de sortie
 | Retour     : Type   : int
 |              Role   : Indicateur d'erreur: 1 = OK,  0 = AIE AIE AIE
 | Parametres : param  : char *
 |              Role   : pointeur vers la chaine a sauvegarder
 |
 +------------------------------------------------------------------------*/
int
IdArgSave ( param )
  char * param;
{
  static int   deja = 0;
  FILE         * fd;

  if ( *ArgParamOut == '\0' ) {
    return 0;
  }
  if(deja) {
    fd = fopen ( ArgParamOut, "a+" );
  }else{
    deja = 1;
    fd = fopen ( ArgParamOut, "w" );
  }

  if ( !fd ) return 0;

  fprintf ( fd, "%s\n", param );
  fclose  ( fd );
  return 1;
}
