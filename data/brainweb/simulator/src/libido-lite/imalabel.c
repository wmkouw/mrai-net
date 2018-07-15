/*************************************************************************
* $Id: imalabel.c,v 1.1 2005/09/09 08:22:48 bellet Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include "idima.h"
#include "idmac.h"
#include "iderr.h"
#include "idprint.h"

/*******************************************************************************
*
* Description :  Avec des images UCHAR
* Recherche de composantes connexes en ligne dans une image binaire
* avec calcul de caracteristiques simples				  
* d : distance horizontale entre les elements a connecter     
* d = 0 : 4 - connexite 						  
* d = 1 : 8 - connexite 				
*
*******************************************************************************/


/* Prototypes fonctions internes *********************************************/
static void _label_trans       ( PPIMAGE_UCHAR, int);
static void _label_resultat    ( FILE * );
static void _label_debut       ( PPIMAGE_SHORT, int, short * );
static void _label_fin         ( FILE*, FILE*, int );
static void _label_poursuite   ( PPIMAGE_SHORT, int ,int );
static void _label_recherche   ( void );
static void _label_maj_fus     ( void );
static void _label_maj_fn      ( void );
static void _label_mise_a_jour ( void );

/**************************************************************************/

#define Malloc(type) 	(type *)malloc(sizeof(type))
#define NB_MAX_TRANS 	25 /* Nb maximum de transitions Noir/Blanc par ligne */
#define NB_MAX_COUL 	1024 /* Nombre maximum d'objets (en cours) */

#define deb 		0 /* 1: Mode debug-suivi - 0 : Mode execution normale */
#if (deb)
#  define DEBUG(a)      a
#else
#  define DEBUG(a)
#endif

typedef enum BOOL { false, true } bool;
typedef struct CONNEX
	{
	int trans[NB_MAX_TRANS],trans_cours[NB_MAX_TRANS];
	short coul[NB_MAX_COUL];
	int surface,xdeb,xfin,ydeb,yfin;
	struct CONNEX *suivant;
	} connexe;
typedef struct MODELE
	{
	int surface,xdeb,xfin,ydeb,yfin;
	short coul[NB_MAX_COUL];
	struct MODELE *suivant;
	} modele;
/******************************************************************************
* connexe  : description de chaque objet en cours de traitement :	
*  - trans : liste des debuts des transitions de l'objet a la ligne precedente;
*  - coul  : liste des couleurs de l'objet;
*  - caracteristiques de l'objet :
*	- suface;
*	- rectangle exinscrit;
*  - pointeurs sur l'objet suivant dans la liste;
* modele : liste des objets traites;
*******************************************************************************/

/****** Declarations de variables globales ******/
static connexe *premier,*courant,*dernier_cx,*precedent,*fus_cx,*precsup;
static modele *pre_mod,*cou_mod,*der_mod;
static int cou[512],sup[512],*pt_cou,*pt_sup;
static int nb_gros,nb_petit,surf_tot,surf_gros,larg,surfmini;

/* FUNCTION DESCRIPTION **************************************************
* 
* IdImaLabel   (fonction)
* 
* RESUME: Cherche les objets connexes ds une image binaire
* 
* DESCRIPTION: 
*   Cherche les objets connexes d'une certaine taille dans une image binaire.
*   L'image labellisee est une image de type SHORT et chaque objet connexe
*   est represente par un niveau de gris different.
*   Le fichier fich_mod contient pour chaque objet le rectangle exinscrit et
*   le numero sous la forme:
* 	xd yd xf yf numero
*   Le fichier fich_res contient pour chaque objet le numero, la surface et
*   le rectangle exinscrit sous la forme:
* 	Objet No : 1    Surface = 1
*         Xmin = 10       Ymin = 5        Xmax = 10       Ymax = 5
* 
* SYNTAXE:	int nbObj= IdImaLabel(PPIMAGE_UCHAR imSource ,PPIMAGE_SHORT imLabel, int surf_min,FILE *fich_res, FILE *fich_mod, int connexite); 
* 
* RETOUR:      type  : int
* 	     role  : -1 si echec, sinon le nombre d'objets trouves
* 
* PARAMETRES:  nom   : im
* 	     type  : PPIMAGE_UCHAR
* 	     role  : Pointeur vers l'image source
* 
* 	     nom   : imo
* 	     type  : PPIMAGE_SHORT
* 	     role  : Pointeur vers l'image 'labellisee'
* 
* 	     nom   : surf_min
* 	     type  : int
* 	     role  : taille minimale des objets a considerer comme tels
* 
* 	     nom   : fich_res
* 	     type  : FILE *
* 	     role  : 
* 
* 	     nom   : fich_mod
* 	     type  : FILE *
* 	     acces : I
* 	     role  : 
* 
* 	     nom   : connexite
* 	     type  : int
* 	     role  : connexite = 1 : 8-connexite  connexite  = 0 : 4-connexite
* 
* FICHIER:     imalabel.c
* 
* EXEMPLE: 
*  if ( (nb= IdImaLabel(im,imo,surf_min,fich_res,fich_mod,1) ) == -1)
*      IdErrPrintf("ERREUR: %s",IdErrMsg(IdErrno));
* 
******************************************************** END DESCRIPTION */

