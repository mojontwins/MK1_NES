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

Lo siguiente es hacer el script para construir tileset.chr. Aquí lo suyo es que vaya explicando cada orden, qué hace, y tal, empezando con que tenemos que rellenar 8192 bytes, 4096 para tiles de fondo y 4096 para los sprites, en dos secciones. Mencionar por qué el recorte de los frames de nadal está centrado diferente y tal.

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

	# Main player, 6 frames, 2x3 patterns, generate flipped versions
	# hot point (where oam_meta_spr places its origin) is (-4, -8).
	# (player active rectangle is 8x16, centered bottom).
	LABEL sspl
	METASPRITESET 1, 1, 2, 3, 6, 1, -4, -8 FLIPPED

	# Main player swim, 3 frames, 2x3 patterns, generate flipped versions
	# hot point (where oam_meta_spr places its origin) is (-4, -4).
	# (player active rectangle is 8x16, centered).
	LABEL sspl2
	METASPRITESET 13, 1, 2, 3, 3, 1, -4, -4 FLIPPED

	# Enemies, 
	# Tall enemies, 4 frames, 2x3 patterns. No need for flipped versions
	# Hot point (where oam_meta_spr places its origin) is (0, -8).
	# (enems rectangle is 16x16, centered bottom).
	LABEL ssena
	METASPRITESET 1, 4, 2, 3, 4, 1, 0, -8

	# Wide enemies, 2 frames, 3x2 patterns. No need for flipped versions
	# Hot point (where oam_meta_spr places its origin) (is -4, 0).
	# (enems rectangle is 16x16, centered bottom).
	LABEL ssenb
	METASPRITESET 9, 4, 3, 2, 2, 1, -4, 0

	# Platforms, 2 frames, 2x2 patterns. No need for flipped versions
	# Hot point (where oam_meta_spr places its origin) is (0, 0).
	# (enems rectangle is 16x16, centered bottom).
	LABEL ssplat
	METASPRITESET 15, 4, 2, 2, 2, 1, 0, 0

	# Water enems 1, 4 frames, 2x2 patterns, No need for flipped versions
	# Hot point (where oam_meta_spr places its origin) is (0, 0).
	# (enems rectangle is 16x16, centered bottom).
	LABEL ssenc
	METASPRITESET 1, 7, 2, 2, 4, 1, 0, 0

	# Water enems 2, 2 frames, 2x2 patterns, generate flipped versions
	# Hot point (where oam_meta_spr places its origin) is (0, 0).
	# (enems rectangle is 16x16, centered bottom).
	LABEL ssend
	METASPRITESET 9, 7, 2, 2, 2, 1, 0, 0 FLIPPED

	# Explosion frame, 2x2 patterns. No need for flipped version
	# Hot point (where oam_meta_spr places its origin) is (0, 0)
	LABEL ssexpl
	METASPRITESET 19, 4, 2, 2, 1, 1, 0, 0

	# Items, 4 faces, 2x2 patterns. No need for flipped versions
	# Hot point (where oam_meta_spr places its origin) (is 0, 0).
	# (items rectangle is 16x16, centered bottom).
	LABEL ssit
	METASPRITESET 1, 9, 2, 2, 4, 1, 0, 0

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

	#include "assets/palettes.h"
	#include "assets/behs.h"
	#include "assets/map0.h"
	#include "assets/map1.h"
	#include "assets/enems0.h"
	#include "assets/enems1.h"
	#include "assets/spritedata.h"
	#include "assets/tiledata.h"
	#include "assets/metasprites.h"
	#ifdef MULTI_LEVEL
		#include "assets/levelset.h"
	#endif
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

Ahora creamos el array para los enemigos. El motor espera 4 cells por enemigo, con dos frames de animación para cada dirección (derecha, izquierda). Nosotros tenemos los mismos cells para ambas direcciones, por lo que los duplicaremos, excepto el pez, que sí los duplica

Después ponemos la explosión (ssexpl), que debe estar en el array de los metasprites de los enemigos. Habrá uno por nivel.

```c
	// Enemy metasprites
	const unsigned char * const spr_enems0 [] = {
		// Linear enems and platforms (ids 1-4)
		ssena_00, ssena_01, ssena_00, ssena_01,			// Enem id 1, right, left, two cells (duplicated)
		ssena_02, ssena_03, ssena_02, ssena_03,			// Enem id 2, right, left, two cells (duplicated)
		ssenb_00, ssenb_01, ssenb_00, ssenb_01,			// Enem id 3, right, left, two cells (duplicated)
		ssplat_00, ssplat_01, ssplat_00, ssplat_01,		// Platform, right, left, two cells (duplicated)

		// Explosion (offset 16)
		ssexpl
	};

	
	const unsigned char * const spr_enems1 [] = {
		// Linear enems and platforms (ids 1-4)
		ssenc_00, ssenc_01, ssenc_00, ssenc_01,			// Enem id 1, right, left, two cells (duplicated)
		ssenc_02, ssenc_03, ssenc_02, ssenc_03,			// Enem id 2, right, left, two cells (duplicated)
		ssend_00_a, ssend_01_a, ssend_00_b, ssend_01_b,	// Enem id 3, right, left, two cells 
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

Behs
----

Tendremos que editar assets/behs.h y añadir comportamientos para los dos sets de tiles (`behs0`, `behs1`).

Levelset
--------

Activamos `#ifndef MULTI_LEVEL` en config.h. Explicamos por qué comentamos `#define SCR_END`.

Abrimos levelset.h y rellenamos todos los valores (aunquen os valen los que hay por defecto; en el código que al final distribuya vendrá el mínimo de multilevel, que son DOS niveles.)

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

~~

Ya funcionan. Ahora toca comprobar los cerrojos.

20180121
========

Me he tirado este tiempo haciendo fixes para que todo funcione. Esto incluye el core (con todos los cambios y mejoras) y los elementos activos en el juego. Habría que probar mil cosas aún, pero me apetece seguir un poco con el tuto.

Por ejemplo, lo siguiente que haré será:

- Construir el marco y adaptar `hud.h`. Le acabo de preguntar a Anjuel (pero parece que no está disponible), porque el juego original era muy minimalista (una linea de texto con los tres marcadores arriba y una linea de texto con `SIR ABABOL - THE MOJON TWINS` debajo).
- Discutir las customizaciones al renderer de la pantalla.

Luego, para seguir, tendría que dejar listo la parte de multilevel. Quiero incluir esto en MK1 de una forma estándar:

- SINGLE_LEVEL coge todos los valores de config.h (constantes).
- MULTI_LEVEL hace que los `#define` resuelvan a variables, y se crea un array para darles valores a todas.

