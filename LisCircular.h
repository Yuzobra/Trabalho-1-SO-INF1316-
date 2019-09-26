typedef struct no {
	//void *pProg;		//Ponteiro pro programa
	int pProg;
	int pos;			//Posi��o Na Lista
	struct no *prox;	//Ponteiro pro pr�ximo n�
	struct no *ant;		//Ponteiro pro n� anterior
}No;

No* CriaLista(int pProg);

No* insereElemento(No *lista, int pProg);

No* removeElemento(No *lista);

No* proxElem(No *lista);

No* antElem(No *lista);

void destroiLista(No *lista);

void printaLista(No *lista);

