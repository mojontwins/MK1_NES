// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code. An interactive has just been interacted.
// You may add checks and set variables and stuff.

// rdc contains which interactive 'f' has been interacted.
// if rdc&128    -> container.
// if !(rdc&128) -> sprite face.

if (level == 0 && rdc == SPR_AMADOR) {
	textbox_dialogue_do (0, 2);
	level0_gate = 1;
}

if (level == 1 && rdc == SPR_MANDEE) {
	textbox_dialogue_do (3, 5);
}

if (level == 2) {
	if (rdc == SPR_GIRL) {
		textbox_dialogue_do (6, 8);
	} else if (rdc == SPR_JON) {
		textbox_dialogue_do (9, 9);
	}
}
