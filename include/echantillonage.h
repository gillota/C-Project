#ifndef _ECHANTILLONAGE_H_
#define _ECHANTILLONAGE_H_


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

void rempli_horizontal(uint8_t***** liste_3blocs, uint16_t h, uint16_t l);

void rempli_vertical(uint8_t***** liste_3blocs, uint16_t h, uint16_t l);

void rempli_horizontal_vertical(uint8_t***** liste_3blocs, uint16_t h, uint16_t l);


#endif // ECHANTILLONAGE_H