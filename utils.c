//
// Created by Adnan Moubarac on 19/05/2025.
//

#include "utils.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


void openImageFile(const char *filename) {
#ifdef _WIN32
    char command[256];
    snprintf(command, sizeof(command), "start %s", filename);
    system(command);
#elif __APPLE__
    char command[256];
    snprintf(command, sizeof(command), "open %s", filename);
    system(command);
#elif __linux__
    char command[256];
    snprintf(command, sizeof(command), "xdg-open %s", filename);
    system(command);
#else
    printf("Plateforme non supportée pour ouverture automatique.\n");
#endif
}

// Fonction qui permet à l'utilisateur de choisir un filtre de convolution
// via un menu interactif avec validation de saisie.
// Permet à l'utilisateur de choisir un type de filtre parmi une liste et retourne le type choisi.
int choixFilter(FilterType type){

    // Boucle de saisie utilisateur jusqu'à obtention d'une entrée valide
    char input[50];            // Stocke la saisie utilisateur
    bool verif_saisie = false; // Indique si la saisie est valide

    // Affiche la liste des filtres disponibles
    printf(" Veuillez choisir un type de filtre parmis les suivants : \n");
    printf("\t 1.box_blur \n");
    printf("\t 2.gaussian_blur \n");
    printf("\t 3.outline \n");
    printf("\t 4.emboss \n");
    printf("\t 5.sharpen \n");
    printf("Votre choix : ");

    // Boucle jusqu'à obtenir une saisie valide
    do {
        // Lit la saisie utilisateur (ligne complète)
        fgets(input, 50, stdin);
        // Supprime le retour à la ligne éventuel
        input[strcspn(input, "\n")] = 0;

        // Si l'utilisateur entre "box_blur" ou "1"
        if (strcmp(input, "box_blur") == 0  || strcmp(input, "1") == 0 || strcmp(input, "1.box_blur") == 0 ){
            type = box_blur;
            verif_saisie = true;
        }
        // Si l'utilisateur entre "gaussian_blur" ou "2"
        else if (strcmp(input,"gaussian_blur") == 0 || strcmp(input,"2") == 0 || strcmp(input,"2.gaussian_blur") == 0 ) {
            type = gaussian_blur;
            verif_saisie = true;
        }
        // Si l'utilisateur entre "outline" ou "3"
        else if (strcmp(input,"outline") == 0 || strcmp(input,"3") == 0 || strcmp(input,"3.outline") == 0 ) {
            type = outline;
            verif_saisie = true;
        }
        // Si l'utilisateur entre "emboss" ou "4"
        else if (strcmp(input,"emboss") == 0 || strcmp(input,"4") == 0 || strcmp(input,"4.emboss") == 0 ) {
            type = emboss;
            verif_saisie = true;
        }
        // Si l'utilisateur entre "sharpen" ou "5"
        else if ( strcmp(input,"sharpen") == 0 || strcmp(input,"5") == 0  || strcmp(input,"5.sharpen")==0) {
            type = sharpen;
            verif_saisie = true;
        }
        else {
            // Affiche un message d'erreur si la saisie est incorrecte
            printf("Votre choix est incorrect \n");
        }

        // Si la saisie n'est pas valide, demande de recommencer
        if (!verif_saisie ) {
            printf("Veuillez resaisir un choix :");
        }
    }while (!verif_saisie);

    // Retourne le filtre sélectionné
    return type;
}




// Retourne le noyau de convolution (matrice) correspondant au type de filtre choisi.
float ** getKernel(FilterType type){

    // Alloue un tableau de pointeurs pour les lignes du noyau
    float ** kernel = malloc(sizeof(float *) * 3);

    // Pour chaque ligne, alloue un tableau de 3 floats
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(sizeof(float) * 3);
    }

    // Remplit le noyau selon le type de filtre choisi
    switch (type) {
        case box_blur: {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    kernel[i][j] = 1.0 / 9.0;
                }
            }
            break;
        }
        case gaussian_blur: {
            float values[3][3] = {
                {1.0, 2.0, 1.0},
                {2.0, 4.0, 2.0},
                {1.0, 2.0, 1.0}
            };
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    kernel[i][j] = values[i][j] / 16.0;
                }
            }
            break;
        }
        case outline: {
            float values[3][3] = {
                {-1,-1,-1},
                {-1,8,-1},
                {-1,-1,-1}
            };
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    kernel[i][j] = values[i][j];
                }
            }
            break;
        }
        case emboss: {
            float values[3][3] = {
                {-2,-1,0},
                {-1,1,1},
                {0,1,2}
            };
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    kernel[i][j] = values[i][j];
                }
            }
            break;
        }
        case sharpen: {
            float values[3][3] = {
                {0,-1,0},
                {-1,5,-1},
                {0,-1,0}
            };
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    kernel[i][j] = values[i][j];
                }
            }
            break;
        }
        default:
            // Affiche un message si le type est inconnu
            printf("Votre choix est incorrect \n");
            return NULL;
    }
    // Retourne le noyau de convolution
    return kernel;
}

