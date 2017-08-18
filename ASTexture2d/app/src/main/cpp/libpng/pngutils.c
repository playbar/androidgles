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

    /* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also check that
     * the library version is compatible with the one used at compile time,
     * in case we are using dynamically linked libraries.  REQUIRED.
     */
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

    /* One of the following I/O initialization functions is REQUIRED */

//#ifdef streams /* I/O initialization method 1 */
    /* Set up the output control if you are using standard C streams */
//   png_init_io(png_ptr, fp);

//#else no_streams /* I/O initialization method 2 */
//    /* If you are using replacement write functions, instead of calling
//     * png_init_io() here you would call
//     */
    png_set_write_fn(png_ptr, (void *) fp, user_write_fn, user_flush_function);

//    /* where user_io_ptr is a structure you want available to the callbacks */
//#endif no_streams /* Only use one initialization method */

#ifdef hilevel
    /* This is the easy way.  Use it if you already have all the
    * image info living in the structure.  You could "|" many
    * PNG_TRANSFORM flags into the png_transforms integer here.
    */
   png_write_png(png_ptr, info_ptr, png_transforms, png_voidp_NULL);

#else
    /* This is the hard way */

    /* Set the image information here.  Width and height are up to 2^31,
     * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
     * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
     * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
     * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
     * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
     * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
     */

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

    /* Other optional chunks like cHRM, bKGD, tRNS, tIME, oFFs, pHYs */

    /* Note that if sRGB is present the gAMA and cHRM chunks must be ignored
     * on read and, if your application chooses to write them, they must
     * be written in accordance with the sRGB profile
     */

    /* Write the file header information.  REQUIRED */
    png_write_info(png_ptr, info_ptr);

    /* If you want, you can write the info in two steps, in case you need to
     * write your private chunk ahead of PLTE:
     *
     *   png_write_info_before_PLTE(write_ptr, write_info_ptr);
     *   write_my_chunk();
     *   png_write_info(png_ptr, info_ptr);
     *
     * However, given the level of known- and unknown-chunk support in 1.2.0
     * and up, this should no longer be necessary.
     */

    /* Once we write out the header, the compression type on the text
     * chunks gets changed to PNG_TEXT_COMPRESSION_NONE_WR or
     * PNG_TEXT_COMPRESSION_zTXt_WR, so it doesn't get written out again
     * at the end.
     */

    /* Set up the transformations you want.  Note that these are
     * all optional.  Only call them if you want them.
     */

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
#endif hilevel

    /* If you png_malloced a palette, free it here (don't free info_ptr->palette,
     * as recommended in versions 1.0.5m and earlier of this example; if
     * libpng mallocs info_ptr->palette, libpng will free it).  If you
     * allocated it with malloc() instead of png_malloc(), use free() instead
     * of png_free().
     */
    png_free(png_ptr, palette);
    palette = NULL;

    free(row_pointers);

    /* Similarly, if you png_malloced any data that you passed in with
     * png_set_something(), such as a hist or trans array, free it here,
     * when you can be sure that libpng is through with it.
     */
//    png_free(png_ptr, trans);
//    trans = NULL;
    /* Whenever you use png_free() it is a good idea to set the pointer to
     * NULL in case your application inadvertently tries to png_free() it
     * again.  When png_free() sees a NULL it returns without action, thus
     * avoiding the double-free security problem.
     */

    /* Clean up after the write, and free any memory allocated */
    png_destroy_write_struct(&png_ptr, &info_ptr);

    /* Close the file */
    fclose(fp);

    /* That's it */
    return true;
}
