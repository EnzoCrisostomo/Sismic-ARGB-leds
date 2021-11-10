# Projeto final Sismic
Programa para a launchpad MSP430F5529 da texas instruments que usa SPI para enviar os dados para uma fita ARGB ws2812b.

## Tópicos abordados
- Timers
- UART
- SPI
- I²c (LCD)
- GPIO
- UCS

#### Conexão dos pinos
- P3.0 -> Fita led ws2812b
- P4.1 -> SDA (LCD)
- P4.2 -> SCL (LCD)

#### Configuração UART
- Console utilizado -> PuTTY
- Baudrate -> 57600
- 8 bits
- sem paridade
- 1 stop

## Lista de comandos terminal UART

#### TODOS os argumentos são opcionais. Exemplos de comandos possíveis:
~~~
rainbow -s 25

breathing -tl 140 -bl 30

off

pulse

ajuda

ajuda colorcycle
~~~

### Ajuda
~~~
ajuda [modo]
~~~
Lista todos comandos ou argumentos caso especifique um modo.
- -modo -> Modo para descrever.

### Mudar Cor
~~~
color [index] [r] [g] [b]
~~~
Muda uma das 5 cores disponíveis.
- -index -> Cor a ser alterada (0 - 4).
- -r -> Valor da cor vermelho (0 - 255).
- -g -> Valor da cor verde (0 - 255).
- -b -> Valor da cor azul (0 - 255).

### Mudar velocidade
~~~
speed [0 - 100]
~~~
Altera a velocidade de vezes que a fita atualiza por segundo.

### Desligar leds
~~~
off
~~~
Desliga todos os leds.

### Modo Single Color
~~~
singlecolor
~~~
Cor primária sólida em todos leds.

### Modo Color Cycle
~~~
colorcycle -s [0-50] -d [0-1]
~~~
Alterna entre as cores do arco-íris.
- -s -> Altera o tamanho do passo entre as cores.
- -d -> Altera a direção da troca de cores.

### Modo Rainbow
~~~
rainbow -s [0-50] -d [0-1]
~~~
Alterna entre as cores do arco-íris led por led.
- -s -> Altera o tamanho do passo entre as cores.
- -d -> Altera a direção da troca de cores.

### Modo Breathing
~~~
breathing -s [0-255] -tl [0-255] -bl [0-255]
~~~
Aumenta e diminui periodicamente o brilho de todos leds.
- -s -> Altera a velocidade do efeito.
- -tl -> Altera o brilho máximo.
- -bl -> Altera o brilho mínimo.

### Modo Pulse
~~~
pulse -p [0-10] -o [0-255] -d [0-255]
~~~
Lança pulsos de luz passando cada led da fita.
- -p -> Altera quantidade de pontos.
- -o -> Altera o tamanho percorrido fora dos leds.
- -d -> Altera o rastro deixado pelo ponto.

### Modo Ping Pong
~~~
pingpong
~~~
Lança dois pulsos, um de cada lado que ao se encontrarem misturam as cores.
