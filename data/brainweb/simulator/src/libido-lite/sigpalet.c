/*
 * Creation de diverses palettes de 128 ou 256 couleurs
 */
#include "idima.h"
#include "iderr.h"
#include "idio.h"
#include "idprint.h"


static RGB tabHotMetal[] = {
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{1, 0, 0},
	{2, 0, 0},
	{3, 0, 0},
	{4, 0, 0},
	{5, 0, 0},
	{6, 0, 0},
	{7, 0, 0},
	{8, 0, 0},
	{9, 0, 0},
	{10, 0, 0},
	{11, 0, 0},
	{12, 0, 0},
	{13, 0, 0},
	{14, 0, 0},
	{15, 0, 0},
	{16, 0, 0},
	{17, 0, 0},
	{18, 0, 0},
	{19, 0, 0},
	{20, 0, 0},
	{21, 0, 0},
	{22, 0, 0},
	{23, 0, 0},
	{24, 0, 0},
	{25, 0, 0},
	{26, 0, 0},
	{27, 0, 0},
	{28, 0, 0},
	{29, 0, 0},
	{30, 0, 0},
	{31, 0, 0},
	{32, 0, 0},
	{34, 0, 0},
	{35, 0, 0},
	{36, 0, 0},
	{37, 0, 0},
	{38, 0, 0},
	{39, 0, 0},
	{40, 0, 0},
	{42, 0, 0},
	{43, 0, 0},
	{44, 0, 0},
	{45, 0, 0},
	{46, 0, 0},
	{47, 0, 0},
	{48, 0, 0},
	{50, 0, 0},
	{51, 0, 0},
	{52, 0, 0},
	{53, 0, 0},
	{54, 0, 0},
	{55, 0, 0},
	{56, 0, 0},
	{57, 0, 0},
	{58, 0, 0},
	{60, 0, 0},
	{61, 0, 0},
	{62, 0, 0},
	{63, 0, 0},
	{64, 0, 0},
	{66, 0, 0},
	{69, 0, 0},
	{72, 0, 0},
	{74, 0, 0},
	{77, 0, 0},
	{79, 0, 0},
	{82, 0, 0},
	{83, 0, 0},
	{84, 0, 0},
	{85, 0, 0},
	{87, 0, 0},
	{88, 0, 0},
	{89, 0, 0},
	{90, 0, 0},
	{93, 0, 0},
	{95, 0, 0},
	{96, 0, 0},
	{97, 0, 0},
	{98, 0, 0},
	{101, 0, 0},
	{102, 0, 0},
	{103, 0, 0},
	{106, 0, 0},
	{107, 0, 0},
	{108, 0, 0},
	{109, 0, 0},
	{110, 0, 0},
	{111, 0, 0},
	{112, 0, 0},
	{113, 0, 0},
	{114, 0, 0},
	{117, 0, 0},
	{119, 0, 0},
	{122, 0, 0},
	{125, 0, 0},
	{127, 0, 0},
	{130, 0, 0},
	{133, 0, 0},
	{135, 0, 0},
	{138, 0, 0},
	{141, 0, 0},
	{143, 0, 0},
	{146, 0, 0},
	{149, 0, 0},
	{151, 0, 0},
	{152, 0, 0},
	{153, 2, 0},
	{154, 5, 0},
	{155, 9, 0},
	{157, 12, 0},
	{158, 16, 0},
	{159, 20, 0},
	{161, 23, 0},
	{162, 27, 0},
	{164, 30, 0},
	{165, 34, 0},
	{166, 38, 0},
	{168, 41, 0},
	{169, 45, 0},
	{171, 49, 0},
	{172, 52, 0},
	{173, 56, 0},
	{175, 59, 0},
	{176, 63, 0},
	{178, 67, 0},
	{179, 70, 0},
	{180, 74, 0},
	{182, 77, 0},
	{183, 81, 0},
	{185, 85, 0},
	{186, 88, 0},
	{187, 92, 0},
	{189, 96, 0},
	{190, 99, 0},
	{192, 103, 0},
	{193, 106, 0},
	{194, 110, 0},
	{196, 114, 0},
	{197, 117, 0},
	{199, 121, 0},
	{200, 124, 0},
	{201, 128, 0},
	{203, 132, 0},
	{204, 135, 0},
	{206, 139, 0},
	{207, 143, 0},
	{208, 146, 0},
	{210, 150, 0},
	{211, 153, 0},
	{213, 157, 0},
	{214, 161, 0},
	{215, 164, 0},
	{217, 168, 0},
	{218, 172, 0},
	{220, 175, 0},
	{221, 179, 0},
	{222, 182, 0},
	{224, 186, 0},
	{225, 190, 0},
	{227, 193, 0},
	{228, 197, 0},
	{229, 200, 0},
	{231, 204, 0},
	{232, 208, 0},
	{234, 211, 0},
	{235, 215, 0},
	{236, 219, 0},
	{238, 222, 0},
	{239, 226, 0},
	{241, 229, 0},
	{242, 233, 0},
	{243, 237, 0},
	{245, 240, 0},
	{246, 244, 0},
	{248, 247, 0},
	{255, 255, 7},
	{255, 255, 10},
	{255, 255, 13},
	{255, 255, 16},
	{255, 255, 20},
	{255, 255, 23},
	{255, 255, 26},
	{255, 255, 30},
	{255, 255, 33},
	{255, 255, 36},
	{255, 255, 40},
	{255, 255, 43},
	{255, 255, 46},
	{255, 255, 49},
	{255, 255, 53},
	{255, 255, 56},
	{255, 255, 59},
	{255, 255, 63},
	{255, 255, 66},
	{255, 255, 69},
	{255, 255, 73},
	{255, 255, 76},
	{255, 255, 79},
	{255, 255, 83},
	{255, 255, 86},
	{255, 255, 89},
	{255, 255, 92},
	{255, 255, 96},
	{255, 255, 99},
	{255, 255, 102},
	{255, 255, 106},
	{255, 255, 109},
	{255, 255, 112},
	{255, 255, 116},
	{255, 255, 119},
	{255, 255, 122},
	{255, 255, 126},
	{255, 255, 129},
	{255, 255, 132},
	{255, 255, 135},
	{255, 255, 139},
	{255, 255, 142},
	{255, 255, 145},
	{255, 255, 149},
	{255, 255, 152},
	{255, 255, 155},
	{255, 255, 159},
	{255, 255, 162},
	{255, 255, 165},
	{255, 255, 169},
	{255, 255, 172},
	{255, 255, 175},
	{255, 255, 178},
	{255, 255, 182},
	{255, 255, 185},
	{255, 255, 188},
	{255, 255, 192},
	{255, 255, 195},
	{255, 255, 198},
	{255, 255, 202},
	{255, 255, 205},
	{255, 255, 208},
	{255, 255, 212},
	{255, 255, 215},
	{255, 255, 218},
	{255, 255, 221},
	{255, 255, 225},
	{255, 255, 228},
	{255, 255, 231},
	{255, 255, 235},
	{255, 255, 238},
	{255, 255, 241},
	{255, 255, 245},
	{255, 255, 248},
	{255, 255, 251},
	{255, 255, 255},
	{255, 255, 255},
	{255, 255, 255},
	{255, 255, 255},
	/* ajout */
	{255, 255, 255},
	{255, 255, 255},
	{255, 255, 255},
	{255, 255, 255}



};


