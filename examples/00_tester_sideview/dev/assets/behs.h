
// Add these:
// + 1 kills      (useless if + 8 obstacle)
// + 2 quicksands ( " , needs ENABLE_QUICKSANDS)
// + 4 platform   (side view)
// + 8 obstacle   (nullifies + 1 & + 2, see bellow)
// +16 breakable
// +32 conveyor   (pushes left, + 1 pushes right)
// +64 slippery

// Special values (= nonsensical combinations):
// 10 lock
// 11 pushable

const unsigned char behs0 [] = {
	0, 8, 8, 1,68, 2, 4,24, 8,36,37, 0, 0, 0,11,10
};

// 68 = 4 + 64, slippery platform
// 24 = 8 + 16, breakable obstacle
// 36 = 4 + 32, conveyor platform, pushes left.
