/**
 * @file    main.c
 *
 * @brief   Test routines

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
#include "screen.h"
#include "midpoint.h"
#include "bresenham.h"
#include "mark.h"

#define WIDTH               300
#define HEIGHT              600


/* Which routines to test: 1 Bresenham, 2:Midpoint */
#define TEST  1


#if TEST == 1
#define drawline            drawlineb
#define drawcircle          drawcircleb
#define drawellipse         drawellipseb
#elif TEST == 2
#define drawline            drawlinem
#define drawcircle          drawcirclem
#define drawellipse         drawellipsem
#else
#error "No test!!!"
#endif


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
INT r = 5;
INT x1 = 10;
INT x2 = 10;
INT x3 = 5;
INT y1 = 5;
INT y2 = 20;
INT y3 = 35;

    markscreen=ScreenCreate(WIDTH,HEIGHT);

    // Teste1
    ScreenDrawPoint(markscreen,xc,yc);
    drawcircle(xc,yc, 100);
    drawcircle(xc+12,yc, 200);
    drawline(x2,y2,x3,y3);
    drawline(x2+5,y2,x2-8,y2+3);
    drawcircle( xc, yc, r+49 );

    fout = fopen("test1.pgm","w");
    ScreenWritePBM(markscreen,fout);

    // Teste 2
    ScreenFill(markscreen,0);
    long rad = 100;
    for(int i=0;i<18;i++) {
        printf("Ponto %d\n",i);
        drawline(xc,yc,xc+ptcircle[i][0]*rad/1000,yc+ptcircle[i][1]*rad/1000);
    }

    fout = fopen("test2.pgm","w");
    ScreenWritePBM(markscreen,fout);

    // Teste 3
    ScreenFill(markscreen,0);
    drawellipse(xc,yc,80,140);
    fout = fopen("test3.pgm","w");
    ScreenWritePBM(markscreen,fout);

    return 0;
}
