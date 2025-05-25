//
// Programme de traitement d'images BMP 24 bits (couleur).
// Permet d'appliquer des filtres, modifier la luminosité, faire un seuillage, convertir en niveaux de gris,
// appliquer une convolution, égaliser l'histogramme, sauvegarder et restaurer l'image.
//
// Created by Adnan Moubarac on 22/04/2025.
//
#include "utils.h"
#include "histogramme.h"
#include "bmp24.h"
#include <stdio.h>
#include <stdlib.h>


int main() {


    char chemin [100] = "images/flowers_color.bmp";
    t_bmp24 *img = bmp24_loadImage(chemin);
    // Charge une image BMP 24 bits (en couleur) depuis le chemin spécifié
    char cheminSauvegarde[100] = "images/image_modifiee24.bmp";
    if (!img) {
        // Vérifie si l'image a bien été chargée
        printf("Erreur lors du chargement de l'image.\n");
        return 1;
    }
    t_bmp24 *img_original = bmp24_copyImage(img);
    // Copie de l'image originale pour pouvoir la restaurer plus tard

    // Sert à contrôler si l'image originale doit être ouverte une seule fois
    int premiere_ouverture = 1;

    int choix;
    do {
        printf("\n===== MENU =====\n");
        printf("1. Afficher les informations de l'image\n");
        printf("2. Appliquer un filtre négatif\n");
        printf("3. Modifier la luminosité\n");
        printf("4. Appliquer un seuillage\n");
        printf("5. Concertir l'image en niveau gris\n");
        printf("6. Appliquer filtre convultion\n");
        printf("7. Appliquer une égalisation d'histogramme\n");
        printf("8. Sauvegarder l'image\n");
        printf("9. Ouvrir l'image\n");
        printf("10. Restaurer l'image originale\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                // Affiche les informations de l'image
                bmp24_printInfo(img);
                break;
            case 2:
                // Applique un filtre négatif et sauvegarde le résultat
                bmp24_free(img);
                img = bmp24_loadImage(chemin);
                bmp24_negative(img);
                bmp24_saveImage(img, cheminSauvegarde);
                openImageFile(cheminSauvegarde);
                printf("Filtre négatif appliqué et sauvegardé dans %s.\n", cheminSauvegarde);
                break;

            case 3: {
                // Modifie la luminosité de l'image
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
                // Applique un seuillage à l'image
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
                // Convertit l'image couleur en niveaux de gris
                bmp24_grayscale(img);
                bmp24_saveImage(img, cheminSauvegarde);
                openImageFile(cheminSauvegarde);
                printf("Filtre niveaux de gris appliqué et image sauvegardée dans %s.\n", cheminSauvegarde);
                break;
            }

            case 6: {
                // Applique un filtre de convolution sur l'image couleur (ex : flou, netteté, détection de contours)
                FilterType type;
                type = choixFilter(type); // Demande à l'utilisateur quel filtre appliquer
                int taille = 3;
                float **kernel = getKernel(type);

                if (kernel != NULL) {
                    // Crée une image temporaire pour stocker le résultat
                    t_bmp24 *temp = bmp24_allocate(img->width, img->height, img->colorDepth);

                    // Applique la convolution pixel par pixel
                    for (int y = 0; y < img->height; y++) {
                        for (int x = 0; x < img->width; x++) {
                            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, taille);

                        }
                    }

                    // Copie le résultat filtré dans l'image d'origine
                    for (int y = 0; y < img->height; y++) {
                        for (int x = 0; x < img->width; x++) {
                            img->data[y][x] = temp->data[y][x];
                        }
                    }
                    bmp24_free(temp);
                    // Libère le tableau kernel
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
            case 7:
                // Égalise l'histogramme de l'image couleur
                bmp24_equalize(img);
                bmp24_saveImage(img, cheminSauvegarde);
                openImageFile(cheminSauvegarde);
                printf("Image 24 bits égalisée.\n");
                break;
            case 8 : {
                // Sauvegarde l'image avec un nom de fichier donné par l'utilisateur
                char savePath[100];
                printf("Nom du fichier de sortie : ");
                scanf("%s", savePath);
                bmp24_saveImage(img, savePath);
                printf("Image sauvegardée.\n");
                break;
            }
            case 9 : {
                // Ouvre l'image (originale ou modifiée)
                if (premiere_ouverture) {
                    openImageFile(chemin); // ouvre l’image originale une seule fois
                    premiere_ouverture = 0;
                } else {
                    openImageFile(cheminSauvegarde); // ouvre l’image modifiée ensuite
                }
                break;
            }
            case 10: {
                // Restaure l'image d'origine depuis la copie
                bmp24_free(img);
                img = bmp24_copyImage(img_original);
                bmp24_saveImage(img, cheminSauvegarde);
                openImageFile(cheminSauvegarde);
                printf("Image restaurée et sauvegardée dans %s.\n", cheminSauvegarde);
                break;
            }

            case 0:
                // Quitte le programme
                printf("Fermeture du programme.\n");
                break;
            default:
                // Message d'erreur en cas de choix invalide
                printf("Choix invalide. Veuillez réessayer.\n");
        }

    } while (choix != 0);

    // Libère la mémoire allouée à l'image avant de quitter le programme
    bmp24_free(img);
    return 0;

}
