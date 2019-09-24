#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"


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




ProcInfo * readCommand(char* line) {
	char* command = strtok(line, " ");
	char* nomeProc = strtok(NULL, " \n");
	char* tipoProc = strtok(NULL, "=");
	ProcInfo * procInfo = (ProcInfo *)malloc(sizeof(ProcInfo));
	if (strcmp(command, "Run") == 0) {
		if(tipoProc == NULL) /* ROUND ROBIN */ {
			procInfo->tipoProc = (char*)malloc(12 * sizeof(char));
			strcpy(procInfo->tipoProc, "Round Robin");

			procInfo->nomeProc = (char*)malloc((strlen(nomeProc) + 1) * sizeof(char));
			strcpy(procInfo->nomeProc, nomeProc);

			procInfo->I = (char*)malloc(1 * sizeof(char));
			strcpy(procInfo->I, "");

			procInfo->D = (char*)malloc(1 * sizeof(char));
			strcpy(procInfo->D, "");

			procInfo->PR = (char*)malloc(1 * sizeof(char));
			strcpy(procInfo->PR, "");
			
			
		}
		else if (strcmp(tipoProc, "I") == 0) /* REAL TIME */ {
			char* I, * D;
			I = strtok(NULL, " ");
			strtok(NULL, "=");
			D = strtok(NULL, " ");
			
			procInfo->tipoProc = (char*)malloc(10 * sizeof(char));
			strcpy(procInfo->tipoProc, "Real Time");

			procInfo->nomeProc = (char*)malloc((strlen(nomeProc) + 1) * sizeof(char));
			strcpy(procInfo->nomeProc, nomeProc);

			procInfo->I = (char*)malloc((strlen(I) + 1) * sizeof(char));
			strcpy(procInfo->I, I);

			procInfo->D = (char*)malloc((strlen(D) + 1) * sizeof(char));
			strcpy(procInfo->D, D);

			procInfo->PR = (char*)malloc(1 * sizeof(char));
			strcpy(procInfo->PR, "");

		}
		else if (strcmp(tipoProc, "PR") == 0) /* PRIORIDADE */ {
			char* PR;
			PR = strtok(NULL, " ");

			procInfo->tipoProc = (char*)malloc(11 * sizeof(char));
			strcpy(procInfo->tipoProc, "Prioridade");

			procInfo->nomeProc = (char*)malloc((strlen(nomeProc) + 1) * sizeof(char));
			strcpy(procInfo->nomeProc, nomeProc);

			procInfo->PR = (char*)malloc((strlen(PR) + 1) * sizeof(char));
			strcpy(procInfo->PR, PR);

			procInfo->I = (char*)malloc(1 * sizeof(char));
			strcpy(procInfo->I, "");

			procInfo->D = (char*)malloc(1 * sizeof(char));
			strcpy(procInfo->D, "");

		}
		
	}
	else {

	}
	return procInfo;
}



int main(void) {
	FILE* listaProc;
	char s[50];
	ProcInfo * procInfo = NULL;
	listaProc = fopen("processos.txt", "r");
	if (listaProc == NULL) {
		printf("Erro ao abrir arquivo \n");
		exit(1);
	}

	while (1) {
		fgets(s, 50, listaProc); //TODO
		if (feof(listaProc)) {
			break;
		}
		procInfo = readCommand(s);
		printf("%s\n",procInfo->nomeProc);
		printf("%s\n",procInfo->tipoProc);
	}

	fclose(listaProc);
	return 0;
}
