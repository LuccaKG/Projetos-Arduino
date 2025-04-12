/* Bibliotecas */
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* Tamanho máximo do Buffer */
#define TAM_MAX_BUFFER 5

/* Declaração do Buffer */
volatile char circBuffer[TAM_MAX_BUFFER];
/* Variáveis de controle para adicionar e remover valores no buffer */
volatile char posAddBuffer = 0;
volatile char posRemoveBuffer = 0;
volatile char posicoesOcupadasBuffer = 0;

/* Função para adicionar valores ao buffer */
void insereBuffer(char valor) {
    /* Se há espaço no buffer */
    if (posicoesOcupadasBuffer < TAM_MAX_BUFFER) {
        circBuffer[posAddBuffer] = valor;
        posicoesOcupadasBuffer++;
        posAddBuffer = (posAddBuffer == (TAM_MAX_BUFFER-1)) ? 0 : posAddBuffer + 1;
    }
}

/* Função para remover valores do buffer */
char extraiBuffer() {
    char dadoRetirado;
    /* Se o buffer contém dados */
    if (posicoesOcupadasBuffer > 0) {
        dadoRetirado = circBuffer[posRemoveBuffer];
        posicoesOcupadasBuffer--;
        posRemoveBuffer = (posRemoveBuffer == (TAM_MAX_BUFFER-1)) ? 0 : posRemoveBuffer + 1;
    }
    return dadoRetirado;
}

/* Ponteiros para controlar GPIOs e UART */
// PORTC como saída -> PC3, PC4 E PC5 conforme enunciado

unsigned char *enderecoPortC;
unsigned char *enderecoDDRc;
unsigned char *enderecoPINc;

// ubrr[H:L] definem baud rate
unsigned char *enderecoUbrr0H;
unsigned char *enderecoUbrr0L;

// ucsr definem configuração e status
unsigned char *enderecoUcsr0A;
unsigned char *enderecoUcsr0B;
unsigned char *enderecoUcsr0C;

// udr é o reg de dados
unsigned char *enderecoUdr0;

/* Função para inicializar periféricos */
void iniciaConfiguracoes() {
    cli();  // Desabilita interrupções durante a configuração
  
    /* Configuração dos GPIOs */
    enderecoDDRc = (unsigned char *) 0x27;
    enderecoPortC = (unsigned char *) 0x28;
    enderecoPINc = (unsigned char *) 0x26;
  
    /* Configuração da UART */
    enderecoUbrr0H = (unsigned char *) 0xC5;
    enderecoUbrr0L = (unsigned char *) 0xC4;
    enderecoUcsr0A = (unsigned char *) 0xC0;
    enderecoUcsr0B = (unsigned char *) 0xC1;
    enderecoUcsr0C = (unsigned char *) 0xC2;
    enderecoUdr0 = (unsigned char *) 0xC6;

    /* Define todo DDRC como saída, já que nao utilizaremos os terminais que nao sejam PC3, PC4 E PC5 */
    *enderecoDDRc |= 0XFF;

    /* Configura UART para 19.200 Baud utilizando os 12 bits LSB de UBRR0 */
    *enderecoUbrr0H = 0;
    *enderecoUbrr0L = 0x33; 

    /* UCSRnA (dos slides)
     U2Xn = 0 para transmissão com velocidade normal, e
     MPCMn = 0 para desabilitar o multiprocessador.
     X = Don't care pros outros
     RXCn TXCn UDREn FEn DORn UPEn U2Xn MPCMn
       X    X    X     X   X     X   0    0 */
    *enderecoUcsr0A = 0;

  
  /* UCSRnB 
     RXCIEn = 1 para habilitar a interrupção associada a recepção;
     TXCIEn = 0 para desabilitar a interrupção associada a transmissão completa;
     UDRIEn = 1 para habilitar a interrupção associada ao reg dados vazio; 
     RXENn = 1 e TXENn = 1 para habilitar recepção e transmissão, respectivamente
     UCSZn2 = 0 para que o n° de bits por frame seja 8. 
 
     RXCIEn TXCIEn UDRIEn RXENn TXENn UCSZn2 RXB8n TXB8n 
       1        0    1      1     1     0      X     X    */  

    *enderecoUcsr0B = 0xB8;


   /* UCSRnC 
     UMSELn1 = 0 e UMSELn0 = 0, para UART operando em modo assíncrono;
     UPMn1 = 0 e UPMn0 = 0, para nao ter bits de paridade;
     USBSn = 0 para um único bit de parada, e
     UCSZn1 = 1 e UCSZn0 = 1 para que o n° de bits por frame seja 8.
     UMSELn1 UMSELn0 UPMn1 UPMn0 USBSn UCSZn1 UCSZn0 UCPOLn 
        0       0      0     0     0     1      1      0    */

    *enderecoUcsr0C = 0x6;


    sei();  // Habilita interrupções
}

