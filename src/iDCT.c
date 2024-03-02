#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "../include/iDCT.h"

double C(uint16_t sigma) {
    if( sigma == 0 ){
        return 1/sqrt(2);
    }
    else {
        return 1;
    }
}

uint8_t** creer_bloc_idct()  /*création d'un bloc 8x8*/
{
    uint8_t** bloc = malloc(8*sizeof(uint8_t*));
    for(int i=0;i<8;i++) {
        bloc[i] = malloc(8*sizeof(uint8_t));
    }
    return bloc;
}


uint8_t** iDCT(int **bloc) {

    double valeur_cos[8][8] = {{1.000000, 0.980785, 0.923880, 0.831470, 0.707107, 0.555570, 0.382683, 0.195090}, /*tableau de cosinus pour gagner du temps de calcul*/
                        {1.000000, 0.831470, 0.382683, -0.195090, -0.707107, -0.980785, -0.923880, -0.555570},
                        {1.000000, 0.555570, -0.382683, -0.980785, -0.707107, 0.195090, 0.923880, 0.831470},
                        {1.000000, 0.195090, -0.923880, -0.555570, 0.707107, 0.831470, -0.382683, -0.980785},
                        {1.000000, -0.195090, -0.923880, 0.555570, 0.707107, -0.831470, -0.382683, 0.980785},
                        {1.000000, -0.555570, -0.382683, 0.980785, -0.707107, -0.195090, 0.923880, -0.831470},
                        {1.000000, -0.831470, 0.382683, 0.195090, -0.707107, 0.980785, -0.923880, 0.555570},
                        {1.000000, -0.980785, 0.923880, -0.831470, 0.707107, -0.555570, 0.382683, -0.195090}};
    /* fonction inverse de la DCT */
    uint8_t** bloc2 = creer_bloc_idct();
    for (uint8_t i = 0 ; i < 8 ; i++) { /* on parcourt le bloc */
        for (uint8_t j = 0 ; j < 8 ; j++) {
            double somme_l = 0; 
            for (uint16_t lambda = 0 ; lambda < 8 ; lambda++) {/*première somme*/
                double somme_m = 0;
                for (uint16_t mu = 0 ; mu < 8 ; mu++) { /*deuxième somme*/
                    somme_m = somme_m + C(lambda) * C(mu) * valeur_cos[i][lambda] * valeur_cos[j][mu] * bloc[lambda][mu]; /*calcul de la somme*/
                }
                somme_l = somme_l + somme_m;
            }
            double resultat =(somme_l / 4) ;
            
            resultat= resultat + 128; /*on ajoute 128 (offset)*/
            if (resultat > 255) {
                resultat = 255;/*saturation*/
            }
            if (resultat < 0) {
                resultat = 0; /*saturation*/
            }
            
            bloc2[i][j] = (uint8_t)resultat;
        }
    }
    return bloc2;
}


// int main() {
   
//      int** bloc = malloc(8*sizeof(int*));
//     for(int i=0;i<8;i++) {
//         bloc[i] = malloc(8*sizeof(int));
//     }
//     bloc[0][0] = 124;
//     bloc[0][1] = 0;
//     bloc[0][2] = -284;
//     bloc[0][3] = 0;
//     bloc[0][4] = 0;
//     bloc[0][5] = 0;
//     bloc[0][6] = -20;
//     bloc[0][7] = 0;
//     bloc[1][0] = -6;
//     bloc[1][1] = 0;
//     bloc[1][2] = -293;
//     bloc[1][3] = 0;
//     bloc[1][4] = 106;
//     bloc[1][5] = 0;
//     bloc[1][6] = -131;
//     bloc[1][7] = 0;
//     bloc[2][0] = -333;
//     bloc[2][1] = 0;
//     bloc[2][2] = -117;
//     bloc[2][3] = 0;
//     bloc[2][4] = 69;
//     bloc[2][5] = 0;
//     bloc[2][6] = 154;
//     bloc[2][7] = 0;
//     bloc[3][0] = -202;
//     bloc[3][1] = 0;
//     bloc[3][2] = -96;
//     bloc[3][3] = 0;
//     bloc[3][4] = 25;
//     bloc[3][5] = 0;
//     bloc[3][6] = 4;
//     bloc[3][7] = 0;
//     bloc[4][0] = 128;
//     bloc[4][1] = 0;
//     bloc[4][2] = 284;
//     bloc[4][3] = 0;
//     bloc[4][4] = 255;
//     bloc[4][5] = 0;
//     bloc[4][6] = 20;
//     bloc[4][7] = 0;
//     bloc[5][0] = -90;
//     bloc[5][1] = 0;
//     bloc[5][2] = 19;
//     bloc[5][3] = 0;
//     bloc[5][4] = 125;
//     bloc[5][5] = 0;
//     bloc[5][6] = -481;
//     bloc[5][7] = 0;
//     bloc[6][0] = -138;
//     bloc[6][1] = 0;
//     bloc[6][2] = 26;
//     bloc[6][3] = 0;
//     bloc[6][4] = -167;
//     bloc[6][5] = 0;
//     bloc[6][6] = 244;
//     bloc[6][7] = 0;
//     bloc[7][0] = 221;
//     bloc[7][1] = 0;
//     bloc[7][2] = -88;
//     bloc[7][3] = 0;
//     bloc[7][4] = -71;
//     bloc[7][5] = 0;
//     bloc[7][6] = -196;
//     bloc[7][7] = 0;





//     // for (uint8_t i = 0 ; i < 8 ; i++) {
//     //     for (uint8_t j = 0 ; j < 8 ; j++) {
//     //         bloc[i][j] = 0;
            
//     //     }
//     // }
//     bloc[0][0] = 1;

//     for (uint8_t i = 0 ; i < 8 ; i++) {
//         for (uint8_t j = 0 ; j < 8 ; j++) {
//             printf("%d ", bloc[i][j]);
//         }
//         printf("\n");
//     }



//     uint8_t** bloc2 = iDCT(bloc);
//     for (uint8_t i = 0 ; i < 8 ; i++) {
//         for (uint8_t j = 0 ; j < 8 ; j++) {
//             printf("%d ", bloc2[i][j]);
//         }
//         printf("\n");
//     }
//     return 0;
// }