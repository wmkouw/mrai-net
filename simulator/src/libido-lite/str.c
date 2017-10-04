/*************************************************************************
* $Id: str.c,v 1.1 2005/09/09 08:22:55 bellet Exp $
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
*  Description  : Divers utilitaires Chaine de caract.
*
**************************************************************************/

#include <string.h>  // For strtok, strstr, strlen
#include <stdlib.h>  // For strtol and strtod
#include <stdio.h>   // For sprintf
#include <ctype.h>   // For toupper

#include "idstr.h"
#include "iderr.h"
//#include "idprint.h"
#include <stdio.h>
#include "iddicom.h"

/* FUNCTION DESCRIPTION **************************************************
* 
* IdStrCreateNewSuffix (fonction)
* 
* RESUME: change le suffixe d'un nom de fichier
* 
* DESCRIPTION: Cree une chaine de caractere decrivant le nom d'un fichier
* 	     a partir d'un nom existant, en changeant le suffixe.
* 
* SYNTAXE:      char *newfilename = IdStrCreateNewSuffix (char * oldname, char * suffix);
* 
* RETOUR:      type  : char *
* 	     role  : pointeur vers la zone allouee contenant le nouveau
* 		     nom de fichier.
* 
* PARAMETRES:  nom   : oldname
* 	     type  : char *
* 	     role  : Nom du fichier 'modele'.
* 
* 	     nom   : suffix
* 	     type  : char *
* 	     role  : Nom du suffixe qui sera mis a la place du suffixe
* 		     precedent.
* 
* FICHIER:     idstr.c
* 
* EXEMPLES:    filename = IdStrCreateNewSuffix ("nom.txt",".lis");
* 
* 			=> filename = "nom.lis"
* 
* 	     filename = IdStrCreateNewSuffix ("nom.txt","00.lis");
* 
* 			=> filename = "nom00.lis"
* 
*    (sur vax) filename = IdStrCreateNewSuffix ("d$extern:[txt.dir]nom",".txt");
* 
* 			=> filename = "d$extern:[txt.dir]nom.txt"
* 
*    (sur unix)filename = IdStrCreateNewSuffix ("/usr/test.dir/nom.o.t",".txt");
* 
* 			=> filename = "/usr/test.dir/nom.o.txt"
* 
*    (sur dos) filename = IdStrCreateNewSuffix ("c:\ess.dir\nom",".text");
* 
* 			=> filename = "c:\ess.dir\nom.text"
* 
******************************************************** END DESCRIPTION */

/**
 * \ingroup   str
 * \brief     Enleve le suffixe d'un nom de fichier et le remplace par un autre
 * @param nom     Pointeur vers le nom
 * @param suffix  Pointeur vers le suffixe
 * @return    nouveau nom
 */

char * IdStrCreateNewSuffix (char * nom, char * suffix )
{
  char        * fin;
  char        * nouveau;
  char        * ajout;
  int           car_copie;
  int           taille;

					  /* fin pointe sur fin du nom **/
  for ( fin = nom; *fin != '\0'; fin ++ );
                                          /* cherche 1 '.' avant la fin */
  for ( ajout = fin; (*ajout!='.'); ajout -- ) {
                                          /* Pas de '.' trouve **********/
    if ( ajout <= nom ) {
      ajout = fin ;
      break;
    }
                                          /* carac. interdit trouve *****/
    if (

#if defined(_MSC_VER)
        (*ajout==':')||(*ajout=='\\')
#else
        (*ajout=='/')
#endif

    ) {
      ajout = fin ;
      break;
    }
  }
					  /* calcul taille finale *******/
  taille = (int) (ajout-nom) + 2 + strlen(suffix) ;
					  /* et reservation memoire *****/
  nouveau = (char *)malloc (taille) ;
					  /* copie du prefixe ***********/
  for ( car_copie = 0; nom < ajout; car_copie ++ )
    nouveau[car_copie] = *(nom++);
					  /* et ajout du suffixe ********/
  for ( ajout = suffix; car_copie < taille; car_copie++ )
    nouveau[car_copie] = *(ajout++);

  return nouveau;
}




