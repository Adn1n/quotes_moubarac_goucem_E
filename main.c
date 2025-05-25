#include "utils.h"
#include "histogramme.h"
#include "bmp24.h"
#include "bmp8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Programme principal permettant le traitement d'images BMP en 8 bits et 24 bits.
// Selon le type d'image, il propose un menu adapté pour appliquer divers filtres, transformations, et égalisation.

int main(void) {

    char chemin1[] = "images/barbara_gray.bmp" ;
    char chemin2[] = "images/flowers_color.bmp" ;
    char cheminSauvegarde[100] = "images/image_bmp8_modifiee.bmp";

    int choix = 0;
    char chemin[100];

    do {
        printf("Quelle image voulez-vous charger ?\n");
        printf("1. barbara_gray.bmp (niveau de gris - bmp8)\n");
        printf("2. flowers_color.bmp (couleur - bmp24)\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        getchar(); // nettoie le '\n' restant
    } while (choix != 1 && choix != 2);




    if (choix == 1) {
        strcpy(chemin, chemin1);
        // Charge une image en niveaux de gris (BMP 8 bits)
        t_bmp8 *img = bmp8_loadImage(chemin);
        if (!img) {
            printf("Erreur lors du chargement de l'image bmp8.\n");
            return 1;
        }

        int choix;
    do {
        printf("\n===== MENU =====\n");
        printf("1. Afficher les informations de l'image\n");
        printf("2. Appliquer un filtre négatif\n");
        printf("3. Modifier la luminosité\n");
        printf("4. Appliquer un seuillage\n");
        printf("5. Appliquer filtre convultion\n");
        printf("6. Appliquer une égalisation d'histogramme\n");
        printf("7. Sauvegarder l'image \n");
        printf("8. Ouvrir l'image \n");
        printf("9. Restaurer l'image originale\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        t_bmp8 *img_original = NULL;
        // Copie de l'image originale pour pouvoir la restaurer plus tard
        img_original = bmp8_copyImage(img);
        // Sert à contrôler si l'image originale doit être ouverte une seule fois
        int premiere_ouverture = 1;

        switch (choix) {
            case 1:
                // Affiche les informations de l'image
                bmp8_printInfo(img);
                break;
            case 2:
                // Applique un filtre négatif
                bmp8_negative(img);
                bmp8_saveImage(cheminSauvegarde, img);
                openImageFile(cheminSauvegarde);
                printf("Filtre négatif appliqué et sauvegardé dans %s.\n", cheminSauvegarde);
                premiere_ouverture = 0;
                break;
            case 3: {
                // Modifie la luminosité
                int val;
                printf("Entrez une valeur de luminosité (-255 à 255) : ");
                scanf("%d", &val);
                bmp8_brightness(img, val);
                bmp8_saveImage(cheminSauvegarde, img);
                openImageFile(cheminSauvegarde);
                printf("Luminosité modifiée et image sauvegardée dans %s.\n", cheminSauvegarde);
                premiere_ouverture = 0;
                break;
            }
            case 4: {
                // Applique un seuillage
                int seuil;
                printf("Entrez le seuil (0 à 255) : ");
                scanf("%d", &seuil);
                bmp8_threshold(img, seuil);
                bmp8_saveImage(cheminSauvegarde, img);
                openImageFile(cheminSauvegarde);
                printf("Seuillage appliqué et image sauvegardée dans %s.\n", cheminSauvegarde);
                premiere_ouverture = 0;
                break;
            }
            case 5: {
                // Applique un filtre de convolution
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
                premiere_ouverture = 0;
                break;
            }
            case 6: {
                // Applique une égalisation d'histogramme
                unsigned int *hist = bmp8_computeHistogram(img);
                if (!hist) {
                    printf("Erreur lors du calcul de l'histogramme.\n");
                    break;
                }

                unsigned int *cdf = bmp8_computeCDF(hist);
                if (!cdf) {
                    free(hist);
                    printf("Erreur lors du calcul de la CDF.\n");
                    break;
                }

                // Étape de normalisation : calcul de hist_eq à partir de la CDF
                unsigned int *hist_eq = malloc(256 * sizeof(unsigned int));
                if (!hist_eq) {
                    free(hist);
                    free(cdf);
                    printf("Erreur d'allocation de hist_eq.\n");
                    break;
                }

                // Trouve le plus petit CDF non nul (cdf_min)
                unsigned int cdf_min = 0;
                for (int i = 0; i < 256; i++) {
                    if (cdf[i] != 0) {
                        cdf_min = cdf[i];
                        break;
                    }
                }

                int total_pixels = img->width * img->height;

                for (int i = 0; i < 256; i++) {
                    hist_eq[i] = (unsigned int)(((float)(cdf[i] - cdf_min) / (total_pixels - cdf_min)) * 255 + 0.5f);
                }

                bmp8_equalize(img, hist_eq);
                bmp8_saveImage(cheminSauvegarde, img);
                openImageFile(cheminSauvegarde);
                printf("Égalisation appliquée et image sauvegardée dans %s.\n", cheminSauvegarde);
                premiere_ouverture = 0;

                free(hist);
                free(cdf);
                free(hist_eq);

                break;
            }
            case 7: {
                // Sauvegarde l'image modifiée
                char savePath[100];
                printf("Nom du fichier de sortie : ");
                scanf("%s", savePath);
                bmp8_saveImage(savePath, img);
                printf("Image sauvegardée.\n");
                break;
            }
            case 8:{
                // Ouvre l'image (originale ou modifiée)
                if (premiere_ouverture) {
                    openImageFile(chemin); // ouvre l’image originale une seule fois
                    premiere_ouverture = 0;
                } else {
                    openImageFile(cheminSauvegarde); // ouvre l’image modifiée ensuite
                }
            }
            case 9: {
                // Restaure l'image originale
                bmp8_free(img);
                img = bmp8_copyImage(img_original);
                bmp8_saveImage(cheminSauvegarde, img);
                openImageFile(cheminSauvegarde);
                printf("Image restaurée et sauvegardée dans %s.\n", cheminSauvegarde);
                break;
            }
            case 0:
                // Quitte le programme
                printf("Fermeture du programme.\n");
                break;
            default:
                // Gère les choix invalides
                printf("Choix invalide. Veuillez réessayer.\n");
        }

    } while (choix != 0);
        // Appelle ici menu_bmp8(img);
        // Libère la mémoire allouée à l'image avant de quitter
        bmp8_free(img);



    } else {
        strcpy(chemin, chemin2);
        // Charge une image couleur (BMP 24 bits)
        t_bmp24 *img = bmp24_loadImage(chemin);
        if (!img) {
            printf("Erreur lors du chargement de l'image bmp24.\n");
            return 1;
        }
        t_bmp8 *img_original = NULL;
        // Copie de l'image originale pour pouvoir la restaurer plus tard
        img_original = bmp8_copyImage(img);
        // Sert à contrôler si l'image originale doit être ouverte une seule fois
        int premiere_ouverture = 1;
        int choix;
    do {
        printf("\n===== MENU =====\n");
        printf("1. Afficher les informations de l'image\n");
        printf("2. Appliquer un filtre négatif\n");
        printf("3. Modifier la luminosité\n");
        printf("4. Appliquer un seuillage\n");
        printf("5. Convertir l'image en niveau gris\n");
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
                // Applique un filtre négatif
                bmp24_free(img);
                img = bmp24_loadImage(chemin);
                bmp24_negative(img);
                bmp24_saveImage(img, cheminSauvegarde);
                openImageFile(cheminSauvegarde);
                printf("Filtre négatif appliqué et sauvegardé dans %s.\n", cheminSauvegarde);
                break;

            case 3: {
                // Modifie la luminosité
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
                // Applique un seuillage
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
                // Applique un filtre de convolution
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
            case 7:
                // Applique une égalisation d'histogramme
                bmp24_equalize(img);
                bmp24_saveImage(img, cheminSauvegarde);
                openImageFile(cheminSauvegarde);
                printf("Image 24 bits égalisée.\n");
                break;
            case 8 : {
                // Sauvegarde l'image
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
                // Restaure l'image originale
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
                // Gère les choix invalides
                printf("Choix invalide. Veuillez réessayer.\n");
        }

    } while (choix != 0);

    // Libère la mémoire allouée à l'image avant de quitter
    bmp24_free(img);
        // Libère la mémoire allouée à l'image avant de quitter
        bmp24_free(img);
    }


    return 0;
}
