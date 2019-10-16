#include <stdio.h>
#include <unistd.h>
int main() {
	int i;
	for(i=0;i<30;i++){
		printf("A4");
		usleep(100000);
		fflush(stdout);
	}
}