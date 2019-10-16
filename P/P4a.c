#include <stdio.h>
#include <unistd.h>
int main() {
	while (1) {
		printf("P4a ");
		usleep(100000);
		fflush(stdout);
	}
}