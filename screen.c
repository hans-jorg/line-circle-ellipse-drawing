/**
 * @file    screen.c
 *
 * @brief   Manages a virtual bitmap

 * @note    Only binary files (PBM)
 *
 * @author  Hans
 *
 * @version 2.0
 *
 * @date    21/11/24
 */

#include <stdlib.h>
#include <stdio.h>
#include "screen.h"
#include "mark.h"


/**
 * @brief Simple Graphics Image routines
 */
///@{
typedef enum { PBM, PGM, PPM } ImageFormatType;

typedef struct ScreenStruct {
    ImageFormatType fmt;        // For now, only PBM
    INT             w;          // width in pixels
    INT             wbytes;     // width in bytes
    INT             h;          // height in pixels
    unsigned char   data[];
} ScreenType;
///@}

/**
 * @brief   Create a Screen
 *
 * @note    It uses only one malloc call. The struct is a header
 */
ScreenType *ScreenCreate(int width, int height) {
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

    ScreenFill(screen,0);

    return screen;
}

/**
 * @brief  ScreenDestroy
 *
 * @note   Long description of function
 *
 * @param  Description of parameter
 *
 * @return Description of return parameters
 */

void ScreenDestroy(ScreenType *screen) {

    // Just in case
    screen->fmt = 0;
    screen->w   = 0;
    screen->h   = 0;
    screen->wbytes = 0;

    free(screen);

}
/**
 * @brief   ScreenFill
 *
 * @note    Fill a screen with value
 */
void ScreenFill(ScreenType *screen, int value) {
int i;

    for(int i=0;i<(screen->wbytes*screen->h);i++)
        screen->data[i] = value;

}


/**
 * @brief   Write a binary image into a file
 *
 * @note    It uses the uncompressed format
 */
void ScreenWritePBM(ScreenType *screen, FILE *fout) {
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
void ScreenDrawPoint(ScreenType *screen, INT x, INT y) {
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
void ScreenDrawVertLine(ScreenType *screen, INT x, INT y1, INT y2) {
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

void ScreenDrawHorizLine(ScreenType *screen, INT x1, INT x2, INT y) {
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
