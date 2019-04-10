#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <time.h>
#include "mand.h"

#define HEIGHT          640.0
#define WIDTH           640.0
#define LEFT           -4.0
#define RIGHT           4.0
#define TOP             2.0
#define BOTTOM         -2.0
#define MAX             256
#define TRUE            1
#define FALSE           0
#define IMAGEWRITE      10

double comp_increment, real_increment;
double right, left, top, bottom;
int iter, screen, start_color, color_seperation, iterations;
struct complex c;



void fewargs()
{
   /* If someone gives too few arguments, then tell them so! */
   printf("Too few Arguments!\n");
   return;
}

void printhelp()
{
   printf("Invalid arguments\n");
   printf("Options:\n");
   printf("  -h: Prints this help\n");
   printf("  -s <s>: The Seperation between each colour\n");
   printf("  -f <f>: The first colour in to use\n");
   printf("  -i <i>: Number of iterations and colours\n");
}


void initialise(int h, int w)
{
   comp_increment = (TOP - BOTTOM) / h;
   real_increment = (RIGHT - LEFT) / w;
   c.real = LEFT;
   c.imaginary = BOTTOM;
   top = TOP;
   bottom = BOTTOM;
   right = RIGHT;
   left = LEFT;
   start_color = 20;
   color_seperation = 10000;
   iterations = 256;
   printf("XMand v1.0\n");
   printf("    By: RedOctober (John Ferlito)\n");
   printf("        e-mail: johnf@zip.com.au\n");
   printf("    Copyright (C) 1996\n");
}

int iterate(int reset)
{
   struct complex z;
   double cond;
   //double pixel,r,g,b;
   int iter, lim;

   lim = right;
   if (!reset)
   {
      c.real = c.real + real_increment;
   }
   else if (reset)
   {
      c.imaginary += comp_increment;
      c.real = left;
   }

   z.real = 0; z.imaginary = 0; z.tempreal = 0; iter = 0;

   while (iter <= iterations + 1)
   {
      if (iter > iterations - 1)
        return 0;
      iter++;
      z.tempreal = (z.real * z.real) - (z.imaginary * z.imaginary) + c.real;
      z.imaginary = (2 * (z.real * z.imaginary)) + c.imaginary;
      z.real = z.tempreal;
      cond = z.real * z.real + z.imaginary * z.imaginary;

      if (cond >= 8)
      {
/*	 r = (unsigned short)iter<<16;
	 g = (unsigned short)iter<<8;
	 b = (unsigned short)iter;
	 pixel = (unsigned short)r + (unsigned short)g + (unsigned short)b;
	 printf("%d %f %f %f %f\n",iter, r,g,b,pixel);
	 fflush(stdout);*/
	 return iter;
      }
   }
   return iter;
}


int myrand(int numb, int seed)
{
   int number, rand_max,f;

   srandom(seed);
   f = random();

   rand_max = numb;
   number = 1 + (f % rand_max);

   return number;

}


void SetMandelPalette(struct globaldata *gb, int f, int *ctab)
{
   /*This function sets the Mandel palette */
   Colormap cm;
   XColor xcl;
   int r, g, b, i;

   b = 20;
   /* if the user asled for the windows own colormap, take all the colors */
   /* the display has when this window is active */


   cm = DefaultColormap(gb->disp, gb->scr);
   xcl.red = (unsigned short)(0 << 8);
   xcl.green = (unsigned short)(0 << 8);
   xcl.blue = (unsigned short)(0 << 8);
   xcl.flags = DoRed|DoGreen|DoBlue;
   XAllocColor(gb->disp, cm, &xcl);
   ctab[0]=(int)xcl.pixel;

   for (i = 1; i < MAX; i++)
   {
      /* r=myrand(300,b);
       * g=myrand(300,r);
       * b=myrand(300,g);
       */
      /* printf("\n%d %d %d\n", r,g,b);*/
      r = i * 20;
      g = i * 20;
      b = i * 10;
      /*if (r<0) r=0;
      if (r > 255) r = 255;
      if (g < 0) g = 0;
      if (g > 255) g = 255;
      if (b < 0) b = 0;
      if (b>255) b=255;*/
      xcl.red = (unsigned short)(r << 8);
      xcl.green = (unsigned short)(g << 8);
      xcl.blue = (unsigned short)(b << 8);
      xcl.flags = DoRed|DoGreen|DoBlue;
      XAllocColor(gb->disp, cm, &xcl);
      ctab[i] = (int)xcl.pixel;
   }
   /* attatch the colormap to the window so it comes into effect */
   XSetWindowColormap(gb->disp, gb->win, cm);
}


