Building our first game: Sir Ababol DX
======================================

In 2010, while we were developing our first modular engine for the ZX Spectrum (MTE MK1, a.k.a. *la Churrera*), we created a number of games to test the engine. One of those games was **Sir Ababol**. The goal in the game was gathering 24 ababol flowers accross 40 screens. Some sections were blocked by locks which could be opened using keys.



Years later, when we were about to release **Sir Ababol 2**, also for the ZX Spectrum, we decided to remake the original and created **Sir Ababol DX**, which added a number of features: 20 extra screens containing an underground river you could dive, and the possibility of getting a special pair of boots which granted you the ability to crush your enemies.



We will be building a port of **Sir Ababol DX** using **MK1_NES/AGNES**. I've chosen this game for several reasons, mainly because:

- The *basic* **Sir Ababol** (the original game) is the most simple game you can implement in **MK1/AGNES**. That way we can learn how to use the toolchain without too much hassle.
- The *enhanced* **Sir Ababol DX** will require actual *modifications* to the engine. You can create many kinds of games without having to modify the engine, but I feel like showing you how you can bend the engine to make it do whatever you need.

We'll provide you with every asset you need to build the game (graphics, maps, enemy placement).

How games are built
-------------------

These steps usually work for me:

- Game design.
- Make the graphics.
- Build the maps.
- Place enemies and stuff.
- First build, debug, etc.
- Fixed screens (title screen, game over, etc).
- Polish.

With **Sir Ababol DX** we'll be actually following the list twice, as we are first creating the *basic* version, then *enhancing* it.

Game design
-----------

In the *basic* **Sir Ababol**, the main character (who, as you might have guessed, is called Sir Ababol) jumps around avoiding enemies and collecting 24 ababol flowers. There are keys to open locks. All enemies are "patrollers" which move around following linear trajectories.

In the *enhanced* **Sir Ababol DX**, an extra set of 20 screens depict an underground river. When Sir Ababol is in this new section, he will be diving. 

So when we get to this point, we'll add an extra level to implement the underground section. We'll have to modify the engine so it activates the jump and run engine for the first level, and the diving engine for the second. We'll have to connect both levels somehow, as well: when Sir Ababol jumps into a pond on the first level, we'll have to change levels and place the main character in the correct place, and vice versa.

Besides, in **Sir Ababol DX** there's a special item we can get: a pair of boots which will grant Sir Ababol the ability to jump over the baddies to crush them. We'll implement the boots as a custom hotspot type which will set a custom variable to true. We will activate "jump over enemies" in the engine, and make it only active if such variable is set via a custom modification.

Starting the project
--------------------

To start, just copy `src` to a new location and rename the folder to `sir_ababol_dx`.

--

Again, don't worry if there's too much information hovering around. Just keep going at a steady pace. Everything will fall in place soon.

We'll cover the graphics in the next entry.
