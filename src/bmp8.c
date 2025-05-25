//
// Created by Adnan Moubarac on 18/04/2025.
//

#include "bmp8.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// Charge une image BMP 8 bits depuis un fichier et retourne un pointeur vers la structure t_bmp8.
t_bmp8 * bmp8_loadImage(const char * filename) {
    // Ouvre le fichier en mode binaire lecture
    FILE * file = fopen(filename, "rb");
    if (!file) {
        // Affiche une erreur si le fichier ne peut pas être ouvert
        printf("Erreur lors de l'ouverture du fichier : %s.\n", filename );
        return NULL;
    }

    // Alloue la mémoire pour la structure t_bmp8
    t_bmp8 * p_img = malloc(sizeof(t_bmp8));

    if (!p_img) {
        // Affiche une erreur si l'allocation échoue
        printf("Impossible d'allouer de la mémoire pour l'image\n");
        fclose(file); // On ferme le fichier si on ne peut pas allouer
        return NULL;
    }

    // Lit les 54 premiers octets du fichier et stocke dans header avec le pointeur créer
    fread(p_img->header, sizeof(unsigned char), 54, file);

    // Récupère la largeur à partir du header BMP
    p_img->width = *(unsigned int *)&p_img->header[18]; // On met (unsigned int *) afin de caster comme header est un unsigned char
    // Récupère la hauteur à partir du header BMP
    p_img->height = *(unsigned int *)&p_img->header[22];
    // Récupère la profondeur de couleur à partir du header BMP
    p_img->colorDepth = *(unsigned int *)&p_img->header[28];

    // Récupère la taille des données de l'image à partir du header BMP
    p_img->dataSize = *(unsigned int *)&p_img->header[34];
    if (p_img->dataSize == 0) { // Ou cas sa marche la manipulation est faites manuellement
        p_img->dataSize = p_img->width * p_img->height;
    }

    // Lit la table de couleurs de 1024 octets
    fread(p_img->colorTable, sizeof(unsigned char), 1024, file);

    // Alloue la mémoire pour les données de l'image
    p_img->data = malloc(sizeof(unsigned char) * p_img->dataSize);
    if (!p_img->data) {
        // Affiche une erreur si l'allocation échoue
        printf("Impossible d'allouer de la mémoire pour les données de l'image\n");
        free(p_img);
        fclose(file);
        return NULL;
    }

    // Lit les données de l'image (pixels)
    fread(p_img->data, sizeof(unsigned char), p_img->dataSize, file);

    // Ferme le fichier
    fclose(file);

    // Retourne le pointeur vers la structure t_bmp8 remplie
    return p_img;
}


// Enregistre une image BMP 8 bits dans un fichier à partir des données contenues dans la structure t_bmp8.
void bmp8_saveImage(const char * filename, t_bmp8 * img) {
    // Ouvre le fichier en écriture binaire
    FILE * file = fopen(filename, "wb");
    if (!file) {
        // Affiche une erreur si le fichier ne peut pas être ouvert
        printf("Echec de l'écriture dans le fichier\n");
        return;
    }

    // Ecrit les 54 octets du header BMP
    if (fwrite(img->header, sizeof(unsigned char), 54, file) != 54) {
        printf("Erreur lors de l'écriture du header.\n");
    }

    // Ecrit la table de couleurs de 1024 octets
    if (fwrite(img->colorTable, sizeof(unsigned char), 1024, file) != 1024) {
        printf("Erreur lors de l'écriture de la palette.\n");
    }

    // Ecrit les données de l'image (pixels)
    if (fwrite(img->data, sizeof(unsigned char), img->dataSize, file) != img->dataSize) {
        printf("Erreur lors de l'écriture des données de l'image.\n");
    }
    // Ferme le fichier
    fclose(file);
}


// Libère la mémoire allouée pour une image BMP 8 bits.
void bmp8_free(t_bmp8 * img) {
    // Libère la mémoire des données de l'image si elle existe
    if (img->data) {
        free(img->data);
    }
    // Libère la structure t_bmp8 elle-même
    free(img);
}


// Affiche les informations (largeur, hauteur, profondeur de couleur, taille des données) de l'image.
void bmp8_printInfo(t_bmp8 * img) {
    // Affiche un en-tête d'information
    printf("Image Info :\n");
    // Affiche la largeur de l'image
    printf("\tWidth : %d\n", img->width);
    // Affiche la hauteur de l'image
    printf("\tHeight : %d\n", img->height);
    // Affiche la profondeur de couleur de l'image
    printf("\tColor depth : %d\n", img->colorDepth);
    // Affiche la taille des données de l'image
    printf("\tData Size : %d\n", img->dataSize);
}