En el tutorial se puede mencionar (y aplicar al juego) que está bien comentar algunos arrays del multi level para ahorrar espacio y tiempo.

Otra cosa que tendré que tener lista para cuando toque el custom es el módulo de textos. El que usé en `in another forest` (en MK2) se puede simplificar y usar, y creo que vendría muy bien.

Pero hoy estoy exhausto, dudo que haga nada. Además, tengo que hacer un TODO del juego de la compo de nesdev para tener claro qué queda por hacer y distribuírmelo en el tiempo, que quedan 10 días.

20180122
========

Cosas que tengo que tener en cuenta para un multilevel:

```
c_pal_bg,
c_pal_fg,
c_ts_tmaps,
c_ts_pals,
c_map,
c_decos,
c_locks,
c_enems,
c_hotspots,
c_max_bolts,

SCR_INI
PLAYER_INI_X, PLAYER_INI_Y

SCR_END
PLAYER_END_X, PLAYER_END_Y

PLAYER_MAX_OBJECTS

```

Estoy, por un lado, haciendo que los `#define` de siempre resuelvan a accesos a arrays `l_*` en modo `PLAYER_MULTI_LEVEL`.

Luego modificaré la inicialización, de hecho la voy a encapsular para no seguir fuyiendo la mangustia.

~~

OK, multilevel bien. Ahora tengo que revisar todos los apuntes para el tutorial porque creo que faltan cosas o hay que cambiar cosas, sobre todo de listados y hostias.

~~

Voy a programar el nadal y tal. Lo suyo es que el tío que siga el tutorial pueda saltar al nivel 0 o 1, configurando 'a mano' el motor, porque aquí es cuando empiezan las modificaciones custom.

Veamos como era el swim de Ninjajar!:

- Desactiva la gravedad!
- Misteriosamente, pone `pgotten = 1`.
- Si No se pulsa ARRIBA o ABAJO, le resta `PLAYER_ASWIM >> 1` a `pvy`.
- Si se pulsa ABAJO, le suma `PLAYER_ASWIM` a `pvy`.
- Si se pulsa ARRIBA o SALTO, le resta `PLAYER_ASWIM` a `pvy`.

Los valores son:

	PLAYER_G 				48
	PLAYER_VY_FALLING_MAX	512
	PLAYER_AY_SWIM 			32
	PLAYER_VY_SWIM_MAX 		128

Con esto se consigue que, efectivamente, si no se pulsa nada vy reciba una aceleración de 48-16 = 32, o sea, caiga más despacio que en gravedad. Además, la velocidad máxima de caer será 128, no 512.

Parece un poco enrevesado pero veo que está muy preparado para poder aprovechar la gravedad que existe, simplemente modificándola a posteriori. 

El pequeño bloque de código se coloca tras el maneje de la G. Vamos a probarlo.

Para probar se pone level = 1 en game.c, remember! Además, este nivel no tiene objetos. Para probarlo y tal, le vamos a poner a manaca 1 objeto (que no existe), para que no salga automáticamente, en levelset.h:

```c
const unsigned char l_player_max_objects [] =	{ MAX_HOTSPOTS_TYPE_1_0, 1 /*MAX_HOTSPOTS_TYPE_1_1*/ };
```

~~

Bueno, con esto funcionando voy a hacer un "snapshot", para no perderme. El proyecto en este punto, para el tutorial, debe estar como snapshot-1--20180122.7z

Lo próximo es:
--------------

1.- Añadir customs al renderer en level 0 con `n_pant < 20` y en level 1.
2.- Añadir cambio de fase custom:

- La posición del jugador no se inicializa con la fase, sino al principio del juego.
- Añadir un array `remember_pant [1]`, para acordarse de la pantalla donde se estaba. Al final de `game_loop`, se hace:

```c
	remember_pant [level] = n_pant;
```

- Al asignar n_pant en `game_init` se asigna el valor de `remember_pant [level]`.
- Tras inicializar al jugador, si `level = 0 && n_pant >= 20` hay que dar a pvy el valor máximo de salto; si level = 1 hay que dar un empujón hacia abajo.
- Además, si `level = 0 && n_pant >= 20`, `py = 192 << FIXBITS`; y si `level == 1`, `py = 16 >> FIX_BITS`;
- Añadir una variable `level_switching` y ponerla a 0 antes de empezar el bucle del juego.
- Modificar en flickscreen.h: Si `pry == 0 && pvy < 0` con `n_pant < MAP_W `, en `level == 1`, hacer `level_switching = 1; break;`. Si `pvy >= 192 && pvy > 0` con `n_pant >= MAP_W` en `level == 0`, lo mismo.
- En game.c, controlar `level_switching == 1` tras salir de `game_loop ()`. Si está a 1, hacer `level = 1 - level` y seguir el loop.

Ojo como todo eso de arriba es muy compatible con meter más niveles independientes (game B, anyone?).

~~

Voy a intentar hacer funcionar Sgt. Helmet's Training Day con esta versión de MK1. A ver qué tal.

~~

Esto debe ser rápido. Es coger los tiestos de *Sgt. Helmet: Training Day* y compilarlos con la última versión del motor. Ni siquiera me voy a rayar en recortar gráficos ni hostias, cojo el tileset.chr original. 

Enemigos y mapas y tal es otra historia, claro, porque los formatos son diferentes.

Empiezo copiando de /src y metiendo los tiestos en las carpetas, a ver si me da tiempo esta tarde.

~~

Apaño compile.bat y me voy a comer.

~~

Pua, al final voy a tener que hacer la conversión de gráficos, que necesito los metatiles y metasprites. Pero tengo hablre.

Y ahora no me puedo poner a hacer esto. Fuckercio, porque tengo que coger el tileset, cambiarle los colores (en aquella época mágica trabajábamos a la virulé), y montar el spriteset como puedaw. Hum. A lo mejor me puedo apañar con el trabajo que hice en el port de MK2...

Voy a ver si puedo hacer el fullero.

~~

Ya está todo importado, voy a hacer metasprites.h.

```c
	// Player metasprites
	const unsigned char * const spr_player [] = {
		sspl_00, sspl_01,	// Right, 2 cells
		sspl_02, sspl_03,	// Left, 2 cells
		sspl_04, sspl_05,	// Up, 2 cells
		sspl_06, sspl_07	// Down, 2 cells
	};

	// Enemy metasprites
	const unsigned char * const spr_enems0 [] = {
		// Linear enems and platforms (ids 1-4)
		ssen_00, ssen_01, ssen_00, ssen_01, 			// Enem id 1, right, left, two cells (duplicated)
		ssen_02, ssen_03, ssen_02, ssen_03,				// Enem id 2, right, left, two cells (duplicated)
		ssen_04, ssen_05, ssen_04, ssen_05, 			// Enem id 3, right, left, two cells (duplicated)
		ssen_06, ssen_07, ssen_06, ssen_07,				// Enem id 4, right, left, two cells (duplicated)

		// Explosion (offset 16)
		ssexpl
	};

	// Item metasprites
	// nothing, object, key, refill, ammo
	const unsigned char * const spr_hs [] = {
		0, ssit_00, ssit_01, ssit_02, ssit_03
	};
```

