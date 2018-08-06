// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code. Timer has ticked (a second passed)
// timer_zero equals 1 if timer == 0.

if (underwater) {
	// spawn a bubble
	bubble_x = prx; bubble_y = pry - 8;	

	// Kill player.
	if (timer_zero) pkill = 1;
}
