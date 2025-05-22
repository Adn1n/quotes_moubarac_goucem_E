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

    // Lit les 54 premiers octets du fichier (header BMP)
    fread(p_img->header, sizeof(unsigned char), 54, file);

    // Récupère la largeur à partir du header BMP
    p_img->width = *(unsigned int *)&p_img->header[18];
    // Récupère la hauteur à partir du header BMP
    p_img->height = *(unsigned int *)&p_img->header[22];
    // Récupère la profondeur de couleur à partir du header BMP
    p_img->colorDepth = *(unsigned int *)&p_img->header[28];
    // Récupère la taille des données de l'image à partir du header BMP
    p_img->dataSize = *(unsigned int *)&p_img->header[34];
    if (p_img->dataSize == 0) {
        p_img->dataSize = p_img->width * p_img->height;
    }

    // Lit la table de couleurs (palette) de 1024 octets
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

    // Ecrit la table de couleurs (palette) de 1024 octets
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


// Applique un effet négatif à l'image en inversant chaque pixel (255 - pixel).
void bmp8_negative(t_bmp8 * img) {
    // Parcourt chaque pixel de l'image
    for (int i = 0; i < img->dataSize; i++) {
        // Inverse la valeur du pixel (effet négatif)
        img->data[i] = 255 - img->data[i];
    }
}


// Modifie la luminosité de l'image en ajoutant ou retirant une valeur à chaque pixel.
void bmp8_brightness(t_bmp8 * img, int value) {
    // Parcourt chaque pixel de l'image
    for (int i = 0; i < img->dataSize; i++) {
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


// Applique un seuillage binaire à l'image : pixels >= seuil → blanc (255), sinon noir (0).
void bmp8_threshold(t_bmp8 * img, int threshold) {
    // S'assure que le seuil est compris entre 0 et 255
    if (threshold < 0)
        threshold = 0;
    if (threshold > 255)
        threshold = 255;

    // Parcourt chaque pixel de l'image
    for (int i = 0; i < img->dataSize; i++) {
        // Si la valeur du pixel est supérieure ou égale au seuil, on met blanc (255)
        if (img->data[i] >= threshold)
            img->data[i] = 255;
        // Sinon, on met noir (0)
        else
            img->data[i] = 0;
    }
}

// Permet à l'utilisateur de choisir un type de filtre parmi une liste et retourne le type choisi.
int choixFilter(FilterType type){
    char input[50];            // Stocke la saisie utilisateur
    bool verif_saisie = false; // Indique si la saisie est valide

    // Affiche la liste des filtres disponibles
    printf(" Veuillez choisir un type de filtre parmis les suivants : \n");
    printf("\t 1.box_blur \n");
    printf("\t 2.gaussian_blur \n");
    printf("\t 3.outline \n");
    printf("\t 4.emboss \n");
    printf("\t 5.sharpen \n");
    printf("Votre choix : ");

    // Boucle jusqu'à obtenir une saisie valide
    do {
        // Lit la saisie utilisateur (ligne complète)
        fgets(input, 50, stdin);
        // Supprime le retour à la ligne éventuel
        input[strcspn(input, "\n")] = 0;

        // Vérifie la saisie pour chaque filtre possible
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
            // Affiche un message d'erreur si la saisie est incorrecte
            printf("Votre choix est incorrect \n");
        }

        // Si la saisie n'est pas valide, demande de recommencer
        if (!verif_saisie ) {
            printf("Veuillez resaisir un choix :");
        }
    }while (!verif_saisie);

    // Retourne le type de filtre choisi
    return type;
}

// Retourne le noyau de convolution (matrice) correspondant au type de filtre choisi.
float ** getKernel(FilterType type, int * kernelSize){
    *kernelSize = 3; // Taille du noyau (3x3)

    // Alloue un tableau de pointeurs pour les lignes du noyau
    float ** kernel = malloc(sizeof(float *) * 3);

    // Pour chaque ligne, alloue un tableau de 3 floats
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(sizeof(float) * 3);
    }

    // Remplit le noyau selon le type de filtre choisi
    switch (type) {
        case box_blur: {
            // Noyau de flou moyen (chaque valeur = 1/9)
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    kernel[i][j] = 1.0 / 9.0;
                }
            }
            break;
        }
        case gaussian_blur: {
            // Noyau de flou gaussien
            float values[3][3] = {
                {1.0, 2.0, 1.0},
                {2.0, 4.0, 2.0},
                {1.0, 2.0, 1.0}
            };
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    kernel[i][j] = values[i][j] / 16.0;
                }
            }
            break;
        }
        case outline: {
            // Noyau de détection de contours
            float values[3][3] = {
                {-1,-1,-1},
                {-1,8,-1},
                {-1,-1,-1}
            };
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    kernel[i][j] = values[i][j];
                }
            }
            break;
        }
        case emboss: {
            // Noyau de relief (emboss)
            float values[3][3] = {
                {-2,-1,0},
                {-1,1,1},
                {0,1,2}
            };
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    kernel[i][j] = values[i][j];
                }
            }
            break;
        }
        case sharpen: {
            // Noyau de renforcement (sharpen)
            float values[3][3] = {
                {0,-1,0},
                {-1,5,-1},
                {0,-1,0}
            };
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    kernel[i][j] = values[i][j];
                }
            }
            break;
        }
        default:
            // Affiche un message si le type est inconnu
            printf("Votre choix est incorrect \n");
            return NULL;
    }
    // Retourne le noyau de convolution
    return kernel;
}

// Applique un filtre de convolution à une image BMP 8 bits.
void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize) {
    // Alloue un nouveau tableau pour stocker les pixels filtrés
    unsigned char *newData = malloc(img->dataSize);

    // Récupère les dimensions de l'image
    int width = img->width;
    int height = img->height;

    // Décale pour parcourir les voisins autour du pixel central (noyau 3x3 → offset = 1)
    int offset = kernelSize / 2;

    // Parcourt les pixels internes (en ignorant les bords)
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            // Initialise la somme de convolution pour le pixel (x, y)
            float sum = 0.0;

            // Parcourt les voisins du pixel (x, y) selon le noyau
            for (int i = -offset; i <= offset; i++) {
                for (int j = -offset; j <= offset; j++) {
                    // Calcule la position du voisin dans l'image
                    int neighborX = x + j;
                    int neighborY = y + i;
                    int imageIndex = neighborY * width + neighborX;

                    // Récupère la valeur du pixel voisin
                    unsigned char pixelValue = img->data[imageIndex];

                    // Récupère le coefficient correspondant dans le noyau
                    float coefficient = kernel[i + offset][j + offset];

                    // Ajoute le produit pondéré à la somme
                    sum += pixelValue * coefficient;
                }
            }

            // Clamp la somme pour rester dans les bornes d’un pixel (0–255)
            if (sum < 0)
                sum = 0;
            else if (sum > 255)
                sum = 255;

            // Stocke le résultat dans le tableau temporaire
            newData[x + y * width] = (unsigned char) sum;
        }
    }

    // Remplace les données d'origine par les nouvelles données filtrées
    memcpy(img->data, newData, img->dataSize);

    // Libère la mémoire allouée au tableau temporaire
    free(newData);
}



