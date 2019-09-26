typedef struct no {
	//void *pProg;		//Ponteiro pro programa
	int pProg;
	int pos;			//Posição Na Lista
	struct no *prox;	//Ponteiro pro próximo nó
	struct no *ant;		//Ponteiro pro nó anterior
}No;

No* CriaLista(int pProg);

No* insereElemento(No *lista, int pProg);

No* removeElemento(No *lista);

No* proxElem(No *lista);

No* antElem(No *lista);

void destroiLista(No *lista);

void printaLista(No *lista);

