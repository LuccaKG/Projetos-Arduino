/* Bibliotecas */
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* CONFIGURAÇÕES GPIOS , UART E TEMPORIZADOR */

/* PONTEIROS PARA OS REGS RELACIONADOS AO TEMPORIZADOR */

unsigned char *p_tccr0a; // Timer/Counter Control Register A
unsigned char *p_tccr0b; // Timer/Counter Control Register B
unsigned char *p_timsk0; // Timer/Counter Interrupt Mask Register
unsigned char *p_ocr0a;  // Output Compare Register A

/* PONTEIROS PARA OS REGS DO PORTB, ONDE OS LEDS ESTAO - PB4 E PB5 */

unsigned char *p_portb;
unsigned char *p_ddrb;

/* PONTEIROS PARA OS REGS RELACIONADOS A OPERAÇÃO DA UART */

unsigned char *p_ubrr0H; // baud rate (bits mais significagivos)
unsigned char *p_ubrr0L; // baud rate (bits menos significativos)

// configuração e status
unsigned char *p_ucsr0A;
unsigned char *p_ucsr0B; 
unsigned char *p_ucsr0C; 

unsigned char *p_udr0;  // reg de dados

/* INICIALIZAÇÕES DOS PERIFÉRICOS ENVOLVIDOS NO SISTEMA*/

/* REGS PROGRAMADOS CONFORME ENDEREÇO DO DATASHEET DO ATMEGA */

void configuracoes_iniciais(void)
{
  cli(); // desabilita as interrupções no inicio 

  /* TEMPORIZADOR */

  p_tccr0a = (unsigned char *)0x44;
  p_tccr0b = (unsigned char *)0x45;
  p_timsk0 = (unsigned char *)0x6E;
  p_ocr0a = (unsigned char *)0x47;

  /* O temporizador requer algumas configurações que podem variar de acordo com a aplicação. No caso de uma interrupção associada ao valor armazenado em OCIE0A,
  a seguinte fórmula é seguida: T = (OCR0A + 1) * P/Fclk. Na execução dessa atividade, era necessário estabelecer estrategicamente uma base única de tempo (T) 
  que fosse um divisor inteiro de 500 ms (período para o qual o LED incorporado deve piscar) e 780 ms (período para o qual o LED externo deve piscar), respeitando
  o limite de T do temporizador, que é de 16,34 ms. O prescaler foi definido como um dos seguintes valores: P = [1, 8, 64, 256, 1024]. OCR0A é um valor inteiro de 
  8 bits (ou seja, representa 256 valores), e Fclk representa a frequência do processador = 16MHz. Como discutido em aula, T = 1 ms resulta em OCR0A = 249, com um 
  prescaler de 64, o que atende bem às especificações detalhadas acima.  */

  /* TCCR0B */
  /* WGM02 = 0, modo de operação CTC */
  /* CS02 = 0, CS01 = 1, CS00 = 1,  prescaler de 64 (é o fator de divisao) */
  /* FOC0A FOC0B – – WGM02 CS02 CS01 CS00
       0     0   - -   0     0    1    1 */
  *p_tccr0b = 3;

  /* TIMSK0 */
  /* OCIE0B = 0, OCIE0A = 1, TOIE0 = 0, para habilitar a interrupção
     associada ao OCIE0A, isto é, gera interrupção associada ao
     temporizador toda vez que o valor armazenado em OCIE0A bater com
     o valor de contagem. */
  /* – – – – – OCIE0B OCIE0A TOIE0
     - - - - -   0      1      0 */
  *p_timsk0 = 2;

  /* OCR0A */

  *p_ocr0a = 0xF9; //249 quando T = 1 ms

  /* TCCR0A */
  /*  COM0A1 = 0, COM0A0 = 0, COM0B1 = 0, COM0B0 = 0, para modo de
      operação normal, com OC0A e OC0B desconectados. */
  /* WGM01 = 1 e WGM00 = 0, devido ao modo de operação CTC*/
  /* COM0A1 COM0A0 COM0B1 COM0B0 – – WGM01 WGM00
       0      0      0      0    - -   1     0  */
  *p_tccr0a = 2;

  /* PORTB */
  p_ddrb = (unsigned char *)0x24;
  p_portb = (unsigned char *)0x25;

  /* LEDS */

  *p_ddrb |= 0x30;    // funcionando como saída nos bits 4 (led externo) e 5 (led interno)
  *p_portb &= ~(0x30); // começa com os leds apagados

  /* UART */
  p_ubrr0H = (unsigned char *)0xC5;
  p_ubrr0L = (unsigned char *)0xC4;
  p_ucsr0A = (unsigned char *)0xC0;
  p_ucsr0B = (unsigned char *)0xC1;
  p_ucsr0C = (unsigned char *)0xC2;
  p_udr0 = (unsigned char *)0xC6;

  /* UART */

  /* 12 bits (UBRR0[11:0]) para selecionar o baud rate da transmissão de 19.200*/
  *p_ubrr0H = 0;
  *p_ubrr0L = 0X33; // 51 (decimal) = 0x33 = 110011

  /* UCSRnA
     U2Xn = 0 para transmissão com velocidade normal, e
     MPCMn = 0 para desabilitar o multiprocessador. */
  /* RXCn TXCn UDREn FEn DORn UPEn U2Xn MPCMn
       X    X    X     X   X     X   0    0    */
  *p_ucsr0A = 0;

  /* UCSRnB
     RXCIEn = 0 para desabilitar a interrupção associada a recepção;
     TXCIEn = 0 para desabilitar a interrupção associada a transmissão completa;
     UDRIEn = 0 para desabilitar a interrupção associada ao reg dados vazio e habilitar apenas na função principal;
     RXENn = 0 e TXENn = 1 para desabilitar a recepção e habilitar a transmissão, e
     UCSZn2 = 0 para que o n° de bits por frame seja 8. */
  /* RXCIEn TXCIEn UDRIEn RXENn TXENn UCSZn2 RXB8n TXB8n
       0        0    0      0     1     0      X     X    */
  *p_ucsr0B = 0x8;

  /* UCSRnC
     UMSELn1 = 0 e UMSELn0 = 0, para UART operando em modo assíncrono;
     UPMn1 = 0 e UPMn0 = 0, para modo de paridade desabilitado;
     USBSn = 0 para um único bit de parada, e
     UCSZn1 = 1 e UCSZn0 = 1 para que o n° de bits por frame seja 8.
  /* UMSELn1 UMSELn0 UPMn1 UPMn0 USBSn UCSZn1 UCSZn0 UCPOLn
        0       0      0     0     0     1      1      0    */
  *p_ucsr0C = 0x6;

  sei(); // habilita as interrupções, após concluir as configurações de sistema
}

