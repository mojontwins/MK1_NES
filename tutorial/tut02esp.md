<img src="http://www.mojontwins.com/dev/wp-content/uploads/2018/04/bleg.jpg" alt="" width="953" height="269" class="aligncenter size-full wp-image-57" />

Antes de empezar a diseñar un juego para cualquier plataforma, y aunque tu perfil no sea excesivamente técnico, es necesario concerla un poco. No vamos a entrar en entresijos complicados (ya te podrás poner a darle Fran por tu cuenta si te interesa, porque la **NES** es una de las plataformas retro mejor documentadas), pero sí es necesario aclarar algunos conceptos. Aquí hay un montón de información, pero sólo es necesario que le eches un vistazo para que luego te suenen los conceptos.

La **NES** tiene dos peculiaridades, principalmente:

- Por un lado, tiene dos espacios de direcciones separados: el de la CPU y el de la PPU (que es como se conoce en este sistema al procesador que se encarga de los gráficos). Esto quiere decir que el sistema gráfico tiene su propia memoria a la que la CPU no tiene acceso directo, ya que está en otro bus diferente.
- Sin un cartucho conectado, el sistema no está completo.

Aunque esto ocurre en otras plataformas, los que vengáis de toquetear ordenadores lo encontraréis bastante diferente. Normalmente, en un ordenador tenéis RAM sobre la que cargáis el software desde un soporte externo. En la NES, al enchufar un cartucho, estáis completando la arquitectura del sistema con memoria que contiene el software *y posiblemente otros componentes*.

Componentes principales
=======================

Dentro de la **consola** tenemos:

- La CPU, o unidad central de proceso, que incorpora un procesador custom compatible con el 6502 pero sin la unidad BCD, encargado de ejecutar el software, y el chip encargado del audio.
- La PPU, que se encarga de formar la imagen que ves en la pantalla. La PPU contiene una serie de registros mediante los cuales la CPU puede comunicarse con ella y 256 bytes donde almacena información relacionada con la ubicación y atributos de los sprites.
- 2Kb de RAM en el espacio de direcciones de la CPU, desde $0000 a $07FF.
- 2Kb de VRAM en el espacio de direcciones de la PPU, desde $2000 a $27FF que están reflejados en $2800 a $2FFF.
- 32 bytes de paleta en el espacio de direcciones de la PPU, desde $3F00 a $3F1F.
- Fullerías que implementan la entrada/salida (no sé, cosas. A algún sitio hay que enchufar los mandos y eso) y otros tiestos que no te interesan.

Dentro del **cartucho**, por su parte, tenemos:

- PRG-ROM: 32K de ROM que la CPU ve en su mapa de memoria, desde la posición $C000 a la $FFFF. En esta memoria reside el programa y sus datos estáticos.
- CHR-ROM: 8K de ROM que la PPU ve en su mapa de memoria, desde la posición $0000 a la $1FFF. En esta memoria reside las dos tablas de patrones, cada una de la cual contiene 256 "trocitos" de 8x8 pixeles con 4 colores (en realidad, 3 más uno que puede considerarse "transparente") con los que se forma la imagen.
- CHR-RAM: Algunos cartuchos llevan un chip de 8K de RAM en lugar de una ROM con los patrones. En estos casos, el contenido de dicha RAM debe ser escrito desde el programa.

32K más 8K puede parecer muy limitado. Todos hemos visto juegos de NES más grandes. ¿Cómo se logra eso? Pues con circuitería adicional dentro del cartucho. Dicha circuitería (que puede ser tan sencilla como un simple contador o tan compleja como un ASIC) se encarga de recibir "señales" de la CPU (de diverso tipo, generalmente escrituras en determinadas posiciones) y "paginar" la memoria, acción que consiste en cambiarle a la CPU el trozo de memoria que está biendo, **sin que la CPU se de cuenta de nada**.

Las ROMs más sencillas, aquellas que no llevan hardware para paginar las ROMs, son las conocidas como NROM. De entre las que sí llevan hardware extra, las más sencillas son las tipo CNROM (que llevan hardware para cambiar CHR-ROM en bloque, esto es, sustituir los 8K por otros) y las tipo GNROM (que llevan hardware para cambiar PRG-ROM en bloque, esto es, sustituir los 32K por otros). Otro tipo de ROMs de las que hablaremos en los tutoriales son las tipo UNROM (que llevan hardware para cambiar 16K de los 32 por otros y CHR-RAM en lugar de CHR-ROM para los patrones).

Cómo funcionan los gráficos
===========================

La NES, como muchos otros sistemas, forma la imagen utilizando dos componentes:

