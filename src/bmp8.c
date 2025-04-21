//
// Created by Adnan Moubarac on 18/04/2025.
//

#include "bmp8.h"

#include <stdio.h>
#include <stdlib.h>

// Charge une image BMP 8 bits depuis un fichier et retourne un pointeur vers la structure t_bmp8.
t_bmp8 * bmp8_loadImage(const char * filename) {


    FILE * file = fopen(filename, "rb");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier : %s.\n", filename );
        return NULL;
    }


    t_bmp8 * p_img = malloc(sizeof(t_bmp8));
    if (!p_img) {
        printf("Could not allocate memory for the image\n");
        return NULL;
    }

    fread(p_img -> header, sizeof(unsigned char), 54, file);

    p_img->width = *(unsigned int *)&p_img->header[18];
    p_img->height = *(unsigned int *)&p_img->header[22];
    p_img->colorDepth = *(unsigned int *)&p_img->header[28];
    p_img->dataSize = *(unsigned int *)&p_img->header[34];

    return p_img;
}

// Enregistre une image BMP 8 bits dans un fichier à partir des données contenues dans la structure t_bmp8.
void bmp8_saveImage(const char * filename, t_bmp8 * img) {
    FILE * file = fopen(filename, "wb");

    if (!file) {
        printf("Echec de l'écriture dans le fichier");
    }

    fwrite(img -> header, sizeof(unsigned char), 54, file);
    if (fwrite(img -> header, sizeof(unsigned char), 54, file) != 54) {
        printf("Erreur lors de l'écriture du header.\n");
    }

    fwrite(img -> colorTable, sizeof(unsigned char), 1024, file);
    if (fwrite(img -> colorTable, sizeof(unsigned char), 1024, file) != 1024) {
        printf("Erreur lors de l'écriture de la palette.\n");
    }

    fwrite(img -> data, sizeof(unsigned char), img->dataSize, file);
    if (fwrite(img -> data, sizeof(unsigned char), img->dataSize, file) != img->dataSize) {
        printf("Erreur lors de l'écriture des données de l'image.\n");
    }

    fclose(file);

}

// Libère la mémoire allouée pour une image BMP 8 bits.
void bmp8_free(t_bmp8 * img) {
    free(img);
}

// Affiche les informations (largeur, hauteur, profondeur de couleur, taille des données) de l'image.
void bmp8_printInfo(t_bmp8 * img) {
    printf("Image Info :\n");
    printf("\tWidth : %d\n", img -> width);
    printf("\tHeight : %d\n", img -> height);
    printf("\tColor depth : %d\n", img -> colorDepth);
    printf("\tData Size : %d\n", img -> dataSize);
}

// Applique un effet négatif à l'image en inversant chaque pixel (255 - pixel).
void bmp8_negative(t_bmp8 * img) {
    for (int i = 0; i < img -> dataSize; i++) {
        img -> data[i] = 255 - img -> data[i];
    }
}


void bmp8_brightness(t_bmp8 * img, int value) {

    for (int i = 0; i < img -> dataSize; i++) {
        img -> data[i] = img -> data[i] + value;
        if (img -> data[i] > 255) {
            img -> data[i] = 255;
        }
        else if (img -> data[i] < 0) {
            img -> data[i] = 0;
        }
    }
}

void bmp8_threshold(t_bmp8 * img, int threshold) {

    // Vérification de threshold
    if (threshold < 0)
        threshold = 0;
    if (threshold > 255)
        threshold = 255;

    for (int i = 0; i < img -> dataSize; i++) {
        if (img -> data[i] >= threshold)
            img -> data[i] = 255;
        else
            img -> data[i] = 0;
    }
}



