#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
/*
 Autor: Pedro Freitas de Moura
 Matéria: Programação Concorrente
 Objetivo do código: Criar uma thread produtora que gera uma sequência N de nums. inteiros consecutivos e os insere no canal de inteiros de tamanho M (M << N) que será utilzado por várias threads consumidoras.

 A thread produtora insere todos os elementos do buffer de uma vez. As consumidoras retiram eles, um de cada vez, e avaliam se eles são primos.

 Além disso, a sincronização das threads deve ser feita com semáforos.

 Input do programa: ./<nome do programa> <numero de threads consumidoras> <valor de m> <valor de n>
 */
int qt_primos = 0; //quantidade de primos total
int producao = 0; // quantos numeros foram produzidos no total
int M = 0; //tamanho do buffer
int N = 0; //quantidade de numeros
int producao_terminou = 0; // na prática um booleano sobre quando ele produziu todos os numeros

sem_t slotCheio, slotVazio; //condição
sem_t mutex; //mutex

int insere=0, retira=0;
int* buffer;

// funcao que encontra primo
int ehPrimo (long long int n){
	int i;
	if (n<=1) return 0;
	if (n==2) return 1;
	if (n%2 == 0) return 0;
	for (i=3; i<sqrt(n) +1; i+=2)
		if(n%i==0) return 0;
	return 1;
}

// funcao do unico produtor
void* produz (void* args){
	printf("Início do produtor \n");
	int nthreads = *(int *)args;
	while(producao < N){ //enquanto tem mais numeros pra produzir...
		printf("Aguardando o buffer esvaziar \n");
		for(int i = 0; i < M; i++){
			sem_wait(&slotVazio); //espera os consumidores terminarem
		}
		sem_wait(&mutex);
		printf("Produzindo novo lote \n");
		int lote = 0;
		for (int i = 0; i < M; i++){
			if (producao >= N){
				break;
			}
                        buffer[insere] = producao;
			printf("PRODUTOR inseriu o valor %d na posição %d \n", producao, insere);
			insere = (insere + 1) % M;
			producao++;
			lote++;
		}
		sem_post(&mutex);
		
		printf("Lote liberado pros consumidores");
		for (int i = 0; i < lote; i++){
			sem_post(&slotCheio);
		}
	}
		printf("Terminou a produção dos %d valores", N);
		producao_terminou = 1;
		for (int i = 0; i < nthreads; i++) {
        		sem_post(&slotCheio);
    		}
		pthread_exit(NULL);
	}


// funcao dos consumidores
void* consome(void* arg){
	long id = (long)arg;
	int conta_primos = 0;
	int numero = 0;
	int resultado_primo;
	while(1){
		sem_wait(&slotCheio);
		sem_wait(&mutex);
		if(producao_terminou && insere == retira){
			sem_post(&mutex);
			sem_post(&slotCheio);
			break;
		}

		numero = buffer[retira];
		printf("Consumidor %ld retirou valor %d da posicao %d \n", id, numero, retira);
		retira = (retira + 1) % M;
		sem_post(&mutex);
		resultado_primo = ehPrimo(numero);
		if (resultado_primo){
			sem_wait(&mutex);
			qt_primos++;
			sem_post(&mutex);
			conta_primos++;
		}
					
		sem_post(&slotVazio);
	}
	int* ret = (int *)malloc(sizeof(int));
	if (ret!=NULL) *ret = conta_primos;
	pthread_exit((void *) ret);
}

int main(int argc, char** argv){
	pthread_t *tid;
	int nthreads;
	if(argc < 4){
		printf("Comando: %s <numero de consumidores> <tamanho do buffer> <quantidade de numeros>\n", argv[0]);
		return 1;
	}
	nthreads = atoi(argv[1]);
	M = atoi(argv[2]);
	N = atoi(argv[3]);
	buffer = (int *)malloc(sizeof(int)*M);	
	tid = (pthread_t *)malloc(sizeof(pthread_t) * (nthreads+1));
	if(tid==NULL || buffer == NULL) {puts("Erro no malloc"); return 2;}
	
	sem_init(&mutex, 0, 1);
	sem_init(&slotCheio, 0, 0);
	sem_init(&slotVazio, 0, M);
	
	
	pthread_create(&tid[0], NULL, produz, (void *)&nthreads);
	for (long i = 0; i < nthreads; i++){
		pthread_create(&tid[i+1], NULL, consome, (void *)i);
	}
	pthread_join(tid[0], NULL);

	int thread_vencedora = -1;
	int maior_valor = -1;
	void* ret;
	for(int i = 0; i < nthreads; i++){
		pthread_join(tid[i+1], &ret);
	
		if (ret != NULL){
		int resultado = *(int*)ret;
		free(ret);

		if (resultado > maior_valor){
			thread_vencedora = i;
			maior_valor = resultado;
	}	
	}
	}
	printf("\n RESULTADOS: \n");
	printf("Total de primos: %d \n", qt_primos);
	printf("Thread vencedora: %d \n", thread_vencedora);
	printf("Primos contados por ela: %d \n", maior_valor);
	free(buffer);
	free(tid);
	sem_destroy(&mutex);
	sem_destroy(&slotCheio);
	sem_destroy(&slotVazio);
	return 0;
}
