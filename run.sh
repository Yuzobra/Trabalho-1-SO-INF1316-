gcc -o reader reader.c procInfo.c
gcc -o processManager processManager.c procInfo.c LisCircular.c LisEncadeada.c
gcc -o debug debug.c
./debug