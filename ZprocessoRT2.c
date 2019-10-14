#include <stdio.h>
#include <unistd.h>
int main() {
    int i;
	for(i = 0; i < 10; i++){
		printf("RT2");
		usleep(100000);
		fflush(stdout);
	}
}