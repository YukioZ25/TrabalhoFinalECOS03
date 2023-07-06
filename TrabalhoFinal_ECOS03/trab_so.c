//André Marcos Leifeld Raicoski - 2021007221
//Enzo Yukio Chinen - 2021005307

#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSOS 20
#define MAX_BUFFER 10
#define QUANTUM 1
int QUANTUM_FILA1, QUANTUM_FILA2;

struct Processo { //estrutura dos processos
    int id;
    int tempo_chegada;
    int tempo_restante;
    int nivel_feedback;
};

struct BufferCircular { //estrutura da fila
    struct Processo processos[MAX_BUFFER];
    int inicio;
    int fim;
    int tamanho;
};

struct BufferCircular fila1, fila2; //buffers utilizados na fila múltipla com realimentação

void inicializarBuffer(struct BufferCircular* buffer) { //inicializa o buffer
    buffer->inicio = 0;
    buffer->fim = 0;
    buffer->tamanho = 0;
}

bool bufferVazio(struct BufferCircular* buffer) { //verifica se o buffer está vazio
    return buffer->tamanho == 0;
}

bool bufferCheio(struct BufferCircular* buffer) { //verifica se o buffer está cheio
    return buffer->tamanho == MAX_BUFFER;
}

void adicionarProcesso(struct BufferCircular* buffer, struct Processo processo, struct BufferCircular* filaEsperaProc) { //adiciona processo no buffer
    int i;

    if (bufferCheio(buffer)) { //verifica se o buffer está cheio, se sim, manda para a fila de espera
        printf("Buffer cheio! O processo com ID %d nao pode ser adicionado no buffer principal.\n", processo.id);
        filaEsperaProc->processos[filaEsperaProc->fim] = processo;
        filaEsperaProc->fim = (filaEsperaProc->fim + 1) % MAX_BUFFER;
        filaEsperaProc->tamanho++;
        return;
    }
    buffer->processos[buffer->fim] = processo;
    buffer->fim = (buffer->fim + 1) % MAX_BUFFER;
    buffer->tamanho++;
}

void removerProcesso(struct BufferCircular* buffer, int indice) { //remove processo
    int i;
    for (i = indice; i < buffer->tamanho - 1; i++) {
        buffer->processos[i] = buffer->processos[i + 1];
    }
    buffer->tamanho--;
    buffer->fim = (buffer->fim - 1 + MAX_BUFFER) % MAX_BUFFER;
}

void executarSRTN(struct Processo processos[], int num_processos) { //execução do SRTN (Shortest Remaining Time Next)
    struct BufferCircular buffer;
    inicializarBuffer(&buffer);
    struct BufferCircular filaEsperaProc;
    inicializarBuffer(&filaEsperaProc);

    int tempo_total = 0; //clock_tick
    int i;

    int processos_concluidos = 0;
    while (processos_concluidos < num_processos) { //verifica se tem processos chegando no tempo atual e adiciona ao buffer
		for (i = 0; i < num_processos; i++) {
            if (processos[i].tempo_chegada <= tempo_total && processos[i].tempo_restante > 0) {
                adicionarProcesso(&buffer, processos[i], &filaEsperaProc);
                processos[i].tempo_restante = 0;
            }
        }

        if (!bufferVazio(&buffer)) { //roda o de menor tempo
            int indice_menor_tempo_restante = 0;
            int menor_tempo_restante = buffer.processos[0].tempo_restante;
            for (i = 1; i < buffer.tamanho; i++) { //encontra o processo com o menor tempo restante no buffer
            	//printf("ID AKI: %d\n", buffer.processos[i].id);
                if (buffer.processos[i].tempo_restante < menor_tempo_restante) {
                    indice_menor_tempo_restante = i;
                    menor_tempo_restante = buffer.processos[i].tempo_restante;
                }
            }

            struct Processo* processo_executando = &buffer.processos[indice_menor_tempo_restante];

            //executa o processo pelo quantum (nesse caso de 1 em 1)
            printf("Executando processo %d por %d unidade de tempo. Resta %d\n", processo_executando->id, QUANTUM, processo_executando->tempo_restante);
            processo_executando->tempo_restante -= QUANTUM;
            tempo_total += QUANTUM;
            if (processo_executando->tempo_restante <= 0) { //processo concluído
                printf("Processo %d concluido.\n", processo_executando->id);
                removerProcesso(&buffer, indice_menor_tempo_restante);
                processos_concluidos++;

                for(i=0;i<10;i++) {
	                if(buffer.processos[indice_menor_tempo_restante].tempo_restante==0) {
	                	adicionarProcesso(&buffer, buffer.processos[indice_menor_tempo_restante], &filaEsperaProc);
	                	removerProcesso(&filaEsperaProc, processos_concluidos-1);
	                	i=10;
					} else {
						i++;
					}
				}
            }
        } else {
            tempo_total++;
        }
    }
}
void executarSRTNComPonteiro(struct Processo processos[], int num_processos) { //ponteiro de função
    executarSRTN(processos, num_processos);
}