Ahora config.h, a ver si recuerdo todo.

Cosas que probaré:

[ ] Basic genital y `TOP_OVER_SIDE`
[ ] `PLAYER_PUSH_BOXES` y `FIRE_TO_PUSH`.
[ ] `PLAYER_BOUNCES` y `PLAYER_FLICKERS`.
[ ] `WALLS_STOP_ENEMIES` (en lineales).
[ ] `ENABLE_PURSUERS` 
[ ] `PERSISTENT ENEMIES`
[ ] `PLAYER_CAN_FIRE`
[ ] `MAX_AMMO`
[ ] `BREAKABLE_WALLS`
[ ] `SCRIPTING`

Lo primero es que con todo esto activado *compile*. Luego ya probaré, tengo que apañar el frame en RLE (por ejemplo).

Ok - no compila. Tengo que hacer fixes. Luego haré diff con lo que tengo en la carpeta de Sir Ababol, y recompilaré Sir Ababol a ver si no rompo nada.

Vamos a ello, tengo 20 minutos a lo más.

~~

Tocamos:

engine/breakable.h
------------------

Usaba la solución penca de varios tilesets que tuve en tiempos, y había una variable no definida. Ahora he introducido las constantes `BREAKABLE_ERASE` y `BREAKABLE_BREAKING` para controlar los tiles que se dibujan en el proceso de romper bloques.

engine/player.h
---------------

Meto la función `player_register_safe_spot` en un bloque `ifdef` con `DIE_AND_RESPAWN`.

También se estaba llamando a `player_points_beneath` en modo genital, lo que no tiene sentido. 

Se estaba accediendo a player_frames para calcular el `psprid`. Lo modifico por otro código más sencillo que no necesita lookups.

engine/playermods/bullets.h
---------------------------

Se emplea el antediluviano `en_touched`, un flag redundante que se eliminó hace eones. 

engine/enengine.h
-----------------

Se queja de que no tiene `BADDIES_COUNT`. Parece que era un `#define` para contar *en algún sitio* todos los enemigos que había en la fase, seguro que para **Cheril Perils**. En efecto, `BADDIES_COUNT` se define como una variable simple si `PLAYER_KILLS_ENEMIES`, con la idea de meter en el scripting de alguna forma si aplicaba. Por ahora es fácil de eliminar la linea si `BADDIES_COUNT` no está definido.

Hay otro fallo que me da la cara en el siguiente archivo, así que tendré paréntesis no balanceados y su puta madre.

Vale, aislando trozos he visto que el fallo está en...

engine/enemmods/enem_pursuers.h
-------------------------------

Paréntesis no balanceados. Además había un typo en el cálculo de colisiones (cx en vez de cx1).

engine/mscnes.h
---------------

Vaya, está llamando a funciones de la splib2 (`sp_updatenow`). Es en el comando `SHOW`, no estoy seguro de cómo ha llegado ahí.

~~

Ya compila. Otra cosa es que ahora se cuelgue de colores.

Sí, el mapa se ve peo total. Pero algo es algo XD. Pronto más.

~~

Algunas cosas más:

- La inicialización del código que pinta las pantallas PACKED estaba mal y además usaba una multiplicación.
- He generalizado el cálculo de `SPRITES_ADJUST` en base a `TOP_ADJUST` y al hecho de que la pantalla se ajusta a `scroll (0, 8)`. Lo próximo es parametrizar también ese scroll, pero luego.
- La precarga de enemigos con `PERSISTENT_ENEMIES` aún tenía XY en vez de YX.
- Los tipo 7 no disparan el tipo correcto de enemigo. Mentira; lo que pasa es que no pintaban el frame correcto.
- El estado "apareciendo" no es persistente. En el motor original los espacios de los sprites eran fijos y bastaba con crear las cosas (y luego había que borrarlas). Ahora la OAM es dinámica. Hay que acomodar esto.
- El disparo está roto. Nuevos disparos invalidan a anteriores. No se pinta con el patrón correcto. Seguramente me hayan bailado los parámetros. Eso era. Además, usaba gpit y algunos de los calls también. Meto bi como en MK2.
- Los breakables no van nada finos. En primer lugar la detección es un tile más abajo: Hay que ajustar la y - 16. Luego parece no ser persistente la rotura, o se está modificando lo que no es. Veamos . . .  Arreglado.

Veamos el checklist, a falta del scripting:

[X] Basic genital y `TOP_OVER_SIDE`
[X] `PLAYER_PUSH_BOXES` y `FIRE_TO_PUSH`.
[X] `PLAYER_BOUNCES` y `PLAYER_FLICKERS`.
[X] `WALLS_STOP_ENEMIES` (en lineales).
[X] `ENABLE_PURSUERS` 
[X] `PERSISTENT ENEMIES`
[X] `PLAYER_CAN_FIRE`
[X] `MAX_AMMO`
[X] `BREAKABLE_WALLS`
[X] `SCRIPTING`

Qué bien :-)

~~

Con el scripting activo, parece que `TEXT_BOX` glitchea un poco (solo aparecen 16 letras). Seguramente habrá que ver en qué estado está el buffer al llamar y cosas así.

No, los textos están truncados en mscnes.h. Me suena a compilador pasándose de listo. Esta es la radio bingow. BINGOW.

Tiene toda la pinta de que va a funcionar, pero ahora no tengo muchas ganas de probar esto. Quizá en otro momento.

Pero guay todo.

~~

En realidad es tontería que me ponga a acabarlo, picha. Pero es codebase con la versión... Bue, por el momento tengo que echar un rato trasladando los cambios a la carpeta src y a la de Sir Ababol.

No pudiendo probar si va el scripting, de todos modos, voy a ver si sigo un poco con el Sir Ababol.

20180123
========

He seguido completando el juego y tal, y he tenido que arreglar un montón de cosas con los pushables... Pero sigue sin ir fino, en la dirección hacia abajo pierde muchos empujones, y tengo que ver por qué es.

~~

Medio portar Helmet me ha ayudado a corregir un montonaco de cambios. Ahora no sé si seguir con este juego o intentar ir activando las cosas que quedan para dejarlo ya pulido del todo...