/* FUNCTION DESCRIPTION **************************************************

IdStrRemoveSuffix (fonction)

RESUME: Supprime le suffixe d'un nom de fichier

DESCRIPTION: Cree une chaine de caractere decrivant le nom d'un fichier
	     a partir d'un nom existant, en suprimant le suffixe.

SYNTAXE:      char *newfilename = IdStrRemoveSuffix (char * oldname);

RETOUR:      type  : char *
	     role  : pointeur vers la zone allouee contenant le nouveau
		     nom de fichier.

PARAMETRES:  nom   : oldname
	     type  : char *
	     role  : Nom du fichier 'modele'.

FICHIER:     idstr.c

EXEMPLES:    filename = IdStrRemoveSuffix ("nom.txt");

			=> filename = "nom"

	     filename = IdStrRemoveSuffix ("/export/home/nom/test/nom.txt");

			=> filename = "/export/home/nom/test/nom"

   (sur vax) filename = IdStrRemoveSuffix ("d$extern:[txt.dir]nom");

			=> filename = "d$extern:[txt.dir]nom"

   (sur unix)filename = IdStrRemoveSuffix ("/usr/test.dir/nom.o");

			=> filename = "/usr/test.dir/nom"

   (sur dos) filename = IdStrRemoveSuffix ("c:\ess.dir\nom.str");

			=> filename = "c:\ess.dir\nom"

******************************************************** END DESCRIPTION */

/**
 * \ingroup   str
 * \brief     Enleve le suffixe d'un nom de fichier 
 * @param nom     Pointeur vers le nom
 * @return        nouveau nom
 */

char *
IdStrRemoveSuffix ( char *nom )
{
  char        * fin;
  char        * nouveau;
  char        * ajout;
  int           car_copie;
  int           taille;

					  /* fin pointe sur fin du nom **/
  for ( fin = nom; *fin != '\0'; fin ++ );
                                          /* cherche 1 '.' avant la fin */
  for ( ajout = fin; (*ajout!='.'); ajout -- ) {
                                          /* Pas de '.' trouve **********/
    if ( ajout <= nom ) {
      ajout = fin ;
      break;
    }
                                          /* carac. interdit trouve *****/
    if (

#if defined(_MSC_VER)
        (*ajout==':')||(*ajout=='\\')
#else
        (*ajout=='/')
#endif

    ) {
      ajout = fin ;
      break;
    }
  }
					  /* calcul taille finale *******/
  taille = (int) (ajout-nom) + 1 ;
					  /* et reservation memoire *****/
  nouveau = (char *)malloc (taille) ;
					  /* copie du prefixe ***********/
  for ( car_copie = 0; nom < ajout; car_copie ++ )
    nouveau[car_copie] = *(nom++);
					  /* et ajout fin de chaine ********/
 
    nouveau[car_copie++] = 0;

  return nouveau;
}





/* FUNCTION DESCRIPTION **************************************************

IdStrCreateUpperCase (fonction)

RESUME: Renvoie une chaine en Majuscules  

DESCRIPTION: Renvoie une chaine en Majuscules

SYNTAXE:      char *nouvChaine = IdStrCreateUpperCase (char *ancChaine);

RETOUR:      
	     type  : char *
	     role  : pointeur vers la zone allouee contenant la nouvelle
		     chaine

PARAMETRES:  nom   : ancChaine
	     type  : char *
	     role  : pointeur sur l'ancienne chaine.

FICHIER:     idstr.c

EXEMPLES:    
******************************************************** END DESCRIPTION */

/**************************************************************************
*                                                                         *
* Nom de la fonction : Majuscule                                          *
* Role ............. : Met une chaine de caracteres en majuscules.        *
* Parametres ....... : Pointeur vers la chaine.                           *
* Valeur retournee . : pointeur vers cette meme chaine en majuscule.      *
*                                                                         *
**************************************************************************/

