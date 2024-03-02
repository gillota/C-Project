#ifndef _YCBCR2RGB_H_
#define _YCBCR2RGB_H_


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

struct pixel_ycbcr  /*structure de données d'un pixel au format YCbCr*/
{
    uint8_t Y;
    uint8_t Cb;
    uint8_t Cr;
};

struct pixel_rgb /*structure de données d'un pixel au format RGB*/
{
    int R;
    int G;
    int B;
};


struct pixel_rgb** creer_bloc_rgb(int rouge,int vert, int bleu);
struct pixel_ycbcr** creer_bloc_ycbcr(uint8_t luminesance, uint8_t crominance_bleu, uint8_t craumience_rouge);

struct pixel_ycbcr**** color(uint8_t***** liste_3blocs, uint16_t hauteur , uint16_t largeur);

struct pixel_rgb ** ycbcr2rgb(struct pixel_ycbcr ** bloc88);

struct pixel_ycbcr** blacknwhite(uint8_t** bloc88);

#endif // YCBCR2RGB_H