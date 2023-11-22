/**
 * @file    mark.c
 *
 * @brief   Routines used by bresenham and midpoint drawing routines
 *
 * @note   They depend on ScreenDrawPoint and MarkHorizLine to draw a point or a horizontal line.
 *
 * @note   The MarkHorizLine is used to fill a circle or a ellipse
 *
 * @author  <Hans>
 *
 * @version 2.0
 *
 * @date    22/11/23
 */

#include <stdlib.h>
#include <stdio.h>
#include "screen.h"
#include "mark.h"


ScreenType *markscreen = 0;
MarkDrawModeType markdrawmode = MARK_CONTOUR;

void (*MarkDrawPoint)(INT,INT) = MarkPoint;
void (*MarkDrawContourQuad)(INT,INT,INT,INT) = MarkBorderPointsQuad;
void (*MarkDrawContourOct)(INT,INT,INT,INT) = MarkBorderPointsOct;
void (*MarkDrawFill)(INT,INT,INT,INT) = MarkHorizFill;


/**
 * @brief  Short description of function
 *
 * @note   Long description of function
 *
 * @param  Description of parameter
 *
 * @return Description of return parameters
 */
void MarkPoint(INT x, INT y) {

    if( !markscreen ) return;

    ScreenDrawPoint(markscreen,x,y);
}


/**
 * @brief   Plot points on markscreen mirroing along the axes
 *
 * @note    <bla bla bla>
 */

void MarkBorderPointsQuad(INT xc, INT yc, INT x, INT y) {

    if( !markscreen ) return;

    ScreenDrawPoint(markscreen,xc+x,yc+y);          // Octant 0
    ScreenDrawPoint(markscreen,xc-x,yc+y);          // Octant 3
    ScreenDrawPoint(markscreen,xc+x,yc-y);          // Octant 7
    ScreenDrawPoint(markscreen,xc-x,yc-y);          // Octant 4

}


/**
 * @brief   Plot points on markscreen mirroing along the axes
 *
 * @note    <bla bla bla>
 */
void MarkBorderPointsOct(INT xc, INT yc, INT x, INT y) {

    if( !markscreen ) return;

    MarkBorderPointsQuad(xc,yc,x,y);
    MarkBorderPointsQuad(xc,yc,y,x);

}


/**
 * @brief   Draw an horizontal line between points
 *
 * @note    <bla bla bla>
 */
void MarkHorizFill(INT xc, INT yc, INT x, INT y) {

    if( !markscreen ) return;

    ScreenDrawHorizLine(markscreen,xc-x,xc+x,yc+y);       // Bottom semicircle
    ScreenDrawHorizLine(markscreen,xc-x,xc+x,yc-y);       // Top semicircle
}
