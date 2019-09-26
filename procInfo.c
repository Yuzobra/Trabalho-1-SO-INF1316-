#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "procInfo.h"

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



char * serializeProcInfo(ProcInfo * procInfo,int * buf_size){
	int n;
	n = strlen(procInfo->nomeProc) + strlen(procInfo->tipoProc) + strlen(procInfo->I) + strlen(procInfo->D) + strlen(procInfo->PR) + 5;
	*buf_size = n;
	char * buf = (char*)malloc(n);
	strcpy(buf,procInfo->nomeProc);
	strcpy(&(buf[strlen(procInfo->nomeProc)+1]),procInfo->tipoProc);
	strcpy(&(buf[strlen(procInfo->nomeProc)+strlen(procInfo->tipoProc)+2]),procInfo->I);
	strcpy(&(buf[strlen(procInfo->nomeProc)+strlen(procInfo->tipoProc)+ strlen(procInfo->I) +3]),procInfo->D);
  strcpy(&(buf[strlen(procInfo->nomeProc)+strlen(procInfo->tipoProc)+ strlen(procInfo->I)+ strlen(procInfo->D) +4]),procInfo->PR);
	return buf;
}


ProcInfo * deserializeProcInfo(char * buf){
	ProcInfo * procInfo = (ProcInfo*)malloc(sizeof(ProcInfo));
	
	
	
	procInfo->nomeProc = (char*)malloc(sizeof(char)*(strlen(buf)+1));
	strcpy(procInfo->nomeProc,buf);
	
	
	procInfo->tipoProc = (char*)malloc(sizeof(char)*(strlen(&(buf[strlen(procInfo->nomeProc)+1]))+1));
	strcpy(procInfo->tipoProc,&(buf[strlen(procInfo->nomeProc)+1]));
	
	procInfo->I = (char*)malloc(sizeof(char)*(strlen(&(buf[strlen(procInfo->nomeProc)+strlen(procInfo->tipoProc)+2]))+1));
	strcpy(procInfo->I,&(buf[strlen(procInfo->nomeProc)+strlen(procInfo->tipoProc)+2]));
	
	procInfo->D = (char*)malloc(sizeof(char)*(strlen(&(buf[strlen(procInfo->nomeProc)+strlen(procInfo->tipoProc)+ strlen(procInfo->I) +3]))+1));
	strcpy(procInfo->D,&(buf[strlen(procInfo->nomeProc)+strlen(procInfo->tipoProc)+ strlen(procInfo->I) +3]));
	
	procInfo->PR = (char*)malloc(sizeof(char)*(strlen(&(buf[strlen(procInfo->nomeProc)+strlen(procInfo->tipoProc)+ strlen(procInfo->I)+ strlen(procInfo->D) +4]))+1));
  strcpy(procInfo->PR,&(buf[strlen(procInfo->nomeProc)+strlen(procInfo->tipoProc)+ strlen(procInfo->I)+ strlen(procInfo->D) +4]));
	
	return procInfo;
}