void XFSetMandelZero(struct globaldata *g, int w, int h, int *ctab)
{
    printf("XFSetMandelZero\n");
   /*This function sets the entire Mandel array to zero*/
   int x, y; //,pixel;

   x = 0; y = 0;

   for (x = 0; x < (w << 1); x++)
   {
      for (y = 0; y < (h << 1); y++)
        XPutPixel(g->xim, x, y, ctab[45]);
   }
   printf("Moo\n");
   XPutImage(g->disp, g->win, g->gc, g->xim, 0, 0, 0, 0, (w << 1), (h << 1));
   printf("Cow\n");
   XFlush(g->disp);
   printf("Frog\n");

}


void XFProcessMandel(struct globaldata *g, int w, int h, int *ctab)
{
    printf("XFProcessMandel\n");
   /*This function processes entire Mandel array */
   int x, y, reset;
   for (y = ((h << 1) - 1); y >= 2; y--)
   {
      if ((y % IMAGEWRITE) == 0)
      {
	 XPutImage(g->disp, g->win, g->gc, g->xim, 0, 0, 0, 0, (w << 1),(h << 1));
	 XFlush(g->disp);
      }
      reset = TRUE;
      for (x = 1; x < ((w << 1) + 2); x++)
      {
	 XPutPixel(g->xim, x, y, ctab[iterate(reset)]);
	 reset = FALSE;
      }
   }
}

int XMandel(struct globaldata *g,int w, int h, int f, int *ctab)
{
    printf("XMandel\n");
   /*This function is the hub of XMandel.. it initialises the Mandel array, */
   XEvent theEvent;
   XFSetMandelZero(g, w>>1, h>>1, ctab);
   /* now loop forever, generating and displaying Mandels */
   XFProcessMandel(g, w >> 1, h >> 1, ctab);
   XSelectInput(g->disp, g->win, PEV_MASK2);

   while(1)
   {
      XAddPixel(g->xim, 1);
      sleep(0.5);
      XPutImage(g->disp, g->win, g->gc, g->xim, 0, 0, 0, 0, w, h);
      XFlush(g->disp);
      if (XCheckMaskEvent(g->disp, PEV_MASK2, &theEvent))
	break;
   }

   return 1;
}


void DrawRectangle(struct globaldata *g, int x1, int y1, int x2, int y2)
{
   if( x1 < x2 && y1 > y2 )
     XDrawRectangle(g->disp, g->win, g->xorgc, x1, y2, x2-x1, y1-y2);
   else if( x1 < x2 && y1 < y2 )
     XDrawRectangle(g->disp, g->win, g->xorgc, x1, y1, x2-x1, y2-y1);
   else if( x1 > x2 && y1 > y2 )
     XDrawRectangle(g->disp, g->win, g->xorgc, x2, y2, x1-x2, y1-y2);
   else
     XDrawRectangle(g->disp, g->win, g->xorgc, x2, y1, x1-x2, y2-y1);

}

int LetsZoom(struct globaldata *g, int w, int h, int *ctab)
{
   int currentX = 0, currentY = 0;
   int lastX = 0, lastY = 0;
   int drawingOn = NOT_DRAWING_ON;
   double diff, diff2, top2, left2;
   XEvent theEvent;

   XSelectInput(g->disp, g->win, EV_MASK);
   do
   {

      XNextEvent(g->disp, &theEvent);

      switch(theEvent.type)
      {
       case Expose:
	 XClearWindow(g->disp, theEvent.xany.window);
	 break;

       case MapNotify:
	 XClearWindow(g->disp, theEvent.xany.window);
	 break;

       case ButtonPress:
	 if(drawingOn == NOT_DRAWING_ON)
	 {
	    lastX = theEvent.xkey.x;
	    lastY = theEvent.xkey.y;
	    currentX = lastX;
	    currentY = lastY;
	    drawingOn = DRAWING_ON;
	    DrawRectangle(g,lastX, lastY, currentX, currentY);
	 }
	 else
	 {
	    DrawRectangle(g,lastX, lastY, currentX, currentY);

	    currentX = theEvent.xkey.x;
	    currentY = theEvent.xkey.y;
	    DrawRectangle(g,lastX, lastY, currentX, currentY);
	    XFlush(g->disp);
	    diff = (right - left) / w;
	    diff2 = (top - bottom) / h;
	    if( lastX < currentX && lastY > currentY )
	    {
 	       printf("a");
	       left2 = left + (float)lastX * diff;
	       right = left + (float)currentX * diff;
	       left = left2;
	       top2 = top - (float)currentY * diff2;
	       bottom = top - (float)lastY * diff2;
	       top = top2;
	    }
	    else if(lastX < currentX && lastY < currentY)
	    {
	       printf("b");
	       left2 = left + (float)lastX * diff;
	       right = left + (float)currentX * diff;
	       left = left2;
	       top2 = top - (float)lastY * diff2;
	       bottom = top - (float)currentY * diff2;
	       top = top2;
	    }
	    else if( lastX > currentX && lastY > currentY )
	    {
	       printf("c");
	       left2 = left + (float)currentX * diff;
	       right = left + (float)lastX * diff;
	       left = left2;
	       top2 = top - (float)currentY * diff2;
	       bottom = top - (float)lastY * diff2;
	       top = top2;
	    }
	    else
	    {
	       printf("d");
	       left2 = left + (float)currentX * diff;
	       right = left + (float)lastX * diff;
	       left = left2;
	       top2 = top - (float)lastY * diff2;
	       bottom = top - (float)currentY * diff2;
	       top = top2;
	    }
	    fflush(stdout);
	    comp_increment = (top - bottom) / h;
	    real_increment = (right - left) / w;
	    c.real = left;
	    c.imaginary = bottom;
	    XMandel(g, w, h, 0, ctab);
	    XSelectInput(g->disp, g->win, EV_MASK);
	    lastX = currentX;
	    lastY = currentY;

	    drawingOn = NOT_DRAWING_ON;
	 }
	 break;

       case MotionNotify:
	 if(drawingOn == DRAWING_ON )
	 {
	    DrawRectangle(g, lastX, lastY, currentX, currentY);
	    currentX = theEvent.xmotion.x;
	    currentY = theEvent.xmotion.y;
	    DrawRectangle(g, lastX, lastY, currentX, currentY);
	    XFlush(g->disp);
	 }
	 break;

       case KeyPress:
	 /* return( processKeyPress(&theEvent) );*/
	 break;

       case ConfigureNotify:
	 break;
      }
   } while(1);

}

