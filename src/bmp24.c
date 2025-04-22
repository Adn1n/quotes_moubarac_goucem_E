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


