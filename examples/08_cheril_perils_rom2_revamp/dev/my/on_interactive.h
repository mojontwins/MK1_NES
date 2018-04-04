// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code. An interactive has just been interacted.
// You may add checks and set variables and stuff.

// rdc contains which interactive 'f' has been interacted.
// if rdc&128    -> container.
// if !(rdc&128) -> sprite face.

// Changed the IFs you can se in README.md for a switch & love.
// I also removed some rdc checks as they were tautology.
// Saved 67 bytes.

rdb = 0;

switch (level) {
	case 0:
		if (rdc == SPR_AMADOR) {
			rda = 0; rdb = 2;
			level0_gate = 1;
		} else {
			rda = 11; rdb = 12;
		}
		break;
	case 1:
		rda = 3; rdb = 5;
		break;
	case 2:
		if (rdc == SPR_GIRL) {
			rda = 6; rdb = 9;
		} else {
			rda = 10; rdb = 10;
		}
		break;
	case 3:
		rda = 13; rdb = 14;
		break;
	case 4:
		if (rdc == SPR_CHEMA) {
			rda = 15; rdb = 16;
		} else {
			rda = 17; rdb = 18;
		}
		break;
}

if (rdb) textbox_dialogue_do (rda, rdb);

// Original:

/*

if (level == 0) {
	if (rdc == SPR_AMADOR) {
		textbox_dialogue_do (0, 2);
		level0_gate = 1;
	} else {
		textbox_dialogue_do (11, 12);
	}
}

if (level == 1 && rdc == SPR_MANDEE) {
	textbox_dialogue_do (3, 5);
}

if (level == 2) {
	if (rdc == SPR_GIRL) {
		textbox_dialogue_do (6, 9);
	} else if (rdc == SPR_JON) {
		textbox_dialogue_do (10, 10);
	}
}

if (level == 3 && rdc == SPR_WORKER) {
	textbox_dialogue_do (13, 14);
}

if (level == 4) {
	if (rdc == SPR_CHEMA) {
		textbox_dialogue_do (15, 16);	
	} else if (rdc == SPR_PIRATE) {
		textbox_dialogue_do (17, 18);
	}
}

*/
