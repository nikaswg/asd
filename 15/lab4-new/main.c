#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для хранения данных о семье
typedef struct Family {
    char surname[50];
    char name[50];
    char patronymic[50];
    char faculty[50];
    char father_specialty[50];
    char mother_specialty[50];
    int siblings_count;
    struct Family *next;
} Family;

// Создание нового элемента списка
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

// Добавление нового элемента в начало списка
void add_family(Family **head, Family *new_family) {
    new_family->next = *head;
    *head = new_family;
}

// Печать списка семей
void print_family(Family *head) {
    Family *current = head;
    int index = 0;
    printf("%-5s %-15s %-10s %-15s %-20s %-20s %-20s %-9s\n",
           "Idx", "Surname", "Name", "Patronymic", "Faculty", "Father Spec.", "Mother Spec.", "Siblings");
    printf("----------------------------------------------------------------------------------------------------------------------\n");

    while (current) {
        printf("%-5d %-15s %-10s %-15s %-20s %-20s %-20s %-9d\n",
               index++, current->surname, current->name, current->patronymic,
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

// Удаление по фамилии и имени
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

// Редактирование по индексу
void edit_family_by_index(Family *head, int index) {
    Family *current = head;
    int current_index = 0;

    while (current && current_index < index) {
        current = current->next;
        current_index++;
    }

    if (!current) {
        printf("Invalid index.\n");
        return;
    }

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
}

// Меню
void display_menu() {
    printf("\nMenu:\n");
    printf("1. Load family data from file\n");
    printf("2. Display family list\n");
    printf("3. Add new family member\n");
    printf("4. Edit family member by index\n");
    printf("5. Delete family member\n");
    printf("6. Sort by number of siblings\n");
    printf("7. Sort by length of faculty\n");
    printf("8. Save family data to file\n");
    printf("9. Exit\n");
}

// Добавление через ввод
void add_family_interactive(Family **head) {
    char surname[50], name[50], patronymic[50], faculty[50];
    char father_specialty[50], mother_specialty[50];
    int siblings_count;

    printf("Enter surname: ");
    scanf("%s", surname);
    printf("Enter name: ");
    scanf("%s", name);
    printf("Enter patronymic: ");
    scanf("%s", patronymic);
    printf("Enter faculty: ");
    scanf("%s", faculty);
    printf("Enter father's specialty: ");
    scanf("%s", father_specialty);
    printf("Enter mother's specialty: ");
    scanf("%s", mother_specialty);
    printf("Enter number of siblings: ");
    scanf("%d", &siblings_count);

    add_family(head, create_family(surname, name, patronymic, faculty, father_specialty, mother_specialty, siblings_count));
}

// Главная функция
int main() {
    Family *family_list = NULL;
    int choice;
    char filename[100];
    char surname[50], name[50];
    int index;

    while (1) {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter filename to load: ");
                scanf("%s", filename);
                family_list = load_from_file(filename);
                if (!family_list) {
                    printf("Error loading data.\n");
                }
                break;

            case 2:
                printf("Family List:\n");
                print_family(family_list);
                break;

            case 3:
                add_family_interactive(&family_list);
                break;

            case 4:
                printf("Enter index of the member to edit: ");
                scanf("%d", &index);
                edit_family_by_index(family_list, index);
                break;

            case 5:
                printf("Enter surname of the member to delete: ");
                scanf("%s", surname);
                printf("Enter name of the member to delete: ");
                scanf("%s", name);
                delete_family(&family_list, surname, name);
                break;

            case 6:
                sort_by_siblings(&family_list);
                printf("List sorted by number of siblings.\n");
                break;

            case 7:
                sort_by_faculty_length(&family_list);
                printf("List sorted by length of faculty.\n");
                break;

            case 8:
                printf("Enter filename to save: ");
                scanf("%s", filename);
                save_to_file(family_list, filename);
                printf("Data saved to %s.\n", filename);
                break;

            case 9:
                free_list(family_list);
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}