typedef struct lista Lista;
typedef struct realTimeProc RealTimeProc;
typedef struct procInfo ProcInfo;

Lista * criaLista();

void freeLista(Lista * lista);

void insFinal(Lista * lista, RealTimeProc *realTimeProc);

int listaVazia(Lista * lista);

RealTimeProc * verificaLista(Lista * lista, ProcInfo * procMorrendo, RealTimeProc * realTime[]); /* Verifica se há algum processo esperando o fim do processo terminando */

void removerDependentes(Lista * lista, RealTimeProc * procMorrendo);

void printListaEnc(Lista * lista);