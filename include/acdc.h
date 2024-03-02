#ifndef _ACDC_H_
#define _ACDC_H_


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "huffman.h"

extern uint8_t bit_courant;
extern uint8_t bit_lu;
extern uint8_t nb_1_suite;

uint8_t lire_bit(FILE* fichier);

uint8_t lecture_huffman(FILE* fichier, struct arbre* arbre_huffman);

uint16_t lecture_indice(FILE* fichier, uint8_t magnitude);


int acdc(int magnitude, uint16_t indice);

int* lit_DC_ou_AC(FILE* fichier, struct arbre* arbre_huffman, bool DCAC);

#endif // ACDC_H