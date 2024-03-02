#ifndef _ECRITURE_PPM_H_
#define _ECRITURE_PPM_H_




#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>


// void invader_ppm(struct pixel_rgb** bloc);
void rgb2ppm(struct pixel_rgb**** blocs, uint32_t largeur, uint32_t hauteur,char* nom_fichier);
struct pixel_rgb** creer_bloc88_rgb();
#endif // __ECRITURE_PPM_H__
