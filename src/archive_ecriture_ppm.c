#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../include/YCbCr2RGB.h"
#include "../include/ecriture_ppm.h"

/*attention, rajouter les .h des structures, notamment YcbCr2RGB*/

// struct pixel_rgb /*structure de données d'un pixel au format RGB*/
// {
// uint32_t R;
// uint32_t G;
// uint32_t B;
// };


/*d'abord invader :*/

// void invader_ppm(struct pixel_rgb** bloc) {
// FILE *fichier = fopen("invader.ppm", "w"); /*on ouvre le fichier*/
// fprintf(fichier, "P1\n"); /*on écrit le type de fichier*/
// fprintf(fichier, "8 8\n"); /*on écrit la taille de l'image*/
// for (int i = 0 ; i < 8; i++) { 
// for (int j = 0; j < 8; j++) {/* on parcourt le bloc*/
// struct pixel_rgb pixel = bloc[i][j]; 
// if (pixel.R > 0) {
// fprintf(fichier, "0 ");/*si le pixel est noir, on écrit 0*/
// }
// else {
// fprintf(fichier, "1 ");/* sinon (blanc) on écrit 1*/
// }
 
// }
// fprintf(fichier,"\n"); /*on passe à la ligne suivante*/
// }
// fclose(fichier);
// }

/*poupoupidou*/

/*void poupoupidou_ppm(struct pixel_rgb** bloc) {
 FILE *fichier = fopen("poupoupidou.ppm", "w");
 fprintf(fichier, "P2\n");
 fprintf(fichier, "8 8\n");
 for (int i = 0 ; i < 8; i++) {
 for (int j = 0; j < 8; j++) {
 struct pixel_rgb pixel = bloc[i][j];
 if (pixel.R > 0) {
 fprinf(fichier, "0 ");
 }
 else {
 fprintf(fichier, "1 ");
 }
 
 }
 fprintf("\n");
 }
 fclose(fichier)
} */












void old_rgb2ppm(struct pixel_rgb**** blocs, uint32_t largeur, uint32_t hauteur){/*blochl de bloc 88 de pixelrgb, et dimension image*/
    FILE *fichier = fopen("nom_image.ppm", "w");
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






















struct pixel_rgb** old_creer_bloc88_rgb()
{
 struct pixel_rgb** bloc88 = malloc(8*sizeof(struct pixel_rgb*));
 for(int i=0;i<8;i++) {
 bloc88[i] = malloc(8*sizeof( struct pixel_rgb));
 for(int j=0;j<8;j++) {
 bloc88[i][j].R =0;
 bloc88[i][j].G =0;
 bloc88[i][j].B =0;
 }
 }
 return bloc88;
}


// void main() {
// struct pixel_rgb** bloc_R = creer_bloc88_rgb();
// for (int i = 0 ; i < 8; i++) {
// for (int j = 0; j < 8; j++) {
// bloc_R[i][j].R = 255;
// bloc_R[i][j].G = 0;
// bloc_R[i][j].B = 0;
// }
// }

// struct pixel_rgb** bloc_G = creer_bloc88_rgb();
// for (int i = 0 ; i < 8; i++) {
// for (int j = 0; j < 8; j++) {
// bloc_G[i][j].R = 0;
// bloc_G[i][j].G = 255;
// bloc_G[i][j].B = 0;
// }
// }

// struct pixel_rgb** bloc_B = creer_bloc88_rgb();
// for (int i = 0 ; i < 8; i++) {
// for (int j = 0; j < 8; j++) {
// bloc_B[i][j].R = 0;
// bloc_B[i][j].G = 0;
// bloc_B[i][j].B = 255;
// }
// }

// struct pixel_rgb** bloc_noir = creer_bloc88_rgb();
// for (int i = 0 ; i < 8; i++) {
// for (int j = 0; j < 8; j++) {
// bloc_noir[i][j].R = 0;
// bloc_noir[i][j].G = 0;
// bloc_noir[i][j].B = 0;
// }
// }


// struct pixel_rgb**** blocs = malloc(2*sizeof(struct pixel_rgb***));
// blocs[0] = malloc(2*sizeof(struct pixel_rgb**));
// blocs[1] = malloc(2*sizeof(struct pixel_rgb**));
// blocs[0][0] = bloc_R;
// blocs[0][1] = bloc_G;
// blocs[1][0] = bloc_B;
// blocs[1][1] = bloc_noir;


// rgb2ppm(blocs, 10, 12);







// }
