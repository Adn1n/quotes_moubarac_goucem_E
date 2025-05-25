/**
bmp8.h
Déclarations des structures et fonctions pour le traitement d'images BMP 8 bits.

Ce fichier contient les définitions des structures de données utilisées pour représenter une image BMP
en niveau de gris (8 bits) ainsi que les prototypes des fonctions de chargement, sauvegarde,
traitement d'image (négatif, luminosité, seuillage, filtres de convolution, égalisation).

Cylia Goucem

Projet de traitement d'images en C
*/
// FICHIER HEADER
//
// Définition structures, types, et prototypes de fonctions.

#ifndef BMP8_H
#define BMP8_H

// Définitions des constantes //



// Définitions des structures //
typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
} t_bmp8;





/// Prototypes des fonctions //


/**
 * Charge une image BMP 8 bits à partir d’un fichier.
 * filename Nom du fichier image à charger.
 * Pointeur vers une structure t_bmp8 représentant l’image, ou NULL en cas d’échec.
 */
t_bmp8 * bmp8_loadImage(const char * filename);


/**
 * Sauvegarde une image BMP 8 bits dans un fichier.
 *
 * filename : Nom du fichier de sortie.
 * img      : Pointeur vers l’image à sauvegarder.
 */
void bmp8_saveImage(const char * filename, t_bmp8 * img);

/**
 * Libère la mémoire allouée pour une image BMP 8 bits.
 *
 * img : Pointeur vers l’image à libérer.
 */
void bmp8_free(t_bmp8 * img);


/**
 * Affiche les informations générales d’une image BMP 8 bits.
 *
 * img : Pointeur vers l’image dont les informations seront affichées.
 */
void bmp8_printInfo(t_bmp8 * img);

/**
 * Applique un filtre négatif sur une image BMP 8 bits.
 *
 * img : Pointeur vers l’image à modifier.
 */
void bmp8_negative(t_bmp8 * img);


/**
 * Modifie la luminosité d’une image BMP 8 bits.
 *
 * img   : Pointeur vers l’image à modifier.
 * value : Valeur à ajouter à chaque pixel (peut être négative).
 */
void bmp8_brightness(t_bmp8 * img, int value);


/**
 * Applique un seuillage à une image BMP 8 bits.
 *
 * img       : Pointeur vers l’image à traiter.
 * threshold : Seuil à utiliser (entre 0 et 255).
 */
void bmp8_threshold(t_bmp8 * img, int threshold);


/**
 * Applique un filtre de convolution à une image BMP 8 bits.
 *
 * img        : Pointeur vers l’image à filtrer.
 * kernel     : Matrice de convolution.
 * kernelSize : Taille du noyau (ex : 3 pour 3x3).
 */
void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize);


/**
 * Alloue dynamiquement une structure t_bmp8.
 *
 * width      : Largeur de l’image.
 * height     : Hauteur de l’image.
 * colorDepth : Profondeur de couleur (doit être 8).
 *
 * Retour : Pointeur vers la structure allouée, ou NULL en cas d’échec.
 */
t_bmp8 *bmp8_allocate(int width, int height, int colorDepth);


/**
 * Réalise une copie complète d’une image BMP 8 bits.
 *
 * src : Image source à copier.
 *
 * Retour : Pointeur vers la copie de l’image.
 */
t_bmp8 *bmp8_copyImage(t_bmp8 *src);

#endif //BMP8_H
