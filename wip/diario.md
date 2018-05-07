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
	#define PLAYER_STEPS_ON_ENEMIES
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

Se queja de que no tiene `BADDIES_COUNT`. Parece que era un `#define` para contar *en algún sitio* todos los enemigos que había en la fase, seguro que para **Cheril Perils**. En efecto, `BADDIES_COUNT` se define como una variable simple si `PLAYER_STEPS_ON_ENEMIES`, con la idea de meter en el scripting de alguna forma si aplicaba. Por ahora es fácil de eliminar la linea si `BADDIES_COUNT` no está definido.

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
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

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

Lo siguiente que quiero meter es la caja de texto y la interacción con las botas. Aunque esté activado `PLAYER_STEPS_ON_ENEMIES`, sólo se debe poder matar enemigos tras coger las botas. Esta modificación es muy sencilla y sólo necesitaremos una variable como flag, por ejemplo `has_boots`, en ram/bss.

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

20180208
========

Voy a empezar a remodelar los enemigos que es una basura desordenada. Lo primero que voy a hacer es mover las colisiones con las balas ahí, y ahorro vueltas (no se comprobarán los enemigos inactivos y tal).

~~

Hecho.

Pensando pensando, lo que necesitan los enemigos hostiados es un frame de punched. A ver como carajo integro esto con los slots de la animacion, que son automáticos para los tipos lineales.

Mira, tampoco son tantos bytes si hago la extensión de forma universal: siempre tengo 8 frames por tipo de enemigo.

Right { Walk1, Walk2, Attacking, Dying }, Left { ... }.

Walk1 y Walk2 son obligatorias. Attacking = 0 en lineales puros. Dying = 0 si no activas `ENEMS_DYING_FRAME`.

Esto lo tengo que madurar porque cada tipo de enemigo tiene sus historias con los frames y tal. Mejor me encargo antes de dejar el manejador de enemigos más organizado y luego me pongo a trabajarme de verdad los sets de sprites para unificarlo todo.

~~

Pero, por ahora, estos son los requerimientos de frames de todos los tipos:

- Fanty: 4 frames R1 R2 L1 L2
- Pezon: 2 frames C A
- Saw: 2 frames animación
- Monococo, hay modo full y no full. Ful: R1 R2 L1 L2 RA RH LA LH, no full solo R1 R2 L1 L2

La idea sería dejarlo todo unificado en 8, podría ser así:

Right (anim1, anim2, attack, dying) Left (anim1, anim2, attack, dying), Extra frames (custom).

Así, dying siempre es `facing + 3`, y solo se muestra si está definido `ENEMS_DYING_FRAME`.  En el caso de los monococos, los frames extra para el tipo B son a partir de +8.

¿Qué hago antes, esto o lo otro? XD Lo tengo que hacer sobre `tester_sideview` primero, que es donde hay más tipos de enemigos. Luego lo traslado a `tester_punchy` para comprobar.

~~

Para que funcione bien el frame hostiado, necesito guardar el facing. Ar.

~~

Todo funcionando.

Vamos a hacer un esqueleto de lo que tendría que ser el módulo de enemigos (la estructura). Además, sacaré a variables normales los arrays principales y tal, y luego miraré cuanto ahorro. Así que antes de empezar vendría bien capturar de nessc.exe

Para todos los enemigos
	- Elegir uno fuera de orden.
	- en_spr = 0xff;
	- Gestión de "tocado". Esto está bien.
	- Si en_t y no está tocado,
		- Cálculo de pregotten y `en_fr` común. **(revisar `en_fr` redundantes)**
		- Mover según en_t.
		- `en_spr_id [gpit] = en_spr;`.
		- Detectar plataformas móviles.
		- Si el enemigo no es colisionable, salir
		- Colisionar con el player / Pisar enemigo
		- Si el enemigo no es matable, salir
		- Golpear enemigo
		- Balear enemigo

	- Si en_spr != 0xff, pintar frame en_spr.

Esto asume que si no es colisionable, no es matable, lo cual es cierto.

Antes del remodelado, tho', voy a hacer lo de extraer las variables, porque así el código es más claro y me equivocaré menos.

~~

Funcionando en `tester_punchy`, que tiene pocos tipos de enemigos. Aquí el ahorro es de **256 bytes**. 

Funcionando en `tester_top_down`, con chicha diferente. Aquí el ahorro es de **1088 bytes**. Joé con los tipo 7.

Funcionando en `tester_sideview`, con un montón de enemigos. Aquí el ahorro es de **2176 bytes**. Cágate lorito.

OJAL: 
[X] Hay algo peío con los tiles destructibles que da la cara en `tester_top_down`.
[X] Homing fanties don't seem to work!

~~

Next

- Añadir sonidos.
- Pasar los cambios a Sir Ababol (AY!).
- Poner que los puñetazos solo sean activos durante sus primeros 3 o 4 frames, que ahora queda raro cuando le das al puño y el muñeco se colisiona con tu puño ya echao por su movimiento.
- Examinar los contenedores y ver como funcionan con el script. Pero esto necesita otro tester.
- Probar los propellers.

Veo que los containers están muy integrados con las animaciones custom de cave. Tengo que hacerlos más sencillos y generales, simplemente contenedores de objetos que intercambien el objeto que llevas, operaciones entre flags, vaya. O mejor me los fumo. O no sé, vienen muy bien para el script...

Los terminaré haciendo.

20180209
========

Los propellers
--------------

Los propellers están hechos de puta madre en MK2 y en Yun había una versión que los colocaba en hotspots, porque fueron un añadido a posteriori para poder meter el juego de Limite.

Originalmente los introduje en 2015 en MK1 para la fase de la fábrica de Cheril Perils que Anjuel iba a hacer y que se quedó en tres pantallas (una lástima, qué gráficos más bonitos).

Voy a descifrar como funcionaban - el código hay que limpiarlo de todos modos.

De forma old (ortopédica lol dioni), se mantienen unos arrays de propellers. Esto es porque hay cierta animación incorporada. Esta se está haciendo con sprites - creo que en MK2 se hace con tiles pero claro, en MK2 es mucho más barato rellenar la estructura de actualización porque es neslib moderno.

Cada vez que se añade un propeller es de forma meramente incremental y se usa un `prp_idx` que controla cuantos hay y sirve para limitar el bucle de las animaciones.

Cuando se crea, examina el buffer de atributos desde la casilla donde se define hacia arriba y almacena un rectángulo con x, x+16, y1 e y2. O sea, que la detección es aparte del sistema de atributos.

HUM. En MK2 se pintan los tiles de bit propeller y la propulsión se hace directamente detectando si estamos allí. Es más eficiente pero yo ya no tengo bits. Y creo que esto puede funcionar. 

Vale, lo voy a apañar mejor y lo hago así, total. Es MK1.

La cosa es que esto parece que (aparte del archivo en la carpeta engine) está totalmente vanished. Voy a mirar en los fuentes viejos de Cheril Perils a ver cómo se integraba, aunque me imagino cómo.

En config.h tenía esto:

```c
	#define ENABLE_PROPELLERS
	#define PROPELLER_AY			16	
	#define PROPELLER_VY_MAX		256
	#define PROPELLER_TILE			32
```

En player.h, dentro de la parte donde hay gravedad, y justo antes de la gravedad, esto:

```c
	#ifdef ENABLE_PROPELLERS
		if (ppropelled) {
			pvy -= PROPELLER_AY;
			if (pvy < -PROPELLER_VY_MAX) pvy = -PROPELLER_VY_MAX;
		} else
	#endif
```

O sea, que cancela la gravedad.

Y en printer.h, durante el render de la pantalla, la detección:

```c
	#ifdef ENABLE_PROPELLERS
		if (rdt == PROPELLER_TILE) {
			add_propeller (rdx, rdy);
		} else
	#endif
```

Joder, es justo como yo pensaba. Si es que me conozco como si me hubiera parío.

~~

Espera, mano, que en vez de gastar espacio en RAM y tener que precalcularlo todo, puedo meterlo en ROM junto con la info del mapa, no?

```c
	unsigned char propellers_scr_XX [] = {
		x1, y1, y2, ...
		0x0ff
	};

	...

	unsigned char propellers_map_0 [] = {
		0, 0, propellers_scr_02, 0, 0, ...
	}
```

Eso lo detecto al convertir el mapa y listo, menos ram, menos código, menos ejecución. 

Ya estoy reimplementando cosas otra vez. Me cago en mi estampa. Pero ¿y lo bien que me lo paso?

~~

¿Y como especificar el tile en el conversor? Tendré que modificar los demás .bat si no quiero romper compatibilidad. Pero eso es lo de menos.

~~

Después de estar toda la tarde jugando con el churum a que es "la bestia bestial" me he dado cuenta de la gilipollez que es pegarse un curro para mejorar una concepción ya de por si poco eficiente, así que lo que voy a hacer es implementar los propellers (y los springs) de MK2 y a tomar por culario.

Pero ahora ya no.

20180210
========

Al igual que hicimos con las escaleras, tenemos el beh == 64 que significa "flotar", obviamente no es obstáculo.

Lo rellenamos al vuelo al detectar un tile `PROPELLER_TILE` en el mapa.

[X] Sacar defines automáticos fuera de config.h y ponerlos aparte para poder sobrescribirlos con las actualizaciones.

[ ] Mover config.h a /my

[ ] Pasar a ASM las asignaciones arrays->variables y variables->arrays en el manejador de enemigos

[ ] Springs

[ ] Containers

20180212
========

[X] meter `PLAYER_STEPS_ON_ENEMIES` para declarar `enems_kill`. Y luego meter un `ENEMIES_MAY_DIE` controlable via autodefines.

[/] Estudiar incluir los enemigos programados de pantanow engine (en especial como integrar la animación).

[X] Portar RLE53.
 
[X] Encontrar una forma de poder poner el marcador arriba.
    Idea: 3 presets: game area arriba, game area abajo, game area centrada. <- esto.
    
20180213
========
[X] Hacer funcionar shines.

[X] Hacer funcionar resonadores.

[X] Hacer funcionar NO!.

[X] Hacer "animación de activar hotspot"

**Tengo que actualiar todos los ejemplos a partir de src**

```
	Overwrite 
		/mainloop.h
		/mainloop/
		/engine/
		/ram/

	Compare
		/game.c
		/config.h
		/assets/levelset.h
```

20180214
========

Simple Warpers
--------------

Se colocan con el ponedor. Tienen `T = 0xff`. El warper se pintará con un gráfico especial `WARPER_BASE_SPRID` en la posición inicial `(x1, y1)`. Cuando se toque (y se pulse, opcionalmente, `b_button`), llevará a la pantalla contenida en `attr`, en las coordenadas `(x2, y2)`. 

En el colocador, se coloca el warper donde se quiere que salga, se le da `T = 0xff`. Para finalizar, en `Attr` se coloca la pantalla destino y en `s1` las coordenadas destino en formato `YX`.

A la hora de gestionarlos, únicamente se pintan con el sprite que sea y se gestiona su colisión de forma independiente.

En config:

```c
	#define ENABLE_SIMPLE_WARPERS
	#define SIMPLE_WARPERS_BASE_SPRID
	#define SIMPLE_WARPERS_FIRE_BUTTON
```

En la inicialización, simplemente copiamos el valor de 'rda' en algún sitio (por ejemplo, mx):

```c
	#ifdef ENABLE_SIMPLE_WARPERS
		case 0xff:
			en_mx [gpit] = rda;
			break;
	#endif
```

