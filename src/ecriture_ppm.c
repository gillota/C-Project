#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../include/YCbCr2RGB.h"
#include "../include/ecriture_ppm.h"


void rgb2ppm(struct pixel_rgb**** blocs, uint32_t largeur, uint32_t hauteur, char* nom_fichier){/*blochl de bloc 88 de pixelrgb, et dimension image*/
    FILE *fichier = fopen(nom_fichier, "w");
    fprintf(fichier, "P3\n");
    fprintf(fichier, "%u %u\n", largeur, hauteur);
    fprintf(fichier, "255\n");
    uint16_t l = largeur/8 + ((largeur % 8) != 0); /*recuperation du nb de blocs par longeur*/
    uint16_t h = hauteur/8 + ((hauteur % 8) != 0); /*recuperation du nb de blocs par hauteur*/
    for (int indice_ligne_bloc = 0; indice_ligne_bloc < h ; indice_ligne_bloc++){ 
        for (int grande_ligne = 0 ; grande_ligne < 8 ; grande_ligne++){/*ligne_bloc = la ou on en est dans la grande ligne de blocs*/
            for (int indice_colonne_bloc = 0 ; indice_colonne_bloc < l ; indice_colonne_bloc++) {
                for (int indice_dans_ligne = 0 ; indice_dans_ligne < 8 ; indice_dans_ligne++){
                    struct pixel_rgb** bloc = blocs[indice_ligne_bloc][indice_colonne_bloc]; /*bloc dans lequel on se situe*/
                    struct pixel_rgb pixel = bloc[grande_ligne][indice_dans_ligne]; /*on ecrit le pixel qui correspond*/
                    if ((indice_colonne_bloc)*8+indice_dans_ligne < largeur && (indice_ligne_bloc)*8+grande_ligne < hauteur) {
                        fprintf(fichier, "%u ", pixel.R);
                        fprintf(fichier, "%u ", pixel.G);
                        fprintf(fichier, "%u ", pixel.B);
                        }
                    }
                }
            }
        fprintf(fichier,"\n");
        }
    fclose(fichier);
} 