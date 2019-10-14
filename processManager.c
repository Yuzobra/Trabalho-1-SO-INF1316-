#define KEY_V 4242
#define KEY_C 5050
#define SIZE 50


#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <time.h>

#include <signal.h>

#include "procInfo.h"
#include "LisCircular.h"
#include "LisEncadeada.h"


ProcInfo * getProcInfo(int fd[]);

void AlrmHandler(int sinal);
void ChildHandler(int sinal);
int afterRTProc(ProcInfo *);

int flag = 0; //flag que diz se o processo deve escalonar ou olhar a pipe
int flagRemove = 0;
TipoProc flagTipo = -1; //flag que diz o tipo do processo que terminou a sua execução

typedef struct realTimeProc{
	int pid;
	ProcInfo * procInfo;
} RealTimeProc;


int main (int argc, char *argv[]) { 
	
	int pidreader,fd[2], sizeOfProcInfo,i, pidProcRealTime = -1;
	RealTimeProc * realTime[60];
	clock_t startTime;
	Lista * waitingList;

	waitingList = criaLista();
	if(pipe(fd) < 0){
			printf("Erro ao criar pipe");
			exit(1);		
	}
	fcntl(fd[0], F_SETFL, O_NONBLOCK); //Set read fd as Non-Blocking
	
	
	if((pidreader = fork()) < 0){
		printf("Erro ao criar processo reader\n");
		exit(1);
	}

	if(pidreader == 0){
		 /* Este processo irá executar o reader */
		printf("Iniciando processo 1\n");
	 	char arg[12];
		char * arr[3];
	 	sprintf(arg,"%d",fd[1]);
		arr[0] = "./reader";
		arr[1] = arg;
		arr[2] = NULL;
		//close(fd[0]);
		execv(arr[0], arr);
	}

	for( i = 0; i < 60; i++ ){
		realTime[i] = NULL;
	}

	/*A partir daqui é o escalonador*/
	No * listaProcs = CriaLista();
	int pidProc;
	ProcInfo *procInfo;
	signal(SIGALRM, AlrmHandler);
	signal(SIGCHLD, ChildHandler);
	alarm(1);
	startTime = clock();
	while (1) {
		if (flagRemove > 1) { /* Um processo foi terminado (flagRemove == pid do processo terminado)*/
			int pidRemovido = flagRemove;

			if (pidRemovido == pidreader) {
				printf("\nInterpretador terminou\n");
				flagRemove = 0;
			}
			else {
				printf("\nRemovendo Processo\n");
				if (pidRemovido == listaProcs->pid ) { /* Se o processo é o que está rodando atualmente, desliga o alarme e fala pra próxima iteração do while escalonar*/
					flagRemove = 0;
					flag = 1;
					listaProcs = removeElemento(listaProcs, pidRemovido);
					ualarm(0,0);
				}
				else if(pidRemovido == pidProcRealTime){
					int k,I,D;
					int currentTime = (int)(((double)(clock() - startTime)) / CLOCKS_PER_SEC) % 60;
					RealTimeProc * realTimeProcInfo = realTime[currentTime]; // Processo tempo real de dado segundo
					ProcInfo * procMorrendo = realTimeProcInfo->procInfo;
					I = atoi(procMorrendo->I);
					D = atoi(procMorrendo->D);
					pidProcRealTime = -1;
					flag = 1;
					flagRemove = 0;

					for(k = I; k < I+D; k++){
						free(realTime[k]);
						realTime[k] = NULL;
					}
					if(!listaVazia(waitingList)) /* Verifica se há processo esperando o fim deste, se há, coloca ele no vetor de realTime */ {
						RealTimeProc * priorityProcInfo = verificaLista(waitingList,procMorrendo,realTime);
						if(priorityProcInfo != NULL) {
							I = atoi(priorityProcInfo->procInfo->I);
							D = atoi(priorityProcInfo->procInfo->D);
							for(k = I; k < I+D; k++){
								realTime[k] = priorityProcInfo;
							}
						}
					}
					free(procMorrendo);
					alarm(0);
				}
				else { /* Se o processo terminado não é o que está rodando, continua normalmente */
					flagRemove = 0;
				}
			}
		}

		if (flag == 0) /* Procura um processo novo na pipe e trata sua chegada da maneira apropriada*/ {
			if ((procInfo = getProcInfo(fd)) != NULL) /* Há um processo novo*/ {
				printf("\nNome do Processo: %s\n", procInfo->nomeProc);
				printf("Tipo do Processo: %s\n", procInfo->tipoProc);
				printf("Inicio do Processo: %s\n", procInfo->I);
				printf("Duracao do Processo: %s\n", procInfo->D);
				printf("Prioridade do Processo: %s\n", procInfo->PR);
				fflush(stdout);
				char *nome = (char*)malloc(sizeof(char)*(3 + strlen(procInfo->nomeProc)));
				nome[0] = '.';
				nome[1] = '/';
				nome[2] = '\0';
				strcat(nome, procInfo->nomeProc);
				if ((pidProc = fork()) == 0) /* Cria o novo processo */ {
					char *arg[] = { nome,NULL };
					execv(arg[0], arg);
				}
				else {
					kill(pidProc, SIGSTOP); /*interrompe o recém nascido*/
					if (*procInfo->I == '\0') /* Prioridade ou Round Robin */ {
						int prioridade;
						if (*procInfo->PR == '\0') /* Round Robin */ {
							prioridade = 8;
						}
						else /* Prioridade */ { prioridade = atoi(procInfo->PR); }
						printf("\nPrioridade: %d\n", prioridade);
						fflush(stdout);
						listaProcs = insereElemento(listaProcs, pidProc, prioridade);  /* este insereElemento vai inserir na posição correta */
					}
					else /* Real Time*/ {
						if (afterRTProc(procInfo)) /* Process starts after another Real Time process */ {
							int k,I,D;
							char strI[2];
							D = atoi(procInfo->D);
							for(k = 0; k < 60 ; k++){
								if(realTime[k] != NULL){
									if(strcmp(realTime[k]->procInfo->nomeProc, procInfo->I) == 0)/* Achou o processo */  {
										k += atoi(realTime[k]->procInfo->D);
										I = k;

										free(procInfo->I);
										sprintf(strI,"%d",I);
										procInfo->I = (char*)malloc(sizeof(char)*strlen(strI));
										strcpy(procInfo->I, strI);
										for(; k < I + D; k++){
											if(realTime[k] != NULL) /* Já existe um processo que ocorre durante esse tempo */ {
												RealTimeProc *newProc = (RealTimeProc*)malloc(sizeof(RealTimeProc));
												newProc->pid = pidProc;
												newProc->procInfo = procInfo;
												printf("Processos conflitantes, colocando ele na lista de espera");
												insFinal(waitingList, newProc);
												break;
											}
										}
										if(k == (D+I)) /* Processo pode rodar normalmente */{
											for(k = I; k < I + D; k++){
												realTime[k] = (RealTimeProc*)malloc(sizeof(RealTimeProc));
												realTime[k]->pid = pidProc;

												realTime[k]->procInfo = procInfo;
											}
										}
									}
									else{
										k = k + atoi(realTime[k]->procInfo->D);
									}
								}
							}							
						}
						else /* Process starts at arbitrary time */ {
							int k, I, D;
							I = atoi(procInfo->I);
							D = atoi(procInfo->D);
							if(I + D > 60){
								printf("O processo %s possui um tempo de execucao invalido, sera descartado", procInfo->nomeProc);
								continue;
							}
							for(k = I; k < I + D; k++){
								if(realTime[k] != NULL) /* Já existe um processo que ocorre durante esse tempo */ {
									RealTimeProc *newProc = (RealTimeProc*)malloc(sizeof(RealTimeProc));
									newProc->pid = pidProc;
									newProc->procInfo = procInfo;
									printf("Processos conflitantes, colocando ele na lista de espera");
									insFinal(waitingList, newProc);
									break;
								}
							}
							if(k == (D+I)) /* Processo pode rodar normalmente */{
								for(k = I; k < I + D; k++){
									realTime[k] = (RealTimeProc*)malloc(sizeof(RealTimeProc));
									realTime[k]->pid = pidProc;
									realTime[k]->procInfo = procInfo;
								}
							}
						}
					}
				}
			}
		}

		else if (flag == 1) /* Um alarme foi disparado */ {
			int currentTime = (int)(((double)(clock() - startTime)) / CLOCKS_PER_SEC) % 60;
			RealTimeProc * realTimeProcInfo = realTime[currentTime]; // Processo tempo real de dado segundo
			printf("\n\nSegundo atual: %d\n", (int)(((double)(clock() - startTime)) / CLOCKS_PER_SEC) % 60);
			//Parar processo atual
			if(flagTipo != RealTime){
				printf("\n");
				kill(listaProcs->pid, SIGSTOP);
			}
			else{
				printf("\n");
				if(pidProcRealTime != -1){
					kill(pidProcRealTime, SIGSTOP);
				}
			}

			//Comecar proximo processo
			if( realTimeProcInfo != NULL ) /* Ha um processo realTime */{
				kill(realTimeProcInfo->pid,SIGCONT);
				alarm(atoi(realTimeProcInfo->procInfo->D) - (currentTime - atoi(realTimeProcInfo->procInfo->I)));
				pidProcRealTime = realTimeProcInfo->pid;
				flag = 0;
				flagTipo = RealTime;
			}
			else {
				listaProcs = proxElem(listaProcs);
				kill(listaProcs->pid, SIGCONT);
				ualarm(500000, 0);
				flag = 0;
				pidProcRealTime = -1;
				if(listaProcs->prio < 8){
					flagTipo = Prioridade;
				}
				else{
					flagTipo = RoundRobin;
				}
			}
		}
		
	}
	
	close(fd[0]);
	close(fd[1]);
}



