# Journey Engine
Videogame engine created for the game engine architecture course 

## Requisitos
Se necesita tener Visual Studio 2019 y CMake 

## Build para Windows

- Clonar el repositorio
- Dentro del repositorio escribir el comando ``git submodule update --init --recursive``
- Compilar cada libreria en release dentro de una carpeta build al lado de la carpeta source (solo glfw y glad nesecitan compilacion).
    - mkdir libs\glfw\build
    - cd libs\glfw\build
    - cmake ..\source\glfw\ .
    - Abrir el .sln
    - Hacer la compilación en modo release.
    - Codigo quedara linkeado a libs\glfw\build\Release\glfw.lib
    - Repetir con las otras librerias necesarias.
- Compilar y ejecutar el juego de ejemplo
    - Desde la carpeta del reppsitorio ingresar : ``mkdir build``
    - cd build
    - cmake ..\ .
    - Abrir el .sln dentro de build
    - Compilar en modo Release
    - El ejecutable estara en build/games/Release

¡¡Es importante ejecutar el juego desde el directorio ``build/games/Release``!! Esto para que funcionene correctamente los path a los assets del demo:

- cd build/games/Release
- ./LittleDemo_v0.exe
