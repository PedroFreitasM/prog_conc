#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "timer.h"
#include <math.h>

//variável para o produto interno
float *vet1;
float *vet2;

//estrutura de dados para os argumentos das threads
typedef struct{
	long int tam;
	short int nthreads;
	short int id;
} t_args;

void *prodVetor (void *args){
	t_args *arg = (t_args *) args;
	int ini, fim, fatia;
	float prod_local=0, *ret;
	
	fatia = arg->tam / arg->nthreads;
	ini = arg->id * fatia;
	fim = ini + fatia;
	if (arg->id == (arg->nthreads-1)) fim = arg->tam; // ultima thread pega os elementos finais 
	
	//faz o produto
	for(int i = ini; i<fim; i++){
		prod_local += (vet1[i]*vet2[i]);
	}

	ret = (float *) malloc(sizeof(float));
	if (ret!=NULL) *ret = prod_local;
	else printf("ERRO no malloc da thread\n");
	pthread_exit((void *) ret);
}

//funcao main
int main(int argc, char *argv[]){
	long int n; //tam do vetor
	short int nthreads; //qt de threads
	FILE *arq; //arquivo de entrada
	size_t ret;
	float prod_par_global; //calcula o produto interno por concorrencia
    float prod_vet_seq; //variavel que pega o produto vetorial no programa sequencial
	float *prod_retorno_threads;
    float var_rel; // erro por variacao relativa (formula no pdf da aula)
    double inicio, fim; //pega o tempo de execução

	pthread_t *tid_sistema;

	if(argc <3) {
	printf("Uso: %s <arquivo de entrada> <qt de threads> \n", argv[0]);
	exit(-1);
	}

	//abre o arquivo
	arq = fopen(argv[1], "rb");
	if(arq==NULL) { 
		printf("ERRO na abertura de arquivo\n");
		exit(-1);
	}

	ret = fread(&n, sizeof(long int), 1, arq);
	if(!ret){
		fprintf(stderr, "Erro de leitura da dimensao das matrizes \n");
		return 3;
	}

	vet1 = (float *)malloc(sizeof(float) * n);
	vet2 = (float *)malloc(sizeof(float) * n);
	if(vet1 == NULL || vet2 == NULL){
		printf("ERRO no malloc dos vetores\n");
		exit(-1);
	}

    GET_TIME(inicio);
    //pega os vetores e o produto original
	ret = fread(vet1, sizeof(float), n, arq);
	ret = fread(vet2, sizeof(float), n, arq);
    ret = fread(&prod_vet_seq, sizeof(float), 1, arq);

    //printa os vetores para ver se o resultado deu certo
    //vou deixar comentado para caso o vetor seja muito grande
    /*
    printf("Vetor 1: \n");
    for(int i = 0; i < n; i++){
        printf("%f ", vet1[i]);
    }
    printf("\nVetor 2: \n");
        for(int i = 0; i < n; i++){
        printf("%f ", vet2[i]);
    }
    printf("\n");
    */

	//le o numero de threads
	nthreads = atoi(argv[2]);

	//se o vetor for muito pequeno, limita no num de threads
	if(nthreads>n) nthreads = n;

	tid_sistema = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
	if(tid_sistema==NULL){
		printf("ERRO no malloc das IDs das threads\n");
		exit(-1);
	}

	//cria threads
	for(long int i = 0; i < nthreads; i++){
		t_args *args;
		args = (t_args*) malloc(sizeof(t_args));
		if(args==NULL){
			printf("ERRO no malloc dos argumentos\n");
			exit(-1);
		}
		args->tam = n;
		args->nthreads = nthreads;
		args->id = i;
		if (pthread_create(&tid_sistema[i], NULL, prodVetor, (void *) args)){
			printf("ERRO no pthread create\n");
			exit(-1);
		}
	}

	//espera as threads terminarem
	prod_par_global=0;
	for(int i = 0; i<nthreads; i++){
		if (pthread_join(tid_sistema[i], (void *)&prod_retorno_threads)){
			printf("ERRO do join da thread\n");
			exit(-1);
		}
		prod_par_global += *prod_retorno_threads;
		free(prod_retorno_threads);
	}
    
    GET_TIME(fim);

	//printa os resultados
	printf("Resultado do produto interno concorrente: %f \n", prod_par_global);
    printf("Resultado do produto interno sequencial: %f \n", prod_vet_seq);

    //calcula o erro:
    var_rel = fabs((prod_vet_seq - prod_par_global)/prod_vet_seq);
    
    inicio = inicio - fim;

    printf("Variancia relativa: %f \n", var_rel);
    //printa o tempo de performance
    printf("Tempo de execucao das threads: %e \n",fabs(inicio));    
    
	//desaloca os espacos
	free(vet1);
	free(vet2);
	free(tid_sistema);

	fclose(arq);
}
