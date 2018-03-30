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
		"HOLA, CHERIL, QUE%"
		"TAL TE VA TODO?%"
		"NO HAY CASI NADIE.%"
		"NO ES EL PEPINONI%"
		"AL QUE ESTAMOS%"
		"ACOSTUMBRADOS";

	const unsigned char dialogue_0_1 [] =
		"AL PRINCIPIO PENSE%"
		"QUE NO HABIA NADIE%"
		"POR LA LLUVIA, PERO%"
		"A LO MEJOR ES POR%"
		"LOS ZOMBIES, NO?%"
		"SI, ES POSIBLE.%";

	const unsigned char dialogue_0_2 [] =
		"PUEDEN MATARSE, YA%"
		"SABES. PARALIZALOS%"
		"CON LAS MAQUINAS%"
		"Y LUEGO LES SALTAS%"
		"EN LA CAMOLLA.%"
		"TE ABRO LA CANCELA%";

	const unsigned char dialogue_1_0 [] =
		"HOLA, CHERIL. TE%"
		"ACUERDAS DE MI? SOY%"
		"MANDEE LA MUSA. VEO%"
		"QUE SIGUES EN ESO%"
		"DE IR POR AHI PI-%"
		"SANDO ZOMBIES...%";

	const unsigned char dialogue_1_1 [] =
		"CASI TODOS LOS MA-%"
		"LOS DE POR AQUI SON%"
		"FACILMENTE ACCESI-%"
		"BLES, PERO LOS PEO-%"
		"RES SE ESCONDEN%"
		"TRAS LA CANCELA.%";

	const unsigned char dialogue_1_2 [] =
		"HAY ALGUNOS TEMPLOS%"
		"CERCA. LOS OBJETOS%"
		"DE LOS ALTARES ES-%"
		"TAN MAL PUESTOS, O%"
		"ESO DICEN POR AHI.%"
		"SUERTE, CHERIL!%";

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

	const unsigned char dialogue_1_0 [] =
		"HI, CHERIL. REMEM-%"
		"BER ME? I'M MANDEE%"
		"THE MUSE. I SEE%"
		"YOU ARE IN YOUR%"
		"USUAL ZOMBIE KILL-%"
		"ING SPREE!%";

	const unsigned char dialogue_1_1 [] =
		"MOST OF THE BAD%"
		"GUYS ARE EASY TO%"
		"REACH AROUND HERE,%"
		"BUY THE WORST OF%"
		"THEM DWELL BEHIND%"
		"THE CLOSED GATE.%";

	const unsigned char dialogue_1_2 [] =
		"THERE ARE SOME%"
		"TEMPLES NEARBY.%"
		"THE OBJECTS THERE%"
		"ARE MISPLACED,%"
		"OR SO THEY SAY.%"
		"GOOD LUCK, CHERIL!%";

#endif

// Dialogue portraits: 0 for none, number for index to spr_hs!

#define PORTRAIT_NONE 		0

const unsigned char dialogue_portraits [] = {
	SPR_AMADOR, SPR_AMADOR, SPR_AMADOR,
	SPR_MANDEE, SPR_MANDEE, SPR_MANDEE, 
};

const unsigned char * const dialogue_texts [] = {
	dialogue_0_0, dialogue_0_1, dialogue_0_2,
	dialogue_1_0, dialogue_1_1, dialogue_1_2,
};

// Defined dialogues for this game:
// Level 0, amador: 0..2
// Level 1, mandee: 3..5
