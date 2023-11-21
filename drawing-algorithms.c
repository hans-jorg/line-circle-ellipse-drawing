/**
 * @file    line.c
 *
 * @brief   Draw line and circle using midpoint and bresenham algorithms algorithm

 * @note    <Info>
 *
 * @author  Hans
 *
 * @version 0.1
 *
 * @date    26/10/2023
 */

#include <stdlib.h>
#include <stdio.h>

#define INT     int
#define LONG    long

/**
 * @brief Simple Graphics Image routines
 */
///@{
typedef enum { PBM, PGM, PPM } ImageFormatType;

typedef struct {
    ImageFormatType fmt;        // For now, only PBM
    INT             w;          // width in pixels
    INT             wbytes;     // width in bytes
    INT             h;          // height in pixels
    unsigned char   data[];
} ScreenType;
///@}

/**
 * @brief  Global data for Call back
 */
///@{
ScreenType *screen = 0;
///@}


/**
 * @brief   Create a Screen
 *
 * @note    It uses only one malloc call. The struct is a header
 */
ScreenType *CreateScreen(int width, int height) {
ScreenType *screen;
INT widthbytes,sizebytes;

    widthbytes = (width+7)/8;
    sizebytes  = height*widthbytes+32;

    screen = (ScreenType *) malloc(sizeof(ScreenType)+sizebytes);
    if( !screen )
        return 0;

    screen->fmt = PBM;
    screen->w = width;
    screen->h = height;
    screen->wbytes = widthbytes;

    for(int i=0;i<sizebytes;i++)
        screen->data[i] = 0;
    return screen;
}


/**
 * @brief   Write a binary image into a file
 *
 * @note    It uses the uncompressed format
 */
void WritePBM(ScreenType *screen, FILE *fout) {
INT wid;
unsigned char *p;
char ch;

    wid = (screen->w+7)/8;
    fprintf(fout,"P1\n%d\n%d\n",screen->w,screen->h);
    for(int j=0;j<screen->h;j++) {
        p = &(screen->data[j*wid]);
        unsigned char m = '\x80';
        for(int i=0;i<screen->w;i++) {
            if( *p&m ) {
                ch = '1';
            } else {
                ch = '0';
            }
            fputc(ch,fout);
            m >>= 1;
            if( m == 0 ) {
                m = '\x80';
                p++;
            }
        }
        fputc('\n',fout);
    }
}


/**
 * @brief Bit mask for each bit
 *
 * @note  I hope it is faster than  (1<<(7-x))
 */
static const unsigned char mask[] = { '\x80','\x40','\x20','\x10','\x08','\x04','\x02','\x01' };


/**
 * @brief Plot point
 *
 * @note  it will be used as a callback
 */
void MarkPoint(INT x, INT y) {
INT wid;
unsigned char *line;
int col,bit;

    if( !screen ) return;
    if( x < 0 ) return;
    if( x >= screen->w ) return;
    if( y < 0 ) return;
    if( y >= screen->h ) return;

//    wid = (screen->w+7)/8;
    wid = screen->wbytes;
    line = &(screen->data[y*wid]);
    col = x/8;
    bit = x&7;
    line[col] |= mask[bit];
}


/*
 * @brief Plot point
 *
 * @note  it will be used as a callback
 */
void MarkVertLine(INT x, INT y1, INT y2) {
INT wid;
unsigned char *line;
int col,bit;

    if( !screen ) return;
    if( x < 0 ) return;
    if( x >= screen->w ) return;
    if( y1 < 0 ) return;
    if( y1 >= screen->h ) return;
    if( y2 < 0 ) return;
    if( y2 >= screen->h ) return;

//    wid = (screen->w+7)/8;
    wid = screen->wbytes;

    col = x/8;
    bit = x&7;
    for(int y=y1;y<y2;y++) {
        line = &(screen->data[y*wid]);
        line[col] |= mask[bit];
    }
}


/*
 * @brief Draw a horizontal line between points
 *
 * @note  it will be used as a callback
 */

void MarkHorizLine(INT x1, INT x2, INT y) {
INT wid;
unsigned char *line;
int p;
int bm1,bm2;
int p1,p2;

    if( !screen ) return;
    if( x1 < 0 ) return;
    if( x1 >= screen->w ) return;
    if( x2 < 0 ) return;
    if( x2 >= screen->w ) return;
    if( y < 0 ) return;
    if( y >= screen->h ) return;

//    wid = (screen->w+7)/8;
    wid = screen->wbytes;
    line = &(screen->data[y*wid]);

    p1 = x1/8;
    p2 = x2/8;

    bm1 = ((mask[x1&7]-1)<<1)|1;
    line[p1] |= bm1;
    bm2 = 0xFF<<(7-x2&7);
    line[p2] |= bm2;
    if( (p2-p1) > 1 ) {
        for(int p=p1+1;p<p2;p++) {
            line[p] = 0xFF;
        }
    }
}

