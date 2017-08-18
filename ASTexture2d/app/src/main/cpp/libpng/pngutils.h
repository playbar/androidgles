#ifndef __PNGUTLIS_H__
#define __PNGUTLIS_H__


#include <malloc.h>
#include <stdbool.h>
#include "png.h"

bool write_png(char *file_name, unsigned char *pdata, int width, int height);
unsigned char *DecodePngFile(char *filename, int *width, int *height);
unsigned char* DecodeMemoryPNG(unsigned char* fData, long fSize, int *width, int *height);


#endif