/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalFalsCol (fonction) 
 * 
 * RESUME: palette 128 fausses couleurs pour affich. images 8 bits
 * 
 * DESCRIPTION: creation d'une palette de 128 fausses couleurs 
 * pour l'affichage de donnees 8 bits .
 * 
 * SYNTAXE:     PSIGNAL_RGB palette = IdPalFalsCol();  
 * 
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : PSIGNAL_RGB representant la LUT de 128 couleurs sur 3
 * octets permettant d'afficher les images.
 * 
 * PARAMETRES:  aucun 
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */

#define NBCOL 128
#define PAS 21


/* passage graduel en 128 couleurs :
 * noir,(0)
 * bleu,(21)
 * vert,(42)
 * jaune,(63)
 * orange,(84)
 * rouge,(105)
 * violet.(126 a 127)
 * 
 * 21 teintes entre deux references.
 * 
 */

PSIGNAL_RGB IdPalFalsCol(void)
{

	PSIGNAL_RGB lut;
	int i;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < PAS; i += 2) {
		lut[i].r = 0;
		lut[i].g = 0;
		lut[i].b = 6 * i;
	}


	for (i = 0; i < PAS; i += 2) {
		lut[i + PAS].r = 0;
		lut[i + PAS].g = 6 * i;
		lut[i + PAS].b = 252;
	}

	for (i = 0; i < PAS; i += 2) {
		lut[i + 2 * PAS].r = 0;
		lut[i + 2 * PAS].g = 252;
		lut[i + 2 * PAS].b = 252 - 6 * i;
	}

	for (i = 0; i < PAS; i += 2) {
		lut[i + 3 * PAS].r = 6 * i;
		lut[i + 3 * PAS].g = 252;
		lut[i + 3 * PAS].b = 0;
	}

	for (i = 0; i < PAS; i += 2) {
		lut[i + 4 * PAS].r = 252;
		lut[i + 4 * PAS].g = 252 - 6 * i;
		lut[i + 4 * PAS].b = 0;
	}

	for (i = 0; i < PAS; i += 2) {
		lut[i + 5 * PAS].r = 252;
		lut[i + 5 * PAS].g = 0;
		lut[i + 5 * PAS].b = 6 * i;
	}

	for (i = 126; i < NBCOL; i += 2) {
		lut[i + 6 * PAS].r = 252;
		lut[i + 6 * PAS].g = 0;
		lut[i + 6 * PAS].b = 252;
	}

	return lut;

}

/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalGreyCol  (fonction)
 * 
 * RESUME: palette 128 niveaux de gris pour affich. images 8 bits
 * 
 * DESCRIPTION: creation d'une palette de 128 niveaux de gris 
 * pour la visualisation des images . 
 * 
 * SYNTAXE:  PSIGNAL_RGB palette = IdPalGreyCol();   
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : signal 256 representant la LUT de 128 niveaux de gris sur 3
 * octets permettant d'afficher les images.
 * 
 * PARAMETRES:  aucun 
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */

