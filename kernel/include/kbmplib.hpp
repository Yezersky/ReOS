#ifndef _KBMPLIB_H_
#define _KBMPLIB_H_

#include <types.hpp>

#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER
{

 uint16_t bfType;
 uint32_t bfSize;
 uint16_t bfReserved1;
 uint16_t bfReserved2;
 uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
 uint32_t biSize;
 uint32_t biWidth;
 uint32_t biHeight;
 uint16_t biPlanes;
 uint16_t biBitCount;
 uint32_t biCompression;
 uint32_t biSizeImage;
 uint32_t biXPelsPerMeter;
 uint32_t biYPelsPerMeter;
 uint32_t biClrUsed;
 uint32_t biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD
{
 uint8_t rgbBlue;
 uint8_t rgbGreen;
 uint8_t rgbRed;
 uint8_t rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO
{
 BITMAPINFOHEADER bmiHeader;
 RGBQUAD bmiColors[1];
} BITMAPINFO;


typedef struct tagBITMAP
{
 BITMAPFILEHEADER bfHeader;
 BITMAPINFO biInfo;
} BITMAPFILE;

#pragma pack(pop)

// typedef struct imgBITMAP
// {
//  uint8_t  bitCountPerPix;
//  uint32_t width;
//  uint32_t height;
//  uint8_t  *pdata;
// } BITMAPIMAGE;

void display_bmp(size_t fd, size_t max, const char* file);
// void FreeBmpData(uint8_t *pdata);

#endif
