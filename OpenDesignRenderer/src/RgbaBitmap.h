#pragma once

#include <stdint.h>

namespace odr {
/*!
    \brief Helper struct to work with binary RGBA bitmap data - encoding and decoding.
    \note Based on: https://github.com/bzotto/rgba_bitmap/
*/
struct RgbaBitmap {

static unsigned char * EndcodeToFileData(
    const unsigned char * input_buffer,
    uint32_t width,
    uint32_t height);

static unsigned char * DecodeFromFileData(
    const unsigned char * file_data,
    uint32_t file_data_length,
    uint32_t * p_width,
    uint32_t * p_height);
};
}
