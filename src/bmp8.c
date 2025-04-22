//
// Created by Adnan Moubarac on 18/04/2025.
//

#include "bmp8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

// Applique un seuillage binaire à l'image : pixels >= seuil → blanc (255), sinon noir (0).
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

int choixFilter(FilterType type){

    char input[50];
    bool verif_saisie = false;

    printf(" Veuillez choisir un type de filtre parmis les suivants : \n");
    printf("\t 1.box_blur \n");
    printf("\t 2.gaussian_blur \n");
    printf("\t 3.outline \n");
    printf("\t 4.emboss \n");
    printf("\t 5.sharpen \n");

    printf("Votre choix : ");


    do {
        fgets(input, 50, stdin);
        input[strcspn(input, "\n")] = 0;  // Supprimer le retour à la ligne

        if (strcmp(input, "box_blur") == 0  || strcmp(input, "1") == 0 || strcmp(input, "1.box_blur") == 0 ){
            type = box_blur;
            verif_saisie = true;
        }
        else if (strcmp(input,"gaussian_blur") == 0 || strcmp(input,"2") == 0 || strcmp(input,"2.gaussian_blur") == 0 ) {
            type = gaussian_blur;
            verif_saisie = true;
        }
        else if (strcmp(input,"outline") == 0 || strcmp(input,"3") == 0 || strcmp(input,"3.outline") == 0 ) {
            type = outline;
            verif_saisie = true;
        }
        else if (strcmp(input,"emboss") == 0 || strcmp(input,"4") == 0 || strcmp(input,"4.emboss") == 0 ) {
            type = emboss;
            verif_saisie = true;
        }
        else if ( strcmp(input,"sharpen") == 0 || strcmp(input,"5") == 0  || strcmp(input,"5.sharpen")==0) {
            type = sharpen;
            verif_saisie = true;
        }
        else {
            printf("Votre choix est incorrect \n");
        }

        if (! verif_saisie ) {
            printf("Veuillez resaisir un choix :");
        }

    }while (!verif_saisie);

    return type;
}

float ** getKernel(FilterType type, int * kernelSize){

    if ((*kernelSize % 2) != 1) {
        printf("Taille incorect");
        return NULL;
    }

    if (type == box_blur) {

    }

}

void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize) {
    unsigned char *newData = malloc(img->dataSize);
    int offset = kernelSize / 2;


    for (int x = 1; x < img -> width - 2; x++) {
        for (int y = 1; y < img -> height - 2; y++) {

        }

    }

}



