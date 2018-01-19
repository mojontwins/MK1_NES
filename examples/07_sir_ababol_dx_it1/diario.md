20180117
========

Preparando para el tutorial, vamos a hacer Sir Ababol DX. Lo primero es hacer que la versión nueva FUNCIONE, porque he cambiado de todo mezclándolo con Yun.

El primer hito es tener el Sir Ababol normal funcionando. Luego, probar todos los módulos (¡a ver como!).

Luego ya construir el DX en base a eso.

Plan asín guay
==============

Assets
------

- Hacer sets de gráficos: dos tilesets, de 16 tiles como el original. No dibujo nada para el primer tileset: creo que todo lo puedo sacar del Sir Ababol de NES. El segundo, lo paso rápido (es el de la zona sumergida).
- Spriteset: Sir Ababol del juego de NES, los enemigos recreados del mismo sitio, si puede ser. Sin importar los tamaños diferentes, of course. Los items, igual.
- Fuente: La fuente del juego original de NES.
- Convertir mapa. Dividir el mapa en dos: superficie y río subterraneo. Expandir con las herramientas. Retocar. Esto no irá en el tutorial, se supone que los mapas los hacemos a manaca.
- Convertir enemigos y retocar.

Programación
------------

Montar el proyecto básico con DOS fases, que ejecute solo la primera. En el tuto decimos que la segunda por ahora se queda "latente" (porque tendremos que hacer convivir dos motores y eso requiere programar). Esto me debería permitir probar el core:

- Multinivel (asignar valores desde un array de niveles).
- Enemigos básicos (lineales).
- Movimiento del player.
- Hotspots.
- Cerrojos.

Debug
-----

Esta parte puede ser un poco coñazo. Debería intentar, sobre esta base, probarlo todo. Disparos, tipos extra de tiles, tipos extra de enemigos, tiles que se rompen... TODO. Incluso la vista genital XD

Programación II
---------------

Aquí es donde montamos el nivel 2 por separado. Metemos el modo de nadal, metemos un selector fácil entre saltos / gravedad y nadal / empuje. 

Programación III
----------------

Aquí empalmamos los dos niveles haciendo una transición custom al tocar ciertas áreas de ciertas pantallas.

¿Y todo esto cuando? ¡¡Ahora no, tengo que terminar D'Veel'Ng!!

20180118
========

Mientras pienso un poco qué hacer con D'Veel'Ng (tengo que terminar una fase pero necesito que me vuelvan las ideas para no repetirme), voy a empezar un poco con esto así como de coña. Voy a documentar todos los pasos que sigo para que luego hacer el tutorial sea más sencillo.

Veamos, lo primero que he hecho es copiar de src/ a una nueva carpeta en examples/ y he creado los tilesets. Voy a hacer el spriteset.

~~

Con los sets hechos, voy a trabajar en el mapa. Lo primero es dividir el original en dos, luego expandir, y retocar.

~~

Ya están los mapas. Es interesante notar que en el mapa 0 (la superficie), en la fila 0, hay que hacer cierta composición en las pantallas:

- Estrellas aleatorias si tile = 0,
- Una tira con las montañas.
- La luna

Ahora toca estirar los enemigos. Voy a ver qué versión más moderna del estirador tengo, porque la de MK1 creo que era una puta basura XD ¿Había otra?

~~

enexpander2.exe de MK1 parece más compleja. ¿Está en MK2? - Sí, es la misma mismita. Bueno, menos da una piedra. El tema es que tendré que cortar el ene original en 2. Creo que tengo algo para eso... 

¡enemscutter.exe! Si es que tengo de todo para no trabajar XD Vamos a ver qué tal.

```cmd
	na_th_an@DESKTOP-F2O9141 C:\Users\na_th\Desktop\git\MK1_NES\examples\07_sir_ababol_dx_it1\enems
	> ..\utils\legacy\enemscutter.exe orig.ene orig0.ene 0 0 20 2

	na_th_an@DESKTOP-F2O9141 C:\Users\na_th\Desktop\git\MK1_NES\examples\07_sir_ababol_dx_it1\enems
	> ..\utils\legacy\enemscutter.exe orig.ene orig1.ene 0 2 20 1
```

