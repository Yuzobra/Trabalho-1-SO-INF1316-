#include <stdlib.h>
#include <stdio.h>
#include "LisCircular.h"



No * CriaLista(){
	No *novo;
	novo = (No*)malloc(sizeof(No));
	novo->pid = 0;
	novo->prio = 0;
	novo->prox = novo;
	novo->ant = novo;
	return novo;
}

No * achaPos(No *lista, int prio){
	int flag = 0;
	No *aux = lista;
	while(1){
		if(flag!=0){
			if(lista==aux){return lista;}
		}
		flag = 1;
		if(lista->prio > prio){
			lista = lista->ant;
		}
		else {
			if(lista->prox->prio > prio){
				return lista;
			}
			lista = lista->prox;
		}
	}
}

No * insereElemento(No *lista,int pid,int prio) {
	No *novo;
	if(lista->pid==0){	//primeiro elemento
		lista->pid=pid;
		lista->prio=prio;
		return lista;
	}
	lista = achaPos(lista,prio);
	novo = (No*)malloc(sizeof(No));
	novo->pid = pid;
	novo->prox = lista->prox;
	lista->prox = novo;
	novo->ant = lista;
	novo->prox->ant = novo;
	novo->prio = prio;
	return novo;
}

No * removeElemento(No *lista) {
	No *aux;
	if(lista->prox==lista){
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

No * proxElem(No *lista){
	return lista->prox;
}

No * antElem(No *lista){
	return lista->ant;
}

void destroiLista(No *lista) {
	while (lista->pid) {
		lista = removeElemento(lista);
	}
	free(lista);
	return;
}

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