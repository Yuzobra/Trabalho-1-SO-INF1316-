#include <stdio.h>
#include <unistd.h>
int main() {									
	char text[29] = "Nao vou sequer ter uma chance";
	int i;
	for(i=0;i<29;i++){
		printf("%c",text[i]);
		usleep(100000);
		fflush(stdout);
	}
}