// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code. An interactive has just been interacted.
// You may add checks and set variables and stuff.

// rdc contains which interactive 'f' has been interacted.
// if rdc&128    -> container.
// if !(rdc&128) -> sprite face.

if (rdc == 8) {
	gp_gen = custom_text0; textbox_do (); 
}

if (rdc == (0x80 | 5)) {
	gp_gen = custom_text1; textbox_do (); 
}
