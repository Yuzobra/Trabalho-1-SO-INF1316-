#include <stdio.h>
#include <unistd.h>
int main() {
	while (1) {
		printf("R1");
		usleep(100000);
		fflush(stdout);
	}
}