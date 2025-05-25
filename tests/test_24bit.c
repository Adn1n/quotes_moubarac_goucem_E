//
// Created by Adnan Moubarac on 22/04/2025.
//
#include "utils.h"
#include <bmp24.h>
#include <stdio.h>
#include <stdlib.h>


int main() {


    char chemin [100] = "images/flowers_color.bmp";
    t_bmp24 *img = bmp24_loadImage(chemin);
    char cheminSauvegarde[100] = "images/image_modifiee24.bmp";
    if (!img) {
        printf("Erreur lors du chargement de l'image.\n");
        return 1;
    }

     int choix;
    do {
        printf("\n===== MENU =====\n");
        printf("1. Afficher les informations de l'image\n");
        printf("2. Appliquer un filtre négatif\n");
        printf("3. Modifier la luminosité\n");
        printf("4. Appliquer un seuillage\n");
        printf("5. Concertir l'image en niveau gris\n");
        printf("6. Sauvegarder l'image\n");
        printf("7. Ouvrir l'image\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                bmp24_printInfo(img);
                break;
            case 2:
                bmp24_free(img);
                img = bmp24_loadImage(chemin);
                bmp24_negative(img);
                bmp24_saveImage(img, cheminSauvegarde);
                openImageFile(cheminSauvegarde);
                printf("Filtre négatif appliqué et sauvegardé dans %s.\n", cheminSauvegarde);
                break;

            case 3: {
                int val;
                printf("Entrez une valeur de luminosité (-255 à 255) : ");
                scanf("%d", &val);
                bmp24_brightness(img, val);
                bmp24_saveImage(img, cheminSauvegarde);
                openImageFile(cheminSauvegarde);
                printf("Luminosité modifiée et image sauvegardée dans %s.\n", cheminSauvegarde);
                break;
            }
            case 4: {
                int seuil;
                printf("Valeur de seuillage (0-255) : ");
                scanf("%d", &seuil);
                bmp24_threshold(img, seuil);
                bmp24_saveImage(img, cheminSauvegarde);
                openImageFile(cheminSauvegarde);
                printf("Seuillage appliqué et image sauvegardée dans %s.\n", cheminSauvegarde);
                break;
            }
            case 5 : {
            bmp24_grayscale(img);
            bmp24_saveImage(img, cheminSauvegarde);
            openImageFile(cheminSauvegarde);
            printf("Filtre niveaux de gris appliqué et image sauvegardée dans %s.\n", cheminSauvegarde);
            break;
            }

            case 6: {
                char savePath[100];
                printf("Nom du fichier de sortie : ");
                scanf("%s", savePath);
                bmp24_saveImage(img, savePath);
                printf("Image sauvegardée.\n");
                break;
            }
            case 7:
                openImageFile(cheminSauvegarde);
                break;
            case 8 : {
                FilterType type;
                type = choixFilter(type); // Demande à l'utilisateur quel filtre appliquer
                int taille = 3;
                float **kernel = getKernel(type);

                if (kernel != NULL) {
                    // Crée une image temporaire pour stocker le résultat
                    t_bmp24 *temp = bmp24_allocate(img->width, img->height, img->colorDepth);

                    for (int y = 0; y < img->height; y++) {
                        for (int x = 0; x < img->width; x++) {
                            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, taille);

                        }
                    }

                    // Copie les pixels filtrés dans l'image d'origine
                    for (int y = 0; y < img->height; y++) {
                        for (int x = 0; x < img->width; x++) {
                            img->data[y][x] = temp->data[y][x];
                        }
                    }
                    bmp24_free(temp);
                    bmp24_saveImage(img, cheminSauvegarde);
                    openImageFile(cheminSauvegarde);
                    printf("Filtre de convolution appliqué et sauvegardé dans %s.\n", cheminSauvegarde);

                    // Libère le kernel
                    for (int i = 0; i < taille; i++) {
                        free(kernel[i]);
                    }
                    free(kernel);
                }
                break;
            }
            case 0:
                printf("Fermeture du programme.\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }

    } while (choix != 0);

    bmp24_free(img);
    return 0;

    return 0;
}
