#ifndef BRESENHAM_H
#define BRESENHAM_H
/**
 * @file    header.h
 * @brief   Bresenham routines for line, circle and ellipse drawing
 *
 * @version 2.0
 * Date:    21/11/2024
 *
 */

#ifndef INT
#define INT     int
#endif

#ifndef LONG
#define LONG    long
#endif


void drawlineb(INT x1, INT y1, INT x2, INT y2);
void drawcircleb(INT xc, INT yc, INT r);
void drawellipseb(INT xc, INT yc, INT rx, INT ry);


#endif// BRESENHAM_H