/* palette standard en niveaux de gris : 128 valeurs
 * du noir (0)
 * au blanc (255)
 */

PSIGNAL_RGB IdPalGreyCol(void)
{

	PSIGNAL_RGB lut;
	int i;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < 256; i += 2) {
		lut[i].r = lut[i + 1].r = i;
		lut[i].g = lut[i + 1].g = i;
		lut[i].b = lut[i + 1].b = i;
	}

	return lut;
}


/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalGrey64  (fonction)
 * 
 * RESUME: palette 64 niveaux de gris pour affich. images 8 bits
 * 
 * DESCRIPTION: creation d'une palette de 64 niveaux de gris 
 * pour la visualisation des images.
 * ( Utile lorsqu'on a, en + d'une image a niveaux de gris,
 * une palette 'riche' en fausses couleurs) 
 * 
 * SYNTAXE:  PSIGNAL_RGB palette = IdPalGrey64();   
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : signal 256 representant la LUT de 74 niveaux de gris sur 3
 * octets permettant d'afficher les images.
 * 
 * PARAMETRES:  aucun 
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */

PSIGNAL_RGB IdPalGrey64(void)
{

	PSIGNAL_RGB lut;
	int i;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < 256; i += 4) {
		lut[i].r = lut[i + 1].r = lut[i + 2].r = lut[i + 3].r = i;
		lut[i].g = lut[i + 1].g = lut[i + 2].g = lut[i + 3].g = i;
		lut[i].b = lut[i + 1].b = lut[i + 2].b = lut[i + 3].b = i;
	}


	return lut;
}


/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalGreyRedCol  (fonction) 
 * 
 * RESUME: Palette 127 niveaux de gris + Rouge pour affich. images 8 bits
 * 
 * DESCRIPTION: creation d'une palette de 127 niveaux de gris + Rouge
 * pour la visualisation des images . 
 * 
 * SYNTAXE:    PSIGNAL_RGB palette = IdPalGreyRedCol();   
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : signal 256 representant la LUT 
 * de 127 niveaux de gris + Rouge sur 3
 * octets permettant d'afficher les images.
 * 
 * PARAMETRES:  aucun 
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */

/* palette standard en niveaux de gris : 128 valeurs
 * du noir (0)
 * au blanc (127) puis Rouge
 */

PSIGNAL_RGB IdPalGreyRedCol(void)
{

	PSIGNAL_RGB lut;
	int i;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < 254; i += 2) {
		lut[i].r = lut[i + 1].r = i;
		lut[i].g = lut[i + 1].g = i;
		lut[i].b = lut[i + 1].b = i;
	}

	lut[254].r = lut[255].r = 255;
	lut[254].g = lut[255].g = 0;
	lut[254].b = lut[255].b = 0;

	return lut;
}


/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalGreyRedToYellowCol  (fonction) 
 * 
 * RESUME: Palette niveaux de gris-> seuil puis de Rouge a Jaune
 * 
 * DESCRIPTION: palette niveaux de gris-> seuil puis de Rouge a Jaune . 
 * 
 * SYNTAXE:    PSIGNAL_RGB palette = IdPalGreyRedToYellowCol(int seuil);   
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : signal 256 representant la LUT 
 * niveaux de gris-> seuil puis de Rouge a Jaune
 * sur 3 octets permettant d'afficher les images.
 * 
 * PARAMETRES:  
 * nom   : seuil
 * type  : int
 * role  : seuil a partir du quel on commence l'echelle Rouge-Jaune
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */

PSIGNAL_RGB IdPalGreyRedToYellowCol(int seuil)
{

	PSIGNAL_RGB lut;
	int i, div;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < seuil - 2; i += 2) {
		lut[i].r = lut[i + 1].r = i;
		lut[i].g = lut[i + 1].g = i;
		lut[i].b = lut[i + 1].b = i;
	}

	div = 255 - seuil;

	for (i = seuil; i < 254; i += 2) {
		lut[i].r = lut[i + 1].r = 255;
		lut[i].g = lut[i + 1].g = (i - seuil) * 255 / div;
		lut[i].b = lut[i + 1].b = 0;
	}

	lut[254].r = lut[255].r = 255;
	lut[254].g = lut[255].g = 255;
	lut[254].b = lut[255].b = 0;


	return lut;
}

/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalHotMetal (fonction) 
 * 
 * RESUME: Palette Hot Metal 256 fausses couleurs pour affich. images 8 bits
 * 
 * DESCRIPTION: creation d'une palette Hot Metal 256 fausse fausses couleurs 
 * pour l'affichage de donnees 8 bits .
 * 
 * SYNTAXE:     PSIGNAL_RGB palette = IdPalHotMetal();  
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : PSIGNAL_RGB representant la LUT de 256 couleurs sur 3
 * octets permettant d'afficher les images.
 * 
 * PARAMETRES:  aucun 
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */
PSIGNAL_RGB IdPalHotMetal(void)
{
	int i;

	PSIGNAL_RGB lut;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < 255; i++) {
		lut[i].r = tabHotMetal[i].r;
		lut[i].g = tabHotMetal[i].g;
		lut[i].b = tabHotMetal[i].b;
	}

	return lut;
}


