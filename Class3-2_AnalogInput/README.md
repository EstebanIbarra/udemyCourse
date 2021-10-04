            ===>    ENTRADAS ANALÓGICAS    <===

Una señal analógica es aquella que toma valores continuos. Puede asumir una infinidad de valores.

Para transformar esta señal analógica en una digital con la que podamos interactuar por medio de código se utiliza un convertidor Análogo-Digital (ADC). Este convertidor se encarga de discretizar la señal, es decir, tomar muestras periódicas del valor de la señal analógica en un momento dado para registrarla dentro de una señal digital, y de esta discretización depende la resolución de nuestra imagen digital.

Todos los DACs del ESP32 tienen las siguientes características:

    -> Resolución máxima    12 bits ~=  4096 posibles combinaciones ~=  805.86µV de resolución
    -> Resolución mínima    9 bits  ~=  512 posibles combinaciones  ~=  6.45mV de resolución

