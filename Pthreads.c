/*
create by Simões 

a função do programa é preeencer um vetor de 10 posições, sendo metade dele preenchido pela thread 1 e a outra metade pela thread 2..
para identificação, foi preenchido com valor 1 quando fosse executado pela thread 1 e com valor 2 quando fosse executado pela thread 2
*/
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

#define SIZE 10
int v[SIZE];

void* function(void *arg){
    int *valor = (int*)(arg);
    int i=0;

    // aqui verifico se é a primeira thread (valor == 1)
    if(valor == 1)
        for (i=0; i<SIZE / 2; i++)
            v[i] = 1;
    
    else 
        for (i = SIZE / 2; i<SIZE; i++)
            v[i] = 2;
    
}

int main(){
    pthread_t t1, t2;
    int a1 , a2;

    //pthread_create inicia uma nova thread e chama o processo passado por parâmetro para ser executado nessa thread
    //nesse caso o processo passado por parametro é a função 'function'
    int suss_1 = pthread_create(&t1, NULL, function, (void *)(&a1));
    int suss_2 = pthread_create(&t2, NULL, function, (void *)(&a2));

    /*
    a função pthread_create retorna o valor 0 caso ela seja executada com sucesso, e qualquer outro valor caso ela tenha 
    identificado algum erro
    */
    if(suss_1 == 0)
        printf("Thread 1 executada com sucesso\n");
    else 
        printf("Thread 1 não foi executada com sucesso\n");

    if(suss_2 == 0)
        printf("Thread 2 executada com sucesso\n");
    else 
        printf("Thread 1 não foi executada com sucesso\n");

    //a função pthread_join espera a execução a thread passada como parametro terminar (nesse caso é t1)..
    //se essa execução terminar, ele retorna ao pthread_join imediatamente
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    //imprimindo o vetor
    for(int i=0; i< SIZE; i++)
        printf("%d, ", v[i]);

    printf("\n");

    exit(0);
}