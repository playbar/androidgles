#include "pngutils.h"


typedef struct
{
    unsigned char* data;
    int size;
    int offset;
}ImageSource;

static void memReadFuncPng(png_structp png_ptr, png_bytep data, png_size_t length)
{
    ImageSource* isource = (ImageSource*) png_get_io_ptr(png_ptr);
    if (isource->offset + length <= isource->size)
    {
        memcpy(data, isource->data + isource->offset, length);
        isource->offset += length;
    }
    else
    {
        png_error(png_ptr, "pngReaderCallback failed");
    }
}

unsigned char* LoadFile(char *filename, long *fSize)
{
    FILE *fp = 0;
    unsigned char * fData = 0;

    fp = fopen (filename, "rb");
    if (fp != 0)
    {
        fseek(fp , 0 , SEEK_END);
        fSize = ftell(fp);
        rewind(fp);

        fData = (unsigned char *)malloc(fSize);
        if (fread(fData, 1, fSize, fp) != fSize)
        {
            free(fData);
            fData = 0;
        }
        fclose(fp);
    }

    return fData;
}

unsigned char *DecodePngFile(char *filename, int *width, int *height)
{
    long fSize = 0;
    unsigned char* fData = LoadFile(filename,fSize);
    unsigned char* ret = 0;
    ret = DecodeMemoryPNG(fData, fSize, width, height);
    free(fData);
    return ret;
}

unsigned char* DecodeMemoryPNG(unsigned char* fData, long fSize, int *width, int *height)
{
    png_structp png_ptr;
    png_infop info_ptr;
    int bit_depth, color_type;
    png_bytep *row_pointers = NULL;
    unsigned char* image_data;
    int rowbytes;

/* Create a png read struct */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        return NULL;
    }

/* Create a png info struct */
    info_ptr = png_create_info_struct (png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct (&png_ptr, NULL, NULL);
        return NULL;
    }

/* Initialize the setjmp for returning properly after a libpng error occured */
    if (setjmp (png_jmpbuf (png_ptr)))
    {
        png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

        if (row_pointers)
            free (row_pointers);

        return NULL;
    }

    ImageSource imgsource;
    imgsource.data = fData;
    imgsource.size = fSize;
    imgsource.offset = 0;
    png_set_read_fn(png_ptr, &imgsource, memReadFuncPng);

/* Read png info */
    png_read_info (png_ptr, info_ptr);

/* Get some usefull information from header */
    bit_depth = png_get_bit_depth (png_ptr, info_ptr);
    color_type = png_get_color_type (png_ptr, info_ptr);

/* Convert index color images to RGB images */
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb (png_ptr);

/* Convert RGB images to RGBA images */
    if (color_type == PNG_COLOR_TYPE_RGB)
        png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

/* Convert 1-2-4 bits grayscale images to 8 bits grayscale. */
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_gray_1_2_4_to_8 (png_ptr);

    if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha (png_ptr);

    if (bit_depth == 16)
        png_set_strip_16 (png_ptr);
    else if (bit_depth < 8)
        png_set_packing (png_ptr);

/* Update info structure to apply transformations */
    png_read_update_info (png_ptr, info_ptr);

/* Retrieve updated information */
    png_get_IHDR (png_ptr, info_ptr, (png_uint_32*)width, (png_uint_32*)height, &bit_depth, &color_type, NULL, NULL, NULL);

    rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    if ((image_data =(unsigned char *) malloc(*height * rowbytes)) == NULL)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }

/* Setup a pointer array.  Each one points at the begening of a row. */
    if ((row_pointers =(png_bytepp) malloc(*height * sizeof(png_bytep))) == NULL)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        free(image_data);
        return NULL;
    }
    for (int i = 0; i < *height; i++)
        row_pointers[*height - 1 - i] = image_data + i*rowbytes;

/* Read pixel data using row pointers */
    png_read_image (png_ptr, row_pointers);

/* Finish decompression and release memory */
    png_read_end (png_ptr, NULL);
    png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

/* We don't need row pointers anymore */
    free (row_pointers);

    return image_data;
}

void user_write_fn(png_structp png_ptr, png_bytep data, png_size_t length)
{
    FILE *pFile = png_get_io_ptr(png_ptr);
    fwrite(data, length, 1,pFile );
}

void user_flush_function(png_structp png_ptr)
{
    FILE *pFile = png_get_io_ptr(png_ptr);
    fflush(pFile);
}