Básicamente queda probar todos los tipos de enemigos, chac-chacs incluidos, más las cosas de escenario (propellers), implementar bien slippery, etc. 

Para hacer eso lo suyo es que montara un proyecto específico para ir activándolo todo, con una sola pantalla donde ir probando cada cosa.

~~

Ahora lo que toca de todos modos, en este juego, es añadir las cosas custom: renderer y cambio de fases.

Creo que la clave para montar un buen renderer en condiciones si vas a tener que dar varias pasadas es modificarlo para que solo se rellenen los buffers `map_attr` y `map_buff`, y luego hacer un dummy copier que pinte cada tile del buffer.

Creo que lo voy a hacer así y lo voy a fijar como definitivo, ¿no? Porque luego hacer modificaciones y tocheces es mucho más sencillo si es sobre el buffer.

~~

Voy a crear mapmods, y la forma actual de escribir el mapa será un `renderer_fast` y la nueva un `renderer_complex`. Pero antes tengo que resolver muy bien el tema de las dependencias.

- `draw_tile (x, y, tl)` pinta el tile tl en (x, y) absolutas.
- `map_set (x, y, n)` establece el tile n en (x, y) del area de juego, incluyendo buffers.
- `draw_map_tile (t)` pinta el tile t en (rdx, rdy) del area de juego. Necesita rdm = rdx | (rdy << 4) definido.

Ahora mismo, `draw_scr` delega en `draw_map_tile`. Recuerdo que había dependencias de `draw_map_tile` que podría, quizá, deshacer... Veamos.

Eu - pues no ! Sólo se usa desde draw_scr. Pero se usa en el pintado de decos y abriendo cerrojos.

Por tanto, `draw_map_tile` se queda aún en `renderer_complex`. Sólo voy a enramar la parte que pinta el tilemap y rellena los buffers.

~~

Eu no. En el renderer complex todo se hará sobre el buffer y luego se pintará en una pasada. Duplico draw_scr COMPLETA.

~~

Jur, esto ya está funcionando. Puedo hacerlo todo incluso mucho más limpio si meto toda la modificación en un archivo aparte para el caso más concreto: modificar antes de los decos.

~~

Lo he hecho muy guay. Hay un archivo *vacío* en /src/ que tienes que rellenar si vas a usar `MAP_RENDERER_COMPLEX`: `map_renderer_customization.h`. Es código que se incluye justo después de haber rellenado `map_buff` con la pantalla actual, antes de los decos y antes de borrar los cerrojos.

```c
// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Use this alongisde map_renderer_complex.h
// (#define MAP_RENDERER_COMPLEX)

// Reaching this point, map_buff has the decompressed map screen.
// You can write as many modifications as you like here:

// Modifications for Sir Ababol DX

for (gpit = 0; gpit < 192; gpit ++) {
	rdt = map_buff [gpit];

	// Precalc this random which will be used all around
	rda = ((rand8 () & 15) == 1);

	// Level 0, top row has many features
	if (level == 0 && n_pant < 20 && gpit < 112) {
		if (rdt == 0 && gpit == 68) { 
			rdt = 17;
		} else if (rdt == 0 && gpit >= 96) {
			rdt = 16;
		} else if (rda) {
			if (rdt == 0) rdt = 19;
			else if (rdt == 6) rdt = 18;
		}
	}

	// Level 1, random bubbles
	if (level == 1 && rdt == 10 && rda) rdt = 4;

	map_buff [gpit] = rdt;
}
```


20180124
========

Estaba pensando en cómo hacer las modificaciones necesarias para el enganche de los dos niveles sin guarrear demasiado, pero va a ser que no hay forma.

Voy a grabar un snapshot "snapshot-2--20180124.7z".

Sigamos el método (completándolo un poco):

1. **La posición del jugador no se inicializa con la fase, sino al principio del juego**

Esto, básicamente, comentar esto en mainloop.h, linea ~27:

```c
	// CUSTOM {
	/*
	px = (signed int) (PLAYER_INI_X << 4) << FIXBITS;
	py = (signed int) (PLAYER_INI_Y << 4) << FIXBITS;
	*/
	// } END_OF_CUSTOM
```

y linea ~63

```c
	// CUSTOM {
	/*
	pobjs = 0;
	pkeys = 0;
	*/
	// } END_OF_CUSTOM
```

Y añadir este bloque en game.c, justo donde inicializamos `level` (por ejemplo) (el bloque `#ifdef` que empieza en ~103):

```c
	#ifdef MULTI_LEVEL
			level = 0;
			// CUSTOM {
				pobjs = 0;
				pkeys = 0;

				px = (signed int) (PLAYER_INI_X << 4) << FIXBITS;
				py = (signed int) (PLAYER_INI_Y << 4) << FIXBITS;
			// } END_OF_CUSTOM
	#endif
```

2.- **Añadir adónde cambiar**

Sirve para que el inicializador del nivel sepa en qué pantalla hay que empezar. Cuando pasemos de la fila de abajo del nivel 0 (n_pant >= 20) a la fila del nivel 1, restaremos 20. Cuando pasemos del nivel 1 al nivel 0, restaremos 20.

```c
// CUSTOM {
	unsigned char n_pant_switch;
// } END_OF_CUSTOM
```

Añadimos una variable `level_switching` (en ram/bss.h) y la ponemos a cero en la inicialización del juego que empezamos antes eb game.c:

```c
		level = 0;
		// CUSTOM {
			pobjs = 0;
			pkeys = 0;

			px = (signed int) (PLAYER_INI_X << 4) << FIXBITS;
			py = (signed int) (PLAYER_INI_Y << 4) << FIXBITS;

			level_switching = 0;
		// } END_OF_CUSTOM
```

Al asignar n_pant en `game_init` (mainloop.h, ~20) se asigna el valor de `n_pant_switch`, si `level_switching` no es 0.

```c
	// CUSTOM {
	if (level_switching) n_pant = n_pant_switch; else
	// } END_OF_CUSTOM
	n_pant = SCR_INI;
```

** Modificaciones al movimiento si venimos de otro nivel **

Tras inicializar al jugador, si `level = 0 && n_pant >= 20` hay que dar a pvy el valor máximo de salto; si level = 1 hay que dar un empujón hacia abajo.

Además, si `level = 0 && n_pant >= 20`, `py = 192 << FIXBITS`; y si `level == 1`, `py = 16 >> FIX_BITS`;

