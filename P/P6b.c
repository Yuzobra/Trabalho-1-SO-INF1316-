#include <stdio.h>
#include <unistd.h>
int main() {
	while (1) {
		printf("P6b ");
		usleep(100000);
		fflush(stdout);
	}
}