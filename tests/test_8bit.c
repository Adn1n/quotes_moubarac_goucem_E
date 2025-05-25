//
// Created by Adnan Moubarac on 18/04/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

#include "utils.h"

int main(void) {

    char chemin[] = "images/barbara_gray.bmp" ;

    t_bmp8 *img = bmp8_loadImage(chemin);
    char cheminSauvegarde[100] = "images/image_modifiee.bmp";
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
        printf("6. Sauvegarder l'image\n");
        printf("7. Ouvrir l'image\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                bmp8_printInfo(img);
                break;
            case 2:
                bmp8_negative(img);
                bmp8_saveImage(cheminSauvegarde, img);
                openImageFile(cheminSauvegarde);
                printf("Filtre négatif appliqué et sauvegardé dans %s.\n", cheminSauvegarde);
                break;
            case 3: {
                int val;
                printf("Entrez une valeur de luminosité (-255 à 255) : ");
                scanf("%d", &val);
                bmp8_brightness(img, val);
                bmp8_saveImage(cheminSauvegarde, img);
                openImageFile(cheminSauvegarde);
                printf("Luminosité modifiée et image sauvegardée dans %s.\n", cheminSauvegarde);
                break;
            }
            case 4: {
                int seuil;
                printf("Entrez le seuil (0 à 255) : ");
                scanf("%d", &seuil);
                bmp8_threshold(img, seuil);
                bmp8_saveImage(cheminSauvegarde, img);
                openImageFile(cheminSauvegarde);
                printf("Seuillage appliqué et image sauvegardée dans %s.\n", cheminSauvegarde);
                break;
            }
            case 5: {
                FilterType type;
                type = choixFilter(type); // Menu de filtre
                int taille = 3;
                float **kernel = getKernel(type);
                if (kernel != NULL) {
                    bmp8_applyFilter(img, kernel, taille);
                    bmp8_saveImage(cheminSauvegarde, img);
                    openImageFile(cheminSauvegarde);
                    printf("Filtre de convolution appliqué et sauvegardé dans %s.\n", cheminSauvegarde);
                    // Libère kernel
                    for (int i = 0; i < taille; i++)
                        free(kernel[i]);
                    free(kernel);
                }
                break;
            }
            case 6: {
                char savePath[100];
                printf("Nom du fichier de sortie : ");
                scanf("%s", savePath);
                bmp8_saveImage(savePath, img);
                printf("Image sauvegardée.\n");
                break;
            }
            case 7:
                openImageFile(cheminSauvegarde);
                break;
            case 0:
                printf("Fermeture du programme.\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }

    } while (choix != 0);

    bmp8_free(img);
    return 0;
}