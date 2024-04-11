#define _CRT_SECURE_NO_WARNINGS

#include "bmpFiller.h"

int bmpFiller(FILE* fp, unsigned char** bmpData, int width, int height, int lineBytes)
{
    int i, j;
    short bmpHead[31] = {0x00};

    //BMP檔案標頭
    /*
    WORD bfType;
    WORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    WORD bfOffBits;

    WORD  biInfoSize;
    WORD  biWidth;
    WORD  biHeigth;
    WORD  biPlanes;
    WORD  biBitCount;
    WORD  biCompression;
    WORD  biSizeImage;
    WORD  biXPelsPerMeter;
    WORD  biYPelsPerMeter;
    WORD  biClrUsed;
    WORD  biClrImportant;

    */

    //檔案標頭填充
    //bfType
    bmpHead[0] = 0x4d42;
    //bfSize
    if(width == 320)
    {
        if(height == 200)
        {
            bmpHead[1] = 0x1f7e;
            bmpHead[2] = 0x0000;
        }
        else if(height = 240)
        {
            bmpHead[1] = 0x25be;
            bmpHead[2] = 0x0000;
        }
    }
    else if(width == 640)
    {
        if(height == 400)
        {
            bmpHead[1] = 0x7d3e;
            bmpHead[2] = 0x0000;
        }
        else if(height = 480)
        {
            bmpHead[1] = 0x963e;
            bmpHead[2] = 0x0000;
        }
    }

    //bfReserved1
    bmpHead[3] = 0x0000;

    //bfReserved2
    bmpHead[4] = 0x0000;

    //bfOffBits
    bmpHead[5] = 0x003e;
    bmpHead[6] = 0x0000;

    //biInfoSize
    bmpHead[7] = 0x0028;
    bmpHead[8] = 0x0000;

    //biWidth
    if(width == 320)
    {
        bmpHead[9] = 0x0140;
        bmpHead[10] = 0x0000;
    }
    else if(width == 640)
    {
        bmpHead[9] = 0x0280;
        bmpHead[10] = 0x0000;
    }

    //biHeight
    if(height == 400)
    {
        bmpHead[11] = 0x0190;
        bmpHead[12] = 0x0000;
    }
    else if(height == 480)
    {
        bmpHead[11] = 0x01e0;
        bmpHead[12] = 0x0000;
    }
    else if(height == 200)
    {
        bmpHead[11] = 0x00c8;
        bmpHead[12] = 0x0000;
    }
    else if(height == 240)
    {
        bmpHead[11] = 0x00f0;
        bmpHead[12] = 0x0000;
    }

    //biPlanes
    bmpHead[13] = 0x0001;

    //biBitCount
    bmpHead[14] = 0x0001;

    //biCompression
    bmpHead[15] = 0x0000;
    bmpHead[16] = 0x0000;

    //biSizeImage
    if(width == 320)
    {
        if(height == 200)
        {
            bmpHead[17] = 0x1f40;
            bmpHead[18] = 0x0000;
        }
        else if(height = 240)
        {
            bmpHead[17] = 0x2580;
            bmpHead[18] = 0x0000;
        }
    }
    else if(width == 640)
    {
        if(height == 400)
        {
            bmpHead[17] = 0x7d00;
            bmpHead[18] = 0x0000;
        }
        else if(height = 480)
        {
            bmpHead[17] = 0x9600;
            bmpHead[18] = 0x0000;
        }
    }

    //biXPelsPerMeter
    bmpHead[19] = 0x0000;
    bmpHead[20] = 0x0000;

    //biYPelsPerMeter
    bmpHead[21] = 0x0000;
    bmpHead[22] = 0x0000;

    //biClrUsed
    bmpHead[23] = 0x0000;
    bmpHead[24] = 0x0000;

    //biClrImportant
    bmpHead[25] = 0x0000;
    bmpHead[26] = 0x0000;

    //空白位
    bmpHead[27] = 0x0000;
    bmpHead[28] = 0x0000;

    //顏色
    bmpHead[29] = 0xffff;
    bmpHead[30] = 0x00ff;

    fwrite(bmpHead, 2, 31, fp);

    for(i = 0; i < height; i++)
    {
        for(j = 0; j < lineBytes; j++)
        {
            fwrite(&bmpData[i][j], sizeof(unsigned char), 1, fp);
        }
    }
    
    //fclose(fp);
    return 0;
}