Al actualizar, x e y no habrán cambiado, y nos vale con esto:

```c
	#ifdef ENABLE_SIMPLE_WARPERS
		case 0xff:
			en_spr = SIMPLE_WARPERS_BASE_SPRID;
			break;
	#endif
```

Justo antes de las plataformas, por ejemplo, hacemos la detección. Así podemos romper el loop de los enemigos y salir inmediatamente con un  break...

```c
	#ifdef ENABLE_SIMPLE_WARPERS
		if (
			_en_t == 0xff && 
			collide (prx, pry, _en_x, _en_y)
			#ifdef SIMPLE_WARPERS_FIRE_BUTTON
				&& (pad_this_frame & PAD_B)
			#endif
		) {
			n_pant = _en_mx; on_pant = 0xff;
			prx = (_en_y2 << 4); px = prx << FIXBITS;
			pry = (_en_y2 & 0xf0); py = pry << FIXBITS;
			break;
		}
	#endif
```

Esto debería funcionar tal cual.

Por supuesto, puedes definir `SIMPLE_WARPERS_BASE_SPRID` con una expresión que genere animación. Asín es esto.

Divagar sobre los enemigos programados
--------------------------------------

Estos son los enemigos que salen en Commandow que tienen programación. Recordemos como funcionan.

Tenemos pencompiler.exe, que compila scripts que tienen esta pinta:

```
	# Place this enemy behind the middle tile of a 3 characters barrier
	# Here:
	#   o
	#  XXX

	:BACK
	IDLE 6
	RIGHT 2
	IDLE 1
	FIRE PLAYER
	IDLE 1
	FIRE PLAYER
	LEFT 4
	IDLE 1
	FIRE PLAYER
	IDLE 1
	FIRE PLAYER
	RIGHT 2
	RETURN BACK
```

Y que resultan en arrays de datos:

```c
	// Place behind a three tiles wide barrier
	const unsigned char enbeh8 [] = {
		0x0e, 0x62, 0x09, 0x82, 0x09, 0x82, 0x44, 0x09, 
		0x82, 0x09, 0x82, 0x62, 0xcc, 0x00
	};
```

Supongo que los enemigos están consumiendo el script poco a poco según van cumpliendo las ordenes. 

Originalmente los personajes se dejaban a su libre albedrío cuando acabase el script, pero aquí no podemos hacer eso, así que habrá que exigir que los scripts terminen con un RETURN a una etiqueta anterior.

Pencompiler admite los comandos:

```
	:LABEL, 
	LEFT, DOWN_LEFT, DOWN, DOWN_RIGHT, RIGHT, UP_RIGHT, UP, UP_LEFT,
	IDLE
	FIRE [PLAYER],
	RETURN
```

Las direcciones llevan el número de celdas que avanzar. Idle lleva un número de mediosegundos que esperar. FIRE dispara en la dirección en la que se mira o al PLAYER, y RETURN está seguido de una etiqueta a la que volver.

Para hacer funcionar esto, cada enemigo debe tener asociado un puntero que apunta a la siguiente instrucción.

Cuando el enemigo está "libre", lee la siguiente acción e incrementa el puntero. Según lo que lee rellena su estado y un contador, que servirá para contar pixels o tiempo. Para avanzar, programa mx y my con los valores correctos (con LUTs `endx` y `endy`). Para disparar simplemente crea un coco. Para simplificar podemos poner que siempre dispare al player. RETURN simplemente cambia el puntero restándole el desplazamiento (atención que siempre se salta *hacia atrás*, de ahí lo de RETURN).

Todo lo de arriba integra muy bien en el motor de Commandow porque los enemigos ya tenían estados para IDLE, WALK, CHOOSING... y tal, pero yo lo puedo integrar igualmente en MK1_NES.

~~

Voy a empezar una implementación tentativa que luego ya revisaré si eso.

20180215
========

Algo sencillo para empezar: "NO!" encima del personaje. Modificamos:

- config.h 
- mainloop.h

Documentación en el README.md de Cheril Perils ROM2 Revamp.

He corregido un bug, actualizar tambien enengine.h.

La otra cosa es la "animación para activar hotspots", en otras palabras, es hacer estándar la animación de Goddess / Cave / etc en la que se interactúa con un container (todavía no) o con un hotspot pulsando FIRE y al final de una animación.

Habría que modificar la interacción con los hotspots en mainloop.h y además introducir el ciclo de animación en player.h.

Lo suyo es que:

- Se detecta la colisión con el hotspot y se pulsa FIRE. Ojal, que esto mola porque si uso `b_button` ya desactivo un posible disparo en `player_move` que se llama después. Esto debería poner al jugador en modo animación.
- Cuando se termina la animación y se muestra el último frame durante X tiempo, es cuando el hotspot debería funcionar!

¿Cómo hago esto?

La forma más fácil es así: 

- La animación se controla con use_ct, un contador.
- La colisión con el hotspot que hay ahora para interactuar con él se refuerza con una guarda: no funciona el hotspot a menos que use_ct tenga el valor que coincide con el último frame de animación (o un valor del "config").
- Al detectar colisión con el hotspot con `use_ct == 0` y `b_button` se inicia la animación.

lo suyo es lograr hacer todo esto configurable. Si uso un `use_sub_ct` como contador de frames antes del cambio de cada cell, guay. Si hago que el último frame sea configurable, guay. Y si hago que podamos configurar qué frame será el que dispare la interacción con el hotspot, más guay aún.

El metasprite que se dibujará se calculará sumando `use_ct` a la constante que sea, eso ya es tarea del programador.

La animación será lineal y por diseño el último frame se quedará 1 segundo.

Mi animación es (el primer cell es con use_ct = 1, cuidado con esto!): 

```
	1 2 3 4 5 6 7 8 9 A B C D <- use_ct
	0 1 2 3 4 5 5 4 3 2 1 0 6 <- frame
```

En mi caso la interacción es cuando `(use_ct == 7 && use_sub_ct == USE_ANIM_FRAMES)`. El número de frames es 0xD, de forma que al incrementar use_ct, cuando valga este valor, `use_sub_ct = 50`, y si al incrementarlo es mayor que ese valor, `use_ct = 0`. 

Además, `use_ct` debe bloquear los controles, y la detección debe hacerse solo si ppossee (en el caso de vista lateral).

~~

Toco:

- autodefs.h
- config.h
- mainloop.h
- ram/bss.h
- ram/zp.h
- mainloop/hotspots.h
- mainloop/flickscreen.h
- mainloop/resonators.h
- engine/player.h
- engine/mapmods/map_detectors.h

Otro ajuste, así que también:

- engine/enengine.h

Tengo que probar esto con los `SIMPLE_OBJECTS`. Debería funcionar pero mejor asegurarse. Se me ocurre que lo primero que tengamos que hacer sea encender los propellers, y para eso haya que llevar la llave de encendido a la máquina. Eso mola. 

Cuando tenga esto hecho y probado, ya propago los cambios y sigo con lo siguiente.

También he cambiado las constantes `ENEMIES_MAY_DIE` por `ENEMS_MAY_DIE` y `PLAYER_KILLS` por `PLAYER_STEPS_ON_ENEMS`, que son más claritas.

20180216
========

Aún tengo que actualizar todos los cambios - pero antes tengo que ver porque creo que el caso de "tengo algo y lo quiero dejar en un sitio vacío" no parece funcionar. También debería probar el "tengo algo y lo quiero poner en un sitio donde hay algo - ¡se intercambia!", aunque eso irá *en este mismo juego*, así que puedo seguir y ya.

Porque la fábrica será la fase 2. Para la fase 1 expandiré el nivel de Goddess de Spectrum, que no se ha usado nunca, y pondré los tres templos para abrir la parte donde están los últimos enemigos. Los templos tendrán cada uno un objeto mal puesto y habrá que reubicarlos. Pero tengo que pensar en un puzzle.

¿Luna->Roca, Estrella->Luz, Mar->Pez? Nah. Es mierda. Ya se me ocurrirá.

Ahora quiero meter algo que necesitaré: un `my/after_scr_draw.h` donde hacer modificaciones sobre la pantalla recién dibujada perow...

Hum. 

Esto se puede hacer ya en el tipo complejo dentro de `map_renderer_customization.h`.

No sé muy bien qué hacer. Lo podría poner como requerimiento. Si quieres modificar la pantalla recién dibujada de forma manual como resultado de alguna acción, deberías usar el `map_renderer_customization.h`.

También estaría bien probar / documentar que si en `on_object_got.h` u `on_object_used.h` necesitas recargar la pantalla no hay más que hacer `on_pant = 0xff`.

Así que nada, no voy a hacer nada. Bueno, sigo con el todo, lo recopio y amplío:

[ ] Mover config.h a /my

[X] Pasar a ASM las asignaciones arrays->variables y variables->arrays en el manejador de enemigos

[ ] Springs

[ ] Containers

[/] Estudiar incluir los enemigos programados de pantanow engine (en especial como integrar la animación).

[/] Simple warpers.

[X] Revisar el funcionamiento de los hotspots en modo easy objects en el caso de "tengo fullería" y "hotspot vacío" -> "tengo nada" y "hotspot fullería".

[X] Hacer que el contenido del array con los tiles del cuadro de texto sean offsets a un valor inicial #define en vez de ser valores fijos 0x20 a 0x28.

[X] Quitar parámetros de las funciones de `printer.h`

~~

20180218
========

Voy apuntando sonidos que necesitaría. Además  habría que mover a `#define SFX_*` los que hay.

- Salto nuevo.
- !floaty -> floaty (ffff).
- pisar enemigo
- activar resonador
- cuenta atrás.
- golpear con hitter.
- golpear breakable.
- breakable se rompe (al pisar o disparar).
- enemigo lanza coco.
- 

20180219
========

Creo que va siendo hora de que propague los cambios a /dev/src y los otros motores. Debería ser fácil porque con mi infraestructura sólo tengo que, en la mayoría de los casos, copiar carpetas enteras. Pero remember que tenemos archiva nueva en la oficina, que se llama Farala y es divina.

~~

Bueno, todo propagado. Voy a ver si puedo avanzar un poco en el "todo" dentro del proyecto de Cheril y tal.

Tocando:

```
	config.h
	engine/textbox.h
	engine/enengine.h
	engine/printer.h
	engine/enemmods/enem_linear.h
```

~~

Textbox ajustado.

Moviendo ahora a asm las asingaciones arrays->ZP y ZP->arrays del manejador de enemigos. Ciclos sé que ahorro, voy a ver cuantos bytes. Tampoco va a ser mucho, pero ahorrar una instrucción por asignación parace guay ¿no?

Antes:   `140*64+49 = 9009` bytes left
Despues: ha liberado `14+64+6 = 84` bytes. Joer, ¿srsly?

Voy a probar suerte ahora con el persistent_update, por ejemplo... Va a ser que no, hay una paranoia de hacer y deshacer una cuenta que sinceramente no entiendo ¿no se podría hacer mejor? Creo que es para ahorrar algo.

Es por el tema del 0.5, que lo tengo implementado de una forma muy de primero. Si es 0.5, `en_status = 1` y las velocidades son 1.  ¿De verdad esta es la mejor forma? Seguro que no. Creo que en pongpong tenía una forma mejor, voy a mirarla un momento.

Nah, es muy parecido, pero permitía codificar enemigos de velocidades de medio, cuarto, octavo de pixel por frame.

A ver, si levanto el bit 4, tendría el valor 17: 00010001. (el bit 7 es el que lleva el signo, por eso me voy a otro que "no moleste"). Ahora tengo:

