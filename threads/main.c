#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// Для измерения времени выполнения фрагмента кода
struct timeval start, end;
long seconds, useconds;
double elapsed_time;

void Start()
{
    gettimeofday(&start, NULL);
}

void End()
{
    gettimeofday(&end, NULL);
    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    elapsed_time = seconds + useconds / 1000000.0;
    printf("Lead time: %.3f seconds\n", elapsed_time);
}

// Генерирует массив размера sz, содержащий случайные числа,
// и возвращает указатель на него
int* GenArray(int sz)
{
    srand((int)time(NULL)); // используется для инициализации
                            // генератора псевдослучайных чисел.
    int* arr = (int*)malloc(sizeof(int) * sz);
    int i;
    for (i = 0; i < sz; ++i) {
        arr[i] = rand() % INT32_MAX - (INT32_MAX / 2);
    }
    return arr;
}

// Структура для передачи в качестве аргумента при создании потока
struct ThreadArgs
{
    int* arr;
    int l;
    int r;
};

// Структура для возврата результата из функции GetMinMax
struct MinMax
{
    int min;
    int max;
};

// Находит минимальный и максимальный элементы на отрезке и
// возвращает указатель типа void* на объект структуры MinMax
void* GetMinMax(void* args)
{
    struct ThreadArgs* ta = (struct ThreadArgs*)args;
    struct MinMax* result = (struct MinMax*)malloc(sizeof(struct MinMax));
    result->min = INT32_MAX;
    result->max = INT32_MIN;

    int i = ta->l;
    for (; i < ta->r; ++i)
    {
        if (ta->arr[i] > result->max) {
            result->max = ta->arr[i];
        }
        if (ta->arr[i] < result->min) {
            result->min = ta->arr[i];
        }
    }

    free(ta);
    return (void*)result;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Необходимо передать количество потоков в качестве ключа запуска программы!\n");
        return -1;
    }
    const int threadCount = atoi(argv[1]);

    const int ARRAY_SIZE = 123456789;
    int* arr = GenArray(ARRAY_SIZE);

    pthread_t tid[threadCount - 1]; // массив для хранения идентификаторов потоков
    
    int l = 0;
    int r = ARRAY_SIZE / threadCount;
    int increase = r;
    
    // Начало измерения времени выполнения фрагмента кода
    Start();

    int i;
    for (i = 0; i < threadCount - 1; i++)
    {
        struct ThreadArgs* args = (struct ThreadArgs*)malloc(sizeof(struct ThreadArgs));
        args->arr = arr;
        args->l = l;
        args->r = r;
        pthread_create(&tid[i], NULL, GetMinMax, args);
        l += increase;
        r += increase;
    }

    struct ThreadArgs* args = (struct ThreadArgs*)malloc(sizeof(struct ThreadArgs));
    args->arr = arr;
    args->l = l;
    args->r = ARRAY_SIZE;

    void* results[threadCount];
    results[0] = (struct MinMax*)GetMinMax(args);
    for (i = 0; i < threadCount - 1; ++i) {
        pthread_join(tid[i], &results[i+1]);
    }
    
    int min = INT32_MAX;
    int max = INT32_MIN;
    for (i = 0; i < threadCount; ++i)
    {
        if (((struct MinMax*)results[i])->min < min) {
            min = ((struct MinMax*)results[i])->min;
        }
        if (((struct MinMax*)results[i])->max > max) {
            max = ((struct MinMax*)results[i])->max;
        }
        free(results[i]);
    }
    
    printf("With threads\n");
    printf("min: %d, max: %d\n", min, max);
    
    // Конец измерения времени выполнения фрагмента кода
    End();
    printf("\n");
    
    // Начало измерения времени выполнения фрагмента кода
    Start();

    printf("Without threads\n");
    args = (struct ThreadArgs*)malloc(sizeof(struct ThreadArgs));
    args->arr = arr;
    args->l = 0;
    args->r = ARRAY_SIZE;
    struct MinMax* result = (struct MinMax*)GetMinMax(args);
    printf("min: %d, max: %d\n", result->min, result->max);
    free(result);
    
    // Конец измерения времени выполнения фрагмента кода
    End();
    
    free(arr);
    return 0;
}
