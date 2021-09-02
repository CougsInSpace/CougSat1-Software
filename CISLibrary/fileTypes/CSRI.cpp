#include "CSRI.h"

#include <CISConsole.h>

// /**
//  * Packages a raw image file into a Cougs in Space RAW Image (CSRI) file
//  * @param src raw image byte data file, 2 byte per 10 bit pixel,
//  * @param dst csri file
//  * @param resolution of image: width[3..2], height[1..0]
//  * @param firstPixel location
//  * @param bayerFormat
//  * @param bitDepth 8 or 10 bits per pixel
//  * @param compressData
//  * @return error code
//  */
// uint8_t CSRI::rawToCSRI(char *srcPath, char *dstPath, uint32_t resolution,
//     PixelLocation firstPixel, BayerFormat bayerFormat, uint8_t bitDepth,
//     bool compressData) {
//   FILE *src = fopen(srcPath, "r");
//   if (src == NULL) {
//     LOGD("rawToCSRI", "Unable to open source file");
//     return ERROR_OPEN_FAILED;
//   }
//   FILE *dst = fopen(dstPath, "w");
//   if (dst == NULL) {
//     LOGD("rawToCSRI", "Unable to open destination file");
//     fclose(src);
//     return ERROR_OPEN_FAILED;
//   }
//   bool customResolution = false;
//   char header[4] =
//   { 0, 0, 0, 0 };
//   fseek(src, 0L, SEEK_END);
//   uint32_t dataLength = ftell(src);
//   rewind(src);

//   switch (resolution) {
//     case RESOLUTION_VGA:
//       header[0] = 0 << CSRI_RESOL_SHIFT;
//       break;
//     case RESOLUTION_2000x1500:
//       header[0] = 1 << CSRI_RESOL_SHIFT;
//       break;
//     case RESOLUTION_2592x1944:
//       header[0] = 2 << CSRI_RESOL_SHIFT;
//       break;
//     default:
//       customResolution = true;
//       header[0] = 3 << CSRI_RESOL_SHIFT;
//       break;
//   }
//   header[0] |= firstPixel << CSRI_PIXEL_SHIFT;
//   header[0] |= bayerFormat << CSRI_BAYER_SHIFT;
//   switch (bitDepth) {
//     case 8:
//       header[0] |= 0 << CSRI_DEPTH_SHIFT;
//       break;
//     case 10:
//       header[0] |= 1 << CSRI_DEPTH_SHIFT;
//       dataLength = dataLength * 2 / 3; //(6bytes -> 4 bytes)
//       break;
//     default:
//       LOGD("rawToCSRI", "Invalid bit depth input");
//       fclose(src);
//       fclose(dst);
//       return ERROR_INVALID_ARGS;
//   }
//   if (compressData) {
//     header[0] |= 1 << CSRI_COMPR_SHIFT;
//     //compress src to temp file
//     //close src file
//     //point src to temp file
//     //adjust data length arguument
//     LOGD("rawToCSRI", "Compression not implemented");
//     fclose(src);
//     fclose(dst);
//     return ERROR_NOT_SUPPORTED;
//   }
//   header[1] = (dataLength >> 16) & 0xFF;
//   header[2] = (dataLength >> 8) & 0xFF;
//   header[3] = (dataLength >> 0) & 0xFF;
//   fprintf(dst, header);

//   if (customResolution) {
//     header[0] = (resolution >> 24) & 0xFF;
//     header[1] = (resolution >> 16) & 0xFF;
//     header[2] = (resolution >> 8) & 0xFF;
//     header[3] = (resolution >> 0) & 0xFF;
//     fprintf(dst, header);
//   }

