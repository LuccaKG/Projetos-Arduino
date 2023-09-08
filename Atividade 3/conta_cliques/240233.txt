#include <util/delay.h>
#define F_CPU 16000000UL

/* Declarando ponteiros para PORTC */
unsigned char *p_portc;
unsigned char *p_ddrc;
unsigned char *p_pinc;
/* Note que *p_pinc é declarado, pois mais adiante definiremos PORTC como ENTRADA e leremos seu valor */

/* Declarando ponteiro para MCU */
unsigned char *p_mcucr;

/* Declarando ponteiros para PORTD */
unsigned char *p_portd;
unsigned char *p_ddrd;
/* Note que *p_pind não é declarado, pois mais adiante definiremos PORTD como SAÍDA */

/* Declarando e definindo variável que armazena o estado do botão */
unsigned char botao = 0;

/* Declarando e definindo variável que armazena o estado do display, ou seja, o numero que ele está representando no momento */
unsigned char num_atual = 0;

/* Inicializando os periféricos do sistema*/
/* Os ponteiros anteriormente declarados armazenam endereços definidos no manual do ATMEGA 328P*/
void setup(void) {

    /* MCUCR */
    p_mcucr = (unsigned char *) 0x55;

    /* PORTC */
    p_ddrc = (unsigned char *) 0x27;
    p_portc = (unsigned char *) 0x28;
    p_pinc = (unsigned char *) 0x26;

    /* PORTD */
    p_ddrd = (unsigned char *) 0x2A;
    p_portd = (unsigned char *) 0x2B;
      
    /* Configurando PC0 (pino A0 na placa) */
    *p_ddrc &= 0XFE; // Operação AND bit a bit entre ddrc e 0xFE (1111 1110) para 'setar' PC0 como ENTRADA, i.é., LSB = 0
    *p_mcucr &= 0xEF; // Operação AND bit a bit no reg MCUCR para habilitar o resistor pull up, i.é., setar o bit 4 de reg mcucr para zero
    *p_portc |= 0x01; // Operação OR bit a bit com o portc para ativar o resistor pull up setando LSB = 1
    // Note que para ativar o resistor de pull up, precisamos de uma combinação nas configs de MCUCR e PORTC. Por isso utilizamos PORTC mesmo setando PC0 como ENTRADA

    /* Configurando PORTD */
    *p_ddrd = 0xFF; // atribuir ao ddrd o valor 0xFF (1111 1111) para que todos os pinos (PD0~PD7) funcionem como SAÍDA
    // Cada pino PD0~PD7 vai controlar individualmente um LED do Display (DP + 7 segmentos)

}

/* Cada segmento do display é associado a uma letra variando de 'a' a 'g', além de DP (ponto decimal) de forma que tal convenção é facilmente encontradas em fotos
na Internet. Os segmentos serão controlados a partir dos pinos PD de forma que:

PD0 = DP
PD1 = a
PD2 = b
PD3 = c
PD4 = d
PD5 = e
PD6 = f
PD7 = g

A partir de diferentes combinações de segmentos acesos, obtemos o formato de um número ou letra. Nosso intuito é representar números em hexadecimal, i.é, variando de 0 a 9 
e de A a F, totalizando 16 representações. 
*/  

