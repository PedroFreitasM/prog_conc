//Aluno: Pedro Freitas de Moura
/* Objetivo do código: Alterar o código soma-lock-atom.c para imprimir os valores múltiplos de 1000 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int soma = 0; //var de soma
pthread_mutex_t mutex;// var de lock para exclusao mutua
pthread_cond_t cond;

void *extra (void *args){
	int threads = *(int*) args;
	printf("Inicio da func. extra\n");
	for (int i = 0; i < 100000 * threads; i++){ // leva em conta o numero de threads pra nao perder o max da soma
		pthread_mutex_lock(&mutex);
		if(soma%1000 == 0){
			printf("soma = %ld \n", soma);
			pthread_cond_broadcast(&cond); //conseguiu printar o multiplo? pode voltar as tarefas
		}
		pthread_mutex_unlock(&mutex);
		if(soma == 100000 * threads){
			break; // só pra não printar tanto...
		}
	}
	printf("Fim da func. extra \n");
	pthread_exit(NULL);
}

void *ExecutaTarefa(void *arg){
	long int id = (long int) arg;
	printf("Thread %ld sendo executada \n", id);

	for (int i = 0; i < 100000; i++){
		pthread_mutex_lock(&mutex);
		if(soma%1000 ==0){
			pthread_cond_wait(&cond, &mutex); //quando chegar ao multiplo, para todos as tarefas temporariamente
		}
		soma++; //coloco o soma dps pra não ocorrer o erro de nunca passas pelo if anterior
		pthread_mutex_unlock(&mutex);
	}
	printf("Fim da thread %ld \n" , id);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	pthread_t *tid;
	int nthreads;
	
	if(argc<2){
		printf("Comando: %s <num de threads>\n", argv[0]);
		return 1;
	}
	nthreads = atoi(argv[1]);

	tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
	if(tid==NULL) {puts("Erro no malloc"); return 2;}

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	for(long int t = 0; t<nthreads; t++){
		if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *) t)){
			printf("Erro no pthread_create\n"); exit(-1);
		}
	}

	if (pthread_create(&tid[nthreads], NULL, extra, &nthreads)){
		printf("Erro no pthread_create do log\n"); exit(-1);
	}

	for (int t=0; t<nthreads+1; t++){
		if(pthread_join(tid[t], NULL)){
			printf("Erro no join das threads\n"); exit(-1);
		}
	}
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	printf("Valor da var. soma = %ld \n", soma);

	return 0;
}
