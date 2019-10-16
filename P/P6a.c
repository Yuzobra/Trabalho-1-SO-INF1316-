#include <stdio.h>

unsigned long fibo(unsigned long n){
	if(n==0){return 1;}
	if(n==1){return 1;}
	return fibo(n-1)+fibo(n-2);
}


int main(){
	unsigned long a;
	while(1){
		a = fibo(44);
		printf("%lu\n",a);
		fflush(stdout);
	}
}