int IdImaLabel(PPIMAGE_UCHAR im, PPIMAGE_SHORT imo, int surf_min, FILE *fich_res, FILE *fich_mod, int connexite)
                 
                  
                         
             
               /* connexite = 1 : 8-connexite  connexite  = 0 :4- connexite */
{
unsigned char h;
short niv,couleur,ctr[1024];
int compt=0,col,col1;
int y,i,j,nbp,nbp2;
                                                 /* Controle des l'images **/
  if ( !im || IdImaType(im) != IMA_UCHAR ){
    IdErrno = IDERR_WRONG_TYPE;
    return -1;
  }
  if ( !imo || IdImaType(imo) != IMA_SHORT ){
    IdErrno = IDERR_WRONG_TYPE;
    return -1;
  }

surfmini=surf_min;
surf_tot=0;surf_gros=0;nb_petit=0;nb_gros=0;couleur=0;
pt_cou=cou; pt_sup=sup;
for(y=0;y<512;y++) { *pt_cou++ = -1; *pt_sup++ = -1; }
pt_cou=cou; pt_sup=sup;
pre_mod=Malloc(modele); premier=Malloc(connexe); dernier_cx=Malloc(connexe);
cou_mod=pre_mod; courant=premier; premier->suivant=dernier_cx;
for(y=0;y<=IdImaDimY(im);y++)
	{
	DEBUG(IdPrintf("\n%d : ",y);)
	courant=premier;
	while(courant->suivant != dernier_cx)
		{
		h=0;
		courant=courant->suivant; 
		while(courant->trans_cours[h] != -1)
			{
			courant->trans[h] = courant->trans_cours[h];
		 	courant->trans[++h] = -1;
			}
		courant->trans_cours[0] = -1;
		}
	pt_cou=cou; pt_sup=sup;
	while(*pt_cou != -1) *pt_sup++ = *pt_cou++;
	*pt_sup = -1;
	pt_cou=cou; pt_sup=sup;
	if(y != IdImaDimY(im)) _label_trans(im,y);
	else {*pt_cou++ = -1; *pt_cou = -1; pt_cou=cou; pt_sup=sup; }
	while((*pt_cou != -1)&&(*pt_sup != -1)) 
		{
		if((*(pt_cou+1)+connexite) < *pt_sup) _label_debut(imo,y,&couleur);
		else if(*pt_cou > (*(pt_sup+1)+connexite)) _label_fin(fich_res,fich_mod,y);
	     	else _label_poursuite(imo,y,connexite);
		}
	if(*pt_cou != -1) while(*pt_cou != -1) _label_debut(imo,y,&couleur);
	else while(*pt_sup != -1) _label_fin(fich_res,fich_mod,y);
	}
_label_resultat(fich_res);
der_mod=cou_mod;cou_mod=pre_mod;
for(col=0;col<1024;col++) ctr[col]=0;
while(cou_mod != der_mod) {
	cou_mod=pre_mod->suivant;
	if(cou_mod->surface >= surf_min){
		compt++;
/*		IdPrintf("\n%3d : ",compt); */
		col=0;
		while(cou_mod->coul[col]) {
			col1=cou_mod->coul[col++];
/*			IdPrintf("%d ",col1); */
			ctr[col1]=compt;
			if(col >= NB_MAX_COUL-2)
			  {IdErrPrintf("NB_MAX_COUL trop petit\n");IdExit(0);}
			}
		}
	pre_mod=cou_mod;
	}

nbp=0;
nbp2=0;
for(i=0;i<IdImaDimY(imo);i++)
	for(j=0;j<IdImaDimX(imo);j++){
		niv = imo[i][j];
		if(niv){
			imo[i][j] = ctr[niv];
			nbp2++;
			if(ctr[niv]) nbp++;
		/*	 else IdPrintf("N %d\t",niv); */
			}
		}

if((nbp != nbp2) && (surfmini < 2)) IdErrPrintf("Pb Tableau des niveaux\n");
IdPrintf("Surface totale %d, des 'gros' objets: %d, Pts image: %d\n",surf_tot,surf_gros,nbp);
/*
i=1; while(ctr[i]) {IdPrintf("%d --> %d\n",i,ctr[i]); i++;}
for(i=1;i<1024;i++) if(ctr[i]) {IdPrintf("%d --> %d\n",i,ctr[i]);}
*/

free((char *)premier);free((char *)dernier_cx);
der_mod=cou_mod;cou_mod=pre_mod;
while(cou_mod != der_mod) 
	{ cou_mod=pre_mod->suivant; free((char *)pre_mod);pre_mod=cou_mod;}
free((char *)der_mod);
return(nb_gros);
}

