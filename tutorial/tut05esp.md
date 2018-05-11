Primera prueba
==============

Lo primero que haremos será copiar el motor tal cual en una nueva carpeta y compilarlo tal cual, con lo que debería generarse el juego tonto por defecto. 

Esta es la prueba más sencilla de que lo tenemos todo.

Lo primero es que descargues AGNES si no lo has hecho ya. Para ello puedes usar [un .ZIP del último release](https://github.com/mojontwins/MK1_NES/releases), o clonar el repositorio con:

```
    $ git clone https://github.com/mojontwins/MK1_NES.git
```

Nuestro primer proyecto
=======================

Necesitamos

1. Asegurarnos de que tenemos un `setenv.bat` bien configurado para la ubicación de **cc65 2.13** 
2. Copiar el contenido de `src` a una nueva ruta.
3. Compilar el proyecto.

Un entorno válido
-----------------

Como mencinamos, **cc65** necesita un entorno mínimo - básicamente necesitamos meter su directorio `bin` en el `PATH`. Esto puede conseguirse de muchas formas pero yo prefiero un enfoque *portable* ya que suelo manejar bastantes compiladores (e incluso diferentes versiones del mismo) en mis máquinas. Por eso suelo crearme un archivo `setenv.bat`. Por comodidad, vamos a editar el archivo `setenv.bat` que hay en `src/dev` para que los cambios sean válidos para todos los proyectos que empecemos.

Es tan sencillo como modificarlo para reflejar la ubicación correcta de **cc65**. Si, por ejemplo, has clonado AGNES en `c:\git\MK1_NES`, el contenido de `src/dev/setenv.bat` debería ser:

```
    @echo off
    set path=c:\git\MK1_NES\cc65_2.13.2\bin\;%path%
    cc65 --version
```

Creando el proyecto
-------------------

Siempre que empecemos un nuevo juego copiaremos el contenido de src a una nueva ubicación. Yo, por ejemplo, he instalado **MK1_NES/AGNES** en `c:\git\MK1_NES\`, y he creado dentro un directorio `projects`. Luego he creado un subdirectorio `first_test` y dentro de él he copiado todo el contenido de `c:\git\MK1_NES\src`, de forma que ha quedado así:

```
    c:\git\MK1_NES
    `-- projects
        `-- first_test
            |-- dev
            |-- enems
            |-- gfx
            |-- map
            |-- ogt
            |-- script
            |-- trash
            `-- utils
```

Compilando el proyecto
----------------------

Normalmente lo que hacemos antes de compilar es quitar todos los placeholders, configurar, rellenar, y completar con nuestros tiestos, pero esto es una pequeña prueba en la que compilaremos el motor con la configuración básica y todos los placeholders.

Para ello abriremos nuestro terminal favorito (deberíais probar [ConEMU](https://conemu.github.io/)), nos desplazaremos al directorio de nuetro proyecto y ejecutaremos primero `setenv.bat` y seguidamente `compile.bat`. Nótese que `setenv.bat` sólo se ejecuta al abrir el terminal para que se registre la ruta de **cc65**.

```
    $ cd c:\git\MK1_NES\projects\first_test\
    $ setenv.bat
    $ compile.bat
```

Tras un divertido proceso deberíamos obtener un archivo `cart.nes` fresquito. Lo abrimos en nuestro emulador de NES favorito y disfrutamos de nuestra obra.



Quería empezar suave, así que lo dejamos aquí hasta la siguiente entrada. ¡A disfrutar!