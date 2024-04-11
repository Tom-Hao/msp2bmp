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
    int windowsVersion = 0; //Windows�汾
    int width; //MSP�ļ����
    int height; //MSP�ļ��߶�
    int ok = 1; //ת�����ȷ��

    int bmpSize; //BMP�ļ�����
    int lineBytes; //BMPһ���ֽ���

        int i, j, a;

    unsigned char** bmpData; //BMP�ļ�����
    unsigned char** mspData; //MSP�ļ�����

    FILE* fp_bmp; //BMP�ļ�
    FILE* fp_msp; //MSP�ļ�
    MSPFILEHEADER mspFileHeader; //MSP�ļ�ͷ

    printf("MSP2BMP 0.5.132 BETA��\n��Ȩ���� (C) 2023-2024 �й����忪���� Tom Hao ����\n");

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("���������Խ�Windows 1.x��2.x��MS-Paint��MSP��ʽͼ��ת��ΪBMP��ʽͼ��\n ʹ��'msp2bmp mspfilename bmpfilename'����ɿ�ʼת����\n");
            return 0;
        }
        else
        {
            printf("��Ҫָ������������'msp2bmp test.msp test.bmp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("��Ҫָ������������'msp2bmp test.msp test.bmp'\n");
        return -1;

    }

    printf("�����MSP�ļ��� %s ��\n", argv[1]);

    if((fp_msp = fopen(argv[1], "rb")) == NULL)
    {
        printf("���ִ���MSP�ļ���ȡʧ�ܡ�\n");
        return -1;
    }

    if ((fp_bmp = fopen(argv[2], "wb+")) == NULL)
    {
        return -1;
    }

    fseek(fp_msp, 0, SEEK_SET);

    fread(&mspFileHeader, 32, 1, fp_msp);

    //showMspFileHead(mspFileHeader);

    //Windows�汾ȷ��
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
        windowsVersion = 0; //�ļ�������ļ���
    }

    if(windowsVersion == 1 || windowsVersion == 2)
    {
        if(windowsVersion == 2)
        {
            printf("Ŀǰ���������ʱ�޷�����Windows 2.x��MS-Paint��MSP�ļ���\n");
            return -1;
        }
        else
        {
            if(mspFileHeader.mfWidth == 0x0140 || mspFileHeader.mfWidth == 0x0280) //ȷ��MSPͼ��Ŀ�ȣ�ֻ��ת�����ٶ�ʮ��������ʮ���ص�ͼƬ��
            {
                if(mspFileHeader.mfHeight == 0x00C8 || mspFileHeader.mfHeight == 0x000F || mspFileHeader.mfHeight == 0x0190 || mspFileHeader.mfHeight == 0x01E0)  //ȷ��MSPͼ��ĸ߶ȣ�ֻ��ת�����١�������ʮ���İ١��İٰ�ʮ���ص�ͼƬ��
                {
                    width = mspFileHeader.mfWidth;
                    height = mspFileHeader.mfHeight;
                    fseek(fp_msp, 32, SEEK_SET); //MSPͼ�����ݶ�λ
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
                    printf("ת����ʼ��\n");
                    mspTurn(mspData, bmpData, width, height);
                    printf("��ʼд�롣\n");
                    ok = bmpFiller(fp_bmp, bmpData, width, height, lineBytes);

                    if(ok == 0) //���ɹ�
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("BMP�ļ�������ɣ�\n");
                        return 0;
                    }
                    else //��ʧ��
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("BMP�ļ�����ʧ�ܣ�\n");
                        return 0;
                    }
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("�޷��������ָ߶ȵ�ͼƬ��\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("�޷��������ֿ�ȵ�ͼƬ��\n");
                return 0;
            }
        }
    }
    else
    {
        printf("��Ϊ�ļ�������ļ��𻵣��޷�ת����\n");
        return -1;
    }
}

