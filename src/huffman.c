#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../include/huffman.h"

void insere_element_arbre(uint16_t code, uint8_t element, uint8_t profondeur,struct arbre* arbre){/*insère un élément(symbôle) dans l'arbre*/
    struct noeud* curr_noeud = arbre -> premier_noeud;/*initialisation de curr_coeud à la racine*/
    uint8_t curr_bit ;/*on initialise le bit de poids fort(premier lu)*/
    for (uint8_t i = profondeur; i>0; i--){/*on parcourt le code binaire de l'élément*/
        curr_bit = (code >> (i-1)) & 1;/*on récupère le bit suivant du plus fort au plus petit */
        if (curr_bit == 0){ /*si le bit est 0, on va à gauche*/
            if (curr_noeud -> fils0 == NULL){/*si le fils gauche n'existe pas, on le crée*/
                curr_noeud -> fils0 = malloc(sizeof(struct noeud));
                curr_noeud -> fils0 -> fils0 = NULL;
                curr_noeud -> fils0 -> fils1 = NULL;
            }
            curr_noeud = curr_noeud -> fils0;/*on tourne à gauche*/
        }
        else {/*sinon(curr_bit =1), on va à droite*/
            if (curr_noeud -> fils1 == NULL){/*si le fils droit n'existe pas, on le crée*/
                curr_noeud -> fils1 = malloc(sizeof(struct noeud));
                curr_noeud -> fils1 -> fils0 = NULL;
                curr_noeud -> fils1 -> fils1 = NULL;
            }
            curr_noeud = curr_noeud -> fils1;/*on tourne à droite*/
        }
    }
    curr_noeud -> valeur = element;/*on insère l'élément dans le noeud au bon endroit*/
}



struct arbre* construit_arbre(uint8_t* liste){/*construit l'arbre de Huffman à partir de la liste:
 2 premier indices:c'est la taille, ensuite si c'est ac ou un dc,
  ensuite le nb de symbole par profondeurs,
   puis les symboles*/
    struct arbre *arbre1 = malloc(sizeof(struct arbre)); /*on crée l'arbre*/
    struct noeud *premier_noeud = malloc(sizeof(struct noeud));
    arbre1 -> premier_noeud = premier_noeud;
    arbre1 -> premier_noeud->fils0 = NULL;
    arbre1 -> premier_noeud->fils1 = NULL;
    uint16_t curr_code = 0;/*on initialise le code binaire à 0*/
    uint8_t curr_elem = 0;/*on initialise le nombre d'éléments insérés à 0*/
    for (uint8_t i = 2; i<18; i++){/*on parcourt les profondeurs*/
        for (uint8_t j = 0; j<liste[i]; j++){/*on parcourt les éléments de la profondeur*/
            insere_element_arbre(curr_code,liste[18+curr_elem],i-1,arbre1);/*on insère l'élément dans l'arbre*/
            curr_code++;
            curr_elem++;
        }
        curr_code = curr_code << 1;
    }
    return arbre1;
}



// int main(){
//     uint8_t arbre[] = {1,0,0,2,1,2,7,0,0,0,0,0,0,0,0,0,0,0,23,24,21,8,25,0,9,19,35,40,41,55};
//     struct arbre *arbre1 = construit_arbre(arbre);
//     return 0;
// }
