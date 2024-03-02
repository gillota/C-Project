/*importe les autres codes, prend un fichier jpeg en argument, decode etape par etape et renvoie un fichier ppm*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../include/iDCT.h"
#include "../include/entete.h"
#include "../include/huffman.h"
#include "../include/YCbCr2RGB.h"
#include "../include/ecriture_ppm.h"
#include "../include/zigzag.h"
#include "../include/quantification.h"
#include "../include/acdc.h"
#include "../include/echantillonage.h"
#include "../include/YCbCr2RGB.h"

uint8_t* ordre_composantes (struct liste_chainee *liste){/*donne liste qui associe à chaque composantes sa place dans l'entete par ordre croissant d'indice de composante*/
    uint8_t *ordre = malloc(sizeof(uint8_t)*liste -> elements[0]);
    struct cellule* curr_cell = liste -> tete;
    for (int i = 0; i<liste -> elements[0]; i++){
        ordre[curr_cell -> element[0]-1] = i;
        curr_cell = curr_cell -> suivant;
    }
    return ordre;
}

uint8_t* trouve_place_tables (struct liste_chainee *liste, uint8_t *composante){
    struct cellule* curr_cell = liste -> tete;
    uint8_t *places_tables  = malloc(sizeof(uint8_t)*3);
    for (int i = 0; i<liste -> elements[0]; i++){
        curr_cell = curr_cell -> suivant;
    }
    for (int i = 0; i<liste -> elements[1]; i++){
        if ((curr_cell -> element[0] == composante[4]) && (curr_cell -> element[1] == 0)){
            places_tables[0] = i + liste -> elements[0];
        }
        else if ((curr_cell -> element[0] == composante[5]) && (curr_cell -> element[1] == 1)){
            places_tables[1] = i + liste -> elements[0];
        }
        curr_cell = curr_cell -> suivant;
    }
    for (int i = 0; i<liste -> elements[2]; i++){
        if ((curr_cell -> element[0]) == composante[3]){
            places_tables[2] = i + liste -> elements[0] + liste -> elements[1];
        }
        curr_cell = curr_cell -> suivant;
    }
    return places_tables;
}



void convertirJpegEnPpm(char* cheminImage) {/*permet de changer le nom de l'image dont l'extension est .jpg ou .jpeg en .ppm*/
    size_t longueur = strlen(cheminImage);
    
    if (longueur >= 5) {
    // Vérifier si l'extension est .jpeg ou .jpg
        char* extension1 = cheminImage + longueur - 4;
        char* extension2 = cheminImage + longueur - 5;
        if (strcmp(extension1, ".jpg") == 0) {
            // Remplacer l'extension par .ppm
            strcpy(extension1, ".ppm");
            } 
        else if (strcmp(extension2, ".jpeg") == 0) {
            // Remplacer l'extension par .ppm
            strcpy(extension2, ".ppm");
        }
 
        else {
            printf("Extension non prise en charge.\n");
            }
    } 
    else {
        printf("Nom de fichier invalide.\n");
        }
}

