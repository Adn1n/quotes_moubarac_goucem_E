//
// Created by Adnan Moubarac on 18/04/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

#include "utils.h"
#include "histogramme.h"
int main(void) {

    char chemin[] = "images/barbara_gray.bmp" ;

    t_bmp8 *img = bmp8_loadImage(chemin);
    t_bmp8 *img_original = NULL;
    char cheminSauvegarde[100] = "images/image_modifiee.bmp";
    if (!img) {
        printf("Erreur lors du chargement de l'image.\n");
        return 1;
    }

    img_original = bmp8_copyImage(img);
    if (!img_original) {
        printf("Erreur lors de la sauvegarde de l'image originale.\n");
        bmp8_free(img);
        return 1;
    }
    int premiere_ouverture = 1;

    int choix;
    do {
        printf("\n===== MENU =====\n");
        printf("1. Afficher les informations de l'image\n");
        printf("2. Appliquer un filtre négatif\n");
        printf("3. Modifier la luminosité\n");
        printf("4. Appliquer un seuillage\n");
        printf("6. Sauvegarder l'image\n");
        printf("7. Ouvrir l'image\n");
        printf("8. Appliquer une égalisation d'histogramme\n");
        printf("9. Restaurer l'image originale\n");
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
                premiere_ouverture = 0;
                break;
            case 3: {
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
                char savePath[100];
                printf("Nom du fichier de sortie : ");
                scanf("%s", savePath);
                bmp8_saveImage(savePath, img);
                printf("Image sauvegardée.\n");
                break;
            }
            case 7:
                if (premiere_ouverture) {
                    openImageFile(chemin); // ouvre l’image originale une seule fois
                    premiere_ouverture = 0;
                } else {
                    openImageFile(cheminSauvegarde); // ouvre l’image modifiée ensuite
                }
            break;

            case 8:{
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
                bmp8_equalize(img, hist_eq);

                free(hist);
                free(cdf);
                free(hist_eq);
                premiere_ouverture = 0;
                break;
            }
            case 9: {
                bmp8_free(img);
                img = bmp8_copyImage(img_original);
                bmp8_saveImage(cheminSauvegarde, img);
                printf("Image restaurée et sauvegardée dans %s.\n", cheminSauvegarde);
                break;
            }

            case 0:
                printf("Fermeture du programme.\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }

    } while (choix != 0);

    bmp8_free(img);
    bmp8_free(img_original);
    return 0;
}