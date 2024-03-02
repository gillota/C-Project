[]# Notre décodeur JPEG à nous

Bienvenue sur la page d'accueil de _votre_ projet JPEG, un grand espace de liberté, sous le regard bienveillant de vos enseignants préférés.
Le sujet sera disponible dès le mardi 2 mai à l'adresse suivante : [https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/](https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/).

Comme indiqué lors de l'amphi de présentation, vous devrez organiser un point d'étape avec vos enseignants pour valider cette architecture logicielle.
Cette page d'accueil servira de base à cette discussion. En pratique, vous pouvez reprendre son contenu comme bon vous semble, mais elle devra au moins comporter les infos suivantes :

1. des informations sur le découpage des fonctionnalités du projet en modules, en spécifiant les données en entrée et sortie de chaque étape ;
2. (au moins) un dessin des structures de données de votre projet (format libre, ça peut être une photo d'un dessin manuscrit par exemple) ;
3. une répartition des tâches au sein de votre équipe de développement, comportant une estimation du temps consacré à chacune d'elle (là encore, format libre, du truc cracra fait à la main, au joli Gantt chart).

Rajouter **régulièrement** des informations sur l'avancement de votre projet est aussi **une très bonne idée** (prendre 10 min tous les trois chaque matin pour résumer ce qui a été fait la veille, établir un plan d'action pour la journée qui commence et reporter tout ça ici, par exemple).

# Liens utiles

- Bien former ses messages de commits : [https://www.conventionalcommits.org/en/v1.0.0/](https://www.conventionalcommits.org/en/v1.0.0/) ;
- Problème relationnel au sein du groupe ? Contactez [Pascal](https://fr.wikipedia.org/wiki/Pascal,_le_grand_fr%C3%A8re) !
- Besoin de prendre l'air ? Le [Mont Rachais](https://fr.wikipedia.org/wiki/Mont_Rachais) est accessible à pieds depuis la salle E301 !
- Un peu juste sur le projet à quelques heures de la deadline ? Le [Montrachet](https://www.vinatis.com/achat-vin-puligny-montrachet) peut faire passer l'envie à vos profs de vous mettre une tôle !


# Architecture Logicielle :

**Lecture en-tête :**

_entrée :_ 
- fichier JPEG

_sortie dans une liste chainée :_  
- type (si c'est bien JFIF), APP0
- tables de quantification, DQT
- hauteur et largeur image, SOF0
- nb de composantes (1 pour N&B, 3 pour couleur), SOF0
- pour chaque composante, SOF0 : 
    - son identifiant
    - facteur echantillonnage horizontal _hi_
    - facteur d'échantillonnage vertical _vi_
    - table de quantification
- table de Huffman (avec les symboles et leur longueur), DHT
- pour chaque composante, SOS : 
    - son identifiant _ic_
    - indice de la table de Huffman (_ih_) pour DC
    - idem pour AC 

**Huffman :**

_entrée :_ 
- liste récupérée avec l'entête : 2 premier indices:c'est la taille, ensuite si c'est ac ou un dc, ensuite le nb de symbole par profondeurs, puis les symboles

_sortie :_ 
- arbre permettant de décoder les indices (table magnitude)

**Extraction blocs :**
--> AC/DC :

_entrée :_ 
- arbre en sortie de Huffman
- le fichier JPEG

_sortie :_ 
- vecteur 64 valeurs avec les valeurs de DC AC décodées grâce à huffman

**Quantification inverse :**

_entrée :_ 
- vecteur 64 valeurs en sortie du AC/DC
- matrice de quantification de l'en-tête

_sortie :_ 
- vecteur 64 valeurs avec multiplication

**Zig-Zag inverse :**

_entrée :_ 
- vecteur 64 valeurs

_sortie :_ 
- bloc 8x8 zigzagué

**iDCT :**

_entrée :_
- bloc 8x8 du zig-zag inverse

_sortie :_ 
- bloc valeur YCbCr 8x8

**Echantillonnage :**

_entrée :_ 
- liste de 3 blocs de la taille de l'image : un pour Y (plein), un pour Cb (non plein) et un pour Cr (non plein). Chaque bloc est une matrice des blocs 88.

_sortie :_ 
- cette même liste remplie selon l'échantillonnage (horizontal, vertical ou les 2)

**Conversion YCbCr vers RGB :**

_entrée :_ 
- bloc YCbCr

_sortie :_ 
- bloc RGB

**Ecriture dans PPM :**

_entrée :_ 
- tous les blocs RGB
- taille de l'image de l'en-tête
- le nom de l'image

_sortie :_ 
- image PPM






