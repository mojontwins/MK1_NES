	
	// Defines the behaviour for each tile. Remember that if keys are activated, tile #15 is a bolt
	// and, therefore, it should be made a full obstacle!

	// 0 = Walkable (no action)
	// 1 = Walkable and kills.
	// 2 = Walkable and hides.
	// 4 = Platform (only stops player if falling on it)
	// 8 = Full obstacle (blocks player from all directions)
	// 10 = locks
	// 11 = pushable
	// 16 = Breakable (#ifdef BREAKABLE_WALLS)
	// 32 = Conveyro (+0 = left, +1 = right
	// You can add the numbers to get combined behaviours
	// Save for 10 (special), but that's obvious, innit?

const unsigned char behs0 [] = {
	0, 8, 8, 8, 0, 0, 0, 8, 0, 0, 8, 4, 8, 1, 8, 10,
	0, 0, 0, 0, 0
};

const unsigned char behs1 [] = {
	0, 0, 8, 1, 0, 8, 8, 8, 8, 8, 0, 8, 8, 8, 8, 8
};
