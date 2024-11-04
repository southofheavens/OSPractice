#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "include/list.h"

struct Filename
{
    char* filename;
    int size;
};

struct Filename GetFilename(void)
{
    struct Filename file;
    char* str;
    int memory = 8;
    str = (char*)malloc(memory);
    int index = 0;
    char c;
    while ((c = getchar()) != '\n')
    {
        str[index] = c;
        index++;
        if (index == (memory - 2))
        {
            memory *= 2;
            str = (char*)realloc(str, memory);
        }
    }
    if (index > 0 && str[index-1] == '\n') {
        str[index-1] = 0;
    }
    else
    {
        str[index] = 0;
    }
    file.filename = str;
    file.size = index;
    return file;
}

int main(void)
{
    int pipeFd[2];          // pipeFd[0] - read, pipeFd[1] - write
    if (pipe(pipeFd) == -1) // Создание канала
    {
        perror("pipe");
        return -1;
    }
    pid_t pid = fork();     // Создание нового процесса
    if (pid == -1)          // Ошибка при создании процесса
    {
        perror("fork");
    }
    if (pid == 0)           // child process code
    {
        dup2(pipeFd[0], fileno(stdin));
        execl("./child.out", NULL);
    }
    else                    // parent process code
    {
        close(pipeFd[0]);
        
        printf("Введите имя файла: ");
        struct Filename file = GetFilename();

        // Записываем в pipe длину названия файла
        if (write(pipeFd[1], &file.size, sizeof(int)) == -1) 
        {
            perror("pipe error");
            return -1;
        }    
        // Записываем в pipe название файла
        int i;
        for (i = 0; i < file.size; ++i)
        {
            if (write(pipeFd[1], &file.filename[i], sizeof(char)) == -1) 
            {
                perror("pipe error");
                return -1;
            }
        }    
        free(file.filename);

        // Объявляем и инициализируем список
        struct List lst;
        Init(&lst);
        printf("Введите массив чисел: ");
        
        // Считываем числа из потока ввода
        int value;
        while ((scanf("%d", &value)) == 1)  // CTRL+D для окончания ввода
        {
            if (value == EOF) { break; }
            PushBack(&lst, MakeInt(value));
        }

        // Записываем в pipe размер списка
        if (write(pipeFd[1], &lst.size, sizeof(int)) == -1) 
        {
            perror("pipe error");
            return -1;
        }

        // Записываем в pipe элементы списка
        struct Link* curr = lst.head;
        curr = lst.head;
        for (i = 0; i < lst.size; ++i)
        {
            if (write(pipeFd[1], &curr->elem, sizeof(int)) == -1)
            {
                perror("pipe error");
                return -1;
            }
            curr = curr->succ;
        }

        Clear(&lst);
        wait(NULL); // Ожидание дочернего процесса
        close(pipeFd[1]);
    }

    return 0;
}
