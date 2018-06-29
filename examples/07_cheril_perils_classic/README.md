Cheril Perils Classic
=====================

As mentioned elsewhere, the third game made with MK1 but never finished was a port of Cheril Perils, which evolved into two lines of existence, one of which was abandoned. The other became Yun, but that's a long story.

Assets from the abandoned branch are scattered accross many of our releases. Very recently, I found the original project folder with the untouched, first version. I was touched with the visuals, but decided not to release it as it was crap, specially the sprite work (which consists on the original ZX Spectrum sprites).

I decided to reuse the levels with new visuals and port it to MK1_NES/AGNES 1.0 as an example, so here it is.

Resonators
==========

Resonators are a pretty custom feature of AGNES. They are push buttons which pause the enemies for a time during which the player can crush them. This is the main gameplay item of Cheril Perils (or Mega Cheril Perils for the SEGA Megadrive) and Cheril the Writer (another AGNES game), but this wasn't always the case. The original Cheril Perils was pretty simple: you just had to explore the map while crushing the enemies. No resonators.

This is the reason why I found interesting to let the user choose if he or she wants the resonator based or the classic gameplay.

This game also showcases proper Chac-Chac enemies (those crushing blades).

How I made this
===============

Activating, configuring and using resonators is fairly straightforward; in `config.h`:

```c
	#define HOTSPOT_TYPE_RESONATOR 	4
	#define HOTSPOT_TYPE_RESONATOR_ON 5

	#define ENABLE_RESONATORS
	#define RESONATOR_BASE_PATTERN			0
	#define RESONATOR_PALETTE				3
	#define RESONATOR_COUNTER_OFFS_X		4
	#define RESONATOR_COUNTER_OFFS_Y		7
```

Using this, everytime you place a hotspot type 4 in ponedor you get a working resonator on screen. Enemies can only be killed if resonators are on.

How do I get to have a "classic gameplay" option? In this game mode:

- Resonators mustn't be shown, i.e. hotspots type 4 must be artificially hidden.
- Enemies should be killable only by crushing them, not requiring a resonator.

Luckily, the resonators subsystem can be turned off partially (i.e. - resonators won't work) using the variable `res_disable`. If this variable is set, resonators will be deactivated - but they will still be there. This variable was conceived to make possible to have the resonators out of power at first and then making them work somehow. We are just going to abuse this feature.

First of all, we need a custom variable to store the game mode, in `my/extra_vars.h`

```c
	unsigned char mode_no_resonators; 
```

If this variable is set, resonators should be disabled (`res_disable = 1`) and they must be hidden. This is easily achievable via the `my/on_entering_screen.h` code injection point, as it gets executed on entering each screen:

```c
	if (mode_no_resonators) {
		res_disable = 1;
		if (hrt == HOTSPOT_TYPE_RESONATOR) hrt = 0;
	}
```

So if `mode_no_resonators` is set, `res_disable` is set to 1 and if the current hotspot (stored in `hrt`) is a resonator (`HOTSPOT_TYPE_RESONATOR`), the `hrt` is cleared - making the hotspot disappear for that screen.

`mode_no_resonators` is set in the title screen via custom code in `pres.h`:

```c
	void title (void) {
		scroll (0,0);
		pal_spr (palss0);
		pal_bg (palts1);
		unrle_vram (title_rle, 0x2000);

		_x = 5; _y = 18; pr_str ("SELECT AND PUSH START!");

		_x = 12; _y = 22; pr_str ("RESONATORS");
		         _y = 24; pr_str ("EASY MODE");

		bat_in ();

		while (1) {
			oam_meta_spr (84, 170 + (mode_no_resonators << 4), 0, sspl_00_a);
			ppu_waitnmi ();
			pad_read ();
			rda = mode_no_resonators;
			if (pad_this_frame & (PAD_SELECT|PAD_DOWN)) {
				++ mode_no_resonators; if (mode_no_resonators == 2) mode_no_resonators = 0;
			}
			if (pad_this_frame & PAD_UP) {
				if (mode_no_resonators) -- mode_no_resonators; else mode_no_resonators = 1;
			}
			if (mode_no_resonators != rda) sfx_play (SFX_USE, 0);
			if (pad_this_frame & PAD_START) break;
		}
		sfx_play (SFX_START, 0); delay (20);
		
		//level = game_mode & 1;
		//mode_no_resonators = (game_mode > 1);
		plife = mode_no_resonators ? 5 : 3;

		bat_out ();
	}
```

Besides this, the game has a very simple yet efective custom renderer. Every metatile in the tileset is duplicated in a slightly different version. The custom renderer chooses one of the duplicates at random for each tile. For tile N, its duplicate is at N+16, so our `map_renderer_customization.h` is as follows:

```c
	set_rand (1 + n_pant);
	gpit = 192; while (gpit --) {
		if (rand8 () & 1) map_buff [gpit] += 16;
	}
```

gives a really nice visuals with variations while the maps can be encoded using RLE44 and taking less ROM.

And that's it!
