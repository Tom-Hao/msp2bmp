#define _CRT_SECURE_NO_WARNINGS

#include "mspTurn.h"

void mspTurn(unsigned char** mspData, unsigned char** bmpData, int width, int height)
{
    int i, j, k;
    int lineBytes = (1 * width) / 8;

    for(i = 0; i < height; i++) //s
    {
        for(j = 0; j < lineBytes; j++) //—ñ
        {
            bmpData[i][j] = mspData[height - 1 - i][j];
        }
    }
}