/**
 * @brief   Plot points on screen mirroing along the axes
 *
 * @note    <bla bla bla>
 */

void MarkBorderPoints(INT xc, INT yc, INT x, INT y) {

    MarkPoint(xc+x,yc+y);          // Octant 0
    MarkPoint(xc-x,yc+y);          // Octant 3
    MarkPoint(xc+x,yc-y);          // Octant 7
    MarkPoint(xc-x,yc-y);          // Octant 4
}

/**
 * @brief   Draw an horizontal line between points
 *
 * @note    <bla bla bla>
 */

void MarkHorizFill(INT xc, INT yc, INT x, INT y) {

    MarkHorizLine(xc-x,xc+x,yc+y);       // Bottom semicircle
    MarkHorizLine(xc-x,xc+x,yc-y);       // Top semicircle
}


/**
 * @brief  Callback for set pixel (mark)
 */

void (*Mark)(INT,INT,INT,INT)  =  MarkHorizFill;



/**
 * @brief Codes for octants
 *
 * @note  Only upper semicircle
 *
 * @note  Code is composed of 3 bits
 *
 *      | Bit |                                      |
 *      |-----|--------------------------------------|
 *      |  0  | 0 if |DX| > |DY| else 1              |
 *      |  1  | 0 if DX >= 0, 1 otherwise            |
 *      |  2  | 0 if DY >= 0, 1 otherwise  (not used |
 *
 *
 * @note  Octant codes
 *
 *    | Octant | DY | DX | DX-DY | Bin | Dec |
 *    |--------|----|----|-------|-----|-----|
 *    |   0    | +  | +  | .+....| 000 |   0 |
 *    |   1    | +  | +  | .-....| 001 |   1 |
 *    |   2    | +  | -  | .-....| 011 |   3 |
 *    |   3    | +  | -  | .+....| 010 |   2 |
 *    |   4    | -  | -  | .+....| 110 |   6 |
 *    |   5    | -  | -  | .-....| 111 |   7 |
 *    |   6    | -  | +  | .-....| 101 |   5 |
 *    |   7    | -  | +  | .+....| 100 |   4 |
 *
 * @note   Gray code!!!
 *
 * @note   Only OCT0 to OCT3 used due to the remapping to upper cycle by inverting
 *         Source and Destination points
 */
///@{
#define   OCT0    0
#define   OCT1    1
#define   OCT2    3
#define   OCT3    2
#define   OCT4    6
#define   OCT5    7
#define   OCT6    5
#define   OCT7    4
///@{

/*
 * @brief  Macro to get absolute value
 *
 * @note   It uses the parameter twice!!! It cause problems in case of size effects
 */
#define ABS(X)  ((X)>0?(X):-(X))


/**
 * @brief   Draw a line using Bresenham algorithm
 */

void drawlineb(INT x1, INT y1, INT x2, INT y2 ) {
int x,y;
int key;
int eps;
INT t;

    INT dx = x2 - x1;
    INT dy = y2 - y1;
    // Build oct value setting bits according octant
    key = 0;
    // Reduce all to positive y semiplane by exchanging p1 and p2
    if( dy < 0 ) {
        t = x1;
        x1 = x2;
        x2 = t;
        t = y1;
        y1 = y2;
        y2 = t;
        dy = -dy;
        dx = -dx;
    }
    // Find quadrant first
    if( dx < 0 ) key |= 2;
    // Find octant using rules according each quadrant
    if( dy > ABS(dx) ) key |= 1;

    // Preparing cycle
    eps = 0;
    x = x1;
    y = y1;

    // Every octant has a different way to cycle and increment.
    // A jump done once is faster than testing and/or using additions instead of increment
    switch(key){
    case OCT0: // 1st octant
        for(x=x1; x<=x2;x++) {
            MarkPoint(x,y);
            eps += dy;
            if( (eps<<1) >= dx ) {
                y++;
                eps -= dx;
            }
        }
        break;
    case OCT1: // 2nd octant
        for(y=y1; y<=y2;y++) {
            MarkPoint(x,y);
            eps += dx;
            if( (eps<<1) >= dy ) {
                x++;
                eps -= dy;
            }
        }
        break;
    case OCT2: // 3rd octant
        for(y=y1; y<=y2;y++) {
            MarkPoint(x,y);
            eps -= dx;
            if( (eps<<1) >= dy ) {
                x--;
                eps -= dy;
            }
        }
        break;
    case OCT3: // 4th octant
        for(x=x1; x>=x2;x--) {
            MarkPoint(x,y);
            eps += dy;
            if( (eps<<1) >= -dx ) {
                y++;
                eps += dx;
            }
        }
        break;
    }
}


