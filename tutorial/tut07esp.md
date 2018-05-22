Gráficos e importación
======================

*Nota: los archivos referenciados en este artículo se encuentran en la carpeta `/tutorial/assets/Sir Ababol DX/gfx`*

Como diseñadores, siempre hemos trabajado en tener un sistema que nos permita ocuparnos únicamente de poner píxeles en imagenes, sin tener que estar convirtiendo, recalculando, y haciendo trabajo manual para conseguir que esos gráficos estén en el juego.

Este sistema, que ha ido evolucionando poco a poco desde mediados de 2015 hasta la actualizad, es una especie de Frankenstein llamado **mkts**. Se trata de una herramienta de linea de comandos que es capaz de leer un script donde explicamos cómo recortar los gráficos a partir de imagenes png para generar binarios con patrones y archivos de código con diversos tipos de metadatos (como, por ejemplo, descripción de metasprites o metatiles).

Aunque **mkts** hace muchas cosas, nosotros vamos a recortar básicamente cuatro tipo de entidades:

- **Charsets**: en el modo más básico, la herramienta genera una serie de patrones de 8x8 a partir de un rectángulo de la imagen de entrada. Este tipo de entidad no genera metadatos.
- **Metatilesets**: nos servirá para hacer los trozos que compondrán las pantallas del mapa de nuestro juego. Se trata de entidades de 2x2 patrones con su propia paleta. En este modo, **mkts** *reaprovecha* patrones ya recortados y genera metadatos: un *mapa* para cada tile indicando qué cuatro patrones lo componen y qué paleta debe usarse para imprimirlo.
- Metasprites: como ya hemos dicho, la PPU de la NES mueve sprites a los que se asocia un único patrón de 8x8 (en el modo que emplea **AGNES**). Para hacer sprites más grandes, tenemos que asociar varios sprites de 8x8 que moveremos juntos. A estas asociaciones se les llama metasprites. **mkts** es capaz de importar metasprites, añadiendo nuevos patrones según los va encontrando, y generando como metadatos las estructuras que relacionan los diferentes sprites que lo componen y su colocación.
- **Metaspritesets**: colecciones de metasprites con las mismas características.

Los metadatos generados estarán etiquetados por identificadores que luego emplearemos cuando estemos construyendo los spritesets. Si tienes prisas, puedes echar un vistazo al archivo `src/dev/assets/metasprites` y ver el mapeo de metasprites del proyecto *por defecto*, aunque a esto llegaremos más adelante. No debería preocuparte por ahora.

Antes de explicar cómo funciona **mkts**, echemos un vistazo a los gráficos que empleará **Sir Ababol** en su versión más básica:

El charset
----------

> **AGNES** utiliza charsets de 64 caracteres que deben estar mapeados al principio del banco de CHR-ROM donde estén ubicados (esto es, deben tener los índices 0 a 63). El orden es el mismo que se define en ASCII, de forma que para obtener un caracter determinado se tomará su valor ASCII menos 32 para obtener el número de patrón. Los charsets incluyen símbolos, números, y el alfabeto en mayúsculas. Es normal sustituir algunos símbolos que no usarás por trozos de gráficos que vayas a necesitar. 

Este es el charset que usaremos en **Sir Ababol**, contenido en `font.png`. Como verás, hay algunos símbolos que en el futuro utilizaremos como bordes para pintar cajas de texto.

[[font.png]]

El tileset
----------

Dependiendo del tipo de codificación que vayamos a usar para almacenar los mapas (más sobre esto en el capítulo correspondiente), el tileset de un nivel podrá tener 16, 32, o hasta 256 tiles diferentes. Cada *tile*, en este contexto, se refiere a los trozos de 16x16 pixeles que componen las pantallas de nuestro mapa.

**Sir Ababol** emplea mapas de 16 tiles. Además, hay cuatro tiles extra que se emplean desde un renderer custom que hará sustituciones dependiendo de una serie de reglas (más sobre esto más adelante). Finalmente, el tileset incluye 9 tiles a partir del tile número 32 que se emplearán en las cajas de texto en el futuro.

