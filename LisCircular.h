typedef struct no {
	//void *pid;		//Ponteiro pro programa
	int pid;
	int prio;			//prioridade do processo
	struct no *prox;	//Ponteiro pro proximo no
	struct no *ant;		//Ponteiro pro no anterior
}No;

/*Funcao que cria a lista com um �nico elemento nulo*/
No* CriaLista();

/*Funcao que insere um elemento*/
No* insereElemento(No *lista, int pid, int prio);

/*Funcao que remove um elemento da lista*/
No* removeElemento(No *lista, int pid);

/*Funcao que avanca o no corrente*/
No* proxElem(No *lista);

/*Funcao que recua o no corrente*/
No* antElem(No *lista);

/*Funcao que libera a lista inteira*/
void destroiLista(No *lista);

/*Funcao que imprime a lista inteira*/
void printaListaCirc(No *lista);

