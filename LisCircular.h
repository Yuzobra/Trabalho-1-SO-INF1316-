typedef struct no {
	//void *pid;		//Ponteiro pro programa
	int pid;
	int prio;			//prioição Na Lista
	struct no *prox;	//Ponteiro pro próximo nó
	struct no *ant;		//Ponteiro pro nó anterior
}No;

/*Função que cria a lista com um único elemento nulo*/
No* CriaLista();

/*Função que insere um elemento*/
No* insereElemento(No *lista, int pid, int prio);

/*Função que remove um elemento da lista*/
No* removeElemento(No *lista, int pid);

/*Função que avança o nó corrente*/
No* proxElem(No *lista);

/*Função que recua o nó corrente*/
No* antElem(No *lista);

/*Função que libera a lista inteira*/
void destroiLista(No *lista);

/*Função que imprime a lista inteira para propósitos de teste (podemos remover depois)*/
void printaLista(No *lista);