- Un "fondo", o *nametable*, que es una rejilla de 32x30 celdas. En cada celda podemos representar un patrón de 3 colores + transparente. Cada grupo de 2x2 celdas (esto es, cada macrocelda de una macrorejilla de 16x15) puede representarse usando cualquiera de las 4 paletas destinadas para el fondo.
- Una capa de sprites. Cada sprite es una entidad que puede colocarse en cualquier lugar de la pantalla de 256x240 píxeles y mostrará un patrón (o dos patrones uno encima del otro dependiendo de una configuración global de la PPU) de 3 colores + transparente que puede representarse usando cualquiera de las 4 paletas destinadas para los sprites. Además, cada sprite lleva asociados unos atributos que indicarán si se debe mostrar espejado vertical u horizontalmente, o si debe ser dibujado *detrás* de la capa de fondo (en los píxeles transparentes).

Un poco de información extra interesante:

- A través del fondo, en los píxeles transparentes, se verá el color de fondo (global para toda la imagen) o los sprites configurados para aparecer tras el fondo.
- Sí, los sprites son de 8x8 o de 8x16 píxeles, dependiendo de una configuración global de la PPU - o sea, *todos* los sprites serán de 8x8 o de 8x16. Elegir unos u otros depende de muchos factores porque ambas configuraciones tienen sus ventajas y sus inconvenientes.
- El hardware sólo puede mostrar 8 sprites en cada linea de píxeles. Si hay un noveno sprite en una determinada linea, no se mostrará.

Paletas
-------

El sistema soporta 8 paletas diferentes de 3 colores cada una más un color "de fondo" global (en total 25 colores simultaneos). Las 4 primeras paletas se utilizarán para dibujar el fondo y las 4 restantes para los sprites. 

Los colores se seleccionan de una tabla de 64 entradas que se parece a esto:

<img src="http://www.mojontwins.com/dev/wp-content/uploads/2018/04/fullnespal.png" alt="" width="128" height="32" class="aligncenter size-full wp-image-42" />

Y digo "se parece" porque, como dicen, la NES se diseñó en un principio para el sistema NTSC - que entre otras cosas también significa *Never The Same Color*.

Pattern Tables
--------------

Los *patrones*, como hemos mencionado, son los trocitos de 8x8 píxeles que se emplean para formar fondos y representar sprites. Dichos patrones se organizan en dos tablas o bancos de patrones de 256 patrones cada una. Los patrones pueden estar en RAM o ROM, dependiendo del tipo de cartucho. El hardware permite elegir qué banco usaremos para representar el fondo y cuál para los sprites. Podemos elegir el mismo banco para ambas cosas, además.

Nametables
----------

Un *nametable* ocupa 1Kb. De estos, los primeros 960 bytes corresponden a los 960 patrones que caben en la rejilla (32x30). Los últimos 64 bytes son *atributos*. Cada byte contiene información sobre qué paleta deben llevar 4 macroceldas de 16x16 pixeles (que corresponden a 2x2 patrones). Esta es la razón por la que sólo podamos elegir paleta cada 16x16 píxeles.

El hardware de la NES soporta 4 *nametables* diferentes: de $2000 a $23FF, desde $2400 a $27FF, desde $2800 a $2BFF y desde $2C00 a $2FFF. Sin embargo, como hemos dicho, el sistema sólo tiene 2K de VRAM para *nametables*, con lo que realmente sólo puede almacenar *dos*. Por suerte, el hardware permite especificar qué dos nametables de las 4 son las que estarán disponibles mediante la configuración del *mirroring*. Esto, que en realidad es muy sencillo, puede tener algo de chicha si tu perfil no es muy técnico. Para entendernos, y simplificar un poco, diremos que las dos nametables pueden colocarse una encima de la otra o una al lado de la otra. Las dos nametables que no están son copias de las que sí estan. Por eso a la configuración vertical (una nametable encima de la otra) se le llama *horizontal mirroring* o *reflejo horizontal* (porque las dos nametables que no están reflejan a las que tienen al lado) y viceversa.

Puedes leer más sobre mirroring (con dibujicos) en la [wiki de Nesdev](https://wiki.nesdev.com/w/index.php/Mirroring).

Scroll
------

No, **MK1_NES/AGNES** no soporta scroll, pero vamos a mencionarlo. Las 4 nametables se colocan en un arreglo de 2x2. La imagen mostrada en pantalla es en realidad una ventana móvil dentro de este arreglo de 2x2 pantallas virtuales. La posición de dicha ventana viene dada por las *coordenadas de scroll*. El arreglo, además, es circular: si la ventana se sale por un lado se muestra lo que hay al otro.

La leche, qué lío
=================

Como hemos dicho, no necesitas memorizar todo esto, pero quería dar la brasa un poco para que luego suenen los conceptos. Así rápido:

- PRG-ROM: donde va el programa y los gráficos.
- CHR-ROM: donde van los patrones en el cartucho.
- CHR-RAM: aquí también van los patrones, pero es RAM que tendremos que rellenar desde el programa.
- Pattern table: tabla de 256 patrones. El sistema maneja dos.
- Nametable: rejilla de 32x20 patrones y 64 atributos para pintar el fondo.
- Sprite: los cachitos que se mueven.

En la próxima entrada empezamos a hablar de **MK1_NES/AGNES**. Lo huro.

(Foto de cabecera de http://blipretro.com/cleaning-famiclone-carts/)