```c
	player_init ();
	// CUSTOM {
	/*
	px = (signed int) (PLAYER_INI_X << 4) << FIXBITS;
	py = (signed int) (PLAYER_INI_Y << 4) << FIXBITS;
	*/
	switch (level) {
		case 0:
			if (n_pant >= 20) {
				pvy = -PLAYER_VY_FALLING_MAX;
				py = 176 << FIXBITS;
			}
			break;
		case 1:
			pvy = PLAYER_VY_SWIM_MAX << 1;
			py = 16 << FIXBITS;
			break;
	}
	// } END_OF_CUSTOM
```

En el propio player.h, en `player_init`, tenemos que quitar `pfacing` y moverlo fuera, con los otros, para que al cambiar de nivel siga mirando en la misma dirección. 

player_init.h:

```c
	// CUSTOM {
	/*
	#ifdef PLAYER_TOP_DOWN	
		pfacing = CELL_FACING_DOWN;
	#else
		pfacing = 0;
	#endif	
	*/
	// } END_OF_CUSTOM
```

game.c:

```c
	#ifdef MULTI_LEVEL
		level = 0;
		// CUSTOM {
			pobjs = 0;
			pkeys = 0;

			px = (signed int) (PLAYER_INI_X << 4) << FIXBITS;
			py = (signed int) (PLAYER_INI_Y << 4) << FIXBITS;
			pfacing = 0;

			level_switching = 0;
			has_boots = 0;
		// } END_OF_CUSTOM
	#endif
```

** Detección de cambios de nivel **

Modificar en flickscreen.h: Si `pry == 0 && pvy < 0` con `n_pant < MAP_W `, en `level == 1`, hacer `level_switching = 1; break;` y calculamos a qué pantalla vamos con `n_pant_switch`. Si `pvy >= 192 && pvy > 0` con `n_pant >= MAP_W` en `level == 0`, lo mismo:

```c
	if (prx == 4 && pvx < 0) {
		n_pant --;
		px = 244 << FIXBITS;
	} else if (prx == 244 && pvx > 0) {
		n_pant ++;
		px = 4 << FIXBITS;
	} else if (pry == 0 && pvy < 0) {
		if (level == 1 && n_pant < MAP_W) {
			n_pant_switch = n_pant + MAP_W;
			level_switching = 1; break;
		} else {
			n_pant -= MAP_W;
			py = 192 << FIXBITS;
			if (pvy > -PLAYER_VY_JUMP_MAX) pvy = -PLAYER_VY_JUMP_MAX;		
		}
	} else if (pry >= 192 && pvy > 0) {
		if (level == 0 && n_pant >= MAP_W) {
			n_pant_switch = n_pant - MAP_W;
			level_switching = 1; break;
		} else {
			n_pant += MAP_W;
			py = 0;
		}
	}
```

En game.c, controlar `level_switching == 1` tras salir de `game_loop ()`, en ~118. Si está a 1, reset, hacer `level = 1 - level` y seguir el loop:

```c
	game_loop ();

	// CUSTOM {
	if (level_switching) {
		level_switching = 0;
		level = 1 - level;
	} else
	// } END_OF_CUSTOM
```

** Selección del tipo de motor (vertical) **

Activar *a la vez* `PLAYER_HAS_JUMP` y `PLAYER_SWIMS`. En `player.h`, guardamos el bloque `#ifdef PLAYER_SWIMS` con un if normal, pero con `if (level == 1)`, y lo contrario con la gravedad y el salto:

```c
	// Gravity

	// CUSTOM {
	//#ifndef PLAYER_SWIMS
	if (level != 1) {
	// } END_OF_CUSTOM
		if (!pj) {
			pvy += PLAYER_G;
			if (pvy > PLAYER_VY_FALLING_MAX) pvy = PLAYER_VY_FALLING_MAX; 
		}
	// CUSTOM {
	}
	//#endif
	// } END_OF_CUSTOM

	[...]

	// CUSTOM {
	//#ifdef PLAYER_SWIMS
	if (level == 1) {
	// } END_OF_CUSTOM	
		// Controller 

		if (!(i & (PAD_DOWN|PAD_UP))) {
			pvy -= PLAYER_AY_SWIM >> 1;
		}

		if (i & PAD_DOWN) {
			pvy += PLAYER_AY_SWIM;
		}

		if (i & PAD_UP) {
			pvy -= PLAYER_AY_SWIM;
		}

		// Limit
		if (pvy < 0 && pvy < -PLAYER_VY_SWIM_MAX) {
			pvy = -PLAYER_VY_SWIM_MAX;
		} else if (pvy > PLAYER_VY_SWIM_MAX) {
			pvy = PLAYER_VY_SWIM_MAX;
		}
	// CUSTOM {
	//#endif
	}
	// } END_OF_CUSTOM

	[...]

```

No se olviden de la selección de frame!

```c
		// Frame selection for side view games

		// CUSTOM {
		//#ifdef PLAYER_SWIMS
		if (level == 1) {
		// } END_OF_CUSTOM 
			if (i && (rdx != prx || rdy != pry)) {
				if (pvx) {
					psprid = CELL_SWIM_CYCLE + ((prx >> 3) & 3);
				} else {
					psprid = CELL_SWIM_CYCLE + ((pry >> 3) & 3);
				}
			} else psprid = CELL_SWIM_CYCLE + 1;
		// CUSTOM {
		//#else
		} else {
		// } END_OF_CUSTOM 
			if (ppossee || pgotten) {

				// On floor

				if (pvx > PLAYER_VX_MIN || pvx < -PLAYER_VX_MIN) {
					psprid = CELL_WALK_CYCLE + ((prx >> 3) & 3);
				} else {
					psprid = CELL_IDLE;
				}
			} else {
				psprid = CELL_AIRBORNE;
			}
		// CUSTOM {
		//#endif
		}
		// } END_OF_CUSTOM 

		psprid += pfacing;
	#endif
```

¡Y este rollo en mainloop.h ~113 sobre registrar el safe spot, importante!

```c
	// CUSTOM {
	//#if defined (DIE_AND_RESPAWN) && (defined (PLAYER_SWIMS) || defined (PLAYER_TOP_DOWN))
	if (level == 1) {
		// } END_OF_CUSTOM
		player_register_safe_spot ();
	// CUSTOM {
	//#endif
	}
	// } END_OF_CUSTOM
```

No se debería poder pisar a los enemigos, en enengine.h, en el if de "step over enemy", linea ~413:

```c
	// CUSTOM {
	level != 1 && 
	// } END_OF_CUSTOM		
```

~~

Eso de arriba debería dejarme ambos niveles enlazados. Voy a ver si va, si no, corregir, y actualizar el texto que acabo de escribir.

Jandero. Hay que tener en cuenta un problema con que te maten nada más entrar al nivel. Solución fácil: eliminar la posibilidad con el diseño del nivel, en concreto de la parte superior: eliminar los enemigos y obligar a saltar antes de encontrarse alguno. Esto se llama diseñar para timar, como siempre, engañar al chamán: circunnavegar debilidades de tu motor con el diseño de niveles porque, a fin de cuentas ¿quién se va a andar fijando?

