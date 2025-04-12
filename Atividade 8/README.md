# Atividade 8 - PWM

Um servo motor é um tipo de motor cujo ângulo do eixo pode ser controlado e mantido com precisão.

Alguns modelos de servos são controlados por um sinal PWM: a largura do pulso (intervalo em que o sinal
permanece em nível alto) de um sinal com uma frequência específica determina a posição do eixo.

As especificações do sinal de controle do micro servo SG90, ilustrado na Figura 1, são mostradas na Tabela 1:

![image](https://github.com/user-attachments/assets/2f61095f-7948-469a-900f-5a35927d433c)

O fio marrom corresponde ao terra (GND), o vermelho ao 5V (VCC) e o laranja recebe o sinal de controle
(PWM). O acionamento do servo deve ser feito através do canal B do Temporizador 1. 

O circuito do sistema é mostrado na Figura 2 e o link para o Tinkercad é: https://www.tinkercad.com/things/0xVUQUq7TMj

![image](https://github.com/user-attachments/assets/1126e1ec-4965-4f25-a92d-0b7ebe899450)

Construa um programa que repita indefinidamente o seguinte movimento do eixo: deslocamento do
ângulo mínimo até o máximo e depois retorno até o ângulo mínimo, em passos de 30°. Ou seja, cada ciclo
do movimento deve ser composto pela seguinte sequência de ângulos: 0°, 30°, 60°, 90°, 120°, 150°, 180°,
150°, 120°, 90°, 60° e 30°. O eixo deve permanecer 800ms em cada ângulo e os ciclos devem se repetir
indefinidamente.

Para definir as configurações que são necessárias para o temporizador gerar o sinal de controle conforme
especificado, os seguintes passos podem ser seguidos:

1. Defina o modo de operação a ser usado;
2. Verifique qual é o valor de prescaler que permite a geração de um sinal PWM de 50Hz, ou seja, considerando
a configuração escolhida, qual é o máximo período que pode ser obtido para cada possível valor de
prescaler;
3. Ajuste a faixa de contagem de forma a obter exatamente um período de 20ms (50Hz);
4. Calcule a largura do pulso associada a cada ângulo do ciclo;
5. Considerando o prescaler escolhido, calcule o número de ciclos de contagem necessários para se obter cada
uma das larguras de pulso.

Com estas informações, é possível obter as configurações dos registradores necessárias para produzir o
sinal especificado.

**IMPORTANTE:** Algumas vezes, a ordem com que os registradores do temporizador são configurados pode
travar a simulação no Tinkercad. Trata-se de um problema do simulador que não tem base no manual do
microcontrolador. 

Portanto, sugiro a seguinte ordem na sequência de configuração: OCR1X, TIMSK1,
TCCR1B e TCCR1A. 

No entanto, usem o Tinkercad com cautela, pois a simulação do Temporizador 1 nem
sempre funciona bem nele. Recomendo que vocês façam o desenvolvimento diretamente placa e usem um
osciloscópio para validar as características do sinal PWM obtido.
