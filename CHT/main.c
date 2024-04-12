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
    int windowsVersion = 0; //Windows����
    int width; //MSP�ɮ׼e��
    int height; //MSP�ɮװ���
    int ok = 1; //�ഫ�����T�{

    int bmpSize; //BMP���?�e
    int lineBytes; //BMP�@��r??

        int i, j, a;

    unsigned char** bmpData; //BMP�ɮפ��e
    unsigned char** mspData; //MSP�ɮפ��e

    FILE* fp_bmp; //BMP�ɮ�
    FILE* fp_msp; //MSP�ɮ�
    MSPFILEHEADER mspFileHeader; //MSP�ɮ׼��Y

    printf("MSP2BMP 0.5.132 BETA��\n���v�Ҧ� (C) 2023-2024 Tom Hao �s�@\n");

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("�o�ӵ{���i�H�NWindows 1.x�M2.x��MS-Paint��MSP�W�椧�Ϲ��ഫ��BMP�W�椧�Ϲ��C\n �ϥ�'msp2bmp mspfilename bmpfilename'�R�O�Y�i�}�l�ഫ�C\n");
            return 0;
        }
        else
        {
            printf("�ݭn���w�ѼơA�ҡG'msp2bmp test.msp test.bmp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("�ݭn���w�ѼơA�ҡG'msp2bmp test.msp test.bmp'\n");
        return -1;

    }

    printf("��J��MSP�ɮ׬O %s �C\n", argv[1]);

    if((fp_msp = fopen(argv[1], "rb")) == NULL)
    {
        printf("�X�{���~�AMSP�ɮ�Ū�����ѡC\n");
        return -1;
    }

    if ((fp_bmp = fopen(argv[2], "wb+")) == NULL)
    {
        return -1;
    }

    fseek(fp_msp, 0, SEEK_SET);

    fread(&mspFileHeader, 32, 1, fp_msp);

    //showMspFileHead(mspFileHeader);

    //Windows�����T�{
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
        windowsVersion = 0; //�ɮ׿��~���ɮ׷l�a
    }

    if(windowsVersion == 1 || windowsVersion == 2)
    {
        if(windowsVersion == 2)
        {
            printf("�ثe�o�ӵ{���ȮɵL�k�B�zWindows 2.x��MS-Paint��MSP�ɮסC\n");
            return -1;
        }
        else
        {
            if(mspFileHeader.mfWidth == 0x0140 || mspFileHeader.mfWidth == 0x0280) //�T�{MSP�Ϲ����e�סA�u���ഫ�T�ʤG�̡����ʥ|�̵e�����Ϲ��C
            {
                if(mspFileHeader.mfHeight == 0x00C8 || mspFileHeader.mfHeight == 0x000F || mspFileHeader.mfHeight == 0x0190 || mspFileHeader.mfHeight == 0x01E0)  //�T�{MSP�Ϲ������סA�u���ഫ�G�ʡ��G�ʥ|�̡��|�ʡ��|�ʤK�̵e�����Ϲ��C
                {
                    width = mspFileHeader.mfWidth;
                    height = mspFileHeader.mfHeight;
                    fseek(fp_msp, 32, SEEK_SET); //MSP�Ϲ����e�w��
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
                    printf("�ഫ�}�l�C\n");
                    mspTurn(mspData, bmpData, width, height);
                    printf("�}�l�g�J�C\n");
                    ok = bmpFiller(fp_bmp, bmpData, width, height, lineBytes);

                    if(ok == 0) //�Y���\
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("BMP�ɮ׳Ыا����I\n");
                        return 0;
                    }
                    else //�Y����
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("BMP�ɮ׳Ыإ��ѡI\n");
                        return 0;
                    }
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("�L�k�B�z�o�ذ��ת��Ϥ��C\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("�L�k�B�z�o�ؼe�ת��Ϥ��C\n");
                return 0;
            }
        }
    }
    else
    {
        printf("�]���ɮ׿��~���ɮ׷l�a�A�L�k�ഫ�C\n");
        return -1;
    }
}

