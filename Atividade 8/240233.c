/* Modo utilizado = Phase Correct PWM
   Prescaler = 1024
   T = 19.968 ms

Não é possível utilizar o modo fast PWM, pois ele atinge um periodo
máximo de T_fast = (Prescaler_max/16M)*256 = (1024/16M)*256 = 16.384 ms < 20ms.
Utilizaremos então o modo Phase Correct que tem período máximo (quando OCR2A = 255) igual a
T_phase = (Prescaler_max/16M)*(2*OCR2A) = 32.640ms > 20ms.
Note que não é possivel trabalhar com um prescaler menor (256 no caso), pois assim
o requisito de T=20ms nao seria satisfeito, mas é possível fazer OCR2A = 156 -> T = 19.968 ms. */

#include <avr/io.h> // para podermos utilizar macros de endereçamento
#include <avr/interrupt.h> // para podermos utilizar interrupções

/*
Nossa base de tempo é de T = 20 ms e o enunciado pede que cada posição permaneça por 800 ms, executaremos uma ação real
a cada ciclo de 800ms/20ms = 40 interrupções.
*/
#define CICLO 40

volatile unsigned int cont = 0; // contador para registrar o número de interrupções em cada ciclo
volatile unsigned char sentido = 0; // variável "binária" para saber se o eixo está rodando em sent. horario (0) ou anti-horario (1)

void configs() {
	cli(); // desabilita interrupções globais
	OCR2A = 156; // == TOP do timer
	TIMSK2 = 1; // para disparar int. de overflow 

	/* 
	TCCR2B – Timer/Counter Control Register B
	FOC2A FOC2B – – WGM22 CS22 CS21 CS20
	  0     0   - -   1     1   1    1

	  WGM22 = 1 -> ativando Waveform Generation Mode
	  CS22 = CS21 = CS20 = 1 -> Prescaler = 1024

	*/
	TCCR2B = 0x0F; // 0000 1111

	/* 
	TCCR2A – Timer/Counter Control Register A
	COM2A1 COM2A0 COM2B1 COM2B0 – – WGM21 WGM20
	  0      0      1      0    - -   0     1

	  COM2B1, COM2B0 = 1, 0 -> Zerar OC2A na correspondência durante a contagem ascendente. Habilitar OC2A na correspondência durante a contagem descendente.
	  WGM21, WGM20 = 0, 1   -> 'PWM, Phase Correct' com TOP limitado por OCRA
	*/
	TCCR2A = 0x21; // 0010 0001

	/* 
	Para calcular a largura do pulso, temos que L = (2*OCR2B)*(P/16M)
	Assim, precisamos calcular L para cada angulo. Vamos começar pelos extremos:

	0º   -> 0.5 ms -> OCR2B = 4
	180º -> 2.5 ms -> OCR2B = 20

	Temos entao uma faixa de 17 valores possíveis para OCR2B. Para descobrir os valores para angulos intermediarios, utilizaremos proporção:

	30º  -> (2.5ms - 0.5ms)/6 + 0.5ms   = 0.8 ms -> OCR2B = 6
	60º  -> (2.5ms - 0.5ms)/3 + 0.5ms   = 1.2 ms -> OCR2B = 9
	90º  -> (2.5ms - 0.5ms)/2 + 0.5ms   = 1.5 ms -> OCR2B = 12
	120º -> (2.5ms - 0.5ms)/1.5 + 0.5ms = 1.8 ms -> OCR2B = 14
	150º -> (2.5ms - 0.5ms)/1.2 + 0.5ms = 2.0 ms -> OCR2B = 17	

	E vale lembrar que o OC2B está vinculado ao pino 3 (PD3). 

	*/

	DDRD |= 0x08; // configura PD3 como saída para acionar o motor 
	sei(); //habilita interrupções
}

ISR(TIMER2_OVF_vect) {
	cont++; // conta uma int. a cada periodo (a cada overflow)

	if (cont >= CICLO) { // quando chegamos no limite do ciclo definido, resetamos o contador para lidar com o prox. ciclo
		cont = 0; 
		if (sentido == 0) { // se sentido == 0 (valor definido para descrever sent. horario), pula para o proximo angulo
			if (OCR2B == 4 || OCR2B == 12) { // pela tabela que calculamos, vimos que de um angulo para o outro o salto no valor de OCR2B pode ser de 2 ou 3 unidades
				OCR2B += 2; 
			}
			else { // para OCR2B != [4, 12], incrementamos 3 unidades em OCR2B
				OCR2B += 3;
			}
			if (OCR2B >= 20) { // quando OCR2B chega em 20 (que representa o ang. maximo), invertemos o sentido
				sentido = 1;
			}
		}
		else { // se o sentido é anti-horario (sentido == 1), vamos diminuir o angulo a cada ciclo 
			if (OCR2B == 6 || OCR2B == 14) { // decrementa 2 unidades para esses casos de OCR2B
				OCR2B -= 2;
			}
			else { // decrementa 3 unidades para OCR2B != [6, 14]
				OCR2B -= 3;
			}
			if (OCR2B <= 4) { // quando OCR2B chega em 4 (que representa o ang. minimo), invertemos o sentido
				sentido = 0;
			}
		}
		
	}

}

int main() {

	configs();
	while (1) {
	}

	return 0;
}
