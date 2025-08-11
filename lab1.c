#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//Valores aleatórios no array
//As treads incrementam seus respectivos valores (thread 1:0,4,8,12,16; Tread 2: 1,5,9,13, etc...)
//no final printa pra ter ctz
typedef struct{
	int valorArgv;
	int *vetor;
	int posicao;
}argThread;


void *incrementVetor(void *arg) {
	argThread *args = (argThread *)arg; //pega os valores no argumento
	int posicaoAtual = args->posicao;
	for(int j = 0; j<args->valorArgv; j++){
		printf("Aumentando a posicao %d (atualmente %d)\n",posicaoAtual, args->vetor[posicaoAtual]);
		args->vetor[posicaoAtual]++;
		posicaoAtual+=4;
	}
	printf("Thread %d terminou.\n", args->posicao);

	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	if (argc<2) {
		printf("Erro: informe o tamanho da array\n");
		printf("Comando correto: %s <tamanho da array>",argv[0]);
	return 1;
	}
	int nthreads = 4;
	int tamanho = atoi(argv[1]);
	int* vetor = (int*)malloc(sizeof(int)*4*tamanho);
	for(int i = 0; i<(4*tamanho); i++){
	//Define os valores do vetor como a sua posição por simplicidade
		vetor[i] = i;
	}
	pthread_t tid_sistema[nthreads];
	argThread* argumentos = (argThread *)malloc(nthreads * sizeof(argThread));
	for(int i=0; i<nthreads; i++) {
		argumentos[i].valorArgv = tamanho;
		argumentos[i].vetor = vetor;
		argumentos[i].posicao = i;
		printf("Thread numero %d criada.\n", i);
		if (pthread_create(&tid_sistema[i], NULL, incrementVetor, (void *)&argumentos[i])){
			printf("Erro na criacao de thread.");
			return 2;
		}
	
	}
	for (int i=0; i<nthreads; i++){
		if(pthread_join(tid_sistema[i], NULL)) {
			printf("Erro no join da thread %d\n", i);
		}
	}

	printf("Fim da main. Valores finais do vetor:\n");
	for(int i = 0; i < 4*tamanho; i++){
		printf("Valor %d: %d\n", i, vetor[i]);
	}

	return 0;
}
