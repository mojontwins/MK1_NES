MK1_NES/AGNES Description
=========================

We will start by describing the engine, list its components, and introduce some basic concepts.

As you may know, **MK1_NES/AGNES** is a simple engine for flick-screen games. The gameplay items featured can be defined by a series of modules which can be turned on or off in several combinations. You can customize the engine further by means of code injections or by using the native, simple yet flexible scripting system.

Every feature can be turned or on off, and most features can be used in combination. Obviously the more stuff you turn on, the more code will be in the cart (so less space will be available for data).

Those are the basic components:

Map
---

The **map** is the representation of the game world. It is divided into **screens**. The map is composed by *tiles* measuring 16x16 pixels, as defined in a **tileset**. Each screen is a grid of 16x12 *tiles*. Each *tile* has an associated **behaviour** which defines how the main character and other actors interact with it. Using such behaviours we will be able to create obstacles, platforms, destructible scenery, etcetera.

There are some basic behaviours, such as "empty", "platform" (only side view), obstacle... And a number of *optional* behaviours which can be opted in:

- *tiles* which **kill**.
- **Quicksands** (only side view): if the player is over a tile marked as **quicksands**, it sinks slowly and can emerge by jumping.
- **Breakable**: it will break if hit (by a projectile or the player striking).
- **Conveyor belt** (only side view in v1.0): it will displace the player left or right, like a conveyor belt.
- **Slippery** (only side view in v1.0): the player will experiment low friction when standing over it.
- **Lock**: the *tile* represents a lock which the player can open using keys.
- **Pushable**: the player can push such *tiles* around.
- **Ladder** (only side view).
- **Float** (only side view): the player will be pushed upwards when passing through a tile with such behaviour.

Behaviours are created using numeric values and can be combined as long as the combination makes sense. For example, you can make a slippery platform by adding the *platform* and *slippery* values, or you can make an electrified, destructible fence by combining *kills* and *breakable*.

Main character
--------------

Represents the character or entity the player controls directly. There is a number of features which are directly related to the main character, for example:

- **Reaction when killed**: We can decide if we want the main character to bounce, flicker, respawn elsewhere or a combination of those whenever it gets killed.
- **Push**: The player can push *pushable* *tiles*.
- Activate a special "use animation" which is displayed when the player interacts with an interactive or hotspot (described later).
- **Hit** (only side view in v1.0): We can make the player able to punch (when standing) and/or kick (when airborne).
- **Shoot**: The player can shoot bullets. You can configure if there's **limited ammo**, it the bullets have a **life span**, and more.
- **Jump** (only side view in v1.0): with two different implementations.
- **Autojump** (only side view in v1.0): the main character jumps automaticly when hitting the ground (bounces).
- **Swim** (only side view).
- **Jet Pack** (only side view).
- **Jump over enemies** without suffering, with the ability to harm them.

Enemies
-------

Enemies are our main character's main antagonists. There are quite a number of different types of enemies as in v1.0, and adding more is relatively easy.

- **Patrollers**: they move around following a lineal, cyclic path.
- **Phantoms**: they hover around chasing the player. You can make them interact with obstacles or not. There are two kinds: those who relentlessly chase the player, or a mild type which only chases the player as it gets closer, getting back to the original position once the player is "out of sight".
- **Monococo**: they are stationary, and keep hidden until they eventually emerge, shoot the player, and hide again.  They can't be killed while they are hidden.
- **Cannon**, or **steady shooters**: stationary, they fire in the direction they are facing at a fixed pace.
- **Fishy**: a fish which jumps out of water.
- **Hitters** and **shooters** are specialized **patrollers** which also hit or shoot the main character.
- **Pursuers** (only top-down view in v1.0): there's an enemy *spawner* which produces an enemy which chases the player until it is killed. Then the *spawner* produces a new instance. You can configure the engine to make the *spawner* visible and killable. If you kill the *spawner*, no more enemies are produced.
- **Saws**: they describe a lineal trajectory, then sink, get back to the initial position, emerge, etc.
- **Chac Chac**: blades to place in nasty corridors.
- **Compiled**: they describe a behaviour you can program in a simple language (they can move around, wait, or shoot the player, in a loop).

Depending on the configuration, enemies can be killed on the first hit or have a life gauge requiring several hits.

Projectiles
------------

There are two kinds of projectiles in v1.0, but this may change in the future:

- **Bullets**, shoot by the main character.
- **Cocos**, shoot by some enemies.

In this implementation, **bullets** only harm enemies, and **cocos** only harm the main character.

Other actors
------------

There are other actors appart from the enemies and projectiles, for example:

- **propellers** (only side view): they create a column (height is configurable) of tiles with the *float* behaviour above them. They can be active or inactive.
- **Breakable tiles**: can be broken by being shoot, hit, or by walking over them.
- **Shines**: eye candy. A selected tile in your tileset can shine from time to time.
- **Resonators** (only side view): they pause the enemies for a while.
- **Warpers**: they take the player to elsewhere in the map.
- **Springs** (only side view): they produce a tile (generally, one which *kills*) when the player steps over them.
- **Interactives**: they can be object containers (see below) or just sprites which can be programmed to react if the main character interacts with them.

Timer
-----

There's a simple timer which counts in seconds and which can be used for several tasks.

Hotspots
--------

**Hotspots** are a "special spot" in each screen where you can place objects (see below), keys or refills (life, ammo or time refills).

Items
-----

The engine implements several item/inventory systems of growing complexity and features:

- **Collect'em all**: all items look the same and a counter is incremented each time you collect one. It's the simples options. The collectible items appear in the **hotspot** of each screen.
- **Easy objects**: you can have different kind of objects. You can put them in hotspots or in the player inventory. You can create "destination" hotspots where a certain kind of item can be used. You can make the player swap what he or she is carrying with what's stored in a hotspot, etc.
- **Interactives**: the most complex implementation, it lets you define a number of *containers* and place stuff in them rather than using **hotspots**.

-

Those are the main components of every game created using **MK1_NES/AGNES**. You should now try the examples and testers and try and identify the engine components in each game. There are `README.md` files in most examples and testers with lots of info about how games are built. 
