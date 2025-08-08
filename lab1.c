#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//Valores aleatórios no array
//As treads incrementam seus respectivos valores (thread 1:0,4,8,12,16; Tread 2: 1,5,9,13, etc...)
//no final printa pra ter ctz
int main(int argc, char* argv[]){
	if (argc<2) {
		printf("Erro: informe o tamanho da array\n");
		printf("Comando correto: %s <tamanho da array>",argv[0]);
	return 1;
	}
	nthreads = 4;
	int tamanho = atoi(argv[1]);
	int* vetor = (int*)malloc(4*4*tamanho);
	pthread_t tid_sistema[nthreads];
	for(int i=0; i<nthreads; i++) {
		printf("Thread numero %d criada.\n", i);
		if (pthread_create(&tid_sistema[i], NULL, /*nome da funcao lmao*/, NULL))
	}




	return 0;
}
