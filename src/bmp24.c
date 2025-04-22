//
// Created by Adnan Moubarac on 22/04/2025.
//

#include "bmp24.h"

#include <stdio.h>
#include <stdlib.h>

t_pixel ** bmp24_allocateDataPixels (int width, int height) {

    t_pixel ** pixels = malloc(sizeof(t_pixel *) * height);
    if (pixels == NULL) {
        printf("Erreur : allocation hauteur\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(sizeof(t_pixel) * width);
        if (pixels[i] == NULL) {
            printf("Erreur: allocation longueur\n");
        }
    }
    return pixels;
}

void bmp24_freeDataPixels (t_pixel ** pixels, int height) {
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

t_bmp24 * bmp24_allocate (int width, int height, int colorDepth) {

    t_bmp24 * bmp24 = malloc(sizeof(t_bmp24));
    if (bmp24 == NULL) {
        printf("Erreur : allocation hauteur\n");
        return NULL;
    }

    bmp24 ->data = bmp24_allocateDataPixels(width, height);
    if (bmp24->data == NULL) {
        free(bmp24);
        printf("Erreur : allocation des pixels échouée\n");
        return NULL;
    }
    bmp24->width = width;
    bmp24->height = height;
    bmp24->colorDepth = colorDepth;

    return bmp24;

}

void bmp24_free (t_bmp24 * bmp24) {
    if (bmp24 != NULL) {
        bmp24_freeDataPixels(bmp24->data, bmp24->height);
        free(bmp24);
    }
}

void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}

t_bmp24 * bmp24_loadImage (const char * filename) {
    FILE * file = fopen(filename, "rb");

    t_bmp24 *image = malloc(sizeof(t_bmp24));
    if (image == NULL) {
        printf("Erreur : allocation \n");
        fclose(file);
        return NULL;
    }

    int width = image->header_info.width;
    int height = image->header_info.height;
    int colorDepth = image -> colorDepth;

    free(image);
    image = bmp24_allocate(width, height,colorDepth);
    if (image == NULL) {
        printf("Erreur : allocation \n");
        fclose(file);
        return NULL;
    }

    file_rawRead(BITMAP_MAGIC, &image->header, sizeof(t_bmp_header), 1, file);
    file_rawRead(HEADER_SIZE, &image->header_info, sizeof(t_bmp_info), 1, file);

    bmp24_readPixelData(image,file);

    fclose(file);
    return image;

}

void bmp24_saveImage(t_bmp24 * bmp24, const char * filename) {
    FILE * file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erreur : ouverture du fichier \n");
        fclose(file);
        return;
    }
    file_rawWrite(BITMAP_MAGIC, &bmp24->header, sizeof(t_bmp_header), 1, file);
    file_rawWrite(HEADER_SIZE, &bmp24->header_info, sizeof(t_bmp_info), 1, file);

    bmp24_writePixelData(bmp24,file);
    fclose(file);

}


