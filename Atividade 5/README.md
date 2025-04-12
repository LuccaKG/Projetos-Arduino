# Atividade 5 - USART e Varredura

O desafio proposto nesta atividade é a construção de um programa que utiliza a **USART** para transmitir
uma mensagem ao computador de acordo com o estado de um botão conectado ao terminal PB1 do
microcontrolador.

O monitoramento do botão e a verificação do término da transmissão de cada caractere
devem ser implementados utilizando a estratégia de varredura.

O botão deve ser monitorado todo o tempo e, se ele não está pressionado, deve ser enviada a seguinte
mensagem para o computador:

<pre>char msg_np[] = "Transmissao serial utilizando a USART: verificacao do termino da transmissao
por varredura. O botao nao esta pressionado.\n\n";</pre>

Cada vez que a sequência inteira de caracteres da mensagem for transmitida, o programa deve
aguardar 500ms (use a função *_delay_ms()* para produzir a espera) antes de enviar a próxima mensagem.

Se o botão estiver pressionado, a seguinte mensagem deve ser enviada:

<pre>char msg_p[] = "Transmissao serial utilizando a USART: verificacao do termino da transmissao
por varredura. O botao esta pressionado.\n\n";</pre> 

Utilize o *resistor de pullup* interno da porta no acionamento do botão, como feito na Atividade 3.

Não é necessário filtrar o transitório do botão. As configurações da USART são as mesmas discutidas na atividade
discutida em aula. 

O fluxograma a seguir sumariza o funcionamento do sistema:

![image](https://github.com/user-attachments/assets/eee97875-6445-491c-b851-baa31b6a88a8)

**Importante:**
1) Antes de enviar o próximo caractere da mensagem, é necessário verificar se a transmissão do
caractere atual já terminou, o que deve ser feito através da estratégia de varredura (polling).
Consulte o manual para entender como funciona o mecanismo de sinalização do evento de término
de transmissão. Há mais de uma opção, escolha a que julgar conveniente.  
Não é permitido o uso de atrasos para aguardar o término da transmissão de cada caractere individualmente.

2) As strings das mensagens (msg_np e msg_p) são terminadas com o caractere especial '\0'. Esse
caractere pode ser usado para identificar o final da mensagem.




  
