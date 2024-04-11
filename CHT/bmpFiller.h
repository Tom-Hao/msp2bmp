#ifndef BMPFILLER
#define BMPFILLER
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int bmpFiller(FILE* fp, unsigned char** bmpData, int width, int height, int lineBytes);

#endif