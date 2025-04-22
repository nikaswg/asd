#include "family_data.h"

// Функция для создания нового элемента списка семьи
Family *create_family(char *surname, char *name, char *patronymic, char *faculty,
                      char *father_specialty, char *mother_specialty, int siblings_count) {
    Family *new_family = (Family *)malloc(sizeof(Family));
    if (!new_family) {
        printf("Memory allocation error.\n");
        exit(1);
    }
    strcpy(new_family->surname, surname);
    strcpy(new_family->name, name);
    strcpy(new_family->patronymic, patronymic);
    strcpy(new_family->faculty, faculty);
    strcpy(new_family->father_specialty, father_specialty);
    strcpy(new_family->mother_specialty, mother_specialty);
    new_family->siblings_count = siblings_count;
    new_family->next = NULL;
    return new_family;
}

// Функция для добавления нового элемента в начало списка
void add_family(Family **head, Family *new_family) {
    new_family->next = *head;
    *head = new_family;
}

// Функция для печати списка
void print_family(Family *head) {
    Family *current = head;
    printf("%-15s %-10s %-15s %-20s %-20s %-20s %-9s\n",
           "Surname", "Name", "Patronymic",
           "Faculty", "Father Spec.", "Mother Spec.", "Siblings");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    while (current) {
        printf("%-15s %-10s %-15s %-20s %-20s %-20s %-9d\n",
               current->surname, current->name, current->patronymic,
               current->faculty, current->father_specialty, current->mother_specialty,
               current->siblings_count);
        current = current->next;
    }
}

// Сохранение списка в файл
void save_to_file(Family *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }
    Family *current = head;
    while (current) {
        fprintf(file, "%s %s %s %s %s %s %d\n",
                current->surname, current->name, current->patronymic,
                current->faculty, current->father_specialty, current->mother_specialty,
                current->siblings_count);
        current = current->next;
    }
    fclose(file);
}

// Загрузка списка из файла
Family *load_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return NULL;
    }
    Family *head = NULL;
    char surname[50], name[50], patronymic[50], faculty[50], father_specialty[50], mother_specialty[50];
    int siblings_count;

    while (fscanf(file, "%49s %49s %49s %49s %49s %49s %d",
                  surname, name, patronymic, faculty, father_specialty, mother_specialty, &siblings_count) == 7) {
        add_family(&head, create_family(surname, name, patronymic, faculty, father_specialty, mother_specialty, siblings_count));
    }
    fclose(file);
    return head;
}

// Очистка памяти
void free_list(Family *head) {
    Family *temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Сортировка по количеству братьев и сестер
void sort_by_siblings(Family **head) {
    if (!head || !(*head)) return;

    Family *sorted = NULL;
    Family *current = *head;

    while (current) {
        Family *next = current->next;
        if (!sorted || sorted->siblings_count > current->siblings_count) {
            current->next = sorted;
            sorted = current;
        } else {
            Family *temp = sorted;
            while (temp->next && temp->next->siblings_count <= current->siblings_count) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    *head = sorted;
}

// Сортировка по длине факультета
void sort_by_faculty_length(Family **head) {
    if (!head || !(*head)) return;

    Family *sorted = NULL;
    Family *current = *head;

    while (current) {
        Family *next = current->next;
        if (!sorted || strlen(sorted->faculty) > strlen(current->faculty)) {
            current->next = sorted;
            sorted = current;
        } else {
            Family *temp = sorted;
            while (temp->next && strlen(temp->next->faculty) <= strlen(current->faculty)) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    *head = sorted;
}

// Удаление члена семьи по фамилии и имени
void delete_family(Family **head, const char *surname, const char *name) {
    Family *current = *head, *prev = NULL;

    while (current) {
        if (strcmp(current->surname, surname) == 0 && strcmp(current->name, name) == 0) {
            if (prev) prev->next = current->next;
            else *head = current->next;
            free(current);
            printf("Record deleted successfully.\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Record not found.\n");
}

// Редактирование члена семьи по фамилии и имени
void edit_family(Family *head, const char *surname, const char *name) {
    Family *current = head;

    while (current) {
        if (strcmp(current->surname, surname) == 0 && strcmp(current->name, name) == 0) {
            printf("Enter new patronymic: ");
            scanf("%s", current->patronymic);
            printf("Enter new faculty: ");
            scanf("%s", current->faculty);
            printf("Enter new father's specialty: ");
            scanf("%s", current->father_specialty);
            printf("Enter new mother's specialty: ");
            scanf("%s", current->mother_specialty);
            printf("Enter new number of siblings: ");
            scanf("%d", &current->siblings_count);
            printf("Record updated successfully.\n");
            return;
        }
        current = current->next;
    }
    printf("Record not found.\n");
}
