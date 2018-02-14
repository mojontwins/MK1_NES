Repegar cuando pueda

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
				n_pant = _en_mx;
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

