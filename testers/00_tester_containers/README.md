Tester Containers
=================

I had to test containers and how they interact with the script system and that's why I made this one. Not very complex, just testing stuff around.

- Containers from the script.
- Place (interactuable) sprites from the script.
- Tons of fixes.

What containers are
===================

"Containers" is a bad name. It's just spritey stuff you put on screen and let the player interact with. They were called "containers" as, originally, they were so: stuff to contain "objects". In fact, they were "graphical representations of flags". A container was "a flag on screen". It had a pair of coordinates and a graphic (which was the metasprite in the spr_it array which index was the value of the flag). I extended this to represent just sprites which contain nothing.

Containers exist in the screen where they are created.

So there's two kinds of containers as of v1.0:

- **Containers**: Represent a flag. Show the value of the flag as a graphic. To show the contents of flag f at (x, y) (tile coordinates):

```
    ADD_CONTAINER f, x, y
```

- **Sprites**: Just a fixed sprite. To show spr_it [m] at (x, y) (tile coordinates):

```
    ADD_SPRITE m, x, y
```

Containers will show `spr_it [flags [f]]` and sprites will show `spr_it [m]`, respectively.

Both kinds can be interacted. If you approach them and press B:

- **Containers**: The object in your pocket will be interchanged with that in the container. *Note that your pocket is in a flag when containers are activated*. Besides, `128 + <the flag number>` will be put to the Script System ARG, JUST_INTERACTED will be set and the fire scripts will be run. Don't worry about that 128. You can check the *exact* value using `IF ARG ~ <the flag number>` (note the ~ instead of the =).

- **Sprites**: The metasprite number will be put to ARG, JUST_INTERACTED will be set and the fire scripts will be run. You can check for interacted sprites using `IF ARG = <the metasprite number>` (note the =).

Confused, aren't you? I'm tired and not explaining things properly, but you are getting this for free.

Don't worry, you'll understand:

How I made this
===============

There are 5 screens laid out vertically. I've placed five containers, four of them contain items, and one of them is empty. There's a monkey closing your path at the top. You have to place one of the objects (the skull) in the empty container so the monkey disappears and you can exit the tower. Cheril will appear in the same screen as the monkey and will tell you what to do if you interact with her. The empty container where you can place objects will show a text every time you interact with it.

Configuration
-------------

Just the important things. But some good practices. For example, I won't be using collectible objects nor keys and bolts so it's good practice to do this and save space:

```c
    #define DEACTIVATE_KEYS
    #define DEACTIVATE_OBJECTS
```

I like animations for players when they interact with containers / hotspots / scripting whatevers. This one is shorter than cheril's usual crouching animation. Check the spritesheet and metasprites.h to understand this:

```c
    #define ENABLE_USE_ANIM
    #define USE_ANIM_MAX_FRAMES             8
    #define USE_ANIM_INTERACT_ON            3
    #define USE_ANIM_FRAMES_PER_STEP        4
```

I want text on screen:

```c
    #define ENABLE_TEXT_BOX
    #define TEXT_BOX_FRAME_TILE_OFFSET      0x30
```

As you may know, the supplied text box implementation needs 9 metatiles to show text boxes and those have to be in your tileset. I am planning to make this a bit less cumbersome in the future, but that's the way it is. Just check the tileset and notice they are laid out from 0x30 onwards (that is: 48).

Text boxes also need actual texts to be defined somewhere. When used from the scripting, you also need an index. If you define `ENABLE_TEXT_BOX` you have to add an `assets/custom_texts.h` file with the text. I will be needing just two for this game:

```c 
    // Custom texts

    const unsigned char custom_text0 [] = 
        "HI, UNNAMED HERO!%"
        "THIS IS CHERIL. IN%"
        "THIS TESTER YOU HAVE%"
        "TO PLACE THE SKULL%"
        "IN THE PEDESTAL.";
     
    const unsigned char custom_text1 [] = 
        "IT SEEMS THAT YOU%"
        "MADE YOUR OFFER!%"
        "IF YOU ARE NOT SURE%"
        "JUST TALK TO CHERIL.%"
        "TESTER AHOY!";
        
    const unsigned char * const custom_texts [] = {
        custom_text0, custom_text1
    };
```

More config stuff. Make sure you are **not** using `ENABLE_SIMPLE_OBJECTS` (check `00_tester_sideview` or `08_cheril_perils_rom2_revamp` to see them in action!). This is important. They don't work alongside containers - you don't need them anyway as they are used for the same purpose!

```c
//#define ENABLE_EASY_OBJECTS
```

And of course, activate scripting, enable containers, 4 is more than enough (we'll be creating a couple per screen at most), define which flag will contain the object you are carrying, and enable the fire zone capability so screen rectangles can launch the fire scripts (we'll be using a fire zone to detect you escaping from the tower).

```c
    #define ACTIVATE_SCRIPTING
    //#define CLEAR_FLAGS

    #define ENABLE_CONTAINERS
    //#define CONTAINERS_FROM_CODE
    #define CONTAINERS_MAX                  4   // As per screen
    #define FLAG_INVENTORY                  0

    //#define FIRE_ON_KILL

    //#define ENABLE_EXTERN_CODE
    #define ENABLE_FIRE_ZONE
```

One last thing:  to show the object you carry in the hud, you just have to define (and give a proper value to) those:

```c
    #define HS_INV_X                136 
    #define HS_INV_Y                11  
```

Apart from this, our `spr_it` metasprites array is setup so it contains:

