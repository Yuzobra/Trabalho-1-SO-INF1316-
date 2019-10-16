#include <stdio.h>
#include <unistd.h>
 
int main(){
	
	float i;
	
	for(i=15; i>0; i-=0.1){
		printf("%.1f\t",i);
		fflush(stdout);
		usleep(100000);
	}
}
 