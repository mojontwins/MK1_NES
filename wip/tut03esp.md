Descripción de MK1_NES/AGNES
============================

Lo primero que haremos será describir el motor, listar sus componentes, e introducir los conceptos básicos.

Como sabréis, **MK1_NES/AGNES** es un motor sencillo para hacer juegos por pantallas. Las características de los mismos se definen por una serie de módulos que pueden activarse o desactivarse y combinarse entre sí, admitiendo más personalización por medio de inyecciones de código en puntos clave del motor o el uso de un sistema de scripting nativo bastante flexible.

Las características asociadas a los diferentes componentes pueden activarse o desactivarse. Obviamente, cada característica activada supondrá más código en la ROM y menos espacio para datos.

Estos son los componentes básicos:

Mapa
----

El mapa es la representación de nuestro mundo, y está dividido en **pantallas**. El mapa está compuesto por *tiles* de 16x16 píxeles, definidos en un **tileset**. Cada pantalla es una rejilla de 16x12 *tiles*. Cada *tile* tiele asociado un **comportamiento** que define cómo interactúa con el jugador y los otros *actores*. Jugando con estos comportamientos crearemos plataformas, escenario destructible, obstáculos, etcétera.

Hay una serie de comportamientos *básicos*, como pueden ser "obstáculo", "plataforma" (sólo vista lateral) o "vacío", por ejemplo, y un montón de comportamientos extra que se pueden activar de forma modular:

- Este *tile* **mata**.
- **Arenas movedizas** (sólo vista lateral): si el jugador está sobre este *tile*, se hunde lentamente y puede liberarse saltando.
- **Destructible**: Este *tile* se romperá si es alcanzado por un proyectil o golpeado por el usuario.
- **Cinta transportadora** (sólo vista lateral en la versión 1.0): El *tile* arrastrará al jugador a la izquierda o a la derecha.
- **Resbaloso** (sólo vista lateral en al versión 1.0): El jugador experimentará menos fricción al posarse sobre él.
- **Cerrojo**: El *tile* actuará como obstáculo y desaparecerá al usar una llave.
- **Empujable**: El jugador puede empujar estos *tiles*.
- **Escalera** (sólo vista lateral): Este *tile* representa a una escalera que el jugador puede trepar.
- **Flotar** (sólo vista lateral): El jugador experimentará un empuje hacia arriba al tocar estos *tiles*.

Los comportamientos se especifican como valores numéricos y se pueden combinar siempre que tenga sentido, por ejemplo para crear una plataforma resbalosa combinaremos *plataforma* con *resbaloso*. Para hacer una alambrada electrificada (que mata) que se pueda destruir, podríamos combinar *mata* con *destructible*, etcétera.

Protagonista
------------

Es el personaje o ente que controla el jugador. Hay una serie de características que pueden activarse, por ejemplo:

- **Reacción al morir**: Podemos decidir si queremos que el jugador rebote, parpadée, o reaparezca en el último "punto seguro" en el que estuvo al ser golpeado por un enemigo, un proyectil, o caer en un *tile* que *mata*.
- **Empujar**: El protagonista puede desplazar *tiles* con el comportamiento *empujable*.
- Activar una **animación especial al interactuar** con items.
- **Golpear** (sólo vista lateral en la versión 1.0): Podemos activar (por separado) que el jugador pueda golpear en el aire y en el suelo.
- **Disparar**: El jugador podrá lanzar proyectiles. Entre las subcaracterísticas configurables está la activación de **munición limitada**, **tiempo de vida de los proyectiles** o **carga y disparo**.
- **Saltar** (sólo vista lateral en la versión 1.0), con dos tipos de física.
- **Autosaltar** (sólo vista lateral en la versión 1.0), o rebotar: el personaje está continuamente saltando o rebotando contra el suelo.
- **Nadar** (sólo vista lateral) estilo Alex Kidd (el jugador "flota").
- **Jet Pack** (sólo vista lateral).
- **Saltar sobre los enemigos** sin sufrir daños, pudiendo dañarlos a ellos o no.

Enemigos
--------

Los enemigos son los antagonistas móviles del protagonista. Hay bastantes tipos de enemigos en la versión 1.0, y añadir más es relativamente fácil:

