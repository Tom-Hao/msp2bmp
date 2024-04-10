#ifndef MSP_H
#define MSP_H

#pragma pack(1)

    /*
    WORD  Key1;              Magic number    
    WORD  Key2;              Magic number    
    WORD  Width;             Width of the bitmap in pixels   
    WORD  Height;            Height of the bitmap in pixels   
    WORD  XARBitmap;         X Aspect ratio of the bitmap   
    WORD  YARBitmap;         Y Aspect ratio of the bitmap   
    WORD  XARPrinter;        X Aspect ratio of the printer   
    WORD  YARPrinter;        Y Aspect ratio of the printer   
    WORD  PrinterWidth;      Width of the printer in pixels   
    WORD  PrinterHeight;     Height of the printer in pixels   
    WORD  XAspectCorr;       X aspect correction (unused)     
    WORD  YAspectCorr;       Y aspect correction (unused)     
    WORD  Checksum;          Checksum of previous 24 bytes   
    WORD  Padding[3];        Unused padding    
    */

typedef struct MSP_FILE_HEADER
{
    unsigned short mfKey1; //Magic number
    unsigned short mfKey2; //Magic number
    unsigned short mfWidth; //Width of the bitmap in pixels
    unsigned short mfHeight; //Height of the bitmap in pixels 
    unsigned short mfXARBitmap; //X Aspect ratio of the bitmap
    unsigned short mfYARBitmap; //Y Aspect ratio of the bitmap   
    unsigned short mfXARPrinter; //X Aspect ratio of the printer 
    unsigned short mfYARPrinter; //Y Aspect ratio of the printer 
    unsigned short mfPrinterWidth; //Width of the printer in pixels  
    unsigned short mfPrinterHeight; //Height of the printer in pixels 
    unsigned short mfXAspectCorr; //X aspect correction (unused)  
    unsigned short mfYAspectCorr; //Y aspect correction (unused) 
    unsigned short mfChecksum; //Checksum of previous 24 bytes 
    unsigned short mfPadding1; //Unused padding
    unsigned short mfPadding2; //Unused padding
    unsigned short mfPadding3; //Unused padding    
}MSPFILEHEADER;


#endif