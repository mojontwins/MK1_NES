Me falta un cacho, hacer merge luego.

20180929
========

De camino a retrozaragoza aprovecho el tren para dar un poco de fran a la idea de hacer que AGNES soporte doble anchura.

He resuelto cosas fáciles, pero ahora tengo que darle al módulo de los enemigos y eso implica darle fran incluso a la colisión.

Veamos: los enemigos siguen estando en su propia pantalla y sus coordenadas X siguen siendo de 0 a 255. Sabemos estas cosas:

- Los enemigos 0 a 2 pertenecen a la pantalla de la izquierda.
- Los enemigos 3 a 5 pertenecen a la pantalla de la derecha.

¿No resolví esto para Lala? Voy a revisar ese diario antes de seguir. Pero antes que nada tendré que ampliar los arrays a 6 espacios y procesar 6 enemigos en lugar de 3.

Tengo este apunte:

> - Enemigos funcionando -- me hice un poco la picha un lío pero es que no recordaba que las coordenadas de los enemigos funcionan dentro de su "pantalla virtual" y que a la hora de comparar con el prx tenía que añadir el offset de 256 si el enemigo era id >= 3.

Pero me suena que esto dio más guerra... Sigo leyendo. Pues parece que no hay más chicha... Veamos.

Me voy a dejar de mierdas y de historias para esto. La colisión ya me dará más quebraderos, así que a lo fácil.