/**
 * \ingroup   str
 * \brief     Met une chaine de caracteres en majuscules 
 * @param nom     Pointeur vers la chaine
 * @return        pointeur vers cette meme chaine en majuscule
 */

 char *
IdStrCreateUpperCase (char *chaine )
{
  char * ptr, *ptr2, *ptr3;
  ptr2 =(char *)malloc(strlen(chaine)*sizeof(char)+1);
  ptr3=ptr2;

  for ( ptr = chaine ; *ptr!='\0' ; ptr ++ ) 
   {  *ptr3 = toupper ( * ptr ); ptr3++; 
   }
  *ptr3='\0';
 
  return ptr2;
}


/* FUNCTION DESCRIPTION **************************************************

IdStrSetUpperCase (fonction)

RESUME: Transforme sur place une chaine en Majuscules

DESCRIPTION: Transforme sur place une chaine en Majuscules

SYNTAXE:      char *chaine = IdStrSetUpperCase (char *chaine);

RETOUR:	     type  : char *
	     role  : pointeur vers la zone contenant la chaine

PARAMETRES:  nom   : chaine
	     type  : char *
	     role  : pointeur sur la chaine.

FICHIER:     idstr.c

EXEMPLES:
    
******************************************************** END DESCRIPTION */


/**
 * \ingroup   str
 * \brief     Transforme sur place une chaine en Majuscules 
 * @param chaine     Pointeur vers la chaine
 * @return        pointeur vers cette meme chaine en majuscule
 */

char * IdStrSetUpperCase (char *chaine )
{
  char *ptr;
  for ( ptr = chaine ; *ptr!='\0' ; *ptr = toupper (*ptr), ptr++ ) ; 
  return chaine;
}

/* FUNCTION DESCRIPTION **************************************************

IdStrCountChar (fonction)

RESUME: Compte le nb d'occurrences d'1 caract ds une chaine

DESCRIPTION:Compte le nb d'occurrences d'1 caract ds une chaine

SYNTAXE:      int nbOccCaract = IdStrCountChar (char *chaine, char caract);

RETOUR:      type  : int 
	     role  : nb d'occurrences du caract ds la chaine

PARAMETRES:  nom   : chaine
	     type  : char *
	     role  : pointeur sur la chaine.
	
	     nom   : caract
	     type  : char 
	     role  : caract compte

FICHIER:     idstr.c

EXEMPLES: 
   
******************************************************** END DESCRIPTION */
/**
 * \ingroup   str
 * \brief     Compte le nb d'occurrences d'1 caract ds une chaine 
 * @param chaine     Pointeur vers la chaine
 * @param caract     caract a compter
 * @return        nombre d'occurences
 */

int
IdStrCountChar (char *chaine, int caract)
{
  int i=0;
  char * ptr;

  for ( ptr = chaine ; *ptr!='\0' ; ptr ++ ) 
   {  if (*ptr==caract) i++; 
   }
 
  return i;
}

/* FUNCTION DESCRIPTION **************************************************

IdStrDirName (fonction)

RESUME: Extrait le DirectoryName d'un FullPathName

DESCRIPTION: cree une chaine de caractere donnant le chemin fourni
	     sans non de fichier 

SYNTAXE:     char *dirOnly = IdStrDirName (char * path);

RETOUR:      type  : char *
	     role  : pointeur vers la zone allouee contenant le
		     DirectoryName

PARAMETRES:  nom   : path
	     type  : char *
	     role  : chemin fourni

FICHIER:     idstr.c

EXEMPLE:    nom_fichier = IdStrDirName("/rep1/rep2/toto.truc");
               renvoie "/rep1/rep2/"

******************************************************** END DESCRIPTION */