ProcInfo * getProcInfo(int fd[]){
	int sizeOfProcInfo;
	char serializedProcInfo[100];
	ProcInfo *procInfo ;

	if((read(fd[0],&sizeOfProcInfo,sizeof(int))) > 0 ) /* Há informação na pipe*/ {
		while (read(fd[0], serializedProcInfo, sizeOfProcInfo) <= 0);
		
		//read(fd[0],&sizeOfProcInfo,sizeof(int));
		//read(fd[0],serializedProcInfo,sizeOfProcInfo);

		procInfo = deserializeProcInfo(serializedProcInfo);
		return procInfo;
	}
	else{
		return NULL;
	}
}

void AlrmHandler(int sinal)
{
	flag = 1;
}

void ChildHandler(int sinal) {
	int pidf;
	pidf = waitpid((pid_t)(-1), 0, WNOHANG); /*WNOHANG é para que ele retorne caso não ache que imediato um filho que terminou*/
	
	if (pidf > 0) {
		flagRemove = pidf; /*Se algum filho terminou, coloca seu pid na flag*/
	}
}

int afterRTProc(ProcInfo * procInfo){
	char *c;
	for(c=procInfo->I ; *c ; c++){

		if(*c < 48 || *c > 57) return 1;
	}
	return 0;
} 


