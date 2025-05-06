#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Структура для хранения данных о семье
typedef struct Family {
    char surname[50];
    char name[50];
    char patronymic[50];
    char faculty[50];
    char father_specialty[50];
    char mother_specialty[50];
    int siblings_count;
    struct Family *prev;
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
    new_family->prev = NULL;
    new_family->next = NULL;
    return new_family;
}

// Получение длины списка
int get_list_length(Family *head) {
    int length = 0;
    while (head) {
        length++;
        head = head->next;
    }
    return length;
}

// Добавление нового элемента по индексу
void add_family_at_index(Family **head, Family *new_family, int index) {
    int length = get_list_length(*head);
    
    // Проверка допустимости индекса
    if (index < 0 || index > length) {
        printf("Invalid index. Must be between 0 and %d.\n", length);
        free(new_family);
        return;
    }
    
    // Добавление в начало списка
    if (index == 0) {
        new_family->next = *head;
        if (*head) {
            (*head)->prev = new_family;
        }
        *head = new_family;
        return;
    }
    
    // Добавление в конец списка
    if (index == length) {
        Family *current = *head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_family;
        new_family->prev = current;
        return;
    }
    
    // Добавление в середину списка
    Family *current = *head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }
    
    new_family->next = current->next;
    new_family->prev = current;
    current->next->prev = new_family;
    current->next = new_family;
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
        Family *new_family = create_family(surname, name, patronymic, faculty, 
                                          father_specialty, mother_specialty, siblings_count);
        add_family_at_index(&head, new_family, get_list_length(head));
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

// Функция для получения элемента по индексу
Family *get_family_by_index(Family *head, int index) {
    Family *current = head;
    int current_index = 0;
    while (current && current_index < index) {
        current = current->next;
        current_index++;
    }
    return current;
}

// Удаление по индексу
void delete_family_by_index(Family **head, int index) {
    if (*head == NULL) {
        printf("List is empty.\n");
        return;
    }
    
    int length = get_list_length(*head);
    if (index < 0 || index >= length) {
        printf("Invalid index. Must be between 0 and %d.\n", length - 1);
        return;
    }
    
    Family *current = *head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    
    if (current->prev) {
        current->prev->next = current->next;
    } else {
        *head = current->next;
    }
    
    if (current->next) {
        current->next->prev = current->prev;
    }
    
    free(current);
    printf("Record deleted successfully.\n");
}

// Быстрая сортировка для числового поля (количество братьев и сестер)
void quick_sort_siblings(Family **head, Family *left, Family *right) {
    if (!left || !right || left == right || left == right->next) return;
    
    Family *pivot = right;
    Family *i = left->prev;
    
    for (Family *j = left; j != right; j = j->next) {
        if (j->siblings_count <= pivot->siblings_count) {
            i = (i == NULL) ? left : i->next;
            
            // Обмен значениями
            Family tmp = *i;
            *i = *j;
            *j = tmp;
            
            // Обмен указателями next и prev
            Family *temp_next = i->next;
            Family *temp_prev = i->prev;
            i->next = j->next;
            i->prev = j->prev;
            j->next = temp_next;
            j->prev = temp_prev;
            
            // Корректировка соседних элементов
            if (i->next) i->next->prev = i;
            if (i->prev) i->prev->next = i;
            if (j->next) j->next->prev = j;
            if (j->prev) j->prev->next = j;
        }
    }
    
    i = (i == NULL) ? left : i->next;
    
    // Обмен значениями с pivot
    Family tmp = *i;
    *i = *right;
    *right = tmp;
    
    // Обмен указателями next и prev
    Family *temp_next = i->next;
    Family *temp_prev = i->prev;
    i->next = right->next;
    i->prev = right->prev;
    right->next = temp_next;
    right->prev = temp_prev;
    
    // Корректировка соседних элементов
    if (i->next) i->next->prev = i;
    if (i->prev) i->prev->next = i;
    if (right->next) right->next->prev = right;
    if (right->prev) right->prev->next = right;
    
    // Рекурсивная сортировка левой и правой частей
    quick_sort_siblings(head, left, i->prev);
    quick_sort_siblings(head, i->next, right);
}

// Сортировка слиянием для текстового поля (факультет)
Family *merge(Family *left, Family *right, bool (*compare)(const char *, const char *)) {
    if (!left) return right;
    if (!right) return left;
    
    if (compare(left->faculty, right->faculty)) {
        left->next = merge(left->next, right, compare);
        left->next->prev = left;
        left->prev = NULL;
        return left;
    } else {
        right->next = merge(left, right->next, compare);
        right->next->prev = right;
        right->prev = NULL;
        return right;
    }
}

void split(Family *head, Family **left, Family **right) {
    Family *slow = head;
    Family *fast = head->next;
    
    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    
    *left = head;
    *right = slow->next;
    slow->next = NULL;
    if (*right) (*right)->prev = NULL;
}

void merge_sort_faculty(Family **headRef, bool (*compare)(const char *, const char *)) {
    Family *head = *headRef;
    if (!head || !head->next) return;
    
    Family *left, *right;
    split(head, &left, &right);
    
    merge_sort_faculty(&left, compare);
    merge_sort_faculty(&right, compare);
    
    *headRef = merge(left, right, compare);
}