/******************************************************************************/
	 /* Determination des transitions de la ligne traitee */
static void
 _label_trans(PPIMAGE_UCHAR im, int lig)
{
int x;

DEBUG( IdPrintf("Ligne %d\n",lig); )
if( im[lig][0] )  {  *pt_cou=0;   /*IdPrintf("1ere colonne non vide\n");*/}
for(x=0;x<IdImaDimX(im)-1;x++)
	{
	if(im[lig][x] < im[lig][x+1]) *pt_cou = x+1;
	else if(im[lig][x] > im[lig][x+1]) { *(++pt_cou) = x; pt_cou++;}
	}
if(im[lig][x])  { *(++pt_cou) = IdImaDimX(im)-1; pt_cou++; }
*pt_cou++ = -1; *pt_cou = -1;
pt_cou=cou; pt_sup=sup;
}

/******************************************************************************/
static void
 _label_resultat(FILE *fich_res)
{
fprintf(fich_res,"Nombre d'objets = %d\t",nb_gros);
if(nb_petit) fprintf(fich_res,"plus %d de surface <= %d pixels\n",nb_petit,surfmini);
fprintf(fich_res,"Somme des Surfaces de tous les objets : %d\n",surf_tot);
fprintf(fich_res,"Somme des Surfaces des gros objets : %d\n",surf_gros);
}

/******************************************************************************/
	/* Debut d'une nouvelle region */
static void
 _label_debut(PPIMAGE_SHORT imo, int lig, short int *pcoul)
{
int xt;
DEBUG(IdPrintf("_label_debut ");)

(*pcoul)++;
for(xt = *pt_cou;xt <= *(pt_cou+1);xt++) imo[lig][xt] = *pcoul;
/*IdPrintf("%d\n",*pcoul);*/
larg = *(pt_cou+1) - *pt_cou + 1;
/* ----- Creation d'un connexe ----- */
precedent=dernier_cx;
precedent->suivant=Malloc(connexe);
/* ----- Initialisation des parametres du nouveau connexe ----- */
precedent->trans_cours[0] = *pt_cou; precedent->trans_cours[1] = -1;
precedent->coul[0] = *pcoul; precedent->coul[1] = 0;
precedent->surface = larg; precedent->ydeb = lig;
precedent->xdeb = *pt_cou++; precedent->xfin = *pt_cou++;
dernier_cx=precedent->suivant;
}

/******************************************************************************/
	 /* Fin d'une region */
