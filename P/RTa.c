#include <stdio.h>

unsigned long fibo(unsigned long n){
	if(n==0){return 1;}
	if(n==1){return 1;}
	return fibo(n-1)+fibo(n-2);
}


int main(){
	unsigned long a;
	int i = 35;
	while(1){
		a = fibo(i);
		printf("numero %d de fibonacci: %lu\n",i,a);
		fflush(stdout);
		i++;
	}
}