```c
	if (!en_status [gpit] || half_life) { }
```

Podría poner algo así como:

```c
	if (!(_en_mx & 0x10) || half_life) {
		_en_x += (_en_mx & 0xef);
	}
```

Y eso luego me ahorra un montón de manejes ¿no? Puedo almacenar y recuparar siempre el valor de `_en_mx` directamente. ¡Voy a probar!

AAAh - amigo, tengo problemos porque cuando se va a negativo es complemento a 2 y se me jode el bit. Estamos complicando las cosas por uno sitio para descomplicarlas en otro sitio. Eso no mola.

Bah, lo dejo como estaba.

~~

Pero voy a intentar simplificarlo de otra manera, a ver si sale ...

Sí sale. Ahora es todo menos cumbersome, y no se está continuamente haciendo y deshaciendo cálculos, al menos para los persistentes. Esto significa que puedo cambiar el almacenamiento de los enemigos para la persistencia a ensamblador y ahorrar unos ciclos. 

El código origina que voy a cambiar es este:

```s
;
; ep_x [ep_it] = en_x [gpit];
;
L268C:	lda     #<(_ep_x)
	clc
	adc     _ep_it
	sta     ptr1
	lda     #>(_ep_x)
	adc     _ep_it+1
	sta     ptr1+1
	ldy     _gpit
	lda     _en_x,y
	ldy     #$00
	sta     (ptr1),y
;
; ep_y [ep_it] = en_y [gpit];
;
	lda     #<(_ep_y)
	clc
	adc     _ep_it
	sta     ptr1
	lda     #>(_ep_y)
	adc     _ep_it+1
	sta     ptr1+1
	ldy     _gpit
	lda     _en_y,y
	ldy     #$00
	sta     (ptr1),y
;
; ep_mx [ep_it] = en_mx [gpit];// << (1 - en_status [gpit]);
;
	lda     #<(_ep_mx)
	clc
	adc     _ep_it
	tay
	lda     #>(_ep_mx)
	adc     _ep_it+1
	tax
	tya
	jsr     pushax
	lda     #<(_en_mx)
	ldx     #>(_en_mx)
	clc
	adc     _gpit
	bcc     L1FA8
	inx
L1FA8:	ldy     #$00
	jsr     ldaidx
	jsr     staspidx
;
; ep_my [ep_it] = en_my [gpit];// << (1 - en_status [gpit]); 
;
	lda     #<(_ep_my)
	clc
	adc     _ep_it
	tay
	lda     #>(_ep_my)
	adc     _ep_it+1
	tax
	tya
	jsr     pushax
	lda     #<(_en_my)
	ldx     #>(_en_my)
	clc
	adc     _gpit
	bcc     L1FAD
	inx
L1FAD:	ldy     #$00
	jsr     ldaidx
	jsr     staspidx
```

Sinceramente, no tengo ni idea de por qué la lía tan parda cuando tiene que hacer el cambio de signo. O por qué hace todas esas sumas. ¡Si los arrays son todos `unsigned char`!

Aaah, ya sé. Es que ep_it es de 16 bits. Esto seguro que es por el tema de que con muchas pantallas me voy al carajo. Seguro que fue un cambio de por ahí en medio para un mapa grande, llámase el de Goddess...

Veamos, si ep_it fuera de 8 bits, tendríamos que podría señalar 256/3 = 85 pantallas. ¿De verdad que no es suficiente? Hay algo que se me escapa. Voy a tirar de hemeroteca a ver... ¡Es que parece que siempre ha sido unsigned int!

Hay algo que se me escapa. ¿De verdad no tenía bastante con 85 pantallas?

¡No sé qué hacewr! Creo que 85 pantallas por nivel es más que suficiente.

Voy a hacerlo unsigned char, qué hostias. O voy a mirar por si hay suerte y escribí algo en el diario de hace tres años... No hay nada. Pues lo pongo unsigned char a ver.

Según miro, `ep_it` valdrá como máximo `3 * MAP_SIZE`. O sea, con `MAP_SIZE` <= 85, nos vale unsigned char.

De hecho me fumo `ep_it` y lo cambio por `gpj`. Leo en el diario que antes había un conato de persistencia mierder cuando los enemigos llevaban el "formato antiguo", que no es más que el que tenían en Spectrum los juegos de Churrera (diseñado para RAM, con las cosas precalculadas). En ese caso sí que hacía falta un `ep_it` tocho, ya que creo que había hasta 8 bytes por enemigo y sabe dios como usaba el tema.

~~

Cambiado, y la actualización pasada a ASM. Con estos últimos cambios he ganado 150 bytes más. Joder, esto es un no parar. Antes de irme, sigo con cosas fáciles. Voy a ver alguna inicialización que sea copia lineal de memoria o fill de memoria y lo cambio por llamadas a neslib.

~~

Estoy viendo que si desintercalara algunos formatos de datos todo sería más fácil para mí. Enemigos, hotspots... todo. Pero bueno, ponerse a hacer esos cambios de formato a estas alturas es complicado. Dejémoslo estar.

Lo que sí podría hacer es tener unas variables en zp `_x, _y, _n` y fumarme un montón de parámetros de las funciones de `printer.h`. Lo meto en todo.

~~ 

Iremos poco a poco. La marca de espacio libre para empezar es de `146*64+42 = 9386` bytes en la ROM de Cherils. Voy a desparametrizar `upd_attr_table`, por ejemplo.

Sólo con esa, que son DOS llamadas, he ahorrado más de 50 bytes. Haré las demás. Sólo puede ser WIN. Pero ahora no time.

20170220
========

Voy a intentar deshacer más llamadas con parámetros. Las siguientes tienen cascada por lo que aún ahorraré más, pero hay que ir con cuidado.

Tocando:

```
	config.h
	mainloop.h
	engine/textbox.h
	engine/breakable.h
	engine/enengine.h
	engine/frame.h
	engine/printer.h
	engine/textbox.h
	engine/propellers.h
	engine/enemmods/enem_linear.h
	engine/playermods/process_tile.h
	engine/mapmods/*
```

`draw_tile` lo primero que hace es llamar a upd_attr_table, por lo que puedo traspasar `_x, _y, _t` directamente. Probando -> ok.

`update_list_tile`, lo mismo. Además, sólo se usa desde `map_set` además de desde las rutinas de text_box, así que creo que voy a cambiar las dos a la vez. 

`map_set` Se llama en breakable, chac_chac, empujar un tile, abrir un cerrojo. Es difícil de probar en este juego, mierda. Pero puedo hacer una actualización parcial en `tester_sideview` cuando lo tenga para ver.

Probados. Estos cambios, por ahora, llevan 320 bytes menos en `tester_sideview`.

Más cosas: `ul_putc` no es destructivo y se usa muchas veces. Probado y guay todo.

`p_t` ya es bastante mierder porque emplea división y resto. Esto es algo que debería intentar solucionar más adelante (obtener la división entre 10 y el resto con una rutina específica en asm) y ahorrar tener que meter mierda del runtime para esto. Por ahora me voy a fumar el paso de parámetros. 

Va quedando poco. Ahora toca `pr_str`, que se usa sobre todo en pantallas fijas (tendré que tocarlo a mano en `tester_top_down`). El string que recibe sí lo dejo como parámetro. Si no, será cumbersome de utilizar y prefiero comodidad aquí.

Ahora tengo que revisar `msc3nes.exe` para el código generado. Coolio.

~~

Veamos las ganancias totales (además de la velocidad):

```
- Cheril:            9386 a  9595 = 209 bytes.
- Tester sideview:  13032 a 13382 = 350 bytes!
- Tester topdown:   Se me ha olvidao mirar antes XD
```

Está muy bien, en el caso peor (sideview incluye muchas cosas) ganar más de 1/3K nos da para muchas otras cosas, o incluso para lo ganado por lo servido (frente a la versión anterior, meter estas cosas sale "gratis"... ¡ah, la relatividad!).

Y ahora es cuando debería empezar a hacer cosas más importantes como completar lo que falta o acabar los ejemplos.

[ ] Mover config.h a /my <- esto lo dejo para el final, cuando deje de añadir cosas.

[ ] Springs

[ ] Containers

[/] Estudiar incluir los enemigos programados de pantanow engine (en especial como integrar la animación).

[X] Simple warpers.

Voy a probar el warper rápidamente y luego me pongo a integrar bien los enemigos programados.

Tocando:

```
	mainloop.h
	engine/enengine.h
	engine/player.h
	ram/bss.h
```

Se me olvidaba por completo que MK1/MK2 tenían un timer. Quizá sea el momento de integrarlo también :-S.

```c 
	#define ENABLE_TIMER
	#define TIMER_INITIAL
	#define TIMER_START_ON
	#define TIMER_REFILL		10
	#define TIMER_TIME_FLAG		0
	#define TIMER_ZERO_FLAG 	1
	#define TIMER_RESET_ON_ENTER
	#define HOTSPOT_TYPE_TIME	5
```

Deberían ser más sencillos que en la churrera:

- `timer` se establece a `TIMER_INITIAL` al empezar.
- `timer` se decrementa cada segundo si `timer_on`.
- `timer_on` vale 1 al principio si `TIMER_START_ON`.
- Cuando `timer == 0` se levanta `timer_zero`, y entonces ya haces tú lo que necesites en `my/extra_checks`.
- Si hay `HOTSPOT_TYPE_TIME`, se contempla este tipo de hotspots. Al coger un objeto time destos, se incrementa `timer` con `TIMER_REFILL`, o se establece a `TIMER_INITIAL` si `TIMER_REFILL` vale 0.

[ ] Mover config.h a /my <- esto lo dejo para el final, cuando deje de añadir cosas.

[ ] Springs

[ ] Containers

[ ] Terminar de integrar los enemigos programados de pantanow engine.

[X] Timers

```
	config.h
	mainloop.h
	mainloop/hotspots.h
	mainloop/timer.h
	engine/frame.h
	ram/bss.h
```

Piensa en steady shooters. No pienses mucho, cópialos de MK2.

~~

```
	config.h
	mainloop.h
	assets/precalcs.h
	engine/enems.h
	engine/cocos.h
	engine/enemmods/enem_steady_shooter.h
	ram/zp.h
```

[X] Probar steady shooters

[ ] Probar que no he roto los shooties

[X] `COCOS_COLLIDE`

20180221
========

Update:

COPY `autodefs.h mainloop.h engine/ ram/ assets/precalcs.h engine/enemmods/enem_steady_shooter.h `

MERGE `config.h`

~~

Quiero hacer el paso de arrays->vars también en el inicializador de enemigos. Para eso sólo necesito la copia vars->arrays del final, que tendría que encapsular para reaprovechar. Esa y otras optimizaciones parecidas (en las funciones auxiliares). Voy a hacerla sobre Cheril y luego propago. Esta vez quiero controlar bien la mejora, así que voy a calcular *ahora* lo que hay libre en cada ROM:

```
Tester punchy    15629 bytes -> 15974 bytes: +345 bytes!
Tester sideview  13338 bytes -> 13933 bytes: +595 bytes!
Tester top-down   7912 bytes ->  8142 bytes: +230 bytes!
Cheril ROM2       8274 bytes ->  8843 bytes: +569 bytes!
```

O_o

Vaya mejora ¿no? Y aún quedarían un par de miserias. Sólo con optimizaciones de este tipo, en tester sideview llevo ~3.5Kb menos, joder, que ahí cabe todo un nivel nuevo :-O

