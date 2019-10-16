#include <stdio.h>
#include <unistd.h>
int main() {									
	char text[42] = "Nao acredite na string que veio em cima^..";
	int i;
	for(i=0;i<42;i++){
		printf("%c",text[i]);
		usleep(100000);
		fflush(stdout);
	}
}