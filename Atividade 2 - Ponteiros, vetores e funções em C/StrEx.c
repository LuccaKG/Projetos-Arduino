#include <stdio.h>

// Trecho do discurso do presidente da assembl�ia constituinte, Ulysses Guimar�es, proferido por ocasi�o da promulga��o da Constitui��o de 1988.
char texto[] = "A Constitui��o certamente n�o � perfeita. Ela pr�pria o confessa ao admitir a reforma. Quanto a ela, discordar, sim. Divergir, sim. Descumprir, jamais. Afront�-la, nunca.\nTraidor da Constitui��o � traidor da P�tria. Conhecemos o caminho maldito. Rasgar a Constitui��o, trancar as portas do Parlamento, garrotear a liberdade, mandar os patriotas para a cadeia, o ex�lio e o cemit�rio.\nQuando ap�s tantos anos de lutas e sacrif�cios promulgamos o Estatuto do Homem da Liberdade e da Democracia bradamos por imposi��o de sua honra.\nTemos �dio � ditadura. �dio e nojo.\nAmaldi�oamos a tirania aonde quer que ela desgrace homens e na��es. Principalmente na Am�rica Latina.\n\nUlysses Guimar�es, Presidente da Assembleia Nacional Constituinte.\n";
char separadores[] = {',', '.', '?', '!', ' ', '-'};

//Defini acima o texto e as pontua��es/separadores poss�veis como vari�veis globais. Saber os separadores vai ajudar
//a identificar quando uma palavra come�a e termina

// como nao podemos usar a biblioteca string.h, criarei meu proprio metodo strlen() para contagem de caracteres numa string (ou num vetor qqr)
int strlen(char* string) {
	int index = 0;
	// percorro toda a string e conto um caractere a cada itera��o, ate encontrar o \0 indicando o final do vetor
	while (string[index] != '\0') {
		index += 1;
	}
	return index;
}
	

// abaixo a implementa��o da fun��o que conta o numero total de palavras
int conta_palavras(char* cadeia_char, char* seps) {
	int qtd_palavras = 0;

	for (int i = 1; i < strlen(cadeia_char); i++) { // este primeiro for percorre caractere por caractere do texto
		for (int j = 0; j < strlen(seps); j++) { // este segundo for percorre o vetor de separadores
			if (cadeia_char[i] == seps[j]) {
			// note que eu identifico a presen�a de um separador e, se antes dele nao houver outro separador (por exemplo duplo espa�o),
			// reconhe�o como uma palavra que acabou de terminar.

			// Se a posi��o anterior (i - 1) ao separador nao for outro separador, como citado acima, obviamente ser� uma letra,
			// por isso reconhe�o que uma palavra terminou.
			
			// Utilizo o operador && (and) para garantir que a condi��o s� seja verdadeira se a posi��o anterior realmente n�o for
			// nenhum separador listado no vetor separadores[]
				if (cadeia_char[i - 1] != seps[0] && cadeia_char[i - 1] != seps[1] && cadeia_char[i - 1] != seps[2] && cadeia_char[i - 1] != seps[3] && cadeia_char[i - 1] != seps[4] && cadeia_char[i - 1] != seps[5]) {
					qtd_palavras += 1;
				}
			}
		}

	}

	return qtd_palavras;
}

// abaixo a fun��o que conta a qtd de palavras iniciadas com a letra c
int conta_c(char *cadeia_char2, char *seps2) {
	int qtd_c = 0;

	// condi��o especial para identificar strings com primeira palavra come�ando com C. 
	// Note que essa condi��o extra � necess�ria, pois se no la�o 'for' abaixo eu come�asse a contagem de int i em 0,
	// a primeira itera��o com cadeia_char2[i-1] retornaria um �ndice negativo e isso n�o faria sentido.
	if (cadeia_char2[0] == 'C') {
		qtd_c += 1;
	}

	// An�logo ao que foi feito na fun��o conta_palavras(), s� que agora desejo percorrer caractere a caractere da string cadeia_char2,
	// por�m identificando palavras iniciadas com a letra C.

	// Identifico que uma palavra come�a com 'c' se o caractere cadeia_char2	[i] for 'c' e a posi��o anterior dela ser ocupada por qualquer
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
		// percorro a string e verifico se estamos no in�cio de uma palavra e se o caractere � 'C' ou 'c'
		if ((index2 == 0 || cadeia_char3[index2 - 1] == ' ' || cadeia_char3[index2 - 1] == '\n') && (cadeia_char3[index2] == 'C' || cadeia_char3[index2] == 'c')) {
			// Se come�ar, imprimo a palavra (identifica que a palavra terminou quando encontra um separador ou o conhecido \0)
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
	//  Pra corrigir essa aparencia, poderiamos utilizar o locale.h (que n�o manipula cadeia de caracteres nem seria utilizado para contagem/identifica��o das palavras).
	printf("O n�mero de palavras no texto �: %d.\n", conta_palavras(texto, separadores));
	printf("O n�mero de palavras iniciadas com C (ou c) �: %d.\n", conta_c(texto, separadores));
	imprime_palavras(texto, separadores);
	}
