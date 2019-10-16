#include <stdio.h>
#include <unistd.h>
int main() {
	char text[55] = "Nao acredite na string que vem a seguir: Agora morremos";
	int i;
	for(i=0;i<55;i++){
		printf("%c",text[i]);
		usleep(100000);
		fflush(stdout);
	}
}