/* 
A função abaixo faz a varredura e atualização do estado do botão. O estado do botão é armazenado na variável 'botao' declarada anteriormente; enquanto isso, a variável
*p_pinc funciona como um 'listener' capturando em tempo real o estado físico do botão. Quando botao != (*p_pinc & 1), significa que o estado lido no momento é diferente do armazenado
anteriormente, ou seja, houve uma mudança de estado (de pressionado pra solto ou vice-versa). Uma vez identificada essa mudança, precisamos assegurar que ela aconteceu de fato e não
é fruto de uma leitura errada por conta de ruídos do transitório. Por conta disso implementamos um delay de 1ms e, após isso, verificamos se a mudança de estado notada anteriormente
continua válida; se sim, atualizamos o estado do botão.
Uma vez atualizado o estado do botão, verificamos se o novo estado tem valor = 0; se tiver, indica que o botão foi pressionado, o que exige que a contagem de cliques seja atualizada
e o próximo numero seja representado (por isso atualizamos a variavel num_atual). Por fim, é importante que verifiquemos o valor da variável num_atual; se ela for diferente de 15,
podemos simplesmente somar 1 e representar o número sucessor, mas se ela for 15, estaremos no último número representável e a sequência deve recomeçar em 0.
Note que isso tudo pode ser descrito por uma máquina de estados em que cada num_atual aponta para seu sucessor, porém se num_atual = 15, ele aponta para o início da sequência.
*/
void botao_listener() {
  if (botao != (*p_pinc & 0x01)) {
    _delay_ms(1);
  if (botao != (*p_pinc & 0x01)) {
    botao = *p_pinc & 0x01;
  if (botao == 0x00) // botao == 0 indica que foi pressionado, então precisamos avançar para o próximo estado
  {
  if (num_atual != 15) // condição necessária para indicar que num_atual == 15 aponta como sucessor o reinicio da sequencia, a partir de 0
  {
    num_atual++;
  }
  else
  {
    num_atual = 0;
  }
  }
  }
  }
}

/* Abaixo, definiremos como cada número hexadecimal será representado, ou seja, quais segmentos serão ativados para tal representação. Note que, como nosso display é de
configuração ANODO-COMUM, a lógica de ativação dos segmentos é inversa à de nivel lógico tradicional, logo um segmento é ativado/aceso quando o pino que o representa recebe valor 0. 
Por exemplo: para representar o número 8, queremos que todos os segmentos estejam acesos. Então precisaremos fornecer aos bits do reg. PORTD o valor 0x01, pois assim asseguramos que 
TODOS os segmentos receberão valor 0, mas o ponto DP (LSB) receberá valor 1 e, inicialmente, estará desligado. */

void represent_display () {

  switch (num_atual){
    case 0:
      *p_portd = 0x81; // segmentos a,b,c,d,e,f ligados
      break;

    case 1:
    *p_portd = 0xF3; // segmentos b,c ligados
    break;
    
    case 2:
    *p_portd = 0x49; // segmentos a, b, d, e, g ligados
    break;

    case 3:
    *p_portd = 0x61; // segmentos a, b, c, d, g ligados
    break;

    case 4:
    *p_portd = 0x33; // segmentos b, c, f, g ligados
    break;

    case 5:
    *p_portd = 0x25; // segmentos a, f, g, c, d ligados
    break;

    case 6:
    *p_portd = 0x05; // segmentos a, f, g, c, d, e ligados
    break;

    case 7:
    *p_portd = 0xF1; // segmentos a, b, c ligados
    break;

    case 8:
    *p_portd = 0x01; // segmentos a, b, c, d, e, f, g ligados
    break;

    case 9:
    *p_portd = 0x21; // segmentos a, b, c, d, f, g ligados
    break;

    case 10:
    *p_portd = 0x11; // 10=A, segmentos a, b, c, e, f, g ligados   
    break;
    
    case 11:
    *p_portd = 0x07; // 11=b, segmentos f, g, c, d, e ligados
    break;

    case 12:
    *p_portd = 0x8D; // 12=C, segmentos a, f, e, d ligados 
    break;

    case 13:
    *p_portd = 0x43; // 13=d, segmentos b, c, d, e, g ligados
    break;

    case 14:
    *p_portd = 0x0D; // 14=E, segmentos a, d, e, f, g ligados 
    break;

    case 15:
    *p_portd = 0x1D; // 15=F, segmentos a, e, f, g ligados 
    break;
      }
}

/* Configuração de DP */
/* Para acender DP sempre que o botão for pressionado, precisamos fazer constantemente a verificação do estado do botão. 
Essa função verifica o estado do botão e, se ele estiver pressionado (nível lógico = 0), ativa o DP enviando o valor 0 para PD0 através
de uma operação AND bit a bit. */
void atualiza_dp()
{
 if (botao == 0x00)
 {
 *p_portd &= 0xFE; // 1111 1110
 }
}


int main() {

  setup(); // configura o setup 

  /* Loop infinito */
  while (1)
  {
  botao_listener(); // atualiza o estado do botao levando em consideração o período transitório

  atualiza_dp(); // Ponto acende toda vez que o botão é acionado

  represent_display(); // representa o número no display
  }
  return 0;
}
