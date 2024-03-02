#ifndef _IDCT_H_
#define _IDCT_H_


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

double C(uint16_t sigma);

uint8_t** creer_bloc_idct();

uint8_t** iDCT(int **bloc);

#endif // IDCT_H
