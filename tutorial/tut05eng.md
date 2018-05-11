First test
==========

We will start copying the engine as is in a new folder and compiling it to generate the default game. This is just a simple test that everything is in place and working.

The first thing you should do is download AGNES if you haven't already. You can use a [ZIP containing the latest release](https://github.com/mojontwins/MK1_NES/releases) or clone the repo using:

```
    $ git clone https://github.com/mojontwins/MK1_NES.git
```

Our first project
=================

We need to:

1. Make sure we have a properly configured `setenv.bat` with the location of **cc65 2.13**.
2. Copy the contents of `src` to a new folder.
3. Compile the project.

A valid environment
-------------------

As mentioned, we need to add the `bin` subfolder of **cc65** to the system `PATH`. I prefer to do this in a *portable* way using a `setenv.bat` batch file and not making permanent changes to the system. 

Locate the file `src/dev/setenv.bat` and modify it to reflect your configuration. For example, if you cloned the repository into `c:\git\MK1_NES\`, the contents of `src/dev/setenv.bat` should be:

```
    @echo off
    set path=c:\git\MK1_NES\cc65_2.13.2\bin\;%path%
    cc65 --version
```

Creating the project
--------------------

Whenever we want to start a new project, we'll begin by copying the contents of `src` to a new location. In this example, I've cloned the repo to `c:\git\MK1_NES`, and I've created a `projects` subdirectory for my games. Inside `projects` I've created `first_test` to contain this first test and copied the contents of `src` inside `c:\git\MK1_NES\projects\first_test`. The results are as follows:

```
    c:\git\MK1_NES
    `-- projects
        `-- first_test
            |-- dev
            |-- enems
            |-- gfx
            |-- map
            |-- ogt
            |-- script
            |-- trash
            `-- utils
```

Compiling
---------

In this test we will compile the engine with the default configuration and all the placeholders (map, enemy placement, graphics, etc.). To do that, we open our favourite terminal (I suggest [ConEMU](https://conemu.github.io/)), move to our directory, and the run `setenv.bat`, then `compile.bat`. Note that you only have to run `setenv.bat` once everytime you open a terminal so **cc65** is added to the `PATH`.

```
    $ cd c:\git\MK1_NES\projects\first_test\
    $ setenv.bat
    $ compile.bat
```

After a funny process we should get a `cart.nes` file we can run and enjoy with our favourite emulator.



And that's all by now. Remember I have a [fetish for coffee](https://www.buymeacoffee.com/nath).