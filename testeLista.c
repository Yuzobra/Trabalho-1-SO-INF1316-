#include "LisCircular.h"

int main(){
	No *n;
	
	n = CriaLista();

	n = insereElemento(n,11,1);
	printaLista(n);
	
	n = insereElemento(n,21,2);
	printaLista(n);
	
	n = insereElemento(n,31,3);
	printaLista(n);
	
	n = insereElemento(n,41,4);
	printaLista(n);
	
	n = insereElemento(n,12,1);
	printaLista(n);
	
	n = insereElemento(n,32,3);
	printaLista(n);
	
	n = insereElemento(n,33,3);
	printaLista(n);
	
	n = removeElemento(n);
	printaLista(n);
	
	n = removeElemento(n);
	printaLista(n);
	
	n = removeElemento(n);
	printaLista(n);
	
	n = removeElemento(n);
	printaLista(n);
	
	destroiLista(n);
	
	return 1;
}