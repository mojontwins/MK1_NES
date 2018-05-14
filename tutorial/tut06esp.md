Montando un nuevo juego: Sir Ababol DX
======================================

En 2010, mientras desarrollábamos nuestro primer motor modular para ZX Spectrum (MTE MK1, más conocido como *la Churrera*), hicimos una serie de juegos con las primeras versiones del motor. Uno de estos juegos fue **Sir Ababol**. En él teníamos que recorrer un mapa de 40 pantallas por el que andaban desperdigados 24 ababoles que teníamos que encontrar y recoger para terminar el juego. Algunas áreas del juego estaban bloqueadas por cerrojos que podíamos abrir con llaves.



Años más tarde, cuando ibamos a sacar **Sir Ababol 2** para ZX Spectrum, nos apeteción reconstruir el original y creamos **Sir Ababol DX**, un remake para ZX Spectrum de **Sir Ababol** que ampliaba el concepto original del juego añadiendo más pantallas, un río subterráneo donde nadar, y la posibilidad de obtener unas botas para eliminar a los enemigos.



Vamos a construir un port de **Sir Ababol DX** sobre **MK1_NES/AGNES**. He elegido este juego porque es bastante interesante por diversos motivos:

- La *Base* de **Sir Ababol** original (sin DX) de ZX Spectrum es el juego más sencillo que puede implementarse en **MK1_NES/AGNES**. Implementando esta versión base primero aprenderemos a manejar el toolchain y a montar un proyecto sencillo.
- Los *Añadidos* de **Sir Ababol DX** implican añadir una fase extra, modificar el motor para hacer un cambio de fase no ortodoxo, manejar varios tilesets, activar algunas características del motor y hacerlas opcionales via modificaciones, etcétera.

En este tutorial os daremos todo hecho (gráficos, mapa, colocación de enemigos) pero es coveniente que los examinéis bien y que entendáis qué estamos haciendo en cada paso.

Cómo se construyen los juegos con AGNES
---------------------------------------

Supongo que habrá mil formas, pero mi método se basa en seguir esta lista:

- Diseño del juego.
- Diseño de los gráficos.
- Construcción de los mapas.
- Colocación de los enemigos y demás.
- Primer montaje, depuración, etc.
- Pantallas fijas (título, game over, etc).
- Pulido y florituras.

En resumen: tener *todo* lo que necesitas para construir el juego, montarlo, probar, y luego ya *terminarlo*.

Con **Sir Ababol DX** seguiremos un proceso doble, ya que primero montaremos **Sir Ababol** y luego ampliaremos con las modificaciones.

No quiero hacer capítulos largos, así que hablaremos sobre el diseño del juego y dejaremos los gráficos para el capítulo siguiente.

Diseño del juego
----------------

Como hemos dicho, el **Sir Ababol** original para ZX Spectrum es un juego muy sencillo. Nuestro personaje es capaz de saltar hasta 3 bloques de alto, recoger llaves y ababoles, y poco más. Todos los enemigos siguen trayectorias lineales. Es, como decimos, el tipo de juego más sencillo que puede hacerse con este motor. Nos bastará con reunir los materiales, ajustar un par de cosas, modificar la configuración base por defecto, y poco más.

**Sir Ababol DX** amplía el mapa con una tira de 20 pantallas extra situada en la parte inferior, que representa un río subterráneo. En esta tira el motor cambia: ahora Sir Ababol irá nadando. 

Cuando lleguemos a este punto, implementaremos esta tira extra como un nivel aparte. Modificaremos el motor para que se seleccione caída y salto para el nivel 0 (superficie) y nadar para el nivel 1 (río subterráneo). Modificaremos también el motor para que se detecte que salimos de la pantalla verticalmente en diferentes puntos para hacer un cambio automático de nivel.

En **Sir Ababol DX** además podemos recoger unas botas con las que podremos pisar y eliminar a los enemigos. Para esto habrá que activar la característica de "pisar" en el motor y luego modificarlo para poder activarlo y desactivarlo en tiempo de ejecución según el valor de una variable. Añadiremos las botas como un tipo de hotspot *custom* que sencillamente pondrá dicha variable a 1.

De nuevo, no os preocupéis si véis que hay demasiada información que no manejáis. Muchas cosas tomarán sentido a medida que vayamos construyendo la versión básica del juego.