/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalHotMetal128 (fonction) 
 * 
 * RESUME: Palette Hot Metal 128 fausses couleurs pour affich. images 8 bits
 * 
 * DESCRIPTION: creation d'une palette Hot Metal 128 fausse fausses couleurs 
 * pour l'affichage de donnees 8 bits .
 * 
 * SYNTAXE:     PSIGNAL_RGB palette = IdPalHotMetal128();  
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : PSIGNAL_RGB representant la LUT de 128 couleurs sur 3
 * octets permettant d'afficher les images.
 * 
 * PARAMETRES:  aucun 
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */
PSIGNAL_RGB IdPalHotMetal128(void)
{
	int i;

	PSIGNAL_RGB lut;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < 255; i += 2) {
		lut[i].r = lut[i + 1].r = tabHotMetal[i].r;
		lut[i].g = lut[i + 1].g = tabHotMetal[i].g;
		lut[i].b = lut[i + 1].b = tabHotMetal[i].b;
	}

	return lut;
}

/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalHotMetal64 (fonction) 
 * 
 * RESUME: Palette Hot Metal 64 fausses couleurs pour affich. images 8 bits
 * 
 * DESCRIPTION: creation d'une palette Hot Metal 64 fausse fausses couleurs 
 * pour l'affichage de donnees 8 bits .
 * 
 * SYNTAXE:     PSIGNAL_RGB palette = IdPalHotMetal64();  
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : PSIGNAL_RGB representant la LUT de 64 couleurs sur 3
 * octets permettant d'afficher les images.
 * 
 * PARAMETRES:  aucun 
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */
PSIGNAL_RGB IdPalHotMetal64(void)
{

	int i;

	PSIGNAL_RGB lut;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < 255; i += 4) {
		lut[i].r = lut[i + 1].r = lut[i + 2].r = lut[i + 3].r =
		    tabHotMetal[i].r;
		lut[i].g = lut[i + 1].g = lut[i + 2].g = lut[i + 3].g =
		    tabHotMetal[i].g;
		lut[i].b = lut[i + 1].b = lut[i + 2].b = lut[i + 3].b =
		    tabHotMetal[i].b;
	}

	return lut;
}


/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalHotMetal32 (fonction) 
 * 
 * RESUME: Palette Hot Metal 16 fausses couleurs pour affich. images 8 bits
 * 
 * DESCRIPTION: creation d'une palette Hot Metal 32 fausse fausses couleurs 
 * pour l'affichage de donnees 8 bits .
 * 
 * SYNTAXE:     PSIGNAL_RGB palette = IdPalHotMetal32();  
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : PSIGNAL_RGB representant la LUT de 32 couleurs sur 3
 * octets permettant d'afficher les images.
 * 
 * PARAMETRES:  aucun 
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */
PSIGNAL_RGB IdPalHotMetal32(void)
{

	int i, j;

	PSIGNAL_RGB lut;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < 255; i += 8) {
		for (j = 0; j < 8; j++) {
			lut[i + j].r = tabHotMetal[i].r;
			lut[i + j].g = tabHotMetal[i].g;
			lut[i + j].b = tabHotMetal[i].b;
		}
	}

	return lut;
}



/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalHotMetal16 (fonction) 
 * 
 * RESUME: Palette Hot Metal 16 fausses couleurs pour affich. images 8 bits
 * 
 * DESCRIPTION: creation d'une palette Hot Metal 16 fausse fausses couleurs 
 * pour l'affichage de donnees 8 bits .
 * 
 * SYNTAXE:     PSIGNAL_RGB palette = IdPalHotMetal16();  
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : PSIGNAL_RGB representant la LUT de 16 couleurs sur 3
 * octets permettant d'afficher les images.
 * 
 * PARAMETRES:  aucun 
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */
PSIGNAL_RGB IdPalHotMetal16(void)
{

	int i, j;

	PSIGNAL_RGB lut;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < 255; i += 16) {
		for (j = 0; j < 16; j++) {
			lut[i + j].r = tabHotMetal[i].r;
			lut[i + j].g = tabHotMetal[i].g;
			lut[i + j].b = tabHotMetal[i].b;
		}
	}

	return lut;
}


/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalHotMetal8 (fonction) 
 * 
 * RESUME: Palette Hot Metal 8 fausses couleurs pour affich. images 8 bits
 * 
 * DESCRIPTION: creation d'une palette Hot Metal 8 fausse fausses couleurs 
 * pour l'affichage de donnees 8 bits .
 * 
 * SYNTAXE:     PSIGNAL_RGB palette = IdPalHotMetal8();  
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : PSIGNAL_RGB representant la LUT de 8 couleurs sur 3
 * octets permettant d'afficher les images.
 * 
 * PARAMETRES:  aucun 
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */
PSIGNAL_RGB IdPalHotMetal8(void)
{

	int i, j;

	PSIGNAL_RGB lut;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < 255; i += 32) {
		for (j = 0; j < 32; j++) {
			lut[i + j].r = tabHotMetal[i].r;
			lut[i + j].g = tabHotMetal[i].g;
			lut[i + j].b = tabHotMetal[i].b;
		}
	}

	return lut;
}


