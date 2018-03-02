// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Containers definition

void containers_add (void) {
	containers_yx [containers_index] = sc_x;
	containers_f [containers_index ++] = sc_y;
}

void containers_paint (void) {
	gpit = containers_index; while (gpit --) if (rda = flags [containers_f [gpit]]) oam_index = oam_meta_spr (
		containers_yx [gpit] << 4, containers_yx [gpit] & 0xf0,
		oam_index,
		spr_hs [rda]
	);
}
