/*************************************************************************
* $Id: cntcdg.c,v 1.1 2005/09/09 08:22:22 bellet Exp $
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
#include <math.h>
#include "idcnt.h"
#include "iderr.h"

static int pic=0;
static int ravin=0,  supprec=0,  infprec=0,  supsuiv=0,  infsuiv=0, fourche=0, seul=1, flag=0,  i,  j;
static PPIMAGE_UCHAR imtmp=0;

static void testprec (void);
static void testsuiv (void);


/* FUNCTION DESCRIPTION **************************************************

IdCntCentreGravite   (fonction)

RESUME: Calcule le Centre de Gravite (non pondere) d'un contour

DESCRIPTION: Calcule le Centre de Gravite (non pondere) d'un contour
		Les points doivent etre 'assez proches' les uns des autres
		Remarque: ds le cas de points 'tres eloignes' il faudrait rendre
		le CNT connexe

SYNTAXE:     void IdCntCentreGravite (PCONTOUR_USHORT cnt, int *x, int *y );

RETOUR:      type  : viod

PARAMETRES:  nom   : cnt
	     type  : PCONTOUR_USHORT
	     role  : Pointeur vers le contour

	     nom   : x
	     type  : int *
	     role  : Pointeur vers la coordonne X du cdg

	     nom   : y
	     type  : int *
	     role  : Pointeur vers la coordonne Y du cdg

FICHIER:     cntcdg.c

EXEMPLE:    
******************************************************** END DESCRIPTION */

void IdCntCentreGravite(Pts, GravX, GravY)
PCONTOUR_USHORT Pts;
int *GravX, *GravY;
{
int i,xt,yt;
int NbPts;

NbPts=IdCntUsedNbX(Pts);
for(i=0, xt=0, yt=0; i<NbPts; i++)
	{
	 xt += IdCntGetX(Pts,i); 
      	 yt += IdCntGetY(Pts,i); 
	}
*GravX= xt/NbPts;
*GravY= yt/NbPts;
}

/* FUNCTION DESCRIPTION **************************************************

IdCntRectEnglob   (fonction)

RESUME: Calcule le Rectangle Englobant (Xmin,Ymin, Xmax, Ymax) d'un contour USHORT

DESCRIPTION:
 
Calcule le Rectangle Englobant (Xmin,Ymin, Xmax, Ymax) d'un contour USHORT

SYNTAXE:     void IdCntRectEnglob (PCONTOUR_USHORT cnt, int *xmin, int *ymin, int *xmax, int *ymax);

RETOUR:      type  : viod

PARAMETRES:  nom   : cnt
	     type  : PCONTOUR_USHORT
	     role  : Pointeur vers le contour

	     nom   : xmin,ymin,xmax,ymax
	     type  : int *
	     role  : Pointeur vers les coordonnees du Rectangle Englobant

FICHIER:     cntcdg.c

EXEMPLE:
    
******************************************************** END DESCRIPTION */

void IdCntRectEnglob(Pts, Xmin, Ymin, Xmax, Ymax)
PCONTOUR_USHORT Pts;
int *Xmin, *Ymin, *Xmax, *Ymax;
{
int i;
int NbPts;

NbPts=IdCntUsedNbX(Pts);
*Xmin= *Ymin= 0x7FFF;  /* CM au lieu de 65535 */
*Xmax= *Ymax= 0;

for(i=0; i<NbPts; i++)
	{
	if(IdCntGetX(Pts,i) > *Xmax) *Xmax=IdCntGetX(Pts,i);
	if(IdCntGetX(Pts,i) < *Xmin) *Xmin=IdCntGetX(Pts,i);
	if(IdCntGetY(Pts,i) > *Ymax) *Ymax=IdCntGetY(Pts,i);
	if(IdCntGetY(Pts,i) < *Ymin) *Ymin=IdCntGetY(Pts,i);
 	}
}


