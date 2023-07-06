#ifndef trab_so.h
#define trab_so.h

#define MAX_PROCESSOS 20
#define MAX_BUFFER 10
#define QUANTUM 1

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

void inicializarBuffer(struct BufferCircular* buffer);
bool bufferVazio(struct BufferCircular* buffer);
bool bufferCheio(struct BufferCircular* buffer);
void adicionarProcesso(struct BufferCircular* buffer, struct Processo processo, struct BufferCircular* filaEsperaProc);
void removerProcesso(struct BufferCircular* buffer, int indice);
void executarSRTNComPonteiro(struct Processo processos[], int num_processos)
void executarSRTN(struct Processo processos[], int num_processos);
void enfileirar(struct BufferCircular* fila, struct Processo processo);
struct Processo desenfileirar(struct BufferCircular* fila);
void executarMFR(struct Processo* processo, int quantum);
void escalonadorMultilevelFeedbackQueue(struct BufferCircular* fila1, struct BufferCircular* fila2);
void escalonadorMFR(struct BufferCircular fila1[], struct BufferCircular fila2[]);

