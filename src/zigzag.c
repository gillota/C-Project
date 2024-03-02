/*prend en entrée un vecteur 1x64 et le transforme en bloc 8x8 
selon la methode zig zag*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/zigzag.h"

int** creer_bloc88_zigzag(){
    int** bloc88 = malloc(8*sizeof(int*));
    for(int i=0;i<8;i++) {
        bloc88[i] = malloc(8*sizeof( int));
        for(int j=0;j<8;j++) {
            bloc88[i][j] =0;
        }
    }
    return bloc88;
}


 int** zigzag (int* vecteur) /* prend vecteur 1x64*/{   
    int** bloc88 = creer_bloc88_zigzag(); /*on crée un bloc 8x8*/
    uint32_t compteur = 0; /*compteur pour parcourir le vecteur*/
    bool montee = 1; /*savoir si on monte ou si l'on descend une diagonale*/
    /* on commence par la partie diagonale superieure gauche, on s'arrete à la diagonale centrale*/
    for (int i=0; i<8; i++){ /*longueur diagonale*/   /*partie gauche*/
        for (int j=0; j<i+1; j++){ /*parcours diagonale*/
            bloc88[montee*(i-j)+  !montee*j][!montee*(i-j)+ montee*j] = vecteur[compteur];  /*on remplit le bloc, 
                    si en montée, l'indice de la ligne diminue au fur et à mesure que l'indice de la colonne augmente,
                    si en descente (non montée), l'indice de la ligne augmente au fur et à mesure que l'indice de la colonne diminue*/
            compteur++; /*on passe à la case suivante du vecteur*/
            }
        montee = !montee; /*on change de sens */
        }
    compteur = 63;
    /*on fait la partie diagonale inferieure droite, on s'arrete avant la diagonale centrale*/
   
   for (int i=7;i>0; i--){    /*partie droite*/
        for (int j=7; j>i-1; j--){ /*parcours diagonale*/
            bloc88[montee*(i-j+7)+  !montee*j][!montee*(i-j+7)+ montee*j] = vecteur[compteur]; /*on remplit le bloc, 
                                                    si en montée, l'indice de la ligne diminue au fur et à mesure que l'indice de la colonne augmente,
                                                    si en descente (non montée), l'indice de la ligne augmente au fur et à mesure que l'indice de la colonne diminue
                                                    on ajoute 7 pour que les indices de la partie droite soient dans la continuité de ceux de la partie gauche*/
            compteur--; /*on passe à la case suivante (en sens inverse) du vecteur*/
            }
        montee = !montee; /*on change de sens */
        }
        free(vecteur);
        return bloc88;
    }

// int** main(){
//     /*test  de la fonction zigzag*/
// int vecteur[64];
// for (uint8_t i=0; i<64; i++){
//     vecteur[i]=i; /*on remplit le vecteur de la forme {0,1,2,3,...63}*/
// }
// int** bloc88 = zigzag(vecteur);
// for (uint8_t i=0; i<8; i++){
//     for (uint8_t j=0; j<8; j++){
//         printf("%d ", bloc88[i][j]);/*on affiche le bloc*/
//     }
//     printf("\n");
// }
// return bloc88;

// }
