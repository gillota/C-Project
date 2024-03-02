#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../include/entete.h"

uint8_t next_octet(FILE *fichier)/*lit le prochain octet d'un fichier*/
{
    uint8_t c=fgetc(fichier);/*on lit un octet*/
    /*printf("%u   ",c);*/
    return c;
}




bool is_jfif(FILE *fichier)/*vérifie si le fichier est au bon format */
{
    uint8_t curr_octet = next_octet(fichier);
    for (uint8_t i = 0; i<2; i++){
        curr_octet = next_octet(fichier);
    }
    if (curr_octet != 74){ /* Il n' y a pas écrit J */
        return false;
    }
    curr_octet = next_octet(fichier);
    if (curr_octet != 70){ /* Il n' y a pas écrit F */
        return false;
    }
    curr_octet = next_octet(fichier);
    if (curr_octet != 73){ /* Il n' y a pas écrit I */
        return false;
    }
    curr_octet = next_octet(fichier);
    if (curr_octet != 70){ /* Il n' y a pas écrit F */
        return false;
    }
    curr_octet = next_octet(fichier);
    if (curr_octet != 00){ /* Il n' y a pas écrit \0 */
        return false;
    }
    curr_octet = next_octet(fichier);
    if (curr_octet != 01){ /* Ce n'est pas la bonne version */
        return false;
    }
    curr_octet = next_octet(fichier);
    if (curr_octet != 01){ /* Ce n'est pas la bonne version */
        return false;
    }
    return true;
}


/*char* get_comment(fichier){
    uint8_t curr_octet = next_octet(fichier);
    uint8_t taille = curr_octet;
    curr_octet = next_octet(fichier);
    taille += curr_octet;  
    for (uint8_t i = 0; i<taille; i++){
        curr_octet = next_octet(fichier);
    }
}*/

uint8_t* recup_table_quantif(FILE *fichier) {/*recupere une table de quantification*/
    uint8_t *quantif = malloc(sizeof(uint8_t)*65);
    uint8_t curr_octet = next_octet(fichier); /* On saute la taille */
    curr_octet = next_octet(fichier);
    curr_octet = next_octet(fichier);
    quantif[0] = curr_octet & 15; /* On récupère l'indice */
    for (uint8_t i = 1; i < 65; i++) /* On récupère la table */
    {
        curr_octet = next_octet(fichier);
        quantif[i] = curr_octet;
    }
    return quantif;
}

struct infos_sof0* recup_SOF0(FILE *fichier) {/*recupere les infos (dimensions et début de la liste de composantes) de l'image*/
    struct infos_sof0 *infos = malloc (sizeof(struct infos_sof0));
    uint16_t dimensions[2];
    uint8_t curr_octet = next_octet(fichier); /* On saute la taille */
    curr_octet = next_octet(fichier);
    curr_octet = next_octet(fichier);
    if (curr_octet != 8){
        return NULL;
    }
    curr_octet = next_octet(fichier);
    dimensions[0] = curr_octet*256;
    curr_octet = next_octet(fichier);
    dimensions[0] += curr_octet;
    curr_octet = next_octet(fichier);
    dimensions[1] = curr_octet*256;
    curr_octet = next_octet(fichier);
    dimensions[1] += curr_octet;
    uint8_t n = next_octet(fichier);
    uint8_t **liste_composante = malloc(sizeof(uint8_t*)*n);
    for (uint8_t i = 0; i<n; i++){
        liste_composante[i] = malloc(6);
    }
    for (uint8_t i = 0; i<n; i++){
        curr_octet = next_octet(fichier);
        liste_composante[i][0] = curr_octet;
        curr_octet = next_octet(fichier);
        liste_composante[i][1] = curr_octet & 240;
        liste_composante[i][1] = liste_composante[i][1] / 16 ;
        liste_composante[i][2] = curr_octet & 15;
        curr_octet = next_octet(fichier);
        liste_composante[i][3] = curr_octet;
    }
    infos -> dimensions[0] = dimensions[0];/*hauteur*/
    infos -> dimensions[1] = dimensions[1];/*largeur*/
    infos -> liste_composante = liste_composante;/*liste des composantes*/
    infos -> n = n;/*nombre de composantes*/
    return infos;
}

uint8_t* recup_table_huffman(FILE *fichier) {/*recupere une table de huffman*/
    uint8_t *table = malloc(18);
    uint8_t curr_octet = next_octet(fichier);
    curr_octet = next_octet(fichier);
    curr_octet = next_octet(fichier);
    if ((curr_octet & 224) != 0){/*si les 3 premiers bits sont non nul on aun probleme */
        return NULL;
    }
    table[1] = curr_octet & 16;/*on recupere le type de la table*/
    table[1] = table[1] / 16;
    table[0] = curr_octet & 15;/*on recupere l'indice de la table*/
    uint32_t nb_symboles = 0;
    for(uint8_t i = 2; i<18; i++){ /*on recupere le nombre de symboles de chaque profondeur*/
        curr_octet = next_octet(fichier);
        table[i] = curr_octet;
        nb_symboles += curr_octet;
    }
    uint8_t *nouvelle_table = realloc(table, 18 + nb_symboles);/*on adapte la taille au nombre de symboles*/
    table = nouvelle_table;
    for(uint32_t i = 18; i<nb_symboles+18; i++){ /*on recupere tous les symboles*/
        curr_octet = next_octet(fichier);
        table[i] = curr_octet;
    }
    return table;
}

