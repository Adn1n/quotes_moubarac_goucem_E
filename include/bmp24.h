//
// Created by Adnan Moubarac on 22/04/2025.
//
#include <stdio.h>
#include <stdint.h>

#ifndef BMP24_H
#define BMP24_H

// Définitions des constantes//

// Constantes pour les offsets des champs de l'en-tête BMP
#define BITMAP_MAGIC 0x00 // offset 0
#define BITMAP_SIZE 0x02 // offset 2
#define BITMAP_OFFSET 0x0A // offset 10
#define BITMAP_WIDTH 0x12 // offset 18
#define BITMAP_HEIGHT 0x16 // offset 22
#define BITMAP_DEPTH 0x1C // offset 28
#define BITMAP_SIZE_RAW 0x22 // offset 34


// Constante pour le type de fichier BMP
#define BMP_TYPE 0x4D42 //'BM' en hexadécimal

// Constantes pour les tailles des champs de l'en-tête BMP
#define HEADER_SIZE 0x0E // 14 octets
#define INFO_SIZE 0x28 // 40 octets

// Constantes pour les valeurs de profondeur de couleur
#define DEFAULT_DEPTH 0x18 // 24



// Définitions des structures//
typedef struct __attribute__((__packed__)) {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} t_bmp_header;

typedef struct __attribute__((__packed__)) {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t xresolution;
    int32_t yresolution;
    uint32_t ncolors;
    uint32_t importantcolors;
} t_bmp_info;


typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_pixel;


typedef struct {
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth;
    t_pixel **data;
} t_bmp24;


// Prototypes des Fonctions //

/**
 * Alloue dynamiquement une matrice de pixels pour une image BMP 24 bits.
 *
 * width  : Largeur de l’image.
 * height : Hauteur de l’image.
 *
 * Retour : Pointeur vers une matrice de pixels (t_pixel **).
 */
t_pixel ** bmp24_allocateDataPixels (int width, int height);

/**
 * Libère la mémoire allouée pour la matrice de pixels d’une image BMP 24 bits.
 *
 * pixels : Matrice de pixels à libérer.
 * height : Hauteur de l’image.
 */
void bmp24_freeDataPixels (t_pixel ** pixels, int height);

/**
 * Alloue dynamiquement une structure t_bmp24.
 *
 * width      : Largeur de l’image.
 * height     : Hauteur de l’image.
 * colorDepth : Profondeur de couleur (doit être 24).
 *
 * Retour : Pointeur vers la structure allouée ou NULL si échec.
 */
t_bmp24 * bmp24_allocate (int width, int height, int colorDepth);

/**
 * Libère la mémoire allouée pour une image BMP 24 bits.
 *
 * img : Pointeur vers l’image à libérer.
 */
void bmp24_free (t_bmp24 * img);

/**
 * Lit des octets bruts à une position donnée dans un fichier.
 *
 * position : Offset de lecture dans le fichier.
 * buffer   : Adresse de stockage des données lues.
 * size     : Taille d’un élément à lire.
 * n        : Nombre d’éléments à lire.
 * file     : Pointeur vers le fichier ouvert.
 */
void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file);

/**
 * Écrit des octets bruts à une position donnée dans un fichier.
 *
 * position : Offset d’écriture dans le fichier.
 * buffer   : Adresse des données à écrire.
 * size     : Taille d’un élément à écrire.
 * n        : Nombre d’éléments à écrire.
 * file     : Pointeur vers le fichier ouvert.
 */
void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file);

/**
 * Charge une image BMP 24 bits à partir d’un fichier.
 *
 * filename : Nom du fichier image à charger.
 *
 * Retour : Pointeur vers une structure t_bmp24 représentant l’image, ou NULL si échec.
 */
t_bmp24 * bmp24_loadImage (const char * filename);

/**
 * Sauvegarde une image BMP 24 bits dans un fichier.
 *
 * img      : Image à sauvegarder.
 * filename : Nom du fichier de sortie.
 */
void bmp24_saveImage (t_bmp24 * img, const char * filename);

/**
 * Lit la valeur d’un pixel (x, y) directement depuis le fichier image.
 *
 * image : Pointeur vers la structure image.
 * x, y  : Coordonnées du pixel.
 * file  : Fichier BMP ouvert.
 */
void bmp24_readPixelValue (t_bmp24 * image, int x, int y, FILE * file);

/**
 * Lit toutes les données de pixels depuis un fichier image BMP.
 *
 * image : Pointeur vers la structure image.
 * file  : Fichier BMP ouvert.
 */
void bmp24_readPixelData (t_bmp24 * image, FILE * file);

/**
 * Écrit la valeur du pixel (x, y) dans un fichier image BMP.
 *
 * image : Structure contenant les données.
 * x, y  : Coordonnées du pixel à écrire.
 * file  : Fichier BMP ouvert en écriture.
 */
void bmp24_writePixelValue (t_bmp24 * image, int x, int y, FILE * file);

/**
 * Écrit toutes les données de pixels dans un fichier BMP.
 *
 * image : Structure contenant l’image.
 * file  : Fichier BMP ouvert.
 */
void bmp24_writePixelData (t_bmp24 * image, FILE * file);

/**
 * Applique un filtre négatif à l’image BMP 24 bits.
 *
 * img : Image à modifier.
 */
void bmp24_negative (t_bmp24 * img);

/**
 * Convertit l’image BMP 24 bits en niveaux de gris.
 *
 * img : Image à transformer.
 */
void bmp24_grayscale (t_bmp24 * img);

/**
 * Modifie la luminosité d’une image BMP 24 bits.
 *
 * img   : Image à modifier.
 * value : Valeur à ajouter (positive ou négative).
 */
void bmp24_brightness (t_bmp24 * img, int value);

/**
 * Applique un seuillage sur une image BMP 24 bits.
 *
 * img       : Image à traiter.
 * threshold : Seuil à appliquer (0-255).
 */
void bmp24_threshold(t_bmp24 * img, int threshold);

/**
 * Applique un filtre de convolution à un pixel donné.
 *
 * img        : Image source.
 * x, y       : Coordonnées du pixel à traiter.
 * kernel     : Matrice de convolution.
 * kernelSize : Taille du noyau (doit être impair).
 *
 * Retour : Valeur du nouveau pixel après filtrage.
 */
t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize);

/**
 * Crée une copie complète d’une image BMP 24 bits.
 *
 * src : Image source à copier.
 *
 * Retour : Nouvelle image identique, allouée dynamiquement.
 */
t_bmp24 *bmp24_copyImage(t_bmp24 *src);

/**
 * Affiche les informations générales d’une image BMP 24 bits.
 *
 * img : Image dont les informations seront affichées.
 */
void bmp24_printInfo(t_bmp24 *img);


#endif //BMP24_H
