#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <time.h>
#include "mand.h"

extern double compinc, realinc;
extern double r, c, right, left, top, bottom;
extern int iter, screen, start_color, color_seperation, iterations;

#define RES 8

int powerof(unsigned int n)
{
  /* This returns the power of a number (eg powerof(8)==3, powerof(256)==8, */
  /* powerof(1367)==11, powerof(2568)==12) */
  int p=32;
  if (n <= 0x80000000) p=31;
  if (n <= 0x40000000) p=30;
  if (n <= 0x20000000) p=29;
  if (n <= 0x10000000) p=28;
  if (n <= 0x08000000) p=27;
  if (n <= 0x04000000) p=26;
  if (n <= 0x02000000) p=25;
  if (n <= 0x01000000) p=24;
  if (n <= 0x00800000) p=23;
  if (n <= 0x00400000) p=22;
  if (n <= 0x00200000) p=21;
  if (n <= 0x00100000) p=20;
  if (n <= 0x00080000) p=19;
  if (n <= 0x00040000) p=18;
  if (n <= 0x00020000) p=17;
  if (n <= 0x00010000) p=16;
  if (n <= 0x00008000) p=15;
  if (n <= 0x00004000) p=14;
  if (n <= 0x00002000) p=13;
  if (n <= 0x00001000) p=12;
  if (n <= 0x00000800) p=11;
  if (n <= 0x00000400) p=10;
  if (n <= 0x00000200) p=9;
  if (n <= 0x00000100) p=8;
  if (n <= 0x00000080) p=7;
  if (n <= 0x00000040) p=6;
  if (n <= 0x00000020) p=5;
  if (n <= 0x00000010) p=4;
  if (n <= 0x00000008) p=3;
  if (n <= 0x00000004) p=2;
  if (n <= 0x00000002) p=1;
  if (n <= 0x00000001) p=0;
  return p;
}


int OpenDisp(struct globaldata *g, char *d)
{
  /*This routine opens up a connection to the X display d */
  Display *disp;

  disp = XOpenDisplay(d);
  if (disp == NULL) {
    /* If there was a problem in opening the display, return 0 */
    return 0;
  }
  else {
    /* We opened the display sucessfully, now get a few parameters of the */
    /* display */
    g->disp=disp;
    /* Get the defalt screen number */
    g->scr=DefaultScreen(disp);
    /* Get the color index of the black pixel */
    g->blk=BlackPixel(disp, g->scr);
    /* Get the color index of the white pixel */
    g->wht=WhitePixel(disp, g->scr);
    /* Get the Visual for the display */
    g->vis=DefaultVisual(disp, g->scr);
    return 1;
  }
}

int MakeGC(struct globaldata *g)
{
  /*This function Creates a GraphicsContext for a Window */
  XGCValues gcvals;
  GC gc, xorgc;

  /* Create the GC */
  gc=XCreateGC(g->disp, g->win, (unsigned long)0, &gcvals);
  if (gc) {
    /* If we got our GC, set then fg & bg and return 1) */
    XSetForeground(g->disp, gc, g->wht);
    XSetBackground(g->disp, gc, g->blk);
    g->gc=gc;
  }
  else {
    /* if we couldn't get our gc, return 0 */
    return 0;
  }

  xorgc=XCreateGC(g->disp, g->win, (unsigned long)0, &gcvals);
  if (xorgc) {
    /* If we got our GC, set then fg & bg and return 1) */
    XSetForeground(g->disp, xorgc, g->wht);
    XSetBackground(g->disp, xorgc, g->blk);
    XSetFunction(g->disp, xorgc, GXxor);
    g->xorgc=xorgc;
    return 1;
  }
  else {
    /* if we couldn't get our gc, return 0 */
    return 0;
  }
}

int OpenWindow(struct globaldata *g, int w, int h)
{
  /*This function opens up a window of width w and height h*/
  g->win=XCreateSimpleWindow(g->disp, RootWindow(g->disp, g->scr), 10, 10, w, h, 5, g->blk, g->blk);
  if (!MakeGC(g)) {
    XDestroyWindow(g->disp, g->win);
    return 0;
  }
  XMapWindow(g->disp, g->win);
  XFlush(g->disp);
  return 1;
}

int GetWinWidth(struct globaldata *g)
{
  /*This function is returns the width of thw window */
  XWindowAttributes wa;

  XGetWindowAttributes(g->disp, g->win, &wa);

  return wa.width;
}

int GetWinHeight(struct globaldata *g)
{
  /*This function is returns the height of the window */
  XWindowAttributes wa;

  XGetWindowAttributes(g->disp, g->win, &wa);

  return wa.height;
}


int SetupXImage(struct globaldata *g, int w, int h)
{
  /*This function sets up the buffered image on which all drawing is done */
  /*and when completed, is copied (in interlaced fashion for better results */
  /*or even using 16x16 square dynamic delta techniques to increase update */
  /*speed on VERY large screens, like the root window, if the user so says) */
  /*to the window, to speedup drawing, and give us Double Buffering */
  int hbits, hs;
  int imsize;
  unsigned char *im;

  hbits=powerof(w);
  hs=(1<<hbits);
  /* calculate the memory needed for the image data */
  imsize=hs*h*sizeof(char);
  /* allocate the memory for the image data */
  im=(char *)malloc(imsize);
  /* if we couldn't allocate the memory, return 0 */
  if (!im) return 0;
  /* create the XImage from the data */
  g->xim=XCreateImage(g->disp, g->vis, RES, ZPixmap, 0, im, hs, h, 32, 0);
  printf("\n%d\n", XBitmapPad(g->disp));
  fflush(stdout);
  g->im=im;
  g->ims=hbits;
  XMapWindow(g->disp, g->win);
  XFlush(g->disp);
  return 1;
}
