// Custom texts

#ifdef SPANISH

	const unsigned char text_propellers [] =
		"LA FABRICA VUELVE A%"
		"LA VIDA. PUEDES OIR%"
		"UN ZUMBIDO CONSTANTE.%"
		"LOS VENTILADORES YA%"
		"FUNCIONAN.%";

	const unsigned char text_open_gate [] =
		"PERFECTO. PARECE QUE%"
		"YA ESTA TODO EN SU%"
		"SITIO. SE OYE UNA%"
		"CANCELA OXIDADA QUE%"
		"SE ABRE NO MUY LEJOS.%";

	const unsigned char dialogue_0_0 [] =
		"EH, CHERIL, QUE%"
		"TAL ESTAS?%"
		"SEH, NO HAY NADIE.%"
		"NO ES EL PEPINONI%"
		"QUE CONOCEMOS...%";

	const unsigned char dialogue_0_1 [] =
		"AL PRINCIPIO PENSE%"
		"QUE NO HABIA NADIE%"
		"POR LA LLUVIA, PERO%"
		"A LO MEJOR ES POR%"
		"LOSZOMBIES, NO?%"
		"SI, ES POSIBLE.%";

	const unsigned char dialogue_0_2 [] =
		"PUEDEN MATARSE, YA%"
		"SABES. PARALIZALOS%"
		"CON LAS MAQUINAS%"
		"Y LUEGO LES SALTAS%"
		"EN LA CAMOLLA.%"
		"TE ABRO LA CANCELA%";
		
#else

	const unsigned char text_propellers [] = 
		"NOW THE FACTORY%"
		"COMES ALIVE! YOU%"
		"CAN HEAR A CONSTANT%"
		"HUM. PROPELLERS SEEM%"
		"TO BE IN WORKING%"
		"ORDER AGAIN!%";
	 
	const unsigned char text_open_gate [] = 
		"AHOY! OBJECTS ARE%"
		"IN PLACE... NOW%"
		"YOU CAN HEAR A%"
		"RUSTY FENCE OPENING%"
		"ELSEWHERE...%";

	const unsigned char dialogue_0_0 [] = 
		"HO, CHERIL, WHAT'S%"
		"UP? YA, NOBODY'ERE%"
		"I RECKON THIS IS%"
		"NOT THE GOOD OL'%"
		"PEPINONI, INNIT?%";

	const unsigned char dialogue_0_1 [] = 
		"FIRST I THO' THAT%"
		"DA LACK O'TOURISTS%"
		"WAS BECAUSE OF THE%"
		"RAIN, BUT MAYBE%"
		"IT'S THE ZOMBIES.%"
		"YEH, I THINK SO.%";

	const unsigned char dialogue_0_2 [] = 
		"THEY CAN BE KILLED%"
		"YA KNOW THE DRILL:%"
		"PARALIZE THEM WITH%"
		"THE MACHINES, THEN%"
		"JUMP ON THEM. LET%"
		"ME OPEN THE GATE.%";

#endif

// Dialogue portraits: 0 for none, number for index to spr_hs!

#define PORTRAIT_NONE 		0
#define PORTRAIT_AMADOR 	30

const unsigned char dialogue_portraits [] = {
	PORTRAIT_AMADOR, PORTRAIT_AMADOR, PORTRAIT_AMADOR,
};

const unsigned char * const dialogue_texts [] = {
	dialogue_0_0, dialogue_0_1, dialogue_0_2,
};

// Defined dialogues for this game:
// Level 0, amador: 0..2