/* FUNCTION DESCRIPTION **************************************************

IdCntClean   (macro)

RESUME: Supprime les points consecutifs identiques d'un CNT USHORT.

DESCRIPTION: Supprime les points consecutifs identiques d'un CNT USHORT.
		A l'attention des developpeurs LibIDO:
		On remarquera que le fait qu'il y ait une Macro derriere
		ne sert A RIEN ... JPRx

SYNTAXE:     int retCode = IdCntClean(PCONTOUR_USHORT cntSource);

RETOUR:      
	     type  : int
             role  : code retour booleen.

PARAMETRES:  
	     nom   : cntSource
             type  : PCONTOUR_USHORT
             role  : pointeur sur le contour
 
FICHIER:

******************************************************** END DESCRIPTION */
int _IdCntClean(cnt1)
PCONTOUR_USHORT *cnt1; 
{

int ai, bi, aj, bj;
int i, j, n1, n2;

if (*cnt1==NULL) return 0;

n1 =n2= IdCntUsedNbX(*cnt1);

for (i=0,j=1; j < n1;j++) 
	{
	ai=IdCntGetX(*cnt1, i);	
	bi=IdCntGetY(*cnt1, i);
	aj=IdCntGetX(*cnt1, j);	
	bj=IdCntGetY(*cnt1, j); 	
	if ((ai==aj) && (bi==bj))
		{
		n2=n2-1; 		
		}
     	else   {IdCntSetX(*cnt1, i+1, aj);
		IdCntSetY(*cnt1, i+1, bj);
		i++;
		}
	}
IdCntSetUsedNbX(*cnt1,n2);
return (1);
} 


/* FUNCTION DESCRIPTION **************************************************

IdCntCleanFromStart   (macro)

RESUME: Supprime les points consecutifs identiques d'un CNT USHORT a partir d'1 pt donne.

DESCRIPTION: Supprime les points consecutifs identiques d'un CNT USHORT a partir d'1 pt donne.

SYNTAXE:     int retCode = IdCntCleanFromStart(PCONTOUR_USHORT cntSource, int startPoint);

RETOUR:      
	     type  : int
             role  : code retour booleen.

PARAMETRES:  
	     nom   : cntSource
             type  : PCONTOUR_USHORT
             role  : pointeur sur le contour

	     nom   : startPoint
             type  : int
             role  : point de depart
 
FICHIER:

******************************************************** END DESCRIPTION */
int _IdCntCleanFromStart(cnt1,stP)
PCONTOUR_USHORT *cnt1;
int stP; 
{

int ai, bi, aj, bj;
int i, j, n1, n2;

if (*cnt1==NULL) return 0;

n1 =n2= IdCntUsedNbX(*cnt1);

for (i=stP,j=stP+1; j < n1;j++) 
	{
	ai=IdCntGetX(*cnt1, i);	
	bi=IdCntGetY(*cnt1, i);
	aj=IdCntGetX(*cnt1, j);	
	bj=IdCntGetY(*cnt1, j); 	
	if ((ai==aj) && (bi==bj))
		{
		n2=n2-1; 		
		}
     	else   {IdCntSetX(*cnt1, i+1, aj);
		IdCntSetY(*cnt1, i+1, bj);
		i++;
		}
	}
IdCntSetUsedNbX(*cnt1,n2);
return (1);
} 

/* FUNCTION DESCRIPTION **************************************************

IdCntConnexPts   (Macro)

RESUME: Intercale des points ds un CNT USHORT pour le rendre connexe (8).

DESCRIPTION: Intercale des points ds un CNT USHORT pour le rendre connexe (8)
		Le pointeur sur contour est modifie!

SYNTAXE:     int retCode = IdCntConnexPts(PCONTOUR_USHORT cntSource, int ferme);

RETOUR:      
	     type  : int
             role  : code retour booleen.

PARAMETRES:  
	     nom   : cntSource
             type  : PCONTOUR_USHORT
             role  : pointeur sur le contour

	     nom   : ferme
             type  : int
             role  : semantique du CNT (ouvert:0 ferme:1)
 
FICHIER:

******************************************************** END DESCRIPTION */