~~

Esto tiene que tener algún buguillo. El tamaño del mapa resultante no está bien, pero meh. Pone "14 14". Wtf.

En efecto, hay un buguillo:

```freebasic0
	' mapw, maph, scrw, scrh, nenems
	Get #fIn, , d: map_w = d: d = w: Put #fOut, , d
	Get #fIn, , d: map_h = d: d = w: Put #fOut, , d
```
 

Lo corrijo, pero esto lo parcheo a hand.

Mierda, hay más cosas mal. Vamos a revisar el cutter y empezamos de nuevo.

~~

OK - Me faltaba contemplar que los hotspots fueran de 2 bytes.

```cmd
	na_th_an@DESKTOP-F2O9141 C:\Users\na_th\Desktop\git\MK1_NES\examples\07_sir_ababol_dx_it1\enems
	> ..\utils\legacy\enemscutter.exe orig.ene orig0.ene 0 0 20 2 2bytes
	....................
	....................
	Bytes per hotspot = 2

	na_th_an@DESKTOP-F2O9141 C:\Users\na_th\Desktop\git\MK1_NES\examples\07_sir_ababol_dx_it1\enems
	> ..\utils\legacy\enemscutter.exe orig.ene orig1.ene 0 2 20 1 2bytes
	....................
	Bytes per hotspot = 2

	na_th_an@DESKTOP-F2O9141 C:\Users\na_th\Desktop\git\MK1_NES\examples\07_sir_ababol_dx_it1\enems    
	> ..\utils\legacy\enexpander2.exe orig0.ene level0.ene ts0.png level0.map 2bytes debug             
	v0.2 20160729                                                                                      
	map size (20, 2)                                                                                   
	08 08 01.. [00]: (8, 8) -> (9, 10).                                                                
	00 08 00.. [01]: (0, 8) -> (0, 10).                                                                
	05 08 01.. [02]: (5, 8) -> (5, 10).                                                                
	07 05 02.. [03]: (7, 5) -> (8, 5).                                                                 
	08 07 01.. [04]: (8, 7) -> (9, 9).                                                                 
	00 00 00.. [05]: (0, 0) -> (0, 0).                                                                 
	07 08 01.. [06]: (7, 8) -> (8, 10).                                                                
	00 00 00.. [07]: (0, 0) -> (0, 0).                                                                 
	0D 08 01.. [08]: (13, 8) -> (14, 10).                                                              
	00 00 00.. [09]: (0, 0) -> (0, 0).                                                                 
	06 06 01.. [0A]: (6, 6) -> (6, 8).                                                                 
	08 06 01.. [0B]: (8, 6) -> (9, 8).                                                                 
	0A 01 01.. [0C]: (10, 1) -> (11, 1).                                                               
	0B 03 01.. [0D]: (11, 3) -> (12, 3).                                                               
	0B 08 01.. [0E]: (11, 8) -> (12, 10).                                                              
	0C 01 01.. [0F]: (12, 1) -> (13, 1).                                                               
	0C 07 01.. [10]: (12, 7) -> (13, 9).                                                               
	00 00 00.. [11]: (0, 0) -> (0, 0).                                                                 
	0A 08 01.. [12]: (10, 8) -> (11, 10).                                                              
	01 02 01.. [13]: (1, 2) -> (1, 2).                                                                 
	02 07 02.. [14]: (2, 7) -> (2, 9).                                                                 
	0D 08 01.. [15]: (13, 8) -> (14, 10).                                                              
	01 08 01.. [16]: (1, 8) -> (1, 10).                                                                
	0D 05 01.. [17]: (13, 5) -> (14, 5).                                                               
	04 03 02.. [18]: (4, 3) -> (4, 3).                                                                 
	04 07 01.. [19]: (4, 7) -> (4, 9).                                                                 
	00 00 00.. [1A]: (0, 0) -> (0, 0).                                                                 
	0B 08 01.. [1B]: (11, 8) -> (12, 10).                                                              
	08 06 01.. [1C]: (8, 6) -> (9, 8).                                                                 
	06 06 01.. [1D]: (6, 6) -> (6, 8).                                                                 
	01 06 02.. [1E]: (1, 6) -> (1, 8).                                                                 
	08 01 02.. [1F]: (8, 1) -> (9, 1).                                                                 
	06 06 01.. [20]: (6, 6) -> (6, 8).                                                                 
	06 01 01.. [21]: (6, 1) -> (6, 1).                                                                 
	0A 05 01.. [22]: (10, 5) -> (11, 5).                                                               
	01 02 02.. [23]: (1, 2) -> (1, 2).                                                                 
	0B 02 00.. [24]: (11, 2) -> (12, 2).                                                               
	00 00 00.. [25]: (0, 0) -> (0, 0).                                                                 
	00 00 00.. [26]: (0, 0) -> (0, 0).                                                                 
	0D 06 01.. [27]: (13, 6) -> (14, 8).                                                               
	DONE!                                                                                              
	                                                                                                   
	na_th_an@DESKTOP-F2O9141 C:\Users\na_th\Desktop\git\MK1_NES\examples\07_sir_ababol_dx_it1\enems    
	> ..\utils\legacy\enexpander2.exe orig1.ene level1.ene ts1.png level1.map 2bytes debug             
	v0.2 20160729                                                                                      
	map size (20, 1)                                                                                   
	00 00 00.. [00]: (0, 0) -> (0, 0).                                                                 
	06 08 03.. [01]: (6, 8) -> (6, 10).                                                                
	00 00 00.. [02]: (0, 0) -> (0, 0).                                                                 
	08 08 03.. [03]: (8, 8) -> (9, 10).                                                                
	00 00 00.. [04]: (0, 0) -> (0, 0).                                                                 
	00 00 00.. [05]: (0, 0) -> (0, 0).                                                                 
	00 00 00.. [06]: (0, 0) -> (0, 0).                                                                 
	09 02 03.. [07]: (9, 2) -> (10, 2).                                                                
	00 00 00.. [08]: (0, 0) -> (0, 0).                                                                 
	04 08 03.. [09]: (4, 8) -> (4, 10).                                                                
	00 00 00.. [0A]: (0, 0) -> (0, 0).                                                                 
	07 07 03.. [0B]: (7, 7) -> (8, 9).                                                                 
	00 00 00.. [0C]: (0, 0) -> (0, 0).                                                                 
	02 08 03.. [0D]: (2, 8) -> (2, 10).                                                                
	00 00 00.. [0E]: (0, 0) -> (0, 0).                                                                 
	00 00 00.. [0F]: (0, 0) -> (0, 0).                                                                 
	00 00 00.. [10]: (0, 0) -> (0, 0).                                                                 
	01 08 03.. [11]: (1, 8) -> (1, 10).                                                                
	00 00 00.. [12]: (0, 0) -> (0, 0).                                                                 
	06 05 03.. [13]: (6, 5) -> (6, 5).                                                                 
	DONE!                                                                                              
```

