#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../include/YCbCr2RGB.h"

 

/*pour invader, entrée = 1 bloc 8x8 avec les valeurs YCbCr, tableau de tableaux de struct pixel_ycbcr*/

/*Pour invader, Cb=Cr=128*/



// struct pixel_ycbcr  /*structure de données d'un pixel au format YCbCr*/
// {
//     uint32_t Y;
//     uint32_t Cb;
//     uint32_t Cr;
// };
// struct pixel_rgb /*structure de données d'un pixel au format RGB*/
// {
//     uint32_t R;
//     uint32_t G;
//     uint32_t B;
// };


struct pixel_rgb** creer_bloc_rgb(int rouge,int vert, int bleu)  /*création d'un bloc 8x8 de pixels RGB dont les composantes sont rouge,vert,bleu, utile pour les test*/
{
    struct pixel_rgb ** bloc88 = malloc(8*sizeof(struct pixel_rgb*));
    for(int i=0;i<8;i++) {
        bloc88[i] = malloc(8*sizeof(struct pixel_rgb));
        for(int j=0;j<8;j++) {
            bloc88[i][j].R = rouge;
            bloc88[i][j].G = vert;
            bloc88[i][j].B = bleu;
        }
    }
    return bloc88;


}





struct pixel_ycbcr** creer_bloc_ycbcr(uint8_t luminesance, uint8_t crominance_bleu, uint8_t craumience_rouge)  /*création d'un bloc 8x8 de pixels ycbcr dont les composantes sont rouge,vert,bleu, utile pour les test*/
{
    struct pixel_ycbcr ** bloc88 = malloc(8*sizeof(struct pixel_ycbcr*));
    for(int i=0;i<8;i++) {
        bloc88[i] = malloc(8*sizeof(struct pixel_ycbcr));
        for(int j=0;j<8;j++) {
            bloc88[i][j].Y = luminesance;
            bloc88[i][j].Cb = crominance_bleu;
            bloc88[i][j].Cr = craumience_rouge;
        }
    }
    return bloc88;
   
}
struct pixel_ycbcr** blacknwhite(uint8_t** bloc88) /*prend en entrée un bloc 8x8 d'entier et le transforme en YCbCr*/
{
    struct pixel_ycbcr ** nv_bloc88 = creer_bloc_ycbcr(0,128,128); /*on crée un bloc 8x8 de pixels YCbCr*/
    for(int i=0;i<8;i++) {
        for(int j=0;j<8;j++) {
            nv_bloc88[i][j].Y = bloc88[i][j];
       
        }
        free(bloc88[i]);
    }
    free(bloc88);
    return nv_bloc88;
}





struct pixel_rgb ** ycbcr2rgb(struct pixel_ycbcr ** bloc88) /*conversion d'un bloc 8x8 de pixels YCbCr en RGB*/
{
    struct pixel_rgb ** nv_bloc88 = creer_bloc_rgb(0,0,0); /*création d'un bloc 8x8 de pixels RGB*/
    for(int i=0;i<8;i++) {
        for(int j=0;j<8;j++) {/*pour chaque pixel du bloc*/
                

            nv_bloc88[i][j].R = bloc88[i][j].Y + 1.402*(bloc88[i][j].Cr-128);         /*formules de conversion (version simplifée)*/

            nv_bloc88[i][j].G = bloc88[i][j].Y - 0.34414*(bloc88[i][j].Cb-128) - 0.71414*(bloc88[i][j].Cr-128);  /* passage de YCbCr à rgb*/

            nv_bloc88[i][j].B = bloc88[i][j].Y + 1.772*(bloc88[i][j].Cb-128);

            
            if (nv_bloc88[i][j].R>255) {
                nv_bloc88[i][j].R=255;
                }  /* gestion de la saturation */
            else if (nv_bloc88[i][j].R<0) {
                nv_bloc88[i][j].R=0;
                }
            if (nv_bloc88[i][j].G>255) {
                nv_bloc88[i][j].G=255;
                }
            else if (nv_bloc88[i][j].G<0) {
                nv_bloc88[i][j].G=0;
                }
            if (nv_bloc88[i][j].B>255) {
                nv_bloc88[i][j].B=255;
                }
            else if (nv_bloc88[i][j].B<0) {
                nv_bloc88[i][j].B=0;
            }
        }
        free(bloc88[i]);
    }
    free(bloc88);
    return nv_bloc88;
}

struct pixel_ycbcr**** color(uint8_t***** liste_3blocs, uint16_t hauteur , uint16_t largeur){
    struct pixel_ycbcr**** nv_bloc = malloc(sizeof(struct pixel_ycbcr***)*hauteur);
    for (int i = 0 ; i < hauteur ; i++){
        nv_bloc[i] = malloc(sizeof(struct pixel_ycbcr**)*largeur);
        for (int j = 0 ; j < largeur ; j++){
            nv_bloc[i][j] = creer_bloc_ycbcr(0,0,0);
        }
    }
    for (int i = 0 ; i < hauteur ; i++){
        for (int j = 0 ; j < largeur ; j++){
            for (int k = 0 ; k < 8 ; k++){
                for (int l = 0 ; l < 8 ; l++){
                    nv_bloc[i][j][k][l].Y = liste_3blocs[0][i][j][k][l];
                    nv_bloc[i][j][k][l].Cb = liste_3blocs[1][i][j][k][l];
                    nv_bloc[i][j][k][l].Cr = liste_3blocs[2][i][j][k][l];
                }
                free(liste_3blocs[0][i][j][k]);
                free(liste_3blocs[1][i][j][k]);
                free(liste_3blocs[2][i][j][k]);
            }
            free(liste_3blocs[0][i][j]);
            free(liste_3blocs[1][i][j]);
            free(liste_3blocs[2][i][j]);
        }
        free(liste_3blocs[0][i]);
        free(liste_3blocs[1][i]);
        free(liste_3blocs[2][i]);
    }
    free(liste_3blocs[0]);
    free(liste_3blocs[1]);
    free(liste_3blocs[2]);
    free(liste_3blocs);
    return nv_bloc;
}

// void main() {
//     /*test de la fonction ycbcr2rgb*/
//     struct pixel_ycbcr ** bloc88 = creer_bloc_ycbcr(12,128,128);
        
    
//     struct pixel_rgb ** nv_bloc88 = ycbcr2rgb(bloc88);


    
    
// }
