// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code. Just entered n_pant.
// Background is rendered, everything initialized.
// Screen is ON.

if (mode_no_resonators) {
	res_disable = 1;
	if (hrt == HOTSPOT_TYPE_RESONATOR) hrt = 0;
}