/**
 * \ingroup   str
 * \brief     Extrait le DirectoryName d'un FullPathName 
 * @param chaine     Pointeur vers le FullPathName
 * @return        Pointeur vers le DirectoryName
 */

char *
IdStrDirName (char * nom )
{

  char      *fin, *pnomfic, *nouveau, *debnouveau;
  int       taille;
		  		 	  /* fin pointe sur fin du nom */
  for ( fin = nom; *fin != '\0'; fin ++ );
                                          /* cherche '/' ou '\' avant la fin */
  for ( pnomfic = fin;
#if defined(_MSC_VER)
                         *pnomfic != '\\'; 
#else
                         *pnomfic != '/';
#endif
					 pnomfic -- )
    {
                                          /* pas de '\' ou '/' trouve */
    if ( pnomfic == nom ) break;
    }

  if (pnomfic != nom)			  /* si rep present */
  {
					  /* calcul taille finale */
    taille = (int) (pnomfic - nom) + 2;
					  /* et reservation memoire */  
    nouveau = debnouveau = (char *) malloc (taille);
 					  /* copie du path */
    for ( ; nom <= pnomfic; *(nouveau++) = *(nom++) );
  }
  else nouveau = debnouveau = (char *) malloc (1);
					  /* ajoute fin */
  *nouveau = '\0';

  return debnouveau;
}

/* FUNCTION DESCRIPTION **************************************************

IdStrFileName (fonction)

RESUME: Extrait le FileName d'un FullPathName

DESCRIPTION: cree une chaine de caractere donnant le dernier nom trouve dans
	     le chemin d'acces fourni 

SYNTAXE:     char *lastName = IdStrFileName (char * path);

RETOUR:      type  : char *
	     role  : pointeur vers la zone allouee contenant le
		     nom de fichier.

PARAMETRES:  nom   : path
	     type  : char *
	     role  : chemin fourni

FICHIER:     idstr.c

EXEMPLE:    nom_fichier = IdStrFileName("/rep1/rep2/toto.truc");

******************************************************** END DESCRIPTION */

/**
 * \ingroup   str
 * \brief     Extrait le FileName d'un FullPathName 
 * @param chaine     Pointeur vers le FullPathName
 * @return        Pointeur vers le FileName
 */

char *
IdStrFileName (char * nom )
{
  char        *fin, *pnomfic, *nouveau, *debnouveau;
  int         taille;
			  		  /* fin pointe sur fin du nom */
  for ( fin = nom; *fin != '\0'; fin ++ );
                                          /* cherche '/' ou '\' avant la fin */
  for ( pnomfic = fin;

#if defined(_MSC_VER)
                         *pnomfic != '\\'; 
#else
                         *pnomfic != '/';
#endif
					 pnomfic -- )
  {
                                          /* pas de '\' ou '/' trouve */
    if ( pnomfic == nom )
    {
      pnomfic --;
      break;
    }
  }
					  /* calcul taille finale */
  taille = (int) (fin - (++ pnomfic)) + 1; 
					  /* et reservation memoire */  
  nouveau = debnouveau = (char *) malloc (taille);
 					  /* copie du nom */
  for ( ; pnomfic <= fin; ) *(nouveau++) = *(pnomfic++);

  return debnouveau;
}


/* FUNCTION DESCRIPTION **************************************************

IdStrMakeFullPathName (fonction)

RESUME: Fabrique le FullPathName d'un Fichier

DESCRIPTION: cree une chaine de caractere donnant le nom hierachique complet
	     
SYNTAXE:     char *FullPathName = IdStrFileName (char * dirName, char * fileName);

RETOUR:      type  : char *
	     role  : pointeur vers la zone allouee contenant le
		     nom de fichier.

PARAMETRES:  nom   : dirName
	     type  : char *
	     role  : nom Dir

	     nom   : fileName
	     type  : char *
	     role  : nom Fich
FICHIER:     idstr.c

EXEMPLE:    nom_fichier = IdStrMakeFullPathName("/rep1/rep2/toto","truc");

******************************************************** END DESCRIPTION */

