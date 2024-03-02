#ifndef _ENTETE_H_
#define _ENTETE_H_




struct cellule
{
    struct cellule* suivant;
    struct cellule* precedent;
    uint8_t* element;
};

struct liste_chainee
{
    struct cellule* tete;
    uint8_t elements[3]; /*L'en tete contient des composantes, des tables de huffman et des tables de quantification*/
    uint16_t dimensions[2]; /*On a deux dimensions*/
};

struct infos_sof0
{
    uint16_t dimensions[2];
    uint8_t **liste_composante;
    uint8_t n;
};

uint8_t next_octet(FILE *fichier);

bool is_jfif(FILE *fichier);

uint8_t* recup_table_quantif(FILE *fichier);

struct infos_sof0* recup_SOF0(FILE *fichier);

uint8_t* recup_table_huffman(FILE *fichier);

uint8_t** recup_SOS(FILE *fichier);

void insere_element(struct liste_chainee *chaine, uint8_t *elements, uint8_t place);

uint8_t* recupere_element(struct liste_chainee *liste, uint8_t indice);

uint16_t somme_liste(uint8_t* liste,uint8_t elem1,uint8_t elem_fin);

void affiche_infos(struct liste_chainee *liste);

struct liste_chainee* decode(FILE *fichier);


struct liste_chainee* decode(FILE *fichier);

void libere_liste(struct liste_chainee* liste);


#endif // ENTETE_H