#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "msp_head.h"
#include "mspHeadReader.h"
#include "bmpFiller.h"
#include "mspTurn.h"


int main(int argc, char* argv[])
{
    int windowsVersion = 0; //Windows版本
    int width; //MSP文件宽度
    int height; //MSP文件高度
    int ok = 1; //转换完成确认

    int bmpSize; //BMP文件内容
    int lineBytes; //BMP一行字节数

        int i, j, a;

    unsigned char** bmpData; //BMP文件内容
    unsigned char** mspData; //MSP文件内容

    FILE* fp_bmp; //BMP文件
    FILE* fp_msp; //MSP文件
    MSPFILEHEADER mspFileHeader; //MSP文件头

    printf("MSP2BMP 0.5.132 BETA版\n版权所有 (C) 2023-2024 中国软体开发组 Tom Hao 制作\n");

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("这个程序可以将Windows 1.x和2.x的MS-Paint的MSP格式图像转换为BMP格式图像。\n 使用'msp2bmp mspfilename bmpfilename'命令即可开始转换。\n");
            return 0;
        }
        else
        {
            printf("需要指定参数，例：'msp2bmp test.msp test.bmp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("需要指定参数，例：'msp2bmp test.msp test.bmp'\n");
        return -1;

    }

    printf("输入的MSP文件是 %s 。\n", argv[1]);

    if((fp_msp = fopen(argv[1], "rb")) == NULL)
    {
        printf("出现错误，MSP文件读取失败。\n");
        return -1;
    }

    if ((fp_bmp = fopen(argv[2], "wb+")) == NULL)
    {
        return -1;
    }

    fseek(fp_msp, 0, SEEK_SET);

    fread(&mspFileHeader, 32, 1, fp_msp);

    //showMspFileHead(mspFileHeader);

    //Windows版本确认
    if(mspFileHeader.mfKey1 == 0x6144 && mspFileHeader.mfKey2 == 0x4d6e)
    {
        windowsVersion = 1; //Windows 1.x
    }
    else if(mspFileHeader.mfKey1 == 0x694c && mspFileHeader.mfKey2 == 0x536e)
    {
        windowsVersion = 2; //Windows 2.x
    }
    else
    {
        windowsVersion = 0; //文件错误或文件损坏
    }

    if(windowsVersion == 1 || windowsVersion == 2)
    {
        if(windowsVersion == 2)
        {
            printf("目前这个程序暂时无法处理Windows 2.x的MS-Paint的MSP文件。\n");
            return -1;
        }
        else
        {
            if(mspFileHeader.mfWidth == 0x0140 || mspFileHeader.mfWidth == 0x0280) //确认MSP图像的宽度，只能转换三百二十、六百四十像素的图片。
            {
                if(mspFileHeader.mfHeight == 0x00C8 || mspFileHeader.mfHeight == 0x000F || mspFileHeader.mfHeight == 0x0190 || mspFileHeader.mfHeight == 0x01E0)  //确认MSP图像的高度，只能转换二百、二百四十、四百、四百八十像素的图片。
                {
                    width = mspFileHeader.mfWidth;
                    height = mspFileHeader.mfHeight;
                    fseek(fp_msp, 32, SEEK_SET); //MSP图像内容定位
                    lineBytes = (1 * width) / 8;
                    mspData = (unsigned char**)malloc(sizeof(unsigned char*) * height);
                    for(i = 0; i < height; i++)
                    {
                        mspData[i] = (unsigned char*)malloc(sizeof(unsigned char) * lineBytes);
                    }

                    bmpData = (unsigned char**)malloc(sizeof(unsigned char*) * height);
                    for(i = 0; i < height; i++)
                    {
                        bmpData[i] = (unsigned char*)malloc(sizeof(unsigned char) * lineBytes);
                    }
                    for(i = 0; i < height; i++)
                    {
                        for(j = 0; j < lineBytes; j++)
                        {
                            fread(&mspData[i][j], sizeof(unsigned char), 1, fp_msp);
                        }
                    }
                    printf("转换开始。\n");
                    mspTurn(mspData, bmpData, width, height);
                    printf("开始写入。\n");
                    ok = bmpFiller(fp_bmp, bmpData, width, height, lineBytes);

                    if(ok == 0) //若成功
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("BMP文件创建完成！\n");
                        return 0;
                    }
                    else //若失败
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("BMP文件创建失败！\n");
                        return 0;
                    }
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("无法处理这种高度的图片。\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("无法处理这种宽度的图片。\n");
                return 0;
            }
        }
    }
    else
    {
        printf("因为文件错误或文件损坏，无法转换。\n");
        return -1;
    }
}

