![AGNES logo](https://raw.githubusercontent.com/mojontwins/MK1_NES/master/wip/img/AGNES_logo_web.png)

MT Engine MK1 NES / AGNES
=========================

**MT Engine MK1 NES / AGNES** (or **MK1_NES/AGNES** for short) is a framework composed by a modular engine coded in C and a set of tools to create simple games for the NES platform. 

This engine was built as an excercise to learn to write simple games for the NES. It compiles using a fairly old version of cc65 (2.13.2), and uses the original neslib by **Shiru**.

The library was used in early 2014 and during 2015 to create several games, some of them released. You can find those games (cleaned up and ready to be compiled) in the `examples` folder, plus four new games created to test / feature the engine.

I've decided to clean **MT Engine MK1 NES** up and make it useable - I've ended rewriting most of it, as always. A tutorial is coming, but I'm still undecided on how to release it.

Update 20180901: **[v1.01](https://github.com/mojontwins/MK1_NES/releases/tag/v1.01) released**

How to Build
============

The engine / framework resides in `src`. The code, as is, features a placeholder game you are supposed to *replace* with your own. You can compile it as-is, of course. To do so, fix `src/dev/setenv.bat` with the correct path to `cc65_2.13.2/bin` and run it and then `src/dev/compile.bat`. 

To compile the tests, enter `tests` and run `make_tests.bat`. `cc65_2.13.2/bin` should be on the PATH.

To compile the examples, enter `examples` and run `make_examples.bat`. `cc65_2.13.2/bin` should be on the PATH.

Documentation
=============

* You can read about [engine features](https://github.com/mojontwins/MK1_NES/blob/master/docs/features.md) .
* [MKTS (GFX cutter & importer) reference](https://github.com/mojontwins/MK1_NES/blob/master/docs/mkts.md).
* Short description of [ANGES Metaspriteset](https://github.com/mojontwins/MK1_NES/blob/master/docs/metaspriteset.md)
* [Game configuration reference](https://github.com/mojontwins/MK1_NES/blob/master/docs/engine_config.md)
* [MSCMK1 (script compiler) reference](https://github.com/mojontwins/MK1_NES/blob/master/docs/mscmk1.md).
* [Code injection points](https://github.com/mojontwins/MK1_NES/blob/master/docs/code_injection.md)
* [MK1_NES / AGNES utilities](https://github.com/mojontwins/MK1_NES/blob/master/src/UTILS.md).
* There's a detailed postmortem on every tester and examples #7 onwards. 
* A tutorial is coming in the near future.

Evolution
=========

Although this isn't our main engine, we will be updating it in the future following a simple roadmap:

- Create a working UNROM-based template.
- Update to the latest version of the cc65 compiler.
- Update to the latest version of neslib.

Credits
=======

**MT Engine MK1 NES** was designed and developed by **The Mojon Twins** and relies on:

* **neslib** and **famitone**, written by Shiru.
* Some tools written by the **Mojon Twins**.
* The old **cc65 v2.13.2**

License
=======

**MT Engine MK1 NES** is copyleft The Mojon Twins and is distributed under a [LGPL license](https://github.com/mojontwins/MK1_NES/blob/master/LICENSE).

**But** if you make a game with the engine we understand you'll want to make it into a cart. **In such case, just tell us!** Write to mojontwins@gmail.com - just as a matter of courtesy.

The **games included in the `examples` folder** are now free to produce, which means **you can put them into carts and sell them, as long as you tell us first and send us a couple of copies**. You will eventually find assets (stickers and possibly cover art) alongside the game ROM in each game folder.

The **game assets** included in the testers and examples (graphics, music, etc.) are [donationware](https://en.wikipedia.org/wiki/Donationware). 

If you like this, you can [buy me a coffee](https://ko-fi.com/I2I0JUJ9).

Have fun.
