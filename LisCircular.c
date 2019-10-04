#include <stdlib.h>
#include <stdio.h>
#include "LisCircular.h"



No * CriaLista(int pid){
	No *novo;
	novo = (No*)malloc(sizeof(No));
	novo->pid = pid;
	novo->pos = 0;
	novo->prox = novo;
	novo->ant = novo;
	return novo;
}

No * insereElemento(No *lista,int pid) {
	No *aux;
	No *novo;
	novo = (No*)malloc(sizeof(No));
	novo->pid = pid;
	novo->prox = lista->prox;
	lista->prox = novo;
	novo->ant = lista;
	novo->prox->ant = novo;
	novo->pos = lista->pos + 1;
	aux = novo->prox;
	while(aux->pos>=novo->pos){	//aumenta em 1 a posição de todos os nós que vierem a seguir
		aux->pos++;
		aux = aux->prox;
	}
	return novo;
}

No * removeElemento(No *lista) {
	No *aux;
	if(lista->prox==lista->ant){
		if(lista->prox==lista){
			printf("Não é possível remover último elemento da lista");
			return NULL;
		}
		aux = lista->prox;
		free(lista);
		aux->prox = aux;
		aux->ant = aux;
		aux->pos = 0;
		return aux;
	}
	lista->ant->prox = lista->prox;
	lista->prox->ant = lista->ant;
	aux = lista->prox;
	while (aux->pos>=lista->pos) {	//diminue em 1 a posição de todos os nós que vierem a seguir
		aux->pos--;
		aux = aux->prox;
	}
	aux = lista->ant;
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
	while (lista->prox != lista) {
		lista = removeElemento(lista);
	}
	free(lista);
	return;
}

void printaLista(No *lista) {
	while (lista->pos != 0) {
		lista = lista->prox;
	}
	printf("{%d ,pos: %d, ant: %d, prox:%d}\t", lista->pid, lista->pos, lista->ant->pid, lista->prox->pid);
	lista = lista->prox;
	while (lista->pos > 0) {
		printf("{%d ,pos: %d, ant: %d, prox:%d}\t", lista->pid, lista->pos, lista->ant->pid, lista->prox->pid);
		lista = lista->prox;
	}
	printf("\n");
	return;
}