uint8_t** recup_SOS(FILE *fichier){/*recupere les infos de SOS (correspondances  tables de huffman et composantes)*/
    uint8_t curr_octet = next_octet(fichier); /* On saute la taille */
    curr_octet = next_octet(fichier);
    curr_octet = next_octet(fichier);
    uint8_t n = curr_octet; /* n est le nombre de composantes */
    uint8_t **liste_composante = malloc(sizeof(uint8_t*)*n);
    for (uint8_t i = 0; i<n; i++){
        liste_composante[i] = malloc(3);
    }
    for (uint8_t i = 0; i<n; i++){
        curr_octet = next_octet(fichier);
        liste_composante[i][0] = curr_octet;
        curr_octet = next_octet(fichier);
        liste_composante[i][1] = curr_octet & 240;/*on recupere les indices des tables de huffman*/
        liste_composante[i][1] = liste_composante[i][1] / 16;
        liste_composante[i][2] = curr_octet & 15;
        }
    curr_octet = next_octet(fichier);
    if (curr_octet != 0){/*on test si l'image est  conforme*/
        return NULL;
    }
    curr_octet = next_octet(fichier);
    if (curr_octet != 63){
        return NULL;
    }
    curr_octet = next_octet(fichier);
    if (curr_octet != 0){
        return NULL;
    }
    return liste_composante;
}

void insere_element(struct liste_chainee *chaine, uint8_t *elements, uint8_t place){/*on place un element au bon endroit dans une liste chainées*/
    if (place == 0){
        struct cellule *nv_cell = malloc(sizeof(struct cellule));
        nv_cell -> element = elements;
        nv_cell -> suivant = chaine -> tete;
        chaine -> tete = nv_cell;
    }
    else {
        struct cellule *curr_cell = chaine -> tete;
        struct cellule *prev_cell = curr_cell;
        for (uint8_t i = 0; i<place; i++){
            prev_cell = curr_cell;
            curr_cell = curr_cell -> suivant;
        }
        struct cellule *nv_cell = malloc(sizeof(struct cellule));
        nv_cell -> element = elements;
        nv_cell -> suivant = curr_cell;
        prev_cell -> suivant = nv_cell;
    }
}

uint8_t* recupere_element(struct liste_chainee *liste, uint8_t indice){/*on recupere un element d'une liste chainée en fonction de son indice*/
    struct cellule *curr_cell = liste -> tete;
    for (uint8_t i = 0; i<indice; i++){
        curr_cell = curr_cell -> suivant;
    }
    return curr_cell -> element;
}

uint16_t somme_liste(uint8_t* liste,uint8_t elem1,uint8_t elem_fin){
    uint16_t somme = 0;
    for (uint8_t i = elem1; i<elem_fin; i++){
        somme += liste[i];
    }
    return somme;
}

void affiche_infos(struct liste_chainee *liste){/*affiche les infos sur une liste chainée, aide pour le debug*/
    printf("Les dimensions sont : %u x %u\n\n",liste -> dimensions[0],liste -> dimensions[1]);
    printf("%u ",liste->elements[0]);
    printf("%u ",liste->elements[1]);
    printf("%u ",liste->elements[2]);
    printf("\n\n");
    struct cellule *curr_cell = liste -> tete;
    for (uint8_t i = 0; i<liste -> elements[0]; i++){
        printf("Les infos sur la composante sont : ");
        for (uint8_t j = 0; j<6; j++){
            printf("%u ",curr_cell -> element[j]);
        }
        curr_cell = curr_cell -> suivant;
        printf("\n\n");
    }
    curr_cell = liste -> tete;
    uint8_t nb_a_sauter = liste->elements[0] + liste->elements[1];
    for (uint8_t i = 0; i<nb_a_sauter; i++){
        curr_cell = curr_cell -> suivant;
    }
    for (uint8_t i = 0; i<liste -> elements[2]; i++){
        printf("La table de quantification est : ");
        for (uint8_t j = 0; j<65; j++){
            printf("%u ",curr_cell -> element[j]);
        }
        curr_cell = curr_cell -> suivant;
        printf("\n\n");
    }
    nb_a_sauter = liste->elements[0];
    curr_cell = liste -> tete;
    for (uint8_t i = 0; i<nb_a_sauter; i++){
        curr_cell = curr_cell -> suivant;
    }
    for (uint8_t i = 0; i<liste -> elements[1]; i++){
        uint16_t elem_huff = somme_liste(curr_cell->element,2,18);
        printf("La table de Huffman est : ");
        for (uint8_t j = 0; j<elem_huff+18; j++){
            printf("%u ",curr_cell -> element[j]);
        }
        curr_cell = curr_cell -> suivant;
        printf("\n\n");
    }
}