~~

Se me había olvidado la fuente. Probaremos con la original pero en alto contraste y sin sombriltas.

Ya que tengo todos los gráficos, vamos con el compile.bat.

Primero hacemos las paletas. Sólo hay tres, con mkts.exe:

```cmd
	echo Generating pals
	..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts0.png out=work\palts0.h label=palts0 silent
	..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts1.png out=work\palts1.h label=palts1 silent
	..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palss0.png out=work\palss0.h label=palss0 silent
	copy /b work\palts0.h + work\palts1.h + work\palss0.h assets\palettes.h > nul
```

Lo siguiente es hacer el script para construir tileset.chr. Aquí lo suyo es que vaya explicando cada orden, qué hace, y tal, empezando con que tenemos que rellenar 8192 bytes, 4096 para tiles de fondo y 4096 para los sprites, en dos secciones.

```
	# import_patterns.spt
	# Imports patterns for Sir Ababol DX NES

	# This is a mkts.exe script file
	# Needs mkts v0.7.0 20170725

	# Bank 0 (patterns for bg)

	# Font
	OPEN font.png
	PALS palts0.png
	CHARSET 0, 0, 32, 2

	# Tilesets are generated as tilemaps. They will be stored in this file:
	MAPFILE ..\dev\assets\tiledata.h

	# ts0
	OPEN ts0.png
	PALS palts0.png
	LABEL ts0
	MAPPED 0, 0, 2, 2, 16, 2, 20

	# ts1
	OPEN ts1.png
	PALS palts1.png
	LABEL ts1
	MAPPED 0, 0, 2, 2, 16, 1

	# Done with patterns for bg. Reset and fill with zeroes:
	RESETPOOL
	FILL 4096

	# Bank 1 (patterns for spr)

	# All sprites are in the same file, and use the same palette
	OPEN ss.png
	PALS palss0.png

	# Metasprites will be stored in this file:
	MAPFILE ..\dev\assets\spritedata.h

	# Main player, 9 frames, 2x3 patterns, generate flipped versions
	# hot point (where oam_meta_spr places its origin) is (-4, -8).
	# (player active rectangle is 8x16, centered bottom).
	LABEL sspl
	METASPRITESET 1, 1, 2, 3, 9, 1, -4, -8 FLIPPED

	# Enemies, 
	# Tall enemies, 4 frames, 2x3 patterns. No need for flipped versions
	# Hot point (where oam_meta_spr places its origin) is (0, -8).
	# (enems rectangle is 16x16, centered bottom).
	LABEL ssena
	METASPRITESET 1, 4, 2, 3, 4, 1, 0, -8

	# Wide enemies, 2 frames, 3x2 patterns. No need for flipped versions
	# Hot point (where oam_meta_spr places its origin) is (-4, 0).
	# (enems rectangle is 16x16, centered bottom).
	LABEL ssenb
	METASPRITESET 9, 4, 3, 2, 2, 1, -4, 0

	# Platforms, 2 frames, 2x2 patterns. No need for flipped versions
	# Hot point (where oam_meta_spr places its origin) is (0, 0).
	# (enems rectangle is 16x16, centered bottom).
	LABEL ssplat
	METASPRITESET 15, 4, 2, 2, 2, 1, 0, 0

	# Explosion frame, 2x2 patterns. No need for flipped version
	# Hot point (where oam_meta_spr places its origin) is (0, 0)
	LABEL ssexpl
	METASPRITESET 19, 4, 2, 2, 1, 1, 0, 0

	# Items, 4 faces, 2x2 patterns. No need for flipped versions
	# Hot point (where oam_meta_spr places its origin) (is 0, 0).
	# (items rectangle is 16x16, centered bottom).
	LABEL ssit
	METASPRITESET 1, 7, 2, 2, 4, 1, 0, 0

	# Done with patterns for spr. Fill with zeroes:
	FILL 8192
```