~~

Sigo teniendo que probar que no he roto los shooties pero ahora no puedor.

[ ] Mover config.h a /my

[X] Springs

[ ] Containers

[ ] Terminar de integrar los enemigos programados.

[ ] Probar que no he roto los shooties

Los springs consistene en un tile que se detecta al pasar sobre él y que saca unos pinchos. En MK2 se detectaba un rectángulo calculado sobre el spring, creo, pero yo lo voy a hacer más sencillo: sólo si estamos posados y pisamos el tile con el punto central del player.

```c
	#define ENABLE_SPRINGS
	#define SPRING_TILE 				10
	#define SPRING_SPIKE_TILE 			11
```

Se me acaba el espacio en los testers así que lo suyo es que lo monte en el tentativo tercer nivel de Cheril que incluirá warpers, steady shooters... y Springs.

Lo dejo implementado aunque no lo pueda probar aún.

~~

Containers
----------

En el modo más sencillo deberían ser dispositivos que permitan intercambiar el valor de un flag con el de otro. Qué mal explicado. Un Container es una representación gráfica de un flag. Se dibuja con el contenido de ese flag.

Esto no es moco de pavo porque se puede hacer de mil formas. Estoy seguro de que esto estaba hecho de una forma en Cave de MK1 que la implementación de MK2 que hice, precisamente, para el remake de Cave.

Voy a mirar el de MK2 primero a ver qué hace y como se integra.

- Hay una estructura que almacena, para cada container, qué flag representa y su ubicación en pantalla. MK2 usa dos variables x e y, pero creo que no es mucho impacto usar una sola (porque además la RAM se acaba rápido).

- Los containers existen para la pantalla actual y se crean en el ENTERING del script. En cada pantalla se reinicia el índice de los arrays y se van añadiendo de forma lineal desde el script.

- Una función `containers_draw` pinta todos los activos, y otra `containers_do` registra la colisión. En realidad todo lo que hace es ver si el punto central del personaje está dentro del área de 16x16 de cada container, y pone el resultado en `containers_get`. Esta función se llama en la parte de `player_move` que controla la pulsación del botón B y debería invalidar la pulsación. 

- Los containers no pueden representar el flag 0, ya que es el valor al que se inicializa `containers_get`. 

- Tras la llamada a `containers_do` con un `containers_get != 0`: 

1. Se invalida el botón B.
2. Se intercambian los valores de los flags `PLAYER_INV_FLAG` y el del contenedor.

Esto tiene su propia forma muy custom (en cave) de integrar con el player animation.

Para integrarlo en MK1 debería quizá localizar las cosas de otra forma, quizá dividir el manejo en dos .h, uno en engine/ para la creación y otro en mainloop/ para el pintado/interacción, y hacer la misma integración que con los hotspots.

Esto no lo voy a poder probar hasta que haga un tester con scripting. Lo puedo dejar planteado, eso sí, pero no me voy a poner a implementarlo aún. Quiero completar cosas, completar Cheril y completar Sir Ababol DX, que si no esto se me va a enquistar.

Una vez diseñados, los aparcamos.

Volviendo a los enemigos programados
------------------------------------

Necesitaré algo en assets con las programaciones de los enemigos e introducir el puntero a programa. Por lo demás ya están bastante planteados. Voy a revisitar para resolver el tema de los sprites, que seguramente limitaré a izquierda y derecha. O no.

Creo que debería simplificar y tirar por el camino de facing left/right aunque estemos en un juego top-down. Para ello tengo que recordar el último facing, lo cual es trivial porque lo tengo guardado, y sólo cambiarlo cuando la dirección contiene left o right.

Recordemos que necesitamos endx, endy en precalcs:

```c
	// Directions are
	// LEFT DOWNLEFT DOWN DOWNRIGHT RIGHT UPRIGHT UP UPLEFT
	// 0    1        2    3         4     5       6  7
	const signed char endx [] = {-1, -1, 0, 1, 1, 1, 0, -1};
	const signed char endy [] = {0, 1, 1, 1, 0, -1, -1, -1};
```

Así es sencillo. Al seleccionar, si `_en_mx < 0`, entonces `_en_facing = 4`; si `_en_mx > 0` entonces `_en_facing = 0`. En otro caso no se toca.

He visto que solo se mueve a un pixel por frame pero creo que esto, por ahora, me va a ir valiendo.

`*en_behptr` debería inicializarse en base al atributo, por ejemplo.

Puedo hacer una animación de idle para las posiciones 2 y 3 de cada orientación.

```
	compile.bat
	game.c
	autodefs.h
	config.h
	mainloop.h
	assets/compiled_enems.h
	assets/precalcs.h
	engine/enengine.h
	engine/enemmods/enem_compiled.h
	ram/zp.h
```

```c
	#define ENABLE_COMPILED_ENEMS
	#define COMPILED_ENEMS_SHOOT
```

Tengo que generalizar pencompiler.exe para que pille todos los comportamientos y genere `compiled_enems.h` automáticamente.

Puedo poner 

```
	BEGIN label
	END 
```

Por ejemplo. Voy a probar.

20180222
========

Sí que he roto los disparos en los shooties. Voy a arreglarlo y a propagar. Fixed! Y tengo que documentar los compiled.

¡Los saws están rotos! Seguramente al cambiar la inicialización los he cagao. Voy a ver. Fixed! Esto de tener testers para todo es la laite.

Con el cambio que hice a la colisión he cagado los sprites de 16x16. Tengo que ver esto. Voy a definir un `PLAYER_COLLSION_TOP_FG` para llevar esto mejor controlado!

El jugador colisionará desde `y - PLAYER_COLLISION_TOP_FG + 2`  hasta `y + 12`, verticalmente.

Cambio tocho. Tengo que cambiar en general, la colisión con los cocos, y en general revisar todos los sitios donde antes usara (mal) `TALL_COLLISION`, `SMALL_COLLISION` o `PLAYER_COLLISION_TOP` para sprites.

Luego debería unificar cambiarndo tambien "TALL_PLAYER".

```
config.h
engine/cocos.h
engine/enengine.h
engine/general.h
engine/player.h
engine/enemmods/enem_punchy.h
```

Hecho y propagado. Ahora todo es mucho más sencillo a nivel de código y configuración, aunque me parece que los nombres que les he puesto a los defines no son del todo intuitivos.

Creo que voy a cambiar "TOP" por "STRETCH". `PLAYER_COLLISION_VSTRETCH_*` se parece más a lo que es: una "ampliación vertical" de los rectángulos de colisión.

~~

Lo siguiente que quiero meter (aunque la lista siga sin estar vacía) es soporte básico para CNROM, que se resume en:

- Rutina de paginación (quizá esto lo tenga que meter para todas las ROMs porque va en neslib.s, aunque son unos bytes).
- Array en levelset que indica qué página de CHR-ROM lleva cada nivel.
- Pertinentes llamadas al iniciar cada nivel.

También puedo hacer que haya que incluir la rutina de paginación de forma específica al compilar crt0.s metiendo un -D CNROM. Voy a probar.

Vaya... Si ya estaba (aunque en modo que no tiene en cuenta los bus conflicts). Modificada es así:

```s
	_bankswitch:
		tax
		sta bankList, x
		rts

	bankList:
		.byte $00, $01, $02, $03
```

¿Qué son, 10 bytes? meh, la dejo por defecto!

```
	nes-CNROM.cfg
	crt0.s
	neslib.s
	mainloop.h
	assets/levelset.h
```

He roto Sir Ababol: por alguna razón al pasar de level1 a level0 se reinicia. Lo miraré. Pero:

[ ] Mover config.h a /my

[ ] Containers

:-D

Buenas noches.

20180223
========

Estoy metiendo los containers, de forma tentativa. Tendré que montar un scripting tester tonto para probarlos, pero bueno.

Por ahora nada de inventarios. Se tendrán que contentar con poder llevar solo un objeto. Más tarde meteremos algo de inventarios si me apetece.

Dejo apuntadas aquí algunas cosas porque luego tendré que modificar msc3nes.bas para que genere el código correcto.

ADD_CONTAINER x, y, f debería generar OP YX F, y el parser debería devolver `sc_x == YX` y `sc_y == F` (y así uso la función que lee dos bytes).

Todo hecho, voy a propagarlo aunque no lo pruebe ahora.

~~

Para el scripting, molaría tener una estructura de sprites fijos que activar por pantalla, para mostrar personajes sin tener que tirar de tileset. Quedará major, aunque sigan alineados a tile. Serán a nivel lógico de 16x16 aunque los podrás estirar para arriba lo que quieras usando los métodos normales (poner el origen Y más arriba, de cajón).

Estoy planteándome portar 1:1 el Cadaverion para probar timers y scripting, mejor que hacer un tester dedicado. Siempre puedo activar contenedores en una pantalla para probarlos rápidamente.

Voy a hacer esto porque la integración que he hecho del timer es una mierda, y necesito eventos `ON_TIMER_OFF` y cosas así.

20180225
========

Me he puestoa medio integrar Cadàveriön y me he dado cuenta de que faltan un montón de cosas que estoy medio montando.

Pero la que más coñazo va a ser (creo) va a ser la que lanza el fire script en la animación, al igual que se comprueban los containers o los hotspots en la animación.

El inicio de la animación ahora mismo no hace nada y se detecta por separado para hotspots y containers. Justo después debería lanzarse anyways si está el scripting y `FIRE_SCRIPT_WITH_ANIMATION`.

La llamada  a `run_fire_script` debería ser o bien al pulsar `b_button` o bien al llegar al frame correcto, según la combinación de `ENABLE_USE_ANIM`.

Ahora tengo al churum en plan destroyer, ya lo pienso más despacio luego.

20180227
========

Terminando Cadàveriön me doy cuenta de que msc3 (y msc4) tienen un montón de mierdas que están ahí por haber estado arrastrando esto tanto tiempo, sobre todo el tema de las comparaciones de flags con valores. Teniendo la construcción #flag, no es necesario que se generen opcodes específicos para IF FLAG n = 1 y IF FLAG n = FLAG m, por ejemplo. El compilador tiene un parchazo que cambia todo por IF FLAG n = X, con X un literal o un #flag.

Bue, por ahora me vale. Pero debería sanear eso también *un poco*.

Tengo que integrar además la animación con el tema de script, como dije hace unos días, pero antes voy a dejar el juego funcionando al 100%.

20180229
========

