#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
typedef struct{
	int id_thread;
	int inicio;
	int fim;
}t_args


long double pi = 0;



void *tarefa(void* args){
	t_args arg = (t_args*)args
	

}


int main (int argc, char** argv){
	pthread_t *tid;
	t_args *args;
	int nthreads;
	int iter_BBP;
	if(argc<3){
		printf("Padrão de input: %s <num de threads> <iteracoes da funcao Bailey-Borwein-Plouffe>\n", argv[0]);
		return 1;
	}
	nthreads = atoi(argv[1]);
	iter_BBP = atoi(argv[2]);
	if (nthreads > iter_BBP) nthreads = iter_BBP;
	tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads));
	args = (t_args *) malloc(sizeof(t_args)*(nthreads));
	if(tid == NULL || args == NULL){
		printf("Erro no malloc\n");
		return 2;
	}
	int range = iter_BBP/nthreads;
	for (long int j = 0; j<nthreads; j++){
	args[j].id_thread = j;
	args[j].inicio = range*j;
	if(range*(j+1) > iter_BBP){
		args[j].fim = iter_BBP;
	}else{
		args[j].fim = range*(j+1);
	}
	if(pthread_create(&tid[j], NULL, tarefa, (void *)args[i])){
		printf("Erro no pthread_create\n");
		exit(-1);
	}
	}

	for(int i = 0; i<nthreads; i++){
		if(pthread_join(tid[i], NULL)){
			printf("Erro no join \n");
			exit(-1);
		}
	}

	printf("Valor de pi: %Lf \n", pi);
	return 0;
}