//   int buf;
//   if (compressData || resolution == 8) {
//     while ((buf = getc(src)) != EOF) {
//       if (ferror(src) != ERROR_SUCCESS) {
//         LOGD("rawToCSRI", "Failed to read source file");
//         fclose(src);
//         fclose(dst);
//         return ERROR_READ;
//       }
//       putc(buf, dst);
//       if (ferror(dst) != ERROR_SUCCESS) {
//         LOGD("rawToCSRI", "Failed to write to destination file");
//         fclose(src);
//         fclose(dst);
//         return ERROR_WRITE;
//       }
//       dataLength--;
//     }
//   } else {
//     //Turn 3 10bit pixels (6 bytes) into 4 bytes
//     //__001111|11112233|33333344|55555555
//     //data[0] = (byte[0] & 0x03) << 4 | byte[1] >> 4
//     //data[1] = byte[1] << 4 | (byte[2] & 0x03) << 2 | byte[3] >> 6
//     //data[2] = byte[3] << 2 | (byte[4] & 0x03)
//     //data[3] = byte[5]
//   }
//   if (dataLength != 0) {
//     LOGD("rawToCSRI", "Data length was incorrect");
//     fclose(src);
//     fclose(dst);
//     return ERROR_INVALID_ARGS;
//   }
//   fclose(src);
//   fclose(dst);
//   return ERROR_SUCCESS;
// }

// /**
//  * Writes the Cougs in Space RAW Image header to a file
//  * @param file to write to
//  * @param resolution of image: width[3..2], height[1..0]
//  * @param firstPixel location
//  * @param bayerFormat
//  * @param bitDepth 8 or 10 bits per pixel
//  * @return error code
//  */
// uint8_t CSRI::writeHeaderCSRI(FILE *file, uint32_t resolution,
//     PixelLocation firstPixel, BayerFormat bayerFormat, uint8_t bitDepth) {

//   if (file == NULL) {
//     LOGD("rawToCSRI", "File pointer is NULL");
//     return ERROR_INVALID_ARGS;
//   }

//   bool customResolution = false;
//   char header[4] =
//   { 0, 0, 0, 0 };
//   uint32_t dataLength = 0;

//   switch (resolution) {
//     case RESOLUTION_VGA:
//       header[0] = 0 << CSRI_RESOL_SHIFT;
//       dataLength = 640 * 480;
//       break;
//     case RESOLUTION_2000x1500:
//       header[0] = 1 << CSRI_RESOL_SHIFT;
//       dataLength = 2000 * 1500;
//       break;
//     case RESOLUTION_2592x1944:
//       header[0] = 2 << CSRI_RESOL_SHIFT;
//       dataLength = 2592 * 1944;
//       break;
//     default:
//       customResolution = true;
//       header[0] = 3 << CSRI_RESOL_SHIFT;
//       dataLength = ((resolution >> 16) & 0xFFFF) * (resolution & 0xFFFF);
//       break;
//   }
//   header[0] |= firstPixel << CSRI_PIXEL_SHIFT;
//   header[0] |= bayerFormat << CSRI_BAYER_SHIFT;
//   switch (bitDepth) {
//     case 8:
//       header[0] |= 0 << CSRI_DEPTH_SHIFT;
//       dataLength = dataLength / 4; //(1 dword / 4 pixels)
//       break;
//     case 10:
//       header[0] |= 1 << CSRI_DEPTH_SHIFT;
//       dataLength = dataLength / 3; //(1 dword / 3 pixels)
//       break;
//     default:
//       LOGD("rawToCSRI", "Invalid bit depth");
//       fclose(file);
//       return ERROR_INVALID_ARGS;
//   }

//   header[1] = (dataLength >> 16) & 0xFF;
//   header[2] = (dataLength >> 8) & 0xFF;
//   header[3] = (dataLength >> 0) & 0xFF;
//   fprintf(file, header);

//   if (customResolution) {
//     header[0] = (resolution >> 24) & 0xFF;
//     header[1] = (resolution >> 16) & 0xFF;
//     header[2] = (resolution >> 8) & 0xFF;
//     header[3] = (resolution >> 0) & 0xFF;
//     fprintf(file, header);
//   }
//   return ERROR_SUCCESS;
// }
