#include "LisCircular.h"

int main(){
	No *n;
	
	n = CriaLista(1);	
	printaLista(n);
	
	n = insereElemento(n,2);
	printaLista(n);
	
	n = insereElemento(n,3);
	printaLista(n);
	
	n = insereElemento(n,4);
	printaLista(n);
	
	n = antElem(n);
	n = removeElemento(n);
	printaLista(n);
	
	n = removeElemento(n);
	printaLista(n);
	n = removeElemento(n);
	printaLista(n);
	
	destroiLista(n);
	
	return 1;
}