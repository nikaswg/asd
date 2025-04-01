#include "family_data.h"

// Функция для создания нового элемента списка семьи
Family *create_family(char *surname, char *name, char *patronymic, char *faculty,
                      char *father_specialty, char *mother_specialty, int siblings_count) {
    Family *new_family = (Family *)malloc(sizeof(Family)); // Выделение памяти для нового элемента
    if (!new_family) {
        printf("Memory allocation error.\n"); // Сообщение об ошибке на английском
        exit(1); // Завершение программы в случае ошибки
    }
    // Копирование данных в новый элемент
    strcpy(new_family->surname, surname);
    strcpy(new_family->name, name);
    strcpy(new_family->patronymic, patronymic);
    strcpy(new_family->faculty, faculty);
    strcpy(new_family->father_specialty, father_specialty);
    strcpy(new_family->mother_specialty, mother_specialty);
    new_family->siblings_count = siblings_count; // Присвоение количества братьев/сестер
    new_family->next = NULL; // Изначально следующий элемент равен NULL
    return new_family; // Возвращаем указатель на новый элемент
}

// Функция для добавления нового элемента в начало списка
void add_family(Family **head, Family *new_family) {
    new_family->next = *head; // Новый элемент указывает на текущую голову списка
    *head = new_family; // Обновляем голову списка
}

// Функция для вывода списка семей
void print_family(Family *head) {
    Family *current = head; // Начинаем с головы списка
    while (current) { // Пока текущий элемент не равен NULL
        // Выводим данные о семье
        printf("%s %s %s | Faculty: %s | Father: %s | Mother: %s | Siblings: %d\n",
               current->surname, current->name, current->patronymic,
               current->faculty, current->father_specialty, current->mother_specialty,
               current->siblings_count);
        current = current->next; // Переходим к следующему элементу
    }
}

// Функция для сохранения списка семей в файл
void save_to_file(Family *head, const char *filename) {
    FILE *file = fopen(filename, "w"); // Открываем файл для записи
    if (!file) {
        printf("Error opening file for writing.\n"); // Сообщение об ошибке на английском
        return; // Если не удалось открыть файл, возвращаемся
    }
    Family *current = head; // Начинаем с головы списка
    while (current) { // Пока текущий элемент не равен NULL
        // Записываем данные о семье в файл
        fprintf(file, "%s %s %s %s %s %s %d\n",
                current->surname, current->name, current->patronymic,
                current->faculty, current->father_specialty, current->mother_specialty,
                current->siblings_count);
        current = current->next; // Переходим к следующему элементу
    }
    fclose(file); // Закрываем файл
}

// Функция для загрузки списка семей из файла
Family *load_from_file(const char *filename) {
    FILE *file = fopen(filename, "r"); // Открываем файл для чтения
    if (!file) {
        printf("Error opening file for reading.\n"); // Сообщение об ошибке на английском
        return NULL; // Если не удалось открыть файл, возвращаем NULL
    }
    Family *head = NULL; // Изначально список пуст
    char surname[50], name[50], patronymic[50], faculty[50], father_specialty[50], mother_specialty[50];
    int siblings_count;

    // Читаем данные из файла, пока это возможно
    while (fscanf(file, "%49s %49s %49s %49s %49s %49s %d",
                  surname, name, patronymic, faculty, father_specialty, mother_specialty, &siblings_count) == 7) {
        // Создаем новый элемент и добавляем его в список
        add_family(&head, create_family(surname, name, patronymic, faculty, father_specialty, mother_specialty, siblings_count));
    }
    fclose(file); // Закрываем файл
    return head; // Возвращаем указатель на голову списка
}

// Функция для освобождения памяти, занятой списком
void free_list(Family *head) {
    Family *temp;
    while (head) { // Пока список не пуст
        temp = head; // Сохраняем текущий элемент
        head = head->next; // Переходим к следующему элементу
        free(temp); // Освобождаем память
    }
}

// Функция для сортировки по количеству братьев и сестер
void sort_by_siblings(Family **head) {
    if (!head || !(*head)) return; // Если список пуст, ничего не делаем

    Family *sorted = NULL; // Указатель на отсортированный список
    Family *current = *head; // Начинаем с головы списка

    // Проходим по исходному списку
    while (current) {
        Family *next = current->next; // Сохраняем следующий элемент
        // Вставляем элемент в отсортированный список
        if (!sorted || sorted->siblings_count > current->siblings_count) {
            current->next = sorted; // Новый элемент указывает на начало отсортированного списка
            sorted = current; // Обновляем указатель на отсортированный список
        } else {
            Family *temp = sorted;
            // Находим место для вставки
            while (temp->next && temp->next->siblings_count <= current->siblings_count) {
                temp = temp->next; // Переходим к следующему элементу
            }
            current->next = temp->next; // Вставляем элемент
            temp->next = current; // Обновляем указатель
        }
        current = next; // Переходим к следующему элементу
    }
    *head = sorted; // Обновляем указатель на голову списка
}

// Функция для сортировки по длине факультета
void sort_by_faculty_length(Family **head) {
    if (!head || !(*head)) return; // Если список пуст, ничего не делаем

    Family *sorted = NULL; // Указатель на отсортированный список
    Family *current = *head; // Начинаем с головы списка

    // Проходим по исходному списку
    while (current) {
        Family *next = current->next; // Сохраняем следующий элемент
        // Вставляем элемент в отсортированный список
        if (!sorted || strlen(sorted->faculty) > strlen(current->faculty)) {
            current->next = sorted; // Новый элемент указывает на начало отсортированного списка
            sorted = current; // Обновляем указатель на отсортированный список
        } else {
            Family *temp = sorted;
            // Находим место для вставки
            while (temp->next && strlen(temp->next->faculty) <= strlen(current->faculty)) {
                temp = temp->next; // Переходим к следующему элементу
            }
            current->next = temp->next; // Вставляем элемент
            temp->next = current; // Обновляем указатель
        }
        current = next; // Переходим к следующему элементу
    }
    *head = sorted; // Обновляем указатель на голову списка
}