void enfileirar(struct BufferCircular* fila, struct Processo processo) { //enfileira os processos
    if (!bufferCheio(fila)) {
    	fila->processos[fila->fim] = processo;
        fila->fim = (fila->fim + 1) % MAX_BUFFER;
        fila->tamanho++;
    }
}

struct Processo desenfileirar(struct BufferCircular* fila) { //desenfileira os processos (funciona como buffer circular)
    struct Processo processo = fila->processos[fila->inicio];
    fila->inicio = (fila->inicio + 1) % MAX_BUFFER;
    fila->tamanho--;
    return processo;
}

void executarMFR(struct Processo* processo, int quantum) { //execução MFR (múltiplas filas com realimentação)
    if (processo->tempo_restante <= quantum) {
    	printf("Executando processo %d por %d unidades de tempo.\n", processo->id, processo->tempo_restante);
        printf("\nProcesso %d concluido.\n\n", processo->id);
        processo->tempo_restante = 0;

    } else { //decrementa o tempo restante e incrementa quantas vezes foi realimentado até o momento
    printf("Executando processo %d por %d unidades de tempo.\n", processo->id, quantum);
        processo->tempo_restante -= quantum;
		processo->nivel_feedback++;
    	printf("Processo %d realimentado para o nivel %d. Falta %d\n\n", processo->id, processo->nivel_feedback, processo->tempo_restante);
    }

}

void escalonadorMultilevelFeedbackQueue(struct BufferCircular* fila1, struct BufferCircular* fila2) {
    int tempo_atual = 0; //clock_tick
    while (!bufferVazio(fila1) || !bufferVazio(fila2)) { //ainda há processos nas filas
        if (!bufferVazio(fila1)) { //fila tem prioridade
            struct Processo processo = desenfileirar(fila1);

            if (processo.tempo_restante > 0) {
                if (processo.tempo_chegada <= tempo_atual) {
                    executarMFR(&processo, QUANTUM_FILA1);
                    if (processo.tempo_restante > 0) {
                        enfileirar(fila1, processo);
                    }
                } else {
                    enfileirar(fila1, processo);
                }
            } else {
                printf("\n\nProcesso %d ja foi concluido.\n\n", processo.id);
            }
        } else if (!bufferVazio(fila2)) {
            struct Processo processo = desenfileirar(fila2);
            if (processo.tempo_restante > 0){
                if (processo.tempo_chegada <= tempo_atual) {
                    executarMFR(&processo, QUANTUM_FILA2);
                    if (processo.tempo_restante > 0) {
                        enfileirar(fila1, processo);
                    }
                } else {
                    enfileirar(fila2, processo);
                }
            } else {
                printf("\n\nProcesso %d ja foi concluido.\n\n", processo.id);
            }
        }
        tempo_atual++;
    }
}

void escalonadorMFR(struct BufferCircular fila1[], struct BufferCircular fila2[]) { //ponteiro de função
    escalonadorMultilevelFeedbackQueue(fila1, fila2);
}

int main() {
	int i;
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
    struct Processo processos[20], processo2[20];
    for(i=0;i<20;i++) {
    	processos[i].id = i+1;
    	scanf("%d %d %d", &processos[i].tempo_chegada, &processos[i].tempo_restante, &processos[i].nivel_feedback);
    	processo2[i] = processos[i];
	}

    printf("SRTN\n\n");

    int num_processos = sizeof(processos) / sizeof(processos[0]); //quantidade de processos

    void (*funcao_executarSRTN)(struct Processo[], int) = executarSRTNComPonteiro;
    funcao_executarSRTN(processos, num_processos); //ponteiro de função

    //multilevel feedback queue
    scanf("%d %d", &QUANTUM_FILA1, &QUANTUM_FILA2);
    printf("\n\nMULTILEVEL FEEDBACK QUEUE\n");

    inicializarBuffer(&fila1);
    inicializarBuffer(&fila2);

    for (i = 0; i < MAX_PROCESSOS; i++) {
        if (i % 2 == 0) {
            if (!bufferCheio(&fila1)) {
                enfileirar(&fila1, processo2[i]);
                printf("Processo %d adicionado na fila 1\n", processo2[i].id);
            }
        } else {
            if (!bufferCheio(&fila2)) {
                enfileirar(&fila2, processo2[i]);
                printf("Processo %d adicionado na fila 2\n", processo2[i].id);
            }
        }
    }

    void (*funcao_escalonadorMFR)(struct BufferCircular, struct BufferCircular) = escalonadorMFR;
    funcao_escalonadorMFR(fila1, fila2); //ponteiro de função

    escalonadorMultilevelFeedbackQueue(&fila1, &fila2);
    return 0;
}