Tras trasladar los cambios para Cadàveriön al tester top-down (el remake de Sgt. Helmet's Training Day), he corregido algunas cosas en la generación de código de msc3nes (¡cómo mola tener tantos testers!) y ahora veo unos cuantos bugs que no sé de qué serán. Hay uno fácil y otro que parece difícil:

[X] No puedo disparar en la fila inferior de la pantalla (FASI).

[X] Tras colocar las bombas, en la pantalla de abajo ¡no se puede disparar! (DIFISI).

Veamos todo.

Lo fasi está ya; lo difísi parece que ocurre cuando el mensaje general "cambia". Este es el código del script que controla eso:

```
	ENTERING ANY
		IF FLAG 1 = 0
		THEN
			#TEXT BUSCA_5_BOMBAS_Y_EL_ORDENADOR!
			TEXT _SET_5_BOMBS_IN_EVIL_COMPUTER_
		END
		
		IF FLAG 1 = 1
		THEN
			#TEXT MISION_CUMPLIDA!_VUELVE_A_BASE
			TEXT BOMBS_ARE_SET!_RETURN_TO_BASE!
		END
	END
```

Tampoco tiene mucha historia, no :-?

Voy a ver dónde se detiene esto. Seguramente "algo" esté invalidando `b_button` y tengo que encontrar qué es. Exacto, `b_button == 0` en la pantalla 1 tras colocar las bombas... Eso justo antes de detectar para disparar; lo que se hace antes es ejecutar la detección de FIRE para lanzar scripts. Eso invalidaría `b_button` bajo algunas circunstancias:

```c
	if (b_button) {
		run_fire_script ();
		if (sc_continuar) b_button = 0;
	}
```

¿Qué fire script se ejecuta ahí? Vuelvo al script. Respuesta: NINGUNO.

Claro, al no ejecutarse nada, el `sc_continue` del ENTERING sigue a 1. Tengo que invalidar `sc_continue` SIEMPRE. Lo hago en el código generado por msc3nes, y así no tengo que tocar nada de engine.

Todo correcto! Propagamos.

~~

Me iba a meter con el sonido pero ¿cómo de viable será integrar con famitracker2? Voy a comparar los puntos de entrada.

crt0.h
------

Las constantes `FT_DPCM_OFF` y `FX_SFX_STREAMS` están presentes en ambos. Los defines `FT_DPCM_ENABLE` y `FT_SFX_ENABLE` también.

`FT_TEMP` parece estar definido; en FT1 son 7 bytes; en FT2 son 3.

`FT_BASE_ADDR` fijo a $100 en ambos.

`FT_DPCM_PTR` no se define para FT2.

De estos tres en FT2 sólo FT_THREAD parece usarse en FT1.

```s
	.define FT_THREAD       1	;undefine if you call sound effects in the same thread as sound update
	.define FT_PAL_SUPPORT	1   ;undefine to exclude PAL support
	.define FT_NTSC_SUPPORT	1   ;undefine to exclude NTSC support
```

Luego vienen las inicializaciones. Para FT1:

```s
		lda <NTSCMODE
		jsr FamiToneInit

		.if(FT_DPCM_ENABLE)
		ldx #<music_dpcm
		ldy #>music_dpcm
		jsr FamiToneSampleInit
		.endif

		.if(FT_SFX_ENABLE)
		ldx #<sounds_data
		ldy #>sounds_data
		jsr FamiToneSfxInit
		.endif
```

Para FT2:

```s
		ldx #<MUSIC_data
		ldy #>MUSIC_data
		lda <NTSC_MODE
		jsr FamiToneInit

		.if(FT_SFX_ENABLE)
		ldx #<sounds_data
		ldy #>sounds_data
		jsr FamiToneSfxInit
		.endif
```

La inicialización para los SFX es igual, pero en FT1 se pasa el resultado de la detección de NTSC y se inicializa el DPCM.

La carga de datos es también diferente; FT1:

```s 
	.segment "RODATA"

		.include "music.s"

		.if(FT_SFX_ENABLE)
	sounds_data:
		.include "sounds.s"
		.endif

	.segment "SAMPLES"

		;.incbin "music_dpcm.bin"
```

FT2:

```s
	.segment "RODATA"

	MUSIC_data:
		.include "music.s"

		.if(FT_SFX_ENABLE)
	sounds_data:
		.include "sounds.s"
		.endif
```

Los samples no aparecen por ningún lado en FT2 y se carga la música exportada tras una etiqueta. La adaptación en este archivo parece sencilla.

neslib.s
--------

El la NMI, para FT1 se llama a `FamiToneUpdate` justo al final:

```s 
	@skipNtsc:

		jsr FamiToneUpdate

		pla
		tay
		pla
		tax
		pla

	irq:
	    rti
```

En FT2 es exactamente igual:

```s
	@skipNtsc:

		jsr FamiToneUpdate

		pla
		tay
		pla
		tax
		pla

	irq:
	    rti
```

Las funciones que integran; FT1:

```s
	;void __fastcall__ music_play(const unsigned char *data);

	_music_play:
		stx <PTR
		tax
		ldy <PTR
		jmp FamiToneMusicStart



	;void __fastcall__ music_stop(void);

	_music_stop=FamiToneMusicStop



	;void __fastcall__ music_pause(unsigned char pause);

	_music_pause=FamiToneMusicPause



	;void __fastcall__ sfx_play(unsigned char sound,unsigned char channel);

	_sfx_play:
		and #$03
		tax
		lda @sfxPriority,x
		tax
		jsr popa
		jmp FamiToneSfxStart

	@sfxPriority:
		.byte FT_SFX_CH0,FT_SFX_CH1,FT_SFX_CH2,FT_SFX_CH3
```

Y FT2:

```s
	;void __fastcall__ music_play(unsigned char song);

	_music_play=FamiToneMusicPlay



	;void __fastcall__ music_stop(void);

	_music_stop=FamiToneMusicStop



	;void __fastcall__ music_pause(unsigned char pause);

	_music_pause=FamiToneMusicPause



	;void __fastcall__ sfx_play(unsigned char sound,unsigned char channel);

	_sfx_play:
		and #$03
		tax
		lda @sfxPriority,x
		tax
		jsr popa
		jmp FamiToneSfxPlay

	@sfxPriority:
		.byte FT_SFX_CH0,FT_SFX_CH1,FT_SFX_CH2,FT_SFX_CH3
```

`music_stop`, `music_pause` y `sfxplay` parece exactamente iguales. `music_play` es más sencilla, supongo que porque se maneja una biblioteca de canciones indexada por un byte.

Finalmente, al final se incluye `famitone.s` y `famitone2.s`. En principio parece sencillo ¿no? ¿Y si pruebo? Siempre puedo hacer stash... Voy a subir los cambios antes de empezar.

~~

¡¡Funciona!! Joder, sí que ha sido fácil. Ahora tengo que portar los cambios a los demás juegos, lo que implica convertir las músicas para unos cuantos.

~~

Veamos el TODO, ampliado como siempre :*)

[ ] Mover config.h a /my

[ ] Containers

[X] Sonidos

[/] Música OGT Cheril Perils y Cadàveriön

[·] Terminar Cheril Perils (fase extra, pantallas fijas).

[ ] Sir Ababol DX se me está quedando muy atrasado.

En realidad al final lo que más me queda es hacer juegos XD

~~

Lo dejo a mitad de montar los sonidos nuevos en Cadàveriön porque el tema de usar con fire script -> OK parece NO FUNCIONAR OTRA PUTA VEZ.

Nah, era que había diseñado muy mal toda la interacción pero al final guay.

20180302
========

Debería establecer una forma de añadir containers desde código, para los que no vayan a usar script. Debería montar un tester. Voy a pensar en qué personaje, Cheril ya me estoy colando.

Debería pasar los gráficos de Bosque y D'Veel'Ng al sprite nuevo de Cheril y tal. Porque yo lo valgo. Debería hacer algo en Bosque que lo haga radicalmente diferente de la versión MK2 de los multicarts.

Debería deber.

Voy a revisar (sin probar realmente, necesito tester) de nuevo a ojal los containers y comprobar que desde el scripting se genera código correcto para crearlos.

También voy a pensar en una forma de hacerlo desde código, aunque lo más sencillo es hacer una especie de estructura rollo locks:

```
	{ NP, YX, F, ... };
```

Al entrar en cada pantalla se recorrería y tal y cual. Tampoco serán listas muy largas, y si algún día lo son, ya me apañaré para acelerar.

No sé, macho, yo lo veo todo bien. Voy a hacer un `00_tester_containers` donde pueda probar toda esta mierda. Pero antes, propago de cadàveriön a src porque lo he hecho ahí, desordenado de mi.

20180303
========

Voy a rehacer el TODO, porque se me olvida de todo que quiero poner.

[ ] Mover config.h a /my

[/] Containers

[/] Música OGT Cheril Perils y Cadàveriön

[·] Terminar Cheril Perils (fase extra, pantallas fijas).

[ ] Sir Ababol DX se me está quedando muy atrasado.

[ ] Estructura de sprites extra que, al igual que los containers, pueda poblarse por pantalla desde script o estructura de memoria, para poner personajes por ahí o simplemente decoraciones.

~~

Veamos, voy a hablar un poco sobre configuración.

- Por un lado tendremos `ENABLE_EASY_OBJECTS`, donde los hotspots pueden contener objetos y se define pinv como el "inventario" del personaje.

- Por otro lado tenemos los containers, que usan las flags, y `flags [FLAG_INVENTORY]` como el "inventario" de los personajes.

Podríamos definir `ENABLE_NORMAL_OBJECTS`... Hum.

```c
	#ifdef ENABLE_EASY_OBJECTS
	#define HOTSPOTS_DYNAMIC
	#define CARRY_ONE_HS_OBJECT
	#endif
```

Podríamos hacer

```c
	#ifdef ENABLE_NORMAL_OBJECTS
	#define ENABLE_CONTAINERS
	#define CONTAINERS_FROM_CODE
	#endif
```

Aunque no creo que sea necesario. Ahora no me puedo concentrar para decidir qué es lo mejor, porque tenemos tres opciones (de nuevo):

- Sistema "easy", con hotspots y pinv.
- Sistema "normal", con containers desde código y flags.
- Sistema "scripting", con containers desde script y flags.

En realidad los tipos "normal" y "scripting" son iguales salvo porque uno define `CONTAINERS_FROM_CODE` Y el otro `ACTIVATE_SCRIPTING`. 

Bueno, con documentarlo creo que es más que suficiente.

20180304
========

Con esto a medio montar, idea integradora:

- Containers y sprites no interactuables juntos.
- Diferencia: el "flag" que representan si AND 128 (containers), o el gráfico que pintar de spr_it si !(AND 128) (fixed sprites).
- Rellenar una variable especial que comprobar desde el script y lanzar el fire script _después_ del intercambio en el caso de los containers.

Cuando me pueda concentrar lo añado.

20180305
========

Vale, tengo que generalizar las funciones de containers.h - Aunque ahora quizá debiera buscarles otro nombre porque ya no son estrictamente containers.

- Si `f & 128` -> Es un container. Dibujar `spr_hs [flags [(f & 0x7f)]]`. 
- Si `!(f & 128)` -> Es un sprite. Dibujar `spr_hs [f]`.

`containers_add` no cambiará. Ya me ocuparé de escribir el valor correcto desde msc3nes. `containers_create` idem, tendré que documentar bien todo esto cuando acabe este tester.

La comunicación con el script es mediante una nueva variable script_arg que voy a crear ya mismo.

- Si `script_arg & 128` -> Se cogió el objeto que había en el container `script_arg & 0x7f`.
- Si `!script_arg & 128` -> Se interactuó con el sprite `script_arg`.

El manejador del script siempre pondrá script_arg a 0xff tras consumirla.

~~

Sobre el tema del script - va siendo hora de sanear, sobre todo el tema de los alias, los flags, los contenidos, etc.

Ahora mismo, N es un número, #N es "el contenido del flag N", $ALIAS equivale a N, y #$ALIAS es "el contenido del flag $ALIAS, o asea, del flag N".

Esto me parece engorroso.

Debería ser $ALIAS "el contenido del flag N" al que representa, directamente, dependiendo del contexto: siempre que aparezca como rvalue. No sé si me explico.

`SET $ALIAS = 3` => `SET FLAG N = 3`.
`SET $HOLA = $ADIOS` => `SET FLAG N = FLAG M`.

Yo debería tener un sólo set de "SET" e "IF" que comparasen A con B, siendo A y/o B:

- N
- FLAG N
- #N
- $ALIAS

