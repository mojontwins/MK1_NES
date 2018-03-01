Listo los sonidos que necesitaré

- Salto nuevo.
- !floaty -> floaty (ffff).
- pisar enemigo
- activar resonador
- cuenta atrás.
- golpear con hitter.
- enemigo punchy golpea (el mismo de antes).
- golpear breakable.
- breakable se rompe (al pisar o disparar).
- enemigo lanza coco.
- spring lanzado

Estos son los que hay

00 Start						Start, Pause
01 TR 							Empujar una caja, abrir un cerrojo, dejar objeto, usar objeto (!)
02 Item (Usar, trit agudo)		Container, objeto a hotspot, coger objeto, coger llave, coger ammo, coger tiempo
03 toitoitoitoi (con eco)		Refill
04 Muerte						Colisión perils -> player
05 TR RT 						
06 TST 							Colisión Bullet -> enemigo
07 Salto 						Salto, salto sobre enemigo, etc.

En el pool murciano me gustan:

Del pool murciano:

- Salto: 			jump #2
- Flotar: 			pescaos
- pisar enemigo:	assplosion (primera nota solo)
- resonador:		shotgun
- cuenta atrás: 	bite
- golpear hitter: 	knife
- punchy golpea: 	knife
- hit breakable:	shotgun
- breakable rompe: 	shotgun
- dispara coco: 	laser 2
- dispara bullet: 	laser 1
- spring (resorte): wrong

Esta es la lista preliminar:

00 SFX_START	Start
01 SFX_TILE		Empujar una caja, abrir un cerrojo, dejar objeto
02 SFX_OBJECT	Container, objeto a hotspot, coger objeto, coger llave, coger ammo, coger tiempo
03 SFX_USE		Refill, usar objeto (!)
04 SFX_PHIT		Colisión perils -> player
05 ?
06 SFX_ENHIT	Colisión Bullet -> enemigo
07 -
08 SFX_JUMP		Salto
09 SFX_BULLET	dispara bullet
10 SFX_COCO		dispara coco
11 SFX_SPRING	spring
12 SFX_COUNT	cuenta atrás (resonador, timer)
13 SFX_BREAKH	hit resonador, hit breakable
14 SFX_HITTER	golpea con hitter (player + punchies)
15 SFX_STEPON	pisar enemigo, chac chac
16 SFX_FLOAT	flotar, pezón
17 SFX_BREAKB	break breakable, enemy dead

Tocamos todo :-/ otro update coñazo.
