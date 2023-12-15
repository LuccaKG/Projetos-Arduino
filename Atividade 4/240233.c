#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* Ponteiros para os registradores que configuram a interrupção INT1*/

unsigned char *p_eicra; 
unsigned char *p_eimsk;

/* Ponteiros para os registradores PORTB e PORTD, que serão utilizados como saída */

unsigned char *p_portb;
unsigned char *p_ddrb;

unsigned char *p_portd; // como PORTB só tem 6 pinos, utilizamos um de PORTD para representar o segmento central (g) do display
unsigned char *p_ddrd;

/* Variável que armazena o número de bordas contadas num determinado intervalo */

unsigned int num_bordas = 0;

/* Variável que armazena a frequência do motor num determinado intervalo */

unsigned int freq = 0;

/* Configurações iniciais necessárias para o sistema */

void configs_iniciais()
{
  cli(); // desabilita as interrupções inicialmente, por boas práticas
  
  /* Atribui endereços (conforme manual do ATMEGA328) para os registradores de controle para INT1*/
  
  p_eicra = (unsigned char *) 0x69; 
  p_eimsk = (unsigned char *) 0x3D;
  
  /* Atribui endereços (conforme manual do ATMEGA328) para os registradores de controle de PORTB e PORTD*/
  
  p_ddrb = (unsigned char *) 0x24; 
  p_portb = (unsigned char *) 0x25;
  
  p_ddrd = (unsigned char *) 0x2A;
  p_portd = (unsigned char*) 0x2B;
  
  /* Configura os registradores de controle para INT1. Iremos utilizar a configuração em que qualquer mudança de nivel em INT1 ativa interrupção
  ISC11 = 0 e ISC10 = 1*/
  
  *p_eicra &= 0xF7; // zera o bit ISC11 do registrador EICRA
  *p_eicra |= 0x04; // atribui valor 1 ao bit ISC10 do registrador EICRA 
  *p_eimsk |= 0x2; // atribui valor 1 ao bit_1 de EIMSK para habilitar interrupção INT1 
  
  /* PORTB e Pino_7 do PORTD funcionando como saída */
  
  *p_ddrb = 0b111111; // atribui valor 1 a todos os pinos de PORTB para funcionarem como saída
  *p_ddrd |= 0x80; // atribui valor 1 ao pino_7 de PORTD para funcionar como saída (segmento g do display)
  sei(); // habilita interrupções 
}
 
/* Função que vai ser executada quando uma interruptação for solicitada */

ISR(INT1_vect) // Declaração padrão da função para utilização de INT1
{
   num_bordas ++; // conta +1 borda para cada mudança de nivel notada em INT1, ou seja, para cada vez que a interrupção for solicitada
}

/* Frequência */

/* A frequência é determinada dividindo-se o número de voltas pelo período. Para calcular o número de voltas, 
divide-se o total de pulsos pelo número de pulsos gerados pelo encoder (49 pulsos).
Já que utilizamos o número de bordas, esse valor deve ser multiplicado por 2, uma vez que um pulso tem, obviamente, 2 bordas. 
Assim, a frequência é obtida dividindo o número de bordas por (49 x 2 = 98) e multiplicando tudo isso pelo inverso do período. */

/* Período */

/*O tempo (delay) definido para contagem das bordas é de 100 ms, conforme sugestao em aula.
No entanto, a frequência é expressa em RPM, o que requer a conversão do delay para minutos, resultando em 0.00167 min.*/

void frequencia()
{
 freq = num_bordas/(98*0.00167);
}

/* Display de 7 segmentos */

/* Mesma lógica utilizada na atividade 3, só que em vez de switch case utilizei if e else if para poder lidar com desigualdades.
Além disso, neste caso o display é do tipo CATODO-COMUM, então tem seu pino COMUM ligado ao GND e seus LEDs se acendem quando recebem
valor 1 (ao contrário do ANODO-COMUM). */

void display ()

{
  if (freq < 100) // o display sempre mostra o digito-centena da frequencia.
  {
    *p_portd &= 0x00;
    *p_portb = 0b111111; // digito 0 no display
  }
  else if (freq < 200)
  {
    *p_portd &= 0x00;
    *p_portb = 0b000110; // digito 1 no display
  }
  else if (freq < 300)
  {
   	*p_portd |= 0x80;
    *p_portb = 0b011011; // digito 2 no display
  }
  else if (freq < 400)
  {
    *p_portd |= 0x80;
   	*p_portb = 0b001111; // digito 3 no display
  }  
  else if (freq < 500)
  {
    *p_portd |= 0x80;
   	*p_portb = 0b100110; // digito 4 no display
  }
  else if (freq < 600)
  {
    *p_portd |= 0x80;
   	*p_portb = 0b101101; // digito 5 no display
  } 
  else if (freq < 700)
  {
    *p_portd |= 0x80;
   	*p_portb = 0b111101; // digito 6 no display
  }
  else if (freq < 800)
  {
    *p_portd &= 0x00;
   	*p_portb = 0b000111; // digito 7 no display
  } 
  else if (freq < 900)
  {
    *p_portd |= 0x80;
   	*p_portb = 0b111111; // digito 8 no display
  } 
  else if (freq >= 900)
  {
    *p_portd |= 0x80;
   	*p_portb = 0b101111; // digito 9 no display
  } 
}

/* main do programa */

int main(){
  
  configs_iniciais(); // carrega as configurações iniciais
  
  while(1){  // loop infinito
   
    num_bordas = 0; // o número de bordas deve ser zerado a cada iteração para não interferir no calculo da próxima frequência

    _delay_ms(100); // delay implementado para servir como intervalo para contagem de bordas (mudanças de nivel)  

    frequencia(); // frequência calculada no fim do programa

    display(); // o digito-centena é representado no display de acordo com a frequencia calculada
  }
  return 0;
}