Y en compile.bat...

```cmd
	echo Exporting chr
	cd ..\gfx
	..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns.spt out=..\dev\tileset0.chr silent
```

Los dos sets de enemigos... Explicar el parámetro yadjust y por qué se pone a 1.

```
	echo Exporting enems
	cd ..\enems
	..\..\..\src\utils\eneexp3.exe level0.ene ..\dev\assets\enems0.h 0 1
	..\..\..\src\utils\eneexp3.exe level1.ene ..\dev\assets\enems1.h 1 1
```

Los dos mapas. Voy a probar cual de packed o rle (versión MK1) ocupa menos... Espera que rlemap2 me está escribiendo decos. Su puta madre. A esto le falta un hervor... Fixed

rlemap:

```
	..\..\..\src\utils\rlemap2.exe ..\map\level0.map ..\dev\assets\map0.h 20 2 15 0
	..\..\..\src\utils\rlemap2.exe ..\map\level1.map ..\dev\assets\map1.h 20 1 15 1
```

2915 y 2020 bytes (4935 para map data)

packmap:

```
	..\..\..\src\utils\mapcnvnes2.exe ..\map\level0.map 20 2 16 12 15 0 packed
	..\..\..\src\utils\mapcnvnes2.exe ..\map\level0.map 20 1 16 12 15 0 packed
```

