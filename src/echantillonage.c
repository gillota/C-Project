/* on transforme les mcu echantilloné verticalement, horizontalement ou les deux en reprenant les valeurs des Cr pours les remettre dans chaque case du mcu*/
/*fonction a effectuer pour chaque matrice de crominance*/


#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "../include/echantillonage.h"


/*
pour horizontal : il y a une ligne sur deux de vide
pour vertical : il y a une colonne sur deux de vide
pour horizontal et vertical : 1 case sur un carré de 4 de plein
*/


void rempli_horizontal(uint8_t***** liste_3blocs, uint16_t h, uint16_t l){
    uint8_t** bloc88_cb0 = malloc(8*sizeof(uint8_t*));
    uint8_t** bloc88_cr0 = malloc(8*sizeof(uint8_t*));
    for(int k=0;k<8;k++) {
        bloc88_cb0[k] = malloc(8*sizeof(uint8_t));
        bloc88_cr0[k] = malloc(8*sizeof(uint8_t));
    }
    for (int i = 0 ; i < h ; i++) {
        for (int j = 0 ; j < l ; j=j+2) {
            for (int k = 0 ; k < 8 ; k++){
                for (int m = 0 ; m < 8 ; m++){
                    bloc88_cb0[k][m] = liste_3blocs[1][i][j][k][m];
                    bloc88_cr0[k][m] = liste_3blocs[2][i][j][k][m]; /*on a dans bloc88_cb0 (idem cr) les valeurs du bloc donné après échantillonage*/
                }
            }
            for (int k = 0 ; k < 8 ; k++){
                for (int m = 0 ; m < 4 ; m++ ){
                    liste_3blocs[1][i][j][k][2*m] = bloc88_cb0[k][m];
                    liste_3blocs[1][i][j][k][2*m+1] = bloc88_cb0[k][m];
                    
                    liste_3blocs[2][i][j][k][2*m] = bloc88_cr0[k][m];
                    liste_3blocs[2][i][j][k][2*m+1] = bloc88_cr0[k][m];
                }
            }
            for (int k = 0 ; k < 8 ; k++){
                for (int m = 0 ; m < 4 ; m++ ){
                    liste_3blocs[1][i][j+1][k][2*m] = bloc88_cb0[k][m+4];
                    liste_3blocs[1][i][j+1][k][2*m+1] = bloc88_cb0[k][m+4];

                    liste_3blocs[2][i][j+1][k][2*m] = bloc88_cr0[k][m+4];
                    liste_3blocs[2][i][j+1][k][2*m+1] = bloc88_cr0[k][m+4]; 
                }
            }
        }
    }
    for(int k=0;k<8;k++) {
        free(bloc88_cb0[k]);
        free(bloc88_cr0[k]);
    }
    free(bloc88_cb0);
    free(bloc88_cr0);
}

void rempli_vertical(uint8_t***** liste_3blocs, uint16_t h, uint16_t l){
    uint8_t** bloc88_cb0 = malloc(8*sizeof(uint8_t*));
    uint8_t** bloc88_cr0 = malloc(8*sizeof(uint8_t*));
    for(int k=0;k<8;k++) {
        bloc88_cb0[k] = malloc(8*sizeof(uint8_t));
        bloc88_cr0[k] = malloc(8*sizeof(uint8_t));
    }
    for (int i = 0 ; i < h ; i=i+2) {
        for (int j = 0 ; j < l ; j++) {
            for (int k = 0 ; k < 8 ; k++){
                for (int m = 0 ; m < 8 ; m++){
                    bloc88_cb0[k][m] = liste_3blocs[1][i][j][k][m];
                    bloc88_cr0[k][m] = liste_3blocs[2][i][j][k][m]; /*on a dans bloc88_cb0 (idem cr) les valeurs du bloc donné après échantillonage*/
                }
            }
            for (int k = 0 ; k < 4 ; k++){
                for (int m = 0 ; m < 8 ; m++ ){
                    liste_3blocs[1][i][j][2*k][m] = bloc88_cb0[k][m];
                    liste_3blocs[1][i][j][2*k+1][m] = bloc88_cb0[k][m];
                    
                    liste_3blocs[2][i][j][2*k][m] = bloc88_cr0[k][m];
                    liste_3blocs[2][i][j][2*k+1][m] = bloc88_cr0[k][m];
                }
            }
            for (int k = 0 ; k < 4 ; k++){
                for (int m = 0 ; m < 8 ; m++ ){
                    liste_3blocs[1][i+1][j][2*k][m] = bloc88_cb0[k+4][m];
                    liste_3blocs[1][i+1][j][2*k+1][m] = bloc88_cb0[k+4][m];

                    liste_3blocs[2][i+1][j][2*k][m] = bloc88_cr0[k+4][m];
                    liste_3blocs[2][i+1][j][2*k+1][m] = bloc88_cr0[k+4][m]; 
                }
            }
        }
    }
    for(int k=0;k<8;k++) {
        free(bloc88_cb0[k]);
        free(bloc88_cr0[k]);
    }
    free(bloc88_cb0);
    free(bloc88_cr0);
}

