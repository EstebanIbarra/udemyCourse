            ===>    INTERRUPCIONES DEL TIMER    <===

El ESP32 cuenta con 4 timers con las siguientes características:

    -> Resolución de 64 bits con un valor máximo de 2^64
    -> Prescaler de 16 bits con un valor máximo de 2^16
    -> Frecuencia de 80 MHz

Al tener una frecuencia de 80MHz, cada tick del contador es de 12.5ns.
Para dividir la frecuencia del contador y así aumentar el tiempo entre cada tick se utiliza el prescaler