[[ts0.png]]

El spriteset
------------

Los spritesets de **AGNES** están formados por tres tipos de entidades:

- Sprites del **protagonista**, con animaciones para todas las acciones que pueda hacer. El mapeo de los diferentes cells de animación al estado del personaje en el juego es custom y se hace sobre el código de **AGNES**.
- Sprites para los **enemigos**, cubriendo todos los tipos de enemigos que aparezcan en el juego. En este spriteset se incluyen los empleados para plataformas móviles.
- Sprites para **items e interactivos** - coleccionables, llaves, recargas, objetos, personajes con los que hablar...

> Para **AGNES**, el protagonista está representado por un rectángulo de 8x16, y los enemigos e items/etc por rectángulos de 16x16. Estos rectángulos se utilizan para la colocación y colisión de los actores en el juego, por eso los llamamos *rectángulos lógicos*. Obviamente los gráficos (metasprites) mostrados en pantalla pueden ser mayores, como veremos pronto.

En el spriteset de **Sir Ababol** tenemos animaciones para Sir Ababol andando, saltando, y nadando. En esta primera encarganción no emplearemos los cells de Sir Ababol nadando, aunque los vayamos importando ya.

Seguidamente tenemos enemigos y plataformas. En la primera fila aparecen tres tipos de enemigos y una plataforma, seguidos por la explosión que se mostrará al eliminar los enemigos. De nuevo, esto no ocurrirá en la primera versión básica que vamos a construir, pero lo importaremos igualmente.

En la siguiente fila tenemos tres tipos de enemigos extra que se emplearán en el nivel del río subterraneo y que, por tanto, no veremos por ahora.

Finalmente tenemos los sprites para items: el Ababol (coleccionable), la llave (para abrir cerrojos), una pata de pollo (recarga de vida), y dos que aparecerán en la versión *DX*: las botas que activarán la habilidad de pisar a los enemigos para eliminarlos, y unos cartelitos que codificaremos como hotspots custom y que mostrarán textos explicativos. De nuevo, aunque no les daremos uso, los iremos importando.

[[ss.png]]

Importaciones y paletas
=======================

Estamos ante la primera ocasión en la que es primordial que entendamos cómo funciona el hardware de la NES, y de ahí aquella entrada de introducción un poco lata que dimos al principio.

Es importante recordar que tendremos cuatro paletas de 3 colores para el fondo más cuatro paletas de 3 colores *que pueden y deberían ser* diferentes para los sprites, más un color de fondo global.

Para que **mkts** pueda funcionar, tiene que saber qué paletas tiene que reconocer. Como los colores que genera la PPU se prestan mucho a diferentes interpretaciones, **mkts** es capaz de *entender* una paleta específica que se incluye en el paquete: `fullnespal.png`.

[[fullnespal.png]]

(Además, **mkts** puede funcionar en un modo en el que es capaz de interpretar la paleta por defecto que viene en Aseprite).

Para que todo funcione debemos, pues, elaborar nuestros gráficos usando colores de esa paleta. Además, tendremos que generar *imagenes de paleta* para **mkts**. En concreto, para esta primera versión de **Sir Ababol**, emplearemos una *imagen de paleta* para el fondo, `palts0.png`, y otra para los sprites, `palss0.png`.

Las *imagenes de paleta* son archivos `.png` de tamaño 16x1 píxels. De cada 4 píxels, el *primero* debe ser el color de fondo común, y los otros tres los que elijamos para cada subpaleta. Por ejemplo, esta es una ampliación x16 de `palts0.png` donde especificamos qué cuatro paletas podrán llevar los tiles de fondo (los de `ts0.png`), y que **mkts** necesitará para poder importarlos.

[[palts0_big.png]]

Cómo funciona mkts
==================

**mkts** puede funcionar de varias formas; la que emplearemos será el *modo scripting*. **mkts** leerá comandos de un script e irá construyendo el binario que empearemos como CHR-ROM, además de los archivos de código C con los metadatos asociados.

