# Atividade 2 - Ponteiros, vetores e funções em C

O objetivo desse exercício é manipular um vetor que armazena uma cadeia de caracteres, uma string. O
texto é um trecho do discurso de promulgação da Constituição de 1988:

> A Constituição certamente não é perfeita. Ela própria o confessa ao admitir a reforma. Quanto a ela, discordar, sim.
Divergir, sim. Descumprir, jamais. Afrontá-la, nunca.
Traidor da Constituição é traidor da Pátria. Conhecemos o caminho maldito. Rasgar a Constituição, trancar as portas
do Parlamento, garrotear a liberdade, mandar os patriotas para a cadeia, o exílio e o cemitério.
Quando após tantos anos de lutas e sacrifícios promulgamos o Estatuto do Homem da Liberdade e da Democracia
bradamos por imposição de sua honra.
Temos ódio à ditadura. Ódio e nojo.
Amaldiçoamos a tirania aonde quer que ela desgrace homens e nações. Principalmente na América Latina.

> Ulysses Guimarães, Presidente da Assembleia Nacional Constituinte.

Os alunos devem implementar a função *conta_palavras* que recebe o texto e retorna o número de palavras nele (palavras separadas por
hífen contam como duas). Devem implementar também a função *imprime_palavras_c* que imprime todas as
palavras que se iniciam com a letra c na sequência em que aparecem. Elas devem ser impressas uma por
linha. 

Não utilizem funções prontas para manipular cadeias de caracteres. **A única biblioteca que pode ser
usada é a stdio.h.**