/**
 * \ingroup   str
 * \brief     Fabrique le FullPathName d'un Fichier
 * @param chaine     Pointeur vers le nom Dir
 * @param chaine     Pointeur vers le nom Fich
 * @return        Pointeur vers le FullPathName
 */

char *
IdStrMakeFullPathName (char *  dirName,char *  fileName )
{
  char        *FullPathName;
  int         taille;


	if(dirName) {
		taille=strlen(dirName)+strlen(fileName)+1;
		} else {
		taille=strlen(fileName)+1;	
		}
	FullPathName=(char *) malloc(taille+1);
	if (!FullPathName) { 
		printf("IdStrMakeFullPathName : impossible allouer FullPathName\n");
		return (NULL);
	}
	
	if (strlen(dirName)!=0) {
#if defined(_MSC_VER) 
  		sprintf(FullPathName,"%s\\%s",dirName,fileName); 
#else
  		sprintf(FullPathName,"%s/%s",dirName,fileName); 
#endif
	} else {
  		sprintf(FullPathName,"%s",fileName); 
	}

  	return FullPathName;
}


/* FUNCTION DESCRIPTION **************************************************

IdStrIntEnum (fonction)

RESUME: renvoie 1 tableau contenant une liste d'intervalles entiers

DESCRIPTION:renvoie 1 tableau contenant une liste d'intervalles entiers 

SYNTAXE:     int [] TabInterv= IdStrIntEnum (char * chaine, int * nbInterv);

RETOUR:      type  : int *
	     role  : pointeur vers la zone allouee contenant les
		     intervalles (deb1,fin1, deb2,fin2, ...)

PARAMETRES:  nom   : path
	     type  : char *
	     role  : chaine caract a decoder

		nom   : nbInterv
		type  : int *
		role  : nb d'interv trouves

FICHIER:     idstr.c

EXEMPLE:   

******************************************************** END DESCRIPTION */


/**
 * \ingroup   str
 * \brief     renvoie 1 tableau contenant une liste d'intervalles entiers
 * @param value     pointeur vers la zone allouee contenant les
 *		     intervalles (deb1,fin1, deb2,fin2, ...)
 * @param number     Pointeur versle nb d'interv trouves
 * @return        1 tableau contenant une liste d'intervalles entiers
 */


int *
IdStrIntEnum ( char*value, int *number)

{
  int* liste;
  int taille;
  int i;

  *number = IdStrCountChar(value,',')+1; /* nb Elements = nb Virgules +1 */
  taille= *number;
/* printf ("taille %d\n",taille); */
  liste = (int *) calloc (2*taille , sizeof(int));
  if ( !liste ) {
    IdErrno = IDERR_ALLOC_MEM;
    return 0;
  }

i=0;

  while ( taille>0 ) {
    liste[i] = (int) strtol ( value, &value, 10 );
      
    if ( *value == '\0' ) 
		{
		liste[i+1]=liste[i];
		return liste;
		}

    if ( *(value++) != '-' ) 	{
			liste[i+1]=liste[i];
			value--;
				}
		else		{
			liste[i+1] = (int) strtol ( value, &value, 10 );
				}
    if ( *value == '\0' ) return liste;

    if ( *(value++) != ',' ) {
      free (liste);
      IdErrno = IDERR_NOT_INTLIST;
      return 0;
    }
    taille --; i+=2;
  }
return liste;

}