**mkts** es capaz de generar bimarios completos para CHR-ROM (de 8K en dos bancos de 4K, llamados banco 0 y banco 1) o de generar binarios parciales (para emplear con otros mappers). Para generar binarios completos tendremos que emplear comandos extra que vayan rellenando. Asimismo, **mkts** va añadiendo los patrones que va importando a un *pool* que consulta cada vez que va a recortar para identificar patrones ya importados - y provée de un comando para *resetear* dicho pool y que emplearemos cuando hayamos terminado de recortar patrones para el banco 0 y vayamos a empezar con el banco 1.

El script de recorte para **Sir Ababol DX** está en `/tutorial/assets/Sir Ababol DX/gfx/import_patterns.spt`, pero lo iremos construyendo paso a paso.

> Lo primero que deberías hacer es entrar en `/gfx/` de la carpeta del proyecto, ubicar el archivo `import_patterns.spt` y eliminar el contenido que trae por defecto.

**mkts** recorta basándose en rectángulos definidos por unas coordenadas de inicio y unas dimensiones, y lo hace de izquierda a derecha y de arriba a abajo.

Antes de empezar, hacer notar que las *coordenadas* y los *tamaños* especificadas suelen estar a nivel de *patrón* (de 8x8 píxeles), las *dimensiones del set* a nivel de *entidad* (por ejemplo, un set de 2x1 a la hora de recortar metasprites se refiere a 2x1 metasprites del tamaño definido) y que los *offsets* (en los metasprites) están en píxeles.

Comandos básicos
----------------

Aquí hay una lista de comandos básicos que estaremos usando continuamente:

```
	OPEN image.png
```

Cargará `image.png`. Todos los comandos de recorte a partir de este punto serán sobre esa imagen, hasta que se cargue otra.

```
	PALS pal.png
```

Cargará la *imagen de paleta* `pal.png` y la establecerá como *paleta actual*. Todos los comandos de recorte a partir de este punto utilizarán esa paleta, hasta que se cargue otra.

```
	MAPFILE metadata.h
```

Establece el archivo de salida para metadatos. Todos los metadatos se escribirán en el archivo especificado, hasta que se especifique otro.

```
	LABEL label
```

Define `label` como *prefijo* de los identificadores generados a partir de este punto en el código de los metadatos asociados. 

```
	RESETPOOL
```

Elimina el *pool* de patrones que el sistema emplea para identificar duplicados. En este ejemplo, lo utilizaremos para terminar en banco 0 antes de empezar el banco 1.

```
	FILL to
```

Rellena con 0 hasta que el tamaño del archivo binario de salida llega al valor `to`. Lo emplearemos para cerrar el banco 0 (`FILL 4096`) y el banco 1 (`FILL 8192`).

Banco 0, charset
----------------

Lo primero que importaremos serán los 64 patrones para escribir textos a partir de `font.png`. Para ello emplearemos el comando `CHARSET`, cuya sintaxis es:

```
	CHARSET x0, y0, w, h
```

Donde `x0, y0` son las coordenadas origen dentro del png y `w, h` las dimensiones del rectángulo que estamos recortando. Como en `font.png` los 64 patrones aparecen en un arreglo de 32x2 a partir de la esquina superior.

```
	OPEN font.png
	PALS palts0.png
	CHARSET 0, 0, 32, 2
```

Como los charsets no generan metadatos, no necesitaremos usar los comandos `MAPFILE` o `LABEL`.

Banco 0, tileset
----------------

En esta primera versión, que ni siquiera tiene pantallas fijas, completaremos el banco 0 con el tileset que se emplea en el mapeado principal (`ts0.png`). Para recortar tilesets empleamos el comando `MAPPED`, cuya sintaxis es:

```
	MAPPED x0, y0, wm, hm, w, h [, max]
```

Donde `x0, y0` son las coordenadas dentro del png y `w, h` las dimensiones *en tiles* (recordad: las dimensiones de los rectángulos que recortamos se miden *en entidades*) del rectángulo. Adicionalmente, `wm, hm` son las dimensiones *en patrones* de cada tile. En **AGNES**, estas dimensiones son *invariablemente* `2, 2`. `max` se emplea si queremos recortar menos tiles de los que caben en el rectángulo.

