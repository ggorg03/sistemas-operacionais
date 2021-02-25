#include <stdio.h>
#include <pthread.h>
#include <math.h>


typedef struct{
    float a;
    float b;
    int qtd_trpz;
    float area;
} Args;

int f_a (float x){
    return 5;
}

int f_b (float x){
    return sin(2 * x) + cos(5 * x);
}

float calcula_area_sobcurva_fa (float a, float b, int qtd_trpz){
    // calculando a area dos trapezios
    float h = (b - a) / qtd_trpz;
    float area = (f_a(a) + f_a(b)) / 2;
    for (int i = 1; i < qtd_trpz; i++){
        area += f_a(a + i*h);
    }
    area *= h;
    return area;
}

float calcula_area_sobcurva_fb (float a, float b, int qtd_trpz){
    // calculando a area dos trapezios
    float h = (b - a) / qtd_trpz;
    float area = (f_b(a) + f_b(b)) / 2;
    for (int i = 1; i < qtd_trpz; i++){
        area += f_b(a + i*h);
    }
    area *= h;
    return area;
}

void* t_calcula_area_sobcurva_fa (void* args){
    Args* p_args = (Args*) args;
    float local_a = p_args->a;
    float local_b = p_args->b;
    int qtd_trpz = p_args->qtd_trpz;
    p_args->area = calcula_area_sobcurva_fa(local_a, local_b, qtd_trpz);
    pthread_exit(NULL);
}

void* t_calcula_area_sobcurva_fb (void* args){
    Args* p_args = (Args*) args;
    float local_a = p_args->a;
    float local_b = p_args->b;
    int qtd_trpz = p_args->qtd_trpz;
    p_args->area = calcula_area_sobcurva_fb(local_a, local_b, qtd_trpz);
    pthread_exit(NULL);
}

void teste_fa(){
    float a;
    float b;
    int qtd_threads;
    int qtd_trpz;
    float area = 0;
    int status;
    void* thread_return;
    // inicializando variaveis
    // teste funcao A
    a = 0;
    b = 10;
    qtd_trpz = 120;
    qtd_threads = 6;
    printf("Rodando função f1 com a = %f, b = %f, %d trapezios e %d threads\n", a, b, qtd_trpz, qtd_threads);
    // calculando quantidade de trapezios por tread
    int trpz_por_thread = qtd_trpz / qtd_threads;
    int trpz_thread_extra = qtd_trpz % qtd_threads;
    // incializando threads
    if(trpz_thread_extra != 0){
        qtd_threads++;
    }
    pthread_t threads[qtd_threads];
    Args argss[qtd_threads];
    // calculando o comprimeto de cada thread
    float h = (b - a) / qtd_threads;
    // rodando threads
    for (int i = 0; i < qtd_threads; i++){
        argss[i].a = a + i*h;
        argss[i].b = a + (i + 1)*h;
        argss[i].qtd_trpz = trpz_por_thread;
        //printf("a + i*h = %f\na + (i + 1)*h = %f\ntrpz_por_threa = %d\nd",a + i*h, a + (i + 1)*h, trpz_por_thread);
        // identificando se estamos na ultima thread
        if (i == qtd_threads - 1){
            // checa se é a thread extra
            if(trpz_thread_extra != 0){
                argss[i].qtd_trpz = trpz_thread_extra; // altera a quantidade de threads
            }
        }
        status = pthread_create(&threads[i], NULL, t_calcula_area_sobcurva_fa, &argss[i]);
        if(status != 0){
            printf("Erro na criacao da thread. Codigo do erro: %d", status);
        }
    }
    // coletando o resultado de cada thread e acumulando o valor na variavel 'area'
    for (int i = 0; i < qtd_threads; i++) {
        pthread_join(threads[i], &thread_return);
        area += argss[i].area;
    }
    // printando a area sob a curva da funcao
    printf("Area = %f\n", area);
}

void teste_fb(){
    float a;
    float b;
    float pi = 3.14159;
    int qtd_threads;
    int qtd_trpz;
    float area = 0;
    int status;
    void* thread_return;
    // inicializando variaveis
    // teste funcao A
    a = 0;
    b = 2 * pi;
    qtd_trpz = 120;
    qtd_threads = 6;
    printf("Rodando função f2 com a = %f, b = %f * pi, %d trapezios e %d threads\n", a, b / pi, qtd_trpz, qtd_threads);
    // calculando quantidade de trapezios por tread
    int trpz_por_thread = qtd_trpz / qtd_threads;
    int trpz_thread_extra = qtd_trpz % qtd_threads;
    // incializando threads
    if(trpz_thread_extra != 0){
        qtd_threads++;
    }
    pthread_t threads[qtd_threads];
    Args argss[qtd_threads];
    // calculando o comprimeto de cada thread
    float h = (b - a) / qtd_threads;
    // rodando threads
    for (int i = 0; i < qtd_threads; i++){
        argss[i].a = a + i*h;
        argss[i].b = a + (i + 1)*h;
        argss[i].qtd_trpz = trpz_por_thread;
        //printf("a + i*h = %f\na + (i + 1)*h = %f\ntrpz_por_threa = %d\nd",a + i*h, a + (i + 1)*h, trpz_por_thread);
        // identificando se estamos na ultima thread
        if (i == qtd_threads - 1){
            // checa se é a thread extra
            if(trpz_thread_extra != 0){
                argss[i].qtd_trpz = trpz_thread_extra; // altera a quantidade de threads
            }
        }
        status = pthread_create(&threads[i], NULL, t_calcula_area_sobcurva_fb, &argss[i]);
        if(status != 0){
            printf("Erro na criacao da thread. Codigo do erro: %d", status);
        }
    }
    // coletando o resultado de cada thread e acumulando o valor na variavel 'area'
    for (int i = 0; i < qtd_threads; i++) {
        pthread_join(threads[i], &thread_return);
        area += argss[i].area;
    }
    // printando a area sob a curva da funcao
    printf("Area = %f\n", area);
}

int main(void){
    teste_fa();
    printf("\n");
    teste_fb();

    return 0;
}
