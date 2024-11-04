#include <assert.h>
#include <stdlib.h>

struct Link
{
    struct Link* prev;
    struct Link* succ;
    int elem;
};

struct List
{
    struct Link* head;
    struct Link* tail;
    int size;
};

struct Link* MakeInt(int value);

void Init(struct List* lst);          // Инициализируем lst
struct List* Create(void);            // Создает пустой список
void Clear(struct List* lst);         // Освобождает все элементы lst
void Destroy(struct List* lst);       // Удаляет все элементы lst, затем удаляет сам список lst

void PushBack(struct List* lst,       // Добавляет элемент p в конец списка lst
              struct Link* p);
void PopBack(struct List* lst);       // Удаляет элемент из конца списка lst
void PushFront(struct List* lst,      // Добавляет элемент p в начало списка lst
               struct Link* p);
void PopFront(struct List* lst);      // Удаляет элемент из начала списка lst

struct Link* Insert(struct List* lst, // Вставляет элемент p после элемента aft в список lst
                    struct Link* aft,
                    struct Link* p);
struct Link* Erase(struct List* lst,  // Удаляет узел p из списка lst, возвращает указатель
                   struct Link* p);   // на следующий узел

struct Link* Advance(struct Link* p,  // Сдвигает указатель на n позиций
                     int n);