En nuestro ejemplo tenemos que recortar 41 tiles de `ts0.png`, donde hay un arreglo de 16x3. Estamos recortando muchos tiles vacíos, pero en este ejemplo no nos vamos a preocupar por eso. Antes del recorte, además, configuramos el archivo que contendrá los metadatos relacionados con los tilesets (porque en **DX** habrá más de uno). Como **mkts** se ejecuta sobre el directorio `gfx/`, tendremos que indicar la ruta relativa del archivo de la siguiente manera, ya que todos los metadatos irán en `dev/assets/`:

```
	MAPFILE ..\dev\assets\tiledata.h
```

Posteriormente, el recorte:

```
	OPEN ts0.png
	PALS palts0.png
	LABEL ts0
	MAPPED 0, 0, 2, 2, 16, 3, 41
```

(La linea `PALS` no sería realmente necesaria ya que esa es la paleta activa llegados a este punto del script, pero personalmente me gusta indicarlo todo de forma explícita porque luego es más fácil hacer reorganizaciones y cambios si es necesario)

En este código abrimos `ts0.png`, establecemos la paleta activa leyéndola de `palts0.png`, indicamos que los próximos metadatos se generen con el prefijo `ts0`, y finalmente recortamos 41 tiles de 2x2 patrones de un rectángulo de 16x3 que empieza en (0, 0).

En los metadatos almacenados en `tiledata.h`, **mkts** generará dos arrays por cada comando `MAPPED`: `<prefix>_pals`, con la paleta que emplea cada tile, y `<prefix>_tmaps`, indicando qué 4 patrones utiliza cada tile. En nuestro caso, serán `ts0_pals` y `ts0_tmaps`.

> Si nuestro juego sólo va a tener un tileset y no queremos tocar demasiado, el prefijo especificado en `LABEL` debe ser `ts0`. 

Banco 0, cierre
---------------

Como ya no vamos a recortar más (al menos, en la versión básica del juego), cerramos:

```
	RESETPOOL
	FILL 4096
```

(En el archivo `import_patterns.spt` suministrado verás lineas para recortar dos tilesets extra que puedes ignorar por ahora, o incluir si lo prefieres).

Como hemos dicho, eso reinicia el `pool` de patrones y rellena hasta los 4Kb con 0. A partir de los 4Kb, empieza el banco 1, el de los patrones empleados como sprites.

Banco 1, preparación
--------------------

Como todos los metadatos relacionados con los sprites irán en el mismo archivo, todos los sprites están definidos en `ss.png`, y todos emplean una única paleta `palss0.png`, empezamos esta sección con:

```
	OPEN ss.png
	PALS palss0.png
	MAPFILE ..\dev\assets\spritedata.h
```

Y esto será válido para todos los recortes siguientes:

Banco 1, protagonista (Sir Ababol)
----------------------------------

[[ss0-detalle-player.png]]

Recordemos que, para **AGNES**, el protagonista es a todos los efectos un rectángulo llamémosle *lógico* de 8x16 píxeles (aunque podamos configurar un poco la altura a la hora de colisionar). Obviamente, nuestro Sir Ababol es bastante más grande que eso, en concreto los cells son de 16x24 (aunque no es un requerimiento que todos los cells tengan el mismo tamaño, ni mucho menos).

Lo que ocurre es que el punto de origen del metasprite se sitúa con respecto a la esquina superior izquierda del rectángulo lógico de 8x16 empleando una pareja de offsets, como se puede ver en esta imagen:

[[logical-metasprite-offset.png]]

Generalmente, querremos que el rectángulo *lógico* esté centrado en la parte inferior del gráfico. Los seis cells de animación (1 x "parado", 4 x "andando", 1 x "en el aire") del juego principal comparten offsets, por lo que podemos recortarlos todos de un tirón empleando el comando `METASPRITESET`, cuya sintaxis es:

