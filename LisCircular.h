typedef struct no {
	//void *pid;		//Ponteiro pro programa
	int pid;
	int prio;			//prioi��o Na Lista
	struct no *prox;	//Ponteiro pro pr�ximo n�
	struct no *ant;		//Ponteiro pro n� anterior
}No;

/*Fun��o que cria a lista com um �nico elemento nulo*/
No* CriaLista();

/*Fun��o que insere um elemento*/
No* insereElemento(No *lista, int pid, int prio);

/*Fun��o que remove um elemento da lista*/
No* removeElemento(No *lista, int pid);

/*Fun��o que avan�a o n� corrente*/
No* proxElem(No *lista);

/*Fun��o que recua o n� corrente*/
No* antElem(No *lista);

/*Fun��o que libera a lista inteira*/
void destroiLista(No *lista);

/*Fun��o que imprime a lista inteira para prop�sitos de teste (podemos remover depois)*/
void printaLista(No *lista);

