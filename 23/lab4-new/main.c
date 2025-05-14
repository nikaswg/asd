#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для хранения данных о сети магазинов
typedef struct StoreChain {
    int number;
    char name[50];
    char address[100];
    char phone[20];
    char owner_name[100];
    char owner_address[100];
    double capital;
    struct StoreChain *next;
} StoreChain;

// Создание нового элемента списка
StoreChain *create_store(int number, char *name, char *address, char *phone,
                         char *owner_name, char *owner_address, double capital) {
    StoreChain *new_store = (StoreChain *)malloc(sizeof(StoreChain));
    if (!new_store) {
        printf("Memory allocation error.\n");
        exit(1);
    }
    new_store->number = number;
    strcpy(new_store->name, name);
    strcpy(new_store->address, address);
    strcpy(new_store->phone, phone);
    strcpy(new_store->owner_name, owner_name);
    strcpy(new_store->owner_address, owner_address);
    new_store->capital = capital;
    new_store->next = NULL;
    return new_store;
}

// Добавление нового элемента в начало списка
void add_store(StoreChain **head, StoreChain *new_store) {
    new_store->next = *head;
    *head = new_store;
}

// Печать списка магазинов
void print_stores(StoreChain *head) {
    StoreChain *current = head;
    int index = 0;
    printf("%-5s %-5s %-20s %-30s %-15s %-30s %-30s %-15s\n",
           "Idx", "No.", "Name", "Address", "Phone", "Owner Name", "Owner Address", "Capital");
    printf("----------------------------------------------------------------------------------------------------------------------\n");

    while (current) {
        printf("%-5d %-5d %-20s %-30s %-15s %-30s %-30s %-15.2f\n",
               index++, current->number, current->name, current->address,
               current->phone, current->owner_name, current->owner_address,
               current->capital);
        current = current->next;
    }
}

// Сохранение списка в файл
void save_to_file(StoreChain *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }
    StoreChain *current = head;
    while (current) {
        fprintf(file, "%d %s %s %s %s %s %.2f\n",
                current->number, current->name, current->address,
                current->phone, current->owner_name, current->owner_address,
                current->capital);
        current = current->next;
    }
    fclose(file);
}

// Загрузка списка из файла
StoreChain *load_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return NULL;
    }
    StoreChain *head = NULL;
    int number;
    char name[50], address[100], phone[20], owner_name[100], owner_address[100];
    double capital;

    while (fscanf(file, "%d %49s %99s %19s %99s %99s %lf",
                  &number, name, address, phone, owner_name, owner_address, &capital) == 7) {
        add_store(&head, create_store(number, name, address, phone, owner_name, owner_address, capital));
    }
    fclose(file);
    return head;
}

