#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

/* Configurando USART */

/* Definindo ponteiros para os registradores cujos bits
serao concatenados para definir o baud rate*/ 
unsigned char *p_ubrr0H; 
unsigned char *p_ubrr0L; 

/* Definindo ponteiros para os registradores de controle e status */
unsigned char *p_ucsr0A; 
unsigned char *p_ucsr0B; 
unsigned char *p_ucsr0C; 

/* Definindo ponteiro para registrador de dados */
unsigned char *p_udr0; 

/* Ponteiro para MCUCR, util para utilização do resistor de pullup */

unsigned char *p_mcucr;

/* Ponteiros para controle dos pinos de PortB, analogo
ao que ja fizemos nos labs passados para PortB, PortC e PortD*/

unsigned char *p_portb;
unsigned char *p_ddrb;
unsigned char *p_pinb;

/* Declarando variável que armazena estado do botão, semelhante ao
que fizemos anteriormente também */

unsigned char botao = 0;

/* Mensagens que serao apresentadas no monitor (fornecidas
no enunciado). As mensagens sao do tipo vetor de caracteres*/

char msg_p[] = "Transmissao serial utilizando a USART: verificacao do termino da transmissao por varredura. O botao esta pressionado.\n\n";
char msg_np[] = "Transmissao serial utilizando a USART: verificacao do termino da transmissao por varredura. O botao nao esta pressionado.\n\n";


/* Configurações iniciais definidas */

void configuracoes_iniciais()
{
  // Atribuindo endereços conforme manual do ATMEGA (e notas de aula)
  /* MCUCR */
  p_mcucr = (unsigned char *) 0x55;
  
  /* PORTB */
  p_ddrb = (unsigned char *) 0x24;
  p_portb = (unsigned char *) 0x25;
  p_pinb = (unsigned char *) 0x23;
  
  /*USART*/  
  p_ubrr0H = (unsigned char *) 0xC5;
  p_ubrr0L = (unsigned char *) 0xC4;
  p_ucsr0A = (unsigned char *) 0xC0;
  p_ucsr0B = (unsigned char *) 0xC1;
  p_ucsr0C = (unsigned char *) 0xC2;
  p_udr0 = (unsigned char *) 0xC6;
  
  /* Configurações para o PORTB e ativação do resistor de pullup */   
  *p_ddrb &= 0XFD; // AND bitwise para forçar valor 0 no pino 9, configurando-o como entrada
  *p_mcucr &= 0xEF; // zerando bit 4 de mcucr atraves de AND bitwise para configurar resistor de pullup
  *p_portb |= 2; // setando valor 1 no bit 2 de PORTB para completar a configuração do resistor de pullup, conforme feito em lab anterior


  /* Utilizaremos 12 bits (UBRR0[11:0]) dos 16 disponíveis para setar o baud rate da transmissão */
  *p_ubrr0H = 0x00; // 8 bits mais significativos da concatenação setados em 0
  *p_ubrr0L = 0x08; // utiliza os 8 bits menos significativos para representar o valor 8, que resultara no baud rate que temos trabalhado em aula (baud rate ideal de 115200, mas real de 111111.111) 


  *p_ucsr0A = 0x00; // desabilita U2X0 (double speed) e MPCM0 (modo de transmissão multi-processador)
  *p_ucsr0B = 0x08; // desativa o receptor (bit 4 = RXEN0 = 0) e ativa o transmissor (bit 3 = TXEN0 = 1)
  
  /* Bits 7 e 6 (UMSEL01 e UMSEL00) = 0b00 para configurar USART Asynch. 
  Bits 5 e 4 = 0b10 conforme tabela para declaração de paridade par. 
  Bit 3 = 0b1 para declaraçao de 2 bits de parada.
  Bits 2 e 1 = 0b11 para definir 8 bits por frame.
  Bit 0 = 0b0 para transmissão assincrona (UCPOL0 = 0)*/
  *p_ucsr0C =  0x2E; 
}  
  
/* Impressao das frases no monitor serial */

void frase(char *msg) // função receberá parâmetro char * (ponteiro para variavel char) para uma cadeia de caracteres
{  
  int i = 0;
  while(msg[i] != '\0') // percorre a cadeia de caracteres até encontrar o \0 que sinaliza o final
  {  
    while((*p_ucsr0A & 40) == 0); // Verifica valor lido em TXC0. Se TXC0 = 0, a condição é verdadeira e o programa entende que ainda existem dados a serem
    // transmitidos    
    *p_udr0 = msg[i]; // escreve o caracter do indice i da frase no reg. de dados udr0, para que ele seja transferido para o buffer de transmissão
    i++; // acumula valores em i para avançar para o proximo caractere na proxima iteração
    
    // note que quando TXC0 = 1, o while é interrompido indicando que não há mais dados a serem transmitidos
   } 
}
  
/* Função que verifica mudanças no estado do botão (como um listener) e imprime a frase adequada */

void verifica_botao()
{
  botao = (*p_pinb & 2); // monitora se o botão foi pressionado, analogo ao q fizemos no lab 3
  if (botao == 0) // botão pressionado
  {
    frase(msg_p); // imprime frase definida globalmente indicando botao pressionado
  } 
  else // botão nao pressionado
  {
    frase(msg_np); // imprime frase definida globalmente indicando botao nao pressionado
  }   
}
  
  
int main(void)
{  
  configuracoes_iniciais(); // carrega configs iniciais de endereços etc
    
  while (1) // loop infinito

  {

    verifica_botao(); // listener para o estado do botão e impressao da frase correspondente

    _delay_ms(500); // delay entre iterações definido no enunciado
  }

  return 0;
}