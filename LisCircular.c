#include <stdlib.h>
#include <stdio.h>
#include "LisCircular.h"

/*Fun��o que cria a lista com um �nico elemento nulo*/
No * CriaLista(){
	No *novo;
	novo = (No*)malloc(sizeof(No));
	novo->pid = 0;
	novo->prio = 0;
	novo->prox = novo;
	novo->ant = novo;
	return novo;
}

/*Fun��o que acha a posi��o da lista na qual um elemento deve ser inserido*/
No * achaPos(No *lista, int prio){
	int flag = 0;
	No *aux = lista;
	while(1){
		if(flag!=0){
			if(lista==aux){return lista;}	//se eu j� dei a volta inteira na lista, coloco aonde estou (lista com todos os elementos de mesma prioridade)
		}									//a flag � apenas para que esse check n�o seja feito na primeira itera��o
		flag = 1;
		if(lista->prio > prio){				//se o atual for maior que eu, avança, mas antes:
			if (lista->prox->prio < lista->prio) { //verifica se o próximo é menor que o atual, se for, sou o menor da lista 
				return lista;
			}
			lista = lista->prox;
		}
		else{								//se o atual for menor que eu, avança, mas antes:
			if (lista->prox->prio > prio || lista->prox->prio < lista->prio) {	//verifica se o próximo é maior que o atual, se for, estou no lugar certo
				return lista;				//(também verifica se a do próximo é menor que o atual, pois se for, sou o maior da lista)
			}
			lista = lista->prox;
		}
	}
}

/*Fun��o que insere um elemento*/
No * insereElemento(No *lista,int pid,int prio) {
	No *novo;
	No *aux = lista;
	if(lista->pid==0){				//Se for o primeiro elemento a ser inserido, apenas insere na posi��o j� alocada
		lista->pid=pid;
		lista->prio=prio;
		return lista;
	}
	lista = achaPos(lista,prio);	//chama a fun��o que acha a posi��o na qual o elemento deve ser inserido
	novo = (No*)malloc(sizeof(No));
	novo->pid = pid;
	novo->prox = lista->prox;
	lista->prox = novo;
	novo->ant = lista;
	novo->prox->ant = novo;
	novo->prio = prio;
	return aux;
}

/*Fun��o que remove um elemento da lista*/
No * removeElemento(No *lista, int pid) {
	No *aux;
	No *pos = lista;
	if (lista->prox == lista) { //se estou tentando remover o �ltimo elemento, o elemento � apenas transformado em um elemento vazio
		lista->pid = 0;
		lista->prio = 0;
		return lista;
	}
	while (lista->pid != pid) {
		lista = lista->prox;
	}
	if (lista == pos) {
		pos = lista->ant;
	}
	aux = lista->ant;
	aux->prox = lista->prox;
	lista->prox->ant = aux;
	free(lista);
	return pos;				//retorna a lista aonde eu estava anteriormente (ou no N� anterior, se o atual foi removido)
}

/*Fun��o que avan�a o n� corrente*/
No * proxElem(No *lista){
	return lista->prox;
}

/*Fun��o que recua o n� corrente*/
No * antElem(No *lista){
	return lista->ant;
}

/*Fun��o que libera a lista inteira*/
void destroiLista(No *lista) {
	while (lista->pid) {				//quando o pid for zero, � por que s� sobrou o elemento final (vazio)
		lista = removeElemento(lista,lista->pid);
	}
	free(lista);
	return;
}

/*Fun��o que imprime a lista inteira para prop�sitos de teste (podemos remover depois)*/
void printaListaCirc(No *lista) {
	int flag = 0;
	No *aux = lista;
	printf("\nPrioridades e Round Robins:\n");
	while (lista->prio > 0) {
		if(flag!=0){
			if(lista==aux){
				printf("\n");
				return;
			}
		}
		flag=1;
		if(lista->prio!=8){
			printf("{%d ,prio: %d}\n", lista->pid, lista->prio);
		}
		else {
			printf("{%d ,round robin}\n", lista->pid);
		}
		lista = lista->prox;
	}
	printf("\n");
	return;
}