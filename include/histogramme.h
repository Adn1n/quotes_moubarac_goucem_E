//
// Created by Adnan Moubarac on 25/05/2025.
//
#include "bmp8.h"
#include "bmp24.h"
#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H

unsigned int * bmp8_computeHistogram(t_bmp8 * img);

unsigned int * bmp8_computeCDF(unsigned int * hist);

void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq);

void bmp24_equalize(t_bmp24 *img);

#endif //HISTOGRAMME_H