// Очистка памяти
void free_list(StoreChain *head) {
    StoreChain *temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Сортировка по длине ФИО владельца (по возрастанию) - метод пузырька
void sort_by_owner_name_length(StoreChain **head) {
    if (!head || !(*head) || !(*head)->next) return;
    
    int swapped;
    StoreChain *ptr1;
    StoreChain *lptr = NULL;
    
    do {
        swapped = 0;
        ptr1 = *head;
        
        while (ptr1->next != lptr) {
            if (strlen(ptr1->owner_name) > strlen(ptr1->next->owner_name)) {
                // Меняем данные местами
                int temp_num = ptr1->number;
                ptr1->number = ptr1->next->number;
                ptr1->next->number = temp_num;
                
                char temp_str[100];
                
                strcpy(temp_str, ptr1->name);
                strcpy(ptr1->name, ptr1->next->name);
                strcpy(ptr1->next->name, temp_str);
                
                strcpy(temp_str, ptr1->address);
                strcpy(ptr1->address, ptr1->next->address);
                strcpy(ptr1->next->address, temp_str);
                
                strcpy(temp_str, ptr1->phone);
                strcpy(ptr1->phone, ptr1->next->phone);
                strcpy(ptr1->next->phone, temp_str);
                
                strcpy(temp_str, ptr1->owner_name);
                strcpy(ptr1->owner_name, ptr1->next->owner_name);
                strcpy(ptr1->next->owner_name, temp_str);
                
                strcpy(temp_str, ptr1->owner_address);
                strcpy(ptr1->owner_address, ptr1->next->owner_address);
                strcpy(ptr1->next->owner_address, temp_str);
                
                double temp_cap = ptr1->capital;
                ptr1->capital = ptr1->next->capital;
                ptr1->next->capital = temp_cap;
                
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// Сортировка по капиталу владельцев (по убыванию) - метод вставки
void sort_by_capital(StoreChain **head) {
    if (!head || !(*head)) return;
    
    StoreChain *sorted = NULL;
    StoreChain *current = *head;
    
    while (current) {
        StoreChain *next = current->next;
        
        if (!sorted || sorted->capital <= current->capital) {
            current->next = sorted;
            sorted = current;
        } else {
            StoreChain *temp = sorted;
            while (temp->next && temp->next->capital > current->capital) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    *head = sorted;
}

// Удаление по номеру магазина и названию
void delete_store(StoreChain **head, int number, const char *name) {
    StoreChain *current = *head, *prev = NULL;
    while (current) {
        if (current->number == number && strcmp(current->name, name) == 0) {
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
void edit_store_by_index(StoreChain *head, int index) {
    StoreChain *current = head;
    int current_index = 0;

    while (current && current_index < index) {
        current = current->next;
        current_index++;
    }

    if (!current) {
        printf("Invalid index.\n");
        return;
    }

    printf("Enter new address: ");
    scanf(" %[^\n]", current->address);
    printf("Enter new phone: ");
    scanf(" %s", current->phone);
    printf("Enter new owner name: ");
    scanf(" %[^\n]", current->owner_name);
    printf("Enter new owner address: ");
    scanf(" %[^\n]", current->owner_address);
    printf("Enter new capital: ");
    scanf("%lf", &current->capital);
    printf("Record updated successfully.\n");
}

// Меню
void display_menu() {
    printf("\nMenu:\n");
    printf("1. Load store data from file\n");
    printf("2. Display store list\n");
    printf("3. Add new store\n");
    printf("4. Edit store by index\n");
    printf("5. Delete store\n");
    printf("6. Sort by owner name length (ascending)\n");
    printf("7. Sort by owner capital (descending)\n");
    printf("8. Save store data to file\n");
    printf("9. Exit\n");
}

// Добавление через ввод
void add_store_interactive(StoreChain **head) {
    int number;
    char name[50], address[100], phone[20], owner_name[100], owner_address[100];
    double capital;

    printf("Enter store number: ");
    scanf("%d", &number);
    printf("Enter store name: ");
    scanf(" %[^\n]", name);
    printf("Enter store address: ");
    scanf(" %[^\n]", address);
    printf("Enter store phone: ");
    scanf(" %s", phone);
    printf("Enter owner name: ");
    scanf(" %[^\n]", owner_name);
    printf("Enter owner address: ");
    scanf(" %[^\n]", owner_address);
    printf("Enter owner capital: ");
    scanf("%lf", &capital);

    add_store(head, create_store(number, name, address, phone, owner_name, owner_address, capital));
}

// Главная функция
int main() {
    StoreChain *store_list = NULL;
    int choice;
    char filename[100];
    char name[50];
    int number;
    int index;

    while (1) {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter filename to load: ");
                scanf("%s", filename);
                store_list = load_from_file(filename);
                if (!store_list) {
                    printf("Error loading data.\n");
                }
                break;

            case 2:
                printf("Store List:\n");
                print_stores(store_list);
                break;

            case 3:
                add_store_interactive(&store_list);
                break;

            case 4:
                printf("Enter index of the store to edit: ");
                scanf("%d", &index);
                edit_store_by_index(store_list, index);
                break;

            case 5:
                printf("Enter number of the store to delete: ");
                scanf("%d", &number);
                printf("Enter name of the store to delete: ");
                scanf(" %[^\n]", name);
                delete_store(&store_list, number, name);
                break;

            case 6:
                sort_by_owner_name_length(&store_list);
                printf("List sorted by owner name length (ascending).\n");
                break;

            case 7:
                sort_by_capital(&store_list);
                printf("List sorted by owner capital (descending).\n");
                break;

            case 8:
                printf("Enter filename to save: ");
                scanf("%s", filename);
                save_to_file(store_list, filename);
                printf("Data saved to %s.\n", filename);
                break;

            case 9:
                free_list(store_list);
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
