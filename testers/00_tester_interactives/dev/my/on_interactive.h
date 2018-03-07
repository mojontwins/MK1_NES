
// rdc contains which interactive 'f' has been interacted.
// if rdc&128    -> container.
// if !(rdc&128) -> sprite face.

if (rdc == 8) {
	gp_gen = custom_text0; textbox_do (); 
}

if (rdc == (0x80 | 5)) {
	gp_gen = custom_text1; textbox_do (); 
}