bool compare_length(const char *a, const char *b) {
    return strlen(a) <= strlen(b);
}

// Меню редактирования
void edit_menu(Family *family) {
    int choice;
    do {
        printf("\nEdit Menu for %s %s:\n", family->surname, family->name);
        printf("1. Edit surname\n");
        printf("2. Edit name\n");
        printf("3. Edit patronymic\n");
        printf("4. Edit faculty\n");
        printf("5. Edit father's specialty\n");
        printf("6. Edit mother's specialty\n");
        printf("7. Edit number of siblings\n");
        printf("8. Edit all fields\n");
        printf("9. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("Enter new surname: ");
                scanf("%s", family->surname);
                break;
            case 2:
                printf("Enter new name: ");
                scanf("%s", family->name);
                break;
            case 3:
                printf("Enter new patronymic: ");
                scanf("%s", family->patronymic);
                break;
            case 4:
                printf("Enter new faculty: ");
                scanf("%s", family->faculty);
                break;
            case 5:
                printf("Enter new father's specialty: ");
                scanf("%s", family->father_specialty);
                break;
            case 6:
                printf("Enter new mother's specialty: ");
                scanf("%s", family->mother_specialty);
                break;
            case 7:
                printf("Enter new number of siblings: ");
                scanf("%d", &family->siblings_count);
                break;
            case 8:
                printf("Enter new surname: ");
                scanf("%s", family->surname);
                printf("Enter new name: ");
                scanf("%s", family->name);
                printf("Enter new patronymic: ");
                scanf("%s", family->patronymic);
                printf("Enter new faculty: ");
                scanf("%s", family->faculty);
                printf("Enter new father's specialty: ");
                scanf("%s", family->father_specialty);
                printf("Enter new mother's specialty: ");
                scanf("%s", family->mother_specialty);
                printf("Enter new number of siblings: ");
                scanf("%d", &family->siblings_count);
                break;
            case 9:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        printf("Record updated successfully.\n");
    } while (choice != 9);
}

// Функция для добавления новой записи с выбором позиции
void add_family_interactive(Family **head) {
    char surname[50], name[50], patronymic[50], faculty[50];
    char father_specialty[50], mother_specialty[50];
    int siblings_count;
    int index;
    int length = get_list_length(*head);

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

    // Запрос индекса с проверкой
    while (1) {
        printf("Enter index to add (0-%d): ", length);
        scanf("%d", &index);
        
        if (index >= 0 && index <= length) {
            break;
        }
        printf("Invalid index. Must be between 0 and %d.\n", length);
    }

    Family *new_family = create_family(surname, name, patronymic, faculty, 
                                      father_specialty, mother_specialty, siblings_count);
    add_family_at_index(head, new_family, index);
}

// Функция для получения последнего элемента списка
Family *get_tail(Family *head) {
    if (!head) return NULL;
    while (head->next) {
        head = head->next;
    }
    return head;
}

// Главное меню
void display_menu() {
    printf("\nMain Menu:\n");
    printf("1. Load family data from file\n");
    printf("2. Display family list\n");
    printf("3. Add new family member\n");
    printf("4. Edit family member by index\n");
    printf("5. Delete family member by index\n");
    printf("6. Sort by number of siblings (Quick Sort)\n");
    printf("7. Sort by length of faculty (Merge Sort)\n");
    printf("8. Save family data to file\n");
    printf("9. Exit\n");
}

// Главная функция
int main() {
    Family *family_list = NULL;
    int choice;
    char filename[100];
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
                } else {
                    printf("Data loaded successfully.\n");
                }
                break;

            case 2:
                if (family_list == NULL) {
                    printf("Family list is empty.\n");
                } else {
                    printf("Family List:\n");
                    print_family(family_list);
                }
                break;

            case 3:
                add_family_interactive(&family_list);
                break;

            case 4:
                if (family_list == NULL) {
                    printf("Family list is empty.\n");
                    break;
                }
                printf("Current list length: %d\n", get_list_length(family_list));
                printf("Enter index of the member to edit (0-%d): ", get_list_length(family_list) - 1);
                scanf("%d", &index);
                Family *to_edit = get_family_by_index(family_list, index);
                if (!to_edit) {
                    printf("Invalid index.\n");
                } else {
                    edit_menu(to_edit);
                }
                break;

            case 5:
                if (family_list == NULL) {
                    printf("Family list is empty.\n");
                    break;
                }
                printf("Current list length: %d\n", get_list_length(family_list));
                printf("Enter index of the member to delete (0-%d): ", get_list_length(family_list) - 1);
                scanf("%d", &index);
                delete_family_by_index(&family_list, index);
                break;

            case 6:
                if (family_list == NULL) {
                    printf("Family list is empty.\n");
                    break;
                }
                Family *tail = get_tail(family_list);
                quick_sort_siblings(&family_list, family_list, tail);
                printf("List sorted by number of siblings (Quick Sort).\n");
                break;

            case 7:
                if (family_list == NULL) {
                    printf("Family list is empty.\n");
                    break;
                }
                merge_sort_faculty(&family_list, compare_length);
                printf("List sorted by length of faculty (Merge Sort).\n");
                break;

            case 8:
                if (family_list == NULL) {
                    printf("Family list is empty.\n");
                    break;
                }
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
