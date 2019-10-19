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
void ContHandler(int sinal);
void PauseHandler(int sinal);
void ShowHandler(int sinal);
int afterRTProc(ProcInfo *);

int flag = 0; //flag que diz se o processo deve escalonar ou olhar a pipe
int flagRemove = 0;
int flagreader = 1;
TipoProc flagTipo = -1; //flag que diz o tipo do processo que terminou a sua execução
No * listaProcs;
int pidProcRealTime;
clock_t startTime;
int pidreader;

typedef struct realTimeProc{
	int pid;
	int pidAnt;
	ProcInfo * procInfo;
} RealTimeProc;


int main (int argc, char *argv[]) { 
	
	int fd[2], i;
	pidProcRealTime = -1;
	RealTimeProc * realTime[60];
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
		printf("Iniciando Interpretador\n");
	 	char arg[12];
		char * arr[3];
	 	sprintf(arg,"%d",fd[1]);
		arr[0] = "./reader";
		arr[1] = arg;
		arr[2] = NULL;
		close(fd[0]);
		execv(arr[0], arr);
	}

	close(fd[1]);

	for( i = 0; i < 60; i++ ){
		realTime[i] = NULL;
	}

	/*A partir daqui é o escalonador*/
	listaProcs = CriaLista();
	int pidProc;
	ProcInfo *procInfo;
	signal(SIGALRM, AlrmHandler);
	signal(SIGCHLD, ChildHandler);
	signal(SIGCONT, ContHandler);
	signal(SIGUSR1, PauseHandler);
	signal(SIGUSR2, ShowHandler);
	alarm(1);
	startTime = clock();
	while (1) {
		fflush(stdout);
		if (flagRemove > 1) { /* Um processo foi terminado (flagRemove == pid do processo terminado)*/
			int pidRemovido = flagRemove;

			if (pidRemovido == pidreader) {
				printf("\nInterpretador terminou\n");
				flagreader = 0; /*flag que diz se o reader já terminou, pro handler de alarmes saber se precisa lidar com isso*/
				flagRemove = 0;
			}
			else {
				printf("\nRemovendo Processo\n");
				if (pidRemovido == listaProcs->pid ) { /* Se o processo é prioridade ou roundrobin, desliga o alarme, remove da lista e fala pra próxima iteração do while escalonar*/
					flagRemove = 0;
					flag = 1;
					listaProcs = removeElemento(listaProcs, pidRemovido);
					ualarm(0,0);
				}
				else if(pidRemovido == pidProcRealTime){
					int k,I,D;
					int currentTime = (int)(((double)(clock() - startTime)) / CLOCKS_PER_SEC) % 60;
					RealTimeProc * realTimeProcInfo = realTime[currentTime]; // Processo real time de dado segundo
					ProcInfo * procMorrendo = realTimeProcInfo->procInfo;
					ProcInfo * procDependente = NULL;
					I = atoi(procMorrendo->I);
					D = atoi(procMorrendo->D);
					pidProcRealTime = -1;
					flag = 1;
					flagRemove = 0;
					if(realTime[I+D] != NULL && realTime[I+D]->pidAnt == realTime[I]->pid) /* Um processo era dependente do que morreu */{
						procDependente = realTime[I+D]->procInfo; // Pega informação do processo depentende
						D = D + atoi(realTime[I+D]->procInfo->D); // Soma a duração do processo dependente
					}
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
						if(procDependente != NULL){
							priorityProcInfo = verificaLista(waitingList,procDependente,realTime);
							if(priorityProcInfo != NULL) {
								I = atoi(priorityProcInfo->procInfo->I);
								D = atoi(priorityProcInfo->procInfo->D);
								for(k = I; k < I+D; k++){
									realTime[k] = priorityProcInfo;
								}
							}
						}
					}
					free(procMorrendo);
					if(procDependente != NULL) free(procDependente);
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
				char *nome = (char*)malloc(sizeof(char)*(5 + strlen(procInfo->nomeProc)));
				nome[0] = '.';
				nome[1] = '/';
				nome[2] = 'P';
				nome[3] = '/';
				nome[4] = '\0';
				
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
						fflush(stdout);
						listaProcs = insereElemento(listaProcs, pidProc, prioridade);  /* este insereElemento vai inserir na posição correta */
					}
					else /* Real Time*/ {
						if (afterRTProc(procInfo)) /* Process starts after another Real Time process */ {
							int k,I,D;
							char strI[40];
							D = atoi(procInfo->D);
							for(k = 0; k < 60 ; k++){
								if(realTime[k] != NULL){
									if(strcmp(realTime[k]->procInfo->nomeProc, procInfo->I) == 0)/* Achou o processo */  {
										int pidAnt = realTime[k]->pidAnt;
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
												newProc->pidAnt = pidAnt;
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
			fflush(stdout);
			int currentTime = (int)(((double)(clock() - startTime)) / CLOCKS_PER_SEC) % 60;
			RealTimeProc * realTimeProcInfo = realTime[currentTime]; // Processo tempo real de dado segundo
			printf("\n\nSegundo atual: %d", (int)(((double)(clock() - startTime)) / CLOCKS_PER_SEC) % 60);
			//Parar processo atual
			if(flagTipo != RealTime){
				printf("\n");
				printf("Parando o processo de pid: %d\n", listaProcs->pid);
				printf("Retorno do SIGSTOP: %d\n",kill(listaProcs->pid, SIGSTOP));
			}
			else{
				printf("\n");
				if(pidProcRealTime != -1){
					kill(pidProcRealTime, SIGSTOP);
				}
			}

			//Comecar proximo processo
			if( realTimeProcInfo != NULL ) /* Ha um processo realTime */ {
				printf("Iniciando o processo: %s\n",realTimeProcInfo->procInfo->nomeProc);
				kill(realTimeProcInfo->pid,SIGCONT);
				alarm(atoi(realTimeProcInfo->procInfo->D) - (currentTime - atoi(realTimeProcInfo->procInfo->I)));
				pidProcRealTime = realTimeProcInfo->pid;
				flag = 0;
				flagTipo = RealTime;
			}
			else {
				printf("PID DO PROCESSO ANTERIOR: %d\n", listaProcs->pid);
				listaProcs = proxElem(listaProcs);
				printf("PID DO PROCESSO ATUAL: %d\n", listaProcs->pid);
				kill(listaProcs->pid, SIGCONT);
				if(listaProcs->prio<8){
					printf("\nIniciando um processo de prioridade: %d\n", listaProcs->prio);
				}
				else {
					printf("\nIniciando um round robin:\n");
				}
				ualarm(500000, 0);
				printf("Tempo depois do ualarm: %d\n", ualarm(500000,0));

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

void AlrmHandler(int sinal){
	printf("ALARM HANDLER\n");

	fflush(stdout);
	flag = 1;
}

void ChildHandler(int sinal) {
	printf("CHILD HANDLER\n");
	
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

void PauseHandler(int sinal) {
	printf("mandei pausar\n");
	if (pidProcRealTime != -1) {
		kill(pidProcRealTime, SIGTSTP);
	}
	kill(listaProcs->pid, SIGTSTP);
	if (flagreader==1) {
		kill(pidreader, SIGTSTP);
	}
	printf("\n--------------PAUSED--------------\n");
	raise(SIGTSTP);
}

void ContHandler(int sinal) {
	if (flagreader==1) {
		kill(pidreader, SIGCONT);
	}
	ualarm(0, 0);
	flag = 1;
}

void ShowHandler(int sinal) {
	if (flagreader == 1) {
		kill(pidreader, SIGTSTP);
	}
	printf("\n---------------SHOW---------------\n");
	printf("\n\nSegundo atual: %d\n", (int)(((double)(clock() - startTime)) / CLOCKS_PER_SEC) % 60);
	printaLista(listaProcs);
	/*SE PUDER, ADICIONE UMA FUNÇÂO QUE EXIBE OS PROCESSOS DE REAL TIME*/
	fflush(stdout);
	raise(SIGTSTP);
}