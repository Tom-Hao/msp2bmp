#define _CRT_SECURE_NO_WARNINGS

#include "mspHeadReader.h"

void showMspFileHead(MSPFILEHEADER mspFileHeader)
{
    int winVer = 0;
    if(mspFileHeader.mfKey1 == 0x6144 && mspFileHeader.mfKey2 == 0x4d6e)
    {
        winVer = 1;
    }
    else if(mspFileHeader.mfKey1 == 0x694c && mspFileHeader.mfKey2 == 0x536e)
    {
        winVer = 2;
    }

    printf("Windows Version: Windows %d\n", winVer);
    printf("Width: %u\n", mspFileHeader.mfWidth);
    printf("Height: %u\n", mspFileHeader.mfHeight);
    printf("XARBitmap: %x\n", mspFileHeader.mfXARBitmap);
    printf("YARBitmap: %x\n", mspFileHeader.mfYARBitmap);
    printf("XARPrinter: %x\n", mspFileHeader.mfXARPrinter);
    printf("YARPrinter: %x\n", mspFileHeader.mfYARPrinter);
    printf("PrinterWidth: %x\n", mspFileHeader.mfPrinterWidth);
    printf("PrinterHeight: %x\n", mspFileHeader.mfPrinterHeight);
    printf("XAspectCorr: %x\n", mspFileHeader.mfXAspectCorr);
    printf("YAspectCorr: %x\n", mspFileHeader.mfYAspectCorr);
    printf("Checksum: %x\n", mspFileHeader.mfChecksum);
    printf("Padding: %x%x%x\n", mspFileHeader.mfPadding1, mspFileHeader.mfPadding2, mspFileHeader.mfPadding3);
}
