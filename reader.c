#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "procInfo.h"


/*
*
*	Retorna um vetor de strings com as informações do processo.
*
*		-Caso Run:
* 		->Caso Real Time:
*				retorna ["Real Time", nomeDoProcesso, tempoDeInicio, duracao]
* 		->Caso Prioridade:
*				retorna ["Prioridade", nomeDoProcesso, prioridade, ""]
* 		->Caso Round Robin:
*				retorna ["Round Robin", nomeDoProcesso, "", ""]
*
*
*/


int main(int argc, char * argv[]) {
	char * buf;
	int buf_size;
	char s[50];
	FILE* listaProc;
	int pipeWriterNum;
	ProcInfo * procInfo = NULL;
	listaProc = fopen("processos.txt", "r");
	
	pipeWriterNum = atoi(argv[1]);
	
	if (listaProc == NULL) {
		printf("Erro ao abrir arquivo \n");
		exit(1);
	}

	while (1) {
		fgets(s, 50, listaProc);
		if (feof(listaProc)) {
			break;
		}
		procInfo = readCommand(s);
		buf = serializeProcInfo(procInfo,&buf_size);
		/*
		printf("buf_size: %d\n",buf_size);
		printf("primeiro valor de buf: %s\n",buf);
		printf("Bytes escritos no pipe: %d\nValor: %d\n",write(pipeWriterNum, &buf_size, sizeof(int)),buf_size);
		printf("Bytes escritos no pipe: %d\nValor: %s\n",write(pipeWriterNum, buf, buf_size),buf);
		*/
		write(pipeWriterNum, &buf_size, sizeof(int));
		write(pipeWriterNum, buf, buf_size);

		sleep(4);
	}
	
	fclose(listaProc);
	return 0;
}