// Applique un effet négatif à l'image en inversant chaque pixel
void bmp8_negative(t_bmp8 * img) {
    // Parcourt chaque pixel de l'image
    for (int i = 0; i < img->dataSize; i++) {
        // Inverse la valeur du pixel
        img->data[i] = 255 - img->data[i];
    }
}


// Modifie la luminosité de l'image en ajoutant ou retirant une valeur à chaque pixel.
void bmp8_brightness(t_bmp8 * img, int value) {
    // Parcourt chaque pixel de l'image
    int nb_data = img->dataSize;
    for (int i = 0; i < nb_data; i++) {
        // Ajoute la valeur de luminosité au pixel
        int temp = img->data[i] + value;
        // Si la nouvelle valeur dépasse 255, on la limite à 255
        if (temp > 255) {
            img->data[i] = 255;
        }
        // Si la nouvelle valeur est inférieure à 0, on la limite à 0
        else if (temp < 0) {
            img->data[i] = 0;
        }
        // Sinon, on prend la nouvelle valeur calculée
        else {
            img->data[i] = (unsigned char) temp;
        }
    }
}


// Applique un seuillage binaire à l'image
void bmp8_threshold(t_bmp8 * img, int threshold) {
    // S'assure que le seuil est compris entre 0 et 255
    if (threshold < 0)
        threshold = 0;
    if (threshold > 255)
        threshold = 255;

    // Parcourt chaque pixel de l'image
    int bmp_data_size = img->dataSize;

    for (int i = 0; i < bmp_data_size; i++) {
        // Si la valeur du pixel est supérieure ou égale au seuil, on met blanc (255)
        if (img->data[i] >= threshold)
            img->data[i] = 255;
        // Sinon, on met noir (0)
        else
            img->data[i] = 0;
    }
}



// Applique un filtre de convolution à une image BMP 8 bits.
void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize ) {
    // Alloue un nouveau tableau pour stocker les pixels filtrés
    unsigned char *newData = malloc(img->dataSize);

    // Récupère les dimensions de l'image
    int width = img->width;
    int height = img->height;

    // Décale pour parcourir les voisins autour du pixel central
    int offset = kernelSize / 2; // (noyau 3x3 → offset = 1)

    // Parcourt les pixels internes (en ignorant les bords)
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            // Initialise la somme de convolution pour le pixel (x, y)
            float sum = 0.0;

            // Parcourt les voisins du pixel (x, y) selon le noyau
            for (int i = -offset; i <= offset; i++) {
                for (int j = -offset; j <= offset; j++) {
                    // Calcule la position du voisin dans l'image
                    int px = x + j;
                    int py = y + i;

                    // Récupère la valeur du pixel voisin
                    unsigned char value = img->data[py * width + px];

                    // Récupère le coefficient correspondant dans le noyau
                    float coefficient = kernel[i + 1][j + 1];

                    // Ajoute le produit pondéré à la somme
                    sum += value * coefficient;
                }
            }

            // Clamp la somme pour rester dans les bornes d’un pixel (0–255)
            if (sum < 0)
                sum = 0;
            else if (sum > 255)
                sum = 255;

            // Stocke le résultat dans le tableau temporaire
            newData[x + y * width] = sum;
        }
    }

    // Remplace les données d'origine par les nouvelles données filtrées
    memcpy(img->data, newData, img->dataSize); // (destination, donnée à prendre, nb d'octets)

    // Libère la mémoire allouée au tableau temporaire
    free(newData);

}

t_bmp8 *bmp8_allocate(int width, int height, int colorDepth) {
    t_bmp8 *img = malloc(sizeof(t_bmp8));
    if (!img) return NULL;

    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    img->dataSize = width * height;

    img->data = malloc(sizeof(unsigned char) * img->dataSize);
    if (!img->data) {
        free(img);
        return NULL;
    }

    // Initialise le header et la palette à zéro (si besoin)
    memset(img->header, 0, 54);
    memset(img->colorTable, 0, 1024);

    return img;
}


t_bmp8 *bmp8_copyImage(t_bmp8 *src) {
    if (!src) return NULL;

    t_bmp8 *copy = bmp8_allocate(src->width, src->height, src->colorDepth);
    if (!copy) return NULL;

    // Copie les données pixel par pixel (format 1D)
    for (int y = 0; y < src->height; y++) {
        for (int x = 0; x < src->width; x++) {
            int index = y * src->width + x;
            copy->data[index] = src->data[index];
        }
    }

    // Copie aussi le header et la palette pour être complet
    memcpy(copy->header, src->header, 54);
    memcpy(copy->colorTable, src->colorTable, 1024);

    return copy;
}