/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalAnalyseStd (fonction) 
 * 
 * RESUME: Palette Analyse Standard 256 fausses couleurs pour affich. images 8 bits
 * 
 * DESCRIPTION: creation d'une palette Analyse Standard 256 fausse fausses couleurs (noir -> vert -> jaune -> rouge -> blanc) pour l'affichage de donnees 8 bits .
 * 
 * SYNTAXE:     PSIGNAL_RGB palette = IdPalAnalyseStd();  
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : PSIGNAL_RGB representant la LUT de 256 couleurs sur 3
 * octets permettant d'afficher les images.
 * 
 * PARAMETRES:  aucun 
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */
PSIGNAL_RGB IdPalAnalyseStd(void)
{
	static RGB tabAnalyseStd[] = { {0, 0, 0},
	{1, 0, 1},
	{2, 0, 2},
	{4, 0, 4},
	{6, 0, 6},
	{8, 0, 8},
	{10, 0, 10},
	{12, 0, 12},
	{14, 0, 14},
	{16, 0, 16},
	{17, 0, 17},
	{19, 0, 19},
	{21, 0, 21},
	{23, 0, 23},
	{25, 0, 25},
	{27, 0, 27},
	{29, 0, 29},
	{31, 0, 31},
	{32, 0, 32},
	{34, 0, 34},
	{36, 0, 36},
	{38, 0, 38},
	{40, 0, 40},
	{42, 0, 42},
	{44, 0, 44},
	{46, 0, 46},
	{48, 0, 48},
	{46, 0, 49},
	{44, 0, 50},
	{43, 0, 52},
	{42, 0, 54},
	{40, 0, 56},
	{39, 0, 57},
	{38, 0, 59},
	{36, 0, 61},
	{35, 0, 63},
	{34, 0, 65},
	{32, 0, 67},
	{31, 0, 68},
	{30, 0, 70},
	{28, 0, 72},
	{27, 0, 74},
	{26, 0, 76},
	{24, 0, 77},
	{23, 0, 79},
	{22, 0, 81},
	{21, 0, 83},
	{19, 0, 85},
	{18, 0, 86},
	{17, 0, 88},
	{15, 0, 90},
	{14, 0, 92},
	{13, 0, 94},
	{11, 0, 95},
	{10, 0, 97},
	{9, 0, 99},
	{7, 0, 101},
	{6, 0, 103},
	{5, 0, 104},
	{3, 0, 106},
	{2, 0, 108},
	{1, 0, 110},
	{0, 0, 112},
	{0, 2, 112},
	{0, 4, 112},
	{0, 7, 112},
	{0, 10, 112},
	{0, 13, 112},
	{0, 15, 112},
	{0, 18, 112},
	{0, 21, 112},
	{0, 24, 112},
	{0, 26, 112},
	{0, 29, 112},
	{0, 32, 112},
	{0, 35, 112},
	{0, 37, 112},
	{0, 40, 112},
	{0, 43, 112},
	{0, 46, 112},
	{0, 48, 112},
	{0, 51, 112},
	{0, 54, 112},
	{0, 57, 112},
	{0, 59, 112},
	{0, 62, 112},
	{0, 65, 112},
	{0, 68, 112},
	{0, 70, 112},
	{0, 73, 112},
	{0, 76, 112},
	{0, 79, 112},
	{0, 81, 112},
	{0, 84, 112},
	{0, 87, 112},
	{0, 90, 112},
	{0, 92, 112},
	{0, 95, 112},
	{0, 98, 112},
	{0, 101, 112},
	{0, 103, 112},
	{0, 106, 112},
	{0, 109, 112},
	{0, 112, 112},
	{0, 113, 109},
	{0, 114, 106},
	{0, 116, 103},
	{0, 117, 101},
	{0, 119, 98},
	{0, 120, 95},
	{0, 122, 93},
	{0, 123, 90},
	{0, 125, 87},
	{0, 126, 85},
	{0, 128, 82},
	{0, 129, 79},
	{0, 131, 77},
	{0, 132, 74},
	{0, 134, 71},
	{0, 136, 69},
	{0, 137, 66},
	{0, 139, 63},
	{0, 140, 61},
	{0, 142, 58},
	{0, 143, 55},
	{0, 145, 53},
	{0, 146, 50},
	{0, 148, 47},
	{0, 149, 45},
	{0, 151, 42},
	{0, 152, 39},
	{0, 154, 37},
	{0, 156, 34},
	{0, 157, 31},
	{0, 159, 29},
	{0, 160, 26},
	{0, 162, 23},
	{0, 163, 21},
	{0, 165, 18},
	{0, 166, 15},
	{0, 168, 13},
	{0, 169, 10},
	{0, 171, 7},
	{0, 172, 5},
	{0, 174, 2},
	{0, 176, 0},
	{5, 176, 0},
	{10, 176, 0},
	{15, 177, 0},
	{20, 178, 0},
	{26, 179, 0},
	{31, 180, 0},
	{36, 180, 0},
	{42, 181, 0},
	{47, 182, 0},
	{52, 183, 0},
	{58, 184, 0},
	{63, 184, 0},
	{68, 185, 0},
	{74, 186, 0},
	{79, 187, 0},
	{84, 188, 0},
	{90, 189, 0},
	{95, 189, 0},
	{100, 190, 0},
	{105, 191, 0},
	{111, 192, 0},
	{116, 193, 0},
	{121, 193, 0},
	{127, 194, 0},
	{132, 195, 0},
	{137, 196, 0},
	{143, 197, 0},
	{148, 198, 0},
	{153, 198, 0},
	{159, 199, 0},
	{164, 200, 0},
	{169, 201, 0},
	{175, 202, 0},
	{180, 202, 0},
	{185, 203, 0},
	{191, 204, 0},
	{196, 205, 0},
	{201, 206, 0},
	{207, 207, 0},
	{208, 202, 0},
	{208, 196, 0},
	{209, 191, 0},
	{210, 185, 0},
	{211, 180, 0},
	{212, 174, 0},
	{213, 169, 0},
	{214, 163, 0},
	{214, 158, 0},
	{215, 152, 0},
	{216, 147, 0},
	{217, 141, 0},
	{218, 136, 0},
	{219, 131, 0},
	{220, 125, 0},
	{220, 120, 0},
	{221, 114, 0},
	{222, 109, 0},
	{223, 103, 0},
	{224, 98, 0},
	{225, 92, 0},
	{226, 87, 0},
	{227, 81, 0},
	{227, 76, 0},
	{228, 70, 0},
	{229, 65, 0},
	{230, 60, 0},
	{231, 54, 0},
	{232, 49, 0},
	{233, 43, 0},
	{233, 38, 0},
	{234, 32, 0},
	{235, 27, 0},
	{236, 21, 0},
	{237, 16, 0},
	{238, 10, 0},
	{239, 5, 0},
	{240, 0, 0},
	{240, 8, 8},
	{240, 16, 16},
	{240, 24, 24},
	{240, 32, 32},
	{240, 41, 41},
	{240, 49, 49},
	{240, 57, 57},
	{240, 65, 65},
	{240, 74, 74},
	{240, 82, 82},
	{240, 90, 90},
	{240, 98, 98},
	{240, 107, 107},
	{240, 115, 115},
	{240, 123, 123},
	{240, 131, 131},
	{240, 140, 140},
	{240, 148, 148},
	{240, 156, 156},
	{240, 164, 164},
	{240, 173, 173},
	{240, 181, 181},
	{240, 189, 189},
	{240, 197, 197},
	{240, 206, 206},
	{240, 214, 214},
	{240, 222, 222},
	{240, 231, 231},
	{143, 188, 143},
	{255, 255, 255},
	{0, 0, 255},
	{255, 255, 0},
	{255, 0, 0}

	};

	int i;



	PSIGNAL_RGB lut;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < 255; i++) {
		lut[i].r = tabAnalyseStd[i].r;
		lut[i].g = tabAnalyseStd[i].g;
		lut[i].b = tabAnalyseStd[i].b;
	}


	return lut;
}


