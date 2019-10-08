typedef struct no {
	//void *pid;		//Ponteiro pro programa
	int pid;
	int prio;			//prioição Na Lista
	struct no *prox;	//Ponteiro pro próximo nó
	struct no *ant;		//Ponteiro pro nó anterior
}No;

No* CriaLista();

No* insereElemento(No *lista, int pid, int prio);

No* removeElemento(No *lista);

No* proxElem(No *lista);

No* antElem(No *lista);

void destroiLista(No *lista);

void printaLista(No *lista);

