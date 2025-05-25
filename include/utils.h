//
// Created by Adnan Moubarac on 19/05/2025.
//

#ifndef UTILS_H
#define UTILS_H

// Définitions des Énumérations //

/**
 * Enumération représentant les types de filtres de convolution disponibles.
 *
 * - box_blur       : Flou moyen (boîte).
 * - gaussian_blur  : Flou gaussien.
 * - outline        : Détection de contours.
 * - emboss         : Effet de relief.
 * - sharpen        : Renforcement des contours.
 */
typedef enum {
    box_blur = 1,
    gaussian_blur = 2,
    outline = 3,
    emboss = 4,
    sharpen = 5
} FilterType;

// Prototypes de fonctions //

/**
 * Ouvre une image à l’aide de la commande système adaptée à l’OS.
 *
 * filename : Chemin du fichier image à ouvrir.
 */
void openImageFile(const char *filename);

/**
 * Permet à l’utilisateur de choisir un filtre dans le menu.
 *
 * type : Variable de type FilterType utilisée pour stocker le choix.
 *
 * Retour : Le filtre sélectionné par l’utilisateur.
 */
int choixFilter(FilterType type);

/**
 * Retourne le noyau de convolution correspondant au filtre sélectionné.
 *
 * type : Type de filtre (valeur de l’énumération FilterType).
 *
 * Retour : Matrice 3x3 allouée dynamiquement contenant les coefficients du filtre.
 */
float ** getKernel(FilterType type);

#endif //UTILS_H
