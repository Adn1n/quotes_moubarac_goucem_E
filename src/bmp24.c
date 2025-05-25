//
// Created by Adnan Moubarac on 22/04/2025.
//

#include "bmp24.h"

#include <stdio.h>
#include <stdlib.h>

// Fonction pour allouer dynamiquement un tableau 2D de pixels
t_pixel ** bmp24_allocateDataPixels (int width, int height) {

    // Allocation du tableau de pointeurs vers les lignes de pixels
    t_pixel ** pixels = malloc(sizeof(t_pixel *) * height);
    // Vérifie que l'allocation a réussi
    if (pixels == NULL) {
        // Vérifie si l'allocation a échoué
        printf("Erreur : allocation hauteur\n");
        return NULL;
    }

    // Allouer chaque ligne de pixels
    for (int i = 0; i < height; i++) {
        // Allocation de mémoire pour chaque ligne
        pixels[i] = malloc(sizeof(t_pixel) * width);
        // Vérifie que l'allocation de la ligne a réussi
        if (pixels[i] == NULL) {
            // Si l'allocation échoue, libérer les lignes précédemment allouées
            printf("Erreur: allocation longueur\n");
            // Libère la mémoire déjà allouée en cas d’échec
            for (int j = 0; j < i; j++)
                free(pixels[j]);  // Libère chaque ligne
            free(pixels);        // Libère le tableau de pointeurs
            return NULL;
        }
    }

    //Retourne le tableau de pixels alloué
    return pixels;
}

void bmp24_freeDataPixels (t_pixel ** pixels, int height) {
    // Libère chaque ligne de pixels
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    // Libère le tableau de pointeurs
    free(pixels);
}

t_bmp24 * bmp24_allocate (int width, int height, int colorDepth) {

    // Alloue la structure t_bmp24
    t_bmp24 * bmp24 = malloc(sizeof(t_bmp24));
    // Vérifie que l’allocation a réussi
    if (bmp24 == NULL) {
        printf("Erreur : allocation hauteur\n");
        return NULL;
    }

    // Alloue le tableau 2D de pixels
    bmp24 ->data = bmp24_allocateDataPixels(width, height);
    // Vérifie que l’allocation des pixels a réussi
    if (bmp24->data == NULL) {
        free(bmp24);
        printf("Erreur : allocation des pixels échouée\n");
        return NULL;
    }
    // Initialise les champs de la structure
    bmp24->width = width;
    bmp24->height = height;
    bmp24->colorDepth = colorDepth;

    return bmp24;

}

void bmp24_free (t_bmp24 * bmp24) {
    // Vérifie que l’image n’est pas NULL
    if (bmp24 != NULL) {
        // Libère les pixels
        bmp24_freeDataPixels(bmp24->data, bmp24->height);
        // Libère la structure image
        free(bmp24);
    }
}

void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    // Positionne le pointeur de fichier
    fseek(file, position, SEEK_SET);
    // Lit/écrit dans le fichier
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    // Positionne le pointeur de fichier
    fseek(file, position, SEEK_SET);
    // Lit/écrit dans le fichier
    fwrite(buffer, size, n, file);
}

t_bmp24 * bmp24_loadImage (const char * filename) {
    // Ouvre le fichier en mode lecture binaire
    FILE * file = fopen(filename, "rb");


    t_bmp_header header;
    t_bmp_info header_info;

    // Lit les en-têtes BMP
    file_rawRead(0, &header, sizeof(t_bmp_header), 1, file);
    file_rawRead(sizeof(t_bmp_header), &header_info, sizeof(t_bmp_info), 1, file);

    int width = header_info.width;
    int height = header_info.height;
    int colorDepth = header_info.bits;

    // Alloue la structure pour l'image
    t_bmp24 *image = bmp24_allocate(width, height, colorDepth);
    if (image == NULL) {
        printf("Erreur : allocation image échouée\n");
        fclose(file);
        return NULL;
    }


    image->width = width;
    image->height = height;

    image->header = header;
    image->header_info = header_info;

    // Lit les données de pixels
    bmp24_readPixelData(image,file);

    // Ferme le fichier après lecture
    fclose(file);
    return image;

}

void bmp24_saveImage(t_bmp24 * bmp24, const char * filename) {
    // Ouvre le fichier en écriture binaire
    FILE * file = fopen(filename, "wb");
    // Vérifie que le fichier a été ouvert correctement
    if (file == NULL) {
        printf("Erreur : impossible de créer le fichier '%s'. Vérifiez que le dossier existe.\n", filename);

        return;
    }

    // Écrit les en-têtes BMP
    file_rawWrite(0, &bmp24->header, sizeof(t_bmp_header), 1, file);
    file_rawWrite(sizeof(t_bmp_header), &bmp24->header_info, sizeof(t_bmp_info), 1, file);

    // Écrit les données de pixels
    bmp24_writePixelData(bmp24,file);
    // Ferme le fichier après écriture
    fclose(file);


}

void bmp24_readPixelValue (t_bmp24 * image, int x, int y, FILE * file) {


    uint8_t bgr[3];
    // Convertit la coordonnée Y pour BMP (origine en bas)
    int real_position = image -> height - 1 - y;

    // Calcule la taille d'une ligne en tenant compte de l'alignement
    int row_size = ((image->width * 3 + 3) / 4) * 4; // arrondi au multiple de 4
    int offset = image->header.offset + real_position * row_size + x * 3;

    // Lit les trois composantes de couleur BGR
    file_rawRead(offset, bgr, 1, 3, file);

    t_pixel color;
    color.blue = bgr[0];
    color.green = bgr[1];
    color.red = bgr[2];

    // Stocke la couleur dans la structure image
    image -> data[y][x] = color;
}