/*fonction principale*/
struct liste_chainee* decode(FILE *fichier)
{
    struct liste_chainee *entete = malloc(sizeof(struct liste_chainee));/*on initialise l'entete*/
    entete->dimensions[0]=0;
    entete->dimensions[1]=0;
    entete->elements[0]=0;
    entete->elements[1]=0;
    entete->elements[2]=0;
   
    uint8_t curr_octet = next_octet(fichier);
    curr_octet = next_octet(fichier);/*on saute ff*/
    if (curr_octet == 216) /*début de lecture d'image, ffd8*/
    {
        bool booleen_de_boucle = true;
        while (booleen_de_boucle)
        {
            curr_octet = next_octet(fichier);
            if (curr_octet == 255)/* si on lit ff*/
            {
                curr_octet = next_octet(fichier);
                if (curr_octet == 224) /* marqueur APP0, info sur jfif, ffe0*/
                {
                    if (is_jfif(fichier) == 0){
                        printf("Le fichier n'est pas conforme");
                        return NULL;
                    }
                }
                else if (curr_octet == 254) /*fffe*/
                {
                    /* struct cell *commentaire = get_comment(fichier); */
                }
                else if (curr_octet == 219)/* DQT, table de quantification, ffdb*/
                {
                    uint8_t *table = recup_table_quantif(fichier);
                    uint8_t nb_de_suiv = entete -> elements[0]+ entete -> elements[1];/*on saute les composantes et les tables de huffman*/
                    insere_element(entete,table,nb_de_suiv);/*on insere la table de quantification à la bonne place*/
                    entete->elements[2]+=1;/*on augmente le nombre de tables de quantification*/
                }

                else if (curr_octet == 192) /*SofO, composante et facteur d'echantillonage, ffc0*/
                {

                    struct infos_sof0 *infos = recup_SOF0(fichier);
                    if (infos == NULL){
                        printf("Le fichier n'est pas conforme");
                        return NULL;
                    }
                    entete -> dimensions[0] = infos -> dimensions[0];
                    entete -> dimensions[1] = infos -> dimensions[1];
                    for (uint8_t i = 0; i<infos -> n; i++){/*on insere les composantes*/
                        insere_element(entete,infos -> liste_composante[i],0);
                        entete->elements[0]+=1;
                    }
                    free(infos);
                }
                else if (curr_octet == 196) /*DHT, table de huffman, ffc4*/
                {
                    uint8_t *table = recup_table_huffman(fichier);
                    if (table == NULL){
                        printf("Le fichier n'est pas conforme");
                        return NULL;
                    }
                    uint8_t nb_de_suiv = entete -> elements[0];/*on saute les composantes*/
                    insere_element(entete,table,nb_de_suiv);/*on insere la table de huffman à la bonne place*/
                    entete->elements[1]+=1;/*on augmente le nombre de tables de huffman*/
                }
                else if (curr_octet == 218)/*SOS, indice de la table de huffman pour chaque composante, ffda*/
                {
                    uint8_t** liste_composantes_huff = recup_SOS(fichier);
                    if (liste_composantes_huff == NULL){
                        printf("Le fichier n'est pas conforme");
                        return NULL;
                    }
                    booleen_de_boucle = false;
                    for (uint8_t i = 0; i<entete -> elements[0]; i++){/*on parcourt les composantes*/
                        uint8_t *curr_composante = recupere_element(entete,i);/*on recupere la composante*/
                        for (uint8_t j = 0; j<entete -> elements[0]; j++){/*On parcourt les composantes recuperees dans SOS*/
                            if (curr_composante[0] == liste_composantes_huff[j][0]){/*on associe les tables de huffman aux bonnes composantes*/
                                curr_composante[4] = liste_composantes_huff[j][1];
                                curr_composante[5] = liste_composantes_huff[j][2];
                                break;
                            }
                        }
                    }
                    for (uint8_t j = 0; j<entete -> elements[0]; j++){
                            free(liste_composantes_huff[j]);
                        }
                        free(liste_composantes_huff);
                }
            }
        }
    }
    return entete;
}

void libere_liste(struct liste_chainee* liste){/*libere la place allouee à une liste chainée*/
    struct cellule* curr_cell = liste -> tete;
    struct cellule* prev_cell;
    for (int i=0; i<liste->elements[0]+liste->elements[1]+liste->elements[2] - 1; i++){
        prev_cell = curr_cell;
        curr_cell = curr_cell -> suivant;
        curr_cell -> precedent = prev_cell;
    }
    for (int i=0; i<liste->elements[0]+liste->elements[1]+liste->elements[2] - 1; i++){
        curr_cell = curr_cell -> precedent;
        free(curr_cell -> suivant -> element);
        free(curr_cell -> suivant);
    }
    free(liste -> tete);
    free(liste);
}

// int main(){
//     FILE *fichier = fopen("images/gris.jpg", "r");
//     struct liste_chainee *entete = malloc(sizeof(struct liste_chainee*));
//     entete = decode(fichier);
//     affiche_infos(entete);
//     return 0;
// }