/* Variáveis para controle do fluxo do programa */
volatile int idxMsg = 0; // auxiliar para percorrer mensagem a ser printada
char dadoRecebido = 0; // auxiliar para armazena dado do monitor serial
volatile char *mensagemAtual; // seleciona msgm printada

/* Estados do LED */
volatile int estadoLed = 0;

/* Mensagens para o usuário */
char mensagemTodosLeds[] = "Comando: Todos os LEDs piscando\n";
char mensagemUmLedAceso[] = "Comando: Varredura com um LED aceso\n";
char mensagemUmLedApagado[] = "Comando: Varredura com um LED apagado\n";
char mensagemComandoInvalido[] = "Comando incorreto\n";
char mensagemBufferVazio[] = "Vazio!\n";

/* Funções de Interrupção */
ISR(USART_RX_vect) {
    dadoRecebido = *enderecoUdr0; // recebe info do reg de dados
    insereBuffer(dadoRecebido); // transfere para o buffer
}

ISR(USART_UDRE_vect) {
    if (mensagemAtual[idxMsg] != '\0') { // verifica se nao chegou no final da string
        *enderecoUdr0 = mensagemAtual[idxMsg]; // coloca caractere no reg de dados
        idxMsg++;
    } else { // se a string acabou, zera idxMsg e desabilita interrupção do reg de dados
        idxMsg = 0;
        *enderecoUcsr0B &=~ 0x20; // ~0x20 = 1101 1111, ou seja, mantém o estado de todos os bits, menos de UDRIEn, que é zerado
    }
}

/* Máquina de Estados */
void verificaComandos() {
// quando o buffer está vazio, imprimimos "vazio" e mantem o estado setado anteriormente
// se o buffer nao estiver vazio, analisamos e executamos a ação conforme tabela do enunciado
    if (posicoesOcupadasBuffer != 0) {
        unsigned char comando = extraiBuffer();
        switch(comando) {
            case '0':
                estadoLed = 0;
                mensagemAtual = &mensagemTodosLeds[idxMsg];
                break;
            case '1':
                estadoLed = 1;
                mensagemAtual = &mensagemUmLedAceso[idxMsg];
                break;
            case '2':
                estadoLed = 2;
                mensagemAtual = &mensagemUmLedApagado[idxMsg];
                break;
            default:
                mensagemAtual = &mensagemComandoInvalido[idxMsg];
                break;
        }
    } else { // condição pro buffer vazio
        mensagemAtual = &mensagemBufferVazio[idxMsg];
    }
    *enderecoUcsr0B |= 0x20; // ao contrario de qd fizemos o & com a negação de 0x20, agora fazemos o OR para ATIVAR somente UDRIEn e manter o estado do resto
}

/* Funções para controle dos LEDs */
void todosLedsPiscando() {
    *enderecoPortC = 0x38; // terminais utilizados recebem valor lógico alto
    _delay_ms(500);
    *enderecoPortC &= ~(0x38); // apaga dps de 500 ms
    _delay_ms(500);
}

void varreComUmLedAceso() {
    *enderecoPortC = 0x20; // PC5 aceso; demais apagados
    _delay_ms(500);
    *enderecoPortC = 0x10; // PC4 aceso; demais apagados
    _delay_ms(500);
    *enderecoPortC = 0x8;  // PC3 aceso; demais apagados
    _delay_ms(500);
    *enderecoPortC = 0x10; // novamente somente PC4 aceso
    _delay_ms(500);
    *enderecoPortC = 0x20; // novamente somente PC5 aceso
    _delay_ms(500);
}

void varreComUmLedApagado() {
    *enderecoPortC = 0x18; // somente PC5 apagado
    _delay_ms(500);
    *enderecoPortC = 0x28; // somente PC4 apagado
    _delay_ms(500);
    *enderecoPortC = 0x30; // somente PC3 apagado
    _delay_ms(500);
    *enderecoPortC = 0x28; // novamente somente PC4 apagado
    _delay_ms(500);
    *enderecoPortC = 0x18; // novamente somente PC5 apagado
    _delay_ms(500);
}

/* Controla LEDs baseado no comando */
void atualizaLeds() {
    switch(estadoLed) {
        case 0:
            todosLedsPiscando();
            break;
        case 1:
            varreComUmLedAceso();
            break;
        case 2:
            varreComUmLedApagado();
            break;
    }
}

/* Função principal */
int main(void) {
    iniciaConfiguracoes();
    while (1) {
        verificaComandos(); // fiscaliza o buffer para saber msgm e ações necessarias
        atualizaLeds(); // atualiza a config. dos leds de acordo com o comando 
        _delay_ms(500); // espera 500 ms até retornar o loop
    }
    return 0;
}