void bmp24_readPixelData (t_bmp24 * image, FILE * file) {
    // Parcours chaque pixel de l'image
    for (int y = 0; y < image->height; y++) {
        // Parcours chaque pixel de l'image
        for (int x = 0; x < image->width; x++) {
            // Lit/écrit la valeur d’un pixel
            bmp24_readPixelValue(image,x,y,file);

        }
    }
}

void bmp24_writePixelValue (t_bmp24 * image, int x, int y, FILE * file) {
    t_pixel pixel = image -> data[y][x];

    uint8_t bgr[3];
    bgr[0] = pixel.blue;
    bgr[1] = pixel.green;
    bgr[2] = pixel.red;

    int real_position = image -> height -1 - y;

    int row_size = ((image->width * 3 + 3) / 4) * 4; // arrondi au multiple de 4
    int offset = image->header.offset + real_position * row_size + x * 3;

    file_rawWrite(offset, bgr, 1, 3, file);

}

void bmp24_writePixelData (t_bmp24 * image, FILE * file)
{
    // Parcours chaque pixel de l'image
    for (int y = 0; y < image->height; y++) {
        // Parcours chaque pixel de l'image
        for (int x = 0; x < image->width; x++) {
            // Lit/écrit la valeur d’un pixel
            bmp24_writePixelValue(image,x,y,file);
        }
    }

}

void bmp24_negative (t_bmp24 * img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            // Inverse chaque composante de couleur
            img->data[y][x].red   = 255 - img->data[y][x].red;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].blue  = 255 - img->data[y][x].blue;
        }
    }
}

void bmp24_grayscale (t_bmp24 * img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->data[y][x];
            // Calcule le niveau de gris moyen
            uint8_t gray = (p->red + p->green + p->blue)/3;

            // Affecte le niveau de gris à chaque composante
            p->red = gray;
            p->green = gray;
            p->blue = gray;
        }
    }
}

void bmp24_brightness (t_bmp24 * img, int value) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->data[y][x];

            // Applique l’ajustement de luminosité
            int r = p->red + value;
            int g = p->green + value;
            int b = p->blue + value;

            // Sature les valeurs pour rester entre 0 et 255
            if (r > 255) r = 255;
            if (r < 0) r = 0;

            // Sature les valeurs pour rester entre 0 et 255
            if (g > 255) g = 255;
            if (g < 0) g = 0;

            // Sature les valeurs pour rester entre 0 et 255
            if (b > 255) b = 255;
            if (b < 0) b = 0;

            p->red = r;
            p->green = g;
            p->blue = b;


        }
    }
}

void bmp24_threshold(t_bmp24 * img, int threshold) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->data[y][x];
            // Calcule le niveau de gris
            uint8_t gray = (p->red + p->green + p->blue)/3;
            // Applique le seuillage
            uint8_t bin = gray >= threshold ? 255 : 0;
            // Met à jour chaque composante avec le niveau binaire
            p->red = p->green = p->blue = bin;
        }
    }
}

void bmp24_printInfo(t_bmp24 *img){
    // Affiche un en-tête d'information
    printf("Image Info :\n");
    // Affiche la largeur de l'image
    printf("\tWidth : %d\n", img->header_info.width);
    // Affiche la hauteur de l'image
    printf("\tHeight : %d\n", img->header_info.height);
    // Affiche la profondeur de couleur de l'image
    printf("\tColor depth : %d\n", img->header_info.bits);
    // Affiche la taille des données de l'image
    printf("\tData Size : %d\n", img->header_info.size);

}

t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
    // Calcule le décalage pour centrer le noyau
    int offset = kernelSize / 2;
    float r = 0, g = 0, b = 0;

    // Parcours le noyau autour du pixel
    for (int i = -offset; i <= offset; i++) {
        // Parcours le noyau autour du pixel
        for (int j = -offset; j <= offset; j++) {
            int xi = x + j;
            int yi = y + i;

            // Vérifie que les coordonnées sont valides
            if (xi >= 0 && xi < img->width && yi >= 0 && yi < img->height) {
                t_pixel p = img->data[yi][xi];
                float k = kernel[i + offset][j + offset];
                // Applique le noyau à chaque composante
                r += p.red * k;
                g += p.green * k;
                b += p.blue * k;
            }
        }
    }

    // Sature les valeurs pour rester entre 0 et 255
    if (r < 0) r = 0;
    if (r > 255) r = 255;
    if (g < 0) g = 0;
    if (g > 255) g = 255;
    if (b < 0) b = 0;
    if (b > 255) b = 255;

    t_pixel out;
    out.red = (uint8_t)r;
    out.green = (uint8_t)g;
    out.blue = (uint8_t)b;

    // Retourne le pixel transformé
    return out;
}

t_bmp24 *bmp24_copyImage(t_bmp24 *src) {
    if (!src) return NULL;

    // Alloue une nouvelle structure pour la copie
    t_bmp24 *copy = bmp24_allocate(src->width, src->height, src->colorDepth);
    if (!copy) return NULL;

    // Copie chaque pixel de l’image source
    for (int y = 0; y < src->height; y++) {
        for (int x = 0; x < src->width; x++) {
            copy->data[y][x] = src->data[y][x];
        }
    }

    // Copie les informations de l’en-tête
    copy->header = src->header;
    copy->header_info = src->header_info;

    return copy;
}
