/*************************************************************************
* $Id: dyimax.c,v 1.2 2005/09/09 14:59:14 yougz Exp $
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
*  Description : Affichage d'une image dans une fenetre X ecran 24 bits
* 		L'ancienne version (conservee dans DYIMAXOLD.C) marchait partout 
*		sauf sous les LINUX 'actuels'
*		(probleme d'incompatibilite entre X11R5 et X11R6 ?)
*		De maniere tres goret, j'ai traite toute image 8 bits comme une image 24 bits
*		mon probleme etant d'avoir un affichage.
*		Sur les ecrans 8 bits, (SUN3, SUN4, et c'est tout) j'appele l'ancienne fonction.
*
**************************************************************************/

#define DEBUG 0

#ifdef HAVE_X11

#include <string.h>  // For memcpy
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "iderr.h"
#include "idima.h"
#include "idprint.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#define RGB24BITS_SW 4
#define RGB24BITS_NW 5
#define COLORS256_SW 2
#define COLORS256_NW 3


static Display *disp;
static Visual *vis;
static int screen_num;
static Window win;

static XImage *xim=NULL;
static GC gc;
static XGCValues val;

static XEvent rep;
static Colormap cm;


static char * imm1;
static int fdp[2];
static int fork_done=0;

static XVisualInfo vTemplate;
static XVisualInfo *visualList;
static int visualsMatched;
static XSetWindowAttributes attributs;
static unsigned long valuemask=0 /*,pixels[2048] */;
static int num_visual;
static PPIMAGE_UCHAR imUCHAR;
static PPIMAGE_RGB imRGB;

