//
// Created by Adnan Moubarac on 18/04/2025.
//
// FICHIER HEADER
//
// Définition structures, types, et prototypes de fonctions.

#ifndef BMP8_H
#define BMP8_H

typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
} t_bmp8;


t_bmp8 * bmp8_loadImage(const char * filename);

void bmp8_saveImage(const char * filename, t_bmp8 * img);

void bmp8_free(t_bmp8 * img);

void bmp8_printInfo(t_bmp8 * img);


// Applique un effet négatif à l'image BMP 8 bits.
void bmp8_negative(t_bmp8 * img);

void bmp8_brightness(t_bmp8 * img, int value);


void bmp8_threshold(t_bmp8 * img, int threshold);




typedef enum {
    box_blur = 1,
    gaussian_blur = 2,
    outline = 3,
    emboss = 4,
    sharpen = 5
} FilterType;

int choixFilter(FilterType type);

float ** getKernel(FilterType type);

void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize);

#endif //BMP8_H