void rempli_horizontal_vertical(uint8_t***** liste_3blocs, uint16_t h, uint16_t l){
    uint8_t** bloc88_cb0 = malloc(8*sizeof(uint8_t*));
    uint8_t** bloc88_cr0 = malloc(8*sizeof(uint8_t*));
    for(int k=0;k<8;k++) {
        bloc88_cb0[k] = malloc(8*sizeof(uint8_t));
        bloc88_cr0[k] = malloc(8*sizeof(uint8_t));
    }
    for (int i = 0 ; i < h ; i = i+2) {
        for (int j = 0 ; j < l ; j = j+2) {
            for (int k = 0 ; k < 8 ; k++){
                for (int m = 0 ; m < 8 ; m++){
                    bloc88_cb0[k][m] = liste_3blocs[1][i][j][k][m];
                    bloc88_cr0[k][m] = liste_3blocs[2][i][j][k][m]; /*on a dans bloc88_cb0 (idem cr) les valeurs du bloc donné après échantillonage*/
                }
            }
            for (int k = 0 ; k < 4 ; k++){ /*bloc en haut à gauche*/
                for (int m = 0 ; m < 4 ; m++){
                    liste_3blocs[1][i][j][2*k][2*m] = bloc88_cb0[k][m];
                    liste_3blocs[1][i][j][2*k][2*m+1] = bloc88_cb0[k][m];
                    liste_3blocs[1][i][j][2*k+1][2*m] = bloc88_cb0[k][m];
                    liste_3blocs[1][i][j][2*k+1][2*m+1] = bloc88_cb0[k][m];

                    liste_3blocs[2][i][j][2*k][2*m] = bloc88_cr0[k][m];
                    liste_3blocs[2][i][j][2*k][2*m+1] = bloc88_cr0[k][m];
                    liste_3blocs[2][i][j][2*k+1][2*m] = bloc88_cr0[k][m];
                    liste_3blocs[2][i][j][2*k+1][2*m+1] = bloc88_cr0[k][m];
                }
            }
            for (int k = 0 ; k < 4 ; k++){ /*bloc en haut à droite*/
                for (int m = 0 ; m < 4 ; m++){
                    liste_3blocs[1][i][j+1][2*k][2*m] = bloc88_cb0[k][m+4];
                    liste_3blocs[1][i][j+1][2*k][2*m+1] = bloc88_cb0[k][m+4];
                    liste_3blocs[1][i][j+1][2*k+1][2*m] = bloc88_cb0[k][m+4];
                    liste_3blocs[1][i][j+1][2*k+1][2*m+1] = bloc88_cb0[k][m+4];

                    liste_3blocs[2][i][j+1][2*k][2*m] = bloc88_cr0[k][m+4];
                    liste_3blocs[2][i][j+1][2*k][2*m+1] = bloc88_cr0[k][m+4]; 
                    liste_3blocs[2][i][j+1][2*k+1][2*m] = bloc88_cr0[k][m+4];
                    liste_3blocs[2][i][j+1][2*k+1][2*m+1] = bloc88_cr0[k][m+4]; 
                }
            }
            for (int k = 0 ; k < 4 ; k++){ /*bloc en bas à gauche*/
                for (int m = 0 ; m < 4 ; m++){
                    liste_3blocs[1][i+1][j][2*k][2*m] = bloc88_cb0[k+4][m];
                    liste_3blocs[1][i+1][j][2*k][2*m+1] = bloc88_cb0[k+4][m];
                    liste_3blocs[1][i+1][j][2*k+1][2*m] = bloc88_cb0[k+4][m];
                    liste_3blocs[1][i+1][j][2*k+1][2*m+1] = bloc88_cb0[k+4][m];

                    liste_3blocs[2][i+1][j][2*k][2*m] = bloc88_cr0[k+4][m];
                    liste_3blocs[2][i+1][j][2*k][2*m+1] = bloc88_cr0[k+4][m];
                    liste_3blocs[2][i+1][j][2*k+1][2*m] = bloc88_cr0[k+4][m];
                    liste_3blocs[2][i+1][j][2*k+1][2*m+1] = bloc88_cr0[k+4][m];
                }
            }
            for (int k = 0 ; k < 4 ; k++){ /*bloc en bas à droite*/
                for (int m = 0 ; m < 4 ; m++){
                    liste_3blocs[1][i+1][j+1][2*k][2*m] = bloc88_cb0[k+4][m+4];
                    liste_3blocs[1][i+1][j+1][2*k][2*m+1] = bloc88_cb0[k+4][m+4];
                    liste_3blocs[1][i+1][j+1][2*k+1][2*m] = bloc88_cb0[k+4][m+4];
                    liste_3blocs[1][i+1][j+1][2*k+1][2*m+1] = bloc88_cb0[k+4][m+4];

                    liste_3blocs[2][i+1][j+1][2*k][2*m] = bloc88_cr0[k+4][m+4];
                    liste_3blocs[2][i+1][j+1][2*k][2*m+1] = bloc88_cr0[k+4][m+4];
                    liste_3blocs[2][i+1][j+1][2*k+1][2*m] = bloc88_cr0[k+4][m+4];
                    liste_3blocs[2][i+1][j+1][2*k+1][2*m+1] = bloc88_cr0[k+4][m+4];
                }
            }
        }
    }
    for(int k=0;k<8;k++) {
        free(bloc88_cb0[k]);
        free(bloc88_cr0[k]);
    }
    free(bloc88_cb0);
    free(bloc88_cr0);
}

