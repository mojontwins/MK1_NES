// NES MK1 v0.1
// Copyleft Mojon Twins 2013

// metasprites.h
// Metaspriteset.

// Main player, 8 frames (v. 1.0)

const unsigned char spr_pl1_1 [] = {
	0, 0, 0, 0, 8, 0, 1, 0, 0, 8, 2, 0, 8, 8, 3, 0, 128
};

const unsigned char spr_pl1_2 [] = {
	0, 0, 4, 0, 8, 0, 5, 0, 0, 8, 6, 0, 8, 8, 7, 0, 128
};

const unsigned char spr_pl1_3 [] = {
	0, 0, 8, 0, 8, 0, 9, 0, 0, 8,10, 0, 8, 8,11, 0, 128
};

const unsigned char spr_pl1_4 [] = {
	0, 0,12, 0, 8, 0,13, 0, 0, 8,14, 0, 8, 8,15, 0, 128
};

const unsigned char spr_pl1_5 [] = {
	0, 0,16, 0, 8, 0,17, 0, 0, 8,18, 0, 8, 8,19, 0, 128
};

const unsigned char spr_pl1_6 [] = {
	0, 0,20, 0, 8, 0,21, 0, 0, 8,22, 0, 8, 8,23, 0, 128
};

const unsigned char spr_pl1_7 [] = {
	0, 0,24, 0, 8, 0,25, 0, 0, 8,26, 0, 8, 8,27, 0, 128
};

const unsigned char spr_pl1_8 [] = {
	0, 0,28, 0, 8, 0,29, 0, 0, 8,30, 0, 8, 8,31, 0, 128
};

// Enemies, 4x2 frames (v. 1.0)

const unsigned char spr_en1_1 [] = {
	0, 0,64, 1, 8, 0,65, 1, 0, 8,66, 1, 8, 8,67, 1, 128
};

const unsigned char spr_en1_2 [] = {
	0, 0,68, 1, 8, 0,69, 1, 0, 8,70, 1, 8, 8,71, 1, 128
};

const unsigned char spr_en1_3 [] = {
	0, 0,72, 1, 8, 0,73, 1, 0, 8,74, 1, 8, 8,75, 1, 128
};

const unsigned char spr_en1_4 [] = {
	0, 0,76, 1, 8, 0,77, 1, 0, 8,78, 1, 8, 8,79, 1, 128
};

const unsigned char spr_en1_5 [] = {
	0, 0,80, 2, 8, 0,81, 2, 0, 8,82, 2, 8, 8,83, 2, 128
};

const unsigned char spr_en1_6 [] = {
	0, 0,84, 2, 8, 0,85, 2, 0, 8,86, 2, 8, 8,87, 2, 128
};

const unsigned char spr_en1_7 [] = {
	0, 0,88, 3, 8, 0,89, 3, 0, 8,90, 3, 8, 8,91, 3, 128
};

const unsigned char spr_en1_8 [] = {
	0, 0,92, 3, 8, 0,93, 3, 0, 8,94, 3, 8, 8,95, 3, 128
};

// Extra sprites:

const unsigned char spr_empty [] = {
	0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 128
};

const unsigned char spr_explosion [] = {
	0, 0, 128, 1, 8, 0, 129, 1, 0, 8, 130, 1, 8, 8, 131, 1, 128
};

// spritesets

const unsigned char *spr_enems [] = {
	spr_en1_1, spr_en1_2, spr_en1_3, spr_en1_4, spr_en1_5, spr_en1_6, spr_en1_7, spr_en1_8
};

const unsigned char *spr_player [] = {
	spr_pl1_1, spr_pl1_2, spr_pl1_3, spr_pl1_4, spr_pl1_5, spr_pl1_6, spr_pl1_7, spr_pl1_8
};
