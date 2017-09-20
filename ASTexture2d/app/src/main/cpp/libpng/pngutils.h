#ifndef __PNGUTLIS_H__
#define __PNGUTLIS_H__


#include <malloc.h>
#include <stdbool.h>
#include "png.h"

typedef struct _pic_data pic_data;
struct _pic_data
{
    int	width, height;	/* 尺寸 */
    int	bit_depth;		/* 位深 */
    int	flag;			/* 一个标志，表示是否有alpha通道 */

    unsigned char	*rgba;	/* 图片数组 */
};

bool write_png(char *file_name, unsigned char *pdata, int width, int height);
void write_png_1(char *file_name, unsigned char *pdata, int width, int height);
unsigned char *DecodePngFile(char *filename, int *width, int *height);
unsigned char* DecodeMemoryPNG(unsigned char* fData, long fSize, int *width, int *height);
int detect_png(char *filepath, pic_data *out);
int write_png_file(char *file_name , pic_data *graph);

#endif
