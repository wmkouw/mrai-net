/*************************************************************************
* $Id: dyimaxold.c,v 1.1 2005/09/09 08:22:24 bellet Exp $
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
*  Description : Ancienne version, qui marchait partout sauf sous LINUX
*		conservée pour afficher sur des ecrans 8 bits
*		sans se compliquer la vie.
*		- Des que j'ai (re)trouve et adapté un code qui affiche sur ecrans 8/16/24/32 bits
*		je jette tout et je remplace JPRx -
*		
**************************************************************************/

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

static PPIMAGE_UCHAR eclate_24_8 (PPIMAGE_RGB);

static Display *disp;
static Visual *vis;
static int screen_num;
static Window win;

static XImage *xim=NULL;
static GC gc;
static XGCValues val;
static XColor xcol;
static XEvent rep;
static Colormap cm;
static int lut[256];
static char **imm;
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

/*void _IdImaDisplayColorOld(PPIMAGE_UCHAR , // pour eviter les warnings
                    int , 
			int , int , int , int ,
			int , int , PSIGNAL_RGB);
*/
void _IdImaDisplayColorOld(PPIMAGE_UCHAR im,
                        int display, // Displays in same window when even
			             // and in newly created window when odd
			int x, int y, int nx, int ny,
			int ox, int oy, PSIGNAL_RGB pal)
{
   int i,j,k,kkk=0;
   int lig,v;
   int FlagSameWindow=0;
   int default24;
   int incr; /* increment indice palette */
   
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
   
   
   switch(IdImaType(im))
   {
   case IMA_UCHAR:
   
   /**************************************************************************/
   /* Affichage en 128 niveaux de gris ou en 128 couleurs garanties avec     */
   /* une palette passee en parametre                                        */
   /**************************************************************************/
   
    default24=0;
   
    vis=DefaultVisual(disp,screen_num);
   
    vTemplate.screen=screen_num;
    vTemplate.depth=24;
    visualList=XGetVisualInfo(disp,VisualScreenMask | VisualDepthMask,     
     				&vTemplate,&visualsMatched);
    for(i=0;i< visualsMatched;i++) {
      	if(visualList[num_visual].visual == vis ) { 
		default24=1;
   		if(display ==0) display=4;
   		if(display ==1) display=5;
   		break;
	}
   				
   }
   
   if(display ==0 || display ==1 ) {
   
    	imUCHAR=(PPIMAGE_UCHAR)im;
    	vis=DefaultVisual(disp,screen_num);
   
    	win=XCreateSimpleWindow(disp,RootWindow(disp,screen_num),
   		0,0,IdImaDimX(im),IdImaDimY(im),50/*4*/,WhitePixel(disp,screen_num),
   		BlackPixel(disp,screen_num));
    	XClearWindow(disp,win);
    	XStoreName(disp,win,IdGetFileName(im));
   
    	xcol.flags=DoRed | DoGreen | DoBlue;
    	if (!pal) {
      		for(i=0;i<256;i+=2) {   /*  pas de palette -> 128 niveaux de gris */

   			xcol.red  =i<<8;
   			xcol.green=i<<8;
   			xcol.blue =i<<8;
   			XAllocColor(disp,DefaultColormap(disp,screen_num),&xcol);

   			if (xcol.pixel<=255) {
   					lut[i]  =xcol.pixel;
   					lut[i+1]=xcol.pixel;
   			}
   	}

    } else {		/* on traite la palette */

	if (IdSigDimX(pal)>128)	// pour toute palette >128, on prend une
                                // couleur sur 2 ...
   		incr=2;
   	else
   		incr=1;
   
      	for(i=0;i<IdSigDimX(pal);i+=incr) {
   		xcol.red  =pal[i].r<<8;
   		xcol.green=pal[i].g<<8;
   		xcol.blue =pal[i].b<<8;
   		XAllocColor(disp,DefaultColormap(disp,screen_num),&xcol);

   		if( xcol.pixel<=255) {
   			lut[i]=xcol.pixel;
   			lut[i+1]=xcol.pixel;
   		}
	}
    }

    imm=   (char **)malloc(IdImaDimY(im)*sizeof(char *));
    imm[0]=(char *) malloc(IdImaDimX(im)*IdImaDimY(im)*IdSizeOfType(im));

    for(i=0;i<IdImaDimY(im);i++)
   	imm[i]=(char *)imm[0] + i * IdImaDimX(im) * IdSizeOfType(im);
   
    for(i=0;i<IdImaDimY(im);i++)for(j=0;j<IdImaDimX(im);j++) {
   	imm[i][j]=lut[imUCHAR[i][j]&0xff];
    }
   
   if (default24)
     	xim=XCreateImage(disp,vis,24,ZPixmap,0,imm[0],
   		IdImaDimX(im),IdImaDimY(im),8,IdImaDimX(im));
   else
    	xim=XCreateImage(disp,vis,8,ZPixmap,0,imm[0],
   		IdImaDimX(im),IdImaDimY(im),8,IdImaDimX(im));
   
    }
   
   /**************************************************************************/
   /* Affichage en 256 niveaux de gris ou en 256 couleurs non garanties avec */
   /* une palette passee en parametre                                        */
   /**************************************************************************/
   
   if(display ==2 || display ==3 )  /* cette partie n'a pas d'interet (pas pratique) */
				    /* laissee ici pour compatibilite avec des versions anterieures */
   {
   
    imUCHAR=(PPIMAGE_UCHAR)im;
    vis=DefaultVisual(disp,screen_num);
    win=XCreateSimpleWindow(disp,RootWindow(disp,screen_num),
   	0,0,IdImaDimX(im),IdImaDimY(im),50/*4*/,WhitePixel(disp,screen_num),
   	BlackPixel(disp,screen_num));
    XClearWindow(disp,win);
    XStoreName(disp,win,IdGetFileName(im));
   
    xcol.flags=DoRed | DoGreen | DoBlue;
    if (!pal)
    {
      for(i=0;i<256;i+=1) {  // on n'a pas passe de palette -> 256 niveaux
                             // de gris
   	xcol.red  =i<<8;
   	xcol.green=i<<8;
   	xcol.blue =i<<8;
   	XAllocColor(disp,DefaultColormap(disp,screen_num),&xcol);
   	if( xcol.pixel<=255) {
        	lut[i]  =xcol.pixel;
   	}
      }
    } else {		// on traite la palette
      for(i=0;i<IdSigDimX(pal);i+=1)
   	{
   	xcol.red  =pal[i].r<<8;
   	xcol.green=pal[i].g<<8;
   	xcol.blue =pal[i].b<<8;
   	XAllocColor(disp,DefaultColormap(disp,screen_num),&xcol);
   	if( xcol.pixel<=255 )
   		{
   		lut[i]=xcol.pixel;
   		}
   	}
    }
    imm=   (char **)malloc(IdImaDimY(im)*sizeof(char *));
    imm[0]=(char *) malloc(IdImaDimX(im)*IdImaDimY(im)*IdSizeOfType(im));
    for(i=0;i<IdImaDimY(im);i++)
   	imm[i]=(char *)imm[0] + i * IdImaDimX(im) * IdSizeOfType(im);
   
    for(i=0;i<IdImaDimY(im);i++)for(j=0;j<IdImaDimX(im);j++)
   	{
   	imm[i][j]=lut[imUCHAR[i][j]&0xff];
   	}
   
    xim=XCreateImage(disp,vis,8,ZPixmap,0,imm[0],
   	IdImaDimX(im),IdImaDimY(im),8,IdImaDimX(im));
   
    }
   
   /**************************************************************************/
   /* Affichage en 256 niveaux de gris garantis avec colormap reservee       */
   /* ou Affichage en 256 couleurs garantis en allouant une colormap conforme*/
   /* a la palette passee en parametre                                       */
   /**************************************************************************/
   
   if( display == 4 || display == 5 )
   {
       vTemplate.screen=screen_num;
       vTemplate.depth=8;
       visualList=XGetVisualInfo(disp,
                        VisualScreenMask    | 
   			VisualDepthMask     | 
   			VisualRedMaskMask   | 
   			VisualGreenMaskMask | 
   			VisualRedMaskMask,  
   	 		&vTemplate,&visualsMatched);
   
       /* Recherche d'un visual type PSEUDO-COLOR */
       num_visual=0; 
       while ( visualList[num_visual].class != PseudoColor ){
           num_visual++;
           if( num_visual >= visualsMatched ) {  
             IdErrPrintf("Erreur impossible d'afficher en pseudocolor");
             IdExit(1);}}
   
        imUCHAR=(PPIMAGE_UCHAR)im;
   
       cm = XCreateColormap(disp,RootWindow(disp,screen_num),
                visualList[num_visual].visual,AllocAll);
       if(!cm)IdErrPrintf("Erreur creation colormap \n");
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
          0,0,IdImaDimX(imUCHAR),IdImaDimY(imUCHAR),50,vTemplate.depth,
          InputOutput,vis,valuemask,&attributs);
       if(!win) IdErrPrintf("Erreur creation fenetre \n");
       XSetWindowColormap(disp,win,cm);
       XClearWindow(disp,win);
       XStoreName(disp,win,IdGetFileName(imUCHAR));
    
       xcol.flags=DoRed | DoGreen | DoBlue;
       if (!pal) {
        	for(i=0;i<256;i+=1) { 	// on n'a pas passe de palette -> niveaux
	                       		// de gris
        		xcol.pixel=i;
   			xcol.red  =i<<8;
   			xcol.green=i<<8;
   			xcol.blue =i<<8;
          		XStoreColor(disp,cm,&xcol);
   		}
       } else {		/* on traite la palette */

        	for(i=0;i<IdSigDimX(pal);i+=1) {
          		 xcol.pixel=i;
   			xcol.red  =pal[i].r<<8;
   			xcol.green=pal[i].g<<8;
   			xcol.blue =pal[i].b<<8;
   			XStoreColor(disp,cm,&xcol);
		}  
   
       }

    imm1=(char *)malloc(IdImaDimX(imUCHAR)*IdImaDimY(imUCHAR)*IdSizeOfType(imUCHAR));
   
    for(i=0;i<IdImaDimY(imUCHAR);i++)		 
       memcpy(imm1+(i*IdImaDimX(imUCHAR)), imUCHAR[i], IdImaDimX(imUCHAR));
    
    
    xim=XCreateImage(disp,vis,8,ZPixmap,0, imm1,
   	IdImaDimX(imUCHAR),IdImaDimY(imUCHAR),8,IdImaDimX(imUCHAR));
    }
   break;
   
   
   case IMA_RGB: 
   
   /**************************************************************************/
   /* Affichage en d'une image RGB sur ecran 24 bits en TRUE-COLOR           */
   /**************************************************************************/
   
     vTemplate.depth=24;
     visualList=XGetVisualInfo(disp,VisualScreenMask | VisualDepthMask,     
     		&vTemplate,&visualsMatched);
   
      if( visualsMatched == 0 )	{
       		im=eclate_24_8((PPIMAGE_RGB)im);
   		break;
   	} else {
       		while(visualList[num_visual].class != TrueColor ) {
        		num_visual++;

        		if ( num_visual >= visualsMatched) {
         			IdErrPrintf("Votre serveur ne permet pas d'afficher en TRUE_COLOR");
         			IdExit(1);
			}
		}
        }

     imRGB=(PPIMAGE_RGB)im;
   
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
          0,0,IdImaDimX(imRGB),IdImaDimY(imRGB),50,vTemplate.depth,
          InputOutput,visualList[num_visual].visual,valuemask,&attributs);
     if(!win) IdErrPrintf("Erreur creation fenetre \n");
     XSetWindowColormap(disp,win,cm);
     XClearWindow(disp,win);
     XStoreName(disp,win,IdGetFileName(imRGB));
     imm1=(char *)malloc(IdImaDimX(imRGB)*IdImaDimY(imRGB)*4);
   
     for(i=0,k=0;i<IdImaDimY(imRGB);i++)
     	for(j=0;j<IdImaDimX(imRGB);j++,k+=4) {
       		imm1[k]  =0; 
       		imm1[k+1]=imRGB[i][j].b;
       		imm1[k+2]=imRGB[i][j].g;
       		imm1[k+3]=imRGB[i][j].r;
      	}
     xim=XCreateImage(disp,vis,24,ZPixmap,0,(char *)imm1,
   	IdImaDimX(imRGB),IdImaDimY(imRGB),16,IdImaDimX(imRGB)*4);
     if(!xim)
      IdErrPrintf("erreur creation image\n");
          
    break;
   
   default:
   
      IdErrPrintf("type d'image (%d) non autorise a l'affichage\n",
      IdImaType(im));
      IdExit(1);

      break;
   }
    
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
   	if( XEventsQueued(disp,QueuedAfterFlush) || display)
   		{
   		XNextEvent(disp,&rep);
   		switch(rep.type)
   			{
   			case ButtonPress:
                                   if( rep.xbutton.button == 1)
                                   {
   				XPutImage(disp,win,gc,xim,0,0,0,
   					0,IdImaDimX(im),IdImaDimY(im));
   				XFreeGC(disp,gc);
   				XCloseDisplay(disp);
   				IdExit(0); }	
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
   		}
   	else
   		{
   		switch (IdImaType(im))
   		{
   		case IMA_UCHAR:
   	      /*---------------*/
   		 if( (v=read(fdp[0],im[lig],IdImaDimX(im))) != -1)
   			{
   			if(v)
   				{
   				for(j=0;j<IdImaDimX(im);j++)
   					{
   					imm[lig][j]=lut[im[lig][j]&0xff];
   					}
   				lig++;
   				}
   			if(lig==IdImaDimY(im))
   				{
   				lig=0;
   				XPutImage(disp,win,gc,xim,0,0,0,0,
   					IdImaDimX(im),IdImaDimY(im));
   				}
   			} /* fin if v=read(fdp... */
   		
   		break;
   
   		case IMA_RGB:
   	      /*-------------*/
   
   		 if( (v=read(fdp[0],im[lig],IdImaDimX(im)*3)) != -1)
   			{
   			if(v)
   				{
   				for(j=0;j<IdImaDimX(im);j++,kkk+=4)
   					{
   
       					imm1[kkk]  =0; 
       					imm1[kkk+1]=imRGB[lig][j].b;
       					imm1[kkk+2]=imRGB[lig][j].g;
       					imm1[kkk+3]=imRGB[lig][j].r;
   					}
   				lig++;
   				}
   			if(lig==IdImaDimY(im))
   				{
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

/* --------------------------------------------------------------------------*/

static PPIMAGE_UCHAR eclate_24_8(PPIMAGE_RGB imInput)
{
   PPIMAGE_UCHAR imDest;
   int i,j;
   
   imDest=(PPIMAGE_UCHAR)IdImaAlloc(IdImaDimX(imInput)*3,IdImaDimY(imInput),IMA_UCHAR);
   for(i=0;i<IdImaDimY(imInput);i++)for(j=0;j<IdImaDimX(imInput);j++)
   	{
   	imDest[i][j]=				imInput[i][j].r;
   	imDest[i][j+IdImaDimX(imInput)]=	imInput[i][j].g;
   	imDest[i][j+2*IdImaDimX(imInput)]=	imInput[i][j].b;
   	}
   
   /* Ecrire des fonctions ... */
   
    vis=DefaultVisual(disp,screen_num);
    win=XCreateSimpleWindow(disp,RootWindow(disp,screen_num),
   	0,0,IdImaDimX(imDest),IdImaDimY(imDest),50,WhitePixel(disp,screen_num),
   	BlackPixel(disp,screen_num));
    XClearWindow(disp,win);
    XStoreName(disp,win,IdGetFileName(imDest));
   
    xcol.flags=DoRed | DoGreen | DoBlue;
   
      for(i=0;i<256;i+=2)    // on n'a pas passe de palette -> 128 niveaux
                             // de gris
   	{
   	xcol.red  =i<<8;
   	xcol.green=i<<8;
   	xcol.blue =i<<8;
   	XAllocColor(disp,DefaultColormap(disp,screen_num),&xcol);
   	if( xcol.pixel<=255 )
   		{
   		lut[i]  =xcol.pixel;
   		lut[i+1]=xcol.pixel;
   		}
   	}
   
    imm=(char **)malloc(IdImaDimY(imDest)*sizeof(char *));
    imm[0]=(char *)malloc(IdImaDimX(imDest)*IdImaDimY(imDest)*
                            IdSizeOfType(imDest));
    for(i=0;i<IdImaDimY(imDest);i++)
   	imm[i]=(char *)imm[0] + i * IdImaDimX(imDest) * IdSizeOfType(imDest);
   
    for(i=0;i<IdImaDimY(imDest);i++)for(j=0;j<IdImaDimX(imDest);j++)
   	{
   	imm[i][j]=lut[imDest[i][j]&0xff];
   	}
   
    xim=XCreateImage(disp,vis,8,ZPixmap,0,imm[0],
   	IdImaDimX(imDest),IdImaDimY(imDest),8,IdImaDimX(imDest));
   
   return imDest;
}
#endif
