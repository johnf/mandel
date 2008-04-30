#define NOT_DRAWING_ON  0
#define DRAWING_ON      1
#define EV_MASK (ButtonPressMask     | \
                 KeyPressMask        | \
                 ExposureMask        | \
                 PointerMotionMask   | \
                 StructureNotifyMask)
#define PEV_MASK2 (KeyPressMask)
#define NONE            0x00
#define ROOT            0x01


struct globaldata
{
   /* These entries are all for X */
   Display *disp; /* the display I'm using*/
   Window win; /* the window I'm using */
   Visual *vis; /* the visual I'm using */
   int scr; /* the screen number i'm using */
   GC gc; /* the graphics context for the window */
   GC xorgc;
   XImage *xim; /* the buffered image. */
   unsigned char *im; /* the actual data for the buffered image */
   int ims; /* the size of the image as 2^ims pixels */
   unsigned long wht; /* the color index for white*/
   unsigned long blk; /* the color index for black*/
};

struct complex
{
  double real;
  double imaginary;
  double tempreal;
};

int powerof(unsigned int);
int powerof(unsigned int);
int OpenDisp(struct globaldata *g, char *d);
int MakeGC(struct globaldata *g);
int OpenWindow(struct globaldata *g,int,int);
int GetWinWidth(struct globaldata *g);
int GetWinHeight(struct globaldata *g);
int SetupXImage(struct globaldata *g,int, int);
void fewargs(void);
void printhelp(void);
void initialise(int, int);
int iterate(int);
int myrand(int, int);
void SetMandelPalette(struct globaldata *gb, int,int *ctab);
void XFSetMandelZero(struct globaldata *g, int, int, int *ctab);
void XFProcessMandel(struct globaldata *g, int, int, int *ctab);
int XMandel(struct globaldata *g,int, int, int, int *ctab);
void DrawRectangle(struct globaldata *g,int,int,int,int);
int LetsZoom(struct globaldata *g, int, int, int *ctab);