int _IdCntConnexPts (cnt1, ferme)
PCONTOUR_USHORT *cnt1;
int ferme;
{

int ai, bi, aj = 0, bj = 0;
int j, n1, retCode;
PCONTOUR_USHORT cnt2;

if (*cnt1==NULL) return 0;

IdCntClean(*cnt1);

n1   = IdCntUsedNbX(*cnt1);
if (n1==1) return 1;

cnt2 = IdCntAlloc(n1,CNT_USHORT);
if(!cnt2) return 0;

ai=IdCntGetX(*cnt1,0);
bi=IdCntGetY(*cnt1,0);

IdCntAddPoint(cnt2,ai,bi);

for(j=1;j<n1;j++)
	{ 
	aj=IdCntGetX(*cnt1,j);
	bj=IdCntGetY(*cnt1,j);
	if( (abs((int)(aj-ai))<2) && (abs((int)(bj-bi))<2) ) 
		{ IdCntAddPoint(cnt2,aj,bj);
		}
	else
		{
		  retCode=IdCntAddPointsOfSegment(cnt2,ai,bi,aj,bj);
		  if (!retCode) return 0;		
		}
	ai=aj; bi=bj;
	}

if (ferme)
	{
	retCode=IdCntAddPointsOfSegment(cnt2,aj,bj,
			IdCntGetX(*cnt1,0),IdCntGetY(*cnt1,0));
	if (!retCode) return 0;	
	}

IdCntFree(*cnt1);
*cnt1=cnt2;
	
return 1;
}	

/* FUNCTION DESCRIPTION **************************************************

IdCntSurfacePoly   (fonction)

RESUME: Calcule le nb de pixels a l'interieur d'un CNT USHORT.

DESCRIPTION: Calcule le nb de pixels a l'interieur d'un CNT USHORT.

SYNTAXE:     double nbPixels = IdCntSurfacePoly(PCONTOUR_USHORT cntSource);

RETOUR:      
	     type  : double
             role  : renvoie le nb de pixels a l'interieur d'un CNT USHORT

PARAMETRES:  
	     nom   : cntSource
             type  : PCONTOUR_USHORT
             role  : pointeur sur le contour

FICHIER:

******************************************************** END DESCRIPTION */
double IdCntSurfacePoly(cntSource)
PCONTOUR_USHORT cntSource;

{
double surface=0.0;
int ox,oy,fx,fy,precedent, conseq;

if (cntSource==NULL) return 0;

IdCntClean(cntSource);
if (IdCntUsedNbX(cntSource)<3) return 0;

IdCntRectEnglob(cntSource,&ox,&oy,&fx,&fy);
imtmp=(PPIMAGE_UCHAR)IdImaAlloc(fx-ox+1,fy-oy+1,IMA_UCHAR);

if(!imtmp) return 0;
imtmp=(PPIMAGE_UCHAR)IdImaClear((PPIMAGE)imtmp);

IdCntDrawPolygonOffset(cntSource,ox,oy,imtmp,0,255); /* 0: trace directe */

for(i=0;i<IdImaDimY(imtmp);i++) 
{
	precedent=0; 
	flag=0;
	conseq=0;
	seul=1;
	for(j=0;j<IdImaDimX(imtmp);j++)
	{
		if ( imtmp[i][j] == 255 )
		{
		 	 surface ++; 
 
		 	if (precedent==255)
			{ 
				conseq=1;
			}
			else /* 1 er pt blanc */
			{
				conseq=0;

					if (j==0){
						if(i==IdImaDimY(imtmp)-1){  if(imtmp[i][1]==0) flag=0; continue; }
						if(i==0)		 {  if(imtmp[0][1]==0) flag=0; continue; }
						 } 
						testprec();

						if ((!pic) && (!ravin))
							{ 
							if (seul==1){ if (flag==0) flag=1; else flag=0; }
							}
			}
		}

		else /* point noir */
	 	{
			if (conseq!=0) /* il y a eu +sieurs pts blancs consecutifs */
			{ 
				testsuiv();
				if (fourche != 1) /* sur une fourche, le flag ne bouge pas */
				{
					if (  (supprec && infsuiv) || (infprec && supsuiv) ) {
						if (flag==0) flag=1; else flag=0;
					}
				}
			}
		 	if (flag==1)  { surface ++;
 				      }		
			conseq=0;
	 	}
		precedent=imtmp[i][j];
	} 
 }
return surface;
}	

