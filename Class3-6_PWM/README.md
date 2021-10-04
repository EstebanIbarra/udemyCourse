            ===>    PULSE WIDTH MODULATION    <===

Es una señal digital con las siguientes características:

    -> Solo puede tener los valores 1 o 0
    -> Estos valores pueden tener una duración distinta
    -> La suma de los tiempos en 1 y en 0 es el periodo (inverso de la frecuencia T = 1/f)

Al tiempo en 1 se le conoce como ciclo útil (duty cycle) y se expresa en porcentajes. El ESP32 recibe esta información como un valor entre 0 y n, siendo n dependiente de la resolución asignada, por ejemplo en una resolución de 10 bits el valor de n sería de 1024, lo que correspondería a un ciclo útil del 100%.

La resolución del ciclo útil en el ESP32 puede ser entre 1 a 16 bits