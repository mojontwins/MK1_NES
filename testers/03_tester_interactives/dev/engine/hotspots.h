// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// hotspots.h
// Hotspot management

void hotspots_load (void) {
	// Copies hotspots from ROM to RAM and initializes them
#ifdef ENEMS_IN_CHRROM
	bankswitch (l_enems_chr_rombank [level]);
	vram_adr (c_hotspots);
	rda = VRAM_READ; 	// Dummy read.
#else
	gp_gen = (unsigned char *) c_hotspots;
#endif

	for (gpit = 0; gpit < MAP_SIZE; ++ gpit) {
#ifdef ENEMS_IN_CHRROM
		ht [gpit] = VRAM_READ;
		hyx [gpit] = VRAM_READ;
#elif defined (HOTSPOTS_DYNAMIC)
		ht [gpit] = *gp_gen ++;
		hyx [gpit] = *gp_gen ++;
#endif
		hact [gpit] = 1;
	}
}

void hotspots_paint (void) {
	#if defined (ENABLE_EASY_OBJECTS) && defined (HS_TYPE_A)
		if (hrt >= HS_OBJ_MIN + 2*HS_USE_OFFS && hrt <= HS_OBJ_MAX + 2*HS_USE_OFFS) 
			rda = hrt - 2*HS_USE_OFFS;
		else if (hrt >= HS_OBJ_MIN + HS_USE_OFFS && hrt <= HS_OBJ_MAX + HS_USE_OFFS) 
			rda = HS_OBJ_EMPTY;
		else
	#endif
	{
		//rda = hrt;
		__asm__ ("lda %v", hrt);	
		__asm__ ("sta %v", rda);
	}

		__asm__ ("lda %v", rda);
		__asm__ ("bne %g", hotspots_paint_do);
		__asm__ ("rts");
	hotspots_paint_do:

	#ifdef ENABLE_RESONATORS
		if (hrt == HOTSPOT_TYPE_RESONATOR && res_on) rda = HOTSPOT_TYPE_RESONATOR_ON;
	#endif

	/*
	oam_index = oam_meta_spr (
		#ifdef DOUBLE_WIDTH
			hrx - scroll_x,
		#else
			hrx, 
		#endif
		hry + SPRITE_ADJUST, 
		oam_index, 
		spr_hs [rda]
	);
	*/

	// Three byte parameters x, y, oam_index
	// One word parameter X:A

		__asm__ ("jsr decsp3");		// Make room
		__asm__ ("ldy #2");			// Y will index SP

		#ifdef DOUBLE_WIDTH
			// hrx - scroll_x : If we are here, this fits in a BYTE
			__asm__ ("lda %v", hrx);
			__asm__ ("sec");
			__asm__ ("sbc %v", scroll_x);
		#else
			__asm__ ("lda %v", hrx);
		#endif

		// Store in the stack.
		__asm__ ("sta (sp), y");
		__asm__ ("dey");

		// hry + SPRITE_ADJUST
		__asm__ ("lda %v", hry);
		__asm__ ("clc");
		__asm__ ("adc #%b", SPRITE_ADJUST);

		// Store in the stack.
		__asm__ ("sta (sp), y");
		__asm__ ("dey");

		// oam_index
		__asm__ ("lda %v", oam_index);

		// Store in the stack.
		__asm__ ("sta (sp), y");

		// Store the address pointed at by 
		// the pointer in spr_hs [rda] into X:A
		// rda is small.
		__asm__ ("lda %v", rda);
		__asm__ ("asl a");
		__asm__ ("adc #<(%v)", spr_hs);	// LSB
		__asm__ ("sta ptr1");			// temporally save
		__asm__ ("lda #0");
		__asm__ ("adc #>(%v)", spr_hs);	// MSB
		__asm__ ("sta ptr1+1");			// MSB
		
		// Read the 16 bits value pointed to by ptr
		__asm__ ("ldy #1");
		__asm__ ("lda (ptr1), y");
		__asm__ ("tax");
		__asm__ ("dey");
		__asm__ ("lda (ptr1), y");

		__asm__ ("jsr %v", oam_meta_spr);
		__asm__ ("sta %v", oam_index);
}

void hotspots_create (void) {	
	#if defined (HOTSPOTS_DYNAMIC) || defined (ENEMS_IN_CHRROM)
		if (ht [HS_CUR_PANT] && hact [HS_CUR_PANT]) {
			hrt = ht [HS_CUR_PANT];
			hrx = hyx [HS_CUR_PANT] << 4;
			hry = hyx [HS_CUR_PANT] & 0xf0;
		} else hrt = 0;
	#else
		gp_gen = (unsigned char *) (c_hotspots + (HS_CUR_PANT << 1));
		//hrt = *gp_gen ++; rda = *gp_gen; 
		SET_FROM_PTR (hrt, gp_gen); ++ gp_gen;
		SET_FROM_PTR (rda, gp_gen);
		if (hrt && hact [HS_CUR_PANT]) {
			hry = rda & 0xf0; hrx = rda << 4;
		} else hrt = 0;
	#endif
}