/* FUNCTION DESCRIPTION **************************************************

IdStrFloatEnum (fonction) 

RESUME: renvoie 1 tableau  contenant une liste d'intervalles FLOAT

DESCRIPTION:renvoie 1 tableau  contenant une liste d'intervalles entiers 

SYNTAXE:     float [] TabInterv= IdStrFloatEnum (char * chaine, int * nbInterv);

RETOUR:      type  : char *
	     role  : pointeur vers la zone allouee contenant le
		     nom de fichier.

PARAMETRES:  nom   : path
	     type  : char *
	     role  : chaine caract a decoder

		nom   : nbInterv
		type  : int *
		role  : nb d'interv trouves

FICHIER:     idstr.c

EXEMPLE:   

******************************************************** END DESCRIPTION */
/**
 * \ingroup   str
 * \brief     renvoie 1 tableau contenant une liste d'intervalles FLOAT
 * @param value     pointeur vers la zone allouee contenant les
 *		     intervalles (deb1,fin1, deb2,fin2, ...)
 * @param number     Pointeur versle nb d'interv trouves
 * @return        1 tableau contenant une liste d'intervalles FLOAT
 */


float *
IdStrFloatEnum (char*  value, int * number)
{
  float* liste;
  int taille;
  int i;

  *number = IdStrCountChar(value,',')+1; /* nb Elements = nb Virgules +1 */
  taille= *number;
/* printf ("taille %d\n",taille); */
  liste = (float *) calloc (2*taille , sizeof(float));
  if ( !liste ) {
    IdErrno = IDERR_ALLOC_MEM;
    return 0;
  }

i=0;

  while ( taille>0 ) {
    liste[i] = (float) strtod ( value, &value );
      
    if ( *value == '\0' ) 
		{
		liste[i+1]=liste[i];
		return liste;
		}

    if ( *(value++) != '-' ) 	{
			liste[i+1]=liste[i];
			value--;
				}
		else		{
			liste[i+1] = (float) strtod ( value, &value );
				}
    if ( *value == '\0' ) return liste;

    if ( *(value++) != ',' ) {
      free (liste);
      IdErrno = IDERR_NOT_INTLIST;
      return 0;
    }
    taille --; i+=2;
  }
return liste;

}

/* FUNCTION DESCRIPTION **************************************************

IdStrSwap (fonction) 

RESUME: renvoie l'entier correspondant au Swap des pixels

DESCRIPTION: renvoie l'entier correspondant au Swap des pixels 

SYNTAXE:     int typeSwap= IdStrSwap (char * chaine);

RETOUR:      type  : int
	     role  : entier correspondant au Swap des pixels.
			"O"	 -> 2143  SHORT, USHORT
			"o"	 -> 2143
			"N"	 -> 1234
			"n"	 -> 1234

			"1234"	 -> 1234  LONG, ULONG
			"2143"	 -> 2143
			"3412"	 -> 3412
			"4321"	 -> 4321

PARAMETRES:  nom   : chaine
	     type  : char *
	     role  : chaine caract a decoder

FICHIER:     idstr.c

EXEMPLE:   

******************************************************** END DESCRIPTION */
/**
 * \ingroup   str
 * \brief     renvoie la valeur SWAP correspondant au code caract passe
 * @param value     pointeur vers le code caract passe
 * @return         valeur SWAP (1234, 4321, 3412, 2143)
 */

int IdStrSwap (char *chaine)
{
if (IdStrCmpNoCase(chaine,"O")==0)	return 2143;
if (IdStrCmpNoCase(chaine,"Y")==0)	return 2143;
if (IdStrCmpNoCase(chaine,"N")==0)	return 1234;
if (IdStrCmpNoCase(chaine,"1234")==0)	return 1234;
if (IdStrCmpNoCase(chaine,"ABCD")==0)	return 1234;
if (IdStrCmpNoCase(chaine,"2143")==0)	return 2143;
if (IdStrCmpNoCase(chaine,"BADC")==0)	return 2143;
if (IdStrCmpNoCase(chaine,"3412")==0)	return 3412;
if (IdStrCmpNoCase(chaine,"CDAB")==0)	return 3412;
if (IdStrCmpNoCase(chaine,"4321")==0)	return 4321;
if (IdStrCmpNoCase(chaine,"DCBA")==0)	return 4321;

return 0;
}


