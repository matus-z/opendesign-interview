#include "RgbaBitmap.h"

#include <string>
#include <stdlib.h>


#define RGBA_BITMAP_MAGIC_NUMBER            0x52474241   /* 'RGBA' */

#define READ_BIG_ENDIAN_UINT32(ptr)         (*ptr << 24 | *(ptr+1) << 16 | *(ptr+2)<<8 | *(ptr+3))
#define WRITE_BIG_ENDIAN_UINT32(ptr, u32)   *ptr = (u32 >> 24) & 0xFF; \
                                            *(ptr+1) = (u32 >> 16) & 0xFF; \
                                            *(ptr+2) = (u32 >> 8) & 0xFF; \
                                            *(ptr+3) = u32 & 0xFF;

/*static*/ unsigned char* odr::RgbaBitmap::EndcodeToFileData(
    const unsigned char* input_buffer,
    uint32_t width,
    uint32_t height)
{
    if (!input_buffer || width == 0 || height == 0) {
        return nullptr;
    }

    const uint32_t imageDataSize = width * height * 4;
    const uint32_t output_size = 12 + imageDataSize;

    unsigned char* rgba_file = (unsigned char*)malloc(output_size);
    if (!rgba_file) {
        return nullptr;
    }

    WRITE_BIG_ENDIAN_UINT32(&rgba_file[0], RGBA_BITMAP_MAGIC_NUMBER);
    WRITE_BIG_ENDIAN_UINT32(&rgba_file[4], width);
    WRITE_BIG_ENDIAN_UINT32(&rgba_file[8], height);

    memcpy(&rgba_file[12], input_buffer, imageDataSize);

    return rgba_file;
}

/*static*/ unsigned char* odr::RgbaBitmap::DecodeFromFileData(
    const unsigned char* file_data,
    uint32_t file_data_length,
    uint32_t* p_width,
    uint32_t* p_height)
{
    if (!file_data || !p_width || !p_height) {
        return nullptr;
    }

    if (file_data_length < 12) {
        return nullptr;
    }

    const uint32_t magic = READ_BIG_ENDIAN_UINT32(&file_data[0]);
    if (magic != RGBA_BITMAP_MAGIC_NUMBER) {
        return nullptr;
    }

    const uint32_t width = READ_BIG_ENDIAN_UINT32(&file_data[4]);
    const uint32_t height = READ_BIG_ENDIAN_UINT32(&file_data[8]);

    const uint32_t imageDataSize = width * height * 4;

    if (file_data_length < (12 + imageDataSize)) {
        return nullptr;
    }

    unsigned char* bitmap = (unsigned char*)malloc(imageDataSize);
    if (!bitmap) {
        return nullptr;
    }

    memcpy(bitmap, &file_data[12], imageDataSize);

    *p_width = width;
    *p_height = height;

    return bitmap;
}