3840 y 1920 bytes (5760 para map data)

Bueno, elegimos rlemap2. Copiamos las lineas en compile.bat

Lo siguiente son las lineas fijas de compilar. Esto dará errores, pero vamos a compilar ya para ver las cosas generadas y si van guay.

```
	cd ..\dev

	:justcompile
	if [%2]==[noscript] goto :noscript

	echo Building script
	cd ..\script
	..\..\..\src\utils\msc3nes.exe script.spt 30
	copy mscnes.h ..\dev\assets\ > nul
	cd ..\dev

	:noscript
	cc65 -Oi game.c --add-source
	ca65 ..\..\crt0.s -o crt0.o
	ca65 game.s
	ld65 -v -C ..\..\nes.cfg -o cart.nes crt0.o game.o runtime.lib

	del *.o
	del game.s

	echo DONE!
```

Comprobamos tiestos:

enems?.h - OK
map?.h - OK
mscnes.h (vacío) - OK
palettes.h - OK
spritedata.h - OK
tiledata.h - OK

Y por ahora lo dejamos. Bye - Cha

~~

Lo siguiente es meter todos estos assets como includes en game.c:

```c
	// **************
	// * const data *
	// **************

	#include "definitions.h"
	#include "config.h"
	#include "assets/palettes.h"
	#include "assets/map0.h"
	#include "assets/map1.h"
	#include "assets/enems0.h"
	#include "assets/enems1.h"
	#include "assets/spritedata.h"
	#include "assets/tiledata.h"
	#include "assets/metasprites.h"
```

Ese metasprites.h hay que hacerlo, con los arrays de los sprites. Vamos al lío:

Creamos el array para el player. Primero facing right, luego facing left. Los cells son, en este orden: 

```
	Standing,
	Walk0, Walk1, Walk2, Walk3,
	Jump,
	Swim0, Swim1, Swim2
```

Para el ciclo de nadar repetiremos el cell 7, Swim1, para que sean 4 y alternen 0 1 2 1.

```c
	// Player metasprites
	const unsigned char *spr_player [] = {
		// Facing right (offset 0)
		sspl_00_a,										// Standing (offset 0)
		sspl_01_a, sspl_02_a, sspl_03_a, sspl_04_a,		// Walk cycle (offset 1)
		sspl_05_a,										// Airborne (offset 5)
		sspl_06_a, sspl_07_a, sspl_08_a, sspl_07_a,		// Swim cycle (offset 6)

		// Facing left (offset 10)
		sspl_00_b,
		sspl_01_b, sspl_02_b, sspl_03_b, sspl_04_b,
		sspl_05_b,
		sspl_06_b, sspl_07_b, sspl_08_b, sspl_07_b,	
	};
```

Anotaremos todos los offsets para luego crearlos como defines.

Ahora creamos el array para los enemigos. El motor espera 4 cells por enemigo, con dos frames de animación para cada dirección (derecha, izquierda). Nosotros tenemos los mismos cells para ambas direcciones, por lo que los duplicaremos. Si estuviésemos muy jodidos de memoria modificaríamos el renderer, pero por ahora eso no es necesario.

Después ponemos la explosión (ssexpl), que debe estar en el array de los metasprites de los enemigos.

