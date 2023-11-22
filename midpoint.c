/**
 * @file    midpoint.c
 *
 * @brief   Draw line, circle and ellipse using midpoint algorithm

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
#include "midpoint.h"
#include "mark.h"
///@}

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
        MARKPOINT(x,y);
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
            MARKPOINT(x,y);
        }
        break;
    case OCT1:
        // initial value for decision parameter d
        d = absdx - (absdy/2);
        // Plot initial given point
        MARKPOINT(x,y);
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
            MARKPOINT(x,y);
        }
        break;
    case OCT2:
        // initial value for decision parameter d
        d = absdx - (absdy/2);
        // Plot initial given point
        MARKPOINT(x,y);
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
            MARKPOINT(x,y);
        }
        break;
    case OCT3:
        // initial value for decision parameter d
        d = absdy - (absdx/2);
        // Plot initial given point
        MARKPOINT(x,y);
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
            MARKPOINT(x,y);
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

            if( markdrawmode ) {
                MARKFILL(xc,yc,x,y);
            } else {
                MARKCONTOUROCT(xc,yc,x,y);
}

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
            if( markdrawmode ) {
                MARKFILL(xc,yc,x,y);
            } else {
                MARKCONTOUROCT(xc,yc,x,y);
            }
        // If the generated point is on the line x = y then
        // the perimeter points have already been printed
        if (x != y) {
            if( markdrawmode ) {
                MARKFILL(xc,yc,x,y);
            } else {
                MARKCONTOUROCT(xc,yc,x,y);
            }
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
void drawellipsem(INT xc, INT yc, INT rx, INT ry) {
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
            if( markdrawmode ) {
                MARKFILL(xc,yc,x,y);
            } else {
                MARKCONTOURQUAD(xc,yc,x,y);
            }
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
            if( markdrawmode ) {
                MARKFILL(xc,yc,x,y);
            } else {
                MARKCONTOURQUAD(xc,yc,x,y);
}

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
}
