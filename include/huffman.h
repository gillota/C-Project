#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_


struct noeud{
    struct noeud *fils0;
    struct noeud *fils1;
    uint8_t valeur;
};

struct arbre{
    struct noeud *premier_noeud;
};


void insere_element_arbre(uint16_t code, uint8_t element, uint8_t profondeur,struct arbre* arbre);
struct arbre* construit_arbre(uint8_t* liste);

#endif // HUFFMAN_TANGUY_H