"Todas" las versiones deberían generar el mismo bytecode. Esto limita el anterior pero lo hace recortando cosas que no usaba, y lo hace menos engorroso de usar. Así pues:

- N se traduce por Chr (N).
- FLAG N, #N o $ALIAS se traducen por Chr (128|N).

Así, por ejemplo, `IF ARG = $CONT_OFFER` se traducirá correctamente. Aunque aquí tampoco es intuitivo. Quizá necesite un nuevo operador ~ para este caso:

`IF ARG ~ $CONT_OFFER`, si ARG "es" el `$CONT_OFFER`.
`IF ARG = $CONT_OFFER`, si ARG vale el contenido de `$CONT_OFFER`.

Tengo que revisar todo el parser e ir apuntando por aquí mismo las cosas que voy viendo / simplificando.

Empezamos con el parser pval. $ALIAS no traducirá a N, sino a #N.

Qué hostias, creo que voy a hacer lo que llevaba mucho tiempo queriendo hacer: un msc nuevo. Llamémosle msc_mk1.bas. Esto va a ser un piponazo a estas alturas, pero me mola y siempre puedo cambiar a otro proyecto (terminar espinete) si me aburro.

20180306
========

¡Viva mi picha! Ya tengo el nuevo compilador de scripts terminado y más fácilmente ampliable / modificable. All hail the pichaw!

Cosas:

- Probar en multilevel.
- Añadir itemset y tal, eso para MK1 v1.1 XD
- Añadir macros

Pero next: 

- Propagar, también el compilador. Ver qué cambios hay que hacer en el script.
- Escribir el README de cómo está hecho, aunque sea un tester.

20180307
========

Tras portar todo al msc nuevo y probar y corregir cosas, voy a revisar que todo está debidamente actualizado y si eso ya por fin retomo el TODO.

[ ] Mover config.h a /my

[X] Probar "containers desde código"

[/] Música OGT Cheril Perils y Cadàveriön

[·] Terminar Cheril Perils (fase extra, pantallas fijas).

[ ] Sir Ababol DX se me está quedando muy atrasado.

~~ 

Estoy probando "containers desde código", y creo que será mejor cambiar el nombre "global" de containers + sprites por "interactives".

20180308
========

[ ] Mover config.h a /my

[/] Música OGT Cheril Perils y Cadàveriön

[·] Terminar Cheril Perils (fase extra, pantallas fijas).

[ ] Sir Ababol DX se me está quedando muy atrasado.

Voy a darle un achuchón a las fases de Cheril Perils a ver si les saco algo más de fran. A la fase de la fábrica le queda nada y sigo pensando qué hacer para la fase 3. Buscaré algo que tenga hecho y le cambiaré el tileset.

~~

[X] Al hacerse `player_process_tile` en dos puntos, al abrir un cerrojo el segundo falla (y pone un NO!). Tengo que revisar esto para que solo se haga una detección, al menos con los cerrojos.

[ ] Los shines aparecen donde no deben (¿No se reinician bien con la pantalla?)

[X] Añadir mapas RLE en CHR-ROM (sólo para los complex).

20180309
========

Más todo, neverending (buy easy):
 
[ ] Tipo 7 en lateral spawns walker-fallers que pueden hostiar.

[ ] Estado hit con recoil (y colisión!) para TODOS los enemigos.

[ ] Bufferless breakables. Breakables sin buffer cuando la vida de los breakables es 1.

20180312
========

Voy a integrar los mapas CNROM en Cheril ROM2. Será mi conejillo de indias para esto. Veamos, necesito:

- El conversor.
- Código.
- Tortilla.

¡Funcionandow! Voy a corregir lo de los shines. Hm - es raro, porque sí que se está reiniciando los tiestos. Pero aparecen malw.

Bueno, aparecían mal el otro día que probé empezando en otra pantalla. Desde el principio, al menos en las primeras pantallas, todo parece guachi. Hm. ¿Será que el máximo de shines será menor que el número de tiles que los necesitan y se jode todo? ¿Está controlado?

Está controlado. Qué raro.

Bueno, ahora tampoco me puedo poner a jugar y tal para reproducirlo, así que pasamos a lo próximo.

~~

El rollo de `player_process_tile`. Meh, fási.

~~

Bufferless breakables. Follarme el buffer si la vida de los breakables es 1. Pero esto es mejor hacerlo en otro tester. Y el hit recoil (en el tester punchy). Pues lo que me queda es propagar y ponerme en el otro tester. Este es el todo:

[ ] Mover config.h a /my

[/] Música OGT Cheril Perils y Cadàveriön

[·] Terminar Cheril Perils (fase extra, pantallas fijas).

[ ] Sir Ababol DX se me está quedando muy atrasado.

[ ] Tipo 7 en lateral spawns walker-fallers que pueden hostiar.

[X] Estado hit con recoil (y colisión!) para TODOS los enemigos.

[X] *Soft* breakables. Breakables sin buffer cuando la vida de los breakables es 1.

[ ] Los shines aparecen donde no deben (¿No se reinician bien con la pantalla?)

~~

También tengo que documentar el tema de los mapas en CHR_ROM.

~~

'soft breakables' listos. ¿Hit con recoil? Es retroceder N pasos (por ejemplo, durante el tiempo en el que está parpadeando / explotao). Tengo que ver cómo calcular la dirección.

```c
	#define ENEMS_RECOIL_ON_HIT
```

¡Hecho! Pero no me da tiempo de propagar. Total, da igual: tengo que implementar los tipo 7 en vista lateral.

20180313
========

Sin ponerme poder a probar Cheril para ver que los shines medio se comportan, tengo que pensar en el tipo 7 para side view. También tengo que recordar que al salir del estado recoil de los enemigos hay que actualizar las variables float, y que enemigos como pezón o monococo no deberían ser afectados.

Espera, hagámoslo mejor: también debería haber recoil con los disparos.

Puedo generalizar y hacer una función que calcule `en_rmx [gpit]` en base al tipo de enemigo y usando `rdx` como factor golpeante (posición del player o posición del bullet).

~~

Divaguemos un poco sobre el tipo 7 para side view, porque básicamente no tengo mucha idea de lo que quiero. Lo suyo sería un tipo de enemigo que tuviese spawns infinitos desde un punto para simular lo que ocurre en Spartan-X con los enemigos que van a por tí. O sea, un tipo 7 de toda la vida, pero habría que definir qué hacen los bichos lanzados.

Esto ya lo planteé antes, pero me dio mucha pereza porque lo suyo es que el muñeco que sale hiciera algo más que lanzarse contra tí, y luego ya surge el tema de integrarlo con lo que ya hay, lo cual multiplica la complejidad.

Lo que sí podría hacer es que los enemigos que matas pudiesen hacer respawn tras cierto tiempo, sin tener por qué salir de la pantalla. Podría indicar esto con algún tipo de atributo extra.

El atributo, que empleo sobre todo para almacenar la velocidad, tiene muchos bits libres teniendo en cuenta de que jamás pongo velocidades mayores que 4. Esto podría emplearse para indicar que ciertos enemigos pueden hacer respawn tras cierto tiempo.

Habría que hacer una animación de apareciendo, como las que tienen los tipo 7.

Imaginate que pillo el nibble más alto del atributo para indicar respawn. Si este nibble > 0, el muñeco hará respawn un número de frames proporcional al valor del nibble.

Esto podría molar, pero tengo que introducir un estado "apareciendo" en los enemigos que ahora no tienen. Por suerte, es parecido al estado de "hit", con lo que no voy a tener que modificar mucho código, más añadir que otra cosa.

Lo sigo macerando.

~~

Me está empezando a parecer una buena solución y todo, Hulio.

~~

¿Y otro bit para que los lineales siempre vayan adonde estás tú?

~~

El parámetro se necesita "verbatim" para steady shooters, pezones, chac-chacs, compiled enems y simple warpers.

~~

He metido este cambio a la hora de tratar el atributo. Debería propagar a los otros testers al menos antes de seguir.

~~

Creo que podría utilizar el propio `en_cttouched` porque no hace más que display y bloquear el movimiento. Sólo tengo que adaptarlo un poco, asegurarme de que no se hará recoil (con `en_rmx` a 0) y que si `ENEMS_FLICKER_ONLY_ON_DYING` esté activo igualmente parpadée porque está apareciendo.

El tema es que necesito algo para distinguir si estoy apareciendo o si estoy muriendo. Pensemos un rato.

Cuando lo encuentre, el primer paso es crear a todos los enemigos "apareciendo", sin tocar nada más, para probar.

~~

A tomar por culo, `en_spawning` nasió.

~~

spawn sería:

```c
	en_cttouched [gpit] = 50;
	en_spawning [gpit] = 1;
	en_life [gpit] = Lo que tengan al principio;
```

Hace algo raro y eso es porque me queda algo sin inicializar...

Fuck. Y hoy no hay quien se concentre.

Pero vaya pifostie se lía si lo activo desde el principio, con increíbles glitches a go gó. A mogollón.

Creo que ya sé por qué es: Está también procesando a los que tienen `en_t` = 0. Y no lo puedo arreglar fácil, porque ya pongo `en_t = 0` cuando mato a un enemigo.

Juer, qué mierda es todo XD Si quiero hacer respawn, no puedo poner `en_t` = 0 cuando mato a un enemigo. Necesito un array nuevo para estados donde meter todas estas cosas, `en_spawning` incluido.

~~

OK - Ya va.

Ahora que el estado de apareciendo es posible, tengo que pensar en cómo integrar todo el resto y cómo controlar el respawn.

Tengo contadores de sobra para hacer todo el tiesto. Sólo haré respawn para lineales y fantasmas PERO NO LO CONTROLARÉ EN EL CÓDIGO. Estas cosas son trabajo del programador que tiene que meter los valores correctamente.

Multiplicador: tendré valores de contador de 1 a 15. Multiplicador = 8 tendríamos 8 a 120 frames (hasta 2 segundos). Multiplicador = 16 tendríamos 16 a 240 (hasta 4 segundos). Creo que está bien. En este caso, sería 11 =~ 3 segundos, 7 = ~2 segundos, 4 = ~1 segundo

Necesitaré un `ENEMS_CAN_RESPAWN` para controlar esto.

~~

Lo he implementado pero va cutre... No sé, no me gusta. No queda limpico y culico como el resto de las cosas y hay poco control.

Para que el respawn sea completo y guay, tengo que almacenar los valores iniciales tal cual: x, y, mx, my. Eso son muchas cosas. A lo mejor los puedo apañar en dos bytes Y-X y MY-MX, con los nibbles del último [0-15] -> [-8..7]. Aunque es tontería por ahorrarme 6 bytes de RAM gastar un montón de código.

~~

Ahora parece que me gusta más.

Debería haber una opción para empezar muerto. Empezar muerto es con el flag puesto y `_en_ct` con el valor de `en_respawn`. Pero debería poder controlarla por enemigo. Y se me acaban los bits. Tengo uno ya pero a lo mejor querría usarlo de otra forma.

Tengo que reorganizar muchas cosas.

Todo lo complico hulio.

Propuesta:

- Tipo enemigo = 0-63.
- + Punch = |64.
- + Fire = |128.

Lineales = 1, 2, 3, 4 (1, 2, 3, 4)
Lineales punch = 65, 66, 67, 68 (41, 42, 43, 44)
Lineales fire = 129, 130, 131, 132 (81, 82, 83, 84)

Sí, me gusta más y funciona. Voy a actualizar docs.