```c
	// Enemy metasprites
	const unsigned char *spr_enems [] = {
		// Linear enems and platforms (ids 1-4)
		ssena_00, ssena_01, ssena_00, ssena_01,			// Enem id 1, right, left, two cells (duplicated)
		ssena_02, ssena_03, ssena_02, ssena_03,			// Enem id 2, right, left, two cells (duplicated)
		ssenb_00, ssenb_01, ssenb_00, ssenb_01,			// Enem id 3, right, left, two cells (duplicated)
		ssplat_00, ssplat_01, ssplat_00, ssplat_01,		// Platform, right, left, two cells (duplicated)

		// Explosion (offset 16)
		ssexpl
	};
```

Por último, el array de metasprites de los items (hotspots). Dejaremos el id 0 vacío, porque el primer hotspot es el 1, y así ahorramos código. Se podría haber hecho con los enemigos pero meh.

```c
	// Item metasprites
	// nothing, object, key, refill, boots
	const char *spr_hs [] = {
		0, ssit_00, ssit_01, ssit_02, ssit_03
	};
```

Y con esto hemos terminado los metasprites.

Seguimos montando sprites. Vamos a dar valores correctos a las constantes `CELL_* ` de definitions.h

**definitions.h**

```c
	// Cell definitions for side view
	
	#define CELL_FACING_RIGHT	0
	#define CELL_FACING_LEFT	10

	#define CELL_IDLE			0
	#define CELL_WALK_CYCLE		1
	#define CELL_AIRBORNE		5
	#define CELL_SWIM_CYCLE		6
```

Ahora nos vamos a player.h y revisamos la selección de cell de animación. Este juego "empieza" siendo bastante estándar, pero habrá que modificarlo luego, cuando montemos el cambio salto / nadar.

**player.h**

```c
	// Frame selection for side view games

	if (ppossee || pgotten) {

		// On floor

		if (pvx > PLAYER_VX_MIN || pvx < -PLAYER_VX_MIN) {
			psprid = CELL_WALK_CYCLE + ((prx >> 3) & 3);
		} else {
			psprid = CELL_IDLE;
		}
	} else {
		psprid = CELL_AIRBORNE
	}

	psprid += pfacing;
```

Ahora voy a hacer una serie de compilaciones para tapar agujeros. Lo que sea "arreglar las patas" con tanto cambio no lo pondré aquí porque no es del tutorial.

~~

Aún no he terminado, pero...

Es la hora de meter la música. Copio los m_* del sir ababol original y los referencio en music.s

```c
	.export _m_title
	.export _m_ingame
	.export _m_gameover
	.export _m_ending
	
	.include "m_instruments.s"
	
_m_title:			.include "m_title.s"
_m_ingame:			.include "m_ingame.s"
_m_gameover:		.include "m_gameover.s"
_m_ending:			.include "m_ending.s"
```

Luego importo los símbolos en game.c

```c
	// Music
	extern const unsigned char m_title [];
	extern const unsigned char m_ingame [];
	extern const unsigned char m_gameover [];
	extern const unsigned char m_ending [];
```

Ahora mismo sólo "m_ingame" está referenciada en el motor, ya que el motor a pelo no trae pantallas fijas.

~~

OK, esto ya compila. Ahora es hora de configurar el motor. Luego tendré que (fuera de tutorial) hacer la configuración multi-level / single level, por ejemplo.

~~

Veamos. Abrimos config.h. 

Dimensión del mapa más grande, en pantallas:

```c
	#define MAP_SIZE				40
```

Definimos los tipos para los hotspots:

```c
	#define HOTSPOTS_WONT_CHANGE			// types of hotspots won't change
	#define HOTSPOT_TYPE_OBJECT		1
	#define HOTSPOT_TYPE_KEYS		2
	#define HOTSPOT_TYPE_REFILL		3
	#define HOTSPOT_TYPE_BOOT		4		// Custom for this game.
```

Nótese que el tipo "BOOT" no funcionará todavía, hay que introducirlo más adelante.

Nuestro mapa es RLE16 (rlemap2), y no tiene decoraciones. Luego habrá que meter código custom para las pantallas de exterior (montaña, estrellas y luna, además de variación random en las hierbas):

