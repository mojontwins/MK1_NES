Me falta un cacho, hacer merge luego.

20180929
========

De camino a retrozaragoza aprovecho el tren para dar un poco de fran a la idea de hacer que AGNES soporte doble anchura.

He resuelto cosas fáciles, pero ahora tengo que darle al módulo de los enemigos y eso implica darle fran incluso a la colisión.

Veamos: los enemigos siguen estando en su propia pantalla y sus coordenadas X siguen siendo de 0 a 255. Sabemos estas cosas:

- Los enemigos 0 a 2 pertenecen a la pantalla de la izquierda.
- Los enemigos 3 a 5 pertenecen a la pantalla de la derecha.

¿No resolví esto para Lala? Voy a revisar euse diario antes de seguir. Pero antes que nada tendré que ampliar los arrays a 6 espacios y procesar 6 enemigos en lugar de 3.

Tengo este apunte:

> - Enemigos funcionando -- me hice un poco la picha un lío pero es que no recordaba que las coordenadas de los enemigos funcionan dentro de su "pantalla virtual" y que a la hora de comparar con el prx tenía que añadir el offset de 256 si el enemigo era id >= 3.

Pero me suena que esto dio más guerra... Sigo leyendo. Pues parece que no hay más chicha... Veamos.

Me voy a dejar de mierdas y de historias para esto. La colisión ya me dará más quebraderos, así que a lo fácil.

20180930
========

En el tren resolví colisión, display, y que los fanties pululen por toda la pantalla. Tengo que revisar los homing fanties, por cierto, porque hay que tener en cuenta de qué pantalla son para la posición a la que vuelven, y probablemente tenga que rehacer partes.

No sé si hacer eso ahora o ponerme con balas y cocos. Pero antes quiero medir cómo voy de frame. Tengo que hacer un split de 32 lineas arriba (intentaré meter mierdas antes del split), por lo que quiero ver como va la cosa, porque la solución más sencilla para cocos y balas es usar enteros en las coordenadas X.

Por ahora parece que tengo bastante frame libre (entre 1/2 y 1/3, más tirando para 1/2), así que creo que por lo pronto tiraré por lo fácil y luego ya veremos. Vamos a por los disparos!

Funciona guay y todo guay, pero cuando hay 4 disparos y 3 enemigos en pantalla la franja gris baja peligrosamente hasta la parte inferior de la pantalla, quedándose a menos de un cuarto.

Voy a hacer ahora los cocos. Tengo que activar:

[X] Monococos.
[X] Compiled.
[X] Shooties.
[X] Steady shooters.

Empezaré por los shooties que no dejan de ser lo más sencillo.

{TODO}: [ ] Estudiar la viabilidad de tener compiled y pursuers por toda la pantalla doble. Aunque creo que va a ser que no. [a menos que haga algún repurpose raro]

{TODO}: [ ] Añadir la posibilidad de que los shooties sean timed y disparen al jugador como los de Ninjajar!

Next -> Hitter

2018102
=======

OK - Está pegando framazos. Ahora es el momento de fliparse un poco con el tiempo de proceso. Cuando hay todos los cocos a la vez en pantalla da picos bestiales. De hecho hay un pico cuando el compiled dispara - imagino que lo mismo pasará cuando lo hace un monococo. Esto es por el cálculo de trayectoria.

Debería hacer algo: al menos, para liberar un poco, debería "invalidar" la ejecución del resto de los cocos para este frame. También me debería plantear hacer una implementación aparte para los cocos lineales porque es tontería estar aplicando cálculos de punto fijo con este tipo de cocos.

Voy a hacer las dos cosas:

[X] Disparar un coco_aimed invalida la ejecución de `cocos_do` este frame.

la invalidación durante un frame de `cocos_do`  no parece ser muy efectiva. El cálculo de la dirección parece seguir pillándose todo el frame del demonio. Voy a tener que pensar en algo para reparar esto porque no le veo salida - bueno, no le veo salida que no sea "no usar"...

O eso, o implementar otros disparadores direccionales menos precisos que disparen en 8 o 16 direcciones precalculadas según particiones del espacio. Pero que tengo que pensar. Con 8 no es suficiente.

- Se me ha ocurrido una forma realmente rocambolesca.

¡Y funciona bastante bien y ya no me pega tirón!

Seguimos :)

Hitters!
========

Esto necesita más infraestructura, necesito sprites que no tengo. Y ahora miran mucho. ¿Lo dejamos para luego luego?

Ah, tengo pendientes los homing fanties.