/**
 * @brief   Draw a circle using Bresenham algorithm
 */

void drawcircleb(INT xc, INT yc, INT r) {
INT xr,yr;
int e;

    xr = 0;
    yr = r;
    e = 3 - (r+r);
    do {
        // Mirrored
        Mark(xc,yc,xr,yr);
        // Transposed
        Mark(xc,yc,yr,xr);
        if( e < 0 ) {
            e = e + 4*xr + 6;
        } else {
            yr--;
            e = e + 4*(xr-yr) + 10;
        }
        xr++;
    } while( xr <= yr);
}



/**
 * @brief   Draw an ellipse using Bresenham algorithm
 *
 * @note    Ellipse axes are horizontal and vertical
 *
 * @note    Draw in first quadrant and mirror the points to other quadrants
 */
void drawellipseb(INT xc, INT yc, INT rx, INT ry) {
INT x,y;
LONG d;
LONG dx,dy;
LONG rx2,ry2;
LONG rx2_x2,ry2_x2;


    // Precalculate squares and double squares
    rx2 = rx*rx;
    ry2 = ry*ry;
    rx2_x2 = 2*rx2;
    ry2_x2 = 2*ry2;

    x = 0;
    y = ry;
    MarkPoint(xc+x,yc+y);
    // Others??

    // Decision factor
    d = 4*ry2 - 4*rx2*ry + rx2;
    dx = 4*ry2_x2*x;
    dy = 4*rx2_x2*y;

    // Octant 0
    while( dx < dy ) {
        x++;
        dx += 4*ry2_x2;
        if( d < 0 ) {
            d += dx + 4*ry2;
        } else {
            y--;
            dy -= 4*rx2_x2;
            d += dx - dy + 4*ry2;
        }
        Mark(xc,yc,x,y);
    }

    // Decision factor
    d = ry2*(2*x+1)*(2*x+1) + rx2*(2*y-2)*(2*y-2)-4*rx2*ry2;
    // Octant 1
    while( y>0 ) {
        y--;
        dy -= 4*rx2_x2;
        if( d > 0 ) {
            d -= dy - 4*rx2;
        } else {
            x++;
            dx += ry2_x2;
            d += dx - dy + 4*rx2;
        }
        Mark(xc,yc,x,y);
    }
}


 /**
  * @brief   draw a line  using midpoint algorithm
  */

void drawlinem(INT x1, INT y1, INT x2, INT y2 ) {
INT d;
INT t;
INT incy = 1;
int key = 0;

    // Use only upper semicircle (dy will be always positive)
    if( y2 < y1 ) {
        t = x1;
        x1 = x2;
        x2 = t;
        t = y1;
        y1 = y2;
        y2 = t;
    }

    // Calculate X and Y differences
    INT dx = x2 - x1;
    INT dy = y2 - y1;
    INT absdx = ABS(dx);
    INT absdy = ABS(dy);

    // Setting octant code
    if( absdy > absdx ) key |= 1;
    if( /*x2 < x1*/ dx < 0  ) key |= 2;

    // Start point
    INT x = x1;
    INT y = y1;

    // Jump to the corresponding octant processing
    switch(key) {
    case OCT0:
        // initial value for decision parameter d
        d = absdy - (absdx/2);
        // Plot initial given point
        MarkPoint(x,y);
        // iterate through value of X, since dx > dy, it is incremented at every step
        while (x < x2) {
            x++;
            if (d < 0) {
                d += absdy;
            } else {
                d += (absdy - absdx);
                y++;
            }
            // Plot intermediate points
            MarkPoint(x,y);
        }
        break;
    case OCT1:
        // initial value for decision parameter d
        d = absdx - (absdy/2);
        // Plot initial given point
        MarkPoint(x,y);
        // iterate through value of y since dy > dx, y is incremented at every step
        while (y < y2) {
            y++;
            if (d < 0) {
                d += absdx;
            } else {
                d += (absdx - absdy);
                x++;
            }
            // Plot intermediate points
            MarkPoint(x,y);
        }
        break;
    case OCT2:
        // initial value for decision parameter d
        d = absdx - (absdy/2);
        // Plot initial given point
        MarkPoint(x,y);
        // iterate through value of y since dy > dx, y is incremented at every step
        while (y < y2) {
            y++;
            if (d < 0) {
                d += absdx;
            } else {
                d += (absdx - absdy);
                x--;
            }
            // Plot intermediate points
            MarkPoint(x,y);
        }
        break;
    case OCT3:
        // initial value for decision parameter d
        d = absdy - (absdx/2);
        // Plot initial given point
        MarkPoint(x,y);
        // iterate through value of X, since dx > dy, it is incremented at every step
        while (x > x2) {
            x--;
            if (d < 0) {
                d += absdy;
            } else {
                d += (absdy - absdx);
                y++;
            }
            // Plot intermediate points
            MarkPoint(x,y);
        }
        break;
    }
}


