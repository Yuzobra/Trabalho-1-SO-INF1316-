#define KEY_V 4242
#define KEY_C 5050
#define SIZE 50


#include <unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <signal.h>

#include "procInfo.h"
#include "LisCircular.h"

ProcInfo * getProcInfo(int fd[]);




void AlrmHandler(int sinal);

int flag = 0; //flag que diz se o processo deve escalonar ou olhar a pipe

int main (int argc, char *argv[]) { 
	
	int pid, i,fd[2], sizeOfProcInfo;
	char serializedProcInfo[100];

	if(pipe(fd) < 0){
			printf("Erro ao criar pipe");
			exit(1);		
	}
	
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
			while(1){
				if(flag==0) /* Pode se inserir um novo processo na lista de processos*/ {
					
					procInfo = getProcInfo(fd);									//precisamos fazer isso ser non-blocking (trocar pra FIFO talvez?)
					printf("Nome do Processo: %s\n", procInfo->nomeProc);
					printf("Tipo do Processo: %s\n", procInfo->tipoProc);
					
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
						kill(pidProc, SIGSTOP); //interrompe o recém nascido
						if(procInfo->I==NULL) /* Prioridade ou Round Robin */ {
							int prioridade;
							if(procInfo->PR == NULL) /* Round Robin */ {prioridade = 8;}
							else /* Prioridade */{prioridade = atoi(procInfo->PR);}
							listaProcs = insereElemento(listaProcs, pid, prioridade);  //este insereElemento vai inserir na posição correta (?)
						}
						else /* Real Time*/ {
						
						
						}
					}
					
				}
				else /* Um processo acabou de ser interrompido (é para isso que serve esse bloco né ?? || honestamente não me lembro mais ) */ {
					
					/*
					OBS:ISTO DEVE SER ENVOLTO EM UM MUTEX, PARA QUE NAO SEJA INTERROMPIDO CASO HAJA UM NOVO SIGALRM
					O SIGALRM TMB DEVE SER ENVOLTO NESTE MUTEX, PARA QUE AGUADE O FIM DA EXECUÇÃO DESTE BLOCO
					
					
					
					
					*/
					
					flag = 0;
				}
			}	
		}
		
		else if(i == 2) /* processo 3 */{
					
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
	ProcInfo *procInfo = (ProcInfo*)malloc(sizeof(ProcInfo));

	printf("Estou lendo do pipe\n");
	read(fd[0],&sizeOfProcInfo,sizeof(int));
	read(fd[0],serializedProcInfo,sizeOfProcInfo);
	
	
	//read(fd[0],&sizeOfProcInfo,sizeof(int));
	//read(fd[0],serializedProcInfo,sizeOfProcInfo);

	procInfo = deserializeProcInfo(serializedProcInfo);
	return procInfo;
}


void AlrmHandler(int sinal)
{
	flag = 1;
}