```
    index   contents
    ------- -------------------------------
    0       the "empty" object 
    1       hotspot collectibles (not used)
    2       hotspot keys (not used)
    3       life refills
    4       a bottle
    5       an eye
    6       a skull
    7       a puff of smoke
    8       a nice sprite of Cheril
```

Scripting
---------

This one is fairly simple but contains some key items you'll like me to explain. First of all, use alias. They work like variables and they are easy to remember:

```
    DEFALIAS
        $PINV           0
        $CONT_BOTTLE    1
        $CONT_EYE       2
        $CONT_SKULL     3
        $CONT_PUFF      4
        $CONT_OFFER     5
    END
```

Note how `$PINV` is 0 as we defined in config.h with `FLAG_INVENTORY`. You can't really automate such things so you have to be careful and use the same numbers.

```
    ENTERING GAME
        IF TRUE
        THEN
            # Inventory: Empty
            SET $PINV = 0

            # Containers with stuff:
            SET $CONT_BOTTLE = 4
            SET $CONT_EYE = 5
            SET $CONT_SKULL = 6
            SET $CONT_PUFF = 7

            # An empty container
            SET $CONT_OFFER = 0
        END IF
    END
```

Remember, on `spr_it`, 4 = bottle, 5 = eye, 6 = skull, 7 = puff of smoke.

On `ENTERING_GAME` you are supposed to populate the containers. Remember: containers are flags. Give them values. 

Let's start from the top.

On screen 0 there's the exit. I like when you start right at the end, but can't yet reach it. It's Machiavelic. The monkey blocking your path is a tile which is on the tileset and actually in the map data, so you'll have to delete it when the puzzle is completed (if one can call this a puzzle). There's also Cheril around for you to talk to her.

```
    ENTERING SCREEN 0
        # If there's a skull in the pedestal...
        IF $CONT_OFFER = 6
        THEN
            SET TILE (8, 6) = 3
            SET_FIRE_ZONE_TILES 15, 0, 15, 6
        END

        IF TRUE
        THEN
            ADD_SPRITE 8, 2, 9
        END
    END
```

Here you see it in action: containers refere to actual metasprites in `spr_it`. If a container contains a "4" it means it will display metasprite 4 from `spr_it` and so on. So if metasprite "6" is a skull and a container (which _contains_ in a flag) is "6", then it _contains_ a skull. Man, seriously, I'm so tired. I'm writing in perfect Engrish.

So if the pedestal, that is, the container `$CONT_OFFER` (which isn't even in this screen) equals "6", this is, it contains the skull, then take the monkey out and create a fire zone so you can trigger the game ending.

This is the concept: _containers are flags_, you can interact with such _flags_ when you place them as containers on screen. Where are my pills?

Don't worry, we'll be placing some very soon.

`ADD_SPRITE` will add the cheril sprite. Remember Cheril si in metasprite 8. We place it at (2, 9). When the player approaches and press B, the fire script will be ran, `JUST_INTERACTED` will be set, and `ARG`'s value  will be 8, so:

```
    PRESS_FIRE AT SCREEN 0
        IF PLAYER_IN_X_TILES 15, 15
        THEN
            WIN GAME
        END

        IF JUST_INTERACTED
        IF ARG = 8
        THEN
            TEXTBOX 0
        END
    END
```

If the player touches the right edge of the screen then the game is won. Then, what we just talked about: the sprite interaction. `JUST_INTERACTED` will be set and `ARG` will equal 8. Then we show a textbox with text 0 as defined in `custom_texts.h`.

Screen 1 contains our first container, the one which represents `$CONT_OFFER`.

```
    ENTERING SCREEN 1
        IF TRUE
        THEN
            ADD_CONTAINER $CONT_OFFER, 8, 8
        END
    END
```

As `$CONT_OFFER` equals 0 (as you defined in `ENTERING_GAME`), it will show the "empty" sprite. If you interact with it carrying something (on $PINV or flag [0], remember you defined `FLAG_INVENTORY` as 0), the object you carry and the object in the container ('nothing') will get swapped. And the fire script (if present) will be executed.

```
    PRESS_FIRE AT SCREEN 1
        IF JUST_INTERACTED
        IF ARG ~ $CONT_OFFER
        THEN
            TEXTBOX 1
        END
    END
```

Note the ~ in the `ARG` line. This is a rule of thumb: You use "=" with sprites, "~" with containers. `IF ARG ~ $CONT_OFFER` will evaluate to true if you just interacted with container `$CONT_OFFER` on screen. Note that if you use `IF ARG = $CONT_OFFER`, this will evalate tu true only if you just interacted with the _sprite_ which metasprite index is that contained in the $CONT_OFFER flag. Aw. Yeah, I will give ya time to read this again slowly.

Next screens just place the remaining containers. One on screen 2, one on screen 3, and two on screen 4.

```
    ENTERING SCREEN 2
        IF TRUE
        THEN
            ADD_CONTAINER $CONT_BOTTLE, 4, 8
        END IF
    END IF

    ENTERING SCREEN 3
        IF TRUE
        THEN
            ADD_CONTAINER $CONT_EYE, 14, 4
        END IF
    END IF

    ENTERING SCREEN 4
        IF TRUE
        THEN 
            ADD_CONTAINER $CONT_SKULL, 1, 2
            ADD_CONTAINER $CONT_PUFF, 13, 3
        END IF
    END IF
```

Play the game and check the script and the files. You'll eventually catch it.
