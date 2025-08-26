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
    float prod_int = 0.0;
	int fator=1;
	FILE * arq;
	
	if (argc < 3) {
		printf("ERRO: formato do programa: %s <dimensao> <nome arquivo saida>\n", argv[0]);
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
	for(long int i = 0; i < dim; i++){
		fprintf(stdout, "%f ",vetor1[i]);
	}
	fprinf(stdout, "\n");
        for(long int i = 0; i < dim; i++){
                fprintf(stdout, "%f ",vetor2[i]);
        }
	fprintf(stdout,"\n");
	#endif

	arq = fopen(argv[2], "wb");
	if(!arq){
		printf("Erro de abertura no arquivo\n");
		return 3;
	}
    //escreve a dimensao e os vetores no arquivo
	fwrite(&dim, sizeof(long int), 1, arq);
	fwrite(vetor1, sizeof(float), dim, arq);
	fwrite(vetor2, sizeof(float), dim, arq);

    //escreve o valor do prod. interno no arquivo
    //primeiro tem que ver o valor do prod...
    for (long int i = 0; i < dim; i++){
        prod_int += (vetor1[i] * vetor2[i]);
    }
    fwrite(&prod_int, sizeof(float), 1, arq);    
    printf("Valor do produto interno: %f \n", prod_int);    

	fclose(arq);
	free(vetor1);
	free(vetor2);
	return 0;
}
