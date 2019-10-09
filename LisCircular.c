#include <stdlib.h>
#include <stdio.h>
#include "LisCircular.h"

/*Função que cria a lista com um único elemento nulo*/
No * CriaLista(){
	No *novo;
	novo = (No*)malloc(sizeof(No));
	novo->pid = 0;
	novo->prio = 0;
	novo->prox = novo;
	novo->ant = novo;
	return novo;
}

/*Função que acha a posição da lista na qual um elemento deve ser inserido*/
No * achaPos(No *lista, int prio){
	int flag = 0;
	No *aux = lista;
	while(1){
		if(flag!=0){
			if(lista==aux){return lista;}	//se eu já dei a volta inteira na lista, coloco aonde eu estou (lista com todos os elementos de mesma prioridade)
		}									//a flag é apenas para que esse check não seja feito na primeira iteração
		flag = 1;
		if(lista->prio > prio){				//se a prioridade do elemento corrente for maior, vai pro anterior
			lista = lista->ant;
		}
		else {								//se não for, vai para o próximo, mas antes:
			if(lista->prox->prio > prio){	//checa se a prioridade do próximo é maior que a dele mesmo, se for é ali mesmo que deve ser inserido
				return lista;
			}
			lista = lista->prox;
		}
	}
}

/*Função que insere um elemento*/
No * insereElemento(No *lista,int pid,int prio) {
	No *novo;
	No *aux = lista;
	if(lista->pid==0){				//Se for o primeiro elemento a ser inserido, apenas insere na posição já alocada
		lista->pid=pid;
		lista->prio=prio;
		return lista;
	}
	lista = achaPos(lista,prio);	//chama a função que acha a posição na qual o elemento deve ser inserido
	novo = (No*)malloc(sizeof(No));
	novo->pid = pid;
	novo->prox = lista->prox;
	lista->prox = novo;
	novo->ant = lista;
	novo->prox->ant = novo;
	novo->prio = prio;
	return aux;
}

/*Função que remove o elemento atual da lista*/
No * removeElemento(No *lista) {
	No *aux;
	if(lista->prox==lista){ //se estou tentando remover o último elemento, o elemento é apenas transformado em um elemento vazio
		lista->pid=0;
		lista->prio=0;
		return lista;
	}
	aux = lista->ant;
	aux->prox=lista->prox;
	lista->prox->ant=aux;
	free(lista);
	return aux;
}

/*Função que avança o nó corrente*/
No * proxElem(No *lista){
	return lista->prox;
}

/*Função que recua o nó corrente*/
No * antElem(No *lista){
	return lista->ant;
}

/*Função que libera a lista inteira*/
void destroiLista(No *lista) {
	while (lista->pid) {				//quando o pid for zero, é por que só sobrou o elemento final (vazio)
		lista = removeElemento(lista);
	}
	free(lista);
	return;
}

/*Função que imprime a lista inteira para propósitos de teste (podemos remover depois)*/
void printaLista(No *lista) {
	int flag = 0;
	No *aux = lista;
	while (lista->prio > 0) {
		if(flag!=0){
			if(lista==aux){
				printf("\n");
				return;
			}
		}
		flag=1;
		printf("{%d ,prio: %d, ant: %d, prox:%d}\t", lista->pid, lista->prio, lista->ant->pid, lista->prox->pid);
		lista = lista->prox;
	}
	printf("\n");
	return;
}