/* MENSAGEM  PRINTADA */

char msg[] = "Atividade 7 - Interrupcoes temporizadas tratam concorrencia entre tarefas! \n\n";

/* Mapeia a mensagem a ser enviada pela UART */
volatile int i = 0;

/* ROTINA DE SERVIÇO DE INTERRUPÇÃO DE TRANSMISSÃO ASSOCIADA AO REGISTRADOR DE DADOS VAZIO */

ISR(USART_UDRE_vect)
{ /* 
Caso, na mensagem em análise, a posição "i" seja distinta do caractere especial indicador de fim de cadeia de caracteres \0, 
o caractere é passado para o registrador de dados para transmissão e exibição no monitor serial. Após essa operação, 
incrementa i (posição) */
  msg[i] != '\0' ? (*p_udr0 = msg[i], i++) : (i = 0, *p_ucsr0B &= ~(0x20));
}

/* VARIÁVEIS CONTADORAS E DE ESTADO */

volatile unsigned int contador_1 = 0; // contador de interrupções - led incorporado
volatile unsigned int contador_2 = 0; // contador de interrupções - led externo
volatile unsigned char status_1 = 0; // estado do led incorporado
volatile unsigned char status_2 = 0; // estado do led externo

/* ROTINA DE SERVIÇO DE INTERRUPÇÃO DO TEMPORIZADOR */

/* Sempre que o valor da contagem se iguala ao do atribuído ao OCR0A, a int. é acionada */

/* Para ativar ou desativar os LEDs no momento adequado, iremos analisar quantas vezes a Rotina de Serviço de Interrupção (ISR) 
foi acionada, ou seja, qual é o valor atual das variáveis contadoras. No caso do LED incorporado, é necessário verificar se a 
contagem atingiu 500. Se isso ocorrer, a contadora é resetada para zero, iniciando uma nova contagem. O LED é aceso se estiver 
apagado e apagado se estiver aceso. O mesmo princípio se aplica ao LED externo, mas nesse caso, a contagem vai até 780.*/

ISR(TIMER0_COMPA_vect)
{
  /* são 500 interrupções para o led incorporado */
  // contadores do num. de interrupções para cada led
  contador_1++; 
  contador_2++;
  if (contador_1 >= 500) // verifica se ocorreram pelo menos 500 interrupções
  {
    contador_1 = 0;           // zera a o contador
    status_1 = status_1 ? 0 : 1;  // inverte o registro do nivel logico do led 
    *p_portb ^= (1 << 5);  // inverte o nivel logico do led
  }
  /* precisamos contar 780 interrupções para o led externo */
  if (contador_2 >= 780)
  {
    contador_2 = 0;
    status_2 = status_2 ? 0 : 1;
    *p_portb ^= (1 << 4);
  }
}

int main(void)
{
  configuracoes_iniciais();
  while (true)
  {
    *p_ucsr0B |= 0x20; // habilita a interrupção da UART para transmitir a mensagem
    _delay_ms(5000);   // intervalo entre cada mensagem
  }
  return 0;
}