/* Write a png file */
bool write_png(char *file_name, unsigned char *pdata, int width, int height)
{
    FILE *fp;
    png_structp png_ptr;
    png_infop info_ptr;
    png_colorp palette;

    /* Open the file */
    fp = fopen(file_name, "wb");
    if (fp == NULL)
        return false;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr == NULL)
    {
        fclose(fp);
        return false;
    }

    /* Allocate/initialize the image information data.  REQUIRED */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        fclose(fp);
        png_destroy_write_struct(&png_ptr,  png_infopp_NULL);
        return false;
    }

    /* Set error handling.  REQUIRED if you aren't supplying your own
     * error handling functions in the png_create_write_struct() call.
     */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        /* If we get here, we had a problem writing the file */
        fclose(fp);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

//   png_init_io(png_ptr, fp);
    png_set_write_fn(png_ptr, (void *) fp, user_write_fn, user_flush_function);

//    /* where user_io_ptr is a structure you want available to the callbacks */
//#endif no_streams /* Only use one initialization method */

    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    /* Set the palette if there is one.  REQUIRED for indexed-color images */
    palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH
                                              * png_sizeof(png_color));
    /* ... Set palette colors ... */
    png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
    /* You must not free palette here, because png_set_PLTE only makes a link to
     * the palette that you malloced.  Wait until you are about to destroy
     * the png structure.
     */

    /* Optional significant bit (sBIT) chunk */
    png_color_8 sig_bit;
    /* If we are dealing with a grayscale image then */
    sig_bit.gray = 8;
    /* Otherwise, if we are dealing with a color image then */
    sig_bit.red = 8;
    sig_bit.green = 8;
    sig_bit.blue = 8;
    /* If the image has an alpha channel then */
    sig_bit.alpha = 8;
    png_set_sBIT(png_ptr, info_ptr, &sig_bit);


    /* Optional gamma chunk is strongly suggested if you have any guess
     * as to the correct gamma of the image.
     */
//    png_set_gAMA(png_ptr, info_ptr, gamma);

    /* Optionally write comments into the image */
    png_text text_ptr[3];
    text_ptr[0].key = "Title";
    text_ptr[0].text = "Mona Lisa";
    text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
    text_ptr[1].key = "Author";
    text_ptr[1].text = "MJ";
    text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
    text_ptr[2].key = "Description";
    text_ptr[2].text = "MJ Png Image";
    text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
#ifdef PNG_iTXt_SUPPORTED
    text_ptr[0].lang = NULL;
   text_ptr[1].lang = NULL;
   text_ptr[2].lang = NULL;
#endif
    png_set_text(png_ptr, info_ptr, text_ptr, 3);

    png_write_info(png_ptr, info_ptr);

    /* Invert monochrome pixels */
    png_set_invert_mono(png_ptr);

    /* Shift the pixels up to a legal bit depth and fill in
     * as appropriate to correctly scale the image.
     */
    png_set_shift(png_ptr, &sig_bit);

    /* Pack pixels into bytes */
    png_set_packing(png_ptr);

    /* Swap location of alpha bytes from ARGB to RGBA */
    png_set_swap_alpha(png_ptr);

    /* Get rid of filler (OR ALPHA) bytes, pack XRGB/RGBX/ARGB/RGBA into
     * RGB (4 channels -> 3 channels). The second parameter is not used.
     */
    png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);

    /* Flip BGR pixels to RGB */
    png_set_bgr(png_ptr);

    /* Swap bytes of 16-bit files to most significant byte first */
    png_set_swap(png_ptr);

    /* Swap bits of 1, 2, 4 bit packed pixel formats */
    png_set_packswap(png_ptr);

    /* Turn on interlace handling if you are not using png_write_image() */
    int  number_passes = png_set_interlace_handling(png_ptr);

    /* The easiest way to write the image (you may have a different memory
     * layout, however, so choose what fits your needs best).  You need to
     * use the first method if you aren't handling interlacing yourself.
     */
    png_uint_32 k;
    int bytes_per_pixel = 4;
