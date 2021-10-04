            ===>    ENTRADAS Y SALIDAS DIGITALES    <===

Una señal digital es aquella que toma un conjunto de valores discretos, es decir que puede asumir un número contable de valores (0 y 1).

Para poder trabajar con señales digitales es necesario designar una resistencia de estado lógico, puede ser de alguno de los siguientes tipos:

    -> PULL UP      Resistencia conectada directamente hacia nuestro voltaje de alimentación, lo que nos genera un estado lógico de 1 por defecto
    -> PULL DOWN    Resistencia conectada directamente hacia nuestro GND, lo que genera un estado lógico de 0 por defeecto

En el ESP32 Las terminales 34 a 39 no tienen resistencia PULL UP o PULL DOWN y solo pueden ser utilizadas como entradas. El resto de terminales digitales pueden ser  utilizadas tanto como entradas o salidas, y cuentan con una resistencia PULL UP y otra PULL DOWN que pueden ser habilitadas por medio de código.