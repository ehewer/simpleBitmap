/*  
 *  simpleBitmap.c
 *  
 *  Created on: February 22, 2020
 *  Author: Elijah Hewer
 *  
 *  for help with BMP file format:
 *  https://en.wikipedia.org/wiki/BMP_file_format
 */

#include <stdio.h>
#include <stdlib.h>
#include "simpleBitmap.h"
// #include "Graphics.h"    // FPGA hardware-accelerated graphics

/*
 * Initializes a BMP struct.
 * Takes a path to the BMP file and a BMP struct as parameters
 */
int initImage(char * filename, BMP * myBMP) {
    FILE* img;
    unsigned char* image_data;

    // open file for reading
    img = fopen(filename, "rb"); 
    if (img == NULL) {
        return 0;
    }

    // read bitmap header and check validity
    fread(&(*myBMP).header, sizeof(BMPHeader), 1, img);
    if ((*myBMP).header.type != 0x4D42) {
        fclose(img);
        return 0;
    }

    // allocate memory for image data
    image_data = (unsigned char*)malloc((*myBMP).header.image_size);
    if (!image_data) {
        free(image_data);
        image_data = NULL;
        fclose(img);
        return 0;
    }
    
    // read the image data
    fseek(img, (*myBMP).header.offset, SEEK_SET);
    fread(image_data, (*myBMP).header.image_size, 1, img);
    if (image_data == NULL) {
        free(image_data);
        image_data = NULL;
        fclose(img);
        return 0;
    }

    (*myBMP).data = image_data;

    // successful initialization
    fclose(img);
    return 1;
}

/*
 * Draws a single-coloured bitmap image on screen given the starting coordinates
 * in the top left corner and a BMP struct.
 * 
 * Only pixels with a value of 1 will be drawn (i.e. background
 * will not be coloured in).
 * 
 * Note: Uses WriteAPixel to draw one pixel at a time.
 */
void drawImage(int topLeftX, int topLeftY, int Colour, BMP* myBMP) {
    int bytes_per_row, valid_bytes;
    int x_val, y_val, pixel_val;
    int offset;
    int byte_data;
    unsigned char mask = 0x01;

    // return if data not initialized
    if ((*myBMP).data == NULL) {
        return;
    }
    
    // determine how many valid bytes are in a single row
    if ((*myBMP).header.width % 8 != 0) {
        valid_bytes = ((*myBMP).header.width / 8) + 1;  // round up
    } else {
        valid_bytes = (*myBMP).header.width / 8;
    }

    // determine how many bytes are in a row by calculating extra padding
    if (valid_bytes % 4 != 0) {
        bytes_per_row = valid_bytes + (4 - (valid_bytes % 4));
    } else {
        bytes_per_row = valid_bytes;
    }

    /*
     * Draws each pixel with a value of 1 by using a bit mask
     * to examine the bits of every valid byte in the image data.
     * 
     * Data is stored using an unsigned char pointer so X and Y
     * coordinates must be converted to an offset and vice versa.
     */
    for (int cur_row = 0; cur_row < (*myBMP).header.height; cur_row++) {
        for (int cur_byte = 0; cur_byte < valid_bytes; cur_byte++) {
            for (int bit = 0; bit < 8; bit++) {
                x_val = (cur_byte * 8) + bit;

                if (x_val >= (*myBMP).header.width) {
                    break;  // reached the last valid bit in the row
                } 
                else {
                    offset = (cur_row * bytes_per_row) + cur_byte;
                    byte_data = *((*myBMP).data + offset);
                    y_val = (*myBMP).header.height - 1 - cur_row;
                    pixel_val = byte_data & (mask << (7 - bit));
                    // printf("X:%i    Y:%i    B:%i\n", x_val + topLeftX, y_val + topLeftY, pixel_val);

                    // only draw pixel if it is within the screen resolution
                    if (pixel_val > 0 && x_val + topLeftX >= 0 && x_val + topLeftX < XRES && y_val + topLeftY >= 0 && y_val + topLeftY < YRES) {
                        // PERFORM ACTION WITH PIXEL HERE
                        // e.g. WriteAPixel(x_val + topLeftX, y_val + topLeftY, Colour); 
                    }
                }
            }
        }
    }
}

/*
 * Frees the memory associated with the image data
 * inside a BMP struct.
 * Returns 0 if data is NULL or previously freed,
 * otherwise returns 1.
 */
int freeImage(BMP* myBMP) {
    if ((*myBMP).data == NULL) {
        return 0;
    }
    else {
        free((*myBMP).data);
        (*myBMP).data = NULL;
        return 1;
    }
}