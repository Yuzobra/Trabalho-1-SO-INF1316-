#define KEY_V 4242
#define KEY_C 5050
#define SIZE 50


#include <unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <signal.h>

//#include<sys/ipc.h>
//#include<sys/shm.h>
//#include<sys/stat.h>

#include "procInfo.h"
#include "LisCircular.h"

//#define NUM_MAX_DE_PROCESSOS 16;

ProcInfo * getProcInfo(int fd[]);




int main (int argc, char *argv[]) { 
	
	int pid, i,fd[2], sizeOfProcInfo;
	char serializedProcInfo[100];

	//int s = shmget(IPC_PRIVATE, NUM_MAX_DE_PROCESSOS * (sizeof(No)), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

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
		 	char path[] = "/root/Codes/SO/Trab1/reader"; 
			char * arr[3];
		 	sprintf(arg,"%d",fd[1]);
			arr[0] = path;
			arr[1] = arg;
			arr[2] = NULL;
			//close(fd[0]);
			execv("./reader", arr);
		}
		
		else if(i == 1) /* processo 2 */{
		/* Este processo irá ficar monitorando o pipe para verificar se há algum processo novo a ser adicionado à lista */
			char path[] = "/root/Codes/SO/Trab1/";
			ProcInfo *procInfo;
			No * lista;
			printf("Iniciando processo 2\n");

			while(1){
				//close(fd[1]);
				procInfo = getProcInfo(fd);
				printf("Nome do Processo: %s\n", procInfo->nomeProc);
				printf("Tipo do Processo: %s\n", procInfo->tipoProc);
				strcat(path,procInfo->NomeProc);
				if ((pid = fork()) == 0) {
					execv("path");
				}
				kill(pid, SIGSTOP);
				if (lista == NULL) {
					lista = criaLista(pid);
				}
				else {
					lista = insereElemento(lista, pid);
				}

			}
		}
		
		else if(i == 2) /* processo 3 */{
		/* Este processo cuida do escalonamento em si */
			printf("Iniciando processo 3\n");				
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

	
	read(fd[0],&sizeOfProcInfo,sizeof(int));
	read(fd[0],serializedProcInfo,sizeOfProcInfo);
	
	
	//read(fd[0],&sizeOfProcInfo,sizeof(int));
	//read(fd[0],serializedProcInfo,sizeOfProcInfo);

	procInfo = deserializeProcInfo(serializedProcInfo);
	return procInfo;
}