```
	METASPRITESET x0, y0, wm, hm, w, h, offs_x, offs_y[, MAX] [FLIPPED]
```

Donde `x0, y0` son las coordenadas dentro del png y `w, h` las dimensiones *en metasprites* (recordad: las dimensiones de los rectángulos que recortamos se miden *en entidades*) del rectángulo. Adicionalmente, `wm, hm` son las dimensiones *en patrones* de cada metasprite. En **AGNES** no hay límite para estas dimensiones más que los impuestos por el hardware y los recursos disponibles.

`FLIPPED` hará que se genere automáticamente una versión mirando al lado contrario.

> Por convención, dibujaremos los sprites mirando hacia la derecha.

Por tanto:

```
	LABEL sspl
	METASPRITESET 1, 1, 2, 3, 6, 1, -4, -8 FLIPPED
```

Atención a los offsets: `-4, -8` significa que el metasprite recortado debe pintarse 4 píxeles a la izquierda y 8 píxeles más arriba del origen de coordenadas (extremo superior izquierdo) del rectángulo *lógico*. Mira de nuevo la imagen de más arriba.

Los metadatos generados para este comando serán doce arrays conteniendo los 6 cells recortados y sus versiones espejadas. `sspl` se tomará como prefijo, y los identificadores generados en el código tendrán un formato

```
	sspl_XX_F
```

donde `XX` será un índice incremental en hexadecimal y `F` valdrá `a` o `b` para las versiones normal y espejada (mirando a la derecha, mirando a la izquierda, por convención). Por tanto, el comando `METASPRITESET` de más arriba generará arrays con los identificadores:

```c
	sspl_00_a, sspl_00_b, sspl_01_a, sspl_01_b, 
	sspl_02_a, sspl_02_b, sspl_03_a, sspl_03_b, 
	sspl_04_a, sspl_04_b, sspl_05_a, sspl_05_b
```

Seguidamente, recortaremos los tres cells para la animación de nadar, aunque aún no los vayamos a usar. En este caso, el offset es levemente diferente: el rectángulo *lógico* se centra dentro del metasprite de 16x24, con lo que los offset serán `-4, -4`.

```
	LABEL sspl2
	METASPRITESET 13, 1, 2, 3, 3, 1, -4, -4 FLIPPED
```

Fíjate de nuevo que las coordenadas de origen `x0, y0` están medidas en patrones de 8x8, y que las dimensiones de los rectángulos de recortes están medidas en *entidades*, en este caso *metasprites* de 2x3 patrones.

Este comando generará seis arrays en el archivo de metadatos (3 cells y sus versiones espejadas):

```c
	sspl2_00_a, sspl2_00_b, 
	sspl2_01_a, sspl2_01_b, 
	sspl2_02_a, sspl2_02_b 
```

Banco 1, enemigos
-----------------

[[ss0-detalle-enems.png]]

> Por lo general, en **AGNES**, los metaspritesets para cada enemigo deben contener 8 cells, 4 mirando en cada sentido (izquierda y derecha). De esas 4, las dos primeras forman la animación de desplazarse, la siguiente es el cell de "ataque" y la última "muriendo". Obviamente no es necesario tener 8 cells diferentes; de hecho, los cells de "ataque" y "muriendo" solo se emplean con configuraciones específicas.

Antes de empezar recordemos que, en **AGNES**, el rectángulo lógico para los enemigos es de 16x16 píxels, al igual que el del jugador era de 8x16 píxels.

Empleando `METASPRITESET`, iremos recortando los enemigos poco a poco. En primer lugar tenemos tres tipos de enemigos con dos cells de animación cada uno. en **AGNES**, se soporta dos cells de animación por orientación. Para estos enemigos no generaremos versiones espejadas horizontalmente sino que emplearemos los mismos cells para ambas direcciones (más adelante).

Empezamos con dos enemigos de 2x3 y uno de 3x2, seguido por una plataforma móvil de 2x2 y una explosión. Como tenemos entidades de diferentes características, las recortaremos por separado.