/* FUNCTION DESCRIPTION **************************************************

IdCntFillExtPolygon   (fonction)

RESUME: Rempli sur une image UCHAR l'exterieur d'un CNT USHORT (polygone qcq).

DESCRIPTION: Rempli sur une image UCHAR l'exterieur d'un CNT USHORT (polygone qcq)

SYNTAXE:     int retCode = IdCntFillExtPolygon(PCONTOUR_USHORT cntSource, PPIMAGE_UCHAR im, int nivGris);

RETOUR:      
	     type  : int
             role  : code de retour booleen.

PARAMETRES:  
	     nom   : cntSource
             type  : PCONTOUR_USHORT
             role  : pointeur sur le contour

	     nom   : im
             type  : PPIMAGE_UCHAR
             role  : pointeur sur l'image

 	     nom   : nivGris
             type  : int
             role  : niveau de gris de remplissage

FICHIER:

********************************************************  END DESCRIPTION */

int IdCntFillExtPolygon(cntSource, im, nivGris)
PCONTOUR_USHORT cntSource;
PPIMAGE_UCHAR im;
int nivGris;
{
PPIMAGE_UCHAR im2;

im2=(PPIMAGE_UCHAR)IdImaAllocLikeImage(im);
if(!im2) return (0);
IdImaClear((PPIMAGE)im2);
IdCntFillPolygon(cntSource, im2, 255);
for(i=0;i<IdImaDimY(im2);i++) 
	for(j=0;j<IdImaDimX(im2);j++)
		if ( im2[i][j] == 0 )
			im[i][j]= nivGris;
return(1);

}	

/* FUNCTION DESCRIPTION **************************************************

IdCntFillPolygon   (fonction)

RESUME: Rempli sur une image UCHAR l'interieur d'un CNT USHORT (polygone qcq).

DESCRIPTION: Rempli sur une image UCHAR l'interieur d'un CNT USHORT (polygone qcq)

SYNTAXE:     int retCode = IdCntFillPolygon(PCONTOUR_USHORT cntSource, PPIMAGE_UCHAR im, int nivGris);

RETOUR:      
	     type  : int
             role  : code de retour booleen.

PARAMETRES:  
	     nom   : cntSource
             type  : PCONTOUR_USHORT
             role  : pointeur sur le contour

	     nom   : im
             type  : PPIMAGE_UCHAR
             role  : pointeur sur l'image

 	     nom   : nivGris
             type  : int
             role  : niveau de gris de remplissage

FICHIER:

********************************************************  END DESCRIPTION */
int IdCntFillPolygon(cntSource, im, nivGris)
PCONTOUR_USHORT cntSource;
PPIMAGE_UCHAR im;
int nivGris;
{

if (cntSource==NULL) return 0;
if (im==NULL)        return 0;

IdCntClean(cntSource);
if (IdCntUsedNbX(cntSource)<3) return 0;

imtmp=(PPIMAGE_UCHAR)IdImaAllocLikeImage(im);

if(!imtmp) return 0;
imtmp=(PPIMAGE_UCHAR)IdImaClear((PPIMAGE)imtmp);


IdCntDrawPolygon(cntSource,imtmp,0,255); /* 0: trace directe */

IdImaFill(imtmp,imtmp,1); /* JPR */

for(i=0;i<IdImaDimY(im);i++)for(j=0;j<IdImaDimX(im);j++)

	if(imtmp[i][j]==255)im[i][j]=nivGris;

	/*if(imtmp[i][j]==0)im[i][j]=nivGris; */ /* GROS pb JPRx */

IdImaFree(imtmp); /* JPR */

return (1);

}

/* ********************************************************************** */


