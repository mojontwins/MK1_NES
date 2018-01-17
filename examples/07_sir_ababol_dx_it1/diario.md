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

