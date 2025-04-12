# Atividade 9 - Assembly

Faça um programa que pisca o LED presente na placa (terminal 13). O LED deve acender por 100ms e
depois permanecer apagado por 100ms, repetindo este ciclo indefinidamente, ou seja, deve piscar
ininterruptamente com uma frequência de 5Hz. 

Utilize interrupções do temporizador 0 para medir os
tempos necessários para produzir o comportamento especificado do LED. Se optar pelo modo CTC, não
use a interrupção associada ao evento de overflow.

Construa o programa e valide-o utilizando o simulador do MPLAB X. Use a ferramenta “Stopwatch”
(Window -> Debugging -> Stopwatch) do simulador, que permite que o intervalo de tempo entre dois
breakpoints seja medido com precisão. Lembre-se que o sinal de relógio do Arduino UNO é de 16 MHz e é
necessário configurar o simulador do MPLAB X com esta frequência para que as medidas de tempo
observadas durante a simulação estejam corretas. 

Após validar o funcionamento usando o simulador, teste
o programa na placa.
