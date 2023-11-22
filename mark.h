#ifndef MARK_H
#define MARK_H
/**
 * @file  mark.h
 * @brief Output routines for drawing algorithms

 * @version 1.0.0
 * Date:    21/11/24
 *
 */

#ifndef INT
#define INT                 int
#endif

#ifndef LONG
#define LONG                long
#endif

#include "screen.h"
/**
 * @brief  Callback for set pixel (mark)
 *
 * @note   Macros are used to avoid the call thru a NULL point
 */
///@{
extern void (*MarkDrawPoint)(INT,INT);
extern void (*MarkDrawContourQuad)(INT,INT,INT,INT);
extern void (*MarkDrawContourOct)(INT,INT,INT,INT);
extern void (*MarkDrawFill)(INT,INT,INT,INT);


typedef enum { MARK_CONTOUR, MARK_FILL } MarkDrawModeType;

extern ScreenType *markscreen;
extern MarkDrawModeType markdrawmode;

#define MARKPOINT(X,Y)          do { \
                                    if (MarkDrawPoint) \
                                        MarkDrawPoint((X),(Y)); \
                                } while(0)

#define MARKFILL(X1,Y1,X2,Y2)   do { \
                                   MarkHorizFill(X1,Y1,X2,Y2); \
                                 } while(0)

#define MARKCONTOURQUAD(X1,Y1,X2,Y2) do { \
                                    MarkBorderPointsQuad(X1,Y1,X2,Y2); \
                                    } while (0);

#define MARKCONTOUROCT(X1,Y1,X2,Y2) do { \
                                    MarkBorderPointsOct(X1,Y1,X2,Y2); \
                                    } while (0);

///@}

extern void MarkBorderPointsQuad(INT xc, INT yc, INT x, INT y);
extern void MarkBorderPointsOct(INT xc, INT yc, INT x, INT y);
extern void MarkHorizFill(INT xc, INT yc, INT x, INT y);
extern void MarkPoint(INT x, INT y);
#endif // MARK_H
