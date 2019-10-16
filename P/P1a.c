#include <stdio.h>
 
int main(){
	
	unsigned long i;
	
	for(i=1; i<10000000000; i++){
		if(i%100000000==0){
			printf("%lu\t",i);
			fflush(stdout);
		}
	}
}
 