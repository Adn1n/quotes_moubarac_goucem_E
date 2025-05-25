//
// Created by Adnan Moubarac on 19/05/2025.
//

#ifndef UTILS_H
#define UTILS_H

// Définitions des Énumérations //
typedef enum {
    box_blur = 1,
    gaussian_blur = 2,
    outline = 3,
    emboss = 4,
    sharpen = 5
} FilterType;

// Prototypes de fonctions //
void openImageFile(const char *filename);

int choixFilter(FilterType type);

float ** getKernel(FilterType type);

#endif //UTILS_H
