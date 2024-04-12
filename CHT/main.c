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
    int width; //MSP檔案寬度
    int height; //MSP檔案高度
    int ok = 1; //轉換完成確認

    int bmpSize; //BMP文件?容
    int lineBytes; //BMP一行字??

        int i, j, a;

    unsigned char** bmpData; //BMP檔案內容
    unsigned char** mspData; //MSP檔案內容

    FILE* fp_bmp; //BMP檔案
    FILE* fp_msp; //MSP檔案
    MSPFILEHEADER mspFileHeader; //MSP檔案標頭

    printf("MSP2BMP 0.5.132 BETA版\n版權所有 (C) 2023-2024 Tom Hao 製作\n");

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("這個程式可以將Windows 1.x和2.x的MS-Paint的MSP規格之圖像轉換為BMP規格之圖像。\n 使用'msp2bmp mspfilename bmpfilename'命令即可開始轉換。\n");
            return 0;
        }
        else
        {
            printf("需要指定參數，例：'msp2bmp test.msp test.bmp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("需要指定參數，例：'msp2bmp test.msp test.bmp'\n");
        return -1;

    }

    printf("輸入之MSP檔案是 %s 。\n", argv[1]);

    if((fp_msp = fopen(argv[1], "rb")) == NULL)
    {
        printf("出現錯誤，MSP檔案讀取失敗。\n");
        return -1;
    }

    if ((fp_bmp = fopen(argv[2], "wb+")) == NULL)
    {
        return -1;
    }

    fseek(fp_msp, 0, SEEK_SET);

    fread(&mspFileHeader, 32, 1, fp_msp);

    //showMspFileHead(mspFileHeader);

    //Windows版本確認
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
        windowsVersion = 0; //檔案錯誤或檔案損壞
    }

    if(windowsVersion == 1 || windowsVersion == 2)
    {
        if(windowsVersion == 2)
        {
            printf("目前這個程式暫時無法處理Windows 2.x的MS-Paint的MSP檔案。\n");
            return -1;
        }
        else
        {
            if(mspFileHeader.mfWidth == 0x0140 || mspFileHeader.mfWidth == 0x0280) //確認MSP圖像的寬度，只能轉換三百二十／六百四十畫素之圖像。
            {
                if(mspFileHeader.mfHeight == 0x00C8 || mspFileHeader.mfHeight == 0x000F || mspFileHeader.mfHeight == 0x0190 || mspFileHeader.mfHeight == 0x01E0)  //確認MSP圖像的高度，只能轉換二百／二百四十／四百／四百八十畫素之圖像。
                {
                    width = mspFileHeader.mfWidth;
                    height = mspFileHeader.mfHeight;
                    fseek(fp_msp, 32, SEEK_SET); //MSP圖像內容定位
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
                    printf("轉換開始。\n");
                    mspTurn(mspData, bmpData, width, height);
                    printf("開始寫入。\n");
                    ok = bmpFiller(fp_bmp, bmpData, width, height, lineBytes);

                    if(ok == 0) //若成功
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("BMP檔案創建完成！\n");
                        return 0;
                    }
                    else //若失敗
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("BMP檔案創建失敗！\n");
                        return 0;
                    }
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("無法處理這種高度的圖片。\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("無法處理這種寬度的圖片。\n");
                return 0;
            }
        }
    }
    else
    {
        printf("因為檔案錯誤或檔案損壞，無法轉換。\n");
        return -1;
    }
}

