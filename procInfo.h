typedef struct procInfo{
	char * tipoProc;
	char * nomeProc;
	char * I;
	char * D;
	char * PR;
} ProcInfo;

typedef enum tipoProc{
	RealTime, Prioridade, RoundRobin
} TipoProc;

ProcInfo * readCommand(char* line);
char * serializeProcInfo(ProcInfo * procInfo,int * buf_size);
ProcInfo * deserializeProcInfo(char * buf);
