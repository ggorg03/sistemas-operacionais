#include <stdio.h>
#include <pthread.h>

int f (int x){
    return 5;
}

void* calcula_area_sobcurva (void* entrada){
    // convertendo entrada para vetor de floats
    float* entradas = (float*)entrada;
    // coletando os parametros
    float a = entradas[0];
    float b = entradas[1];
    int qtd_trpz = entradas[2];
    // calculando a area dos trapezios
    float h = (b - a) / qtd_trpz;
    float area = (f(a) + f(b)) / 2;
    for (int i = 1; i < qtd_trpz; i++){
        area += f(a + i*h);
    }
    area *= h;
    // retornando a area dos trapezios
    pthread_exit((void*)(size_t)area);
}

int main(void){
    float a;
    float b;
    int qtd_threads;
    int qtd_trpz;
    float area = 0;
    int status;
    void* thread_return;
    // inicializando variaveis
    a = 0;
    b = 10;
    qtd_trpz = 10;
    qtd_threads = 5;
    // calculando quantidade de trapezios por tread
    int trpz_por_thread = (int) qtd_trpz / qtd_threads;
    // incializando threads
    pthread_t threads[qtd_threads];
    // calculando o comprimeto de cada thread
    float h = (b - a) / qtd_threads;
    // rodando threads
    for (int i = 0; i < qtd_threads; i++){
        float entradas[] = {a + i*h, a + (i + 1)*h, trpz_por_thread};
        status = pthread_create(&threads[i], NULL, calcula_area_sobcurva, (void*) entradas);
        if(status != 0){
            printf("Erro na criacao da thread. Codigo do erro: %d", status);
        }
    }
    // coletando o resultado de cada thread e acumulando o valor na variavel 'area'
    for (int i = 0; i < qtd_threads; i++) {
        pthread_join(threads[i], &thread_return);
        area += (int)(size_t)thread_return;
    }
    // printando a area sob a curva da funcao
    printf("Area %f\n", area);
    return 0;
}