/*void _IdImaDisplayColorOld(PPIMAGE_UCHAR ,
                    int , 
			int , int , int , int ,
			int , int , PSIGNAL_RGB);
*/
void _IdImaDisplayColor(PPIMAGE_UCHAR im,
                        int display, // Displays in same window when even
			             	 // and in newly created window when odd
			int x, int y, int nx, int ny,
			int ox, int oy, PSIGNAL_RGB pal)
{
   int i,j,k,kkk=0;
   int lig,v;
   int FlagSameWindow=0;
   
   if (!im) {
	IdErrPrintf("Pointeur image NULL !!\n"); 
	return;
   }
   
   FlagSameWindow=display&0x0001; 
   
   if(!FlagSameWindow) {  /* si PAIR meme fenetre */
   
   	if(!fork_done) {
   		pipe(fdp);
   		fork_done=1;
   		fcntl(fdp[0],F_SETFL,O_NDELAY);
   
   		if(fork())return;

   	} else {
    		switch(IdImaType(im))	{ 
    			
    			case IMA_UCHAR: 
   				for(i=0;i<IdImaDimY(im);i++)
   					write(fdp[1],im[i],IdImaDimX(im));
   				return;
   			
    			case IMA_RGB:		 
    				for(i=0;i<IdImaDimY(im);i++) 
    					write(fdp[1],im[i],IdImaDimX(im)*3);
    				return; 
    		} 
   	}

   } else {  /* si IMPAIR nouvelle fenetre */

	if (fork()) 
		return; 
  }
   
   if(!(disp=XOpenDisplay(0))) {
   	IdPrintf("Pb de display (XOpenDisplay)\n");
	return;
   }
   screen_num=DefaultScreen(disp);
   
   
   /**************************************************************************/
   /* Affichage en d'une image RGB sur ecran 24 bits en TRUE-COLOR           */
   /**************************************************************************/
   
     vTemplate.depth=24;
     visualList=XGetVisualInfo(disp,VisualScreenMask | VisualDepthMask,     
     		&vTemplate,&visualsMatched);
   
	if( visualsMatched == 0 )	{
       		//im=eclate_24_8((PPIMAGE_RGB)im);

		 _IdImaDisplayColorOld( im,  	//juste pour continuer a afficher sur des ecrans 8 bits
                         display,		// sans se casser la tete			         
			 x,  y,  nx,  ny,
			 ox,  oy,  pal);
   		return;
	} else {
       		while(visualList[num_visual].class != TrueColor ) {
        		num_visual++;

        		if ( num_visual >= visualsMatched) {
         			IdErrPrintf("Votre serveur ne permet pas d'afficher en TRUE_COLOR");
         			IdExit(1);
			}
		}
        }

     cm = XCreateColormap(disp,RootWindow(disp,screen_num),
                visualList[num_visual].visual,AllocNone);
     attributs.colormap = cm;
     attributs.event_mask= ButtonPressMask | KeyPressMask | ExposureMask;
     attributs.background_pixel=BlackPixel(disp,screen_num);
     attributs.border_pixel=WhitePixel(disp,screen_num);
     vis=visualList[num_visual].visual;
     valuemask |= CWColormap;
     valuemask |= CWEventMask;
     valuemask |= CWBackPixel;
     valuemask |= CWBorderPixel;
     win=XCreateWindow(disp,RootWindow(disp,screen_num),
          0,0,IdImaDimX(im),IdImaDimY(im),50,vTemplate.depth,
          InputOutput,visualList[num_visual].visual,valuemask,&attributs);
     if(!win) IdErrPrintf("Erreur creation fenetre \n");
     XSetWindowColormap(disp,win,cm);
     XClearWindow(disp,win);
     XStoreName(disp,win,IdGetFileName(im));
     imm1=(char *)malloc(IdImaDimX(im)*IdImaDimY(im)*4);
     
     
    switch(IdImaType(im))	{
    
     case IMA_RGB: 
     imRGB=(PPIMAGE_RGB)im;
   
     for(i=0,k=0;i<IdImaDimY(im);i++)
     	for(j=0;j<IdImaDimX(im);j++,k+=4) {
     	/*
       		imm1[k]  =0; 
       		imm1[k+1]=imRGB[i][j].b;
       		imm1[k+2]=imRGB[i][j].g;
       		imm1[k+3]=imRGB[i][j].r;
       	*/
		imm1[k]  =imRGB[i][j].b; 
		imm1[k+1]=imRGB[i][j].g;
		imm1[k+2]=imRGB[i][j].r;
		imm1[k+3]=0;
      	}
      	
      break;
      
      case IMA_UCHAR:
      
      imUCHAR=(PPIMAGE_UCHAR)im;
      if (!pal)
      		for(i=0,k=0;i<IdImaDimY(im);i++)
     		for(j=0;j<IdImaDimX(im);j++,k+=4) {
			imm1[k]  = imm1[k+1] = imm1[k+2] = imUCHAR[i][j]; 
       			imm1[k+3]=0;
      		} 
      else
            	for(i=0,k=0;i<IdImaDimY(im);i++)
     		for(j=0;j<IdImaDimX(im);j++,k+=4) {  
			
			imm1[k]  =pal[imUCHAR[i][j]].b; 
			imm1[k+1]=pal[imUCHAR[i][j]].g;
			imm1[k+2]=pal[imUCHAR[i][j]].r;
       			imm1[k+3]=0;
      		} 
               
	break;
   
     	default:
   
      	IdErrPrintf("type d'image (%04x) non autorise a l'affichage\n",
      	IdImaType(im));
      	IdExit(1);

      	break;
   }
    
    
        xim=XCreateImage(disp,vis,24,ZPixmap,0,(char *)imm1,
   	IdImaDimX(im),IdImaDimY(im),16,IdImaDimX(im)*4);
     if(!xim)
      IdErrPrintf("erreur creation image\n");
      
      
   /*************************************************************************/
   /* Partie commune affichage des images xim                               */
   /*************************************************************************/
   
   gc=XCreateGC(disp,win,(GCForeground|GCBackground),&val);
   
   XMapWindow(disp,win);
   XSelectInput(disp,win,ButtonPressMask | KeyPressMask | ExposureMask);
   XPutImage(disp,win,gc,xim,0,0,0,0,IdImaDimX(im),IdImaDimY(im));
   lig=0;
   while(1)
   	{
   	if( XEventsQueued(disp,QueuedAfterFlush) || display) {
   		
   		XNextEvent(disp,&rep);
   		switch(rep.type) {
   			case ButtonPress:
                    	if( rep.xbutton.button == 1) {
   							XPutImage(disp,win,gc,xim,0,0,0,
   									0,IdImaDimX(im),IdImaDimY(im));
   							XFreeGC(disp,gc);
   							XCloseDisplay(disp);
   							IdExit(0); 
   				}	
                        break;

                    case Expose:
   				XPutImage(disp,win,gc,xim,0,0,
   					0,0,IdImaDimX(im),IdImaDimY(im));
   				break;

   			case KeyPress:
   				XPutImage(disp,win,gc,xim,0,0,
   					0,0,IdImaDimX(im),IdImaDimY(im));
   				break;
   			}
   	} else {
   		switch (IdImaType(im)) {
   		
   
   				case IMA_UCHAR :
   	      	  	  /*---------------*/
   	      
      			if( (v=read(fdp[0],im[lig],IdImaDimX(im))) != -1) {
   			
   				if(v) {
   				
   					for(j=0;j<IdImaDimX(im);j++,kkk+=4) {
   				
   						imm1[kkk]  = imm1[kkk+1] = imm1[kkk+2] = imUCHAR[lig][j]; 
       						imm1[kkk+3]=0;		
   		   			}
   					lig++;
   				}
   				if(lig==IdImaDimY(im)) {
   				
   					lig=0;
   					kkk=0;
   					XPutImage(disp,win,gc,xim,0,0,0,0,
   						IdImaDimX(im),IdImaDimY(im));
   				}
   			} /* fin if v=read(fdp... */
   		
   			break;	
   			
   						      
   			case IMA_RGB:
   	      	  /*-------------*/
   
   		 	if( (v=read(fdp[0],im[lig],IdImaDimX(im)*3)) != -1) {
   			
   				if(v) {
   				
   					for(j=0;j<IdImaDimX(im);j++,kkk+=4) {
   					
   						/*
       						imm1[kkk]  =0; 
       						imm1[kkk+1]=imRGB[lig][j].b;
       						imm1[kkk+2]=imRGB[lig][j].g;
       						imm1[kkk+3]=imRGB[lig][j].r;
       						*/
       					
       						imm1[kkk]  =imRGB[lig][j].b; 
						imm1[kkk+1]=imRGB[lig][j].g;
						imm1[kkk+2]=imRGB[lig][j].r;
						imm1[kkk+3]=0;
   					}
   					lig++;
   				}
   				if(lig==IdImaDimY(im)) {
   				
   					lig=0;
   					kkk=0;
   
   					XPutImage(disp,win,gc,xim,0,0,0,0,
   						IdImaDimX(im),IdImaDimY(im));
   				}
   			} /* fin if v=read(fdp... */
   
   			break;
   
   		} /* fin Switch	*/
   
   	   }
   	}
}

#else /* HAVE_X11 */
#include "idima.h"
/* Here we put dummy functions to satisfy filters programs compilation
 * even when X11 is not available
 */
void _IdImaDisplayColor(PPIMAGE_UCHAR im,
                        int display, 
                        int x, int y, int nx, int ny,
                        int ox, int oy, PSIGNAL_RGB pal)
{}
#endif /* HAVE_X11 */