static void
 _label_fin(FILE *fich_res, FILE *fich_mod, int lig)
{
int col,*pt_temp;
bool existe;

DEBUG(IdPrintf("_label_fin ");)
existe=false;
_label_recherche();
fus_cx=courant; precsup=precedent;
pt_temp=pt_sup;
if(courant->trans_cours[0] != -1) existe=true;
while(*(pt_sup+2) != -1)
      { pt_sup += 2;_label_recherche();if(courant==fus_cx) {existe=true;break;} }
pt_sup=pt_temp;
if(!existe)
	{
	DEBUG(IdPrintf("! ");)
/* ----- Creation d'un objet du modele ----- */
	cou_mod->suivant=Malloc(modele);cou_mod=cou_mod->suivant;
	surf_tot += fus_cx->surface;
/* ----- Affectation des parametres du nouvel objet du modele ----- */
	col=0;
	while(fus_cx->coul[col]) {
		cou_mod->coul[col] = fus_cx->coul[col];
		/*IdPrintf("%d ",cou_mod->coul[col]);*/
		col++;
		if(col >= NB_MAX_COUL-2)
			{IdErrPrintf("NB_MAX_COUL trop petit\n");IdExit(0);}
		}
	cou_mod->coul[col] = 0;
	cou_mod->surface = fus_cx->surface;
	cou_mod->xdeb = fus_cx->xdeb; cou_mod->xfin = fus_cx->xfin;
	cou_mod->ydeb = fus_cx->ydeb; cou_mod->yfin = lig-1;
/* ----- Elimination du connexe ----- */
	precsup->suivant = fus_cx->suivant;
	free((char *)fus_cx);
/* ----- Affichage des parametres  de chaque modele ----- */
  if(cou_mod->surface >= surfmini) {
    nb_gros++;surf_gros += cou_mod->surface;
    fprintf(fich_mod,"%d\t%d\t",cou_mod->xdeb,cou_mod->ydeb);
    fprintf(fich_mod,"%d\t %d\t%d\n",cou_mod->xfin,cou_mod->yfin,nb_gros);
    fprintf(fich_res,"Objet No : %d\tSurface = %d\n",nb_gros,cou_mod->surface);
    fprintf(fich_res,"\tXmin = %d\tYmin = %d",cou_mod->xdeb,cou_mod->ydeb);
    fprintf(fich_res,"\tXmax = %d\tYmax = %d\n",cou_mod->xfin,cou_mod->yfin);
    }
  else nb_petit++;
  }
pt_sup += 2;
}

/******************************************************************************/
		 /* Poursuite d'une region */
static void
 _label_poursuite(PPIMAGE_SHORT imo, int lig, int connexite)
{
int xt,col,col1;
short niveau;

_label_recherche();
if((*(pt_sup+2) <= (*(pt_cou+1)+connexite)) && (*(pt_sup+2) != -1))
	/*-*-* fusion *-*-*/
{
	fus_cx=courant;
	pt_sup += 2;
	_label_recherche();
   	if(courant != fus_cx) {
DEBUG( IdPrintf("\n%d %d %d %d\n",*pt_cou,*(pt_cou+1),*pt_sup,*(pt_sup+1));)
		col=0;
		while(fus_cx->coul[col]){
			col++;
			if(col >= NB_MAX_COUL-2)
			  {IdErrPrintf("NB_MAX_COUL trop petit\n");IdExit(0);}
			}
		col1=0;
		while(courant->coul[col1]){
			fus_cx->coul[col++] = courant->coul[col1++];
			if((col1 >= NB_MAX_COUL-2)||(col >= NB_MAX_COUL-2))
			  {IdErrPrintf("NB_MAX_COUL trop petit\n");IdExit(0);}

			}
		fus_cx->coul[col] = 0;
		_label_maj_fus();	/*-*-* Detection d'une fusion  *-*-*/
DEBUG( IdPrintf("fusion "); )
		}
	 else { _label_maj_fn(); /*-*-* Detection d'une fusion "normale" *-*-*/
DEBUG( if(deb) IdPrintf("fusion normale "); )
		}
	while((*(pt_sup+2) <= (*(pt_cou+1)+connexite)) && (*(pt_sup+2) != -1))
	{
		pt_sup += 2;
		fus_cx=courant;
		_label_recherche();
   		if(courant != fus_cx) {
			col=0;
			while(fus_cx->coul[col]){
				col++;
				if(col >= NB_MAX_COUL-2)
					{IdErrPrintf("NB_MAX_COUL trop petit\n");IdExit(0);}
				}
			col1=0;
			while(courant->coul[col1]){
				fus_cx->coul[col++] = courant->coul[col1++];
				if((col1 >= NB_MAX_COUL-2)||(col >= NB_MAX_COUL-2))
					{IdErrPrintf("NB_MAX_COUL trop petit\n");IdExit(0);}
				}
			fus_cx->coul[col] = 0;
			_label_maj_fus(); /*-* Detection d'une fusion *-*/
DEBUG(IdPrintf("fusion ");)
			}
		 else { _label_maj_fn(); /*-* Detection d'une fusion "normale" *-*/
DEBUG( IdPrintf("fusion normale ");)
			}
	}
}

else    {
	col1=0;
	while(courant->coul[col1]){
		col1++;
		if(col1 >= NB_MAX_COUL-2)
			{IdErrPrintf("NB_MAX_COUL trop petit\n");IdExit(0);}
		}
	niveau=courant->coul[col1-1];
	DEBUG( IdPrintf("niv = %d ",niveau); )
	DEBUG( IdPrintf("%d ",*pt_sup); )
	larg = *(pt_cou+1) - *pt_cou + 1;
	for(xt = *pt_cou;xt<= *(pt_cou+1);xt++) imo[lig][xt]=niveau;
	if((*(pt_cou+2) <= (*(pt_sup+1)+connexite)) && (*(pt_cou+2) != -1))
		{                       /*-*-* Detection d'un eclatement *-*-*/
		DEBUG(IdPrintf("eclatement ");)
		_label_mise_a_jour();
		}
	else	{			/*-*-* Poursuite normale *-*-*/
		_label_mise_a_jour();pt_sup += 2;
		DEBUG( IdPrintf("pours. normale ");)
		}
	}
}

