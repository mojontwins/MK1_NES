Apunto cosas que se me van ocurriendo y quiero añadir, y las voy tachando según las voy añadiendo.

[ ] Hostias en genital.

[X] Integrar swimming mediante switch y no mediante define excluyente.

> Símplemente proteger con una guarda sobre flag y que se pueda activar a la vez que cualquier tipo de salto/jet pac. Diseñar cómo podría hacer funcionar esto de forma sencilla.

[X] Implementar `TILE_GET` y algún tipo de persistencia (como en Sonic Mal). 

> Sonic Mal codificaba cada fila de tiles con 1 byte, lo que nos daba la posibilidad de NO pintar el `TILE_GET` si aparecía un 1 en el bit correspondiente a dos tiles adyacentes. Este arreglo es *suficiente* y sólo toma 12 bytes de RAM por pantalla -> Equivale a activar `PERSISTENT_ENEMIES`.

[X] Crear un code injection point para meter soporte de nuevos tipos de hotspots, y así eliminar la necesidad de tocar el core.

[ ] Recorridos de plataformas rectangulares.

[ ] Portar desde MK2 los enemigos tipo lineal para ahorrar ciclos (aunque sean menos precisos)

> Más cosas que tengo que documentar:

- Todo el tema del double width:
[ ] Explicar el split y cómo montarlo. Explicar el `NO_SPLIT` que está pensado para probar al principio o por si quieren usar sprites para el _hud_.
[ ] Usar el modo `REAL_TIME_MAP_ATTR` para ahorrar RAM
[ ] `SINGLE_SCREEN_SUPPORT` y qué significa. Pantallas sencillas forzadas.