~~

Lo siguiente que quiero meter es la caja de texto y la interacción con las botas. Aunque esté activado `PLAYER_KILLS_ENEMIES`, sólo se debe poder matar enemigos tras coger las botas. Esta modificación es muy sencilla y sólo necesitaremos una variable como flag, por ejemplo `has_boots`, en ram/bss.

Pero antes pensemos en los recuadros de texto. Puedo hacer una reimplementación hiper pulida y sencilla de los de **Cheril in another Forest** e incluirla en el paquete estándar por si se quiere usar.

Esta solución se basa en incluir un "custom tapestry" creado a base de un tilemap. Hay que añadir 9 tiles al tileset con los bordes de la caja y esta se imprime directamente con las funciones de actualizar el buffer, con mucho ojal porque con esta neslib el ancho de banda para actualizar es bastante menor que en la nueva.

El tema es que esto implica editar el tileset y ahora mismo eso está fuera de cuestión por temas de mirones.

Meh, hecho.

~~

Hum - es pequeño y sexy, pero lo he programado demasiado rápido. Tengo que probarlo pero ahora no va a poder ser.

Vamos añadiendo textos para intro y para coger las botas. Lo haremos en un archivo nuevo `assets/custom_texts.h` que incluiremos en game.c, al final de todos los assets...

```c
	#include "assets/custom_texts.h"
```

En custom_texts.h crearemos ambos textos así:

```c
	// Custom texts

	const unsigned char text_intro [] = 
		"THE RARE ABABOL%"
		"FLOWER GROWS AROUND%"
		"HERE. AN UNDERGROUND%"
		"RIVER, THERE IS. AND%"
		"THE LEGENDARY POWER%"
		"BOOTS YOU MAY FIND!";

	const unsigned char text_boots [] =
		"AND HERE THEY ARE!%"
		"NOW I'M INVINCIBLE.%"
		"WITH THOSE BOOTS,%"
		"CRUSH THE BADDIES I%"
		"CAN. FEAR ME, YOU%"
		"WIMPY MONSTERS!"
	;
```

~~ 

Definimos `has_boots` en nuestra zona custom de ram/bss.h:

```c
	// CUSTOM {
		unsigned char remember_pant [MAX_LEVELS];
		unsigned char level_switching;
		unsigned char has_boots;
	// } END_OF_CUSTOM
```

La inicializamos al empezar el juego, justo después de asignar `level`, por ejemplo, en la zona custom que ya tenemos:

```c
	// CUSTOM {
		px = (signed int) (PLAYER_INI_X << 4) << FIXBITS;
		py = (signed int) (PLAYER_INI_Y << 4) << FIXBITS;
		level_switching = 0;
		has_boots = 0;
	// } END_OF_CUSTOM
```

en config.h tenemos hotspots de tipo custom

```c
	#define HOTSPOT_TYPE_BOOT		4		// Custom for this game.
	#define HOTSPOT_TYPE_SIGN		5		// Custom for this game.
```

Añadimos soporte para el tipo de hotspot custom (que ya tenemos definido en config.h con el valor 4, por poner), en mainloop/hotspots.h @ ~70:

```c
	// CUSTOM {
		case HOTSPOT_TYPE_BOOT:
			has_boots = 1;
			break;
	// } END_OF_CUSTOM
```

~~

Va a quedar más mejor si para presentar los textos iniciales hay que leer un cartelito que pondré como hotspot custom (igual que en Ninjajar de MK3 de Spectrum XD).

Eso significa que tendré que mover el ababol de la primera pantalla a otro sitio. O poner el cartel en la segunda pantalla.

Este hotspot solo se debe activar si al tocarlo se pulsa FIRE. 


```c
		// CUSTOM {
			case HOTSPOT_TYPE_BOOT:
				gp_gen = text_boots;
				textbox_do ();
				has_boots = 1;
				break;

			case HOTSPOT_TYPE_SIGN:
				if (pad_this_frame & (PAD_B|PAD_DOWN)) {
					gp_gen = text_intro;
					textbox_do ();
				}
				break;
		// } END_OF_CUSTOM
	}

	// CUSTOM {
		/*
		sfx_play (rda, 1);
		hrt = 0;
		hact [n_pant] = 0;
		*/
		if (hrt != HOTSPOT_TYPE_SIGN) {
			sfx_play (rda, 1);
			hrt = 0;
			hact [n_pant] = 0;	
		}
	// } END_OF_CUSTOM	
```

~~

Obviamente no funfuña XD

- No se muestra el cartel.
- El recuadro sale mal.
- Coco-crash!

~~

Fixed esto y otras cosas más. Fala añadir que sólo se pueda matar a los enemigos cuando tienes las botas. En el mismo bloque custom de enengine.h, en ~414, añadimos para que quede:

```c
	// CUSTOM {
	level != 1 && 
	has_boots &&
	// } END_OF_CUSTOM	
```

Y con esto, si mal no me equivoco, ¡tendríamos listo el juego! Nada más que lo pruebe pongo un nuevo snapshot. Ahora hay que:

- Plantearse si meter una fase más con chac-chacks.
- Convertir en CNROM.
- Título / Ending / Game Over.

~~ 

Antes de seguir voy a hacer un tester de sideview para probar un montón de cosas:

- Tipos de enemigos.
- Slippery (que tengo que meter, está incompleto).X
- Quicksands (idem).X
- Empujables en vista lateral.
- Disparos en vista lateral.
- Breakables en vista lateral.
- ?????
- Profit

Luego tendré que hacer patch al main y a los ejemplos, claro.

~~

20180129
========

Entre que descanso de Miedow voy poniendo cosas. En el tester todo está programado y funcionando, y estoy ahora a vueltas con el tema de los inventarios y las hostias.

Hay dos cosas implementadas (obviamente, incompletas y sin funcionar, porque hice un batiburrillo bestial al ir pasando las versiones):

- Si estás usando scripting, el objeto que llevas debería ir a un flag y los contenedores deberían gestionarse con containers.h

- Si no estás usando scripting, el objeto que llevas va a pinv, y debería haber un rango de contenidos de hotspots que se consideren "objetos". Cuando tocas un hotspot (con posible interacción de PAD_B), tu pinv y el objeto del hotspot te intercambian. Para ello es necesario desactivar `HOTSPOTS_WONT_CHANGE`. Pero debería poder blindar este submotor y tenerlo todo junto.