/******************************************************************************/
static void
 _label_recherche(void)
{
bool trouve;
unsigned char k;

trouve=false;
courant=premier;
DEBUG( IdPrintf("\nRech (%d): ",*pt_sup);)
while((courant->suivant != dernier_cx) && (!trouve))
	{
	k=0;
	precedent=courant;
	courant=courant->suivant; 
	DEBUG(IdPrintf("**  ");)
	while(courant->trans[k] != -1)
		{ 
		DEBUG(IdPrintf("%d ",courant->trans[k]);)
		if(courant->trans[k] == *pt_sup) {trouve=true; break;}
		k++;
		}
	}
DEBUG( IdPrintf("\n");)
}

/******************************************************************************/
/* ---  Mise a jour des parametres du connexe en cours : FUSION --- */
/* ---  Fusion de deux regions qui appartenaient a des connexes differents --- */
static void
 _label_maj_fus(void)
{
unsigned char k,j;

k=0;j=0;
while(fus_cx->trans_cours[k] != -1) k++;
while(courant->trans_cours[j] != -1) 
	{ fus_cx->trans_cours[k] = courant->trans_cours[j]; j++; k++; }
fus_cx->trans_cours[k++] = *pt_cou;
fus_cx->trans_cours[k] = -1;
k=0;j=0;
while(fus_cx->trans[k] != -1) k++;
while(courant->trans[j] != -1) 
	{ 
	DEBUG( IdPrintf("\nNo %d",courant->trans[j]);)
	fus_cx->trans[k] = courant->trans[j]; j++; k++;
	}
fus_cx->trans[k] = -1;
fus_cx->surface += courant->surface;
fus_cx->xdeb = min(min(fus_cx->xdeb,courant->xdeb),*pt_cou);
fus_cx->xfin = max(max(fus_cx->xfin,courant->xfin),*(pt_cou+1));
fus_cx->ydeb = min(fus_cx->ydeb,courant->ydeb);
precedent->suivant = courant->suivant;
free((char *)courant);
courant=fus_cx;
}

/******************************************************************************/
/* ---  Mise a jour des parametres du connexe en cours : FUSION NORMALE ---*/
/* ---  Fusion de deux regions qui appartenaient au meme connexe ---*/
static void
 _label_maj_fn(void)
{
unsigned char k=0;

while(courant->trans_cours[k] != -1) k++;
courant->trans_cours[k++] = *pt_cou;
courant->trans_cours[k] = -1;
courant->xdeb = min(courant->xdeb,*pt_cou);
courant->xfin = max(courant->xfin,*(pt_cou+1));
}

/******************************************************************************/
/* ---  Mise a jour des parametres du connexe en cours : POURSUITE NORMALE ---*/
static void
 _label_mise_a_jour(void)
{
unsigned char k=0;

courant->surface += larg;
while(courant->trans_cours[k] != -1) k++;
courant->trans_cours[k++] = *pt_cou;
courant->trans_cours[k] = -1;
courant->xdeb = min(courant->xdeb,*pt_cou);
courant->xfin = max(courant->xfin,*(pt_cou+1));
pt_cou += 2;
}