//    png_byte image[height][width*bytes_per_pixel];
    png_bytep image = pdata;
    png_bytepp row_pointers= (png_bytepp)malloc(height * sizeof(png_bytep));

    if (height > PNG_UINT_32_MAX/png_sizeof(png_bytep))
        png_error (png_ptr, "Image is too tall to process in memory");

    for (k = 0; k < height; k++)
        row_pointers[k] = image + k*width*bytes_per_pixel;

    /* One of the following output methods is REQUIRED */

    png_write_image(png_ptr, row_pointers);

    /* You can write optional chunks like tEXt, zTXt, and tIME at the end
     * as well.  Shouldn't be necessary in 1.2.0 and up as all the public
     * chunks are supported and you can use png_set_unknown_chunks() to
     * register unknown chunks into the info structure to be written out.
     */

    /* It is REQUIRED to call this to finish writing the rest of the file */
    png_write_end(png_ptr, info_ptr);

    png_free(png_ptr, palette);
    palette = NULL;

    free(row_pointers);

    /* Similarly, if you png_malloced any data that you passed in with
     * png_set_something(), such as a hist or trans array, free it here,
     * when you can be sure that libpng is through with it.
     */
//    png_free(png_ptr, trans);
//    trans = NULL;


    /* Clean up after the write, and free any memory allocated */
    png_destroy_write_struct(&png_ptr, &info_ptr);

    /* Close the file */
    fclose(fp);

    /* That's it */
    return true;
}


void write_png_1(char *file_name, unsigned char *pdata, int width, int height)
{
    FILE *fp;
    png_structp png_ptr;
    png_infop info_ptr;
    png_colorp palette;

    /* 打开需要写入的文件 */
    fp = fopen(file_name, "wb");
    if (fp == NULL)
        return;

    /* 创建并初始化 png_struct 及其所需的错误处理函数，如果你想使用默
     * 认的 stderr 和 longjump() 方法，你可以将最后三个参数设为 NULL，
     * 在使用动态链接库的情况下，我们也会检测函数库版本是否与在编译时
     * 使用的版本是否兼容。（必要）
     */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr == NULL)
    {
        fclose(fp);
        return;
    }

    /* 分配内存并初始化图像信息数据。（必要）*/
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        fclose(fp);
        png_destroy_write_struct(&png_ptr,  NULL);
        return;
    }

    /* 设置错误处理。如果你在调用 png_create_write_struct() 时没
     * 有设置错误处理函数，那么这段代码是必须写的。*/
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        /* 如果程序跑到这里了，那么写入文件时出现了问题 */
        fclose(fp);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }

    /* 设置输出控制，如果你使用的是 C 的标准 I/O 流 */
//    png_init_io(png_ptr, fp);
    png_set_write_fn(png_ptr, (void *) fp, user_write_fn, user_flush_function);

    /* 这是一种复杂的做法 */

    /* （必需）在这里设置图像的信息，宽度、高度的上限是 2^31。
     * bit_depth 取值必需是 1、2、4、8 或者 16, 但是可用的值也依赖于 color_type。
     * color_type 可选值有： PNG_COLOR_TYPE_GRAY、PNG_COLOR_TYPE_GRAY_ALPHA、
     * PNG_COLOR_TYPE_PALETTE、PNG_COLOR_TYPE_RGB、PNG_COLOR_TYPE_RGB_ALPHA。
     * interlace 可以是 PNG_INTERLACE_NONE 或 PNG_INTERLACE_ADAM7,
     * 而 compression_type 和 filter_type 目前必需是 PNG_COMPRESSION_TYPE_BASE
     * 和 and PNG_FILTER_TYPE_BASE。
     */
    int bit_depth = 8;
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, PNG_COLOR_TYPE_RGB_ALPHA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    /* 写入文件头部信息（必需） */
    png_write_info(png_ptr, info_ptr);

    png_uint_32 k;
    int bytes_per_pixel = 4;

    /* 在这个示例代码中，"image" 是一个一维的字节数组（每个元素占一个字节空间） */
    png_byte image[height*width*bytes_per_pixel];

    png_bytepp row_pointers = malloc(height * sizeof(png_bytep));

    if (height > PNG_UINT_32_MAX/(sizeof (png_bytep)))
        png_error (png_ptr, "Image is too tall to process in memory");

    /* 将这些像素行指针指向你的 "image" 字节数组中对应的位置，即：指向每行像素的起始处 */
    for (k = 0; k < height; k++)
        row_pointers[k] = image + k*width*bytes_per_pixel;

    /* 一次调用就将整个图像写进文件 */
    png_write_image(png_ptr, row_pointers);
    /* 必需调用这个函数完成写入文件其余部分 */
    png_write_end(png_ptr, info_ptr);
    /* 写完后清理并释放已分配的内存 */
    png_destroy_write_struct(&png_ptr, &info_ptr);
    /* 关闭文件 */
    fclose(fp);
    free(row_pointers);

    /* That's it */
    return;
}