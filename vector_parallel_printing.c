/*
 * create by simoes
 *
 * O objetivo deste algoritmo é projetar e implementar um programa paralelo utilizando Pthreads que gere um histograma para um dado conjunto de dados.
 * Como entrada, deve ser pasado dois valores reais, a e b, representando os valores mínimo e máximo do intervalo sobre os quais os números devem ser gerados.
 * Após a geração dos valores floats aletórios, ele trunca todos os valores, e imprimi paralelamente um histograma informando quantas vezes um determinado valor saiu. 
 * Por exemplo, considere o seguinte vetor de números reais: [0.55, 0.39, 2.78, 3.91, 3.80, 3.12]
 * Se truncarmos os valores, teremos o seguinte vetor [0, 0, 2, 3, 3, 3]
 * Após truncar, podemos ver que ele contém dois valores 0, um valor 2, e três valores 3
 * Portando o histograma deve ser o seguinte:
 * 0 ##2
 * 2 #1
 * 3 ###3
 *
 *
 * OBS: IMPORTANTE RESSALTAR QUE O NÚMERO DE THREADs DEVE SER INFORMADO POR LINHA DE COMANDO QUANDO FOR EXECUTAR O PROGRAMA.
 * COMPILAÇÃO: gcc -g -Wall -o <nome_executavel> <nome_programa.c> -lpthread -lm
 * EXECUÇÃO: ./<nome_executal> <numero_de_threads>
 * */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

#define VECTOR_SIZE 10

float float_rand(float min, float max);
void generating_vectors(float a, float b);
void* print_vectors(void *rank);

double v[VECTOR_SIZE];
int result[VECTOR_SIZE], quantidade[VECTOR_SIZE], v_trunc[VECTOR_SIZE], 
	thread_count, loop_numbers = 0;

int main(int argc, char* argv[]) {
	srand((unsigned int)time(NULL));

	//obtendo o número de threads passado por linha de comando
	thread_count = strtol(argv[1], NULL, 10);
	pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));

	float a=0.0, b=10.0;

	printf("nmr minimo a ser gerado: ");
	scanf("%f", &a);

	printf("nmr maximo a ser gerado: ");
	scanf("%f", &b);	

	generating_vectors(a, b);

    //A função pthread_create() inicia um novo thread no processo de chamada.
	//Fazemos isso dentro de um for pq temos que gerar a quantidade de threads
	//que foi informado pela linha de comando.
	//Ela espera o endereço da memóra de uma thread (No nosso caso a gente passa 
	//thread_handles na posição thread), os atributos para uma thread (caso queira
	//manter o padrão passa NULL), a função que sera exeutada paralelamente e um argumento
	//caso vc queira passar.
	//Voce ainda pode verificar se a thread foi criada com sucesso verificando se o
	//retorno dela é == 0. Qualquer número diferente de 0, é porque ela não foi
	//criada com sucesso
	for(long thread = 0; thread < thread_count; thread ++)
		pthread_create(&thread_handles[thread], NULL, print_vectors, (void*) thread);

    //pthread_join espera a finalização de algum procedimento caso seja necessário. 
	//Ela recebe como parâmetros a estrutura de controle do thread que será
	//finalizado e o endereço de um ponteiro (void**) para o valor de retorno da thread.
	for(long thread = 0; thread < thread_count; thread ++)
		pthread_join(thread_handles[thread], NULL);

	free(thread_handles);

	return 0;
}

float float_rand(float min, float max){
	float scale = rand() / (float) RAND_MAX;
	return min + scale * (max - min);
}

void generating_vectors(float a, float b){
	for(int i=0; i<VECTOR_SIZE; i++)
		v[i] = float_rand(a, b);

	//truncando os valores
	for(int i=0; i<VECTOR_SIZE; i++)
		v_trunc[i] = trunc(v[i]);

	//insertion sort
	int key;
	for(int i=1; i<VECTOR_SIZE; i++){
		key = v_trunc[i];
		int j = i -1;
		while(j >= 0 && v_trunc[j] > key){
			v_trunc[j + 1] = v_trunc[j];
			j = j-1;
		}

		v_trunc[j + 1] = key;
	}

	//contando numeros repetidos	
	for(int i=0; i<VECTOR_SIZE; i++)	
		quantidade[i] = 0;

	int x = 0;
	for(int i=0; i<VECTOR_SIZE; i++){
		i--;
		do{
			i++;
			quantidade[x]++;
			if((i + 1) == VECTOR_SIZE)	
				break;

		}while(v_trunc[i] == v_trunc[i + 1]);
		x++;
	}

	//criando vetor sem numeros repetidos
	for(int i=0; i<VECTOR_SIZE; i++)
		result[i] = -1;

	int j, m=0;
	for(int i=0; i<VECTOR_SIZE; i++){
		for(j = 0; j < m; j++)
			if(v_trunc[i] == result[j])
				break;
		if(j == m){
			result[m] = v_trunc[i];
			m++;
		}
	}

    //A variável loop_numbers vai ser responsável por conter o tamanho do vetor
	//que será exibido no histograma.
	//Pela razão de não conter números repetidos, ele ficará menor do que o vetor v[].
	//E o for para exibir o histograma, então, só deve ir de 0 a loop_numbers
	int i=0;
	while(result[i] != -1){
		loop_numbers++;
		i++;
	}
}

//Essa função imprimirá paralelamente o histograma
void* print_vectors(void *rank){
	long my_rank = (long) rank;

    //Como o número de threads podem ser maior do que o número de loops necessários para exibir
	//o histograma, fazemos essa verificação
	if(my_rank <= loop_numbers){

        //A variável end fica responsável por identificar até onde o loop deve ir.
		//Caso o rank da thread seja igual a quantidade de thread - 1, significa que 
		//o loopr está sendo maior do que a quantidade de threads, logo a última thread
		//executa o loop até o fim
		int end = (my_rank == thread_count - 1) ? loop_numbers : my_rank;

		for(int i=(my_rank -1); i<end; i++){
			printf("%d", result[i]);
			for(int j=0; j<quantidade[i]; j++)
				printf("#");
			printf("%d \n", quantidade[i]);
		}
	}
}
