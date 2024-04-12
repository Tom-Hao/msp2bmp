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
    int windowsVersion = 0; //Windowsのバージョン
    int width; //MSPファイルの幅
    int height; //MSPファイルの高さ
    int ok = 1; //変換完了するか否かを確認

    int bmpSize; //BMPファイルのサイズ
    int lineBytes; //BMP一行のバイト数

        int i, j, a;

    unsigned char** bmpData; //BMPファイル内容
    unsigned char** mspData; //MSPファイル内容

    FILE* fp_bmp; //BMPファイル
    FILE* fp_msp; //MSPファイル
    MSPFILEHEADER mspFileHeader; //MSPファイルの頭

    printf("MSP2BMP 0.5.132 BETA版\nCopyright (C) 2023-2024 Tom Hao 製作\n");

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("此のプログラムは、Windows 1.xと2.xのMS-PaintのMSP画像をBMP画像に変換出来ます。\n 'msp2bmp mspfilename bmpfilename'と言う命令で、変換開始します。\n");
            return 0;
        }
        else
        {
            printf("パラメータは必要です、例：'msp2bmp test.msp test.bmp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("パラメータは必要です、例：'msp2bmp test.msp test.bmp'\n");
        return -1;

    }

    printf("此のMSPファイルは %s です。\n", argv[1]);

    if((fp_msp = fopen(argv[1], "rb")) == NULL)
    {
        printf("エラーの為MSPファイル読み込めません。\n");
        return -1;
    }

    if ((fp_bmp = fopen(argv[2], "wb+")) == NULL)
    {
        return -1;
    }

    fseek(fp_msp, 0, SEEK_SET);

    fread(&mspFileHeader, 32, 1, fp_msp);

    //showMspFileHead(mspFileHeader);

    //Windowsバージョンの確認
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
        windowsVersion = 0; //間違いファイル又ファイル破損
    }

    if(windowsVersion == 1 || windowsVersion == 2)
    {
        if(windowsVersion == 2)
        {
            printf("今此のプログラムは暫くWindows 2.xのMS-PaintのMSPファイルを処理出来ません。\n");
            return -1;
        }
        else
        {
            if(mspFileHeader.mfWidth == 0x0140 || mspFileHeader.mfWidth == 0x0280) //MSP画像の幅を確認、唯三百二十・六百四十画素幅だけは変換出来ます。
            {
                if(mspFileHeader.mfHeight == 0x00C8 || mspFileHeader.mfHeight == 0x000F || mspFileHeader.mfHeight == 0x0190 || mspFileHeader.mfHeight == 0x01E0)  //MSP画像の高さを確認、唯二百・二百四十・四百・四百八十画素の高さだけは変換出来ます。
                {
                    width = mspFileHeader.mfWidth;
                    height = mspFileHeader.mfHeight;
                    fseek(fp_msp, 32, SEEK_SET); //MSP画像内容定位
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
                    printf("変換開始。\n");
                    mspTurn(mspData, bmpData, width, height);
                    printf("書き込み開始。\n");
                    ok = bmpFiller(fp_bmp, bmpData, width, height, lineBytes);

                    if(ok == 0) //成功すれば
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("BMPファイル作成完了！\n");
                        return 0;
                    }
                    else //失敗すれば
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("BMPファイル作成失敗！\n");
                        return 0;
                    }
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("此の高さを処理出来ません。\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("此の幅を処理出来ません。\n");
                return 0;
            }
        }
    }
    else
    {
        printf("間違いファイル又はファイル破損ですので、変換出来ません。\n");
        return -1;
    }
}

