

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "../include/quantification.h"


void quantification_inverse(int *vecteur, uint8_t *table) {
    for(uint8_t i=0;i<64;i++) {/*on parcourt les 64 cases du vecteur*/
        vecteur[i] = vecteur[i]*table[i+1]; /*on multiplie chaque case du vecteur par la case correspondante de la table*/
    }/*on prend i+1 parce qu'il y a d'abord l'indice de la table en premier element*/
} 

// void main()
// {      /*test de la quantification inverse*/

//     uint8_t vecteur[65] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//     uint8_t table[64] = {1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//     quantification_inverse(vecteur,table);
//     for(int i=0;i<64;i++) {
//         printf("%d ",vecteur[i]);
//     }
//     printf("\n");
// }