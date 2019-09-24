#define KEY_V 4242
#define KEY_C 5050
#define SIZE 50

#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/shm.h> //shmctl()

#include <stdio.h>
#include <stdlib.h>

#include "reader.h"

int main (int argc, char *argv[]) { 
	int seg_vProc,*vProc,seg_cont,*cont, id, pid, status; 
	
	seg_cont = shmget (KEY_C, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); 
	if(seg_cont == -1) {
		printf("Erro ao alocar memoria compartilhada");
		exit(1);
	}
	vProc = (ProcInfo *) shmat (seg_cont, 0, 0);

	seg_vProc = shmget (KEY_V, sizeof (ProcInfo)*SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); 
	if(seg_vProc == -1) {
		printf("Erro ao alocar memoria compartilhada");
		exit(1);
	}
	vProc = (ProcInfo *) shmat (seg_vProc, 0, 0);
	
	
	
	
	
	shmdt(vProc);
}