int IdCntFillPolygonXXX(PCONTOUR_USHORT cntSource, PPIMAGE_UCHAR im, int nivGris)
{
int ox,oy,fx,fy;
int precedent, conseq;

if (cntSource==NULL) return 0;
if (im==NULL)        return 0;

IdCntClean(cntSource);
if (IdCntUsedNbX(cntSource)<3) return 0;

IdCntRectEnglob(cntSource,&ox,&oy,&fx,&fy);
imtmp=(PPIMAGE_UCHAR)IdImaAlloc(fx-ox+1,fy-oy+1,IMA_UCHAR);

if(!imtmp) return 0;
imtmp=(PPIMAGE_UCHAR)IdImaClear((PPIMAGE)imtmp);

IdCntDrawPolygonOffset(cntSource,ox,oy,imtmp,0,255); /* 0: trace directe */

for(i=0;i<IdImaDimY(imtmp);i++) 
{
	precedent=0; 
	flag=0;
	conseq=0;
	seul=1;
	for(j=0;j<IdImaDimX(imtmp);j++)
	{
		if ( imtmp[i][j] == 255 )
		{
		 	 im[i+oy][j+ox]= nivGris; 
 
		 	if (precedent==255)
			{ 
				conseq=1;
			}
			else /* 1 er pt blanc */
			{
				conseq=0;

					if (j==0){
						if(i==IdImaDimY(imtmp)-1){  if(imtmp[i][1]==0) flag=0; continue; }
						if(i==0)		 {  if(imtmp[0][1]==0) flag=0; continue; }
						 } 
						testprec();

						if ((!pic) && (!ravin))
							{ 
							if (seul==1) {if (flag==0) flag=1; else flag=0; }
							}
			}
		}

		else /* point noir */
	 	{
			if (conseq!=0) /* il y a eu +sieurs pts blancs consecutifs */
			{ 
				testsuiv();
				if (fourche != 1) /* sur une fourche, le flag ne bouge pas */
				{
					if (  (supprec && infsuiv) || (infprec && supsuiv) ) 
					{
						if (flag==0) flag=1; else flag=0;
					}
				}

			}
		 	if (flag==1)  { im[i+oy][j+ox]= nivGris;
 				      }		
			conseq=0;
	 	}
		precedent=imtmp[i][j];
	} 
 }
return 1;
}	
	
/* ===================================================================================================== */

static void testprec()
{
pic=ravin=infprec=supprec=0;
seul=1;

if (j !=IdImaDimX(imtmp)-2) if (imtmp[i][j+1]==255) seul=0;

if (i==0)
{
	if 	(imtmp[0][j+1]==0) { pic=1; ravin=0;if(imtmp[1][j]==255) imtmp[0][j]=0; } else { infprec=1; supprec=0;}	
}

else if (i==IdImaDimY(imtmp)-1)
{
	if 	(imtmp[i][j+1]==0) { pic=0; ravin=1; } else { infprec=0; supprec=1;}	
}

else if (j==0)  /* premiere colonne */

{
     if (  (imtmp[i-1][j]==0) && (imtmp[i-1][j+1]==0) && (imtmp[i][j+1]==0) )
	{ pic=1; if(imtmp[i+1][j]==255) imtmp[i][j]=0; }

else if (  (imtmp[i+1][j]==0) && (imtmp[i+1][j+1]==0) && (imtmp[i][j+1]==0) )
	{ ravin=1;}

else if ( ( (imtmp[i-1][j]==255) || (imtmp[i-1][j+1]==255)) && (imtmp[i+1][j+1]==0) )
	{ supprec=1; }

else if (  imtmp[i+1][j]==255  )
	{ infprec=1; }
return;
}

			/* autres colonnes */
else {
 if ( (imtmp[i-1][j-1]==0) && (imtmp[i-1][j]==0) && (imtmp[i-1][j+1]==0) && (imtmp[i][j+1]==0) )
	{ pic=1; if(imtmp[i+1][j]==255) imtmp[i][j]=0; }

else if ( (imtmp[i+1][j-1]==0) && (imtmp[i+1][j]==0) && (imtmp[i+1][j+1]==0) && (imtmp[i][j+1]==0) )
	{ ravin=1;}

else if ( ((imtmp[i-1][j-1]==255) || (imtmp[i-1][j]==255) || (imtmp[i-1][j+1]==255)) && (imtmp[i+1][j+1]==0) )
	{ supprec=1; }

else if ( ((imtmp[i+1][j-1]==255) || (imtmp[i+1][j]==255) || (imtmp[i+1][j-1]==255)) && (imtmp[i-1][j-1]==0) )
	{ infprec=1; }
     }

return;

}

