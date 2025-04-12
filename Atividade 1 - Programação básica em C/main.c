	/* Contador de bits
	*
	* Este programa conta o numero de bits em um byte
	*/

	#include <stdio.h>

	int main() {

		unsigned char entrada;
		unsigned int tmp;
		int counter();

		/* Ler entrada em hexadecimal */
		scanf_s("%x", &tmp);
		entrada = (unsigned char)tmp;

		/* Escrever numero de bits */
		printf("%d\n", counter(entrada));
		return 0;
	}

	int counter(int entrada) {

		unsigned int n_bits = 0; /* inicializando acumulador usado para contagem */

		/* Como a ideia é analisar sempre o LSB do numero, vamos descartando-o após análise ao deslocar para a direita.
		Conforme deslocamos o numero para a direita, diminuimos seu valor (análogo a dividir por 2 a cada iteração). 
		Quando o número se tornar nulo, quer dizer que não há mais bits a serem analisados e o laço pode ser encerrado */
	
		while (entrada > 0) {
			/* Como 1 [decimal] = 0000...00001 [binário], a operação AND bit a bit (i.e. &) fará com que todos
			os bits comparados à 'entrada' resultem 0 a menos do LSB, que pode ser 0 ou 1.
			Se o LSB for 0, entrada & 1 == 0 == False e não entramos na condicional.
			Se o LSB for 1, entrada & 1 == 1 == True e entramos na condicional */
			if (entrada & 1) {
				n_bits++; /* soma 1 no contador se entrar na condicional, i.e., se o bit analisado for 1 */
			}

			entrada >>= 1; /* feita a análise, deslocamos o número para a direita a fim de analisar o próximo bit*/
		}

		return n_bits;
	}