```c 
	//#define MAP_FORMAT_PACKED				// Each byte in map data = 2 tiles, 16 tiles max.
	#define MAP_FORMAT_RLE16				// RLE'd by rlemap2. 16 tiles max.
	//#define MAP_WITH_DECORATIONS			// Add decorations when use a 16 tiles mode.
```

Enemigos
--------

El id del cell de "sprite muriendo" es el 16, según definimos en metasprites.h, por tanto:

```c
	#define SPRITE_BADDIE_DYING 			16
```

No vamos a usar ningún tipo de enemigo además del lineal (por ahora), por lo que comentamos todas las definiciones que tienen que ver con los tipos extra de enemigos:

```c
	//#define ENABLE_FANTY
	//#define ENABLE_HOMING_FANTY
	[...]

	//#define ENABLE_SAW
	[...]

	//#define ENABLE_MONOCOCO
	[...]

	//#define ENABLE_COCOS
	[...]

	//#define ENABLE_PEZONS
	[...]

	//#define ENABLE_CHAC_CHAC
	[...]
```

Player
------

Por ahora sólo activaremos

```c
	#define PLAYER_HAS_JUMP
	#define PLAYER_KILLS_ENEMIES
```

Y posteriormente supeditaremos de forma custom el que se pueda matar enemigos a que se haya pillado el hotspot tipo 4 (las botas).

Parece ser que MK1 exigía el HUD abajo del todo ?! Voy a cambiar esto. Por ahora no tenemos hud, y pondré toda la info sobre el marco del juego. Comentamos además `KILLED_*` y `AMMO_*` para que el motor no los enseñe.

Por ahora voy a dejar los tiestos tal y como están en el movimiento, y los behs en config.h. Luego me pienso la mejor forma de hacer el multilevel y quitar mierda de aquí. Probablemente como MK2, o parecido.

~~

Esto compila y se inicia y a veces funciona, pero empieza con la pantalla en negro y un TUT TUT de morirse y a veces se cuelga. WTF? Me falla algo de la inicialización champión.

De entrada la música no empieza del tirón. Eso significa que algo MUY CHUNGO está pasando en la inicialización. Bueno, sabíamos que esto iba a pasar :-)

Estamos en la etapa de hacer esto funcionar de nuevo.

~~

Espera no, es que hace un fade in TELA de lento !? WTF ?!!? `fade_delay` está puesto a 1, así que hay algo que está follándoselo pero bien.

`fade_delay` está en BSS. Justo antes tenemos `update_list` y los buffers de pantalla. ¿Hemos inicializado bien `update_list`? Pues creo que nol.

Pero no es eso.

Mecano en mi puta calavera, el -Ln no te saca las variables en esta versión de cc65 ??! Espera, -Ln hace cosas que no son. Probemos con -m

Qué va, joder. Mierda puta. A ver cómo me apaño sin debugger.

Habrá que hacerlo en plan comando.

Vale, la joda está tras `draw_scr`. Código seminuevo. Bien. Y en la parte que pinta los tiles y rellena los buffers. Reviso esto y daré con la moña. 

Vale, ya sé. La posición donde se va a escribir en los bufferes se calcula para cada tile (vaya tela) en `draw_map_tile`, y se hace así: 

```c
rda = rdx + (rdy << 4);
```

Asuminedo que rdy termina en 0. Esto TENGO que cambiarlo, no arreglarlo, CAMBIARLO. Cuidao porque hay llamadas a `draw_map_tile`.

Y yo soy monguer. He metido `SCR_Y` a vellón cuando tengo `TOP_ADJUST` como en MK2. Joder hostia ya. Pero al menos me ha servido para optimizar una mierda de método.

OK

~~

[X] Tengo que revisar las coordenadas de colisión teniendo en cuenta que el muñeco se pinta en -4,-8 y que tiene 8x16. 
[ ] Arreglar las posiciones de los enemigos, que convertí sin mirar.

Pero esto mañana.

20180119
========

