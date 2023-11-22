#ifndef SCREEN_H
#define SCREEN_H

/**
 * @file header.h
 * @brief Simple Header

 * @version 1.0.0
 * Date:    7 October 2023
 *
 */


#ifndef INT
#define INT                 int
#endif

#ifndef LONG
#define LONG                long
#endif

typedef struct ScreenStruct ScreenType;

ScreenType *ScreenCreate(int width, int height);
void ScreenDestroy(ScreenType *screen);
void ScreenFill(ScreenType *screen, int value);
void ScreenWritePBM(ScreenType *screen, FILE *fout);
void ScreenDrawPoint(ScreenType *screen, int x, int y);
void ScreenDrawVertLine(ScreenType *screen, int x, int y1, int y2);
void ScreenDrawHorizLine(ScreenType *screen,int x1, int x2, int y);

#endif // SCREEN_H