/* Here's the MAIN part of the program */
int main(int argc, char **argv)
{
   struct globaldata glob;
   char disp[256];
   int flags;
   int width, height, i, colortab[MAX];
   /* Set all the variable to default values */
   strcpy(disp, "");
   flags = NONE;
   width = WIDTH;
   height = HEIGHT;

   /* Check command line for arguments */
   if (argc > 1)
   {
      for (i = 1 ;i <= argc; i++)
      {
	 /* if the user requests help */
	 if (!strcmp("-h", argv[i - 1]))
	 {
	    printhelp();
	    exit(0);
	 }
	 /* if the user requests to run in the root window */
	 if (!strcmp("-root", argv[i - 1]))
	 {
	    flags |= ROOT;
	 }
	 /* if the user requests to run on a particular display */
	 if (!strcmp("-display", argv[i - 1]))
	 {
	    if ((i + 1) > argc)
	    {
	       fewargs();
	       exit(1);
	    }
	    strcpy(disp, argv[i]);
	    i++;
	 }
	 /* if the user requests a particular width */
	 if (!strcmp("-width", argv[i - 1]))
	 {
	    if ((i + 1) > argc)
	    {
	       fewargs();
	       exit(1);
	    }
	    width = atoi(argv[i]);
	    if (width < 16)
	      width=16;
	    i++;
	 }
	 /* if the user requests a particular height */
	 if (!strcmp("-height", argv[i-1]))
	 {
	    if ((i + 1) > argc)
	    {
	       fewargs();
	       exit(1);
	    }
	    height = atoi(argv[i]);
	    if (height < 16)
	      height = 16;
	    i++;
	 }
      }
   }
   if (!OpenDisp(&glob, disp))
   {
      printf("Could not open Display %s\n", disp);
      exit(1);
   }
   if (flags & ROOT)
   {
      glob.win = DefaultRootWindow(glob.disp);
      MakeGC(&glob);
   }
   else
   {
      if (!OpenWindow(&glob, width, height))
      {
	 printf("Could not create a GC for the window!\n");
	 exit(1);
      }
   }

   /* Set up the palette for the mandel*/
   SetMandelPalette(&glob, flags, colortab);
   /* Check the dimensions of the window (in case it didn't end up being the */
   /* size we requested) */
   width = GetWinWidth(&glob) + 2;
   height = GetWinHeight(&glob) + 2;
   /* Initialise some important constants */
   initialise(height, width);
   /* Allocate ourselves a buffered image */
   if (!SetupXImage(&glob, width, height))
   {
      printf("Could not allocate memory for image buffer!\n");
      exit(1);
   }
   /* Start displaying the Mandel!*/
   if (!XMandel(&glob, width, height, flags, colortab))
   {
      /* if XMandel returned 0, it encountered an error in allocating memory */
      printf("Not enough memory to allocate to the Mandel arrays\n");
      exit(1);
   }


   LetsZoom(&glob, width, height, colortab);

   return 0;
}