Cosas que he visto: 

[X] Los enemigos a la virulé es que creo que estoy exportando como YX (moderno) y esta peste espera XY. Esto es fácil de arreglar.
[ ] Remember que los enemigos y hotspots llevan yadjust = 1, ahora tengo que bajar todo el jugador también para yadjust = 1, y esto implica toda la colisión y todas las conversiones de coordenadas.

Lo primero es poner el SPRITE_ADJUST correctamente (restando 15).

Para ello tengo que revisar y cambiar o modificar los accessos a attr y qtile.

Se accede a attr:

- Para la colisión en `enem_fanty.h`, `enem_homing_fanty.h`, `enem_linear.h` y `enem_pursuers`. Tengo que revisar Yun y MK2 para buscar fixes sin pensar. Nah, Yun no, que no implementa nada de esto y no usa attr. [X]
- En `player.h`, concretamente en lo relacionado con `player_process_tile`. Aquí habrá que ver de qué forma lo resuelvo más indolora e incoloramente.
- En `playermods/bullets.h` para interactuar con las `BREAKABLE_WALLS`. Aquí puedo hacer el fix de coordenadas in situ, como en D'Veel'Ng.
- En `gargajo.h`, pero ya va siendo hora de que elimine esto, que era super custom para Bootee.
- En `propellers.h`, pero como no se utiliza desde una entidad con sprites, no hay que tocar.

Se accede a qtile:

- En `player.h` -> `player_process_tile`. Es para ver si el tile de tipo 10 con el que interactuamos es el 14 o el 15.

Voy a ello.

**Nota para luego**: ¿Y si me fumo todo esto de detectar primero beh=10 y luego decidir si es tile = 14 o 15 y detectarlo todo del tirón? Si pongo beh=11 para las cajas, con & 10 se detecta igual y ya lo tengo para luego. Bueh, cuando me ponga.

~~

Ya he adaptado todo menos `player_process_tile`, que es algo que tengo que replantear un poco. Cuando quite eso, me podré fumar `attr` y `qtile`.

Ahora voy a hacer un poco de limpieza: en el proceso me he librado de variables temporales por un tubo: `pt??`, `et??` y `bt?`. 

Las `pt??` se utilizan ya sólamente en `player_process_tile`. ¡Seeh! XD. Las demás están ya erradicadas.

~~

Diseñemos esto.

Veamos, esto parece que cuando te chocas con un obstáculo te pone una variable `wall` con la dirección en la que te has chocado. Esto no es del todo perfecto pero funciona y no se nota, **que es la clave cuando desarrollas en 8 bits, que puedas poner toda tu mierda asquerosa e inexacta y que no se note**.

Si se detecta `wall`, mira que el atributo SEA 10, y luego llama a `player_process_tile` con el tile donde está el centro del jugador y el tile siguiente, dependiendo de la dirección. Como digo, esto es un poco inexacto pero fucking funciona. 

Lo que quiero es dejarlo más chico y sin attr o qtile. Bien.

En vez de tener una detección en el choque que escribe `wall` con el valor correcto y luego otro bloque más abajo que mira `wall`, vuelve a calcular cosas, y llama a `player_process_tile` podría integrarlo todo en la detección de choque y aprovechar que las variables son globales para hacer fullerías en `player_process_tile`. 

Seh, es lo que haré.

~~

Si pongo 10 -> cerrojo y 11 -> empujable, ambas cumplen (& 10) y además ambas son "sinsentidos" (8+2, 8+2+1). 

~~

Se ha quedao todo mucho más chiquilino. Voy a compilar, batallar errores, y ver qué tal va de funcional.

Funcionan muchas cosas (tras apañar un par de olvidos garas). Voy a ver si van los hotspots y los cerrojos, voy a ver si todo lo básico funciona, y luego sigo con el tutorial.

~~

Tengo que ajusta a Sir Ababol para que solo salte 2 espacios.

De entrada los hotspots no funcionan - bueno, al menos no se muestran.
