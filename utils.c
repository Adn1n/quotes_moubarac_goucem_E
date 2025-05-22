//
// Created by Adnan Moubarac on 19/05/2025.
//

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


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