/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalAnalyseRet (fonction) 
 * 
 * RESUME: Palette Analyse Retouchee  256 fausses couleurs pour affich. images 8 bits
 * 
 * DESCRIPTION: creation d'une palette Analyse Retouchee (pour + de luminosite) 256 fausse fausses couleurs (noir -> vert -> jaune -> rouge -> blanc) pour l'affichage de donnees 8 bits .
 * 
 * SYNTAXE:     PSIGNAL_RGB palette = IdPalAnalyseRet();  
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * role  : PSIGNAL_RGB representant la LUT de 256 couleurs sur 3
 * octets permettant d'afficher les images.
 * 
 * PARAMETRES:  aucun 
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */
PSIGNAL_RGB IdPalAnalyseRet(void)
{
	static RGB tabAnalyseRet[] = {

		{0, 0, 0},
		{1, 0, 1},
		{46, 0, 49},
		{0, 2, 68},
		{0, 3, 68},
		{0, 5, 69},
		{0, 6, 69},
		{0, 8, 70},
		{0, 9, 71},
		{0, 11, 71},
		{0, 12, 72},
		{0, 14, 72},
		{0, 15, 73},
		{0, 17, 74},
		{0, 19, 74},
		{0, 20, 75},
		{0, 22, 76},
		{0, 23, 76},
		{0, 25, 77},
		{0, 26, 77},
		{0, 28, 78},
		{0, 29, 79},
		{0, 31, 79},
		{0, 32, 80},
		{0, 34, 81},
		{0, 36, 81},
		{0, 37, 82},
		{0, 39, 82},
		{0, 40, 83},
		{0, 42, 84},
		{0, 43, 84},
		{0, 45, 85},
		{0, 46, 85},
		{0, 48, 86},
		{0, 50, 87},
		{0, 51, 87},
		{0, 53, 88},
		{0, 54, 89},
		{0, 56, 89},
		{0, 57, 90},
		{0, 59, 90},
		{0, 60, 91},
		{0, 62, 92},
		{0, 63, 92},
		{0, 65, 93},
		{0, 67, 94},
		{0, 68, 94},
		{0, 70, 95},
		{0, 71, 95},
		{0, 73, 96},
		{0, 74, 97},
		{0, 76, 97},
		{0, 77, 98},
		{0, 79, 98},
		{0, 81, 99},
		{0, 82, 100},
		{0, 84, 100},
		{0, 85, 101},
		{0, 87, 102},
		{0, 88, 102},
		{0, 90, 103},
		{0, 91, 103},
		{0, 93, 104},
		{0, 94, 105},
		{0, 96, 105},
		{0, 98, 106},
		{0, 99, 107},
		{0, 101, 107},
		{0, 102, 108},
		{0, 104, 108},
		{0, 105, 109},
		{0, 107, 110},
		{0, 108, 110},
		{0, 110, 111},
		{0, 111, 112},
		{0, 113, 109},
		{0, 114, 106},
		{0, 116, 103},
		{0, 117, 101},
		{0, 119, 98},
		{0, 120, 95},
		{0, 122, 93},
		{0, 123, 90},
		{0, 125, 87},
		{0, 126, 85},
		{0, 128, 82},
		{0, 129, 79},
		{0, 131, 77},
		{0, 132, 74},
		{0, 134, 71},
		{0, 136, 69},
		{0, 137, 66},
		{0, 139, 63},
		{0, 140, 61},
		{0, 142, 58},
		{0, 143, 55},
		{0, 145, 53},
		{0, 146, 50},
		{0, 148, 47},
		{0, 149, 45},
		{0, 151, 42},
		{0, 152, 39},
		{0, 154, 37},
		{0, 156, 34},
		{0, 157, 31},
		{0, 159, 29},
		{0, 160, 26},
		{0, 162, 23},
		{0, 163, 21},
		{0, 165, 18},
		{0, 166, 15},
		{0, 168, 13},
		{0, 169, 10},
		{0, 171, 7},
		{0, 172, 5},
		{0, 174, 2},
		{0, 176, 0},
		{5, 176, 0},
		{9, 176, 0},
		{13, 177, 0},
		{17, 178, 0},
		{22, 179, 0},
		{26, 180, 0},
		{30, 181, 0},
		{35, 182, 0},
		{39, 183, 0},
		{43, 184, 0},
		{48, 184, 0},
		{52, 185, 0},
		{56, 186, 0},
		{61, 187, 0},
		{65, 188, 0},
		{69, 189, 0},
		{74, 190, 0},
		{78, 191, 0},
		{82, 192, 0},
		{87, 193, 0},
		{91, 193, 0},
		{95, 194, 0},
		{100, 195, 0},
		{104, 196, 0},
		{108, 197, 0},
		{112, 198, 0},
		{117, 199, 0},
		{121, 200, 0},
		{125, 201, 0},
		{130, 202, 0},
		{134, 202, 0},
		{138, 203, 0},
		{143, 204, 0},
		{147, 205, 0},
		{151, 206, 0},
		{156, 207, 0},
		{160, 208, 0},
		{164, 209, 0},
		{169, 210, 0},
		{173, 211, 0},
		{177, 211, 0},
		{182, 212, 0},
		{186, 213, 0},
		{190, 214, 0},
		{195, 215, 0},
		{199, 216, 0},
		{203, 217, 0},
		{208, 218, 0},
		{212, 219, 0},
		{216, 220, 0},
		{221, 220, 0},
		{221, 221, 0},
		{221, 216, 0},
		{221, 212, 0},
		{222, 207, 0},
		{222, 203, 0},
		{222, 198, 0},
		{223, 194, 0},
		{223, 190, 0},
		{224, 185, 0},
		{224, 181, 0},
		{224, 176, 0},
		{225, 172, 0},
		{225, 167, 0},
		{225, 163, 0},
		{226, 159, 0},
		{226, 154, 0},
		{227, 150, 0},
		{227, 145, 0},
		{227, 141, 0},
		{228, 137, 0},
		{228, 132, 0},
		{228, 128, 0},
		{229, 123, 0},
		{229, 119, 0},
		{230, 114, 0},
		{230, 110, 0},
		{230, 106, 0},
		{231, 101, 0},
		{231, 97, 0},
		{232, 92, 0},
		{232, 88, 0},
		{232, 83, 0},
		{233, 79, 0},
		{233, 75, 0},
		{233, 70, 0},
		{234, 66, 0},
		{234, 61, 0},
		{235, 57, 0},
		{235, 53, 0},
		{235, 48, 0},
		{236, 44, 0},
		{236, 39, 0},
		{236, 35, 0},
		{237, 30, 0},
		{237, 26, 0},
		{238, 22, 0},
		{238, 17, 0},
		{238, 13, 0},
		{239, 8, 0},
		{239, 4, 0},
		{240, 0, 0},
		{240, 8, 8},
		{240, 15, 15},
		{240, 22, 22},
		{240, 29, 29},
		{240, 36, 36},
		{240, 43, 43},
		{240, 51, 51},
		{240, 58, 58},
		{240, 65, 65},
		{240, 72, 72},
		{240, 79, 79},
		{240, 87, 87},
		{240, 94, 94},
		{240, 101, 101},
		{240, 108, 108},
		{240, 115, 115},
		{240, 123, 123},
		{240, 130, 130},
		{240, 137, 137},
		{240, 144, 144},
		{240, 151, 151},
		{240, 159, 159},
		{240, 166, 166},
		{240, 173, 173},
		{240, 180, 180},
		{240, 187, 187},
		{240, 195, 195},
		{240, 202, 202},
		{240, 209, 209},
		{240, 216, 216},
		{240, 223, 223},
		{240, 230, 230},
		{143, 188, 143},
		{255, 255, 255},
		{0, 0, 255},
		{255, 255, 0},
		{255, 0, 0}

	};

	int i;

	PSIGNAL_RGB lut;

	lut = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);

	if (!(lut)) {
		IdErrPrintf("Erreur d'allocation de memoire palette\n");
		return 0;
	}

	for (i = 0; i < 255; i++) {
		lut[i].r = tabAnalyseRet[i].r;
		lut[i].g = tabAnalyseRet[i].g;
		lut[i].b = tabAnalyseRet[i].b;
	}


	return lut;
}


