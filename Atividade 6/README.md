# Atividade 6 - UART Tx/Rx

O desafio proposto nesta atividade é desenvolver um programa que utilize a **UART** para criar um sistema
de acionamento de um conjunto de 3 LEDs a partir de comandos fornecidos pelo teclado do computador ao
qual a placa de desenvolvimento está conectada. 

Além disso, também vamos utilizar o transmissor da
UART para encaminhar mensagens de confirmação e status.

## Recepção
A interrupção do tipo “recepção completa” será explorada nesta atividade para que o microcontrolador
ATMega328P receba comandos de controle através do teclado do computador. Uma vez que o
microcontrolador pode estar realizando operações que consomem tempo enquanto novos comandos são
enviados pelo usuário, vamos utilizar a estrutura de um buffer circular para armazenar temporariamente a
sequência de comandos recebidos. 

O processamento dos comandos, então, se dará a partir do primeiro
elemento da fila (ou seja, do comando mais antigo armazenado no buffer).

Por simplicidade, vamos
trabalhar com um buffer circular relativamente pequeno, de tamanho igual a 5 (cinco).

A tabela abaixo
mostra os possíveis comandos e os respectivos efeitos sobre o conjunto de LEDs:

![image](https://github.com/user-attachments/assets/4dd75b7b-7082-4bbf-a1ed-1a69b709d5f6)

A rotina de serviço associada à recepção deve, portanto, inserir cada novo comando na próxima posição
livre do buffer circular, caso haja alguma.

Com isso, sempre que houver comandos disponíveis no buffer circular, o programa deve processar o
comando mais antigo. Além, disso, ao executar um comando, o sistema deve enviar uma mensagem de
confirmação pela porta serial, uma única vez, imediatamente após o início da execução. 

Se o comando não for válido, ou seja, se o caractere recebido for diferente de 0, 1 ou 2, o programa deve encaminhar uma
mensagem de erro e permanecer repetindo o último comando válido. 

A tabela abaixo apresenta as mensagens referentes a cada comando recebido:

![image](https://github.com/user-attachments/assets/a2512cae-3150-4cd2-a0df-cb8655c96b28)

## Transmissão
Conforme já descrito, cada vez que um comando for processado, uma mensagem de texto deve ser
retornada pela porta serial, segundo a tabela já apresentada.

Para enviar cada mensagem, *i.e.*, cada sequência de caracteres, vamos novamente utilizar o mecanismo de interrupção.

No caso da transmissão,
há duas opções de eventos que disparam interrupções: 

(1) “transmissão completa”

ou

(2) “buffer de transmissão vazio”.


Além disso, sempre que não houver comandos a processar no buffer circular (ou seja, o buffer está vazio), a
seguinte mensagem "Vazio!\n" deve ser enviada a cada 500ms.

**Cuidado:**
* A interrupção associada ao evento “buffer de transmissão vazio” é disparada continuamente caso o
buffer esteja limpo. Sendo assim, é preciso desligar esta interrupção ao terminar de transmitir uma
sequência completa de caracteres; caso contrário, uma nova interrupção será gerada mesmo se não
quisermos transmitir outra mensagem.

**Lembrete:** caso as rotinas de serviço de interrupção tenham de acessar ou modificar conteúdos de variáveis
do programa principal, é importante declará-las como variáveis globais com o qualificador volatile para
assegurar o correto funcionamento.

## Especificações da USART:
* Velocidade de transmissão normal (*i.e.*, modo double-speed desativado);
* Modo de transmissão multi-processador desabilitado;
* Número de bits de dados por frame igual a 8;
* Modo assíncrono de funcionamento da USART;
* Sem bits de paridade;
* Uso de um bit de parada;
* *Baud rate* igual a 19.200 bps.

## Circuito

Por convenção, vamos empregar os terminais PC3, PC4 e PC5 do microcontrolador para acionar os LEDs. A
montagem pode ser acessada no link: https://www.tinkercad.com/things/hJmsrahXvPS.

![image](https://github.com/user-attachments/assets/09d77f66-7be5-4c25-88d1-2ace5182b1ac)


