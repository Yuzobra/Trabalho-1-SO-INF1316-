#include <stdio.h>
#include <unistd.h>
int main() {
    int i;
	while (1) {
		printf("RT1");
		usleep(100000);
		fflush(stdout);
	}
}