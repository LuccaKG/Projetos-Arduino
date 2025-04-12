# Atividade 3 - GPIOs

O desafio proposto nesta atividade é desenvolver um programa que faça com que o microcontrolador
ATMega328P conte quantas vezes um botão táctil (push-button) foi pressionado, exibindo o valor atual da
contagem em um display de 7 segmentos.

Além disso, enquanto o botão táctil estiver pressionado, o ponto
decimal do display deve permanecer aceso, caso contrário deve permanecer apagado. Por simplicidade,
vamos exibir somente um dígito, de maneira que a contagem estará limitada ao intervalo de 0 a F
(hexadecimal). Ou seja, caso a contagem ultrapasse o valor F, ela deve ser reiniciada a partir de 0.
Para evitar comportamentos indesejados do sistema, utilize a técnica de *debounce*, via software.

O display de 7 segmentos (anodo comum) pode ser acionado por um microcontrolador utilizando-se GPIOs
distintas para acender ou apagar cada um dos seus segmentos. Nesse caso, considerando o ponto decimal,
são necessárias oito GPIOs. 

Notem que, como estamos usando um display do tipo anodo comum, os
segmentos se acendem com nível zero e apagam com nível 1 (lógica invertida).


**Importante:** Por convenção, vamos empregar os pinos 0 a 7 da porta D (ou seja, PD0 a PD7, indicados no
diagrama de pinos do Arduino UNO, disponibilizado na página do curso) para acionar os segmentos de *a*
a *g* do display, incluindo o ponto decimal *dp*. 

O botão, por sua vez, deverá ser conectado ao pino 0 da porta
C (PC0). 

O link para o template do Tinkercad, mostrado na figura abaixo, está disponível em:
https://www.tinkercad.com/things/liSNWjJikD4

![image](https://github.com/user-attachments/assets/3e1403d8-fe79-4188-9c14-545b94dbb42a)
