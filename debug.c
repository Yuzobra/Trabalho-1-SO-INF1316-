#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
void procpause(int pid);
void procshow(int pid);
void procresume(int pid);
int main (int argc, char *argv[]) { 
	int pidmanager;
	char command[7];
	if ((pidmanager = fork()) == 0) {
		printf("Iniciando escalonador\n");
		char nome[] = "./processManager";
		char *arg[] = { nome,NULL };
		execv(arg[0], arg);
	}
	while (1) {
		scanf("%s",command);
		if (strcmp(command,"pause")==0) {
			procpause(pidmanager);
		}
		else if (strcmp(command, "resume") == 0) {
			procresume(pidmanager);
		}
		else if (strcmp(command, "show") == 0) {
			procshow(pidmanager);
		}
	}
}
void procpause(int pid) {
	kill(pid, SIGUSR1);
}
void procresume(int pid) {
	kill(pid, SIGCONT);
	printf("\n-------------RESUMED--------------\n");
}
void procshow(int pid) {
	kill(pid, SIGCONT);
	kill(pid, SIGUSR2);
}