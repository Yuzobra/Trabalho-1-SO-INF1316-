#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LisEncadeada.h"
#include "procInfo.h"
//TAD Lista:

typedef struct no No;

typedef struct realTimeProc{
	int pid;
    int pidAnt;
	ProcInfo * procInfo;
} RealTimeProc;

struct no{
    RealTimeProc * realTimeProc;
    No * prox;
    No * ant;
};

typedef struct lista{
    No * inicio;
    No * fim;
} Lista;

static No * criaNo();
static void deletaNo(No* no);


Lista * criaLista(){
    Lista * newLista = (Lista*)malloc(sizeof(Lista));
    newLista->inicio = NULL;
    newLista->fim = NULL;
    return newLista;
}

void freeLista(Lista * lista){
    No* no;
    while(!listaVazia(lista)){
        no = lista->inicio;
        lista->inicio = no->prox;
        deletaNo(no);
    }
    free(lista);
}


void insFinal(Lista * lista, RealTimeProc *realTimeProc){
    No * no = criaNo();
    if(lista->inicio == NULL) /* Primeiro elemento */ {
        lista->inicio = no;
        lista->fim = no;
        no->realTimeProc = realTimeProc;
    }
    else{
        lista->fim->prox = no; 
        no->ant = lista->fim;
        no->realTimeProc = realTimeProc;
        lista->fim = no;
    }
}

int listaVazia(Lista * lista){
    if(lista->inicio == NULL){
        return 1;
    }
    return 0;
}


//Funcoes especificas para o nosso projeto:


RealTimeProc * verificaLista(Lista * lista, ProcInfo * procMorrendo, RealTimeProc * realTime[]) /* Verifica se há algum processo esperando o fim do processo terminando */ {
    int IProcMorrendo, DProcMorrendo, IProcLista, DProcLista, k, flag;
    No * no;
    IProcMorrendo = atoi(procMorrendo->I);
    DProcMorrendo = atoi(procMorrendo->D);

    for(no = lista->inicio; no != NULL; no = no->prox){
        IProcLista = atoi(no->realTimeProc->procInfo->I);
        DProcLista = atoi(no->realTimeProc->procInfo->D);
        flag = 0;
        if(  ((IProcLista >= IProcMorrendo) && (IProcLista <= (IProcMorrendo + DProcMorrendo))) ||
             ((IProcLista + DProcLista) >= IProcMorrendo && (IProcLista + DProcLista) <= (IProcMorrendo + DProcMorrendo)) ||
             ((IProcLista <= IProcMorrendo) && (IProcLista + DProcLista >= IProcMorrendo + DProcMorrendo)))/* Processo ocorre durante a execução do processo morrendo */{
            //verificar se ele conflita com outro processo
            for(k = IProcLista; k < IProcLista + DProcLista ; k++){
                if(realTime[k] != NULL) /* Conflita com outro processo */{
                    flag = 1;
                    break;
                }
            }
            if(flag == 1) continue;
            else /* Processo na lista não conflita com nenhum processo no escalonamento e pode rodar */ {
                RealTimeProc * procFila = no->realTimeProc;
                deletaNo(no);
                if(no->ant == NULL) lista->inicio = no->prox;
                if(no->prox == NULL) lista->fim = no->ant;
                return procFila;
            }
        }
    }
    return NULL;
}

void removerDependentes(Lista * lista, RealTimeProc * procMorrendo){
    int IProcMorrendo, DProcMorrendo, IProcLista, DProcLista, k, flag;
    No * no;
    IProcMorrendo = atoi(procMorrendo->procInfo->I);
    DProcMorrendo = atoi(procMorrendo->procInfo->D);
    for(no = lista->inicio; no != NULL; no = no->prox){
        if(no->realTimeProc->pidAnt == procMorrendo->pid){
            free(no->realTimeProc->procInfo);
            free(no->realTimeProc);
            
            if(no->ant == NULL) lista->inicio = no->prox;
            if(no->prox == NULL) lista->fim = no->ant;
            deletaNo(no);

        }
    }
}


void printListaEnc(Lista * lista){
    No * no;
    printf("Lista de espera:\n");
    for(no = lista->inicio; no != NULL; no = no->prox){
        printf("{%d , %s, I: %s, D: %d}\n",no->realTimeProc->pid,no->realTimeProc->procInfo->nomeProc,no->realTimeProc->procInfo->I, atoi(no->realTimeProc->procInfo->D));
    }
}


//Funcoes estaticas:


static No * criaNo(){
    No* no = (No*) malloc (sizeof(No));
    no->ant = NULL;
    no->prox = NULL;
    no->realTimeProc = NULL;
    return no;
}

static void deletaNo(No* no){
    if(no != NULL){
        if(no->ant != NULL){
            no->ant->prox = no->prox;
        }
        if(no->prox != NULL){
            no->prox->ant = no->ant;
        }
        printf("antes do free no\n");
        free(no);
        printf("depois do free no\n");
    }
}