/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalReadAsciiFile  (fonction) 
 * 
 * RESUME: Lit un fichier Ascii Palette { i, r, g, b}
 * 
 * DESCRIPTION: Lit un fichier Ascii Palette { i, r, g, b}. 
 * le 'i' indique a quelle 'valeur de pixel' va correspondre
 * la 'fausse couleur' decrite par r, g, b.
 * Les 'valeurs de pixel' superieures a 255 sont ignorees.
 * Aucun controle n'est effectue quant a la duplication 
 * des 'valeurs pixel' (c'est la derniere qui sera retenue)
 * ni quant a la presence de valeurs absentes.
 * Les eventuelles 'valeurs de pixel' absentes de la liste 
 * des 'fausses couleurs' fournies sont laissees a leur 
 * valeur de gris nominale.
 * Le SIGNAL retourne a une longueur egale a la
 * 'valeur de pixel' maximum fournie dans la liste des
 * 'fausses couleurs'.
 * 
 * SYNTAXE:    PSIGNAL_RGB palette = IdPalReadAsciiFile(char * nomFich);   
 * 
 * RETOUR:      type  : PSIGNAL_RGB 
 * 
 * PARAMETRES:  
 * nom   : nomFich  
 * type  : char *
 * acces : I
 * role  : nom du fichier ...
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */

PSIGNAL_RGB IdPalReadAsciiFile(char *nfpal)
{
	FILE *fpal;
	PSIGNAL_RGB pal;
	int i, imax = 0;
	int r, g, b;

	if (nfpal == NULL) {
		IdErrno = IDERR_EMPTY_FILE_NAME;
		return (PSIGNAL_RGB) NULL;
	}

	fpal = fopen(nfpal, ID_RFILE_TEXT);

	if (fpal) {
		pal = (PSIGNAL_RGB) IdSigAlloc(256, SIG_RGB);
		if (!pal)
			IdErrPrintf(" echec Alloc Palette\n");
		else {
			for (i = 0; i < 255; i++) {
				pal[i].r = i;
				pal[i].g = i;
				pal[i].b = i;
			}

			while (EOF !=
			       fscanf(fpal, "%d %d %d %d", &i, &r, &g, &b)) {
				if (i > 255)
					continue;
				if (i > imax)
					imax = i;
				pal[i].r = r;
				pal[i].g = g;
				pal[i].b = b;
			}
			if (imax < 255)
				pal =
				    (PSIGNAL_RGB) IdSigModifLongueur(pal,
								     imax);
		}
	} else {
		IdErrPrintf(" echec Ouverture Fichier Palette\n");
		IdErrno = IDERR_OPEN_FAILED;
		return (PSIGNAL_RGB) NULL;
	}
	return (pal);
}

/* FUNCTION DESCRIPTION **************************************************
 * 
 * IdPalWriteAsciiFile  (fonction)
 * 
 * RESUME: Ecrit un fichier Ascii Palette {i, r, g, b}
 * 
 * DESCRIPTION: Ecrit un fichier Ascii Palette {i, r, g, b}. 
 * 
 * SYNTAXE:    int retCode  = IdPalWriteAsciiFile(char * nomFich, PSIGNAL_RGB pal);   
 * RETOUR:      type  : int 
 * role  : code de retour booleen.
 * 
 * 
 * PARAMETRES:  
 * nom   : nomFich  
 * type  : char *
 * role  : nom du fichier ds lequel on ecrit
 * 
 * nom   : pal 
 * type  : PSIGNAL_RGB
 * role  : palette
 * 
 * FICHIER:     idpalett.c
 * 
 * EXEMPLE:     
 * 
 * ******************************************************** END DESCRIPTION */

int IdPalWriteAsciiFile(char *nfpal, PSIGNAL_RGB pal)
{
	FILE *fp;
	int i, r, g, b;

	if (nfpal == NULL) {
		IdErrno = IDERR_EMPTY_FILE_NAME;
		return 0;
	}

	if (pal == NULL) {
		IdErrno = IDERR_POINTER_IS_NULL;
		return 0;
	}
	fp = fopen(nfpal, ID_WFILE_TEXT);
	if (!fp) {
		IdErrno = IDERR_OPEN_FAILED;
		return (0);
	}

	for (i = 0; i < IdSigDimX(pal); i++) {
		r = pal[i].r;
		g = pal[i].g;
		b = pal[i].b;
		fprintf(fp, "%d %d %d %d\n", i, r, g, b);
	}
	return (1);
}
