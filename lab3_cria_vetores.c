// A funcao desse programa eh criar o arquivo dos vetores (com tamanho , vetor A e vetor B)
// O outro programa irá realizar o produto com o arquivo.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100 //valor maximo de um elemento
//#define LOG

int main(int argc, char* argv[]){
	float *vetor1;
	float *vetor2;
	long int dim;
	float elem;
	int fator=1;
	FILE * arq;
	
	if (argc < 3) {
		printf("ERRO: formato do programa: %s <dimensao>< <nome arquivo saida>\n", argv[0]);
	return 1;
	}
	dim = atoi(argv[1]);
	
	vetor1 = (float*)malloc(sizeof(float) * dim);
	vetor2 = (float*)malloc(sizeof(float) * dim);
	if(!vetor1 || !vetor2){
		printf("Erro de alocacao nos vetores\n");
		return 2;
	}

	srand(time(NULL));
	for(long int i = 0;i< 2*dim;i++){
		printf("Valor de i: %ld \n", i);
		elem = (rand() % MAX)/3.0 * fator;
		if (i < dim){
			vetor1[i] = elem;
		}else{
			vetor2[i - dim] = elem;
		}
		fator*=-1;
	}
	#ifdef LOG
	fprintf(stdout, "%ld\n", n);
	for(int i = 0; i < dim; i++){
		fprintf(stdout, "%f ",vetor1[i]);
	}
	fprinf(stdout, "\n");
        for(int i = 0; i < dim; i++){
                fprintf(stdout, "%f ",vetor2[i]);
        }
	fprintf(stdout,"\n");
	#endif

	arq = fopen(argv[2], "wb");
	if(!arq){
		printf("Erro de abertura no arquivo\n");
		return 3;
	}
	fwrite(&dim, sizeof(long int), 1, arq);
	fwrite(vetor1, sizeof(float), dim, arq);
	fwrite(vetor2, sizeof(float), dim, arq);
	fclose(arq);
	free(vetor1);
	free(vetor2);
	return 0;
}