De entrada voy a quitar `HOTSPOTS_WONT_CHANGE`, invertir la lógica, e introducir `HOTSPOTS_DYNAMIC`, implicando que pueden cambiar.

```c
#define ENABLE_EASY_OBJECTS
```

Eso debería activar, entre bambalinas, `CARRY_ONE_HS_OBJ` y `HOTSPOTS_DYNAMIC`, además de precisar un `(HS_INV_X, HS_INV_Y)`, un id de "no llevo nada" (`HS_INV_EMPTY`), y el rango de valores de hotspots que se consideran "objetos".

El problema de este tipo de juegos es que ya nos metemos en un rollo místico-custom, pero bueno, ahí está. Por ejemplo, lo más sencillo es acabar el juego cuando se comprueba que todos los objetos están en su sitio.

Todas esas cosas se pueden comprobar en mil sitios y tal, pero bueno, yo lo que voy a dar es la infraestructura básica.

20180130
========

Lo añado todo. Tengo `HS_USE_OFFS`. 

- Los hotspots entre `HS_INV_MIN` y `HS_INV_MAX`, inclusive, son objetos.
- Los hotspots entre `HS_INV_MIN + HS_USE_OFFS` y `HS_INV_MAX + HS_USE_OFFS`, inclusive, son donde se usan esos objetos, y se pintan con el objeto "vacío" `HS_INV_EMPTY`.
- Cuando pulsas `B` en un hotspot de valor H entre `HS_INV_MIN + HS_USE_OFFS` y `HS_INV_MAX + HS_USE_OFFS`, inclusive, y llevas el objeto `H - HS_USE_OFFS`, `pinv` pasa a valer `HS_INV_EMPTY`, y el valor del hotspot pasa a ser `H + HS_USE_OFFS`.

Por ejemplo, si:

```
	HS_INV_MIN = 10
	HS_INV_MAX = 19
	HS_USE_OFFS = 10
```

Y llevamos el objeto 14 a un hotspot con valor 24, el valor del hotspot pasará a ser 34 (¡y se pintará el objeto 14!).

Para aclararnos, a la hora de pintar el gráfico de un hotspot:

- Objeto: Si su valor está entre `HS_INV_MIN` y `HS_INV_MAX`, pintamos el metasprite correspondiente.
- Receptor: Si su valor está entre `HS_INV_MIN + HS_USE_OFFS` y `HS_INV_MAX + HS_USE_OFFS`, pintamos el vacío (`HS_INV_EMPTY`).
- Usado: Si su valor está entre `HS_INV_MIN + 2*HS_USE_OFFS` y `HS_INV_MAX + 2*HS_USE_OFFS`, pintamos el metasprite correspondiente - `2*HS_USE_OFFS`.

No es complicado, coño. 

Incluso he apañado dos includes, `mainloop/on_object_got.h` y `mainloop/on_object_used.h` para que no haya que engorrinar mucho.

~~

Ya está todo. Voy a compilar, luego ya haré cosas para probar que no la he liao.

20180202
========

Lo dejé funcionando. Ahora que se ha acabado la compo y estoy menos agobiao, voy a pensar en los próximos pasos, pero probablemente serán:

1.- Comprobar de una vez por todas que el scripting funciona bien. Para eso sólo tengo que echar una buena partida a Sgt. Helmet: Training Day remaster.

2.- Hacer unos enemigos tipo 7 para vista lateral: lo mismo, pero que sean capaz de avanzar y caer. Básicamente los tengo en MK2, son los que introduje para Vesta Vaal (tenían un nombre especialmente estúpido ¿Generatonis?).

3.- Introducir un hitter, no tanto como Ninjajar de ZX sino más parecido a Alex Kidd (como lo tengo implementado en el Maritrini).

4.- Con eso funcionando, pensar en un hitter volador (patada voladora), que se activable de forma separada.

Y después de esto ya he terminado.

Ayer pensé más seriamente en ampliar Sir Ababol DX con una fase que tenga:

- Lava.
- Chac-Chacs.
- Pezones (en realidad, gotas de lava).
- Fantis (sin homing)
- Suelos que se derrumban.

Hago un tileset de tonos rojizos reaprovechando gráficos y una fase lineal de 20x2. Ahora mismo hay 9728 bytes libres, cabe de sobra

20180205
========

Estuve el fin de semana juntando tileset y haciendo la fase extra de Helmet (a falta de poner enemigos). Habrá que buscar dos llaves para poner en el extremo final, todo muy lineal. Pero creo que en vez de ponerme con esto o a seguir ampliando el motor debería mirar un poco el tema del scripting, porque creo que no va fino (jugando rápido y mal el otro día no vi la moto seminueva avanzando bastantes pantallas, y creo que aparecía bastante temprano en el juego).

Me pongo con eso en los ratos muertos.

Otra cosa que me gustaría hacer (y me di cuenta de la necesidad al actualizar el motor de Helmet el otro día) es sacar completamente las cosas custom de engine, como por ejemplo todo pres.h o el dibujo del marco del juego de frame.h. Quizá debería incluir todo lo custom en una carpeta /dev/my: condiciones de los hotspots extendidos, selección del frame del personaje principal, customs en el renderer del escenario, variables extra... todo ahí. De esa forma la actualización será coger y soltar /engine, /ram y /mainloop (siempre que no hayamos hecho muchos custom, como en Ababol).

Voy examinando lo del script.

~~

Tengo que sacar los tiestos de `SCRIPTING_TEXT_BOX` de msc. No lo voy a volver a usar, mejor un extern o un TEXTBOX que llame a unas funciones que tengas que definir (como las de `textbox.h`).

~~

Ok - El amoto es en la pantalla 21, en el ENTERING. Voy a verlo primero en modo offline. Seguramente se ejecute el script fuera de ciclo de update. Um no, porque si no el texto no se imprimiría. Voy a fijarme mejor, quizá no exista el bug!

~~

Si que sale, joder XD.

Pues ya que estoy voy a apañar lo del textbox. Que sea TEXTBOX n y que n sea el índice de un array `ingame_texts` que tenga que estar definido, al igual que `textbox.h` activado.

20180206
========

Los juegos de hostias necesitan enemigos que hostien igualmente. Necesito ocupar un slot custom con enemigos que hagan esto:

1.- Patrollers que disparen cocos.
2.- Generadores de enemigos hostiadores.

Veamos cómo apañaríamos todo esto:

Patrollers que disparan cocos
-----------------------------

El principal problema de esto es que tengo frame de disparar coco, y ahora mismo el cálculo de frame de los patrollers de tipo 1-4 (incluye plataformas) implica que en_s = 4*(tipo-1), y ahí no me cabe el frame extra (dos frames en cada dirección).

