Changes to do for final 1.0 version
===================================

Integrate enhancements introduced with Yun. Yun evolved from an older MK1 version, it was "forked" around 0.4, so many later stuff isn't included. But it contains sensible enhancements in terms both of size and speend which should be included!

[X] Dynamic OAM table building instead of fixed.
[X] Change fors for reverse whiles where possible.
[X] Add enemy types in Yun.
[X] Cambiar XY por XY
	[X] Bolts
		- `c_max_bolts`
	[X] Hotspots
[X] Enems, mix 50/50
	[X] Integrate monococos -> 11
	[X] Centralize rendering!
	[X] CLEANSE, for god's sake!
[X] Player
	[X] Cambiar colisión a `cm_two_points` ~ in fact, replace collision with Yun's.
[X] Optimize update list writes in `printer.h` [From goddess]
	[X] Remove every instance of `update_index = 0;` outside the function
[ ] Parametrize all maxs & values so multilevel is easier (half done!)
[ ] Rename stuff so modularizing is easier (`kill_enemy` -> `enems_kill`, for instance)
[ ] Revise propellers

Extra stuff

[ ] Undo/redo arrays in enems processing ?
[ ] `ENABLE_SLIPPERY`:
	- pice
	- `ICE_RX`
	- `ICE_AX`
[ ] `ENABLE_QUICKSANDS`:
	- `QUICKSANDS_SINK_VY`
