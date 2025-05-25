//
// Created by Adnan Moubarac on 25/05/2025.
//
#include "bmp8.h"
#include "bmp24.h"
#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H

/**
 * Calcule l'histogramme d'une image BMP 8 bits.
 *
 * img : Image BMP 8 bits.
 *
 * Retour : Tableau de 256 entiers représentant la fréquence de chaque niveau de gris.
 */
unsigned int * bmp8_computeHistogram(t_bmp8 * img);

/**
 * Calcule la fonction de répartition cumulée (CDF) à partir d'un histogramme.
 *
 * hist : Histogramme de 256 valeurs.
 *
 * Retour : Tableau de 256 entiers représentant la CDF.
 */
unsigned int * bmp8_computeCDF(unsigned int * hist);

/**
 * Applique une égalisation d'histogramme sur une image BMP 8 bits.
 *
 * img     : Image à modifier.
 * hist_eq : Tableau de transformation pré-calculé (valeurs 0 à 255).
 */
void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq);

/**
 * Applique une égalisation d'histogramme sur une image BMP 24 bits.
 *
 * img : Image couleur BMP 24 bits à traiter (chaque canal est égalisé séparément).
 */
void bmp24_equalize(t_bmp24 *img);

#endif //HISTOGRAMME_H