/**
 * @brief  Draw circle using midpoint algorithm
 *
 */
void drawcirclem(INT xc, INT yc, INT r) {

    INT x = r;
    INT y = 0;

    Mark(xc,yc,x,y);

    INT P = 1 - r;
    while (x > y) {
        y++;
        // Mid-point is inside or on the perimeter
        if (P <= 0) {
            P = P + 2*y + 1;
        // Mid-point is outside the perimeter
        } else {
            x--;
            P = P + 2*y - 2*x + 1;
        }

        // All the perimeter points have already been printed
        if (x < y)
            break;

        // Printing the generated point and its reflection
        // in the other octants after translation
        Mark(xc,yc,x,y);

        // If the generated point is on the line x = y then
        // the perimeter points have already been printed
        if (x != y) {
            Mark(xc,yc,y,x);
        }
    }
}


/**
 * @brief   Draw an ellipse using midpoint algorithm
 *
 * @note    Ellipse axes are horizontal and vertical
 *
 * @note    Draw in first quadrant and mirror the points to other quadrants
 */
int drawellipsem(INT xc, INT yc, INT rx, INT ry) {
INT x,y;
LONG d1,d2;
LONG dx,dy;
LONG rx2,ry2;

    //
    x = 0;
    y = ry;

    // Precalculate squares
    rx2 = rx*rx;
    ry2 = ry*ry;
    // Decision factor
    // d1 = ry^2 - rx^2*ry - rx^2/4);
    // d1, dx and dy are multplied by 4 to avoid floating point
    d1 = 4*ry2 - 4*rx2*ry + rx2;
    dx = 8*ry2*x;
    dy = 8*rx2*y;

    // Octant 0
    while( dx < dy ) {
        Mark(xc,yc,x,y);

        if( d1 < 0 ) {
            x++;
            dx += 8*ry2;
            d1 += dx + 4*ry2;
        } else {
            x++;
            y--;
            dx += 8*ry2;
            dy -= 8*rx2;
            d1 += dx - dy + 4*ry2;
        }
    }

    d2 = ry2*(2*x+1)*(2*x+1) + rx2*(2*y-2)*(2*y-2) - 4*rx2*ry2;
    // Octant 1
    while( y >= 0 ) {
        Mark(xc,yc,x,y);

        if( d2 > 0 ) {
            y--;
            dy -= 8*rx2;
            d2 += 4*rx2 - dy;
        } else {
            y--;
            x++;
            dx += 8*ry2;
            dy -= 8*rx2;
            d2 += dx - dy - 4*rx2;
        }
    }
    return 0;
}


#if 1

#define WIDTH               300
#define HEIGHT              600


const int ptcircle[18][2] = {
     {  1000,      0   },
     {   940,    342   },
     {   766,    643   },
     {   500,    866   },
     {   174,    985   },
     {  -174,    985   },
     {  -500,    866   },
     {  -766,    643   },
     {  -940,    342   },
     { -1000,      0   },
     {  -940,   -342   },
     {  -766,   -643   },
     {  -500,   -866   },
     {  -174,   -985   },
     {   174,   -985   },
     {   500,   -866   },
     {   766,   -643   },
     {   940,   -342   },
};

int main (int argc, char *argv[])  {
INT xc = WIDTH/2;
INT yc = WIDTH/2;
FILE *fout = stdout;
/*INT r = 5;*/
/*INT x2 = 10;*/
/*INT y2 = 20;*/
/*INT x3 = 5;*/
/*INT y3 = 35;*/


    if( argc > 1 ) {
        fout = fopen(argv[1],"w");
        if( !fout )
            fout = stdin;
    }

    screen=CreateScreen(WIDTH,HEIGHT);


#ifdef TEST_LINE
    mark(p1,screen);
    drawcirclem(x1,y1, r);
    drawcirclem(x1+12,y1, r);
    drawlineb(x2,y2,x3,y3);
    drawlinem(x2+5,y2,x2-8,y2+3);
    drawcircle( p1, r+49, screen);
#endif

#ifdef TEST_CIRCLE
    long r = 500;
    for(int i=0;i<18;i++) {
        printf("Ponto %d\n",i);
        drawlinem(x1,y1,x1+ptcircle[i][0]*r/1000,y1+ptcircle[i][1]*r/1000);
    }
#endif

#ifdef TEST_ELLIPSE
    drawellipseb(x1,y1,80,160);
#endif



    WritePBM(screen,fout);

    return 0;
}

#endif