static void testsuiv()
{
fourche=supsuiv=infsuiv=0;

if (i==0)  { infsuiv=1; }

else if (i==IdImaDimY(imtmp)-1)  { supsuiv=1; }

else if (j==IdImaDimX(imtmp)-1) 
	{ if 		( (imtmp[i-1][j-1]==255) || (imtmp[i-1][j]==255)) { supsuiv=1;}
	   else if	( (imtmp[i+1][j-1]==255) || (imtmp[i+1][j]==255)) { infsuiv=1;}
	}
	
else 
	{	if ( (imtmp[i-1][j+1]==0) && ((imtmp[i+1][j+1]==255) || (imtmp[i+1][j]==255)) )
			{ infsuiv=1; }
		if ( (imtmp[i+1][j+1]==0) && ((imtmp[i-1][j+1]==255) || (imtmp[i-1][j]==255)) )
			{ supsuiv=1; }

		if (     ((imtmp[i-1][j-1]==255) || (imtmp[i-1][j]==255))   
		     &&  ((imtmp[i+1][j-1]==255) || (imtmp[i+1][j]==255))  )
		{ fourche=1;}
	}
return;
}

/* FUNCTION DESCRIPTION **************************************************

IdImaFillContour   (fonction)

RESUME: Rempli dans une image UCHAR l'interieur de contours a 255.

DESCRIPTION: Rempli dans une image UCHAR l'interieur de contours a 255

SYNTAXE:     int retCode = IdImaFillContour(PPIMAGE_UCHAR im, int nivGris);

RETOUR:      
	     type  : int
             role  : code de retour booleen.

PARAMETRES:  
	     nom   : im
             type  : PPIMAGE_UCHAR
             role  : pointeur sur l'image

 	     nom   : nivGris
             type  : int
             role  : niveau de gris de remplissage

FICHIER:

********************************************************  END DESCRIPTION */

int IdImaFillContour(PPIMAGE_UCHAR im, int nivGris)
{
int precedent, conseq;

if (im==NULL)        return 0;

imtmp=(PPIMAGE_UCHAR)IdImaAllocLikeImage(im);

if(!imtmp) return 0;
imtmp=(PPIMAGE_UCHAR)IdImaClear((PPIMAGE)imtmp);

for(i=0;i<IdImaDimY(im);i++)for(j=0;j<IdImaDimY(im);j++) if (im[i][j]==255) imtmp[i][j]=255;

for(i=0;i<IdImaDimY(imtmp);i++) 
{
	precedent=0; 
	flag=0;
	conseq=0;
	seul=1;
	for(j=0;j<IdImaDimX(imtmp);j++)
	{
		if ( imtmp[i][j] == 255 )
		{
		 	 im[i][j]= nivGris; 
 
		 	if (precedent==255)
			{ 
				conseq=1;
			}
			else /* 1 er pt blanc */
			{
				conseq=0;

					if (j==0){
						if(i==IdImaDimY(imtmp)-1){  if(imtmp[i][1]==0) flag=0; continue; }
						if(i==0)		 {  if(imtmp[0][1]==0) flag=0; continue; }
						 } 
						testprec();

						if ((!pic) && (!ravin))
							{ 
							if (seul==1)
								{
								if (flag==0) flag=1; else flag=0; 
								}
							}
			}
		}

		else /* point noir */
	 	{
			if (conseq!=0) /* il y a eu +sieurs pts blancs consecutifs */
			{ 
				testsuiv();
				if (fourche != 1) /* sur une fourche, le flag ne bouge pas */
				{
					if (  (supprec && infsuiv) || (infprec && supsuiv) ) 
					{
						if (flag==0) flag=1; else flag=0;
					}
				}

			}
		 	if (flag==1)  { im[i][j]= nivGris;
 				      }		
			conseq=0;
	 	}
		precedent=imtmp[i][j];
	} 
 }
return 1;
}	
	
