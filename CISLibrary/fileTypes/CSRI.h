#ifndef _LIBRARY_FILE_TYPES_CSRI_H_
#define _LIBRARY_FILE_TYPES_CSRI_H_

#include <mbed.h>

// #define RESOLUTION_VGA        0x028001E0
// #define RESOLUTION_1280x960   0x050003C0
// #define RESOLUTION_2000x1500  0x07D005DC
// #define RESOLUTION_2592x1944  0x0A200768

// enum PixelLocation{
// 	NW = 0,
// 	NE,
// 	SW,
// 	SE
// };

// enum BayerFormat{
// 	RGGB = 0,
// 	BGGR,
// 	GBRG,
// 	GRBG
// };

// #define CSRI_RESOL_SHIFT 6
// #define CSRI_PIXEL_SHIFT 4
// #define CSRI_BAYER_SHIFT 2
// #define CSRI_DEPTH_SHIFT 1
// #define CSRI_COMPR_SHIFT 0

// static uint8_t rawToCSRI(char * srcPath, char * dstPath, uint32_t resolution,
//     PixelLocation firstPixel, BayerFormat bayerFormat, uint8_t bitDepth,
//     bool compressData);

// static uint8_t writeHeaderCSRI(FILE * file, uint32_t resolution,
//     PixelLocation firstPixel, BayerFormat bayerFormat, uint8_t bitDepth);

#endif /* SRC_DRIVERS_CSRI_H_ */
