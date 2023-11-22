/**
 * @file    bresenham.c
 *
 * @brief   Draw line, circle and ellipse using bresenham algorithm
 *
 * @note   Integer types used are specified by preprocessor symbols INT and LONG
 *         to enhance portability
 *
 * @note   LONG is only used for ellipses
 *
 *
 * @author  Hans
 *
 * @version 2.0
 *
 * @date    21/11/2023
 */

#include <stdlib.h>
#include <stdio.h>
#include "bresenham.h"
#include "mark.h"


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
///@}

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
            MARKPOINT(x,y);
            eps += dy;
            if( (eps<<1) >= dx ) {
                y++;
                eps -= dx;
            }
        }
        break;
    case OCT1: // 2nd octant
        for(y=y1; y<=y2;y++) {
            MARKPOINT(x,y);
            eps += dx;
            if( (eps<<1) >= dy ) {
                x++;
                eps -= dy;
            }
        }
        break;
    case OCT2: // 3rd octant
        for(y=y1; y<=y2;y++) {
            MARKPOINT(x,y);
            eps -= dx;
            if( (eps<<1) >= dy ) {
                x--;
                eps -= dy;
            }
        }
        break;
    case OCT3: // 4th octant
        for(x=x1; x>=x2;x--) {
            MARKPOINT(x,y);
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
        if( markdrawmode==MARK_FILL ) {
              MARKFILL(xc,yc,xr,yr);
        } else {
              MARKCONTOUROCT(xc,yc,xr,yr);
        }
        // Transposed
        if( markdrawmode==MARK_FILL ) {
              MARKFILL(xc,yc,xr,yr);
        } else {
              MARKCONTOUROCT(xc,yc,xr,yr);
        }
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
    MARKCONTOURQUAD(xc,yc,x,y);
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
        if( markdrawmode==MARK_FILL ) {
            MARKFILL(xc,yc,x,y);
        } else {
            MARKCONTOURQUAD(xc,yc,x,y);
        }
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
        if( markdrawmode==MARK_FILL ) {
             MARKFILL(xc,yc,x,y);
        } else {
             MARKCONTOURQUAD(xc,yc,x,y);
        }
    }
}
