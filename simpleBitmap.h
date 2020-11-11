/*  
 *  simpleBitmap.h
 *  
 *  Created on: February 22, 2020
 *  Author: Elijah Hewer
 *  
 *  for help with BMP file format:
 *  https://en.wikipedia.org/wiki/BMP_file_format
 */

#ifndef IMAGES_H_
#define IMAGES_H_

#include <stdint.h>
#include <stdio.h>

#define XRES 1280   // horizontal resolution
#define YRES 720    // vertical resolution

// pack structure together for reading
#pragma pack(push,1)
typedef struct {
    // Bitmap file header
    uint16_t  type;             // is always "BM", which is 0x4d42
    uint32_t  size;             // file size in bytes
    uint16_t  reserved1;        // reserved (unused)
    uint16_t  reserved2;        // reserved (unused)
    uint32_t  offset;           // bytes from beginning of file to start of pixel data

    // DIB header (info header)
    uint32_t  dib_header_size;  // DIB header size in bytes (will change based on header type)
    int32_t   width;            // image width in pixels
    int32_t   height;           // image height in pixels
                                /*
                                 * Note that this is signed because a positive value
                                 * has the origin in the BOTTOM-LEFT corner, but a negative
                                 * value has the origin in the UPPER-LEFT corner
                                 */
    uint16_t  planes;           // # of colour planes
    uint16_t  bits_ppx;         // # of bits per pixel (colour depth)
    uint32_t  compression;      // type of compression method (0 is no compression)
    uint32_t  image_size;       // image size in bytes
    int32_t   x_res_pxpm;       // horizontal resolution in pixels per meter
    int32_t   y_res_pxpm;       // vertical resolution in pixels per meter
    uint32_t  palette_colors;   // # of colours in the colour palette
    uint32_t  important_colors; // # of important colours
} BMPHeader;
#pragma pack(pop)

typedef struct {
    BMPHeader header;
    unsigned char* data;
} BMP;

// Function Definitions
int initImage(char* filename, BMP * mybmp);
void drawImage(int topLeftX, int topLeftY, int Colour, BMP* myBMP);
int freeImage(BMP* myBMP);

#endif /* IMAGES_H_ */