[ ] Si te cargas trozos de la fase y hay un reenter al morir, puedes aparecer bloqueado. Buscar solución

[ ] En la animación de usar, si pulsas el salto "da saltitos".

Voy a ver si me da tiempo a propagar lo de los respawners y me voy.

20180315
========

Ayer estuvimos haciendo un regalo a Chema que ha quedado muy chulo, reimplementando una versión modificada de Trabajo Basura (Dire Job) con MK1/AGNES. Tengo que hacer postmortem de este porque he reinmplementado algunas cosas y he usado bastante extensivamente la inyección de código propio en los archivos de la carpeta `my/`.

Hoy poco tiempo tendré para hacer más, así que por lo pronto voy a repasar la lista de TO DO. Hay varios bugs, debería librarme de ellos primero.

[ ] Mover config.h a /my

[/] Música OGT Cheril Perils y Cadàveriön

[·] Terminar Cheril Perils (fase extra, pantallas fijas).

[ ] Sir Ababol DX se me está quedando muy atrasado.

[ ] Los shines aparecen donde no deben (¿No se reinician bien con la pantalla?)

[ ] Si te cargas trozos de la fase y hay un reenter al morir, puedes aparecer bloqueado. Buscar solución

[ ] En la animación de usar, si pulsas el salto "da saltitos".

Tengo unos minutos, voy a ver si reproduzco lo de los shines. - Ok, los shines están bien. Pero tenía definido MAX_PROPELLERS como 4 y en la pantalla donde lo detecté hay más. Voy a revisar el mapa para establecer bien un máximo mínimo (!).

Son 6.

¡Pues fuera de la lista!

También he apañado una comprobación extra para este juego en el que los enemigos no te matan si están parados y se espera que los pises: `pvy > 0` para detectar pisada. Lo amalgamo con la comprobación de que estén los resonadores activados.

[ ] Mover config.h a /my

[/] Música OGT Cheril Perils y Cadàveriön

[·] Terminar Cheril Perils (fase extra, pantallas fijas).

[ ] Sir Ababol DX se me está quedando muy atrasado.

[ ] Si te cargas trozos de la fase y hay un reenter al morir, puedes aparecer bloqueado. Buscar solución

[ ] En la animación de usar, si pulsas el salto "da saltitos".

20180317
========

Veamos lo de los salitos, que estoy en medio de un "la Casa de Mickey" y es la paz que tendré hoy ¡Resuelto!

[ ] Mover config.h a /my

[/] Música OGT Cheril Perils y Cadàveriön

[·] Terminar Cheril Perils (fase extra, pantallas fijas).

[ ] Sir Ababol DX se me está quedando muy atrasado.

[ ] Si te cargas trozos de la fase y hay un reenter al morir, puedes aparecer bloqueado. Buscar solución

[X] En la animación de usar, si pulsas el salto "da saltitos".

Vamos a hacer la fase que queda de Cheril de un barco, y por petición popular meteré también la del cementerio, aunque intentaré que sea algo diferente, al menos en gráficos.

20180319
========

Ahora se me ha antojado un revamp bestial de Gran Sabiduría (MK2 polishing, that's it), pero antes me voy a obligar a terminar esto. Además tengo que darle un laneo para dejar listo el MiedoW. A ver si me quito esta lista de encima:

[ ] Mover config.h a /my

[/] Música OGT Cheril Perils y Cadàveriön

[·] Terminar Cheril Perils (fase extra, pantallas fijas).

[ ] Sir Ababol DX se me está quedando muy atrasado.

[ ] Si te cargas trozos de la fase y hay un reenter al morir, puedes aparecer bloqueado. Buscar solución

20180321
========

[ ] Mover config.h a /my

[/] Música OGT Cheril Perils y Cadàveriön

[/] Terminar Cheril Perils (fase extra, pantallas fijas).

[ ] Sir Ababol DX se me está quedando muy atrasado.

[ ] Si te cargas trozos de la fase y hay un reenter al morir, puedes aparecer bloqueado. Buscar solución

Tengo la fase del barco a puntito de caramelo melo, pero quiero ponerle algo de videoaventura. Por lo pronto sería para abrir dos partes: 

- La bodega, en la pantalla 14. Si no se cumple "el resultado del primer puzzle", hay que bloquear los tiles X = (A, B, C), Y = 9 con el tile 25.
- El castillo, en la pantalla 15. Si no se cumple "el resultado del segundo puzzle", hay que bloquear los tiles X = 15, Y = (6, 7, 8) con el tile 25.

Mientras tanto puedo ver de ir metiendo el cementerio pelao, que le hace ilusión a Anjuel. Lo bueno de meter el cementerio es que creo que de esta hay música, aunque quizá tenga que meterle bien metida la tijera.

~~

Los puzzles molaría que fueran de hablar con gente y correveydiles o quizá objetos y gente. Pero tiene que haber diálogo.

20180323
========

La cosa es ver qué personajes puedo meter. El problema es que me he liado a hacer este juego sin inventarme historia ni hostias, y quizá debería resolver esto antes de empezar.

La fase 1 es en la playa de pepinoni. Ahí hay que romper los templos raros para que se abra la puerta fosfi porque así podemos matar a todos los enemigos.  La fase 2 es una fábrica de conservas. La fase 3 es el barco. ¿Dónde encasqueto el cementerio? Creo que puedo meterlo como tercera fase y dejar el barco como fase 4.

Aquí el tema debería estar en limpiar a todos los enemigos de todas las fases por un puto motivo. Si resuelvo estas cosas puedo meter más personajes y hablar y tal.

Por ejemplo, como me mola mucho Yun, puedo meter a Yun como recurrente. ¿Tengo paleta para Yun? Bueno, se podría intentar resolver. Podría decirse que Yun quiere expandir el negocio y como no puede dejar el mostrador en temporada alta llama a Cheril - pero esto tampoco tiene mucho sentido. Supuestamente Yun es una demonia (antiheroína) y Cheril suele trabajar por su propio oficio y beneficio. Esto no me casa. Es crossover metido con calzador. No me gusta.

También podemos ponerlo de precuela a The Nine. Quizá sea buena idea limpiar la costa de Pepinoni de escoria antes de que empiecen las olimpiadas. En este caso podría hablar con las musas y tal.

Otra cosa que podría resultarme es pensar en una lista de tonterías que quiero meter y a partir de ahí ver qué personajes necesito e inventarme algo.

Llevarse toda la mañana programando en typescript no te deja el celebro muy creativo, por lo que se ve.

~~

Voy a hacer una fase intro reaprovechando pepinoni (TS) y con texto introductorio. "ah, pepinoni, mi sitio preferido" "playas soleadas, alegres bañistas" "¿Pero qué ha pasado? ¿Por qué está todo tan mustio?" Y luego tras unas pantallas te encuentras a la novia de Ninjajar o a quien sea y te explica que han venido los Ohú, unos fantasmas horroribles que dan un montón de mieo. Y que hay que cargarse a todos los monstruos de cada sección para debilitar la magia maligna que hace que salgan más. O algo.

~~

Ya estoy otra vez con fases desordenadas por mierdas de no pensar y diseñar antes de hacer. W.

```
	0 - assets*4 (pepinoni intro)
	1 - assets*1 (pepinoni)
	2 - assets*0 (factory)
	3 - assets*3 (cementerio)
	4 - assets*2 (barco pirata)
```

AFU PICHA.

20180327
========

Anjuel es un amor y me ha pasado esta idea para argumento que voy a desarrollar metiendo personajes y diálogos:

> Cheril se va a Pepinoni a pasar unas vacaciones y ya de paso comenzar a pensar en su propia novela, aprovechando que ahora todo el mundo escribe la suya. Ahí ha quedado con Aitor, el Editor que le va a conseguir toda la gente necesaria para que le rellene párrafos de su libro y así no tenga que hacer nada. Ah, y por supuesto que sean graciosos, qué menos. Al llegar a Pepinoni se da cuenta del chocho que tienen montado con los zombis. Joder, no le dejan a una descansar en paz! 

> Una vez destrozados todos los zombies, y hecho eso de los templos que dice el nath y que no me acuerdo qué era, se da cuenta que se había dejado su libreta de APÚNTALO en casa. Así es imposible que te escriban una novela. Mientras volvía a casa, recordó que en su último picnic en el cementerio se había llevado la libreta de APÚNTALO para los posibles chascarrillos que pudieran surgir, pero una vez más, el cementerio estaba lleno de zombis! puñeteros zombis... cunden más que los crowdfundings. Crowdzombis!

> Pero la libreta de APÚNTALO no estaba ahí. ¿Y dónde estará? Lo mejor es contratar a un detective que estuviera libre un domingo por la mañana. El único era Perry Uribe, el Detective, que casualmente estaba por la ciudad. Este citó a Cheril en la vieja fábrica de Longanizas Nardo, pero el asunto no pintaba bien. ¿Quién podría quedar un domingo por la mañana en un sitio tan sugerente? Justo, un guarro de cuidado. Y encima también había zombis. 

> Total, que Cheril decidió, una vez matados todos los zombis, volverse a Pepinoni. Hasta que vio un maravilloso y lujoso Barco Pirata de Playmobil aparcado, que no anclado, a los muelles de Pepinoni. Evidentemente, infestado de zombis. Su sueño de tener un sombrero pirata cada vez más cerca. ¡Qué pena que no cabía en el juego!

Mancantao candela. Ahora tengo que cambiar las fases de orden:

```
	- Intro (4)
	- Pepinoni (1)
	- Cementerio (3)
	- Fábrica (0)
	- Barco pirata (2)
```

Pero eso es cosa fásil. De `4 1 0 3 2` a `4 1 3 0 2`. No te qué, no te qué, no te quejes, nene.

Anda que no molaría un level manager que automáticamente te generase el código en orden y tal, pero no lo voy a escribir dos veces porque me pondré a hacerlo y tampoco es que haga tanta falta.

Ea, cambiao.

~~

También quiero cutscenes y tal, pero antes voy a hacer los diálogos ingame. Y tengo que modificar textbox para que saque un sprite en la esquinita como en el Otro Bosque de MK2.

Veamos, extensión a text box para portraits. Lo suyo es que haya la opción de no llevar personaje. Los sprites deberían poder sacarse de `spr_hs`, como todo lo demás asín que no es player o enemigos. Así que lo suyo sería codificar 0 = no sprite, cualquier número = índice en `spr_hs` de lo que hay que dibujar.

En `textbox_draw_text`, la linea de texto empieza en `0x2000 + 6 + (rdy << 5)` para cada linea. Tengo que modificar ese 6. En modo "con portraits", ese 6 debe venir de una variable que valga 6 (no portraits) o 9 (portraits).

Hecho. Ha sido una modificación ínfima (aunque tendré que ajustar, que la Y del metasprite la he puesto un poco a boleo).

~~

Tengo que montar un rollo script de diálogo que sencillamente secuencie textboxes. Rollo dos listas de `id_portrait` y `custom_text`, y que vaya consumiéndolas hasta un end marker o algo.


20180403
========

Estos días he avanzado un montón (además, documentándolo todo en el README.md del juego), y ahora mismo tengo todas las fases hechas, dos dibujos de cutscenes listos, y personajes metidos en todas menos en la última fase. Además tengo que probar BIEN el cementerio y la fábrica.

~~

Para ganar un poco de memoria, creo que voy a reorganizar el tema del textbox. 

Ahora mismo requiere definir 9 metatiles del tileset. Eso suponen 9*5 = 45 bytes, a lo que sumamos el array de 64 bytes que hace las veces de tilemap. El tema es que estamos gastando, con nuestros 4 tilemaps diferentes, 64 + 4 * 45 = 180 bytes.

El array de 64 bytes está bien. Otra forma sería definir 3 cadenas de 32 bytes, pero ya estamos usando más espacio aunque nos ahorremos los tilemaps...

Bueno, hago un commit y un push ahora y lo cambio a ver :*)

