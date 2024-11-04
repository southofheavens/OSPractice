#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int sz; // Длина названия файла
    if (read(fileno(stdin), &sz, sizeof(int)) == -1) 
    {            
        perror("pipe error");
        return -1;
    }
    char filename[sz+1]; // Название файла
    int i;
    for (i = 0; i < sz; ++i)
    {
        if (read(fileno(stdin), &filename[i], sizeof(char)) == -1) 
        {
            perror("pipe error");
            return -1;
        }
    }
    filename[sz] = 0;   // Нуль-терминатор

    int lstSize;        // Размер списка
                        // Необходим для того, чтобы дочерний процесс знал,
                        // сколько ему нужно считать чисел из pipe
    if (read(fileno(stdin), &lstSize, sizeof(int)) == -1) 
    {            
        perror("pipe error");
        return -1;
    }
        
    // Считываем числа из pipe, складываем их и записываем результат в файл
    int result = 0;     
    int value;
    for (i = 0; i < lstSize; ++i)
    {
        if (read(fileno(stdin), &value, sizeof(int)) == -1) 
        {            
            perror("pipe error");
            return -1;
        }
        result += value;
    }

    FILE* file;
    file = fopen(filename, "w");
    if (file == NULL) 
    {
        perror("file open error");
        return -1;
    }
    fprintf(file, "%d\n", result);
    fclose(file);
    
    exit(0);
}
