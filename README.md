# Traitement d'image BMP en C

## 1. Contributeurs

Ce projet a été réalisé par :

- **Adnan Moubarac** : Développement principal, architecture, traitement d'image (bmp8 & bmp24), égalisation, filtres de convolution, gestion mémoire.
- **Cylia Goucem** : Développement secondaire, tests, validation fonctionnelle, support de documentation, coordination, suivi de version.

---

## 2. Présentation

**Traitement d'image BMP en C** est une application console permettant de lire, modifier et sauvegarder des images aux formats BMP 8 bits (niveaux de gris) et 24 bits (couleur) en utilisant uniquement la bibliothèque standard du langage C.

---

## 3. Fonctionnalités principales

- Lecture/écriture d’images BMP 8 bits & 24 bits
- Filtres négatif, luminosité, seuillage
- Égalisation d’histogrammes (niveaux de gris & couleur)
- Filtres de convolution : flou, renforcement, contours
- Conversion RVB → niveaux de gris (bmp24)
- Interface utilisateur en ligne de commande 
- Visualisation directe via ouverture de fichiers image
- Restauration rapide de l’image originale

---

## 4. Interface utilisateur

Le programme utilise une interface en ligne de commande structurée autour d’un système de menus interactifs. Lors de l'exécution, l'utilisateur est d'abord invité à choisir une image à traiter parmi une sélection proposée. Deux options sont disponibles :

1. `barbara_gray.bmp` — une image en niveaux de gris (BMP 8 bits)
2. `flowers_color.bmp` — une image couleur (BMP 24 bits)

Une fois l’image sélectionnée, le programme affiche dynamiquement un menu d’actions adapté au type de fichier. Par exemple, pour une image 24 bits, l'utilisateur peut :

- Afficher les métadonnées de l’image (dimensions, taille, profondeur)
- Appliquer un filtre négatif
- Modifier la luminosité globale
- Effectuer un seuillage de l’intensité
- Convertir l’image en niveaux de gris
- Appliquer un filtre de convolution (ex. flou, contours)
- Égaliser l’histogramme
- Sauvegarder l’image modifiée
- L’ouvrir automatiquement dans une visionneuse système
- Restaurer l’image originale (réinitialisation)
- Quitter le programme

Chaque choix est guidé pas à pas, rendant l'application accessible aussi bien aux développeurs qu’aux utilisateurs non spécialistes.

## 5. Problèmes rencontrés

Au cours du développement, plusieurs difficultés techniques ont été identifiées et résolues :

- **Gestion mémoire complexe** : Le traitement des images nécessitait de nombreuses allocations dynamiques (tableaux 2D, buffers). Une mauvaise gestion pouvait provoquer des fuites ou des erreurs de segmentation. Des fonctions dédiées de libération (`*_free`) et de copie (`*_copy`) ont été mises en place pour fiabiliser le code.

- **Chargement et validation des fichiers BMP** : Certains fichiers étaient corrompus ou mal formatés. Une vérification rigoureuse des en-têtes BMP et des offsets de données a été ajoutée pour éviter les crashs.

- **Bugs liés à la manipulation des pixels** : L'utilisation incorrecte des pointeurs lors de l'application des filtres provoquait des effets visuels erronés. Une phase de débogage et des impressions conditionnelles a permis d’identifier les erreurs mémoire.

- **Égalisation de l’histogramme couleur** : Appliquer une égalisation directe sur les canaux RVB créait parfois des déformations de couleur.

- **Débogage difficile en ligne de commande** : L’absence d’interface graphique compliquait la visualisation des résultats. Pour pallier cela, une ouverture automatique de l’image dans une visionneuse système a été intégrée après chaque traitement.


## 6. Améliorations futures

Le projet peut être enrichi par plusieurs fonctionnalités et optimisations supplémentaires :

- **Rotation d’image** : Ajout d’une fonction pour faire pivoter les images de 90°, 180° ou à un angle personnalisé.
- **Rognage** : Implémentation d’un système permettant de sélectionner une région rectangulaire de l’image à extraire.
- **Effet miroir** : Possibilité d’inverser une image horizontalement ou verticalement pour générer des effets de symétrie.
- **Historique des traitements** : Ajout d’un mécanisme de suivi des opérations appliquées pour pouvoir annuler/refaire.
- **Interface graphique** : Une version avec interface utilisateur 2D.
- **Prise en charge d’autres formats** : Élargir la compatibilité avec des formats comme PNG ou JPEG à l’avenir.


## 7. Remarques

Ce projet a offert une opportunité concrète d’explorer les bases du traitement d’image en bas niveau tout en consolidant notre compréhension des pointeurs, de la gestion dynamique de la mémoire.

La rigueur imposée par le format BMP et la gestion manuelle des pixels ont permis d’aborder des problématiques souvent masquées par les bibliothèques haut niveau. Le travail en équipe, la structuration en modules et l’approche itérative ont été essentiels pour assurer la robustesse et la lisibilité du code.

Enfin, le projet a servi de tremplin pour réfléchir à des extensions plus ambitieuses  et se projeter vers une application plus complète et accessible.