int main(int argc, char *argv[]){/* programme principal du decodeur*/
    char* nom_image = argv[1];



    FILE *fichier = fopen((nom_image), "r");/*ouverture du fichier*/
    struct liste_chainee* entete=decode( fichier);
    affiche_infos(entete);
    /*liste de liste de vecteurs 64*/
    uint16_t h = entete -> dimensions[0];
    uint16_t l = entete -> dimensions[1];

    uint16_t hauteur = h/8 + (entete -> dimensions[0]%8 != 0);/*nb de bloc de taille 8*8 en ordones*/
    uint16_t largeur = l/8 + (entete -> dimensions[1]%8 != 0);/*nb de bloc de taille 8*8 en abscisse*/


    struct arbre** liste_arbres = malloc(sizeof(struct arbre*)*entete -> elements[0]*2);/*on initialise les arbres de huffman*/
    uint8_t** liste_quantif = malloc(sizeof(uint8_t*)*entete -> elements[0]);/*on initialise tout*/
    uint8_t* liste_composantes = ordre_composantes(entete);
    uint8_t echantillonage[entete -> elements[0]*2];
    uint8_t vertical_ou_horizontal;
    uint8_t* composante;
    for (int i = 0; i<entete -> elements[0]; i++){/* creer les arbres et les met dans le bon ordre*/
        composante = recupere_element(entete, liste_composantes[i]);
        echantillonage[2*i] = composante[1];
        echantillonage[2*i+1] = composante[2];
        uint8_t* liste_table = trouve_place_tables(entete, composante);
        uint8_t *table= recupere_element(entete,liste_table[0]);
        liste_arbres[2*i] = construit_arbre(table);
        table= recupere_element(entete,liste_table[1]);
        liste_arbres[2*i+1] = construit_arbre(table);
        liste_quantif[i] = recupere_element(entete,liste_table[2]);/*met les tables de quantifications dans le bon ordre*/
        free(liste_table);
    }

    free(liste_composantes);


    if (entete -> elements[0] == 3){/*si notre image est en couleur*/
        if ((echantillonage[0] == 1) && (echantillonage[1] == 1)){
            vertical_ou_horizontal = 0;/*il n'y a pas d'echantillonage*/
            }
        else if ((echantillonage[0] == 2) && (echantillonage[1] == 2)){
            vertical_ou_horizontal = 3;/*c'est echantilloné horizontalement et verticalement*/
            hauteur += (hauteur%2==1);
            largeur += (largeur%2==1);
        }
        else {
            if (echantillonage[0] == 2 && echantillonage[1] == 1){
                vertical_ou_horizontal = 1;/*c'est echantilloné horizontalement*/
                largeur += (largeur%2==1);
            }
    else {
        vertical_ou_horizontal = 2;/*c'est echantilloné verticalement*/
        hauteur += (hauteur%2==1);
            }
        }
    }

    int ****liste_blocs = malloc(sizeof(int***)*entete -> elements[0]);/*on initialise la liste de blocs*/
    for (int numero_composante = 0; numero_composante<entete -> elements[0]; numero_composante++){
        liste_blocs[numero_composante] = malloc(sizeof(int**)*hauteur);
        for (int ligne=0; ligne<hauteur; ligne++){
            liste_blocs[numero_composante][ligne] = malloc(sizeof(int*)*largeur);
            for (int colonne=0; colonne<largeur; colonne++){
                liste_blocs[numero_composante][ligne][colonne] = malloc(sizeof(int)*64);
            }
        }
    }

    
    uint8_t i = 0;
    int ancien_dc[entete -> elements[0]];
    int* coeffs;

    for (int ligne=0; ligne<hauteur; ligne+=echantillonage[1]){ /*on parcourt les mcu*/
        for (int colonne=0; colonne<largeur; colonne+=echantillonage[0]){
            for (int composante = 0; composante<entete -> elements[0]; composante++){/*on parcourt les composantes*/
                for (int ligne_mcu = 0; ligne_mcu<echantillonage[2*composante+1]; ligne_mcu++){
                    for (int colonne_mcu = 0; colonne_mcu<echantillonage[2*composante]; colonne_mcu++) {/*pour chaque bloc de la mcu*/
                        int *dc = lit_DC_ou_AC(fichier, liste_arbres[2*composante], false); /*on lit le dc*/
                        if (ligne+colonne+colonne_mcu+ligne_mcu == 0){        /*si on est au TOUT  premier bloc de l'image*/    /*on lit les prochains symbole, 1er dc, 63 ac*/
                            liste_blocs[composante][ligne+ligne_mcu][colonne+colonne_mcu][i]=dc[0];
                            ancien_dc[composante] = liste_blocs[composante][ligne+ligne_mcu][colonne+colonne_mcu][i];
                        }
                        else { 
                            liste_blocs[composante][ligne+ligne_mcu][colonne+colonne_mcu][i]=dc[0] + ancien_dc[composante];
                            ancien_dc[composante] = liste_blocs[composante][ligne+ligne_mcu][colonne+colonne_mcu][i];
                        }
                        free(dc);
                        i++;
                        /*les autres dc sont des differences avec celui d'avant*/
                        /* et calculer  tous les coeffs de tous les blocs*/
                        while (i<64){ /*on lit les coeff nul consécutifs jusqu'à un non nul et on recommence jusqu'à remplir le vecteur */
                            coeffs = lit_DC_ou_AC(fichier, liste_arbres[2*composante+1], true);
                            for (int m=0; m<coeffs[0]; m++){
                                liste_blocs[composante][ligne+ligne_mcu][colonne+colonne_mcu][i]=coeffs[m+1];
                                i++;
                                if (i==64){
                                    break;
                                }
                            }
                            free(coeffs);
                        }
                    i = 0;
                    }
                }
            }
        }
    }
    
    for(int i=0; i<entete -> elements[0]*2;i++){/*on free les arbres*/
        free(liste_arbres[i]);
    }


    /*on inverse la quantification*/
    /*boucle sur chaque bloc*/
    /*une table pour Y et une table pour CRCB*/
    for (int ligne=0; ligne<hauteur; ligne+=echantillonage[1]){
        for (int colonne=0; colonne<largeur; colonne+=echantillonage[0]){
            for (int composante = 0; composante<entete -> elements[0]; composante++){
                for (int colonne_mcu = 0; colonne_mcu<echantillonage[2*composante]; colonne_mcu++) {
                    for (int ligne_mcu = 0; ligne_mcu<echantillonage[2*composante+1]; ligne_mcu++){
                        quantification_inverse(liste_blocs[composante][ligne+ligne_mcu][colonne+colonne_mcu],liste_quantif[composante]);
                    }
                }
            }
        }
    }
    free(liste_quantif);

    uint8_t *****liste_complete_pix;
    struct pixel_ycbcr ****liste_blocs_88_ycbcr;

    if (entete->elements[0] == 3){/*si notre image est en couleur on initialise la liste de pixels*/
        liste_complete_pix = malloc(sizeof(uint8_t****)*entete->elements[0]);
        for (int composante = 0; composante <entete -> elements[0]; composante ++){
            liste_complete_pix[composante ] = malloc(sizeof(uint8_t***)*hauteur);
            for (int ligne=0; ligne<hauteur; ligne++){
                liste_complete_pix[composante ][ligne] = malloc(sizeof(uint8_t**)*largeur);
                for (int colonne=0; colonne<largeur; colonne++){
                    liste_complete_pix[composante ][ligne][colonne] = malloc(sizeof(uint8_t*)*8);
                    for (int ligne_dans_bloc=0; ligne_dans_bloc<8; ligne_dans_bloc++){
                        liste_complete_pix[composante ][ligne][colonne][ligne_dans_bloc] = malloc(sizeof(uint8_t)*8);
                        for (int colonne_dans_bloc=0; colonne_dans_bloc<8; colonne_dans_bloc++){
                            liste_complete_pix[composante ][ligne][colonne][ligne_dans_bloc][colonne_dans_bloc] = 0;
                        }
                    }
                }
            }
        }
    }
    else {
        liste_blocs_88_ycbcr = malloc(sizeof(struct pixel_ycbcr***)*hauteur);
        for (int ligne=0; ligne<hauteur; ligne++){
            liste_blocs_88_ycbcr[ligne] = malloc(sizeof(struct pixel_ycbcr**)*largeur);
        }
    }
    int ****liste_blocs_zigzag = malloc(sizeof(int***)*hauteur);/*on initialise les differentes listes de blocs*/
    for (int ligne=0; ligne<hauteur; ligne++){
        liste_blocs_zigzag[ligne] = malloc(sizeof(int**)*largeur);
    }
    uint8_t ****liste_blocs_88 = malloc(sizeof(uint8_t***)*hauteur);
    for (int ligne=0; ligne<hauteur; ligne++){
        liste_blocs_88[ligne] = malloc(sizeof(uint8_t**)*largeur);
    }
    struct pixel_rgb ****liste_blocs_88_rgb = malloc(sizeof(struct pixel_rgb***)*hauteur);
    for (int ligne=0; ligne<hauteur; ligne++){
        liste_blocs_88_rgb[ligne] = malloc(sizeof(struct pixel_rgb**)*largeur);
    }

    for (int ligne=0; ligne<hauteur; ligne+=echantillonage[1]){/*on boucle sur chaque mcu*/
        for (int colonne=0; colonne<largeur; colonne+=echantillonage[0]){
            for (int composante = 0; composante<entete -> elements[0]; composante++){/*on boucle sur chaque composantes*/
                for (int colonne_mcu = 0; colonne_mcu<echantillonage[2*composante]; colonne_mcu++) {
                    for (int ligne_mcu = 0; ligne_mcu<echantillonage[2*composante+1]; ligne_mcu++){
                        liste_blocs_zigzag[ligne+ligne_mcu][colonne+colonne_mcu]=zigzag(liste_blocs[composante][ligne+ligne_mcu][colonne+colonne_mcu]);/* on transforme le vecteur en bloc 88 zig-zagué*/

                        liste_blocs_88[ligne+ligne_mcu][colonne+colonne_mcu]=iDCT(liste_blocs_zigzag[ligne+ligne_mcu][colonne+colonne_mcu]);/*on fait l' idct sur chaque bloc*/

                        if (entete->elements[0] == 1){/*si notre image est en noir et blanc on remplit les blocs de pixels*/
                            liste_blocs_88_ycbcr[ligne][colonne]=blacknwhite(liste_blocs_88[ligne][colonne]);

                            liste_blocs_88_rgb[ligne][colonne]=ycbcr2rgb(liste_blocs_88_ycbcr[ligne][colonne]);/*on transforme les blocs de pixels YCbCr en rgb*/
                            
                        }
                        
                        else {/*si notre image est en couleur on remplit les blocs de pixels*/
                            liste_complete_pix[composante][ligne+ligne_mcu][colonne+colonne_mcu] = liste_blocs_88[ligne+ligne_mcu][colonne+colonne_mcu];
                        }
                    }
                }
            }
        }
    }

    if (entete->elements[0] == 3){/*si notre image est en couleur, il peut y avoir un echentillonage*/
        if (vertical_ou_horizontal == 1){/*on echantillonne horizontalement*/
            rempli_horizontal(liste_complete_pix, hauteur, largeur);
        }
        else if (vertical_ou_horizontal == 2){ /*on echantillonne verticalement*/
            rempli_vertical(liste_complete_pix, hauteur, largeur);
        }
        else if (vertical_ou_horizontal == 3){/*on echantillonne horizontalement et verticalement*/
            rempli_horizontal_vertical(liste_complete_pix, hauteur, largeur);
        }

        liste_blocs_88_ycbcr=color(liste_complete_pix,hauteur,largeur);/*on transforme nos 3 tableaux en un seul de structure ycbcr*/
        for (int ligne=0; ligne<hauteur; ligne++){
            for (int colonne=0; colonne<largeur; colonne++){
                liste_blocs_88_rgb[ligne][colonne]=ycbcr2rgb(liste_blocs_88_ycbcr[ligne][colonne]);/*on transforme les blocs de pixels YCbCr en rgb*/
            }
        }
    }
             
                
    convertirJpegEnPpm(nom_image);/*on change le nom de notre image en format ppm*/
    rgb2ppm(liste_blocs_88_rgb,entete->dimensions[1],entete->dimensions[0],nom_image);/*on ecrit dans le fichier nom_image en format ppm*/

    for (int ligne=0; ligne<hauteur; ligne++){/*on libere la memoire*/
        for (int colonne=0; colonne<largeur; colonne++){
            for (int m = 0; m<8; m++){
                free(liste_blocs_zigzag[ligne][colonne][m]);
                free(liste_blocs_88_rgb[ligne][colonne][m]);
            }
            free(liste_blocs_zigzag[ligne][colonne]);
            free(liste_blocs_88_rgb[ligne][colonne]);
        }
        free(liste_blocs_88[ligne]);
        free(liste_blocs_zigzag[ligne]);
        free(liste_blocs_88_ycbcr[ligne]);
        free(liste_blocs_88_rgb[ligne]);
    }
    free(liste_blocs_88);
    free(liste_blocs_zigzag);
    free(liste_blocs_88_ycbcr);
    free(liste_blocs_88_rgb);

    for (int composante = 0; composante<entete -> elements[0]; composante++){
        for (int ligne=0; ligne<hauteur; ligne++){
            free(liste_blocs[composante][ligne]);
        }
        free(liste_blocs[composante]);
    }
    free(liste_blocs);

    libere_liste(entete);

    return argc;
}