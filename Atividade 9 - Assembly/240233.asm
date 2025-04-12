
; As diretivas .org definem o endereço de memória onde a instrução a seguir vai residir

.org 0x0000    ; o programa comeca no endereco 0x0000 (de palavra) da memoria de programa, que corresponde ao endereco carregado no PC (Program Counter) apos um reset do microcontrolador
    jmp main    ; salto para evitar a regiao da memoria onde ficam armazenados os vetores de interrupcao

.org 0x001C ; atribui a rsi o Prog. Add. 0x001C, que é a int 15 - TIMER0_COMPA_vect
    jmp rsi      

.org 0x0034    ; primeiro endereco (de palavra) livre fora da faixa reservada para os vetores de interrupcao

main:
    cli ; desabilita interrupções
   
    ; Carrega o SP (pilha) como o ultimo endereco da memoria RAM 0x08FF
    ldi r16,0xFF
    sts 0x5D,r16
    ldi r16,0x08
    sts 0x5E,r16

; Configuração do temporizador
    ; utilizamos out para escrever em reg de I/O e sts para escrever em um end. de memoria SRAM

    ldi r16, 0xF9  ; Valor para OCR0A (249 em decimal)
    out OCR0A, r16
    ; Igual ao que foi feito na atividade 7, para atingirmos T = 1 ms podemos setar OCR0A = 249 e P = 64.

    ldi r16, (1 << OCIE0A)  ; Ativa a interrupção de comparação A do Timer 0 setando o bit 1 (OCIE0A) de TIMSK0
    sts TIMSK0, r16

    ldi r16, (1 << CS01) | (1 << CS00)  ; prescaler = 64 setando os bits 0 e 1 (CS01 e CS00) de TCCR0B
    out TCCR0B, r16

    ldi r16, (1 << WGM01)  ; Modo CTC setando o bit 1 (WGM01) de TCCR0A. O modo CTC zera o timer após a contagem atingir o valor de OCR0A
    out TCCR0A, r16

    sbi DDRB, 5  ; Configura o pino do LED como saída -> seta o bit 5 de DDRB 
    cbi PORTB, 5  ; Inicializa o LED apagado -> zera o bit 5 de PORTB

    ; Inicialização de variáveis
    ldi r16, 0  ; Contador inicializando 0
    ldi r17, 1  ; variável unitária constante para incremento do contador a cada iteração
    ldi r18, 100  ; Limite do contador = 100 pois a base de tempo (T) é 1 ms e queremos atingir 100 ms de intervalo conforme enunciado
    ldi r19, 0  ; Estado do LED (0 = apagado, 1 = aceso)

    sei ; habilita interrupções

loop:
    rjmp loop ; mantém o loop em execução 'ouvindo' as chamadas de interrupções 

rsi:
    add r16, r17  ; Incrementa o contador
    cp r16, r18  ; Compara com o limite estabelecido de 100
    breq muda ; se a comparação retornar True, muda o estado do LED

    rjmp fim ; se a comparação retornar False, vai para a subrotina fim, que encerra a rotina de interrupção

muda:
    ldi r16, 0 ; zera o contador após chegar no seu limite (100)
    cp r16, r19 ; verifica se o estado do LED (r19) é 0 (apagado)
    breq liga_led ; se for igual (ou seja, LED apagado), desvia para a subrotina que liga/acende o led (ou seja, inverte seu estado)

    ldi r19, 0 ; se a comparação retornar False, quer dizer que o LED estava aceso, então primeiro trocamos o valor na variavel de estado...
    cbi PORTB, 5 ; ... e depois zeramos o bit 5 de PORTB apagando o LED (ou seja, invertemos seu estado)

    rjmp fim ; desvia para a subrotina que retorna da interrupção

liga_led:
    ldi r19, 1 ; muda o valor da variável de estado do LED para 1 (aceso)
    sbi PORTB, 5 ; seta o bit 5 de PORTB, acendendo o LED

fim:
    reti
   