/* FUNCTION DESCRIPTION **************************************************
* 
* IdStrSort    (fonction)
* 
* RESUME: Tri d'un tableau de pointeurs sur N'IMPORTE QUOI, sur N'IMPORTE QUEL critere
* 
* DESCRIPTION:	 Tri d'un tableau de pointeurs sur N'IMPORTE QUOI, 
* 		sur N'IMPORTE QUEL critere.
* 		(Methode de Shell-Metzner)
* 		La fonction de comparaison de deux objets 
* 		est a la charge de l'utilisateur.
* 		(lui seul connait leur structure, et le critere de tri)
* 
* SYNTAXE: IdStrSort ( void *tab,  int (*foncComparObj)(void * ptrObj1, void * ptrObj2);
* 
* 
* RETOUR:		type  : aucun
*  
* PARAMETRES:	nom   : tab
* 		type  : void *
* 		role  : tableau de pointeurs sur N'IMPORTE QUOI
* 
* 		nom   : n
* 		type  : int
* 		role  : nb d'elements du tableau
* 
* 		nom   : foncComparObj
* 		type  : pointeur sur fonction
* 		role  : Fonction de comparaison de deux Objets
* 			retourne	-1  si 1 er < 2 eme
* 					0   si 1 er = 2 eme
* 					+1  si 1 er > 2 eme
* 					-2  si les deux Objets 
* 						NE SONT PAS comparables
* 
* FICHIER:   idstr.c  	
* 
* EXEMPLE:  Soit le tableau T1, de pointeurs vers des 'Objets'
* 	  qui sont des tableaux de chaines de caracteres
* 	 le critere de tri est la chaine num 16 du tableau ...
* 
* 	on triera comme suit:
* 
* 	int comp (void *, void *);
* 	IdStrSort(T1, comp); 
* 
* avec :
*  
* int comp (a, b)
* void *a, *b;
* {
* 	int i;
* 	i=strcmp( ((char **)a)[16],((char **)b)[16]);
* 	if      (i<0) i=-1;
* 	else if (i>0) i= 1;
* 	return i;
* }
*   
* 
******************************************************** END DESCRIPTION */

/**
 * \ingroup   str
 * \brief     	Tri d'un tableau avec 'tableau d'indices'
 * @param tab     pointeur vers le tableau
 * @param n      nb elem du tableau
 * @param foncComparObj     pointeur vers la fonction de comparaison
 * @return  tableau d'indices donnant une 'image triée' du tableau
 */

int * IdStrSort (void **tab, int n,
                INT_FUNCTION_VOIDP_VOIDP_POINTER foncComparObj)
{
   int i,j,m;
   void *b;
   int *tabInd =(int *) malloc(n*sizeof(int));
   if (!tabInd) 
   	return NULL;
   for(i=0;i<n;i++) 
   	tabInd[i]=i;
   
   //  Shell-Metzner' Method
   
   m=1;
   do {
   	m = 3*m+1;
   } while(m <= n);
   do {
   	m = m/3;
   	for (i=m; i<n; i++) {
   		j = i-m;
   		b = tab[i];
   		while( foncComparObj(tab[tabInd[j]],b)==1) {
   			tab[tabInd[j+m]] = tab[tabInd[j]];
   			j = j-m;
   			if (j<0) break;
   		}
   		tab[j+m] = b;
   	}
   }while (m>1);
   
  return (tabInd);
   
}

