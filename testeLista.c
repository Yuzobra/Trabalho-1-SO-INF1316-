#include "LisCircular.h"

int main(){
	No *n;
	
	n = CriaLista();

	n = insereElemento(n, 41, 4);
	printaLista(n);
	
	n = insereElemento(n, 42, 4);
	printaLista(n);
	
	n = insereElemento(n, 61, 6);
	printaLista(n);

	n = insereElemento(n, 31, 3);
	printaLista(n);

	n = insereElemento(n, 81, 8);
	printaLista(n);

	n = insereElemento(n, 62, 6);
	printaLista(n);

	n = insereElemento(n, 11, 1);
	printaLista(n);

	n = insereElemento(n, 82, 8);
	printaLista(n);

	destroiLista(n);
	
	return 1;
}