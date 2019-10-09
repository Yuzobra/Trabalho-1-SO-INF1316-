#define KEY_V 4242
#define KEY_C 5050
#define SIZE 50


#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <signal.h>

#include "procInfo.h"
#include "LisCircular.h"

ProcInfo * getProcInfo(int fd[]);

void AlrmHandler(int sinal);
int afterRTProc(ProcInfo *);

int flag = 0; //flag que diz se o processo deve escalonar ou olhar a pipe


int main (int argc, char *argv[]) { 
	
	int pid, i,fd[2], sizeOfProcInfo, realTime[60];
	char serializedProcInfo[100];
	
	if(pipe(fd) < 0){
			printf("Erro ao criar pipe");
			exit(1);		
	}
	fcntl(fd[0], F_SETFL, O_NONBLOCK); //Set read fd as Non-Blocking
	
	
	for(i=0; i<3 ;i++){
		if((pid = fork()) < 0){
			printf("Erro ao criar processo %d\n",i);
			exit(1);
		}
		if(pid == 0) break; //processo filho
	}

	if(pid == 0){

		if(i == 0) /* processo 1 */{
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
		
		else if(i == 1) /* processo 2 */{
			No * listaProcs = CriaLista();
			int pidProc;
			ProcInfo *procInfo;
			signal(SIGALRM, AlrmHandler); 
			alarm(1);
			while(1){
				if(flag==0) /* Procura um processo novo na pipe e trata sua chegada da maneira apropriada*/ {
					if((procInfo = getProcInfo(fd)) != NULL) /* Há um processo novo*/ {
						printf("\nNome do Processo: %s\n", procInfo->nomeProc);
						printf("Tipo do Processo: %s\n", procInfo->tipoProc);
						fflush(stdout);

						char *nome = (char*) malloc (sizeof(char)*(3+strlen(procInfo->nomeProc))); 
						nome[0] = '.';
						nome[1] = '/';
						nome[2] = '\0';
						
						strcat(nome,procInfo->nomeProc);
						if ((pidProc = fork()) == 0) /* Cria o novo processo */ {
							char *arg[] = {nome,NULL};
							execv(arg[0], arg);
						}
						else{
							kill(pidProc, SIGSTOP); /*interrompe o recém nascido*/
							if (*procInfo->I == '\0') /* Prioridade ou Round Robin */ {
								int prioridade;
								if (*procInfo->PR == '\0') /* Round Robin */ { 
									prioridade = 8; }
								else /* Prioridade */ { prioridade = atoi(procInfo->PR); }
								printf("\nPrioridade: %d\n", prioridade);
								fflush(stdout);
								listaProcs = insereElemento(listaProcs, pidProc, prioridade);  /* este insereElemento vai inserir na posição correta */
							}
							else /* Real Time*/ {
								if (afterRTProc(procInfo)) /* Process starts after another Real Time process */ {

								}
								else /* Process starts at arbitrary time */ {

								}

							}
						}
					}
				}

				else /* Um processo acabou de ser interrompido */ {					
					
					printf("\n");
					kill(listaProcs->pid, SIGSTOP);
					listaProcs = proxElem(listaProcs);
					kill(listaProcs->pid, SIGCONT);
					ualarm(500000,0);
					flag = 0;

				}
			}	
		}
		
		else if(i == 2) /* processo 3 */{
					/*
					Este processo cuida do escalonamento
					
						-Deve enviar alarmes ao processo 2 quanto um processo for interrompido
					
					*/
		} 
	}
	else{
	wait(NULL);
	}
	
	close(fd[0]);
	close(fd[1]);
}



ProcInfo * getProcInfo(int fd[]){
	int sizeOfProcInfo;
	char serializedProcInfo[100];
	ProcInfo *procInfo ;

	if((read(fd[0],&sizeOfProcInfo,sizeof(int))) > 0 ) /* Há informação na pipe*/ {
		read(fd[0],serializedProcInfo,sizeOfProcInfo);		
		
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

int afterRTProc(ProcInfo * procInfo){
	char *c;
	for(c=procInfo->I ; *c ; c++){
		if(*c < 48 || *c > 57) return 0;
	}
	return 1;
}


