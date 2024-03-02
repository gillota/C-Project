#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/acdc.h"
#include "../include/huffman.h"


uint8_t lire_bit(FILE* fichier){
    if (bit_courant == 0) {
        bit_lu = fgetc(fichier);
        nb_1_suite = 0;
       
        bit_courant = 8;
        }
    bit_courant--;
    uint8_t bit = (bit_lu >> bit_courant) & 1;
    if (bit == 1){
        nb_1_suite++;
        }
    if (nb_1_suite == 8){
        fgetc(fichier);
        }
    return bit;
    }


uint8_t lecture_huffman(FILE* fichier, struct arbre* arbre_huffman){/*prend les cararctere d'un fichier et les decode dans un arbre de huffman*/
    struct noeud* noeud_courant=arbre_huffman->premier_noeud; /*on commence par la racine*/
    uint8_t c;/*inialitsation du caractere 0 ou 1*/
    while ((noeud_courant->fils0!=NULL) || (noeud_courant->fils1!=NULL)){ /*tant que l'on est pas à une feuille de l'arbre de huffman*/
        c=lire_bit(fichier); 
        //printf("j'ai lu un %d\n",c);
        if (c==0){ /*si le bit lu est 0 on descend dans le fils de gauche*/
            noeud_courant=noeud_courant->fils0;
            }
        else if (c==1){ /*si le bit lu est 1 on descend dans le fils de droite*/
            noeud_courant=noeud_courant->fils1;
            }
        }
    return noeud_courant->valeur;   /*on renvoie la valeur stockée dans la feuille*/
    }


uint16_t lecture_indice(FILE* fichier, uint8_t magnitude){ /*apres avoir la magnitude grace à huffman, les prochains bits indique l'indice */
    /*l'indice est codé sur les magnitude +1 bits suivants*/
    uint16_t indice=0;
    for (int i=0; i<magnitude; i++){
        indice=indice<<1;
        indice=indice+lire_bit(fichier);
        }
    return indice;
    }


int acdc(int magnitude, uint16_t indice){/*magnitude >= 0 & indice >= 0 & indice < 2^magnitude*/
    if (indice<1<<(magnitude-1)){ /*partie négative*/
        return (int) indice-(1<<magnitude)+1;
        }
    else{ /*partie positive*/
        return  (int)indice; /* renvoie la valeur de l'AC ou DC */
        }
    }


int* lit_DC_ou_AC(FILE* fichier, struct arbre* arbre_huffman, bool DCAC){
    int valeur=lecture_huffman(fichier, arbre_huffman);/*on lit la valeur de la magnitude(DC) ou magnitude et nb de coeff nul (AC)*/
    if (DCAC==false){ /*si on lit un DC*/
        int *coeff_dc = malloc(sizeof(int));/*on initialise un tableau de taille 1*/
        uint16_t indice=lecture_indice(fichier, valeur);/*on lit l'indice*/
        coeff_dc[0]=acdc(valeur,indice);/*on calcule la valeur du DC*/
        return coeff_dc;
        }
    else{
        if (valeur==0){
            // printf("fin de bloc\n");
            int* fin_bloc = malloc(sizeof(int)*65);
            fin_bloc[0]=64;
            for (int i=1; i<65; i++){
                fin_bloc[i]=0;
                }
            return fin_bloc;
            }
        else if ( valeur== 240){
            // printf("16 0\n");
            int* liste_coeff = malloc(sizeof(int)*17);
            liste_coeff[0]=16;
            for (int i=1; i<17; i++){
                liste_coeff[i]=0;
                }
            return liste_coeff;
            }
        else if ((valeur & 15)==0){
            // printf("erreur\n");
            return NULL;
            }
        else{
            uint8_t nb_coeff_nul= (valeur & 0xF0)/16;/*on recupere le nombre de coeff nul*/
            // printf("nb coeff nul : %d\n",nb_coeff_nul);
            int* liste_coeff = malloc(sizeof(int)*(nb_coeff_nul+2));                                                
            liste_coeff[0]=nb_coeff_nul+1;
            for (int i=1; i<nb_coeff_nul+1; i++){
                liste_coeff[i]=0;
                }
            uint8_t magnitude=valeur & 0x0F;
            uint16_t indice=lecture_indice(fichier, magnitude);
            liste_coeff[nb_coeff_nul+1]=acdc(magnitude,indice);
            return liste_coeff;
            }
        }
    return NULL ;
    }




// void main()
// {
//  for (int magnitude = 3; magnitude < 4; magnitude++)
//     {
        
//         printf(" %d\n",1<<magnitude);
//         for (int indice = 0; indice < 1<<magnitude; indice++)
//         {
//             printf("acdc(%d,%d) = %d    ",magnitude,indice,acdc(magnitude,indice));
//         }
//         printf("\n");
//     }


// }


