// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// custom_center_detections.h
// at1 constains the tile beh at the center of the player sprite
// For this code to be included, enable CUSTOM_CENTER_DETECTIONS in config.h

if (!pflickering && !pbouncing) {
	if (at1 == 66) {
		// Custom: electricity rays
		if (elec_state == 2) {
			
			if (prings) {
				ring_create ();
				prings = 0;

				// Simulate hit
				pvx = -pvx; pvy = -64; 
				pbouncing = 16;
				pflickering = 100;
			} else pkill = 1;
		} 
	}
}