Primero los enemigos de 2x3, 2 en total, 4 cells:

```
	LABEL ssena
	METASPRITESET 1, 4, 2, 3, 4, 1, 0, -8
```

Como el rectángulo lógico es de 16x16 y nuestros enemigos *altos* de 16x24, habrá que dibujar el metasprite 8 píxeles más arriba; de ahí el `0, -8`.

El siguiente enemigo es el murciélago, de 3x2, 2 cells:

```
	LABEL ssenb
	METASPRITESET 9, 4, 3, 2, 2, 1, -4, 0
```

Aquí al tener un metasprite de 24x16 y un rectángulo lógico de 16x16, habrá que pintar el metasprite 4 píxeles más a la izquierda; de ahí el `-4, 0`.

Las plataformas son de 2x2.

```
	LABEL ssplat
	METASPRITESET 15, 4, 2, 2, 2, 1, 0, 0
```

Aquí el tamaño lógico corresponde con el metasprite, de ahí que el offset sea `0, 0`.

Cuando ampliemos a DX necesitaremos un pequeño set de enemigos para la fase subacuática, más un sprite para la explosión que aparece cuando Sir Ababol pisa a un malo.

Los dos primeros enemigos acuáticos no necesitan espejado y son de 16x16. 4 cells en total:

```
	LABEL ssenc
	METASPRITESET 1, 7, 2, 2, 4, 1, 0, 0
```

El tercer enemigo acuático es un pez que sí se debe diferenciar a izquierda y derecha. Recortamos sus dos cells así:

```
	LABEL ssend
	METASPRITESET 9, 7, 2, 2, 2, 1, 0, 0 FLIPPED
```

Finalmente, la explosión es un único frame:

```
	LABEL ssexpl
	METASPRITESET 19, 4, 2, 2, 1, 1, 0, 0
```

Todos estos comandos crearán un montón de arrays en el archivo de metadatos que usaremos cuando construyamos el metaspriteset:

```c
	ssena_00, ssena_01, ssena_02, ssena_03, 
	ssenb_00, ssena_01, 
	ssplat_00, ssplat_01,
	ssenc_00, ssenc_01, ssenc_02, ssenc_03, 
	ssend_00_a, ssend_00_b, ssend_01_a, ssend_01_b,
	ssexpl
```

Banco 1, items/extra
--------------------

Recortaremos un total de 5 metasprites con el ababol (item coleccionable), la llave, una pata de pollo (recarga de vida), las botas (para DX) y el letrero (también para DX). Recordemos que los items también tienen un rectángulo lógico de 16x16.

```
	LABEL ssit
	METASPRITESET 1, 9, 2, 2, 5, 1, 0, 0
```

Banco 1, cierre
---------------

Como ya no vamos a recortar más, cerramos:

```
	FILL 8192
```

Esto rellena el binario de salida hasta los 8Kb con 0. Con esto, el resultado nos sirve para importar directamente como CHR-ROM.

Ejecutando mkts
===============

Vamos a probar que todo funcione correctamente. Desde una ventana de intérprete de comandos (¿has probado [conEmu](https://conemu.github.io/)?), nos desplazamos al directorio `/gfx/` de nuestro proyecto y ejecutamos:

```batch
	$ ..\utils\mkts.exe mode=scripted in=import_patterns.spt out=..\dev\tileset.chr
```

Esto generará una salida en la consola y además creará `tileset.chr` en `/dev/`, además de los archivos `/dev/assets/tiledata.h` y `dev/assets/spritedata.h`.

Comprobemos que el contenido de `tileset.chr` (la imagen de la CHR-ROM de nuestro cartucho) es correcto:

```batch
	$ ..\utils\chrview.exe ..\dev\tileset.chr
```

lo cual debe mostrar 

[[contenido-de-tileset.png]]

-

En la próxima entrada veremos como organizar todos los arrays de los metasprites que hemos recortado en el **metaspriteset de AGNES**. Hasta entonces puedes intentar [mantenerme despierto](https://www.buymeacoffee.com/nath) ;-) .
