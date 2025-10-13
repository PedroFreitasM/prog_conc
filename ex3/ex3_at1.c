#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
/*
 Autor: Pedro Freitas de Moura
 Matéria: Programação Concorrente
 Objetivo do código: Criar uma thread produtora que gera uma sequência N de nums. inteiros consecutivos e os insere no canal de inteiros de tamanho M (M << N) que será utilzado por várias threads consumidoras.

 A thread produtora insere todos os elementos do buffer de uma vez. As consumidoras retiram eles, um de cada vez, e avaliam se eles são primos.

 Além disso, a sincronização das threads deve ser feita com semáforos.

 Input do programa: ./<nome do programa> <numero de threads consumidoras> <valor de m> <valor de n>
 */
sem_t slotCheio, slotVazio; //condição
sem_t mutexCons; //mutex
			    //
int contador=0, insere=0, remove=0;
int* buffer;

// funcao do unico produtor
void* produz (void* args){
	int* n = (int *)args;
	for (int i = 0; i <= n; i++){
			
	}
}

// funcao dos consumidores
void* ehPrimo(void* num){

	long long int* n = (long long int *) num
	int i;
	if (n <= 1) return 0;
	if (n == 2) return 1;
	if (n%2 == 0) return 0;
	for (i=3; i<sqrt(n)+1; i+=2)
		if(n%i==0) return 0;
	return 1;
}

int main(int argc, char** argv){
	pthread_t *tid;
	int nthreads;
	int m, n;
	if(argc < 4){
		printf("Comando: %s <num de threads consumidoras> <tamanho m do buffer> <quantidade de numeros (n)>\n", argv[0]);
		return 1;
	}
	nthreads = atoi(argv[1]);
	m = atoi(argv[2]);
	n = atoi(argv[3]);
	buffer = (int *)malloc(sizeof(int)*m);	
	tid = (pthread*)malloc(sizeof(pthread_t)*(nthreads+1));
	if(tid==NULL) {puts("Erro no malloc"); return 2;}
	
	sem_init(&mutexCons, 0, 1);
	sem_init(&slotCheio, 0, 0);
	sem_init(&slotVazio, 0, m);






	return 0;
}
