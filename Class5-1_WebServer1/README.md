            ===>    SERVIDOR WEB I    <===

Para poder crear nuestro servidor web dentro del ESP32 es necesario configurarlo como AP, de modo que pueda controlar las rutas, solicitudes y respuestas que sean necesarias. No se debe olvidar que el ESP32 cuenta con entre 4 a 8 MB de almacenamiento flash, el cual debe ser repartido entre el programa y nuestros assets web.

Para poder servir contenido web es necesario utilizar la librería SPIFFS que nos ayuda creando un sistema de archivos dentro de nuestro ESP32. Para esto también es necesario crear la carpeta /data que es donde van a ir todos nuestros assets  así como toda la estructura de la página web.

Además de la librería SPIFFS también necesitamos la librería ESPAsyncWebServer la cual nos ayuda a resolver todas las solicitudes a nuestro servidor de manera asíncrona.

Cabe mencionar que todo desarrollo backend debe ser realizado dentro de nuestro /src puesto que en este caso el lenguaje utilizado para todo el backend es C++

Al momento de subir el proyecto al ESP32 primero debemos crear y subir el sistema de archivos, para posteriormente compilar y subir el resto del programa.

        =>  Pasos a seguir para subir el sistema de archivos
    
    1. PlatformIO -> esp32dev -> Platform -> Build Filesystem Image
    2. PlatformIO -> esp32dev -> Platform -> Upload Filesystem Image