Uf, es que me voy a hartar de trabajar para salvar unas decenas de bytes y además voy a complicarlo todo y tener que modificar todos los testers y ejemplos y 

buaaah, así se queda.

También puedo mirar de meter los enemigos indexados de MK2 si me veo chungui pirati... Porque tengo que meter.

- Módulo de cutscenes.
- Los RLE de las cutscenes (!).
- Música (!!)

Y me quedan 2507 bytes. Uf, lo veo chungui.

Claro que para las cutscenes puedo pasar de boniteces y mostrar el texto del tirón, a piñón, y eso ocupa mucho menos. Pero aún tengo RLE a saco.

Voy a ir convirtiendo las cutscenes en RLE para ir haciéndome una idea. 

La pantalla de título va a tener que ser minimalista de la leche.

- Cuts 0 : 128 bytes.
- Cuts 1 : 133 bytes.
- Cuts 2 : 125 bytes.

Bueno, no es tanto. Los voy a ir metiendo para que luego no me pegue un zusto. 2124 bytes libres para música, código, y lo que quede. Me veo metiendo nametables en chr-rom XD

He metido un rearreglo de la música "fábrica" y quedan 1646 bytes. Miedo.

Voy a ir metiendo pres y luego ya lo dejo para llorar XD.

1201 bytes. Buaaaah.

Texto de las cutscenes metido... Y queda poco más de medio K... XD

20180405
========

Ya sé lo que voy a hacer, después de pasarme pensando desde ayer a las 18:00 hasta hoy a las 9:25. Voy a sacar los enemigos a CHR-ROM. La forma en la que lo montaré, es crear una especie de librarian que junte bins y genere un índice. También haré que algunas herramientas, namely el exportador de enemigos, tengan un switch para que generen binario en vez de h (o simplemente detecten un .bin en el nombre de archivo de salida).

Me pondré primero con las herramientas y luego a dar soporte en el código. Esto debería liberarme una cantidad bestial de PRG-ROM que podré usar en darle a esto mejor aspecto y sobre todo meter música (`12*12 + 4*12*24` = 1296 bytes extra).

~~

Herramientas hechas y montado todo en el .bat. Esto va a necesitar un artículo para sí, algo que hable sobre CNROM, qué se espera de él, cómo se monta, y cómo está montado Cheril en concreto.

Ahora voy a programar la parte de engine. No debería ser complicado :*)

~~

Mierda, no había pensado en los hotspots, que deben estar todos disponibles. Voy a necesitar copia en RAM. Ehpehre, que ya lo tengo. Si son dinámicos tengo ya `ht` y `hyx`. Los puedo usar también en el caso este.

20180411
========

Working en la sombra (y en otras cosas), voy a intentar afeitar unos bytes más del código, que siempre viene bien. Voy a ver si gano algo cambiando los recorridos de arrays con puntero a recorridos con índice. Empezamos por ejemplo en `interactives_create`.

2023 <- Antes de empezar

Nu, en este caso termina ocupando algunos bytes más. ¿Ensamblador mierder? Voy a ver. Nah.

Este es el punto en el que gano bytes buscando cosquillas al compilador. O pasando a ensamblador cosas. ¡Algo habrá! Y como no puedo hacer cosas creativas porque mira todo el mundo y esto canta menos, pues eso. Win.

~~

2023 - inicial
2031 - `bolts_load`, de for a `memfill`
2039 - Como sólo se llama `bolts_load` y a `bolts_clear` en un sitio, deshacer encap.
2058 - `ep_flags` sólo se usa para muertos, simplificar.
2079 - Restructurar pantalla de título.

~~

Me estoy desanimando, no encuentro mucho más de donde rascar XD

TODO por los cambios:

- Delete bolts.h
- Test cerrojos.
- Test they stay dead.

~~

Voy a propagar a /src y luego a los demás para tenerlo todo actualizado. Total, algún día tendría que hacerlo.

20180412
========

Para ahorrar un poco más (porque creo que es más liviana) voy a añadir al motor la implementación del salto / gravedad / release de MK2 y la dejaré como opcional. Veré cuanto salvo, además de tener un salto mejor / distinto (depende de los gustos).

La hostia, he ganado 10 bytes XD pa freírme.

TODO: Revisar todos los supuestos de salto (faltan: salto sobre enemigo, salto sobre hotspot) y modificar el inicio de salto para MK2. Probar: encapsular el "inicio de salto" (son tres asignaciones) y usar una llamada ¿salvará unos bytes?

¡Hecho! Y he ahorrado 41 bytes. Ahora mismo la cuenta está en 2130 bytes. He arañado 107 puros bytes con el sudor de mi frentew.

20180413
========

He modificado algunas cosas para meter las cutscenes. En concreto he apañado el salto de linea a la función que imprime un texto. Idealmente debería haber gastado pocos bytes metiendo las cutscenes (a falta del texto del final y de meter dos escenas: la de antes de la última fase y la del final).

Ahora mismo estamos en 2058 bytes libres en la versión en inglés y 1978 bytes libres en la versión en español (probablemente debido a que el título en RLE ocupa más).

A unas malas podría meter los nametables en RLE dentro de CHR-ROM si me queda algo libre, pero necesitaría un buffer en RAM del tamaño de la que más ocupe, que ahora mismo es 169 bytes (el primer cutscene). Dejémonos esa carta en la manga por ahora.

Tengo que:

- Completar la cutscene que falta y ver qué hacer en el final.
- Música.
- Cerrar la puerta de la cueva de los fantasmas del barco hasta que solo queden precisamente 2 enemigos por matar (esos dos).

~~

A ver si afeito algo más... Poco.

~~

Metidos todos los cutscenes, todos los textos. Todo

- (más) Música.
- Cerrar la puerta de la cueva de los fantasmas del barco hasta que solo queden precisamente 2 enemigos por matar (esos dos).

Quedan 1777 bytes (peor caso, versión en español).

20180416
========

He optimizado un millón de cosas aparte de meter lo que quedaba en la fase del barco: el tapón que bloquea el paso a los fantasmas y a la chica que te lo cuenta (texto incluido). Ahora tenemos 2070 bytes libres en la versión inglesa y 1978 bytes en la versión española. ¡Vaya mejora!

El juego está terminado, creo, a falta de probar mil y de meter las músicas que faltan, y creo que en el espacio que tengo me da :) Hasta podré florear un poco una vez que todo esté dentro :-)

Habrá que propagar los cambios. Pero antes tengo que probarlo bien, que he cambiado muchas cosas y a lo mejor me he cargado algo.

<<<<<<< HEAD
20180505
========

Joder, hace un siglo que no escribo por aquí. Esto está ya casi listo:

- Voy a implementar una solución para el respawn con reenter y breakable tiles: una búsqueda circular horizontal hasta encontrar un tile libre. Por suerte tengo el tester punchy para probarlo.

- Eliminar la carpeta de Sir Ababol DX - En realidad moverla a otro sitio porque es el proyecto que iré usando para hacer el tutorial. 

- Probar Bootee porque es lo primero que va a salir.

- Escribir el principio del tutorial: describir los componentes básicos de un juego AGNES, como interactúan, etc. Luego empezar con los conversores. Y montar el proyecto Ababol DX primero en modo básico.


=======
20180423
========

Estoy empezando a escribir algunos tutoriales mientras juego y rejuego y pulo y repulo el Cheril. Ahora mismo voy a propagar los últimos cambios en el motor con vistas de añadir **Bootèe** a la ecuación (es sencillo). Aún estoy a vueltas con qué voy a hacer para el tutorial. **Bootèe** es un juego un poco raro para hacer de tutorial. No sé si empezar con **Sir Ababol DX** como tenía pensado, pero empezando por lo super básico (el juego pelado sin ampliaciones) y luego ir construyéndolas.

Voy a propagar.

~~

Cosas de Bootee:

- En el movimiento del player, el salto automático (sustituye al bloque `PLAYER_JUMPS`), y la detección de PAD_DOWN para frenar verticalmente.
- Los gargajos, que probablemente reimplemente si veo que el código es un horror.

Qué gracia, hace poco tiempo me follé el módulo gargajos.h del paquete porque pensaba descartarlo o dejarlo como ultra custom para este juego. 

Por lo pronto veo una función con una jartá de código. Tengo un `MAX_GARGAJOS` que indica el número máximo de gargajos. Creo que esto debería generalizarlo con las balas para evitar que haya tantas constantes, ya que no voy a hacer coincidir balas y gargajos ¿no? Bueno, nunca se sabe ... Es un número bajo, 2.

Hay un tiempo entre gargajos, `GARGAJO RELOAD`, controlado por `pgargajocounter`, en el que no se puede lanzar otro gargajo.

Los gargajos tienen una vida que tiene que ver con `pfiring`. `pfiring` es un contador de frames a partir de que se pulsa `PAD_B`. cuando se suelta `PAD_B`, se mira si `pfiring` supera un umbral. Si esto pasa, se lanza un gargajo con el valor de `pfiring` como "vida".

El gargajo colisiona con el fondo y con los enemigos.

Originalmente el gargajo podía ser animado y tener facing. Creo que esto me lo puedo fumar para algo tan pequeño y que se mueve tan rápido. Con un `GARGAJO_PATTERN` me vale.

El gargajo describe una trayectoria lineal y muere tras N frames, en realidad es como una puta bala simple ¿No?

Una bala simple con gauge. Joder, es que podría generalizarlo y dejarme de hostierías. Voy a pensar.

~~
>>>>>>> 83c7722dae2364e9f241fbb9ed83aae08e1996c8

A ver, si metiese a las balas:

- Tiempo de "recarga": un tiempo en el que no puedes disparar. Se establece tras un disparo y se decrementa automáticamente.
- Tiempo de "vida", a establecer de forma externa.
- Charge & release: forma de disparar que se basa en dejar pulsado, cargar, y soltar.

HMMM...

Creo que sería mucho más mejol.

~~

Ya lo tengo, es muy sencillo y total win. Y la barra de carga es custom y va en el código del marcador, creando un nuevo código de inserción: `my/extra_hud_update.h` (también he cambiado levemente `my/game_frame.h`).

~~

Creo que lo he añadido todo a ciegas XD Ya lo probaré cuando tenga el resto montado.

~~

Cambio algunos tiles para optimizar el mapa. Luego meteré mierdas custom para que quede bonico.

Antes 2244, después 2126. Oye, que casi 100 bytes está muy bien, macanaqui.

20180425
========

Asín un TODO, como rápido:

- Músicas y más testing para Cheril the Writer / Apúntalo. ¿Activar `NO_HORIZONTAL_EVIL_TILE`?
- Reusar una música "que pegue" para Bootèe, menú de selección de GAME A/B/C y los dos textos finales que faltan.
- Propagar Bootèe y activar el recoiling para Sgt. Helmet, y así se prueba en top-down.
- Scan horizontal al reaparecer si tenemos activado el reenter on respawn y los breakables buscando un hueco, en tester_sideview y propagar.

He empezado con el recoil top-down y no funciona bien. Tendré que fix, propagar a src, propagar a bootèe, retest, y luego de src a todos.


