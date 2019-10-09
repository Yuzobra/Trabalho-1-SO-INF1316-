#include <stdio.h>
#include <unistd.h>
int main() {
	while (1) {
		printf("A4");
		usleep(100000);
		fflush(stdout);
	}
}