- **Patrollers**: se desplazan en una trayectoria cíclica y lineal.
- **Fantasmas**: persiguen al protagonista revoloteando. Puede configurarse que se detengan en los obstáculos o no. Existen dos tipos: el que persigue sin más y el que sólo persigue si el protagonista se acerca, volviendo a su posición original si lo "pierde de vista".
- **Monococo**: permanecen fijos en el escenario, ocultos, para aparecer brevemente y disparar un proyectil en dirección al protagonista. Mientras están ocultos son invulnerables.
- **Cañón**: permanecen fijos en el escenario disparando de forma periódica en una dirección predefinida. Pueden configurarse para ser indestructibles.
- **Pezón**: un pez que salta y da un bocao.
- **Pegadores** y **disparadores**: tipos especiales de **Patrollers** que pegan o disparan.
- **Perseguidores** (sólo para la vista cenital en la versión 1.0): generador de enemigos que "lanza" enemigos que persiguen al jugador. Al eliminar una instancia, el generador lanzará otro. Puede configurarse para que el generador sea visible y destructible.
- **Sierras**: una sierra que describe una trayectoria lineal, para luego ocultarse y volver al principio.
- **Chac Chac**: cuchilla para poner en pasillos y hacer mucha pupa.
- **Compilados**: enemigos que responden a un comportamiento programado sencillo (desplazamiento en las 8 direcciones, esperar o disparar, en bucle).

Dependiendo de la configuración, los enemigos podrán eliminarse de uno o varios impactos.

Proyectiles
-----------

Existen dos tipos de proyectiles en la versión 1.0, aunque es posible que esto cambie en el futuro:

- **Balas**: proyectiles que dispara el protagonista.
- **Cocos**: proyectiles que disparan los enemigos.

En esta implementación, las **balas** sólo dañan a los enemigos y los **cocos** sólo dañan al protagonista.

Otros actores
-------------

Existen otros actores que no son específicamente *enemigos*, por ejemplo:

- **Ventiladores** (sólo vista lateral): crean un área de *tiles* tipo **flotar** sobre ellos, de longitud configurable. Pueden activarse o desactivarse.
- **Bloques destructibles**: pueden ser destruidos por impactos de valas o golpes, o configurarse para destruirse al caminar sobre ellos.
- **Brillos**: sólo para hacer bonito: los *tiles* que *matan* destellean de cuando en cuando.
- **Resonadores** (sólo vista lateral): paralizan a los enemigos durante unos segundos.
- **Teletransportadores** o **warpers**: llevan al jugador a otro punto del mapa.
- **Muellepinchos** o **springs** (sólo vista lateral): sacan un *tile* (generalmente de los que *matan*) cuando el jugador los pisa.
- **Interactivos**: contenedores de objetos (ver *items*) o simples sprites a los que se puede asociar una respuesta cuando el protagonista interactúa con ellos.

Temporizador
------------

Podemos activar un temporizador muy sencillo que cuenta segundos y al que se le puede asociar diferentes acciones. 

Hotspots
--------

El **hotspot** es un punto importante de cada pantalla donde puede haber un objeto (ver en el siguiente apartado), una llave o una recarga (de vida, munición o tiempo).

Items
-----

El motor incorpora varios tipos de sistemas de objetos e inventario, desde uno muy sencillo hasta implementaciones más complejas, as saber:

- **Colecciónalos todos**: todos los objetos son iguales y al recogerlos se incrementa un marcador. Es la opción más simple. Los objetos coleccionables aparecerán en el hotspot.
- **Easy objects**: implementación sencilla que permite almacenar objetos en los hotspots, e interactuar con ellos. El protagonista podrá acarrear uno de estos objetos. El objeto podrá ser depositado en otro hotspot. Se puede configurar los hotspots para tener sitios específicos donde utilizar los objetos, que pueden quedar fijados en el destino y lanzar una respuesta.
- **Interactives**: implementación más compleja que permite definir varios *contenedores* en cada pantalla con los que interactuar. Los contenedores podrán estar vacíos o contener un objeto, y podrán ser utlizados como recipientes destino para utilizarlos. 

-

Estos son los componentes básicos de todo juego creado con **MK1_NES/AGNES**. ¿Puedes examinar los ejemplos de **AGNES** e intentar indentificar los componentes en los diferentes juegos? En varios ejemplos y en los *testers* tienes un `README.md` con bastantes indicaciones sobre cómo están construidos.