La codificación mierder no me deja mucho margen, y no la quiero complicar porque, recuerden amigos, este era el motor fácil.

Aunque lo gestione como lineal, tendré que hacer algún maneje:

a) Inicialización: detectar el nuevo tipo (le asigno el siguiente libre, el tipo 12) junto con los lineales, y hacer todo igual menos la asignación de en_s, que irá sobre una constante de Sprite base.

b) Ejecución: ejecutar el tipo lineal normalmente, pero cambiar el cálculo de frame y además disparar un proyectil horizontal si el jugador está en un rango vertical alcanzable por el proyectil.

Debo definir a qué altura sale el proyectil y con qué offset horizontal. Además, tengo que crear cocos lineales. Pueden compartir todas las estructuras. Creo que así lo hice en Yun o en algún otro, no sé.

Voy a tocar los cocos y luego meto este tipo básico, aunque lo haga sin probar por ahora.

~~

La putanez va a ser modificar los bounding boxes, sobre todo si son más grandes de 16! - esto tengo que replantearlo, macho.

~~

Creo que lo primero es apañar un "big collision" para colisiones de hasta 24 pixels de alto. Ahora mismo hay una que puede variar de 1 a 16 pixels de alto, es necesario otra hasta 32 (porque necesita TRES puntos de colisión).

```c
#define TALL_PLAYER
```

Voy a montarme el tester en modo barebones para esto. Voy a crear enemigos lineales mierder aunque sean siempre el mismo con diferentes colores, necesito fillers y placeholders.

Por lo pronto, tall player. Los enemigos siempre van a ser de 16x16, pero modificaré las colisiones.

Tengo que repensar como voy a manejar las colisiones player<->enemigos.

~~

He metido `TALL_COLLISION` por ahora para los enemigos. Voy a meter algún enemigo y pruebo.

~~

Todo guay. Ya he propagado a master y me pongo con los shooties.

~~

Los shooties parecen funcionar perfectamente, y además están documentados. Voy a propagar y subo al gitano.

Patrollers que hostian
----------------------

Los punchies básicos serán añadidos a los patrollers que soltarán un sopapo cuando te tengan a tiro.

La colisión sopapal debe poder detectarse. Será sobre un cuadrado de 8x8 que colisionará como una bala sobre el jugador.

Bueno...

Player que hostia
-----------------

Ambos estados deberían durar N tiles, como hago en Maritrini. Aquí el rollo es que también hay una patada voladora que debería durar hasta que se registre un ppossee.

~~

He metido el tema y medio funciona, pero es tosco, sobre todo en la gestión de lo que le pasa a los enemigos cuando les das. Además, he tocado muchas partes y me he dado cuenta de que quizá haya que retocar un tanto el módulo de los enemigos, en especial la gestión de cómo se actualizan, qué frame muestran, etc.

Pero ahora ya no.

¿Qué debería hacer primero? ¿Adecentar el módulo de enemigos? ¿Intentarlo con las escaleras?

Ahora no tengo tiempo de ninguna de las dos cosas, pero puedo divagar sobre las escaleras.

Un problema muy grande es que no me quedan bits libres. Siempre puedo reaprovechar otro, vaya. 

[Otra cosa que tengo que hacer es integrar las hostias y patadas con los bloques que se rompen]

20180207
--------

Tareas asín:

[ ] Adecentar el módulo de enemigos, con pies de plomo. De todos modos, tengo testers bien montados y sólo sería tocar el módulo de enemigos.
[ ] Integrar hostias con bloques que se strozan.
[ ] Implementar las escaleras.

Más divagaciones escaleras: Las escaleras pueden complicarse ad infinitum pero no es mi intención hacerlo en MK1. Vamos a hacer las escaleras más sencillas posibles: las que no tienen principio o fin "en el aire", sino en obstáculos:

```
	XXXXXXXXXXXX
	    H
	    H
	XXXXH  XXXXX
	    H
	    H
	XXXXXXXXXXXX
```

Aquí no hay casos límites, y todo se reduce a una detección de lo que pasa en el centro inferior del rectángulo de colisión del muñaco. Si este pixel está dentro de un tile escalera (que debe ser walkable!), se pasa al modo escalera:

- Se puede andar a izquierda y derecha y saltar, of course. Si saltamos, NO estamos en modo escalera, ojal.
- Se puede andar arriba y abajo.
- En IDLE se conserva el frame anterior.
- La animación es la de escaleras.
- No aplica la gravedad.
- No se puede pegar.

Es parecido a la detección de Nadar en MK2: se debe hacer antes de nada y modificar el comportamiento del motor.

El tema ahora es cómo codificarlo. Lo bueno es que *no tiene sentido que los tiles escalera sean otra cosa*, por lo que se puede dar un valor concreto y detectar con `==`. Pero debe elegirse de forma que no colisione.

Si por ejemplo hago, digamos, conveyor+2, y ajusto conveyor para que si el bit 1 está ON no se detecte, podría encasquetarlo y compatibilizarlo, por poner un ejemplo. Lo malo es que esto me complica los quicksands. Pero los quicksands tampoco deberían combinar, con lo que puedo detectar los quicksands con `== 2` en lugar de con `& 2`.

~~

Creo que voy a empezar limitando bien los estados exclusivos en el motor, para luego integrar los bloques que se rompen con las hostias.

Creo que quizá empiece a ser buena idea que las hostias sean "proyectiles especiales", o algo así, para no tener que integrar tanto. Hum...

Esto me dejaría fumarme la detección de colisión con los enemigos, que es código redundante, y simplificar un poco los enemigos... Pero antes me gustaría que estuviesen algo más "limpitos". Y con los arrays sacados a variables y todo.

~~

Los conveyors sólo se detectan al colisionar hacia abajo, al igual que los slippery. Esto me está liberando `& 32` y `& 64` para tiles caminables, creo. De todos modos, ya he exclusivizado `== 2` los quicksands.

Creo que puedo detectar bien las escaleras con un simple `== 32` al principio de todo, ya que en este punto estaré bien posicionado al pixel sin colisionar. Voy a probarlo estableciendo un indicador de paleta.

~~

Hostia, lo he hecho entero y no he escrito nada. Hay que dejarlo fino. Ahora detecto la escalera con el pixel central inferior, pero a lo mejor debería detectar dos pixeles (y hacer un AND).

Además, no debo mostrar los frames de escalera si estoy caminando sobre suelo.

~~

Hecho, e integrados los breakable. Necesito efectos de sonido. Y necesito algún tipo de efecto de hostiazo, me refiero a cuando le damos a los enemigos que retrocedan algunos pixeles o algo. Pero tengo que pensar la forma de hacerlo y antes quiero dejar el modulo de enemigos más limpio.

