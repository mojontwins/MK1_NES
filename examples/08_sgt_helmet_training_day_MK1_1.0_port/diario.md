20180122
========

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