/* FUNCTION DESCRIPTION **************************************************

IdStrPatternMatching (fonction) 

RESUME: Teste la concordance d'une chaine de caract a un masque

DESCRIPTION: Teste la concordance d'une chaine de caract a un masque 
		(caractere JOKER : * )

SYNTAXE:     int retCode= IdStrPatternMatching (char * chaine, char *masque);

RETOUR:      type  : int
	     role  : pointeur vers la zone allouee contenant le
		     nom de fichier.

PARAMETRES:  nom   : chaine
	     type  : char *
	     role  : chaine caract a decoder

 	     nom   : masque
	     type  : char *
	     role  : masque de concordance

FICHIER:     idstr.c

EXEMPLE:   bool = IdStrPatternMatching (fileName, *.ima);

******************************************************** END DESCRIPTION */
int
IdStrPatternMatching (char *  chaine, char * masque )
{
  int     taille, nbElem, i;
  char ** liste;
  char ** elem;
  char *  chainecur;
  char *  ptrDeb;

  if (!masque) {    
	IdErrno = IDERR_LABEL_UNDEF;
    	return 0;
  }

  nbElem = IdStrCountChar(masque,'*');

  if (masque[0]                == '*') nbElem--;
  if (masque[strlen(masque)-1] != '*') nbElem++;

  taille = nbElem;

  liste = (char **) malloc (sizeof(char*) * taille + strlen(masque)+1);
  if ( !liste ) {
    IdErrno = IDERR_ALLOC_MEM;
    return 0;
  }

  masque = strcpy( ((char*)liste)+sizeof(char*) * taille, masque );

  for ( elem = liste, chainecur = strtok(masque,"*");
	taille>0;
	taille--, chainecur = (chainecur) ? strtok ( 0, "*" ) : 0 )
  {
    *(elem++) = chainecur;
  }

/*printf ("fin Rech :nbElem %d \n",nbElem); 		*/
/*for(i=0;i<nbElem;i++) printf (">%s<\n",liste[i]);	*/

ptrDeb=chaine;
for(i=0;i<nbElem;i++) 
	{
	ptrDeb=strstr(ptrDeb,liste[i]);
	if (!ptrDeb) return(0);
	ptrDeb=ptrDeb + strlen(liste[i]) - 1;
	}

/*
printf ("ptrDeb %x chaine %x strlen(chaine) %d \n", ptrDeb,chaine,strlen(chaine));
*/

if(ptrDeb != chaine+strlen(chaine)-1 ) return 0;
		 
return 1;
}


/**
 * \ingroup   str
 * \brief     Enleve d'une chaine de caract les slash, antislah et espaces
 * 	      elle doit etre termininee par \0
 * @param nom  Pointeur vers le tableau de caract
 * @return    Tableau 'nettoye' (termine par \0)
 */


char * IdStrCleanName (char * nom)
{
	char *ptr, *ptr2, *ptrSecu;
	int indJpr;

		ptr=nom;
		ptr2=ptrSecu=(char *) malloc(strlen(nom)+1);
		indJpr=0;
		while(*ptr != 0) 
		{ 
			if (0) 
			{ 
				indJpr ++; 
				printf ("%d : %c %x\n",indJpr,*ptr,*ptr); 
			}
			if(*ptr != ' ') 
			{ 
				if( (*ptr == '/') || (*ptr == '\\') || (*ptr == '\'') || (*ptr == 0x5e)) 
				{ 
					*ptr2++ = '_';
					*ptr++; 
				} 
				else
				 	*ptr2++ = *ptr++;
			}
			else 
				ptr++;
		}
		*ptr2++=0;

	return(ptrSecu);
}

/**
 * \ingroup   str
 * \brief     Retourne le libelle en clair d'une TRANSFERT SYNTAX
 * 	      
 * @param nom  Pointeur vers la chaine Transf Synt
 * @return    libelle en clair
 */
 
char * _IdStrShowTransfertSyntax (char * codeTransfSynt) {
   int i;
   int nbTransfSynt=23; // voir dicom.c, Sorry
   for(i=0; i<nbTransfSynt; i++) {
      if( strcmp(codeTransfSynt, _ID_transfert_syntax[i].transf_synt_code) == 0 ) 
      return(_ID_transfert_syntax[i].transf_synt_libelle);
   }
 	
   printf("code Transfert Syntax inconnu : [%s]\n",codeTransfSynt);
 	
   return("code Transfert Syntax inconnu");
}
