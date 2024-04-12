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
    int windowsVersion = 0; //Windows�̃o�[�W����
    int width; //MSP�t�@�C���̕�
    int height; //MSP�t�@�C���̍���
    int ok = 1; //�ϊ��������邩�ۂ����m�F

    int bmpSize; //BMP�t�@�C���̃T�C�Y
    int lineBytes; //BMP��s�̃o�C�g��

        int i, j, a;

    unsigned char** bmpData; //BMP�t�@�C�����e
    unsigned char** mspData; //MSP�t�@�C�����e

    FILE* fp_bmp; //BMP�t�@�C��
    FILE* fp_msp; //MSP�t�@�C��
    MSPFILEHEADER mspFileHeader; //MSP�t�@�C���̓�

    printf("MSP2BMP 0.5.132 BETA��\nCopyright (C) 2023-2024 Tom Hao ����\n");

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("���̃v���O�����́AWindows 1.x��2.x��MS-Paint��MSP�摜��BMP�摜�ɕϊ��o���܂��B\n 'msp2bmp mspfilename bmpfilename'�ƌ������߂ŁA�ϊ��J�n���܂��B\n");
            return 0;
        }
        else
        {
            printf("�p�����[�^�͕K�v�ł��A��F'msp2bmp test.msp test.bmp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("�p�����[�^�͕K�v�ł��A��F'msp2bmp test.msp test.bmp'\n");
        return -1;

    }

    printf("����MSP�t�@�C���� %s �ł��B\n", argv[1]);

    if((fp_msp = fopen(argv[1], "rb")) == NULL)
    {
        printf("�G���[�̈�MSP�t�@�C���ǂݍ��߂܂���B\n");
        return -1;
    }

    if ((fp_bmp = fopen(argv[2], "wb+")) == NULL)
    {
        return -1;
    }

    fseek(fp_msp, 0, SEEK_SET);

    fread(&mspFileHeader, 32, 1, fp_msp);

    //showMspFileHead(mspFileHeader);

    //Windows�o�[�W�����̊m�F
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
        windowsVersion = 0; //�ԈႢ�t�@�C�����t�@�C���j��
    }

    if(windowsVersion == 1 || windowsVersion == 2)
    {
        if(windowsVersion == 2)
        {
            printf("�����̃v���O�����͎b��Windows 2.x��MS-Paint��MSP�t�@�C���������o���܂���B\n");
            return -1;
        }
        else
        {
            if(mspFileHeader.mfWidth == 0x0140 || mspFileHeader.mfWidth == 0x0280) //MSP�摜�̕����m�F�A�B�O�S��\�E�Z�S�l�\��f�������͕ϊ��o���܂��B
            {
                if(mspFileHeader.mfHeight == 0x00C8 || mspFileHeader.mfHeight == 0x000F || mspFileHeader.mfHeight == 0x0190 || mspFileHeader.mfHeight == 0x01E0)  //MSP�摜�̍������m�F�A�B��S�E��S�l�\�E�l�S�E�l�S���\��f�̍��������͕ϊ��o���܂��B
                {
                    width = mspFileHeader.mfWidth;
                    height = mspFileHeader.mfHeight;
                    fseek(fp_msp, 32, SEEK_SET); //MSP�摜���e���
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
                    printf("�ϊ��J�n�B\n");
                    mspTurn(mspData, bmpData, width, height);
                    printf("�������݊J�n�B\n");
                    ok = bmpFiller(fp_bmp, bmpData, width, height, lineBytes);

                    if(ok == 0) //���������
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("BMP�t�@�C���쐬�����I\n");
                        return 0;
                    }
                    else //���s�����
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("BMP�t�@�C���쐬���s�I\n");
                        return 0;
                    }
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("���̍����������o���܂���B\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("���̕��������o���܂���B\n");
                return 0;
            }
        }
    }
    else
    {
        printf("�ԈႢ�t�@�C�����̓t�@�C���j���ł��̂ŁA�ϊ��o���܂���B\n");
        return -1;
    }
}