// void main()
// {
//     uint8_t** bloc = creer_bloc88_ech();
//     uint8_t** bloc2 = creer_bloc88_ech();
//     uint8_t** bloc3 = creer_bloc88_ech();
//     uint8_t** bloc4 = creer_bloc88_ech();
//     int compteur = 0;
//     for (int i=0; i<8; i++)
//    {
//        for (int j=0; j<8; j++)
//     {

//             bloc[i][j] = compteur;
//             compteur++;
//             if (compteur>9)
//             {
//                 compteur = 0;
//             }
       
//     }
//    }
//    bloc2 = horizontal_echantillon(bloc);
// bloc3 = vertical_echantillon(bloc);
// bloc4 = horizontal_vertical_echantillon(bloc);
//    for (int i=0; i<8; i++)
//    {
//        for (int j=0; j<8; j++)
//     {
        
//             printf("%d",bloc2[i][j]); /* test horizontal*/
        
       
//     }
//     printf("\n");
//    }
//    printf("\n");
//    for (int i=0; i<8; i++)
//    {
//        for (int j=0; j<8; j++)
//     {
        
//             printf("%d",bloc3[i][j]); /* test vertical*/
        
       
//     }
//     printf("\n");
//    }
//    printf("\n");
//    for (int i=0; i<8; i++)
//    {
//        for (int j=0; j<8; j++)
//     {
        
//             printf("%d",bloc4[i][j]); /* test horizontal et vertical*/
        
       
//     }
//     printf("\n");
//    }
//    printf("\n");
// }   