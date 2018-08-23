// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add support for custom hotspots types here. Support for each
// new type of hotspot should be coded in a 'case', like this:

/*
	case HOTSPOT_TYPE_MY_OWN:
		rda = SFX_OBJECT;
		// do stuff
		break;
*/

// If rda != 0, the engine will play the sound in rda when getting
// the hotspot and will clear it.

// hrt contains the hotspot type, in case you need it.

	case HOTSPOT_TYPE_EMMERALD:
		rda = SFX_FANFARE;

		// Fill in the correct bit. Levels with emmeralds
		// are 1, 4, 7 and 10.
		pemmeralds |= bits [level_world];

		break;

	case HOTSPOT_TYPE_TOGGLE_OFF:
		hrt = HOTSPOT_TYPE_TOGGLE_ON;
		toggle_timer = 50;
		toggle_switch = !toggle_switch;		// Toggle!
		sfx_play (SFX_STEPON, 0);
	case HOTSPOT_TYPE_TOGGLE_ON:
		rda = 0;							// Do not clear hotspot!
		break;
