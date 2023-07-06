#include <stdio.h>
#include <limits.h>

struct process {
    char name;
    int AT, BT, WT, TAT, RT, CT, TSF;//TSF=tempo saida da fila
} Q1[10], Q2[10], Q3[10]; // Três filas

int n;

void sortByArrival() {
    struct process temp;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (Q1[i].AT > Q1[j].AT) {
                temp = Q1[i];
                Q1[i] = Q1[j];
                Q1[j] = temp;
            }
        }
    }
}

void sortByArrivalSRTN(int num) {
    struct process temp;
    int i, j;
    for (i = 0; i < num; i++) {
        for (j = i + 1; j < num; j++) {
            if (Q3[i].AT < Q3[j].AT) {
                temp = Q3[i];
                Q3[i] = Q3[j];
                Q3[j] = temp;
            }
        }
    }
}

int main() {

    freopen("entrada.txt", "r", stdin);
    freopen("saida.txt", "w", stdout);


    int i, j, k = 0, r = 0, time = 0, tq1 = 1, tq2 = 2, flag = 0;
    char c;
    printf("Digite o numero de processos:");
    scanf("%d", &n);
    for (i = 0, c = 'A'; i < n; i++, c++) {
        Q1[i].name = c;
        printf("\nDigite o tempo de chegada e tempo de execucao do processo %c: ", Q1[i].name);
        scanf("%d%d", &Q1[i].AT, &Q1[i].BT);
        Q1[i].RT = Q1[i].BT; /* salva o tempo de execução no tempo restante para cada processo */
    }
    sortByArrival();
    time = Q1[0].AT;
    printf("Processos na primeira fila executando RR com qt=%d", tq1);
    printf("\nProcesso\tRT\t\tWT\t\tTAT\t\tExecucao\t\n");
    for (i = 0; i < n; i++) {
        if(Q1[i].AT<=time){
            if (Q1[i].RT <= tq1) {
                time += Q1[i].RT; /* do tempo de chegada do primeiro processo até a conclusão deste processo */
                Q1[i].RT = 0;
                Q1[i].WT = time - Q1[i].AT - Q1[i].BT; /* quantidade de tempo que o processo esperou na primeira fila */
                Q1[i].TAT = Q1[i].WT + Q1[i].BT;/* quantidade de tempo para executar o processo */
                printf("%c\t\t%d\t\t%d\t\t%d\t\t%d - %d\t\n", Q1[i].name, Q1[i].BT, Q1[i].WT, Q1[i].TAT, time - Q1[i].BT, time);
            } else /* processo passa para a fila 2 com qt=2 */
            {
                Q2[k].WT = time;
                time += tq1;
                Q1[i].RT -= tq1;
                Q2[k].BT = Q1[i].RT;
                Q2[k].RT = Q2[k].BT;
                Q2[k].name = Q1[i].name;
                Q1[i].TSF = time;
                Q2[k].TSF = Q1[i].TSF;
                k++;
                Q1[i].WT = time - Q1[i].AT - tq1;
                Q1[i].TAT = Q1[i].WT + tq1;

                flag = 1;
                printf("%c\t\t%d\t\t%d\t\t%d\t\t%d - %d\t\n", Q1[i].name, Q1[i].BT, Q1[i].WT, Q1[i].TAT, time - tq1, time);

            }
        }
        else{

            time = Q1[i].AT;
            i--;
        }

    }
    if (flag == 1) {
        printf("\nProcessos na segunda fila executando RR com qt=%d", tq2);
        printf("\nProcesso\tRT\t\tWT\t\tTAT\t\tExecucao\n");
    }
    for (i = 0; i < k; i++) {
        if (Q2[i].RT <= tq2) {
            time += Q2[i].RT; /* do tempo de chegada do primeiro processo +BT deste processo */
            Q2[i].RT = 0;

            Q2[i].WT = time - Q2[i].TSF - Q2[i].BT;/* quantidade de tempo que o processo esperou na fila de prontidão */
            Q2[i].TAT = Q2[i].WT + Q2[i].BT;
            printf("%c\t\t%d\t\t%d\t\t%d\t\t%d - %d\n", Q2[i].name, Q2[i].BT, Q2[i].WT, Q2[i].TAT, time - Q2[i].BT, time);
        } else /* processo passa para a fila 3 com FCFS */
        {
            Q3[r].AT = time;
            time += tq2;
            Q2[i].RT -= tq2;
            Q3[r].BT = Q2[i].RT;
            Q3[r].RT = Q3[r].BT;
            Q3[r].name = Q2[i].name;
            Q2[i].WT = time - Q2[i].TSF -tq2;
            Q3[r].TSF = time;
            Q2[i].TAT = Q2[i].WT + tq2;
            r++;
            flag = 2;
            printf("%c\t\t%d\t\t%d\t\t%d\t\t%d - %d\t\n", Q2[i].name, Q2[i].BT, Q2[i].WT, Q2[i].TAT, time - tq2, time);
        }
    }
    if (flag == 2) {
        printf("\nProcessos na terceira fila executando SRTN");
        printf("\nProcesso\tRT\t\tWT\t\tTAT\t\tExecucao\n");
    }
    int completed = 0;  // Variável para controlar quantos processos foram concluídos
    while (completed < r) {
        int shortestIndex = -1;  // Índice do processo com menor tempo restante
        int shortestTime = INT_MAX;  // Inicialmente definido como um valor muito alto
        // Procura o processo com o menor tempo restante entre os processos não concluídos
        for (int i = 0; i < r; i++) {
            if (Q3[i].RT > 0 && Q3[i].RT < shortestTime) {
                shortestTime = Q3[i].RT;
                shortestIndex = i;
            }
        }
        if (shortestIndex != -1) {
            // Executa o processo com menor tempo restante
            int executionTime = (Q3[shortestIndex].RT < tq1) ? Q3[shortestIndex].RT : tq1;
            time += executionTime;
            Q3[shortestIndex].RT -= executionTime;

            if (Q3[shortestIndex].RT == 0) {
                // Processo concluído
                completed++;
                Q3[shortestIndex].CT = time;
                Q3[shortestIndex].WT = time - Q3[shortestIndex].BT - Q3[shortestIndex].TSF;
                Q3[shortestIndex].TAT = Q3[shortestIndex].WT + Q3[shortestIndex].BT;
                printf("%c\t\t%d\t\t%d\t\t%d\t\t%d - %d\n", Q3[shortestIndex].name, Q3[shortestIndex].BT, Q3[shortestIndex].WT, Q3[shortestIndex].TAT, Q3[shortestIndex].CT - Q3[shortestIndex].BT, Q3[shortestIndex].CT);
            } else {
                // Processo não concluído, volta para a fila Q3 para a próxima execução
                Q3[shortestIndex].CT = time;
                Q3[shortestIndex].WT = Q3[shortestIndex].CT - Q3[shortestIndex].AT - Q3[shortestIndex].BT;
                Q3[shortestIndex].TAT = Q3[shortestIndex].WT + Q3[shortestIndex].BT;
            }
        } else {
            // Não há processos na fila para execução
            time++;
        }
    }


    return 0;
}
