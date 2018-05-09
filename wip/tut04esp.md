Las herramientas
================

En el paquete hay un conjunto bastante nutrido de herramientas, que podemos dividir en dos grupos: herramientas externas y herramientas internas. **MK1_NES/AGNES** emplea versiones legacy del compilador **cc65** y de **neslib** (uno de los hitos del *roadmap* es pasarse a una versión moderna de **neslib**). Hemos querido añadir al paquete todo lo necesario para funcionar. Simplemente descomprimiendo el ZIP o clonando el repositorio de Github deberías tener todo lo necesario a mano.

Herramientas externas
=====================

cc65 v2.13
----------

Se trata de la última versión oficial del compilador antes del fork actual. La versión antigua de **neslib** fue escrita con esta versión del compilador en mente. En futuras versiones, **MK1 NES/AGNES** será adaptado para usar una versión moderna de **cc65** y la **neslib** más reciente. Existen varias características de **neslib** que han cambiado bastante, en especial la estructura de datos empleada por la rutina que actualiza VRAM durante VBLANK.

**cc65** reside en `/cc65_2.13.2/bin/`. Para que el compilador funcione, este directorio debe estar en el *PATH*. Existen varias maneras para extender el *PATH* en Windows. Yo prefiero el enfoque *portable* que consiste en crearla en un archivo .bat que ejecuto al principio de cada sesión de terminal.

```
	@echo off
	set path=c:\AGNES\cc65_2.13.2\bin;%path%
	cc65 --version
```

(El ejemplo asume que ANGES está instalada en c:\AGNES. Modifica la ruta para adecuarse a tu configuración ~ siempre es buena idea olvidarse de espacios o caracteres extraños en las rutas cuando se hacen estas cosas, por cierto).

La linea `cc65 --version` debe mostrar la versión del compilador. Es simplemente para comprobar que lo hemos hecho bien.

Famitracker 0.4.6
-----------------

**MK1 NES/AGNES** emplea la biblioteca **famitone2** para el sonido, que fue diseñada para funcionar con la versión 0.4.6 de Famitracker. Es por eso que hemos incluido una copia del tracker en `/ext_utils/FamiTracker0.4.6`. Necesitarás este programa para crear música y efectos de sonido.

Mappy (Mapwin)
--------------

Se trata de una vieja aplicación para diseñar mapas. Hace bastante tiempo que dejó de estar disponible [en su web oficial](https://tilemap.co.uk/mappy.php); además está configurado y personalizado con varios extras. Hemos incluido una copia en `/ext_utils/Mappy/`. Con este programa compondremos los mapas de los juegos de **MK1 NES/AGNES**.

Nes Screen Tool 1.43
--------------------

Se trata de una versión bastante antigua, pero más que suficiente para lo que la vamos a usar: comprimir en RLE los *nametables* que usaremos en diferentes pantallas fijas en nuestros juegos.

En una futura versión de **MK1 NES/AGNES** añadiremos soporte para realizar esta conversión mediante una de las herramientas del *toolchain*, eliminando la necesidad de tener que trabajar *a mano* en un programa visual.

Sublime Addons
--------------

Si empleas el editor Sublime Text 3, aquí encontrarás un sencillo addon que añade resaltado de sintaxis para los archivos de script que maneja **MK1 NES/AGNES**.

Herramientas internas
=====================

Se trata de una serie de herramientas, la mayoría de linea de comandos y preparadas para formar parte de un *toolchain*. Todas están en `/src/utils/`.

Binary Paster binpaster.exe
---------------------------

Esta herramienta concatena los archivos binarios de una lista y genera un índice de offsets a los diferentes binarios dentro del global que podemos usar directamente en nuestro proyecto. Se utiliza principalmente para amalgamar datos binarios para enviarlos a CHR-ROM y disponer de un índice para su acceso desde el código.

CHR Viewer chrview.exe
----------------------

Visualizador de CHR-ROM. Muestra una ventana mostrando los patrones de dos bancos de CHR-ROM (8 Kb). Puedes ver el índice de cada patrón señalándolo con el mouse.

.ene Exporter 3 eneexp3.exe
---------------------------

Convierte los archivos .ene de colocación de enemigos en código (si vas a almacenar los datos sobre los enemigos y hotspots en PRG-ROM) o datos binarios (si los vas a almacenar en CHR-ROM) directamente usables por el motor.

Zero filler fillto.exe
----------------------

Permite establecer el tamaño en bytes de un archivo binario, que será truncado si el tamaño especificado es menor que el original, o rellenado con ceros al final si es mayor.

Map Converter NES 2 mapcnvnes2.exe
----------------------------------

Es un conversor de mapas. Toma como entrada archivos .MAP de **Mappy** generando código directamente utilizable por el motor. El formato de salida de los mapas puede ser 1 byte por *tile* (máximo de 256 *tiles*) o de 1 byte por cada dos *tiles* (packed) (máximo de 16 *tiles*). También genera *decoraciones*, de las que hablaremos en el capítulo que habla de los mapas.

Conversores de mapas en RLE53 rle53mapMK1.exe y rle53mapcchrrom.exe
-------------------------------------------------------------------

Conversores de mapas, como `mapcnvnes2.exe`, pero que generan los datos con compresión **RLE53** (5 bits de valor, 3 bits de repetición). Permite emplear tilesets de 32 *tiles* y los datos generados ocupan bastante menos que los del equivalente descomprimido. Si tus tilesets tienen 32 *tiles* o menos, este es el mejor método. Más sobre esto en el capítulo que habla de los mapas.

`rle53mapMK1.exe` genera mapas en formato **RLE53** en formato código y lo usarás si los vas a almacenar en PRG-ROM. `rle53mpachrrom.exe` hace lo propio si los vas a almacenar en CHR-ROM.

Conversor de mapas RLE legacy rlemap2.exe
-----------------------------------------

Este conversor genera los mapas en formato RLE16, que funciona bastante bien para tilesets de 16 *tiles* más decoraciones. Más sobre esto en el capítulo que habla de los mapas.

Graphics importer mkts.exe
--------------------------

Es un conversor de gráficos. Es capaz de convertir varios tipos de entes y extraer patrones y metadatos. Lo emplearemos para generar patrones de charsets, tilesets, spritesets, y generar paletas. Se trata de un programa bastante complejo y potente al que dedicaremos todo un capítulo.

Nametable generator namgen.exe
------------------------------

Este programa toma un archivo .png con una imagen de pantalla completa, una paleta, y una imagen binaria de CHR-ROM (o *pattern set*), y genera un *nametable* de 1024 bytes con atributos. Dicho *nametable* se puede cargar posteriormente en **NES Screen tool**.

Enemy Compiler pencompiler.exe
------------------------------

Es un compilador de scripts de comportamiento para enemigos de tipo *compiled*. Lee un archivo con el comportamiento y lo traduce a datos utilizables por el motor.

Ponedor de enemigos ponedor.exe
-------------------------------

Programa gráfico para colocar enemigos y hotspots en el mapa. Este programa genera los archivos .ene que consume `eneexp3.exe`.

Importador de textos text2carray.exe
------------------------------------

Genera arrays de C a partir de archivos de texto. 

-

El el próximo capítulo empezaremos a describir los pasos para la creación de un juego con el motor de forma práctica: construyendo un port del juego de ZX Spectrum **Sir Ababol DX**. Hasta entonces, [recordad que me encanta el café](https://www.buymeacoffee.com/nath) ;-)
