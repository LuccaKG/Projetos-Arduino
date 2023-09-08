#include <stdio.h>

// Trecho do discurso do presidente da assembléia constituinte, Ulysses Guimarães, proferido por ocasião da promulgação da Constituição de 1988.
char texto[] = "A Constituição certamente não é perfeita. Ela própria o confessa ao admitir a reforma. Quanto a ela, discordar, sim. Divergir, sim. Descumprir, jamais. Afrontá-la, nunca.\nTraidor da Constituição é traidor da Pátria. Conhecemos o caminho maldito. Rasgar a Constituição, trancar as portas do Parlamento, garrotear a liberdade, mandar os patriotas para a cadeia, o exílio e o cemitério.\nQuando após tantos anos de lutas e sacrifícios promulgamos o Estatuto do Homem da Liberdade e da Democracia bradamos por imposição de sua honra.\nTemos ódio à ditadura. Ódio e nojo.\nAmaldiçoamos a tirania aonde quer que ela desgrace homens e nações. Principalmente na América Latina.\n\nUlysses Guimarães, Presidente da Assembleia Nacional Constituinte.\n";
char separadores[] = {',', '.', '?', '!', ' ', '-'};

//Defini acima o texto e as pontuações/separadores possíveis como variáveis globais. Saber os separadores vai ajudar
//a identificar quando uma palavra começa e termina

// como nao podemos usar a biblioteca string.h, criarei meu proprio metodo strlen() para contagem de caracteres numa string (ou num vetor qqr)
int strlen(char* string) {
	int index = 0;
	// percorro toda a string e conto um caractere a cada iteração, ate encontrar o \0 indicando o final do vetor
	while (string[index] != '\0') {
		index += 1;
	}
	return index;
}
	

// abaixo a implementação da função que conta o numero total de palavras
int conta_palavras(char* cadeia_char, char* seps) {
	int qtd_palavras = 0;

	for (int i = 1; i < strlen(cadeia_char); i++) { // este primeiro for percorre caractere por caractere do texto
		for (int j = 0; j < strlen(seps); j++) { // este segundo for percorre o vetor de separadores
			if (cadeia_char[i] == seps[j]) {
			// note que eu identifico a presença de um separador e, se antes dele nao houver outro separador (por exemplo duplo espaço),
			// reconheço como uma palavra que acabou de terminar.

			// Se a posição anterior (i - 1) ao separador nao for outro separador, como citado acima, obviamente será uma letra,
			// por isso reconheço que uma palavra terminou.
			
			// Utilizo o operador && (and) para garantir que a condição só seja verdadeira se a posição anterior realmente não for
			// nenhum separador listado no vetor separadores[]
				if (cadeia_char[i - 1] != seps[0] && cadeia_char[i - 1] != seps[1] && cadeia_char[i - 1] != seps[2] && cadeia_char[i - 1] != seps[3] && cadeia_char[i - 1] != seps[4] && cadeia_char[i - 1] != seps[5]) {
					qtd_palavras += 1;
				}
			}
		}

	}

	return qtd_palavras;
}

// abaixo a função que conta a qtd de palavras iniciadas com a letra c
int conta_c(char *cadeia_char2, char *seps2) {
	int qtd_c = 0;

	// condição especial para identificar strings com primeira palavra começando com C. 
	// Note que essa condição extra é necessária, pois se no laço 'for' abaixo eu começasse a contagem de int i em 0,
	// a primeira iteração com cadeia_char2[i-1] retornaria um índice negativo e isso não faria sentido.
	if (cadeia_char2[0] == 'C') {
		qtd_c += 1;
	}

	// Análogo ao que foi feito na função conta_palavras(), só que agora desejo percorrer caractere a caractere da string cadeia_char2,
	// porém identificando palavras iniciadas com a letra C.

	// Identifico que uma palavra começa com 'c' se o caractere cadeia_char2	[i] for 'c' e a posição anterior dela ser ocupada por qualquer
	// separador. Por isso utilizei o operador OR caso qualquer separador seja identificado antes da ocorrencia da letra
	for (int i = 1; i < strlen(cadeia_char2); i++) {
		if (cadeia_char2[i] == 'C' || cadeia_char2[i] == 'c') {
			if (cadeia_char2[i - 1] == seps2[0] || cadeia_char2[i - 1] == seps2[1] || cadeia_char2[i - 1] == seps2[2] || cadeia_char2[i - 1] == seps2[3] || cadeia_char2[i - 1] == seps2[4] || cadeia_char2[i - 1] == seps2[5]) {
				qtd_c += 1;
			}
		}
			}

	return qtd_c;

}

void imprime_palavras(char *cadeia_char3, char *seps3) {
	int index2 = 0;
	while (cadeia_char3[index2] != '\0') {
		// percorro a string e verifico se estamos no início de uma palavra e se o caractere é 'C' ou 'c'
		if ((index2 == 0 || cadeia_char3[index2 - 1] == ' ' || cadeia_char3[index2 - 1] == '\n') && (cadeia_char3[index2] == 'C' || cadeia_char3[index2] == 'c')) {
			// Se começar, imprimo a palavra (identifica que a palavra terminou quando encontra um separador ou o conhecido \0)
			while (cadeia_char3[index2] != ' ' && cadeia_char3[index2] != '\0' && cadeia_char3[index2] != '\n' && cadeia_char3[index2] != seps3[0] && cadeia_char3[index2] != seps3[1] && cadeia_char3[index2] != seps3[2] && cadeia_char3[index2] != seps3[3] && cadeia_char3[index2] != seps3[4] && cadeia_char3[index2] != seps3[5]) {
				putchar(cadeia_char3[index2]);
				index2++;
			}
			putchar('\n');
		}
		else {
			index2++;
		}
	}

}

int main() {

	// note que o programa pode ter dificuldade em reconhecer caracteres especiais, dependendo da IDE.
	//  Pra corrigir essa aparencia, poderiamos utilizar o locale.h (que não manipula cadeia de caracteres nem seria utilizado para contagem/identificação das palavras).
	printf("O número de palavras no texto é: %d.\n", conta_palavras(texto, separadores));
	printf("O número de palavras iniciadas com C (ou c) é: %d.\n", conta_c(texto, separadores));
	imprime_palavras(texto, separadores);
	}
