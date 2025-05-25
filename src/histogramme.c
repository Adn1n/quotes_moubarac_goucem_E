//
// Created by Adnan Moubarac on 25/05/2025.
//

#include "../include/histogramme.h"
#include "../include/utils.h"
#include "../include/bmp8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned int *bmp8_computeHistogram(t_bmp8 *img) {
    if (!img || !img->data) return NULL;

    // Alloue un tableau pour l'histogramme (256 niveaux de gris)
    unsigned int *hist = malloc(256 * sizeof(unsigned int));
    if (!hist) return NULL;

    // Initialise toutes les cases à 0
    for (int i = 0; i < 256; i++) hist[i] = 0;

    // Parcourt l’image pour remplir l’histogramme
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            uint8_t pixel = img->data[y * img->width + x];
            hist[pixel]++;
        }
    }
    return hist; // Pointeur vers l'histogramme
}

unsigned int *bmp8_computeCDF(unsigned int *hist) {
    if (!hist) return NULL;

    // Alloue un tableau pour la CDF
    unsigned int *cdf = malloc(256 * sizeof(unsigned int));
    if (!cdf) return NULL;

    // Initialise la première valeur de la CDF
    cdf[0] = hist[0];
    // Calcule la CDF en cumulant les valeurs de l’histogramme
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }
    return cdf;
}


void bmp8_equalize(t_bmp8 *img, unsigned int *hist_eq) {
    if (!img || !img->data || !hist_eq) return;

    // Applique la transformation d’égalisation à chaque pixel
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            uint8_t pixel = img->data[y * img->width + x];
            // Remplace l’ancien niveau de gris par le nouveau
            img->data[y * img->width + x] = (uint8_t)hist_eq[pixel];
        }
    }
}

void bmp24_equalize(t_bmp24 *img) {
    if (!img || !img->data) return;

    // Histogrammes pour chaque canal
    unsigned int hist_r[256] = {0}, hist_g[256] = {0}, hist_b[256] = {0};
    unsigned int cdf_r[256], cdf_g[256], cdf_b[256];
    uint8_t hist_eq_r[256], hist_eq_g[256], hist_eq_b[256];

    int total_pixels = img->width * img->height;

    // Histogrammes
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->data[y][x];
            hist_r[p->red]++;
            hist_g[p->green]++;
            hist_b[p->blue]++;
        }
    }


    // Calcule les CDF cumulées pour chaque canal
    cdf_r[0] = hist_r[0];
    cdf_g[0] = hist_g[0];
    cdf_b[0] = hist_b[0];
    for (int i = 1; i < 256; i++) {
        cdf_r[i] = cdf_r[i-1] + hist_r[i];
        cdf_g[i] = cdf_g[i-1] + hist_g[i];
        cdf_b[i] = cdf_b[i-1] + hist_b[i];
    }


    // Recherche le premier CDF non nul (évite la division par zéro)
    unsigned int cdf_min_r = 0, cdf_min_g = 0, cdf_min_b = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf_r[i] != 0) { cdf_min_r = cdf_r[i]; break; }
    }
    for (int i = 0; i < 256; i++) {
        if (cdf_g[i] != 0) { cdf_min_g = cdf_g[i]; break; }
    }
    for (int i = 0; i < 256; i++) {
        if (cdf_b[i] != 0) { cdf_min_b = cdf_b[i]; break; }
    }

    // Calcule les valeurs égalisées pour chaque niveau (formule d’égalisation)
    for (int i = 0; i < 256; i++) {
        hist_eq_r[i] = (uint8_t)(((float)(cdf_r[i] - cdf_min_r) / (total_pixels - cdf_min_r)) * 255 + 0.5f);
        hist_eq_g[i] = (uint8_t)(((float)(cdf_g[i] - cdf_min_g) / (total_pixels - cdf_min_g)) * 255 + 0.5f);
        hist_eq_b[i] = (uint8_t)(((float)(cdf_b[i] - cdf_min_b) / (total_pixels - cdf_min_b)) * 255 + 0.5f);
    }


    // Applique la nouvelle valeur égalisée à chaque pixel
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {

            t_pixel *p = &img->data[y][x];

            p->red   = hist_eq_r[p->red];
            p->green = hist_eq_g[p->green];
            p->blue  = hist_eq_b[p->blue];
        }
    }

    // Affiche un message indiquant que l’égalisation est terminée
    printf("Égalisation des canaux R, G, B terminée.\n");
}